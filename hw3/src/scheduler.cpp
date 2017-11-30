#include "scheduler.h"

scheduler::scheduler(char* filename) {
	FILE* f = fopen(filename, "r");

	fscanf(f, "%*[^\n]\n", NULL);	// skip header
	
	while(1) {
		process p;
		if (fscanf(f, "%d\t%d\t%d\t%d\t%d\t\t%d\n", &p.pid, &p.burst, &p.arrival, &p.priority, &p.deadline, &p.io) == EOF) {
			break;
		}
		
		p.timeLeft = p.burst;
	}

	cpuOccupied = false;
}
