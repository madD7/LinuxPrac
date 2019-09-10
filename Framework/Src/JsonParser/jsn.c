/****************************************

Description	:
			Json Parser Library code.
				Adopted from and with reference to Json Lite Library jsmn (jasmine),
				available on github https://github.com/zserge/jsmn.git
			
			The original library was modified to make it compatible with our framework.

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* DDMMYY * ** madD7 ** * Originator                                                       *

*/



/* Include Files **************************************************************************
 * @{
 */
#include "jsonlib.h"

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
Description	: Allocates a new token from Token Pool 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
static JSNTOKEN* JSN_AllocToken( JSNPARSER *psJsnPrsr, JSNTOKEN *psTknPool,
									const SIZE_T iTotalTkn )
{
	JSNTOKEN *psNew = NULL;

	if ( psJsnPrsr-> uiNxtTkn >= iTotalTkn )
	{
		return NULL;
	}

	psNew = &psTknPool[psJsnPrsr->uiNxtTkn++];
	psNew->iStart = -1;
	psNew->iEnd = -1;
	psNew->iSize = 0;
	
	#ifdef JSMN_PARENT_LINKS
	psNew->iParent = -1;
	#endif 

	return psNew;
}


/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
static JSN_SetToken(JSNTOKEN *psToken, const JSNTYPE_T iType,
					const INT iStart, const INT iEnd)
{
	psToken->iType = iType;
	psToken->iStart = iStart;
	psToken->iEnd = iEnd;
	psToken->iSize = 0;
}


/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
static RETVAL JSN_ParsePrimitive(JSNPARSER *psJsnPrsr, const CHAR *pcaJsnStr,
									const SIZE_T iStrLen, JSNTOKEN *psTokenPool,
									const SIZE_T iTotalTkn)
{
	JSNTOKEN *psTempTkn = NULL;
    INT iStart = psJsnPsrs->uiPos;
    CHAR cChar = 0;

    for (  ; (psJsnPrsr->uiPos < iStrLen) && (pcaStr[psJsnPrsr->uiPos] != NULL_CHAR) ;
            psJsnPrsr->uiPos++ )
    {
        cChar = pcaStr[psJsnPrsr->uiPos];

        switch (cChar)
        {
            #ifndef JSMN_STRICT
            /* In strict mode primitive must be followed by "," or "}" or "]" */
            case COLON_CHAR:
            #endif
            case SQ_BRCKT_CLOSE_CHAR:
            case CURL_BRCKT_CLOSE_CHAR:
            case TAB_CHAR:
            case NL_CHAR:
            case CR_CHAR:
            case SPACE_CHAR:
			case COMMA_CHAR:
				goto found;
            break;

			default:
			break;
        }

		if ( cChar < 32 || cChar > 127 )
		{
			psJsnPrsr->uiPos = iStart;
			return JSN_ERROR_INVAL;
		}
    }

	#ifdef JSMN_STRICT
  	/* In strict mode primitive must be followed by a comma/object/array */
  	psJsnPrsr->uiPos = iStart;
  	return JSN_ERROR_PART;
	#endif

	found:
		if( psTokenPool == NULL )	
		{
			psJsnPrsr->uiPos--;
			return SUCCESS;
		}

		psTempTkn = JSN_AllocToken(psJsnPrsr, psTokenPool, iTotalTkn);
		if ( psTempTkn == NULL )
		{
			psJsnPrsr->uiPos = iStart;
			return JSN_ERROR_NOMEM;
		}
		
		JSN_SetToken(psTempTkn, JSN_PRIMITIVE, iStart, psJsnPrsr-uiPos);
		
		#ifdef JSMN_PARENT_LINKS
		psTempTkn->iParent = psJsnPrsr->iSuperTkn;
		#endif

		psJsnPrsr->uiPos--;

	return SUCCESS;
}


