#ifndef MY_CONSOLE_H
#define MY_CONSOLE_H

#include "copyright.h"
#include "console.h"

class MyConsole
{
private:
    Console *console;
public:
    MyConsole();

    void GetLock();

    void ReleaseLock();

    char GetChar();

    void PutChar(char ch);
};

#endif // MY_CONSOLE_H
