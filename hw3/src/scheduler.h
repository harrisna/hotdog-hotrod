#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "processList.h"

class scheduler {
	protected:
	processList* incoming;
	processNode* cpu;
	
	public:
	//virtual scheduler(processList*);
	virtual bool tick() {return 0;}
};

#endif
