/****************************************

Description	: 
		Generic key_t generator, which can be used for msgget(), shmget(), semget()
			
Theory:
		msgget(), shmget(), semget() functions require a key_t type key that can be used 
		between two or more processes for IPC.

		Such a key can be generated using ftok().
		
Syntax:
		key_t ftok(const char *pathname, int proj_id)

		include -	sys/types.h
					sys/ipc.h
		
		The ftok() function uses the identity of the file named by the given
		pathname (which must refer to an existing, accessible file) and the
		least significant 8 bits of proj_id (which must be nonzero) to
		generate a key_t type System V IPC key.
		The resulting value is the same for all pathnames that name the same
		file, when the same value of proj_id is used.  The value returned
		should be different when the (simultaneously existing) files or the
		project IDs differ.

		On success, the generated key_t value is returned.  On failure -1 is
		returned, with errno indicating the error as for the stat(2) system
		call.

		However,
			no guarantee can be given that the resulting key_t is unique. 
			Typically, a best-effort attempt combines the given proj_id
			byte, the lower 16 bits of the inode number, and the lower 8 bits of
			the device number into a 32-bit result.  Collisions may easily happen, 
			for example between files on /dev/hda1 and files on /dev/sda1.

	
	Thus, we require a lib that would help generate a unique key_t that can be used.
	Now, this can be achieved by creating a file at specified path and use the inode
	of that file as key_t.

Hint:
	Create a folder where the files will be created whose inode can be used as key_t.
	Lets say 'KEY' folder. Generate a file name with the following pattern

	<PROCESSNAME>_<IPCTYPE>

	For process - Receiver, Ipc - shared memory, 
	the filename can be - RECEIVER_SHM
	
	Input the complete relative filepath with key file name to GenerateKey function.
		
								
Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* DDMMYY * ** madD7 ** * Originator                                                       *

*/



/* Include Files **************************************************************************
 * @{
 */
#include <sys/ipc.h>
#include "log.h"
#include "key.h"
/*
 * @}
 */

/* Macros *********************************************************************************
 * @{
 */
#define	KEY_FILE_PERMISSION			0640

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
Input		: pcKeyString	pointer to the keyfile 
			  pkey			pointer to key_t to store generated key_t 
