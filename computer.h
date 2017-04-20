#ifndef COMPUTER
#define COMPUTER
#include <stdlib>
#include "sensor.h"

class Computer
{

public:


private:

	int id;
	std::vector<Computer> computerNeighbors;
	std::vector<Sensor> sensorNeighbors;
	int master;
	int isThisDevice;
	//Clock params from olson's paper
	std::vector<float> times;
	std::vector<float> offsets;
	std::vector<float> drift; //?

//Once I have a working class definition going work on reading from xml files
	//Should make sure to make my constructors in a way that assigns intelligent defaults
	/*
	ID
	Neighbors
	clock params
	/master/slave
	/this device
	current offset
	current drift
	variables measured

	*/
};

#endif