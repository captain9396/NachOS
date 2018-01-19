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






//----------------------------------------------------------------------
// ThreadTest
// 	Set up a ping-pong between two threads, by forking a thread
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------





void ThreadTest(){
    DEBUG('t', "Entering SimpleTest");

}
