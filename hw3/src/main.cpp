#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>

#include "processList.h"
#include "scheduler.h"
#include "scheduler_rts.h"

int main(int argc, char** argv) {
	
	/*
	processList* start = new processList(argv[1]);
	start->sortByArrival();
	*/
	processList* start = new processList(argv[1]);
	start->sortByArrival();

	scheduler_rts* s = new scheduler_rts(start);

	while (!s->tick());
	//	printf("tick");

	return 0;
}
