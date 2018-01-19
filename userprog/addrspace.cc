// addrspace.cc
//	Routines to manage address spaces (executing user programs).
//
//	In order to run a user program, you must:
//
//	1. link with the -N -T 0 option
//	2. run coff2noff to convert the object file to Nachos format
//		(Nachos object code format is essentially just a simpler
//		version of the UNIX executable object code format)
//	3. load the NOFF file into the Nachos file system
//		(if you haven't implemented the file system yet, you
//		don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"

//#include "noff.h"


extern MemoryManager* memory_manager;


int minimum(int a, int b){
	if(a < b) return a;
	else return b;
}

//----------------------------------------------------------------------
// SwapHeader
// 	Do little endian to big endian conversion on the bytes in the
//	object file header, in case the file was generated on a little
//	endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader *noffH)
{
	noffH->noffMagic = WordToHost(noffH->noffMagic);
	noffH->code.size = WordToHost(noffH->code.size);
	noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
	noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
	noffH->initData.size = WordToHost(noffH->initData.size);
	noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
	noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
	noffH->uninitData.size = WordToHost(noffH->uninitData.size);
	noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
	noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}




bool check_valid_code_and_data(unsigned int a, unsigned int b){
	return (a > 0 || b > 0);
}



//----------------------------------------------------------------------
// AddrSpace::AddrSpace
// 	Create an address space to run a user program.
//	Load the program from a file "executable", and set everything
//	up so that we can start executing user instructions.
//
//	Assumes that the object code file is in NOFF format.
//
//	First, set up the translation from program memory to physical
//	memory.  For now, this is really simple (1:1), since we are
//	only uniprogramming, and we have a single unsegmented page table
//
//	"executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace(OpenFile *executable, int space_id)
{
    NoffHeader noffH;
    unsigned int i, size, total_page;

	


	// reading contents of noff header which
	// are located at the first sizeof(noffh)
	// bytes of OpenFile "executable"
	this -> executable = executable;
	this -> space_id = space_id;
    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
    
    
    my_noffH = &noffH;

    if ((noffH.noffMagic != NOFFMAGIC) &&
		(WordToHost(noffH.noffMagic) == NOFFMAGIC))
    	SwapHeader(&noffH);
    ASSERT(noffH.noffMagic == NOFFMAGIC);

// how big is address space?
    size = noffH.code.size
    	   + noffH.initData.size
		   + noffH.uninitData.size
		   + UserStackSize;	// we need to increase the size
						// to leave room for the stack
						
	Size = size;
    numPages = divRoundUp(size, PageSize);
    size = numPages * PageSize;
    
    



	//    ASSERT(numPages <= NumPhysPages);		// check we're not trying
						// to run anything too big --
						// at least until we have
						// virtual memory
	/*if(memory_manager -> GetUnallocatedPages() < numPages){
		printf("Memory Insufficient! Cannot Allocate Page.\n");
	}*/

    DEBUG('a', "Initializing address space, num pages %d, size %d\n",
				numPages, size);

	swapPage = new SwapPage();

// first, set up the translation
    pageTable = new TranslationEntry[numPages];

    for (i = 0; i < numPages; i++) {
		pageTable[i].virtualPage = i;
		pageTable[i].physicalPage = -1;
//		pageTable[i].valid = true;
		pageTable[i].valid = false; // causing a pageFaultException deliberately
		pageTable[i].use = false;
		pageTable[i].dirty = false;
		pageTable[i].readOnly = false;  // if the code segment was entirely on
						// a separate page, we could set its
						// pages to be read-only
    }







// looping through each of the virtual
// pages and zeroing that page
// get a pointer to the physical page
// by mapping the virtual page to the
// physical page and zero the physical page
// using the pointer to it

//	let's say you want to
// reference page 3 (the fourth page since page numbers start at 0).
// Page 3 has a "physical address" of "3 * PageSize", and therefore
// begins at machine->mainMemory[3 * PageSize]. To get a pointer to Page
// 3, you would take its address: &machine->mainMemory[3 * PageSize].
/*
    for( i = 0; i < numPages; i++){
		bzero(&(machine -> mainMemory[pageTable[i].physicalPage * PageSize]), PageSize);
	}





	//
	total_page = divRoundUp((noffH.initData.size + noffH.code.size), PageSize);


*/



/*	GIVEN PSEUDO-CODE
	----------------------
	while (still code (or data) in executable file) {
		file_off = determine current offset into executable file;
		virt_addr = determine the virtual address that the code gets loaded at;
		phys_addr = convert the virt_addr to a physical address (a pointer
		into machine->mainMemory) using your page table;
		size = PageSize;
		executable->ReadAt(phys_addr, size, file_off)
	}

	here offset = noffH.code.inFileAddr*/


/*
	// checking if the program being loaded has valid code and data sections
	if(check_valid_code_and_data(noffH.initData.size,noffH.code.size)){

	// looping through all pages to read contents of file 'executable' into main memory page by page
        for (int i = 0; i < total_page; i++){
	        int file_offset = noffH.code.inFileAddr + (i * PageSize);
        	char* physical_address = &(machine -> mainMemory[pageTable[i].physicalPage * PageSize]);

    	    executable -> ReadAt( physical_address, PageSize, file_offset);
        }
	}
	*/


/*
	// PREVIOUS SCHEME WHICH CANNOT BE USED
	// AS WE ARE CREATING MULTIPROGRAMMING
	// SUPPORT


	then, copy in the code and data segments into memory
    if (noffH.code.size > 0) {
        DEBUG('a', "Initializing code segment, at 0x%x, size %d\n",
			noffH.code.virtualAddr, noffH.code.size);
        executable->ReadAt(&(machine->mainMemory[noffH.code.virtualAddr]),
			noffH.code.size, noffH.code.inFileAddr);
    }
    if (noffH.initData.size > 0) {
        DEBUG('a', "Initializing data segment, at 0x%x, size %d\n",
			noffH.initData.virtualAddr, noffH.initData.size);
        executable->ReadAt(&(machine->mainMemory[noffH.initData.virtualAddr]),
			noffH.initData.size, noffH.initData.inFileAddr);
    }
*/

}


