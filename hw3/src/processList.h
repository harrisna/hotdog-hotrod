#ifndef _PROCESSLIST_H_
#define _PROCESSLIST_H_

struct processNode {
	int pid;
	int burst;
	int arrival;
	int priority;
	int deadline;
	int io;

	processNode* next;
};

class processList {
	public:
	processList(char* fileName);
	
	void sort();
	
	void print();
	
	processNode* head;
};

#endif