#include "sensor.h"
/*
 * Default constructor
 */
Sensor::Sensor()
{
	id = -1;
	alphaFast = 0.0;
	alphaSlow = 0.0;
	messageCount = 0;
	frequency = -1;
	timestampPosition = -1;
	inputFilename = "";
	outputFilename = "";
	processOnCurDevice = 1;	
}


/*
 * Parametrized constructur from user entered data
 * (Will need to call initialization function to set sharedMeasurements
 * might want to do the when setting neighbors and valuesMeasured)
 */
Sensor::Sensor(int _id, float _alphaFast, float _alphaSlow, float _frequency, 
			   int _timestampPosition, std::string _inputFilename, std::string _outputFilename,
			   std::vector<int> _neighbors, std::vector<std::string> _valuesMeasured, int _processOnCurDevice)
{
	id = _id;
	alphaFast = _alphaFast;
	alphaSlow = _alphaSlow;
	frequency = _frequency;
	timestampPosition = _timestampPosition;
	inputFilename = _inputFilename;
	outputFilename = _outputFilename;
	neighborIDs = _neighbors;
	valuesMeasured = _valuesMeasured;
	processOnCurDevice = _processOnCurDevice;
}

void Sensor::computeOffset()
{
//	std::vector<std::tuple<int,Sensor *,int, float>> sharedMeasurements;
//sharedMeasurements - list of tuples of (my data column index, other sensor pointer, other data column index, time offset between streams) - used to compute the phase between measurements of same quantities 
	for (size_t i = 0; i < this->sharedMeasurements.size(); i++)
	{
		std::tuple<int, Sensor *, int, float> cur = this->sharedMeasurements[i];
	}
}

static float offsetHelper(std::vector<float> &dataBuffer1, int col1, std::vector<float> &dataBuffer2, int col2)
{
	return 0.0;
}