#ifndef SENSOR
#define SENSOR
#include <cstdlib>
#include <vector>
#include <tuple>
#include <mutex>

class Computer;

class Sensor 
{

public:
	Sensor();
	Sensor(int _id, double _alphaFast, double _alphaSlow, double _frequency, 
			   int _timestampPosition, std::string _inputFilename, std::string _outputFilename,
			   std::vector<int> _neighbors, std::vector<std::string> _valuesMeasured, int _processOnCurDevice);
	void computeOffset();

	int id;
	std::vector<Computer *> neighbors;
	std::vector<int> neighborIDs;
	double alphaFast;
	double alphaSlow;
	std::vector<double> sensorTimes;
	int messageCount;
	double frequency;
	std::vector<double> hostRecievedTimes;
	std::vector<double> offsets;
	std::vector<double> drifts; //?
	int timestampPosition;
	std::vector<std::string> valuesMeasured;
	std::vector<double> dataBuffer;
	int processOnCurDevice;
	std::string inputFilename;
	std::string outputFilename;
	std::vector<std::tuple<int,Sensor *,int, double>> sharedMeasurements;
	std::mutex *mu;

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