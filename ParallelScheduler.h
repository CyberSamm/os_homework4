#ifndef PARALLEL_SHEDULER_H
#define PARALLEL_SHEDULER_H

#include <pthread.h>
#include <queue>
#include <utility>

typedef void (*scheduler_fn_t)(void*);

class ParallelScheduler {

private:
	int capacity; 
	pthread_t* threads;
	std::queue< std::pair<scheduler_fn_t, void*> > functions;
	
	pthread_mutex_t* queueLock; // mutex
	pthread_cond_t* hasFunction; // conditional valuable
	
private:	
	static void* execute(void* arg);
	
public:
	ParallelScheduler(int capacity);
	void run(scheduler_fn_t func, void* arg);
	~ParallelScheduler();
};

#endif

