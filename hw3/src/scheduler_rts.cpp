#include "scheduler_rts.h"

#include <iostream>

#include "scheduler.h"
#include "processList.h"


scheduler_rts::scheduler_rts(processList* pl) {
	incoming = pl;
	queue = new processList();
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
		queue->sortByDeadline();

		// fail processes that have exceeded their deadline
		while (queue->peek()->deadline < currentTick) {
			processNode *p = queue->dequeue();	// TODO: yell something
			delete p;
		}
	}

	if (cpu != NULL) {
		cpu = queue->peek();
		cpu->timeLeft--;
		
		// if the process is finished, get rid of it
		if (cpu->timeLeft == 0) {
			queue->dequeue();
			// add to out list
		}
	}

	return (queue->peek() == NULL);
}
