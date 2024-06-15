#pragma once

class Schedule
{
	friend ostream& operator<<(ostream& output, Schedule& sch);	//friend function
private:
	int schID;
	Vehicle vehicleObj;
	Commuter commuterObj;
public:
	Schedule(int schID, Vehicle vehicle, Commuter commuter);
	void displaySchedule();
	int comparenum(int id);
};
