#include <iostream>
#include <cstdio>
#include <cstring>
#include "processList.h"

processList::processList(char* fileName) {	
	// read list from file
	// TODO: error checking
	FILE* f = fopen(fileName, "r");

	processNode* last = NULL;
	processNode* prev = NULL;

	fscanf(f, "%*[^\n]\n", NULL);	// skip the header

	while(1) {
		if (last == NULL) {
			last = new processNode();
			head = last;
		} else {
			last->next = new processNode();
			prev = last;
			last = last->next;
		}

		if (fscanf(f, "%d\t%d\t%d\t%d\t%d\t%d\n", &last->pid, &last->burst, &last->arrival, &last->priority, &last->deadline, &last->io) == EOF) {
			// done reading, remove the invalid last node
			delete last;
			prev->next = NULL;
			last = prev;

			break;
		}
		last->next = NULL;

		// validate input TODO: actually validate
		bool invalid = false;

		if (invalid) {
			delete last;
			prev->next = NULL;
			last = prev;
		}
	}

	fclose(f);
}

void processList::print() {
	// debugging print of the whole process list
	processNode* node = head;

	while(node != NULL) {
		printf("pid %d, burst %d, arrival %d, priority %d, deadline %d, io %d\n", node->pid, node->burst, node->arrival, node->priority, node->deadline, node->io);
		node = node->next;
	}
}


