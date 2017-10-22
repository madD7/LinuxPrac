#!/bin/sh
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


set -x
LOG_InitFunc

LOG_LogFunc "Success"
exit 0



if [ $1 -eq 1]
then
	`$SQL_CMD CREATE TABLE IF NOT EXISTS PayDtls
		{
			GRP_NO		INTEGER NOT NULL PRIMARY_KEY,
			TITLE		VARCHAR(50),
			JOIN_DATE	DATE,
			SESS_TOTAL	INTEGER,
			HRDWR_TOTAL	INTEGER,
			INSTL1_DT	DATE,
			INSTL1_AMNT	INTEGER,
			INSTL2_DT	DATE,
			INSTL2_AMNT	INTEGER,
			INSTL3_DT	DATE
			INSTL3_AMNT	INTEGER,
			FNF_DT		DATE,
			FNF_AMNT	INTEGER
		}`
	
	if [ $? -ne 0 ]
	then
		echo "FATAL: Unable to create table"
		exit -9
	fi


	`$SQL_CMD CREATE TABLE IF NOT EXISTS GrpDtls
		{
			GRP_NO		INTEGER AUTO_INCREMENT NOT NULL PRIMARY_KEY,
			TITLE		VARCHAR(50),
			MEM1_NAME	VARCHAR(25),
			MEM1_PHN	INTEGER,
			MEM1_EMAIL	VARCHAR(40),
			MEM2_NAME	VARCHAR(25),
			MEM2_PHN    INTEGER,
			MEM2_EMAIL  VARCHAR(40),
			MEM3_NAME	VARCHAR(25),
			MEM3_PHN    INTEGER,
			MEM3_EMAIL  VARCHAR(40),
			MEM4_NAME	VARCHAR(25),
			MEM4_PHN    INTEGER,
			MEM4_EMAIL  VARCHAR(40),
			MEM5_NAME	VARCHAR(25),
			MEM5_PHN    INTEGER,
			MEM5_EMAIL  VARCHAR(40),
			BRNCH		VARCHAR(4),
			COLLEGE		VARCHAR(25),
			GUIDE		VARCHAR(25),
			HOD			VARCHAR(25),
			YR			INTEGER,
			GRAD		BOOLEAN,
			CAL_DATE	DATE
		}`
	
	if [ $? -ne 0 ]
	then
		echo "FATAL: Unable to create table"
		exit -9
	fi
else
	echo "Invalid arguments"
	echo "Eg: CreateTableStructGap.sh 1"
	exit -9
fi





