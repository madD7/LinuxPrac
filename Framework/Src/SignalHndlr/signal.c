/**************************************** MAD Engineers ****************************************

  MAD Engineers
  Copyright (c) 2014

Description	: Demostrating the difference between system() and exec() functions.	
				Also, usage of atexit() function.




Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 120818 * ** madD7 ** * Originator                                                       *	
*/



/* Include Files **************************************************************************
 * @{
 */
#include <stdio.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
/*
 * @}
 */


/* Variables ******************************************************************************
 * @{
 */

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

