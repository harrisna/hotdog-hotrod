#ifndef _SCHEDULER_RTS_H_
#define _SCHEDULER_RTS_H_

#include "scheduler.h"

#include "processList.h"

class scheduler_mfqs: public scheduler {
	private:
	processList **queue;
	int currentTick;
	int numQueues;

	public:
	scheduler_mfqs(processList*, int);
	bool tick();
};

#endif
