/**************************************** MAD Engineers ***************************************

Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* DDMMYY * ** madD7 ** * Originator                                                       *

*/



/* Include Files ************************************************************************** @{
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "log.h"

/* @}
*/


/* Variables ****************************************************************************** @{
*/
static FILE* pFOut = NULL;
static FILE* pFErr = NULL;

static INT iFileDes=0;
static INT iSetLevel=0;

/* @}
*/



/* Functions ****************************************************************************** @{
*/

/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
static RETVAL CreateOutFile(CHAR* pcaLogFile)
{
	if ( pcaLogFile[0] == NULL_CHAR )
	{
		printf("%s[%d]: No Filename given %c\n", 
					__func__, __LINE__, pcaLogFile[0]);
		return FAILURE;
	}

	/*
	 *	Create a copy of the file descriptor for stdout	using i
	 *	the lowest-numbered unused descriptor for the new descriptor.
	 *	(fileno - returns integer descriptor for FILE* stream)
	 */
	if( (iFileDes = dup(fileno(stdout)) ) < 0)
	{
		perror("Failed to create copy of file descriptor for stdout");
		return FAILURE;
	}

	/*
	 * The freopen() function opens the file "caLogFile" and associates
	 * the stream stdout with it. 
	 * The original stream (if it exists, i.e. stdout) is closed.  
	 * The mode argument "a+" is just as in the fopen() function.  
	 * The primary use of the freopen() function is to change the file 
	 * associated with a standard text stream (stderr, stdin, or stdout).
	 */
	if((pFOut = freopen(pcaLogFile, "a+", stdout)) == NULL)
	{
		printf("%s[%d]: Failed to open file [%s]", 
				__func__, __LINE__, pcaLogFile);
		perror("Failed to open file to log stdout");
		return FAILURE;
	}

	if((pFErr = freopen(pcaLogFile, "a+", stderr)) == NULL)
	{
		LOG_DeInit();
		printf("%s[%d]: Failed to open [%s] file to log stderr\n",
				__func__, __LINE__, pcaLogFile);
		perror("Failed to open file to log stdout");
		return FAILURE;
	}

	/*
	 *	The  setvbuf()  function  may be used on any open stream to change its buffer.
	 */
	if(setvbuf(stdout,NULL, _IONBF,0) != 0)
	{
		printf("%s[%d]: Failed to set buffer for stdout\n", __func__, __LINE__);
		return FAILURE;
	}
			
	if(setvbuf(stderr, NULL, _IONBF,0) != 0)
	{
		printf("%s[%d]:Failed to set buffer for stderr\n",__func__, __LINE__);
		return FAILURE;
	}
	
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	printf("Log file initialization successful.\n");

	return 0;
}


/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
static INT LOG_SetVerbosity(INT iVblvl)
{
	iSetLevel=iVblvl;
	return iSetLevel;
}



/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
void LOG_DeInit()
{
	dup2(iFileDes, fileno(stdout));
	dup2(iFileDes, fileno(stderr));
	clearerr(stdout);
	clearerr(stderr);
}

/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
RETVAL LOG_Init(CHAR* pcaLogFilepath, CHAR *pcaAppName, INT iLogLevel)
{
	time_t sNow;
	INT iDay;
	INT iMonth=1;		// Month range : 0 to 11
	INT iYear=1900;		// Must add 1900 
	struct tm* psLocal = NULL;
	FILEPATH caLogFile;

	time(&sNow);
	psLocal = localtime(&sNow);

	iDay = psLocal->tm_mday;
	iMonth += psLocal->tm_mon;
	iYear += psLocal->tm_year;

	if( pcaLogFilepath == NULL || pcaLogFilepath[0] == NULL_CHAR )
	{
		printf("No Log Filename given. Writing to default log file [%s]\n",
				DEFAULT_LOG_DIR);

		if ((strlen(pcaAppName) + strlen(DEFAULT_LOG_DIR) 
								+ strlen(LOG_EXTENSION)) >= PATH_MAX)
		{
			sprintf(caLogFile, "%s%s%s_%02d_%02d_%04d.%s",
						DEFAULT_LOG_DIR, DIR_SEPERATOR, 
						pcaAppName,
						iDay,iMonth,iYear,
						LOG_EXTENSION);
		}
		else 
		{
			printf("Unable to create log file. Application name too long");
			return FAILURE;
		}
	}
	else
	{
		sprintf(caLogFile, "%s%s%s_%02d_%02d_%04d.%s", 
						pcaLogFilepath, DIR_SEPERATOR, 
						pcaAppName,
						iDay, iMonth, iYear,
						LOG_EXTENSION);
		printf("Creating log file [%s]\n",caLogFile);
	}

	if(CreateOutFile(caLogFile) != SUCCESS)
	{
		printf("Failed to redirect output on stdout stream to %s file\n", caLogFile);
		return FAILURE;
	}

	LOG_SetVerbosity(iLogLevel);
	
	return SUCCESS;
}

