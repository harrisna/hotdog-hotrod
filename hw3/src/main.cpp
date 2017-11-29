#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>

#include "scheduler.h"
#include "scheduler_rts.h"
#include "scheduler_mfqs.h"

int main(int argc, char** argv) {
	
	/*
	processList* start = new processList(argv[1]);
	start->sortByArrival();
	*/
	//processList* start = new processList(argv[1]);
	//start->sortByArrival();
	scheduler *s;
	int number;
	printf("Select Scheduler:\n 1. MFQS \n 2. RTS \n");
	fscanf(stdin, "%d", &number);
	switch (number) {
	case 1:
		int numQueues, timeQuantum, ageTimer;

		printf("Enter number of queues\n");
		fscanf(stdin, "%d", &numQueues);

		printf("Enter time quantum\n");
		fscanf(stdin, "%d", &timeQuantum);

		printf("Enter aging timer\n");
		fscanf(stdin, "%d", &ageTimer);
		
		s = new scheduler_mfqs(argv[1], 5, 16, 64);
		break;
	case 2:
		int soft;
		
		printf("Enter 0 for soft RTS, 1 for hard RTS\n");
		fscanf(stdin, "%d", &soft);

		s = new scheduler_rts(argv[1], soft == 0);
		break;
	}

	while (!s->tick());

	return 0;
}
