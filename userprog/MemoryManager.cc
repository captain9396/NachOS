#include "copyright.h"
#include "MemoryManager.h"
#include "synch.h"


Lock* mem_lock;

MemoryManager :: MemoryManager(int numPages){
	mem_lock = new Lock("memory_manager_lock");	
	page_tracker = new BitMap(numPages);
}



// allocate a free page, return its physical
// page #. if can't allocate then return -1
int MemoryManager :: AllocPage(){
	mem_lock -> Acquire();
	
    int allocated_page = page_tracker -> Find();
    
    mem_lock -> Release();
    return allocated_page;
}


//free a page having page # = physPageNum
void MemoryManager :: FreePage(int physPageNum){
    mem_lock -> Acquire();
    
    page_tracker -> Clear(physPageNum);
    
    mem_lock -> Release();
    return;
}



// returns total # of unallocated 
// pages i.e. # of free pages
int MemoryManager :: GetUnallocatedPages(){
    mem_lock -> Acquire();
    
    int unallocated_pages = page_tracker -> NumClear();
    
    mem_lock -> Release();

    return unallocated_pages;
}



// return T/F if a page havimg page# = physPageNum
// is allocated or not
bool MemoryManager :: PageIsAllocated(int physPageNum){
	mem_lock -> Acquire();
	
    bool is_allocated =  page_tracker -> Test(physPageNum);
    
    mem_lock -> Release();

    return is_allocated;
}

























