

#ifndef MEMMANAGER_H
#define MEMMANAGER_H

#include "copyright.h"
#include "bitmap.h"


//-----------------------------------------------------------------------
// 'MemoryManager' class is actually a 
// class which is keeping track of the
// physical pages residing in main memory
// whether a page is allocated to a user 
// process of being unallocated 
// It is allocating and unallocating pages too
//-----------------------------------------------------------------------



class MemoryManager{

	BitMap *page_tracker;


	public:
		
	MemoryManager(int numPages);
	
	int  GetUnallocatedPages();
	
	int AllocPage();
	
	void FreePage(int physPageNum);
	
	bool PageIsAllocated(int physPageNum);
	
	
	
	
};


#endif

