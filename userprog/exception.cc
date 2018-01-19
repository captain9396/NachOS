// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "syscall.h"
#include "synch.h"
#include "MyConsole.h"


// Syscall function prototypes

void PageFaultSyscall();

void ExecSyscall();

void ExitSyscall();

void ReadSyscall();

void WriteSyscall();

void HaltSyscall();


extern Table* process_table;

void do_task(void* arg);



//---------------------------------------------------------------------
// EXACTLY FROM BLUE SLIDE GIVEN
// Updates program counter after
// an instruction is fetched
//---------------------------------------------------------------------

void UpdatePCReg(){
	int pc = machine -> ReadRegister(PCReg);
	machine -> WriteRegister(PrevPCReg,pc);
	pc = machine -> ReadRegister(NextPCReg);
	machine -> WriteRegister(PCReg,pc);
	pc += 4;
	machine -> WriteRegister(NextPCReg,pc);
}





extern MyConsole* my_console;
extern MemoryManager* memory_manager;

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    
    
	if((which == SyscallException)){
   		if ((which == SyscallException) && (type == SC_Halt)) {
			DEBUG('a', "Shutdown, initiated by user program.\n");
			printf("< HALT SYSTEM CALL >\n");	
		   	interrupt->Halt();
		}
		else if((which == SyscallException) && (type == SC_Exit)){
	  
			printf("< EXIT SYSTEM CALL >\n");
			ExitSyscall();
		}
		
		else if((which == SyscallException) && (type == SC_Exec)){
		 	printf("-----------------------------------\n");
			printf("|       EXECUTE SYSTEM CALL       |\n");
			printf("-----------------------------------\n\n\n");
			
			ExecSyscall();
		}
		else if((which == SyscallException) && (type == SC_Read)){
			ReadSyscall();
		}
		else if((which == SyscallException) && (type == SC_Write)){
			WriteSyscall();
		}
		UpdatePCReg();
    }
    
    
    
    
    
 	else if(which == PageFaultException){
		PageFaultSyscall();
    	//ExitSyscall();
    }
    
    else if(which == ReadOnlyException){
    	printf("ronly\n");
    	ExitSyscall();
    }
    
    else if(which == BusErrorException){
    	printf("berror\n");
    	ExitSyscall();
    }
    
    else if(which == OverflowException){
    	printf("overflo\n");
    	ExitSyscall();
    }
    
    else if(which == IllegalInstrException){
    	printf("ill\n");
    	ExitSyscall();
    }
    
    else if(which == NumExceptionTypes){
    	printf("numexep\n");
    	ExitSyscall();
    }
    
    else{
		//printf("Unexpected user mode exception %d %d\n", which, type);
		ExitSyscall();
	//	ASSERT(false);	    
    }
 
    
    
    
    // update PC

}







void ExecSyscall(){

	
		
	int address_buffer = machine->ReadRegister(4);
	char name[100]; // buffer contating file name
	int i = 0, process_id;

//	bool ReadMem(int addr, int size, int* value);
// Read or write "size" bytes of virtual memory (at addr).  Return false if a correct translation couldn't be found.

	//reading FILE NAME directly from main memory whose physical address was
	// passed as argument in the Exec() system call
	bool name_too_long = false;
	for(i = 0;; i++){
		machine->ReadMem(address_buffer + i, 1, (int*)&name[i]);
		if(name[i] == NULL) 
			break;
		if(i > 100){
			name_too_long = true; 
			break;		
		}
	}
	
	if(name_too_long){
		printf("FILE NAME TOO LONG -- more than 100 characters are not allowed\n");
		machine->WriteRegister(2, -1);
        return;	
	}

    
    
    
    
    //fetch file from file system
    OpenFile* file = fileSystem->Open(name);
    
   
    if(file == NULL){
        printf("Unable to open file %s\n", name);
        
        machine->WriteRegister(2, -1);
        return;
    }
    
    
    
    // start a new thread executing the specified program
    //printf(" HERE -- > %s\n", currentThread->getName());
	Thread* new_thread = new Thread(name);
    process_id = process_table -> Alloc((void*) new_thread);
	

	if(process_id == -1){
		printf("process allocation failed\n");
		return ;
	}
	// create a new pageTable i.e. create a new process address space for the specified process
    AddrSpace* new_process_space = new AddrSpace(file, process_id);
	    
    // set the addresspace of the running thread 
    // equal to the address space of the specified process
    new_thread -> space = new_process_space;
   // new_thread -> space -> InitRegisters();
   // new_thread -> space -> RestoreState();
    

    
    
    // if no allocation in process table is possible then
    if(process_id == -1){
        printf("Cannot allocate the process\n" );
        machine->WriteRegister(2, -1);
        return;
    }
    
    
    
    new_thread -> setId(process_id);

    
    
	//machine->WriteRegister(2, process_id);
    
	
    new_thread -> Fork(do_task, new_thread);
    
    
	currentThread -> Yield();
    free(file);
    
	return ;
	
}




