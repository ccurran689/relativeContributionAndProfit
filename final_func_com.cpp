//this program calculates the relative contribution and profit for each user

//library declarations
#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>

using namespace std;

//function declaration 
void total_investment(double& number_of_rows_var, vector <string>& file_data_vector);
void relative_contribution(double& number_of_rows_var, vector <string>& file_data_vector, double& total_investment_var, int& current_years_investment_col);
void write_file( vector <double> &pRelative_c, vector <double> &pProfit_array, vector <string> &file_data_vector);
void profit(double& number_of_rows_var, vector <double> &pRelative_c, double& total_investment_var, vector <string> &file_data_vector, int& current_years_investment_col);
void instructions();

int main() {

	//initilze variables
	string line = "";
	string file_name = "";
	double number_of_rows_var = 0;
	int cell_index = 0;
	vector <string> file_data_vector;


	try {

		instructions();
		cin >> file_name; 
		file_name.append(".csv"); //appends .csv to user input 

		ifstream user_file(file_name);  //opens user file 

		clock_t start_clock;
		start_clock = clock(); //starts clock for timing program execution time

		if (user_file.fail()) {
			throw runtime_error("No file found, remember not to type .csv");  //throw custom exception class as throwing a string can cause an error of it's own
		}

		while (!user_file.eof()) {  //repeats until end of file

			cell_index++; //counter used to increment through the csv cells

			if (cell_index == 4) {  //calls get rest of line at 4th column due to lack of ','
				getline(user_file, line);

				if (line.find(",") != string::npos)  //checks for ',' as this indicates too many columns or incorrect format
				{
					throw runtime_error("Format error, please refer to instructions for expected format");
				}
				cell_index = 0;   //resets counter
				number_of_rows_var++;  //counts number of rest of lines called e.g rows in file
			}

			else {
				getline(user_file, line, ',');  //reads all the entries from the file that are seperated by a comma

			}
			file_data_vector.push_back(line); //adds the file data to the vector
		}

		user_file.close(); // Closes the file after finishing

		file_data_vector.pop_back(); //Removes garbage value
		

		total_investment(number_of_rows_var, file_data_vector);
		printf("\nTime taken: %.5fs\n", (float)(clock() - start_clock) / CLOCKS_PER_SEC);  //prints time taken for program to execute to the user
	}
	catch (exception const& e) {
		cout << "\nError:" << " " << e.what() << "\n";   //prints thrown errors to the user
	}
	system("pause");   //pauses code so the user can read the time taken to execute
	return 0;
}
	

//prints instruction manual to user 
void instructions() {
	cout << "\n\nPlease type the name of the csv file you wish to have expected profit and relative contribution calculated for.\n";
	cout << "Example input : Investors_file  ( Do not include .csv)\n\n";

	cout << "-----------------------------------RULES-----------------------------------------------\n";
	cout << "1) Do not insert commas into csv file, commas should only be present between columns (this does not include the last variable of each row).\n";
	cout << "2)CSV file should only contain 4 columns as seen in the example below\n";
	cout << "3)CSV file columns should be ordered as seen in the example below\n\n\n";
	

	cout << "--------------------------CSV file should follow below format--------------------------\n\n";

	cout << "---------------------------------------------------------------------------------\n";
	cout << "|Investor's name || Bank account || Sort code || Current year's investment (£)	|\n";
	cout << "|----------------||--------------||-----------||--------------------------------|\n";
	cout << "|      Chris     ||   12345678   || 00-11-22  ||             100000             |\n";
	cout << "|-------------------------------------------------------------------------------|\n";
}

