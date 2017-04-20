#Change this to work with current project 
EXENAME = timeSync
OBJS = main.o sensor.o computer.o

CXX = clang++
CXXFLAGS = -std=c++1y -stdlib=libc++ -c -g -O3 -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi

all : $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -v  -o $(EXENAME)

main.o : main.cpp sensor.h computer.h
	$(CXX) $(CXXFLAGS) main.cpp

sensor.o : sensor.cpp sensor.h computer.h
	$(CXX) $(CXXFLAGS) png.cpp

computer.o : computer.cpp computer.h sensor.h
	$(CXX) $(CXXFLAGS) rgbapixel.cpp

clean :
	-rm -f *.o $(EXENAME)
