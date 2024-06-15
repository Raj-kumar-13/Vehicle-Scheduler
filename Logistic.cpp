#include <iostream>
#include <string>
#include <iomanip>  //for set() and setfill()
using namespace std;

#include "Logistic.h"

class Vehicle;
class Commuter;

Logistic::Logistic(char type, int num, string destination, int hour, int min)
{
	this->type = type;
	this->num = num;
	this->destination = destination;
	this->hour = hour;
	this->min = min;
}
void Logistic::setType(char type) {
	this->type = type;
}
char Logistic::getType() {
	return type;
}
void Logistic::setNum(int num) {
	this->num = num;
}
int Logistic::getNum() {
	return num;
}
void Logistic::setDestination(string destination) {
	this->destination = destination;
}
string Logistic::getDestination() {
	return destination;
}
void Logistic::setHour(int hour) {
	this->hour = hour;
}
int Logistic::getHour() {
	return hour;
}
void Logistic::setMin(int min) {
	this->min = min;
}
int Logistic::getMin() {
	return min;
}
void Logistic::displayLogistic() {
	cout << left << type << setw(15) << num;
	cout << left << setw(15) << destination;
	cout << right << setw(2) << setfill('0') << hour << ':'; //2 spaces + 1 space for colon
	cout << right << setw(2) << setfill('0') << min;		 //2 spaces
	cout << left << setw(10) << setfill(' ') << ' ';		 //arrange 10 space more
}

bool Logistic::checkTime(int oHr, int oMin) {
	double obj = (double)(oHr)+(double)(oMin / 60.0);
	double rhs = (double)(hour)+(double)(min / 60.0);
	double diff = obj - rhs;				//Commuter time - Vehicle time, arrival earlier
	if ((diff >= 0) && (diff <= 30.0 / 60.0))
		return true;
	else
		return false;
}

int Logistic::getTime() {
	return (hour * 1000) + min;
}
