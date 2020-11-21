#include <iostream>
#include <vector>
#include <pthread.h>
#include "ParallelScheduler.h"


ParallelScheduler::ParallelScheduler (int count)
	: numOfFuncs{count}
	, tid { new pthread_t[numOfFuncs] }
{
	for(int i = 0; i < numOfFuncs; ++i){
		int threadCreated = pthread_create(&tid[i], NULL, thread, this);
		
		if(threadCreated != 0){
			std::cerr << "Something wrong with ctreating thread!\n";
		}
	}
};

void* ParallelScheduler::thread ( void* arg){
	ParallelScheduler* scheduler = (ParallelScheduler*) arg;
	
	pthread_mutex_lock(&scheduler->my_mutex); // mutex lock
	while(scheduler->functions.empty()) {
		// WAIT
		pthread_cond_wait(&scheduler->my_cond, &scheduler->my_mutex);
	}
	
	// run function
	scheduler->functions.back().first(scheduler->functions.back().second);
	scheduler->functions.pop_back();
	
	
	pthread_mutex_unlock(&scheduler->my_mutex);
		
	return NULL;
}

void ParallelScheduler::run(void (*func)(void*), void* arg){
	ParallelScheduler* scheduler = (ParallelScheduler*) arg;
	
	pthread_mutex_lock(&scheduler->my_mutex);
	
	
	
	// add function
	functions.push_back( std::make_pair(func, arg) );

	
	
	pthread_mutex_unlock(&scheduler->my_mutex);
	
	pthread_cond_broadcast(&scheduler->my_cond);
	
}


ParallelScheduler::~ParallelScheduler() {

	for(int i = 0; i < numOfFuncs; ++i){
		pthread_join(tid[i], NULL); // or pthread_cancel(tid[i]);
	}
}
