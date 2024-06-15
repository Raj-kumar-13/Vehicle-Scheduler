#include <iostream>
#include <string>
#include <vector>
#include <iomanip>			//For set() and setfill()
#include <fstream>			//For file access
#include <sstream>			//For stringstream
#include <thread>			//For thread control - sleep
#include <algorithm>

using namespace std;

#include "Vehicle.h"
#include "Commuter.h"
#include "Schedule.h"

/****************** Enumeration ***********************************/

enum {								//Enumerate through options
	M_MAIN,
	M_DISP_VEHICLES,
	M_DISP_COMMUTERS,
	M_SCH_ARRIVAL_TIME,
	M_SCH_MIN_VEHICLE,
	M_DISP_VEHICLE_NOT_FULL,
	M_DISP_REMAINING_COMMUTERS,
	M_DISP_SCH_PLAN,
	M_SAVE_SCH_PLAN,
	M_MODIFY_VEHICLES,
	M_MODIFY_COMMUTERS,
	M_LOAD_DATA,
	M_SEARCH_SCHEDULE,
	
	M_MAX,
	M_EXIT = -1
}Menu;

/****************** Function Prototypes ***************************/
void m_welcome();
int m_main();
int m_disp_vehicle(vector <Vehicle>& vList);
int m_disp_commuter(vector <Commuter>& cList);
int m_load_data(vector <Commuter>& cList, vector <Vehicle>& vList);
int m_modify_vehicle(vector <Vehicle>& vList,vector <string> dest);
int m_modify_commuter(vector <Commuter>& cList,vector <string> dest);
int m_schByArrTime(vector <Schedule>& sList, vector <Vehicle>& vList, vector <Commuter>& cList);
int m_schByMinVeh(vector <Schedule>& sList, vector <Vehicle>& vList, vector <Commuter>& cList);
int m_disp_sch_plan(vector <Schedule>& sList);
int m_disp_vehicle_not_full(vector<Vehicle>& vehUnderCap);
int m_disp_remaining_commuters(vector<Commuter>& comLeft);
int m_save_sch_plan(vector <Schedule>& sList);
int m_search_schedule(vector <Schedule>& sList);
void o_create_vehicle(string data, vector <Vehicle>& vList);
void o_create_commuter(string data, vector <Commuter>& cList);
bool compVehDestDescendCap(Vehicle v1, Vehicle v2);
bool compComDestTime(Commuter c1, Commuter c2);
bool compVehDestTime(Vehicle v1, Vehicle v2);
bool compComDestDescendTime(Commuter c1, Commuter c2);
bool checkDestination(string dest, vector <string> dList);
bool checkTime(string t);
bool checkCapacity(int c);

/****************** Main ******************************************/
int main() {
	bool exit_menu = false;
	int choice = M_MAIN;
	vector <Commuter> cList;
	vector <Vehicle> vList;
	vector <Schedule> sList;
	vector <Vehicle> vehUnderCap;
	vector <Commuter> comLeft;
	const vector<string> destination = { "Aljunied","Bedok","Bishan","Changi","ChoaChuKang","Holland","Jurong","MarinaParade","Punggol","ToaPayoh","Tampines","Yishun","WestCoast"};//Destination list that can be used by functions

	m_welcome();

	while (!exit_menu)
	{
		switch (choice) {
		case M_MAIN:
			choice = m_main();				//Go to main choice
			break;
		case M_DISP_VEHICLES:
			choice = m_disp_vehicle(vList);		//Go to display vehicle choice
			break;
		case M_DISP_COMMUTERS:
			choice = m_disp_commuter(cList);	//Go to display commuters
			break;
		case M_SCH_ARRIVAL_TIME:
			vehUnderCap = vList;
			comLeft = cList;
			sList.clear();
			choice = m_schByArrTime(sList, vehUnderCap, comLeft);
			break;
		case M_SCH_MIN_VEHICLE:
			vehUnderCap = vList;			//clone a copy of vehicle vector list
			comLeft = cList;				//clone a copy of commuter vector list
			sList.clear();				//empty previous schedule plan
			choice = m_schByMinVeh(sList, vehUnderCap, comLeft);		//start schedule action
			break;
		case M_DISP_VEHICLE_NOT_FULL:
			choice = m_disp_vehicle_not_full(vehUnderCap);				//display vehicles that are not full
			break;
		case M_DISP_REMAINING_COMMUTERS:
			choice = m_disp_remaining_commuters(comLeft);
			break;
		case M_DISP_SCH_PLAN:
			choice = m_disp_sch_plan(sList);
			break;
		case M_SAVE_SCH_PLAN:
			choice = m_save_sch_plan(sList);
			break;
		case M_MODIFY_VEHICLES:
			choice = m_modify_vehicle(vList,destination);
			break;
		case M_MODIFY_COMMUTERS:
			choice = m_modify_commuter(cList,destination);
			break;
		case M_LOAD_DATA:
			choice = m_load_data(cList, vList);
			break;
		case M_SEARCH_SCHEDULE:
			choice = m_search_schedule(sList);
			break;
		case M_EXIT:			//exit path
		default:
			exit_menu = true;
			break;
		}
	}

	return 0;
}

