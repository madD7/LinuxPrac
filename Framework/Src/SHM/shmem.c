/****************************************

Description	:



Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 120818 * ** madD7 ** * Originator                                                       *	
*/



/* Include Files **************************************************************************
 * @{
 */
#include "shmlib.h"

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

/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
RETVAL SHMEM_CreateOrOpen()
{

	LOG_FUNC_IN

	LOG_FUNC_OUT

	return SUCCESS;
}


/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
RETVAL SHMEM_DeleteOrClose()
{
	LOG_FUNC_IN

	LOG_FUNC_OUT
	return SUCCESS;
}


/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
RETVAL SHMEM_GetOrSetInfo()
{
	LOG_FUNC_IN

	LOG_FUNC_OUT

	return SUCCESS;
}


/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
RETVAL SHMEM_Init()
{
	RETVAL iRetVal;
	
	LOG_FUNC_IN

	

	LOG_FUNC_OUT
	return SUCCESS;
}


#if __TESTING__
/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
static void PrintHelp(void)
{
	printf("Usage: <ExeName> <LogPath>\n");
}


/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
int main(INT iArgc, CHAR *pArgs[])
{
	CHAR* pcAppname;

	if(iArgc < 2)
	{
		PrintHelp();
		errno = EINVAL;
		perror("Input argument count less than required arguments");
		return FAILURE;
	}
	
	pcAppname = strrchr(pArgs[0], '/');
	if(pcAppname == NULL )
	{
		perror("Unable to parse app name ");
		return FAILURE;
	}
	pcAppname++;

	if ( LOG_Init(pArgs[1], pcAppname, LOGS_DEBUG) != SUCCESS )
	{
		perror("Failed to initialise log library");
		return FAILURE;
	}


	LOG_DeInit();

	return SUCCESS;
}
#endif


/*
 * @}
 */





/* ~~~~~ END OF FILE ~~~~~ */

