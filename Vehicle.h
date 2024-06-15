#pragma once

#include "Logistic.h"
class Commuter;

class Vehicle : public Logistic
{
	int capacity;
	static const int V_SMALL = 2;
	static const int V_LARGE = 4;
	static const int V_SUPER = 6;
public:
	Vehicle();								//default constructor
	Vehicle(char type, int num, string destination, int hour, int min, int capacity = V_SMALL); //non-default constructor
	void setCapacity(int capacity);
	int getCapacity();
	void displayVehicle();
	void decCapacity();
};
