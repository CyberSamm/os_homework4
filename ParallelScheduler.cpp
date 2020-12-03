#include "ParallelScheduler.h"
#include <signal.h>
#include <iostream>

ParallelScheduler::ParallelScheduler (int capacity)
	: capacity(capacity)
{
	this->threads = new pthread_t[this->capacity];
	this->queueLock = new pthread_mutex_t(); 
	this->hasFunction  = new pthread_cond_t();

	pthread_mutex_init(this->queueLock, NULL);
	pthread_cond_init(this->hasFunction, NULL);
	
	for(int i = 0; i < this->capacity; ++i){
		
		int threadCreated = pthread_create(&threads[i], NULL, execute, this);
		
		if(threadCreated != 0){
			std::cerr << "Something wrong with ctreating thread!\n";
		}
	}
};

void* ParallelScheduler::execute ( void* arg){

	ParallelScheduler* scheduler = (ParallelScheduler*)arg;
	while(true){
		
		pthread_mutex_lock(scheduler->queueLock); // mutex lock
		
		while(scheduler->functions.empty()) {
			// WAIT
			pthread_cond_wait(scheduler->hasFunction, scheduler->queueLock);
		}
		
		auto funcPair = scheduler->functions.front();
		scheduler->functions.pop();
		
		pthread_mutex_unlock(scheduler->queueLock);
		
		funcPair.first(funcPair.second); // run function
	}
	return NULL;
}

void ParallelScheduler::run(scheduler_fn_t func, void* arg){

	pthread_mutex_lock(this->queueLock);
	
	// add a function
	this->functions.push( std::make_pair(func, arg) );

	pthread_mutex_unlock(this->queueLock);
	
	pthread_cond_signal(this->hasFunction);
}


ParallelScheduler::~ParallelScheduler() {

	for(int i = 0; i < this->capacity; ++i){
		pthread_kill(this->threads[i], SIGKILL);
	}
	
	delete[] this->threads;
	
	pthread_mutex_destroy(this->queueLock);
	delete this->queueLock;
	
	pthread_cond_destroy(this->hasFunction);
	delete this->hasFunction;
	
}

