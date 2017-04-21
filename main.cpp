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


int main()
{
    std::cout<<"Running \n";
    /************ Load in config file and populate objects ********************/
    using namespace tinyxml2;
    XMLDocument doc;
    XMLError err;
    err = doc.LoadFile( "config.xml");
    XMLCheckResult(err);
std::cout<<"Hi\n";
    XMLNode * pRoot = doc.FirstChild();
    std::cout<<"here\n";
    XMLNullPointerCheck(pRoot);
std::cout<<"Loaded document\n";
    XMLElement *listElement = pRoot->FirstChildElement("List");
    XMLNullPointerCheck(listElement);
    listElement = listElement->FirstChildElement("Sensor");
    XMLNullPointerCheck(listElement);
    XMLElement *curField;
    while(listElement != NULL)
    {
        int id;
        curField = listElement->FirstChildElement("id");
        XMLNullPointerCheck(curField);
        err = curField->QueryIntText(&id);
        XMLCheckResult(err);
        std::cout<<"id is: " << id <<"\n";
        listElement = listElement->NextSiblingElement("Sensor");
    }


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
state estimations
observability strength values
How to associate observability strength with particular time as state estimations might be delayed
    current plan: ignore it and use the observability strength when it is computed
observability threshold set by user for when to turn ptp on

*/