#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <queue>
#include <vector>

#include "process.h"
#include "gantt.h"

class scheduler {
	protected:
	std::queue<process> incoming_unsorted;
	std::queue<process> out;
	std::queue<gantt> chart;

	int currentTick;

	bool cpuOccupied;
	process cpu;
	gantt cpuGantt;

	double avgWait;
	double avgTurnaround;
	
	public:
	scheduler(char*);
	scheduler(std::queue<process>);
	virtual bool tick() {return true;}
	void printStats();
};

#endif
