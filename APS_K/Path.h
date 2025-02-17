#pragma once
#include "stdafx.h"

#define		BASE_DIR			"D:\\EVMS"
#define		BASE_LOG_DIR		"D:\\EVMS\\LOG"
#define		LOG_DIR				"D:\\EVMS\\LOG\\EtcLog"
#define		BACKUP_DIR			"D:\\EVMS\\LOG\\Backup"
#define		IMAGE_BASE_DIR		"D:\\EVMS\\LOG\\ImageLog"
#define		ALARM_DIR			"D:\\EVMS\\LOG\\AlarmLog"



#define		BASE_AA_MODEL_LOAD_DIR	"D:\\EVMS\\APS_MODEL1"
#define		BASE_AA_BACKUP_DIR		"D:\\EVMS\\APS_MODEL1\\Backup"

#if (____AA_WAY == PCB_TILT_AA)
#define		AA_MODEL_LOAD_DIR		"D:\\EVMS\\APS_MODEL1\\PCBAA"
#define		AA_BACKUP_LOAD_DIR		"D:\\EVMS\\APS_MODEL1\\Backup\\PCBAA"

#else

#define		AA_MODEL_LOAD_DIR		"D:\\EVMS\\APS_MODEL1\\LENSAA"
#define		AA_BACKUP_LOAD_DIR		"D:\\EVMS\\APS_MODEL1\\Backup\\LENSAA"
#endif


//#elif  ( == GCM2)
//#define		ADO_RECORD_DIR		"D:\\EVMS\\GCM2"			//제품 생산 시 편차값 저장
//#define		IMG_DIR				"D:\\EVMS\\LOG\\ImageLog\\GCM2"
//#define		DATA_DIR			"D:\\EVMS\\GCM2\\Data"
//#define		MIU_DIR				"D:\\EVMS\\GCM2\\Initialize"
//#define		INSP_DIR			"D:\\EVMS\\LOG\\AALOG_GCM2"
//#define		STAIN_INSP_DIR		"D:\\EVMS\\LOG\\StainLog\\GCM2"
//#define		INSPTESTER_DIR		"D:\\EVMS\\LOG\\AACompleteLog_GCM2"
//#define		RESULT_DIR			"D:\\EVMS\\LOG\\Result"
//#define		ALARM_DIR			"D:\\EVMS\\LOG\\AlarmLog\\GCM2"
