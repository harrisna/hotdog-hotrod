#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include "processList.h"

processList::processList(char* fileName) {	
	
	FILE* f = fopen(fileName, "r");

	processNode* pn = NULL;

	fscanf(f, "%*[^\n]\n", NULL);

	while(1) {
		if (pn == NULL) {
			pn = new processNode();
			head = pn;
		} else {
			pn->next = new processNode();
			pn = pn->next;
		}

		if (fscanf(f, "%d\t%d\t%d\t%d\t%d\t%d\n", &pn->pid, &pn->burst, &pn->arrival, &pn->priority, &pn->deadline, &pn->io) == EOF)
			break;

		pn->next = NULL;

		//printf("pid %d, burst %d, arrival %d, priority %d, deadline %d, io %d\n", pn->pid, pn->burst, pn->arrival, pn->priority, pn->deadline, pn->io);
	}
}

void processList::print() {
	processNode* node = head;
	while(node != NULL) {
		printf("pid %d, burst %d, arrival %d, priority %d, deadline %d, io %d\n", node->pid, node->burst, node->arrival, node->priority, node->deadline, node->io);
		node = node->next;
	}
}


