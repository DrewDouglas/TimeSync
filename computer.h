#ifndef COMPUTER
#define COMPUTER
#include <cstdlib>
#include <vector>
#include <tuple>
#include "sensor.h"

class Sensor;

class Computer
{

public:
	Computer();
	Computer(int _id, int _master, int _isThisDevice, 
 					std::vector<Computer *> _computerNeighbors, std::vector<Sensor *> _sensorNeighbors);

	int id;
	std::vector<Computer *> computerNeighbors;
	std::vector<Sensor *> sensorNeighbors;
	int master;
	int isThisDevice;
	//Clock params from olson's paper
	std::vector<float> times;
	std::vector<float> offsets;
	std::vector<float> drift; //?

private:

};

#endif /*COMPUTER*/