//**********************************************************//
// Welcome
//**********************************************************//
void m_welcome()
{
	system("CLS");
	cout << "*****************************************************************" << endl;
	cout << "*****************************************************************" << endl;
	cout << "**                                                             **" << endl;
	cout << "**             \033[34mWelcome to transportation hub\033[0m		       **" << endl; //blue lol
	cout << "**                                                             **" << endl;
	cout << "**                                                             **" << endl;
	cout << "*****************************************************************" << endl;
	cout << "*****************************************************************" << endl;
	this_thread::sleep_for(chrono::seconds(1));		//Add delay to show on screen,uncomment this before finalising
}

//**********************************************************//
// Main Menu
//**********************************************************//
int m_main() {
	int choice = M_MAIN;
	const string ERROR_MSG_CHOICE_IS_NOT_INT = "The choice entered is not int!";

	system("CLS");
	cout << "MAIN MENU:" << endl;
	cout << "1. Display all vehicles" << endl;
	cout << "2. Display all commuters" << endl;
	cout << "3. Schedule commuter by arrival time" << endl;
	cout << "4. Schedule commuter by minimum vehicle" << endl;
	cout << "5. Display vehicle that is under capacity" << endl;
	cout << "6. Display remaining commuters" << endl;
	cout << "7. Display scheduling plan" << endl;
	cout << "8. Save scheduling plan" << endl;
	cout << "9. Modify vehicle list" << endl;
	cout << "10. Modify commuter list" << endl;
	cout << "11. Load data file" << endl;
	cout << "12.Search Schedule" << endl << endl;
	cout << "Key -1 To End Program" << endl << endl;
	cout << "Please enter your option:\n>";

	try {//Author: Gary Tey
		int check = cin.peek();	//Fixed menu bug where entering anything other than number will cause infinite loop
		if (isdigit(check)||check=='-' )
		{
			cin >> choice;
		}
		else {
			throw(ERROR_MSG_CHOICE_IS_NOT_INT);
		}
	}
	catch (string msg) {
		choice = M_MAIN;
		cout << msg<<endl;
	}
	
										
	if ((choice > M_MAIN) && (choice < M_MAX)) {
		//do nothing, valid keep data
	}
	else if (choice == M_EXIT) {
		//do nothing, valid exit
	}
	else {
		cout << "Invalid options! Please try again!" << endl;
		choice = M_MAIN;
		this_thread::sleep_for(chrono::seconds(1));  // stay for 1 sec to show warning
	}

	cin.ignore();		//flush cin buffer
	return choice;
}

//**********************************************************//
// Menu Display Vehicle
//**********************************************************//
int m_disp_vehicle(vector <Vehicle>& vList) {				//Author: Jing Heng 	displays the vehicle data from data extracted 
	int choice = M_MAIN;

	system("CLS");
	cout << "Display All Available Vehicles:" << endl;
	cout << "-----------------------------------------------------------------------" << endl;

	/* Title */
	cout << left << setw(15) << "Name";
	cout << left << setw(15) << "Destination";
	cout << left << setw(15) << "Time";
	cout << left << setw(15) << "Capacity" << endl;

	for (auto& v:vList) {								//iterate through vList
		v.displayVehicle();									//call displayVehicle() for each vehicle in vList
		cout << endl;
	}

	/* Ask User to return to Main Menu */
	cout << endl;
	cout << "Please enter any number key to return to main menu:\n>";

	if (cin.peek() != '\n') {
		cin >> choice;
	}
	cin.ignore();		//flush cin buffer
	choice = M_MAIN;	//overwrite choice

	return choice;
}

int m_disp_commuter(vector <Commuter>& cList) {	//Author: Jing Heng 		displays the commuter data from data extracted 
	int choice = M_MAIN;

	system("CLS");
	cout << "Display All Commuters:" << endl;
	cout << "-----------------------------------------------------------------------" << endl;

	/* Title */
	cout << left << setw(15) << "Name";
	cout << left << setw(15) << "Destination";
	cout << left << setw(15) << "Time" << endl;

	for (auto& c:cList) {								//iterate through cList
		c.displayCommuter();									//call displayCommuter() for each commuter in cList
	}

	/* Ask User to return to Main Menu */
	cout << endl;
	cout << "Please enter any number key to return to main menu:\n>";

	if (cin.peek() != '\n') {
		cin >> choice;
	}
	cin.ignore();		//flush cin buffer
	choice = M_MAIN;	//overwrite choice

	return choice;
}

