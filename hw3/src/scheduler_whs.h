#ifndef _SCHEDULER_WHS_H_
#define _SCHEDULER_WHS_H_

#include <queue>
#include <vector>

#include "scheduler.h"
#include "process.h"

class scheduler_whs: public scheduler {
	private:
	int numQueues;
	std::queue<process> queue[100];
	std::priority_queue<process, std::vector<process>, procCmpIOWHS> wait;
	std::priority_queue<process, std::vector<process>, procCmpArrivalWHS> incoming;

	int baseTimeQuantum;
	int agingThreshold;

	int timeQuantum;
	int demoteQueue;

	public:
	scheduler_whs(char*, int baseTimeQuantum, int agingThreshold);
	scheduler_whs(std::queue<process>, int baseTimeQuantum, int agingThreshold);
	bool tick();
};

#endif
