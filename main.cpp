#include <iostream>
#include <string>
#include <fstream>
#include "sensor.h"
#include "computer.h"
#include "tinyxml2.h"
#include <thread>
#include <mutex>

#ifndef XMLCheckResult
    #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error processing XML file: %i\n", a_eResult); return a_eResult; }
#endif

#ifndef XMLNullPointerCheck
    #define XMLNullPointerCheck(pointer) if ((pointer) == NULL) {return -1;}
#endif

std::string getValue(std::string input)
{
    int first = input.find("\"");
    int last = input.find_last_of("\"");
    return input.substr(first+1, last-first-1);
}

void initializeObjects(std::vector<Sensor> sensors, std::vector<Computer> computers)
{
    //Initialize sensor neighbors
    for (size_t i = 0; i < sensors.size(); i++)
    {
        for (size_t j = 0; j < sensors[i].neighborIDs.size(); j++)
        {
            for (size_t k = 0; k < computers.size(); k++)
            {
                if (computers[k].id == sensors[i].neighborIDs[j])
                    sensors[i].neighbors.push_back(&computers[k]);
            }
        }
    }

    //Initialize computer neighbors
    for (size_t i = 0; i < computers.size(); i++)
    {
        for (size_t m = 0; m < computers[i].sensorNeighborIDs.size(); m++)
        {
            for (size_t p = 0; p < sensors.size(); p++)
            {
                if (sensors[p].id == computers[i].sensorNeighborIDs[m])
                    computers[i].sensorNeighbors.push_back(&sensors[p]);
            }
        }

        for (size_t n = 0; n < computers[i].computerNeighborIDs.size(); n++)
        {
            for (size_t q = 0; q < computers.size(); q++)
            {
                if (computers[q].id == computers[i].computerNeighborIDs[n])
                    computers[i].computerNeighbors.push_back(&computers[q]);
            }
        }
    }

    //Initialize sharedMeasurements for each sensor
    for (size_t i = 0; i < sensors.size(); i ++)
    {
        for (size_t j = 0; j < sensors.size(); j++)
        {
            if (i != j)
            {
                //Compare the variables measured by each pair of sensor to fill out sharedMeasurements 
                for (size_t m = 0; m < sensors[i].valuesMeasured.size(); m++)
                {
                    std::string firstSensorVal = sensors[i].valuesMeasured[m];
                    for (size_t n = 0; n < sensors[j].valuesMeasured.size(); n++)
                    {
                        std::string secondSensorVal = sensors[j].valuesMeasured[n];
                        //Found a shared variable between sensors 
                        if (firstSensorVal == secondSensorVal)
                        {
                            sensors[i].sharedMeasurements.push_back(std::make_tuple(m, &sensors[j], n, -100.0));
                        }
                    }
                }
            }
        }
    }
}

