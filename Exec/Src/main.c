/**************************************** MAD Engineers ****************************************

  MAD Engineers
  Copyright (c) 2014

Description	: Demostrating the difference between system() and exec() functions.	
				Also, usage of atexit() function.

Theory:

	_________________________________ system() _________________________________
	A program can be run from inside a program using the system() function. It 
	returns the return value of command executed. 

	Syntax:
		int system(const char* StringOfCommandToBeExecuted)	
	
	Example:
		system("echo Hello");
	
	The program waits until complete execution of the command.
	To continue with execution of program, without waiting for the command to complete,
	the command can be run in background.
		
	Command can be run in background using '&' as suffix to the command.
	system("ps -ax &");

	_________________________________ exec() _________________________________
	An exec function replaces the current process with a new process specified 
	by the path or file argument.
	


Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 120818 * ** madD7 ** * Originator                                                       *	
*/



/* Include Files **************************************************************************
 * @{
 */
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
/*
 * @}
 */

#define SLEEP5_COMMAND_STRING		"sleep 5"
#define COMMAND_STRING_BCKGRND 		"sleep 10 &"

#define SLEEP_STRING				"sleep"
#define ARG20_STRING				"20"
#define ARG15_STRING				"15"

#define PS_COMMAND_STRING			"ps"
#define PS_COMMAND_ARG				"ax"


/* Variables ******************************************************************************
 * @{
 */
static int HelpFlg = 1;

/*
 * @}
 */



/* Functions ******************************************************************************
 * @{
 */
static void PrintHelp(void)
{
	if (HelpFlg)
	{
		printf("Usage: <ExeName> <Option>\n"\
				"\t1 - Execute command using system() function call\n"\
				"\t2 - Execute command in background using system() function call\n"\
				"\t3 - Execute command using exec() function call\n"\
				"\t4 - Execute command in child, using exec() function call\n"
				);
	}
}

/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
static int CallSystem(void)
{
	printf("Executing system(\"%s\")\n", SLEEP5_COMMAND_STRING);
	return system(SLEEP5_COMMAND_STRING);
}

/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
static int CallSystemBckgrnd(void)
{
	printf("Executing system(\"%s\")\n", COMMAND_STRING_BCKGRND);
	return system(COMMAND_STRING_BCKGRND);
}


/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
static void CallExecv(void)
{
	printf("This thread will be replaced on executing exec()\n");
	execlp(PS_COMMAND_STRING, PS_COMMAND_STRING, PS_COMMAND_ARG, NULL);
}

/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
static void CallForkedExecv(void)
{
	pid_t pid;

	printf("In the Parent program\n");
	pid = fork();

	switch (pid)
	{
		case -1:
			printf("Error: Unable to fork() \n" );
			break;
		
		case 0:
			printf("In Child process. Executing \"%s %s \"\n", SLEEP_STRING, ARG20_STRING);
			execlp(SLEEP_STRING, SLEEP_STRING, ARG20_STRING, NULL);
			printf("Child execution completed\n");
			break;

		default:
			printf("In parent process. Executing \"%s %s\"\n", SLEEP_STRING, ARG15_STRING);
			execlp(SLEEP_STRING, SLEEP_STRING, ARG15_STRING, NULL);
			printf("Parent execution completed\n");
			break;
	}


}


/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
int main(int iArgc, char* pcArgv[])
{
	int iInpCmd=0;
	int iRetVal=0;

	/* Set PrintHelp() as callback function */
	atexit(PrintHelp);

	if (iArgc < 2)
	{
		exit(EXIT_FAILURE);
	}
	
	iInpCmd = atoi(pcArgv[1]);

	switch(iInpCmd)
	{
		case 1:
			iRetVal = CallSystem(); 
			if (!iRetVal)
			{
				printf("Execution completed successfully\n");
			}

			HelpFlg = 0;
			break;

		case 2:
			iRetVal = CallSystemBckgrnd();
			if(!iRetVal)
			{
				printf("Back in main program\n");
			}

			HelpFlg = 0;
			break;

		case 3:
			CallExecv();
			printf("This statement will never be printed\n");
			break;

		case 4:
			CallForkedExecv();
			break;

		default:
			exit(EXIT_FAILURE);
			break;
	}

	exit(EXIT_SUCCESS);
}

/*
 * @}
 */





/* ~~~~~ END OF FILE ~~~~~ */

