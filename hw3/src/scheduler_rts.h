#ifndef _SCHEDULER_RTS_H_
#define _SCHEDULER_RTS_H_

#include <queue>
#include <vector>

#include "scheduler.h"
#include "process.h"

class scheduler_rts: public scheduler {
	private:
	//processNode *cpu;
	//processList *incoming;
	std::priority_queue<process, std::vector<process>, procCmpDeadline> queue;
	std::priority_queue<process, std::vector<process>, procCmpArrival> incoming;
	bool soft;

	public:
	scheduler_rts(char*, bool);
	bool tick();
};

#endif
