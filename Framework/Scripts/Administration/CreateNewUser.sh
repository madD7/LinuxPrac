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

cd 
. .bash_aliases

LogInitFunc

# String of Random special character
SPC_CHARS_STRING=`echo "!@#$%^&*()_+"`

#Check for the Root permissions
CheckRootPermission()
{
	if [ "${UID}" -ne 0 ]
	then 
		LogErrFunc "You do not sufficient permissions. This will be reported."
		exit 2
	fi
}


# Print script usage
PrintHelp()
{
	echo "Usage: ${0} <UserName> [Comment] ..."
	echo "Creates an account on local system with <UserName>"
	echo "with comments, if any." 
	exit 1
}


# Function accepts a modulus value and returns
# a random number less than the mod value
# ${1} - is not the input to script. Its argument to function
GetRandomNumWithIn()
{
	# Check if the range is passed to the function
	if [ -z "${1}" ]
	then
		TRUNCATE=false
	fi

	if [ "${1}" -ge 1 ]
	then 
		TRUNCATE=true
		MOD="${1}"
	else
		TRUNCATE=false
	fi

	while [ true ]
	do
		RANDOM_DIV=`echo ${RANDOM}`

		if [ "${TRUNCATE}" == "true" ]
		then
			RANDOM_DIV=`echo $(( ${RANDOM_DIV} % ${MOD}))`
		fi

		if [ "${RANDOM_DIV}" -ge 1 ]
		then
			return ${RANDOM_DIV}
		fi
	done
}


# Get a Random special character
# fold - wrap each line to specified width
# shuf - shuffles lines 
GetSpecialChar()
{
	SPC_CHAR=`echo ${SPC_CHARS_STRING} | fold -w1 | shuf | head -c1`
}

# Generates a random number using date 
GetRandomChksum()
{
	# date +%s - Epoch Time in secs, %N for nano seconds
	CURSEC=`date +%s%N`

	GetRandomNumWithIn 12
	RETVAL=${?}

	# ${RANDOM} is a shell variable that gets a random number each time  
	RANDOM_VAL=`echo ${CURSEC}${RANDOM}${RANDOM} | md5sum | head -c${RETVAL}`
}


# Temporary random Password
# tr - translate or delete characters 
GenPasswd()
{
	GetRandomNumWithIn 12
	RETVAL=${?}
	
	# Generate random character string with length less than 12
	RANDOM_STR=`cat /dev/random | tr -dc 'a-zA-Z0-9' | head -c${RETVAL}`

	GetSpecialChar
	GetRandomChksum

	TEMP_PASSWD=`echo ${RANDOM_STR}${SPC_CHAR}${RANDOM_VAL}`
}


# Adds new user, sets temporary passwd
AddUsr()
{
	# add user
	useradd -c "${COMMENTS}" -m "${USR_NAME}"
	RETVAL="${?}"
	if [ "${RETVAL}" -ne 0 ]
	then
		LogFunc "Unable to create account"
		return "${RETVAL}"
	fi

	# Set password to the created user.
	#echo "${TEMP_PASSWD}" | passwd --stdin "${USR_NAME}" 

	# --stdin option is not available in Ubuntu.
	# instead use chpasswd 
	echo "${USR_NAME}:${TEMP_PASSWD}" | sudo chpasswd	

	RETVAL="${?}"
	if [ "${RETVAL}" -ne 0 ]
	then 
		LogFunc "Unable to set password for ${USR_NAME}"
		return "${RETVAL}"
	fi

	# Expire passwd immediately. This forces user to 
	# set new passwd at next login
	passwd -e "${USR_NAME}"

	return 0
}

############# MAIN ###############

CheckRootPermission

# Check arguments passed 
if [ "${#}" -lt 1 ]
then
	PrintHelp
fi

# The first parameter is username
USR_NAME="${1}"

# shift the other arguments 
shift

# The remaining arguments will be treated as comments
# "${@}" - Expands to seperate (space seperated) arguments
# "${*}" - Expands to a single argument 
COMMENTS="${@}"

# Generate a password 
GenPasswd

AddUsr
RETVAL="${?}"
if [ "${RETVAL}" -ne 0 ] 
then
	exit "${RETVAL}" 
fi

LogFunc "Created new user [${USR_NAME}] with Passwd [${TEMP_PASSWD}] on Host [${HOSTNAME}]"
exit 0

# End of File
