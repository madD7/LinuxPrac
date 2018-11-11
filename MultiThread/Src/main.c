#include "appInclude.h"
#include "reentrant.h"

#include <semaphore.h>


/**
 * Multiple flow of control in a single program are known as threads.
 * when a new thread is created, the new thread gets its own stack of variables.
 * But, the global variables, file descriptors, signal handlers & current directory state are shared.
 *
 * Some synchronisation methods must be adopted to prevent simultaneous access of global variables by multiple threads.
 * Synchronisation can be achieved by using semaphore or mutex.
 *
 * Semaphore
 *		A semaphore is a special type of variable that can be incremented or decremented, 
 *		but crucial access to the variable is guaranteed to be atomic, even in a multithreaded program.
 * 		Semaphores can be binary or counting sempahores.
 */

/* The global variable is shared between threads */
int gVar1=0;

/* But this global variable is thread-local. */
__thread int tgVar1=0;


sem_t BufSem;
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


void* readBuf(void* pUnused)
{
	printf("readBuf thread\n");

	/* strstr return the pointer to first occurance of 'end' in CommonBuf. Returns NULL if not found */
	do
	{
		/* sem_wait decreases value of semaphore.
		 * If the value of the semaphore is 0, then flow of execution waits untill semaphore is non-zero.
		 */
		#ifdef USE_SEMAPHORE
			sem_wait(&BufSem);
		#else
			printf("Mutex:\t");
			pthread_mutex_unlock(&BufMutx);
		#endif
		printf("You input %d characters\n", (int)strlen(CommonBuf)-1);
		sleep(1);
	}while(strstr ((const char*)CommonBuf, "end") == NULL);

	printf("Existing read thread.\n");

	pthread_exit(NULL);
}

/* The limitation of using semaphore 
 *		The other thread gets enough time to count the number of words, while the main thread is waiting for the user input.
 *		But if the main thread reads input faster, there is no time for other thread to execute.
 * 		However, the semaphore is incremented more than once. Hence, other thread counts the words untill the semaphore becomes 0.
 *		Thus, timing consideration in a multithreaded program becomes a necessity.
 *		Note: To enable faster input read, define the macro, FAST_INPUT
 */
void readInput_Semaphore()
{
	int status=0;
	pthread_t tid1;
	void *pThreadArg;

	/* Initialising Semaphore */
	status = sem_init(&BufSem, 0, 0);
	if(status != 0)
	{
		perror("Failed to initialize semaphore\n");
		exit(EXIT_FAILURE);
	}

	status =  pthread_create(&tid1, NULL, &readBuf, NULL);
	if(status != 0)
	{
		perror("Failed to create thread\n");
		exit(EXIT_FAILURE);
	}

	printf("Enter some text. Type 'end' to finish\n");
	do
	{
		#if		FAST_INPUT
		if(strstr(CommonBuf, "FAST") != NULL)
		{
			sem_post(&BufSem);
			strcpy(CommonBuf, "Faster");
		}
		else
		{
			fgets(CommonBuf, sizeof(CommonBuf), stdin);
		}
		#else
			fgets(CommonBuf, sizeof(CommonBuf), stdin);
		#endif
		sem_post(&BufSem);
		
		/* To read the value of semaphore*/
		#if		SEMAPHOREVALUE	
		sem_getvalue(&BufSem, &status);
		printf("Value of semaphore is %d\n", status);
		#endif

	}while(strstr(CommonBuf, "end") == NULL);

	printf("Waiting for thread to finish...\n");
	status = pthread_join(tid1, &pThreadArg);
	if (status != 0)
	{
		perror("Thread join failed\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Thread joined\n");
	sem_destroy(&BufSem);
}

/**
 *	The limitation of semaphore can be overcomed using Mutex
 */
void readInput_Mutex()
{
	int status=0;
	pthread_t tid1;
	void *pThreadArg;

	/* Initialising Semaphore */
	status = pthread_mutex_init(&BufMutx, NULL);
	if(status != 0)
	{
		perror("Failed to initialize mutex\n");
		exit(EXIT_FAILURE);
	}

	status =  pthread_create(&tid1, NULL, &readBuf, NULL);
	if(status != 0)
	{
		perror("Failed to create thread\n");
		exit(EXIT_FAILURE);
	}

	printf("Enter some text. Type 'end' to finish\n");
	do
	{
//		pthread_mutex_lock(&BufMutx);
		fgets(CommonBuf, sizeof(CommonBuf), stdin);
		pthread_mutex_lock(&BufMutx);
	}while(strstr(CommonBuf, "end") == NULL);

	printf("Waiting for thread to finish...\n");
	status = pthread_join(tid1, &pThreadArg);
	if (status != 0)
	{
		perror("Thread join failed\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Thread joined\n");
	sem_destroy(&BufSem);

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

	#ifdef		USE_SEMAPHORE
	readInput_Semaphore();
	#else
	readInput_Mutex();
	#endif
	
	return 0;
}




