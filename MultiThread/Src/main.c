#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


/**
 * Multiple flow of control in a single program are known as threads.
 * when a new thread is created, the new thread gets its own stack of variables.
 * But, the global variables, file descriptors, signal handlers & current directory state are shared.
 */

/* The global variable is shared between threads */
int gVar1=0;

void* threadFunct1(void* pUnused)
{
	int i;
	printf("Hello Thread 1\n");

	for(i=0; i<5;i++)
	{
		printf("threadFunct1: value of global variable is %d\n", gVar1++);
		usleep(10);
	}

	/* This string is passed to the parent thread, arg 2 of pthread_join function */
	pthread_exit("threadFunct1 exit\n");
}


int main()
{
	int status=0, i;
	pthread_t tid1;
	void* pThreadArg;
		
	printf("Main thread\n");

	status = pthread_create(&tid1, NULL, &threadFunct1, NULL);
	if(status != 0)
	{
		perror("Failed to create thread\n");
		exit(EXIT_FAILURE);
	}

	for(i=0; i<5;i++)
	{
		printf("main: value of global variable is %d\n", gVar1++ );
		usleep(12);
	}

	printf("Waiting for threadFunct1 to finish\n");

	/**
	 * pthread_join is the equivalent function of wait, that parent process use to collect child process.
	 * If this function is not used, main thread may get completed without child thread execution.
	 */
	status = pthread_join(tid1, &pThreadArg);
	if(status != 0)
	{
		perror("Failed to join thread\n");
		exit(EXIT_FAILURE);
	}

	printf("Message from child thread: %s", (char *)pThreadArg);

	return 0;
}




