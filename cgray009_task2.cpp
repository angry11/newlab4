#include <iostream>
#include "cpp.h"
#include <string.h>
using namespace std;

void createCars();
void car();
void display();
facility_ms pump_wait("pumps", 8);
double serviceTime(double);
const double profit = .025; // dollars per litre
double TotalLitresSold = 0.0;
double TotalLitresMissed = 0.0;
const double cost = 20;
int numPumps = 4;
double total_profit = 0.0;
double lost_profit = 0.0;
meter whoBalked("Discouraged customers");

extern "C" void sim()      // main process
{
	int sim_time;
	create("sim");
	cout << "Enter simulation time: ";
	cin >> sim_time;
	createCars();  // generate a stream of arriving customers
	hold (sim_time);              // wait for a whole day (in minutes) to pass
	report();
	display();
}

void createCars()
{
	create("car");
	while(1)          // run for one day (in minutes)
	{
		hold(expntl(10));           // exponential interarrivals, mean 10 minutes
		car();      // new passenger appears at this location
	}
}

void car()
{
	create("car life");
	double liters;
	liters = 10 + uniform(0, 1) * 50;
	double time = 0.0;
	int pumps_busy = pump_wait.qlength();
	double balk_val = (40 + liters) / (25 * (3 + pumps_busy));
	if(uniform(0, 1) < balk_val)
	{
		pump_wait.reserve();
		time = serviceTime(liters);
		hold(time);
		TotalLitresSold += liters;
		pump_wait.release();
	}
	else
	{
		whoBalked.note_passage();
		TotalLitresMissed += liters; 
	}
}

void display()
{
	total_profit = TotalLitresSold * profit
	- cost * numPumps;
	lost_profit = TotalLitresMissed * profit;
	cout << "Lost profit: " << lost_profit << endl;
	cout << "Total profit: " << total_profit << endl;
}

double serviceTime(double liters)
{
	double howLong = -6;
	for(int i = 1; i <= 12; i++)
		howLong += uniform(0, 1);
	return 150 + 0.5 * liters + 30 * howLong;
}
