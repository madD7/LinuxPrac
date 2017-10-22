#!/bin/bash
#/**************************************** MAD Engineers ****************************************
#    MAD Engineers
#     Copyright (c) 2014
#
# Description : 
#
#Revision History **************************************************************************
#** Date ** ** Coder ** ***** Remarks ******************************************************
#* 221017 * ** madD7 ** * Originator                                                       *
#*/


LOG_InitFunc()
{
	SYSCONF=System.conf
	LOGDIR_PATH_PATTERN=LOGDIR_PATH

	if [ -z "$CNTCTDTLS_ROOTDIR_PATH" ]
	then
		echo "FATAL: ROOTDIR Env variable not configured"
		exit -9
	fi

	CONFDIR_PATH=$CNTCTDTLS_ROOTDIR_PATH/Framework/conf
	if [ -z "$CONFDIR_PATH" ]
	then
		echo "FATAL: CONFDIR Env variable not configured"
		exit -9
	fi

	if [ ! -f "$CONFDIR_PATH/$SYSCONF" ]
	then
		echo "FATAL: Could not find $SYSCONF at $CONFDIR_PATH"
		exit -9
	fi

	LOGDIR_PATH=$(cat $CONFDIR_PATH/$SYSCONF | grep $LOGDIR_PATH_PATTERN | awk -F "=" '{print$2}')
	if [ -z "$LOGDIR_PATH" ]
	then 
		echo "FATAL: Unable to find $LOGDIR_PATH_PATTERN in $SYSCONF"
		exit -9
	fi

	InitChar=`echo $LOGDIR_PATH | cut -c 1`
	if [ "$InitChar" == "/" ]
	then
		if [ ! -d $LOGDIR_PATH ]
		then
			echo "INFO: $LOGDIR_PATH does not exist. Creating directory $LOGDIR_PATH"
			mkdir -p $LOGDIR_PATH

			if [ $? -ne 0 ]
			then 
				echo "FATAL: Unable to create directory $LOGDIR_PATH. Check for sufficient disk space and valid permissions."
				exit -9
			fi
		fi
	else
		echo "$LOGDIR_PATH is not an Absolute path"
		if [ ! -d $CNTCTDTLS_ROOTDIR_PATH/$LOGDIR_PATH ]
		then 
			echo "INFO: $CNTCTDTLS_ROOTDIR_PATH/$LOGDIR_PATH does not exist. Creating directory $CNTCTDTLS_ROOTDIR_PATH/$LOGDIR_PATH"
			mkdir -p $CNTCTDTLS_ROOTDIR_PATH/$LOGDIR_PATH
			if [ $? -ne 0 ]
			then
				echo "FATAL: Unable to create directory $CNTCTDTLS_ROOTDIR_PATH/$LOGDIR_PATH. Check for sufficient disk space and valid permissions."
				exit -9
			fi
		fi
		LOGDIR_PATH="$CNTCTDTLS_ROOTDIR_PATH/$LOGDIR_PATH"
	fi

	LOG_LOGFILENAME="$LOGDIR_PATH/`basename $0 .sh`_Script_"`date +%d_%m_%Y`.log
	touch $LOG_LOGFILENAME

	if [ $? -ne 0 ]
	then
		echo "FATAL: Unable to create $LOG_LOGFILENAME"
		exit -9
	else
		LOG_LOGFILE_CREATED="TRUE"
	fi

	export LOGDIR_PATH
	export LOG_LOGFILENAME
	export LOG_LOGFILE_CREATED

	return 0
}

LOG_LogFunc()
{
	if [ "$LOG_LOGFILE_CREATED" == "TRUE" ]
	then
		printf '%-12s | %-12s | %-9s | %s | %6s | %s | %s | %s-%d | %s\n' "`date "+%b %d %Y"`" "`date +%T.%N|cut -b1-12`" "PID=$$" "APP" " "
		"ERROR=     " "$*" "`basename $0`" "$BASH_LINENO[0]" "$FUNCNAME[1]" >> $LOG_LOGFILENAME

		echo -e "`date "+%b %d %Y | %H:%M:%S"` | PID=$$ | `basename $0 .sh` | $*" >> $LOG_LOGFILENAME
	else
		echo "Log is not initialised.... so exiting"
		exit 9
	fi
}


LOG_TeeFunc()
{
	if [ -z $LOG_LOGFILE_CREATED ]
	then
		printf '%-12s | %-12s | %-9s | %s | %6s | %s | %s | %s-%d | %s\n' "`date "+%b %d %Y"`" "`date +%T.%N|cut -b1-12`" "PID=$$" "APP" " "
		"ERROR=     " "$*" "`basename $0`" "$BASH_LINENO[0]" "$FUNCNAME[1]" | tee -a $LOG_LOGFILENAME

		echo -e "`date "+%b %d %Y | %H:%M:%S"` | PID=$$ | `basename $0 .sh` | $*" >> $LOG_LOGFILENAME
	else
	    echo "Log is not initialised.... so exiting"
	    exit 9
	fi
}


export -f LOG_InitFunc
export -f LOG_LogFunc
export -f LOG_TeeFunc

