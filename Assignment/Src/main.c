/***************************************
Description	: 

Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 251117 * ** madD7 ** * Originator                                                       *

*/



/* Include Files **************************************************************************
 * @{
 */

#include "log.h"
#include "file.h"

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
Description	: Skips the String upto the first occurance of '=' character. pcRdAdd points the first character after the '=' character
Input		: pcRdAdd		- Pointer to pointer to string
Output		: None
Returns		: SUCCESS 		- if '=' is found in string
			: NOT_FOUND 	- if '=' is not found in whole string  
Notes		: 
*/
int SkipUpto(char** pcRdAdd, char cSkipCh)
{
	int iCount=INIT_VAL;
	char *pcRd = *pcRdAdd;

	/* Skip all characters upto first equals */
	while ((*pcRd != STRING_TERMINATION_CH) && (*pcRd != cSkipCh) && (iCount < MAX_BUF_SIZE))
	{
		iCount++;
		pcRd++;
	}

	pcRd++;

	if( (iCount > MAX_BUF_SIZE) || (*pcRd == STRING_TERMINATION_CH) )
		return NOT_FOUND;
	else
	{
		*pcRdAdd = pcRd;
		return SUCCESS;
	}
}

/***************************************************************************************
Description	: Convert the string into equivalent long value 
Input		: pcRd			- Pointer to Input string
			: plCnt 		- Pointer to long where the count is to be stored
Output		: None
Returns		: SUCCESS	 	- Successful conversion
			: NOT_FOUND		- If '=' character is not found in input string.
Notes		: None
*/
int GetCount(char *pcRd, long *plCnt)
{
	int iCount=INIT_VAL;
	int iRetval=SUCCESS;
	long lVal=INIT_VAL;
	char cPostBuf[MAX_BUF_SIZE +1]={0};
	char *pcPostBuf=cPostBuf;
	
	iRetval = SkipUpto(&pcRd,EQUALS_CH);
	if(iRetval != SUCCESS)
	{
		LOG_ERROR("Failure in finding '=' character in string");
		return iRetval;
	}

	iCount=INIT_VAL;
	
	while ((*pcRd != STRING_TERMINATION_CH) && (*pcRd != SQ_BRCKT_CLOSE_CH) && (iCount < MAX_BUF_SIZE))
	{
		cPostBuf[iCount] = *pcRd++;
		iCount++;
	}

	iRetval = SkipUpto(&pcPostBuf, SPACE_CH);

    sscanf(pcPostBuf, "%ld", &lVal);
	*plCnt = lVal;

	return iRetval;

}

/***************************************************************************************
Description	: Get next line from Input File
Input		: pfInp			- Input file pointer
			: pNxtLine		- Pointer to buffer where the next line is to be stored
Output		: None
Returns		: EOF_REACHED		- if File is read completely
			: MAX_STRING_SIZE	- if string exceeds size of the storage buffer (pointed by pNxtLine)
			: cStartCh			- Start character of line (not of much importance)
Notes		: None
*/
int GetNxtLine(FILE *pfInp, char *pNxtLine)
{
	char cNxtCh=INIT_VAL;
	char cStartCh=INIT_VAL;
	int iCount=INIT_VAL;
	int iRetval=SUCCESS;
	static long iCurOffPos=INIT_VAL;

	memset(pNxtLine, CLR_MEM_VAL, MAX_STRING_SIZE);

	do
	{
		cNxtCh = fgetc(pfInp);

		if(iCount == INIT_VAL)
		{
			cStartCh = cNxtCh;
		}

		*pNxtLine++ = cNxtCh;
		iCurOffPos++;
	}while( (cNxtCh != NEXTLINE_CH) && (cNxtCh != EOF) && (iCount++ < MAX_STRING_SIZE) );

	if(cNxtCh == EOF)
		iRetval = EOF_REACHED;
	else if (iCount >= MAX_STRING_SIZE)
		iRetval = STRING_SIZE_EXCEEDED;
	else
	{	
		pNxtLine--;
		*pNxtLine = STRING_TERMINATION_CH;
		iRetval = cStartCh;
	}

	return iRetval;
}


