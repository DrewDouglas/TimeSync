#include "computer.h"

/*
 * Default constructor
 */
Computer::Computer()
{
	id = -1;
	master = 0;
	isThisDevice = 0;
}

/*
 * Parameterized constructor
 * (Might initialize neighbors with separate function but we'll see)
 */
 Computer::Computer(int _id, int _master, int _isThisDevice, 
 					std::vector<Computer *> _computerNeighbors, std::vector<Sensor *> _sensorNeighbors)
 {
 	id = _id;
 	master = _master;
 	isThisDevice = _isThisDevice;
 	computerNeighbors = _computerNeighbors;
 	sensorNeighbors = _sensorNeighbors;
 }