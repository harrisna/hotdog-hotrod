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
	int ioStart;
	int waitStart;

	int queue;
};

struct procCmpArrivalRTS {
	bool operator()(process const &a, process const &b) {
		if (a.arrival == b.arrival) {
			if (a.deadline == b.deadline)
				return a.burst > b.burst;
			else
				return a.deadline > b.deadline;
		} else {
			return a.arrival > b.arrival;
		}
	}
};

struct procCmpArrivalMFQS {
	bool operator()(process const &a, process const &b) {
		if (a.arrival == b.arrival) {
			if (a.priority == b.priority)
				return a.pid > b.pid;
			else
				return a.priority > b.priority;
		} else {
			return a.arrival > b.arrival;
		}
	}
};

struct procCmpArrivalWHS {
	bool operator()(process const &a, process const &b) {
		if (a.arrival == b.arrival) {
			if (a.priority == b.priority)
				return a.pid > b.pid;
			else
				return a.priority < b.priority;
		} else {
			return a.arrival > b.arrival;
		}
	}
};

struct procCmpIOMFQS {
	bool operator()(process const &a, process const &b) {
		if (a.ioStart == b.ioStart) {
			if (a.priority == b.priority)
				return a.pid > b.pid;
			else
				return a.priority > b.priority;
		} else {
			return a.arrival > b.arrival;
		}
	}
};

struct procCmpIOWHS {
	bool operator()(process const &a, process const &b) {
		if (a.ioStart == b.ioStart) {
			if (a.priority == b.priority)
				return a.pid > b.pid;
			else
				return a.priority < b.priority;
		} else {
			return a.arrival > b.arrival;
		}
	}
};

struct procCmpDeadline {
	bool operator()(process const &a, process const &b) {
		//return a.deadline > b.deadline;
		if (a.deadline == b.deadline) {
			if (a.timeLeft == b.timeLeft)
				return a.arrival > b.arrival;
			else
				return a.timeLeft > b.timeLeft;
		} else {
			return a.deadline > b.deadline;
		}
	}
};

struct procCmpPriority {
	bool operator()(process const &a, process const &b) {
		return a.priority > b.priority;
	}
};

#endif
