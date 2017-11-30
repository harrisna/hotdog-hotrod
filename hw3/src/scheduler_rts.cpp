#include "scheduler_rts.h"

#include <iostream>

#include "scheduler.h"

scheduler_rts::scheduler_rts(char* filename, bool soft)
	: scheduler(filename) {
	while (!incoming_unsorted.empty()) {
		process p = incoming_unsorted.front();
		incoming_unsorted.pop();

		bool invalid = false;
	
		invalid = invalid || p.burst < 1;
		invalid = invalid || p.arrival < 1;
		invalid = invalid || p.deadline <= p.arrival;
		
		if (!invalid)
			incoming.push(p);
	}
	this->soft = soft;
};

scheduler_rts::scheduler_rts(std::queue<process> procQueue, bool soft)
	: scheduler(procQueue) {
	while (!incoming_unsorted.empty()) {
		process p = incoming_unsorted.front();
		incoming_unsorted.pop();

		bool invalid = false;
	
		invalid = invalid || p.burst < 1;
		invalid = invalid || p.arrival < 1;
		invalid = invalid || p.deadline <= p.arrival;
		
		if (!invalid)
			incoming.push(p);
	}
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
		// fail processes that have exceeded their deadline
		if (!soft) {
			while (!queue.empty() && queue.top().deadline < currentTick) {
				queue.pop();
				printf("pid %d passed deadline!  Exiting...\n", queue.top().pid);
				return 1;
			}
		}

		if (!queue.empty() && (!cpuOccupied || queue.top().deadline < cpu.deadline)) {
			if (cpuOccupied) {
				cpuGantt.end = currentTick;
				chart.push(cpuGantt);

				queue.push(cpu);
				printf("EVICT %d FOR %d\n", cpu.pid, queue.top().pid);
			}
			cpu = queue.top();

			cpuGantt.pid = cpu.pid;
			cpuGantt.start = currentTick;

			queue.pop();
			//printf("CPU = %d\n", cpu.pid);
			cpuOccupied = true;
		}
	}

	if (cpuOccupied) {
		cpu.timeLeft--;
		
		// if the process is finished, get rid of it
		if (cpu.timeLeft == 0) {
			//printf("FINISHED %d\n", currentTick);
			avgWait += (currentTick + 1) - cpu.arrival - cpu.burst;
			avgTurnaround += (currentTick + 1) - cpu.arrival;

			cpuGantt.end = currentTick + 1;
			chart.push(cpuGantt);

			out.push(cpu);

			cpuOccupied = false;
		}
	}

	//printf("TICK %d\n", currentTick);
	currentTick++;

	return (queue.empty() && incoming.empty());
}
