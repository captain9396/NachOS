#include "copyright.h"
#include "MemoryManager.h"
#include "synch.h"
#include "bits/stdc++.h"
#define RANDOM 1
#define LRU 2


const int policy = 2;



Lock* mem_lock;





MemoryManager :: MemoryManager(int numPages){
	mem_lock = new Lock("memory_manager_lock");	
	page_tracker = new BitMap(numPages);
	pages = new Frame[numPages];
	total_page = numPages;
//	swap = (SwapPage*)malloc(sizeof(SwapPage) * 256);
	//init
	for(int i = 0 ; i <numPages; i++){
		pages[i].processno = -1;
		pages[i].entry = NULL;	
		pages[i].time_stamp = 0;
		
	}
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




int MemoryManager :: Alloc(int processNo, TranslationEntry* entry){
	mem_lock -> Acquire();
	int allocated_page = page_tracker -> Find();
	pages[allocated_page].processno = processNo;
	pages[allocated_page].entry = entry;
	
//	printf("@@@@@@@@@@ TIMER TICK = %d\n", pages[allocated_page].entry->time);
    mem_lock -> Release();
    
    
    return allocated_page;
}





int MemoryManager :: AllocByForce(){
	mem_lock -> Acquire();
	
	int ret;
	
	
	if(	policy == LRU){
		int max = pages[0].entry -> time;
		ret = 0;
		for(int i = 0 ; i < total_page ; i++){
			if(pages[i].entry -> time < max && pages[i].entry->dirty == false){
				max = pages[i].entry -> time;
				ret = i;
			}
		}
		printf("~~~~~~~~~OOPS, FRAME %d GOT EVICTED WITH TIME %d (LRU)\n", ret, max);
	}
	else if(policy == RANDOM){
		ret = rand() % 	total_page;
		printf("~~~~~~~~~OOPS, FRAME %d GOT EVICTED WITH TIME %d\n", ret, pages[ret].entry -> time);
	}
    mem_lock -> Release();

    //
    return ret;
}




