int m_schByArrTime(vector <Schedule>& sList, vector <Vehicle>& vList, vector <Commuter>& cList) {		//Author: Gary Tey schedules vehicle by arrival time 
	system("CLS");
	cout << "Display Schedule By Arrival Time:" << endl;
	cout << "-----------------------------------------------------------------------" << endl;

	int choice = M_MAIN;
	int sid = 1;
	size_t i = 0, j = 0;
	sort(vList.begin(), vList.end(), compVehDestTime);							// sort Vehicle, same destination, Descending time
	sort(cList.begin(), cList.end(), compComDestDescendTime);						// sort Commuter, same destination, Descending time

	while (i < vList.size()) {										// Loop vehicle list
		j = 0;												// reset back to beginning of commuter list
		while (j < cList.size()) {									// Loop commuter list
			int c_hr = cList[j].getHour();
			int c_min = cList[j].getMin();
			if ((vList[i].getDestination() == cList[j].getDestination()) &&				// Destination matched,
				(vList[i].checkTime(c_hr, c_min) == true) &&					// Within Grace period,
				(vList[i].getCapacity() > 0)) 							// Within vehicle Capacity
			{
				Schedule tmpSch(sid, vList[i], cList[j]);					// store tmp object data, call Schedule constructor
				sList.push_back(tmpSch);							// save into schedule list
				sid++;
				vList[i].decCapacity();								// decrement capacity
				cList.erase(cList.begin() + j);							// erase commuter element by index
				// cannot increase j counter,
				// stay in the same position to do next check
			}
			else
				j++;										// go for next commuter element
		}

		if (vList[i].getCapacity() == 0) {								// check capacity reached zero
			cout << "Vehicle: " << vList[i].getType() << vList[i].getNum() << " Capacity Full!" << endl;;
			vList.erase(vList.begin() + i);								// erase vehicle element by index
			// cannot increase i counter,
			// stay in the same position to do next check
		}
		else
			i++;											// go for the next freight element
	}

	/* Display Schedule Plan */
	cout << endl;
	cout << "-----------------------------------------------------------------------" << endl;
	cout << left << setw(15) << "ID";
	cout << left << setw(15) << "Vehicle";
	cout << left << setw(15) << "Destination";
	cout << left << setw(15) << "Time";
	cout << left << setw(15) << "Capacity";
	cout << left << setw(15) << "Commuter";
	cout << left << setw(15) << "Destination";
	cout << left << setw(15) << "Time" << endl;

	for (auto& v : sList) {
		v.displaySchedule();
	}

	/* Ask User to return to Main Menu */
	cout << endl;
	cout << "Please enter any number key to return to main menu:\n>";

	if (cin.peek() != '\n') {
		cin >> choice;
	}
	cin.ignore();		//flush cin buffer
	choice = M_MAIN;	//overwrite choice

	return choice;
}

int m_schByMinVeh(vector <Schedule>& sList, vector <Vehicle>& vList, vector <Commuter>& cList) {// schedules by minimizing the usage of vehicles 
	system("CLS");
	cout << "Display Schedule By Minimum Vehicle:" << endl;
	cout << "-----------------------------------------------------------------------" << endl;

	int choice = M_MAIN;
	int sid = 1;
	size_t i = 0, j = 0;

	sort(vList.begin(), vList.end(), compVehDestDescendCap);						// sort Vehicle, same destination, Descending capacity
	sort(cList.begin(), cList.end(), compComDestTime);							// sort Commuter, same destination, Ascending time

	while (i < vList.size()) {										// Loop vehicle list
		j = 0;												// reset back to beginning of commuter list
		while (j < cList.size()) {									// Loop commuter list
			int c_hr = cList[j].getHour();
			int c_min = cList[j].getMin();
			if ((vList[i].getDestination() == cList[j].getDestination()) &&				// Destination matched,
				(vList[i].checkTime(c_hr, c_min) == true) &&					// Within Grace period,
				(vList[i].getCapacity() > 0)) 							// Within vehicle Capacity
			{
				Schedule tmpSch(sid, vList[i], cList[j]);					// store tmp object data, call Schedule constructor
				sList.push_back(tmpSch);							// save into schedule list
				sid++;
				vList[i].decCapacity();								// decrement capacity
				cList.erase(cList.begin() + j);							// erase commuter element by index
				// cannot increase j counter,
				// stay in the same position to do next check
			}
			else
				j++;										// go for next commuter element
		}

		if (vList[i].getCapacity() == 0) {								// check capacity reached zero
			cout << "Vehicle: " << vList[i].getType() << vList[i].getNum() << " Capacity Full!" << endl;;
			vList.erase(vList.begin() + i);								// erase vehicle element by index
			// cannot increase i counter,
			// stay in the same position to do next check
		}
		else
			i++;											// go for the next freight element
	}

	/* Display Schedule Plan */
	cout << endl;
	cout << "-----------------------------------------------------------------------" << endl;
	cout << left << setw(15) << "ID";
	cout << left << setw(15) << "Vehicle";
	cout << left << setw(15) << "Destination";
	cout << left << setw(15) << "Time";
	cout << left << setw(15) << "Capacity";
	cout << left << setw(15) << "Commuter";
	cout << left << setw(15) << "Destination";
	cout << left << setw(15) << "Time" << endl;

	for (auto& v : sList) {
		v.displaySchedule();
	}

	/* Ask User to return to Main Menu */
	cout << endl;
	cout << "Please enter any number key to return to main menu:\n>";

	if (cin.peek() != '\n') {
		cin >> choice;
	}
	cin.ignore();		//flush cin buffer
	choice = M_MAIN;	//overwrite choice

	return choice;
}

