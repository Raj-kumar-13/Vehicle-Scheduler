#include <iostream>
#include <string>
#include <iomanip>  //for set() and setfill()
using namespace std;

#include "Commuter.h"

Commuter::Commuter()
	:Logistic() {}

Commuter::Commuter(char type, int num, string destination, int hour, int min)
	: Logistic(type, num, destination, hour, min) {}

void Commuter::displayCommuter() {
	cout << left << Logistic::getType() << setw(15) << Logistic::getNum();
	cout << left << setw(15) << Logistic::getDestination();
	cout << right << setw(2) << setfill('0') << Logistic::getHour() << ':'; //2 spaces + 1 space for colon
	cout << right << setw(2) << setfill('0') << Logistic::getMin();		 //2 spaces
	cout << left << setw(10) << setfill(' ') << ' ' << endl;		 //arrange 10 space more
}