#!/bin/bash

#*************************************** MAD Engineers ***************************************
#MAD Engineers
#Copyright (c) 2014

#Description   : Creates a user name on the local system.
#					The script expects username as an argument to the script
#					Optionally, you may provide a comment for the account as
#					an argument to the script.
#					The script will automatically generate a random password.

#Revision History **************************************************************************
#** Date ** ** Coder ** ***** Remarks ******************************************************
#* DDMMYY * ** madD7 ** * Originator                                                       *
#

LogInitFunc

#Check for the Root permissions
CheckRootPermission()
{
	if [ "${UID}" -ne 0 ]
	then 
		LogFunc "You do not sufficient permissions. This will be reported."
		return 2
	fi
}





#### main ####

CheckRootPermission



# End of File