int m_disp_sch_plan(vector <Schedule>& sList) {										// displays the schedule plan to the user
	int choice = M_MAIN;
	system("CLS");
	cout << "Display Schedule:" << endl;
	cout << "-----------------------------------------------------------------------" << endl;
	cout << endl;
	cout << "-----------------------------------------------------------------------" << endl;
	cout << left << setw(15) << "ID";
	cout << left << setw(15) << "Vehicle";
	cout << left << setw(15) << "Destination";
	cout << left << setw(15) << "Time";
	cout << left << setw(15) << "Capacity";
	cout << left << setw(15) << "Commuter";
	cout << left << setw(15) << "Destination";
	cout << left << setw(15) << "Time" << endl;

	for (auto& v : sList) {		//iterate schedule list
		v.displaySchedule();	//display schedule list to console
	}

	/* Ask User to return to Main Menu */
	cout << endl;
	cout << "Please enter any number key to return to main menu:\n>";

	if (cin.peek() != '\n') {
		cin >> choice;
	}
	cin.ignore();		//flush cin buffer
	choice = M_MAIN;	//overwrite choice

	return choice;
}

int m_save_sch_plan(vector <Schedule>& sList) { //author: Gary Tey							// saves the schedule plan to hard disk
	int choice = M_MAIN;
	system("CLS");

	ofstream output_file;												//declare file variable
	output_file.open("schedule.txt");										//open file, if file does not exist, create file
	if (output_file.is_open()) {											//verify file is opened
		output_file << "Schedule :" << endl;									//UI display
		output_file << "-----------------------------------------------------------------------" << endl;
		output_file << endl;
		output_file << "-----------------------------------------------------------------------" << endl;
		output_file << left << setw(15) << "ID";
		output_file << left << setw(15) << "Vehicle";
		output_file << left << setw(15) << "Destination";
		output_file << left << setw(15) << "Time";
		output_file << left << setw(15) << "Capacity";
		output_file << left << setw(15) << "Commuter";
		output_file << left << setw(15) << "Destination";
		output_file << left << setw(15) << "Time" << endl;
		for (int i = 0; i < sList.size(); i++) {								//iterate through schedule list
			output_file << sList[i] << endl;								//overload schedule << operator to output to file
		}
	}
	output_file.close();												//closes file
	cout << endl;
	cout << "Your Schedule has been saved!\n";
	cout << "Please enter any number key to return to main menu:\n";

	while (cin.peek() != '\n') {
		cin >> choice;
	}
	cin.ignore();		//flush cin buffer
	choice = M_MAIN;	//overwrite choice

	return choice;
}

int m_load_data(vector <Commuter>& cList, vector<Vehicle>& vList) { //author: Gary Tey						// extracts the data from user provided files
	int choice = M_LOAD_DATA, index = 0, menu = 0;
	system("CLS");
	string filename, data;
	while (menu != -1) {
		cout << "Enter File Name (with .txt) : " << endl;
		cin >> filename;

		//clear previous list
		if (filename == "Vehicle.txt") { vList.clear(); }
		else if (filename == "Commuter.txt") { cList.clear(); }

		ifstream input_file;
		input_file.open(filename, ios::in);
		if (input_file.is_open())
		{
			int index = 0;
			while (getline(input_file, data))
			{	// check first character of input stream. If start with V, create vehicle, if start with C, create commuter
				if (data.at(0) == 'V') {			
					o_create_vehicle(data, vList);
				}
				else if (data.at(0) == 'C') {
					o_create_commuter(data, cList);
				}
				else {
					//Informs user if data contains invalid detail
					cout << "Information at line,"<<index<<" is not properly structured"<<endl;
				}
				index++;
			}
		}
		else {
			cout << "File could not be opened" << endl;
		}
		input_file.close();												//closes file
		cout << "To load another data file, key in any number. To exit to menu, key in -1: " << endl;
		cin >> menu;
	}

	//cout << endl;
	//cout << "Please enter any number key to return to main menu:\n>";

	if (cin.peek() != '\n') {
		cin >> choice;
	}
	cin.ignore();		//flush cin buffer
	choice = M_MAIN;	//overwrite choice

	return choice;
}

