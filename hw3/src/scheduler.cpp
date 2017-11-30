#include "scheduler.h"

#include <vector>
#include <queue>

scheduler::scheduler(char* filename) {
	FILE* f = fopen(filename, "r");

	fscanf(f, "%*[^\n]\n", NULL);	// skip header
	
	while(1) {
		process p;
		if (fscanf(f, "%d\t%d\t%d\t%d\t%d\t\t%d\n", &p.pid, &p.burst, &p.arrival, &p.priority, &p.deadline, &p.io) == EOF) {
			break;
		}
		
		p.timeLeft = p.burst;

		incoming_unsorted.push(p);
	}

	cpuOccupied = false;
	avgWait = 0;
	avgTurnaround = 0;
}

scheduler::scheduler(std::queue<process> procQueue) {
	incoming_unsorted = procQueue;

	cpuOccupied = false;
	avgWait = 0;
	avgTurnaround = 0;
}

void scheduler::printStats() {
	printf("\nHistory:");
	while (!chart.empty()) {
		printf("\npid: %d\nenters: %d\nleaves %d\n", chart.front().pid, chart.front().start, chart.front().end);
		chart.pop();
	}
	printf("\nStatistics:\n");
	printf("%lu Processes Scheduled in %d ticks\n", out.size(), currentTick);
	printf("Average wait time: %f ticks\n", avgWait / (double) out.size());
	printf("Average turnaround time: %f ticks\n", avgTurnaround / (double) out.size());
}
