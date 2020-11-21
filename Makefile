all: fin

fin: main.o ParallelScheduler.o
	g++ main.o ParallelScheduler.o -lpthread -o fin

main.o: main.cpp
	g++ -c main.cpp 

ParllelScheduler.o: ParallelScheduler.cpp
	g++ -c ParallelScheduler.cpp

clean:
	rm -rf *.o fin
