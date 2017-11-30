#include "scheduler_mfqs.h"

#include <iostream>

#include "scheduler.h"

scheduler_mfqs::scheduler_mfqs(char* filename, int numQueues, int baseTimeQuantum, int agingThreshold)
	: scheduler(filename) {
	while (!incoming_unsorted.empty()) {
		process p = incoming_unsorted.front();
		incoming_unsorted.pop();
		
		bool invalid = false;

		invalid = invalid || p.burst < 1;
		invalid = invalid || p.arrival < 1;
		invalid = invalid || p.priority < 0;
		invalid = invalid || p.priority > 99;
		invalid = invalid || p.io < 0;

		if (!invalid)
			incoming.push(p);
	}

	this->numQueues = numQueues;
	timeQuantum = 0;
	this->baseTimeQuantum = baseTimeQuantum;
	this->agingThreshold = agingThreshold;
};

scheduler_mfqs::scheduler_mfqs(std::queue<process> procQueue, int numQueues, int baseTimeQuantum, int agingThreshold)
	: scheduler(procQueue) {
	while (!incoming_unsorted.empty()) {
		process p = incoming_unsorted.front();
		incoming_unsorted.pop();
		
		bool invalid = false;

		invalid = invalid || p.burst < 1;
		invalid = invalid || p.arrival < 1;
		invalid = invalid || p.priority < 0;
		invalid = invalid || p.priority > 99;
		invalid = invalid || p.io < 0;

		if (!invalid)
			incoming.push(p);
	}

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

	while (!wait.empty()) {
		process p = wait.top();
		if (p.ioStart + p.io == currentTick) {
			//printf("scheduling\n");
			//printf("TOP: %d\n", incoming.top().pid);
			queue[p.queue].push(wait.top());
			wait.pop();
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

			cpuGantt.pid = cpu.pid;
			cpuGantt.start = currentTick;

			queue[i].pop();
			//printf("CPU = %d\n", cpu.pid);
			cpuOccupied = true;
			cpu.queue = i;
			if (i == numQueues - 1) {
				timeQuantum = -1;
			}
			//printf("running queue %d\n", i);
		}
	}

	if (numQueues > 1 && !queue[numQueues-1].empty()) {
		/* because only one process can be demoted at a time, there are no two processes with the same age, so only check the first */
		process p = queue[numQueues-1].front();
		queue[numQueues-1].pop();

		if (p.queue > 0 && currentTick - p.waitStart > agingThreshold)
			p.queue--;	

		queue[p.queue].push(p);

	}

	if (cpuOccupied) {
		cpu.timeLeft--;
		if(timeQuantum > 0)
			timeQuantum--;
		
		// if the process is finished, get rid of it
		if (cpu.timeLeft == 0) {
			//printf("FINISHED %d AT %d\n", cpu.pid, currentTick);
			avgWait += currentTick - cpu.arrival - cpu.burst;
			avgTurnaround += currentTick - cpu.arrival;

			cpuGantt.end = currentTick;
			chart.push(cpuGantt);

			out.push(cpu);

			cpuOccupied = false;
			timeQuantum = 0;
		} else if (timeQuantum == 0) {
			// time quantum has expired, evict and demote
			//printf("EVICT TO %d\n", demoteQueue);
			cpuOccupied = false;

			cpuGantt.end = currentTick;
			chart.push(cpuGantt);

			cpu.waitStart = currentTick;
			cpu.queue++;
			queue[cpu.queue].push(cpu);
		} else if (cpu.io > 0 && timeQuantum == 1) {
			// do i/o
			cpuOccupied = false;

			cpuGantt.end = currentTick;
			chart.push(cpuGantt);

			cpu.ioStart = currentTick;
			if (cpu.queue != 0)
				cpu.queue--; 
			wait.push(cpu);
		}
	}

	//printf("TICK %d\n", currentTick);
	currentTick++;

	return (!cpuOccupied &&
		queue[0].empty() &&
		queue[1].empty() &&
		queue[2].empty() &&
		queue[3].empty() &&
		queue[4].empty() &&
		incoming.empty() &&
		wait.empty());
}
