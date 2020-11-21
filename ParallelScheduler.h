#pragma once
#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>


class ParallelScheduler {
private:
	pthread_t* tid;
	std::vector< std::pair<void(*)(void*), void*> > functions;
	
	int N;
	int numOfFuncs;
	
	pthread_mutex_t my_mutex; // mutex
	pthread_cond_t my_cond;
	 
	
	//sem_t lock; // Only N funcs can work in parallel. Other funcs have to wait. 
 	
	static void* thread(void* arg);
	
public:
	ParallelScheduler(int count);
	void run(void (*func)(void*) , void* arg);
	~ParallelScheduler();
};
