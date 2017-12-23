CXX = g++
CXXFLAGS = -Wall -pg -O2 -std=gnu++0x
SRC = ../src/

all: render cross mutate

render: render.o Genome.o Cell.o Plant.o
	$(CXX) $(CXXFLAGS) -o render render.o Genome.o Cell.o Plant.o 

render.o: $(addprefix $(SRC), render.cpp)
	$(CXX) $(CXXFLAGS) -c $(addprefix $(SRC), render.cpp)

cross: cross.o Genome.o
	$(CXX) $(CXXFLAGS) -o cross cross.o Genome.o

cross.o: $(addprefix $(SRC), cross.cpp)
	$(CXX) $(CXXFLAGS) -c $(addprefix $(SRC), cross.cpp)

mutate: mutate.o Genome.o
	$(CXX) $(CXXFLAGS) -o mutate mutate.o Genome.o

mutate.o: $(addprefix $(SRC), mutate.cpp)
	$(CXX) $(CXXFLAGS) -c $(addprefix $(SRC), mutate.cpp)

Genome.o: $(addprefix $(SRC), Genome.cpp Genome.hpp)
	$(CXX) $(CXXFLAGS) -c $(addprefix $(SRC), Genome.cpp)

Cell.o: $(addprefix $(SRC), Cell.cpp Cell.hpp)
	$(CXX) $(CXXFLAGS) -c $(addprefix $(SRC), Cell.cpp)

Plant.o: $(addprefix $(SRC), Plant.cpp Plant.hpp)
	$(CXX) $(CXXFLAGS) -c $(addprefix $(SRC), Plant.cpp)

clean:
	cd bin
	rm -f render render.o cross cross.o mutate mutate.o Genome.o Cell.o Plant.o
