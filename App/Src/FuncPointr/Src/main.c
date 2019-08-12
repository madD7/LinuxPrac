/**************************************** MAD Engineers ****************************************

  MAD Engineers
  Copyright (c) 2014

Description	: Function Pointers - Dos and Donts

Theory:
		Function Pointers (FPs) are the pointers to a function - which means the program execution can
		jump/branch to the called function.
		An array of FPs is also known as Branch Table or a Jump Table.
		Branch tables are used bcoz of their unique blend of Compactness and execution speed.

		Note worthy points:
		1.	All functions to be named in a jump table should be prototyped. Prototyping is the best line 
			of defense against including a function that expects the wrong parameters, or returns the wrong type.

		2.	The function table is declared within the function that makes use of it (and, thus, static)

		3.	The array is made const signifying that we wish it to remain unchanged.

		4.	The indices into the array are unsigned, such that only single sided bounds checking need be done.

		5.	In this case, I have chosen to use the assert() macro to provide the bounds checking. 
			This is a good compromise between debugging ease and runtime efficiency.

		6.	A dummy function NullFunc() has been declared. This is used where array element is undefined.
			This is usually the most efficient method of handling an function array that is only partially populated.
		
		7.	The functions that are called need not be unique. 
			For example, a function such as FuncMenu may appear many times in the same jump table.



Revision History **************************************************************************
** Date ** ** Coder ** ***** Remarks ******************************************************
* 161217 * ** madD7 ** * Originator                                                       *

*/



/* Include Files **************************************************************************
 * @{
 */
#include <RandomFunctions.h>
/*
 * @}
 */


/* Variables ******************************************************************************
 * @{
 */

/* Declaration of Pointer to a function that take no args and returns nothing */
void (* pfFuncPointr)(void);

/* Declaration of an Array of 10 Function pointers */
/* 
Problems:
	1.	As can be seen, the array is of 10 elements (0-9), but contains only 5 elements.
		This can be a potential Bug/Error. Trying to call a function pointed by uninitialized elements, may lead to 
		segmentation faults or worse jump/branch to unknown location in code memory.
			To prevent problems, use a Null Function (Dummy Filler function) to fill unused poistions of Array.
			All functions named in the array must be prototyped.
   	2.	Also, this array will be in the RAM and not in code memory, thus increasing the RAM.
   		This can be potentially harmful in Environment with constrainted memory like Embedded platforms & uControllers.
	3.	Accessible by anyone.
	4.	Difficult to read
 */
void (* pfaArr[10])(void) = {Func1,Func2,Func3,Func4,Func5};


/* Declaration of Function Pointer using Typedef 
   Increases readability. 
   The downside is that it is not obvious that the variable is of the type Pointer to Function.
   But by following some naming convention, PFUNC_V_V indicates, pointer to function that takes void and returns void.
   		Thus, PFUNC_I_C is a pointer to function that accepts char and returns int.
		&	  PFUNC_I_cCP_D is a pointer to function that accpets const pointer to char, double and returns Int.
		and so on.
 */
typedef void (*PFUNC_V_V)(void);

/* Declaration of array */
// We will prefer a static const array of function pointers local to a function as explained above.
PFUNC_V_V pfvvArry[] = {Func1, Func2, Func3, Func4, Func5};

/*
 * @}
 */



/* Functions ******************************************************************************
 * @{
 */

/***************************************************************************************
Description	: Calls function from the pfaArr preventing potential bugs
Input		: iFuncNum - Is the Jump index (Specifies the position of Function Pointer in Array)
Output		: None
Returns		: None
Notes		: This is more safer method.
*/
void SaferFuncCall(unsigned int iFuncNum)
{
	/* All elements of the array are initialized (and non-modifiable).
	   Can be accesed only by this function.
	   const ensures that the array is in ROM. RAM saved ;) .
	 */
	static void (* const pfaArray[])(void) = {Func1,Func2,Func3,Func4,Func5};

	/* Call the function for valid iFuncNum */
	if ( iFuncNum < (sizeof(pfaArray)/sizeof(*pfaArray)) )
	{
		/* Check for Non Null. Not mandatory. */
		if( pfaArray[iFuncNum] != NULL )
		{
			pfaArray[iFuncNum]();			
		}
	}
	else
	{
		printf("Error: No function at position %d\n", iFuncNum);

	}
}


/***************************************************************************************
Description	: This function calls the function from the pfaArr
Input		: iFuncNum - Is the Jump index (Specifies the position of Function Pointer in Array)
Output		: None
Returns		: None
Notes		: This is not a safe way as no bounds checking. 
				Now, for erronous value of iFuncNum would result to disaster.
*/
void DangerousFuncCall(int iFuncNum)
{
	/* Call function pointed by the iFuncNum-th element of Array */
	pfaArr[iFuncNum]();
}


/***************************************************************************************
Description	: Main function - C entry function
Input		: None
Output		: None
Returns		: 0 - Success, else ErrorCode
Notes		: None
*/
int main()
{
	DangerousFuncCall(1);
	
	/* Invalid and unsafe */
	// DangerousFuncCall(11);

	SaferFuncCall(2);

	/* Invalid. But Safe */
	SaferFuncCall(10);

}

/*
 * @}
 */





/* ~~~~~ END OF FILE ~~~~~ */

