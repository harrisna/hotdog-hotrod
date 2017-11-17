#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include "processList.h"

int main(int argc, char** argv) {
	
	processList* start = new processList(argv[1]);
	start->print();

	return 0;
}
