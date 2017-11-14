#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>

struct processNode {
	int pid;
	int burst;
	int arrival;
	int priority;
	int deadline;
	int io;

	processNode* next;
};

int main(int argc, char** argv) {
	FILE* f = fopen(argv[1], "r");

	char* s;
	processNode* pn = NULL;

	fscanf(f, "%*[^\n]\n", NULL);

	while(1) {
		if (pn == NULL)
			pn = new processNode();
		else {
			pn->next = new processNode();
			pn = pn->next;
		}

		if (fscanf(f, "%d\t%d\t%d\t%d\t%d\t%d\n", &pn->pid, &pn->burst, &pn->arrival, &pn->priority, &pn->deadline, &pn->io) == EOF)
			break;

		pn->next = NULL;

		printf("pid %d, burst %d, arrival %d, priority %d, deadline %d, io %d\n", pn->pid, pn->burst, pn->arrival, pn->priority, pn->deadline, pn->io);
	}

	processNode* node;
	while(node != NULL) {
		printf("pid %d, burst %d, arrival %d, priority %d, deadline %d, io %d\n", node->pid, node->burst, node->arrival, node->priority, node->deadline, node->io);
		node = node->next;
	}

	return 0;
}
