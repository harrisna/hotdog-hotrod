#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "processList.h"

class scheduler {
	public:
	scheduler(processList*);
	int tick();
};

#endif
