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

/* Global variables are shared between multiple threads. Hence their use is not threadsafe */
int g=0;

/* However a variable of type __thread is threadsafe, i.e. they are local to respective thread */
__thread int tg=0;


void nonReentrant()
{
	/**
	   Non static local variable will be re-initialized everytime the function is called, 
	   irrespective of the thread from which it is called.
	   But a static variable, retains its value, irrespective of the thread from which the function is called.
	   Thus, this is not a threadsafe function.
	 */
	static int i=0;
	printf("nonReEntrant function:\t local variable i=%d\t", i++);
	printf("global variable g=%d\n", g++);
}


void reEntrant()
{
	/**
	   Though a static variable retains its value, irrespective of the thread from which the function is called,
	   a static __thread type variable is local to the respective thread.
	   Thus, this it is threadsafe and as the function uses only threadsafe variables, it is threadsafe function.
	 */
	static __thread int i=0;
	printf("reEntrant function:\t local variable i=%d\t", i++);
	printf("global variable tg=%d\n", tg++);
}



