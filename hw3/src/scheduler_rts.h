#ifndef _SCHEDULER_RTS_H_
#define _SCHEDULER_RTS_H_

#include "scheduler.h"

#include "processList.h"

class scheduler_rts: public scheduler {
	private:
	processNode *cpu;
	processList *incoming;
	processList *queue;
	int currentTick;

	public:
	void scheduler(processList*);
	void tick();
};

#endif
