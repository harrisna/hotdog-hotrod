#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "processList.h"

class scheduler {
	void schedule(processList*);
	int tick();
};

#endif