//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
// 	Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace()
{
   delete  pageTable;
   delete executable;
  	delete swapPage;			// close file
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
// 	Set the initial values for the user-level register set.
//
// 	We write these directly into the "machine" registers, so
//	that we can immediately jump to user code.  Note that these
//	will be saved/restored into the currentThread->userRegisters
//	when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister(i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister(PCReg, 0);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister(NextPCReg, 4);

   // Set the stack register to the end of the address space, where we
   // allocated the stack; but subtract off a bit, to make sure we don't
   // accidentally reference off the end!
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %d\n", numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
// 	On a context switch, save any machine state, specific
//	to this address space, that needs saving.
//
//	For now, nothing!
//----------------------------------------------------------------------

void AddrSpace::SaveState()
{}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
// 	On a context switch, restore the machine state so that
//	this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}






// release unused pages
void AddrSpace :: ReleasePages(){
	int i;
	for (i = 0; i < numPages; i++){
		if(pageTable[i].valid){ // freeing only valid page entries
			memory_manager -> FreePage(pageTable[i].physicalPage);
		}
	}
}




void AddrSpace :: loadIntoFreePage(int addr, int physicalPageNo){
	int vpn= addr/PageSize;
	
 	int codeoffset;
 	int codeSize;
 	
 	printf("ECHO ADDR %d\n", addr);
	printf("ECHO ADDR %d\n", 	my_noffH -> code.virtualAddr);
 	printf("ECHO SIZE %d\n", Size);
 	//If addr is between code.virtualBaseAddress to code.virtualBaseAddress+size
 	if((addr >= my_noffH -> code.virtualAddr) && (addr <= my_noffH -> code.virtualAddr + Size)){
 		//printf("ECHO ROMIO 5\n");
 		//Find Codeoffset, means how many pages addr from the virtual base
		codeoffset = (addr - my_noffH->code.virtualAddr) / PageSize;
		printf("ECHO CODEOFFSET %d\n", codeoffset);
		
		
		//Find how many bytes can actually we read from the code segment
		int x = my_noffH->code.virtualAddr + Size;
		int y = codeoffset * PageSize;
		int z = x - y;
		codeSize = minimum(z, PageSize);
		printf("ECHO HOW MANY BYTES WE CAN READ FROM CODE SEGMENT = %d\n", codeSize);
 	}
 	// cases have to be put here
 	
 	
 	
	char* physical_address = &(machine->mainMemory[physicalPageNo * PageSize]);
	int file_offset = my_noffH->code.inFileAddr + vpn*PageSize ;
 	executable->ReadAt( physical_address, PageSize, file_offset);
 	pageTable[vpn].physicalPage = physicalPageNo;
 	pageTable[vpn].valid = true;
}


void AddrSpace :: saveIntoSwapSpace(int vpn){


}
void AddrSpace :: loadFromSwapSpace(int vpn){

}
bool AddrSpace :: isSwapPageExists(int vpn){

}













