

#ifndef MEMMANAGER_H
#define MEMMANAGER_H

#include "copyright.h"
#include "translate.h"
#include "bitmap.h"
#include "SwapPage.h"
#include <map>


//-----------------------------------------------------------------------
// 'MemoryManager' class is actually a 
// class which is keeping track of the
// physical pages residing in main memory
// whether a page is allocated to a user 
// process of being unallocated 
// It is allocating and unallocating pages too
//-----------------------------------------------------------------------



struct Frame{
	int processno;
	TranslationEntry* entry;
	int time_stamp;

	
	bool operator<(const Frame& f)const{
	return time_stamp < f.time_stamp;}
};




class MemoryManager{
	
	BitMap *page_tracker;
	SwapPage* swap;
	Frame* pages;
	int total_page;

	public:
		
	MemoryManager(int numPages);
	
	int  GetUnallocatedPages();
	
	int AllocPage();
	
	void FreePage(int physPageNum);
	
	bool PageIsAllocated(int physPageNum);
	
	int Alloc(int processNo, TranslationEntry* entry);
	int AllocByForce();
	
	
	
};






#endif