/***************************************************************************************
Description	: 
Input		: 
Output		: None
Returns		: SUCCESS 
Notes		: None
*/
int ParseLineData(char* pcRd, char* pcWr, InfoStruct* psStruct)
{
	char cStartCh=INIT_VAL;
	int iCount=INIT_VAL;
	int iRetval=SUCCESS;
	char cPreBuf[MAX_BUF_SIZE +1]={0};

	strcpy(pcWr, pcRd);

	cStartCh = *pcRd;
	if (cStartCh == SQ_BRCKT_OPEN_CH)
		pcRd++;

	do
	{
		cPreBuf[iCount] = *pcRd++;
	}while ( (*pcRd != STRING_TERMINATION_CH) && (*pcRd != EQUALS_CH) && (iCount++ < MAX_BUF_SIZE) );

	if( cStartCh == SQ_BRCKT_OPEN_CH)
	{
		if( !strncmp(APPLE_LEN_STR,cPreBuf, strlen(APPLE_LEN_STR)) )
		{
			iRetval = GetCount(pcRd, &psStruct->lAppleLen);
		}
		else if ( !strncmp(BEE_LEN_STR, cPreBuf, strlen(BEE_LEN_STR)) )
		{
			iRetval = GetCount(pcRd, &psStruct->lBeeLen);
		}
		else if ( !strncmp(CAT_LEN_STR, cPreBuf, strlen(CAT_LEN_STR)) )
		{
			iRetval = GetCount(pcRd, &psStruct->lCatLen);
		}
	}
	else if ( isalpha(cStartCh) )
	{
		if( !strncmp(APPLE_STR,cPreBuf,strlen(APPLE_STR)) )
		{
			iRetval = SkipUpto(&pcRd,EQUALS_CH);
			if(iRetval != SUCCESS)
			{
				LOG_ERROR("Failure in finding '=' character in string");
				return NOT_FOUND;
			}

			memcpy(psStruct->pcApple,pcRd,MAX_STRING_SIZE);
		}
		else if ( !strncmp(BEE_STR, cPreBuf,strlen(BEE_STR)) )
		{
			iRetval = SkipUpto(&pcRd,EQUALS_CH);
			if(iRetval != SUCCESS)
			{
				LOG_ERROR("Failure in finding '=' character in string");
				return NOT_FOUND;
			}

			memcpy(psStruct->pcBee, pcRd, MAX_STRING_SIZE);
		}
		else if ( !strncmp(CAT_STR, cPreBuf,strlen(CAT_STR)) )
		{
			iRetval = SkipUpto(&pcRd,EQUALS_CH);
			if(iRetval != SUCCESS)
			{
				LOG_ERROR("Failure in finding '=' character in string");
				return NOT_FOUND;
			}

			memcpy(psStruct->pcCat, pcRd, MAX_STRING_SIZE);

			iRetval = CAT_STR_FOUND;
		}
	}

	return iRetval;
}



/***************************************************************************************
Description	: Process some data
Input		: 
Output		: None
Returns		: 0 
			: 1
Notes		: None
*/
int do_the_work(char * apple_binary, int apple_len, char * bee_binary, int bee_len, char * cat_binary, int cat_len)
{
	static	int iRetval=1;

	/*
	   Do Some processing
	   and update iRetval flag to 0 or 1
	 */
	
	return iRetval;

}


