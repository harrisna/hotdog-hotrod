#include "scheduler_whs.h"

#include <iostream>

#include "scheduler.h"

scheduler_whs::scheduler_whs(char* filename, int baseTimeQuantum, int agingThreshold)
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

	timeQuantum = 0;
	this->baseTimeQuantum = baseTimeQuantum;
	this->agingThreshold = agingThreshold;
};

scheduler_whs::scheduler_whs(std::queue<process> procQueue, int baseTimeQuantum, int agingThreshold)
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

	timeQuantum = 0;
	this->baseTimeQuantum = baseTimeQuantum;
	this->agingThreshold = agingThreshold;
};

bool scheduler_whs::tick() {
	// place processes into the queue
	while (!incoming.empty()) {
		process p = incoming.top();
		if (p.arrival == currentTick) {
			//printf("scheduling\n");
			//printf("TOP: %d\n", incoming.top().pid);
			queue[incoming.top().priority].push(incoming.top());
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
		int i = 99;
		while (queue[i].empty() && i >= 0) {
			i--;
		}

		if (i >= 0) {
			cpu = queue[i].front();
			cpuGantt.pid = cpu.pid;
			cpuGantt.start = currentTick;
			queue[i].pop();
			//printf("CPU = %d\n", cpu.pid);
			cpuOccupied = true;
			cpu.queue = i;

			//printf("running queue %d\n", i);
		}
	}

	for (int i = 0; i < 10; i++) {
		while (!queue[i].empty() && currentTick - queue[i].front().waitStart > agingThreshold) {
			process p = queue[i].front();
			queue[i].pop();
			p.priority++;
			queue[p.priority].push(p);
		}
	}

	if (cpuOccupied) {
		cpu.timeLeft--;
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
			if (cpu.priority != 50 && cpu.priority != 0) {
				cpu.priority--;
			}
			queue[cpu.priority].push(cpu);
		} else if (cpu.io > 0 && timeQuantum == 1) {
			// do i/o
			cpuOccupied = false;

			cpuGantt.end = currentTick;
			chart.push(cpuGantt);

			cpu.ioStart = currentTick;
			if ((cpu.priority < 50 - cpu.io && cpu.priority > 0) && (cpu.priority > 50 && cpu.priority < 100 - cpu.io)) {
				cpu.priority += cpu.io; 
			}
			wait.push(cpu);
		}
	} 

	//printf("TICK %d\n", currentTick);
	currentTick++;

	bool queuesEmpty = true;
	
	for(int i = 0; i < 100; i++) {
		queuesEmpty = queuesEmpty && queue[i].empty();
	}

	return (!cpuOccupied &&
		queuesEmpty &&
		incoming.empty() && 
		wait.empty());
}
