#Change this to work with current project 
EXENAME = timeSync
OBJS = main.o sensor.o computer.o tinyxml2.o

CXX = g++
CXXFLAGS = -std=c++1y -c -g -O3 -Wall -Wextra -pedantic
LD = g++
LDFLAGS = -std=c++1y

all : $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -v  -o $(EXENAME)

main.o : main.cpp sensor.h computer.h
	$(CXX) $(CXXFLAGS) main.cpp

sensor.o : sensor.cpp sensor.h computer.h
	$(CXX) $(CXXFLAGS) sensor.cpp

computer.o : computer.cpp computer.h sensor.h
	$(CXX) $(CXXFLAGS) computer.cpp

libtinyxml2.a: tinyxml2.o
	$(AR) $(ARFLAGS)s $@ $^

tinyxml2.o: tinyxml2.cpp tinyxml2.h


clean :
	-rm -f *.o $(EXENAME)
