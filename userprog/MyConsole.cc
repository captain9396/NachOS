#include "copyright.h"
#include "MyConsole.h"
#include "synch.h"

Lock* consoleLock    = new Lock("Console Lock");
Semaphore* readAvail = new Semaphore("read avail", 0);
Semaphore* writeDone = new Semaphore("write done", 0);

void ReadAvail(void* arg)
{
    readAvail->V();
}
void WriteDone(void* arg)
{
    writeDone->V();
}


MyConsole::MyConsole()
{
    //console = new Console(in.txt, out.txt, readAvail, writeDone, 0);
    console = new Console(NULL, NULL, ReadAvail, WriteDone, 0);
    // NULL, NULL ==> stdin, stdout
}

void MyConsole::GetLock()
{
    consoleLock->Acquire();
}

void MyConsole::ReleaseLock()
{
    consoleLock->Release();
}

char MyConsole::GetChar()
{
    readAvail->P();
    return console->GetChar();
}

void MyConsole::PutChar(char ch)
{
    console->PutChar(ch);
    writeDone->P();
}
