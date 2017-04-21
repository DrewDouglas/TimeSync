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
			   std::vector<Computer *> _neighbors, std::vector<std::string> _valuesMeasured, int _processOnCurDevice)
{
	id = _id;
	alphaFast = _alphaFast;
	alphaSlow = _alphaSlow;
	frequency = _frequency;
	timestampPosition = _timestampPosition;
	inputFilename = _inputFilename;
	outputFilename = _outputFilename;
	neighbors = _neighbors;
	valuesMeasured = _valuesMeasured;
	processOnCurDevice = _processOnCurDevice;
}