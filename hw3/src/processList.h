#ifndef _PROCESSLIST_H_
#define _PROCESSLIST_H_

#include <iostream>

struct processNode {
	int pid;
	int burst;
	int arrival;
	int priority;
	int deadline;
	int io;

	int timeLeft;

	processNode *next;
	processNode *prev;
};

class processList {
	public:
	processList();
	processList(char *fileName);

	processNode *peek();
	void enqueue(processNode *p);
	processNode *dequeue();
	
	void sortByArrival();
	void sortByDeadline();
	void print();
	
	private:
	processNode *head;
	processNode *tail;
	int length;
};

#endif
