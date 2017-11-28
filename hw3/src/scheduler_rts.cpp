#include "scheduler_rts.h"

#include <iostream>

#include "scheduler.h"
#include "processList.h"

scheduler_rts::scheduler_rts(char* filename, bool soft)
	: scheduler(filename) {
	this->soft = soft;
};

bool scheduler_rts::tick() {
	// place processes into the queue
	while (!incoming.empty()) {
		process p = incoming.top();
		if (p.arrival == currentTick) {
			//printf("scheduling\n");
			queue.push(incoming.top());
			incoming.pop();
		} else {
			break;
		}
	}

	if (!queue.empty()) {
		//queue->sortByDeadline();

		// fail processes that have exceeded their deadline
		if (!soft) {
			while (!queue.empty() && queue.top().deadline < currentTick) {
				queue.pop();
				printf("OOPS\n");
			}
		}

		if (!cpuOccupied || (!queue.empty() && queue.top().deadline < cpu.deadline)) {
			//printf("CPU\n");
			cpu = queue.top();
			cpuOccupied = true;
		}
	}

	if (cpuOccupied) {
		cpu.timeLeft--;
		
		// if the process is finished, get rid of it
		if (cpu.timeLeft == 0) {
			//printf("FINISHED %d\n", currentTick);
			out.push(queue.top());
			queue.pop();
			cpuOccupied = false;
		}
	}

	//printf("TICK %d\n", currentTick);
	currentTick++;

	return (queue.empty() && incoming.empty());
}
