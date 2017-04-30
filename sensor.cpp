#include "sensor.h"
#include "stdafx.h"
#include "interpolation.h"
#include "fasttransforms.h"
#include <algorithm>
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
Sensor::Sensor(int _id, double _alphaFast, double _alphaSlow, double _frequency, 
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
//	std::vector<std::tuple<int,Sensor *,int, double>> sharedMeasurements;
//sharedMeasurements - list of tuples of (my data column index, other sensor pointer, other data column index, time offset between streams) - used to compute the phase between measurements of same quantities 
	for (size_t i = 0; i < this->sharedMeasurements.size(); i++)
	{
		std::tuple<int, Sensor *, int, double> cur = this->sharedMeasurements[i];
		//Calculate discrete cross correlation using alglib
		//Find maximum value -> gives time delay between streams
		double myFreq = this->frequency;
		int myCol = std::get<0>(cur);

		Sensor * other = std::get<1>(cur);
		double otherFreq = other->frequency;
		int otherCol = std::get<2>(cur);
		//NOTE: With large discrepancy in frequency the interpolations might not be accurate 
		//Use last n measurements from both sensors for interpolation for now -- can come back and optimize by considering frequency
		

		//Both sensors must have made 20 measurements so far
		if (this->sensorTimes.size() < 20 || other->sensorTimes.size() < 20)
			continue;

		std::vector<double> myData;
		std::vector<double> otherData;

		int myLastDataIdx = this->dataBuffer.size() - 1 - this->valuesMeasured.size() + myCol;
		int otherLastDataIdx = other->dataBuffer.size() - 1 - other->valuesMeasured.size() + otherCol;

		for (int j = 0; j < 20; j++)
		{
			myData.push_back(this->dataBuffer[myLastDataIdx - (j * (this->valuesMeasured.size() + 1))]);
			otherData.push_back(other->dataBuffer[otherLastDataIdx - (j * (other->valuesMeasured.size() + 1))]);
		}

		//Need to reverse myData and otherData as they were populated from higher times to lower times
		std::reverse(myData.begin(), myData.end());
		std::reverse(otherData.begin(), otherData.end());

		alglib::real_1d_array myX, myY, otherX, otherY;
		
		myX.setcontent(20, (const double *)&(this->sensorTimes[this->sensorTimes.size() - 21]));
		myY.setcontent(20, (const double *)&(myData[0]));

		otherX.setcontent(20, (const double *)&(other->sensorTimes[other->sensorTimes.size() - 21]));
		otherY.setcontent(20, (const double *)&(otherData[0]));

		alglib::spline1dinterpolant myS, otherS;
		alglib::spline1dbuildlinear(myX, myY, myS);
		alglib::spline1dbuildlinear(otherX, otherY, otherS);

		//Working with 20 interpolated data points
		std::vector<double> myInterpolation;
		std::vector<double> otherInterpolation;
		for (int k = 21; k > 0; k++)
		{
			double interpolationTime = this->sensorTimes[this->sensorTimes.size() - k];
			myInterpolation.push_back(alglib::spline1dcalc(myS, interpolationTime));
			otherInterpolation.push_back(alglib::spline1dcalc(otherS, interpolationTime));
		}

		alglib::real_1d_array mySignal, otherSignal;
		alglib::real_1d_array result;
		mySignal.setcontent(20, (const double *)&(myInterpolation[0]));
		otherSignal.setcontent(20, (const double *)&(otherInterpolation[0]));
		//Compute the cross correlation between the two sensors
		alglib::corrr1d(mySignal, 20, otherSignal, 20, result);

		//Lags range from -20 to 20 -- starting at index 0
		double max = -100;
		double maxIdx = -1;
		for (int p = 0; p < result.length(); p++)
		{
			if (result[p] > max)
			{
				max = result[p];
				maxIdx = p;
			}
		}

		//maxIdx
		double lags = maxIdx - 20;
		//lags now contains the number of delta_ts that the other sensor is ahead (delta_t is the 1/frequency of cur sensor)
		double timeOffset = lags / myFreq;
		//timeOffset should be added to the other sensor's time in order to be adjusted to the time of 'this' sensor
		std::get<3>(this->sharedMeasurements[i]) = timeOffset;
	}
}

