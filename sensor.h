#ifndef SENSOR
#define SENSOR
#include <cstdlib>
#include <vector>
#include <tuple>

class Computer;

class Sensor 
{

public:
	Sensor();

private:

	int id;
	std::vector<Computer *> neighbors;
	//Clock params from olson's paper
	std::vector<float> sensorTimes;
	int messageCount;
	float frequency;
	std::vector<float> hostRecievedTimes;
	std::vector<float> offsets;
	std::vector<float> drifts; //?
	int timestampPosition;
	std::vector<std::string> valuesMeasured;
	std::vector<float> dataBuffer;
	std::string inputFilename;
	std::string outputFilename;
	std::vector<std::tuple<int,int,int, float>> sharedMeasurements;

/*
Explanation of variables:
id - id of the sensor or computer
neighbors - list of all neighbors that this sensor reports to
sensorTime - the computed sensor time from messageCount and frequency - in general I am going to trust hostRecievedTime over this value 
messageCount - number of measurements received
frequency - how often the sensor delivers messages
hostRecievedTime - the timestamp on the host when this message was received 
offset - offset from master cpu
drift - drift relative to master cpu
timestampPosition - the index of the column in the csv that is the timestamp value
valuesMeasured - strings describing the state variable(s) that is measured by the sensor
dataBuffer - holds all measurements made by the sensor
inputFilename - where the sensor data is written to
outputFilename - sensor data with adjust timestamp is written t
sharedMeasurements - list of tuples of (my data column index, other sensor id, other data column index, time offset between streams) - used to compute the phase between measurements of same quantities 
*/
};

#endif /*SENSOR*/