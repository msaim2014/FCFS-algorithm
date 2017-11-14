#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

//*****************************************************************************
//P R O G R A M  H E A D E R
//  Name:  Muhammad Saim
//	Z#:  23306519
//	Instructor:  Dr.Tami Sorgente
//	Class:  Computer Operating System
//	Term:  Fall 2017
//	Scheduling Algorithm Project
//	Due Date:   November 13, 2017
//	Due Time:  11:59PM
//	Points:  40
//	
//  Description: This program simulates First Come First Serve(FCFS) scheduling
//		algorithm where the program runs the first process in the ready queue.
//		after the process is run, it will go into the IO queue.
//******************************************************************************

class process
{
public:
	string name;
	int CPUBurstAndIO[100];
	int currentCPUBurst;
	int currentIO;
	int responseTime;
	int waitTime;
	int turnAroundTime;
	process *previous;
	process *next;
};

class Algorithm
{
public:
	Algorithm();
	void initializeQueue(process initial[], int elements);
	void sendToQueue(process current);
	process current();
	process last();
	void print(process current, int time);
	void printReadyQueue();
	void printIOQueue();
	void deQueue();
	void deQueueSpecific(process transfer);
	void adjustQueue(int IO);
	void readyQueuePrint();
	void IOQueuePrint();
	bool isEmpty();
	void reduce();
	bool transfer();
	process getFinishIO();
	bool isCompleted(process transfer, int time);
	bool hasOne();
	void printFinished();
	void computeWaitTime(process initial[]);
	void setResponseTime(int time, process now);
	void printCompletedQueue();

private:
	process *front;
	process *back;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: Algorithm()
//Description: This is the default constructor that is used to initialize the doubly linked list.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
Algorithm::Algorithm() {
	front = 0;
	back = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: initializeQueue
//Description: This function is used to create a linked list from an array of processes.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::initializeQueue(process readyQueue[],int elements) {
	int i=0;
	//getting the size of the array
	int array[] = { 1,2,3,4,5,6,7,8 };
	int num2 = sizeof(array) / sizeof(*array);
	int num = sizeof(readyQueue)/sizeof(*readyQueue);
	process *created;
	do{
		//creating the intial node
		if (isEmpty()) {
			created = new process;
			created->name = readyQueue[i].name;
			for (int j = 0; readyQueue[i].CPUBurstAndIO[j]!=0; j++) {
				created->CPUBurstAndIO[j] = readyQueue[i].CPUBurstAndIO[j];
			}
			created->currentCPUBurst = readyQueue[i].currentCPUBurst;
			created->currentIO = readyQueue[i].currentIO;
			created->responseTime = readyQueue[i].responseTime;
			created->waitTime = readyQueue[i].waitTime;
			created->turnAroundTime = readyQueue[i].turnAroundTime;
			front = back = created;
			i++;
		}
		//attaching the nodes to the end
		else {
			created = new process;
			back->next = created;
			created->previous = back;
			back = created;
			created->name = readyQueue[i].name;
			for (int j = 0; readyQueue[i].CPUBurstAndIO[j]!=0; j++) {
				created->CPUBurstAndIO[j] = readyQueue[i].CPUBurstAndIO[j];
			}
			created->currentCPUBurst = readyQueue[i].currentCPUBurst;
			created->currentIO = readyQueue[i].currentIO;
			created->responseTime = readyQueue[i].responseTime;
			created->waitTime = readyQueue[i].waitTime;
			created->turnAroundTime = readyQueue[i].turnAroundTime;
			i++;
		}
	} while (i < elements);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: deQueue
//Description: This function deletes the front of the queue.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::deQueue() {
	if (isEmpty()) {
		return;
	}
	else if (front->next == 0) {
		delete front;
		front = 0;
		back = 0;
	}
	else {
		process *temp = front;
		front = front->next;
		front->previous = 0;
		delete temp;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: current
//Description: This function returns the first element of the queue. 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
process Algorithm::current() {
	process IDLE{ "IDLE"};
	//returns what the algorithm should run next
	if (isEmpty()) {
		return IDLE;
	}
	process *p = front;
	return *p;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: last
//Description: This function returns the last element of the queue.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
process Algorithm::last() {
	process *p = back;
	return *p;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: sendToQueue
//Description: This function sends a process to the back of the queue
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::sendToQueue(process current) {
	process *created;
	//creating the intial node
	if (isEmpty()) {
		created = new process;
		created->name = current.name;
		for (int j = 0; current.CPUBurstAndIO[j] != 0; j++) {
			created->CPUBurstAndIO[j] = current.CPUBurstAndIO[j];
		}
		created->currentCPUBurst = current.currentCPUBurst;
		created->currentIO = current.currentIO;
		created->responseTime = current.responseTime;
		created->waitTime = current.waitTime;
		created->turnAroundTime = current.turnAroundTime;
		front = back = created;
	}
	//attaching the node to the back
	else {
		created = new process;
		back->next = created;
		created->previous = back;
		back = created;
		created->name = current.name;
		for (int j = 0; current.CPUBurstAndIO[j] != 0; j++) {
			created->CPUBurstAndIO[j] = current.CPUBurstAndIO[j];
		}
		created->currentCPUBurst = current.currentCPUBurst;
		created->currentIO = current.currentIO;
		created->responseTime = current.responseTime;
		created->waitTime = current.waitTime;
		created->turnAroundTime = current.turnAroundTime;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: adjustQueue
//Description: This function sets the current IO and determines what the current CPU burst is.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::adjustQueue(int IO) {
	process *point = back;
	//updating the current IO and CPU burst
	point->currentIO = IO;
	point->currentCPUBurst = point->currentCPUBurst + 2;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: print
//Description: This function prints the current time and name of the process
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::print(process current, int time) {
	cout << "Current Time: "<< time << endl;
	cout << "Now Running: " << current.name << endl;
	cout << "---------------------------------------------------------" << endl;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: printReadyQueue
//Description: This function sets up the display for printing the ready queue.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::printReadyQueue() {
		cout << "Ready Queue: Process Burst: " << endl;
		readyQueuePrint();
		cout << endl;
		cout << "---------------------------------------------------------" << endl;

	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: printIOQueue
//Description: This function sets up the display for printing the IO queue.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::printIOQueue() {
	cout << "Now in I/O:  Process  Remaining I/O Time" << endl;
	IOQueuePrint();
	cout << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: readyQueuePrint
//Description: This function prints the readyQueue. It informs the user of the process name and CPU burst.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::readyQueuePrint() {
	process *i = front;
	int p = 0;
	if (isEmpty()) {
		cout << "             [Empty]" << endl;
	}
	else {
		//printing what's in the ready queue
		for (i = front; i != back->next; i = i->next) {
			cout << "               " << i->name << "      " << i->CPUBurstAndIO[i->currentCPUBurst] << endl;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: IOQueuePrint
//Description:This function prints the IOQueue. It informs the user of the process name and current IO
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::IOQueuePrint() {
	process *i = front;
	int p = 0;
	if (front == 0) {
		cout << "             [Empty]" << endl;
	}
	else {
		//printing what's in the IO queue
		for (i = front; i != back->next; i = i->next) {
			cout << "               " << i->name << "      " << i->currentIO << endl;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: reduce
//Description: This function reduces the current IO of all the process in IO Queue by one.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::reduce() {
	if (front == 0) {
		return;
	}
	//decrementing the current IO of the front node in the IO queue by one
	else if (front->next == 0) {
		front->currentIO--;
	}
	else {
		//decrementing the current IO of all processes in the IO queue by one
		for (process *p = front; p != 0; p = p->next) {
			p->currentIO--;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: isEmpty
//Description: This function is used to see if the linked list has nodes in it.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Algorithm::isEmpty() {
	if (front == 0) {
		return true;
	}
	else {
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: transfer
//Description: This function returns true if there is a process in IO queue that has completed it's IO time.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Algorithm::transfer() {
	process *p = front;
	if (isEmpty()==false) {
		//checks through the list to find a finished IO
		for (p = front; p != 0; p = p->next) {
			if (p->currentIO == 0) {
				return true;
			}
		}
		return false;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: getFinishIO
//Description: This function returns the process in IO queue that has finished it's IO time.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
process Algorithm::getFinishIO() {
	for (process *p = front; p != 0; p = p->next) {
		if (p->currentIO == 0) {
			return *p;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: deQueueSpecific
//Description: This function deletes a specific element from the linked list.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::deQueueSpecific(process transfer) {
	process *p = front;
	if (isEmpty()) {
		return;
	}
	//deleting the front
	else if (front->next == 0) {
		delete p;
		front = 0;
	}
	else {
		//finding the node to delete
		for (p = front; p != 0; p = p->next) {
			//if it's in the front
			if (p==front && p->name == transfer.name) {
				process *q = p->next;
				q->previous = 0;
				front = q;
				delete p;
				p = q;
				break;
			}
			//if it's in the back
			else if (p==back && p->name == transfer.name) {
				process *q = p->previous;
				q->next = 0;
				back = q;
				delete p;
				p = q;
				break;
			}
			//if it's in the middle
			else if(p->name==transfer.name){
				process *q = p->previous;
				q->next = p->next;
				q->next->previous = q;
				delete p;
				p = q;
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: isCompleted
//Description:  This function returns true if a process has completed all the bursts. Then it sets the
//				turnaround time.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Algorithm::isCompleted(process transfer, int time) {
	process *p = front;
	if (transfer.name == "IDLE") {
		return true;
	}
	//check to see if the last burst has been done
	else if (transfer.CPUBurstAndIO[transfer.currentCPUBurst + 1] == 0) {
		while (p->name != transfer.name) {
			p = p->next;
		}
		//sets the turnaround time
		p->turnAroundTime = time;
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: hasOne
//Description: This function returns true if there is only one node in the linked list.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Algorithm::hasOne() {
	if (front != 0 && front->next == 0) {
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: printFinished
//Description: This function calculates the average Wait, Response, and Turnaround times. Then it prints 
//			   all the Wait, Response, and Turnaround times for each process with the averages 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::printFinished() {
	double avgWait = 0;
	double avgResponse = 0;
	double avgTurnaround = 0;
	//calculates the averages for wait, response, and turnaround times
	for (process *q = front; q != 0; q = q->next) {
		avgWait = avgWait + q->waitTime;
		avgResponse = avgResponse + q->responseTime;
		avgTurnaround = avgTurnaround + q->turnAroundTime;
	}
	avgWait = avgWait / 8;
	avgResponse = avgResponse / 8;
	avgTurnaround = avgTurnaround / 8;
	//prints the wait, turnaround, and response times for each process along with the averages
	cout << "Process      Wait Time   Turnaround Time   Response Time" << endl;
	for (process *p = front; p != 0; p = p->next) {
		cout << left << setw(17) << p->name << setw(14) << p->waitTime << setw(14) << p->turnAroundTime << p->responseTime << endl;
	}
	cout << "Averages         " << left << setw(14) << avgWait << setw(14) << avgTurnaround << avgResponse << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: computeWaitTime
//Description: This function totals all the IO and Burst times and subtracts if from the turnaround time
//				to get the wait time.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::computeWaitTime(process initial[]) {
	int totalBurstAndIO = 0;
	int burstOrIO = 0;
	//calculates the total for all the bursts and IOs
	for (process *i = front; i != back->next; i = i->next) {
		totalBurstAndIO = 0;
		for (int j = 0; i->CPUBurstAndIO[j] != 0; j++) {
			burstOrIO = i->CPUBurstAndIO[j];
			totalBurstAndIO = totalBurstAndIO + burstOrIO;
		}
		//sets the wait time
		i->waitTime = i->turnAroundTime - totalBurstAndIO;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: setResponeTime
//Description: This function sets the response time to the current time if it has not already been set.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::setResponseTime(int time, process now) {
	process *p = front->next;
	while (p != 0 && p->name != now.name) {
		p = p->next;
	}
	if (p == 0) {
		return;
	}
	//sets the response time if it hasn't already been set
	if (p->responseTime == 0) {
		p->responseTime = time;
	}
	else {
		return;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name: printCompletedQueue
//Description: This function prints all the processes that have been completed. 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Algorithm::printCompletedQueue() {
	if (front == 0) {
		cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
		return;
	}
	//prints all the completed processes in order
	else {
		cout << "---------------------------------------------------------" << endl;
		cout << "Completed: ";
		for (process *p = front; p != 0; p = p->next) {
			cout << p->name << " ";
		}
	}
	cout << endl << endl << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
}

int main() {
	//These are the different queues used
	Algorithm ReadyQueue;
	Algorithm IOQueue;
	Algorithm Finished;
	Algorithm Completed;
	process IO;
	int time = 0;

	//hard coding the initial values
	process p1{ "P1",{ 4, 15, 5, 31, 6, 26, 7, 24, 6, 41, 4, 51, 5, 16, 4}, 0, 0 };
	process p2{ "P2",{ 9, 28, 11, 22, 15, 21, 12, 28, 8, 34, 11, 34, 9, 29, 10, 31, 7}, 0, 0 };
	process p3{ "P3",{ 24, 28, 12, 21, 6, 27, 17, 21, 11, 54, 22, 31, 18}, 0, 0 };
	process	p4{ "P4",{ 15, 35, 14, 41, 16, 45, 18, 51, 14, 61, 13, 54, 16, 61, 15}, 0, 0 };
	process p5{ "P5",{ 6, 22, 5, 21, 15, 31, 4, 26, 7, 31, 4, 18, 6, 21, 10, 33, 3}, 0, 0 };
	process p6{ "P6",{ 22, 38, 27, 41, 25, 29, 11, 26, 19, 32, 18, 22, 6, 26, 6}, 0, 0 };
	process p7{ "P7",{ 4, 36, 7, 31, 6, 32, 5, 41, 4, 42, 7, 39, 6, 33, 5, 34, 6, 21, 9}, 0, 0 };
	process p8{ "P8",{ 5, 14, 4, 33, 6, 31, 4, 31, 6, 27, 5, 21, 4, 19, 6, 11, 6}, 0, 0 };
	
	//initializing some queues
	process initial[] = { p1, p2, p3, p4, p5, p6, p7, p8 };
	int elements = sizeof(initial) / sizeof(*initial);
	ReadyQueue.initializeQueue(initial, elements);
	Finished.initializeQueue(initial, elements);

	//setting the needed variables
	process now = ReadyQueue.current();
	process previous = ReadyQueue.current();
	process transfer;

	int j = 0;
	int endTime = 0;
	//keeps going until the ready and IO queue are emtpy
	while (ReadyQueue.isEmpty()!=true || IOQueue.isEmpty()!=true) {
		//keeps track of the burst being decremented
		for (int i = ReadyQueue.current().CPUBurstAndIO[j]; i >= 0; i--) {
			//start of the algorithm P1
			if (time == 0) {
				cout << "FIRST COME FIRST SERVE" << endl;
				endTime = ReadyQueue.current().CPUBurstAndIO[j];
				previous = ReadyQueue.current();
				ReadyQueue.deQueue();
				ReadyQueue.print(now, time);
				ReadyQueue.printReadyQueue();
				IOQueue.printIOQueue();
				cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
				now = ReadyQueue.current();
				time++;
			}
			else {
				//stops at the context switch
				if (time == endTime) {
					//the process is not completed yet
					if (Finished.isCompleted(previous, time) == false) {
						IOQueue.sendToQueue(previous);
						j = IOQueue.last().currentCPUBurst;
						IOQueue.adjustQueue(IOQueue.last().CPUBurstAndIO[j + 1]);
					}
					else {
						//the process is completed
						if (now.name != "IDLE") {
							Completed.sendToQueue(now);
						}
					}
					now = ReadyQueue.current();
					//This breaks out of the loop to finish the program
					if (now.name == "IDLE" && ReadyQueue.isEmpty() == true && IOQueue.isEmpty() == true) {
						cout << "Current Time: " << time << endl;
						cout << "DONE!" << endl << endl;
						Completed.printCompletedQueue();
						break;
					}
					Finished.setResponseTime(time, now);
					ReadyQueue.print(now, time);
					previous = ReadyQueue.current();
					ReadyQueue.deQueue();
					ReadyQueue.printReadyQueue();
					IOQueue.printIOQueue();
					Completed.printCompletedQueue();
					endTime = time + now.CPUBurstAndIO[now.currentCPUBurst];
					i = now.CPUBurstAndIO[now.currentCPUBurst];
				}
				time++;
				IOQueue.reduce();
				//forever loop to find the processes in IO queue that have 
				//completed their IO time and transfers them to ready queue
				for (;;) {
					if (IOQueue.transfer() == true) {
						transfer = IOQueue.getFinishIO();
						IOQueue.deQueueSpecific(transfer);
						ReadyQueue.sendToQueue(transfer);
						if (ReadyQueue.hasOne()&& i==0) {
							if (now.name == "IDLE") {
								endTime = time;
							}
							else {
								endTime = time + transfer.CPUBurstAndIO[transfer.currentCPUBurst];
							}	
						}
					}
					else {
						break;
					}
				}
			}
		}
	}
	//calculates the CPU utilization
	double utilization = 0;
	for (int i=0; i<=elements-1; i++)
		for (int j = 0; initial[i].CPUBurstAndIO[j] > 0; j=j+2) {
			utilization = initial[i].CPUBurstAndIO[j] + utilization;
		}

	//prints the final results of the simulation
	utilization = (utilization / (time-1)) * 100;
	cout << "CPU utilization is: " << utilization << endl;
	Finished.computeWaitTime(initial);
	Finished.printFinished();

	return 0;
}