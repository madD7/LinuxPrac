/****************************************
Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 290717 * ** madD7 ** * Originator                                                       *

*/



#ifndef		__LOG_H_
#define		__LOG_H_



/* Include Files **************************************************************************
* @{
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "constants.h"

/*
* @}
*/


/* Data Types & Variables *****************************************************************
 * @{
 */

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
void LOG_LogMsg(const CHAR* pcFName, INT iLineno, INT iLvl, const CHAR* cMsg, ...);
void LOG_DeInit();

/***************************************************************************************
 LOG FUNCTION LIKE MACROS
*/
#define LOG_ERROR(msg, ...)				LOG_LogMsg(__FILE__, __LINE__, LOGS_ERROR, msg, ## __VA_ARGS__)
#define LOG_WARN(msg, ...)				LOG_LogMsg(__FILE__, __LINE__, LOGS_WARN, msg, ## __VA_ARGS__)
#define LOG_INFO(msg, ...)				LOG_LogMsg(__FILE__, __LINE__, LOGS_INFO, msg, ## __VA_ARGS__)
#define LOG_DEBUG(msg, ...)				LOG_LogMsg(__FILE__, __LINE__, LOGS_DEBUG, msg, ## __VA_ARGS__)

#define LOG_FUNC_IN						LOG_LogMsg(__FILE__, __LINE__, LOGS_DEBUG, ENTER_FUNC_DBG_MSG)
#define LOG_FUNC_OUT					LOG_LogMsg(__FILE__, __LINE__, LOGS_DEBUG, EXIT_FUNC_DBG_MSG)

/*
* @}
*/


#endif


/* ~~~~~ END OF FILE ~~~~~ */

