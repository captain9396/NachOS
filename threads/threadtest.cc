       // threadtest.cc
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield,
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "synch.h"
#include "unistd.h"
#include "stdlib.h"
#include "consumer.h"
#include "producer.h"
#include "bits/stdc++.h"
#define SLEEPTIME 0
#define DELAY 1000000000
using namespace std;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread
//	each iteration.
//
//	"arg" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------





// RUN WITH /.nachos -rs seed    -------------> for random wake up of threads



extern queue<int> items;
extern int MAX;


Condition* producer_condition = new Condition("producer");
Condition* consumer_condition = new Condition("consumer");
Lock* lock = new Lock("myLock");



string to_string(int x){
	string val = "";
	do{ val = ((char)((x%10) + '0')) + val; x/=10; }while(x);
	return val;
}



void simpleProduce(void* arg){
	while(true){
		Producer producer = *((Producer*)arg);
		if(items.size() == MAX){
			cout << producer.getName()<< " cannot add to buffer : buffer FULL" << endl;
//			sleep(SLEEPTIME);
			for(int i = 0 ; i < DELAY; i++); // delay
			producer_condition -> Wait(lock);
		}


		lock->Acquire();
		producer.produce();

		for(int i = 0 ; i < DELAY; i++); // delay

		lock->Release();

		if(items.size() == 1){
			consumer_condition->Broadcast(lock);
		}
		currentThread -> Yield();
	}
}



void simpleConsume(void* arg){
	while(true){
		Consumer consumer = *((Consumer*)arg);

		if(items.size() == 0){
			cout << consumer.getName()<< " cannot take from buffer : buffer EMPTY" << endl;
			//sleep(SLEEPTIME);
			for(int i = 0 ; i < DELAY; i++); // delay
			consumer_condition -> Wait(lock);
		}


		lock->Acquire();
		consumer.consume();
		//sleep(SLEEPTIME);
		for(int i = 0 ; i < DELAY; i++); // delay
		lock->Release();

		if (items.size() == MAX - 1){
            producer_condition -> Broadcast(lock);
        }
		currentThread -> Yield();
	}
}


void do_tas(void* arg){
	
	printf("CURRENT THREAD = %s\n", currentThread -> getName());
	return ;
}






//----------------------------------------------------------------------
// ThreadTest
// 	Set up a ping-pong between two threads, by forking a thread
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------





void ThreadTest(){
    DEBUG('t', "Entering SimpleTest");

	int no_of_producers;
	int no_of_consumers;


    cout << "Enter # of Producers: " ;
    cin >> no_of_producers;
    cout << "Enter # of Consumers: " ;
    cin >> no_of_consumers;

	cout << "Maximum size of BUFFER: ";
	cin >> MAX;

	cout << "\n\n --------------------------------------------------------" << endl << endl;




   	for(int i = 0 ; i < no_of_producers; i++){
   		string name = "Producer " + to_string(i+1);
   		Thread* t1 = new Thread("test");
   		Producer* producer = new Producer(strdup(name.c_str()));
   		t1->Fork((VoidFunctionPtr) simpleProduce, producer);

   	}


	for(int i = 0 ; i < no_of_consumers; i++){
		string name = "Consumer " + to_string(i+1);
		Thread* t2 = new Thread("test");
   		Consumer* consumer = new Consumer(strdup(name.c_str()));

   		t2->Fork((VoidFunctionPtr) simpleConsume, consumer);
   	}
   	
   	
   	
   	/*Thread* t = new Thread("first");
   	Thread* t2 = new Thread("second");
   	printf("current thread = %s\n", currentThread -> getName());
   	t2->Fork(do_tas, NULL);
	t->Fork(do_tas, NULL);*/


}
