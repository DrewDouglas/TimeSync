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
 */
 Computer::Computer(int _id, int _master, int _isThisDevice, 
 					std::vector<int> _computerNeighbors, std::vector<int> _sensorNeighbors)
 {
 	id = _id;
 	master = _master;
 	isThisDevice = _isThisDevice;
 	computerNeighborIDs = _computerNeighbors;
 	sensorNeighborIDs = _sensorNeighbors;
 }