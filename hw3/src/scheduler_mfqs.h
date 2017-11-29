#ifndef _SCHEDULER_MFQS_H_
#define _SCHEDULER_MFQS_H_

#include <queue>
#include <vector>

#include "scheduler.h"
#include "process.h"

class scheduler_mfqs: public scheduler {
	private:
	int numQueues;
	std::queue<process> queue[5];
	std::priority_queue<process, std::vector<process>, procCmpIOMFQS> wait;

	int baseTimeQuantum;
	int agingThreshold;

	int timeQuantum;
	int demoteQueue;

	public:
	scheduler_mfqs(char*, int numQueues, int baseTimeQuantum, int agingThreshold);
	bool tick();
};

#endif
