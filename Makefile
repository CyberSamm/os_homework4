all: ParallelScheduler

ParallelScheduler: main.o ParallelScheduler.o
	g++ -pthread main.o ParallelScheduler.o -o ParallelScheduler

main.o: main.cpp
	g++ -c main.cpp -o main.o

ParllelScheduler.o: ParallelScheduler.cpp
	g++ -c ParallelScheduler.cpp -o ParallelScheduler.o

clean:
	rm -rf *.o ParallelScheduler
