/****************************************

Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* DDMMYY * ** madD7 ** * Originator                                                       *

*/



#ifndef		__KEY_H_     //Replace filename
#define		__KEY_H_     //Replace filename



/* Include Files **************************************************************************
 * @{
 */
#include <string.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "log.h"

/*
 * @}
 */


/* Data Types & Variables *****************************************************************
 * @{
 */
#define KEY_DIR		"KEYS"



typedef enum 
{
	KEY_SEM = 1,
	KEY_SHM,
	KEY_MSGQ
}KeyType;


#define KEY_UNKNOWN_STR "UNKNOWN"
#define KEY_SEM_STR		"SEM"
#define KEY_SHM_STR		"SHM"
#define KEY_MSGQ_STR	"MSGQ"


/*
 * @}
 */



/* Functions ******************************************************************************
 * @{
 */
RETVAL KEY_GetKeyforFile(CHAR *pcKeyString, key_t* piKey);
RETVAL KEY_GenerateKey(INT iKeyType, CHAR* pcKeyPath, CHAR* pcProcName, key_t* piKey);

/*
 * @}
 */


#endif


/* ~~~~~ END OF FILE ~~~~~ */

