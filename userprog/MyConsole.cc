#include "copyright.h"
#include "MyConsole.h"
#include "synch.h"



Lock* con_lock;
Lock* read_lock;
Lock* write_lock;

void readAvail(void* arg);
void writeDone(void* arg);


MyConsole :: MyConsole(){
	// instantiate locks
	con_lock = new Lock("console_lock");
	read_lock = new Lock("read_lock");
	write_lock = new Lock("write_lock");
	
	// instantiate console object
	con = new Console(NULL, NULL, readAvail, writeDone, 0);
	
}
void MyConsole :: ConsoleAcquire(){
	con_lock -> Acquire();
}
void MyConsole :: ConsoleRelease(){
	con_lock -> Release();
}
char MyConsole :: ConsoleGetChar(){
	read_lock -> Acquire();
	char in = con -> GetChar();
	return in;
}
void MyConsole :: ConsolePutChar(char c){
	con -> PutChar(c);
	write_lock -> Acquire();
}





void readAvail(void* arg){
	read_lock -> Release();
}
void writeDone(void* arg){
	write_lock -> Release();
}

