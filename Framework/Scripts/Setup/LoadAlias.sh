#!/bin/bash

#*************************************** MAD Engineers ***************************************
#MAD Engineers
#Copyright (c) 2014

#Description   : 
#				Loads aliases for navigation into the project folder 

#Revision History **************************************************************************
#** Date ** ** Coder ** ***** Remarks ******************************************************
#* DDMMYY * ** madD7 ** * Originator                                                       *
#

APP_NAME=LinuxPrac
ROOTDIR_PATH=`echo ${LINUX_DEVDIR}`

if [ -f "${ROOTDIR_PATH}" ]
then 
	echo "ROOTDIR_PATH [${ROOTDIR_PATH}] path not found" 1>&2 
	return 1
fi

# PATHS
FW_PATH="${ROOTDIR_PATH}/Framework"
FSRC_PATH="${FW_PATH}/Src"
FSCR_PATH="${FW_PATH}/Scripts"
FCONF_PATH="${FW_PATH}/Config"
FLOG_PATH="${FW_PATH}/Logs"

APP_PATH="${ROOTDIR_PATH}/App"
APPLOG_PATH="${APP_PATH}/Logs"
APPCONF_PATH="${APP_PATH}/Config"


# Aliases 
alias fpath="cd ${FW_PATH}"
alias fsrc="cd ${FSRC_PATH}"
alias fscr="cd ${FSCR_PATH}"
alias fconf="cd ${FCONF_PATH}"
alias flog="cd ${FLOG_PATH}"

alias apath="cd ${APP_PATH}"
alias logs="cd ${APPLOG_PATH}"


# End of File

