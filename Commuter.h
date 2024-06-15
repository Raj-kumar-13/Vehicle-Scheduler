#pragma once
#include "Logistic.h"
class Vehicle;

class Commuter : public Logistic
{
public:
	Commuter();
	Commuter(char type, int num, string destination, int hour, int min);
	void displayCommuter();
};