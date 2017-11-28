#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "processList.h"

processList::processList() {
	head = NULL;
	tail = NULL;
	length = 0;
}

processList::processList(char* fileName) {	
	// read list from file
	// TODO: error checking
	head = NULL;
	tail = NULL;
	length = 0;
	
	FILE* f = fopen(fileName, "r");

	fscanf(f, "%*[^\n]\n", NULL);	// skip the header

	while(1) {
		if (tail == NULL) {
			tail = new processNode();
			tail->next = NULL;
			tail->prev = NULL;
			head = tail;
		} else {
			tail->next = new processNode();
			tail->next->prev = tail;
			tail = tail->next;
			tail->next = NULL;
		}

		if (fscanf(f, "%d\t%d\t%d\t%d\t%d\t%d\n", &tail->pid, &tail->burst, &tail->arrival, &tail->priority, &tail->deadline, &tail->io) == EOF) {
			// done reading, remove the invalid last node
			tail = tail->prev;
			delete tail->next;
			tail->next = NULL;

			break;
		}
		tail->timeLeft = tail->burst;
		tail->next = NULL;

		// validate input TODO: actually validate
		bool invalid = false;

		invalid = invalid || tail->burst < 1;
		invalid = invalid || tail->arrival < 1;
		//invalid = invalid || tail->priority < 0;
		//invalid = invalid || tail->io < 0;
		invalid = invalid || tail->deadline < tail->arrival;

		if (invalid) {
			printf("Invalid Process!!\n");
			printf("pid %d, burst %d, arrival %d, priority %d, deadline %d, io %d\n", tail->pid, tail->burst, tail->arrival, tail->priority, tail->deadline, tail->io);

			tail = tail->prev;
			delete tail->next;
			tail->next = NULL;
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
	if (p == NULL)
		return;

	p->next = NULL;
	p->prev = NULL;

	if (tail != NULL) {
		tail->next = p;
		p->prev = tail;
		tail = p;
	} else {
		head = p;
		tail = p;
	}

	length++;
}

processNode *processList::dequeue() {
	processNode *result = head;

	head = head->next;

	if (head == NULL)
		tail = NULL;
	else
		head->prev = NULL;

	result->prev = NULL;
	result->next = NULL;
	
	length--;

	return result;
}

void processList::print() {
	// debugging print of the whole process list
	processNode* node = head;

	while(node != NULL) {
		printf("pid %d, burst %d, arrival %d, priority %d, deadline %d, io %d\n", node->pid, node->burst, node->arrival, node->priority, node->deadline, node->io);
		node = node->next;
	}
	printf("take it back now ya'll\n");
	node = tail;
	while(node != NULL) {
		printf("pid %d, burst %d, arrival %d, priority %d, deadline %d, io %d\n", node->pid, node->burst, node->arrival, node->priority, node->deadline, node->io);
		node = node->prev;
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
		procArray[i]->prev = NULL;
	}

	// remove all nodes to sort
	processNode **node = &head;
	processNode *prevNode = NULL;
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
		top->prev = prevNode;
		tail = *node;
		prevNode = *node;
		node = &((*node)->next);
	}

	free(procArray);
}

void processList::sortByDeadline() {
	// heap sort by deadline
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
		procArray[i]->prev = NULL;
	}

	// remove all nodes to sort
	processNode **node = &head;
	processNode *prevNode = NULL;
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
		top->prev = prevNode;
		tail = *node;
		prevNode = *node;
		node = &((*node)->next);
	}

	free(procArray);
}
