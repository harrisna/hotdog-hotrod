#ifndef _PROCESSLIST_H_
#define _PROCESSLIST_H_

struct processNode {
	int pid;
	int burst;
	int arrival;
	int priority;
	int deadline;
	int io;

	int timeLeft;

	processNode *next;
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
	int length;
};

#endif
