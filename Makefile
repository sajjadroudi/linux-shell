CXX = g++
OBJ = main.o
CXXFLAGS = -w

all: main

main: $(OBJ)
	$(CXX) $(CXXFLAGS) -o main $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.out *.o main