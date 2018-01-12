#include "copyright.h"
#include "Table.h"
#include "synch.h"


Lock* process_lock;

/* Create a table to hold at most "size" entries. */
Table :: Table(int size){
	int i;
	table_size = size;
	total_process = 0;
	process = new void*[size];
	process_lock = new Lock("process_lock");
	
	//printf("# SIZE OF PROCESS TABLE IS = %d\n", size);
	
	for(i = 0; i < table_size; i++){
		process[i] = NULL;
	}
}



	
	
/* Allocate a table slot for "object", returning the "index" of the
	allocated entry; otherwise, return -1 if no free slots are available. */
int Table :: Alloc(void *object){
	process_lock -> Acquire();
	
	int i, index = -1;
	for(i = 0; i < table_size; i++){
		if(process[i] == NULL){
			process[i] = object;
			index = i;
			total_process++;
			break;
		}
	}
	
	process_lock -> Release();
	
	return index;
}
	
	
	
	
/* Retrieve the object from table slot at "index", or NULL if that
	slot has not been allocated. */
void* Table ::  Get(int index){
	process_lock -> Acquire();
	
	void* ret_process = NULL;
	ret_process = process[index];
	
	process_lock -> Release();
	
	return ret_process;
}




/* Free the table slot at index. */
void Table :: Release(int index){
	process_lock -> Acquire();
	
	total_process--;	
	process[index] = NULL;

	process_lock -> Release();
}


/* Get total # of processes */
int Table :: GetTotalProcess(){ return total_process; }












