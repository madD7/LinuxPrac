#!/bin/bash

#*************************************** MAD Engineers ***************************************
#MAD Engineers
#Copyright (c) 2014

#Description   : Creates a user name on the local system.
#					The script expects username as an argument to the script
#					Optionally, you may provide a comment for the account as
#					an argument to the script.
#					The script will automatically generate a random password.
#
#					NOTE: " quotes around the variable name may not seem justified. 
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
	if [[ "${UID}" -ne 0 ]]
	then 
		LogErrFunc "2" "You do not sufficient permissions. This will be reported."
		exit 2
	fi
}


# Print script usage
PrintHelp()
{
	echo "Usage: ${0} [OPTIONS]... <username> [Comment]"
	echo "-a Creates an account on local system with <UserName>"
	echo "   with comments, if any." 
	echo "-c Used to add Comments while adding a new user"
	echo "-d Delete user"
	echo "-b Backup user directory to archieve path. Used with delete user option only."
	echo "-r Remove home directory"
	echo "-t Temporarily Disable user"
	exit 1
}


# Function accepts a modulus value and returns
# a random number less than the mod value
# ${1} - is not the input to script. Its argument to function
GetRandomNumWithIn()
{
	# Check if the range is passed to the function
	if [[ -z "${1}" ]]
	then
		TRUNCATE=false
	fi

	if [[ "${1}" -ge 1 ]]
	then 
		TRUNCATE=true
		MOD="${1}"
	else
		TRUNCATE=false
	fi

	while [[ true ]]
	do
		RANDOM_DIV=`echo ${RANDOM}`

		if [[ "${TRUNCATE}" == "true" ]]
		then
			RANDOM_DIV=`echo $(( ${RANDOM_DIV} % ${MOD}))`
		fi

		if [[ "${RANDOM_DIV}" -ge 1 ]]
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
	TEMP_PASSWD=""
	
	TEMP_PASSWD_LEN="${#TEMP_PASSWD}"

	while [ "${TEMP_PASSWD_LEN}" -le 10 ]
	do
		GetRandomNumWithIn 12
		RETVAL=${?}
	
		# Generate random character string with length less than 12
		RANDOM_STR=`cat /dev/random | tr -dc 'a-zA-Z0-9' | head -c${RETVAL}`

		GetSpecialChar
		GetRandomChksum

		TEMP_PASSWD=`echo ${TEMP_PASSWD}${RANDOM_STR}${SPC_CHAR}${RANDOM_VAL}`
		TEMP_PASSWD_LEN="${#TEMP_PASSWD}"
	done
}


# Adds new user, sets temporary passwd
AddUsr()
{
	# add user
	useradd -c "${COMMENTS}" -m "${USR_NAME}"
	RETVAL=${?}

	case ${RETVAL} in
		9)
			LogFunc "${RETVAL}" "User [${USR_NAME}] already exists"
			return ${RETVAL}
			;;

		0)
			# LogFunc "${RETVAL}" "User [${USR_NAME}] created successfully"
			;;

		?)
			LogFunc "${RETVAL}" "Failed to create account for User [${USR_NAME}]"
			return ${RETVAL}
			;;
	esac

	# Set password to the created user.
	#echo "${TEMP_PASSWD}" | passwd --stdin "${USR_NAME}" 

	# --stdin option is not available in Ubuntu.
	# instead use chpasswd 
	echo "${USR_NAME}:${TEMP_PASSWD}" | sudo chpasswd	

	RETVAL=${?}
	if [[ "${RETVAL}" -ne 0 ]]
	then 
		LogFunc "${RETVAL}" "Unable to set password for ${USR_NAME}"
		return ${RETVAL}
	fi

	# Expire passwd immediately. This forces user to 
	# set new passwd at next login
	passwd -e "${USR_NAME}"

	return 0
}


############# MAIN ###############

# Check arguments passed 
if [[ "${#}" -lt 1 ]]
then
	PrintHelp
	LogFunc "2" "No arguments passed"
	exit 2
fi

ADD_USR=0
DELETE_USR=0
DISABLE_USR=0

# get all arrguments 
while getopts a:b:c:d:rt: OPTIONS
do
	case ${OPTIONS} in
		a)
			ADD_USR=1
			USR_NAME="${OPTARG}"
			;;

		b)
			BACKUP='true'
			ARCHIVE_PATH="${OPTARG}"
			;;

		c)
			COMMENT='true'
			COMMENT_MSG="${OPTARG}"
			;;

		d)
			DELETE_USR=1
			USR_NAME="${OPTARG}"
			;;

		r)
			REMOVE_HOME_DIR="-r"
			;;

		## Temporarily disable user 
		t)
			DISABLE_USR=1
			USR_NAME="${OPTARG}"
			;;

		?)
			PrintHelp
			LogFunc "1" "Invalid Argument"
			exit 1
			;;
	esac
