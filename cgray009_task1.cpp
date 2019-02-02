#include <iostream>
#include "cpp.h"
#include <string.h>
using namespace std;

void make_customer();
void customer();
facility barber_que("wait");

extern "C" void sim()      // main process
{
  create("sim");
  int sim_time;
  make_customer();  // generate a stream of arriving customers
  cout << "Enter simulation time: ";
  cin >> sim_time;
  hold (sim_time);              // wait for a whole day (in minutes) to pass
  report();
}

void make_customer()
{
	create("person");
	while(1)          // run for one day (in minutes)
	{
		hold(expntl(10));           // exponential interarrivals, mean 10 minutes
		customer();      // new passenger appears at this location
	}
}

void customer()
{
	create("customer life");
	barber_que.reserve();
	hold(uniform(10, 30));
	barber_que.release();
}
