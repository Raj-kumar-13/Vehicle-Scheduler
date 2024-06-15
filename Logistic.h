#pragma once

#include <string>
class Commuter;
class Vehicle;

using namespace std;

class Logistic
{
	char type;
	int num;
	string destination;
	int hour;
	int min;
public:
	Logistic(char type = '?', int num = 0, string destination = "No_location", int hour = 0, int min = 0);
	bool checkTime(int oHr, int oMin);
	void setType(char type);
	char getType();
	void setNum(int num);
	int getNum();
	void setDestination(string destination);
	string getDestination();
	void setHour(int hour);
	int getHour();
	void setMin(int min);
	int getMin();
	int getTime();
	void displayLogistic();
};
