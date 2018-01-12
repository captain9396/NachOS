/*
 * exectest.c
 *
 * Exec a simple program.  Return the result of Exec.
 */

#include "syscall.h"

int
main()
{
    int result = 9396;
	char str[4];
	
//	Exit(123);
	result = Exec("../test/matmult");
	result = Exec("../test/array");
	result = Exec("../test/array");
	result = Exec("../test/array");
	result = Exec("../test/array");
}
	
	
