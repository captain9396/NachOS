#include "consumer.h"
#include "bits/stdc++.h"
#include "unistd.h"
#include "stdlib.h"
#include "thread.h"
#include "synch.h"
#include "system.h"
using namespace std;





extern int MAX;
extern queue<int> items;




Consumer::Consumer(char* debugName){
	name = debugName;
	
	
	
}

Consumer::~Consumer(){}

void Consumer:: consume(){
	if(!items.empty()){
		
		items.pop();
		cout << name << " has taken item from buffer  ----- BUFFER SIZE = "<< items.size() << endl;
		
	}
}

