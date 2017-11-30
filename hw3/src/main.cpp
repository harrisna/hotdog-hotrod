#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <chrono>
#include <vector>
#include <queue>

#include "process.h"
#include "scheduler.h"
#include "scheduler_rts.h"
#include "scheduler_mfqs.h"
#include "scheduler_whs.h"

int main(int argc, char** argv) {
	scheduler *s;
	std::queue<process> procQueue;
	int number;
	int numQueues, timeQuantum, ageTimer;

	if (argc != 2) {
		process p;
		p.pid = 0;
		while (1) {
			p.pid++;
			printf("Enter burst for pid %d (enter 0 to finish entering processes):\n", p.pid);
			fscanf(stdin, "%d", &p.burst);
			if (p.burst == 0)
				break;
			printf("Enter arrival for pid %d:\n", p.pid);
			fscanf(stdin, "%d", &p.arrival);
			printf("Enter priority for pid %d:\n", p.pid);
			fscanf(stdin, "%d", &p.priority);
			printf("Enter deadline for pid %d:\n", p.pid);
			fscanf(stdin, "%d", &p.deadline);
			printf("Enter io for pid %d:\n", p.pid);
			fscanf(stdin, "%d", &p.io);

			p.timeLeft = p.burst;

			procQueue.push(p);
		}
	}

	printf("Select Scheduler:\n 1. MFQS \n 2. RTS \n 3. WHS \n");
	fscanf(stdin, "%d", &number);
	switch (number) {
	case 1:
		printf("Enter number of queues\n");
		fscanf(stdin, "%d", &numQueues);

		printf("Enter time quantum\n");
		fscanf(stdin, "%d", &timeQuantum);

		printf("Enter aging timer\n");
		fscanf(stdin, "%d", &ageTimer);
	
		if (argc == 2)	
			s = new scheduler_mfqs(argv[1], numQueues, timeQuantum, ageTimer);
		else
			s = new scheduler_mfqs(procQueue, numQueues, timeQuantum, ageTimer);

		break;
	case 2:
		int soft;
		
		printf("Enter 0 for soft RTS, 1 for hard RTS\n");
		fscanf(stdin, "%d", &soft);

		if (argc == 2)
			s = new scheduler_rts(argv[1], soft == 0);
		else
			s = new scheduler_rts(procQueue, soft == 0);

		break;
	case 3:
		printf("Enter time quantum\n");
		fscanf(stdin, "%d", &timeQuantum);

		printf("Enter aging timer\n");
		fscanf(stdin, "%d", &ageTimer);
		
		if (argc == 2)
			s = new scheduler_whs(argv[1], timeQuantum, ageTimer);
		else
			s = new scheduler_whs(procQueue, timeQuantum, ageTimer);

		break;
	}

	std::chrono::high_resolution_clock::time_point start, end;

	start = std::chrono::high_resolution_clock::now();
	
	while (!s->tick());

	end = std::chrono::high_resolution_clock::now();
	
	s->printStats();

	std::chrono::duration<double> time = std::chrono::duration_cast<std::chrono::duration<double> >(end - start);
	printf("Scheduling completed in %f seconds\n", time.count());

	return 0;
}
