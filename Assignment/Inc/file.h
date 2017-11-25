/****************************************
Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 251117 * ** madD7 ** * Originator                                                       *
*/


#ifndef		__FILE_H_
#define		__FILE_H_



/* Include Files **************************************************************************
 * @{
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * @}
 */


/* Data Types & Variables *****************************************************************
 * @{
 */

/* FILE NAMES */
/* Currently only absolute path or relative path will work. */
#define PATH_TO_INPUT	"Input.txt"
#define PATH_TO_OUTPUT	"Output.txt"

/* BUFFER SIZES */
#define MAX_STRING_SIZE			2048
#define MAX_BUF_SIZE			100

/* CONSTANTS */
#define CLR_MEM_VAL				0
#define INIT_VAL				0

/* CHARACTER CONSTANTS */
#define STRING_TERMINATION_CH	'\0'
#define NEXTLINE_CH				'\n'
#define SQ_BRCKT_OPEN_CH		'['
#define SQ_BRCKT_CLOSE_CH		']'
#define EQUALS_CH				'='
#define SPACE_CH				' '

/* CONSTANT STRINGS */
#define APPLE_LEN_STR			"apple_len"
#define BEE_LEN_STR				"bee_len"
#define	CAT_LEN_STR				"cat_len"
#define APPLE_STR				"apple"
#define BEE_STR					"bee"
#define CAT_STR					"cat"
#define YES_STR					"Yes"
#define NO_STR					"No"

/* RETURN CODES */
#define SUCCESS					0
#define DYN_MEM_ALLOC_FAIL		1
#define FILE_OPN_FAIL			2
#define STRING_SIZE_EXCEEDED	3
#define EOF_REACHED				4
#define NOT_FOUND				5
#define CAT_STR_FOUND			7


/***************************************************************************************
Data Structure Name	: 
Description	: Holds value for strings and respective lengths
Notes		: None
*/
typedef struct
{
	char* pcApple;
	long   lAppleLen;
	char* pcBee;
	long   lBeeLen;
	char* pcCat;
	long   lCatLen;
}InfoStruct;

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

