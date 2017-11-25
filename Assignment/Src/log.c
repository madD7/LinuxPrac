/****************************************
Description	: Debug framework

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 290717 * ** madD7 ** * Originator                                                       *

*/



/* Include Files **************************************************************************
 * @{
 */

#include "log.h"

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
Published	: No
*/
void LOG_LogMsg(const char* pcfname, int ilineno, short slvl, const char* msg, ...)
{
	va_list argptr;
	FILE *pF = stdout;

	if(slvl < LOGS_INFO )
	{
		pF = stderr;
	}
	else
	{ 
		pF = stdout;
	}

	if(pcfname != NULL)
		fprintf(pF, "%s:%d | ", pcfname, ilineno);

	if(slvl == LOGS_ERROR)
		fprintf(pF, "%s", "ERROR: ");
	else if(slvl == LOGS_WARN)
		fprintf(pF, "%s", "WARN: ");
	else if(slvl == LOGS_INFO)
		fprintf(pF, "%s", "INFO: ");

	va_start(argptr, msg);

	vfprintf(pF, msg, argptr);

	fprintf(pF,"\n");
		
	va_end(argptr);	
}


/*
 * @}
 */





/* ~~~~~ END OF FILE ~~~~~ */