//calculates the total investment 
void total_investment(double& number_of_rows_var, vector <string>& file_data_vector) 
{
	int current_years_investment_col = 3;  //variable declaration 
	double total_investment_var = 0;
	double file_data_double = 0;

	for (int row_count_TI = 1; row_count_TI < number_of_rows_var; row_count_TI++) // total investment start 1 to avoid header title
	{
		current_years_investment_col = current_years_investment_col + 4; //increments through "current years investment" column

		try {
			file_data_double = stod(file_data_vector[current_years_investment_col]);   //converts current years investment string to a double value
		}
		catch (exception)
		{
			throw runtime_error("Non-int value found inside current years investment column");
		}

		total_investment_var = total_investment_var + file_data_double;  //sums all the investments
	}
	relative_contribution(number_of_rows_var, file_data_vector, total_investment_var, current_years_investment_col);
}

//calculates relative contribution
void relative_contribution(double& number_of_rows_var, vector <string>& file_data_vector,double& total_investment_var, int& current_years_investment_col)
{
	double pRelative_c_val = 0;
	vector <double> pRelative_c;
	current_years_investment_col = 3;   //resets "current year's investment" column to the first row
	double current_yield_var = 0;

		for (int row_count_RC = 1; row_count_RC < number_of_rows_var; row_count_RC++) // relative contribution start 1 to avoid header title
		{
			current_years_investment_col = current_years_investment_col + 4; 
			
			current_yield_var = stod(file_data_vector[current_years_investment_col]);
			pRelative_c_val = (total_investment == 0) ? 0 : pRelative_c_val = ((current_yield_var / total_investment_var) * 100); //relative contribution calculation
			pRelative_c.push_back(pRelative_c_val);  //appends to end of vector
		}
		profit(number_of_rows_var, pRelative_c, total_investment_var, file_data_vector, current_years_investment_col);
	}


void profit(double& number_of_rows_var, vector <double> &pRelative_c, double& total_investment_var, vector <string> &file_data_vector, int& current_years_investment_col)
{
	vector <double> pProfit_array;
	double negative_check = 0;
	current_years_investment_col = 3;   //resets "current year's investment" column to the first row

	for (int row_count_profit = 0; row_count_profit < number_of_rows_var-1; row_count_profit++) // number of rows -1 due to 0 starting point to fetch first vector value 
	{
		if (total_investment_var < 0) { pProfit_array.push_back(0); }  //sets all profit values to 0 if total investment is negative

		else {
			current_years_investment_col = current_years_investment_col + 4;
			negative_check = stod(file_data_vector[current_years_investment_col]);  //runs through all the values and checks for negative investment

			if (negative_check < 0) { pProfit_array.push_back(0), row_count_profit++; } //applies 0 to negative investment and increments

			if (row_count_profit < number_of_rows_var)//performs profit caluclation 
			{
				double pProfit_array_val = (0.2 * pRelative_c[row_count_profit] * total_investment_var);  //profit calculation
				pProfit_array.push_back(pProfit_array_val);  
			}
		}

	}
	
	write_file( pRelative_c, pProfit_array, file_data_vector);
}


void write_file(vector <double> &pRelative_c, vector <double> &pProfit_array, vector <string> &file_data_vector) {

	ofstream newfile("Investors_file_new.csv");  //opens a new file for writing
	int col_count=0;
	int first_col_count=0;
	int array_count =0; 

	for (unsigned int vector_increment = 0; vector_increment < file_data_vector.size(); vector_increment++) {   //runs through each element of the vector 

		if (vector_increment < file_data_vector.size()) {  //writes vector values to the new file
			newfile << file_data_vector[vector_increment]<< ',';	
			col_count++; //increments the column counter with every write
		}
		
		
		if ( (col_count == 4) & (first_col_count == 0))  //appends new column titles after the 4th column 
		{	
		newfile << "relative contribution of the investor (%)" << ',';
		newfile << "expected profit (£)";
		newfile << endl;
		col_count=0;
		first_col_count=1;  //prevents this if statement from happening again
		}	
		
		if ( (col_count == 4) & (first_col_count == 1)) //will place caluclated values in the columns below the titles
		{
		newfile << pRelative_c[array_count] << ','; 
		newfile << pProfit_array[array_count];
		newfile << endl;
		col_count=0;  //resets the column count
		array_count++; //increments through the profit and relative contribution vectors
		}

	}	
	newfile.close();  //closes file before ending

}