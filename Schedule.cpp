#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#include "Vehicle.h"
#include "Commuter.h"
#include "Schedule.h"

Schedule::Schedule(int schID, Vehicle vehicle, Commuter commuter)
	: vehicleObj(vehicle), commuterObj(commuter)
{
	this->schID = schID;
}

void Schedule::displaySchedule() {
	cout << left << setw(15) << schID;
	vehicleObj.displayVehicle();
	commuterObj.displayCommuter();
	cout << endl;
}

ostream& operator<<(ostream& output,Schedule& sch) { //Overload << function for schedule class, Polymorphism. Author: Gary Tey
	output << left << setw(15) << sch.schID << left << sch.vehicleObj.getType() << setw(15) << sch.vehicleObj.getNum() << left << setw(15) << sch.vehicleObj.getDestination() << right << setw(2) << setfill('0') << sch.vehicleObj.getHour() << ':' << right << setw(2) << setfill('0') << sch.vehicleObj.getMin() << left << setw(10) << setfill(' ') << ' ' << left << setw(15) << sch.vehicleObj.getCapacity() << left << sch.commuterObj.getType() << setw(15) << sch.commuterObj.getNum() << left << setw(15) << sch.commuterObj.getDestination() << right << setw(2) << setfill('0') << sch.commuterObj.getHour() << ':' << right << setw(2) << setfill('0') << sch.commuterObj.getMin() << left << setw(10) << setfill(' ') << ' ';
	return output;
}
int Schedule::comparenum(int id)
{
	if (id == commuterObj.getNum())
	{
		return vehicleObj.getNum();
	}
	else
		return NULL;
}