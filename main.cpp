



//Code to read from a growing file 
#include <iostream>
#include <string>
#include <fstream>

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

/*
Globals that need to be take care of:
state estimations
observability strength values
How to associate observability strength with particular time as state estimations might be delayed
    current plan: ignore it and use the observability strength when it is computed
observability threshold set by user for when to turn ptp on

*/