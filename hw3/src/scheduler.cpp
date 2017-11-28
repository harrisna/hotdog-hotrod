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
		
		bool invalid = false;

		invalid = invalid || p.burst < 1;
		invalid = invalid || p.arrival < 1;
		//invalid = invalid || p.priority < 0;
		//invalid = invalid || p.io < 0;
		invalid = invalid || p.deadline < p.arrival;

		if (invalid) {
			
		} else {
			incoming.push(p);
		}
	}

	cpuOccupied = false;
}
