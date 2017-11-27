#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "processList.h"

processList::processList() {
	head = NULL;
}

processList::processList(char* fileName) {	
	// read list from file
	// TODO: error checking
	length = 0;
	
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

		invalid = invalid || last->burst < 1;
		invalid = invalid || last->arrival < 1;
		invalid = invalid || last->priority < 0;
		invalid = invalid || last->io < 0;
		invalid = invalid || last->deadline < last->arrival;

		if (invalid) {
			printf("Invalid Process!!\n");
			printf("pid %d, burst %d, arrival %d, priority %d, deadline %d, io %d\n", last->pid, last->burst, last->arrival, last->priority, last->deadline, last->io);
			delete last;
			prev->next = NULL;
			last = prev;
		} else {
			length++;
		}
	}

	fclose(f);
}

processNode *processList::peek() {
	return head;
}

void processList::enqueue(processNode *p) {
	p->next = NULL;

	if (head != NULL) {
		processNode *node = head;

		while (node->next != NULL)
			node = node->next;

		node->next = p;
	} else {
		head = p;
	}
}

processNode *processList::dequeue() {
	processNode *result = head;
	head = head->next;
	return result;
}

void processList::print() {
	// debugging print of the whole process list
	processNode* node = head;

	while(node != NULL) {
		printf("pid %d, burst %d, arrival %d, priority %d, deadline %d, io %d\n", node->pid, node->burst, node->arrival, node->priority, node->deadline, node->io);
		node = node->next;
	}
}

#define parentNode(N) ((N-1)/2)
#define leftChild(N) ((2*N)+1)
#define rightChild(N) ((2*N)+2)

void processList::sortByArrival() {
	// heap sort by arrival time
	processNode **procArray = (processNode **) malloc(sizeof(processNode *) * length);

	// insert all nodes into heap
	processNode *current = head;
	for (int i = 0; i < length; i++) {
		procArray[i] = current;
		current = current->next;

		int ind = i;
		
		while(1) {
			if (ind == 0)
				break;
			
			if (procArray[ind]->arrival < procArray[parentNode(ind)]->arrival) {
				processNode *tmp = procArray[ind];
				procArray[ind] = procArray[parentNode(ind)];
				procArray[parentNode(ind)] = tmp;

				ind = parentNode(ind);
			} else {
				break;
			}
		}
	}

	// remove all references in list
	for (int i = 0; i < length; i++) {
		procArray[i]->next = NULL;
	}

	// remove all nodes to sort
	processNode **node = &head;
	int remlen = length;
	for (int i = 0; i < length; i++) {
		processNode *top = procArray[0];
		
		processNode *tmp = procArray[remlen - 1];
		procArray[0] = tmp;
		procArray[remlen - 1] = NULL;	// TODO: remove
		remlen--;

		int ind = 0;
		
		// sift down
		while(1) {
			if (leftChild(ind) < remlen && leftChild(ind) > 0) {
				int sc;
				if (rightChild(ind) < remlen && rightChild(ind) > 0) {
					if (procArray[leftChild(ind)]->arrival > procArray[rightChild(ind)]->arrival)
						sc = rightChild(ind);
					else
						sc = leftChild(ind);
				} else {
					sc = leftChild(ind);
				}

				if (procArray[ind]->arrival > procArray[sc]->arrival) {
					processNode *tmp = procArray[ind];
					procArray[ind] = procArray[sc];
					procArray[sc] = tmp;

					ind = sc;
				} else {
					break;
				}
			} else {
				break;
			}
		}
			
		*node = top;
		node = &((*node)->next);
	}

	free(procArray);
}

void processList::sortByDeadline() {
	// heap sort by arrival time
	processNode **procArray = (processNode **) malloc(sizeof(processNode *) * length);

	// insert all nodes into heap
	processNode *current = head;
	for (int i = 0; i < length; i++) {
		procArray[i] = current;
		current = current->next;

		int ind = i;
		
		while(1) {
			if (ind == 0)
				break;
			
			if (procArray[ind]->deadline < procArray[parentNode(ind)]->deadline) {
				processNode *tmp = procArray[ind];
				procArray[ind] = procArray[parentNode(ind)];
				procArray[parentNode(ind)] = tmp;

				ind = parentNode(ind);
			} else {
				break;
			}
		}
	}

	// remove all references in list
	for (int i = 0; i < length; i++) {
		procArray[i]->next = NULL;
	}

	// remove all nodes to sort
	processNode **node = &head;
	int remlen = length;
	for (int i = 0; i < length; i++) {
		processNode *top = procArray[0];
		
		processNode *tmp = procArray[remlen - 1];
		procArray[0] = tmp;
		procArray[remlen - 1] = NULL;	// TODO: remove
		remlen--;

		int ind = 0;
		
		// sift down
		while(1) {
			if (leftChild(ind) < remlen && leftChild(ind) > 0) {
				int sc;
				if (rightChild(ind) < remlen && rightChild(ind) > 0) {
					if (procArray[leftChild(ind)]->deadline > procArray[rightChild(ind)]->deadline)
						sc = rightChild(ind);
					else
						sc = leftChild(ind);
				} else {
					sc = leftChild(ind);
				}

				if (procArray[ind]->deadline > procArray[sc]->deadline) {
					processNode *tmp = procArray[ind];
					procArray[ind] = procArray[sc];
					procArray[sc] = tmp;

					ind = sc;
				} else {
					break;
				}
			} else {
				break;
			}
		}
			
		*node = top;
		node = &((*node)->next);
	}

	free(procArray);
}