int main()
{
//NOTE: may want a global state update frequency 

    std::vector<Sensor> sensors;
    std::vector<Computer> computers;
    std::vector<std::string> stateVars;
    double unobservabilityIndexThreshold;
    double conditionNumberThreshold;
    double unobservabilityIndex;
    double conditionNumber;

    /************ Load in config file and populate objects ********************/
    using namespace tinyxml2;
    XMLDocument doc;
    XMLError err;
    err = doc.LoadFile( "config.xml");
    XMLCheckResult(err);
    XMLNode * pRoot = doc.FirstChild();
    XMLNullPointerCheck(pRoot);

    //Get the input file that state estimations are read from
    XMLPrinter printer;
    XMLElement *listElement = pRoot->FirstChildElement("stateInputFile");
    std::string stateFilename;
    listElement->Accept(&printer);
    stateFilename = printer.CStr();
    stateFilename = getValue(stateFilename);
    printer.ClearBuffer();

    //Get the list of all state variables 
    listElement = listElement->NextSiblingElement("List");
    XMLNullPointerCheck(listElement);
    XMLElement * stateVariable = listElement->FirstChildElement("stateVar");
    XMLNullPointerCheck(stateVariable);
    while(stateVariable != NULL)
    {
        stateVariable->Accept(&printer);
        stateVars.push_back(getValue(printer.CStr()));
        printer.ClearBuffer();
        stateVariable = stateVariable->NextSiblingElement("stateVar");
    }
    
    //Read in the observability strength threshold values 
    listElement = listElement->NextSiblingElement("unobservabilityIndexThreshold");
    XMLNullPointerCheck(listElement);
    err = listElement->QueryDoubleText(&unobservabilityIndexThreshold);
    XMLCheckResult(err);

    listElement = listElement->NextSiblingElement("conditionNumberThreshold");
    XMLNullPointerCheck(listElement);
    err = listElement->QueryDoubleText(&conditionNumberThreshold);
    XMLCheckResult(err);

    //Read in all of the sensor information
    listElement = listElement->NextSiblingElement("List");
    XMLNullPointerCheck(listElement);
    XMLElement * curSensor = listElement->FirstChildElement("Sensor");
    XMLNullPointerCheck(curSensor);
    XMLElement *curField;

    //Each listElement points to a sensor
    while(curSensor != NULL)
    {
        int id, timestampPosition, processOnDevice, temp;
        std::vector<int> neighborIDs;
        double alphaFast, alphaSlow, frequency;
        std::vector<std::string> valsMeasured;
        std::string inFile, outFile;

        curField = curSensor->FirstChildElement("id");
        XMLNullPointerCheck(curField);
        curField->QueryIntText(&id);

        curField = curField->NextSiblingElement("List");
        XMLNullPointerCheck(curField);
        XMLElement * cpuID = curField->FirstChildElement("computerID");
        XMLNullPointerCheck(cpuID);
        while(cpuID != NULL)
        {
            cpuID->QueryIntText(&temp);
            neighborIDs.push_back(temp);
            cpuID = cpuID->NextSiblingElement("computerID");
        }

        curField = curField->NextSiblingElement("alphaFast");
        XMLNullPointerCheck(curField);
        curField->QueryDoubleText(&alphaFast);

        curField = curField->NextSiblingElement("alphaSlow");
        XMLNullPointerCheck(curField);
        curField->QueryDoubleText(&alphaSlow);

        curField = curField->NextSiblingElement("frequency");
        XMLNullPointerCheck(curField);
        curField->QueryDoubleText(&frequency);

        curField = curField->NextSiblingElement("timestampPosition");
        XMLNullPointerCheck(curField);
        curField->QueryIntText(&timestampPosition);

        curField = curField->NextSiblingElement("List");
        XMLNullPointerCheck(curField);
        XMLElement * valMeas = curField->FirstChildElement("valueMeasured");
        XMLNullPointerCheck(valMeas);
        while(valMeas != NULL)
        {
            valMeas->Accept(&printer);
            valsMeasured.push_back(getValue(printer.CStr()));
            printer.ClearBuffer();
            valMeas = valMeas->NextSiblingElement("valueMeasured");
        }

        curField = curField->NextSiblingElement("inputFile");
        XMLNullPointerCheck(curField);
        curField->Accept(&printer);
        inFile = getValue(printer.CStr());
        printer.ClearBuffer();

        curField = curField->NextSiblingElement("outputFile");
        XMLNullPointerCheck(curField);
        curField->Accept(&printer);
        outFile = getValue(printer.CStr());
        printer.ClearBuffer();        

        curField = curField->NextSiblingElement("processOnCurDevice");
        XMLNullPointerCheck(curField);
        err = curField->QueryIntText(&processOnDevice);
        XMLCheckResult(err);

        curSensor = curSensor->NextSiblingElement("Sensor");

        //Construct the sensor and add it to the list
        Sensor * newSensor = new Sensor(id, alphaFast, alphaSlow, frequency, timestampPosition, 
                                        inFile, outFile, neighborIDs, valsMeasured, processOnDevice);
        sensors.push_back(*newSensor);
        delete newSensor;
    }

    //Read in all computer data
    listElement = listElement->NextSiblingElement("List");
    XMLNullPointerCheck(listElement);
    XMLElement * curComputer = listElement->FirstChildElement("Computer");
    XMLNullPointerCheck(curComputer);
    //Go through and change everything to be correct to computer and not sensor 
    while(curComputer != NULL)
    {
        int id, master, thisDevice, temp;
        std::vector<int> computerNeighborIDs, sensorNeighborIDs;

        curField = curComputer->FirstChildElement("id");
        XMLNullPointerCheck(curField);
        curField->QueryIntText(&id);

        curField = curField->NextSiblingElement("List");
        XMLNullPointerCheck(curField);
        XMLElement * cpuID = curField->FirstChildElement("computerID");
        XMLNullPointerCheck(cpuID);
        while(cpuID != NULL)
        {
            cpuID->QueryIntText(&temp);
            computerNeighborIDs.push_back(temp);
            cpuID = cpuID->NextSiblingElement("computerID");
        }

        curField = curField->NextSiblingElement("List");
        XMLNullPointerCheck(curField);
        XMLElement * sensorID = curField->FirstChildElement("sensorID");
        XMLNullPointerCheck(sensorID);
        while(sensorID != NULL)
        {
            sensorID->QueryIntText(&temp);
            sensorNeighborIDs.push_back(temp);
            sensorID = sensorID->NextSiblingElement("sensorID");
        }

        curField = curField->NextSiblingElement("master");
        XMLNullPointerCheck(curField);
        curField->QueryIntText(&master);

        curField = curField->NextSiblingElement("isThisDevice");
        XMLNullPointerCheck(curField);
        curField->QueryIntText(&thisDevice);

        curComputer = curComputer->NextSiblingElement("Computer");

        //Construct CPU and add it to list 
        Computer * newComputer = new Computer(id, master, thisDevice, computerNeighborIDs, sensorNeighborIDs);
        computers.push_back(*newComputer);
        delete newComputer;
    }

    initializeObjects(sensors, computers);

    //Each sensor needs a mutex to protect its data buffer 
    //Actually this might not be true -- only need a thread for each sensor actually connected to this computer
    //Only launch the minimum number of threads and create minimum number of mutexes
    //Think about what data is going to be needed on what cpu
    std::vector<std::mutex> mutexes(sensors.size());
    for (size_t i = 0; i < sensors.size(); i++)
    {
        sensors[i].mu = &mutexes[i];
    }
    //H
    std::mutex unobsIdxMutex;
    std::mutex conditionNumMutex;

//TODO:
    //Write sensor functions
    //Write computer functions
    //Think about who should be writing the sensor adjusted data -- want it all in relation to master host time
    //Figure out how obs calculations go 

    //Come up with test cases later 

    return 0;
}

/*
//Code to read from a growing file 
int main()
{
    std::ifstream ifs("test.log");

    if (ifs.is_open())
    {
        std::string line;
        while (true)
        {
            while (std::getline(ifs, line)) std::cout << line << "\n";
            if (!ifs.eof()) break; // Ensure end of read was EOF.
            ifs.clear();

            // You may want a sleep in here to avoid
            // being a CPU hog.
        }
    }

    return 0;
}
*/

/*
Globals that need to be take care of:
state estimations - input file
observability strength values
How to associate observability strength with particular time as state estimations might be delayed
    current plan: ignore it and use the observability strength when it is computed
observability threshold set by user for when to turn ptp on

*/