/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
static RETVAL JSN_ParseString(JSNPARSER *psJsnPrsr, const CHAR *pcaStr,
                                    const SIZE_T iStrLen, JSNTOKEN *psTokenPool,
                                    const SIZE_T iTotalTkn)
{
	JSNTOKEN *psTempTkn = NULL;
	INT iStart = psJsnPsrs->uiPos;
	CHAR cChar = 0;
	INT iLoopCntr=0;
	
	for (  ; (psJsnPrsr->uiPos < iStrLen) && (pcaStr[psJsnPrsr->uiPos] != NULL_CHAR) ;
            psJsnPrsr->uiPos++ ) 
	{
		cChar = pcaStr[psJsnPrsr->uiPos];

		/* Indicates End of String */
		if ( cChar == DBL_INVERTED_COMMA_CHAR )
		{
			if ( psTokenPool == NULL )
			{
				return SUCCESS;
			}

			psTempTkn = JSN_AllocToken(psJsnPrsr, psTokenPool, iTotalTkn);
			if ( psTempTkn == NULL )
			{
				psJsnPrsr->uiPos = iStart;
				return JSMN_ERROR_NOMEM;
			}

			JSN_SetToken(psTempTkn, JSN_STRING, iStart+1, psTknPsrs->uiPos);
			#ifdef JSMN_PARENT_LINKS
	        psTempTkn->iParent = psJsnPrsr->iSuperTkn;
	        #endif

			return SUCCESS;
		}

   		if ( (cChar == BACKSLASH_CHAR) &&
				( (psJsnPrsr->uiPos+1) < iStrLen ) 
			)
		{	
			psJsnPrsr->uiPos++;
			cChar = pcaStr[psJsnPrsr->uiPos];

			switch (cChar)
    	    {
				case DBL_INVERTED_COMMA_CHAR:
   				case FWDSLASH_CHAR:
				case BACKSLASH_CHAR:
				case 'b':
				case 'f':
				case 'r':
				case 'n':
				case 't':
			        break;

				/* Allows escaped symbol \uXXXX */
				case 'u':
					psJsnPrsr->uiPos++;

					for (iLoopCntr = 0; 
							iLoopCntr < 4 && ( (psJsnPrsr->uiPos < iStrLen) && (pcaStr[psJsnPrsr->uiPos] != NULL_CHAR ) );
       				      	iLoopCntr++)
					{
						/* If it isn't a hex character we have an error */
						if ( ! ((pcaStr[psJsnPrsr->uiPos] >= '0' && pcaStr[psJsnPrsr->uiPos] <= '9') ||		/* 0-9 */
								(pcaStr[psJsnPrsr->uiPos] >= 'A' && pcaStr[psJsnPrsr->uiPos] <= 'F') ||		/* A-F */
								(pcaStr[psJsnPrsr->uiPos] >= 'a' && pcaStr[psJsnPrsr->uiPos] <= 'f'))		/* a-f */
							)
						{
				            psJsnPrsr->uiPos = iStart;
				            return JSN_ERROR_INVAL;
          				}
          				
						psJsnPrsr->uiPos++;
        			}
			
					psJsnPrsr->uiPos--;
					break;
				
				default:
					psJsnPrsr->uiPos = iStart;
					return JSN_ERROR_INVAL
					break;
			}
		}
	}

	psJsnPrsr->uiPos = iStart;
	return JSN_ERROR_PART;
}


/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
void JSN_Init(JSNPARSER *psJsnPrsr)
{
	psJsnPrsr->uiPos = 0;
	psJsnPrsr->uiNxtTkn = 0;
	psJsnPrsr->iSuprTkn = -1;
}


