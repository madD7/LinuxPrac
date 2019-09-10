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
#include "loglib.h"

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
void LOG_LogMsg(const CHAR* pcFName, const CHAR* pcFuncName, 
					INT iLineno, INT iLvl,
					RETVAL iRetVal,
					const CHAR* cMsg, ...)
{
	va_list argptr;
	FILE *pF = stdout;
	time_t sNow;
	struct tm* psLocal = NULL;

	time(&sNow);
	psLocal = localtime(&sNow);

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

	fprintf(pF,"%02d-%02d-%04d %02d:%02d:%02d |",
				psLocal->tm_mday,
				psLocal->tm_mon + 1,
				psLocal->tm_year + 1900,
				psLocal->tm_hour,
				psLocal->tm_min,
				psLocal->tm_sec);

	if(iLvl == LOGS_ERROR)
		fprintf(pF, " %s=%5d | ", "ERROR",iRetVal);
	else if(iLvl == LOGS_WARN)
		fprintf(pF, " %s=%5d | ", "WARN ",iRetVal);
	else if(iLvl == LOGS_INFO)
		fprintf(pF, " %s=%5d | ", "INFO ",iRetVal);
	else if (iLvl >= LOGS_DEBUG)
		fprintf(pF, " %s=%5d | ", "DEBUG",iRetVal);

	va_start(argptr, cMsg);
	vfprintf(pF, cMsg, argptr);
	
	if(pcFName != NULL)
		fprintf(pF, " | %s:%d- %s() ", pcFName, iLineno, pcFuncName);

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
static void PrintHelp()
{
	printf("Usage \n <Logfilepath> <verbosity>\n verbsity: ERROR/WARN/INFO/DEBUG\n");
}

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
	FILENAME caAppname = "Log"; 
	CHAR* pcAppname=NULL;

	if(iArgc < 3)
	{
		errno = EINVAL;
		perror("Input argument count less than required arguments");
		PrintHelp();
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
        PrintHelp();
        return FAILURE;
	}

	pcAppname = strrchr(pArgs[0], '/');
	if (pcAppname == NULL)
	{
		pcAppname = caAppname;
	}
	else
	{
		pcAppname++;
	}

	if ( LOG_Init(pArgs[1], pcAppname, iVerb) != 0)
	{
		perror("Failed to Initialize LOG ");
		return -1;
	}

	LOG_FUNC_IN
	LOG_ERROR(errno, "Error logging tested");
	LOG_WARN(errno, "Warning logging tested");
	LOG_INFO(errno, "Info logging tested");
	LOG_DEBUG(errno, "Debug logging tested");
	LOG_FUNC_OUT

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


