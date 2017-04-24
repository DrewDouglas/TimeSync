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
 					std::vector<int> _computerNeighbors, std::vector<int> _sensorNeighbors);

	int id;
	std::vector<Computer *> computerNeighbors;
	std::vector<int> computerNeighborIDs;
	std::vector<Sensor *> sensorNeighbors;
	std::vector<int> sensorNeighborIDs;
	int master;
	int isThisDevice;
	std::vector<float> times;
	std::vector<float> offsets;
	std::vector<float> drift;

private:

};

#endif /*COMPUTER*/