/****************************************

Description	: Generic key_t generator, which can be used for msgget(), shmget(), semget()
			
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
RETVAL GenerateKey(CHAR *pcKeyString, key_t *pikey)
{
	INT iFd = 0;
	INT	iErrno=0;
	struct stat sFileStat;

	/* Open the Key File */
	iFd = open(pcKeyString, O_CREATE|O_RDWR, KEY_FILE_PERMISSION);
	iErrno = errno;
	if (iFd == -1)
	{
		LOG_ERROR(iErrno, "Error in creating the key file [%s] Error [%s]",
					pcKeyString,
					strerror(errno));
		
		return iErrno;
	}

	close(iFd);

	if ( lstat (pcKeyString, &sFileStat) == -1 )
	{
		iErrno=errno;
		LOG_ERROR(iErrno, "Error in getting inode number for the key file [%s] Error [%s]",
				pcKeyString,
				strerror(errno));

		return iErrno;
	}


	*piKey = sFileStat.st_ino;

	if ( *piKey == -1)
	{
		iErrno = -1;
		LOG_ERROR( iErrno,"Error in generating key for the key file [%s]",
					pcKeyString	);
		return iErrno;
	}

	return SUCCESS;
}

/*
 * @}
 */





/* ~~~~~ END OF FILE ~~~~~ */

