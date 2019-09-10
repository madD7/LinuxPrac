/****************************************

Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* DDMMYY * ** madD7 ** * Originator                                                       *

*/



#ifndef		__JSONLIB_H_     //Replace filename
#define		__JSONLIB_H_     //Replace filename



/* Include Files **************************************************************************
* @{
*/
#include <stddef.h>
#include "constants.h"
#include "loglib.h"

/*
* @}
*/


/* Data Types & Variables *****************************************************************
 * @{
 */

/***************************************************************************************
Data Structure Name	: JSNTYPE_T
Description	: JSON type identifier. 
				Basic types are:
						Object
						Array
						String
						Other primitive: number, boolean (true/false) or null
Notes		: None
*/
typedef enum {
  JSN_UNDEFINED = 0,
  JSN_OBJECT = 1,
  JSN_ARRAY = 2,
  JSN_STRING = 3,
  JSN_PRIMITIVE = 4
} JSNTYPE_T;

enum JSN_ERR {
  JSN_ERROR_NOMEM = -1,		/* Not enough tokens were provided */
  JSN_ERROR_INVAL = -2,		/* Invalid character inside JSON string */
  JSN_ERROR_PART = -3 		/* The string is not a full JSON packet, more bytes expected */
};


/***************************************************************************************
Data Structure Name	: JSON Token 
Description	: The Token Object definition
Notes		: None
*/
typedef struct{
	JSNTYPE_T iType;
	INT iStart;
	INT iEnd;
	INT iSize;
	#ifdef JSN_PARENT_LINKS
  	INT iParent;
	#endif
} JSNTOKEN;


/***************************************************************************************
Data Structure Name	: Json Parser
Description	: Contains array of available token blocks.
				Stores the string being parsed now &
				Current position in that string
Notes		: None
*/
typedef struct{
	UINT	uiPos;		/* Offset in the Json string */
	UINT	uiNxtTkn;	/* Next Token to allocate */ 
	INT		iSuprTkn;i	/* Superior token node. Eg: Parent Object or array */
} JSNPARSER;	


/*
* @}
*/



/* Functions ******************************************************************************
* @{
*/


/*
* @}
*/


#endif


/* ~~~~~ END OF FILE ~~~~~ */

