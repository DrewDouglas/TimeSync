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
	Sensor(int _id, float _alphaFast, float _alphaSlow, float _frequency, 
			   int _timestampPosition, std::string _inputFilename, std::string _outputFilename,
			   std::vector<int> _neighbors, std::vector<std::string> _valuesMeasured, int _processOnCurDevice);

	int id;
	std::vector<Computer *> neighbors;
	std::vector<int> neighborIDs;
	float alphaFast;
	float alphaSlow;
	std::vector<float> sensorTimes;
	int messageCount;
	float frequency;
	std::vector<float> hostRecievedTimes;
	std::vector<float> offsets;
	std::vector<float> drifts; //?
	int timestampPosition;
	std::vector<std::string> valuesMeasured;
	std::vector<float> dataBuffer;
	int processOnCurDevice;
	std::string inputFilename;
	std::string outputFilename;
	std::vector<std::tuple<int,Sensor *,int, float>> sharedMeasurements;


//NOTE -- need to place restrictions on inputFile and outputFile based on the local cpu
//Assume that each cpu has runs their 

private:

/*
Explanation of variables:
id - id of the sensor or computer
neighbors - list of all neighbors that this sensor reports to
alphaFast - upper bound on positive clock skew increase in one timestep (as a percent)
alphaSlow - upper bond on negative clock skew increase in one timestep (as a percent)
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
sharedMeasurements - list of tuples of (my data column index, other sensor pointer, other data column index, time offset between streams) - used to compute the phase between measurements of same quantities 
*/
};

#endif /*SENSOR*/