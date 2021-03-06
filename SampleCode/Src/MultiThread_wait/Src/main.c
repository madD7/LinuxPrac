#include "appInclude.h"



/**
 * Multiple flow of control in a single program are known as threads.
 * when a new thread is created, the new thread gets its own stack of variables.
 * But, the global variables, file descriptors, signal handlers & current directory state are shared.
 *
 */

pthread_mutex_t BufMutx;
char CommonBuf[256]={0};


void* threadFunct1(void* pUnused)
{
	int i;
	printf("Hello threadFunct1\n");

	for(i=0; i<5;i++)
	{
		printf("threadFunct1: \t gVar1=%d \t tgVar1=%d\n", gVar1++, tgVar1++);
		nonReentrant();
		reEntrant();
		//usleep(10);
		sleep(2);
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
		printf("main: \t\t gVar1=%d \t tgVar1=%d\n", gVar1++, tgVar1++);
		nonReentrant();
		reEntrant();
		sleep(3);
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




