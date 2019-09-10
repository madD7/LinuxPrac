/****************************************
Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 290717 * ** madD7 ** * Originator                                                       *

*/



#ifndef		__SIGNALHANDLELIB_H_
#define		__SIGNALHANDLELIB_H_



/* Include Files **************************************************************************
* @{
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/resource.h>
#include "constants.h"
#include "loglib.h"
/*
* @}
*/


/* Data Types & Variables *****************************************************************
 * @{
 */

/***************************************************************************************
 LOG LEVEL MACROS
*/

typedef RETVAL (*FuncSignalHandler_I_I) (INT32);

typedef RETVAL (*FuncAppExitHandler_V_I)(void);


/*
 * @}
 */



/* Functions ******************************************************************************
 * @{
 */


/***************************************************************************************
 LOG FUNCTION LIKE MACROS
*/

INT32 SIG_SendSig(pid_t iProcId, INT32 iSigNum);
RETVAL SIG_Init(FuncSignalHandler_I_I pfnSig, FuncAppExitHandler_V_I pfnAppEx);

/*
* @}
*/


#endif


/* ~~~~~ END OF FILE ~~~~~ */