int m_modify_vehicle(vector<Vehicle>& vList,vector <string> dest)					//Author:Rajkumar
{																//adding new vehicle, delete existing vehicle and edit existing vehicle
	int choice = M_MAIN;
	int option = 0;
	string Destination, time;
	int capacity, Vno, Hr, Min = 0;
	bool check = true;
	system("CLS");

	cout << "MENU:" << endl;
	cout << "1. Add a new Vehicle" << endl;
	cout << "2. Delete an existing vehicle" << endl;
	cout << "3. Edit existing Vehicle" << endl;
	cout << "Press any other key to return to Main Menu" << endl << endl;
	cout << "\nPlease enter your option:";
	cin >> option;

	switch (option) {										//switch case for accessing different editing options
	case 1:
	{
		int flag = 1;
		cout << "Enter Vehicle Number:";
		cin >> Vno;
		//for (int i = 0; i < cList.size(); i++)							//limited no of reattempts due to size of the list
		//{									//linear search for ID
		//	while (Cno == cList[i].getNum())
		//	{
		//		cout << "Number already exist,Please enter a valid number:";
		//		cin >> Cno;
		//		i=0;
		//	}
		//}
		while (flag == 1)									//loops until entered number is not found in the existing list
		{
			for (int i = 0; i < vList.size(); i++)
			{
				if (Vno == vList[i].getNum())
				{
					flag = 1;
					break;										//exits for loop
				}

				else
				{
					flag = 0;
				}

			}
			if (flag == 1)
			{
				cout << "Number already exist,Please enter a valid number:";
				cin >> Vno;
			}
			else if (flag == 0)
			{
				break;
			}
		}
		cout << "Enter the vehicle information:" << endl << "Destinations:" << endl;
		for (auto& d1 : dest) { cout << d1 << endl; }					//displays the destination list

		do {
			cout << "Enter Destination : ";
			cin >> Destination;
			if (checkDestination(Destination,dest)) { check = false; break; }				//checks entered information against destination list
			else{ cout << "The Destination you entered is not on the list!" << endl; }
		} while (check);
		check = true;																	//reset and reuse check
		do {
			cout << "Enter time in HHMM form:";
			cin >> time;
			if (checkTime(time)) { check = false; }
			else { cout << "The time you entered is in the wrong format!" << endl; }
		} while (check);
		check = true;
		do {
			cout << "Enter capacity(2 or 4 or 6):";
			cin >> capacity;
			if (checkCapacity(capacity)) { check = false; }
			else { cout << "The capacity you entered is not valid!" << endl; }
		} while (check);
		check = true;
		Hr = stoi(time.substr(0, 2));
		Min = stoi(time.substr(2, time.size()));
		Vehicle vehicle('V', Vno, Destination, Hr, Min, capacity);						//create an object of class vehicle
		vList.push_back(vehicle);														//push the object into a vector
		break;
	}

	
	case 2:
	{
		int ID, pos = 0;
		cout << "Enter the vehicle number to delete:";
		cin >> ID;
		for (int i = 0; i < vList.size(); i++) {									//linear search for ID
			if (ID == vList[i].getNum())
			{
				vList.erase(vList.begin() + i);
				cout << "The vechicle num V" << ID << " was deleted";
			}
			else
			{
				cout << "Item not found and unable to delete";
			}
		}
		break;
	}

	case 3:
	{
		int ID, id = 0;
		bool flag = true;
		
		while (flag)												//loops until the number entered is found in the existing list
		{
			cout << "Enter the vehicle number to edit,-1 to exit:";
			cin >> ID;
			if (ID == -1) { break; }								//allows user to exit
			for (auto& v1:vList)
			{
				if (ID == v1.getNum())
				{
					flag = false;
					id = ID-1;		//offset id for index position
					cout << "*****************************************************************" << endl;
					v1.displayVehicle();																//prints out the information about the vehicle if found
					cout << "\n*****************************************************************" << endl;
					break;
				}

			}
			if(flag){cout << "Vehicle does not exist"<<endl;}
		}
		cout << "-----------------------------------------------------------------------------------\n";
		int option = 0;
		bool exit = false;
		while (option != -1)
		{
			cout << "\nEdit Vehicle ITEMS:" << endl;
			cout << "1. Vehicle Name" << endl;
			cout << "2. Capacity" << endl;
			cout << "3. Destination" << endl;
			cout << "4. Recharging Time" << endl;
			cout << "Press -1 to return to Main Menu" << endl << endl;
			cout << "\nPlease enter your option:";
			cin >> option;
			switch (option)																		//switch case for editing each variables of vehicle
			{
			case 1:
			{
				int num = 0;				
					do{																			//do while loop verifies that input does not exist on the list of vehicle									
						check = false;
						cout << "\nEnter the Number to be changed to:";
						cin >> num;
						for (auto& v : vList) {
							if (num == v.getNum()) {
								check = true;
								cout << "Vehicle Number already exist!" << endl;
							}
						}
						if (check) { exit = false; }
						else if (!check) { exit = true; };

					} while (!exit);										
				vList[id].setNum(num);															//calls set function
				cout << "\nVehicle Number has been changed to:" << num;
				check = true;	//reset and reuse check
				break;
			}
			case 2:
			{		
				do {
					cout << "\nEnter the capacity(2,4 or 6) you wish to change to:";
					cin >> capacity;
				} while (!checkCapacity(capacity));												//verify capacity input is correct
				vList[id].setCapacity(capacity);												//calls set function
				cout << "\nVehicle Capacity has been changed to:" << capacity;
				break;
			}
			case 3:
			{
				cout << "Destination to choose from: " << endl;
				for (auto& ds : dest) { cout << ds << endl; }
				do {
					cout << "\nEnter the destination to be changed:";
					cin >> Destination;
				} while (!checkDestination(Destination, dest));									//verify destination input is correct	
				vList[id].setDestination(Destination);											//may call a destination menu function (possible improvement)
				cout << "\nVehicle Destination has been changed to:" << Destination;
				break;
			}
			case 4:
			{
				do {
					cout << "\nEnter Recharge time to be changed(in HHMM format):";
					cin >> time;
				} while (!checkTime(time));														//verify time input is correct
				Hr = stoi(time.substr(0, 2));
				Min = stoi(time.substr(2, time.size()));
				vList[id].setHour(Hr);															//calls set function
				vList[id].setMin(Min);															//calls set function
				cout << "\nRecharge time of the vehicle has been changed to:" << time;
				break;
			}
			default:
				option = -1;
			}
		}
		break;
	}
	default:
		return choice;
	}
	cout << "Please enter any other number key to return to main menu:\n";

	while (cin.peek() != '\n') {
		cin >> choice;
	}
	cin.ignore();		//flush cin buffer
	choice = M_MAIN;	//overwrite choice

	return choice;
}

