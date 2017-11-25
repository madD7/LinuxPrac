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

/*
* @}
*/


/* Data Types & Variables *****************************************************************
 * @{
 */


#define LOGS_ERROR			0
#define LOGS_WARN			1
#define LOGS_INFO			2



/*
 * @}
 */



/* Functions ******************************************************************************
 * @{
 */

void LOG_LogMsg(const char* pcfname, int ilineno, short slvl, const char* msg, ...);

#define LOG_ERROR(msg, ...)				LOG_LogMsg(__func__, __LINE__, LOGS_ERROR, msg, ## __VA_ARGS__)
#define LOG_WARN(msg, ...)				LOG_LogMsg(__func__, __LINE__, LOGS_WARN, msg, ## __VA_ARGS__)
#define LOG_INFO(msg, ...)				LOG_LogMsg(__func__, __LINE__, LOGS_INFO, msg, ## __VA_ARGS__)


/*
* @}
*/


#endif


/* ~~~~~ END OF FILE ~~~~~ */

