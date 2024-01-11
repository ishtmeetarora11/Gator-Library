CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: gatorLibrary

gatorLibrary: gatorLibrary.cpp
	$(CXX) $(CXXFLAGS) -o gatorLibrary gatorLibrary.cpp

clean:
	rm -f gatorLibrary