Output		: key_t
Returns		: 
Notes		: None
*/
RETVAL KEY_GetKeyforFile(CHAR *pcKeyString, key_t *piKey)
{
	INT iFd = 0;
	INT	iErrno=0;
	struct stat sFileStat;

	LOG_FUNC_IN

	/* Open the Key File */
	iFd = open(pcKeyString, O_CREAT|O_RDWR, KEY_FILE_PERMISSION);
	iErrno = errno;
	if (iFd == -1)
	{
		LOG_ERROR(iErrno, "Error in creating the key file [%s] Error [%s]",
					pcKeyString,
					strerror(errno));
		
		LOG_FUNC_OUT
		return iErrno;
	}

	close(iFd);

	if ( lstat (pcKeyString, &sFileStat) == -1 )
	{
		iErrno=errno;
		LOG_ERROR(iErrno, "Error in getting inode number for the key file [%s] Error [%s]",
				pcKeyString,
				strerror(errno));

		LOG_FUNC_OUT
		return iErrno;
	}


	*piKey = sFileStat.st_ino;

	if ( *piKey == -1)
	{
		iErrno = -1;
		LOG_ERROR( iErrno,"Error in generating key for the key file [%s]",
					pcKeyString	);
		LOG_FUNC_OUT
		return iErrno;
	}

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
RETVAL KEY_GenerateKey(INT iKeyType, CHAR* pcKeyPath, CHAR* pcProcName, key_t* piKey)
{
	RETVAL iRetVal = SUCCESS;
	FILEPATH caKeyFilePath;
	const CHAR *pcKeyStr=NULL;
	

	LOG_FUNC_IN

	if( piKey == NULL)
	{
		LOG_ERROR(FAILURE,
					"Invalid argument piKey [NULL]");

		LOG_FUNC_OUT
		return FAILURE;
	}

	memset (caKeyFilePath, NULL_CHAR, sizeof(caKeyFilePath));
	
	switch(iKeyType)
	{
		case KEY_SEM:
			if( (strlen(pcKeyPath) + 
				 strlen(pcProcName) + 
				 strlen(KEY_SEM_STR)) >= sizeof(caKeyFilePath)
					)
			{
				LOG_WARN(iRetVal,
							"Filepath too long. Using default key file [%s]",
							DEFAULT_LOG_DIR);

				pcKeyPath = NULL;
			}

			pcKeyStr=KEY_SEM_STR;
			break;

		case KEY_SHM:
			if( (strlen(pcKeyPath) + 
				 strlen(pcProcName) + 
				 strlen(KEY_SHM_STR)) >= sizeof(caKeyFilePath)
					)
			{
				LOG_WARN(iRetVal,
							"Filepath too long. Using default key file [%s]",
							DEFAULT_LOG_DIR);
				pcKeyPath = NULL;
			}
			pcKeyStr=KEY_SHM_STR;
			break;

		case KEY_MSGQ:
			if( (strlen(pcKeyPath) + 
				 strlen(pcProcName) + 
				 strlen(KEY_MSGQ_STR)) >= sizeof(caKeyFilePath)
					)
			{
				LOG_WARN(iRetVal,
							"Filepath too long. Using default key file [%s]",
							DEFAULT_LOG_DIR);
				pcKeyPath = NULL;
			}
			pcKeyStr=KEY_MSGQ_STR;
			break;

		default:
			pcKeyStr = NULL;
			break;
	}

	sprintf(caKeyFilePath,"%s/%s_%s",
							(pcKeyPath == NULL ? DEFAULT_LOG_DIR : pcKeyPath),
							(pcProcName == NULL ? "_" : pcProcName),
							(pcKeyStr == NULL ? KEY_UNKNOWN_STR : pcKeyStr)
				);

	iRetVal = KEY_GetKeyforFile(caKeyFilePath,piKey);
	if (iRetVal != SUCCESS)
	{
		LOG_ERROR(iRetVal,
					"Failed to generate key");
		LOG_FUNC_OUT
		return iRetVal;
	}

	LOG_DEBUG(iRetVal, 
				"Creating key file at path[%s] with Key[%d]",
				caKeyFilePath,
				*piKey);


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
INT main(INT iArgc, CHAR *pArgs[])
{
	key_t iKeyShm;
	key_t iKeyMsgQ;
	key_t iKeySem;
	RETVAL iRetVal = SUCCESS;
	FILEPATH caKeyPath;
	CHAR* pcAppname=NULL;
	CHAR* pcTemp=NULL;

	memset (caKeyPath, NULL_CHAR, sizeof(caKeyPath));

	if(iArgc < 2)
	{
		//PrintHelp();
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

	sprintf(caKeyPath, "%s/%s",
				pArgs[1],
				KEY_DIR);

	pcTemp = pcAppname;
	while (*pcAppname)
	{
		*pcAppname = toupper((unsigned char) *pcAppname);
		pcAppname++;
	}
	pcAppname=pcTemp;

	if ( (iRetVal = KEY_GenerateKey(KEY_SHM, caKeyPath, pcAppname, &iKeyShm)) != SUCCESS )
	{
		LOG_ERROR(iRetVal,
					"Failed to Generate Shm key for App[%s] at Path[%s]",
					pcAppname,
					caKeyPath);
		LOG_DeInit();
		return iRetVal;
	}

	if ( (iRetVal = KEY_GenerateKey(KEY_SEM, caKeyPath, pcAppname, &iKeySem)) != SUCCESS )
	{
		LOG_ERROR(iRetVal,
					"Failed to Generate Sem key for App[%s] at Path[%s]",
					pcAppname,
					caKeyPath);
		LOG_DeInit();
		return iRetVal;
	}

	if ( (iRetVal = KEY_GenerateKey(KEY_MSGQ, caKeyPath, pcAppname, &iKeyMsgQ)) != SUCCESS )
	{
		LOG_ERROR(iRetVal,
		            "Failed to Generate MsgQ key for App[%s] at Path[%s]",
					pcAppname,
					caKeyPath);
		LOG_DeInit();
		return iRetVal;
	}

	LOG_DeInit();
	return SUCCESS;
}

#endif


/*
 * @}
 */





/* ~~~~~ END OF FILE ~~~~~ */