done

CheckRootPermission

# Check Username string length
if [[ -z "${USR_NAME}" ]]
then
	LogErrFunc "1" "Invalid username [${USR_NAME}] string"
	exit 1
fi

TEMP=$(( ${ADD_USR} + ${DELETE_USR} + ${DISABLE_USR} ))
if [[ "${TEMP}" -ne 1 ]]
then
	echo "-a -t and -d can not be used simultaneously"
	LogFunc "2" "Use either Add/Delete/Disable"
	PrintHelp
	exit 2
fi

# Check operation to be performed
if [[ "${ADD_USR}" -eq 1 ]]
then

	# The remaining arguments will be treated as comments
	# "${@}" - Expands to seperate (space seperated) arguments
	# "${*}" - Expands to a single argument 
	# COMMENT_MSG="${@}"

	# Generate a password 
	GenPasswd

	AddUsr
	RETVAL=${?}
	if [[ "${RETVAL}" -ne 0 ]] 
	then
		exit "${RETVAL}" 
	fi

	LogFunc "${RETVAL}" "Created new user [${USR_NAME}] with Passwd [${TEMP_PASSWD}] on Host [${HOSTNAME}]"

elif [[ "${DELETE_USR}" -eq 1 ]]
then
	USR_ID=`id -u "${USR_NAME}"`
	RETVAL=${?}
	if [[ "${RETVAL}" -ne 0 ]]
	then
		LogFunc "${RETVAL}" "Invalid Username [${USR_NAME}]"
		exit ${RETVAL}
	fi

	# UserID must be 1000 or greater.
	# UserID less than 1000 are reserved for System account
	if [ "${USR_ID}" -lt 1000 ]
	then
		LogErrFunc "2" "Permission deined to delete User [${USR_NAME}] [${USR_ID}]"
		exit 2
	fi

	if [[ "${BACKUP}" == "true" ]]
	then
		# Check if path exists
		if [[ ! -d "${ARCHIVE_PATH}" ]]
		then
			mkdir -p "${ARCHIVE_PATH}"
			RETVAL=${?}
			if [[ "${RETVAL}" -ne 0 ]]
			then
				LogErrFunc "${RETVAL}" "Unable to create Archive Path [${ARCHIVE_PATH}]"
				exit ${RETVAL}
			fi
		fi

		#Check for user home directory
		USR_HOME_PATH="/home/${USR_NAME}"
		if [[ -d "${USR_HOME_PATH}" ]]
		then
			ARCHIVE_FILE="${ARCHIVE_PATH}/${USR_NAME}.tgz"
			tar -zcf ${ARCHIVE_FILE} ${USR_HOME_PATH} > /dev/null  
			RETVAL=${?}
			if [[ ${RETVAL} -ne 0 ]]
			then
				LogErrFunc "${RETVAL}" "Failed to create Archive"
				exit ${RETVAL}
			fi

		else
			LogTeeFunc "1" "User home directory [${USR_HOME_PATH}] not found"
		fi
	fi

	# Delete user
	userdel ${REMOVE_HOME_DIR} ${USR_NAME}
	RETVAL=${?}
	if [[ ${RETVAL} -ne 0 ]]
	then
		LogErrFunc "${RETVAL}" "Unable to delete user [${USR_NAME}]"
		exit ${RETVAL}
	fi

	LogFunc "${RETVAL}" "User [${USR_NAME}] deleted successfully"

elif [[ "${DISABLE_USR}" -eq 1 ]]
then
	USR_ID=`id -u "${USR_NAME}"`
	RETVAL=${?}
	if [[ "${RETVAL}" -ne 0 ]]
	then
		LogFunc "${RETVAL}" "Invalid Username [${USR_NAME}]"
		exit ${RETVAL}
	fi

    # UserID must be 1000 or greater.
    # UserID less than 1000 are reserved for System account
    if [ "${USR_ID}" -lt 1000 ]
    then
        LogErrFunc "2" "Permission deined to delete User [${USR_NAME}] [${USR_ID}]"
        exit 2
    fi
	
	# Change User expiry date 
	chage -E 0 ${USR_NAME}
	RETVAL=${?}
	if [[ ${RETVAL} -ne 0 ]]
	then
		LogErrFunc "${RETVAL}" "Unable to disable user [${USR_NAME}]"
		exit ${RETVAL}
	fi

	LogFunc "${RETVAL}" "User [${USR_NAME}] account disabled successfully"	
fi


exit 0

# End of File
