#include "thread.h"
#include "synch.h"



class Producer{
	public:
	Producer(char* debugName);
	~Producer();
	char* getName(){ return name; }
	void setName(char* debugName) { name = debugName;}
	Thread* getThread(){ return producerThread;}
	
	void produce();

	
	private:
	char* name;
	Thread* producerThread;
};

