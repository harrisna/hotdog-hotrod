#ifndef _SCHEDULER_RTS_H_
#define _SCHEDULER_RTS_H_

#include "scheduler.h"

#include "processList.h"

class scheduler_rts: public scheduler {
	private:
	processList *ps;
	processList *queue;
	int currentTick;

	public:
	void scheduler(processList*);
	void tick();
};

#endif
