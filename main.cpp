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
    #define XMLNullPointerCheck(pointer) if (pointer == NULL) {return -1;}
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
    std::cout << stateFilename << "\n";
    std::cout << "\n";
    for (int i = 0; i < stateVars.size(); i++)
        std::cout << "State variables: " << stateVars[i] <<"\n";

    /*

    listElement = pRoot->FirstChildElement("List");
    XMLNullPointerCheck(listElement);
    listElement = listElement->FirstChildElement("Sensor");
    XMLNullPointerCheck(listElement);
    XMLElement *curField;
    //Each listElement points to a sensor
    while(listElement != NULL)
    {

        int id;
        curField = listElement->FirstChildElement("id");
        XMLNullPointerCheck(curField);
        err = curField->QueryIntText(&id);

        XMLCheckResult(err);
        listElement = listElement->NextSiblingElement("Sensor");
    }
*/

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