int m_modify_commuter(vector<Commuter>& cList,vector <string> dest)			//Author:Rajkumar		//adding new commuter, delete existing commuter and edit existing commuter
{
	int choice = M_MAIN;
	int option = 0;
	string Destination, time;
	int Cno, Hr, Min = 0;
	bool check = true;
	system("CLS");

	cout << "MENU:" << endl;
	cout << "1. Add a new Commuter" << endl;
	cout << "2. Delete an existing Commuter" << endl;
	cout << "3. Edit existing Commuter" << endl;
	cout << "Press any other key to return to Main Menu" << endl << endl;
	cout << "\nPlease enter your option:";
	cin >> option;

	switch (option) {												//switch case to edit the contents of the commuter
	case 1:
	{int flag = 1;
	cout << "Enter Commuter Number:";
	cin >> Cno;
	while (flag == 1)												//loops until the number entered by user is not found in the existing list
	{
		for (int i = 0; i < cList.size(); i++)
		{
			if (Cno == cList[i].getNum())
			{
				flag = 1;
				break;
			}

			else
				flag = 0;
		}
		if (flag == 1)
		{
			cout << "Number already exist,Please enter a valid number:";
			cin >> Cno;
		}
		else if (flag == 0)
		{
			break;
		}
	}

	cout << "Enter the Commuter information:" << endl << "Destinations:" << endl;
	for (auto& d1 : dest) { cout << d1 << endl; }
	do {
		cout << "Enter Destination : ";
		cin >> Destination;
		if (checkDestination(Destination, dest)) { check = false; break; }
		else { cout << "The Destination you entered is not on the list!" << endl; }
	} while (check);
	check = true;
	do {
		cout << "Enter arrival time in HHMM form:";
		cin >> time;
		if (checkTime(time)) { check = false; }
		else { cout << "The time you entered is in the wrong format!" << endl; }
	} while (check);
	check = true;
	Hr = stoi(time.substr(0, 2));
	Min = stoi(time.substr(2, time.size()));
	Commuter commuter('C', Cno, Destination, Hr, Min);						//create an object of class commuter
	cList.push_back(commuter);												//pushes the content into the object of class commuter
	break;
	}

	case 2:
	{
		int ID, pos = 0;
		cout << "Enter the commuter number to delete:";
		cin >> ID;
		for (int i = 0; i < cList.size(); i++) {									//linear search for ID
			if (ID == cList[i].getNum())
			{
				cList.erase(cList.begin() + i);
			}
			else
			{
				cout << "Item not found and unable to delete";
			}
		}
		break;
	}

	case 3:
	{
		int ID, id = 0;
		int flag = 1;

		cout << "Enter the Commuter number to edit:";
		cin >> ID;
		while (flag == 1)												//loops until the user entered value is found in the existing list
		{
			for (int i = 0; i < cList.size(); i++)
			{
				if (ID == cList[i].getNum())
				{
					flag = 0;
					id = i;
					break;
				}

				else
				{
					flag = 1;
				}

			}
			if (flag == 0)
			{
				cout << "*****************************************************************" << endl;
				cList[id].displayCommuter();																//displays the content in the commuter to show the user
				cout << "\n*****************************************************************" << endl;
			}
			else if (flag == 1)
			{
				cout << "Vehicle does not exist,Please enter a valid number:";
				cin >> ID;
			}
		}
		
		cout << "-----------------------------------------------------------------------------------\n";
		int option = 0;
		bool exit = false;
		while (option != -1)
		{
			cout << "\nEdit Commuter ITEMS:" << endl;
			cout << "1. Commuter Name" << endl;
			cout << "2. Destination" << endl;
			cout << "3. Arrival Time" << endl;
			cout << "Press -1 to return to Main Menu" << endl << endl;
			cout << "\nPlease enter your option:";
			cin >> option;
			switch (option)															//switch case for editing each variables of vehicle
			{
			case 1:
			{
				int num = 0;
				do {																			//do while loop verifies that input does not exist on the list of vehicle									
					check = false;
					cout << "\nEnter the Number to be changed to:";
					cin >> num;
					for (auto& v : cList) {
						if (num == v.getNum()) {
							check = true;
							cout << "Commuter Number already exist!" << endl;
						}
					}
					if (check) { exit = false; }
					else if (!check) { exit = true; };

				} while (!exit);
				cList[id].setNum(num);													//calls set function
				cout << "\nCommuter Number has been changed to:" << num;
				break;
			}

			case 2:
			{
				cout << "Destination to choose from: " << endl;
				for (auto& ds : dest) { cout << ds << endl; }
				do {
					cout << "\nEnter the destination to be changed:";
					cin >> Destination;
				} while (!checkDestination(Destination, dest));											//checks entered information against destination list
				cList[id].setDestination(Destination);													//may call and destination menu function (possible improvement)
				cout << "\nCommuter Destination has been changed to:" << Destination;
				break;
			}
			case 3:
			{
				do {
					cout << "\nEnter Recharge time to be changed(in HHMM format):";
					cin >> time;
				} while (!checkTime(time));
				Hr = stoi(time.substr(0, 2));
				Min = stoi(time.substr(2, time.size()));
				cList[id].setHour(Hr);													//calls set function
				cList[id].setMin(Min);													//calls set function
				cout << "\nArrival time of the commuter has been changed to:" << time;
				break;
			}
			default:
				option = -1;
			}
		}
		break;
	}
	default:
		return choice;
	}
	return choice;
}

