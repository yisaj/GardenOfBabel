CXX = g++
CXXFLAGS = -Wall -std=gnu++0x

all: render cross mutate

render: render.o Genome.o Cell.o Plant.o
	$(CXX) $(CXXFLAGS) -o render render.o Genome.o Cell.o Plant.o 

render.o: render.cpp
	$(CXX) $(CXXFLAGS) -c render.cpp

cross: cross.o Genome.o
	$(CXX) $(CXXFLAGS) -o cross cross.o Genome.o

cross.o: cross.cpp
	$(CXX) $(CXXFLAGS) -c cross.cpp

mutate: mutate.o Genome.o
	$(CXX) $(CXXFLAGS) -o mutate mutate.o Genome.o

mutate.o: mutate.cpp
	$(CXX) $(CXXFLAGS) -c mutate.cpp

Genome.o: Genome.cpp Genome.hpp
	$(CXX) $(CXXFLAGS) -c Genome.cpp

Cell.o: Cell.cpp Cell.hpp
	$(CXX) $(CXXFLAGS) -c Cell.cpp

Plant.o: Plant.cpp Plant.hpp
	$(CXX) $(CXXFLAGS) -c Plant.cpp

clean:
	rm -f $(BIN)render render.o $(BIN)cross cross.o $(BIN)mutate mutate.o Genome.o Cell.o Plant.o
