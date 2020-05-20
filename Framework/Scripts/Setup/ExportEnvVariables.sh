#!/bin/bash

#/**************************************** MAD Engineers ****************************************
#    MAD Engineers
#	  Copyright (c) 2014
#
# Description : Exports variables from the System.conf into the environment. 
#
# Revision History **************************************************************************
#** Date ** ** Coder ** ***** Remarks ******************************************************
#* 221017 * ** madD7 ** * Originator                                                       *
#*/

APP_NAME=LinuxPrac
ROOTDIR_PATH=`echo ${LINUX_DEVDIR}`

# ROOTDIR_PATH=""

# If $ROOTDIR is not set from bash_profile,
# but the script is run from bash_profile with the correct path,
# then the script path is set as ROOTDIR
if [[ -z "${ROOTDIR_PATH}" ]]
then 
	SCRIPT=`echo ${BASH_SOURCE[0]}`
	SCR_PATH=`echo $(dirname $SCRIPT)`
	LOCATION="$( cd $SCR_PATH && pwd )"
	
	#ROOTDIR_PATH=`echo ${LOCATION%$APPFOLDER*}`$APPFOLDER
	ROOTDIR_PATH=`echo ${LOCATION} | awk -F'/Framework' '{print $1}' `
	
	if [[ -d "${ROOTDIR_PATH}" ]]
	then
		export ROOTDIR_PATH
	else
		# Print to stderr 
		echo "Failed to export ROOTDIR_PATH [$ROOTDIR_PATH]. Path does not exists." 1>&2
		return 1
	fi
fi

SYSTEM_CONF_FILE="${ROOTDIR_PATH}/Framework/Config/System.conf"
if [[ ! -f "${SYSTEM_CONF_FILE}" ]]
then 
	echo "System conf File ["${SYSTEM_CONF_FILE}"] not found" 1&>2
	return 1
fi

export SYSTEM_CONF_FILE

Chk=""
Chk=`grep '^ROOTDIR_PATH' "${SYSTEM_CONF_FILE}" | awk -F "=" '{print$2}'`
if [[ -z "${Chk}" ]]
then
	echo "ROOTDIR_PATH=${ROOTDIR_PATH}" >> "${SYSTEM_CONF_FILE}"
fi

# Perform Inplace edit in System.conf whilst taking backup of the original file 
sed -i.setup "s,\${ROOTDIR_PATH},${ROOTDIR_PATH},g" "${SYSTEM_CONF_FILE}"

Chk=""
Chk=`grep '^APP_LOG_PATH' "${SYSTEM_CONF_FILE}" | awk -F "=" '{print$2}'`
if [[ -z "${Chk}" ]]
then
	APP_LOG_PATH="${ROOTDIR_PATH}/App/Logs"
    echo "APP_LOG_PATH=${APP_LOG_PATH}" >> "${SYSTEM_CONF_FILE}"
else
	APP_LOG_PATH=${Chk}
fi


if [[ ! -d "${APP_LOG_PATH}" ]]
then
	mkdir -p "${APP_LOG_PATH}"
	export APP_LOG_PATH 
fi

FSCR_PATH="${ROOTDIR_PATH}/Framework/Scripts"
if [[ -f "${FSCR_PATH}/LogFunctions.sh" ]]
then
	chmod 755 ${FSCR_PATH}/LogFunctions.sh
	if [[ $? -ne 0 ]]
	then
		echo "FATAL: Unable to change the permissions for LogFunctions.sh" 1>&2 
		exit 2
	fi

	# if executions is '. ./Scrname' the, script is run in current shell.
	# The defined varilables are available, even after completion of script.
	. ${FSCR_PATH}/LogFunctions.sh

else
	echo "FATAL: ${FSCR_PATH}/LogFunctions.sh script does not exists." 1>&2
	return 1
fi
