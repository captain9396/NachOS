#include "thread.h"
#include "synch.h"




class Consumer{
	public:
	Consumer(char* debugName);
	~Consumer();
	char* getName(){ return name; }
	void setName(char* debugName) { name = debugName;}
	Thread* getThread(){ return consumerThread;}
	
	void consume();
	
	
	
	private:
	char* name;
	Thread* consumerThread;
};

