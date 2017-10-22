#!/bin/bash

#/**************************************** MAD Engineers ****************************************
#    MAD Engineers
#	  Copyright (c) 2014
#
# Description : 
#
#Revision History **************************************************************************
#** Date ** ** Coder ** ***** Remarks ******************************************************
#* 221017 * ** madD7 ** * Originator                                                       *
#*/

cd

APPFOLDER=ContactDetails
ROOTDIR=`echo $CNTCTDTLS_ROOTDIR_PATH`

if [[ -z "${CNTCTDTLS_ROOTDIR_PATH}" ]]
then 
	SCRIPT=`echo ${BASH_SOURCE[0]}`
	SCR_PATH=`echo $(dirname $SCRIPT)`
	LOCATION="$( cd $SCR_PATH && pwd )"
	CNTCTDTLS_ROOTDIR_PATH=`echo ${LOCATION%$APPFOLDER*}`$APPFOLDER
	export CNTCTDTLS_ROOTDIR_PATH
fi

Chk=`grep APP_NAME $CNTCTDTLS_ROOTDIR_PATH/Framework/conf/System.conf | awk -F "=" '{print$2}'`
if [ -z $Chk ]
then 
	echo "Adding APP_NAME variable in System.conf"
	echo "APP_NAME=$APPFOLDER" >> $CNTCTDTLS_ROOTDIR_PATH/Framework/conf/System.conf
fi

Chk=""
Chk=`grep ROOTDIR_PATH $CNTCTDTLS_ROOTDIR_PATH/Framework/conf/System.conf | awk -F "=" '{print$2}'`
if [ -z $Chk ]
then
    echo "Adding ROOTDIR_PATH variable in System.conf"
	echo "ROOTDIR_PATH=$CNTCTDTLS_ROOTDIR_PATH" >> $CNTCTDTLS_ROOTDIR_PATH/Framework/conf/System.conf
fi


if [ -f "$SCR_PATH/LogFunctions.sh" ]
then
	chmod 755 $SCR_PATH/LogFunctions.sh

	if [ $? -ne 0 ]
	then
		echo "FATAL: Unable to change the permissions for LogFunctions.sh"
		exit 9
	fi

	$SCR_PATH/LogFunctions.sh
fi



