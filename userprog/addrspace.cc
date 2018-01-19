

#include "copyright.h"
#include "system.h"
#include "addrspace.h"

//#include "noff.h"


extern MemoryManager* memory_manager;


int minimum(int a, int b){
	if(a < b) return a;
	else return b;
}

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
				
    }



}

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
    machine->WriteRegister(PCReg, 0);

    machine->WriteRegister(NextPCReg, 4);

    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %d\n", numPages * PageSize - 16);
}


void AddrSpace::SaveState()
{}


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