int m_search_schedule(vector <Schedule>& sList)										//Author:Rajkumar
{																					//enables the user to search through the schedule list using the commuter ID
	system("CLS");
	int choice = M_MAIN;
	int id = 0;
	int search = 0;
	int vnum = 0;
	while (id != -1)
	{
		cout << "Enter the commuter id you want to know the allotted vehicle or -1 to exit:" << endl;
		cin >> id;
		for (auto& v : sList) {
			search = v.comparenum(id);												//calls compare function to check if the user entered value and content in the schedule list matches
			if (search != NULL)
			{
				system("CLS");
				vnum = search;
				cout << left << setw(15) << "ID";
				cout << left << setw(15) << "Vehicle";
				cout << left << setw(15) << "Destination";
				cout << left << setw(15) << "Time";
				cout << left << setw(15) << "Capacity";
				cout << left << setw(15) << "Commuter";
				cout << left << setw(15) << "Destination";
				cout << left << setw(15) << "Time" << endl;
				v.displaySchedule();
				break;
			}
			else{vnum = 0;}

		}
		if (vnum != 0){cout << "The vehicle num is:" << vnum << endl;}
		else {cout << "Entered commuter is not alloted or not found in the schedule" << endl;}
	}
	return choice;
}

void o_create_vehicle(string data, vector <Vehicle>& vList) {     //author: aiklong
	int vNum = stoi(data.substr(1, data.find(" ")));				//use empty space between first section of data and second section as reference point
	data = data.erase(0, data.find(" ") + 1);						//trim section of string that has been read
	int vCap = stoi(data.substr(0));								//use string to int function to cast data from string to int
	data = data.erase(0, data.find(" ") + 1);						//trim section of string that has been read
	string destination = data.substr(0, data.find(" "));			
	string time = data.substr(data.find(" ") + 1, data.size());
	int hr = stoi(time.substr(0, time.find(":")));
	int min = stoi(time.substr(time.find(":") + 1, time.size()));

	Vehicle vehicle('V', vNum, destination, hr, min, vCap);			//create vehicle object with data
	vList.push_back(vehicle);										//add vehicle object to vector list
}

