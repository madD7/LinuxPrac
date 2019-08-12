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
 * @{
 */
#define NULL_CHAR	'\0'

/***************************************************************************************
 RETURN STATUS MACROS
*/
#define SUCCESS		0
#define FAILURE		-1

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

typedef short int			SHORTINT;
typedef SHORTINT			INT16;

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

