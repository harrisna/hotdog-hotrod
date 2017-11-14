#ifndef _SCHEDULER_RTS_H_
#define _SCHEDULER_RTS_H_

#include "scheduler.h"

class scheduler_rts: public scheduler {
	void schedule(processList*);
};

#endif