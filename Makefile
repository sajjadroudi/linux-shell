CC = g++

main: main.o

main.o: main.cpp

clean:
	rm -f *.out *.o main