void ExitSyscall(){
	
	int position_in_process_table;
	int arg1 = machine->ReadRegister(4);
	//printf("===============> EXIT ARGUMENT VALUE =  %d <================\n\n", arg1);

   	
   	
   	
   	// releasing current threads pages
	AddrSpace* cur_space = currentThread -> space;
	cur_space -> ReleasePages();
	free(cur_space);	
	
	

	int total_process = process_table -> GetTotalProcess();
	printf("$$$$$$$$$ ECHO TOTAL_PROCESSES = %d\n", total_process);
	
    if(total_process > 1){
    	process_table -> Release(currentThread -> getId());
    	currentThread -> Finish();
	}
    else{
		interrupt -> Halt();
    }
    
    return ;
}




void ReadSyscall(){


	unsigned int addr = machine->ReadRegister(4);
    unsigned int size = machine->ReadRegister(5);

    my_console->GetLock();

    char* buffer = new char[size];

    for(int i = 0; i < size; i++)
    {
        buffer[i] = my_console->GetChar();
    }
    buffer[size] = '\0';
    for(int i = 0; i < size; i++)
    {
        machine->WriteMem(addr + i,1, (int)buffer[i]);
    }

    DEBUG('z', "Size of string = %d\n", size);
    machine->WriteRegister(2,size);


    DEBUG('z', "got :  %s\n", buffer);

    bzero(buffer, sizeof(char) * size);

    my_console->ReleaseLock();

    return;
}





void WriteSyscall(){
	unsigned int addr = machine->ReadRegister(4);	
    unsigned int size = machine->ReadRegister(5);

    my_console->GetLock();
    for(int i = 0; i < size; i++)
    {
        int c;
        machine->ReadMem(addr + i, 1, &c);
        my_console->PutChar((char)c);
    }
    my_console->PutChar('\n');

    my_console->ReleaseLock();

    return;
}





void HaltSyscall(){

	interrupt->Halt();
    return;
    
}



void PageFaultSyscall(){
	printf("A PAGEFAULT OCCURRED\n");
	
	int addr = machine -> ReadRegister(39); // contains bad register
	printf("!!!ECHO BAD REGISTER = %d\n", addr);
	
	int vpn = addr / PageSize;
	
	int physicalPageNo;
	
	if(memory_manager -> GetUnallocatedPages()){
		//physicalPageNo = memory_manager -> AllocPage();  // normal case task 1
		physicalPageNo = memory_manager -> Alloc(currentThread->getId(), &(machine->pageTable[vpn])); // RANDOM REP
		
		
	}else{
		// will be handled later
		physicalPageNo = memory_manager -> AllocByForce();
		
		//do swapfile case later
	}
	
	printf("ECHO physicalPageNo %d\n", physicalPageNo);
	currentThread -> space -> loadIntoFreePage(addr, physicalPageNo);
	stats -> numPageFaults++;	
}







//-------------------------
















void do_task(void* arg){

	
	AddrSpace* cur_space = currentThread -> space;
	cur_space -> InitRegisters();
	cur_space -> RestoreState();
	machine->Run();
	
	
	//printf("doing task\n");
	

	return;
}






