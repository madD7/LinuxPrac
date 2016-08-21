#include "appInclude.h"


/**
 * Demonstrating the effect of two threads executing single function.
 * 
 * ThreadSafe function
 *		A function is thread safe, iff it is safe for concurrent access.
 *		
 * 
 * 
 * A thread safe function is always Rentrant, but reverse may not always be true.
 */


void nonReentrant()
{
	/**
	   Non static local variable will be re-initialized everytime the function is called, 
	   irrespective of the thread from which it is called.
	   But a static variable, retains its value, irrespective of the thread from which the function is called.
	   Thus, this is not a threadsafe function.
	 */
	static int i=0;

	printf("nonReEntrant function: local variable i=%d\n", i++);
}




