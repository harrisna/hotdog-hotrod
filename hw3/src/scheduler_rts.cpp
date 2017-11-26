#include "scheduler_rts.h"
#include "scheduler.h"
#include "processList.h"


scheduler_rts::scheduler(processList* processList) {
	ps = processList;
};

void scheduler_rts::tick() {
	currentTick++;
	
	while (1) {
		processNode *p = ps->peek();
		if (p->arrival == currentTick) {
			p = ps->dequeue();
			// TODO: check if deadline is reasonable, if not, get rid of p
			queue->enqueue(p);
		} else {
			break;
		}
	}

	queue->sortByDeadline();

	processNode *p = queue->peek();
	// TODO: have "cpu" var that holds current process in cpu
	p->timeLeft--;

	if (p->timeLeft == 0)
		queue->dequeue();
}
