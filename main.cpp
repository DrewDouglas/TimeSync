#include <iostream>
#include <string>
#include <fstream>
#include "sensor.h"
#include "computer.h"
#include "tinyxml2.h"

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

int main()
{
    //All values that should be read in from the xml file 
    std::vector<Sensor> sensors;
    std::vector<Computer> computers;
    std::vector<std::string> stateVars;
    float unobservabilityIndexThreshold;
    float conditionNumberThreshold;

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
    err = listElement->QueryFloatText(&unobservabilityIndexThreshold);
    XMLCheckResult(err);

    listElement = listElement->NextSiblingElement("conditionNumberThreshold");
    XMLNullPointerCheck(listElement);
    err = listElement->QueryFloatText(&conditionNumberThreshold);
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
        float alphaFast, alphaSlow, frequency;
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
        curField->QueryFloatText(&alphaFast);

        curField = curField->NextSiblingElement("alphaSlow");
        XMLNullPointerCheck(curField);
        curField->QueryFloatText(&alphaSlow);

        curField = curField->NextSiblingElement("frequency");
        XMLNullPointerCheck(curField);
        curField->QueryFloatText(&frequency);

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
    }

    //Read in all computer data
    listElement = listElement->NextSiblingElement("List");
    XMLNullPointerCheck(listElement);
    XMLElement * curComputer = listElement->FirstChildElement("Computer");
    XMLNullPointerCheck(curComputer);
//Go through and change everything to be correct to computer and not sensor 
    while(curSensor != NULL)
    {
        int id, timestampPosition, processOnDevice, temp;
        std::vector<int> neighborIDs;
        float alphaFast, alphaSlow, frequency;
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
        curField->QueryFloatText(&alphaFast);

        curField = curField->NextSiblingElement("alphaSlow");
        XMLNullPointerCheck(curField);
        curField->QueryFloatText(&alphaSlow);

        curField = curField->NextSiblingElement("frequency");
        XMLNullPointerCheck(curField);
        curField->QueryFloatText(&frequency);

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
    }

    //First compile lists of all sensors, all cpus, and fill out global constants
    //Then run initialization function on lists to set all of the more complex fields
    //Once all objects are well defined -- create syncrhonization functions
    //Then run threads for all objects 


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