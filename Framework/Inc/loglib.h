/****************************************
Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 290717 * ** madD7 ** * Originator                                                       *

*/



#ifndef		__LOGLIB_H_
#define		__LOGLIB_H_



/* Include Files **************************************************************************
* @{
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "constants.h"

/*
* @}
*/


/* Data Types & Variables *****************************************************************
 * @{
 */

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/***************************************************************************************
 LOG LEVEL MACROS
*/
#define LOGS_ERROR			0
#define LOGS_WARN			1
#define LOGS_INFO			2
#define LOGS_DEBUG			3

/***************************************************************************************
 PATHS & SEPERATOR STRING MACROS
*/
#define DEFAULT_LOG_DIR     "/var/log"
#define DIR_SEPERATOR       "/"
#define EXTN_SEPERATOR      "."
#define LOG_EXTENSION	    "log"

#define ENTER_FUNC_DBG_MSG	"Entering function"
#define EXIT_FUNC_DBG_MSG	"Leaving function"

/*
 * @}
 */



/* Functions ******************************************************************************
 * @{
 */
RETVAL LOG_Init(CHAR* pcaLogFilepath, CHAR *pcaAppName, INT iLogLevel);
void LOG_LogMsg(const CHAR* pcFName, const CHAR* pcFuncName, INT iLineno, INT iLvl,
			RETVAL iRetVal, const CHAR* cMsg, ...);

void LOG_DeInit();

/***************************************************************************************
 LOG FUNCTION LIKE MACROS
*/
#define LOG_ERROR(retval, msg, ...)			LOG_LogMsg(__FILENAME__, __func__, __LINE__, LOGS_ERROR, retval, msg, ## __VA_ARGS__)
#define LOG_WARN(retval, msg, ...)			LOG_LogMsg(__FILENAME__, __func__, __LINE__, LOGS_WARN, retval, msg, ## __VA_ARGS__)
#define LOG_INFO(retval, msg, ...)			LOG_LogMsg(__FILENAME__, __func__, __LINE__, LOGS_INFO, retval, msg, ## __VA_ARGS__)
#define LOG_DEBUG(retval, msg, ...)			LOG_LogMsg(__FILENAME__, __func__, __LINE__, LOGS_DEBUG, retval, msg, ## __VA_ARGS__)

#define LOG_FUNC_IN							LOG_LogMsg(__FILENAME__, __func__, __LINE__, LOGS_DEBUG, 0, ENTER_FUNC_DBG_MSG);
#define LOG_FUNC_OUT						LOG_LogMsg(__FILENAME__, __func__, __LINE__, LOGS_DEBUG, 0, EXIT_FUNC_DBG_MSG);

/*
* @}
*/


#endif


/* ~~~~~ END OF FILE ~~~~~ */

