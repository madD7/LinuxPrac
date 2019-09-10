/****************************************

Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* DDMMYY * ** madD7 ** * Originator                                                       *

*/

#ifndef		__CONSTANTS_H_     //Replace filename
#define		__CONSTANTS_H_     //Replace filename



/* Include Files **************************************************************************
 * @{
 */
#include <linux/limits.h>
#include <errno.h>


/*
 * @}
 */


/* Macros *********************************************************************************
 * @{ ASCII Characters
 */
#define NULL_CHAR					'\0'
#define TAB_CHAR                    '\t'
#define NL_CHAR                     '\n'
#define CR_CHAR                     '\r'
#define SPACE_CHAR                  ' '

#define SQ_BRCKT_OPEN_CHAR          '['
#define SQ_BRCKT_CLOSE_CHAR         ']'
#define CURL_BRCKT_OPEN_CHAR        '{'
#define CURL_BRCKT_CLOSE_CHAR       '}'
#define DBL_INVERTED_COMMA_CHAR     '\"'
#define COLON_CHAR                  ':'
#define COMMA_CHAR                  ','
#define BACKSLASH_CHAR				'\\'
#define FWDSLASH_CHAR				'/'

/***************************************************************************************
 RETURN STATUS MACROS
*/
#define SUCCESS		EXIT_SUCCESS
#define FAILURE		EXIT_FAILURE

/*
 * @}
 */


/* Data Types & Variables *****************************************************************
 * @{
 */

typedef char				CHAR;
typedef CHAR				INT8;

typedef unsigned char		UCHAR;
typedef UCHAR				UINT8;

typedef short int			SHORT;
typedef SHORT				INT16;

typedef unsigned short int	USHORT;
typedef USHORT				UINt16;

typedef int					INT;
typedef INT					INT32;

typedef unsigned int		UINT;
typedef UINT				UINT32;

typedef long				LONG;
typedef LONG				LINT32;

typedef	unsigned long		ULONG;
typedef ULONG				ULINT32;

typedef long long int		INT64;
typedef unsigned long long  UINT64;
typedef float				FLOAT;

typedef double 				DOUBLE;

typedef long double			LDOUBLE;

typedef LDOUBLE				DOUBLE96;


typedef	INT					RETVAL;
typedef CHAR FILEPATH[PATH_MAX];
typedef CHAR FILENAME[PATH_MAX];

typedef SIZE_T				size_t;


typedef void* (FuncThreadStart_pV_pV)(void *);

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

