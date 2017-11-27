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
	start->print();
	*/

	processList* start = new processList(argv[1]);
	scheduler_rts* s = new scheduler_rts(start);

	while (s->tick());

	return 0;
}