/***************************************************************************************
Description	: C entry function
Input		: 
Output		: None
Returns		: 
Notes		: None
*/
int main(int iArgc, char* pcArgv[])
{
	int iRetval=SUCCESS;
	char *pcRdLine, *pcWrLine;
	InfoStruct sStruct;
	FILE *pfInpFile;
	FILE *pfOutFile;
	char *pcApple, *pcBee, *pcCat;

	pcRdLine = (char*)calloc(MAX_STRING_SIZE+1, sizeof(char));
	if (pcRdLine == NULL)
	{
		LOG_ERROR("Failure in allocating memory");
		exit (DYN_MEM_ALLOC_FAIL);
	}

	pcWrLine = (char*)calloc(MAX_STRING_SIZE+1, sizeof(char));
	if (pcWrLine == NULL)
	{
		LOG_ERROR("Failure in allocating memory");
		
		/* 
		   Though a good OS always reclaims dynamically allocated memory after exit() function call, 
		   memory free is a good practice and would be encouraged hence forth. 
		   We never know a Good programmer may rewrite exit() some day in future & may leave memory leaks  ;) *wink*  
		 */
		free(pcRdLine);
		exit (DYN_MEM_ALLOC_FAIL);
	}

	pcApple = (char*)calloc(MAX_STRING_SIZE+1, sizeof(char));
	if(pcApple == NULL)
	{
		LOG_ERROR("Failure in allocating memory");
		free(pcRdLine);
		free(pcWrLine);

		exit(DYN_MEM_ALLOC_FAIL);
	}
	
	pcBee = (char*)calloc(MAX_STRING_SIZE+1, sizeof(char));
	if(pcBee == NULL)
	{
		LOG_ERROR("Failure in allocating memory");
		free(pcRdLine);
        free(pcWrLine);
		free(pcApple);
		exit(DYN_MEM_ALLOC_FAIL);
	}
	
	pcCat = (char*)calloc(MAX_STRING_SIZE+1, sizeof(char));
	if(pcCat == NULL)
	{	
		LOG_ERROR("Failure in allocating memory");
		free(pcRdLine);
		free(pcWrLine);
		free(pcApple);
		free(pcBee);
		exit(DYN_MEM_ALLOC_FAIL);
	}

	pfInpFile = fopen(PATH_TO_INPUT, "r");
	if(pfInpFile == NULL)
	{
		LOG_ERROR("Failure in opening the file: %s", PATH_TO_INPUT);
		free(pcRdLine);
		free(pcWrLine);
		free(pcApple);
		free(pcBee);
		free(pcCat);
		exit (FILE_OPN_FAIL);
	}

	pfOutFile = fopen(PATH_TO_OUTPUT, "w+");
	if (pfOutFile == NULL)
	{
		LOG_ERROR("Failure in opening the file: %s", PATH_TO_OUTPUT);
		free(pcRdLine);
		free(pcWrLine);
		free(pcApple);
	    free(pcBee);
	    free(pcCat);
		fclose(pfInpFile);
		exit (FILE_OPN_FAIL);
	}
	sStruct.pcApple = pcApple;
	sStruct.pcBee = pcBee;
	sStruct.pcCat = pcCat;
	
	do 
	{
		memset(pcRdLine, CLR_MEM_VAL, MAX_STRING_SIZE+1);
		iRetval = GetNxtLine(pfInpFile, pcRdLine);
		if(iRetval == STRING_SIZE_EXCEEDED)
		{
			LOG_ERROR("Length of Line exceeded max allowable size");
			break;
		}
		else if(iRetval == EOF_REACHED)
		{
			LOG_INFO("End of File reached");
			break;
		}

		LOG_INFO("%s", pcRdLine);

		memset(pcWrLine, CLR_MEM_VAL, MAX_STRING_SIZE+1);
		iRetval = ParseLineData(pcRdLine, pcWrLine, &sStruct);
		
		fprintf(pfOutFile, "%s\n", pcWrLine);
		if(iRetval == CAT_STR_FOUND)
		{
			LOG_INFO("#############\napple_len=%ld \napple=%s \n\nbee_len=%ld \nbee=%s \n\ncat_len=%ld \ncat=%s\n\n",
						sStruct.lAppleLen,sStruct.pcApple, sStruct.lBeeLen,sStruct.pcBee, sStruct.lCatLen,sStruct.pcCat);

			iRetval = do_the_work(sStruct.pcApple, sStruct.lAppleLen, sStruct.pcBee, sStruct.lBeeLen, sStruct.pcCat, sStruct.lCatLen);
			if(iRetval == SUCCESS)
				fprintf(pfOutFile, "%s\n", NO_STR);
			else
				fprintf(pfOutFile, "%s\n", YES_STR);
		}

	}while((iRetval != EOF_REACHED));

	free(pcRdLine);
	free(pcWrLine);
	free(pcApple);
    free(pcBee);
    free(pcCat);
	fclose(pfInpFile);
	fclose(pfOutFile);

	return iRetval;
}

/*
 * @}
 */





/* ~~~~~ END OF FILE ~~~~~ */

