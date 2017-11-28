#ifndef _PROCESS_H_
#define _PROCESS_H_

struct process {
	int pid;
	int burst;
	int arrival;
	int priority;
	int deadline;
	int io;

	int timeLeft;
};

struct procCmpArrival {
	bool operator()(process const &a, process const &b) {
		return a.arrival > b.arrival;
	}
};

struct procCmpDeadline {
	bool operator()(process const &a, process const &b) {
		return a.deadline > b.deadline;
	}
};

#endif
