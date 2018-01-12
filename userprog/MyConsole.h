#ifndef MY_CONSOLE_H
#define MY_CONSOLE_H

#include "copyright.h"
#include "console.h"

class MyConsole{

	Console* con;
	
	public:
	
	MyConsole();
	void ConsoleAcquire();
	void ConsoleRelease();
	char ConsoleGetChar();
	void ConsolePutChar(char c);
	
};

#endif // MY_CONSOLE_H
