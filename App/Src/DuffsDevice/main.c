/**************************************** MAD Engineers ****************************************
Description	: 
			Duff's device is a way of manually implementing loop unrolling by interleaving two
			syntactic constructs of C: the do-while loop and a switch statement.
*/


#include <stdio.h>

/* Functions ******************************************************************************
 * @{
 */


void MyMemCpy(int* piDest, int* piSrc, int iCount)
{
	int n = (iCount+7)/8;
	
	switch(iCount % 8)
	{
		case 0:		do{	*piDest++ = *piSrc++;
		case 7:			*piDest++ = *piSrc++;
		case 6:			*piDest++ = *piSrc++;
		case 5:         *piDest++ = *piSrc++;
		case 4:         *piDest++ = *piSrc++;
		case 3:         *piDest++ = *piSrc++;
		case 2:         *piDest++ = *piSrc++;
		case 1:         *piDest++ = *piSrc++;
					}while (--n>0);
	}
}



int main()
{
	int iArr1[20]={0};
	int iArr2[20]={0};
	int i;

	for (i=0;i<sizeof(iArr1)/sizeof(int);i++)
	{
		iArr1[i] = i;
		printf("iArr1[%d] = %d \t iArr2[%d] = %d \n", i, iArr1[i], i, iArr2[i]);
	}

	printf("Performing memcopy using Duff's Device\n");

	MyMemCpy(iArr2, iArr1, sizeof(iArr1)/sizeof(int));

	for (i=0;i<sizeof(iArr1)/sizeof(int);i++)
	{
		printf("iArr[%d] = %d \n", i, iArr2[i]);
    }


	return 0;
}


/*
 * @}
 */





/* ~~~~~ END OF FILE ~~~~~ */