void o_create_commuter(string data, vector <Commuter>& cList) {   //author: aiklong
	int cNum = stoi(data.substr(1, data.find(" ")));				//use empty spaces between first section of data and second section as reference point
	data = data.erase(0, data.find(" ") + 1);						//trim section of string that has been read
	string destination = data.substr(0, data.find(" "));			
	string time = data.substr(data.find(" ") + 1, data.size());
	int hr = stoi(time.substr(0, time.find(":")));					//use string to int function to cast data from string to int
	int min = stoi(time.substr(time.find(":") + 1, time.size()));

	Commuter commuter('C', cNum, destination, hr, min);				//create commuter object with id num
	cList.push_back(commuter);										//add commuter object to vector list
}

int m_disp_remaining_commuters(vector < Commuter>& comLeft)				//Author: Hazim Luo
{																		//enables user to view commuters left
	int choice = M_MAIN;

	system("CLS");

	cout << "Display Commuters Left:" << endl;
	cout << "-----------------------------------------------------------------------------------\n";

	cout << left << setw(15) << "CommuterNumber\t";
	cout << left << setw(15) << "Destination";
	cout << left << setw(15) << "Time" << endl;

	for (int i = 0; i < comLeft.size(); i++) {
		comLeft[i].displayCommuter();
		cout << endl;
	}

	cout << endl;
	cout << "Please enter any number key to return to main menu:\n";

	if (cin.peek() != '\n') {
		cin >> choice;
	}
	cin.ignore(); //flush cin buffer
	choice = M_MAIN; //overwrite choice

	return choice;
}

int m_disp_vehicle_not_full(vector <Vehicle>& vehUnderCap) {		//author Lim Jing Heng
																	//enables user to view vehicles under capacity
	int choice = M_MAIN;

	system("CLS");
	cout << "Displaying Vehicles under Capacity:" << endl;
	cout << "-----------------------------------------------------------------------" << endl;

	cout << left << setw(15) << "Name";
	cout << left << setw(15) << "Destination";
	cout << left << setw(15) << "Time";
	cout << left << setw(15) << "Capacity" << endl;

	for (int i = 0; i < vehUnderCap.size(); i++) {
		vehUnderCap[i].displayVehicle();
		cout << endl;
	}
	cout << endl;
	cout << "Please enter any number key to return to main menu:\n>";

	if (cin.peek() != '\n') {
		cin >> choice;
	}
	cin.ignore();  //flush cin buffer
	choice = M_MAIN; //overwrite choice

	return choice;
}

bool compVehDestDescendCap(Vehicle v1, Vehicle v2)
{
	if (v1.getDestination() != v2.getDestination())
		return (v1.getDestination() < v2.getDestination());
	return (v1.getCapacity() > v2.getCapacity());
}

bool compComDestTime(Commuter c1, Commuter c2) {
	if (c1.getDestination() != c2.getDestination())
		return (c1.getDestination() < c2.getDestination());
	return (c1.getTime() < c2.getTime());
}

bool compVehDestTime(Vehicle v1, Vehicle v2) {			//Author: Gary Tey
	if (v1.getDestination() != v2.getDestination())
		return (v1.getDestination() < v2.getDestination());
	return (v1.getTime() < v2.getTime());				//sort vehicle by ascending time
}

bool compComDestDescendTime(Commuter c1, Commuter c2) { //Author: Gary Tey
	if (c1.getDestination() != c2.getDestination())
		return (c1.getDestination() < c2.getDestination());
	return (c1.getTime() < c2.getTime());				//sort commuter by ascending time
}

//destination crosschecked with predefined destination list
bool checkDestination(string dest, vector <string> dList) { //author: Gary Tey
	for (auto& d : dList) {
		if (dest == d) { return true; }
	}
	return false;
}

//verify time function
bool checkTime(string t) { //author: Gary Tey
	if (stoi(t) >= 0 && stoi(t) <= 2359) { return true; } 
	return false;
}

//check for capacity
bool checkCapacity(int c) { //author: Gary Tey
	if (c <= 6 && (c % 2) == 0) { return true; }
	return false;
}
