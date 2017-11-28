#include "scheduler_rts.h"

#include <iostream>

#include "scheduler.h"
#include "processList.h"


scheduler_rts::scheduler_rts(processList* pl) {
	pl->sortByArrival();
	currentTick = 0;
	incoming = pl;
	queue = new processList();
	cpu = NULL;
};

bool scheduler_rts::tick() {
	currentTick++;
	
	// place processes into the queue
	while (1) {
		processNode *p = incoming->peek();
		if (p == NULL) {
			break;
		} else if (p->arrival == currentTick) {
			queue->enqueue(incoming->dequeue());
		} else {
			break;
		}
	}

	if (queue->peek() != NULL) {
		//queue->sortByDeadline();

		// fail processes that have exceeded their deadline
		/*
		while (queue->peek() != NULL && queue->peek()->deadline < currentTick) {
			processNode *p = queue->dequeue();	// TODO: yell something
			printf("OOPS\n");
			delete p;
		}
		*/

		if (cpu == NULL || (queue->peek() != NULL && queue->peek()->deadline < cpu->deadline)) {
			cpu = queue->peek();
		}
	}

	if (cpu != NULL) {
		cpu->timeLeft--;
		
		// if the process is finished, get rid of it
		if (cpu->timeLeft == 0) {
			//printf("FUCK %d\n", currentTick);
			queue->dequeue();
			// add to out list
			cpu = NULL;
		}
	}

	return (queue->peek() == NULL && incoming->peek() == NULL);
}
