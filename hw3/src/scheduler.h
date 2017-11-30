#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <queue>
#include <vector>

#include "process.h"

class scheduler {
	protected:
	std::queue<process> incoming_unsorted;
	std::queue<process> out;

	int currentTick;

	bool cpuOccupied;
	process cpu;
	
	public:
	scheduler(char*);
	virtual bool tick() {return true;}
};

#endif
