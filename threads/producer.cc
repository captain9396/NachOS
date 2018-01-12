#include "producer.h"
#include "Abul.h"
#include "bits/stdc++.h"
#include "unistd.h"
#include "system.h"
#include "stdlib.h"
#include "thread.h"
#include "synch.h"
using namespace std;

extern int MAX;
extern queue<int> items;


Producer::Producer(char* debugName){
	name = debugName;
	
}

Producer::~Producer(){}

void Producer:: produce(){
	items.push(0);
	cout << name << " has added item to the buffer ----- BUFFER SIZE = "<< items.size() << endl;
}