/***************************************************************************************
Description	: Parse Json string and Fill Tokens 
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
RETVAL JSN_PARSE(JSNPARSER *psJsnPrsr, const CHAR *pcaStr,
					const SIZE_T iStrLen, JSNTOKEN *psTokenPool,
					const UINT uiTotalTkn
					)
{
	RETVAL iRetVal = FAILURE;
	INT	iCntr = 0;
	JSNTOKEN *psTmpTkn = NULL;
	JSNTOKEN *psTmpChkTkn = NULL;
	INT iCount = psJsnPrsr->uiNxtTkn;
	CHAR cChar;
	JSNTYPE_T iJsnType;

	/* Parse String and check individual characters */ 
	for ( ; (psJsnPrsr->uiPos < iStrLen) && (pcaStr[psJsnPrsr->uiPos] != NULL_CHAR) ; 
			psJsnPrsr->uiPos++
		)
	{
		cChar = pcaStr[psJsnPrsr->uiPos];

		switch (cChar)
		{
			case SQ_BRCKT_OPEN_CHAR:
			case CURL_BRCKT_OPEN_CHAR:
				iCount++;

				if ( psTokenPool == NULL )
				{
					break;
				}

				psTmpTkn = JSN_AllocToken(psJsnPrsr, psTokenPool, uiTotalTkn);
				if ( psTmpTkn == NULL )
				{
					return JSN_ERROR_NOMEM
				}

				if ( psJsnPrsr->iSuperTkn != -1 )
				{
					psTmpChkTkn = &psTokenPool[psJsnPrsr->iSuperTkn];
					#ifdef JSMN_STRICT
			        /* In strict mode an object or array can't become a key */
			        if (psTmpChkTkn->iType == JSN_OBJECT) 
					{
						return JSN_ERROR_INVAL;
					}
					#endif
					
					psTmpChkTkn->iSize++;

					#ifdef JSMN_PARENT_LINKS
					psTmpTkn->iParent = psJsnPrsr->iSuperTkn;
					#endif
				}

				psTmpTkn->iType = ( cChar == CURL_BRCKT_OPEN_CHAR ? 
										JSN_OBJECT : JSN_ARRAY
									);

				psTmpTkn->iStart = psJsnPrsr->uiPos;
				psTmpTkn->iParent = psJsnPrsr->uiNxtTkn - 1;
			break;

			case SQ_BRCKT_CLOSE_CHAR:
			case CURL_BRCKT_CLOSE_CHAR:
				if ( psTokenPool == NULL)
				{
					break;
				}
				
				iType = ( cChar == CURL_BRCKT_OPEN_CHAR ?
                                   JSN_OBJECT : JSN_ARRAY
                         );

				#ifdef JSMN_PARENT_LINKS
				if (parser->toknext < 1) 
				{
					return JSN_ERROR_INVAL;
				}
				
				psTmpTkn = &psTokenPool[psJsnPrsr->uiNxtTkn - 1];

				for (;;) 
				{
					if ( (psTmpTkn->iStart != -1) && 
							(psTmpTkn->iEnd == -1)
					    )
					{
						if (psTmpTkn->iType != iType) 
						{
			            	return JSN_ERROR_INVAL;
						}
						
						psTmpTkn->iEnd = psJsnPrsr->iPos + 1;
						psJsnPrsr->iSuperTkn = psTmpTkn->iParent;
						break;
					}
					
					if ( psTmpTkn->iParent == -1) 
					{
						if ( (psTmpTkn->iType != iType) ||
								(psJsnPrsr->iSuperTkn == -1) 
						   )
						{
							return JSN_ERROR_INVAL;
						}
						break;
					}
					
					psTmpTkn = &psTokenPool[psTmpTkn->iParent];
				}
				#else
				for ( iCntr = psJsnPrsr->iNxtTkn - 1; 
						iCntr >= 0; iCntr--) 
				{
					psTmpTkn = &psTokenPool[iCntr];
					if ( psTmpTkn->iStart != -1 && 
							psTmpTkn->iEnd == -1) 
					{
						if (psTmpTkn->iType != iType) 
						{
							return JSN_ERROR_INVAL;
						}
						
						psJsnPrsr->iSuperTkn = -1;
						psTmpTkn->iEnd = psJsnPrsr->iPos + 1;
						break;
					}
				}

				/* Error if unmatched closing bracket */
				if (iCntr == -1) 
				{
					return JSN_ERROR_INVAL;
				}

				for (; iCntr >= 0; iCntr--)
				{
					psTmpTkn = &psTokenPool[iCntr];

					if ( psTmpTkn->iStart != -1 && 
							psTmpTkn->iEnd == -1) 
					{
						psJsnPrsr->iSuperTkn = iCntr;
						break;
					}
				}
				#endif
				break;

			case DBL_INVERTED_COMMA_CHAR:
				iRetVal = JSN_ParseString(psJsnPrsr, pcaStr, iStrLen, psTokenPool, iTotalTkn);
				if ( iRetVal != SUCCESS )
				{
					return iRetVal;
				}
				
				iCount++;

				if ( psJsnPrsr->iSuperTkn != -1 && 
						psTmpTkn == NULL )
				{
					psTokenPool[psJsnPrsr]->iSize++;
				}

				break;

			case TAB_CHAR:
			case NL_CHAR:
			case CR_CHAR:
			case SPACE_CHAR:
				break;

			case COLON_CHAR:
				psJsnPrsr->iSuperTkn = psJsnPrsr->iNxtTkn - 1;
				break;
			
			case COMMA_CHAR:
				if ( psTokenPool != NULL &&
						psJsnPrsr->iSuperTkn != -1 &&
						psTokenPool[psJsnPrsr->iSuperTkn].iType != JSN_ARRAY &&
						psTokenPool[psJsnPrsr->iSuperTkn].iType != JSN_OBJECT
					) 
				{
					#ifdef JSN_PARENT_LINKS
					psJsnPrsr->iSuperTkn = psTokenPool[psJsnPrsr->iSuperTkn].iParent;
					#else
					for ( iCntr = psJsnPrsr->iNxtTkn - 1; iCntr >= 0; iCntr--) 
					{
						if (psTokenPool[iCntr].iType == JSN_ARRAY || 
							psTokenPool[iCntr].iType == JSN_OBJECT) 
						{
							if ( psTokenPool[iCntr].iStart != -1 &&
									psTokenPool[iCntr].iEnd == -1) 
							{
								psJsnPrsr->iSuperTkn = iCntr;
								break;
							}
						}
					}
					#endif
				}
			break;

			#ifdef JSMN_STRICT
		    /* In strict mode primitives are: numbers and booleans */
		    case '-':
		    case '0':
		    case '1':
		    case '2':
		    case '3':
			case '4':
		    case '5':
		    case '6':
		    case '7':
		    case '8':
		    case '9':
		    case 't':
		    case 'f':
		    case 'n':
				/* And they must not be keys of the object */
				if ( psTokenPool != NULL &&
						psJsnPrsr->iSuperTkn != -1)
				{
        			psTmpChkTkn = &psTokenPool[psJsnPrsr->iSuperTkn];
					if (psTmpChkTkn->iType == JSMN_OBJECT ||
						(psTmpChkTkn->iType == JSMN_STRING && psTmpChkTkn->iSize != 0)
						)
					{
						return JSN_ERROR_INVAL;
					}
				}
			#else
		    /* In non-strict mode every unquoted value is a primitive */
		    default:
			#endif
				iRetVal = JSN_ParsePrimitive(psJsnPrsr, pcaStr, iStrLen, psTokenPool, iTotalTkn);
				if ( iRetVal != SUCCESS)
				{
					return iRetVal;
				}
				iCount++;
				
				if ( psJsnPrsr->iSuperTkn != -1 && psTokenPool != NULL) 
				{
					psTokenPool[psJsnPrsr->iSuperTkn].iSize++;
				}
				break;

				#ifdef JSMN_STRICT
				/* Unexpected char in strict mode */
				default:
					return JSN_ERROR_INVAL;
				#endif
		}
	}

	if ( psTokenPool != NULL)
	{
		for (iCntr = psJsnPrsr->iNxtTkn - 1; iCntr >= 0; iCntr--) 
		{
			/* Unmatched opened object or array */
			if ( psTokenPool[iCntr].iStart != -1 && psTokenPool[iCntr].iEnd == -1) 
			{
				return JSN_ERROR_PART;
			}
		}
	}
}

/*
 * @}
 */





/* ~~~~~ END OF FILE ~~~~~ */

