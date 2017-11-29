#include "scheduler_mfqs.h"

#include <iostream>

#include "scheduler.h"
#include "processList.h"

scheduler_mfqs::scheduler_mfqs(char* filename, int numQueues, int baseTimeQuantum, int agingThreshold)
	: scheduler(filename) {
	this->numQueues = numQueues;
	timeQuantum = 0;
	this->baseTimeQuantum = baseTimeQuantum;
	this->agingThreshold = agingThreshold;
};

bool scheduler_mfqs::tick() {
	// place processes into the queue
	while (!incoming.empty()) {
		process p = incoming.top();
		if (p.arrival == currentTick) {
			//printf("scheduling\n");
			//printf("TOP: %d\n", incoming.top().pid);
			queue[0].push(incoming.top());
			incoming.pop();
		} else {
			break;
		}
	}

	if (!cpuOccupied) {
		// find first non-empty queue if possible
		timeQuantum = baseTimeQuantum;
		int i = 0;
		while (queue[i].empty() && i < numQueues) {
			i++;
			timeQuantum = (timeQuantum / 2) + (timeQuantum % 2);
		}

		if (i < numQueues) {
			cpu = queue[i].front();
			queue[i].pop();
			//printf("CPU = %d\n", cpu.pid);
			cpuOccupied = true;
			if (i < numQueues - 1) {
				demoteQueue = i + 1;
			} else {
				timeQuantum = -1;
				demoteQueue = numQueues - 1;
			}

			//printf("running queue %d\n", i);
		}
	}

	if (numQueues > 1 && !queue[numQueues-1].empty()) {
		/* because only one process can be demoted at a time, there are no two processes with the same age, so only check the first */
		process p = queue[numQueues-1].front();
		queue[numQueues-1].pop();

		if (currentTick - p.waitStart > agingThreshold) {
			queue[numQueues-2].push(p);
		} else {
			queue[numQueues-1].push(p);
		}
	}

	if (cpuOccupied) {
		cpu.timeLeft--;
		if(timeQuantum > 0)
			timeQuantum--;
		
		// if the process is finished, get rid of it
		if (cpu.timeLeft == 0) {
			//printf("FINISHED %d AT %d\n", cpu.pid, currentTick);
			out.push(cpu);
			cpuOccupied = false;
			timeQuantum = 0;
		} else if (timeQuantum == 0) {
			// time quantum has expired, evict and demote
			//printf("EVICT TO %d\n", demoteQueue);
			cpuOccupied = false;
			cpu.waitStart = currentTick;
			queue[demoteQueue].push(cpu);
			// TODO: demote
		}
	}

	//printf("TICK %d\n", currentTick);
	currentTick++;

	return (queue[0].empty() &&
		queue[1].empty() &&
		queue[2].empty() &&
		queue[3].empty() &&
		queue[4].empty() &&
		incoming.empty());
}
