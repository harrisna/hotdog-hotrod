#include "scheduler_rts.h"
#include "scheduler.h"
#include "processList.h"


scheduler_rts::scheduler(processList* processList) {
	ps = processList;
};

void scheduler_rts::tick() {
	currentTick++;
	
	while (1) {
		processNode *p = incoming->peek();
		if (p->arrival == currentTick) {
			p = incoming->dequeue();
			// TODO: check if deadline is reasonable, if not, get rid of p
			queue->enqueue(p);
		} else {
			break;
		}
	}

	queue->sortByDeadline();

	cpu = queue->peek();
	// TODO: have "cpu" var that holds current process in cpu
	cpu->timeLeft--;
	
	if (cpu->timeLeft == 0) {
		queue->dequeue();
	}
}
