#include "SwapPage.h"





SwapPage :: SwapPage(){
	sarr = new char[PAGESIZE];
}


char* SwapPage :: getSwapPage(){
	return sarr;
}
