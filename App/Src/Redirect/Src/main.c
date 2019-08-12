#include <stdio.h>
#include <unistd.h>


#define	LOG_FILENAME	"output.log"

FILE* pFOut;
FILE* pFErr;

int iFileDes=0;



int CreateOutFile()
{
	/*
	 *	Create a copy of the file descriptor for stdout	using i
	 *	the lowest-numbered unused descriptor for the new descriptor.
	 *	(fileno - returns integer descriptor for FILE* stream)
	 */
	if((iFileDes = dup(fileno(stdout))) < 0)
	{
		printf("Failed to create copy of file descriptor for stdout\n");
		return -1;
	}

	/*
	 * The freopen() function opens the file "LOG_FILENAME" and associates
	 * the stream stdout with it. 
	 * The original stream (if it exists, i.e. stdout) is closed.  
	 * The mode argument "a+" is just as in the fopen() function.  
	 * The primary use of the freopen() function is to change the file 
	 * associated with a standard text stream (stderr, stdin, or stdout).
	 */
	if((pFOut = freopen(LOG_FILENAME, "a+", stdout)) == NULL)
	{
		printf("Failed to open %s file to log stdout\n", LOG_FILENAME);
		return -1;
	}

	if((pFErr = freopen(LOG_FILENAME, "a+", stderr)) == NULL)
	{
		printf("Failed to open %s file to log stderr\n", LOG_FILENAME);
		return -1;
	}

	/*
	 *	The  setvbuf()  function  may be used on any open stream to change its buffer.
	 */
	if(setvbuf(stdout,NULL, _IONBF,0) != 0)
	{
		printf("Failed to set buffer for stdout\n");
		return -1;
	}
			
	if(setvbuf(stderr, NULL, _IONBF,0) != 0)
	{
		printf("Failed to set buffer for stderr\n");
		return -1;
	}
	
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	printf("Log file initialization successful.\n");

	return 0;
}



void RevertStdOut()
{
	dup2(iFileDes, fileno(stdout));
	clearerr(stdout);
}


int main()
{

	if(CreateOutFile() != 0)
	{
		printf("Failed to redirect output on stdout stream to %s file\n", LOG_FILENAME);
		return -1;
	}

	printf("In main function\n");
	printf("Reverting back to stdout stream\n");

	RevertStdOut();
	printf("Printing in log file reverted successfully back to stdout\n");
	return 0;
}



