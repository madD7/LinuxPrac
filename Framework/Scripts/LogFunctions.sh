#!/bin/bash
#/**************************************** MAD Engineers ****************************************
#    MAD Engineers
#     Copyright (c) 2014
#
# Description :
#				The script exports 4 shell functions that can be used for logging.
#				The functions give generic patterned log into log file.
#				The Log file is created with name - <ScriptName>_Script_DD_MM_YYYY.log
#
#				Log message pattern in log file
#	DDMMYYYY | HH:MM:SS.uSec | PID=<PID of Script> | APP | ERROR= <ErrorCode> | User Log Message | <ScriptName> +<LineNo> | <FunctionName>()
#
#				LogInitFunc - Creates Log file in the Log directory. 
#					The function must be called from with user script before using 
#					the Log functions
#				
#				LogFunc - Prints log message into the log file.
#					Agruments - ErrorCode
#								User log message
#					Syntax: 	LogFunc "ErrorCode" "User log message"
#
#				LogErrFunc - Prints log message into the log file and reports 
#							the log message with time stamp on stderr
#
#				LogTeeFunc - Prints log message into the log file and simultaneously to stdout
#
#
#Revision History **************************************************************************
#** Date ** ** Coder ** ***** Remarks ******************************************************
#* 221017 * ** madD7 ** * Originator                                                       *
#*/

LogInitFunc()
{
	SYSCONF_FILE=System.conf
	LOGDIR_PATH_PATTERN=APP_LOG_PATH

	if [ -z "${ROOTDIR_PATH}" ]
	then
		echo "FATAL: ROOTDIR Env variable not configured"
		exit -9
	fi

	if [ -z ${SYSTEM_CONF_FILE} ] 
	then 
		echo "FATAL: SYSTEM_CONF_FILE Env variable not configured"
        exit -9
	elif [ ! -f ${SYSTEM_CONF_FILE} ]
	then
		CONFDIR_PATH="${ROOTDIR_PATH}/Framework/Config"
		SYSTEM_CONF_FILE="${CONFDIR_PATH}/${SYSCONF_FILE}"
	
		if [ ! -f "${SYSTEM_CONF_FILE}" ]
		then
			echo "FATAL: Could not find ${SYSTEM_CONF_FILE} at ${CONFDIR_PATH}"
			exit -9
		fi
	fi

	LOGDIR_PATH=`grep "^${LOGDIR_PATH_PATTERN}" ${SYSTEM_CONF_FILE} | awk -F "=" '{print$2}'`
	if [ -z "$LOGDIR_PATH" ]
	then 
		echo "FATAL: Unable to find $LOGDIR_PATH_PATTERN in ${SYSTEM_CONF_FILE}"
		exit -9
	fi

	InitChar=`echo $LOGDIR_PATH | cut -c 1`
	if [ "$InitChar" == "/" ]
	then
		if [ ! -d "${LOGDIR_PATH}" ]
		then
			echo "INFO: ${LOGDIR_PATH} does not exist. Creating directory ${LOGDIR_PATH}"

			mkdir -p ${LOGDIR_PATH}
			if [ $? -ne 0 ]
			then 
				echo "FATAL: Unable to create directory ${LOGDIR_PATH}. Check for sufficient disk space and valid permissions."
				exit -9
			fi
		fi
	else
		echo "${LOGDIR_PATH} is not an Absolute path"
		if [ ! -d "${ROOTDIR_PATH}/${LOGDIR_PATH}" ]
		then 
			echo "INFO: ${ROOTDIR_PATH}/${LOGDIR_PATH} does not exist. Creating directory ${ROOTDIR_PATH}/${LOGDIR_PATH}"
			mkdir -p "${ROOTDIR_PATH}/${LOGDIR_PATH}"
			if [ $? -ne 0 ]
			then
				echo "FATAL: Unable to create directory ${ROOTDIR_PATH}/${LOGDIR_PATH}."
				echo "Check for sufficient disk space and valid permissions."
				exit -9
			fi
		fi
		LOGDIR_PATH="${ROOTDIR_PATH}/${LOGDIR_PATH}"
	fi

	LOG_LOGFILENAME="${LOGDIR_PATH}/`basename $0 .sh`_Script_"`date +%d_%m_%Y`.log
	touch ${LOG_LOGFILENAME}
	if [ $? -ne 0 ]
	then
		echo "FATAL: Unable to create ${LOG_LOGFILENAME}"
		exit -9
	else
		LOG_LOGFILE_CREATED="TRUE"
	fi

	export LOGDIR_PATH
	export LOG_LOGFILENAME
	export LOG_LOGFILE_CREATED

	return 0
}

LogFunc()
{
	ERR_CODE="${1}"
	shift
	LOG_MSG="${*}"

	if [ "${LOG_LOGFILE_CREATED}" == "TRUE" ]
	then
		printf ' %-9s | %-12s | %-9s | %s | %s%5d | %s | %s +%d | %s()\n' "`date "+%d%b%Y"`" "`date +%T.%N|cut -b1-12`" "PID=$$" "APP" "ERROR= " "${ERR_CODE}" "${LOG_MSG}" "`basename $0`" "${BASH_LINENO[0]}" "${FUNCNAME[1]}" >> ${LOG_LOGFILENAME}

#		echo -e "`date "+%b %d %Y | %H:%M:%S"` | PID=$$ | `basename $0 .sh` | $*" >> ${LOG_LOGFILENAME}
	else
		echo "Log is not initialised.... so exiting"
		exit 9
	fi

	return 0
}


LogErrFunc()
{
	ERR_CODE="${1}"
	shift
	LOG_MSG="${*}"

	printf ' %-9s %-12s | %s | %s \n' "`date "+%d%b%Y"`" "`date +%T.%N|cut -b1-12`" "$*" "`basename $0`" >&2

	if [ "${LOG_LOGFILE_CREATED}" == "TRUE" ]
	then
		printf ' %-9s | %-12s | %-9s | %s | %s%5d | %s | %s +%d | %s()\n' "`date "+%d%b%Y"`" "`date +%T.%N|cut -b1-12`" "PID=$$" "APP" "ERROR= " "${ERR_CODE}" "${LOG_MSG}" "`basename $0`" "${BASH_LINENO[0]}" "${FUNCNAME[1]}" >> ${LOG_LOGFILENAME}

#		echo -e "`date "+%b %d %Y | %H:%M:%S"` | PID=$$ | `basename $0 .sh` | $*" >> ${LOG_LOGFILENAME}
	else
		echo "Log is not initialised.... so exiting"
		exit 9
	fi

	return 0
}


LogTeeFunc()
{
	ERR_CODE="${1}"
	shift	
	LOG_MSG="${*}"

	if [ "${LOG_LOGFILE_CREATED}" == "TRUE" ]
	then
		printf ' %-9s | %-12s | %-9s | %s | %s%5d | %s | %s +%d | %s()\n' "`date "+%b %d %Y"`" "`date +%T.%N|cut -b1-12`" "PID=$$" "APP" "ERROR= " "${ERR_CODE}" "${LOG_MSG}" "`basename $0`" "${BASH_LINENO[0]}" "${FUNCNAME[1]}" | tee -a ${LOG_LOGFILENAME}

#		echo -e "`date "+%b %d %Y | %H:%M:%S"` | PID=$$ | `basename $0 .sh` | $*" >> ${LOG_LOGFILENAME}
	else
	    echo "Log is not initialised.... so exiting"
	    exit 9
	fi

	return 0
}


export -f LogInitFunc
export -f LogFunc
export -f LogErrFunc
export -f LogTeeFunc

