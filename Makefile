#Change this to work with current project 
EXENAME = timeSync
OBJS = main.o sensor.o computer.o tinyxml2.o interpolation.o ap.o alglibinternal.o alglibmisc.o linalg.o solvers.o optimization.o specialfunctions.o integration.o fasttransforms.o

CXX = g++
CXXFLAGS = -std=c++1y -c -g -O3 -Wall -Wextra -pedantic
LD = g++
LDFLAGS = -std=c++1y

all : $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -v -w -o $(EXENAME)

main.o : main.cpp sensor.h computer.h
	$(CXX) $(CXXFLAGS) main.cpp

sensor.o : sensor.cpp sensor.h computer.h stdafx.h interpolation.h
	$(CXX) $(CXXFLAGS) sensor.cpp

computer.o : computer.cpp computer.h sensor.h
	$(CXX) $(CXXFLAGS) computer.cpp

libtinyxml2.a: tinyxml2.o
	$(AR) $(ARFLAGS)s $@ $^

tinyxml2.o: tinyxml2.cpp tinyxml2.h

#interpolation.o: interpolation.h interpolation.cpp ap.h
#	$(CXX) $(CXXFLAGS) interpolation.cpp

#stdafx.o: stdafx.h

#ap.o: ap.h ap.cpp
#	$(CXX) $(CXXFLAGS) ap.cpp

#alglibinternal.o: alglibinternal.h alglibinternal.cpp
#	$(CXX) $(CXXFLAGS) alglibinternal.cpp

#alglibmisc.o: alglibmisc.h alglibmisc.cpp
#	$(CXX) $(CXXFLAGS) alglibmisc.cpp	
#include "ap.h"
#include "alglibinternal.h"
#include "alglibmisc.h"
#include "linalg.h"
#include "solvers.h"
#include "optimization.h"
#include "specialfunctions.h"
#include "integration.h"



clean :
	-rm -f *.o $(EXENAME)