/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
void LOG_LogMsg(const CHAR* pcFName, INT iLineno, INT iLvl, const CHAR* cMsg, ...)
{
	va_list argptr;
	FILE *pF = stdout;

	/* If Set verbosity is higher than message verbosity,
	   then dont log the message
	*/
	if(iSetLevel<iLvl)
		return;

	if(iLvl < LOGS_INFO )
	{
		pF = stderr;
	}
	else
	{ 
		pF = stdout;
	}

	if(pcFName != NULL)
		fprintf(pF, "%s:%d | ", pcFName, iLineno);

	if(iLvl == LOGS_ERROR)
		fprintf(pF, "%s", "ERROR: ");
	else if(iLvl == LOGS_WARN)
		fprintf(pF, "%s", "WARN : ");
	else if(iLvl == LOGS_INFO)
		fprintf(pF, "%s", "INFO : ");
	else if (iLvl >= LOGS_DEBUG)
		fprintf(pF, "%s", "DEBUG: ");

	va_start(argptr, cMsg);
	vfprintf(pF, cMsg, argptr);
	fprintf(pF,"\n");											
	va_end(argptr);	
}


#if __TESTING__
/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
INT main(INT iArgc, CHAR** pArgs)
{
	INT iVerb=LOGS_ERROR;
	FILENAME caAppname = "LogLib";

	if(iArgc < 2)
	{
		errno = EINVAL;
		perror("Input arguments less than required arguments");
		printf("Usage \n <Logfilepath> <verbosity>\n verbsity: ERROR/WARN/INFO/DEBUG\n");
		return FAILURE;
	}

	if ( strcmp(pArgs[2], "ERROR") == SUCCESS)
	{
		iVerb=LOGS_ERROR;
	}
	else if ( strcmp(pArgs[2], "WARN") == SUCCESS )
	{
		iVerb=LOGS_WARN;
	}
	else if (strcmp(pArgs[2], "INFO") == SUCCESS )
	{
		iVerb=LOGS_INFO;
	}
	else if (strcmp(pArgs[2], "DEBUG") == SUCCESS )
	{
		iVerb=LOGS_DEBUG;	
	}
	else
	{
		errno = EINVAL;
		perror("Invalid verbosity");
        printf("Usage \n <Logfilepath> <verbosity>\n verbsity: ERROR/WARN/INFO/DEBUG\n");
        return FAILURE;
	}

	printf("%s %s %s\n", pArgs[0], caAppname, pArgs[2]);

	if ( LOG_Init(pArgs[1], caAppname, iVerb) != 0)
	{
		perror("Failed to Initialize LOG ");
		return -1;
	}

	LOG_FUNC_IN;
	LOG_ERROR("Error logging tested");
	LOG_WARN("Warning logging tested");
	LOG_INFO("Info logging tested");
	LOG_DEBUG("Debug logging tested");
	LOG_FUNC_OUT;

	printf("In main function\n");
	printf("Reverting back to stdout stream\n");

	LOG_DeInit();
	printf("Printing in log file reverted successfully back to stdout\n");
	return 0;
}
#endif


/* @}
   */


/* ~~~~~ END OF FILE ~~~~~ */


