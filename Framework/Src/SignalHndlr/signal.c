/****************************************

Description	:
				Signal handler library. 
				The file contains sample code to implement a signal handler thread,
				with the application for handling the singals.
				All signals are blocked for the main thread, during initialization.
				A seperate thread is created to handle signals except SIGTERM and SIGINT.
				For Signal, SIGUSR1, default handler is called and for SIGUSR2, the 
				handler calls callback function passed by the application.
				Also, usage of atexit() function.

				Initialize Log (using log library) before calling the Init function


Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 120818 * ** madD7 ** * Originator                                                       *	
*/



/* Include Files **************************************************************************
 * @{
 */
#include "singalhandler.h"

/*
 * @}
 */


/* Variables ******************************************************************************
 * @{
 */

// _I_I denote, the argument is of type int and return type is of int
FuncSignalHandler_I_I 	pfnSigHndler;
FuncAppExitHandler_V_I 	pfnAppExitHandler;


INT iErrNo;

/*
 * @}
 */



/* Functions ******************************************************************************
 * @{
 */

/***************************************************************************************
Description	: Set size of Core files to maximum 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
static void SetCoreUnlimited()
{
	struct rlimit sRLimit;

	sRLimit.rlim_cur = RLIM_INFINITY;		// Set Soft limit to Infinity
	sRLimit.rlim_max = RLIM_INFINITY;		// Set Hard limit to Infinity

	setrlimit(RLIMIT_CORE, &sRLimit);
}

/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
void DefaultSigHandler(INT32 iSignal)
{
	LOG_FUNC_IN

	LOG_INFO(iSignal, "In Default Signal Handle Function");

	LOG_FUNC_OUT
}


/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
static void* SignalHandlerFunction(void* pVoid)
{
	RETVAL iRetVal;	
	INT32  iSignal;
	sigset_t sSigSet;

	LOG_FUNC_IN

	sigfillset(&sSigSet);
	// Set all signals that are to be UN-blocked
	sigdelset(&sSigSet, SIGTERM);
	sigdelset(&sSigSet, SIGSEGV);
	sigdelset(&sSigSet, SIGKILL);
	sigdelset(&sSigSet, SIGINT);
	sigdelset(&sSigSet, SIGFPE);
	sigdelset(&sSigSet, SIGKILL);
	sigdelset(&sSigSet, SIGQUIT);
	sigdelset(&sSigSet, SIGBUS);
	sigdelset(&sSigSet, SIGSEGV);

	// Wait for signal
	while(1)
	{
		iRetVal = sigwait(&sSigSet, &iSignal);
		
		switch(iSignal)
		{
			case SIGHUP:
				LOG_INFO(iRetVal, "Received SIGHUP");
				break;

			case SIGCHLD:
				LOG_INFO(iRetVal, "Received SIGCHLD");
				break;

			case SIGUSR1:
				LOG_INFO(iRetVal, "Received SIGUSR1");
				DefaultSigHandler(iSignal);
				break;

			case SIGUSR2:
				LOG_INFO(iRetVal, "Received SIGUSR2");

				if(pfnSigHndler != NULL)
					pfnSigHndler(iSignal);
				else
				{
					LOG_INFO(iRetVal, "No callback hanlder for SIGUSR2. "
								"Calling defualt signal handler");

					DefaultSigHandler(iSignal);
				}
				break;

			default:
				LOG_INFO(iRetVal, "Received %d", iSignal);
				break;
		}
	}

	LOG_FUNC_OUT
	return pVoid;
}

/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
INT32 SIG_SendSig(pid_t iProcId, INT32 iSigNum)
{
	RETVAL iRetVal = SUCCESS;

	LOG_FUNC_IN
	
	iRetVal = kill(iProcId, iSigNum);
	iErrNo = errno;
	
	if (iRetVal != SUCCESS)
	{
		LOG_WARN(iRetVal, 
					"Failed to send Signal %d to Process with PId %d", 
					iSigNum,
					iProcId);
		iRetVal = iErrNo;
	}
	
	LOG_FUNC_OUT
	return iRetVal;
}


/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
RETVAL SIG_Init(FuncSignalHandler_I_I pfnSig, FuncAppExitHandler_V_I pfnAppEx)
{
	RETVAL iRetVal;	
	pthread_t sSigThread;
	sigset_t sSigSet;

	LOG_FUNC_IN

	SetCoreUnlimited();

	pfnSigHndler = pfnSig;
	pfnAppExitHandler = pfnAppEx;

	if( pfnAppEx == NULL ) 
		atexit(&LOG_DeInit);

	// Set all signals that are to be UN-blocked
	sigdelset(&sSigSet, SIGPROF);
	sigdelset(&sSigSet, SIGKILL);
	sigdelset(&sSigSet, SIGSTOP);
	sigdelset(&sSigSet, SIGBUS);
	sigdelset(&sSigSet, SIGFPE);
	sigdelset(&sSigSet, SIGILL);
	sigdelset(&sSigSet, SIGSEGV);

	/*  Block all signals in SigSet.
		we deleted above signals from SigSet.
		Thus all singals except above signals will not be blocked
	*/
	iRetVal = pthread_sigmask(SIG_BLOCK, &sSigSet, NULL);
	iErrNo = errno;
	if( iRetVal != SUCCESS )
	{
		LOG_WARN(iRetVal,"Unable to set sigmask");
		LOG_FUNC_OUT
		return iErrNo;
	}

	iRetVal = pthread_create(&sSigThread, NULL,
								&SignalHandlerFunction, NULL); 
	iErrNo = errno;
	if(iRetVal != SUCCESS)
	{
		LOG_ERROR(iRetVal,"Failed to create signal handler thread");
		LOG_FUNC_OUT
		return iErrNo;
	}

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
	pcAppname++;
	if(pcAppname == NULL )
	{
		perror("Unable to parse app name ");
		return FAILURE;
	}

	if ( LOG_Init(pArgs[1], pcAppname, LOGS_DEBUG) != SUCCESS )
	{
		perror("Failed to initialise log library");
		return FAILURE;
	}

	if(	SIG_Init(NULL, NULL) != SUCCESS)
	{
		perror("Failed to initialise signal handler library");
		return FAILURE;
	}

	SIG_SendSig(getpid(), SIGUSR1);
	SIG_SendSig(getpid(), SIGUSR2);
	SIG_SendSig(getpid(), SIGKILL);
	LOG_DeInit();

	return SUCCESS;
}
#endif


/*
 * @}
 */





/* ~~~~~ END OF FILE ~~~~~ */

