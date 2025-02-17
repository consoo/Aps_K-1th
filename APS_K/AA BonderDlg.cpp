




// AA BonderDlg.cpp : ±¸Çö ÆÄÀÏ
//

#include "stdafx.h"
#include "AA Bonder.h"
#include "AA BonderDlg.h"
#include "afxdialogex.h"

/* Dialog */
#include "LensDlg.h"
#include "LensEdgeDlg.h"
#include "PcbDlg.h"
#include "CcdDlg.h"

#include "CCDSFRDlg.h"
#include "CCDInspModeDlg.h"

#include "LensTeaching.h"
#include "PcbTeaching.h"
#include "PcbTeachingDis.h"
#include "IoDlg.h"
#include "ModelDlg.h"
#include "SfrSpec.h"
#include "ChartSetDlg.h"
#include "Insp_Spec_Set.h"
#include "AutoDispDlg.h"
#include "AlarmDialog.h"
#include "LightDlg.h"
#include "ImageInsp/Inspection_Alg.h"
#include "InformDlg.h"
#include "EEPROMTest.h"

//

//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CInspection_Alg Alg;
//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNgList_Data		NgList_Data;

/* Dialog */
CSfrSpec*					sfrSpecDlg = NULL;
CChartSetDlg*				chartSetDlg = NULL;
CLensDlg*					lensDlg = NULL;
CLensEdgeDlg*			lensEdgeDlg = NULL;
CPcbDlg*					pcbDlg = NULL;
CCcdDlg*					ccdDlg = NULL;
CMotorDlg*				motorDlg = NULL;
CMotorDlg2*				motorDlg2 = NULL;
CMotorDlg3*				motorDlg3 = NULL;
CIoDlg*						ioDlg = NULL;
CModelDlg*				modelDlg = NULL;
CAutoDispDlg*			autodispDlg = NULL;
CInsp_Spec_Set*		InspSpecSet = NULL;
CLightDlg*					lightDlg = NULL;
CLightDlg			*m_pLEDDlg;
CEEPROMTest*		eepromDlg = NULL;



#include <mmsystem.h>				// ¸ÖÆ¼ ¹Ìµð¾î Å¸ÀÌ¸Ó..
#pragma comment(lib, "winmm.lib")
MMRESULT		m_mmResult;			//¸ÖÆ¼¹Ìµð¾îÅ¸ÀÌ¸Ó..



//CInformDlg*		InformDlg;
//CForceAlignDlg* g_pFoceDlg = NULL;
//CTiltingManualDlg* TiltingManualdlg;//Manual Æ¿ÆÃ ÆíÂ÷ ÀÔ·ÂÃ¢


//CDPoint		LaserPos[4];
//double		LaserValue[4];
//CPoint cpFiducialPos[4];
//CRect crFiducialRoi[4];

//bool bInsCenter = false;	


/* Thread */
/************************************************************************/

//! Modified by LHW (2013/2/25)
//! ¿µ»ó Ãëµæ ¼Óµµ¸¦ ¿Ã¸®±â À§ÇÏ¿© ¿©·¯ °³ÀÇ Thread·Î ¿ªÇÒÀ» ³ª´©¾î¼­ ÀÛ¾÷ÇÑ´Ù.
//! 4°³ÀÇ Thread·Î Ç×»ó Â¦ÀÌ µÇ¾î, »ý¼º/ÇØÁ¦ µÇ¾î¾ß ÇÑ´Ù.
bool bThreadCcmGrab = false;
bool bThreadCcmGrabRun = false;
bool bFlag_First_Grab_Display = false;
CWinThread* pThread_CCM_Grab     = NULL;
CWinThread* pThread_CCM_CvtColor = NULL;
CWinThread* pThread_CCM_CvtMil   = NULL;
CWinThread* pThread_CCM_Display  = NULL;
CWinThread* pThread_CCM_Mil_CvtColor_Ready = NULL;
CWinThread* pThread_CCM_Mil_CvtColor = NULL;
CWinThread* pThread_CCM_Auto_Exposure_Time = NULL;
CWinThread* pThread_CCM_Auto_WhiteBalance = NULL;
CWinThread*	pThread_CheckDate  = NULL;

CWinThread* pThread_TaskOrigin = NULL;
bool bThreadOriginRun = false;
//

CWinThread* pThread_TaskUv = NULL;
bool bThreadUvRun = false;
//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Added by LHW (2013/4/21)
bool bThreadServoAlarm = false;
bool bThreadServoAlarmRun = false;
CWinThread* pThread_ServoAlarm = NULL;	//! Added by LHW (2013/4/19)
//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool bThreadTaskLens = false;
bool bThreadTaskLensRun = false;
CWinThread* pThread_TaskLens = NULL;
bool bThreadTaskLens_Align = false;
bool bThreadTaskLensRun_Align = false;
bool bThreadTaskPcb = false;
bool bThreadTaskPcbRun = false;
CWinThread* pThread_TaskPcb = NULL;

bool bThreadTaskReady= false;
bool bThreadTaskReadyRun = false;
int	 iReadyRunCnt = 0;
CWinThread* pThread_TaskReady = NULL;

bool bThreadLenVac= false;
CWinThread* pThread_LenVac = NULL;


bool bThreadTaskOsCheckRun = false;
CWinThread* pThread_TaskOsCheck = NULL;


bool bThread_MIUCheckRun = false;
CWinThread* pThread_MIUCheck = NULL;

bool bThreadMonitor = false;
bool bThreadMonitorRun = false;
CWinThread* pThread_Monitor = NULL;


bool bThreadClock = false;
bool bThreadClockRun = false;
CWinThread* pThread_Clock = NULL;


bool bThreadGrab = false;
bool bThreadGrabRun = false;
CWinThread* pThread_Grab = NULL;

bool bThreadSocket = false;
CWinThread* pThread_SocketRead = NULL;

CWinThread* pThread_CCDCheck = NULL;


bool bThreadEpoxyRun = false;
CWinThread* pThread_Epoxy = NULL;
/************************************************************************/


/* Àü¿ª º¯¼ö */
bool	g_chkdateTH_flag = false;



//! Added by LHW (2013/2/25)
//!	USB Restart Event
void CALLBACK USBEventHandler(char iIndex, int Event)
{
	//CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	//switch(Event)
	//{
	//case MIU_EVENT_TIMEOUT:
	//	{
	//		theApp.MainDlg->putListLog("USB ¿µ»ó È¹µæ TimeOut ¹ß»ý");
	//		//MIU.Close();					// 95 ~ 100 msec
	//		errMsg2(Task.AutoFlag, "USB ¿µ»ó È¹µæ TimeOut ¹ß»ý.");
	//	}
	//	break;

	//case MIU_EVENT_ATTACHED:
	//	{
	//		theApp.MainDlg->putListLog("USB attached Event");
	//		TRACE("USB attached Event \n");
	//		/*if(!MIU.Open())	
	//		{
	//			theApp.MainDlg->m_bMiuRun = false;
	//			theApp.MainDlg->putListLog("MIU Open ½ÇÆÐ.");
	//		}*/
	//		//int iSize_Device = MIU.Search_Device();
	//		//if ( iSize_Device > 0 )
	//		//{
	//		//	if ( gMIUDevice.bMIUOpen == 0 )
	//		//	{
	//		//		//! LPMC-500 Frame Grabber°¡ PC¿¡ USB 3.0À¸·Î ¿¬°áµÇ¾î, PC¿¡¼­ ÀÎ½ÄµÇ¾úÀ» ¶§
	//		//		Task.iUsbModuleFlag = 1;
	//		//		pFrame->m_labelUsbModule.SetBkColor(M_COLOR_GREEN);
	//		//		pFrame->m_labelUsbModule.Invalidate();
	//		//	}
	//		//}
	//	}
	//	break;

	//case MIU_EVNET_DETACHED:
	//	{
	//		//theApp.MainDlg->putListLog("USB detached Event");
	//		TRACE("USB detached Event\n"); 

	//		//int iSize_Device = MIU.Search_Device();
	//		//if ( iSize_Device <= 0 )
	//		//{
	//		//	Task.iUsbModuleFlag = 0;

	//		//	pFrame->m_labelUsbModule.SetBkColor(M_COLOR_RED);
	//		//	errMsg2(Task.AutoFlag, "USB Ä«¸Þ¶ó ¸ðµâ ¿¬°áÀÌ ÇØÁ¦ µÇ¾ú½À´Ï´Ù.");
	//		//	pFrame->m_labelUsbModule.Invalidate();

	//		//	//! LPMC-500 Frame Grabber°¡ PC¿ÍÀÇ ¿¬°áÀÌ ²÷°åÀ» ¶§
	//		//	MIU.Stop();
	//		//	MIU.Close();					// 95 ~ 100 msec
	//		//}

	//		
	//		//theApp.MainDlg->m_bMiuRun = false; 
	//		//gMIUDevice.CurrentState = 0;
	//		//gMIUDevice.bMIUOpen = 0;
	//		//gMIUDevice.bMIUInit = 0;
	//		//gMIUDevice.bReadyVCM = 0;
	//		//MIU.disConnectedInit();
	//		//Sleep(5000);
	//		//MIU.Stop();
	//		//MIU.Close();	
	//	}
	//	break;

	//default:

	//	break;
	//}
}
UINT Thread_Epoxy(LPVOID parm)
{
	bThreadEpoxyRun = true;
	g_bMovingflag = true;

	//CRICLE_EPOXY, RECT_EPOXY, POLYGON_EPOXY
	if (sysData.nEpoxyIndex == CRICLE_EPOXY)
	{
		motor.func_Epoxy_CircleDraw();
	}
	else if (sysData.nEpoxyIndex == RECT_EPOXY)
	{
		motor.func_Epoxy_Draw();
	}
	else if(sysData.nEpoxyIndex == POLYGON_EPOXY)
	{
		motor.func_Epoxy_Rect_Circle_Draw();
	}
	else if (sysData.nEpoxyIndex == POINT_EPOXY)
	{
		//point Çü
	}

	if (!motor.PCB_Z_Motor_Move(Wait_Pos))
	{
		g_bMovingflag = false;
		return 1;
	}
	/*if (!motor.Pcb_Motor_Move(Wait_Pos))
	{
		g_bMovingflag = false;
		return 1;
	}*/
	g_bMovingflag = false;
	bThreadEpoxyRun = false;
	return 1;
}
UINT Thread_Grab(LPVOID parm)
{
	bThreadGrab = bThreadGrabRun = true;

	while (bThreadGrab)
	{
		if (vision.UserHookData.liveMode == 1)
		{
			MdigGrab(vision.MilDigitizer, vision.MilGrabImage[0]);
			MdigGrabWait(vision.MilDigitizer, M_GRAB_FRAME_END);

			//MimFlip(vision.MilGrabImageChild[CAM2], vision.MilGrabImageChild[CAM2], M_HORIZONTAL, M_DEFAULT);
			//MimRotate(vision.MilGrabImageChild[CAM2], vision.MilGrabImageChild[CAM2], 90,CAM_SIZE_X/2,CAM_SIZE_Y/2,CAM_SIZE_X/2,CAM_SIZE_Y/2,M_DEFAULT);

			MimResize(vision.MilGrabImageChild[0], vision.MilSmallImageChild[0], CAM_REDUCE_FACTOR_X, CAM_REDUCE_FACTOR_Y, M_DEFAULT);
			//MimResize(vision.MilGrabImageChild[1], vision.MilSmallImageChild[1], CAM_REDUCE_FACTOR_X, CAM_REDUCE_FACTOR_Y, M_DEFAULT);
		}

		::Sleep(10);
	}

	return 1;
}


//! Added by LHW (2013/4/21)
//! Servo µå¶óÀÌ¹öÀÇ Alarm È®ÀÎ
//! [ÁÖÀÇ »çÇ×] PCI-R1604, PCI-R1604-MLII¿Í ¿¬°áµÈ ¼­º¸µå¶óÀÌ¹ö Ãà¿¡¼­ »ç¿ë
UINT Thread_ServoAlarm(LPVOID parm)
{
	bThreadServoAlarm = true;
	bThreadServoAlarmRun = true;

	int   i = 0;
	long  lAxisNo; 
	DWORD dwRet = 0;

	long  BoardNo, ModulePos;
	DWORD ModuleID;

	DWORD uReturnMode = 0;
	DWORD upAlarmCode;	//! È®ÀÎµÈ ¾Ë¶÷ ÄÚµå

	const int iBufSize_GetAlarm = 1024;
	char  szGetAlarm[iBufSize_GetAlarm];
	CString asAlarm[MAX_MOTOR_NO];
	for ( i = 0; i < MAX_MOTOR_NO; i++ )
	{
		asAlarm[i] = _T("");
	}
			
	while (bThreadServoAlarm)
	{
		for ( i = 0; i < MAX_MOTOR_NO; i++ )
		{
			lAxisNo = i;

			//! Servo Alarm ¿©ºÎ È®ÀÎ
			if ( motor.GetAmpFault(lAxisNo) == false )
			{
				continue;
			}

			//! Servo µå¶óÀÌ¹öÀÇ AlarmÀ» È®ÀÎÇÒ ¼ö ÀÖ´Â Áö¸¦ È®ÀÎ
			dwRet = AxmInfoGetAxis(lAxisNo, &BoardNo, &ModulePos, &ModuleID);
			if ( dwRet != AXT_RT_SUCCESS )
			{
				continue;
			}
			if ( ModuleID != AXT_SMC_R1V04A4 && ModuleID != AXT_SMC_R1V04A5 )
			{
				continue;
			}

			//! ÁöÁ¤ÃàÀÇ ¾Ë¶÷ ³»¿ëÈ®ÀÎÀ» ¿äÃ»ÇÑ´Ù.
			dwRet = AxmStatusRequestServoAlarm(lAxisNo);

			//! ÁöÁ¤ÃàÀÇ ¾Ë¶÷ ³»¿ëÈ®ÀÎ ¿äÃ»¿¡ ´ëÇÑ ÀÀ´ä °á°ú¸¦ È®ÀÎÇÑ´Ù.
			//! uReturnMode : ÇÔ¼ö ½ÇÇà Á¶°ÇÀ» ¼³Á¤
			//! - [0] ÇÔ¼ö ½ÇÇà ÈÄ ¹Ù·Î ¹ÝÈ¯
			//! - [1] ¼­º¸ÆÑÀ¸·ÎºÎÅÍ ¾Ë¶÷ ÄÚµå¸¦ ÀÐÀ» ¶§ ±îÁö ¹ÝÈ­ÇÏÁö ¾ÊÀ½
			//! - [2] ¼­º¸ÆÑÀ¸·ÎºÎÅÍ ¾Ë¶÷ ÄÚµå¸¦ ÀÐÀ» ¶§ ±îÁö ¹ÝÈ­ÇÏÁö ¾ÊÀ¸³ª ÇÁ·Î±×·¥ Blocking µÇÁö ¾ÊÀ½
			uReturnMode = 0;
			dwRet = AxmStatusReadServoAlarm(lAxisNo, uReturnMode, &upAlarmCode);
			if ( dwRet != AXT_RT_SUCCESS )
			{
				//! [4210] AXT_RT_MOTION_READ_ALARM_WAITING : ¼­º¸ÆÑÀ¸·ÎºÎÅÍ ¾Ë¶÷ ÄÚµå°¡ Àü´ÞµÇÁö ¾ÊÀ½
				//! [4211] AXT_RT_MOTION_READ_ALARM_NO_REQUEST : ¾Ë¶÷ ÄÚµå ÀÐ±â ¿äÃ» µÇÁö ¾ÊÀ½.
				//! [4212] AXT_RT_MOTION_READ_ALARM_TIMEOUT : ¾Ë¶÷ ÄÚµå ÀÐ±âÁß ½Ã°£ ÃÊ°ú (1ÃÊ ÀÌ»ó)
				//! [4213] AXT_RT_MOTION_READ_ALARM_FAILED : ¾Ë¶÷ ÄÚµå ÀÐ±â ½ÇÆÐ(³×Æ®¿öÅ© ¿¬°á ¿À·ùµî)
				continue;
			}

			//! ÁöÁ¤ÃàÀÇ ¾Ë¶÷ ÄÚµå¸¦ »ç¿ëÇÏ¿© ¾Ë¶÷ ³»¿ë¿¡ ´ëÇÑ ¹®ÀÚ¿­ °ªÀ» È®ÀÎÇÑ´Ù.
			//! ¾Ë¶÷ ÄÚµå¿¡ ´ëÇÑ ¹®ÀÚ¿­À» ¿Ã¹Ù¸£°Ô Ç¥½ÃÇÏ±â À§ÇØ¼­´Â ¶óÀÌºê·¯¸®¿Í °°ÀÌ ¹èÆ÷µÇ´Â ¾Ë¶÷ ¹®ÀÚ¿­ ÀÏ¶÷ ÆÄÀÏÀÌ 
			//! Axl.dll¿Í °°Àº Æú´õ¿¡ ÀÖ°Å³ª OS ¼³Ä¡ µð·ºÅä¸® ³»ÀÇ System32 Æú´õ³»¿¡ Á¸ÀçÇÏ¿©¾ß ÇÑ´Ù. 
			//! ±×·¸Áö ¾ÊÀ» °æ¿ì ÇÔ¼ö ½ÇÇà °á°ú°¡ ¡®AXT_RT_MOTION_READ_ALARM_FILES¡¯·Î ¸®ÅÏµÈ´Ù. 
			//! ¾Ë¶÷ ÄÚµå¿¡ ´ëÇÑ ¹®ÀÚ¿­ ÆÄÀÏÀº EzSoftwre ¼³Ä¡ Æú´õ³»ÀÇ ¡®AXL(Library)\Library\DefFile¡¯¿¡ ÀÖ´Ù.
			//!  (Alarm-A4N.def, Alarm-A5N.def)
			dwRet = AxmStatusGetServoAlarmString(lAxisNo, upAlarmCode, iBufSize_GetAlarm, szGetAlarm);
			if ( dwRet != AXT_RT_SUCCESS )
			{
				//! [4220] AXT_RT_MOTION_READ_ALARM_UNKNOWN : Àß¸øµÈ ¾Ë¶÷ ÄÚµå
				//! [4221] AXT_RT_MOTION_READ_ALARM_FILES : ¾Ë¶÷ ÄÚµå¿¡ ÇØ´çÇÏ´Â ¹®ÀÚ¿­ ÆÄÀÏÀÌ ·ÎµåµÇÁö ¾ÊÀ½.
				switch(dwRet)
				{
				case AXT_RT_MOTION_READ_ALARM_UNKNOWN:
					{
						asAlarm[lAxisNo].Format(_T("[%s] Servo Alarm : Unknown Alarm"), MotorName[lAxisNo]);
					}
					break;
				case AXT_RT_MOTION_READ_ALARM_FILES:
					{
						asAlarm[lAxisNo].Format(_T("[%s] Servo Alarm : Def File not exist"), MotorName[lAxisNo]);
					}
					break;
				default:
					{
						asAlarm[lAxisNo].Format(_T("[%s] Servo Alarm : Error"), MotorName[lAxisNo]);
					}
					break;
				}

				continue;
			}

			asAlarm[lAxisNo].Format(_T("[%s] Servo Alarm : %s"), MotorName[lAxisNo], szGetAlarm);
			LogSave(asAlarm[lAxisNo]);

			Sleep(200);
		}//! for ( i = 0; i < AxisCount; i++ )

	}//! while (bThreadServoAlarm)

	bThreadServoAlarmRun = false;

	return true;
}

//! Modified by LHW (2013/2/25)
//! CCM ¿µ»ó Ãëµæ¿ë Thread
//! [ÁÖÀÇ »çÇ×] ModelÀÌ º¯°æµÇ¸é, Thread¸¦ ´Ý°í, ´Ù½Ã ½ÇÇà½ÃÄÑ¾ß ÇÕ´Ï´Ù.
UINT Thread_Ccm_Grab(LPVOID parm)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	if(pFrame == NULL)
	{
		bThreadCcmGrab = false;
		bThreadCcmGrabRun = false;
		return 1;
	}

	CString sMessage;
	if(gMIUDevice.nWidth < 100 || gMIUDevice.nHeight < 100)
	{
		sMessage.Format(_T("MIL ¿µ»ó ¹öÆÛ È£Ãâ Å©±â°¡ ºñÁ¤»óÀÔ´Ï´Ù."));
		errMsg2(Task.AutoFlag,sMessage);
		return 1;
	}

	vision.MiuBufferFree();

	if(!vision.MiuBufferAlloc(gMIUDevice.nWidth, gMIUDevice.nHeight))
	{
		errMsg2(Task.AutoFlag,"MIL ¿µ»ó ¹öÆÛ »ý¼º ½ÇÆÐ.");
		return 1;
	}
	

	int tmpCnt = 0;

	int    iIndexDevice=0;
	int    errorCode =0;
//	INT64	TimeStamp;
	bThreadCcmGrab = true;
	bThreadCcmGrabRun = true;

	bool saveFlag = false;
		
	try
	{
		while(bThreadCcmGrab)
		{
			if ( pFrame == NULL )
			{
				break;
			}

			if ( gMIUDevice.bMIUOpen != 1 )
			{
				Sleep(500);
				continue;
			}

			if(gMIUDevice.CurrentState==0)
			{
				Sleep(10);
				continue;
			}

			if(gMIUDevice.CurrentState==4)
			{
				pFrame->m_labelUsbLive.SetBkColor(M_COLOR_GREEN);
				pFrame->m_labelUsbLive.Invalidate();
			}
			else
			{
				pFrame->m_labelUsbLive.SetBkColor(M_COLOR_RED);
				pFrame->m_labelUsbLive.Invalidate();
			}


			if( gMIUDevice.CurrentState < 3 )
			{
				Sleep(10);
				continue;
			}

			iIndexDevice = MIU.GetIndexDevice();
			if ( iIndexDevice < 0 )
			{
				Sleep(500);
				continue;
			}

			if ( MIU.m_iIndex_Grab_Working < 0 )
			{
				Sleep(500);
				continue;
			}

			if ( MIU.m_iIndex_Grab_Working == MIU.m_iIndex_Grab_Ready )
			{
				//! ÇØ´ç ¹öÆÛÀÇ ¿µ»óÀ» ¾ÆÁ÷ È­¸é¿¡ Ç¥½ÃÇÏÁö ¸øÇÔ
				Sleep(5);
				continue;
			}


			//! ¿µ»ó Ãëµæ ¹öÆÛÀÇ »óÅÂ°ª ÀÓ½Ã ÀúÀå 
			//! [ÁÖÀÇ »çÇ×] 1 Frame¸¸ Âï¾î¼­ Á¤Áö ¿µ»ó DisplayÇÏ´Â ±â´É¿¡ ÇÊ¿ä, Process¿ë ¹öÆÛ °ü¸®¿¡ Áß¿äÇÏ´Ù.
			if ( gMIUDevice.CurrentState == 3)
			{
				gMIUDevice.CurrentState = 2;
			}

			//! ¿µ»ó Ãëµæ °ü·Ã Thread°£ °æ°ú ½Ã°£ ÃøÁ¤ ½ÃÀÛ
			MIU.Init_Grab_Time(0);
			MIU.Start_Grab_Time(0);

			//! ¿µ»ó Ãëµæ ½Ã°¢À» ¾Ë¾Æ³½´Ù.
			::GetLocalTime( &(MIU.m_aTemp[ MIU.m_iIndex_Grab_Working ].TimeGrab) );
			try
			{
				//! ¿µ»ó Ãëµæ
				if (MIU.m_pBoard->GetFrame(MIU.m_pFrameRawBuffer, MIU.m_pFrameBMPBuffer))
				{
					//gMIUDevice.imageGrayItp->imageData = (char*)MIU.m_pFrameBMPBuffer; 
					gMIUDevice.imageItp->imageData = (char*)MIU.m_pFrameBMPBuffer;
					//cvSaveImage("D:\\m_pFrameBMPBuffer.bmp", gMIUDevice.imageGrayItp);	 
					MIU.Grab_StopImage();

					//! ¿µ»ó Ãëµæ °ü·Ã Thread°£ °æ°ú ½Ã°£ ÃøÁ¤
					MIU.Measure_Grab_Time(0);

					//! ¿µ»ó Ãëµæ °ü·Ã Thread°£ °æ°ú ½Ã°£ ÃøÁ¤ ½ÃÀÛ
					MIU.Init_Grab_Time(1);
					MIU.Start_Grab_Time(1);

					//! ¿µ»ó ÃëµæÀ» ¸¶Ä¡°í, ÇØ´ç ¿µ»óÀÇ ¹öÆÛ index¸¦ ¾Ë·ÁÁØ ÈÄ¿¡, 
					MIU.m_iIndex_Grab_Ready = MIU.m_iIndex_Grab_Working;
					//! ´ÙÀ½ ¹öÆÛÀÇ ¿µ»óÀ» ÃëµæÇÒ ¼ö ÀÖµµ·Ï, index Áõ°¡
					(MIU.m_iIndex_Grab_Working)++;
					//! ¹öÆÛÀÇ Å©±â¸¦ ³Ñ¾î°¡¸é, ¹öÆÛ index¸¦ ÃÊ±âÈ­
					if (MIU.m_iIndex_Grab_Working >= iBufSize_CCM_Temp)
					{
						MIU.m_iIndex_Grab_Working = 0;
					}
				}
			}
			catch (std::exception& e)//catch (CException *e)//catch (CFileException *e)
			{
				//e->ReportError();
			}

			Sleep(5);
		}
	}
	catch (CException* e)
	{
		TCHAR czCause[255];
		e->GetErrorMessage(czCause, sizeof(czCause));
		TRACE(_T("ThreadFunc_CCM_Grab - CException [%s] \n"), czCause);
		e->Delete();
	}


	gMIUDevice.CurrentState = -1;

	bThreadCcmGrab = false;
	bThreadCcmGrabRun = false;

	return 1;
}

//! Modified by LHW (2013/2/25)
//! CCMºÎ¿¡¼­ Color º¯È¯ ÀÛ¾÷¿ë Thread
//! [ÁÖÀÇ »çÇ×] ModelÀÌ º¯°æµÇ¸é, Thread¸¦ ´Ý°í, ´Ù½Ã ½ÇÇà½ÃÄÑ¾ß ÇÕ´Ï´Ù.
UINT Thread_Ccm_CvtColor(LPVOID parm)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	if(pFrame == NULL)
	{
		bThreadCcmGrab = false;
		bThreadCcmGrabRun = false;
		return 1;
	}
	
	bThreadCcmGrab = true;
	bThreadCcmGrabRun = true;

	try
	{
		while(bThreadCcmGrab)
		{
			if ( pFrame == NULL )
			{
				break;
			}

			if ( gMIUDevice.bMIUOpen != 1 )
			{
				Sleep(500);
				continue;
			}

			if(gMIUDevice.CurrentState==0)
			{
				Sleep(10);
				continue;
			}

			if(gMIUDevice.CurrentState<3)
			{
				Sleep(10);
				continue;
			}

			if ( MIU.m_iIndex_Grab_Ready < 0 )
			{
				Sleep(50);
				continue;
			}

			if ( MIU.m_iIndex_Grab_Ready == MIU.m_iIndex_Grab_Used )
			{
				Sleep(5);
				continue;
			}


			//! ¿µ»ó Ãëµæ ¹öÆÛÀÇ »óÅÂ°ª ÀÓ½Ã ÀúÀå
			//! [ÁÖÀÇ »çÇ×] 1 Frame¸¸ Âï¾î¼­ Á¤Áö ¿µ»ó DisplayÇÏ´Â ±â´É¿¡ ÇÊ¿ä, Process¿ë ¹öÆÛ °ü¸®¿¡ Áß¿äÇÏ´Ù.
			//MIU.m_aTemp[ MIU.m_iIndex_Cvt_Clr_Working ].CurrentState = MIU.m_aTemp[ MIU.m_iIndex_Grab_Ready ].CurrentState;

			//! ¾ÆÁ÷ Ã³¸®ÇÏÁö ¸øÇÑ, ÃëµæµÈ ¿µ»óÀÌ ÀÖÀ¸¹Ç·Î, 

			//! ¿µ»ó Ãëµæ °ü·Ã Thread°£ °æ°ú ½Ã°£ ÃøÁ¤
			MIU.Measure_Grab_Time(1);

			//! ¿µ»ó Ãëµæ °ü·Ã Thread°£ °æ°ú ½Ã°£ ÃøÁ¤ ½ÃÀÛ
			MIU.Init_Grab_Time(2);
			MIU.Start_Grab_Time(2);

			MIU.m_iIndex_Cvt_Clr_Working = MIU.m_iIndex_Grab_Ready;

			MIU.Measure_Grab_Time(2);

			//! ¿µ»ó Ãëµæ °ü·Ã Thread°£ °æ°ú ½Ã°£ ÃøÁ¤ ½ÃÀÛ
			MIU.Init_Grab_Time(3);
			MIU.Start_Grab_Time(3);

			MIU.m_iIndex_Grab_Used = MIU.m_iIndex_Grab_Ready;

			MIU.m_iIndex_Cvt_Clr_Used = MIU.m_iIndex_Cvt_Clr_Working;

			Sleep(5);
		}
	}
	catch (CException* e)
	{
		TCHAR czCause[255];
		e->GetErrorMessage(czCause, sizeof(czCause));
		TRACE(_T("Thread_Ccm_CvtColor - CException [%s] \n"), czCause);
		e->Delete();
	}
	
	gMIUDevice.CurrentState = -1;

	bThreadCcmGrab = false;
	bThreadCcmGrabRun = false;

	return 1;
}

//! Modified by LHW (2013/2/25)
//! CCMºÎ¿¡¼­ÀÇ Open CV --> Mil ÀÌ¹ÌÁö ¹öÆÛ º¯È¯¿ë Thread
//! [ÁÖÀÇ »çÇ×] ModelÀÌ º¯°æµÇ¸é, Thread¸¦ ´Ý°í, ´Ù½Ã ½ÇÇà½ÃÄÑ¾ß ÇÕ´Ï´Ù.
UINT Thread_Ccm_CvtMil(LPVOID parm)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	//SYSTEMTIME	sysTime;

	if(pFrame == NULL)
	{
		bThreadCcmGrab = false;
		bThreadCcmGrabRun = false;
		return 1;
	}
		
	bThreadCcmGrab = true;
	bThreadCcmGrabRun = true;

	try
	{
		while(bThreadCcmGrab)
		{
			if ( pFrame == NULL )
			{
				break;
			}

			if ( gMIUDevice.bMIUOpen != 1 )
			{
				Sleep(500);
				continue;
			}

			if(gMIUDevice.CurrentState==0)
			{
				Sleep(100);
				continue;
			}

			if(gMIUDevice.CurrentState<3)
			{
				Sleep(10);
				continue;
			}

			if ( MIU.m_iIndex_Cvt_Clr_Used < 0 )
			{
				Sleep(100);
				continue;
			}

			if ( MIU.m_iIndex_Cvt_Clr_Used == MIU.m_iIndex_Cvt_Mil_Working )
			{
				Sleep(5);
				continue;
			}

			//! ¿µ»ó Ãëµæ ¹öÆÛÀÇ »óÅÂ°ª ÀÓ½Ã ÀúÀå
			//! [ÁÖÀÇ »çÇ×] 1 Frame¸¸ Âï¾î¼­ Á¤Áö ¿µ»ó DisplayÇÏ´Â ±â´É¿¡ ÇÊ¿ä, Process¿ë ¹öÆÛ °ü¸®¿¡ Áß¿äÇÏ´Ù.
			//MIU.m_aTemp[ MIU.m_iIndex_Cvt_Mil_Working ].CurrentState = MIU.m_aTemp[ MIU.m_iIndex_Cvt_Clr_Used ].CurrentState;

			MIU.m_iIndex_Cvt_Mil_Working = MIU.m_iIndex_Cvt_Clr_Used;

			//! ¿µ»ó Ãëµæ °ü·Ã Thread°£ °æ°ú ½Ã°£ ÃøÁ¤
			MIU.Measure_Grab_Time(3);

			//! ¿µ»ó Ãëµæ °ü·Ã Thread°£ °æ°ú ½Ã°£ ÃøÁ¤ ½ÃÀÛ
			MIU.Init_Grab_Time(4);
			MIU.Start_Grab_Time(4);

			//! RGB Ã¤³Î ºÐ¸®..
			vision.m_acsGrabLock[1].Lock();



			//! ¿µ»ó Ãëµæ ½Ã°¢À» º¹»çÇÑ´Ù.
			CopySystemTime(MIU.m_aTemp[ MIU.m_iIndex_Cvt_Mil_Working ].TimeGrab, vision.m_aTimeGrab[1]);


			vision.m_acsGrabLock[1].Unlock();

			MIU.Measure_Grab_Time(4);

			//! ¿µ»ó Ãëµæ °ü·Ã Thread°£ °æ°ú ½Ã°£ ÃøÁ¤ ½ÃÀÛ
			MIU.Init_Grab_Time(5);
			MIU.Start_Grab_Time(5);

			/*MbufPut(vision.MilProcImageChild[3], MIU.m_acvChildImage[0]->imageData);
			MbufPut(vision.MilProcImageChild[4], MIU.m_acvChildImage[1]->imageData);
			MbufPut(vision.MilProcImageChild[5], MIU.m_acvChildImage[2]->imageData);*/

			MIU.m_iIndex_Cvt_Mil_Used = MIU.m_iIndex_Cvt_Mil_Working;			
			
			Sleep(5);
		}
	}
	catch (CException* e)
	{
		TCHAR czCause[255];
		e->GetErrorMessage(czCause, sizeof(czCause));
		TRACE(_T("Thread_Ccm_CvtMil - CException [%s] \n"), czCause);
		e->Delete();
	}
	
	gMIUDevice.CurrentState = -1;

	bThreadCcmGrab = false;
	bThreadCcmGrabRun = false;

	return 1;
}



//! Modified by LHW (2013/3/12)
//! CCMºÎ¿¡¼­ Color º¯È¯ ÀÛ¾÷¿ë Thread (MbufBayer ÇÔ¼ö »ç¿ë)
//! [ÁÖÀÇ »çÇ×] ModelÀÌ º¯°æµÇ¸é, Thread¸¦ ´Ý°í, ´Ù½Ã ½ÇÇà½ÃÄÑ¾ß ÇÕ´Ï´Ù.
UINT Thread_Ccm_Mil_CvtColor(LPVOID parm)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	int iOldImgFrame=-1;

	if(pFrame == NULL)
	{
		bThreadCcmGrab = false;
		bThreadCcmGrabRun = false;
		return 1;
	}

	bThreadCcmGrab = true;
	bThreadCcmGrabRun = true;

	try
	{
		while(bThreadCcmGrab)
		{
			if ( pFrame == NULL )				break;

			if ( gMIUDevice.bMIUOpen != 1 )
			{
				Sleep(500);
				continue;
			}

			if(gMIUDevice.CurrentState==0)
			{
				Sleep(100);
				continue;
			}

			if(gMIUDevice.CurrentState<3)
			{
				Sleep(10);
				continue;
			}

			if ( vision.m_iIndex_Cvt_Clr_Ready < 0 )
			{
				Sleep(100);
				continue;
			}

			if ( vision.m_iIndex_Cvt_Clr_Ready == vision.m_iIndex_Cvt_Clr_Used )
			{
				Sleep(5);
				continue;
			}

			if ( vision.MilBayerImage[ vision.m_iIndex_Cvt_Clr_Ready ] == M_NULL )
			{
				Sleep(100);
				continue;
			}
			if( iOldImgFrame == vision.m_iIndex_Cvt_Clr_Ready ) 
			{
				Sleep(5);
				continue;
			}
			iOldImgFrame = vision.m_iIndex_Cvt_Clr_Ready;

			//! ¿µ»ó Ãëµæ °ü·Ã Thread°£ °æ°ú ½Ã°£ ÃøÁ¤
			MIU.Measure_Grab_Time(3);

			//! ¿µ»ó Ãëµæ °ü·Ã Thread°£ °æ°ú ½Ã°£ ÃøÁ¤ ½ÃÀÛ
			MIU.Init_Grab_Time(4);
			MIU.Start_Grab_Time(4);

			//! MbufBayer ÇÔ¼ö »ç¿ë, Bayer ÀÌ¹ÌÁö -->> Color ÀÌ¹ÌÁö

			vision.m_acsGrabLock[1].Lock();
			//! ¿µ»ó Ãëµæ ½Ã°¢À» º¹»çÇÑ´Ù.
			CopySystemTime(vision.m_aTimeBayer[ vision.m_iIndex_Cvt_Clr_Ready ], vision.m_aTimeGrab[1]);
			//vision.m_bFlag_Use_WB_Coeff = true;


			//if ( vision.m_bFlag_Use_WB_Coeff == true )
			//{
			//	MbufBayer( vision.MilBayerImage[ vision.m_iIndex_Cvt_Clr_Ready ], vision.MilGrabImage[1], vision.MilWBCoefficients, M_BAYER_RG );				
			//}
			//else 
			//{
			//	//! 'M_ADAPTIVE'¸¦ Æ÷ÇÔÇÏ¸é, 500 msecÀÇ ½Ã°£ÀÌ °É¸°´Ù.
			//	MbufBayer( vision.MilBayerImage[ vision.m_iIndex_Cvt_Clr_Ready ], vision.MilGrabImage[1], M_DEFAULT, M_BAYER_RG );		
			//}

			vision.m_csProcImageCV.Unlock();
			vision.m_acsGrabLock[1].Unlock();

			MIU.Measure_Grab_Time(4);

			//! ¿µ»ó Ãëµæ °ü·Ã Thread°£ °æ°ú ½Ã°£ ÃøÁ¤ ½ÃÀÛ
			MIU.Init_Grab_Time(5);
			MIU.Start_Grab_Time(5);

			vision.m_iIndex_Cvt_Clr_Used = vision.m_iIndex_Cvt_Clr_Ready;

			//! Thread_Ccm_Display¸¦ ÀÛµ¿½ÃÅ°±â À§ÇØ¼­ ÇÊ¿äÇÏ´Ù. 
			MIU.m_iIndex_Cvt_Mil_Used = vision.m_iIndex_Cvt_Clr_Ready;

			Sleep(5);
		}
	}
	catch (CException* e)
	{
		TCHAR czCause[255];
		e->GetErrorMessage(czCause, sizeof(czCause));
		TRACE(_T("Thread_Ccm_Mil_CvtColor - CException [%s] \n"), czCause);
		e->Delete();
	}

	gMIUDevice.CurrentState = -1;

	bThreadCcmGrab = false;
	bThreadCcmGrabRun = false;

	return 1;
}

//! Modified by LHW (2013/2/25, 2013/3/27)
//! CCM ¿µ»ó Display¿ë Thread
//! [ÁÖÀÇ »çÇ×] ModelÀÌ º¯°æµÇ¸é, Thread¸¦ ´Ý°í, ´Ù½Ã ½ÇÇà½ÃÄÑ¾ß ÇÕ´Ï´Ù.
UINT Thread_Ccm_Display(LPVOID parm)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	if(pFrame == NULL)
	{
		bThreadCcmGrab = false;
		bThreadCcmGrabRun = false;
		return 1;
	}
	
	bThreadCcmGrab = true;
	bThreadCcmGrabRun = true;

	double dReduceFactorX = 0.;
	double dReduceFactorY = 0.;
	
	try
	{
		while(bThreadCcmGrab)
		{
			if ( pFrame == NULL )
			{
				break;
			}

			if ( gMIUDevice.bMIUOpen != 1 )
			{
				Sleep(500);
				continue;
			}

			if(gMIUDevice.CurrentState==0)
			{
				Sleep(100);
				continue;
			}

			if(gMIUDevice.CurrentState<3)
			{
				Sleep(10);
				continue;
			}

			if ( MIU.m_iIndex_Cvt_Mil_Used < 0 )
			{
				Sleep(100);
				continue;
			}
			
			if ( MIU.m_iIndex_Cvt_Mil_Used == MIU.m_iIndex_Display_Working )
			{
				Sleep(5);
				continue;
			}

			if ( gMIUDevice.nWidth <= 0 || gMIUDevice.nHeight <= 0 )
			{
				Sleep(100);
				continue;
			}

			//! ¿µ»ó Ãëµæ °ü·Ã Thread°£ °æ°ú ½Ã°£ ÃøÁ¤
			MIU.Measure_Grab_Time(5);

			MIU.Init_Grab_Time(6);
			MIU.Start_Grab_Time(6);

			//! ¿µ»ó Ãëµæ ¹öÆÛÀÇ »óÅÂ°ª ÀÓ½Ã ÀúÀå
			//! [ÁÖÀÇ »çÇ×] 1 Frame¸¸ Âï¾î¼­ Á¤Áö ¿µ»ó DisplayÇÏ´Â ±â´É¿¡ ÇÊ¿ä, Process¿ë ¹öÆÛ °ü¸®¿¡ Áß¿äÇÏ´Ù.

			//! ¾ÆÁ÷ Ã³¸®ÇÏÁö ¸øÇÑ, ÃëµæµÈ ¿µ»óÀÌ ÀÖÀ¸¹Ç·Î, 

			dReduceFactorX = (double)SMALL_CCD_SIZE_X / gMIUDevice.nWidth;
			dReduceFactorY = (double)SMALL_CCD_SIZE_Y / gMIUDevice.nHeight;

			MIU.m_iIndex_Display_Working = MIU.m_iIndex_Cvt_Mil_Used;

			pFrame->Update_CCD_Display();

			MIU.Measure_Grab_Time(6);

			//! Display¿ë Frame Rate °è»êÀ» À§ÇØ È£ÃâÇÑ´Ù. 
			MIU.Add_Display_Count();	

			if ( bFlag_First_Grab_Display == false )
			{
				//! Thread°¡ »ý¼ºµÇ°í ³ª¼­, Ã¹¹øÂ°ÀÇ ¿µ»ó Ãëµæ¿¡ ¼º°øÇÏ´Ù.
				bFlag_First_Grab_Display = true;
			}

			Sleep(5);
		}
	}
	catch (CException* e)
	{
		TCHAR czCause[255];
		e->GetErrorMessage(czCause, sizeof(czCause));
		TRACE(_T("Thread_Ccm_Display - CException [%s] \n"), czCause);
		e->Delete();
	}
	
	gMIUDevice.CurrentState = -1;

	bThreadCcmGrab = false;
	bThreadCcmGrabRun = false;

	return 1;
}

//! Auto Exposure Time º»Ã¼ Thread
//! Auto Exposure Time ¼³Á¤ °ü·Ã Áß¿ä Flag ¼³Á¤À» À§ÇØ¼­ Á¸ÀçÇÑ´Ù. 
//! (¿µ»ó Ãëµæ ½ÃÀÛ ¿©ºÎ¸¦ È®ÀÎÇÏ±â À§ÇÑ °ÍÀÌ´Ù.)
UINT Thread_Ccm_Auto_Exposure_Time(LPVOID parm)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	if(pFrame == NULL)
	{
		Task.bFlag_Auto_Exposure_Time = false;
		return 1;
	}

	Task.bFlag_Auto_Exposure_Time = true;

	bool bRet = false;

	try
	{
		while(bThreadCcmGrab)
		{
			if ( pFrame == NULL )
			{
				break;
			}

			if ( gMIUDevice.bMIUOpen != 1 )
			{
				break;
			}
			
			if(gMIUDevice.CurrentState<3)
			{
				break;
			}

			if ( bFlag_First_Grab_Display == true )
			{				
				break;
			}

			Sleep(10);

		}//! while(bThreadCcmGrab)
	}
	catch (CException* e)
	{
		TCHAR czCause[255];
		e->GetErrorMessage(czCause, sizeof(czCause));
		TRACE(_T("Thread_Ccm_Auto_Exposure_Time - CException [%s] \n"), czCause);
		e->Delete();
	}

	Task.bFlag_Auto_Exposure_Time = false;

	return 1;
}

UINT Thread_TaskReady(LPVOID parm)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	CString logStr="";
	CString sTemp="";

	vision.setLiveMode(true);
	int delayTime = 5;


	if( bThreadTaskLensRun == true ||  bThreadTaskPcbRun == true )
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//"ÀÚµ¿ µ¿ÀÛ Áß ÀÔ´Ï´Ù."
		delayMsg(sLangChange);
		return 0;
	}

	if( Task.AutoFlag == 2 )
	{
		sLangChange.LoadStringA(IDS_STRING1362);	//"ÀÏ½Ã Á¤Áö Áß ÀÔ´Ï´Ù."
		errMsg2(Task.AutoFlag, sLangChange);
		return 0;
	}

	if ( bThreadTaskReadyRun == true)
	{
		sLangChange.LoadStringA(IDS_STRING1322);	//"¿îÀü ÁØºñ µ¿ÀÛ Áß ÀÔ´Ï´Ù."
		errMsg2(Task.AutoFlag, sLangChange);
		return 0;
	}

	if(g_bMovingflag)
	{
		sLangChange.LoadStringA(IDS_STRING1324);	//"¿îÀü ÁØºñ ½ÇÆÐ - Àåºñ µ¿ÀÛ Áß ÀÔ´Ï´Ù."
		sTemp.Format(sLangChange);
		errMsg2(Task.AutoFlag, sTemp);
		return 0;
	}

	g_bMovingflag =true;

	bThreadTaskReady = true;
	bThreadTaskReadyRun = true;
	iReadyRunCnt = 0;
	Task.m_iStatus_Unit_Epoxy = 1;

	for(int i=0; i<MAX_MOTOR_NO; i++)
	{
		if(motor.m_bOrgFlag[i]==false)
		{
			bThreadTaskReady = false;
			break;
		}
	}
	int i_alarm_flag;

	while(bThreadTaskReady)
	{
		if(Task.AutoFlag != 1)
		{
			sLangChange.LoadStringA(IDS_STRING402);	//"AutoRunFlag OFF »óÅÂ . LensºÎ step :-1"
			logStr.Format(sLangChange);
			pFrame->putListLog(logStr);
			break;
		}

		i_alarm_flag = pFrame->checkAutoRunLensAlarm(Task.ReadyTask);

		if(i_alarm_flag != 0)
		{
			Dio.setAlarm(ALARM_ON);
			Task.ReadyTask = -abs(Task.ReadyTask);
			Task.AutoFlag = 0;
			pFrame->AutoRunView(Task.AutoFlag);

			g_bMovingflag = false;
		}


		if(Task.AutoFlag)
		{
			if ( Task.ReadyTask >= 10000 && Task.ReadyTask <19900 )
			{
				Task.ReadyTask = pFrame->Ready_process(Task.ReadyTask);		/* ·»Áî ·Îµå -> ·»Áî ¾ó¶óÀÎ -> ·»Áî º»µù À§Ä¡ */	
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING309);	//"[¿îÀü ÁØºñ]¿îÀü ÁØºñ ¿Ï·á Çß½À´Ï´Ù."
				pFrame->putListLog(sLangChange);
				break;
			}
		}


		if ( Task.ReadyTask < 0 ){
			break;
		}
		Sleep(5);
	}
	if ( Task.ReadyTask == 19900 )
	{
		sLangChange.LoadStringA(IDS_STRING1321);	//"¿îÀü ÁØºñ µ¿ÀÛ ¿Ï·á."
		logStr.Format(sLangChange);
		pFrame->putListLog(logStr);
	}
	else
	{
		sLangChange.LoadStringA(IDS_STRING1325);	//	"¿îÀü ÁØºñ ½ÇÆÐ."
		logStr.Format(sLangChange);
		pFrame->putListLog(logStr);
	}

	if(Task.ReadyTask == 19900)
	{
		bThreadTaskReadyRun = false;		// ¾²·¹µå°¡ Á¾·á Çß´ÂÁö È®ÀÎ..

		pFrame->AutoRunView(3);
	}
	else
	{
		bThreadTaskReadyRun = false;		// ¾²·¹µå°¡ Á¾·á Çß´ÂÁö È®ÀÎ..

		pFrame->AutoRunView(0);
	}

	Task.AutoFlag = 0;

	bThreadTaskReady = false;			// ¾²·¹µå °­Á¦ Á¾·á ¿ë..
	bThreadTaskReadyRun = false;		// ¾²·¹µå°¡ Á¾·á Çß´ÂÁö È®ÀÎ..

	g_bMovingflag = false;
	Task.m_iStatus_Unit_Epoxy = 0;

	return true;
}


UINT Thread_CCDCheck(LPVOID parm)
{
	return true;
}
UINT Thread_MIUCheck(LPVOID parm)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString logStr="";

	
	bThread_MIUCheckRun=true;
	if( !pFrame->MIUCheck_process())// || gMIUDevice.CurrentState != 4)
	{
		logStr.Format("CCD ¸ðµâ ¿µ»ó ÃÊ±âÈ­ ½ÇÆÐ.\n Á¦Ç° ¾ÈÂø »óÅÂ ¹× Á¦Ç° ºÒ·® È®ÀÎ ÇÏ¼¼¿ä.");
		errMsg2(Task.AutoFlag, logStr);
		bThread_MIUCheckRun=false;
		return false;
	}
	Sleep(300);

	///MIU.SwitchHDRToLinearMode();		//zoox ¸ðµ¨¸¸

	bThread_MIUCheckRun=false;

	if(Task.PCBTask > 60000 && Task.MUICheckflag == false)
	{
		//sLangChange.LoadStringA(IDS_STRING446);	//"CCD µ¿¿µ»ó ½ÇÆÐ."
		//errMsg2(Task.AutoFlag, sLangChange);
		//return false;
	}
	return true;
}

UINT Thread_Monitor(LPVOID parm)
{
#ifndef ON_LINE_MODE
	return true;
#endif
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	bThreadMonitor		= true;
	bThreadMonitorRun	= true;

	int iOldHomeErrorFlag, oldAmpFaultFlag, iOldBuzzerFlag, i_Old_EM_SwitchFlag, iOldDoorOpenFlag, iOldMainAirFlag, iOldLimitFlag ;
	iOldHomeErrorFlag = oldAmpFaultFlag = iOldBuzzerFlag = i_Old_EM_SwitchFlag = iOldDoorOpenFlag = iOldMainAirFlag = iOldLimitFlag = -1;

	bool iOldThread1=false, iOldThread2=false;

	char sLog[100];

	while(bThreadMonitor)
	{
		motor.InDIO(0, curInDio[0]);
		motor.InDIO(2, curInDio[1]);

		sprintf_s(sLog,"Ready %d", Task.ReadyTask);
		pFrame->m_labelThread1.SetText(sLog);

		if ( Task.ReadyTask == 19900 )
		{
			pFrame->m_labelThread1.SetBkColor(M_COLOR_GREEN);
		}
		else
		{
			pFrame->m_labelThread1.SetBkColor(M_COLOR_GRAY);
		}

		iOldThread1 = bThreadTaskLensRun;

		pFrame->m_labelThread1.Invalidate();

		sprintf_s(sLog, "Pcb %d", Task.PCBTask);
		pFrame->m_labelThread2.SetText(sLog);

		if ( iOldThread2 != bThreadTaskPcbRun )
		{
			if ( bThreadTaskPcbRun )
			{
				pFrame->m_labelThread2.SetBkColor(M_COLOR_GREEN);
			}
			else
			{
				pFrame->m_labelThread2.SetBkColor(M_COLOR_GRAY);
			}

			iOldThread2 = bThreadTaskPcbRun;

			pFrame->m_labelThread2.Invalidate();
		}
		
////////////////////////////////////////////////////////////////////////////////
// ¸ðÅÍ ¿øÁ¡ º¹±Í È®ÀÎ..

		Task.iHomeErrorFlag = motor.HomeCheck();

		//if(Task.iHomeErrorFlag)
		//{
		//	if(!delayDlg->IsWindowVisible())
		//	{
		//		//sLangChange.LoadStringA(IDS_STRING1399);	//ÀüÃ¼ ¿øÁ¡ º¹±Í¸¦ ¿Ï·áÇÏÁö ¸ø Çß½À´Ï´Ù.
		//		//delayMsg(sLangChange.GetBuffer(99), 5000, M_COLOR_RED);
		//	}
		//}

		if(iOldHomeErrorFlag != Task.iHomeErrorFlag)
		{
			if(Task.iHomeErrorFlag && !iOldHomeErrorFlag)
			{
				pFrame->m_labelHom.SetBkColor(M_COLOR_RED);

				pFrame->m_btnOrigin.m_iStateBtn =2;
				pFrame->m_btnOrigin.Invalidate();

				if(Task.AutoFlag)
				{
					Dio.setAlarm(ALARM_ON);
					sLangChange.LoadStringA(IDS_STRING1399);	//ÀüÃ¼ ¿øÁ¡ º¹±Í¸¦ ¿Ï·áÇÏÁö ¸ø Çß½À´Ï´Ù.
					pFrame->putListLog(sLangChange);
					pFrame->Invalidate();
				}
			}
			else if(!Task.iHomeErrorFlag && iOldHomeErrorFlag)
			{
				pFrame->m_labelHom.SetBkColor(M_COLOR_GREEN);
				pFrame->Invalidate();

				pFrame->m_btnOrigin.m_iStateBtn = 3;
				pFrame->m_btnOrigin.Invalidate();
			}
			iOldHomeErrorFlag = Task.iHomeErrorFlag;
		}

////////////////////////////////////////////////////////////////////////////////
// ¸ðÅÍ µå¶óÀÌºê ¾Ë¶÷ Display

		int iAmpfault = 0;
		//for(int i=0;i<MAX_MOTOR_NO;i++)
		//{
		//	//if(motor.GetAmpFault(i))
		//	//{
		//	//	iAmpfault += i+1;
		//	//	if(!delayDlg->IsWindowVisible())
		//	//	{
		//	//		//sLangChange.LoadStringA(IDS_STRING1053);	//Servo Alarm ÀÌ °¨Áö µÇ¾î ÀÖ½À´Ï´Ù.
		//	//		//delayMsg(sLangChange.GetBuffer(99), 5000, M_COLOR_RED);
		//	//	}
		//	//}
		//}

		if(iAmpfault == 0){
			pFrame->m_labelServo.SetBkColor(M_COLOR_GREEN);
		}else{
			pFrame->m_labelServo.SetBkColor(M_COLOR_RED);
		}
		if(iAmpfault != oldAmpFaultFlag){
			pFrame->	m_labelServo.Invalidate();
		}
		Task.iAmpFaultFlag = oldAmpFaultFlag = iAmpfault; 


////////////////////////////////////////////////////////////////////////////////
// Limit ¼¾¼­ °¨Áö Display
#if 1
 		//for(int i=0; i<MAX_MOTOR_NO; i++)
 		//{
			//if ( motor.GetNegaSensor(i) )
 		//	{
			//	if (bThreadTaskReadyRun == true || i == Motor_PCB_Z)// && i==Motor_Lens_Yt)
			//		continue;
 		//		Task.iLimitErrorFlag =1;
			//	break;
 		//	}
			//else 
			//	Task.iLimitErrorFlag =0;
			//
			//if ( motor.GetPosiSensor(i) ) 
 		//	{
			//	if(i == Motor_Lens_Z)continue;
 		//		Task.iLimitErrorFlag =1;
			//	break;
 		//	}
			//else			Task.iLimitErrorFlag =0;
 
			//if (motor.GetAmpFault(i))
			//{
			//	sTempLang.LoadStringA(IDS_STRING152);	//[%s] AMP ¾Ë¶÷ °¨Áö
			//	sLangChange.Format(sTempLang, MotorName[i]);
			//	sprintf_s(sLog, sLangChange);
			//	if (!delayDlg->IsWindowVisible())				//delayMsg(sLog,3000,M_COLOR_RED);
			//		Task.iLimitErrorFlag = 1;
			//	break;
			//}
			//else			Task.iLimitErrorFlag =0;
			//Sleep(15);
 		//}
#endif

		if(Task.iLimitErrorFlag ==1 && Task.AutoFlag)
		{
			CString sLog2="";
			sLog2.Format("%s",sLog);
			errMsg2(Task.AutoFlag, sLog2);
			Task.AutoFlag = 0;			//	ÀÏ½ÃÁ¤Áö½Ã »ç¿ëÇÔ 0: ÀÏ½ÃÁ¤Áö 1: Run	
			pFrame->AutoRunView(Task.AutoFlag);
			bThreadTaskPcb =0;			//	 Á¤Áö ÇÒ¶§ »ç¿ë 0:¾²·¹µå ºüÁ® ³ª¿È
			bThreadTaskLens =0;			//	 Á¤Áö ÇÒ¶§ »ç¿ë 0:¾²·¹µå ºüÁ® ³ª¿È
			bThreadTaskLens_Align =0;			//	 Á¤Áö ÇÒ¶§ »ç¿ë 0:¾²·¹µå ºüÁ® ³ª¿È
		}

		if(iOldLimitFlag != Task.iLimitErrorFlag)
		{
			if(Task.iLimitErrorFlag ==1 && Task.AutoFlag)
			{
				Dio.setAlarm(ALARM_ON);
				Task.AutoFlag = 0;			//	ÀÏ½ÃÁ¤Áö½Ã »ç¿ëÇÔ 0: ÀÏ½ÃÁ¤Áö 1: Run	
				pFrame->AutoRunView(Task.AutoFlag);
				bThreadTaskPcb =0;			//	 Á¤Áö ÇÒ¶§ »ç¿ë 0:¾²·¹µå ºüÁ® ³ª¿È
				bThreadTaskLens =0;			//	 Á¤Áö ÇÒ¶§ »ç¿ë 0:¾²·¹µå ºüÁ® ³ª¿È
				bThreadTaskLens_Align =0;			//	 Á¤Áö ÇÒ¶§ »ç¿ë 0:¾²·¹µå ºüÁ® ³ª¿È
			}

			iOldLimitFlag = Task.iLimitErrorFlag;
		}

		////////////////////////////////////////////////////////////////////////////////
		// Door Open °¨Áö..
		Task.iDoorFlag = 0x00;
        int doorFlag = 0x00;
		if(curInDio[0] & DIO_IN_DOORSENSOR1)			Task.iDoorFlag += 0x0001;
		if(curInDio[0] & DIO_IN_DOORSENSOR2)			Task.iDoorFlag += 0x0002;
		if(curInDio[0] & DIO_IN_DOORSENSOR3)			Task.iDoorFlag += 0x0004;
        if (curInDio[0] & DIO_IN_DOORSENSOR4)			Task.iDoorFlag += 0x0008;
        if (curInDio[0] & DIO_IN_DOORSENSOR5)			Task.iDoorFlag += 0x0010;
        doorFlag = 0x0007;
		if(iOldDoorOpenFlag != Task.iDoorFlag) 
		{
			if (Task.iDoorFlag != doorFlag) //0x003F = 63  //0x001F
			{
				pFrame->m_LabelDoor.SetBkColor(M_COLOR_RED);
				Task.iDoorFlag2 = 1;//door ÀüºÎ ¾È ´Ý°ÜÀÖÀ»¶§
			}
			else
			{
				pFrame->m_LabelDoor.SetBkColor(M_COLOR_GREEN);
				Task.iDoorFlag2 = 0;
			}
			pFrame->m_LabelDoor.Invalidate();	
		}
		iOldDoorOpenFlag = Task.iDoorFlag;


//-- ¼öµ¿¸ðµå Áß¿¡ PCB °­Á¦·Î ÇØÁ¦ ÇßÀ»¶§.. Àü·ù ÃøÁ¤À¸·Î µ¿¿µ»óÀ» ÇØÁ¦
		//if(Task.AutoFlag == 0 && gMIUDevice.CurrentState == 4)
		//{
		//	if( !func_Insp_CurrentMeasure(false))
		//	{//µ¿¿µ»ó ÇØÁ¦
		//		gMIUDevice.CurrentState = 3;

		//		pFrame->m_csLock_Miu.Lock();
		//		if(!MIU.Stop())
		//		{
		//			pFrame->putListLog(MIU.sLogMessage);
		//			delayMsg(MIU.sLogMessage.GetBuffer(999));
		//		}
		//		else
		//		{
		//			sLangChange.LoadStringA(IDS_STRING1424);	//Ä«¸Þ¶ó ¸ðµâ Åë½Å Close.
		//			pFrame->putListLog(sLangChange);
		//		}

		//		pFrame->m_bMiuRun = false;
		//		pFrame->m_csLock_Miu.Unlock();
		//	}
		//}

//-- ¹ÙÄÚµå Reading
		CString sBarCode = "";
		if( barcode.func_Barcode_Read(sBarCode) ) 
		{	//¹ÙÄÚµå°¡ ÀÐÇô Á³À»¶§...
			CString sData="";
			int len = sBarCode.GetLength();
			sData = sBarCode.Mid(0, sBarCode.GetLength()-2);
			//if(len > 12)
			//{
			//	//sData = sBarCode.Mid(0, 12);
			//}
			//else	
			//{
			//	//sData = sBarCode;
			//}
			if( Task.AutoFlag == 1 )
			{
				if( (Task.PCBTask > 10000) &&  (Task.PCBTask < 10180) )
				{//Loading ºÎÅÍ ¸ðµ¨ ÀÎ½Ä È®ÀÎ±îÁö¸¸ Data Task·Î Push
					Task.m_bPBStart = 1;	//¹ÙÄÚµå Á¤»óÀûÀ¸·Î ÀÐÇô Á³À»¶§.		 
					sprintf_s(Task.ChipID, sData, sizeof(sData) );
					pFrame->func_ChipID_Draw();	
				}
			}
			else if( Task.AutoFlag == 0 )
			{//Á¤Áö(¼öµ¿)»óÅÂ¿¡¼­´Â Ç×»ó Push
				Task.m_bPBStart = 1;	//¹ÙÄÚµå Á¤»óÀûÀ¸·Î ÀÐÇô Á³À»¶§.		
				sprintf_s(Task.ChipID, sData, sizeof(sData) );
				pFrame->func_ChipID_Draw();	
			}
		}
		Sleep(1);
	}



	bThreadMonitorRun = false;

	return true;
}

UINT Thread_Socket_ReadingData(LPVOID parm)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	int iRtnStep = 1;
	CString sSocketMsg;
	double dSocketTime = myTimer(true);
	char logMsg[1000];

	//sprintf_s(logMsg, "¹ÙÄÚµå AA DATA Åë½Å Reading ½ÃÀÛ..");
	//pFrame->putListLog(logMsg);

	bThreadSocket = true;
	while(bThreadSocket)
	{
		checkMessage();
		Sleep(5);
		checkMessage();

		switch(iRtnStep)
		{
		case 1:
			//sprintf_s(logMsg, "THREAD Àü¼Û¿ä±¸.");
			//pFrame->putListLog(logMsg); 

			Task.iRecvLenACK[0] = Task.iRecvLenACK[1] = -1;
			sSocketMsg.Format("GETTILT_%s", Task.ChipID);
			pFrame->SendMessageToClient(0, sSocketMsg);

			sSocketMsg.Format("GETTILT_%s", Task.ChipID);
			pFrame->SendMessageToClient(1, sSocketMsg);
			dSocketTime = myTimer(true);
			iRtnStep = 2;
			break;
		case 2:
			if( Task.iRecvLenACK[0] == 1  || Task.iRecvLenACK[1] == 1 )
			{//AA2´ëÁß ÇÑ´ë¶óµµ ÀÏÄ¡ ÇÒ °æ¿ì Data ¿ä±¸
				sSocketMsg.Format("GETDATA_%s", Task.ChipID);
				if( Task.iRecvLenACK[0] == 1 )
				{
					pFrame->SendMessageToClient(0, sSocketMsg);
					Task.iSocketRecvNo = 0;
				}
				else
				{
					pFrame->SendMessageToClient(1, sSocketMsg);
					Task.iSocketRecvNo = 1;
				}

				Task.iRecvLenCnt[0] = Task.iRecvLenCnt[1] = Task.iRecvLenCnt[2] = Task.iRecvLenCnt[3] = Task.iRecvLenCnt[4] = -1;
				dSocketTime = myTimer(true);
				iRtnStep = 10;
			}
			else if((myTimer(true) - dSocketTime) > 5000)
			{
				//sprintf_s(logMsg, "AA¼³ºñ ÀÏÄ¡ ¸ðµ¨ÀÌ Á¸ÀçÇÏÁö ¾Ê½À´Ï´Ù.");
				//pFrame->putListLog(logMsg);
				bThreadSocket = false;
			}
			break;

		case 10:
			if(Task.iRecvLenCnt[0] == 1 &&
			Task.iRecvLenCnt[1] == 1 &&
			Task.iRecvLenCnt[2] == 1 &&
			Task.iRecvLenCnt[3] == 1 &&
			Task.iRecvLenCnt[4] == 1 )
			{//Data ¼ö½Å ¿Ï·á 
				sLangChange.LoadStringA(IDS_STRING1087);	//Socket Åë½Å ¼ö½Å ¿Ï·á.
				sprintf_s(logMsg, sLangChange);
				pFrame->putListLog(logMsg);
				iRtnStep = 999;
				bThreadSocket = false;
			}
			else if(myTimer(true) - dSocketTime > 10000)
			{
				sLangChange.LoadStringA(IDS_STRING1086);	//Socket Åë½Å ¼ö½Å ½Ã°£ ÃÊ°ú.
				sprintf_s(logMsg, sLangChange);
				pFrame->putListLog(logMsg);
				bThreadSocket = false;
			}
			break;
		}

		Sleep(5);
		checkMessage();
	}

	if(iRtnStep == 999)
	{
		sLangChange.LoadStringA(IDS_STRING932);	//PCB DATA[%s] :Tx(%.04lf), Ty(%.04lf), X(%.04lf), Y(%.04lf), T(%.04lf)
		sprintf_s(logMsg, sLangChange, 
				Task.ChipID, Task.dTiltingManual[0], Task.dTiltingManual[1],
				Task.dAlignManual[0], Task.dAlignManual[1], Task.dAlignManual[2]);
		pFrame->putListLog(logMsg);
	}
	//else
	//{
	//	sprintf_s(logMsg, "¹ÙÄÚµå AA DATA Åë½Å ½ÇÆÐ..");
	//	pFrame->putListLog(logMsg);
	//}

	bThreadSocket = false;
	pThread_SocketRead = NULL;
	return 1;
}
//CWinThread* pThread_TaskUv = NULL;
//bool bThreadUvRun = false; 
UINT Thread_TaskUv(LPVOID parm)
{
	bThreadUvRun = true;

	UVCommand.UV_Shutter_Open();
	if (sysData.nUvIndex == 1)
	{
		Sleep(100);
		UVCommand2.UV_Shutter_Open();
	}


	Sleep(model.UV_Time);
	UVCommand.UV_Shutter_Close();


	if (sysData.nUvIndex == 1)
	{
		Sleep(100);
		UVCommand2.UV_Shutter_Close();
	}


	bThreadUvRun = false;
	return 1;
}
UINT Thread_TaskOrigin(LPVOID parm)
{
	bThreadOriginRun = true;
	CString sTemp = _T("");
	
	sTemp.Format("ÀüÃ¼ ¿øÁ¡ º¹±Í ¼öÇàÇÕ´Ï´Ù");	//ÀüÃ¼ ¿øÁ¡ º¹±Í ¼öÇàÇÕ´Ï´Ù
	delayMsg(sTemp, 50000, M_COLOR_DARK_GREEN);
	theApp.MainDlg->putListLog(sTemp);
	Sleep(100);
	//
	memset(Task.ChipID, 0x00, 256);
	sprintf_s(Task.ChipID, "EMPTY");
	Task.m_bPBStart = 0;	//¹ÙÄÚµå Á¤º¸ ÃÊ±âÈ­
	theApp.MainDlg->func_ChipID_Draw();



	bool bHomeComplete = false;
	bHomeComplete = motor.HomeProcessAll();

	if (bHomeComplete)
	{
		Dio.setAlarm(ALARM_OFF);
		sLangChange.LoadStringA(IDS_STRING1397);	//ÀüÃ¼ ¿øÁ¡ º¹±Í ¿Ï·á
		sTemp.Format(sLangChange);
		delayMsg(sTemp.GetBuffer(99), 50000, M_COLOR_GREEN);
	}
	else
	{
		sLangChange.LoadStringA(IDS_STRING1393);	//ÀüÃ¼ ¿øÁ¡ º¹±Í ½ÇÆÐ
		sTemp.Format(sLangChange);
		delayMsg(sTemp.GetBuffer(99), 50000, M_COLOR_RED);
	}
	theApp.MainDlg->putListLog(sTemp);
	g_bMovingflag = false;
	bThreadOriginRun = false;
	return 1;
}
UINT Thread_TaskPcb(LPVOID parm)
{
	CString logStr="";
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	 
	vision.setLiveMode(true);

	pFrame->putListLog(""); 
	pFrame->putListLog("");  


 	pFrame->m_iCurCamNo =  0;
	if(Task.PCBTask >= 60000)
	{
		pFrame->ctrlSubDlg(MAIN_DLG);
		pFrame->changeMainBtnColor(MAIN_DLG);
		pFrame->setCamDisplay(3,1);		
	}
	else
	{
 		pFrame->ctrlSubDlg(MAIN_DLG);
		pFrame->changeMainBtnColor(MAIN_DLG);
	}

	vision.clearOverlay(CAM1);
	vision.drawOverlay(CAM1);


	if ( bThreadTaskPcbRun == true)
	{
//		errMsg2(Task.AutoFlag, "PCB ¾²·¹µå°¡ µ¿ÀÛ Áß ÀÔ´Ï´Ù.");
		pFrame->putListLog("PCB ¾²·¹µå°¡ µ¿ÀÛ Áß ÀÔ´Ï´Ù.");
		return 0;
	}

	int i_alarm_flag = pFrame->checkAutoRunPcbAlarm(Task.PCBTask);

	if(i_alarm_flag)
	{
		logStr.Format("[¿îÀü Á¤Áö] %s", pFrame->sz_PCB_Error_Msg);
		errMsg2(Task.AutoFlag,logStr);

		Task.AutoFlag = 0;
		pFrame->AutoRunView(Task.AutoFlag);
		Task.PCBTask = 0;
		bThreadTaskPcb = false;
		bThreadTaskPcbRun = false;
		g_bMovingflag = false;

		return 1;
	}


	bThreadTaskPcb = true;
	bThreadTaskPcbRun = true;

	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	Task.m_iHour	= sysTime.wHour;
	Task.m_iMin		= sysTime.wMinute;
	Task.m_iSec		= sysTime.wSecond;

	int oldTask = 0;
	pFrame->DisableButton(true);

	while(bThreadTaskPcb && (Task.PCBTask >= Task.m_iStart_Step_PCB) && (Task.PCBTask < Task.m_iEnd_Step_PCB))
	{
		if(Task.AutoFlag != 1)			//0:Á¤Áö 1: ÀÚµ¿ 
		{
			sLangChange.LoadStringA(IDS_STRING403);
			logStr.Format(sLangChange);	//AutoRunFlag OFF »óÅÂ . PCBºÎ step :-1
			pFrame->putListLog(logStr);
			break;
		}
		
		if( Task.PCBTask >= 12500)
		{
			//if( Dio.StartPBOnCheck(true, false) )
			//{
			//	//logStr.Format(_T("ÀÚµ¿ ÁøÇà Áß Àü¸é Door Stop ½ºÀ§Ä¡°¡ OFF »óÅÂ ÀÔ´Ï´Ù."));
			//	sLangChange.LoadStringA(IDS_STRING1373);	//ÀÚµ¿ ÁøÇà Áß Àü¸é Start/Stop ½ºÀ§Ä¡°¡ ÀÛµ¿..\n ÀÚµ¿¸ðµå¸¦ ÀÏ½Ã Á¤Áö ÇÕ´Ï´Ù.
			//	logStr.Format(sLangChange);
			//	errMsg2(Task.AutoFlag, logStr);
			//	break;
			//}
		}
		//if( (sysData.m_iFront == 0))
		//{
		//	if( (Dio.LightCurtainOnCheck(true, false)) && (Task.PCBTask > 11000) )
		//	{//Àü¸é Ä¿Æ° °¨Áö¼¾¼­°¡ °¨Áö µÉ¶§
		//		sLangChange.LoadStringA(IDS_STRING1372);	//ÀÚµ¿ ÁøÇà Áß Àü¸é Door Area °¨Áö.. ÀÚµ¿¸ðµå ÀÏ½Ã Á¤Áö.
		//		logStr.Format(sLangChange);
		//		errMsg2(Task.AutoFlag, logStr);
		//		break;
		//	}
		//}
		//

		////////////////////////////////////////////////////////////////////////////////
		// Door Open °¨Áö..
		if( (Task.iDoorFlag2 == 1) && (sysData.iDoorInterlock == 1) )
		{
			logStr.Format("ÀÚµ¿ ÁøÇà Áß Door Open ¾Ë¶÷ ¹ß»ý.. ÀÚµ¿¸ðµå ÀÏ½Ã Á¤Áö.");
			errMsg2(Task.AutoFlag, logStr);
			break;
		}

		i_alarm_flag = pFrame->checkAutoRunPcbAlarm(Task.PCBTask);

		if(i_alarm_flag != 0)
		{
			Dio.setAlarm(ALARM_ON);
			delayMsg(pFrame->sz_PCB_Error_Msg, 1000000, M_COLOR_RED);

			Task.AutoFlag = 0;
			pFrame->AutoRunView(Task.AutoFlag);
			Task.PCBTask = -abs(Task.PCBTask);
			continue;
		}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////¤»¤»¤»
		if ( Task.PCBTask >= 10000 && Task.PCBTask < 11000 )
		{
			Task.PCBTask = pFrame->RunProc_ProductLoading(Task.PCBTask); //! »ç¿ëÀÚ Á¦Ç° Loading	
		}
		else if ( Task.PCBTask >= 11000 && Task.PCBTask < 49000 )
		{
			if(sysData.m_iProductComp == 1)
			{
					Task.PCBTask = pFrame->procProductComplete(Task.PCBTask);	// ¿Ï¼ºÇ° °Ë»ç ÀÏ °æ¿ì
			}else
			{
#if (____AA_WAY == PCB_TILT_AA)
				if( Task.PCBTask >= 11000 && Task.PCBTask < 15000 )			Task.PCBTask = pFrame->RunProc_PCBOutsideAlign(Task.PCBTask);		//! PCB ¿ÜºÎ Align Step		
				else if( Task.PCBTask >= 15000 && Task.PCBTask < 26000 )	Task.PCBTask = pFrame->RunProc_LensNewPassPickup(Task.PCBTask);		//! Lens Pickup¿¡ Á¦Ç° ³Ñ±è Step
				else if( Task.PCBTask >= 26000 && Task.PCBTask < 27000 )	Task.PCBTask = pFrame->RunProc_SensorAlign(Task.PCBTask);		//! Sensor Align Step + Laser ÃøÁ¤
				else if (Task.PCBTask >= 27000 && Task.PCBTask < 29000)		Task.PCBTask = pFrame->RunProc_LaserMeasure(Task.PCBTask);			//pcb ·¹ÀÌÀú ÃøÁ¤
				else if( Task.PCBTask >= 30000 && Task.PCBTask < 39000 )	Task.PCBTask = pFrame->RunProc_EpoxyNewResing(Task.PCBTask);		//! Epoxy µ¿ÀÛ ¹× µµÆ÷ °Ë»ç Step
				else if( Task.PCBTask >= 39000 && Task.PCBTask <= 40000 )	Task.PCBTask = pFrame->RunProc_InspAAPos(Task.PCBTask);			//! Defect °Ë»çÈÄ º»µùÀ§Ä¡ Step
#elif (____AA_WAY == LENS_TILT_AA)
				if( Task.PCBTask >= 11000 && Task.PCBTask < 15000 )			Task.PCBTask = pFrame->RunProc_PCBOutsideAlign(Task.PCBTask);					//È¦´õ ¾ó¶óÀÎ
				else if( Task.PCBTask >= 26000 && Task.PCBTask < 27000 )	Task.PCBTask = pFrame->RunProc_SensorAlign(Task.PCBTask);						//¼¾¼­ ¾ó¶óÀÎ
				else if( Task.PCBTask >= 27000 && Task.PCBTask < 28000 )	Task.PCBTask = pFrame->RunProc_LaserMeasure(Task.PCBTask);						//pcb ·¹ÀÌÀú ÃøÁ¤
				else if( Task.PCBTask >= 30000 && Task.PCBTask < 35000 )	Task.PCBTask = pFrame->RunProc_EpoxyNewResing(Task.PCBTask);					//µµÆ÷
				else if( Task.PCBTask >= 35000 && Task.PCBTask < 39000 )	Task.PCBTask = pFrame->RunProc_LensAlign(Task.PCBTask);							//·»Áî ¾ó¶óÀÎ
				else if (Task.PCBTask >= 39000 && Task.PCBTask < 40000)		Task.PCBTask = pFrame->RunProc_InspAAPos(Task.PCBTask);							//´ÙÅ©,µðÆå°Ë»ç
				else if( Task.PCBTask >= 40000 && Task.PCBTask < 41000 )	Task.PCBTask = pFrame->RunProc_LensLoading(Task.PCBTask);						//·»Áî ·Îµù
#endif
				else if( Task.PCBTask >= 41000 && Task.PCBTask < 49000 )	Task.PCBTask = pFrame->RunProc_Bonding_Pos_GO(Task.PCBTask);					//·»Áî ·Îµù
			}
		}
		else if ( Task.PCBTask >= 49000 && Task.PCBTask < 50000 )
		{
			Task.PCBTask = 60000;
			Task.iMTFCnt = 0;
		}
		else if ( Task.PCBTask >= 60000 && Task.PCBTask <80000 )
		{
			Task.PCBTask = pFrame->procAutoFocus(Task.PCBTask);			//Active Align µ¿ÀÛ
		}
		else if ( Task.PCBTask >= 80000 && Task.PCBTask <110000 )
		{
			Task.iMTFCnt = 0;
			Task.bFirstAA = true;
			Task.PCBTask = 60200;	//O.C Align ¾ÈÇÏ°í... 2Â÷ AA·Î ...
			MIU.func_Set_InspImageCopy(CHART_RAW);			//1Â÷ AAÈÄ 
		}		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		else if ( Task.PCBTask >= 110000 && Task.PCBTask <120000 )
		{
			Task.PCBTask = pFrame->UV_process(Task.PCBTask);
		}
		else if ( Task.PCBTask >= 120000 && Task.PCBTask <130000)
		{			
            if (sysData.m_iProductComp == 1)
            {
                Task.PCBTask = pFrame->Complete_FinalInsp(Task.PCBTask);
            }
            else
            {
                Task.PCBTask = pFrame->func_MandoFinalSFR(Task.PCBTask);
            }
		}
		if (Task.PCBTask >= Task.m_iEnd_Step_PCB)
		{
			//if(!bThreadTaskLens)	//Dio.setAlarm(ALARM_OFF);	// Lens  ¾²·¹µå ¸ØÃß¸é Yellow	
			Dio.setAlarm(ALARM_OFF);	// Lens  ¾²·¹µå ¸ØÃß¸é Yellow
			break;
		}
		

		Sleep(5);

	}

	pFrame->DisableButton(false);
	pFrame->dispGrid();
	if(Task.m_iEnd_Step_PCB < 100000)
	{
		Task.AutoFlag = 0;
		pFrame->AutoRunView(Task.AutoFlag);
	}

//	Task.AutoFlag = 0;
//	Task.PCBTask = 0;

	Task.PausePCBStep = abs(Task.PCBTask);

	bThreadTaskPcb = false;
	bThreadTaskPcbRun = false;

	Task.PcbOnStage = 100;
	logStr.Format("PCB AA-Bonding µ¿ÀÛ ¾²·¹µå Á¾·á.");
	pFrame->putListLog(logStr);
	
	Task.m_iHour = Task.m_iMin = Task.m_iSec = 0;

	delayMsg(logStr.GetBuffer(999), 3000, M_COLOR_RED);

	g_bMovingflag = false;
	
	if (Task.AutoFlag == 0)
	{
		MIU.Stop();					// 95 ~ 100 msec
	}					// 95 ~ 100 msec

	
	logStr.Empty();
	return 1;
}

UINT Thread_TaskLens(LPVOID parm)
{
	
	CString logStr="";
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;


	vision.setLiveMode(true);

	pFrame->putListLog("");
	pFrame->putListLog("");


 	pFrame->m_iCurCamNo = 0;
	if(Task.LensTask >= 60000)
	{
		pFrame->ctrlSubDlg(MAIN_DLG);
		pFrame->changeMainBtnColor(MAIN_DLG);
		pFrame->setCamDisplay(3,1);		
	}
	else
	{
 		pFrame->ctrlSubDlg(MAIN_DLG);
		pFrame->changeMainBtnColor(MAIN_DLG);
	}

	vision.clearOverlay(CAM2);
	vision.drawOverlay(CAM2);

	if ( bThreadTaskLensRun == true)
	{
//		errMsg2(Task.AutoFlag, "Lens ¾²·¹µå°¡ µ¿ÀÛ Áß ÀÔ´Ï´Ù.");
		return 0;
	}

	int i_alarm_flag = pFrame->checkAutoRunLensAlarm(Task.LensTask);

	if(i_alarm_flag)
	{
		sLangChange.LoadStringA(IDS_STRING297); //[¿îÀü Á¤Áö]
		logStr.Format(sLangChange, pFrame->sz_LENS_Error_Msg);
		errMsg2(Task.AutoFlag,logStr);

		Task.AutoFlag = 0;
		pFrame->AutoRunView(Task.AutoFlag);
		//Task.LensTask = 0;
		//bThreadTaskLens = false;
		//bThreadTaskLensRun = false;
		//bThreadTaskLens_Align = false;
		g_bMovingflag = false;

		return 1;
	}


	bThreadTaskLens = true;
	bThreadTaskLensRun = true;

	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	Task.m_iHour	= sysTime.wHour;
	Task.m_iMin		= sysTime.wMinute;
	Task.m_iSec		= sysTime.wSecond;

	int oldTask = 0;
	pFrame->DisableButton(true);

	while(bThreadTaskLens && (Task.LensTask >= Task.m_iStart_Step_LENS) && (Task.LensTask < Task.m_iEnd_Step_LENS))
	{
		if(Task.AutoFlag != 1)			//0:Á¤Áö 1: ÀÚµ¿ 
		{
			sLangChange.LoadStringA(IDS_STRING403);
			logStr.Format(sLangChange);	//AutoRunFlag OFF »óÅÂ . LensºÎ step :-1
			pFrame->putListLog(logStr);
			break;
		}
		
		if( Task.LensTask >= 12500)
		{
			//if( Dio.StartPBOnCheck(true, false) )
			//{
			//	//logStr.Format(_T("ÀÚµ¿ ÁøÇà Áß Àü¸é Door Stop ½ºÀ§Ä¡°¡ OFF »óÅÂ ÀÔ´Ï´Ù."));
			//	sLangChange.LoadStringA(IDS_STRING1373);	//ÀÚµ¿ ÁøÇà Áß Àü¸é Start/Stop ½ºÀ§Ä¡°¡ ÀÛµ¿..\n ÀÚµ¿¸ðµå¸¦ ÀÏ½Ã Á¤Áö ÇÕ´Ï´Ù.
			//	logStr.Format(sLangChange);
			//	errMsg2(Task.AutoFlag, logStr);
			//	break;
			//}
		}
		if( (sysData.m_iFront == 0) )
		{
			if( (Dio.LightCurtainOnCheck(true, false)) && (Task.LensTask > 11000) )
			{//Àü¸é Ä¿Æ° °¨Áö¼¾¼­°¡ °¨Áö µÉ¶§
				sLangChange.LoadStringA(IDS_STRING1372);	//ÀÚµ¿ ÁøÇà Áß Àü¸é Door Area °¨Áö.. ÀÚµ¿¸ðµå ÀÏ½Ã Á¤Áö.
				logStr.Format(sLangChange);
				errMsg2(Task.AutoFlag, logStr);
				break;
			}
		}
		////////////////////////////////////////////////////////////////////////////////
		if( (Task.iDoorFlag2 == 1) && (sysData.iDoorInterlock == 1) )
		{
			sLangChange.LoadStringA(IDS_STRING1371);	//ÀÚµ¿ ÁøÇà Áß Door Open ¾Ë¶÷ ¹ß»ý.. ÀÚµ¿¸ðµå ÀÏ½Ã Á¤Áö.
			logStr.Format(sLangChange);
			errMsg2(Task.AutoFlag, logStr);
			break;
		}

		i_alarm_flag = pFrame->checkAutoRunLensAlarm(Task.LensTask);

		if(i_alarm_flag != 0)
		{
			Dio.setAlarm(ALARM_ON);
			delayMsg(pFrame->sz_LENS_Error_Msg, 1000000, M_COLOR_RED);

			Task.AutoFlag = 0;
			pFrame->AutoRunView(Task.AutoFlag);
			Task.LensTask = -abs(Task.LensTask);

			continue;
		}
		
//¡Ú¡Ú¡Ú¡Ú///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		if(Task.LensTask >= 30000 && Task.LensTask < 50000)
		{
			Task.LensTask = pFrame->RunProc_LENS_AlignLaserMeasure(Task.LensTask);		//! LENS Align Step
		}

//¡Ú¡Ú¡Ú¡Ú///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if(Task.LensTask != oldTask)
		{
			SYSTEMTIME time;
			::GetLocalTime(&time);

			FILE *out; 

			CFileFind finder;
			BOOL IsFind=FALSE;
			CString FolderName = "";

			FolderName.Format("%s\\%04d%02d\\%02d", LOG_DIR, time.wYear, time.wMonth, time.wDay);

			IsFind = finder.FindFile(FolderName);
			if(!IsFind)
			{
				FolderName.Format("%s", BASE_DIR);
				IsFind = finder.FindFile(FolderName);
				if(!IsFind)
				{
					CreateDirectory(FolderName,NULL); 
				}

				FolderName.Format("%s", LOG_DIR);
				IsFind = finder.FindFile(FolderName);
				if(!IsFind)
				{
					CreateDirectory(FolderName,NULL); 
				}

				FolderName.Format("%s\\%04d%02d", LOG_DIR, time.wYear, time.wMonth);
				CreateDirectory(FolderName,NULL); 

				FolderName.Format("%s\\%04d%02d\\%02d", LOG_DIR, time.wYear, time.wMonth, time.wDay);
				CreateDirectory(FolderName,NULL); 
			}

			finder.Close();

			char fileName[1000];
			wsprintf(fileName, "%s\\Lens Thread.txt", FolderName);

			if(!fopen_s(&out, fileName,"a"))
			{
				fprintf_s(out, "%02d:%02d:%02d:%03d, %d, %d, %d\n",		time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, oldTask, Task.PCBTask, Task.LensTask);
				if(out)		fclose(out);
			}

			oldTask = Task.LensTask;
		}

		if (Task.LensTask >= Task.m_iEnd_Step_LENS)
		{
			//if(!bThreadTaskLens)	//Dio.setAlarm(ALARM_OFF);	// Lens  ¾²·¹µå ¸ØÃß¸é Yellow	
//			Dio.setAlarm(ALARM_OFF);	// Lens  ¾²·¹µå ¸ØÃß¸é Yellow
			break;
		} 
		

		Sleep(5);

		checkMessage();
	}

	pFrame->DisableButton(false);

	pFrame->dispGrid();

	if(Task.m_iEnd_Step_LENS < 100000)
	{
//		Task.AutoFlag = 0;
		pFrame->AutoRunView(Task.AutoFlag);
	}

//	Task.AutoFlag = 0;
//	Task.LensTask = 0;

	Task.PauseLensStep = abs(Task.LensTask);

	bThreadTaskLens = false;
	bThreadTaskLensRun = false;

	Task.LensOnStage = 100;
	sLangChange.LoadStringA(IDS_STRING900);	//"Lens AA-Bonding µ¿ÀÛ ¾²·¹µå Á¾·á.
	logStr.Format(_T("Lens AA-Bonding µ¿ÀÛ ¾²·¹µå Á¾·á."));//sLangChange);
	pFrame->putListLog(logStr);
	
	Task.m_iHour = Task.m_iMin = Task.m_iSec = 0;

//	delayMsg(logStr.GetBuffer(999));

	g_bMovingflag = false;
	
	return 1;
}

UINT Thread_Clock(LPVOID parm)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	WORD wOldDay = 0;
	CString strTime;
	CString strOldTime;
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	wOldDay = sysTime.wDay;

	bThreadClock = true;
	bThreadClockRun = true;

	while (bThreadClock)
	{
		::GetLocalTime(&sysTime);

		strTime.Format("%02d : %02d : %02d", sysTime.wHour,sysTime.wMinute, sysTime.wSecond);
		if(strOldTime != strTime){
			pFrame->m_labelTime.SetText(strTime);
			strOldTime = strTime;
		}

		Sleep(200);
		checkMessage();
	}
	 
	bThreadClockRun = false;

	return true;
}

UINT Thread_CheckDate(LPVOID parm)
{//----- ³¯Â¥¸¦ ¼ö½Ã·Î CheckÇÏ¿© ºÒÇÊ¿äÇÑ µ¥ÀÌÅÍ¸¦ »èÁ¦½ÃÅ²´Ù...
	CAABonderDlg *pFrame = (CAABonderDlg *)(AfxGetApp()->m_pMainWnd); 

	int year=0, month=0, day=0,hour=0,minute=0,second=0;
	GetDateAndTime(year, month, day, hour, minute, second);

	while(1)
	{
		if(!g_chkdateTH_flag)	break;		

		GetDateAndTime(year, month, day, hour, minute, second);

		pFrame->DeleteOldData(year, month, day);


		::Sleep(600000);
	}

	return 0;
}


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ´ëÈ­ »óÀÚ µ¥ÀÌÅÍÀÔ´Ï´Ù.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Áö¿øÀÔ´Ï´Ù.

// ±¸ÇöÀÔ´Ï´Ù.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CAABonderDlg ´ëÈ­ »óÀÚ




CAABonderDlg::CAABonderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAABonderDlg::IDD, pParent)
	, m_iCcd(0)
	, bEpoxyTimeChk(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//! <---------------------------------------------------------------------------
	//! Added by LHW (2013/2/5, 2013/3/6)
	m_iMode_Mouse_Box = 0;	//! Mouse·Î »ç°¢ ¿µ¿ª ¼³Á¤ÇÏ´Â ÀÛ¾÷ÀÇ Á¾·ù °ª, 0 based	
		

	pThread_CCM_Grab = NULL; 
	pThread_CCM_Display = NULL;

	pThread_CCM_Mil_CvtColor_Ready = NULL;
	pThread_CCM_Mil_CvtColor = NULL;

	pThread_CCM_CvtColor = NULL;
	pThread_CCM_CvtMil = NULL;
	//! <---------------------------------------------------------------------------

	//! <---------------------------------------
	//! Added by LHW (2013/3/27)
	m_bState_CCD_Zoom = false;
	m_bBox_CCD_Zoom = false;
	m_bBox_Acting_CCD_Zoom = false;
	m_bActing_CCD_Zoom = false;
	m_bPan_CCD_Zoom = false;
	m_bActing_Pan_CCD_Zoom = false;
	m_rect_CCD_Zoom.SetRectEmpty();
	//! <---------------------------------------

	//! Added by LHW (2013/5/2)
	m_pos[0] = m_pos[1] = NULL;
}

void CAABonderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
	DDX_Control(pDX, IDC_LABEL_MENU, m_labelMenu);
	DDX_Control(pDX, IDC_LABEL_TITLE, m_labelTitle);
	DDX_Control(pDX, IDC_LABEL_STATUS_HOM, m_labelHom);
	DDX_Control(pDX, IDC_LABEL_STATUS_SERVO, m_labelServo);
	DDX_Control(pDX, IDC_LABEL_STATUS_MES, m_labelMes);
	
	DDX_Control(pDX, IDC_LABEL_STATUS_USB_MODULE, m_labelUsbModule);
	DDX_Control(pDX, IDC_LABEL_TIME, m_labelTime);

	DDX_Control(pDX, IDC_LABEL_PICKUP_NO_LENS, m_labelPickupNoLensCentering);
	DDX_Control(pDX, IDC_LABEL_PICKUP_NO_LENS_GRIP, m_labelPickupNoLensGrip);
	DDX_Control(pDX, IDC_LABEL_PICKUP_NO_PCB, m_labelPickupNoPcb);

	DDX_Control(pDX, IDC_LABEL_STATUS_THREAD1, m_labelThread1);
	DDX_Control(pDX, IDC_LABEL_STATUS_THREAD2, m_labelThread2);

	DDX_Control(pDX, IDC_LABEL_MODELNAME, m_labelCurModelName);
	DDX_Control(pDX, IDC_LABEL_ID, m_labelCCD_ID);
	DDX_Control(pDX, IDC_LABEL_LOT_NAME, m_labelLotName);
	DDX_Control(pDX, IDC_LABEL_CCD_RETRY, m_labelCcdRetryCnt);
	DDX_Control(pDX, IDC_LABEL_STATUS_USB_LIVE, m_labelUsbLive);

	DDX_Control(pDX, IDC_BUTTON_ORIGIN, m_btnOrigin);
	DDX_Control(pDX, IDC_BUTTON_READY, m_btnReady);
	DDX_Control(pDX, IDC_BUTTON_AUTORUN, m_btnAutorun);
	DDX_Control(pDX, IDC_BUTTON_PAUSE, m_btnPause);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);
	DDX_Control(pDX, IDC_BUTTON_NG_OUT, m_btnNgOut);
	DDX_Control(pDX, IDC_AUTORUN_START, m_btnStart);
	

	DDX_Check(pDX, IDC_CHECK_DIST, m_bMeasureDist);

	DDX_Control(pDX, IDC_LABEL_STATUS_DOOR, m_LabelDoor);
	DDX_Control(pDX, IDC_BUTTON_MAIN, m_bMainBtn_Main);
	DDX_Control(pDX, IDC_BUTTON_MODEL, m_bMainBtn_Model);
	DDX_Control(pDX, IDC_BUTTON_LENS, m_bMainBtn_Align);
	DDX_Control(pDX, IDC_BUTTON_CCD, m_bMainBtn_CCD);
	DDX_Control(pDX, IDC_BUTTON_MOTOR, m_bMainBtn_Motor);
	DDX_Control(pDX, IDC_BUTTON_IO, m_bMainBtn_IO);
	DDX_Control(pDX, IDC_BUTTON_LIGHT, m_bMainBtn_Light);
	DDX_Control(pDX, IDC_BUTTON_ALARM, m_bMainBtn_Alarm);
	DDX_Control(pDX, IDC_BUTTON_PCB_RESULT, m_bPcbFinish);
	DDX_Control(pDX, IDC_BUTTON_DISPENSE_RESULT, m_bDispenseFinish);
	DDX_Control(pDX, IDC_BUTTON_LENS_PASS_RESULT, m_bLensPassFinish);
	DDX_Control(pDX, IDC_BUTTON_TIME_CHECK, m_EpoxyTimeCheck);
	DDX_Control(pDX, IDC_BUTTON_PROCOMP, m_bProCompCheck);
	DDX_Control(pDX, IDC_BUTTON_SMINI_OQMODE, m_bSminiOQCheck);
	
}

BEGIN_MESSAGE_MAP(CAABonderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CAABonderDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAABonderDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CAABonderDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_MAIN, &CAABonderDlg::OnBnClickedButtonMain)
	ON_BN_CLICKED(IDC_BUTTON_LENS, &CAABonderDlg::OnBnClickedButtonLens)
	ON_BN_CLICKED(IDC_BUTTON_CCD, &CAABonderDlg::OnBnClickedButtonCcd)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR, &CAABonderDlg::OnBnClickedButtonMotor)
	ON_BN_CLICKED(IDC_BUTTON_IO, &CAABonderDlg::OnBnClickedButtonIo)
	ON_BN_CLICKED(IDC_BUTTON_MODEL, &CAABonderDlg::OnBnClickedButtonModel)
//	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_LENS_SUPPLY, &CAABonderDlg::OnBnClickedButtonLensSupply)
	ON_BN_CLICKED(IDC_BUTTON_PCB_OS_CHECK, &CAABonderDlg::OnBnClickedButtonPcbOsCheck)
	ON_BN_CLICKED(IDC_BUTTON_PCB_SUPPLY, &CAABonderDlg::OnBnClickedButtonPcbSupply)
	ON_BN_CLICKED(IDC_BUTTON_CCD_ALIGN, &CAABonderDlg::OnBnClickedButtonCcdAlign)
	ON_BN_CLICKED(IDC_BUTTON_CCD_INSP, &CAABonderDlg::OnBnClickedButtonCcdInsp)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN, &CAABonderDlg::OnBnClickedButtonOrigin)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON_LIGHT, &CAABonderDlg::OnBnClickedButtonLight)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CAABonderDlg::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CAABonderDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_AUTORUN, &CAABonderDlg::OnBnClickedButtonAutorun)
	ON_BN_CLICKED(IDC_BUTTON_READY, &CAABonderDlg::OnBnClickedButtonReady)
	ON_BN_CLICKED(IDC_RADIO_ALIGN, &CAABonderDlg::OnBnClickedRadioAlign)
	ON_BN_CLICKED(IDC_RADIO_CCD2, &CAABonderDlg::OnBnClickedRadioCcd2)
	ON_STN_CLICKED(IDC_LABEL_STATUS_USB_LIVE, &CAABonderDlg::OnStnClickedLabelStatusUsbLive)
	ON_BN_CLICKED(IDC_CHECK_DIST, &CAABonderDlg::OnBnClickedCheckDist)
	ON_BN_CLICKED(IDC_BUTTON_ALARM, &CAABonderDlg::OnBnClickedButtonAlarm)
	ON_STN_CLICKED(IDC_LABEL_STATUS_SERVO, &CAABonderDlg::OnStnClickedLabelStatusServo)
	ON_BN_CLICKED(IDC_BUTTON_NG_OUT, &CAABonderDlg::OnBnClickedButtonNgOut)
	ON_BN_CLICKED(IDC_BUTTON_PCB_RESULT, &CAABonderDlg::OnBnClickedButtonPcbResult)
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_LABEL_TITLE, &CAABonderDlg::OnClickedLabelTitle)
	ON_STN_CLICKED(IDC_LABEL_CCD_RETRY, &CAABonderDlg::OnStnClickedLabelCcdRetry)
	ON_BN_CLICKED(IDC_BUTTON_DISPENSE_RESULT, &CAABonderDlg::OnBnClickedButtonDispenseResult)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_LENS_PASS_RESULT, &CAABonderDlg::OnBnClickedButtonLensPassResult)
	ON_BN_CLICKED(IDC_BUTTON_MINIMIZE, &CAABonderDlg::OnBnClickedButtonMinimize)
	ON_BN_CLICKED(IDC_BUTTON_TIME_CHECK, &CAABonderDlg::OnBnClickedButtonTimeCheck)
	ON_STN_CLICKED(IDC_LABEL_LOT_NAME, &CAABonderDlg::OnStnClickedLabelLotName)
	ON_BN_CLICKED(IDC_SERVER_START, &CAABonderDlg::OnBnClickedServerStart)
	ON_BN_CLICKED(IDC_SERVER_STOP, &CAABonderDlg::OnBnClickedServerStop)
	ON_BN_CLICKED(IDC_CLIENT_CONNECT, &CAABonderDlg::OnBnClickedClientConnect)
	ON_BN_CLICKED(IDC_CLIENT_DISCONNECT, &CAABonderDlg::OnBnClickedClientDisconnect)
	ON_BN_CLICKED(IDC_CLIENT_SEND, &CAABonderDlg::OnBnClickedClientSend)
	ON_BN_CLICKED(IDC_BUTTON_PROCOMP, &CAABonderDlg::OnBnClickedButtonProcomp)
	ON_STN_CLICKED(IDC_LABEL_ID, &CAABonderDlg::OnStnClickedLabelId)
	ON_NOTIFY(NM_DBLCLK, IDC_STATIC_RESULT_GRID, &CAABonderDlg::OnDBClickedGridResult)
	ON_BN_CLICKED(IDC_AUTORUN_START, &CAABonderDlg::OnBnClickedAutorunStart)
	ON_BN_CLICKED(IDC_DOOR_OPEN, &CAABonderDlg::OnBnClickedDoorOpen)
	ON_BN_CLICKED(IDC_DOOR_CLOSE, &CAABonderDlg::OnBnClickedDoorClose)
	ON_STN_CLICKED(IDC_LABEL_STATUS_MES, &CAABonderDlg::OnStnClickedLabelStatusMes)
	ON_STN_CLICKED(IDC_LABEL_MODELNAME, &CAABonderDlg::OnStnClickedLabelModelname)
	ON_BN_CLICKED(IDC_BUTTON_SMINI_OQMODE, &CAABonderDlg::OnBnClickedButtonSminiOqmode)
END_MESSAGE_MAP()


// CAABonderDlg ¸Þ½ÃÁö Ã³¸®±â

BOOL CAABonderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// IDM_ABOUTBOX´Â ½Ã½ºÅÛ ¸í·É ¹üÀ§¿¡ ÀÖ¾î¾ß ÇÕ´Ï´Ù.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadStringA(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	theApp.MainDlg = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

#if (____AA_WAY == PCB_TILT_AA)
	TITLE_MOTOR_Z = Motor_PCB_Z;
	TITLE_MOTOR_TX = Motor_PCB_Xt;
	TITLE_MOTOR_TY = Motor_PCB_Yt;
	TITLE_MOTOR_X = Motor_PCB_X;
	TITLE_MOTOR_Y = Motor_PCB_Y;
#elif (____AA_WAY == LENS_TILT_AA)
	TITLE_MOTOR_Z = Motor_Lens_Z;
	TITLE_MOTOR_TX = Motor_Lens_Xt;
	TITLE_MOTOR_TY = Motor_Lens_Yt;
	TITLE_MOTOR_X = Motor_Lens_X;
	TITLE_MOTOR_Y = Motor_Lens_Y;
#endif

	// ÀÌ ´ëÈ­ »óÀÚÀÇ ¾ÆÀÌÄÜÀ» ¼³Á¤ÇÕ´Ï´Ù. ÀÀ¿ë ÇÁ·Î±×·¥ÀÇ ÁÖ Ã¢ÀÌ ´ëÈ­ »óÀÚ°¡ ¾Æ´Ò °æ¿ì¿¡´Â
	//  ÇÁ·¹ÀÓ¿öÅ©°¡ ÀÌ ÀÛ¾÷À» ÀÚµ¿À¸·Î ¼öÇàÇÕ´Ï´Ù.
	SetIcon(m_hIcon, TRUE);			// Å« ¾ÆÀÌÄÜÀ» ¼³Á¤ÇÕ´Ï´Ù.
	SetIcon(m_hIcon, FALSE);		// ÀÛÀº ¾ÆÀÌÄÜÀ» ¼³Á¤ÇÕ´Ï´Ù.
	Start_Btn_On = false;

	LogSave("ÇÁ·Î±×·¥ ½ÇÇà.");

	modelList.NewLoad();
	sysData.Load();
	g_AlarmFlag = true;
	g_AlarmCnt = 0;
	output_Mode = ASCII_MODE;


	password.Load();
	MandoSfrSpec.NewSfrLoad();
	//MandoSfrSpec.load();
	myTimer(false);

	m_bMiuRun = false;

#ifdef ON_LINE_VISION
	if(!vision.initVB())
	{
		errMsg2(Task.AutoFlag, "Ä«¸Þ¶ó ¿µ»ó º¸µå ÃÊ±âÈ­ ½ÇÆÐ");
		return FALSE;
	}
#endif

	
	//modelList.Load();
	MIU.setInterface();
	model.Load();
	work.Load();
	model.PatternLoad();
	
    model.AcmisDataLoad();

    model.RoiLoad();
	bool bRet = false;
	func_Set_SFR_N_Type();

	//! MbufBayer ÇÔ¼ö »ç¿ë ¿©ºÎ¿¡ µû¶ó¼­, MIU ¸ðµâÀÇ ¹öÆÛ »ý¼º ¹æ¹ýÀ» º¯°æÇØ¾ß ÇÑ´Ù.
	//! Added by LHW (2013/3/12)
	if ( model.m_eGrabMethod == MIL_BAYER_CVT_COLOR )
	{
		MIU.m_bFlag_Color_Covert_Ext = true;
		MIU.m_bFlag_Mil_Convert = true;
	}
	else
	{
		MIU.m_bFlag_Color_Covert_Ext = false;
		MIU.m_bFlag_Mil_Convert = true;
	}

#ifdef ON_LINE_VISION
	vision.initDisplay();
	vision.initMarkDisplay(MARK_SIZE_X, MARK_SIZE_Y);

	CWnd* pWnd;
	pWnd = (CWnd*)GetDlgItem(IDC_DISP_PCB);//IDC_DISP_LENS
	MdispSelectWindow(vision.MilDisplay[0], vision.MilSmallImageChild[0], pWnd->m_hWnd);
	//
	pWnd = (CWnd*)GetDlgItem(IDC_DISP_CCD);
	MdispSelectWindow(vision.MilDisplay[3], vision.MilSmallImage[1], pWnd->m_hWnd);

	vision.enableOverlay();
	
#endif
#ifdef		ON_LINE_MOTOR
	bool initFlag = motor.Axl_Init();
	if(!initFlag)
	{
		//errMsg2(Task.AutoFlag, msg); 

		char logStr[1000];
		sprintf_s(logStr, "¸ðÅÍ ÃÊ±âÈ­¿¡ ½ÇÆÐ ÇÏ¿´½À´Ï´Ù.");
		errMsg2(Task.AutoFlag, logStr);

		sprintf_s(logStr, "¸ðÅÍ ÃÊ±âÈ­¿¡ ½ÇÆÐ ÇÏ¿´½À´Ï´Ù.");
		LogSave(logStr);

		OnCancel();
	}

	//Dio.PCBvaccumOn(VACCUM_OFF, true);
#endif
	
	SetInterface();


#ifdef ON_LINE_VISION
	bRet = vision.MiuBufferAlloc(gMIUDevice.nWidth, gMIUDevice.nHeight);
#endif
	//! <---------------------------------------
	//! Added by LHW (2013/3/27)	
	m_rect_CCD_Zoom.left = 0;
	m_rect_CCD_Zoom.top = 0; 
	m_rect_CCD_Zoom.right = gMIUDevice.nWidth;
	m_rect_CCD_Zoom.bottom = gMIUDevice.nHeight;
	//! <---------------------------------------

	m_bisAlignBtn = false;
	m_bisMotorBtn = false;
	m_bisLightBtn = false;

	m_iCurCamNo = 3;
	ctrlSubDlg(MAIN_DLG);
	changeMainBtnColor(MAIN_DLG);
	vision.clearOverlay(CAM1);
	vision.drawOverlay(CAM1);
	vision.clearOverlay(CAM2);
	vision.drawOverlay(CAM2);
	vision.clearOverlay(CCD);
	vision.drawOverlay(CCD);
	 
	m_mmResult = timeSetEvent(1000, 1, NULL, NULL, TIME_PERIODIC );
	 

#ifdef ON_LINE_MONITOR
	pThread_Monitor    = ::AfxBeginThread(Thread_Monitor, this);
#endif
	pThread_Clock	   = ::AfxBeginThread(Thread_Clock, this);


#ifdef ON_LINE_VISION
	vision.setLiveMode(true);
	pThread_Grab = ::AfxBeginThread(Thread_Grab, this);
#endif

	/* ¼­¹ö ¼ÒÄÏ ¹× DB */
	if(!g_ADOData.func_AA_DBConnect())
	{
		putListLog(" [ ACCESS DB ] Open Fail!");
	}

	//MESConnectToServer();

	/*µµÆ÷ ½Ã°£ Timer*/
	if( work.m_Epoxy_Time_Flag == 0)
	{
		sLangChange.LoadStringA(IDS_STRING538);//EPOXY TIME START
		GetDlgItem(IDC_BUTTON_TIME_CHECK)->SetWindowText(sLangChange);
		m_EpoxyTimeCheck.m_iStateBtn = 0;
	}
	else 
	{
		sLangChange.LoadStringA(IDS_STRING1451);//EPOXY TIME STOP
		GetDlgItem(IDC_BUTTON_TIME_CHECK)->SetWindowText(sLangChange);
		m_EpoxyTimeCheck.m_iStateBtn = 1;
	}
	m_EpoxyTimeCheck.Invalidate();


	m_oldDlg = -1;

	g_chkdateTH_flag = true;
	/////pThread_CheckDate = AfxBeginThread(Thread_CheckDate, this);
	
	SetTimer(999, 500, NULL);
	SetTimer(9, 3000, NULL);

#ifdef ON_LINE_VISION
	for(int i = 0; i < MARK_CNT; i++)
	{
		int iCh = 1;
		if(iCh == PCB_Chip_MARK)	iCh = 0;

		for(int j = 0; j < 2; j++){
			vision.geometricMarkPreProc(iCh, i, j);
		}
	}
#endif
	 
	CString logStr="";
	logStr.Format("%d", work.m_iCoverUpDownCnt);
	m_labelCcdRetryCnt.SetText(logStr);
	m_labelCcdRetryCnt.Invalidate();
	
	Rs232Init();


#if (____AA_WAY == PCB_TILT_AA)
	putListLog(" [INFO] PCB AA");
#elif (____AA_WAY == LENS_TILT_AA)
	putListLog(" [INFO] LENS AA");
#endif


	logStr.Empty();
	return TRUE;  // Æ÷Ä¿½º¸¦ ÄÁÆ®·Ñ¿¡ ¼³Á¤ÇÏÁö ¾ÊÀ¸¸é TRUE¸¦ ¹ÝÈ¯ÇÕ´Ï´Ù.
}
void CAABonderDlg::Rs232Init()
{
	SerialPortList.GetList_SerialPort();
	// ¹ÙÄÚµå Æ÷Æ® ¿ÀÇÂ
	CString logStr="";
	if( !barcode.func_Comm_Open(sysData.iCommPort[COMM_BARCODE], sysData.iBaudRate[COMM_BARCODE]) )
	{
		sLangChange.LoadStringA(IDS_STRING263);	//[¹ÙÄÚµå ¸®´õ±â] Åë½Å ¿¬°á ½ÇÆÐ
		logStr.Format("[Barcode] Åë½Å ¿¬°á ½ÇÆÐ : COM%d" , sysData.iCommPort[COMM_BARCODE]);

		//delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
		putListLog(logStr);
		Task.bConnectBarcode = false;
	}
	else
	{
		//sLangChange.LoadStringA(IDS_STRING262);	//[¹ÙÄÚµå ¸®´õ±â] Åë½Å ¿¬°á ¼º°ø
		logStr.Format("[Barcode] Åë½Å ¿¬°á ¼º°ø : COM%d", sysData.iCommPort[COMM_BARCODE]);
		Task.bConnectBarcode = true;
		putListLog(logStr);
	}
	bool bRet_Con_RS232C = false;
	CString sCommPort = _T("");
 
	if (sysData.m_iIrChartUse == 1)
	{
		if (!IrvAlignLed.Irv_RS232_CommOpen(sysData.iCommPort[COMM_LIGHT2], sysData.iBaudRate[COMM_LIGHT2], IR_CHART))
		{

			logStr.Format("[IR CHART] Åë½Å ¿¬°á ½ÇÆÐ : COM%d", sysData.iCommPort[COMM_LIGHT2]);
			//delayMsg(logStr.GetBuffer(99), 50000, M_COLOR_RED);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("[IR CHART] Åë½Å ¿¬°á ¼º°ø : COM%d", sysData.iCommPort[COMM_LIGHT2]);
			putListLog(logStr);
			Sleep(100);
			IrvAlignLed.IrvLight_Power(1, IR_CHART);
		}

		if (!IrvAlignLed.Irv_RS232_CommOpen(sysData.iCommPort[COMM_LIGHT1], sysData.iBaudRate[COMM_LIGHT1], IR_OC))
		{

			logStr.Format("[IR OC] Åë½Å ¿¬°á ½ÇÆÐ : COM%d", sysData.iCommPort[COMM_LIGHT1]);
			//delayMsg(logStr.GetBuffer(99), 50000, M_COLOR_RED);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("[IR OC] Åë½Å ¿¬°á ¼º°ø : COM%d", sysData.iCommPort[COMM_LIGHT1]);
			putListLog(logStr);
			Sleep(100);
			IrvAlignLed.IrvLight_Power(1, IR_OC);
		}

		sCommPort.Format("COM%d", sysData.iCommPort[COMM_LIGHT3]);
		LightControlthird.SetReceiveProcPtr(this);
		bRet_Con_RS232C = LightControlthird.Connect_Device(sCommPort, 0);
	}
	else
	{
		sCommPort.Format("COM%d", sysData.iCommPort[COMM_LIGHT1]);
		LightControl.SetReceiveProcPtr(this);
		bRet_Con_RS232C = LightControl.Connect_Device(sCommPort, 0);
		//

		//Á¶¸í 2
		sCommPort.Format("COM%d", sysData.iCommPort[COMM_LIGHT2]);
		LightControlSecond.SetReceiveProcPtr(this);
		bRet_Con_RS232C = LightControlSecond.Connect_Device(sCommPort, 0);

		//Á¶¸í 3

		sCommPort.Format("COM%d", sysData.iCommPort[COMM_LIGHT3]);
		LightControlthird.SetReceiveProcPtr(this);
		bRet_Con_RS232C = LightControlthird.Connect_Device(sCommPort, 0);

	}
	

	
	

	
   
   


	

#ifdef ON_LINE_KEYENCE

	if( !Keyence.func_RS232_CommOpen(sysData.iCommPort[COMM_LASER], sysData.iBaudRate[COMM_LASER]) )
	{
		sLangChange.LoadStringA(IDS_STRING275);
		logStr.Format("[º¯À§ ÃøÁ¤±â] Åë½Å ¿¬°á ½ÇÆÐ : COM%d",sysData.iCommPort[COMM_LASER]);
		//delayMsg(logStr.GetBuffer(99), 50000, M_COLOR_RED);
		putListLog(logStr);
	}
	else
	{
		if( !Keyence.func_LT9030_KeyLock(true))
		{
			logStr.Format("[º¯À§ ÃøÁ¤±â] Åë½Å ÀÌ»ó ¹ß»ý..Lock µ¿ÀÛ Fail..CableÀ» È®ÀÎ ÇÏ¼¼¿ä.");
			//delayMsg(logStr.GetBuffer(99), 50000, M_COLOR_RED);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("[º¯À§ ÃøÁ¤±â] Åë½Å ¿¬°á ¼º°ø : COM%d", sysData.iCommPort[COMM_LASER]);
			putListLog(logStr);
		}
		
	}
#endif
	
	if (!UVCommand.Connect_Device(sysData.iCommPort[COMM_UV]))
	{
		logStr.Format("[UV] Åë½Å ¿¬°á ½ÇÆÐ : COM%d", sysData.iCommPort[COMM_UV]);
		putListLog(logStr);
	}
	else
	{
		logStr.Format("[UV] Åë½Å ¿¬°á ¼º°ø : COM%d", sysData.iCommPort[COMM_UV]);
		putListLog(logStr);
		Sleep(100);
		//UVCommand.UV_Shutter_Open();//
		UVCommand.UV_Shutter_PowerSet(model.UV_Power);// 95);//
		logStr.Format("	#1 UV POWER SET: %d", model.UV_Power);
		putListLog(logStr);

	}
	if (!UVCommand2.Connect_Device(sysData.iCommPort[COMM_UV2]))
	{
		logStr.Format("[UV 2] Åë½Å ¿¬°á ½ÇÆÐ : COM%d", sysData.iCommPort[COMM_UV2]);
		putListLog(logStr);
	}
	else
	{
		logStr.Format("[UV 2] Åë½Å ¿¬°á ¼º°ø : COM%d", sysData.iCommPort[COMM_UV2]);
		putListLog(logStr);
		Sleep(100);
		//UVCommand.UV_Shutter_Open();//
		UVCommand2.UV_Shutter_PowerSet(model.UV_Power);// 95);//
		logStr.Format("	#2 UV POWER SET : %d", model.UV_Power);
		putListLog(logStr);
		
		//UVCommand.UV_Shutter_Open(90);//

	}
}
void CAABonderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ´ëÈ­ »óÀÚ¿¡ ÃÖ¼ÒÈ­ ´ÜÃß¸¦ Ãß°¡ÇÒ °æ¿ì ¾ÆÀÌÄÜÀ» ±×¸®·Á¸é
//  ¾Æ·¡ ÄÚµå°¡ ÇÊ¿äÇÕ´Ï´Ù. ¹®¼­/ºä ¸ðµ¨À» »ç¿ëÇÏ´Â MFC ÀÀ¿ë ÇÁ·Î±×·¥ÀÇ °æ¿ì¿¡´Â
//  ÇÁ·¹ÀÓ¿öÅ©¿¡¼­ ÀÌ ÀÛ¾÷À» ÀÚµ¿À¸·Î ¼öÇàÇÕ´Ï´Ù.

void CAABonderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ±×¸®±â¸¦ À§ÇÑ µð¹ÙÀÌ½º ÄÁÅØ½ºÆ®ÀÔ´Ï´Ù.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Å¬¶óÀÌ¾ðÆ® »ç°¢Çü¿¡¼­ ¾ÆÀÌÄÜÀ» °¡¿îµ¥¿¡ ¸ÂÃä´Ï´Ù.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ¾ÆÀÌÄÜÀ» ±×¸³´Ï´Ù.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// »ç¿ëÀÚ°¡ ÃÖ¼ÒÈ­µÈ Ã¢À» ²ô´Â µ¿¾È¿¡ Ä¿¼­°¡ Ç¥½ÃµÇµµ·Ï ½Ã½ºÅÛ¿¡¼­
//  ÀÌ ÇÔ¼ö¸¦ È£ÃâÇÕ´Ï´Ù.
HCURSOR CAABonderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAABonderDlg::OnBnClickedOk(){}
void CAABonderDlg::OnBnClickedCancel(){}


//void CAABonderDlg::setCamDisplay(int camNo, int mode)
//{
//	if(camNo<0 || camNo>=MAX_CAM_NO)
//	{
//		CString errStr="";
//		errStr.Format("setCamDisplay() Function Call Error. [cam %d, mode %d]", camNo, mode);
//
//		LogSave(errStr);
//		camNo = 0;
//	}
//	int disp1 = 0;
//	int disp2 = 0;
//	int disp3 = 0;
//	int disp4 = 0;
//
//	if(camNo==0)
//	{
//		disp1 = IDC_DISP_PCB;
//		disp2 = IDC_DISP_LENS;
//		disp3 = IDC_DISP_CCD;
//		disp4 = IDC_DISP_PCB2;
//	}
//	else if(camNo==1)
//	{
//		disp1 = IDC_DISP_LENS;
//		disp2 = IDC_DISP_PCB;
//		disp3 = IDC_DISP_CCD;
//		disp4 = IDC_DISP_PCB2;
//	}
//	else if(camNo==2)
//	{
//		disp1 = IDC_DISP_CCD;
//		disp2 = IDC_DISP_PCB;
//		disp3 = IDC_DISP_LENS;
//		disp4 = IDC_DISP_PCB2;
//	}
//	else if(camNo==3)
//	{
//		disp1 = IDC_DISP_CCD;
//		disp2 = IDC_DISP_PCB;
//		disp3 = IDC_DISP_LENS;
//		disp4 = IDC_DISP_PCB2;
//	}
//
//	GetDlgItem(disp1)->MoveWindow(m_rectCamDispPos1.left, m_rectCamDispPos1.top, SMALL_CAM_SIZE_X, SMALL_CAM_SIZE_Y);
//	GetDlgItem(disp2)->MoveWindow(m_rectCamDispPos2.left, m_rectCamDispPos2.top, SMALL_CAM_SIZE_X, SMALL_CAM_SIZE_Y);
//	GetDlgItem(disp3)->MoveWindow(m_rectCamDispPos3.left, m_rectCamDispPos3.top, SMALL_CAM_SIZE_X, SMALL_CAM_SIZE_Y);
//
//	GetDlgItem(disp1)->ShowWindow(SW_SHOW);
//	GetDlgItem(disp2)->ShowWindow(SW_SHOW);
//	GetDlgItem(disp3)->ShowWindow(SW_SHOW);
//
//	GetDlgItem(disp4)->ShowWindow(SW_HIDE);
//	//}
//
//	m_iCurCamNo = camNo;
//}
void CAABonderDlg::MainEpoxyRun()
{
	if (bThreadEpoxyRun == true)
	{
		g_bMovingflag = false;
		putListLog("¸ðÅÍ ÀÌµ¿ÁßÀÔ´Ï´Ù.");
		return;
	}
	pThread_Epoxy = ::AfxBeginThread(Thread_Epoxy, this);
}
void CAABonderDlg::setCamDisplay(int camNo, int mode)
{
	if (camNo<0 || camNo >= MAX_CAM_NO)
	{
		CString errStr;
		errStr.Format("setCamDisplay() Function Call Error. [cam %d, mode %d]", camNo, mode);

		LogSave(errStr);
		camNo = 0;
	}

	int disp1, disp2, disp3, disp4;

	if (mode == 0)
	{
		if (camNo == 0)
		{
			disp1 = IDC_DISP_LENS;
			disp2 = IDC_DISP_PCB;
			disp3 = IDC_DISP_PCB2;
			disp4 = IDC_DISP_CCD;
		}
		else if (camNo == 1)
		{
			disp1 = IDC_DISP_PCB;
			disp2 = IDC_DISP_LENS;
			disp3 = IDC_DISP_CCD;
			disp4 = IDC_DISP_PCB2;
		}
		else if (camNo == 2)
		{
			disp1 = IDC_DISP_CCD;
			disp2 = IDC_DISP_LENS;
			disp3 = IDC_DISP_PCB;
			disp4 = IDC_DISP_PCB2;
		}
		else if (camNo == 3)
		{
			disp1 = IDC_DISP_CCD;
			disp2 = IDC_DISP_LENS;
			disp3 = IDC_DISP_PCB;
			disp4 = IDC_DISP_PCB2;
		}
		// 20151006 ¿µÁø ºñÀüÄ«¸Þ¶ó Ãß°¡·Î ÀÎÇÑ ¼öÁ¤
		else if (camNo == 4)
		{
			disp1 = IDC_DISP_PCB;
			disp2 = IDC_DISP_PCB2;
			disp3 = IDC_DISP_LENS;
			disp4 = IDC_DISP_CCD;
		}

		if (camNo == 2 || camNo == 3)
		{
			GetDlgItem(disp1)->MoveWindow(ccdPosLeft, CamPosY, SMALL_CCD_SIZE_X, SMALL_CCD_SIZE_Y);
		}
		else
		{
			GetDlgItem(disp1)->MoveWindow(m_rectCamDispPos1.left, m_rectCamDispPos1.top, SMALL_CAM_SIZE_X, SMALL_CAM_SIZE_Y);
		}


		GetDlgItem(disp1)->ShowWindow(SW_SHOW);
		GetDlgItem(disp2)->ShowWindow(SW_HIDE);
		GetDlgItem(disp3)->ShowWindow(SW_HIDE);
		GetDlgItem(disp4)->ShowWindow(SW_HIDE);

		//		GetDlgItem(IDC_LIST_LOG)->ShowWindow(SW_HIDE);

		autodispDlg->ShowWindow(false);
	}
	else
	{
		if (camNo == 0)
		{
			disp1 = IDC_DISP_CCD;// IDC_DISP_LENS;
			if (m_iCurCamNo == 3)
			{
				disp2 = IDC_DISP_PCB2;
				disp3 = IDC_DISP_PCB;
			}
			else
			{
				disp2 = IDC_DISP_PCB;
				disp3 = IDC_DISP_PCB2;
			}

			disp4 = IDC_DISP_LENS;// IDC_DISP_CCD;
		}
		else if (camNo == 1)
		{
			disp1 = IDC_DISP_PCB;
			disp2 = IDC_DISP_LENS;
			disp3 = IDC_DISP_PCB2;
			disp4 = IDC_DISP_CCD;
		}
		else if (camNo == 2)
		{
			disp1 = IDC_DISP_LENS;
			disp2 = IDC_DISP_PCB2;
			disp3 = IDC_DISP_PCB;
			disp4 = IDC_DISP_CCD;
		}
		else if (camNo == 3)
		{
			disp1 = IDC_DISP_CCD;
			disp2 = IDC_DISP_PCB;
			disp3 = IDC_DISP_LENS;
			disp4 = IDC_DISP_PCB2;
		}

		if (camNo == 3)
		{
			GetDlgItem(disp1)->MoveWindow(ccdPosLeft, CamPosY, SMALL_CCD_SIZE_X, SMALL_CCD_SIZE_Y);
		}
		else
		{
			GetDlgItem(disp1)->MoveWindow(m_rectCamDispPos1.left, m_rectCamDispPos1.top, SMALL_CCD_SIZE_X, SMALL_CCD_SIZE_Y);
			GetDlgItem(disp2)->MoveWindow(m_rectCamDispPos2.left, m_rectCamDispPos2.top, SMALL_CAM_SIZE_X, SMALL_CAM_SIZE_Y);
		}


		GetDlgItem(disp1)->ShowWindow(SW_SHOW);
		GetDlgItem(disp3)->ShowWindow(SW_HIDE);
		GetDlgItem(disp4)->ShowWindow(SW_HIDE);
		if (camNo == 3)
		{
			GetDlgItem(disp2)->ShowWindow(SW_HIDE);

			autodispDlg->ShowWindow(true);
		}
		else
		{
			GetDlgItem(disp2)->ShowWindow(SW_SHOW);
			autodispDlg->ShowWindow(false);
		}
	}

	m_iCurCamNo = camNo;
}
void CAABonderDlg::SetInterface()
{
	WINDOWPLACEMENT wndpl;
	CRect rTemp;
	CString sTemp;
	baseGap = 1;
	//---------------Main Frame-----------------------
	wndpl.rcNormalPosition.left = 0;
	wndpl.rcNormalPosition.top = 0;
	wndpl.rcNormalPosition.right = MAIN_DLG_SIZE_X;
	wndpl.rcNormalPosition.bottom = MAIN_DLG_SIZE_Y;
	this->MoveWindow(&wndpl.rcNormalPosition);
	//	GetDlgItem(IDC_LABEL_RUN_MODE)->GetWindowPlacement(&wndpl);
	GetDlgItem(IDC_LABEL_MODELNAME)->GetWindowPlacement(&wndpl);

	CamPosY = wndpl.rcNormalPosition.top + baseGap;
	int cent = wndpl.rcNormalPosition.right + 1; //(wndpl.rcNormalPosition.right + MAIN_DLG_SIZE_X) / 2;

	

	m_rectCamDispPos1.left = cent;
	m_rectCamDispPos1.right = m_rectCamDispPos1.left + SMALL_CAM_SIZE_X + 2;
	m_rectCamDispPos1.top = wndpl.rcNormalPosition.top;
	m_rectCamDispPos1.bottom = wndpl.rcNormalPosition.top + SMALL_CAM_SIZE_Y;

	//ccd¿µ»ó ÁÂÇ¥ ¼³Á¤
	ccdPosLeft = m_rectCamDispPos1.left;// baseGap;
	ccdPosRight = ccdPosLeft + (SMALL_CCD_SIZE_X);

	m_rectCcdDispPos.left = cent;// baseGap;
	m_rectCcdDispPos.right = ccdPosRight;
	m_rectCcdDispPos.top = CamPosY;
	m_rectCcdDispPos.bottom = CamPosY + SMALL_CCD_SIZE_Y;
	
	m_rectCamDispPos2.top = wndpl.rcNormalPosition.top;
	m_rectCamDispPos2.bottom = wndpl.rcNormalPosition.top + SMALL_CAM_SIZE_Y;
	
	m_rectCamDispPos2.left = ccdPosRight;
	m_rectCamDispPos2.right = m_rectCamDispPos2.left + SMALL_CCD_SIZE_X + 1;

	

	g_iCCDCamView = 5;		// È­¸é Å©±â ¼³Á¤
							/* ListBox */
	wndpl.rcNormalPosition.left = m_rectCamDispPos1.left;
	wndpl.rcNormalPosition.right = m_rectCamDispPos2.left;// -10;// m_rectCamDispPos1.right + ;
	wndpl.rcNormalPosition.top = m_rectCamDispPos1.bottom + 145;
	wndpl.rcNormalPosition.bottom = MAIN_DLG_SIZE_Y - 85;
	GetDlgItem(IDC_LIST_LOG)->MoveWindow(&wndpl.rcNormalPosition);

	/* ¸¶¿ì½º Ä¿¼­ */
	loadStandardCursor();

	DispCurModelName(model.mCurModelName);// model.name);

	//initGrid();
	//InitGridCtrl_Result();
	initInspResGrid();

	SetInterface_CreateDlg();

	SetInterface_Button();
	SetInterface_Label();
	m_iOldDlgNo = -1;
	m_bDrawFlag = false;
	m_bBoxMoveFlag = false;
	m_bBoxMoveFlag_CCD = false;

	Task.AutoFlag = 0;
	AutoRunView(Task.AutoFlag);

	m_bMainBtn_Main.m_iStateBtn = 1;
	m_bMainBtn_Main.Invalidate();


	m_bPcbFinish.m_iStateBtn = 0;
	m_bPcbFinish.Invalidate();

	m_bDispenseFinish.m_iStateBtn = 0;
	m_bDispenseFinish.Invalidate();

	m_bLensPassFinish.m_iStateBtn = 0;
	m_bLensPassFinish.Invalidate();

	m_EpoxyTimeCheck.m_iStateBtn = 0;
	m_bLensPassFinish.Invalidate();

	m_bProCompCheck.m_iStateBtn = 0;
	m_bProCompCheck.Invalidate();

	m_bSminiOQCheck.m_iStateBtn = 0;
	m_bSminiOQCheck.Invalidate();

	
	font.CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_listLog.SetFont(&font, 1);
	font.DeleteObject();


	m_bIsLensMode = 0;
	m_bIsMotorMode = 0;

	if (eepromDlg == NULL) {
		eepromDlg = new CEEPROMTest;
		eepromDlg->Create(IDD_DIALOG_EEPROM);
		eepromDlg->ShowWindow(SW_HIDE);
	}

	NgList_Data.Load();
}

void	CAABonderDlg::SetInterface_Button()
{
	WINDOWPLACEMENT wndpl;
	sLangChange.LoadStringA(IDS_STRING1328);
	GetDlgItem(IDC_BUTTON_ORIGIN)->SetWindowText(sLangChange);
	sLangChange.LoadStringA(IDS_STRING1320);
	GetDlgItem(IDC_BUTTON_READY)->SetWindowText(sLangChange);
	sLangChange.LoadStringA(IDS_STRING1369);
	GetDlgItem(IDC_BUTTON_AUTORUN)->SetWindowText(sLangChange);
	sLangChange.LoadStringA(IDS_STRING1360);
	GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowText(sLangChange);
	sLangChange.LoadStringA(IDS_STRING1400);
	GetDlgItem(IDC_BUTTON_STOP)->SetWindowText(sLangChange);
	sLangChange.LoadStringA(IDS_STRING389);
	GetDlgItem(IDC_RADIO_ALIGN)->SetWindowText(sLangChange);
	sLangChange.LoadStringA(IDS_STRING431);
	GetDlgItem(IDC_RADIO_CCD2)->SetWindowText(sLangChange);
	sLangChange.LoadStringA(IDS_STRING1147);
	GetDlgItem(IDC_CHECK_DIST)->SetWindowText(sLangChange);
	sLangChange.LoadStringA(IDS_STRING1309);
	GetDlgItem(IDC_BUTTON_PROCOMP)->SetWindowText(sLangChange);
	sLangChange.LoadStringA(IDS_STRING1249 );
	GetDlgItem(IDC_BUTTON_MINIMIZE)->SetWindowText(sLangChange);
	sLangChange.LoadStringA(IDS_STRING1406);
	GetDlgItem(IDC_BUTTON_LIGHT)->SetWindowText(sLangChange);
	sLangChange.LoadStringA(IDS_STRING1297);
	GetDlgItem(IDC_BUTTON_ALARM)->SetWindowText(sLangChange);
	sLangChange.LoadStringA(IDS_STRING1292);
	GetDlgItem(IDC_BUTTON_LENS)->SetWindowText(sLangChange);
	sLangChange.LoadStringA(IDS_STRING451);
	GetDlgItem(IDC_BUTTON_CCD)->SetWindowText(sLangChange);
	sLangChange.LoadStringA(IDS_STRING1407);
	GetDlgItem(IDC_BUTTON_EXIT)->SetWindowText(sLangChange);
	//----------------------------------------------------------------------------------------- 
	//-----------------------------------------------------------------------------------------
	/* ¹öÆ° ¹è°æ */
	wndpl.rcNormalPosition.left		= 0;
	wndpl.rcNormalPosition.right	= MAIN_DLG_SIZE_X - 1;
	wndpl.rcNormalPosition.top		= MAIN_DLG_SIZE_Y - 80;
	wndpl.rcNormalPosition.bottom	= MAIN_DLG_SIZE_Y - 1;
	m_labelMenu.MoveWindow(&wndpl.rcNormalPosition);
	m_labelMenu.SetBkColor(M_COLOR_LIGHT_GREEN);

	//-----------------------------------------------------------------------------------------
	CWnd *pbutton[9] = {NULL,};
	pbutton[0] = &m_bMainBtn_Main;
	pbutton[1] = &m_bMainBtn_Model;
	pbutton[2] = &m_bMainBtn_Align;
	pbutton[3] = &m_bMainBtn_Motor;
	pbutton[4] = &m_bMainBtn_CCD;
	pbutton[5] = &m_bMainBtn_IO;
	pbutton[6] = &m_bMainBtn_Light;
	pbutton[7] = &m_bMainBtn_Alarm;
	pbutton[8] = GetDlgItem(IDC_BUTTON_EXIT);

	int btnSize_w = 142;//213;
	wndpl.rcNormalPosition.left = 6;
	wndpl.rcNormalPosition.right = btnSize_w;//btnSize_w;//140;
	wndpl.rcNormalPosition.top = MAIN_DLG_SIZE_Y - 74;
	wndpl.rcNormalPosition.bottom = MAIN_DLG_SIZE_Y - 7;

	for (int i = 0; i < 9; i++) {
		if (i != 0) {
			wndpl.rcNormalPosition.left = wndpl.rcNormalPosition.right + 3;
			wndpl.rcNormalPosition.right += btnSize_w; ;// btnSize_w;//125; 
		}
		pbutton[i]->MoveWindow(&wndpl.rcNormalPosition);
	}

	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------»ó´Ü ¸Þ´º Á¤·Ä
	CRect posRect;
	
	int baseGap = 1;
	GetDlgItem(IDC_LABEL_TITLE)->GetWindowPlacement(&wndpl);

	

	int topClock_W = 220;
	int topDoor_W = 100;
	int topServo_W = 100;
	int topHome_W = 100;
	int topMes_W = 100;
	int topCCD_W = 130;
	int top_H = 68;
	//=================================================================
	//½Ã°£
	
	wndpl.rcNormalPosition.right	= MAIN_DLG_SIZE_X-baseGap;
	wndpl.rcNormalPosition.left		= wndpl.rcNormalPosition.right - topClock_W;
	wndpl.rcNormalPosition.top		= baseGap;
	wndpl.rcNormalPosition.bottom	= top_H;
	GetDlgItem(IDC_LABEL_TIME)->MoveWindow(&wndpl.rcNormalPosition);

	//Door
	wndpl.rcNormalPosition.right	= wndpl.rcNormalPosition.left - baseGap;
	wndpl.rcNormalPosition.left		= wndpl.rcNormalPosition.right - topDoor_W;
	wndpl.rcNormalPosition.top		= baseGap;
	wndpl.rcNormalPosition.bottom	= top_H;
	m_LabelDoor.MoveWindow(&wndpl.rcNormalPosition);
	//mes
	
	wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left - baseGap;
	wndpl.rcNormalPosition.left = wndpl.rcNormalPosition.right - topMes_W;
	wndpl.rcNormalPosition.top = baseGap;
	wndpl.rcNormalPosition.bottom = top_H;
	m_labelMes.MoveWindow(&wndpl.rcNormalPosition);
	//Servo
	wndpl.rcNormalPosition.right	= wndpl.rcNormalPosition.left - baseGap;
	wndpl.rcNormalPosition.left		= wndpl.rcNormalPosition.right - topServo_W;
	wndpl.rcNormalPosition.top		= baseGap;
	wndpl.rcNormalPosition.bottom	= top_H;
	m_labelServo.MoveWindow(&wndpl.rcNormalPosition);

	//Home
	wndpl.rcNormalPosition.right	= wndpl.rcNormalPosition.left - baseGap;
	wndpl.rcNormalPosition.left		= wndpl.rcNormalPosition.right - topHome_W;
	wndpl.rcNormalPosition.top		= baseGap;
	wndpl.rcNormalPosition.bottom	= top_H;
	m_labelHom.MoveWindow(&wndpl.rcNormalPosition);

	//CCD
	wndpl.rcNormalPosition.right	= wndpl.rcNormalPosition.left - baseGap;
	wndpl.rcNormalPosition.left		= wndpl.rcNormalPosition.right - topCCD_W;
	wndpl.rcNormalPosition.top		= baseGap;
	wndpl.rcNormalPosition.bottom	= top_H;
	m_labelUsbLive.MoveWindow(&wndpl.rcNormalPosition);
	
	//Title
	wndpl.rcNormalPosition.right	= wndpl.rcNormalPosition.left - baseGap;
	wndpl.rcNormalPosition.left		= 1;
	wndpl.rcNormalPosition.top		= baseGap;
	wndpl.rcNormalPosition.bottom	= top_H;
	m_labelTitle.MoveWindow(&wndpl.rcNormalPosition);
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	m_btnStart.m_iStateBtn = 0;
}

void CAABonderDlg::SetInterface_Label()
{
	CString sTemp="";
	CString temp="";
	sTemp.Format(" Active Alignment [Ver.%s]", VER_STR);

	
//
	//DEF_VER_DAY
	m_labelTitle
		.SetBkColor(M_COLOR_DARK_GREEN)//M_COLOR_BLUE)
		.SetTextColor(M_COLOR_WHITE)
		.SetFontBold(TRUE)
		.SetText(sTemp)
		.SetFontSize(24);

	

	sLangChange.LoadStringA(IDS_STRING1328);
	m_labelHom
		.SetBkColor(M_COLOR_RED)
		.SetTextColor(M_COLOR_BLACK)
		.SetFontBold(TRUE)
		.SetText(sLangChange) //¿øÁ¡º¹±Í
		.SetFontSize(12);

	sLangChange.LoadStringA(IDS_STRING509);
	m_LabelDoor
		.SetBkColor(M_COLOR_RED)
		.SetTextColor(M_COLOR_BLACK)
		.SetFontBold(TRUE)
		.SetText(sLangChange)
		.SetFontSize(12);
		
	sLangChange.LoadStringA(IDS_STRING1450);
	m_labelThread1
		.SetBkColor(M_COLOR_GRAY)
		.SetTextColor(M_COLOR_BLACK)
		.SetFontBold(TRUE)
		.SetText(sLangChange)
		.SetFontSize(12);

	sLangChange.LoadStringA(IDS_STRING1449);
	m_labelThread2
		.SetBkColor(M_COLOR_GRAY)
		.SetTextColor(M_COLOR_BLACK)
		.SetFontBold(TRUE)
		.SetText(sLangChange)
		.SetFontSize(12);

	sLangChange.LoadStringA(IDS_STRING1056);
	m_labelServo
		.SetBkColor(M_COLOR_RED)
		.SetTextColor(M_COLOR_BLACK)
		.SetFontBold(TRUE)
		.SetText(sLangChange)
		.SetFontSize(12);
	m_labelMes
		.SetBkColor(M_COLOR_RED)
		.SetTextColor(M_COLOR_BLACK)
		.SetFontBold(TRUE)
		.SetFontSize(12);

	m_labelUsbModule
		.SetBkColor(M_COLOR_GRAY)
		.SetTextColor(M_COLOR_BLACK)
		.SetFontBold(TRUE)
		.SetFontSize(12);


	sLangChange.LoadStringA(IDS_STRING441);//"CCD µ¿¿µ»ó" 
	m_labelUsbLive  
		.SetBkColor(M_COLOR_RED) 
		.SetTextColor(M_COLOR_BLACK)
		.SetFontBold(TRUE)
		.SetText(sLangChange)
		.SetFontSize(12);

	SYSTEMTIME time;
	::GetLocalTime(&time);

	sLangChange.Format("%02d : %02d : %02d", time.wHour, time.wMinute, time.wSecond);

	m_labelTime
		.SetBkColor(M_COLOR_DARK_CYAN)
		.SetTextColor(M_COLOR_GREEN)
		.SetFontBold(TRUE)
		.SetText(sLangChange)
		.SetFontSize(30);

	m_labelPickupNoPcb
		.SetBkColor(M_COLOR_GRAY)
		.SetTextColor(M_COLOR_GREEN)
		.SetFontBold(TRUE)
		.SetText("-")
		.SetFontSize(16);

	m_labelPickupNoLensCentering
		.SetBkColor(M_COLOR_GRAY)
		.SetTextColor(M_COLOR_GREEN)
		.SetFontBold(TRUE)
		.SetText("-")
		.SetFontSize(16);

	m_labelPickupNoLensGrip
		.SetBkColor(M_COLOR_GRAY)
		.SetTextColor(M_COLOR_GREEN)
		.SetFontBold(TRUE)
		.SetText("-")
		.SetFontSize(16);

	m_labelCurModelName
		.SetBkColor(M_COLOR_WHITE)
		.SetTextColor(M_COLOR_BLACK)
		.SetFontBold(TRUE)
		.SetFontSize(17);

	m_labelCCD_ID
		.SetBkColor(M_COLOR_WHITE)
		.SetTextColor(M_COLOR_BLACK)
		.SetFontBold(TRUE)
		.SetFontSize(12);
	sLangChange.LoadStringA(IDS_STRING756);	//"Lot ¸í"
	m_labelLotName
		.SetBkColor(M_COLOR_WHITE)
		.SetTextColor(M_COLOR_BLACK)
		.SetFontBold(TRUE)
		.SetText(sLangChange)
		.SetFontSize(14);

	m_labelCcdRetryCnt
		.SetBkColor(M_COLOR_GRAY)
		.SetTextColor(M_COLOR_GREEN)
		.SetFontBold(TRUE)
		.SetText("0")
		.SetFontSize(16);
}

void CAABonderDlg::SetInterface_CreateDlg()
{
	/* Dialog Open */

	if(sfrSpecDlg == NULL){
		sfrSpecDlg = new CSfrSpec;
		sfrSpecDlg->Create(IDD_DIALOG_SFR_SPEC);
		sfrSpecDlg->ShowWindow(SW_HIDE);
	}
	if (chartSetDlg == NULL) {
		chartSetDlg = new CChartSetDlg;
		chartSetDlg->Create(IDD_DIALOG_CHART_SET);
		chartSetDlg->ShowWindow(SW_HIDE);
	}
	
	if(modelDlg == NULL){
		modelDlg = new CModelDlg;
		modelDlg->Create(IDD_DIALOG_MODEL);
		modelDlg->ShowWindow(SW_HIDE);
	}

	if(lensDlg == NULL){
		lensDlg = new CLensDlg;
 		lensDlg->Create(IDD_DIALOG_LENS);
 		lensDlg->ShowWindow(SW_HIDE);
	}

	if(lensEdgeDlg == NULL){
		lensEdgeDlg = new CLensEdgeDlg;
 		lensEdgeDlg->Create(IDD_DIALOG_LENS_EDGE);
 		lensEdgeDlg->ShowWindow(SW_HIDE);
	}

	if(pcbDlg == NULL){
 		pcbDlg = new CPcbDlg;
 		pcbDlg->Create(IDD_DIALOG_PCB);
 		pcbDlg->ShowWindow(SW_HIDE);
	}


	if(ccdDlg == NULL){
 		ccdDlg = new CCcdDlg;
 		ccdDlg->Create(IDD_DIALOG_CCD);
 		ccdDlg->ShowWindow(SW_HIDE);
	}
 
	if(motorDlg == NULL){
 		motorDlg = new CMotorDlg;
 		motorDlg->Create(IDD_DIALOG_LENS_TEACHING);
 		motorDlg->ShowWindow(SW_HIDE);
	}
 
	if(motorDlg2 == NULL){
		motorDlg2 = new CMotorDlg2;
		motorDlg2->Create(IDD_DIALOG_PCB_TEACHING);
		motorDlg2->ShowWindow(SW_HIDE);
	}

	if(motorDlg3 == NULL){
		motorDlg3 = new CMotorDlg3;
		motorDlg3->Create(IDD_DIALOG_PCB2_TEACHING);
		motorDlg3->ShowWindow(SW_HIDE);
	}

	if(lightDlg == NULL){
		lightDlg = new CLightDlg;
		lightDlg->Create(IDD_DIALOG_LIGHT);
		lightDlg->ShowWindow(SW_HIDE);
	}

	
// 	DataSet = new CDataSet;
// 	DataSet->Create(IDD_DIALOG_DATASET);
// 	DataSet->ShowWindow(SW_HIDE);

	if(ioDlg == NULL){
		ioDlg = new CIoDlg;
 		ioDlg->Create(IDD_DIALOG_IO);
 		ioDlg->ShowWindow(SW_HIDE);
	}

	if(delayDlg == NULL){
		delayDlg = new CDelayMsgDlg;
		delayDlg->Create(IDD_DIALOG_DELAY);
		delayDlg->ShowWindow(SW_HIDE);
	}

	/*if( TiltingManualdlg == NULL )
	{
		TiltingManualdlg = new CTiltingManualDlg;
		TiltingManualdlg->Create(IDD_DIALOG_MANUAL_TILTING);
		TiltingManualdlg->ShowWindow(SW_HIDE);
	}*/

	if(autodispDlg == NULL){
		autodispDlg = new CAutoDispDlg;
		autodispDlg->Create(IDD_AUTODISP_DIALOG);
		autodispDlg->ShowWindow(SW_HIDE);
	}

	if(alarmDlg == NULL){
		alarmDlg = new CAlarmDialog;
		alarmDlg->Create(IDD_DIALOG_ALARM);
		alarmDlg->ShowWindow(SW_HIDE);
	}

	/*if(g_pFoceDlg == NULL){
		g_pFoceDlg = new CForceAlignDlg;
		g_pFoceDlg->Create(IDD_DIALOG_FORCE_ALIGN);
		g_pFoceDlg->ShowWindow(SW_HIDE);
	}*/

	Make_Child_Dialog();		// Á¶¸í, CCD ÃÊ±âÈ­
}

//! Added by LHW (2013/3/27)
//! ÀÌ¹° °Ë»ç¿¡¼­ ÀÌ¹ÌÁö È®´ë »óÅÂ¿¡¼­ÀÇ Pan ±â´É Ãß°¡¸¦ ¿ëÀÌÇÏ°Ô ÇÏ±â À§ÇØ ÇÔ¼ö·Î ºÐ¸®
int CAABonderDlg::Update_CCD_Display()
{
	double dReduceFactorX = 0.;
	double dReduceFactorY = 0.;

    dReduceFactorX = (double)SMALL_CCD_SIZE_X / gMIUDevice.nWidth; 
	dReduceFactorY = (double)SMALL_CCD_SIZE_Y / gMIUDevice.nHeight;

	//bool   bBox_CCD_Zoom = m_bBox_CCD_Zoom;
	bool   bCCD_Zoom     = m_bActing_CCD_Zoom;

	//CPoint ViewPos;
	CRect  rect_CCD_Zoom;

	if ( bCCD_Zoom == true )
	{
		m_csLock_CCD_Zoom.Lock();
		rect_CCD_Zoom = m_rect_CCD_Zoom;
		m_csLock_CCD_Zoom.Unlock();

		m_ViewPos.x = rect_CCD_Zoom.left;
		m_ViewPos.y = rect_CCD_Zoom.top;

		//! ÀÌ¹ÌÁö ½ÃÀÛ À§Ä¡ È®ÀÎ
		if ( m_ViewPos.x < 0 )
		{
			m_ViewPos.x = 0;
		}
		if ( m_ViewPos.x + SMALL_CCD_SIZE_X > gMIUDevice.nWidth )
		{
			m_ViewPos.x = gMIUDevice.nWidth - SMALL_CCD_SIZE_X;
		}
		if ( m_ViewPos.y < 0 )
		{
			m_ViewPos.y = 0;
		}
		if ( m_ViewPos.y + SMALL_CCD_SIZE_Y > gMIUDevice.nHeight )
		{
			m_ViewPos.y = gMIUDevice.nHeight - SMALL_CCD_SIZE_Y;
		}
	}
	else
	{
		if(g_iCCDCamView == 0)		// ÁÂ»ó
		{
			m_ViewPos.x = 0;
			m_ViewPos.y = 0;
		}
		else if(g_iCCDCamView == 1)	// ¿ì»ó
		{
			m_ViewPos.x = gMIUDevice.nWidth - SMALL_CCD_SIZE_X;
			m_ViewPos.y = 0;
		}
		else if(g_iCCDCamView == 2)		//ÁÂÇÏ
		{
			m_ViewPos.x = 0;
			m_ViewPos.y = gMIUDevice.nHeight - SMALL_CCD_SIZE_Y;
		}
		else if(g_iCCDCamView == 3)		//¿ìÇÏ
		{
			m_ViewPos.x = gMIUDevice.nWidth - SMALL_CCD_SIZE_X;
			m_ViewPos.y = gMIUDevice.nHeight - SMALL_CCD_SIZE_Y;
		}
		else if(g_iCCDCamView == 4)		// Áß½É
		{
			m_ViewPos.x = gMIUDevice.nWidth/2 - SMALL_CCD_SIZE_X /2;
			m_ViewPos.y = gMIUDevice.nHeight/2 - SMALL_CCD_SIZE_Y /2;
		}
	}			


	if(vision.m_iDispMode==1 && !Task.AutoFlag)
	{
		MimBinarize(vision.MilGrabImageChild[3], vision.MilGrabImageChild[3], M_GREATER_OR_EQUAL, vision.m_iThValue, M_NULL);

		MbufCopy(vision.MilGrabImageChild[3], vision.MilGrabImageChild[4]);
		MbufCopy(vision.MilGrabImageChild[3], vision.MilGrabImageChild[5]);

		if ( bCCD_Zoom == true )
		{
			m_bState_CCD_Zoom = true;
			MbufCopyColor2d(vision.MilProcImage[1], vision.MilSmallImage[1], M_ALL_BAND, m_ViewPos.x, m_ViewPos.y, M_ALL_BAND, 0, 0, SMALL_CCD_SIZE_X, SMALL_CCD_SIZE_Y);
		}
		else
		{
			if(g_iCCDCamView >= 0 && g_iCCDCamView <= 4)	// È®´ë ¿µ»ó
			{
				m_bState_CCD_Zoom = true;
				MbufCopyColor2d(vision.MilGrabImage[1], vision.MilSmallImage[1], M_ALL_BAND, m_ViewPos.x, m_ViewPos.y, M_ALL_BAND, 0, 0, SMALL_CCD_SIZE_X, SMALL_CCD_SIZE_Y);
			}
			else
			{
				m_bState_CCD_Zoom = false;
				MimResize(vision.MilGrabImage[1], vision.MilSmallImage[1], dReduceFactorX, dReduceFactorY, M_DEFAULT);
			}
		}
	}
	else if(vision.m_iDispMode==2 && !Task.AutoFlag)
	{
		MbufCopy(vision.MilGrabImage[1], vision.MilProcImage[1]);

		if ( bCCD_Zoom == true )
		{
			m_bState_CCD_Zoom = true;
			MbufCopyColor2d(vision.MilProcImage[1], vision.MilSmallImage[1], M_ALL_BAND, m_ViewPos.x, m_ViewPos.y, M_ALL_BAND, 0, 0, SMALL_CCD_SIZE_X, SMALL_CCD_SIZE_Y);
		}
		else
		{
			if(g_iCCDCamView >= 0 && g_iCCDCamView <= 4)	// È®´ë ¿µ»ó
			{
				m_bState_CCD_Zoom = true;
				MbufCopyColor2d(vision.MilProcImage[1], vision.MilSmallImage[1], M_ALL_BAND, m_ViewPos.x, m_ViewPos.y, M_ALL_BAND, 0, 0, SMALL_CCD_SIZE_X, SMALL_CCD_SIZE_Y);
			}
			else
			{
				m_bState_CCD_Zoom = false;
				MimResize(vision.MilProcImage[1], vision.MilSmallImage[1], dReduceFactorX, dReduceFactorY, M_DEFAULT);
			}
		}
	}
	else
	{
		if ( bCCD_Zoom == true )
		{
			m_bState_CCD_Zoom = true;
			MbufCopyColor2d(vision.MilProcImage[1], vision.MilSmallImage[1], M_ALL_BAND, m_ViewPos.x, m_ViewPos.y, M_ALL_BAND, 0, 0, SMALL_CCD_SIZE_X, SMALL_CCD_SIZE_Y);
		}
		else
		{
			if(g_iCCDCamView >= 0 && g_iCCDCamView <= 4)	// È®´ë ¿µ»ó
			{
				m_bState_CCD_Zoom = true;
				MbufCopyColor2d(vision.MilGrabImage[1], vision.MilSmallImage[1], M_ALL_BAND, m_ViewPos.x, m_ViewPos.y, M_ALL_BAND, 0, 0, SMALL_CCD_SIZE_X, SMALL_CCD_SIZE_Y);
			}
			else
			{
				m_bState_CCD_Zoom = false;
				MimResize(vision.MilGrabImage[1], vision.MilSmallImage[1], dReduceFactorX, dReduceFactorY, M_DEFAULT);
			}
		}
	}

	return 1;
}

void CAABonderDlg::OnBnClickedButtonExit()
{
	sLangChange.LoadStringA(IDS_STRING1408);	//Á¾·áÇÏ½Ã°Ú½À´Ï±î?
	if ( askMsg(sLangChange) == IDOK)
	{
		timeKillEvent(m_mmResult);


		bThreadMonitor = false;
		bThreadTaskLens = false;
		bThreadTaskPcb = false;
		bThreadCcmGrab = false;
		bThreadClock = false;
		bThreadGrab = false;
	
		g_ADOData.func_AA_DBDisConnect();

		//! Added by LHW (2013/4/21)
		bThreadServoAlarm = false;

		//! Added by LHW (2013/4/29)
		//! UV Lamp Front Panel¿¡¼­ Á¦¾îÇÏ´Â ¹æ½ÄÀ¸·Î º¯°æ
		//! ¾ÈÀüÀ» À§ÇØ ¼ÅÅÍ´Â ´Ý°í, Lamp´Â ²¨µÐ´Ù. 
		//UVCommand.UV_Shutter_Close();
		//Sleep(200);
		//UVCommand.UV_Lamp_Off();
		//Sleep(200);
		//UVCommand.Set_Control_Mode_Front_Panel();
		//Sleep(100);		
		////! UV Lamp RS-232C Åë½Å ÇØÁ¦
		//UVCommand.Close_Device();
		
		//! Added by LHW (2013/5/3)
		//! Á¶¸í ÄÁÆ®·Ñ·¯ RS-232C Åë½Å ÇØÁ¦
		LightControl.Close_Device();
		LightControlSecond.Close_Device();
		LightControlthird.Close_Device();
		motor.AmpDisableAll();

		int cnt1 = 0;
		int cnt2 = 0;
		int cnt3 = 0;
		int cnt4 = 0;
		int cnt5 = 0;

		cnt1 = 0;
		while(bThreadClockRun)
		{
			::WaitForSingleObject(pThread_Clock->m_hThread, 100);
			cnt1++;

			if(cnt1>10)
			{
				sLangChange.LoadStringA(IDS_STRING1432);	//Åë½Å Á¾·á ´ë±â ½Ã°£À» ÃÊ°ú ÇÏ¿´½À´Ï´Ù. °­Á¦ Á¾·á ÇÕ´Ï´Ù.
				delayMsg(sLangChange, 5000, M_COLOR_RED);
				Sleep(1000);
				break;
			}
		}

		cnt1 = 0;
		while(bThreadGrab)
		{
			::WaitForSingleObject(pThread_Grab->m_hThread, 100);
			cnt1++;

			if(cnt1>10)
			{
				sLangChange.LoadStringA(IDS_STRING1432);	//Åë½Å Á¾·á ´ë±â ½Ã°£À» ÃÊ°ú ÇÏ¿´½À´Ï´Ù. °­Á¦ Á¾·á ÇÕ´Ï´Ù.
				delayMsg(sLangChange, 5000, M_COLOR_RED);
				Sleep(1000);
				break;
			}
		}

		cnt1 = 0;
		while(bThreadTaskLensRun)
		{
			::WaitForSingleObject(pThread_TaskLens->m_hThread, 100);
			cnt1++;

			if(cnt1>10)
			{
				sLangChange.LoadStringA(IDS_STRING1432);	//Åë½Å Á¾·á ´ë±â ½Ã°£À» ÃÊ°ú ÇÏ¿´½À´Ï´Ù. °­Á¦ Á¾·á ÇÕ´Ï´Ù.
				delayMsg(sLangChange, 5000, M_COLOR_RED);
				Sleep(1000);
				break;
			}
		}

		cnt2 = 0;
		while (bThreadTaskPcbRun)
		{
			::WaitForSingleObject(pThread_TaskPcb->m_hThread, 100);
			cnt2++;

			if (cnt2>10)
			{
				sLangChange.LoadStringA(IDS_STRING1432);	//Åë½Å Á¾·á ´ë±â ½Ã°£À» ÃÊ°ú ÇÏ¿´½À´Ï´Ù. °­Á¦ Á¾·á ÇÕ´Ï´Ù.
				delayMsg(sLangChange, 5000, M_COLOR_RED);
				Sleep(1000);
				break;
			}
		}
		cnt2 = 0;
		while (bThreadOriginRun)
		{
			::WaitForSingleObject(pThread_TaskOrigin->m_hThread, 100);
			cnt2++;

			if (cnt2>10)
			{
				sLangChange.LoadStringA(IDS_STRING1432);	//Åë½Å Á¾·á ´ë±â ½Ã°£À» ÃÊ°ú ÇÏ¿´½À´Ï´Ù. °­Á¦ Á¾·á ÇÕ´Ï´Ù.
				delayMsg(sLangChange, 5000, M_COLOR_RED);
				Sleep(1000);
				break;
			}
		}

		//Thread_TaskOrigin
		cnt3 = 0;
		while(bThreadMonitorRun)
		{
			::WaitForSingleObject(pThread_Monitor->m_hThread, 1000);
			cnt3++;

			if(cnt3>10)
			{
				sLangChange.LoadStringA(IDS_STRING1432);	//Åë½Å Á¾·á ´ë±â ½Ã°£À» ÃÊ°ú ÇÏ¿´½À´Ï´Ù. °­Á¦ Á¾·á ÇÕ´Ï´Ù.
				delayMsg(sLangChange, 5000, M_COLOR_RED);
				Sleep(1000);
				break;
			}
		}

		//! Added by LHW (2013/4/21)
		cnt1 = 0;
		while(bThreadServoAlarmRun)
		{
			::WaitForSingleObject(pThread_ServoAlarm->m_hThread, 100);
			cnt1++;

			if(cnt1>10)
			{
				sLangChange.LoadStringA(IDS_STRING1432);	//Åë½Å Á¾·á ´ë±â ½Ã°£À» ÃÊ°ú ÇÏ¿´½À´Ï´Ù. °­Á¦ Á¾·á ÇÕ´Ï´Ù.
				delayMsg(sLangChange, 5000, M_COLOR_RED);
				Sleep(1000);
				break;
			}
		}

		cnt4 = 0;
		
		//! Modified by LHW (2013/2/25, 2013/3/13)	
		while(bThreadCcmGrabRun)
		{
			if ( pThread_CCM_Grab != NULL )
			{
				::WaitForSingleObject(pThread_CCM_Grab->m_hThread, 100);
			}
			if ( pThread_CCM_Mil_CvtColor_Ready != NULL )
			{
				::WaitForSingleObject(pThread_CCM_Mil_CvtColor_Ready->m_hThread, 100);
			}
			if ( pThread_CCM_Mil_CvtColor != NULL )
			{
				::WaitForSingleObject(pThread_CCM_Mil_CvtColor->m_hThread, 100);
			}			
			if ( pThread_CCM_CvtColor != NULL )
			{
				::WaitForSingleObject(pThread_CCM_CvtColor->m_hThread, 100);
			}
			if ( pThread_CCM_CvtMil != NULL )
			{
				::WaitForSingleObject(pThread_CCM_CvtMil->m_hThread, 100);
			}
			if ( pThread_CCM_Display != NULL )
			{
				::WaitForSingleObject(pThread_CCM_Display->m_hThread, 100);
			}
						
			cnt4++;

			if(cnt4>10)
			{
				sLangChange.LoadStringA(IDS_STRING1432);	//Åë½Å Á¾·á ´ë±â ½Ã°£À» ÃÊ°ú ÇÏ¿´½À´Ï´Ù. °­Á¦ Á¾·á ÇÕ´Ï´Ù.
				delayMsg(sLangChange, 5000, M_COLOR_RED);
				Sleep(1000);
				break;
			}
		}

		g_chkdateTH_flag = false;
		if(pThread_CheckDate != NULL)
		{
			::WaitForSingleObject(pThread_CheckDate->m_hThread, 3000);
			pThread_CheckDate = NULL;
		}
		if (MIU.m_pBoard != NULL)
		{
			if (MIU.m_pBoard->IsGrabStarted())
				MIU.Stop();
			MIU.Close();
		}
		
	

		if (sfrSpecDlg != NULL)
		{
			sfrSpecDlg->DestroyWindow();
			delete sfrSpecDlg;
			sfrSpecDlg = NULL;
		}
		if (chartSetDlg != NULL)
		{
			chartSetDlg->DestroyWindow();
			delete chartSetDlg;
			chartSetDlg = NULL;
		}
		
		if (modelDlg!=NULL)
		{
			modelDlg->DestroyWindow();
			delete modelDlg;
			modelDlg = NULL;
		}

		if (lensDlg != NULL)
		{
			lensDlg->DestroyWindow();
			delete lensDlg;
			lensDlg = NULL;
		}
		if (lensEdgeDlg != NULL)
		{
			lensEdgeDlg->DestroyWindow();
			delete lensEdgeDlg;
			lensEdgeDlg = NULL;
		}

		if (pcbDlg != NULL)
		{
			pcbDlg->DestroyWindow();
			delete pcbDlg;
			pcbDlg = NULL;
		}

		/*if (pcbInspDlg != NULL)
		{
			pcbInspDlg->DestroyWindow();
			delete pcbInspDlg;
			pcbInspDlg = NULL;
		}*/

		if (ccdDlg != NULL)
		{
			ccdDlg->DestroyWindow();
			delete ccdDlg;
			ccdDlg = NULL;
		}
		if (motorDlg != NULL)
		{
			motorDlg->DestroyWindow();
			delete motorDlg;
			motorDlg = NULL;
		}
		if (motorDlg2 != NULL)
		{
			motorDlg2->DestroyWindow();
			delete motorDlg2;
			motorDlg2 = NULL;
		}
		if (motorDlg3 != NULL)
		{
			motorDlg3->DestroyWindow();
			delete motorDlg3;
			motorDlg3 = NULL;
		}

		if (lightDlg != NULL)
		{
			lightDlg->DestroyWindow();
			delete lightDlg;
			lightDlg = NULL;
		}

		
// 		if (DataSet != NULL)
// 		{
// 			DataSet->DestroyWindow();
// 			delete DataSet;
// 			DataSet = NULL;
// 		}
		if (ioDlg != NULL)
		{
			ioDlg->DestroyWindow();
			delete ioDlg;
			ioDlg = NULL;
		}

		if (delayDlg != NULL)
		{
			delayDlg->DestroyWindow();
			delete delayDlg;
			delayDlg = NULL;
		}

		/*if ( TiltingManualdlg != NULL )
		{
			TiltingManualdlg->DestroyWindow();
			delete TiltingManualdlg;
			TiltingManualdlg = NULL;
		}*/

		if (autodispDlg != NULL)
		{
			autodispDlg->DestroyWindow();
			delete autodispDlg;
			autodispDlg = NULL;
		}

		if (alarmDlg != NULL)
		{
			alarmDlg->DestroyWindow();
			delete alarmDlg;
			alarmDlg = NULL;
		}

		Delete_Child_Dialog();

		#ifdef ON_LINE_VISION
		vision.closeVB();
		#endif

// 		if ( InformDlg != NULL )
// 		{
// 			InformDlg->DestroyWindow();
// 			delete InformDlg;
// 			InformDlg = NULL;
// 		}

		destoryStandardCursor();

		KillTimer(999);

		CDialogEx::OnOK();
	}
}


void CAABonderDlg::ctrlSubDlg(int iDlgNo)
{

	if ((iDlgNo == MOTOR_DLG && motorDlg2->IsWindowVisible()) ||
		(iDlgNo == MOTOR_DLG2 && motorDlg->IsWindowVisible()) ||
		(iDlgNo == MOTOR_DLG3 && motorDlg3->IsWindowVisible()))
	{

	}
	else
		password.m_bFlag = false;


	if (modelDlg->IsWindowVisible())
		modelDlg->ShowWindow(SW_HIDE);

	if (lensDlg->IsWindowVisible())
		lensDlg->ShowWindow(SW_HIDE);

	if (lensEdgeDlg->IsWindowVisible())
		lensEdgeDlg->ShowWindow(SW_HIDE);

	if (pcbDlg->IsWindowVisible())
		pcbDlg->ShowWindow(SW_HIDE);

	if (ccdDlg->IsWindowVisible())
	{
		ccdDlg->Hide_All_Child_Dialog();
		ccdDlg->ShowWindow(SW_HIDE);
	}

	if (motorDlg->IsWindowVisible())
		motorDlg->ShowWindow(SW_HIDE);

	if (motorDlg2->IsWindowVisible())
		motorDlg2->ShowWindow(SW_HIDE);

	if (motorDlg3->IsWindowVisible())
		motorDlg3->ShowWindow(SW_HIDE);

	if (lightDlg->IsWindowVisible())
		lightDlg->ShowWindow(SW_HIDE);


	if (ioDlg->IsWindowVisible())
		ioDlg->ShowWindow(SW_HIDE);

	if (alarmDlg->IsWindowVisible())
		alarmDlg->ShowWindow(SW_HIDE);

	//if (g_pFoceDlg->IsWindowVisible())
	//	g_pFoceDlg->ShowWindow(SW_HIDE);

	if (m_iOldDlgNo == iDlgNo)
		iDlgNo = MAIN_DLG;

	/*if (iDlgNo == IDD_DIALOG_LIGHT)
	{
	if (!m_bisLightBtn)
	{
	iDlgNo = MAIN_DLG;
	}
	}*/

	if (iDlgNo == LENS_DLG || iDlgNo == PCB_DLG)
	{
		if (!m_bisAlignBtn)
		{
			iDlgNo = MAIN_DLG;
		}
	}

	if (iDlgNo == MOTOR_DLG ||
		iDlgNo == MOTOR_DLG2 ||
		iDlgNo == MOTOR_DLG3 ||
		iDlgNo == MOTOR_INSPDLG)
	{
		if (!m_bisMotorBtn)
		{
			iDlgNo = MAIN_DLG;
		}
	}

	if (m_bisAlignBtn && iDlgNo != LENS_DLG && iDlgNo != PCB_DLG && LENS_EDGE_DLG != iDlgNo)
		m_bisAlignBtn = false;

	if (m_bisMotorBtn && iDlgNo != MOTOR_DLG && iDlgNo != MOTOR_DLG2 && iDlgNo != MOTOR_DLG3 && iDlgNo != MOTOR_INSPDLG)
		m_bisMotorBtn = false;

	/*if(iDlgNo==MAIN_DLG)
	{
	autodispDlg->ShowWindow(true);
	}else
	{
	autodispDlg->ShowWindow(false);
	}*/

	switch (iDlgNo)
	{
	case MAIN_DLG:
		setCamDisplay(m_iCurCamNo, 1);//1); 201012

		break;
	case MODEL_DLG:
		setCamDisplay(1, 0);//1);
		modelDlg->ShowWindow(SW_SHOW);

		break;
	case LENS_DLG:
		m_iCurCamNo = 1;
		setCamDisplay(1, 0);//PCB//setCamDisplay(0, 1);//PCB
		lensDlg->ShowWindow(SW_SHOW);
		break;
	case LENS_EDGE_DLG:
		m_iCurCamNo = 1;
		setCamDisplay(1, 0);
		lensEdgeDlg->ShowWindow(SW_SHOW);
		break;
	case PCB_DLG:
		m_iCurCamNo = 1;
		setCamDisplay(1, 0);//setCamDisplay(0, 1);//PCB
		pcbDlg->ShowWindow(SW_SHOW);
		break;

	case CCD_DLG:
		m_iCurCamNo = 3;
		setCamDisplay(3, 0);//setCamDisplay(3, 1);
		ccdDlg->ShowWindow(SW_SHOW);

		break;
	case MOTOR_DLG:
		setCamDisplay(m_iCurCamNo, 0);// 1);
		motorDlg->ShowWindow(SW_SHOW);

		break;
	case MOTOR_DLG2:
		setCamDisplay(m_iCurCamNo, 0);// 1);
		motorDlg2->ShowWindow(SW_SHOW);

		break;
	case MOTOR_DLG3:
		setCamDisplay(m_iCurCamNo, 0);// 1);
		motorDlg3->ShowWindow(SW_SHOW);
		break;

	case IDD_DIALOG_LIGHT:
		setCamDisplay(m_iCurCamNo, 0);//1);
		lightDlg->ShowWindow(SW_SHOW);

		break;
	case IO_DLG:
		setCamDisplay(m_iCurCamNo, 0);//1);
		ioDlg->ShowWindow(SW_SHOW);

		break;

	case ALARM_DLG:
		setCamDisplay(m_iCurCamNo, 0);//1);

		alarmDlg->m_iAlarmKind = e_AlarmHistory;
		alarmDlg->ShowWindow(SW_SHOW);

		break;
	}

	m_iOldDlgNo = iDlgNo;
}



void CAABonderDlg::OnBnClickedButtonMain()
{
	ctrlSubDlg(MAIN_DLG);
	//setCamDisplay(m_iCurCamNo, 0);
	changeMainBtnColor(MAIN_DLG);
	for(int i=0; i<MARK_CNT; i++)
	{
		//SetDigReference(i);
	}
}


void CAABonderDlg::OnBnClickedButtonLens()
{
	if (m_bisAlignBtn)	m_bisAlignBtn = false;
	else				m_bisAlignBtn = true;

	if (m_bIsLensMode == 0)		ctrlSubDlg(LENS_DLG);
	else if(m_bIsLensMode == 1)	ctrlSubDlg(PCB_DLG);
	else						ctrlSubDlg(LENS_EDGE_DLG);

	changeMainBtnColor(PCB_DLG);			
}


void CAABonderDlg::OnBnClickedButtonCcd()
{
	ctrlSubDlg(CCD_DLG);
	changeMainBtnColor(CCD_DLG);
}


void CAABonderDlg::OnBnClickedButtonMotor()
{ 
	if (m_bisMotorBtn)	m_bisMotorBtn = false;
	else				m_bisMotorBtn = true;

	if( m_bIsMotorMode == 0 )		ctrlSubDlg(MOTOR_DLG);
	else if( m_bIsMotorMode == 1 )	ctrlSubDlg(MOTOR_DLG2);
	else							ctrlSubDlg(MOTOR_DLG3);

	changeMainBtnColor(MOTOR_DLG2);
}


void CAABonderDlg::OnBnClickedButtonIo()
{
	ctrlSubDlg(IO_DLG);
	changeMainBtnColor(IO_DLG);
}


void CAABonderDlg::OnBnClickedButtonModel()
{
	ctrlSubDlg(MODEL_DLG);
	changeMainBtnColor(MODEL_DLG);
}



void CAABonderDlg::dispStepOnButton(int iCtrlNo, int iState)
{

}

//! ÇöÀç È­¸é¿¡ Ç¥½ÃµÇ¾î ÀÖ´Â »ç°¢ ¿µ¿ªÀÇ »ö»óÀ» º¯°æ
void CAABonderDlg::Change_Mode_Mouse_Box(int iMode_Mouse_Box)
{
	//! ÇöÀç ¼±ÅÃµÈ °ª¿¡ µû¶ó »ç°¢ ¿µ¿ª Ç¥½Ã¸¦ ÇÏ´Â »ö»óÀ» °áÁ¤ÇÑ´Ù. 
	COLORREF clrBoxArea = GetColor_Mouse_Box(iMode_Mouse_Box);

	vision.ChangeColor_Box(m_iCurCamNo, clrBoxArea);
	vision.drawOverlay(m_iCurCamNo);
}

//! ÀÔ·ÂµÈ °ª¿¡ ¸Â´Â »ç°¢ ¿µ¿ªÀÇ »ö»óÀ» ¹ÝÈ¯
COLORREF CAABonderDlg::GetColor_Mouse_Box(int iMode_Mouse_Box)
{
	COLORREF clrRet = RGB(255, 0, 0);

	switch(iMode_Mouse_Box)
	{
	case 0:
		{
//			clrRet = RGB(0, 255, 0);
			clrRet = RGB(255, 0, 0);
		}
		break;
	case 1:
		{
			clrRet = RGB(0, 0, 255);
		}
		break;
	case 2:
		{
			clrRet = RGB(255, 255, 0);
		}
		break;
	case 3:
		{
			clrRet = RGB(0, 255, 255);
		}
		break;
	case 4:
		{
			clrRet = RGB(255, 128, 0);
		}
		break;		
	}

	return clrRet;
}


void CAABonderDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bDrawFlag			= false;
	m_bBoxMoveFlag		= false;
	m_bBoxMoveFlag_CCD	= false;
	m_bDrawMeasureLine	= false;
	//! <-------------------------------------
	//! Added by LHW (2013/3/27)
	if ( m_bPan_CCD_Zoom == true )
	{
		::SetCursor(m_hCursor_Standard);		
	}	
	if ( m_bBox_CCD_Zoom == true )
	{
		if ( m_bBox_Acting_CCD_Zoom == true && m_bState_CCD_Zoom == false )
		{
			//! »ç°¢ ¿µ¿ªÀÇ ½ÃÀÛ À§Ä¡¿¡¼­ È®´ëÇÑ´Ù.

			m_csLock_CCD_Zoom.Lock();
			m_rect_CCD_Zoom.left = m_rBox.left;
			m_rect_CCD_Zoom.top  = m_rBox.top;
			if ( m_rect_CCD_Zoom.left < 0 )
			{
				m_rect_CCD_Zoom.left = 0;
			}
			if ( m_rect_CCD_Zoom.left + SMALL_CCD_SIZE_X > gMIUDevice.nWidth )
			{
				m_rect_CCD_Zoom.left = gMIUDevice.nWidth - SMALL_CCD_SIZE_X;
			}
			if ( m_rect_CCD_Zoom.top < 0 )
			{
				m_rect_CCD_Zoom.top = 0;
			}
			if ( m_rect_CCD_Zoom.top + SMALL_CCD_SIZE_Y > gMIUDevice.nHeight )
			{
				m_rect_CCD_Zoom.top = gMIUDevice.nHeight - SMALL_CCD_SIZE_Y;
			}
			m_csLock_CCD_Zoom.Unlock();

			m_bActing_CCD_Zoom = true;
			m_bPan_CCD_Zoom = true;

			if ( gMIUDevice.CurrentState < 3 )
			{
				//! CCD OffLine »óÅÂÀÏ ¶§
				Update_CCD_Display();
			}
		}

		m_rBox.SetRectEmpty();
		vision.clearOverlay(m_iCurCamNo);
		vision.drawOverlay(m_iCurCamNo);
	}
	m_bActing_Pan_CCD_Zoom = false;
	m_bBox_Acting_CCD_Zoom = false;
	//! <-------------------------------------

	if (m_iCurCamNo==CAM1)
	{
		lensDlg->m_rMarkBox = lensDlg->m_rcFixedBox = m_rBox;		
		pcbDlg->m_rMarkBox = pcbDlg->m_rcFixedBox =m_rBox;

		lensEdgeDlg->m_rMarkBox = lensEdgeDlg->m_rcFixedBox = m_rBox;

	}
	else if (m_iCurCamNo==CAM2)
	{
		lensEdgeDlg->m_rMarkBox = lensEdgeDlg->m_rcFixedBox =m_rBox;
	}

	if (point.x>m_rectCamDispPos1.left	&&
		point.x<m_rectCamDispPos1.right &&
		point.y>m_rectCamDispPos1.top	&&
		point.y<m_rectCamDispPos1.bottom)
	{
		if (motorDlg->m_bCalcResol) 
		{
			motorDlg->registPatMark(m_iCurCamNo, m_rBox);
			motorDlg->m_bFindTop = motorDlg->m_bFindBottom = motorDlg->m_bFindLeft = motorDlg->m_bFindRight = false;
			//		vision.clearOverlay(m_iCurCamNo);
			motorDlg->drawResolBackGround();
			vision.drawOverlay(m_iCurCamNo);

			m_rBox.left		= 0;
			m_rBox.top		= 0;
			m_rBox.right	= 0;
			m_rBox.bottom	= 0;
		}else if (motorDlg2->m_bCalcResol)
		{
			motorDlg2->registPatMark(m_iCurCamNo, m_rBox);
			motorDlg2->m_bFindTop = motorDlg2->m_bFindBottom = motorDlg2->m_bFindLeft = motorDlg2->m_bFindRight = false;
			//		vision.clearOverlay(m_iCurCamNo);
			motorDlg2->drawResolBackGround();
			vision.drawOverlay(m_iCurCamNo);

			m_rBox.left		= 0;
			m_rBox.top		= 0;
			m_rBox.right	= 0;
			m_rBox.bottom	= 0;
		}
		else if (motorDlg3->m_bCalcResol)
		{
			motorDlg3->registPatMark(m_iCurCamNo, m_rBox);
			motorDlg3->m_bFindTop = motorDlg3->m_bFindBottom = motorDlg3->m_bFindLeft = motorDlg3->m_bFindRight = false;
			//		vision.clearOverlay(m_iCurCamNo);
			motorDlg3->drawResolBackGround();
			vision.drawOverlay(m_iCurCamNo);

			m_rBox.left		= 0;
			m_rBox.top		= 0;
			m_rBox.right	= 0;
			m_rBox.bottom	= 0;
		}
		
	}

	ReleaseCapture();

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CAABonderDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if((nFlags & 0x01) == false)
	{
		m_bDrawFlag			= false;
		m_bBoxMoveFlag		= false;
		m_bBoxMoveFlag_CCD	= false;
		m_bDrawMeasureLine	= false;
	}

	double dExpandFactorX, dExpandFactorY;
	if (m_bBoxMoveFlag_CCD)
	{
		if (point.x<m_rectCcdDispPos.left ||
			point.x>m_rectCcdDispPos.right ||
			point.y<m_rectCcdDispPos.top ||
			point.y>m_rectCcdDispPos.bottom && !m_bMeasureDist)
		{
			m_iMoveType = checkMousePos(point, m_rBox);
		}
	}
	else
	{
		if (point.x<m_rectCamDispPos1.left ||
			point.x>m_rectCamDispPos1.right ||
			point.y<m_rectCamDispPos1.top ||
			point.y>m_rectCamDispPos1.bottom && !m_bMeasureDist)
		{
			m_iMoveType = checkMousePos(point, m_rBox);
		}
	}

	/*if (point.x<m_rectCamDispPos1.left	||
		point.x>m_rectCamDispPos1.right ||
		point.y<m_rectCamDispPos1.top   ||
		point.y>m_rectCamDispPos1.bottom  && !m_bMeasureDist )
	{
		m_iMoveType = checkMousePos(point, m_rBox);	
	}*/
	
	//! if ( ccdDlg->m_pDefectDlg->IsWindowVisible() )	

	if ( m_bMeasureDist && !m_bDrawMeasureLine )
	{
		m_iType_MeasureLine = changeCursor_MeasureMode(point);
	}
	else
		changeCursor(point, m_rBox);

	if (m_bDrawFlag)
	{
		if (point.x>m_rectCamDispPos1.left	&&
			point.x<m_rectCamDispPos1.right &&
			point.y>m_rectCamDispPos1.top	&&
			point.y<m_rectCamDispPos1.bottom)
		{
			if (m_iCurCamNo<3)
			{
				dExpandFactorX = CAM_EXPAND_FACTOR_X;
				dExpandFactorY = CAM_EXPAND_FACTOR_Y;
			}
			else
			{
				dExpandFactorX = (double)gMIUDevice.nWidth/SMALL_CCD_SIZE_X;
				dExpandFactorY = (double)gMIUDevice.nHeight/SMALL_CCD_SIZE_Y;
			}

			point.x -= m_rectCamDispPos1.left;
			point.y -= m_rectCamDispPos1.top;

			if (m_bBoxMoveFlag)
			{
				int iMoveX = (int)(point.x * dExpandFactorX + 0.5) -(int)(m_ClickP.x * dExpandFactorX + 0.5);
				int iMoveY = (int)(point.y * dExpandFactorY + 0.5) - (int)(m_ClickP.y * dExpandFactorY + 0.5);

				/* ÀÌµ¿ */
				if (m_iMoveType==MOVE_ALL)
				{
					m_rBox.left		+= iMoveX;
					m_rBox.top		+= iMoveY;
					m_rBox.right	+= iMoveX;
					m_rBox.bottom	+= iMoveY;
				}
				/* ÁÂ Å©±â */
				else if (m_iMoveType==MOVE_WIDTH_LEFT)
				{
					m_rBox.left		+= iMoveX;
				}
				/* ¿ì Å©±â */
				else if (m_iMoveType==MOVE_WIDTH_RIGHT)
				{
					m_rBox.right	+= iMoveX;
				}
				/* »ó Å©±â */
				else if (m_iMoveType==MOVE_HEIGHT_TOP)
				{
					m_rBox.top		+= iMoveY;
				}
				/* ÇÏ Å©±â */
				else if (m_iMoveType==MOVE_HEIGHT_BOTTOM)
				{
					m_rBox.bottom	+= iMoveY;
				}
				/* ÁÂ»ó Å©±â */
				else if (m_iMoveType==MOVE_NW)
				{
					m_rBox.left		+= iMoveX;
					m_rBox.top		+= iMoveY;
				}
				/* ¿ì»ó Å©±â */
				else if (m_iMoveType==MOVE_NE)
				{
					m_rBox.top		+= iMoveY;
					m_rBox.right	+= iMoveX;
				}
				/* ÁÂÇÏ Å©±â */
				else if (m_iMoveType==MOVE_SW)
				{
					m_rBox.left		+= iMoveX;
					m_rBox.bottom	+= iMoveY;
				}
				/* ¿ìÇÏ Å©±â */
				else if (m_iMoveType==MOVE_SE)
				{
					m_rBox.right	+= iMoveX;
					m_rBox.bottom	+= iMoveY;
				}

				m_ClickP = point;
			}
			else
			{
				m_rBox.left		= (int)(m_ClickP.x * dExpandFactorX + 0.5);
				m_rBox.top		= (int)(m_ClickP.y * dExpandFactorY + 0.5);
				m_rBox.right	= (int)(point.x	* dExpandFactorX + 0.5);
				m_rBox.bottom	= (int)(point.y * dExpandFactorY + 0.5);
			}

			if (m_rBox.left>m_rBox.right)
				SWAP(m_rBox.left, m_rBox.right);
			if (m_rBox.top>m_rBox.bottom)
				SWAP(m_rBox.top, m_rBox.bottom);

			m_rcFixedBox = m_rBox;

			if (m_rBox.left<1)
			{
				m_rBox.left		= 1;
				m_rBox.right	= 1 + m_rcFixedBox.Width();
			}
			if (m_rBox.top<1)
			{
				m_rBox.top		= 1;
				m_rBox.bottom	= 1 + m_rcFixedBox.Height();
			}
			if (m_iCurCamNo<3)
			{
				if (m_rBox.right>CAM_SIZE_X-1)
				{
					m_rBox.right	= CAM_SIZE_X - 1;
					m_rBox.left		= m_rBox.right - m_rcFixedBox.Width();
				}
				if (m_rBox.bottom>CAM_SIZE_Y-1)
				{
					m_rBox.bottom	= CAM_SIZE_Y -1;
					m_rBox.top		= m_rBox.bottom - m_rcFixedBox.Height();
				}
			}
			else
			{
				if (m_rBox.right>gMIUDevice.nWidth-1)
				{
					m_rBox.right	= gMIUDevice.nWidth - 1;
					m_rBox.left		= m_rBox.right - m_rcFixedBox.Width();
				}
				if (m_rBox.bottom>gMIUDevice.nHeight-1)
				{
					m_rBox.bottom	= gMIUDevice.nHeight -1;
					m_rBox.top		= m_rBox.bottom - m_rcFixedBox.Height();
				}
			}

			if ( m_iCurCamNo == 3 )
			{
				//! Added by LHW (2013/3/27)

				//! ÇöÀç ¼±ÅÃµÈ °ª¿¡ µû¶ó »ç°¢ ¿µ¿ª Ç¥½Ã¸¦ ÇÏ´Â »ö»óÀ» °áÁ¤ÇÑ´Ù. 
				COLORREF clrBoxArea = GetColor_Mouse_Box(m_iMode_Mouse_Box);

				//! »ç°¢ ¿µ¿ªÀ» ±×¸°´Ù. 
				vision.clearOverlay(m_iCurCamNo);
				vision.boxlist[m_iCurCamNo].addList(m_rBox, PS_SOLID, clrBoxArea);

				vision.drawOverlay(m_iCurCamNo);
			}
			else
			{
				//! <----------------------------------------------------------------------------
				//! Modify by LHW (2013/2/5)
				//vision.clearOverlay(m_iCurCamNo);
				//vision.boxlist[m_iCurCamNo].addList(m_rBox, PS_DOT, M_COLOR_GREEN);
				//vision.drawOverlay(m_iCurCamNo);

				//! ÇöÀç ¼±ÅÃµÈ °ª¿¡ µû¶ó »ç°¢ ¿µ¿ª Ç¥½Ã¸¦ ÇÏ´Â »ö»óÀ» °áÁ¤ÇÑ´Ù. 
				COLORREF clrBoxArea = GetColor_Mouse_Box(m_iMode_Mouse_Box);

				//! »ç°¢ ¿µ¿ªÀ» ±×¸°´Ù. 
				vision.clearOverlay(m_iCurCamNo);
				vision.boxlist[m_iCurCamNo].addList(m_rBox, PS_SOLID, clrBoxArea);

				if (m_iCurCamNo==3)	ccdDlg->m_pSFRDlg->drawROI();

				vision.drawOverlay(m_iCurCamNo);
				//! <---------------------------------------------------------------------------
			}

			GetDlgItem(IDC_DISP_LENS+m_iCurCamNo)->UpdateWindow();
		}

		SetCapture();
	}
	else if (m_bBoxMoveFlag_CCD)
	{
		if (m_iNo_SFR!=-1)
		{
			dExpandFactorX = (double)gMIUDevice.nWidth/SMALL_CCD_SIZE_X;
			dExpandFactorY = (double)gMIUDevice.nHeight/SMALL_CCD_SIZE_Y;

			//point.x -= m_rectCamDispPos1.left;
			//point.y -= m_rectCamDispPos1.top;
			point.x -= m_rectCcdDispPos.left;//m_rectCamDispPos1.left;
			point.y -= m_rectCcdDispPos.top;//m_rectCamDispPos1.top;

			int iMoveX = (int)(point.x * dExpandFactorX + 0.5) -(int)(m_ClickP.x * dExpandFactorX + 0.5);
			int iMoveY = (int)(point.y * dExpandFactorY + 0.5) - (int)(m_ClickP.y * dExpandFactorY + 0.5);

			/* ÀÌµ¿ */
			if (m_iMoveType==MOVE_ALL)
			{
				m_rBox.left		+= iMoveX;
				m_rBox.top		+= iMoveY;
				m_rBox.right	+= iMoveX;
				m_rBox.bottom	+= iMoveY;
			}
			/* ÁÂ Å©±â */
			else if (m_iMoveType==MOVE_WIDTH_LEFT)
			{
				m_rBox.left		+= iMoveX;
			}
			/* ¿ì Å©±â */
			else if (m_iMoveType==MOVE_WIDTH_RIGHT)
			{
				m_rBox.right	+= iMoveX;
			}
			/* »ó Å©±â */
			else if (m_iMoveType==MOVE_HEIGHT_TOP)
			{
				m_rBox.top		+= iMoveY;
			}
			/* ÇÏ Å©±â */
			else if (m_iMoveType==MOVE_HEIGHT_BOTTOM)
			{
				m_rBox.bottom	+= iMoveY;
			}
			/* ÁÂ»ó Å©±â */
			else if (m_iMoveType==MOVE_NW)
			{
				m_rBox.left		+= iMoveX;
				m_rBox.top		+= iMoveY;
			}
			/* ¿ì»ó Å©±â */
			else if (m_iMoveType==MOVE_NE)
			{
				m_rBox.top		+= iMoveY;
				m_rBox.right	+= iMoveX;
			}
			/* ÁÂÇÏ Å©±â */
			else if (m_iMoveType==MOVE_SW)
			{
				m_rBox.left		+= iMoveX;
				m_rBox.bottom	+= iMoveY;
			}
			/* ¿ìÇÏ Å©±â */
			else if (m_iMoveType==MOVE_SE)
			{
				m_rBox.right	+= iMoveX;
				m_rBox.bottom	+= iMoveY;
			}

			m_ClickP = point;

			if (m_rBox.left>m_rBox.right)
				SWAP(m_rBox.left, m_rBox.right);
			if (m_rBox.top>m_rBox.bottom)
				SWAP(m_rBox.top, m_rBox.bottom);

			m_rcFixedBox = m_rBox;

			if (m_rBox.left<1)
			{
				m_rBox.left		= 1;
				m_rBox.right	= 1 + m_rcFixedBox.Width();
			}
			if (m_rBox.top<1)
			{
				m_rBox.top		= 1;
				m_rBox.bottom	= 1 + m_rcFixedBox.Height();
			}

			if (m_rBox.right>gMIUDevice.nWidth-1)
			{
				m_rBox.right	= gMIUDevice.nWidth - 1;
				m_rBox.left		= m_rBox.right - m_rcFixedBox.Width();
			}
			if (m_rBox.bottom>gMIUDevice.nHeight-1)
			{
				m_rBox.bottom	= gMIUDevice.nHeight -1;
				m_rBox.top		= m_rBox.bottom - m_rcFixedBox.Height();
			}

			if (m_iNo_SFR < model.mGlobalChartCount)	//if(m_iNo_SFR<LAST_MARK_CNT)					// »ç°¢ ¸¶Å© À§Ä¡..
			{
				int iTemp;
				if(m_rBox.right < m_rBox.left)
				{
					iTemp=m_rBox.right;
					m_rBox.right=m_rBox.left;
					m_rBox.left=iTemp;
				}
				if(m_rBox.bottom < m_rBox.top)
				{
					iTemp=m_rBox.bottom;
					m_rBox.bottom=m_rBox.top;
					m_rBox.top=iTemp;
				}				
				
				ccdDlg->m_pSFRDlg->m_iOffsetX_SFR[m_iNo_SFR]	= m_rBox.left;
				ccdDlg->m_pSFRDlg->m_iOffsetY_SFR[m_iNo_SFR]	= m_rBox.top;

				if (m_iMoveType!=MOVE_ALL)
				{
					ccdDlg->m_pSFRDlg->m_iSizeX_SFR[m_iNo_SFR]	= m_rBox.right - m_rBox.left;
					ccdDlg->m_pSFRDlg->m_iSizeY_SFR[m_iNo_SFR]	= m_rBox.bottom - m_rBox.top;
				}
				else
				{
					ccdDlg->m_pSFRDlg->m_iSizeX_SFR[m_iNo_SFR]	= m_rBox.right - m_rBox.left;
					ccdDlg->m_pSFRDlg->m_iSizeY_SFR[m_iNo_SFR]	= m_rBox.bottom - m_rBox.top;
				}
			}
			else if (m_iNo_SFR < model.mGlobalChartCount + 4)//else if(m_iNo_SFR<LAST_MARK_CNT+4)			// ¿øÇü ¸¶Å© °Ë»ö À§Ä¡..
			{
				ccdDlg->m_pSFRDlg->m_rcRoiBox[m_iNo_SFR - model.mGlobalChartCount] = m_rBox;
				//ccdDlg->m_pSFRDlg->m_rcRoiBox[m_iNo_SFR - LAST_MARK_CNT] = m_rBox;
			}


			COLORREF clrBoxArea = GetColor_Mouse_Box(m_iMode_Mouse_Box);

			vision.clearOverlay(CCD);
			ccdDlg->m_pSFRDlg->drawRectSFR(m_iNo_SFR);
			vision.drawOverlay(CCD);

			GetDlgItem(IDC_DISP_LENS+m_iCurCamNo)->UpdateWindow();
		}

		SetCapture();
	}
	else if ( point.x>m_rectCamDispPos1.left	&&
			  point.x<m_rectCamDispPos1.right	&&
			  point.y>m_rectCamDispPos1.top		&&
			  point.y<m_rectCamDispPos1.bottom	&& m_bDrawMeasureLine == true )
	{
		if (m_iCurCamNo<3)
		{
			dExpandFactorX = CAM_EXPAND_FACTOR_X;
			dExpandFactorY = CAM_EXPAND_FACTOR_Y;
		}
		else
		{
			dExpandFactorX = (double)gMIUDevice.nWidth/SMALL_CCD_SIZE_X;
			dExpandFactorY = (double)gMIUDevice.nHeight/SMALL_CCD_SIZE_Y;
		}

		point.x -= m_rectCamDispPos1.left;
		point.y -= m_rectCamDispPos1.top;

		int iMoveX = (int)(point.x * dExpandFactorX + 0.5) - (int)(m_ClickP.x * dExpandFactorX + 0.5);
		int iMoveY = (int)(point.y * dExpandFactorY + 0.5) - (int)(m_ClickP.y * dExpandFactorY + 0.5);

		m_ClickP = point;

		/* Left Line */
		if ( m_iType_MeasureLine == 1 )
		{
			m_iLine_Left += iMoveX;
		}
		/* Top Line */
		else if ( m_iType_MeasureLine == 2 )
		{
			m_iLine_Top += iMoveY;
		}
		/* Right Line */
		else if ( m_iType_MeasureLine == 3 )
		{
			m_iLine_Right += iMoveX;
		}
		/* Bottom Line */
		else if ( m_iType_MeasureLine == 4 )
		{
			m_iLine_Bottom += iMoveY;
		}

		drawLine_MeasureDist(1);

		SetCapture();
	}


	CDialogEx::OnMouseMove(nFlags, point);
}


void CAABonderDlg::OnBnClickedButtonLensSupply()
{
}



void CAABonderDlg::OnBnClickedButtonPcbSupply()
{
}


void CAABonderDlg::OnBnClickedButtonPcbOsCheck()
{
}


void CAABonderDlg::OnBnClickedButtonCcdAlign()
{
	/* ÀÌµ¿ÁßÀÌ¸é Return */
	if(g_bMovingflag)
		return;
	g_bMovingflag =true;

//	Task.LensTask = 40000;
	Task.LensTask = 0;
	Task.PCBTask	= 60000;
	Task.m_iStart_Step_PCB = 60000;//10000;
	Task.m_iEnd_Step_PCB = 110000;// 70000;

	pThread_TaskPcb = ::AfxBeginThread(Thread_TaskPcb, this);
	//pThread_TaskLens = ::AfxBeginThread(Thread_TaskLens, this);
}


void CAABonderDlg::OnBnClickedButtonCcdInsp()
{
	/* ÀÌµ¿ÁßÀÌ¸é Return */
	if(g_bMovingflag)
		return;
	g_bMovingflag =true;

	Task.m_iStart_Step_PCB	= 60000;
	Task.m_iEnd_Step_PCB	= 110000;

	pThread_TaskPcb = ::AfxBeginThread(Thread_TaskPcb, this);
	//pThread_TaskLens = ::AfxBeginThread(Thread_TaskLens, this);
}


void CAABonderDlg::putListLog(CString strLog)
{
	if (!m_listLog)
	{
		return;
	}
	SYSTEMTIME stSysTime;
	CString sListLog;
	int nCount;

	GetLocalTime(&stSysTime);

	sListLog.Format(_T("[%02d:%02d:%02d.%03d] %s"), stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond, stSysTime.wMilliseconds, (TCHAR*)(LPCTSTR)strLog);

	nCount = m_listLog.GetCount();
	if (nCount > 1000)
	{
		m_listLog.DeleteString(0);
	}
	nCount = m_listLog.AddString(sListLog);
	m_listLog.SetCurSel(nCount);
	LogSave(sListLog);

	//
	/*if (m_listLog.GetCount()>10000)
		m_listLog.ResetContent();

	m_listLog.InsertString(m_listLog.GetCount(), strLog);
	m_listLog.SetCurSel(m_listLog.GetCount()-1);

	LogSave(strLog);*/
}


void CAABonderDlg::loadStandardCursor()
{
	m_hCursor_Standard	= NULL;
	m_hCursor_Width		= NULL;
	m_hCursor_Height	= NULL;
	m_hCursor_Move		= NULL;
	m_hCursor_NWSE		= NULL;
	m_hCursor_NESW		= NULL;

	m_hCursor_Standard	= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCursor_Width		= AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
	m_hCursor_Height	= AfxGetApp()->LoadStandardCursor(IDC_SIZENS);
	m_hCursor_Move		= AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
	m_hCursor_NWSE		= AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE);
	m_hCursor_NESW		= AfxGetApp()->LoadStandardCursor(IDC_SIZENESW);
}

void CAABonderDlg::destoryStandardCursor()
{
	if (m_hCursor_Standard != NULL)
		::DestroyCursor(m_hCursor_Standard);

	if (m_hCursor_Width != NULL)
		::DestroyCursor(m_hCursor_Width);

	if (m_hCursor_Height != NULL)
		::DestroyCursor(m_hCursor_Height);

	if (m_hCursor_Move != NULL)
		::DestroyCursor(m_hCursor_Move);

	if (m_hCursor_NWSE != NULL)
		::DestroyCursor(m_hCursor_NWSE);

	if (m_hCursor_NESW != NULL)
		::DestroyCursor(m_hCursor_NESW);
}

void CAABonderDlg::changeCursor(CPoint p, CRect rcTemp)
{
	double dExpandFactorX;
	double dExpandFactorY;
	int	iGap;

	if (m_iCurCamNo<3)
	{
		dExpandFactorX = CAM_EXPAND_FACTOR_X;
		dExpandFactorY = CAM_EXPAND_FACTOR_Y;
		iGap = 20;
	}
	else
	{
		dExpandFactorX = (double)gMIUDevice.nWidth/SMALL_CCD_SIZE_X;
		dExpandFactorY = (double)gMIUDevice.nHeight/SMALL_CCD_SIZE_Y;
		
		// 20140905 Overlay Box À§Ä¡ ÀÌµ¿ ½Ã Box ¼±¿¡¼­ ¾ó¸¶³ª °¡±îÀÌ ÀÖ¾î¾ß ¸¶¿ì½º Ä¿¼­ ¸ð¾ç º¯°æ °Å¸® È®ÀÎ ¿ë..
		//iGap = 50;
		iGap = int(dExpandFactorX * 5);
	}

	CPoint point = p;

	int iRtn = -1;

	point.x -= m_rectCamDispPos1.left;
	point.y -= m_rectCamDispPos1.top;

	p.x = (int)(point.x * dExpandFactorX + 0.5);
	p.y = (int)(point.y * dExpandFactorY + 0.5);

	/* ¹Ú½º ÀÌµ¿ */
	if (p.x>rcTemp.left+iGap	&&
		p.x<rcTemp.right-iGap	&&
		p.y>rcTemp.top+iGap		&&
		p.y<rcTemp.bottom-iGap)
	{
		::SetCursor(m_hCursor_Move);
	}
	/* ÁÂ Å©±â */
	else if (p.y>rcTemp.top+iGap && p.y<rcTemp.bottom-iGap && p.x>rcTemp.left-iGap && p.x<rcTemp.left+iGap)
	{
		::SetCursor(m_hCursor_Width);
	}
	/* ¿ì Å©±â */
	else if (p.y>rcTemp.top+iGap && p.y<rcTemp.bottom-iGap && p.x>rcTemp.right-iGap && p.x<rcTemp.right+iGap)
	{
		::SetCursor(m_hCursor_Width);
	}
	/* »ó Å©±â */
	else if (p.x>rcTemp.left+iGap && p.x<rcTemp.right-iGap && p.y>rcTemp.top-iGap && p.y<rcTemp.top+iGap)
	{
		::SetCursor(m_hCursor_Height);
	}
	/* ÇÏ Å©±â */
	else if (p.x>rcTemp.left+iGap && p.x<rcTemp.right-iGap && p.y>rcTemp.bottom-iGap && p.y<rcTemp.bottom+iGap)
	{
		::SetCursor(m_hCursor_Height);
	}
	/* ÁÂ»ó Å©±â */
	else if (p.x>rcTemp.left-iGap && p.x<rcTemp.left+iGap && p.y>rcTemp.top-iGap && p.y<rcTemp.top+iGap)
	{
		::SetCursor(m_hCursor_NWSE);
	}
	/* ¿ì»ó Å©±â */
	else if (p.x>rcTemp.right-iGap && p.x<rcTemp.right+iGap && p.y>rcTemp.top-iGap && p.y<rcTemp.top+iGap)
	{
		::SetCursor(m_hCursor_NESW);
	}
	/* ÁÂÇÏ Å©±â */
	else if (p.x>rcTemp.left-iGap && p.x<rcTemp.left+iGap && p.y>rcTemp.bottom-iGap && p.y<rcTemp.bottom+iGap)
	{
		::SetCursor(m_hCursor_NESW);
	}
	/* ¿ìÇÏ Å©±â */
	else if (p.x>rcTemp.right-iGap && p.x<rcTemp.right+iGap && p.y>rcTemp.bottom-iGap && p.y<rcTemp.bottom+20)
	{
		::SetCursor(m_hCursor_NWSE);
	}
	else
	{
		::SetCursor(m_hCursor_Standard);
	}
}


int CAABonderDlg::checkMousePos(CPoint p, CRect rcTemp)
{
	double dExpandFactorX;
	double dExpandFactorY;
	int	iGap;

	if (m_iCurCamNo<3)
	{
		dExpandFactorX = CAM_EXPAND_FACTOR_X;
		dExpandFactorY = CAM_EXPAND_FACTOR_Y;
		iGap = 20;
	}
	else
	{
		dExpandFactorX = (double)gMIUDevice.nWidth/SMALL_CCD_SIZE_X;
		dExpandFactorY = (double)gMIUDevice.nHeight/SMALL_CCD_SIZE_Y;
		// 20140905 Overlay Box À§Ä¡ ÀÌµ¿ ½Ã Box ¼±¿¡¼­ ¾ó¸¶³ª °¡±îÀÌ ÀÖ¾î¾ß ¸¶¿ì½º Ä¿¼­ ¸ð¾ç º¯°æ °Å¸® È®ÀÎ ¿ë..
		//iGap = 50;
		iGap = int(dExpandFactorX * 5);
	}

	CPoint point = p;

	int iRtn = -1;

	point.x -= m_rectCamDispPos1.left;
	point.y -= m_rectCamDispPos1.top;

	p.x = (int)(point.x * dExpandFactorX + 0.5);
	p.y = (int)(point.y * dExpandFactorY + 0.5);

	/* ¹Ú½º ÀÌµ¿ */
	if (p.x>rcTemp.left+iGap	&&
		p.x<rcTemp.right-iGap	&&
		p.y>rcTemp.top+iGap		&&
		p.y<rcTemp.bottom-iGap)
	{
		iRtn = MOVE_ALL;
	}
	/* ÁÂ Å©±â */
	else if (p.y>rcTemp.top+iGap && p.y<rcTemp.bottom-iGap && p.x>rcTemp.left-iGap && p.x<rcTemp.left+iGap)
	{
		iRtn = MOVE_WIDTH_LEFT;
	}
	/* ¿ì Å©±â */
	else if (p.y>rcTemp.top+iGap && p.y<rcTemp.bottom-iGap && p.x>rcTemp.right-iGap && p.x<rcTemp.right+iGap)
	{
		iRtn = MOVE_WIDTH_RIGHT;
	}
	/* »ó Å©±â */
	else if (p.x>rcTemp.left+iGap && p.x<rcTemp.right-iGap && p.y>rcTemp.top-iGap && p.y<rcTemp.top+iGap)
	{
		iRtn = MOVE_HEIGHT_TOP;
	}
	/* ÇÏ Å©±â */
	else if (p.x>rcTemp.left+iGap && p.x<rcTemp.right-iGap && p.y>rcTemp.bottom-iGap && p.y<rcTemp.bottom+iGap)
	{
		iRtn = MOVE_HEIGHT_BOTTOM;
	}
	/* ÁÂ»ó Å©±â */
	else if (p.x>rcTemp.left-iGap && p.x<rcTemp.left+iGap && p.y>rcTemp.top-iGap && p.y<rcTemp.top+iGap)
	{
		iRtn = MOVE_NW;
	}
	/* ¿ì»ó Å©±â */
	else if (p.x>rcTemp.right-iGap && p.x<rcTemp.right+iGap && p.y>rcTemp.top-iGap && p.y<rcTemp.top+iGap)
	{
		iRtn = MOVE_NE;
	}
	/* ÁÂÇÏ Å©±â */
	else if (p.x>rcTemp.left-iGap && p.x<rcTemp.left+iGap && p.y>rcTemp.bottom-iGap && p.y<rcTemp.bottom+iGap)
	{
		iRtn = MOVE_SW;
	}
	/* ¿ìÇÏ Å©±â */
	else if (p.x>rcTemp.right-iGap && p.x<rcTemp.right+iGap && p.y>rcTemp.bottom-iGap && p.y<rcTemp.bottom+iGap)
	{
		iRtn = MOVE_SE;
	}
	else
	{
	}

	return iRtn;
}

void CAABonderDlg::OnBnClickedButtonOrigin()
{
	CString sTemp="";

	if(Task.AutoFlag == 1) 
	{
		sLangChange.LoadStringA(IDS_STRING1395);	//"ÀüÃ¼ ¿øÁ¡ º¹±Í ½ÇÆÐ - ÀÚµ¿¿îÀü Áß
		sTemp.Format(sLangChange);
		delayMsg(sTemp.GetBuffer(99), 3000, M_COLOR_DARK_GREEN);
		return;
	}

	if(g_bMovingflag)
	{
		sLangChange.LoadStringA(IDS_STRING1394);	//ÀüÃ¼ ¿øÁ¡ º¹±Í ½ÇÆÐ - ¸ðÅÍ ÀÌµ¿ Áß
		sTemp.Format(sLangChange);
		delayMsg(sTemp.GetBuffer(99), 3000, M_COLOR_DARK_GREEN);
		return;
	}


	bool home_process_run_flag = true;
	g_bMovingflag =true;
	sLangChange.LoadStringA(IDS_STRING1398);	//ÀüÃ¼ ¿øÁ¡ º¹±Í¸¦ ½ÇÇà ÇÏ½Ã°Ú½À´Ï±î?
	sTemp.Format("ÀüÃ¼ ¿øÁ¡ º¹±Í¸¦ ½ÇÇà ÇÏ½Ã°Ú½À´Ï±î?");	//ÀüÃ¼ ¿øÁ¡ º¹±Í¸¦ ½ÇÇà ÇÏ½Ã°Ú½À´Ï±î?
	if (askMsg(sTemp))
	{
		pThread_TaskOrigin = ::AfxBeginThread(Thread_TaskOrigin, this);
	}

	
}


BOOL CAABonderDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
		else if(pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
		else if(pMsg->wParam == VK_F2)
		{
		}
		else if(pMsg->wParam == VK_F3)
		{
		}
		else if(pMsg->wParam == VK_F4)
		{
		}
		else if(pMsg->wParam == VK_F5)
		{
		}
		else if(pMsg->wParam == VK_F6)
		{
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}



//! Modified by LHW (2013/2/25, 2013/3/13)
void CAABonderDlg::CcmThreadStart()
{
	pThread_CCM_Grab = NULL; 
	pThread_CCM_Display = NULL;

	pThread_CCM_Mil_CvtColor_Ready = NULL;
	pThread_CCM_Mil_CvtColor = NULL;

	pThread_CCM_CvtColor = NULL;
	pThread_CCM_CvtMil = NULL;
	
	bFlag_First_Grab_Display = false;

	pThread_CCM_Grab = ::AfxBeginThread(Thread_Ccm_Grab, this);
	pThread_CCM_CvtColor = ::AfxBeginThread(Thread_Ccm_CvtColor, this);
	pThread_CCM_CvtMil = ::AfxBeginThread(Thread_Ccm_CvtMil, this);
	pThread_CCM_Display = ::AfxBeginThread(Thread_Ccm_Display, this);//norinda

	CCMThreadStart_Auto_Exposure_Time();
}

//! Auto Exposure Time Thread ½ÃÀÛ
void CAABonderDlg::CCMThreadStart_Auto_Exposure_Time()
{
	pThread_CCM_Auto_Exposure_Time = NULL;

	pThread_CCM_Auto_Exposure_Time = ::AfxBeginThread(Thread_Ccm_Auto_Exposure_Time,  this);
}

//! Auto White Balance Thread ½ÃÀÛ
void CAABonderDlg::CCMThreadStart_Auto_WhiteBalance()
{
}

double CAABonderDlg::HolderThetaCalc(double _theta , int index)
{
	//Áß½ÉÃàÀÇ È¸Àü·®ÀÇ µû¶ó Æ¯Á¤Æ÷ÀÎÆ®(x,y)ÀÇ ÀÌµ¿°Å¸® ±¸ÇÏ±â
	double temp;
	temp = 0.0;
	_theta = _theta * (M_PI/180);
	switch(index)
	{
		case 0://xÃà
			temp = (model.holderx-((model.holderx*cos(_theta)) - (model.holdery*sin(_theta))))*-1;
			//((model.holderx-centerX)*cos(_theta) - (model.holdery-centerY)*sin(_theta) + centerX)
			break;

		case 1://yÃà
			temp = (model.holdery-((model.holderx*sin(_theta)) + (model.holdery*cos(_theta))))*-1;
			//((model.holderx-centerX)*sin(_theta) + (model.holdery-centerY)*cos(_theta) + centerY)
			break;

		default:

			break;
	}
	return temp;
}
int CAABonderDlg::AlignLimitCheck(int Insptype, double dOffsetX, double dOffsetY, double dOffsetTh)										// [Insptype] 0:Lens PreAlign 1:PCB PreAlign
{																					// Return	0:NG,  1:Retry,  2:OK
	if(Insptype ==1)
	{
		if(fabs(dOffsetX)  > model.axis[Motor_Lens_X].m_dLimit_Err 
			|| fabs(dOffsetY)  > model.axis[Motor_Lens_Y].m_dLimit_Err)
		{
			putListLog("LensºÎ X, Y º¸Á¤°ª ÀÌµ¿ Limit¸¦ ÃÊ°ú ÇÏ¿´½À´Ï´Ù.");
			return 0;
		}
	}
	else if(Insptype ==0)
	{
		if(fabs(dOffsetX)  > model.axis[Motor_PCB_X].m_dLimit_Err 
			|| fabs(dOffsetY)  > model.axis[Motor_PCB_Y].m_dLimit_Err 
			|| fabs(dOffsetTh)  > model.axis[Motor_PCB_TH].m_dLimit_Err)
			return 0;
	}


	if(Insptype ==1)
	{
		if(fabs(dOffsetX)  > model.axis[Motor_Lens_X].m_dLimit_OK 
			|| fabs(dOffsetY)  > model.axis[Motor_Lens_Y].m_dLimit_OK
			|| fabs(dOffsetTh)  > model.axis[Motor_PCB_TH].m_dLimit_OK)
			return 1;
	}
	else if(Insptype ==0)
	{
		if(fabs(dOffsetX)  > model.axis[Motor_PCB_X].m_dLimit_OK 
			|| fabs(dOffsetY)  > model.axis[Motor_PCB_Y].m_dLimit_OK
			|| fabs(dOffsetTh)  > model.axis[Motor_PCB_TH].m_dLimit_OK)
			return 1;
	}

	return 2;
}



int CAABonderDlg::procCamComAlign(int camNo, int iMarkType, bool liveMode, double &dOffsetX, double &dOffsetY, double &dOffsetTh)
{
	int iMarkNo;
	CString sTemp;
	dOffsetTh = 0.0f;
	
	vision.clearOverlay(camNo);
	double ep1;
	MappTimer(M_TIMER_RESET, &ep1);
#ifndef		NORINDA_MODE
	vision.getSnapImage(camNo);
	Sleep(200);
#endif
	double ep2;
	MappTimer(M_TIMER_READ, &ep2);
#ifdef		NORINDA_MODE
	if(vision.getLiveMode())
	{
		vision.getSnapImage(camNo);
		vision.setLiveMode(true);
	}
#else
	if(liveMode){
		vision.setLiveMode(true);
	}
#endif
	iMarkNo = vision.findComAlignMark(camNo, iMarkType);

	sTemp.Format(_T(" %.0f msec"), (ep2 - ep1)*1000);
	vision.textlist[camNo].addList((CAM_SIZE_X-140), (CAM_SIZE_Y-60), sTemp, M_COLOR_WHITE, 24, 10, _T("arialuni"));

	vision.drawOverlay(camNo);

	if ( iMarkNo!= -1 )
	{
		MmodControl(vision.ModResult[iMarkType][iMarkNo], M_DEFAULT, M_DRAW_SCALE_X, CAM_REDUCE_FACTOR_X);
		MmodControl(vision.ModResult[iMarkType][iMarkNo], M_DEFAULT, M_DRAW_SCALE_Y, CAM_REDUCE_FACTOR_Y);

		MgraColor(M_DEFAULT, M_COLOR_GREEN);
		MmodDraw(M_DEFAULT, vision.ModResult[iMarkType][iMarkNo], vision.MilOverlayImage[camNo], M_DRAW_BOX+M_DRAW_POSITION+M_DRAW_EDGES+M_DRAW_AXIS, M_DEFAULT, M_DEFAULT);

		if (calcAlignData(camNo, iMarkType, dOffsetX, dOffsetY, dOffsetTh) == false)
		{
			sTemp.Format("¿ÏÁ¦Ç° Align °Ë»ç NG");
			putListLog(sTemp);

			return -1;
		}
	}
	return iMarkNo;
}

int CAABonderDlg::procCamAlign(int camNo, int iMarkType, bool liveMode, double &dOffsetX, double &dOffsetY, double &dOffsetTh)
{
	int iMarkNo;
	CString sTemp;
	dOffsetX = dOffsetY = dOffsetTh = 0.0f;

	vision.clearOverlay(camNo);
	double ep1;
	MappTimer(M_TIMER_RESET, &ep1);

	vision.getSnapImage(camNo);
	Sleep(200);

	double ep2;
	MappTimer(M_TIMER_READ, &ep2);

	if(liveMode)
		vision.setLiveMode(true);
	
	iMarkNo = vision.findMark(camNo, iMarkType);
	
	sTemp.Format(_T(" %.0f msec"), (ep2 - ep1)*1000);
	vision.textlist[camNo].addList((CAM_SIZE_X-140), (CAM_SIZE_Y-60), sTemp, M_COLOR_WHITE, 24, 10, _T("arialuni"));

	vision.drawOverlay(camNo);

	if ( iMarkNo!= -1 )
	{
		MmodControl(vision.ModResult[iMarkType][iMarkNo], M_DEFAULT, M_DRAW_SCALE_X, CAM_REDUCE_FACTOR_X);
		MmodControl(vision.ModResult[iMarkType][iMarkNo], M_DEFAULT, M_DRAW_SCALE_Y, CAM_REDUCE_FACTOR_Y);

		MgraColor(M_DEFAULT, M_COLOR_GREEN);
		MmodDraw(M_DEFAULT, vision.ModResult[iMarkType][iMarkNo], vision.MilOverlayImage[camNo], M_DRAW_BOX+M_DRAW_POSITION+M_DRAW_EDGES+M_DRAW_AXIS, M_DEFAULT, M_DEFAULT);

		if (calcAlignData(camNo, iMarkType, dOffsetX, dOffsetY, dOffsetTh) == false)
		{
			if(camNo==1)
			{
				sLangChange.LoadStringA(IDS_STRING656);	//Lens Align °Ë»ç NG.
				sTemp.Format(sLangChange);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING906);	//PCB Align °Ë»ç NG.
				sTemp.Format(sLangChange);
			}

			putListLog(sTemp.GetBuffer(99));

			return -1;
		}
	}

	if (iMarkNo==-1)
		return -1;

	return 0;
}


int CAABonderDlg::_findOpticalCenter()
{
	double fShiftX, fShiftY;
	CRect roi;
	CString sTmpLog;
	bool bRtn;

	roi.left	= (int)(gMIUDevice.nWidth * 0.1 + 0.5);
	roi.top		= (int)(gMIUDevice.nHeight * 0.1 + 0.5);
	roi.right	= (int)(gMIUDevice.nWidth * 0.9 + 0.5);
	roi.bottom	= (int)(gMIUDevice.nHeight * 0.9 + 0.5);

	vision.clearOverlay(3);

	double ep = myTimer(true);

	bRtn = _GetOpticAxis(4, 0, fShiftX, fShiftY);

	double eTime = myTimer(true);

	if(bRtn)
	{
		vision.crosslist[3].addList((int)fShiftX, (int)fShiftY, 500, M_COLOR_RED);
		vision.drawOverlay(CCD);
	
		sLangChange.LoadStringA(IDS_STRING1162);	//±¤Ãà Shift (%.01f, %.01f) °Ë»ç ¿Ï·á. %.01lf msec
		sTmpLog.Format(sLangChange, fShiftX, fShiftY, eTime-ep);
		putListLog(sTmpLog);
	}
	else
	{
		saveInspImage(OPTIC_IMAGE_SAVE, Task.PCBTask);
		sLangChange.LoadStringA(IDS_STRING1165);		//±¤Ãà °Ë»ç ½ÇÆÐ.
		putListLog(sLangChange);
		return -1;
	}

	Task.dOpticalPosX = fShiftX;
	Task.dOpticalPosY = fShiftY;

	Task.dOpticalShiftX = (gMIUDevice.nWidth/2 - fShiftX) * model.m_dSize_CCD_Cell / 1000;
	Task.dOpticalShiftY = (gMIUDevice.nHeight/2 - fShiftY) * model.m_dSize_CCD_Cell / 1000;

	if ( fabs(Task.dOpticalShiftX) > 1 || fabs(Task.dOpticalShiftY) > 1 )
		return -1;

	return 1;
}

void CAABonderDlg::calcCamRotatePos(int iCamNo, double dFindX, double dFindY, double dBaseX, double dBaseY, double& dCx, double& dCy)
{
	double dCosVal,dSinVal;
	double dTheta;

	//--- È¸Àü·®ÀÌ Degree·Î ÀúÀåµÇ¹Ç·Î...
	dTheta = (sysData.dCamAngle[iCamNo].x*M_PI) / 180.0;
	dCosVal = cos(dTheta);
	dSinVal = sin(dTheta);

	dCx = (dCosVal*(dFindX-dBaseX))-(dSinVal*(dBaseY-dFindY));
	dCx = dCx + dBaseX;

	//--- È¸Àü·®ÀÌ Degree·Î ÀúÀåµÇ¹Ç·Î...
	dTheta = (sysData.dCamAngle[iCamNo].y*M_PI) / 180.0;
	dCosVal = cos(dTheta);
	dSinVal = sin(dTheta);

	dCy = (dSinVal*(dFindX-dBaseX))+(dCosVal*(dBaseY-dFindY));
	dCy = dCy*(-1) + dBaseY;
}


bool CAABonderDlg::calcAlignData(int camNo, int iMarkType, double &dOffsetX, double &dOffsetY, double &dOffsetTh)
{
	dOffsetX = dOffsetY = dOffsetTh = 0;

	double dRadianTh = g_dFindAngle[iMarkType] * M_PI / 180.0;

	double dTempCurX=0.0, dTempCurY=0.0;

	calcCamRotatePos(camNo, g_dFindCenterX[iMarkType], g_dFindCenterY[iMarkType], (CAM_SIZE_X>>1), (CAM_SIZE_Y>>1), dTempCurX, dTempCurY);

	CDPoint curPos;

	curPos.x		=	dTempCurX - (CAM_SIZE_X>>1);
	curPos.y		=	(CAM_SIZE_Y>>1) - dTempCurY;

	CDPoint rotatePos;
	double dCosVal=0.0, dSinVal=0.0;

	dCosVal = cos(dRadianTh);
	dSinVal = sin(dRadianTh);

	rotatePos.x = (curPos.x*dCosVal) - (curPos.y*dSinVal);
	rotatePos.y = (curPos.x*dSinVal) + (curPos.y*dCosVal);

	if(camNo == 1)
	{
		dOffsetX	=	rotatePos.x;
		dOffsetY	=	rotatePos.y;
		dOffsetTh	=	g_dFindAngle[iMarkType];
	}
	else
	{
		dOffsetX	=	rotatePos.x;
		dOffsetY	=	rotatePos.y;
		dOffsetTh	=	g_dFindAngle[iMarkType];
	}

	dOffsetX *= sysData.dCamResol[camNo].x;
	dOffsetY *= sysData.dCamResol[camNo].y;

	CString sTemp="";
	sLangChange.LoadStringA(IDS_STRING1482);	//º¸Á¤°ª : %.3lf %.3lf %.3lf
	sTemp.Format(sLangChange, dOffsetX, dOffsetY, dOffsetTh);
	vision.textlist[camNo].addList(20, (CAM_SIZE_Y-50), sTemp, M_COLOR_CYAN, 24, 10, "Arial Black");

	return true;
}

/////////////////////// ¾ÆÁ÷ ÀÛ¾÷ Áß ÀÔ´Ï´Ù~~
int CAABonderDlg::checkAutoRunLensAlarm(int fi_step)					// 0:Á¤»ó, 1:Á¤Áö ÇÒÁö ¸»Áö ¼±ÅÃ ÇÒ ¼ö ÀÖµµ·Ï, 2:¹«Á¶°Ç Á¤Áö
{
	////////////////////////////////////////////////////////////////////////////////
	// ¸ðÅÍ AmpFault

	motor.AmpFaultCheck();
	if(Task.iAmpFaultFlag)
	{
		m_labelServo.SetBkColor(M_COLOR_RED);
		m_labelServo.Invalidate();
		sLangChange.LoadStringA(IDS_STRING1327);	//¿îÀü Áß AMP Fault°¡ °¨Áö µÇ¾ú½À´Ï´Ù.
		_stprintf_s(sz_PCB_Error_Msg, sLangChange);
		return 2;
	}


	////////////////////////////////////////////////////////////////////////////////
	// ¸ðÅÍ ¿øÁ¡ º¹±Í È®ÀÎ..

	motor.HomeCheck();

	if(Task.iHomeErrorFlag)
	{
		m_labelHom.SetBkColor(M_COLOR_RED);
		m_labelHom.Invalidate();
		sLangChange.LoadStringA(IDS_STRING1331);	//¿øÁ¡ º¹±Í ÇØÁö°¡ °¨Áö µÇ¾ú½À´Ï´Ù.
		_stprintf_s(sz_PCB_Error_Msg, sLangChange);
		return 2;
	}
	
	//motor.InDIO(0, curInDio[0]);
	return 0;
}


int CAABonderDlg::checkAutoRunPcbAlarm(int fi_step)					// 0:Á¤»ó, 1:Á¤Áö ÇÒÁö ¸»Áö ¼±ÅÃ ÇÒ ¼ö ÀÖµµ·Ï, 2:¹«Á¶°Ç Á¤Áö
{
	////////////////////////////////////////////////////////////////////////////////
	// ¸ðÅÍ AmpFault

	bool testFlag = true;

	motor.AmpFaultCheck();
	if(Task.iAmpFaultFlag)
	{
		m_labelServo.SetBkColor(M_COLOR_RED);
		m_labelServo.Invalidate();
		sLangChange.LoadStringA(IDS_STRING1327);	//¿îÀü Áß AMP Fault°¡ °¨Áö µÇ¾ú½À´Ï´Ù.
		_stprintf_s(sz_PCB_Error_Msg, sLangChange);
		return 2;
	}


	////////////////////////////////////////////////////////////////////////////////
	// ¸ðÅÍ ¿øÁ¡ º¹±Í È®ÀÎ..

	motor.HomeCheck();

	if(Task.iHomeErrorFlag)
	{
		m_labelHom.SetBkColor(M_COLOR_RED);
		m_labelHom.Invalidate();

		sLangChange.LoadStringA(IDS_STRING1331);	//¿øÁ¡ º¹±Í ÇØÁö°¡ °¨Áö µÇ¾ú½À´Ï´Ù.
		_stprintf_s(sz_PCB_Error_Msg, sLangChange);
		return 2;
	}

	//motor.InDIO(0, curInDio[0]);

	return 0;
}

int CAABonderDlg::_checkPcbMotor()
{
	double dMotorPosX  = motor.GetEncoderPos(Motor_PCB_X);
	double dMotorPosY  = motor.GetEncoderPos(Motor_PCB_Y);
	double dMotorPosTH = motor.GetEncoderPos(Motor_PCB_TH);

	double dMotorSavePosX  = model.axis[Motor_PCB_X].pos[Bonding_Pos];
	double dMotorSavePosY  = model.axis[Motor_PCB_Y].pos[Bonding_Pos];
	double dMotorSavePosTH = model.axis[Motor_PCB_TH].pos[Bonding_Pos];

	if ( fabs(dMotorPosX-dMotorSavePosX) < model.axis[Motor_PCB_X].m_dLimit_Err		&&
		 fabs(dMotorPosY-dMotorSavePosY) < model.axis[Motor_PCB_Y].m_dLimit_Err		&&
		 fabs(dMotorPosTH-dMotorSavePosTH) < model.axis[Motor_PCB_TH].m_dLimit_Err )
	{
		return 1;
	}
	else
	{
		return -1;
	}
}


int CAABonderDlg::_moveZMotor(double dDist, double dVel)
{
	double dCurPos = 0.0;
	double dMovePos = 0.0;
#if (____AA_WAY == PCB_TILT_AA)
	dMovePos = motor.GetCommandPos(TITLE_MOTOR_Z) + dDist;
#elif (____AA_WAY == LENS_TILT_AA)
	dMovePos = motor.GetCommandPos(TITLE_MOTOR_Z) - dDist;
#endif
	bool b_move_ok_flag = motor.MoveAxis(TITLE_MOTOR_Z, ABS, dMovePos, dVel, sysData.fMotorAccTime[TITLE_MOTOR_Z]);


	if(!b_move_ok_flag){
		return -1;
	}
	double ep = myTimer(true);

	while (1)
	{

		if ( myTimer(true)-ep< 5000)
		{
			if ( motor.IsStopAxis(TITLE_MOTOR_Z))// && motor.GetInposition(TITLE_MOTOR_Z) )
			{
				dCurPos = motor.GetCommandPos(TITLE_MOTOR_Z);

				if (fabs(dCurPos-dMovePos)<=0.003)
				{
					break;
				}
			}
		}
		else
		{
			putListLog("Auto Focusing ñé Z Motor ÀÌµ¿ ½Ã°£ ÃÊ°ú.");
			return -1;
		}
	}

	return 1;
}


int CAABonderDlg::_getMTF(int Mode)  
{
	const int mLocalMtfCnt = model.mGlobalSmallChartCount;
	bool bRtn = false;
	//CString sLog="";
	//CString logStr="";
	int iCnt_Focus = 0; 
	int i = 0;
	int j = 0;
	TCHAR szLog[SIZE_OF_1K];
	dSFR_N_8_PositionX = model.m_Line_Pulse;
	dSFR_N_4_PositionX = model.m_Line_Pulse;
	//
	float fMax_SFR_N4[COMMON_MTF_INSP_CNT] = {0, };  //MTF_INSP_CNT

	if ( model.strInfo_CCM.m_iCnt_Average_Thru_Focus <= 0 )
	{
		model.strInfo_CCM.m_iCnt_Average_Thru_Focus = 1;
	}

	//gMIUDevice.CurrentState = 3;	//Á¤Áö¿µ»ó
	vision.clearOverlay(CCD);
	vision.MilBufferUpdate();	//Ã¹Àå¿¡¼­ ¾²·¹±â Image°¡ »ý±â´Â Çö»ó ¹ß»ýÀ¸·Î ÃÊ±â ÇÑ¹ø ¿µ»ó °¡Á®¿È


	Task.getROI();				// ¿øÇü ¸¶Å© À§Ä¡ ÀÎ½Ä..
	for(int i=0; i<COMMON_MTF_INSP_CNT; i++)
	{ 
		fMax_SFR_N4[i] = 0.0;
		Task.SFR.dMaxSfr_StepLog[i] = 0.0;
	}

	
	if(Mode==SFR_FINAL)
	{
		iCnt_Focus=model.strInfo_CCM.m_iCnt_Average_Fine_Focus;
	}
	else
	{
		iCnt_Focus=model.strInfo_CCM.m_iCnt_Average_Thru_Focus;
	}
	double dFrequency[1] = {dSFR_N_8_PositionX}; // Spatial frequency for SFR calculation  
	double dSfrFinalResult[COMMON_MTF_INSP_CNT][1] = {{0.0},}; // SFR result
	
	int nWidth = gMIUDevice.imageItp->width;
	int nHeight = gMIUDevice.imageItp->height;
	int iSizeX = model.m_iSize_ROI_X+1;
	int iSizeY = model.m_iSize_ROI_Y+1;
	vYImgBuffer.resize(nHeight * nWidth, 0);
	vYImgBuffer.clear();
	
	int dic = 0;
	CString strView = "";
	bool sfrRt = false;


    //==============================================================================================================================================
    //==============================================================================================================================================
    //new
	double _offset = 0.0;
    int nBlackLevel = 0;
	std::shared_ptr<CACMISResolutionSFR> m_pSFRProc = std::make_shared<CACMISResolutionSFR>();
	POINT ptROICenter[COMMON_MTF_INSP_CNT];
	TSFRSpec m_stSFRSpec;
    for (j = 0; j < iCnt_Focus; j++)
    {
        if (j == 0) 
		{
            bRtn = Task.getROI_SFR(Mode);				// »ç°¢Çü Box À§Ä¡ ÀÎ½Ä..
			for (i = 0; i < model.mGlobalSmallChartCount; i++)	//for (int i = 0; i < MTF_INSP_CNT; i++)
			{
				ptROICenter[i].x = Task.SFR._64_Sfr_Rect[i].left + (iSizeX / 2);
				ptROICenter[i].y = Task.SFR._64_Sfr_Rect[i].top + (iSizeY / 2);
				//
				Task.m_vDirection[i] = model.m_MTF_Direction[i]; // 0: Vertical, 1: Horizontal
				Task.m_vFrquency[i] = model.m_Line_Pulse;//0.225
				Task.m_vOffset[i] = 0.0;
				Task.m_vSFR[i] = 0.0;
				Task.m_vThreshold[i] = 0.0;
				Task.m_vROI[i].ptCenter.x = ptROICenter[i].x;
				Task.m_vROI[i].ptCenter.y = ptROICenter[i].y;
				Task.m_vROI[i].nROIWidth = model.m_MTF_Direction[i] == 1 ? iSizeX : iSizeY;
				Task.m_vROI[i].nROIHeight = model.m_MTF_Direction[i] == 0 ? iSizeY : iSizeX;//SFR_ROI_HORIZONTAL ? iSizeY : iSizeX;
			}
			
			m_stSFRSpec.tSFRConfig.nMaxROIWidth = iSizeX;
			m_stSFRSpec.tSFRConfig.nMaxROIHeight = iSizeY;
			m_stSFRSpec.tSFRConfig.dMaxEdgeAngle = 45.0;
			m_stSFRSpec.tSFRConfig.dPixelSize = model.m_dSize_CCD_Cell;// 4.2;
			m_stSFRSpec.tSFRConfig.eAlgorithmType = ESFRAlgorithm_ISO12233;
			m_stSFRSpec.tSFRConfig.eAlgorithmMethod = ESFRMethod_Freq2SFR;
			/////m_stSFRSpec.tSFRConfig.eFrequencyUnit = ESFRFreq_LinePairPerMilliMeter;	//½ÊÀÇÀÚ¸®
			m_stSFRSpec.tSFRConfig.eFrequencyUnit = ESFRFreq_CyclePerPixel;		//¼Ò¼öÁ¡
			m_stSFRSpec.eSFRDeltaAlgorithmType = ESFRDelta_Diff;
			m_stSFRSpec.dEdgeDir = Task.m_vDirection.data();
			m_stSFRSpec.dFrequency = Task.m_vFrquency.data();
			m_stSFRSpec.dSFR = Task.m_vSFR.data();
			m_stSFRSpec.dThreshold = Task.m_vThreshold.data();
			m_stSFRSpec.dGamma = 1.0;
			m_stSFRSpec.tROI.dOffset = Task.m_vOffset.data();
			m_stSFRSpec.tROI.eROIType = ROIType_POINT;
			m_stSFRSpec.tROI.pROIData = Task.m_vROI.data();
			m_stSFRSpec.tROI.ROICount = model.mGlobalSmallChartCount;// MTF_INSP_CNT;
			m_stSFRSpec.tDelataSpec = NULL;
        }
        
		if (bRtn == false)
		{
			if (Task.AutoFlag == 1)
			{
				saveInspImage(AA_CIRCLE_NG_IMAGE_SAVE, Task.m_iCnt_Step_AA_Total);
			}
			_stprintf_s(szLog, SIZE_OF_1K, _T("[SFR ÃøÁ¤] ¿øÇü ¸¶Å© ÀÎ½Ä ½ÇÆÐ"));
			errMsg2(Task.AutoFlag, szLog);
			return -1;
		}

		bool bRet = m_pSFRProc->Inspect(MIU.m_pFrameRawBuffer, nWidth, nHeight, m_stSFRSpec,
			gMIUDevice.dTDATASPEC_n.eDataFormat, gMIUDevice.dTDATASPEC_n.eOutMode, gMIUDevice.dTDATASPEC_n.eSensorType, nBlackLevel, false, false, gMIUDevice.dTDATASPEC_n.eDemosaicMethod);//gMIUDevice.dTDATASPEC_n.eSensorType

        int sfrMax = m_pSFRProc->GetMaxResolutionCount();
        float sfrValue = 0.0;
        

        for (i = 0; i < sfrMax; i++)
        {
            const TSFRROIResult* pROIResult = m_pSFRProc->GetSFRROIResult(i);
			sfrValue = pROIResult->dFinalResult[0];
            if (pROIResult)
            {
                if (!_finite(sfrValue) || sfrValue > 0.99 || sfrValue < 0.0)
                {
                    Task.SFR._64_fSfrN4[i] = 0.0;
                }
                else
                {
					Task.SFR._64_fSfrN4[i] = sfrValue;
                }
                //
				if (Task.SFR._64_fSfrN4[i] < 0.0)
				{
					Task.SFR._64_fSfrN4[i] = 0.0;
				}
#ifdef SFR_COMP_MAX
				if (fMax_SFR_N4[i] < Task.SFR._64_fSfrN4[i])
				{
					fMax_SFR_N4[i] = Task.SFR._64_fSfrN4[i];
				}
				else
				{
					fMax_SFR_N4[i] = fMax_SFR_N4[i];
				}
				strView.Format("SFR [%d]  =  %f", i, fMax_SFR_N4[i]);
				putListLog(strView);
				int aaa = model.sfrElem.m_SfrTestUse[0][i];	//20,4
				aaa = model.sfrElem.m_AATiltCalRoi[0][i];	//20,4
#else
                fMax_SFR_N4[i] += Task.SFR._64_fSfrN4[i]; 

#endif
				Task.SFR.dMaxSfr_StepLog[i] = fMax_SFR_N4[i];
            }
        }
    }

	// SFR°è»êÀ» Æò±ÕÀ¸·Î ÇÒÁö ÃÖ´ëÄ¡·Î ÇÒÁö 
	// Æò±ÕÀº SONY1
#ifndef SFR_COMP_MAX
	for (int i = 0; i<model.mGlobalSmallChartCount; i++)
	{
		fMax_SFR_N4[i] = fMax_SFR_N4[i] / iCnt_Focus;
		//logStr.Format("SFR(%d) = %f", i, fMax_SFR_N4[i]);
		_stprintf_s(szLog, SIZE_OF_1K, _T("SFR(%d) = %f"), i, fMax_SFR_N4[i]);
		putListLog(logStr);
	}
#endif

	//Plus Çü 
	//[0]CENTER
	//[1]TOP , [2]BOTTOM , [3]LEFT , [5]RIGHT
	//
	//¿¢½ºÀÚÇü
	//[0]CENTER
	//[1]LT  , [2]RT	 , [3]BL   , [4]BR

	//model.sfrElem.m_SfrTestUse[index][i]
	//model.sfrElem.m_AATiltCalRoi[index][i]
	int nPartternRoiIndex = 0;
	int nSmallRoiIndex = 0;
	double mAvrSfrData = 0.0;
	double mAvrSfrPosX = 0.0;
	double mAvrSfrPosY = 0.0;

	double mTempPosX = 0.0;
	double mTempPosY = 0.0;
	int mIndex = 0;
	int mAvrIndex = 0;
	//new ¹æ½Ä
	for (i = 0; i < 5; i++)	//AA ÆÐÅÏ ¿µ¿ª
	{
		nPartternRoiIndex = model.sfrElem.m_AATiltRoiIndex[i];
		mAvrSfrData = 0.0;
		mTempPosX = 0.0;
		mTempPosY = 0.0;
		mAvrIndex = 0;
		for (j = 0; j < 4; j++)		//ÆÐÅÏ ³»¿¡¼­ T,B,L,R ¿µ¿ª
		{
			if (model.sfrElem.m_AATiltCalRoi[nPartternRoiIndex][j] == 1)	//aa°è»ê ¿µ¿ªÀ¸·Î Ã¼Å©µÅÀÖÀ¸¸é
			{
				mIndex = Task.SFR.nTiltCalIndex[nSmallRoiIndex];
				mAvrSfrData += fMax_SFR_N4[mIndex];
				mTempPosX += (Task.SFR._64_Sfr_Rect[mIndex].left + Task.SFR._64_Sfr_Rect[mIndex].right) / 2.0;
				mTempPosY += (Task.SFR._64_Sfr_Rect[mIndex].top + Task.SFR._64_Sfr_Rect[mIndex].bottom) / 2.0;
				nSmallRoiIndex++;
				mAvrIndex++;
			}
		}
		mAvrSfrPosX = mTempPosX / mAvrIndex;
		mAvrSfrPosY = mTempPosY / mAvrIndex;
		Task.SFR.fSfrN4[Task.m_iCnt_Step_AA_Total][i] = mAvrSfrData / mAvrIndex; 
		Task.SFR.Sfr_pos[Task.m_iCnt_Step_AA_Total][i].x = mAvrSfrPosX;
		Task.SFR.Sfr_pos[Task.m_iCnt_Step_AA_Total][i].y = mAvrSfrPosY;
		_stprintf_s(szLog, SIZE_OF_1K, _T("[%d]Tilt Cal Average Sfr = %f[x:%.3lf/y:%.3lf]"), i, Task.SFR.fSfrN4[Task.m_iCnt_Step_AA_Total][i], Task.SFR.Sfr_pos[Task.m_iCnt_Step_AA_Total][i].x, Task.SFR.Sfr_pos[Task.m_iCnt_Step_AA_Total][i].y);
		putListLog(szLog);
	}

	//end new ¹æ½Ä
	//for (i = 0; i < 5; i++)		//for(int i=0; i<LAST_MARK_CNT; i++)
	//{
	//	if( i == 0)
	//	{
	//		mAvrSfrData = (fMax_SFR_N4[0] + fMax_SFR_N4[1] + fMax_SFR_N4[2] + fMax_SFR_N4[3]) / 4.0;
	//		mAvrSfrPosX = (((Task.SFR._64_Sfr_Rect[0].left + Task.SFR._64_Sfr_Rect[0].right) / 2.0) +((Task.SFR._64_Sfr_Rect[1].left + Task.SFR._64_Sfr_Rect[1].right) / 2.0) +
	//			((Task.SFR._64_Sfr_Rect[2].left + Task.SFR._64_Sfr_Rect[2].right) / 2.0) + ((Task.SFR._64_Sfr_Rect[3].left + Task.SFR._64_Sfr_Rect[3].right) / 2.0)) / 4.0;
	//		mAvrSfrPosY = (((Task.SFR._64_Sfr_Rect[0].top + Task.SFR._64_Sfr_Rect[0].bottom) / 2.0) + ((Task.SFR._64_Sfr_Rect[1].top + Task.SFR._64_Sfr_Rect[1].bottom) / 2.0) +
	//			((Task.SFR._64_Sfr_Rect[2].top + Task.SFR._64_Sfr_Rect[2].bottom) / 2.0) + ((Task.SFR._64_Sfr_Rect[3].top + Task.SFR._64_Sfr_Rect[3].bottom) / 2.0)) / 4.0;
	//		//
	//		Task.SFR.fSfrN4[Task.m_iCnt_Step_AA_Total][i] = mAvrSfrData;
	//		Task.SFR.Sfr_pos[Task.m_iCnt_Step_AA_Total][i].x = mAvrSfrPosX;
	//		Task.SFR.Sfr_pos[Task.m_iCnt_Step_AA_Total][i].y = mAvrSfrPosY;
	//			
	//	}
	//	else
	//	{
	//		nForIndex = sfrIndex[3 + i * 2 - 1];	//4,6,8,10
	//		nBackIndex = sfrIndex[3 + i * 2];		//5,7,9,11
	//		//
	//		mAvrSfrData = (fMax_SFR_N4[nForIndex] + fMax_SFR_N4[nBackIndex]) / 2.0;
	//		mAvrSfrPosX = (((Task.SFR._64_Sfr_Rect[nForIndex].left + Task.SFR._64_Sfr_Rect[nForIndex].right) / 2.0)
	//			+ ((Task.SFR._64_Sfr_Rect[nBackIndex].left + Task.SFR._64_Sfr_Rect[nBackIndex].right) / 2.0)) / 2.0;
	//		mAvrSfrPosY = (((Task.SFR._64_Sfr_Rect[nForIndex].top + Task.SFR._64_Sfr_Rect[nForIndex].bottom) / 2.0)
	//			+ ((Task.SFR._64_Sfr_Rect[nBackIndex].top + Task.SFR._64_Sfr_Rect[nBackIndex].bottom) / 2.0)) / 2.0;
	//		//
	//		Task.SFR.fSfrN4[Task.m_iCnt_Step_AA_Total][i] = mAvrSfrData;
	//		Task.SFR.Sfr_pos[Task.m_iCnt_Step_AA_Total][i].x = mAvrSfrPosX;
	//		Task.SFR.Sfr_pos[Task.m_iCnt_Step_AA_Total][i].y = mAvrSfrPosY;
	//	}
	//	
	//}


#if (____AA_WAY == PCB_TILT_AA)
	for (int i=Motor_PCB_X ; i<= Motor_PCB_Xt; i++)
	{

		if (i == Motor_PCB_Xt || i == Motor_PCB_Yt)
		{
			Task.SFR.fMotorPos[Task.m_iCnt_Step_AA_Total][i] = (float)motor.GetCommandPos(i);
		}else
		{
			Task.SFR.fMotorPos[Task.m_iCnt_Step_AA_Total][i] = (float)motor.GetEncoderPos(i);
		}
	}
#elif (____AA_WAY == LENS_TILT_AA)
	for (int i=Motor_Lens_X ; i<= Motor_Lens_Z; i++)
	{
		if (i == Motor_Lens_Xt || i == Motor_Lens_Yt)
		{
			Task.SFR.fMotorPos[Task.m_iCnt_Step_AA_Total][i] = (float)motor.GetCommandPos(i);
		}else
		{
			Task.SFR.fMotorPos[Task.m_iCnt_Step_AA_Total][i] = (float)motor.GetEncoderPos(i);
		}
	}
#endif
	if (Mode == SFR_FINAL)
	{
		for (int i = 0; i < model.mGlobalSmallChartCount; i++)	//for (int i=0 ; i<MTF_INSP_CNT; i++)
		{
			Task.SFR._64_fSfrN4[i] = fMax_SFR_N4[i];
		}
	}
	if (Mode == SFR_MANUAL)
	{
		if (_calcImageAlignment())
		{
			_stprintf_s(szLog, SIZE_OF_1K, _T("	OC_X: %.3lf, OC_Y: %.3lf"), MandoInspLog.dOCResult[0], MandoInspLog.dOCResult[1]);
			putListLog(szLog);
			//
			_stprintf_s(szLog, SIZE_OF_1K, _T("	OC ÃøÁ¤ = X: %.3lf, Y: %.3lf   Spec(%.03lf, %.03lf)"), Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y, sysData.m_dOcSpec.x, sysData.m_dOcSpec.y);
			putListLog(szLog);
			_stprintf_s(szLog, SIZE_OF_1K, _T("	Rotation = %.3lf"), Task.m_dShift_IMG_TH);
			putListLog(szLog);
		}
	}

	if(!saveSfrLog(Mode))
	{
		return -1;
	} 

	if (Task.SFR.fSfrN4[Task.m_iCnt_Step_AA_Total][0] > model.strInfo_AF1.m_fLimit_MTF || //0.25
		Task.SFR.fSfrN4[Task.m_iCnt_Step_AA_Total][1] > model.strInfo_AF1.m_fLimit_MTF || 
		Task.SFR.fSfrN4[Task.m_iCnt_Step_AA_Total][2] > model.strInfo_AF1.m_fLimit_MTF || 
		Task.SFR.fSfrN4[Task.m_iCnt_Step_AA_Total][3] > model.strInfo_AF1.m_fLimit_MTF || 
		Task.SFR.fSfrN4[Task.m_iCnt_Step_AA_Total][4] > model.strInfo_AF1.m_fLimit_MTF )	/* ¼¾ÅÍ MTF °ªÀÌ ¸®¹ÌÆ® ÀÌ»óÀÌ¸é fine pitch ÀÌµ¿ */
	{
		Task.m_bFlag_Fine_Move = true;
	}
	Task.m_iCnt_Step_AA_Total++;


	
	if ( Mode != SFR_FINAL )
	{
		autodispDlg->DrawGraph(2);		/* ÇöÀç À§Ä¡ SFR µ¥ÀÌÅÍ ±×·¡ÇÁ ±×¸®±â */
	}
	vision.drawOverlay(CCD);
	return 1;
}

int CAABonderDlg::_checkMaxSfrPos(int iMode) 
{
	double dMaxSFR[5] = {-1.0, -1.0, -1.0, -1.0, -1.0};

	int i_data_cnt = 0;
	int i_max_index = 0;

	double d_data_x[5];
	double d_data_y[5];

	double d_calc_A = 0.0;
	double d_calc_B = 0.0;
	double d_calc_C = 0.0;
	double sfrLimit = sysData.dMax_Sfr_Limit;
	if (sfrLimit < 0.01)
	{
		sfrLimit = 0.1;
	}
	if ( Task.m_iCnt_Step_AA < 4 ){
		return 0;
	}
	int i = 0;
	int iStartIndex = 0;

	if ( Task.bFirstAA ){
		iStartIndex = Task.m_iCnt_Second_AA_Start;
	}else if ( Task.m_bFlag_Decrease_SFR == true ){ 
		iStartIndex = 3;
	}

	for (i=0 ; i<5 ; i++)
	{
		for (int j=iStartIndex ; j<Task.m_iCnt_Step_AA_Total ; j++)
		{
			if ( Task.SFR.fSfrN4[j][i] > dMaxSFR[i] )
			{
				dMaxSFR[i] = Task.SFR.fSfrN4[j][i];
				Task.SFR.iMaxIndex[i] = j;
			}
		}

		if ( dMaxSFR[i] < sfrLimit){
			return -1;
		}

		if ( Task.SFR.iMaxIndex[i] > (Task.m_iCnt_Step_AA_Total-3) )
			return -1;


		i_max_index = Task.SFR.iMaxIndex[i];

		if(i_max_index==0)
		{
			i_data_cnt = 1;
			d_data_x[0] = Task.SFR.fMotorPos[0][TITLE_MOTOR_Z];
			d_data_y[0] = Task.SFR.fSfrN4[0][i];
		}
		else if(i_max_index==1)
		{
			if(Task.m_iCnt_Step_AA_Total<4)
			{
				return -1;
			}

			i_data_cnt = 4;
			d_data_x[0] = Task.SFR.fMotorPos[0][TITLE_MOTOR_Z];
			d_data_x[1] = Task.SFR.fMotorPos[1][TITLE_MOTOR_Z];
			d_data_x[2] = Task.SFR.fMotorPos[2][TITLE_MOTOR_Z];
			d_data_x[3] = Task.SFR.fMotorPos[3][TITLE_MOTOR_Z];

			d_data_y[0] = Task.SFR.fSfrN4[0][i];
			d_data_y[1] = Task.SFR.fSfrN4[1][i];
			d_data_y[2] = Task.SFR.fSfrN4[2][i];
			d_data_y[3] = Task.SFR.fSfrN4[3][i];
		}
		else
		{
			i_data_cnt = 3;
			d_data_x[0] = Task.SFR.fMotorPos[i_max_index-1][TITLE_MOTOR_Z];
			d_data_x[1] = Task.SFR.fMotorPos[i_max_index-0][TITLE_MOTOR_Z];
			d_data_x[2] = Task.SFR.fMotorPos[i_max_index+1][TITLE_MOTOR_Z];

			d_data_y[0] = Task.SFR.fSfrN4[i_max_index-1][i];
			d_data_y[1] = Task.SFR.fSfrN4[i_max_index-0][i];
			d_data_y[2] = Task.SFR.fSfrN4[i_max_index+1][i];
		}


		if(i_max_index==0)
		{
			Task.SFR.dMaxPos[i] = Task.SFR.fMotorPos[0][TITLE_MOTOR_Z];
		}
		else if(i_max_index==(Task.m_iCnt_Step_AA_Total-1) )
		{
			Task.SFR.dMaxPos[i] = Task.SFR.fMotorPos[Task.m_iCnt_Step_AA_Total-1][TITLE_MOTOR_Z];
		}
		else
		{
			if(!Alg._calcDoublePeakPos(i_data_cnt, d_data_x, d_data_y, d_calc_A, d_calc_B, d_calc_C))
			{
				putListLog("[Active Align] Peak ¸ðÅÍ À§Ä¡ °è»ê ½ÇÆÐ");
				Task.SFR.dMaxPos[i] = Task.SFR.fMotorPos[Task.SFR.iMaxIndex[i] ][TITLE_MOTOR_Z];
			}
			else
			{
				double d_max_Z_pos = -d_calc_B / (2*d_calc_A);

				if( (Task.SFR.fMotorPos[i_max_index+1][TITLE_MOTOR_Z] < d_max_Z_pos) && (d_max_Z_pos<Task.SFR.fMotorPos[i_max_index-1][TITLE_MOTOR_Z]) )
					Task.SFR.dMaxPos[i] = d_max_Z_pos;
				else
				{
					Task.SFR.dMaxPos[i] = Task.SFR.fMotorPos[i_max_index][TITLE_MOTOR_Z];
					LogSave("ERROR ==========> Æ÷Ä¿½º µ¿ÀÛ ÃÖ¼Ò ÀÚ½Â¹ý °è»ê °á°ú ÀÌ»ó.");
				}
			}
		}
	}
	

	return 1;
}

int	CAABonderDlg::procAutoFocus(int iStep)
{
	int iRtn=0;
	int iRtnFunction = 0;
	CString logStr = _T("");
	int iDelayTime=0;

	bool limitCheckFlag = false;

	double dMovePos=0.0;
 
	double dAutoZ=0.0;

	if (iStep >= 60300 && iStep!=69999)
	{
		if ( gMIUDevice.bMIUOpen == 0 || gMIUDevice.CurrentState != 4 )
		{
			logStr.Format("CCD µ¿¿µ»ó Á¤Áö »óÅÂÀÔ´Ï´Ù.");
			errMsg2(Task.AutoFlag, logStr);
			return iStep;
		}
	}

	if ( Task.m_bFlag_Fine_Move == false )
	{
		iDelayTime = model.strInfo_AF1.m_iDelayTime;
	}
	else
	{
		iDelayTime = model.strInfo_AF2.m_iDelayTime;
	}

	iRtnFunction = iStep;

	switch (iStep)
	{
	case 60000:
		{
			MbufClear(vision.MilOptImage, 0x00);
			MbufClear(vision.MilDefectImage, 0x00);

			Task.initSFRElem();

			Task.PCBTaskTime = myTimer(true);
			 
			autodispDlg->dispCurTaskOnLabel(1, "-");

			autodispDlg->DrawGraph(0);
			autodispDlg->DrawGraph(1);
			MandoInspLog.bInspRes = true;	//60000 ÃÊ±âÈ­
			Task.m_timeChecker.Measure_Time(6);	//AA ½ÃÀÛ ½Ã°£
			iRtnFunction = 60010;
			logStr.Format("	AA Step [%d]" , iRtnFunction);
			putListLog(logStr);
		}
		break;
	case  60010:
		if(bThread_MIUCheckRun == false)		//	CCD ¿µ»ó ¾²·¹µå ¸ØÃß¸é ÁøÇà... step = 12000 Âü°í
		{
			iRtnFunction = 60015;
			logStr.Format("	AA Step [%d]" , iRtnFunction);
			putListLog(logStr);
		}else
		{
			logStr.Format("CCD ¿µ»ó Open ÁøÇàÁßÀÔ´Ï´Ù[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -60010;
		}
		break;
	case  60015:
		if (motor.LENS_Z_Motor_Move(Bonding_Pos))
		{
			logStr.Format("Lens Z Bonding À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iStep);
			putListLog(logStr);
			iRtnFunction = 60020;

		}
		else
		{
			logStr.Format("Lens Z Bonding À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iStep);
			putListLog(logStr);
			iRtnFunction = -60015;
		}
		break;
	case 60020:
		if( !motor.PCB_Z_Motor_Move(Bonding_Pos) )
		{
			logStr.Format("PCB ZÃà Bonding À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -60020;
			break;
		}else 
		{
			iRtnFunction = 60030;
			logStr.Format("PCB ZÃà Bonding À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iRtnFunction);
			putListLog(logStr);
		}
		
		break;
	case 60030:
		{
			double MotorPos[2];

			MotorPos[0] = fabs(motor.GetEncoderPos(TITLE_MOTOR_X) - model.axis[TITLE_MOTOR_X].pos[Bonding_Pos]);
			MotorPos[1] = fabs(motor.GetEncoderPos(TITLE_MOTOR_Y) - model.axis[TITLE_MOTOR_Y].pos[Bonding_Pos]);

			if( (MotorPos[0] < model.axis[TITLE_MOTOR_X].m_dLimit_Err && MotorPos[1] < model.axis[TITLE_MOTOR_Y].m_dLimit_Err) )
			{
				logStr.Format("PCBºÎ x,y º»µù À§Ä¡¿¡ µµ´Þ ¿Ï·á[%d]", iStep);
				putListLog(logStr);
				iRtnFunction = 60040;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
				break;
			}
			else
			{
				logStr.Format("Bonding À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iRtnFunction);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -60030;
				break;
			}
		}
		break;
	
	case 60040:		/* PCB Z¸ðÅÍ º»µùÀ§Ä¡ ÀÌµ¿ */
		{
			iRtn = _checkPcbMotor();
			if ( iRtn > 0 )
			{
				logStr.Format("Z ¸ðÅÍ º»µù À§Ä¡ ÀÌµ¿");
				autodispDlg->dispCurTaskOnLabel(1, logStr);

				setCamDisplay(3, 1);
				double dCurPos = 0.0;
				double dMovePos = model.axis[Motor_Lens_Z].pos[Bonding_Pos];
				motor.MoveAxis(Motor_Lens_Z, ABS, dMovePos, sysData.fMotorSpeed[Motor_Lens_Z], sysData.fMotorAccTime[Motor_Lens_Z]);
				double ep = myTimer(true);

				while (1)
				{
					if ( myTimer(true)-ep < 5000)
					{
						if ( motor.IsStopAxis(Motor_Lens_Z))// && motor.GetInposition(Motor_Lens_Z) )
						{
							dCurPos = motor.GetCommandPos(Motor_Lens_Z);

							if (fabs(dCurPos-dMovePos)<=0.003)
							{
								Task.PCBTaskTime = myTimer(true);

								logStr.Format("Auto Z Pass");
								putListLog(logStr);
								iRtnFunction = 60900;
								logStr.Format("	AA Step [%d]", iRtnFunction);
								putListLog(logStr);
								break;
							}
						}
					}
					else
					{
						putListLog("Auto Focusing ñé LENS Z Motor ÀÌµ¿ ½Ã°£ ÃÊ°ú.");
						iRtnFunction = -60040;
						break;
					}
				}
			}
			else
			{
				putListLog("PCB °¡ º»µùÀ§Ä¡¿¡ µµ´ÞÇÏÁö ¸øÇß½À´Ï´Ù.");
				iRtnFunction = -60040;
			}
		}
		break;
	case 60070:		/* SFR °ªÀÌ ¿¬¼ÓÀ¸·Î ÇÏ¶ôÇÒ ¶§, Z¸ðÅÍ ÀÌµ¿ */
		{
			double dCurPos = 0.0;
			double dMovePos = model.axis[TITLE_MOTOR_Z].pos[Bonding_Pos] + model.m_dMove_Offset_Z;
			//¿¬¼ÓÀ¸·Î ÇÏ¶ô½Ã model.m_dMove_Offset_Z¸¸Å­ zÃàÀ» ¿òÁ÷ÀÎ´Ù 
			if(fabs(model.m_dMove_Offset_Z) > 0.2)
			{
				logStr.Format("zÃà ÀÌµ¿Ä¡ Limit ÃÊ°ú : Z : %.3lf", dMovePos);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -60070;
				break;
			}
			motor.MoveAxis(TITLE_MOTOR_Z, ABS, dMovePos, sysData.fMotorSpeed[TITLE_MOTOR_Z], sysData.fMotorAccTime[TITLE_MOTOR_Z]);
			double ep = myTimer(true);
			Task.m_bFlag_Fine_Move = false;
			while (1)
			{
				if (myTimer(true)-ep< 5000)
				{
					if ( motor.IsStopAxis(TITLE_MOTOR_Z))// && motor.GetInposition(TITLE_MOTOR_Z))
					{
						dCurPos = motor.GetCommandPos(TITLE_MOTOR_Z);
						if (fabs(dCurPos-dMovePos)<=0.003)
						{
							Task.PCBTaskTime = myTimer(true);
							iRtnFunction =  60900;
							logStr.Format("	AA Step [%d]" , iRtnFunction);
							putListLog(logStr);
							break;
						}
					}
				}
				else
				{
					putListLog("Auto Focusing ñé Z Motor ÀÌµ¿ ½Ã°£ ÃÊ°ú.");
					iRtnFunction = -60070;
					break;
				}
			}
		}
		break;

	case 60100:
		{
			double dMoveDist = 0.0;
			if (!Task.m_bFlag_Fine_Move)
			{
				dMoveDist = model.strInfo_AF1.m_dMoveDistance;
				logStr.Format("Through Focusing");
				autodispDlg->dispCurTaskOnLabel(1, logStr);
			}
			else
			{
				dMoveDist = model.strInfo_AF2.m_dMoveDistance;
				logStr.Format("Fine Focusing");
				autodispDlg->dispCurTaskOnLabel(1, logStr);
			}

			iRtn = _moveZMotor(dMoveDist, model.strInfo_AF1.m_dMoveVel);		/* Z¸ðÅÍ Rough ÀÌµ¿ */ 
			if(iRtn > 0){
				iRtnFunction = 61000;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
			}else{
				iRtnFunction = -60100;
			}
			Task.PCBTaskTime = myTimer(true);
		}
		break;
	case 60200:
		{
			if (Task.bFirstAA)
			{
				if(sysData.m_i1stAaEbable)	//1Â÷ AAÈÄ Á¾·á½Ã
				{
					iRtn = _getMTF(SFR_FINAL);

					if(iRtn > 0)
					{
						Task.m_iCnt_Second_AA_Start = Task.m_iCnt_Step_AA_Total;
						Task.m_iDrawBarStep = Task.m_iCnt_Step_AA_Total;
						Task.m_iCnt_Step_AA = 0;

						autodispDlg->DrawBarGraph();
						//MIU.func_Set_InspImageCopy(CHART_RAW);			//1Â÷ AAÈÄ 
						Task.sfrResult = func_MTF(MIU.vChartBuffet);		//1Â÷ AAÈÄ Á¾·á
						if( Task.sfrResult )
						{ 
							logStr.Format("1Â÷ AA °á°ú Spec OK.");
							putListLog(logStr);
							iRtnFunction = 69999;
							logStr.Format("	AA Step [%d]", iRtnFunction);
							putListLog(logStr);
							break;
							////int boxIndex[5] = {SFR_CENTER_UP, MTF_INSP_CNT-8, MTF_INSP_CNT-6, MTF_INSP_CNT-4, MTF_INSP_CNT-2};

							//double maxCornor = Task.SFR._64_fSfrN4[boxIndex[1]];//SFR_LEFT_UP_INDEX
							//double minCornor = Task.SFR._64_fSfrN4[boxIndex[1]];//SFR_LEFT_UP_INDEX

							//for (int i=2; i<5; i++)
							//{
							//	if (maxCornor < Task.SFR._64_fSfrN4[boxIndex[i]])
							//		maxCornor = Task.SFR._64_fSfrN4[boxIndex[i]];
							//	if (minCornor > Task.SFR._64_fSfrN4[boxIndex[i]])
							//		minCornor = Task.SFR._64_fSfrN4[boxIndex[i]];
							//}

							//if( maxCornor-minCornor < sysData.dLimit_SFR_Balance)
							//{
							//	//saveSfrSumLog();//ÃÖÁ¾ Data log ÀúÀå
							//	logStr.Format("1Â÷ AA °á°ú Spec OK.");
							//	putListLog(logStr);
							//	iRtnFunction = 69999;
							//	logStr.Format("	AA Step [%d]" , iRtnFunction);
							//	putListLog(logStr);
							//	break;
							//}
						}
					}
				}
				else
				{
					Task.m_iCnt_Second_AA_Start = Task.m_iCnt_Step_AA_Total;
					Task.m_iCnt_Step_AA = 0;
				}
			}
			double dMovePos=0.0;
#if (____AA_WAY == PCB_TILT_AA)
			dMovePos = 1 * sysData.m_dMove_MaxPos_Fine;
#elif (____AA_WAY == LENS_TILT_AA)
			dMovePos = -1 * sysData.m_dMove_MaxPos_Fine;
#endif	

			/*if( fabs(Task.SFR.dTilt_X)>1 || fabs(Task.SFR.dTilt_Y)>1 )
			{
				logStr.Format("Tx, Ty º¸Á¤°ª 1 ÀÌ»ó - Z 2¹è °Å¸® »ó½Â [Tx %.03f, Ty %.03f]", Task.SFR.dTilt_X, Task.SFR.dTilt_Y );
				dMovePos = -2 * sysData.m_dMove_MaxPos_Fine;
			}*/

			iRtn = _moveZMotor(dMovePos, model.strInfo_AF1.m_dMoveVel);		/* Z¸ðÅÍ Rough ÀÌµ¿ */

			if ( iRtn > 0 )
			{
				iRtnFunction = 60900;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
			}
			else
				iRtnFunction = -60200;
		}

		break;

	case 60900:			/* ZÃà º»µù À§Ä¡ µµÂø ÈÄ */
		{
			if( (myTimer(true) - Task.PCBTaskTime) > model.strInfo_Cam[CCD].m_iDelayTime)
			{
				iRtnFunction = 60910;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
			}
			else
			{
				iRtnFunction = 60900;
				//logStr.Format("	AA Step [%d]" , iRtnFunction);
				//putListLog(logStr);
			}
		}
		break;

	case 60910:	// AAÀü Lens X/Y, PCB Theta º¸Á¤
		
		iRtn = _getMTF(SFR_FIRST/*SFR_FINAL*/);
		if ( iRtn > 0 )
		{
			if ( !_calcImageAlignment() )	
			{
				logStr.Format("NG.  X : %.3lf Y : %.3lf", Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
				putListLog(logStr);
				iRtnFunction = -60910;
			}
			else
			{
				if( fabs(Task.m_dShift_IMG_X) <= sysData.m_dOcSpec.x &&fabs(Task.m_dShift_IMG_Y) <= sysData.m_dOcSpec.y )
				{
					logStr.Format("		 ¿µ»ó Shift. Spec(%.03lf, %.03lf) In : X %.3lf, Y %.3lf", 
						sysData.m_dOcSpec.x, sysData.m_dOcSpec.y,
						Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
					putListLog(logStr);
					iRtnFunction = 62000;
					logStr.Format("	AA Step [%d]" , iRtnFunction);
					putListLog(logStr);
					//Task.ocCheckIng = false; 
					break;
				}
				else
				{
					logStr.Format("		 ¿µ»ó Shift.  X %.3lf, Y %.3lf ,Spec(%.03lf, %.03lf)", Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y, sysData.m_dOcSpec.x, sysData.m_dOcSpec.y);
					putListLog(logStr);
					iRtnFunction = 60920;
					logStr.Format("	AA Step [%d]" , iRtnFunction);
					putListLog(logStr);
					//Task.ocCheckIng = true;
				}
			}
		}
		else
		{
			logStr.Format("SFR ¿µ»ó È¹µæ ½ÇÆÐ.");	//SFR ¿µ»ó È¹µæ ½ÇÆÐ.
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -60910;
		}
		break;

	case 60920:
		if ( !_MotorMove_IMG_Align() )
		{
			iRtnFunction = -60920;
		}
		else
		{
			iRtnFunction = 60930;
			logStr.Format("	AA Step [%d]" , iRtnFunction);
			putListLog(logStr);
		}
		break;

	case 60930:
		if( !_MotorMove_IMG_AlignTheta() ) 
		{
			iRtnFunction = -60930;
		}
		else
		{
			Sleep(200);
			iRtnFunction = 60900;
			logStr.Format("	AA Step [%d]" , iRtnFunction);
			putListLog(logStr);
		}
		break;

	case 61000:
		{
			if( (myTimer(true) - Task.PCBTaskTime) > iDelayTime )
			{
				iRtnFunction = 62000;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
			}
		}

		break;

	case 62000: 
		{
			iRtn = _getMTF(( Task.bFirstAA == true )?SFR_FINAL:SFR_FIRST);
			logStr.Format("Step :[%d] SFR [%.03f] [Z %.03f]", Task.m_iCnt_Step_AA, Task.SFR.fSfrN4[Task.m_iCnt_Step_AA][0], motor.GetEncoderPos((TITLE_MOTOR_Z)));
			putListLog(logStr);
			if (iRtn>0)
			{
				Task.m_iCnt_Step_AA++;		/* Auto Focus rough ÃøÁ¤ È½¼ö Áõ°¡ */

				if (Task.bFirstAA)
				{
					autodispDlg->DrawGraph(1);		/* Z ¸ðÅÍ À§Ä¡º° SFR µ¥ÀÌÅÍ ±×·¡ÇÁ ±×¸®±â */
				}
				else
				{
					autodispDlg->DrawGraph(0);		/* Z ¸ðÅÍ À§Ä¡º° SFR µ¥ÀÌÅÍ ±×·¡ÇÁ ±×¸®±â */
				}

				if ( !Task.bFirstAA == true )
				{
					if ( Task.m_iCnt_Step_AA == model.m_iCnt_Check_SFR )
					{
						iRtnFunction = 63000;
						logStr.Format("	AA Step [%d]" , iRtnFunction);
						putListLog(logStr);
					}
					else if ( Task.m_bFlag_Decrease_SFR == true && Task.m_iCnt_Step_AA == model.m_iCnt_Check_SFR * 2 )
					{
						iRtnFunction = 63000;
						logStr.Format("	AA Step [%d]" , iRtnFunction);
						putListLog(logStr);
					}
					else {
						iRtnFunction = 64000;
						logStr.Format("	AA Step [%d]" , iRtnFunction);
						putListLog(logStr);
					}
				}
				else{
					iRtnFunction = 64000;
					logStr.Format("	AA Step [%d]" , iRtnFunction);
					putListLog(logStr);
				}
				//////////////////////////////////////////////////////////////////////////
			}
			else{
				iRtnFunction = -63000;
			}
		}

		break;

	case 63000:		/* ÃÊ±â 3°³ µ¥ÀÌÅÍ È®ÀÎ ÈÄ °¨¼Ò ÁßÀÌ¸é ZÃà ¸ðÅÍ ÀÌµ¿ */
		{
			iRtn = _checkDecreaseSFR();
			if ( iRtn > 0 ){
				iRtnFunction = 64000;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
			}else
			{
				Task.m_bFlag_Decrease_SFR = true;
				autodispDlg->m_iCur_Index_Through = Task.m_iCnt_Step_AA_Total;

				//autodispDlg->DrawGraph(0);			//------>20180607_1
				//autodispDlg->DrawGraph(1);
				iRtnFunction = 60070;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
			}
		}
		break;

	case 64000:
		{
			iRtn = 1;
			int iRtn2 = 1;

 			iRtn = _checkMaxSfrPos(0);		/* MAX °ª Ã£±â */

			ccdDlg->SetAlignData_Tilt(Task.SFR.dTilt_X, Task.SFR.dTilt_Y);

			if (iRtn>0)			// && iRtn2>0)
			{
				//if (!Task.bFirstAA)
				//{  
				//	Task.SFR.dTilt_Y = _calcTiltX();//*model.Tilt_Weight_Pre;//*modelSign;//modelWeight_Pre;
				//	Task.SFR.dTilt_X = _calcTiltY();//*model.Tilt_Weight_Pre;//*modelSign;//modelWeight_Pre;
				//}else
				//{
				//	Task.SFR.dTilt_Y = _calcTiltX();//*model.Tilt_Weight_Post;//*modelSign;//modelWeight_Post;
				//	Task.SFR.dTilt_X = _calcTiltY();//*model.Tilt_Weight_Post;//*modelSign;//modelWeight_Post;
				//} 
				//
#if 0
				if (Task.bFirstAA == false)
				{
					double dCurPos = motor.GetCommandPos(TITLE_MOTOR_Z);
					double setZ = sysData.m_dOffset_Conti_Down_Z;
					if (true)//setZ < 0)
					{
						double dMovePos = setZ + dCurPos;
						logStr.Format("Z CurPos : (%.3f) , Z down : (%.3f)", dMovePos, setZ);
						putListLog(logStr);
						////
						if (fabs(setZ) > 0.5)
						{
							logStr.Format("Z Down Limit :%.3f [0.5]", fabs(setZ));
							putListLog(logStr);
							errMsg2(Task.AutoFlag, logStr);
							iRtnFunction = -64000;
							break;
						}
						motor.MoveAxis(TITLE_MOTOR_Z, ABS, dMovePos, sysData.fMotorSpeed[TITLE_MOTOR_Z], sysData.fMotorAccTime[TITLE_MOTOR_Z]);
						double ep = myTimer(true);
						while (1)
						{
							if (myTimer(true) - ep < 5000)
							{
								if (motor.IsStopAxis(TITLE_MOTOR_Z))// && motor.GetInposition(TITLE_MOTOR_Z))
								{
									dCurPos = motor.GetEncoderPos(TITLE_MOTOR_Z);
									if (fabs(dCurPos - dMovePos) <= 0.003)
									{
										Task.PCBTaskTime = myTimer(true);
										logStr.Format("Z Down À§Ä¡ ÀÌµ¿ ¿Ï·á");
										putListLog(logStr);
										break;
									}
								}
							}
							else
							{
								logStr.Format("Z Down À§Ä¡ ÀÌµ¿ ½Ã°£ ÃÊ°ú");
								putListLog(logStr);
								iRtnFunction = -64000;
								break;
							}
						}
						Sleep(600);
						//------------------------------------------------------------------------
						dCurPos = motor.GetCommandPos(TITLE_MOTOR_Z);
						setZ = sysData.m_dOffset_Conti_Down_Z * -1; 
						dMovePos = setZ + dCurPos;
						logStr.Format("Z CurPos 2: (%.3f) , Z down : (%.3f)", dMovePos, setZ);
						putListLog(logStr);
						//end 1
						motor.MoveAxis(TITLE_MOTOR_Z, ABS, dMovePos, sysData.fMotorSpeed[TITLE_MOTOR_Z], sysData.fMotorAccTime[TITLE_MOTOR_Z]);
						ep = myTimer(true);
						while (1)
						{
							if (myTimer(true) - ep < 5000)
							{
								if (motor.IsStopAxis(TITLE_MOTOR_Z))// && motor.GetInposition(TITLE_MOTOR_Z))
								{
									dCurPos = motor.GetEncoderPos(TITLE_MOTOR_Z);
									if (fabs(dCurPos - dMovePos) <= 0.003)
									{
										Task.PCBTaskTime = myTimer(true);
										logStr.Format("Z Down Á¤À§Ä¡ ÀÌµ¿ ¿Ï·á");
										putListLog(logStr);
										break;
									}
								}
							}
							else
							{
								logStr.Format("Z Down Á¤À§Ä¡ ÀÌµ¿ ½Ã°£ ÃÊ°ú"); 
								putListLog(logStr);
								iRtnFunction = -64000;
								break;
							}
						}
						//end 2
					}
				}
#endif

				int mDixTx = 1;
				int mDixTy = 1;
				if (model.Tilt_Diretion[2] < 0)
				{
					mDixTx = -1;
				}
				if (model.Tilt_Diretion[3] < 0)
				{
					mDixTy = -1;
				}

				if (sysData.m_iDicChange == 0)
				{
					Task.SFR.dTilt_X = _calcTiltX() *  mDixTx;
					Task.SFR.dTilt_Y = _calcTiltY() *  mDixTy;
				}
				else
				{
					Task.SFR.dTilt_X = _calcTiltY() *  mDixTy;
					Task.SFR.dTilt_Y = _calcTiltX() *  mDixTx;
				}
				logStr.Format("TiltX : %.4f TiltY : %.4f", Task.SFR.dTilt_X, Task.SFR.dTilt_Y);
				putListLog(logStr);

				if (saveSfrLog(SFR_SECOND) == false)
				{
					iRtnFunction = -64000;
				}
				else if ( fabs(Task.SFR.dTilt_X) > model.axis[TITLE_MOTOR_TX].m_dLimit_Err ||
						  fabs(Task.SFR.dTilt_Y) > model.axis[TITLE_MOTOR_TY].m_dLimit_Err	)
				{
					logStr.Format("Tilt º¸Á¤°ª Limit ÃÊ°ú : Tx : %.4f Ty : %.4f", Task.SFR.dTilt_X, Task.SFR.dTilt_Y);
					errMsg2(Task.AutoFlag, logStr);
					iRtnFunction = -64000;
				}
				else
				{
					if (Task.bFirstAA == true)
					{
						//2Â÷
						logStr.Format("	AA Step 2Â÷[%d]", iRtnFunction);
						putListLog(logStr);
					}
					else
					{
						//1Â÷
						logStr.Format("	AA Step 1Â÷[%d]", iRtnFunction);
						putListLog(logStr);
					}
					iRtnFunction = 64200;		/* Focusing ¿Ï·á */	
					logStr.Format("	AA Step [%d]" , iRtnFunction);
					putListLog(logStr);
				}
			}
			else{
				iRtnFunction = 60100;		/* ´Ù½Ã ÃøÁ¤ */
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
			}

			if (Task.m_iCnt_Step_AA>model.strInfo_AF1.m_iStepCnt+5)
			{
				logStr.Format("SFR ÃøÁ¤ ÃÖ´ë È½¼ö ÃÊ°ú.");
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -64000;	/* Ä«¿îÆ® ÃÊ°ú½Ã ¿¡·¯ */
			}

			double z_move_dist = fabs(model.axis[TITLE_MOTOR_Z].pos[Bonding_Pos] - motor.GetCommandPos(TITLE_MOTOR_Z));

			if (z_move_dist > 1.0)//0.5
			{
				logStr.Format("[SFR ÃøÁ¤] Z ¸ðÅÍ ÇÏ°­ °Å¸® ÃÊ°ú (1.0mm ÀÌ»ó ³»¸± ¼ö ¾ø½À´Ï´Ù.)");
				errMsg2(Task.AutoFlag, logStr);

				iRtnFunction = -64000;
			}
		}
		break;

	case 64200:		/* Trough Focus ÈÄ Center SFR °ª MAX À§Ä¡·Î Z¸ðÅÍ ÀÌµ¿ */
		{
			double dCurPos		= 0.0;
			double dMovePos		= Task.SFR.dMaxPos[0];//-3.970

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

			double z_move_dist = 0.0;
			//PCB Z­sÀÌ ¿ò©ÀÌ¹Ç·Î Tiltº¸Á¤½Ã PCB-Z­sÀÌ ÇÏ±ø Á…¾ßÇÔ.
			//
			//2Â÷ ³Ñ¾î°¡¸é¼­ ZÃà ¶ç¿ì´Â ºÎºÐ
			//Lens´Â ZÃà »ó½Â
			//Pcb´Â ZÃà ÇÏ°­
#if (____AA_WAY == PCB_TILT_AA)
			z_move_dist = fabs(model.axis[TITLE_MOTOR_Z].pos[Bonding_Pos] - dMovePos) * -1;	//1Â÷ ->2 Â÷ÀÌµ¿
#elif (____AA_WAY == LENS_TILT_AA)
			z_move_dist = fabs(model.axis[TITLE_MOTOR_Z].pos[Bonding_Pos] - dMovePos) * 1;	//1Â÷ ->2 Â÷ÀÌµ¿
#endif
			if (z_move_dist > 1.0) 
			{
				logStr.Format("[SFR ÃøÁ¤] Z ¸ðÅÍ ÇÏ°­ °Å¸® ÃÊ°ú (1.0mm ÀÌ»ó ³»¸± ¼ö ¾ø½À´Ï´Ù.)");
				errMsg2(Task.AutoFlag, logStr);

				iRtnFunction = -64200;
				break;
			}

			motor.MoveAxis(TITLE_MOTOR_Z, ABS, dMovePos, /*model.strInfo_AF1.m_dMoveVel*/10.000, sysData.fMotorAccTime[TITLE_MOTOR_Z]);

			double ep = myTimer(true);

			while (1)
			{
				if ( myTimer(true)-ep < 5000)
				{
					if ( motor.IsStopAxis(TITLE_MOTOR_Z))// && motor.GetInposition(TITLE_MOTOR_Z) )
					{
						dCurPos = motor.GetCommandPos(TITLE_MOTOR_Z);

						if (fabs(dCurPos-dMovePos)<=0.003)
						{
							iRtnFunction =  64400;
							logStr.Format("	AA Step [%d]" , iRtnFunction);
							putListLog(logStr);
							Task.PCBTaskTime = myTimer(true);

							break;
						}
					}
				}
				else
				{
					putListLog("Auto Focusing ñé LENS Z Motor ÀÌµ¿ ½Ã°£ ÃÊ°ú.");
					errMsg2(Task.AutoFlag, sLangChange);
					iRtnFunction = -64200;
					break;
				}
			}
		}

		break;


	case 64400:
		{
			iRtn = _procTiltX();

			if (iRtn>0){
				iRtnFunction = 64600;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
			}else{
				iRtnFunction = -64400;
			}
		}

		break;

	case 64600:
		{
			iRtn = _procTiltY();

			if (iRtn>0){
				iRtnFunction = 64700;
			}else{
				iRtnFunction = -64600;
			}
		}

		break;

	case 64700:			/* Tilt º¸Á¤ ÈÄ Delay */
		if ( Task.bFirstAA == true )
		{
			if( (myTimer(true) - Task.PCBTaskTime) > model.strInfo_Cam[CCD].m_iDelayTime)
			{
				iRtnFunction = 65000;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
			}
			else
			{
				iRtnFunction = 64700;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
			}
		}
		else
		{
			iRtnFunction = 65000;
			logStr.Format("	AA Step [%d]" , iRtnFunction);
			putListLog(logStr);
		}
		break;

	case 65000:			/* Tilt ÈÄ MTF ÃøÁ¤ */
		{
			//Task.bFirstAA=true;
			if ( Task.bFirstAA == true )//! Active Alignment 2¹øÂ° ¼öÇà 
			{
				Sleep(200);
				iRtn = _getMTF(SFR_FINAL);

				Task.m_iDrawBarStep = Task.m_iCnt_Step_AA_Total;
				autodispDlg->DrawBarGraph();

				if ( iRtn > 0 )
				{
					if (!_calcImageAlignment())
					{//-- Align º¸Á¤ Limit(ÀÓÀÇÀÇ °ª »ç¿ëÇÔ...1.0)
						sLangChange.LoadStringA(IDS_STRING605);	//ImageCenter NG.  X : %.3lf Y : %.3lf
						logStr.Format(sLangChange, Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
						putListLog(logStr);
						iRtnFunction = -65000;
						break;
					}
					else
					{
						if (Task.iMTFCnt < 6)
						{
							if (fabs(Task.m_dShift_IMG_X) > sysData.m_dOcSpec.x &&
								fabs(Task.m_dShift_IMG_Y) > sysData.m_dOcSpec.y)
							{
								logStr.Format("ImageCenter ¿µ»ó Shift.  X %.3lf, Y %.3lf", Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
								putListLog(logStr);
								Sleep(200);
								iRtnFunction = 65500;
								logStr.Format("	AA Step [%d]", iRtnFunction);
								putListLog(logStr);
								break;
							}
							else
							{
								logStr.Format("ImageCenter ¿µ»ó Shift. Spec(%.03lf, %.03lf) In : X %.3lf, Y %.3lf", sysData.m_dOcSpec.x, sysData.m_dOcSpec.y, Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
								putListLog(logStr);
							}
						}
					}

					bool bMtfCheck2 = true;
					vision.MilBufferUpdate();
					Sleep(500);
					MIU.func_Set_InspImageCopy(UV_BEFORE_CHART);			//AA UV BEFORE
					Task.sfrResult = func_MTF( MIU.vChartBuffet);				// [AA UV BEFORE] #1 MIU.m_pFrameRawBuffer);// 
					MandoInspLog.func_LogSave_UVBefore();	// UVÀü Log Data ÀúÀå
				
					autodispDlg->DrawBarGraph();
					if(!Task.sfrResult && bMtfCheck2)
					{
						saveInspImage(AA_NG_IMAGE_SAVE, Task.m_iCnt_Step_AA_Total);
						logStr.Format("UVÀü SFR °Ë»ç NG.  \nAA RETRY ÇÏ½Ã°Ú½À´Ï±î?");//logStr.Format("SFR ÃÖÁ¾ °Ë»ç NG.  \nAA RETRY ÇÏ½Ã°Ú½À´Ï±î?");
						if(askMsg(logStr))
						{
							iRtnFunction = 60000;
							logStr.Format("	AA Step [%d]" , iRtnFunction);
							putListLog(logStr); 
							break;
						}
					}
					
					//saveSfrSumLog();//ÃÖÁ¾ Data log ÀúÀå

					double dCurPosX, dCurPosY, dCurPosTH, dCurPosTX, dCurPosTY, dCurPosZ;
					dCurPosX = motor.GetEncoderPos(Motor_PCB_X);
					dCurPosY = motor.GetEncoderPos(Motor_PCB_Y);
					dCurPosTH = motor.GetCommandPos(Motor_PCB_TH);
					dCurPosTX = motor.GetCommandPos(Motor_PCB_Xt);
					dCurPosTY = motor.GetCommandPos(Motor_PCB_Yt);
					dCurPosZ = motor.GetCommandPos(Motor_PCB_Z);
					if( !g_ADOData.func_AATaskToAAPcbMoter(Task.ChipID, dCurPosX, dCurPosY, dCurPosTH, dCurPosTX, dCurPosTY, dCurPosZ) )
					{
						logStr.Format("DataBase »ý¼º ½ÇÆÐ.[%d]\n MS Office¸¦ ´Ý¾ÆÁÖ¼¼¿ä.", iStep);
						errMsg2(Task.AutoFlag,logStr);
						iRtnFunction = -69999;
						break;
					}
					iRtnFunction = 69999;
					logStr.Format("	AA Step [%d]" , iRtnFunction);
					putListLog(logStr);
				}
				else
				{
					logStr.Format("SFR ÃÖÁ¾ °Ë»ç ½ÇÆÐ.");
					errMsg2(Task.AutoFlag, logStr);
					iRtnFunction = -65000;
				}
			}
			else
			{//! Active Alignment 1¹øÂ° ¼öÇà
				iRtn = _getMTF(SFR_FINAL);
				if ( iRtn > 0 )
				{
					if ( !_calcImageAlignment() )	//¿©±â´Ù //¼öµ¿AA
					{//-- Align º¸Á¤ Limit(ÀÓÀÇÀÇ °ª »ç¿ëÇÔ...1.0)
						sLangChange.LoadStringA(IDS_STRING848);	//NG.  X : %.3lf Y : %.3lf
						logStr.Format(sLangChange, Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
						putListLog(logStr);

						iRtnFunction = -65000;

						break;
					}
					else
					{
						if( fabs(Task.m_dShift_IMG_X) <=sysData.m_dOcSpec.x &&
							fabs(Task.m_dShift_IMG_Y) <=sysData.m_dOcSpec.y)
						{//X/Y º¸Á¤°ªÀÌ Spec InÀÏ °æ¿ì ´ÙÀ½ Step
							sLangChange.LoadStringA(IDS_STRING1302);	//¿µ»ó Shift. Spec(%.03lf, %.03lf) In : X %.3lf, Y %.3lf
							logStr.Format(_T("		 ") + sLangChange, sysData.m_dOcSpec.x, sysData.m_dOcSpec.y, Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
							putListLog(logStr);
							iRtnFunction = 69999;
							logStr.Format("	AA Step [%d]" , iRtnFunction);
							putListLog(logStr);
							break;
						}
						else
						{
							sLangChange.LoadStringA(IDS_STRING1301);	//¿µ»ó Shift.  X %.3lf, Y %.3lf
							logStr.Format(_T("		 ") + sLangChange, Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
							putListLog(logStr);
							iRtnFunction = 65500;
							logStr.Format("	AA Step [%d]" , iRtnFunction);
							putListLog(logStr);
						}
					}
				}
				else
				{
					sLangChange.LoadStringA(IDS_STRING1074);	//SFR ¿µ»ó È¹µæ ½ÇÆÐ.
					errMsg2(Task.AutoFlag,sLangChange);
					iRtnFunction = -65000;
				}
			}
		}

		//iRtnFunction = -65000;
		break;

	case 65500:
		if ( !_MotorMove_IMG_Align()){
			iRtnFunction = -65500;
		}else
		{
			if( Task.iMTFCnt >= 5 )
			{
				if( Task.bFirstAA == true ){
					iRtnFunction = 65520;// 65000;
					logStr.Format("	AA Step [%d]" , iRtnFunction);
					putListLog(logStr);
					Task.PCBTaskTime = myTimer(true);
				}else
				{
					iRtnFunction = 69999;
					logStr.Format("	AA Step [%d]" , iRtnFunction);
					putListLog(logStr);
				}
			} 
			else
			{
				Task.iMTFCnt++;
				iRtnFunction = 65520;// 65000;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
			}
		}
		break;

	case 65520:
		if ((myTimer(true) - Task.PCBTaskTime) > iDelayTime)
		{
			iRtnFunction = 65000;
			logStr.Format("	AA Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		
		break;

	case 69000:								// ÃÖÁ¾ SFR °Ë»ç °á°ú ÀúÀå..
		{									// SFR °Ë»ç NG ¹ß»ý ½Ã 1È¸ Àç°Ë»ç ÈÄ ¾çºÒ ÆÇÁ¤.
			if(myTimer(true)-Task.PCBTaskTime > model.strInfo_Cam[CCD].m_iDelayTime*2 || (myTimer(true)<Task.PCBTaskTime))
			{
				iRtn = _getMTF(SFR_FINAL);		/* MTF°ª È¹µæ */
				 
				Task.m_iDrawBarStep = Task.m_iCnt_Step_AA_Total;
				autodispDlg->DrawBarGraph();
				
				if (iRtn>0)
				{
					if(Task.m_b_AA_Retry_Flag==false)
					{
						Task.m_b_AA_Retry_Flag = true;		// TRUEÀÎ °æ¿ì ´ÙÀ½¹ø¿¡´Â °Ë»ç °á°ú NG ½Ã ¾Ë¶÷ ¹ß»ý..
						iRtnFunction = 60000;				// 60000¿¡¼­ 1, 2Â÷ AA °Ë»ç °á°ú ÃÊ±âÈ­..
						logStr.Format("	AA Step [%d]" , iRtnFunction);
						putListLog(logStr);
					}
					else
					{
						saveInspImage(AA_NG_IMAGE_SAVE, Task.m_iCnt_Step_AA_Total);
						Dio.setAlarm(ALARM_ON);

						logStr.Format("SFR ÃÖÁ¾ °Ë»ç NG. ÀÚµ¿ ÁøÇà ÇÏ½Ã°Ú½À´Ï±î?");
						if(askMsg(sLangChange))
						{
							Dio.setAlarm(ALARM_OFF);
							logStr.Format("NG ¹èÃâ ÇÏ½Ã°Ú½À´Ï±î. (¿¹:NG, ¾Æ´Ï¿À:OK)");
							if (askMsg(logStr)){
								Task.m_bOkFlag = 0;
							}else{
								Task.m_bOkFlag = 1;
							}
							iRtnFunction = 69999;
							logStr.Format("	AA Step [%d]" , iRtnFunction);
							putListLog(logStr);
						}
						else
						{
							Dio.setAlarm(ALARM_OFF);

							//Task.PauseLensStep = abs(Task.LensTask);
							Task.PausePCBStep = abs(Task.PCBTask);
							Task.AutoFlag = 2;		// ÀÏ½ÃÁ¤Áö
							AutoRunView(Task.AutoFlag);
							iRtnFunction = -65000;
						}
					}
				}
				else
				{
					logStr.Format("SFR ÃÖÁ¾ °Ë»ç (2Â÷) ½ÇÆÐ.");
					errMsg2(Task.AutoFlag, sLangChange);
					iRtnFunction = -65000;
				}
			}
		}
		break;
	case 69999:
		{
			double dCurPosX, dCurPosY, dCurPosTH, dCurPosTX, dCurPosTY, dCurPosZ;
			dCurPosX = motor.GetEncoderPos(Motor_PCB_X);
			dCurPosY = motor.GetEncoderPos(Motor_PCB_Y);
			dCurPosTH = motor.GetCommandPos(Motor_PCB_TH);
			dCurPosTX = motor.GetCommandPos(Motor_PCB_Xt);
			dCurPosTY = motor.GetCommandPos(Motor_PCB_Yt);
			dCurPosZ = motor.GetCommandPos(Motor_PCB_Z);
			//
			theApp.MainDlg->putListLog("	");
			logStr.Format("database teaching data save [%s]", Task.ChipID);
			theApp.MainDlg->putListLog("dbData/teachingData");
			theApp.MainDlg->putListLog(logStr);
			logStr.Format("AA x: %lf/%lf", dCurPosX, model.axis[Motor_PCB_X].pos[Bonding_Pos]);
			theApp.MainDlg->putListLog(logStr);
			logStr.Format("AA y: %lf/%lf", dCurPosX, model.axis[Motor_PCB_Y].pos[Bonding_Pos]);
			theApp.MainDlg->putListLog(logStr);
			logStr.Format("AA th: %lf/%lf", dCurPosX, model.axis[Motor_PCB_TH].pos[Bonding_Pos]);
			theApp.MainDlg->putListLog(logStr);
			logStr.Format("AA tx: %lf/%lf", dCurPosX, model.axis[Motor_PCB_Xt].pos[Bonding_Pos]);
			theApp.MainDlg->putListLog(logStr);
			logStr.Format("AA ty: %lf/%lf", dCurPosX, model.axis[Motor_PCB_Yt].pos[Bonding_Pos]);
			theApp.MainDlg->putListLog(logStr);
			logStr.Format("AA z: %lf/%lf", dCurPosX, model.axis[Motor_PCB_Z].pos[Bonding_Pos]);
			theApp.MainDlg->putListLog(logStr);
			//
			if( !g_ADOData.func_AATaskToAAPcbMoter(Task.ChipID, dCurPosX, dCurPosY, dCurPosTH, dCurPosTX, dCurPosTY, dCurPosZ) )
			{
				logStr.Format("DataBase »ý¼º ½ÇÆÐ.[%d]\n MS Office¸¦ ´Ý¾ÆÁÖ¼¼¿ä.", iStep);
				errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = -69999;
				break;
			}
			if (Task.bFirstAA == true)
			{
				Task.m_timeChecker.Measure_Time(7);	//AA ¿Ï·á ½Ã°£
				Task.m_dTime_FineAA = Task.m_timeChecker.m_adTime[5] - Task.m_timeChecker.m_adTime[4];
				dispGrid();

				iRtnFunction = 110000;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
			}
			else
			{
				//procOptical AlingÀ¸·Î...
				//Task.m_timeChecker.Measure_Time(4);
				//Task.m_dTime_ThroughAA = Task.m_timeChecker.m_adTime[4] - Task.m_timeChecker.m_adTime[3];

				iRtnFunction = 80000;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
			}
		}

		break;
	}
	logStr.Empty();
	return iRtnFunction;
}

//--Laser º¯À§ÃøÁ¤ÈÄ Tilting°¢µµ ¾ò±â - 
//-- ¸ðÅÍ ¹æÇâ°ú ¼ø¼­´Â 4»çºÐ¸é ±âÁØÀ¸·Î 2»çºÐ¸é->1»çºÐ¸é->4»çºÐ¸é->3»çºÐ¸éÀ¸·Î °è»ê
bool CAABonderDlg::_calcLaserTilt(CDPoint dMotorPos[4], double dLaser[4], double &TX, double &TY)
{
	CString strLog = "";
	double offsetTx = 1.0;
	double offsetTy = 1.0;
	double tempLaser = 0.0;
	double Length[4];
	// ======================= °è»ê
	//¸ðÅÍ°£ °Å¸®(¹Øº¯)
	Length[0] = abs(dMotorPos[0].y - dMotorPos[3].y);  // left  
	Length[1] = abs(dMotorPos[0].x - dMotorPos[1].x);  // top
	Length[2] = abs(dMotorPos[1].y - dMotorPos[2].y);  // Right 
	Length[3] = abs(dMotorPos[2].x - dMotorPos[3].x);  // bottom
	
	// ======================= X,Y Æò±Õ( Áß½É ·¹ÀÌÀú)
	double AvgTop, AvgBottom;
	double AvgLeft, AvgRight;
	AvgTop		 = (dLaser[0] + dLaser[1]) / 2;
	AvgBottom	 = (dLaser[2] + dLaser[3]) / 2;
	AvgLeft		 = (dLaser[0] + dLaser[3]) / 2;
	AvgRight	 = (dLaser[1] + dLaser[2]) / 2;

	double Height, Width;
	double radian, theta;

	//-------------------------------------------------------
	Width = Length[1];                   // top
	Height = AvgRight - AvgLeft;    //+ - ºÎÈ£ ¾È¸ÂÀ¸¸é ¼ø¼­ ¹Ù²Ù±â
	radian = atan(Height / Width);
	theta = radian * 180 / M_PI;
	//-------------------------------------------------------
	tempLaser = (-theta + Task.oldLaserTy);
	double perValue = 100 * (tempLaser / Task.oldLaserTy);
	if (fabs(perValue) > 100 || Task.oldLaserTy == 0)
	{
		offsetTy = 1.0;//Á¤»ó
	}
	else if (fabs(perValue) > 80)
	{
		offsetTy = 0.8;
	}
	else if (fabs(perValue) > 70)
	{
		offsetTy = 0.7;
	}
	else
	{
		offsetTy = 0.5;
	}
	offsetTy = 0.9;
	TY = -theta *offsetTy;// 0.6;
	strLog.Format("[ty] %lf/ %lf/ %lf/ %lf/ %lf ", -theta, tempLaser,Task.oldLaserTy, offsetTy, perValue);
	putListLog(strLog);
	Task.oldLaserTy = TY;
	// ======================= ¼¼·Î °¢µµ °è»ê (TX)
	Width = Length[0];                   // left

    Height = AvgBottom - AvgTop;
//	Height = AvgBottom - AvgTop;	//+ - ºÎÈ£ ¾È¸ÂÀ¸¸é ¼ø¼­ ¹Ù²Ù±â
	
	radian = atan(Height / Width);
	theta = radian * 180 / M_PI;
	//-------------------------------------------------------
	tempLaser = (-theta + Task.oldLaserTx);
	perValue = 100 * (tempLaser / Task.oldLaserTx);
	if (fabs(perValue) > 100 || Task.oldLaserTx == 0)
	{
		offsetTx = 1.0;//Á¤»ó
	}
	else if (fabs(perValue) > 80)
	{
		offsetTx = 0.8;
	}
	else if (fabs(perValue) > 70)
	{
		offsetTx = 0.7;
	}
	else
	{
		offsetTx = 0.5;
	}
	offsetTx = 0.9;
	TX = -theta *offsetTx;// 0.6;
	strLog.Format("[tx] %lf/ %lf/ %lf/ %lf/ %lf ", -theta, tempLaser,Task.oldLaserTx, offsetTx, perValue);
	putListLog(strLog);
	Task.oldLaserTx = -theta;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////


double CAABonderDlg::_calcTiltX()
{
	double dTopZPos_A = 0.0;
	double dBottomZPos_A = 0.0;

	double dHeight_A = 0.0;
	double dTopPixelPos_A = 0.0;
	double dBottomPixelPos_A = 0.0;

	double dWidth_A = 0.0;

	double dTiltRad_A = 0.0;
	double dTiltDeg_A = 0.0;
	if (model.mGlobalChartType == 0)
	{
		//xÀÚÇü
		dTopZPos_A = (Task.SFR.dMaxPos[1] + Task.SFR.dMaxPos[2]) / 2;
		dBottomZPos_A = (Task.SFR.dMaxPos[3] + Task.SFR.dMaxPos[4]) / 2;

		dHeight_A = dBottomZPos_A - dTopZPos_A;
		dTopPixelPos_A = (Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[1]][1].y + Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[2]][2].y) / 2;
		dBottomPixelPos_A = (Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[3]][3].y + Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[4]][4].y) / 2;

		dWidth_A = (dBottomPixelPos_A - dTopPixelPos_A) * model.m_dSize_CCD_Cell / 1000;	// CCD Cell ºÐÇØ´É

		dTiltRad_A = atan(dHeight_A / dWidth_A);
		dTiltDeg_A = dTiltRad_A * 180.0 / M_PI;
	}
	else
	{
		//+ÀÚÇü
		//  + ÇüÂ÷Æ®·Î Æ¿Æ®°ª °è»ê
		dTopZPos_A = Task.SFR.dMaxPos[1];
		dBottomZPos_A = Task.SFR.dMaxPos[2];

		dHeight_A = dBottomZPos_A - dTopZPos_A;
		dTopPixelPos_A = Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[1]][1].y;
		dBottomPixelPos_A = Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[2]][2].y;

		dWidth_A = (dBottomPixelPos_A - dTopPixelPos_A) * model.m_dSize_CCD_Cell / 1000;	//* CCD Cell ºÐÇØ´É

		dTiltRad_A = atan(dHeight_A / dWidth_A);
		dTiltDeg_A = dTiltRad_A * 180.0 / M_PI;
	}
	return dTiltDeg_A;
}


double CAABonderDlg::_calcTiltY()
{
	double dLeftZPos_A = 0.0;
	double dRightZPos_A = 0.0;

	double dHeight_A = 0.0;

	double dLeftPixelPos_A = 0.0;
	double dRightPixelPos_A = 0.0;

	double dWidth_A = 0.0;

	double dTiltRad_A = 0.0;
	double dTiltDeg_A = 0.0;
	if (model.mGlobalChartType == 0)
	{
		//xÀÚÇü
		dLeftZPos_A = (Task.SFR.dMaxPos[1] + Task.SFR.dMaxPos[3]) / 2;
		dRightZPos_A = (Task.SFR.dMaxPos[2] + Task.SFR.dMaxPos[4]) / 2;

		dHeight_A = dRightZPos_A - dLeftZPos_A;

		dLeftPixelPos_A = (Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[1]][1].x + Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[3]][3].x) / 2;
		dRightPixelPos_A = (Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[2]][2].x + Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[4]][4].x) / 2;

		dWidth_A = (dRightPixelPos_A - dLeftPixelPos_A) * model.m_dSize_CCD_Cell / 1000;		// CCD Cell ºÐÇØ´É

		dTiltRad_A = atan(dHeight_A / dWidth_A);
		dTiltDeg_A = dTiltRad_A * 180.0 / M_PI;
	}
	else
	{
		dLeftZPos_A = Task.SFR.dMaxPos[3];
		dRightZPos_A = Task.SFR.dMaxPos[4];
		dHeight_A = dRightZPos_A - dLeftZPos_A;

		dLeftPixelPos_A = Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[3]][3].x;
		dRightPixelPos_A = Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[4]][4].x;

		dWidth_A = (dRightPixelPos_A - dLeftPixelPos_A) * model.m_dSize_CCD_Cell / 1000;		//* CCD Cell ºÐÇØ´É

		dTiltRad_A = atan(dHeight_A / dWidth_A);
		dTiltDeg_A = dTiltRad_A * 180.0 / M_PI;
	}

	return dTiltDeg_A;
}

int CAABonderDlg::_procTiltY()
{
	if(Task.SFR.dTilt_Y == 0.0f)return 1;
  	CString logStr="";

	/* °è»ê°ª°ú ¸ðÅÍ °°Àº ¹æÇâ ÀÌµ¿ */
	double dCurPos=0.0;
	double dMovePos = 0.0;
#if (____AA_WAY == PCB_TILT_AA)
	dMovePos = motor.GetCommandPos(TITLE_MOTOR_TY) + Task.SFR.dTilt_Y;
#elif (____AA_WAY == LENS_TILT_AA)
	dMovePos = motor.GetEncoderPos(TITLE_MOTOR_TY) + Task.SFR.dTilt_Y;
#endif
	logStr.Format("[procTiltY] TiltY : %.03lf", Task.SFR.dTilt_Y);
	putListLog(logStr);
	
	motor.MoveAxis(TITLE_MOTOR_TY, 1, dMovePos, sysData.fMotorSpeed[TITLE_MOTOR_TY], sysData.fMotorAccTime[TITLE_MOTOR_TY]);

	logStr.Format("[procTiltY] MovePos %.03lf", dMovePos);
	putListLog(logStr);

	double ep = myTimer(true);
	double ep2 = myTimer(true);
	Sleep(100);

	while (1)
	{
		ep2 = myTimer(true);
		if ( ep2-ep < 5000)
		{
#if (____AA_WAY == PCB_TILT_AA)
			logStr.Format("[procTiltY] CurPos %.03lf", motor.GetCommandPos(TITLE_MOTOR_TY));
			putListLog(logStr);
			if ( motor.IsStopAxis(TITLE_MOTOR_TY))
			{
				logStr.Format("[procTiltY] EpTime : %.01lf", myTimer(true)-ep);
				putListLog(logStr);
				dCurPos = motor.GetCommandPos(TITLE_MOTOR_TY);
#elif (____AA_WAY == LENS_TILT_AA)
			logStr.Format("[procTiltY] CurPos %.03lf", motor.GetCommandPos(TITLE_MOTOR_TY));
			putListLog(logStr);
			if ( motor.IsStopAxis(TITLE_MOTOR_TY))
			{
				logStr.Format("[procTiltY] EpTime : %.01lf", myTimer(true)-ep);
				putListLog(logStr);
				dCurPos = motor.GetCommandPos(TITLE_MOTOR_TY);
#endif	

				if (fabs(dCurPos-dMovePos)<=0.01)
				{
					break;
				}
				else
				{
					putListLog("Tilting PCB Yt Motor ÀÌµ¿ ºñÁ¤»ó");
					errMsg2(Task.AutoFlag, sLangChange);
					return -1;
				}
			}
		}
		else
		{
			putListLog("Tilting PCB Yt Motor ÀÌµ¿ ½Ã°£ ÃÊ°ú.");	
			return -1;
		}
	}
	logStr.Empty();
	return 1;
}


int	CAABonderDlg::_procTiltX()
{
  	CString logStr="";

	/* °è»ê°ª°ú ¸ðÅÍ °°Àº ¹æÇâ ÀÌµ¿ */
	double dCurPos = 0.0;
	double dOrgPos = motor.GetCommandPos(TITLE_MOTOR_TX);
	double dMovePos = motor.GetCommandPos(TITLE_MOTOR_TX) + Task.SFR.dTilt_X;

	motor.MoveAxis(TITLE_MOTOR_TX, 1, dMovePos, sysData.fMotorSpeed[TITLE_MOTOR_TX], sysData.fMotorAccTime[TITLE_MOTOR_TX]);

	double ep = myTimer(true);
	Sleep(100);

	while (1)
	{

		if ( myTimer(true)-ep < 5000)
		{

			logStr.Format("[procTiltX] CurPos %.03lf", motor.GetCommandPos(TITLE_MOTOR_TX));
			putListLog(logStr);
			if ( motor.IsStopAxis(TITLE_MOTOR_TX) )
			{

				logStr.Format("[procTiltX] EpTime : %.01lf", myTimer(true)-ep);
				putListLog(logStr);

				dCurPos = motor.GetCommandPos(TITLE_MOTOR_TX);

				if (fabs(dCurPos-dMovePos)<=0.01)
				{
					break;
				}
				else
				{
					logStr.Format("Tilting Xt Motor ÀÌµ¿ ºñÁ¤»ó  (Limit ¼¾¼­ È®ÀÎ).");
					putListLog(logStr);
					errMsg2(Task.AutoFlag, logStr);
					return -1;
				}
			}
		}
		else
		{
			putListLog("Tilting Xt Motor ÀÌµ¿ ½Ã°£ ÃÊ°ú.");
			return -1;
		}
	}
	logStr.Empty();
	return 1;
}


int CAABonderDlg::_moveXYMotor()
{
	char logMsg[1000];
	double eTime;
	short axis[2];
	double pos[2];

	for (int i=Motor_Lens_X; i<=Motor_Lens_Y; i++)
	{
		if(!motor.m_bOrgFlag[i])
		{
			sLangChange.LoadStringA(IDS_STRING725);	//Lens ºÎ X, Y ¸ðÅÍ°¡ ¿øÁ¡ º¹±Í¸¦ ÇÏÁö ¾Ê¾Ò½À´Ï´Ù.
			_stprintf_s(logMsg, _T("%s ") + sLangChange, MotorName[i]);
			errMsg2(Task.AutoFlag,logMsg);
			return false;
		}

		axis[i-Motor_Lens_X]		= i;

		if(i==Motor_Lens_X)					pos[i-Motor_Lens_X]	= motor.GetEncoderPos(Motor_Lens_X) - Task.dOpticalShiftX;
		else if(i==Motor_Lens_Y)			pos[i-Motor_Lens_X]	= motor.GetEncoderPos(Motor_Lens_Y) - Task.dOpticalShiftY;
	}


	int step=0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	double ep;

	while(1)
	{
		switch(step)
		{
		case 0:									// 3Ãà ¸ðÅÍ ´ë±â À§Ä¡·Î ÀÌµ¿ ¸í·É..
			ep = myTimer(true);
			motor.goMotorPos(2, axis, pos);
			step = 100;
			break;

		case 100:								// ÀÌµ¿ ¿Ï·á È®ÀÎ..
			if ( motor.IsStopAxis(Motor_Lens_X) && motor.GetInposition(Motor_Lens_X) )
			{
				step = 101;
			}
			break;

		case 101:
			if ( motor.IsStopAxis(Motor_Lens_Y) && motor.GetInposition(Motor_Lens_Y) )
			{
				step = 1000;
			}
			break;

		default:
			break;
		}

		if(step==1000)
			break;


		eTime = myTimer(true);
		if(eTime-ep>MOTOR_MOVE_TIME)
		{
			motor.StopAxis(Motor_Lens_X);
			motor.StopAxis(Motor_Lens_Y);

			if(step>100)
			{
				int motorNo = step - 100;
				sLangChange.LoadStringA(IDS_STRING723);	//Lens ºÎ [%s ¸ðÅÍ] ÀÌµ¿ ½Ã°£ ÃÊ°ú
				_stprintf_s(logMsg, sLangChange, MotorName[motorNo]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING724);	//Lens ºÎ X, Y ¸ðÅÍ ±¤Ãà º¸Á¤ ÀÌµ¿ ½ÇÆÐ.
				_stprintf_s(logMsg, sLangChange);
			}

			errMsg2(Task.AutoFlag, logMsg);

			return -1;
		}

		Sleep(20);
		checkMessage();
	}

	
	return 1;
}



int CAABonderDlg::procOpticalCheck(int iStep)
{
	int iRtn;
	int iRtnFunction = 0;

	int ocDelayTime = 500;
	CString sLog;


	if ( gMIUDevice.bMIUOpen == 0 || gMIUDevice.CurrentState != 4 )
	{
		sLangChange.LoadStringA(IDS_STRING449);
		errMsg2(Task.AutoFlag, sLangChange);	//CCD µ¿¿µ»ó Á¤Áö »óÅÂÀÔ´Ï´Ù. 3
		return iStep;
	}

	switch (iStep)
	{
	case 130000:		
		{
			if(sysData.m_iAlignMode==MODE_OPTICAL)
			{
				Task.PCBTaskTime = myTimer(true);

				LightControl.ctrlLedVolume(LIGHT_OC_6500K, model.m_iLedValue[LEDDATA_OC]);

				if(Dio.BackLightOn(true, true))		
				{
					sLangChange.LoadStringA(IDS_STRING861);
					sLog.Format(sLangChange, iStep);
					putListLog(sLog);
					iRtnFunction = 130100;

					sLangChange.LoadStringA(IDS_STRING885);
					autodispDlg->dispCurTaskOnLabel(1, sLangChange);
				}
				else
				{
					sLangChange.LoadStringA(IDS_STRING860);
					sLog.Format(sLangChange, iStep);
					putListLog(sLog);
					iRtnFunction = -130000;
				}
			}
			else
			{
				iRtnFunction = 120000;
			}
		}

		break;

	case 130100:
		{
			if ( myTimer(true) - Task.PCBTaskTime > ocDelayTime )
			{
				iRtnFunction = 131000;
			}
			else
			{
				iRtnFunction = 130100;
			}
		}

		break;

	case 131000:
		{
			vision.MilBufferUpdate();

			iRtn = _findOpticalCenter();

			autodispDlg->DispResultImage_Opt();

			if (iRtn>0)
			{
				//saveSfrSumLog();

				if (saveSfrLog(OC_INSP) == true)			// ±¤Ãà Á¤º¸ ÀúÀå
					iRtnFunction = 132000;
				else
					iRtnFunction = -131000;
			}
			else
			{
				iRtnFunction = -131000;

				sLangChange.LoadStringA(IDS_STRING1173);	//±¤Ãà ¼¾ÅÍ¸µ ÃÖÁ¾ °Ë»ç ½ÇÆÐ [%d]
				sLog.Format(sLangChange, iStep);
				errMsg2(Task.AutoFlag, sLog);
			}
		}

		break;

	case 132000:
		{
			int iCnt = 0;
			Dio.BackLightOn(false);		/* Back Light ÈÄÁø */
			LightControl.ctrlLedVolume(LIGHT_OC_6500K, 0);
			while (1)
			{
				if (iCnt<10)
				{
					//if (Dio.BackLightOnCheck(false))		/* Back Light ÈÄÁø Ã¼Å© */
					//{
					//	iRtnFunction = 133000;

					//	sLangChange.LoadStringA(IDS_STRING412);
					//	sLog.Format(sLangChange, iStep);
					//	putListLog(sLog);

					//	Task.PCBTaskTime = myTimer(true);

					//	break;
					//}
				}
				else
				{
					iRtnFunction = -132000;
					break;
				}

				iCnt++;
			}
		}

		break;

	case 133000:
		iRtnFunction = 120000;

		break;


	default:
		sLangChange.LoadStringA(IDS_STRING862);	//OC ÃÖÁ¾ °Ë»ç Step ¹øÈ£°¡ ºñÁ¤»óÀÔ´Ï´Ù. [%d]
		sLog.Format(sLangChange, iStep);
		errMsg2(Task.AutoFlag, sLog);
		break;
	}

	return iRtnFunction;
}


int CAABonderDlg::UV_process(int iStep)
{
	CString sLog = _T("");
	CString sImgPath = _T("");
//	int iRtn;
	int reCnt = 0;
	int iRtnFunction = iStep;
	double curMotorDist[3];

	switch(iStep)
	{
	case 110000:

		iRtnFunction =110500;
		break;

	case 110500:
		if(Bonding_Pos_Ckeck())
		{
			sLog.Format("AA after PCB Z : %f , set Offset Z : %f", motor.GetCommandPos(TITLE_MOTOR_Z),sysData.m_dOffset_Prev_UV_Z);	//AAÈÄ Lens Z : %f , ¼³Á¤ Offset Z : %f
			putListLog(sLog);

			MoveOffset_Prev_UV();	/* PCB X, Y, Z, Tx, Ty, PCB Th UV Àü offset ÀÌµ¿ */
			
			Sleep(200);
			if(motor.IsStopAxis(TITLE_MOTOR_Z))// && motor.GetInposition(TITLE_MOTOR_Z))
			{
				iRtnFunction =110600;
				Task.PCBTaskTime = myTimer(true);
			}
			else
			{
				iRtnFunction =-110500;
			}
		}
		break;

	case 110600:									// 20140612 À©ÅØ Ãß°¡.. UV °æÈ­ Àü SFR ÃøÁ¤.
		{      
			
		//	sLangChange.LoadStringA(IDS_STRING864);	//Offset Z Àû¿ë ÈÄ Lens Z : %f
			//sLog.Format(sLangChange, motor.GetEncoderPos(Motor_Lens_Z));
			//putListLog(sLog);
			Sleep(200);
			saveSfrLog(SFR_OFFSET_DATA);
			_getMTF(SFR_AFTER_OFFSET_MOVE);
			//saveSfrLog(SFR_OFFSET_DATA);
			iRtnFunction =110650;
			Task.PCBTaskTime = myTimer(true);
		}
		break;
		
	case 110650:					//UVÀü MTF °Ë»ç ÁøÇà
		{
			Task.bInsCenter = true;

			_calcImageAlignment();	 
			Task.bInsCenter = false;
			
			iRtnFunction = 110700;	

		}
		break;


	case 110700:	// Lens Z  UV Àü offset ÀÌµ¿ 
// 20150730 HJH Offset Áßº¹ Àû¿ëÀ¸·Î Á¦°Å
//		MoveOffset_Z();	/* Lens Z  UV Àü offset ÀÌµ¿ */

		iRtnFunction =111000;
		Task.PCBTaskTime = myTimer(true);
		break;

	case 111000:	// UV °æÈ­
		{
			sLangChange.LoadStringA(IDS_STRING1122);
			autodispDlg->dispCurTaskOnLabel(1, sLangChange);

			Task.PCBTaskTime = myTimer(true);
			//Task.m_timeChecker.Measure_Time(9);

			if(sysData.m_iUVPass == 1)
			{
				sLog.Format("====UV Pass [%d]====",iStep);
				putListLog(sLog);
				iRtnFunction = 113500;
				break;
			}
			UVCommand.UV_Shutter_Open();
			if (sysData.nUvIndex == 1)
			{
				Sleep(100);
				UVCommand2.UV_Shutter_Open();
			}
			Sleep(10);

			sLog.Format("===> UV ON");
			putListLog(sLog);
			Sleep(model.UV_Time);

			UVCommand.UV_Shutter_Close();
			if (sysData.nUvIndex == 1)
			{
				Sleep(100);
				UVCommand2.UV_Shutter_Close();
			}
			sLog.Format("===> UV OFF [%d]",iStep);

			putListLog(sLog);
			iRtnFunction = 113500;
		}
		break;

	case 113500:
		sLangChange.LoadStringA(IDS_STRING1124);
		autodispDlg->dispCurTaskOnLabel(1, sLangChange);//"UV °æÈ­ ¿Ï·á"
		
		Task.m_timeChecker.Measure_Time(8);	//UV ¿Ï·á ½Ã°£
		Task.m_dTime_UV = Task.m_timeChecker.m_adTime[8] - Task.m_timeChecker.m_adTime[7];


		if(myTimer(true) - Task.PCBTaskTime> 100)  // Delay Ã¼Å©
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 115000;
		}
		break;

	case 115000:
		curMotorDist[0] = fabs(motor.GetEncoderPos(Motor_Lens_X) - model.axis[Motor_Lens_X].pos[Bonding_Pos]);
		curMotorDist[1] = fabs(motor.GetEncoderPos(Motor_Lens_Y) - model.axis[Motor_Lens_Y].pos[Bonding_Pos]);
		curMotorDist[2] = fabs(motor.GetCommandPos(Motor_Lens_Z) - model.axis[Motor_Lens_Z].pos[Bonding_Pos]);

		if((curMotorDist[0]<model.axis[Motor_Lens_X].m_dLimit_Err && curMotorDist[1]<model.axis[Motor_Lens_Y].m_dLimit_Err && curMotorDist[2]<model.axis[Motor_Lens_Z].m_dLimit_Err))
		{
			Task.PcbOnStage = 200;			// °Ë»ç ¿Ï·á.
				
			iRtnFunction = 115300;
			
			//if(!Dio.HolderGrip(false, true))				//Lens Gripper open
			//{
			//	sLog.Format("Lens Grip Open ½ÇÆÐ [%d]", iStep);
			//	errMsg2(Task.AutoFlag, sLog);
			//	iRtnFunction = -115000;
			//}
			//else
			//{
			//	Task.m_bOKLensPass = -1;//¸ÞÀÎ ³Ñ±è ¿Ï·á Lamp Ãë¼Ò
			//	Task.PCBTaskTime = myTimer(true);
			//	iRtnFunction = 115300;
			//}
			
		}

		break;

	case 115300:		
		if(!Dio.LensMotorGrip(false, true))
		{

			sLog.Format("Lens Unit Gripper ÇØÁ¦ ½ÇÆÐ[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = -115300;
			break;
		}
		Task.m_bOKLensPass = -1;//¸ÞÀÎ ³Ñ±è ¿Ï·á Lamp Ãë¼Ò
		Task.m_iFineThCnt = 0;
#if (____AA_WAY == PCB_TILT_AA)
		iRtnFunction = 115400;
#elif (____AA_WAY == LENS_TILT_AA)
		iRtnFunction = 115500;
#endif
		break;
	case 115400:
		iRtnFunction = 115450;
		/*if(Dio.CamLaserSlinderMove(false, true))
		{
 			iRtnFunction = 115450;
		}
		else
		{
			sLog.Format("Laser/Cam ½Ç¸°´õ ÈÄÁø ½ÇÆÐ[%d]", iStep);
			errMsg2(Task.AutoFlag,sLog);
 			iRtnFunction = -115400;
		}*/
		break;
	case 115450:
		iRtnFunction = 115500;
		//if(Dio.CamLaserSlinderCheck(false, false))
		//{
		//	sLog.Format("Laser/Cam ½Ç¸°´õ ÈÄÁø ¿Ï·á[%d]", iStep);
		//	putListLog(sLog);
		//	iRtnFunction = 115500;
		//}
		//else if((myTimer(true) - Task.PCBTaskTime) > 10000)//IO_DELAY_TIME)
		//{
		//	sLog.Format("Laser/Cam ½Ç¸°´õ ÀüÁø ½Ã°£ ÃÊ°ú[%d]", iStep);
		//	errMsg2(Task.AutoFlag,sLog);
		//	iRtnFunction = -115450;
		//}
		break;
	case 115500: //UV¿Ï·áÈÄ ÃÖÁ¾°Ë»ç
		Task.m_bOkFlag = 0;
		work.m_iCnt_Output++;
		work.Save();
		iRtnFunction = 120000;
		_getMTF(SFR_AFTER_UV);  
		break;


	default:
		sLangChange.LoadStringA(IDS_STRING1120);	//UV Process Step ¹øÈ£°¡ ºñÁ¤»óÀÔ´Ï´Ù.
		sLog.Format(sLangChange + _T(" [%d]"), iStep);
		errMsg2(Task.AutoFlag, sLog);
		break;
	}
	sLog.Empty();
	sImgPath.Empty();
	return iRtnFunction;
}

bool CAABonderDlg::Bonding_Pos_Ckeck()
{
	if ( ( motor.IsStopAxis(Motor_Lens_X) && motor.GetInposition(Motor_Lens_X) ) && 
		 ( motor.IsStopAxis(Motor_Lens_Y) && motor.GetInposition(Motor_Lens_Y) ) && 
		 ( motor.IsStopAxis(Motor_Lens_Z)) && //motor.GetInposition(Motor_Lens_Z) ) && 
		 ( motor.IsStopAxis(Motor_Lens_Xt) )&& 
		 ( motor.IsStopAxis(Motor_Lens_Yt) ) && //motor.GetInposition(Motor_Lens_Yt) ) && 
		 ( motor.IsStopAxis(Motor_PCB_X) && motor.GetInposition(Motor_PCB_X) ) && 
		 ( motor.IsStopAxis(Motor_PCB_Y) && motor.GetInposition(Motor_PCB_Y) ) && 
		 ( motor.IsStopAxis(Motor_PCB_TH)) &&// motor.GetInposition(Motor_PCB_TH) ) &&
		 ( motor.IsStopAxis(Motor_PCB_Xt) ) && 
		 ( motor.IsStopAxis(Motor_PCB_Yt) )  )
	{
		return true;
	}
	else
	{
		return false;
	}
}

int	CAABonderDlg::Complete_FinalInsp(int iStep)
{
	CString sLog = _T("");
	//CString sTemp = _T("");
	//CString sNgMsg = _T("");
	//CString sImgPath = _T("");
	//CString logStr = _T("");
	double	dFinalDelay = (double)(model.strInfo_Cam[3].m_iDelayTime);
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;
	int iRtn = 0;
	int iRtnFunction = iStep;
	bool bMtfCheck4 = true;
    switch (iStep)
    {
	case 120000:
		setCamDisplay(3, 1);
		Sleep(dFinalDelay);

		/*sLog.Format("Gen2 Current:%.03lf", model.dGen2Current);
		putListLog(sLog);
		g_clPriInsp.func_insp_CDP800_Current();*/

		iRtnFunction = 121000;
		break;
    case 121000:

        iRtnFunction = 122000;
        break;

    case 122000:	//¿ÏÁ¦Ç° °Ë»ç¸¸ ÇØ´ç (Final °Ë»çÀü X/Y/T º¸Á¤)
        Sleep(dFinalDelay);
        iRtn = _getMTF(SFR_FINAL);
        Sleep(500);
        if (iRtn > 0)
        {
            if (!_calcImageAlignment())	//-- Align º¸Á¤ Limit(ÀÓÀÇÀÇ °ª »ç¿ëÇÔ...1.0)
            {
                sLog.Format("NG.  X : %.3lf Y : %.3lf", Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
                putListLog(sLog);
                iRtnFunction = -122000;
                break;
            }
            else
            {

                //Æ¿ÆÃ°ª ¾È¸ÂÀ» °æ¿ì °ªÀÌ µ¿ÀÏÇÏ°Ô ¹Ýº¹µÊ.. 3È¸ Á¤µµ¸¸ ÇÏ°í ¾È¸ÂÀ¸¸é Spec IN ½ÃÅ°±â.., 
                if (Task.m_iFineThCnt > 3)	Task.m_dShift_IMG_TH = 0.0;
                if (fabs(Task.m_dShift_IMG_TH) <= sysData.dSpec_OC_Theta)//X/Y º¸Á¤°ªÀÌ Spec InÀÏ °æ¿ì ´ÙÀ½ Step
                {
                    sLog.Format("	¿µ»ó Shift. Spec In(%.03lf) In : T: %.3lf", sysData.dSpec_OC_Theta, Task.m_dShift_IMG_TH);
                    putListLog(sLog);
                    iRtnFunction = 122500;
                }
                else
                {
                    sLog.Format("	¿µ»ó Shift(Spec : %.03lf).  T: %.3lf", sysData.dSpec_OC_Theta, Task.m_dShift_IMG_TH);
                    putListLog(sLog);
                    Task.m_iFineThCnt++;
                    iRtnFunction = 122100;
                }
            }
        }
        else
        {
            errMsg2(Task.AutoFlag, "MTF ÃøÁ¤ Àü SFR ¿µ»ó È¹µæ ½ÇÆÐ..");
            iRtnFunction = -122000;
        }
        break;

    case 122100:
        if (!_MotorMove_IMG_AlignTheta())
        {
            iRtnFunction = -122100;
        }
        else
        {
            Sleep(100);
			Task.PCBTaskTime = myTimer(true);
            iRtnFunction = 122200;	//Retry
        }
        break;
	case 122200:
		if ((myTimer(true) - Task.PCBTaskTime) > 300)
		{
			iRtnFunction = 122000;
			sLog.Format("	AA Step [%d]", iRtnFunction);
			putListLog(sLog);
		}
		break;
    case 122500:	//Final Àç °Ë»ç - È®ÀÎ¿ë
		setCamDisplay(3, 1);
		Sleep(dFinalDelay);
		//dms´Â ÀÌ¹ÌÁö µÎÀå
		MIU.func_Set_InspImageCopy(EOL_CHART);	//¿ÏÁ¦Ç° ¸ðµå
		Sleep(300);
		iRtnFunction = 122520;
		break;
	case 122520:
        Task.bInsCenter = true;
        
		_calcImageAlignment();

        Task.bInsCenter = false;

		Task.sfrResult = func_MTF(MIU.vChartBuffet);//EOL È­»ó #1

        if (Task.sfrResult == true)
        {
            sLangChange.LoadStringA(IDS_STRING229);
            putListLog("[°Ë»ç] MTF °Ë»ç ¼º°ø.");// Optic_Axis,
        }
        else
        {
            sLangChange.LoadStringA(IDS_STRING228);
            sLog.Format(sLangChange, iStep);
            putListLog(sLog);
        }
        Task.m_iDrawBarStep = Task.m_iCnt_Step_AA_Total;
        autodispDlg->DrawBarGraph();
        iRtnFunction = 122550;
		break;
    case 122550:
		vision.clearOverlay(CCD);
		vision.drawOverlay(CCD, true);

        iRtnFunction = 122600;
        break;
    case 122600:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCBºÎ zÃà ¸ðÅÍ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -122600;
			break;
		}
        iRtnFunction = 122700;
        break;
    case 122700:
		if (!motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING747);	//LensºÎ zÃà ¸ðÅÍ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -122700;
			break;
		}

        Task.LensTaskTime = myTimer(true);
        //Task.m_iRetry_Opt = 0;
        iRtnFunction = 123000;
        break;
    case 123000:
		if (sysData.m_iDefectInspPass == 1)	// ÀÌ¹°°Ë»ç À¯¹«
		{
			sLog.Format(_T("[PASS] Defect Insp"), iStep);
			putListLog(sLog);
			iRtnFunction = 123400;
		}
		else
		{
			iRtnFunction = 123100;
		}
        break;
	case 123100:
		if (!motor.Pcb_Motor_Move(Dark_Pos))
		{
			sLog.Format("PCBºÎ Dark À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = -123100;
		}
		else
		{
			sLog.Format("PCBºÎ Dark À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = 123200;
		}
		break;
	case 123200:
		if (!motor.PCB_Z_Motor_Move(Dark_Pos))
		{
			sLog.Format("PCBºÎ Dark Z À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = -123200;
			break;
		}
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 123300;
		break;
	case 123300:
		Sleep(100);
		if (motor.IsStopAxis(Motor_PCB_Z))
		{
			Sleep(100);
			MIU.func_Set_InspImageCopy(LOW_LEVEL_RAW);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 123400;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > 10000)
		{
			sLog.Format("PCB Z DAR Á¤ÁöÈ®ÀÎ ½Ã°£ ÃÊ°ú[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -123200;
		}
		break;
	case 123400:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCBºÎ zÃà ¸ðÅÍ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -123400;
			break;
		}
		iRtnFunction = 123500;
		break;
	case 123500:
		if (sysData.m_iStaintInspPass == 1 && sysData.m_iDefectInspPass == 1)
		{
			iRtnFunction = 127000;
			break;
		}
		if (sysData.m_iIrChartUse == 1)
		{
			IrvAlignLed.Irv_Light_SetValue(model.m_iLedValue[LEDDATA_6500K], IR_OC);
		}
		else
		{
			LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, model.m_iLedValue[LEDDATA_6500K]);
		}

		iRtnFunction = 123600;
		break;
    case 123600:

        if (!motor.Pcb_Motor_Move(OC_6500K_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
        {
            sLangChange.LoadStringA(IDS_STRING1023);	//PCBºÎ ¸ðÅÍ Defect °Ë»ç À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
            sLog.Format(sLangChange, iStep);
            putListLog(sLog);
            iRtnFunction = -123600;
        }
        else
        {
            iRtnFunction = 123700;
        }
        break;
    case 123700:
        if (motor.PCB_Z_Motor_Move(OC_6500K_Pos))
        {
            sLog.Format("PCB Z Axis OC 6500K Pos Move Complete[%d]", iStep);
            putListLog(sLog);
            Task.PCBTaskTime = myTimer(true);
            iRtnFunction = 123800;
        }
        else
        {
            sLog.Format("PCB Z Axis OC 6500K Pos Move Fail[%d]", iStep);
            errMsg2(Task.AutoFlag, sLog);
            iRtnFunction = -123700;
        }
        break;
	case 123800:
		if ((myTimer(true) - Task.PCBTaskTime) > 500)
		{
			iRtnFunction = 123850;
		}
		break;
	case 123850:
        if (motor.IsStopAxis(Motor_PCB_Z))
        {
            Task.PCBTaskTime = myTimer(true);

            //6500k ÀÌ¹ÌÁö ÀúÀå
			Sleep(300);
			MIU.func_Set_InspImageCopy(MID_6500K_RAW);
            //ccdDlg->m_pSFRDlg->Raw_ImageSave(MIU.m_pFrameRawBuffer, MID_6500K_RAW);
            //MID_6500K_RAW ÀÌ¹ÌÁö ÀúÀå

            iRtnFunction = 123900;
        }
        else
        {
            sLog.Format("PCB Z Axis OC 6500K Pos Move Check Fail[%d]", iStep);
            errMsg2(Task.AutoFlag, sLog);
            iRtnFunction = -123700;
        }
        break;
    case 123900:
		Sleep(dFinalDelay);
		if (sysData.m_iDefectInspPass == 1)	// ÀÌ¹°°Ë»ç À¯¹«
		{
			sLog.Format(_T("[PASS] Defect Insp"), iStep); 
			putListLog(sLog);
		}
		else
		{
			if (g_clPriInsp.func_Insp_Defect(MIU.vDefectMidBuffer_6500K, MIU.vDefectLowBuffer, true) == true)	//EOL
			{
				putListLog("[°Ë»ç] Defect °Ë»ç ¼º°ø.");
			}
			else
			{
				sLog.Format("[°Ë»ç] Defect °Ë»ç  Error[%d]", iStep);
				putListLog(sLog);
				MandoInspLog.bInspRes = false;
			}
		}
		if (sysData.m_iStaintInspPass == 1)	// ÀÌ¹°°Ë»ç À¯¹«
		{
			sLog.Format(_T("[PASS] Stain Insp"), iStep);
			putListLog(sLog);

		}
		else
		{
			if (g_clPriInsp.func_Insp_Stain(MIU.vDefectMidBuffer_6500K, false) == true)
			{
				putListLog("[°Ë»ç] Stain °Ë»ç ¼º°ø.");
			}
			else
			{
				sLog.Format("[°Ë»ç] Stain °Ë»ç  Error[%d]", iStep);
				putListLog(sLog);
				MandoInspLog.bInspRes = false;
			}
		}
        iRtnFunction = 127000;
        break;
    case 127000:
        if (motor.PCB_Z_Motor_Move(Wait_Pos))
        {
            sLog.Format("PCB ZÃà ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iStep);
            putListLog(sLog);
            iRtnFunction = 127600;
        }
        else
        {
            sLog.Format("PCB ZÃà ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
            sLog.Format(sLangChange, iStep);
            errMsg2(Task.AutoFlag, sLog);
            iRtnFunction = -127000;
        }
        break;
   
	case 127600:
		Task.m_iRetry_Opt = 0; 
		iRtnFunction = 127700;
		break;
	case 127700:
		//
		if (MIU.Close())
		{
			iRtnFunction = 127800;
		}
		else
		{
			iRtnFunction = 127700;

			if (Task.m_iRetry_Opt>3)
			{
				sLog.Format("CCD ¿µ»ó Close ½ÇÆÐ [%d]", iStep);	//CCD ¿µ»ó Close ½ÇÆÐ[%d]
				errMsg2(Task.AutoFlag, sLog);
				iRtnFunction = -127700;
			}
			else {
				Task.m_iRetry_Opt++;
			}
		}
		break;
	case 127800:

		iRtnFunction = 127900;
		break;
    case 127900:
        //MIU.m_acsInspImage.Unlock();
        Task.m_iRetry_Opt = 0;
        MandoInspLog.func_LogSave_UVAfter();	//¸¸µµ Â÷·®¿ëCamera °Ë»ç Log ÀúÀå(°Ë»ç/º¸Á¤·® ÀúÀå -> ÃÖÁ¾Log)
        //! °Ë»ç Count Ã³¸®
        g_FinalInspLog();
		//inspMES();
		//MESCommunication.Messave();
        work.iInspCnt_Output++;
        if (MandoInspLog.bInspRes == false)	work.iInspCnt_NG_Output++;
        dispInspResGrid();
        work.Save();
        work.Load();

        Task.m_timeChecker.Measure_Time(13);	//Final °Ë»ç ¿Ï·á ½Ã°£
        Task.m_dTime_TotalAA = Task.m_timeChecker.m_adTime[13] - Task.m_timeChecker.m_adTime[12];
        dispGrid();

        iRtnFunction = 128000;
        break;
    case 128000:
        Sleep(100);
        vision.clearOverlay(CCD);
        if (MandoInspLog.bInspRes == true)
        {
            vision.textlist[CCD].addList(50, 100, "PASS", M_COLOR_GREEN, 65, 85, "Arial");
        }
        else
        {
            vision.textlist[CCD].addList(50, 100, "FAIL", M_COLOR_RED, 80, 100, "Arial");
            MandoInspLog.func_DrawNG_Overlay(CCD);	//È­¸é Overlay¿¡ NG List Draw
            work.m_iCnt_NG_Output++;
        }
        vision.drawOverlay(CCD, true);

        //ccd¿µ»ó ÀüÈ¯

        ctrlSubDlg(MAIN_DLG);
        setCamDisplay(3, 1);
        changeMainBtnColor(MAIN_DLG);
        iRtnFunction = 128100;
        break;

    case 128100:
		iRtnFunction = 129200;

        break;
    case 129200:
        if (!motor.Lens_Motor_MoveXY(Wait_Pos))
        {
            sLangChange.LoadStringA(IDS_STRING748);	//LensºÎ ¸ðÅÍ ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
            sLog.Format(sLangChange, iStep);
            putListLog(sLog);
            iRtnFunction = -129200;
        }
        else
        {
            iRtnFunction = 129220;
        }
        break;

	case 129220:
		
		iRtnFunction = 129300;
		break;
    case 129300:
        if (motor.Pcb_Motor_Move(Wait_Pos))
        {
            sLangChange.LoadStringA(IDS_STRING987);	//PCB ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á [%d]
            sLog.Format(sLangChange, iStep);
            putListLog(sLog);
            iRtnFunction = 129350;
        }
        else
        {
            sLangChange.LoadStringA(IDS_STRING986);	//PCB ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]
            sLog.Format(sLangChange, iStep);
            errMsg2(Task.AutoFlag, sLog);
            iRtnFunction = -129300;
        }
        
        break;
    case 129350:
        //Dio.PcbGrip(false, false);
        iRtnFunction = 129400;
        break;
    case 129400:
        if (motor.Pcb_Tilt_Motor_Move(Wait_Pos))
        {
            sLog.Format("PCB TX,TY ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á [%d]", iStep);
            putListLog(sLog);
            iRtnFunction = 129500;
        }
        else
        {
            sLog.Format("PCB TX,TY ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
            errMsg2(Task.AutoFlag, sLog);
            iRtnFunction = -129400;
        }
        break;

    case 129500:
        Task.m_bOkDispense = -1;	//¸ÞÀÎ µµÆ÷ ¿Ï·á Lamp Ãë¼Ò
        putListLog("		");
        putListLog("		");
        sLog.Format("			END =======%s", Task.ChipID);
        putListLog(sLog);
        putListLog("		");
        putListLog("		");
        Task.m_bPBStart = 0;	//¹ÙÄÚµå Á¤º¸ ÃÊ±âÈ­
        if (true)
        {
            memset(Task.ChipID, 0x00, 256);
            sprintf_s(Task.ChipID, "EMPTY");
            func_ChipID_Draw();
        }

        Task.m_timeChecker.Measure_Time(14);	//¿ÏÁ¦Ç° ¹èÃâ ½Ã°£
        Task.m_dTime_Total = Task.m_timeChecker.m_adTime[14] - Task.m_timeChecker.m_adTime[1];

        dispGrid();
        EpoxyTimeCheck(bEpoxyTimeChk);
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        Task.PCBTask = 10000;
        Task.LensTask = 10000;
        iRtnFunction = 10000;
        break;
    default:
        sLog.Format("Mando Test Process Step Number Error. [%d]", iStep);
        errMsg2(Task.AutoFlag, sLog);
        iRtnFunction = -iStep;
        break;
    }
    return iRtnFunction;
}
int	CAABonderDlg::func_MandoFinalSFR(int iStep)
{
	CString sLog = _T("");

	double	dFinalDelay = (double)(model.strInfo_Cam[3].m_iDelayTime);
	double offsetX = 0.0;
	double offsetY = 0.0;
	double offsetTh = 0.0;
	int iRtn = 0;
	int iRtnFunction = iStep;
	bool bUseDispense = true;	//µµÆ÷ Pass ¿©ºÎ
    
	if( (sysData.m_iDispensePass == 1) )
	{
		bUseDispense = false;//µµÆ÷ Pass Mode ÀÌ°Å³ª, µµÆ÷ ¿Ï·á ÀÏ °æ¿ì
	}
	else
	{
		bUseDispense = true;	
	}

	switch(iStep)
	{
	case 120000:

		iRtnFunction = 121000;
		break;

	case 121000:
		
		iRtnFunction = 122000;
		break;
	case 122000:	//¿ÏÁ¦Ç° °Ë»ç¸¸ ÇØ´ç (Final °Ë»çÀü X/Y/T º¸Á¤)
		Sleep(dFinalDelay);
		iRtn = _getMTF(SFR_FINAL);
		if ( iRtn > 0 )
		{
			if ( !_calcImageAlignment())	//-- Align º¸Á¤ Limit(ÀÓÀÇÀÇ °ª »ç¿ëÇÔ...1.0)
			{
				sLog.Format("NG.  X : %.3lf Y : %.3lf", Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
				putListLog(sLog);
				iRtnFunction = -122000;
				break;
			}
			else
			{
				//Æ¿ÆÃ°ª ¾È¸ÂÀ» °æ¿ì °ªÀÌ µ¿ÀÏÇÏ°Ô ¹Ýº¹µÊ.. 3È¸ Á¤µµ¸¸ ÇÏ°í ¾È¸ÂÀ¸¸é Spec IN ½ÃÅ°±â..,
				if( Task.m_iFineThCnt > 5)	Task.m_dShift_IMG_TH = 0.0;	
				if( fabs(Task.m_dShift_IMG_TH) <= sysData.dSpec_OC_Theta )//X/Y º¸Á¤°ªÀÌ Spec InÀÏ °æ¿ì ´ÙÀ½ Step
				{
					sLog.Format("	¿µ»ó Shift. Spec In(%.03lf) In : T: %.3lf",sysData.dSpec_OC_Theta, Task.m_dShift_IMG_TH);
					putListLog(sLog);
					iRtnFunction = 122500;
				}
				else
				{
					sLog.Format("	¿µ»ó Shift(Spec : %.03lf).  T: %.3lf",sysData.dSpec_OC_Theta, Task.m_dShift_IMG_TH);
					putListLog(sLog);
					if( bUseDispense == false ) //AAB, µµÆ÷ ¹Ì»ç¿ëÀÏ °æ¿ì->¿µ»ó Shift¾ÈÇÏ°í ÁøÇà
					{
						iRtnFunction = 122500;
					}
					else//¿µ»ó Shift ÁøÇà
					{
						Task.m_iFineThCnt++;
						iRtnFunction = 122100;
					}
				}
			}
		}
		else
		{
			if (askMsg("ÁøÇà?") == IDOK)
			{
				iRtnFunction = 122500;
				break;
			}
			else
			{
				errMsg2(Task.AutoFlag, "MTF ÃøÁ¤ Àü SFR ¿µ»ó È¹µæ ½ÇÆÐ..");
				iRtnFunction = -122000;
			}
		}
		break;

	case 122100:
		if ( !_MotorMove_IMG_AlignTheta() )
		{
			iRtnFunction = -122100;
		}
		else
		{
			Sleep(100);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 122200;	//Retry
		}
		break;
	case 122200:
		if ((myTimer(true) - Task.PCBTaskTime) > 300)
		{
			iRtnFunction = 122000;
			sLog.Format("	AA Step [%d]", iRtnFunction);
			putListLog(sLog);
		}
		break;
	case 122500:	//Final Àç °Ë»ç - È®ÀÎ¿ë
		setCamDisplay(3, 1);

		Sleep(dFinalDelay);
		MIU.func_Set_InspImageCopy(UV_AFTER_CHART);		//AA UV AFTER
		Sleep(300);
		iRtnFunction = 122520;
		break;
	case 122520:
		
		Task.bInsCenter = true;

		_calcImageAlignment();
			
		Task.bInsCenter = false;
			
		Task.sfrResult = func_MTF(MIU.vChartBuffet);			// [AA UV AFTER] #1

		if(Task.sfrResult == true)
		{
			putListLog("[°Ë»ç] MTF °Ë»ç ¼º°ø.");// Optic_Axis,
		}
		else
		{
			putListLog("[°Ë»ç] MTF °Ë»ç ½ÇÆÐ.");// Optic_Axis,
		}
		Task.m_iDrawBarStep = Task.m_iCnt_Step_AA_Total;
		autodispDlg->DrawBarGraph();
		iRtnFunction = 122550;

		break;
	case 122550:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCBºÎ zÃà ¸ðÅÍ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -122550;
			break;
		}

		if (!motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING747);	//LensºÎ zÃà ¸ðÅÍ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -122550;
		}

		

		iRtnFunction = 122700;
		break;
	case 122700:
		Task.LensTaskTime = myTimer(true);
		Task.m_iRetry_Opt = 0;
		iRtnFunction = 122800;
		break;
	case 122800:
		if (sysData.m_iDefectInspPass == 1)	// ÀÌ¹°°Ë»ç À¯¹«
		{
			sLog.Format(_T("[PASS] Defect Insp"), iStep);
			putListLog(sLog);
			iRtnFunction = 123000;
		}
		else
		{
			iRtnFunction = 122850;
		}
		break;
	case 122850:
		if (!motor.Pcb_Motor_Move(Dark_Pos))
		{
			sLog.Format("PCBºÎ Dark À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = -122850;
		}
		else
		{
			sLog.Format("PCBºÎ Dark À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = 122900;
		}
		
		break;
	case 122900:
		if (!motor.PCB_Z_Motor_Move(Dark_Pos))
		{
			sLog.Format("PCBºÎ Dark Z À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = -122900;
			break;
		}
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 122950;
		break;
	case 122950:
		if ((myTimer(true) - Task.PCBTaskTime) > 500)
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 122960;
		}
		break;
	case 122960:
		Sleep(100);
		if (motor.IsStopAxis(Motor_PCB_Z))
		{
			MIU.func_Set_InspImageCopy(LOW_LEVEL_RAW);
			iRtnFunction = 122970;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > 10000)
		{
			sLog.Format("PCB Z DAR Á¤ÁöÈ®ÀÎ ½Ã°£ ÃÊ°ú[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -122900;
		}
		
		break;
	case 122970:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCBºÎ zÃà ¸ðÅÍ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -122970;
			break;
		}
		iRtnFunction = 123000;
		break;
	case 123000:
		if (sysData.m_iDefectInspPass == 1 && sysData.m_iStaintInspPass == 1)
		{
			iRtnFunction = 124500;
			break;
		}
		if (sysData.m_iIrChartUse == 1)
		{
			IrvAlignLed.Irv_Light_SetValue(model.m_iLedValue[LEDDATA_6500K], IR_OC);
		}
		else
		{
			LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, model.m_iLedValue[LEDDATA_6500K]);
		}
		iRtnFunction = 123050;
		
		break; 
	case 123050:
		if( !motor.Pcb_Motor_Move(OC_6500K_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]) )
		{
			sLangChange.LoadStringA(IDS_STRING1023);	//PCBºÎ ¸ðÅÍ Defect °Ë»ç À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -123050;
		}
		else
		{
			iRtnFunction = 123100;
		}
		break;
	case 123100:
		if( motor.PCB_Z_Motor_Move(OC_6500K_Pos) )
		{
			sLog.Format("PCB Z Axis OC 6500K Pos Move Complete[%d]", iStep);
			putListLog(sLog);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 123200;
		}
		else
		{
			sLog.Format("PCB Z Axis OC 6500K Pos Move Fail[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -123100;
		}
		break;
	case 123200:
		if ((myTimer(true) - Task.PCBTaskTime) > 500)
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 123400;
		}
		break;
	case 123400:
		if(motor.IsStopAxis(Motor_PCB_Z))
		{
			Sleep(100);
			MIU.func_Set_InspImageCopy(MID_6500K_RAW);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 124000;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > 10000)
		{
			sLog.Format("PCB Z 6500K Á¤ÁöÈ®ÀÎ ½Ã°£ ÃÊ°ú[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -123100;
		}
		break;
	case 124000:
		Sleep(dFinalDelay);
		if (sysData.m_iDefectInspPass == 1)	// ÀÌ¹°°Ë»ç À¯¹«
		{
			sLog.Format(_T("[PASS] Defect Insp"), iStep);
			putListLog(sLog);
		}
		else
		{
			if (g_clPriInsp.func_Insp_Defect(MIU.vDefectMidBuffer_6500K, MIU.vDefectLowBuffer, true) == true)	//AA
			{
				putListLog("[°Ë»ç] Defect °Ë»ç ¼º°ø.");
			}
			else
			{
				sLog.Format("[°Ë»ç] Defect °Ë»ç  Error[%d]", iStep);
				putListLog(sLog);
				MandoInspLog.bInspRes = false;
			}
		}
		if (sysData.m_iStaintInspPass == 1)	// ÀÌ¹°°Ë»ç À¯¹«
		{
			sLog.Format(_T("[PASS] Stain Insp"), iStep);
			putListLog(sLog);
			
		}
		else
		{
			if (g_clPriInsp.func_Insp_Stain(MIU.vDefectMidBuffer_6500K, false) == true)
			{
				putListLog("[°Ë»ç] Stain °Ë»ç ¼º°ø.");
			}
			else
			{
				sLog.Format("[°Ë»ç] Stain °Ë»ç  Error[%d]", iStep);
				putListLog(sLog);
				MandoInspLog.bInspRes = false;
			}
		}
       
		iRtnFunction = 124500;
		break;
	case 124500:
		
		if (MIU.Close())//if(MIU.Stop())
		{
			iRtnFunction = 125000;
		}
		else
		{
			iRtnFunction = 124500;

			if(Task.m_iRetry_Opt>3)
			{
				sLog.Format("CCD ¿µ»ó Close ½ÇÆÐ [%d]", iStep);	//CCD ¿µ»ó Close ½ÇÆÐ[%d]
				errMsg2(Task.AutoFlag, sLog);
				iRtnFunction = -124500;
			}
			else{
				Task.m_iRetry_Opt++;
			}
		}
		break;
	case 125000: 
		if(motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLog.Format("PCB ZÃà ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = 127000;
		}
		else
		{
			sLog.Format("PCB ZÃà ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
			sLog.Format(sLangChange, iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -125000;
		}
		break;
	case 127000:
		//MIU.m_acsInspImage.Unlock();
		Task.m_iRetry_Opt = 0; 
		MandoInspLog.func_LogSave_UVAfter();	//¸¸µµ Â÷·®¿ëCamera °Ë»ç Log ÀúÀå(°Ë»ç/º¸Á¤·® ÀúÀå -> ÃÖÁ¾Log)
		/*inspMES();
		MESCommunication.Messave();*/
		//! °Ë»ç Count Ã³¸®
		work.iInspCnt_Output++;	
		if(MandoInspLog.bInspRes == false)	work.iInspCnt_NG_Output++;
		dispInspResGrid();
		work.Save();
		work.Load();

		Task.m_timeChecker.Measure_Time(13);	//Final °Ë»ç ¿Ï·á ½Ã°£
		Task.m_dTime_TotalAA = Task.m_timeChecker.m_adTime[13] - Task.m_timeChecker.m_adTime[12];
		dispGrid();

		iRtnFunction = 127300;
		break;
	case 127300:
		Sleep(100);
		vision.clearOverlay(CCD);
		if(MandoInspLog.bInspRes == true)
		{
			vision.textlist[CCD].addList(50, 100, "PASS", M_COLOR_GREEN, 65, 85, "Arial");
		}
		else
		{
			vision.textlist[CCD].addList(50, 100, "FAIL", M_COLOR_RED, 80, 100, "Arial");
			MandoInspLog.func_DrawNG_Overlay(CCD);	//È­¸é Overlay¿¡ NG List Draw
			work.m_iCnt_NG_Output++;
		}
		vision.drawOverlay(CCD, true);

		//ccd¿µ»ó ÀüÈ¯

		ctrlSubDlg(MAIN_DLG);
		setCamDisplay(3, 1);
		changeMainBtnColor(MAIN_DLG);
		iRtnFunction = 127400;
		break;

	case 127400:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLog.Format("PCB ZÃà ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
			putListLog(sLog);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -127400;
			break;
		}
		iRtnFunction = 127500;
		break;
	case 127500:
		if(motor.Pcb_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING987);	//PCB ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á [%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = 127900;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING986);	//PCB ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]
			sLog.Format(sLangChange, iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -127500;
		}
		break;
	case 127900:

		//////Dio.PcbGrip(false, false);   //¾ðÁ¦»ç¿ë?
		
		iRtnFunction = 128000;
		break;
	case 128000:
		if(!motor.Lens_Motor_MoveXY(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING748);	//LensºÎ ¸ðÅÍ ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -128000;
		}else
		{
			iRtnFunction = 128200;
		}
		break;
	case 128200:
		if( motor.Lens_Tilt_Motor_Move(Wait_Pos))
		{
			iRtnFunction = 128500;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING745);	//LENSºÎ Tx, TyÃà ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ
			sLog.Format(sLangChange);
			putListLog(sLog);	
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -128200;
		}
		break;
	case 128500:
		if( motor.Pcb_Tilt_Motor_Move(Wait_Pos))
		{
			sLog.Format("PCB TX,TY ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á [%d]", iStep);
			putListLog(sLog);
			iRtnFunction = 129000;
		}
		else
		{
			sLog.Format("PCB TX,TY ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -128500;
		}
		break;
	case 129000:
		iRtnFunction = 129500;
		break;
	case 129500:
		Task.m_bOkDispense = -1;	//¸ÞÀÎ µµÆ÷ ¿Ï·á Lamp Ãë¼Ò
		putListLog("		");
		putListLog("		");
		sLog.Format("			END =======%s" , Task.ChipID);
		putListLog(sLog);
		putListLog("		");
		putListLog("		");
		putListLog("		");
		putListLog("		");
		Task.m_bPBStart = 0;	//¹ÙÄÚµå Á¤º¸ ÃÊ±âÈ­
		if(true)
		{
			memset(Task.ChipID, 0x00, 256);
			sprintf_s(Task.ChipID, "EMPTY");
			func_ChipID_Draw();
		}

		Task.m_timeChecker.Measure_Time(14);	//¿ÏÁ¦Ç° ¹èÃâ ½Ã°£
		Task.m_dTime_Total = Task.m_timeChecker.m_adTime[14] - Task.m_timeChecker.m_adTime[1];

		dispGrid();
		EpoxyTimeCheck(bEpoxyTimeChk);

		//Dio.PCBvaccumOn(VACCUM_OFF, false);

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Task.PCBTask = 10000;
		Task.LensTask = 10000;
		iRtnFunction = 10000;
		break;
	
	default:
		sLog.Format("Mando Test Process Step Number Error. [%d]", iStep);
		errMsg2(Task.AutoFlag, sLog);
		iRtnFunction =  -iStep;
		break;
	}
	sLog.Empty();
	return iRtnFunction;
}



int CAABonderDlg::Ready_process(int iStep)
{
	CString sLog="";
 	int iRtnFunction = iStep;

	switch(iStep)
	{
	case 10000:
		Task.PCBTaskTime = myTimer(true);
		vision.clearOverlay(CCD);
		
		sLog.Format("		Ready Step [%d]" , iRtnFunction);
		putListLog(sLog);

		iRtnFunction = 10150;
		break;
	

	case  10150:
		if( Dio.LensMotorGripCheck(true, false))
		{
			if(Task.m_bOKLensPass==1)   //Lens ³Ñ±è ¿Ï·á»óÅÂ
			{ 
				iRtnFunction = 10160;
				sLog.Format("		Ready Step [%d]" , iRtnFunction);
				putListLog(sLog);
			}else
			{
				sLog.Format("[¿îÀü ÁØºñ] Lens Grip ÀüÁø »óÅÂ ÀÔ´Ï´Ù[%d]\n·»Áî³Ñ±è»óÅÂ È®ÀÎ¹Ù¶ø´Ï´Ù", iStep);
				errMsg2(Task.AutoFlag, sLog);
				iRtnFunction = -10150;
				break;
			}
		}else
		{
			if(!Dio.LensMotorGripCheck(false, false))
			{
				sLog.Format("[¿îÀü ÁØºñ]Lens Grip ÀüÁø È®ÀÎ ½ÇÆÐ [%d]", iStep);
				errMsg2(Task.AutoFlag, sLog);
				iRtnFunction = -10150;
			}else
			{
				if(Task.m_bOKLensPass==1)   //Lens ³Ñ±è ¿Ï·á»óÅÂ
				{ 
					sLog.Format("[¿îÀü ÁØºñ]Lens Grip ÈÄÁø »óÅÂ [%d]]\n·»Áî³Ñ±è»óÅÂ È®ÀÎ¹Ù¶ø´Ï´Ù", iStep);
					errMsg2(Task.AutoFlag, sLog);
					iRtnFunction = -10150;
				}else
				{
					iRtnFunction = 10160;
					sLog.Format("		Ready Step [%d]" , iRtnFunction);
					putListLog(sLog);
				}
				
			}
		}
		break;
		
		//Laser ½Ç¸°´õ ÃÊ±âÈ­
	case 10160:
		if (Dio.LaserCylinderUPDOWN(true, false))
		{
			sLog.Format("LASER ½Ç¸°´õ »ó½Â ¿Ï·á[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(sLog);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 10170;
		}
		else
		{
			sLog.Format("LASER ½Ç¸°´õ »ó½Â ½ÇÆÐ [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -10160;
		}
		break;

	case 10170:
		if (Dio.LaserCylinderCheck(true, false))
		{
			sLog.Format("LASER ½Ç¸°´õ »ó½Â È®ÀÎ ¿Ï·á[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(sLog);
			iRtnFunction = 10200;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			sLog.Format("LASER ½Ç¸°´õ »ó½Â ½Ã°£ ÃÊ°ú..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -10170;
		}
		break;
		//Laser ½Ç¸°´õ ÃÊ±âÈ­ END

	case 10200:
		if (motor.PCB_Z_Motor_Move(Wait_Pos))		//PCB ZÃà ´ë±â À§Ä¡ ÀÌµ¿
		{
			iRtnFunction = 10300;
			sLog.Format("		Ready Step [%d]", iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING305);
			sLog.Format(sLangChange, iStep);	//"[¿îÀü ÁØºñ] PCB ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]"
			putListLog(sLog);
			iRtnFunction = -10200;
		}
		
		break;

	case 10300:
		if (motor.LENS_Z_Motor_Move(Wait_Pos))		//Lens ZÃà ´ë±â À§Ä¡
		{
			iRtnFunction = 10400;
			sLog.Format("		Ready Step [%d]", iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING303);
			sLog.Format(sLangChange, iStep);	//"[¿îÀü ÁØºñ] Lens ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]"
			putListLog(sLog);
			iRtnFunction = -10300;
		}
		break;
	case 10400:

		iRtnFunction = 10600;
		sLog.Format("		Ready Step [%d]" , iRtnFunction);
		putListLog(sLog);
		break;

	case 10600:
		if(motor.Pcb_Motor_Move(Wait_Pos))			//PCBºÎ ¸ðÅÍ ´ë±âÀ§Ä¡ ÀÌµ¿
		{
			iRtnFunction = 10700;
			sLog.Format("		Ready Step [%d]" , iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING306);
			sLog.Format(sLangChange, iStep);	//"[¿îÀü ÁØºñ] PCB ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]"
			putListLog(sLog);
			iRtnFunction = -10600;
		}
		break;
	case 10700:
		if(motor.Lens_Motor_MoveXY(Wait_Pos))			//LensºÎ ¸ðÅÍ ´ë±â À§Ä¡ ÀÌµ¿(Lens X/Y/ZÃà)
		{
			iRtnFunction = 10750;
			sLog.Format("		Ready Step [%d]" , iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING304);
			sLog.Format(sLangChange, iStep);	//"[¿îÀü ÁØºñ] Lens ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]"
			putListLog(sLog);
			iRtnFunction = -10700;
		}
		break;
	case 10750:
		if( motor.Pcb_Tilt_Motor_Move(Wait_Pos)) 
		{
			iRtnFunction = 10800;
			sLog.Format("		Ready Step [%d]" , iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING1017);	//PCBºÎ Tx, TyÃà ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ
			sLog.Format(sLangChange);
			putListLog(sLog);	
			iRtnFunction = -10750;
		}
		break;
	case 10800:
		if( motor.Lens_Tilt_Motor_Move(Wait_Pos) )
		{
			iRtnFunction = 10900;
			sLog.Format("		Ready Step [%d]" , iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING745);	//LENSºÎ Tx, TyÃà ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ
			sLog.Format(sLangChange);
			putListLog(sLog);	
			iRtnFunction = -10800;
		}
		break;

	case  10900:
			///////////////////////
		//if( !Dio.LensGrip(true, true) )
		//{		//-- Lens °¨Áö »óÅÂ¿¡¼­ Grip ÀüÁø ¾ÈµÉ °æ¿ì NG
		//	sLangChange.LoadStringA(IDS_STRING300);
		//	sLog.Format(sLangChange, iStep);	//"[¿îÀü ÁØºñ] Lens Grip ÀüÁø ½ÇÆÐ [%d]"
		//	putListLog(sLog);
		//	iRtnFunction = -10900;
		//	break;
		//}			
		//else if( Dio.LensGripCheck(false, false) )			//Lens °¨Áö°¡ ¾ÈµÇ¾úÀ» °æ¿ì  
		//{//-- Lens Grip ÈÄÁø µ¿ÀÛ
		//if(! Dio.HolderGrip(false,false) )    //-- Lens °¨Áö ¾ÈµÇ°í/ Grip ÈÄÁø ¾ÈµÉ °æ¿ì NG
		//{  
		//	sLangChange.LoadStringA(IDS_STRING301);
		//	sLog.Format(sLangChange, iStep);	//"[¿îÀü ÁØºñ] Lens Grip ÈÄÁø ½ÇÆÐ [%d]"
		//	putListLog(sLog);
		//	iRtnFunction = -10900;
		//	break;
		//}
		iRtnFunction = 10950;
		sLog.Format("		Ready Step [%d]" , iRtnFunction);
		putListLog(sLog);
		break;		
	case 10950:
		iRtnFunction = 10970;
		sLog.Format("		Ready Step [%d]" , iRtnFunction);
		putListLog(sLog);
		break;	
	case 10970:
		if(!MIU.Stop())					// 95 ~ 100 msec
		{
			sLog.Format(_T("CCD CLOSE FAIL [%d]"), iStep);
			putListLog(sLog);
			iRtnFunction = -10970;
			break;
		}
		iRtnFunction = 15000;	//While¹® Á¾·á
		sLog.Format("		Ready Step [%d]" , iRtnFunction);
		putListLog(sLog);
		break;
	case 15000:
		UVCommand.UV_Shutter_PowerSet(model.UV_Power);// 95);//
		sLog.Format("	#1 UV POWER SET: %d", model.UV_Power);
		putListLog(sLog);
		Sleep(500);
		UVCommand2.UV_Shutter_PowerSet(model.UV_Power);// 95);//
		sLog.Format("	#2 UV POWER SET: %d", model.UV_Power);
		putListLog(sLog);
		iRtnFunction = 19000;
		break;
	case 19000:
		Task.AutoReday = 1;
//		AutoRunView(3);
		iRtnFunction =19900;
		sLog.Format("		Ready Step [%d]" , iRtnFunction);
		putListLog(sLog);
		break;
	}
	return iRtnFunction;
}

void CAABonderDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CRect rcTemp;

	GetDlgItem(IDC_LABEL_TIME)->GetWindowRect(rcTemp);

	CDialogEx::OnLButtonDblClk(nFlags, point);
}



// MilProcImageChild[4] ÀÌ¿ë °Ë»ç ¼öÇà..
bool CAABonderDlg::_GetOpticAxis(int fi_scale, int fi_thVal, double &fo_dOffsetX, double &fo_dOffsetY)
{
	int pitch = MbufInquire(vision.MilProcImageChild[3], M_PITCH, NULL);
	int sizeX = MbufInquire(vision.MilProcImageChild[3], M_SIZE_X, NULL);
	int sizeY = MbufInquire(vision.MilProcImageChild[3], M_SIZE_Y, NULL);


	fo_dOffsetX = sizeX>>1;
	fo_dOffsetY = sizeY>>1;


	int* pHist_x;
	int* pHist_y;

	pHist_x = new int[sizeX];
	memset(pHist_x, 0 , sizeof(int)*sizeX);
	pHist_y = new int[sizeY];
	memset(pHist_y, 0 , sizeof(int)*sizeY);


	CRect  centRoi, upperRoi, inspRoi;

	centRoi.left	= (long)fo_dOffsetX - sizeX/20;
	centRoi.right	= (long)fo_dOffsetX + sizeX/20;
	centRoi.top		= (long)fo_dOffsetY - sizeY/20;
	centRoi.bottom	= (long)fo_dOffsetY + sizeY/20;

	upperRoi.left	= (long)fo_dOffsetX - sizeX/20;
	upperRoi.right	= (long)fo_dOffsetX + sizeX/20;
	upperRoi.top	= (long)0;
	upperRoi.bottom	= (long)sizeY/20;

	inspRoi.left	= (long)sizeX/20;
	inspRoi.right	= (long)sizeX - inspRoi.left;
	inspRoi.top		= (long)sizeY/20;
	inspRoi.bottom	= (long)sizeY - inspRoi.top;

	vision.boxlist[CCD].addList(centRoi, PS_SOLID, M_COLOR_GREEN);
	vision.boxlist[CCD].addList(upperRoi, PS_SOLID, M_COLOR_RED);
	vision.boxlist[CCD].addList(inspRoi, PS_SOLID, M_COLOR_BLUE);


	int centAvg, upperAvg, inspAvg;
	int x, y, pos, sum;
	

	int sx = centRoi.left;
	int ex = centRoi.right;
	int sy = centRoi.top;
	int ey = centRoi.bottom;

	sum = 0;
	for (y=sy; y<ey; y++)
	{
		pos = y * pitch + sx;

		for (x=sx; x<ex; x++)
		{
			sum += vision.MilImageBuffer[4][pos];
			pos++;
		}
	}
	centAvg = sum / ((ex-sx) * (ey-sy));


	sx = upperRoi.left;
	ex = upperRoi.right;
	sy = upperRoi.top;
	ey = upperRoi.bottom;

	sum = 0;
	for (y=sy; y<ey; y++)
	{
		pos = y * pitch + sx;

		for (x=sx; x<ex; x++)
		{
			sum += vision.MilImageBuffer[4][pos];
			pos++;
		}
	}
	upperAvg = sum / ((ex-sx) * (ey-sy));


	if(centAvg < 20/*60*/)	//KYH Mando Camera ÃøÁ¤ ¼öÁ¤
	{
		sLangChange.LoadStringA(IDS_STRING236);
		putListLog(sLangChange);
		return false;
	}

	if(centAvg < upperAvg)
	{
		sLangChange.LoadStringA(IDS_STRING237);
		putListLog(sLangChange);
		return false;
	}

	if(centAvg - upperAvg < 10/*20*/)	//KYH Mando Camera ÃøÁ¤ ¼öÁ¤
	{
		sLangChange.LoadStringA(IDS_STRING235);
		putListLog(sLangChange);
		return false;
	}


	Task.iOpticalThVal = inspAvg = (centAvg+upperAvg) / 2;


	sx = inspRoi.left;
	ex = inspRoi.right;
	sy = inspRoi.top;
	ey = inspRoi.bottom;

	int val;
	sum = 0;
	for (y=sy; y<ey; y++)
	{
		pos = y * pitch + sx;

		for (x=sx; x<ex; x++)
		{
			val = vision.MilImageBuffer[4][pos];

			if(val>=inspAvg)
			{
				pHist_x[x]++;
				pHist_y[y]++;
				sum++;
			}
			pos++;
		}
	}


	float fAddVal = 0;
	for (x=sx; x<ex; x++)
	{
		fAddVal += (float)(pHist_x[x] * x);
	}

	float f_Cent_X = fAddVal/sum;

	if( (f_Cent_X < pitch*0.3) || (f_Cent_X > pitch*0.7) )
	{
		sLangChange.LoadStringA(IDS_STRING238);
		putListLog(sLangChange);
		return false;

	}


	fAddVal = 0;
	for (y=sy; y<ey; y++)
	{
		fAddVal += (float)(pHist_y[y] * y);
	}

	float f_Cent_y = fAddVal/sum;

	if( (f_Cent_y < sizeY*0.3) || (f_Cent_y > sizeY*0.7) )
	{
		sLangChange.LoadStringA(IDS_STRING239);
		putListLog(sLangChange);
		return false;

	}

	fo_dOffsetX = f_Cent_X;
	fo_dOffsetY = f_Cent_y;

	delete [] pHist_x;
	pHist_x = NULL;

	delete [] pHist_y;
	pHist_y = NULL;

	return true;
}


void CAABonderDlg::Delete_Child_Dialog()
{
	if ( m_pLEDDlg != NULL )
	{
		m_pLEDDlg->DestroyWindow();
		delete m_pLEDDlg;
		m_pLEDDlg = NULL;
	}

	/*if( m_pDisplacementDlg != NULL )
	{
		m_pDisplacementDlg->DestroyWindow();
		delete m_pDisplacementDlg;
		m_pDisplacementDlg = NULL;
	}*/
}

void CAABonderDlg::Make_Child_Dialog()
{
	Delete_Child_Dialog();

	BOOL bCreate = FALSE;

	m_pLEDDlg  = new CLightDlg;
	if ( m_pLEDDlg == NULL )
	{
		return ;
	}
	bCreate = m_pLEDDlg->Create(IDD_DIALOG_LIGHT);
	if ( bCreate == FALSE )
	{
		return;
	}
	m_pLEDDlg->ShowWindow(SW_HIDE);

	/*m_pDisplacementDlg = new CDisplacementDlg;
	if( m_pDisplacementDlg == NULL )	return;
	bCreate = m_pDisplacementDlg->Create(IDD_DIALOG_DISPLACEMENT);
	if ( bCreate == FALSE )
	{
		return ;
	}
	m_pDisplacementDlg->ShowWindow(SW_HIDE);*/
	
	return;
}

int CAABonderDlg::procProductComplete(int iStep)
{
	CString logStr="";
	CString sSocketMsg="";
	CString sBarCode="";
	//
	int iRtnFunction = iStep;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;	//PCB ¿µ»ó È¹µæ Delay
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser ÃøÁ¤Àü Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	double offsetX = 0.0;
	double offsetY = 0.0;
	double offsetTh = 0.0;
	int iTestPatCnt = 0;
	int iRtn = 0;
	//
	vision.clearOverlay(CCD);
	vision.drawOverlay(CCD);
	ctrlSubDlg(MAIN_DLG);
	m_iCurCamNo = 0;
	changeMainBtnColor(MAIN_DLG);
	//
	switch (iStep)
	{ 
		case 11000:	// PCB MIU ¿¬°á	
			Sleep(1000);
			Task.oldLaserTx = 0.1;
			Task.oldLaserTy = 0.1;
			Task.PCBTaskTime = myTimer(true);
			if (sysData.m_FreeRun == 0)
			{
				bThread_MIUCheckRun = true;
				if (!MIUCheck_process() || gMIUDevice.CurrentState != 4)
				{
					sLangChange.LoadStringA(IDS_STRING452);	//CCD ¸ðµâ ¿µ»ó ÃÊ±âÈ­ ½ÇÆÐ.[%d]\n Á¦Ç° ¾ÈÂø »óÅÂ ¹× Á¦Ç° ºÒ·® È®ÀÎ ÇÏ¼¼¿ä.
					logStr.Format(sLangChange, iStep);
					errMsg2(Task.AutoFlag, logStr);
					iRtnFunction = -11000;
					break;
				}
				bThread_MIUCheckRun = false;
			}
			iRtnFunction = 12000;
			break;
		case 12000:
			//È®ÀÎÇÊ¿ä norinda
			Task.m_bPBStart = 1;
			//if( Task.m_bPBStart ==  1 )
			//{
			//	func_Socket_Barcode();
			//}
			//else
			//{
			//	//! ¹ÙÄÚµå°¡ ¾ø±â ¶§¹®¿¡ Åë½Å »ç¿ë ¾ÈÇÔ.
			//	Task.iRecvLenACK[0] = Task.iRecvLenACK[1] = 0;
			//	Task.dTiltingManual[0] = Task.dTiltingManual[1] = 0.0;
			//	Task.dAlignManual[0] = Task.dAlignManual[0] = Task.dAlignManual[0] = 0.0;
			//}
			iRtnFunction = 12100;
			break;

	case 12100:

		Task.m_iRetry_Opt = 0;
		//SetDigReference(PCB_Holder_MARK);
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_HOLDER]);		// Align¸¸ Á¶¸í ON

		Task.d_Align_offset_x[PCB_Chip_MARK] = 0;
		Task.d_Align_offset_y[PCB_Chip_MARK] = 0;
		Task.d_Align_offset_th[PCB_Chip_MARK] = 0;

		if(motor.Pcb_Motor_Move(CompleteAlign_Pos))
		{
			Task.PCBTaskTime = myTimer(true);
			logStr.Format("Pcb ¿ÏÁ¦Ç° Align À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iStep);
  			putListLog(logStr);
  			iRtnFunction = 12200;
		}
		else
		{
			logStr.Format("Pcb ¿ÏÁ¦Ç° Align À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -12100;
		}
		break;
	case 12200:
		if(motor.PCB_Z_Motor_Move(CompleteAlign_Pos))
		{
			Task.PCBTaskTime = myTimer(true);
			logStr.Format("Pcb ¿ÏÁ¦Ç° Align ZÀ§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iStep);
  			putListLog(logStr);
  			iRtnFunction = 12400;
		}
		else
		{
			logStr.Format("Pcb ¿ÏÁ¦Ç° Align ZÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -12200;
		}
		break;
	
	case 12400:
		checkMessage();
		if((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			iRtnFunction = 12500;
		}
		break;
	case 12500:

		if (Task.m_iRetry_Opt>iAlignRetry)
		{	
			logStr.Format("PCB ¿ÏÁ¦Ç° Align Àç°Ë»ç %d È¸ ½ÇÆÐ[%d]\n º¸Á¤ ¹ÌÀû¿ëÀ¸·Î ÁøÇàÇÏ½Ã°Ú½À´Ï±î?", Task.m_iRetry_Opt, iStep);
			if ( askMsg(logStr) == IDOK)
			{
				Task.d_Align_offset_x[PCB_Chip_MARK] -= 0;
				Task.d_Align_offset_y[PCB_Chip_MARK] -= 0;
				Task.d_Align_offset_th[PCB_Chip_MARK] += 0;
				iRtnFunction = 12800;
			}
			else
			{
				iRtnFunction = -12500;
				logStr.Format("PCB Align Àç°Ë»ç %d È¸ ½ÇÆÐ[%d]", Task.m_iRetry_Opt, iStep);
				errMsg2(Task.AutoFlag,logStr);
			}
			break;
			
		}
		//-----------------------------------
		offsetX = offsetY = offsetTh = 0.0;
		iRtn = procCamComAlign(CAM1, PCB_Chip_MARK, false, offsetX, offsetY, offsetTh);

		offsetTh = 0.0;
		saveInspImage(PCB_IMAGE_SAVE, Task.m_iRetry_Opt);
		Task.m_iRetry_Opt++;
		if (iRtn == 1)
		{
			Task.d_Align_offset_x[PCB_Chip_MARK] -= offsetX;
			Task.d_Align_offset_y[PCB_Chip_MARK] -= offsetY;
			Task.d_Align_offset_th[PCB_Chip_MARK] += offsetTh;

			logStr.Format("PCB ¿ÏÁ¦Ç° Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
			//pcbDlg->m_labelAlignResult.SetText(logStr);
			//pcbDlg->m_labelAlignResult.Invalidate();
			putListLog(logStr);

			int iRtnVal = AlignLimitCheck(CAM1, offsetX, offsetY, offsetTh); // PCBÀÌÀç±â ¿òÁ÷ÀÌ´Â ºÎºÐÀÌ¶û Limit Check ÇÒ¶§ CAM1ÀÓ

			if(iRtnVal==1)
			{
				iRtnFunction = 12700;				// º¸Á¤ ÀÌµ¿
			}
			else if(iRtnVal==2)
			{
				iLaser_Pos = 0;
				logStr.Format("		PCB ¿ÏÁ¦Ç° Align [%d] ¿Ï·á [%d]", Task.m_iRetry_Opt, iStep);
				putListLog(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);
				logStr.Format("		PCB ¿ÏÁ¦Ç° Align Complete :  %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
				AlignResultSave(logStr);	
				Task.m_iRetry_Opt = 0;
				Task.dSocketRecvTime = myTimer(true);
				iRtnFunction = 12800;
				
			}
			else
			{
				logStr.Format("PCB ¿ÏÁ¦Ç° Align [%d] º¸Á¤ ¹üÀ§ ÃÊ°ú[%d]", Task.m_iRetry_Opt, iStep);
				errMsg2(Task.AutoFlag,logStr);
				AlignLogSave(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);
				iRtnFunction = -12500;	
			}
		}
		else
		{
			logStr.Format("PCB ¿ÏÁ¦Ç° Align [%d] ¸¶Å© ÀÎ½Ä ½ÇÆÐ[%d]", Task.m_iRetry_Opt, iStep);
			putListLog(logStr);
			iRtnFunction = 12500;				//  Àç°Ë»ç 
		}
		break;

	case 12700://!! º¸Á¤·® ÀÌµ¿.- Retry
		if(motor.Pcb_Holder_Align_Move(CompleteAlign_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 12200;
		}
		else
		{
			logStr.Format("Pcb ¿ÏÁ¦Ç° Align º¸Á¤ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -12700;
		}
		break;
	case 12800:
		//LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		//·¹ÀÌÀú °Ë»çÀü off
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCBºÎ zÃà ¸ðÅÍ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			logStr.Format(sLangChange, iStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -12800;	
			break;
		}

		iRtnFunction = 12850;

		
		break;
	case 12850:
		iLaser_Pos = 0;
		vision.clearOverlay(CAM1);
		//vision.drawOverlay(CAM1);
		//¿ÜºÎ º¯À§ÃøÁ¤°ª Read
#if (____AA_WAY == PCB_TILT_AA) 
		if (g_ADOData.func_Get_RecordData(Task.ChipID))//DB¿¡ Data°¡ ÀÖÀ» °æ¿ì
		{

			Task.dAAPcbMotor[0] = g_ADOData.adoRegist.dPCBMotor[0];   //PCB X
			Task.dAAPcbMotor[1] = g_ADOData.adoRegist.dPCBMotor[1];   //PCB Y
			Task.dAAPcbMotor[2] = g_ADOData.adoRegist.dPCBMotor[2];   //PCB TH
																	  //TX[3] , TY[4]
			Task.dAAPcbMotor[3] = g_ADOData.adoRegist.dPCBMotor[3];   //PCB TX 
			Task.dAAPcbMotor[4] = g_ADOData.adoRegist.dPCBMotor[4];   //PCB TY 
																	  //
			Task.dAAPcbMotor[5] = g_ADOData.adoRegist.dPCBMotor[5];   //PCB Z
			Task.dAlignManual[0] = g_ADOData.adoRegist.dOffset_Align[0];  //AlignOffsetX
			Task.dAlignManual[1] = g_ADOData.adoRegist.dOffset_Align[1];  //AlignOffsetY
			Task.dAlignManual[2] = g_ADOData.adoRegist.dOffset_Align[2];  //AlignOffsetT
			//
			logStr.Format("PCB È­»ó°Ë»ç TX(%.2lf), TY(%.2lf)", Task.dAAPcbMotor[3] , Task.dAAPcbMotor[4]);
			putListLog(logStr);
			iRtnFunction = 12900;

			
			//iRtnFunction = 12900;
			break;
		}
		else
		{
			Task.dAAPcbMotor[0] = model.axis[Motor_PCB_X].pos[Bonding_Pos];   //PCB X
			Task.dAAPcbMotor[1] = model.axis[Motor_PCB_Y].pos[Bonding_Pos];   //PCB Y
			Task.dAAPcbMotor[2] = model.axis[Motor_PCB_TH].pos[Bonding_Pos];   //PCB TH
																			   //TX[3] , TY[4]
			Task.dAAPcbMotor[3] = model.axis[Motor_PCB_Xt].pos[Bonding_Pos];   //PCB TX 	model.m_InspPcbTilt[0];//
			Task.dAAPcbMotor[4] = model.axis[Motor_PCB_Yt].pos[Bonding_Pos];   //PCB TY 	model.m_InspPcbTilt[1];//

														 //
			Task.dAAPcbMotor[5] = model.axis[Motor_PCB_Z].pos[Bonding_Pos];   //PCB Z

			iRtnFunction = 12900;

			//iRtnFunction = 12900;
			//logStr.Format("ÀÏÄ¡ ¸ðµ¨ ¾ø½À´Ï´Ù.\ bonding À§Ä¡·Î ÁøÇà ÇÏ½Ã°Ú½À´Ï±î?");	//ÀüÃ¼ ¿øÁ¡ º¹±Í¸¦ ½ÇÇà ÇÏ½Ã°Ú½À´Ï±î?
			//if (askMsg(logStr))
			//{

			//	
			//	break;
			//}
			/*logStr.Format("ÀÏÄ¡ ¸ðµ¨ ¾ø½À´Ï´Ù.[%d]", iStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -12850;*/
			//ÀÏÄ¡ ¸ðµ¨ ¾øÀ» °æ¿ì ¼öµ¿ Æ¼Äª°ª Ã¢À¸·Î.
			/*Task.m_iRtnTiltingDlg = 2;
			TiltingManualdlg->ShowWindow(SW_SHOW);
			iRtnFunction = 12870;*/
		}
			
		
#elif (____AA_WAY == LENS_TILT_AA)
			Task.dTiltingManual[0] = 0.0;
			Task.dTiltingManual[1] = 0.0;
			iRtnFunction = 12900;
			/*
					Task.dTiltingManual[0] = g_ADOData.adoRegist.dOffset_TiltTx;
					Task.dTiltingManual[1] = g_ADOData.adoRegist.dOffset_TiltTy;

					Task.dAlignManual[0] = g_ADOData.adoRegist.dOffset_Align[0];
					Task.dAlignManual[1] = g_ADOData.adoRegist.dOffset_Align[1];
					Task.dAlignManual[2] = g_ADOData.adoRegist.dOffset_Align[2];
			*/
		
		break;
#endif
		break;
	case 12870:
		if( Task.m_iRtnTiltingDlg == 0 )
		{
			logStr.Format("º¯À§ÃøÁ¤ Data ÀÔ·Â Áß ÀÚµ¿¸ðµå Á¤Áö ¼±ÅÃ[%d]", iStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -12870;
			break;
		}
		else if ( Task.m_iRtnTiltingDlg == 1 )
		{
			logStr.Format("¿ÜºÎ º¯À§ Tilting ¿Ï·á À§Ä¡ : Tx=%.03lf, Ty=%.03lf", Task.dTiltingManual[0], Task.dTiltingManual[1]);
  			putListLog(logStr);
			
			Task.m_timeChecker.Measure_Time(4);	//Laser º¯À§ ÃøÁ¤ ½ÃÀÛ ½Ã°£
			//iRtnFunction = 12900;//pcb aa¶Ç ·¹ÀÌÀú ÇÏµµ·Ï 200708

			iRtnFunction = 12900;

		}
		break;
	case 12900:	
		if (sysData.m_iLaserInspPass == 1)		//¿ÏÁ¦Ç°
		{
			iRtnFunction = 13300;		//¿ÏÁ¦Ç°½Ã ·¹ÀÌÀú ÆÐ½º
			break;
		}

		
		if(motor.Pcb_Motor_Move_Laser(iLaser_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			if(motor.PCB_Z_Motor_Move(Com_Laser_Pos))//Laser_Lens_Pos))
			{
				Task.PCBTaskTime = myTimer(true);
				logStr.Format("Laser º¯À§ ÃøÁ¤ ¿ÜºÎ À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iStep);
  				putListLog(logStr);
				iRtnFunction = 12910;// 12950;
			} else {
				logStr.Format("PCB Com_Laser_Pos Z ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
				putListLog(logStr);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -12900;
				break;
			}
		}
		else
		{
			logStr.Format("Laser º¯À§ ÃøÁ¤ ¿ÜºÎ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -12900;
		}
		
		break;
	case 12910:
		if (iLaser_Pos > 0)
		{
			iRtnFunction = 12950;
			break;
		}
		if (Dio.LaserCylinderUPDOWN(false, false))
		{
			logStr.Format("LASER ½Ç¸°´õ ÇÏ°­ ¿Ï·á[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 12920;
		}
		else
		{
			logStr.Format("LASER ½Ç¸°´õ ÇÏ°­ ½ÇÆÐ [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -12910;
		}
		break;

	case 12920:
		if (Dio.LaserCylinderCheck(false, false))
		{
			logStr.Format("LASER ½Ç¸°´õ ÇÏ°­ È®ÀÎ ¿Ï·á[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 12950;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER ½Ç¸°´õ ÇÏ°­ ½Ã°£ ÃÊ°ú..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -12920;
		}
		break;
		//laser½Ç¸°´õ ÇÏ°­ END
	case 12950:	
		checkMessage();
		if((myTimer(true) - Task.PCBTaskTime) > iLaserDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			iRtnFunction = 13100;
		}
		break;
	case 13100:
		Keyence.func_CL3000_Scan(Task.m_Laser_Point[iLaser_Pos]);
		Sleep(100);
		logStr.Format("º¯À§ ¼¾¼­ %lf[%d] - ¿ÜºÎ - %d À§Ä¡", Task.m_Laser_Point[iLaser_Pos], iStep, iLaser_Pos+1);
  		putListLog(logStr);
		LaserPos[iLaser_Pos].x = motor.GetEncoderPos(Motor_PCB_X);
		LaserPos[iLaser_Pos].y = motor.GetEncoderPos(Motor_PCB_Y);
		LaserValue[iLaser_Pos] = Task.m_Laser_Point[iLaser_Pos];
		
		if(Task.m_Laser_Point[iLaser_Pos] != 0 )
		{
			if(iLaser_Pos == 3)
			{
				if( func_Check_LaserValueErr(LaserValue) == false)	
				{
					logStr.Format("Laser ÃøÁ¤°ª ÀÌ»ó..¿ÜºÎ º¯À§ ÃøÁ¤°ªÀÌ ÀÌ»ó..[%d]\n ÃøÁ¤°ª:%.04lf, %.04lf, %.04lf, %.04lf ", iStep, LaserValue[0], LaserValue[1], LaserValue[2], LaserValue[3]);
					errMsg2(Task.AutoFlag,logStr);
 					iRtnFunction = -13100;
					break;
				}

				if(_calcLaserTilt(LaserPos, LaserValue, Task.d_Align_offset_xt[0], Task.d_Align_offset_yt[0]))
				{
				/*	logStr.Format("		º¸Á¤·® Tx: %.04lf, Ty: %.04lf", Task.d_Align_offset_xt[0], Task.d_Align_offset_yt[0] );
  					putListLog(logStr);
					logStr.Format("		³»ºÎ º¸Á¤·® ÆíÂ÷ Tx: %.04lf, Ty: %.04lf", Task.dTiltingManual[0], Task.dTiltingManual[1] );
  					putListLog(logStr);
					int Rnd = TiltAlignLimitCheck(Task.d_Align_offset_xt[0] + Task.dTiltingManual[0], Task.d_Align_offset_yt[0] + Task.dTiltingManual[1] );
					*/

					
					//Task.m_dataOffset_x[0] =  Task.d_Align_offset_xt[PCB_Holder_MARK];
					//Task.m_dataOffset_y[0] =  Task.d_Align_offset_yt[PCB_Holder_MARK];
					logStr.Format("		º¸Á¤·® Tx: %.04lf, Ty: %.04lf", Task.d_Align_offset_xt[0], Task.d_Align_offset_yt[0] );
  					putListLog(logStr);
					logStr.Format("		¿ÜºÎ º¸Á¤·® ÆíÂ÷ Tx: %.04lf, Ty: %.04lf", Task.dTiltingManual[0], Task.dTiltingManual[1] );
  					putListLog(logStr);
					//int Rnd = TiltAlignLimitCheck(Task.d_Align_offset_xt[PCB_Holder_MARK],Task.d_Align_offset_yt[PCB_Holder_MARK]);
					//int Rnd = TiltAlignLimitCheck(Task.d_Align_offset_xt[0] + Task.dTiltingManual[0], Task.d_Align_offset_yt[0] + Task.dTiltingManual[1] );
					int Rnd = TiltAlignLimitCheck(Task.d_Align_offset_xt[0], Task.d_Align_offset_yt[0]);
					if(Rnd == 2)
					{
						Task.m_timeChecker.Measure_Time(5);	//Laser º¯À§ ÃøÁ¤ ¿Ï·á ½Ã°£
						Task.m_dTime_LaserDpm = Task.m_timeChecker.m_adTime[5] - Task.m_timeChecker.m_adTime[4]; 
						//dispGrid();
						iRtnFunction = 13300;	
						
					}
					else if (Rnd == 1)
					{
						iRtnFunction = 13200;	//º¸Á¤·® ÀÌµ¿		
					}
					else
					{
						logStr.Format("Laser Tilt  º¸Á¤°ª Limit¸¦ ÃÊ°ú ÇÏ¿´½À´Ï´Ù.[%d]", iStep);
						errMsg2(Task.AutoFlag,logStr);
 						iRtnFunction = -13100;
					}
				}
			}
			else
			{
				iRtnFunction = 12900;
				iLaser_Pos++;
			}
		}
		else
		{
			iRtnFunction = 13100;
		}
		break;
	case 13200:
		//if(motor.Pcb_Move_Tilt(Task.d_Align_offset_xt[0] + Task.dTiltingManual[0],Task.d_Align_offset_yt[0] + Task.dTiltingManual[1] ))
		if (motor.Pcb_Move_Tilt(Task.d_Align_offset_xt[0] , Task.d_Align_offset_yt[0] ))
		{
			iLaser_Pos = 0;
			iRtnFunction = 12900;
		}
		break;

	case 13300:
		if (Dio.LaserCylinderUPDOWN(true, false))
		{
			logStr.Format("LASER ½Ç¸°´õ »ó½Â ¿Ï·á[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 13310;
		}
		else
		{
			logStr.Format("LASER ½Ç¸°´õ »ó½Â ½ÇÆÐ [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -13300;
		}
		break;
	case 13310:
		if (Dio.LaserCylinderCheck(true, false))
		{
			logStr.Format("LASER ½Ç¸°´õ »ó½Â È®ÀÎ ¿Ï·á[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 13350;
			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER ½Ç¸°´õ »ó½Â ½Ã°£ ÃÊ°ú..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -13310;
		}
		break;
		//laser½Ç¸°´õ »ó½Â END
	case 13350:	
		if(motor.PCB_Z_Motor_Move(Wait_Pos)) 
		{
			logStr.Format("PCB Z ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iRtnFunction);
  			putListLog(logStr);
  			iRtnFunction = 14000;
		}
		else
		{
			logStr.Format("PCB Z ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iRtnFunction);
  			putListLog(logStr);
  			iRtnFunction = -13350;
		}
		break;
	case 14000:

		iRtnFunction = 14100;
		break;
	case 14100:	

		iRtnFunction = 14200;
		break;

	case 14200:	
		checkMessage();
		if((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			iRtnFunction = 14300;
			Task.m_iRetry_Opt = 0;
		}
		break;
	case 14300:	

		iRtnFunction = 14400;				// ¿Ï·á
		break;
	case 14400:	
		iRtnFunction = 14500;
		break;
	case 14500:	
		if(motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB Z ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iStep);
  			putListLog(logStr);
  			iRtnFunction = 15200;
		}
		else
		{
			logStr.Format("PCB Z ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
  			putListLog(logStr);
  			iRtnFunction = -14500;
		}
		break;
	case 15200:	
		iRtnFunction = 19600;
		break;	
		  
	case 19600: // º»µù À§Ä¡ ÀÌµ¿ 
		//if(motor.Pcb_Motor_Move(Bonding_Pos, Task.d_Align_offset_x[PCB_Holder_MARK] + Task.dAlignManual[0], Task.d_Align_offset_y[PCB_Holder_MARK] + Task.dAlignManual[1], Task.d_Align_offset_th[PCB_Holder_MARK] + Task.dAlignManual[2]) )
  		if(motor.Pcb_Motor_Move(Bonding_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
  		{
			Task.PCBTaskTime = myTimer(true);
  			logStr.Format("PCB Bonding º¸Á¤ À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iStep);
  			putListLog(logStr);
  			iRtnFunction = 19700;
  		}
  		else
  		{
  			logStr.Format("PCB Bonding À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -19600;
  		}
		break;
	case 19700:
		iRtnFunction = 19800;//pcb aaµµ ·¹ÀÌÀú ÇÏµµ·Ï 200708
		break;
#if (____AA_WAY == PCB_TILT_AA)
		if(motor.Pcb_Move_Tilt( Task.dAAPcbMotor[3], Task.dAAPcbMotor[4] ))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 19800;
		}else
		{
			logStr.Format(_T("PCB Tilt Bonding Pos Move Fail[%d]"), iStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -19700;
		}
#elif (____AA_WAY == LENS_TILT_AA)
		iRtnFunction = 19800;
#endif
		break;
	case 19800: //zÃà º»µù À§Ä¡ ÀÌµ¿
		if(!motor.PCB_Z_Motor_Move(Bonding_Pos))
		{
			logStr.Format(_T("PCB Z Bonding Pos Move Fail[%d]"), iStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -19800;
		}
		iRtnFunction = 19900;
		break;
	case 19900:
		checkMessage();
		if((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			if (sysData.m_iIrChartUse == 1)
			{
				IrvAlignLed.Irv_Light_SetValue(model.m_iLedValue[LEDDATA_TOP1_CHART], IR_CHART);
			}
			else
			{
				LightControl.ctrlLedVolume(LIGHT_TOP_CHART1, model.m_iLedValue[LEDDATA_TOP1_CHART]);
				LightControl.ctrlLedVolume(LIGHT_TOP_CHART2, model.m_iLedValue[LEDDATA_TOP2_CHART]);
				LightControl.ctrlLedVolume(LIGHT_TOP_CHART3, model.m_iLedValue[LEDDATA_TOP3_CHART]);
				LightControl.ctrlLedVolume(LIGHT_TOP_CHART4, model.m_iLedValue[LEDDATA_TOP4_CHART]);
			}


			LightControl.ctrlLedVolume(LIGHT_TOP_CHART1, model.m_iLedValue[LEDDATA_TOP1_CHART]);
			LightControl.ctrlLedVolume(LIGHT_TOP_CHART2, model.m_iLedValue[LEDDATA_TOP2_CHART]);
			LightControl.ctrlLedVolume(LIGHT_TOP_CHART3, model.m_iLedValue[LEDDATA_TOP3_CHART]);
			LightControl.ctrlLedVolume(LIGHT_TOP_CHART4, model.m_iLedValue[LEDDATA_TOP4_CHART]);
			LightControl.ctrlLedVolume(LIGHT_LEFT_CHART, model.m_iLedValue[LEDDATA_CHART_L]);
			LightControl.ctrlLedVolume(LIGHT_RIGHT_CHART, model.m_iLedValue[LEDDATA_CHART_R]);



			//if( func_MIU_ConnectLiveCheck() == false)		//MIU ¿µ»ó Check
			//{
			//	iRtnFunction = -19900;
			//	break;
			//}
			setCamDisplay(3, 1);

			iRtnFunction = 120000;//¿ÏÁ¦Ç° °Ë»ç
		}
		break;
		
	default:
		sLangChange.LoadStringA(IDS_STRING1004);	//PCB ¿ÏÁ¦Ç° °Ë»ç Thread Step ¹øÈ£ ºñÁ¤»ó . [%d]
		logStr.Format(sLangChange, iStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -20000;
		break;
	}
	return iRtnFunction;
}



void CAABonderDlg::OnBnClickedButtonPause()
{
	if(Task.AutoFlag != 1)
	{
		sLangChange.LoadStringA(IDS_STRING1374);	//ÀÚµ¿¿îÀüÁßÀÌ ¾Æ´Õ´Ï´Ù.
		errMsg2(Task.AutoFlag, sLangChange);
		return;
	}

	Dio.setAlarm(AUTO_STOP);


	//if(sysData.m_FreeRun == 1)
	//{
	//	Task.AutoFlag		= 0;			//	// 0:Á¤Áö 1:ÀÚµ¿ 2: ÀÏ½ÃÁ¤Áö 20130405	
	//	Task.PausePCBStep	= 0;
	//}
	//else
	//{
		Task.AutoFlag		= 2;			//	// 0:Á¤Áö 1:ÀÚµ¿ 2: ÀÏ½ÃÁ¤Áö 20130405	
		Task.PausePCBStep	= Task.PCBTask;
		//Task.PauseLensStep	= Task.LensTask;
	//}

	AutoRunView(Task.AutoFlag);
	Task.AutoReday = 0;

	bThreadTaskPcb =0;			//	 Á¤Áö ÇÒ¶§ »ç¿ë 0:¾²·¹µå ºüÁ® ³ª¿È
	bThreadTaskLens =0;

	CString sTemp;
	sLangChange.LoadStringA(IDS_STRING895);
	sTemp.Format(sLangChange, Task.AutoFlag, Task.PCBTask);
	putListLog(sTemp);

	sysData.m_FreeRun	= 0;
}


void CAABonderDlg::OnBnClickedButtonStop()
{
	bThreadEpoxyRun = false;

	motor.StopAxisAll();
	if (pThread_Epoxy != NULL)
	{
		::WaitForSingleObject(pThread_Epoxy->m_hThread, 100);
	}

	vision.clearOverlay(CAM1);
	vision.clearOverlay(CAM2);
	//	UVCommand.UV_Lamp_Off();
	Dio.StartPBLampOn(false);
	Dio.setAlarm(AUTO_STOP);
	//Dio.PCBvaccumOn(VACCUM_OFF, false);
	Task.AutoFlag = 0;
	AutoRunView(Task.AutoFlag);

	bThreadTaskPcb		= 0;
	bThreadTaskLens		= 0;
	Task.AutoReday		= 0;
	sysData.m_FreeRun	= 0;
	Task.PcbOnStage = 100;
	Task.m_iStatus_Unit_Epoxy = 0;
	m_btnStart.m_iStateBtn = 0;
	m_btnStart.Invalidate();
	//UVCommand.ReadyDevice();
	
	//if (g_pFoceDlg->IsWindowVisible())
	//	g_pFoceDlg->ShowWindow(SW_HIDE);

	vision.clearOverlay(CCD);
	
	if(!bThread_MIUCheckRun)
	{
		putListLog("MIU Stop ½ÃÀÛ.");
		MIU.Close();					// 95 ~ 100 msec//MIU.Close();					// 95 ~ 100 msec
		putListLog("MIU Stop ¿Ï·á.");
	}
	DisableButton(Task.AutoFlag);
	g_bMovingflag=false;
}



void CAABonderDlg::OnBnClickedButtonAutorun()
{
	if(g_bMovingflag){
		putListLog("	¸ðÅÍÀÌµ¿Áß!");
		return;
	}

	if(Task.AutoFlag == 1)
	{
		return;
	}
	if ( bThreadTaskReadyRun == true)
	{
		putListLog("	¿îÀüÁØºñÁßÀÔ´Ï´Ù!");
		return;
	}
	if(Task.AutoReday == 0 && Task.AutoFlag == 0)
	{
#ifndef	ON_LINE_MODE
		Task.AutoFlag = 1;
#endif
		sLangChange.LoadStringA(IDS_STRING1326);	//¿îÀü ÁØºñ°¡ µÇÁö ¾Ê¾Ò½À´Ï´Ù.
		errMsg2(Task.AutoFlag, sLangChange);
		return;
	}


	if( Dio.StartPBOnCheck(true, false) && Task.AutoFlag != 2 )
	{//½ºÀ§Ä¡°¡ ´­·ÁÁ® ÀÖ°í, ÀÏ½Ã Á¤Áö »óÅÂ°¡ ¾Æ´Ò °æ¿ì¸¸ Check
		sLangChange.LoadStringA(IDS_STRING1383);	//Àü¸é Door Start/Stop ½ºÀ§Ä¡°¡ On »óÅÂ ÀÔ´Ï´Ù.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
		return;
	}
	else if( Dio.StartPBOnCheck(false, false) )
	{
		Dio.StartPBLampOn(false);
	}

	//if (g_pFoceDlg->IsWindowVisible())
		//g_pFoceDlg->ShowWindow(SW_HIDE);


	for(int iCamNo=0; iCamNo<MARK_CNT; iCamNo++)
	{
		int iCh = 1;
		if(iCamNo == PCB_Chip_MARK)	iCh = 0;

		for (int iMarkNo=0; iMarkNo<2; iMarkNo++)
		{
			vision.geometricMarkPreProc(iCh, iCamNo, iMarkNo);
		}
	}

	if(Task.AutoFlag ==0 )
	{// 0:Á¤Áö 1:ÀÚµ¿ 2: ÀÏ½ÃÁ¤Áö 20130405
		//Task.LensTask	= 10000;		
		Task.PCBTask	= 10000;
	}
	else if(Task.AutoFlag ==2)
	{
		Task.LensTask	= Task.PauseLensStep;		
		Task.PCBTask	= Task.PausePCBStep;	
		Dio.StartPBLampOn(true);
	}

	Task.m_iStart_Step_PCB	= 10000;
	Task.m_iEnd_Step_PCB	= 170000;
	Task.m_iStart_Step_LENS	= 10000;
	Task.m_iEnd_Step_LENS	= 60000;

	#ifdef ON_LINE_VISION
		if(!m_bMiuRun && Task.AutoFlag)
		{
			
			if(!bThread_MIUCheckRun)
			{
				bThread_MIUCheckRun = true;
				MIUCheck_process();
				bThread_MIUCheckRun = false;
			}
			
		}
	#endif

	g_bMovingflag =true;
	Dio.setAlarm(AUTO_RUN);
	Task.AutoFlag = 1;
	Task.m_iStatus_Unit_Epoxy = 0;
	AutoRunView(Task.AutoFlag);
	Task.PCBTaskTime = Task.LensTaskTime = myTimer(true);
	Task.AutoReday = 0;

	g_AlarmFlag = true;

	Task.m_timeChecker.Init_Time();
	Task.m_timeChecker.Start_Time();

	pThread_TaskPcb = ::AfxBeginThread(Thread_TaskPcb, this);
	pThread_TaskLens = ::AfxBeginThread(Thread_TaskLens, this);
}


void CAABonderDlg::OnBnClickedButtonReady()		
{
	if(g_bMovingflag){
		return;
	}

	if(Task.AutoFlag==1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//ÀÚµ¿ ¿îÀü Áß ÀÔ´Ï´Ù.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
		return;
	}

	if(Task.AutoFlag==2)
	{
		sLangChange.LoadStringA(IDS_STRING1362);	//"ÀÏ½Ã Á¤Áö Áß ÀÔ´Ï´Ù."
		delayMsg(sLangChange, 3000, M_COLOR_RED);
		return;
	}

	//if(Dio.PCBCoverCloseCheck(true, false) == false)
	//{
	//	CString sTemp;
	//	sLangChange.LoadStringA(IDS_STRING1026);	//PCBºÎ Ä¿¹ö ¼ÒÄÏÀÌ ¿­·Á ÀÖ½À´Ï´Ù.
	//	sTemp.Format(sLangChange);
	//	delayMsg(sTemp.GetBuffer(99), 3000, M_COLOR_DARK_GREEN);
	//	return ;
	//}
	CString sTemp="";
	Dio.StartPBLampOn(false);

	/*if (!Dio.CamLaserSlinderMove(false, false))
	{
		sTemp.Format("Laser/Cam ½Ç¸°´õ ÈÄÁø ½ÇÆÐ");
		errMsg2(Task.AutoFlag, sTemp);
		return;
	}
	if (!Dio.CamLaserSlinderCheck(false, true))
	{
		sTemp.Format("Laser/Cam ½Ç¸°´õ ÈÄÁø È®ÀÎ ½ÇÆÐ");
		errMsg2(Task.AutoFlag, sTemp);
		return;
	}*/

	if( Dio.DoorPBOnCheck(true, false) )
	{
		Dio.DoorPBLampOn(true);
	}
	else 
	{
		Dio.DoorPBLampOn(false);
	}
	//if(sysData.m_iFront==0)
	//{
	//	if(!Dio.DoorLift(false, true) ) 
	//	{	
	//	
	//		sLangChange.LoadStringA(IDS_STRING1471);	//¿îÀü ÁØºñ ½ÇÆÐ. Door Open »óÅÂ..
	//		sTemp.Format(sLangChange);
	//		delayMsg(sTemp.GetBuffer(99), 50000, M_COLOR_RED);
	//		g_bMovingflag = false;
	//		return;
	//	}
	//}
	//

	Dio.setAlarm(ALARM_OFF);

	Task.AutoFlag = 1;
	AutoRunView(Task.AutoFlag);

	for(int i=0; i<MAX_MTF_NO;i++)
	{
		Task.SFR.fSfrN4[Task.m_iDrawBarStep-1][i] = 0.0;
	}

	myTimer(false);
	Task.bManual_FindEpoxy = false;

	iLaser_Pos = 0;

	Task.ReadyTask	= 10000;
	Task.PCBTask	= 10000;
	Task.LensTask = 10000;

	Task.PausePCBStep	= 10000;

	pThread_TaskReady = ::AfxBeginThread(Thread_TaskReady, this);
}


void CAABonderDlg::DispCurModelName(CString sName)
{
	m_labelCurModelName.SetText(sName);
	m_labelCurModelName.Invalidate();
}

void CAABonderDlg::initInspResGrid()
{//¸¸µµ Â÷·®¿ë Camer °Ë»ç °á°ú 
	dispInspResGrid();
}

void CAABonderDlg::dispInspResGrid()
{

}

void CAABonderDlg::InitGridCtrl_Result()
{
	CRect rect;
	CWnd *pWnd= (CWnd*)GetDlgItem(IDC_STATIC_RESULT_GRID); 
	ResultRow = 8;//¾Æ·¡
	ResultCol = 2;//¿·
	int margin = 4;
	int gridHeight = 32;
	int gridWidth1 = 100;
	int gridWidth2 = 82;
	int totalWidth = gridWidth1+(gridWidth2*(ResultCol-1));
	//
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);

	rect.right = totalWidth +margin;
	rect.bottom = (gridHeight*ResultRow) +margin;
	pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//´ÙÀÌ¾ó·Î±×ÀÇ Å©±â¿Í À§Ä¡°ª Á¶Á¤À» À§ÇÑ ÇÔ¼ö.

	GetDlgItem(IDC_STATIC_RESULT_GRID)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_clGridResult.Create(rect, this, IDC_STATIC_RESULT_GRID, WS_TABSTOP | WS_VISIBLE);

	m_clGridResult.SetTextBkColor(GRID_COLOR_WHITE);
	m_clGridResult.SetFixedBkColor(GRID_COLOR_TITLE);
	m_clGridResult.SetFixedTextColor(GRID_COLOR_WHITE);
	m_clGridResult.SetReference_Setting();
	m_clGridResult.EnableSelection(FALSE);
	m_clGridResult.SetRowCount(ResultRow);
	m_clGridResult.SetColumnCount(ResultCol);
	m_clGridResult.SetFixedRowCount(1);
	m_clGridResult.SetFixedColumnCount(1);

	CString tmpStr="";

	m_clGridResult.SetItemText(0, 1, "SEC");
	m_clGridResult.SetItemText(1, 0, "»ý»ê·®");
	m_clGridResult.SetItemText(2, 0, "NG ¼ö·®");
	m_clGridResult.SetItemText(3, 0, "TOTAL T/T");
	m_clGridResult.SetItemText(4, 0, "Dispensing T/T");
	m_clGridResult.SetItemText(5, 0, "Laser ÃøÁ¤ T/T");
	m_clGridResult.SetItemText(6, 0, "AA T/T");
	m_clGridResult.SetItemText(7, 0, "°Ë»ç T/T");


	//
	int i=0, j=0;
	for (i = 0; i < ResultRow; i++)
	{
		m_clGridResult.SetRowHeight(i, gridHeight);
		
		for (j = 0; j < ResultCol; j++)
		{
			m_clGridResult.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (i == 0)
			{
				if (j == 0)
				{
					m_clGridResult.SetColumnWidth(j, gridWidth1);
				}else{
					m_clGridResult.SetColumnWidth(j, gridWidth2);
				}
			}			
		}
	}
	m_clGridResult.Invalidate();
	dispGrid();
}
void CAABonderDlg::ShowGridCtrl_Result()
{

}
void CAABonderDlg::OnDBClickedGridResult(NMHDR* pNMHDR, LRESULT* pResult)
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1463);
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;

	int iRow = pNMGridView->iRow;
	int iCol = pNMGridView->iColumn;

	if ( iRow <= 0 || iRow > 2 )	return;
	CString sTemp="";
	if ( iRow == 1 || iRow == 2 )
	{
		sTemp.Format("»ý»ê·®À» ÃÊ±âÈ­ ÇÏ½Ã°Ú½À´Ï±î?");
		if ( askMsg(sTemp.GetBuffer()) == IDOK )
		{
	
			work.m_iCnt_Output = 0;
			work.m_iCnt_NG_Output = 0;
			// 20141103 LHC - »ý»ê·® ÃÊ±âÈ­½Ã T/T ÃÊ±âÈ­
			Task.m_dTime_Total = 0;
			Task.m_dTime_Epoxy = 0;
			Task.m_dTime_LaserDpm = 0;
			Task.m_dTime_FineAA = 0;
			Task.m_dTime_TotalAA = 0;
		}
	}
	
	dispGrid();

}
void CAABonderDlg::initGrid()
{
	
}
bool CAABonderDlg::g_CalcImageAlign()
{
    CString sLog = "";
    double dShiftX, dShiftY;
    double dRad, dAng;
    double dWidth, dHeight;
	double dCenterX = gMIUDevice.nWidth / 2;
	double dCenterY = gMIUDevice.nHeight / 2;	//(g_clModelData[nUnit].m_nHeight - 4) / 2;
    double m_Centerx = (Task.m_CirclePos_x[0] + Task.m_CirclePos_x[1] + Task.m_CirclePos_x[2] + Task.m_CirclePos_x[3]) / 4;
    double m_Centery = (Task.m_CirclePos_y[0] + Task.m_CirclePos_y[1] + Task.m_CirclePos_y[2] + Task.m_CirclePos_y[3]) / 4;

    model.m_oc_x = (gMIUDevice.nWidth / 2) - m_Centerx;
    model.m_oc_y = (gMIUDevice.nHeight / 2) - m_Centery;
	//
	//rotation
	double dSize_Cell = model.m_dSize_CCD_Cell;
	dWidth = (Task.m_CirclePos_x[3] - Task.m_CirclePos_x[2]) * (dSize_Cell / 1000);
	dHeight = (Task.m_CirclePos_y[3] - Task.m_CirclePos_y[2]) * (dSize_Cell / 1000);

	dRad = atan(dHeight / dWidth);
	dAng = dRad * 180.0f / M_PI;
	//
	Task.dLensRotation = dAng; 
	sLog.Format("ROTATION:%.3lf", Task.dLensRotation);
	putListLog(sLog);
	MESCommunication.m_dMesOC[0] = model.m_oc_x + dCenterX;
	MESCommunication.m_dMesOC[1] = model.m_oc_y + dCenterY;
	MESCommunication.m_dMesDeltaOC[0] = model.m_oc_x;//delta °¡ ÀÛÀº°ª
	MESCommunication.m_dMesDeltaOC[1] = model.m_oc_y;

    sLog.Format("oc x: %.2lf , oc y:%.2lf", model.m_oc_x, model.m_oc_y);
    putListLog(sLog);


    return true;
}


void CAABonderDlg::dispGrid()
{
	return;
	CString sTemp="";

	if ( work.m_iCnt_Output < 0 )		sTemp = "-";
	else								sTemp.Format("%d", work.m_iCnt_Output);
	m_clGridResult.SetItemText(1, 1, sTemp);

	if ( work.m_iCnt_NG_Output < 0 )	sTemp = "-";
	else								sTemp.Format("%d", work.m_iCnt_NG_Output);
	m_clGridResult.SetItemText(2, 1, sTemp);

	if ( Task.m_dTime_Total < 0 )		sTemp = "-";
	else								sTemp.Format("%.1lf", Task.m_dTime_Total/1000);
	m_clGridResult.SetItemText(3, 1, sTemp);
	
	if ( Task.m_dTime_Epoxy < 0 )		sTemp = "-";
	else								sTemp.Format("%.1lf", Task.m_dTime_Epoxy/1000);
	m_clGridResult.SetItemText(4, 1, sTemp);

	if ( Task.m_dTime_LaserDpm < 0 )	sTemp = "-";
	else								sTemp.Format("%.1lf", Task.m_dTime_LaserDpm/1000);
	m_clGridResult.SetItemText(5, 1, sTemp);

	if (Task.m_dTime_FineAA < 0 )		sTemp = "-";
	else								sTemp.Format("%.1lf", Task.m_dTime_FineAA/1000);
	m_clGridResult.SetItemText(6, 1, sTemp);

	if ( Task.m_dTime_TotalAA < 0 )		sTemp = "-";
	else								sTemp.Format("%.1lf", Task.m_dTime_TotalAA/1000);
	m_clGridResult.SetItemText(7, 1, sTemp);
	m_clGridResult.Invalidate();
}

BEGIN_EVENTSINK_MAP(CAABonderDlg, CDialogEx)

END_EVENTSINK_MAP()




bool CAABonderDlg::_calcImageAlignment()
{
	double dSize_Cell = model.m_dSize_CCD_Cell;

	double dShiftX, dShiftY;
	double dRad, dAng;
	double dWidth, dHeight;
	double dCenterX = gMIUDevice.nWidth/2;
	double dCenterY = gMIUDevice.nHeight/2;
	CString sLog; 

	  
	dShiftX = ((Task.m_CircleP[0].x+Task.m_CircleP[1].x+Task.m_CircleP[2].x+Task.m_CircleP[3].x)/4.0) - dCenterX;
	dShiftY = (((Task.m_CircleP[0].y + Task.m_CircleP[1].y + Task.m_CircleP[2].y + Task.m_CircleP[3].y) / 4.0) - dCenterY);
#if (____MODEL_NAME == M_TRINITY_B4)
	//dShiftX = 0;
	//dShiftY = 0;
#endif
    MandoInspLog.dOCResult[0] = dShiftX;
	MandoInspLog.dOCResult[1] = dShiftY;
	dShiftX *= dSize_Cell/1000.0f; 
	dShiftY *= dSize_Cell/1000.0f; 

	dWidth = (Task.m_CircleP[3].x - Task.m_CircleP[2].x) * (dSize_Cell / 1000);
	dHeight = (Task.m_CircleP[3].y - Task.m_CircleP[2].y) * (dSize_Cell / 1000);
	 
	dRad = atan(dHeight/dWidth);
	dAng = dRad * 180.0f / M_PI;


	if(fabs(dShiftX) > 3.0 || fabs(dShiftY) > 3.0 || fabs(dAng) > 20.0)//3.0)		// ÀÓÀÇ°ª...
	{

		if(!Task.bInsCenter)
		{
			errMsg2(Task.AutoFlag, "Image Center ÀÌ¹ÌÁö ¾ó¶óÀÎ º¸Á¤°ª ¸®¹ÌÆ® ¿¡·¯.");
			return false;
		}
		else
		{
			sLog.Format("Image Center ÃøÁ¤ error");		//Image Center ÃøÁ¤ error
			putListLog(sLog);
		}

	}
	 
	if(!Task.bInsCenter)
	{
		Task.m_dShift_IMG_X = dShiftX;
		Task.m_dShift_IMG_Y = dShiftY;
		Task.m_dShift_IMG_TH = dAng;
	}
	else
	{
		               
		int CX  = (Task.m_CircleP[0].x + Task.m_CircleP[1].x + Task.m_CircleP[2].x + Task.m_CircleP[3].x)/4;
		int CY  = (Task.m_CircleP[0].y + Task.m_CircleP[1].y + Task.m_CircleP[2].y + Task.m_CircleP[3].y)/4;
		dWidth  = (Task.m_CircleP[3].x - Task.m_CircleP[2].x);
		dHeight = (Task.m_CircleP[3].y - Task.m_CircleP[2].y);
		dRad = atan(dHeight/dWidth);
		dAng = dRad * 180.0 / M_PI;
  		sLog.Format("[°Ë»ç] Image Center [X=%f, Y=%f, T=%f]", dShiftX, dShiftY, dAng);
		putListLog(sLog);
	}

	Task.m_dShift_IMG_X = dShiftX;
	Task.m_dShift_IMG_Y = dShiftY;
	sLog.Empty();
	
	return true;
}


bool CAABonderDlg::_MotorMove_IMG_Align()//¼öµ¿ sfr
{ 
	double ep = myTimer(true);
	int dicX = 1;
	int dicY = 1;


	if (model.Tilt_Diretion[0] < 0)
	{
		dicX = -1;
	}
	if (model.Tilt_Diretion[1] < 0)
	{
		dicY = -1;
	}

	if (sysData.m_iDicChange == 0)
	{
		motor.MoveAxis(TITLE_MOTOR_X, REL, Task.m_dShift_IMG_X*dicX, sysData.fMotorSpeed[TITLE_MOTOR_X], sysData.fMotorAccTime[TITLE_MOTOR_X]);
		motor.MoveAxis(TITLE_MOTOR_Y, REL, Task.m_dShift_IMG_Y*dicY, sysData.fMotorSpeed[TITLE_MOTOR_Y], sysData.fMotorAccTime[TITLE_MOTOR_Y]);
	}
	else
	{
		motor.MoveAxis(TITLE_MOTOR_X, REL, Task.m_dShift_IMG_Y*dicY, sysData.fMotorSpeed[TITLE_MOTOR_X], sysData.fMotorAccTime[TITLE_MOTOR_X]);
		motor.MoveAxis(TITLE_MOTOR_Y, REL, Task.m_dShift_IMG_X*dicX, sysData.fMotorSpeed[TITLE_MOTOR_Y], sysData.fMotorAccTime[TITLE_MOTOR_Y]);
	}
	
 
	
	Sleep(10);

	while (1)
	{
		if ( ( motor.IsStopAxis(TITLE_MOTOR_X) && motor.GetInposition(TITLE_MOTOR_X) ) && 
			 ( motor.IsStopAxis(TITLE_MOTOR_Y) && motor.GetInposition(TITLE_MOTOR_Y) ) )
		{
			break;
		}
		else if ( myTimer(true)-ep > 1000 )
		{
			sLangChange.LoadStringA(IDS_STRING1356);
			errMsg2(Task.AutoFlag, sLangChange);
			return false;
		}
	}

	return true;
}

bool CAABonderDlg::_MotorMove_IMG_AlignTheta()
{
	double ep = myTimer(true);
	int dicTH = 1;  

	if (model.Tilt_Diretion[4] < 0)
	{
		dicTH = -1;
	}


	motor.MoveAxis(Motor_PCB_TH, REL, Task.m_dShift_IMG_TH * dicTH, sysData.fMotorSpeed[Motor_PCB_TH], sysData.fMotorAccTime[Motor_PCB_TH]);//¹æÇâ È®ÀÎÇØ¾ßµÊ
	Sleep(10);

	while (1)
	{
		if ( ( motor.IsStopAxis(Motor_PCB_TH)))// && motor.GetInposition(Motor_PCB_TH) ) )
		{
			break;
		}
		else if ( myTimer(true)-ep > 1000 )
		{
			sLangChange.LoadStringA(IDS_STRING1357);	//ÀÌ¹ÌÁö ¾ó¶óÀÎ PCB ThetaÃà º¸Á¤ ÀÌµ¿ ½Ã°£ ÃÊ°ú.
			errMsg2(Task.AutoFlag, sLangChange);
			return false;
		}
	}

	return true;
}


void CAABonderDlg::OnBnClickedRadioAlign()
{
	ctrlSubDlg(MAIN_DLG);
	m_iCurCamNo =0;
	setCamDisplay(0, 1);
	changeMainBtnColor(MAIN_DLG);
}


void CAABonderDlg::OnBnClickedRadioCcd2()
{
	ctrlSubDlg(MAIN_DLG);
	setCamDisplay(3, 1);
	changeMainBtnColor(MAIN_DLG);
}


void CAABonderDlg::OnStnClickedLabelStatusUsbLive()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1463);//"ÀÚµ¿ ¿îÀü Áß »ç¿ë ºÒ°¡"
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}
	
	pThread_MIUCheck = ::AfxBeginThread(Thread_MIUCheck, this);
	return;
}


int CAABonderDlg::_checkDecreaseSFR()
{
	int j=0;
	int iIndex_Start=0;
	int iIndex_Check = model.m_iCnt_Check_SFR;//ÆÄ¶ó¹ÌÅÍÃ¢ ¼³Á¤ , ÇöÀç3
	double sfrLimit = sysData.dMax_Sfr_Limit;
	if (sfrLimit < 0.01)
	{
		sfrLimit = 0.1;
	}
	if ( Task.m_bFlag_Decrease_SFR == true )
	{
		iIndex_Start = model.m_iCnt_Check_SFR;
		iIndex_Check = model.m_iCnt_Check_SFR * 2;
	}
	else{
		iIndex_Start = 0;
	}
	for (int i=iIndex_Start ; i<MAX_MTF_NO ; i++)
	{
//////////////////////////////////////////////////////////////////////////////////////////
			for (j=iIndex_Start ; j<iIndex_Check ; j++)
			{
				if (Task.SFR.fSfrN4[j][i] >= sfrLimit)//if ( Task.SFR.fSfrN4[j][i] >= 0.2 )
				{
					break;
				}
			}

			if (j == iIndex_Check){
				continue;
			}
			for (j=1 ; j<iIndex_Check ; j++)
			{
				if ( (Task.SFR.fSfrN4[j-1][i] - Task.SFR.fSfrN4[j][i]) < 0 )
					break;
			}

			if ( j == iIndex_Check ){
				return -1;
			}else{
				continue;
			}
	}

	return 1;
}


void CAABonderDlg::OnBnClickedCheckDist()
{
	m_bMeasureDist = !m_bMeasureDist;

	if ( m_bMeasureDist )
	{
		drawLine_MeasureDist(0);
	}
	else
	{
		vision.clearOverlay(m_iCurCamNo);
		vision.drawOverlay(m_iCurCamNo);
	}
}


void CAABonderDlg::drawLine_MeasureDist(int iMode)
{
	vision.clearOverlay(m_iCurCamNo);

	int iSx, iSy, iEx, iEy;
	int iSizeX, iSizeY;

	if ( m_iCurCamNo < 3 )
	{
		iSizeX = CAM_SIZE_X;
		iSizeY = CAM_SIZE_Y;
	}
	else
	{
		iSizeX = gMIUDevice.nWidth - 1;
		iSizeY = gMIUDevice.nHeight - 1;
	}

	iSx = 0;
	iSy = 0;
	iEx = iSizeX;
	iEy = iSizeY;

	if ( iMode == 0 )
	{
		m_iLine_Left	= (int)(iSizeX * 0.25 + 0.5);
		m_iLine_Top		= (int)(iSizeY * 0.25 + 0.5);
		m_iLine_Right	= (int)(iSizeX * 0.75 + 0.5);
		m_iLine_Bottom	= (int)(iSizeY * 0.75 + 0.5);
	}

	vision.linelist[m_iCurCamNo].addList(m_iLine_Left, iSy, m_iLine_Left, iEy, PS_SOLID, M_COLOR_RED);
	vision.linelist[m_iCurCamNo].addList(m_iLine_Right, iSy, m_iLine_Right, iEy, PS_SOLID, M_COLOR_RED);
	vision.linelist[m_iCurCamNo].addList(iSx, m_iLine_Top, iEx, m_iLine_Top, PS_SOLID, M_COLOR_MAGENTA);
	vision.linelist[m_iCurCamNo].addList(iSy, m_iLine_Bottom, iEx, m_iLine_Bottom, PS_SOLID, M_COLOR_MAGENTA);

	double dDistWidth, dDistHeight;
	int	iPos_Text;

	if ( m_iCurCamNo < 3 )
	{
		dDistWidth		= (m_iLine_Right - m_iLine_Left) * sysData.dCamResol[m_iCurCamNo].x;
		dDistHeight		= (m_iLine_Bottom - m_iLine_Top) * sysData.dCamResol[m_iCurCamNo].y;
		iPos_Text = 50;
	}
	else
	{
		dDistWidth		= (m_iLine_Right - m_iLine_Left) * model.m_dSize_CCD_Cell / 1000;
		dDistHeight		= (m_iLine_Bottom - m_iLine_Top) * model.m_dSize_CCD_Cell / 1000;
		iPos_Text = 150;
	}

	CString sTemp1, sTemp2;
	sTemp1.Format("Width  : %.3lf mm", dDistWidth);
	sTemp2.Format("Height : %.3lf mm", dDistHeight);

	vision.textlist[m_iCurCamNo].addList(150, 50, sTemp1, M_COLOR_RED, 16, 12, "Arial");
	vision.textlist[m_iCurCamNo].addList(150, 50+iPos_Text, sTemp2, M_COLOR_MAGENTA, 16, 12, "Arial");

	vision.drawOverlay(m_iCurCamNo);
}

int	CAABonderDlg::changeCursor_MeasureMode(CPoint point)
{
	int iRtn = -1;
	int iGap = 20;
	double dExpandFactorX;
	double dExpandFactorY;
	CPoint p;

	if (m_iCurCamNo<3)
	{
		dExpandFactorX = CAM_EXPAND_FACTOR_X;
		dExpandFactorY = CAM_EXPAND_FACTOR_Y;
		iGap = 20;
	}
	else
	{
		dExpandFactorX = (double)gMIUDevice.nWidth/SMALL_CCD_SIZE_X;
		dExpandFactorY = (double)gMIUDevice.nHeight/SMALL_CCD_SIZE_Y;
		// 20140905 Overlay Box À§Ä¡ ÀÌµ¿ ½Ã Box ¼±¿¡¼­ ¾ó¸¶³ª °¡±îÀÌ ÀÖ¾î¾ß ¸¶¿ì½º Ä¿¼­ ¸ð¾ç º¯°æ °Å¸® È®ÀÎ ¿ë..
		//iGap = 50;
		iGap = int(dExpandFactorX * 5);
	}
	point.x -= m_rectCamDispPos1.left;
	point.y -= m_rectCamDispPos1.top;

	p.x = (int)(point.x * dExpandFactorX + 0.5);
	p.y = (int)(point.y * dExpandFactorY + 0.5);

	if ( m_iLine_Left - iGap < p.x && m_iLine_Left + iGap > p.x )
	{
		::SetCursor(m_hCursor_Width);
		iRtn = 1;
	}
	else if ( m_iLine_Top - iGap < p.y && m_iLine_Top + iGap > p.y )
	{
		::SetCursor(m_hCursor_Height);
		iRtn = 2;
	}
	else if ( m_iLine_Right - iGap < p.x && m_iLine_Right + iGap > p.x )
	{
		::SetCursor(m_hCursor_Width);
		iRtn = 3;
	}
	else if ( m_iLine_Bottom - iGap < p.y && m_iLine_Bottom + iGap > p.y )
	{
		::SetCursor(m_hCursor_Height);
		iRtn = 4;
	}

	return iRtn;
}


void CAABonderDlg::DisableButton(bool AutorunFlag)
{
//	GetDlgItem(IDC_BUTTON_MAIN)->EnableWindow(!AutorunFlag);
//	GetDlgItem(IDC_BUTTON_MODEL)->EnableWindow(!AutorunFlag);
//	GetDlgItem(IDC_BUTTON_LENS)->EnableWindow(!AutorunFlag);
//	GetDlgItem(IDC_BUTTON_PCB)->EnableWindow(!AutorunFlag);
//	GetDlgItem(IDC_BUTTON_RESIN)->EnableWindow(!AutorunFlag);
//	GetDlgItem(IDC_BUTTON_CCD)->EnableWindow(!AutorunFlag);
//	GetDlgItem(IDC_BUTTON_MOTOR)->EnableWindow(!AutorunFlag);
//	GetDlgItem(IDC_BUTTON_IO)->EnableWindow(!AutorunFlag);
//	GetDlgItem(IDC_BUTTON_LIGHT)->EnableWindow(!AutorunFlag);
	GetDlgItem(IDC_BUTTON_EXIT)->EnableWindow(!AutorunFlag);

	GetDlgItem(IDC_BUTTON_ORIGIN)->EnableWindow(!AutorunFlag);
	GetDlgItem(IDC_BUTTON_READY)->EnableWindow(!AutorunFlag);
	GetDlgItem(IDC_BUTTON_AUTORUN)->EnableWindow(!AutorunFlag);
}



// readMode¿¡ µû¶ó 8M (Y475A), 13M (Y472A) ±¸ºÐ.
// 0 : 13M CCD. Address 0x3B02 - 0À¸·Î, 0x3B00 -1·Î Write ÈÄ 0x3B24ºÎÅÍ Read.. 
// 1 :  8M CCD. Address 0x01¹øºÎÅÍ 0x09±îÁö ¼ø¼­´ë·Î Read.. (0x05 Á¦¿Ü)

bool	CAABonderDlg::ReadSensorID(int readMode, CString strID)
{
	//memset(Task.ChipID, 0x00, 256);
	//sprintf_s(Task.ChipID, "(NULL)");
	bool rtnFlag = true;

	/*CString tmpStr="";
	

	if(readMode == 0)
	{
		unsigned int	nAddress = 0x3B24;
		unsigned int	nData = 0;
		int				errorCode;

		char ID_Data[256] = {0, };


		nAddress = 0x3B02;
		errorCode = MIUWriteRegister(0, (MIU_REG_TYPE)1, (unsigned short)nAddress, (unsigned short)0x00);
		if(errorCode)
		{
			tmpStr.Format("Error [0x3B02]: %d", errorCode);
			putListLog(tmpStr);

			rtnFlag = false;
		}


		Sleep(5);

		nAddress = 0x3B00;
		errorCode = MIUWriteRegister(0, (MIU_REG_TYPE)1, (unsigned short)nAddress, (unsigned short)0x01);
		if(errorCode)
		{
			tmpStr.Format("Error [0x3B00]: %d", errorCode);		
			putListLog(tmpStr);

			rtnFlag = false;
		}


		Sleep(5);

		nAddress = 0x3B24;
		errorCode = MIUReadRegister(0, (MIU_REG_TYPE)1, (unsigned short)nAddress, (unsigned short*)&nData);
		if(errorCode)
		{
			tmpStr.Format("Error [0x3B24]: %d", errorCode);		
			putListLog(tmpStr);

			rtnFlag = false;
		}
		else
		{
			ID_Data[0] = nData & 0xFF;
		}	


		nAddress = 0x3B25;
		errorCode = MIUReadRegister(0, (MIU_REG_TYPE)1, (unsigned short)nAddress, (unsigned short*)&nData);
		if(errorCode)
		{
			tmpStr.Format("Error [0x3B25]: %d", errorCode);		
			putListLog(tmpStr);
		}
		else
		{
			ID_Data[1] = nData & 0xFF;
		}	

		nAddress = 0x3B26;
		errorCode = MIUReadRegister(0, (MIU_REG_TYPE)1, (unsigned short)nAddress, (unsigned short*)&nData);
		if(errorCode)
		{
			tmpStr.Format("Error [0x3B26]: %d", errorCode);		
			putListLog(tmpStr);
		}
		else
		{
			ID_Data[2] = nData & 0xFF;
		}	

		nAddress = 0x3B27;
		errorCode = MIUReadRegister(0, (MIU_REG_TYPE)1, (unsigned short)nAddress, (unsigned short*)&nData);
		if(errorCode)
		{
			tmpStr.Format("Error [0x3B27]: %d", errorCode);		
			putListLog(tmpStr);
		}
		else
		{
			ID_Data[3] = nData & 0xFF;
		}	

		nAddress = 0x3B28;
		errorCode = MIUReadRegister(0, (MIU_REG_TYPE)1, (unsigned short)nAddress, (unsigned short*)&nData);
		if(errorCode)
		{
			tmpStr.Format("Error [0x3B28]: %d", errorCode);		
			putListLog(tmpStr);
		}
		else
		{
			ID_Data[4] = nData & 0xFF;
		}	

		nAddress = 0x3B29;
		errorCode = MIUReadRegister(0, (MIU_REG_TYPE)1, (unsigned short)nAddress, (unsigned short*)&nData);
		if(errorCode)
		{
			tmpStr.Format("Error [0x3B29]: %d", errorCode);		
			putListLog(tmpStr);
		}
		else
		{
			ID_Data[5] = nData & 0xFF;
		}	

		nAddress = 0x3B2A;
		errorCode = MIUReadRegister(0, (MIU_REG_TYPE)1, (unsigned short)nAddress, (unsigned short*)&nData);
		if(errorCode)
		{
			tmpStr.Format("Error [0x3B2A]: %d", errorCode);		
			putListLog(tmpStr);
		}
		else
		{
			ID_Data[6] = nData & 0xFF;
		}	

		nAddress = 0x3B2B;
		errorCode = MIUReadRegister(0, (MIU_REG_TYPE)1, (unsigned short)nAddress, (unsigned short*)&nData);
		if(errorCode)
		{
			tmpStr.Format("Error [0x3B2B]: %d", errorCode);		
			putListLog(tmpStr);
		}
		else
		{
			ID_Data[7] = nData & 0xFF;
		}	*/


		//int tmp;
		//for(int i=0; i<8; i++)
		//{
		//	tmp = (ID_Data[i]>>4) & 0x0f;
		//	if(tmp<10)
		//		Task.ChipID[i*2 + 0]	= '0' + tmp;
		//	else
		//		Task.ChipID[i*2 + 0]	= 'A' + (tmp-10);

		//	tmp = (ID_Data[i]) & 0x0f;
		//	if(tmp<10)
		//		Task.ChipID[i*2 + 1]	= '0' + tmp;
		//	else
		//		Task.ChipID[i*2 + 1]	= 'A' + (tmp-10);
		//}

		//Task.ChipID[16] = 0;
		//strID.Format("%s", Task.ChipID);

		//m_labelCCD_ID.SetText(Task.ChipID); 
		//m_labelCCD_ID.Invalidate();
	//}
	//else if(readMode == 1)				// 8M CCD. Address 0x01¹øºÎÅÍ 0x09±îÁö ¼ø¼­´ë·Î Read.. (0x05 Á¦¿Ü)
	//{
	//	unsigned int	nAddress = 0x01;
	//	unsigned int	nData = 0;

	//	char ID_Data[256] = {0, };
	//	
	//	int index = 0;
	//	for (int i=0; i<9; i++)
	//	{
	//		if(i==5)
	//			continue;
	//		
	//		MIUReadRegister(0, REG_IIC1, (unsigned short)nAddress+i, (unsigned short*)&nData);

	//		sprintf_s(ID_Data, "%02X", nData);

	//		Task.ChipID[index++]	= ID_Data[0];
	//		Task.ChipID[index++]	= ID_Data[1];
	//	}
	//	Task.ChipID[index] = 0;
	//	Task.ChipID[16] = 0;

	//	strID.Format("%s", Task.ChipID);

	//	m_labelCCD_ID.SetText(Task.ChipID); 
	//	m_labelCCD_ID.Invalidate();
	//}
	return rtnFlag;
}

void CAABonderDlg::AutoRunView(int curstate)		// Display¿ë
{// Á¤Áö:0 ÀÚµ¿¿îÀü:1 ÀÏ½ÃÁ¤Áö:2 ¿îÀüÁØºñ:3 
	m_btnReady.m_iStateBtn = 0;
	m_btnAutorun.m_iStateBtn = 0;
	m_btnPause.m_iStateBtn = 0;
	m_btnStop.m_iStateBtn = 0;
	m_btnNgOut.m_iStateBtn = 0;

	if(curstate == 0)		
		m_btnStop.m_iStateBtn = 1;	
	else if(curstate == 1)		
		m_btnAutorun.m_iStateBtn = 1;	
	else if(curstate == 2)		
	{
		m_btnAutorun.m_iStateBtn = 1;	
		m_btnPause.m_iStateBtn = 1;
	}
	else if(curstate == 3)
		m_btnReady.m_iStateBtn = 1;	

	m_btnReady.Invalidate();
	m_btnAutorun.Invalidate();
	m_btnPause.Invalidate();
	m_btnStop.Invalidate();
	m_btnNgOut.Invalidate();
}


////#define PI 3.141592653589793238462643383279
class AFX_EXT_CLASS CSFR
{
	public:
		//double Input_Spec[4]={0.0625,0.125,0.25,0.5};1/16 , 1/8, 1/4, 1/2 ÁÖÆÄ¼öº° SFR °ª Ãâ·Â
		//double Out_Sfr_Data[4];
		bool Get_SFR_Data(int nSfrX, int nSfrY, int nSizeX, int nSizeY, int nWidth, double *InImg, double *Input_Spec, double *Out_Sfr_Data,int Sfr_Count );


};

//extern "C" __declspec(dllimport) int BlackSpot(unsigned char* src, int Width, int Height, BlackSpotInfo config, BlackSpotResult* retResult, CString sMainpath, CString FolderName, BOOL flag);
//extern "C" __declspec(dllimport) int Blemish(unsigned char* src, int Width, int Height, BlemishInfo config, BlemishResult* retResult, CString sMainpath, CString FolderName);
//extern "C" __declspec(dllimport) int BlackSpot_Cluster(unsigned char* src, int Width, int Height, BlackSpotInfo config, BlackSpotResult* retResult, CString sMainpath, CString FolderName, BOOL flag);

BOOL RAWtoRGB3(UINT32 Width, UINT32 Height, UINT8 Format_Num,LPSTR Image_Buf,COLORREF *C_RGB)
{
	
	UINT8				r,g1,g2,g,b;
	LPSTR Raw_Data = Image_Buf;
   
	switch(Format_Num)
	{
		case 0:
			//BGBG
			//GRGR
			for( UINT32 j=0; j<Height/2; j++)
			{
				
				for(UINT32 i=0; i<Width/2;i++)
				{
					b = *(Raw_Data);
					g1 = *(Raw_Data+1);
					g2 = *(Raw_Data+Width);
					r = *(Raw_Data+Width+1);
					g=(g1+g2)/2;

					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;


					*(C_RGB+(j*2)*Width+(i*2)) = RGB(b, g, r);

					
					g2 = *(Raw_Data+Width);
					r = *(Raw_Data+Width+1);
					b = *(Raw_Data+Width*2);
					g1 = *(Raw_Data+Width*2+1);
					g =(g1+g2)/2;

					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;

					*(C_RGB+(j*2+1)*Width+(i*2)) = RGB(b, g, r);
					
					
					Raw_Data++;
					
					g1 = *(Raw_Data);
					b = *(Raw_Data+1);
					r = *(Raw_Data+Width);
					g2 = *(Raw_Data+Width+1);
					g=(g1+g2)/2;
					
					
					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;
					
					*(C_RGB+(j*2)*Width+(i*2+1)) = RGB(b, g, r);

					r = *(Raw_Data+Width);
					g2 = *(Raw_Data+Width+1);
					g1 = *(Raw_Data+Width*2);
					b = *(Raw_Data+Width*2+1);
					
					g=(g1+g2)/2;

					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;

					*(C_RGB+(j*2+1)*Width+(i*2+1)) = RGB(b, g, r);
					Raw_Data++;
				}
				if(j!=Height-1)
				{
					
					Raw_Data += Width;
				}
				
			}
			
			for( UINT32 j=0; j<Height; j++)
			{
				*(C_RGB+(j*Width+Width-1))=*(C_RGB+(j*Width+Width-2));
			}
			for( UINT32 i=0; i<Width; i++)
			{
				*(C_RGB+((Height-1)*Width+i))=*(C_RGB+((Height-2)*Width+i));
			}
			break;
		case 1:
			//GRGR
			//BGBG
			for( UINT32 j=0; j<Height/2; j++)
			{
				
				for(UINT32 i=0; i<Width/2;i++)
				{
					g1 = *(Raw_Data);
					r = *(Raw_Data+1);
					b = *(Raw_Data+Width);
					g2 = *(Raw_Data+Width+1);
					g=(g1+g2)/2;

					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;


					*(C_RGB+(j*2)*Width+(i*2)) = RGB(b, g, r);

					
					b = *(Raw_Data+Width);
					g2 = *(Raw_Data+Width+1);
					g1 = *(Raw_Data+Width*2);
					r = *(Raw_Data+Width*2+1);
					g =(g1+g2)/2;

					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;

					*(C_RGB+(j*2+1)*Width+(i*2)) = RGB(b, g, r);
					
					
					Raw_Data++;
					
					r = *(Raw_Data);
					g1 = *(Raw_Data+1);
					g2 = *(Raw_Data+Width);
					b = *(Raw_Data+Width+1);
					g=(g1+g2)/2;
					
					
					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;
					
					*(C_RGB+(j*2)*Width+(i*2+1)) = RGB(b, g, r);

					g2 = *(Raw_Data+Width);
					b = *(Raw_Data+Width+1);
					r = *(Raw_Data+Width*2);
					g1 = *(Raw_Data+Width*2+1);
					
					g=(g1+g2)/2;

					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;

					*(C_RGB+(j*2+1)*Width+(i*2+1)) = RGB(b, g, r);
					Raw_Data++;
				}
				if(j!=Height-1)
				{
					
					Raw_Data += Width;
				}
				
			}
			
			for( UINT32 j=0; j<Height; j++)
			{
				*(C_RGB+(j*Width+Width-1))=*(C_RGB+(j*Width+Width-2));
			}
			for( UINT32 i=0; i<Width; i++)
			{
				*(C_RGB+((Height-1)*Width+i))=*(C_RGB+((Height-2)*Width+i));
			}
			break;
		case 2:
			//GBGB
			//RGRG
			for( UINT32 j=0; j<Height/2; j++)
			{
				
				for(UINT32 i=0; i<Width/2;i++)
				{
					g1 = *(Raw_Data);
					b = *(Raw_Data+1);
					r = *(Raw_Data+Width);
					g2 = *(Raw_Data+Width+1);
					g=(g1+g2)/2;

					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;


					*(C_RGB+(j*2)*Width+(i*2)) = RGB(b, g, r);

					
					r = *(Raw_Data+Width);
					g2 = *(Raw_Data+Width+1);
					g1 = *(Raw_Data+Width*2);
					b = *(Raw_Data+Width*2+1);
					g =(g1+g2)/2;

					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;

					*(C_RGB+(j*2+1)*Width+(i*2)) = RGB(b, g, r);
					
					
					Raw_Data++;
					
					b = *(Raw_Data);
					g1 = *(Raw_Data+1);
					g2 = *(Raw_Data+Width);
					r = *(Raw_Data+Width+1);
					g=(g1+g2)/2;
					
					
					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;
					
					*(C_RGB+(j*2)*Width+(i*2+1)) = RGB(b, g, r);

					g2 = *(Raw_Data+Width);
					r = *(Raw_Data+Width+1);
					b = *(Raw_Data+Width*2);
					g1 = *(Raw_Data+Width*2+1);
					
					g=(g1+g2)/2;

					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;

					*(C_RGB+(j*2+1)*Width+(i*2+1)) = RGB(b, g, r);
					Raw_Data++;
				}
				if(j!=Height-1)
				{
					
					Raw_Data += Width;
				}
				
			}
			
			for( UINT32 j=0; j<Height; j++)
			{
				*(C_RGB+(j*Width+Width-1))=*(C_RGB+(j*Width+Width-2));
			}
			for( UINT32 i=0; i<Width; i++)
			{
				*(C_RGB+((Height-1)*Width+i))=*(C_RGB+((Height-2)*Width+i));
			}
			break;
		default :
			//RGRG
			//GBGB
			for( UINT32 j=0; j<Height/2; j++)
			{
				
				for(UINT32 i=0; i<Width/2;i++)
				{
					r = *(Raw_Data);
					g1 = *(Raw_Data+1);
					g2 = *(Raw_Data+Width);
					b = *(Raw_Data+Width+1);
					g=(g1+g2)/2;

					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;


					*(C_RGB+(j*2)*Width+(i*2)) = RGB(b, g, r);

					
					g2 = *(Raw_Data+Width);
					b = *(Raw_Data+Width+1);
					r = *(Raw_Data+Width*2);
					g1 = *(Raw_Data+Width*2+1);
					g=(g1+g2)/2;

					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;

					*(C_RGB+(j*2+1)*Width+(i*2)) = RGB(b, g, r);
					
					
					Raw_Data++;
					
					g1 = *(Raw_Data);
					r = *(Raw_Data+1);
					b = *(Raw_Data+Width);
					g2 = *(Raw_Data+Width+1);
					g=(g1+g2)/2;
					
					
					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;
					
					*(C_RGB+(j*2)*Width+(i*2+1)) = RGB(b, g, r);

					b = *(Raw_Data+Width);
					g2 = *(Raw_Data+Width+1);
					g1 = *(Raw_Data+Width*2);
					r = *(Raw_Data+Width*2+1);
					
					g=(g1+g2)/2;

					if (r <= 0) r = 0;
					if (g <= 0) g = 0;
					if (b <= 0) b = 0;
					if (r >= 255) r = 255;
					if (g >= 255) g = 255;
					if (b >= 255) b = 255;

					*(C_RGB+(j*2+1)*Width+(i*2+1)) = RGB(b, g, r);
					Raw_Data++;
				}
				if(j!=Height-1)
				{
					
					Raw_Data += Width;
				}
				
			}
			
			for( UINT32 j=0; j<Height; j++)
			{
				*(C_RGB+(j*Width+Width-1))=  *(C_RGB+(j*Width+Width-2));
			}
			for( UINT32 i=0; i<Width; i++)
			{
				*(C_RGB+((Height-1)*Width+i))=*(C_RGB+((Height-2)*Width+i));
			}
			break;
	}
	return true;
}

BOOL PCI_Convert_Data(UINT32 Width, UINT32 Height, LPSTR Image_Buf,UINT8 *Rtn_ImageBuf)
{
	
	int z=0;
	
	for( UINT32 i=0; i<	Width *Height/4;  i++)
	{

			Rtn_ImageBuf[z++] = (unsigned char)((unsigned char)Image_Buf[i*5]<<2&0xFC| (unsigned char)(Image_Buf[i*5+4]>>6)&0x03);
			Rtn_ImageBuf[z++] = (unsigned char)(Image_Buf[i*5]>>6)&0x03;

			Rtn_ImageBuf[z++] =  (unsigned char)((unsigned char)Image_Buf[i*5+1]<<2&0xFC|(unsigned char)(Image_Buf[i*5+4]>>4)&0x03);
			Rtn_ImageBuf[z++] = (unsigned char)(Image_Buf[i*5+1]>>6)&0x03;

			Rtn_ImageBuf[z++] = (unsigned char)( (unsigned char)Image_Buf[i*5+2]<<2&0xFC|(unsigned char)(Image_Buf[i*5+4]>>2)&0x03);
			Rtn_ImageBuf[z++] = (unsigned char)(Image_Buf[i*5+2]>>6)&0x03;

			Rtn_ImageBuf[z++] = (unsigned char)((unsigned char) Image_Buf[i*5+3]<<2&0xFC|(unsigned char)(Image_Buf[i*5+4])&0x03);
			Rtn_ImageBuf[z++] =(unsigned char)(Image_Buf[i*5+3]>>6)&0x03;
			
	}

	return true;
}


bool CAABonderDlg::checkDarkDefect_PreUV()		// Dark°Ë»ç °á°ú È®ÀÎ¿ë.	// Defect Pixel°Ë»ç ¿ÜºÎDLL »ç¿ë
{
	return true;
}

bool CAABonderDlg::checkLightDefect_PreUV()		// ÃÖÁ¾ °á°ú È®ÀÎ¿ë.
{
	return true;
}


bool CAABonderDlg::func_MTF(BYTE* ChartRawImage, bool bAutoMode, int dindex)
{  
	vision.drawOverlay(CCD);
	char szTmp[256];
	double mtfOffset = 0.0;
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString sTemp="";
	if(!bAutoMode)
	{
		sTemp.Format(" -------------MTF ¼öµ¿ °Ë»ç");	
	}else
	{
		sTemp.Format(" -------------MTF °Ë»ç [step:%d]", Task.PCBTask);	
	}
	putListLog(sTemp);
	if(sysData.m_iProductComp == 1)
	{
		putListLog(" --MTF UV AFTER INSP");
	}else
	{
		if(sysData.m_iMTFUvInsp==1)
		{
			putListLog(" --MTF UV BEFORE INSP");
		}else
		{
			putListLog(" --MTF UV AFTER INSP");
		}
	}
	
	
	
	vision.clearOverlay(CCD);

	MandoInspLog.sBarcodeID = Task.ChipID;


	IplImage *cvImgMtf = cvCreateImage(cvSize(gMIUDevice.nWidth, gMIUDevice.nHeight), 8, 3); // bmp for display
	cvImgMtf->imageData = (char*)MIU.m_pFrameBMPBuffer;
	Jpg_ImageSave(cvImgMtf,MTF_JPG);
    CString sLog;
    bool bResult=true;
    double dSFR[COMMON_MTF_INSP_CNT];//MTF_INSP_CNT];
     
    CPoint Center;
    int dic = 0;
    bool sfrRt = true;
    int nBlackLevel = 0;
    Task.getROI();				// ¿øÇü ¸¶Å© À§Ä¡ ÀÎ½Ä..
    vision.MilBufferUpdate();
    if (Task.getROI_SFR(MTF_INSP) == false)
    {
		cvReleaseImage(&cvImgMtf);
        return false;
    }
	g_CalcImageAlign();
    Center.x = (Task.m_CircleP[0].x+Task.m_CircleP[1].x+Task.m_CircleP[2].x+Task.m_CircleP[3].x)/4;
    Center.y =(Task.m_CircleP[0].y+Task.m_CircleP[1].y+Task.m_CircleP[2].y+Task.m_CircleP[3].y)/4; 

    double m_Centerx = (Task.m_CirclePos_x[0]+Task.m_CirclePos_x[1]+Task.m_CirclePos_x[2]+Task.m_CirclePos_x[3])/4;
    double m_Centery =(Task.m_CirclePos_y[0]+Task.m_CirclePos_y[1]+Task.m_CirclePos_y[2]+Task.m_CirclePos_y[3])/4; 

    model.m_oc_x = m_Centerx - (gMIUDevice.nWidth / 2);//(gMIUDevice.nWidth/2) - m_Centerx;		//220812
	model.m_oc_y = m_Centery - (gMIUDevice.nHeight / 2);//((gMIUDevice.nHeight / 2) - m_Centery);

	model.m_LogOC_X = m_Centerx;// model.m_oc_x + (gMIUDevice.nWidth / 2);
	model.m_LogOC_Y = m_Centery;// model.m_oc_y + (gMIUDevice.nHeight / 2);
    model.m_LogOC_DelatX = model.m_oc_x;
    model.m_LogOC_DelatY = model.m_oc_y;
	double mTestLinePulse = model.m_Line_Pulse;
//
	sTemp.Format("LinePulse = [%.4f]", mTestLinePulse);
	pFrame->putListLog(sTemp);

    //-----------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------
    int nWidth = gMIUDevice.imageItp->width;
    int nHeight = gMIUDevice.imageItp->height;
    int iSizeX = model.m_iSize_ROI_X + 1;
    int iSizeY = model.m_iSize_ROI_Y + 1;
    std::shared_ptr<CACMISResolutionSFR> m_pSFRProc = std::make_shared<CACMISResolutionSFR>(); 
    POINT ptROICenter[COMMON_MTF_INSP_CNT];//MTF_INSP_CNT];
	for (int i = 0; i < model.mGlobalSmallChartCount; i++)//for (int i = 0; i < MTF_INSP_CNT; i++)
    {
		ptROICenter[i].x = Task.SFR._64_Sfr_Rect[i].left + (iSizeX / 2);
		ptROICenter[i].y = Task.SFR._64_Sfr_Rect[i].top + (iSizeY / 2);
        //
        Task.m_vDirection[i] = model.m_MTF_Direction[i]; // 0: Vertical, 1: Horizontal
		Task.m_vFrquency[i] = mTestLinePulse;//0.225

        Task.m_vOffset[i] = 0.0;
        Task.m_vSFR[i] = 0.0;

        Task.m_vThreshold[i] = 0.0;
        Task.m_vROI[i].ptCenter.x = ptROICenter[i].x;
        Task.m_vROI[i].ptCenter.y = ptROICenter[i].y;
        Task.m_vROI[i].nROIWidth = model.m_MTF_Direction[i] == 1 ? iSizeX : iSizeY;
        Task.m_vROI[i].nROIHeight = model.m_MTF_Direction[i] == 0 ? iSizeY : iSizeX;
    }
    TSFRSpec m_stSFRSpec;
    m_stSFRSpec.tSFRConfig.nMaxROIWidth = iSizeX;
    m_stSFRSpec.tSFRConfig.nMaxROIHeight = iSizeY;
    m_stSFRSpec.tSFRConfig.dMaxEdgeAngle = 45.0;
    m_stSFRSpec.tSFRConfig.dPixelSize = model.m_dSize_CCD_Cell;// 4.2;

	m_stSFRSpec.tSFRConfig.eAlgorithmType = ESFRAlgorithm_ISO12233;	//ESFRAlgorithm_VNE
	m_stSFRSpec.tSFRConfig.eAlgorithmMethod = ESFRMethod_Freq2SFR;

	////m_stSFRSpec.tSFRConfig.eFrequencyUnit = ESFRFreq_LinePairPerMilliMeter;	//½ÊÀÇÀÚ¸®
	m_stSFRSpec.tSFRConfig.eFrequencyUnit = ESFRFreq_CyclePerPixel;//¼Ò¼öÁ¡
    m_stSFRSpec.eSFRDeltaAlgorithmType = ESFRDelta_Diff;
    m_stSFRSpec.dEdgeDir = Task.m_vDirection.data();
    m_stSFRSpec.dFrequency = Task.m_vFrquency.data();
    m_stSFRSpec.dSFR = Task.m_vSFR.data();
    m_stSFRSpec.dThreshold = Task.m_vThreshold.data();
    m_stSFRSpec.dGamma = 1.0;
    m_stSFRSpec.tROI.dOffset = Task.m_vOffset.data();
    m_stSFRSpec.tROI.eROIType = ROIType_POINT;
    m_stSFRSpec.tROI.pROIData = Task.m_vROI.data();
    m_stSFRSpec.tROI.ROICount = model.mGlobalSmallChartCount;//MTF_INSP_CNT;
    m_stSFRSpec.tDelataSpec = NULL;

	//bool bRet = m_pSFRProc->Inspect(ChartRawImage, nWidth, nHeight, m_stSFRSpec,
		//gMIUDevice.dTDATASPEC_n.eDataFormat, gMIUDevice.dTDATASPEC_n.eOutMode, SENSORTYPE_RGGB_G_ONLY, nBlackLevel);//SENSORTYPE_RGGB_G_ONLY, nBlackLevel); 220718

	bool bRet = m_pSFRProc->Inspect(ChartRawImage, nWidth, nHeight, m_stSFRSpec,
		gMIUDevice.dTDATASPEC_n.eDataFormat, gMIUDevice.dTDATASPEC_n.eOutMode, gMIUDevice.dTDATASPEC_n.eSensorType, nBlackLevel, false, false, gMIUDevice.dTDATASPEC_n.eDemosaicMethod);//gMIUDevice.dTDATASPEC_n.eSensorType


	//gMIUDevice.dTDATASPEC_n.eDataFormat, gMIUDevice.dTDATASPEC_n.eOutMode, gMIUDevice.dTDATASPEC_n.eSensorType, nBlackLevel);
    // 
    int sfrMax = m_pSFRProc->GetMaxResolutionCount();
    float sfrValue = 0.0;

    //bool bRtn = Task.getROI_SFR(MTF_INSP);				// »ç°¢Çü Box À§Ä¡ ÀÎ½Ä..
    //if (!bRtn)
    //{
    //    if (Task.AutoFlag == 1) {
    //        saveInspImage(AA_CIRCLE_NG_IMAGE_SAVE, Task.m_iCnt_Step_AA_Total);
    //    }
    //    sLog.Format("[SFR ÃøÁ¤] ¿øÇü ¸¶Å© ÀÎ½Ä ½ÇÆÐ");
    //    errMsg2(Task.AutoFlag, sLog);
    //    return -1;
    //}
	double _offset = 0.0;
    for (int i = 0; i < sfrMax; i++)
    {
        const TSFRROIResult* pROIResult = m_pSFRProc->GetSFRROIResult(i);
		sfrValue = pROIResult->dFinalResult[0];

		//sfrValue = pROIResult->dFinalResult[0] * 0.01;		//M_DMS3

        
        if (pROIResult)
        {
            //	sfrRt = m_pChartProc->CalcSFR(Task.SFR._64_Sfr_Rect[i],dic, dFrequency, 1,dSfrFinalResult[i]);
           // sprintf_s(szTmp, "%d", i);
            //vision.textlist[CCD].addList(Task.SFR._64_Sfr_Rect[i].left, Task.SFR._64_Sfr_Rect[i].top, szTmp, M_COLOR_LIGHT_GREEN, 17, 7, "Arial");
            //vision.boxlist[CCD].addList(Task.SFR._64_Sfr_Rect[i], PS_SOLID, M_COLOR_CYAN);
            if (!_finite(sfrValue) || sfrValue < 0.0)
            {
                dSFR[i]  = 0.0;
			}
			else
			{
				dSFR[i] = (float)(sfrValue);
				model.m_LogSfr[i] = sfrValue;

                

            }
            if(Task.PCBTask == 65000 || Task.PCBTask == 60200)
			{
                MandoInspLog.dMTF_PreUV[i] = model.m_LogSfr[i];// dSFR[i];
            }
            else
            {
                MandoInspLog.dMTF_PostUV[i] = model.m_LogSfr[i]; //dSFR[i];
                		
            }
			MESCommunication.m_dMesMTF[i] = model.m_LogSfr[i];
            //
            //
        }

    }
	vision.drawOverlay(CCD);
	//CString MtfPos[9] = { "Center" , "04TL" , "04TR" , "04BL" , "04BR" , "08TL" , "08TR" , "08BL" , "08BR" };

	double dMTFTemp = 0;
	double dMTFSpecTemp = 0;
	
	double ocMinSpec = 0.0;
	double ocMaxSpec = 0.0;

	if(sysData.m_iProductComp == 1)
	{
		ocMinSpec = MandoSfrSpec.INSP_Procmode_OC_Min_Spec;
		ocMaxSpec = MandoSfrSpec.INSP_Procmode_OC_Max_Spec;
	}else
	{
		ocMinSpec = MandoSfrSpec.INSP_AAmode_OC_Min_Spec;
		ocMaxSpec = MandoSfrSpec.INSP_AAmode_OC_Max_Spec;
	}

	int sfrIndex = 0;
	
	if (Task.PCBTask == 65000)	// UVÀü MTF¸¸ °á°ú ÆÇ´Ü¡Ú¡Ú¡Ú¡Ú¡Ú¡Ú¡Ú¡Ú¡Ú¡Ú
	{
		dMTFTemp = (MandoInspLog.dMTF_PreUV[0] + MandoInspLog.dMTF_PreUV[1] + MandoInspLog.dMTF_PreUV[2] + MandoInspLog.dMTF_PreUV[3]) / 4;
		double dCenter = (MandoInspLog.dMTF_PreUV[0] + MandoInspLog.dMTF_PreUV[1] + MandoInspLog.dMTF_PreUV[2] + MandoInspLog.dMTF_PreUV[3]) / 4;


		/*double dTL05 = (MandoInspLog.dMTF_PreUV[4] + MandoInspLog.dMTF_PreUV[5]) / 2;
		double dTR05 = (MandoInspLog.dMTF_PreUV[6] + MandoInspLog.dMTF_PreUV[7]) / 2;
		double dBL05 = (MandoInspLog.dMTF_PreUV[8] + MandoInspLog.dMTF_PreUV[9]) / 2;
		double dBR05 = (MandoInspLog.dMTF_PreUV[10] + MandoInspLog.dMTF_PreUV[11]) / 2;
		double dTL07 = (MandoInspLog.dMTF_PreUV[12] + MandoInspLog.dMTF_PreUV[13]) / 2;
		double dTR07 = (MandoInspLog.dMTF_PreUV[14] + MandoInspLog.dMTF_PreUV[15]) / 2;
		double dBL07 = (MandoInspLog.dMTF_PreUV[16] + MandoInspLog.dMTF_PreUV[17]) / 2;
		double dBR07 = (MandoInspLog.dMTF_PreUV[18] + MandoInspLog.dMTF_PreUV[19]) / 2;*/

		
		if (dindex == 1)
		{
			sfrIndex = 3;
		}
		//=====================================================================================================================================================================

		for (int i = 0; i<model.mGlobalSmallChartCount; i++)//for (int i = 0; i<MTF_INSP_CNT; i++)
		{
			dMTFSpecTemp = MandoSfrSpec.dAASFR_Spec[i][sfrIndex];
			if (MandoInspLog.dMTF_PreUV[i] < dMTFSpecTemp)
			{
				sTemp.Format("MTF UV BEFORE == [%d]   Spec NG: %lf (Spec:%.2f)", i, MandoInspLog.dMTF_PreUV[i], dMTFSpecTemp);
				//! Main DisplayÈ­¸é Overlay NG List
				MandoInspLog.sDispNG[MandoInspLog.iNGCnt].Format("MTF UV BEFORE == [%d]: %.3f (Spec:%.2f)", i, MandoInspLog.dMTF_PreUV[i], dMTFSpecTemp);
				MandoInspLog.iNGCnt++;
				bResult = false;
				pFrame->putListLog(sTemp);
				MandoInspLog.sNGList += sTemp;
			}
			else
			{
				sTemp.Format("MTF UV BEFORE == [%d] %lf (Spec:%.2f)", i, MandoInspLog.dMTF_PreUV[i], dMTFSpecTemp);
				pFrame->putListLog(sTemp);
			}
		}

		// 
		
		
		if(sysData.m_iMTFUvInsp==1 && sysData.m_iProductComp == 0)//uvÀü °Ë»ç ¼±ÅÃ (1)
		{

			if(model.m_oc_x >ocMaxSpec || model.m_oc_x < ocMinSpec)
			{
				bResult=false;
				MandoInspLog.sDispNG[MandoInspLog.iNGCnt].Format("UVBefore_OC_X Spec:%.1f(INSP:%.2f~%.2f)",model.m_oc_x, ocMinSpec, ocMaxSpec);
				MandoInspLog.iNGCnt++;
				pFrame->putListLog(sTemp);
				sTemp.Format("[UVBefore_OC_X Spec %.1f]", model.m_oc_x);
				MandoInspLog.sNGList += sTemp;
			}
			if(model.m_oc_y > ocMaxSpec || model.m_oc_y < ocMinSpec)
			{
				bResult=false;
				MandoInspLog.sDispNG[MandoInspLog.iNGCnt].Format("UVBefore_OC_Y Spec:%.1f(INSP:%.2f~%.2f)",model.m_oc_y, ocMinSpec, ocMaxSpec);
				MandoInspLog.iNGCnt++;
				pFrame->putListLog(sTemp);
				sTemp.Format("[UVBefore_OC_Y Spec %.1f]", model.m_oc_y);
				MandoInspLog.sNGList += sTemp;
			} 
		}else
		{
			//AA Retry ¸¦ À§ÇØ °Ë»ç  , UV ÈÄ °Ë»çÀÏ¶§´Â PassµÅ¼­ Ãß°¡ÇÔ
			/*if(dCenter<MandoSfrSpec.dAASFR_Spec[0][sfrIndex] || dTL05<MandoSfrSpec.dAASFR_Spec[4][sfrIndex] || dTR05<MandoSfrSpec.dAASFR_Spec[6][sfrIndex]
			|| dBL05<MandoSfrSpec.dAASFR_Spec[8][sfrIndex] || dBR05<MandoSfrSpec.dAASFR_Spec[10][sfrIndex])
			{
				return false;
			}*/
		}
		//==================================================================================================================================
	}
	else
	{ 	// UVÈÄ MTF¸¸ °á°ú ÆÇ´Ü¡Ú¡Ú¡Ú¡Ú¡Ú¡Ú¡Ú¡Ú¡Ú¡Ú
		sfrIndex = 1;
		if(sysData.m_iProductComp == 1)
		{
			sfrIndex = 2;
		}
		if (dindex == 1)
		{
			sfrIndex = 3;
		}
		//uvÈÄ °³º°
		for (int i = 0; i<model.mGlobalSmallChartCount; i++)
		{
			dMTFSpecTemp = MandoSfrSpec.dAASFR_Spec[i][sfrIndex];
			if (MandoInspLog.dMTF_PostUV[i] < dMTFSpecTemp)
			{
				sTemp.Format("MTF UV AFTER [%d] Spec NG: %lf (Spec:%.2f)", i, MandoInspLog.dMTF_PostUV[i], dMTFSpecTemp);
				//! Main DisplayÈ­¸é Overlay NG List
				MandoInspLog.sDispNG[MandoInspLog.iNGCnt].Format("MTF UV AFTER [%d]: %.3f (Spec:%.2f)", i, MandoInspLog.dMTF_PostUV[i], dMTFSpecTemp);
				MandoInspLog.iNGCnt++;
				bResult = false;
				pFrame->putListLog(sTemp);
				MandoInspLog.sNGList += sTemp;
			}
			else
			{
				sTemp.Format("MTF UV AFTER [%d] %lf (Spec:%.2f)", i, MandoInspLog.dMTF_PostUV[i], dMTFSpecTemp);
				pFrame->putListLog(sTemp);
			}
		}
		//for(int i=0;i<MTF_INSP_CNT;i++)
		//{
		//	if(MandoInspLog.dMTF_PostUV[i]<MandoSfrSpec.dAASFR_Spec[i][sfrIndex])
		//	{
		//		sTemp.Format("MTF UV AFTER == [%d]   Spec NG: %.2f(Spec:%.2f)",i, MandoInspLog.dMTF_PostUV[i], MandoSfrSpec.dAASFR_Spec[i][sfrIndex]);
		//		//! Main DisplayÈ­¸é Overlay NG List
		//		MandoInspLog.sDispNG[MandoInspLog.iNGCnt].Format("MTF ¿ÏÁ¦Ç°[%d]: %.2f(Spec:%.2f)",i, MandoInspLog.dMTF_PostUV[i],MandoSfrSpec.dAASFR_Spec[i][sfrIndex]);
		//		MandoInspLog.iNGCnt++;
		//		bResult=false;
		//		pFrame->putListLog(sTemp);
		//		MandoInspLog.sNGList += sTemp;
		//	}else
		//	{
		//		sTemp.Format("MTF UV AFTER == [%d] %.2f(Spec:%.2f)", i, MandoInspLog.dMTF_PostUV[i] , MandoSfrSpec.dAASFR_Spec[i][sfrIndex]);
		//		pFrame->putListLog(sTemp);
		//	}
		//
		//}
		
		//=================uvÈÄ
		

		//double dCenter_Post =(MandoInspLog.dMTF_PostUV[0]+MandoInspLog.dMTF_PostUV[1]+MandoInspLog.dMTF_PostUV[2]+MandoInspLog.dMTF_PostUV[3])/4;
		
		//

//		double dTL_Post05 = (MandoInspLog.dMTF_PostUV[4] + MandoInspLog.dMTF_PostUV[5] + MandoInspLog.dMTF_PostUV[6] + MandoInspLog.dMTF_PostUV[7]) / 4;
//		double dTR_Post05 = (MandoInspLog.dMTF_PostUV[8] + MandoInspLog.dMTF_PostUV[9] + MandoInspLog.dMTF_PostUV[10] + MandoInspLog.dMTF_PostUV[11]) / 4;
//		double dBL_Post05 = (MandoInspLog.dMTF_PostUV[12] + MandoInspLog.dMTF_PostUV[13] + MandoInspLog.dMTF_PostUV[14] + MandoInspLog.dMTF_PostUV[15]) / 4;
//		double dBR_Post05 = (MandoInspLog.dMTF_PostUV[16] + MandoInspLog.dMTF_PostUV[17] + MandoInspLog.dMTF_PostUV[18] + MandoInspLog.dMTF_PostUV[19]) / 4;
//		double dTL_Post07 = (MandoInspLog.dMTF_PostUV[20] + MandoInspLog.dMTF_PostUV[21] + MandoInspLog.dMTF_PostUV[22] + MandoInspLog.dMTF_PostUV[23]) / 4;
//		double dTR_Post07 = (MandoInspLog.dMTF_PostUV[24] + MandoInspLog.dMTF_PostUV[25] + MandoInspLog.dMTF_PostUV[26] + MandoInspLog.dMTF_PostUV[27]) / 4;
//		double dBL_Post07 = (MandoInspLog.dMTF_PostUV[28] + MandoInspLog.dMTF_PostUV[29] + MandoInspLog.dMTF_PostUV[30] + MandoInspLog.dMTF_PostUV[31]) / 4;
//		double dBR_Post07 = (MandoInspLog.dMTF_PostUV[32] + MandoInspLog.dMTF_PostUV[33] + MandoInspLog.dMTF_PostUV[34] + MandoInspLog.dMTF_PostUV[35]) / 4;
//

		//=====================================================================================================================================================================
		

		//if(sysData.m_iMTFUvInsp==0 || sysData.m_iProductComp == 1)//uvÈÄ °Ë»ç ¼±ÅÃ (0)
		//{
		//	dMTFSpecTemp = MandoSfrSpec.dAASFR_Spec[1][sfrIndex]; 
		//	if(dCenter_Post < dMTFSpecTemp)
		//	{
		//		sTemp.Format("MTF %s Spec NG :%.3f(Spec:%.2f)", MtfPos[0],dCenter_Post, dMTFSpecTemp);
		//		//! Main DisplayÈ­¸é Overlay NG List
		//		MandoInspLog.sDispNG[MandoInspLog.iNGCnt].Format("MTF %s Spec NG: %.3f(Spec:%.2f)", MtfPos[0],dCenter_Post, dMTFSpecTemp);
		//		MandoInspLog.iNGCnt++;
		//		bResult=false;
		//		pFrame->putListLog(sTemp);
		//		sTemp.Format(_T("[MTF %s: %.3f]"), MtfPos[0],dCenter_Post);
		//		MandoInspLog.sNGList += sTemp;
		//	}
		//	else
		//	{
		//		sTemp.Format("MTF %s Spec OK :%.3f(Spec:%.2f)", MtfPos[0], dCenter_Post, dMTFSpecTemp);
		//		pFrame->putListLog(sTemp);
		//	}
		//	//=====================================================================================================================================================================
		//	dMTFSpecTemp = MandoSfrSpec.dAASFR_Spec[4][sfrIndex];
		//	if(dTL_Post05 < dMTFSpecTemp)
		//	{
		//		sTemp.Format("MTF %s Spec NG: %.3f(Spec:%.2f)", MtfPos[1],dTL_Post05, dMTFSpecTemp);
		//		//! Main DisplayÈ­¸é Overlay NG List
		//		MandoInspLog.sDispNG[MandoInspLog.iNGCnt].Format("MTF %s Spec NG: %.2f(Spec:%.2f)", MtfPos[1],dTL_Post05, dMTFSpecTemp);
		//		MandoInspLog.iNGCnt++;
		//		bResult=false;
		//		pFrame->putListLog(sTemp);
		//		//MandoInspLog.dMTF_PreUV_0_4F++;
		//		sTemp.Format("[MTF 0.7F %s: %.3f]",  MtfPos[1],dTL_Post05);
		//		MandoInspLog.sNGList += sTemp;
		//	}
		//	else
		//	{
		//		sTemp.Format("MTF %s Spec OK :%.3f(Spec:%.2f)", MtfPos[1], dTL_Post05, dMTFSpecTemp);
		//		pFrame->putListLog(sTemp);
		//	}

		//	dMTFSpecTemp = MandoSfrSpec.dAASFR_Spec[6][sfrIndex];

		//	if(dTR_Post05 < dMTFSpecTemp)
		//	{
		//		sTemp.Format("MTF %s Spec NG: %.3f(Spec:%.2f)", MtfPos[2],dTR_Post05, dMTFSpecTemp);
		//		//! Main DisplayÈ­¸é Overlay NG List
		//		MandoInspLog.sDispNG[MandoInspLog.iNGCnt].Format("MTF %s Spec NG: %.3f(Spec:%.2f)", MtfPos[2],dTR_Post05, dMTFSpecTemp);
		//		MandoInspLog.iNGCnt++;
		//		bResult=false;
		//		pFrame->putListLog(sTemp);
		//		sTemp.Format("[MTF %s : %.3f]",  MtfPos[2],dTR_Post05);
		//		MandoInspLog.sNGList += sTemp;
		//	}
		//	else
		//	{
		//		sTemp.Format("MTF %s Spec OK :%.3f(Spec:%.2f)", MtfPos[2], dTR_Post05, dMTFSpecTemp);
		//		pFrame->putListLog(sTemp);
		//	}
		//	dMTFSpecTemp = MandoSfrSpec.dAASFR_Spec[8][sfrIndex];

		//	if(dBL_Post05 < dMTFSpecTemp)
		//	{
		//		sTemp.Format("MTF %s Spec NG: %.3f(Spec:%.2f)", MtfPos[3],dBL_Post05, dMTFSpecTemp);
		//		//! Main DisplayÈ­¸é Overlay NG List
		//		MandoInspLog.sDispNG[MandoInspLog.iNGCnt].Format("MTF %s Spec NG: %.3f(Spec:%.2f)", MtfPos[3],dBL_Post05, dMTFSpecTemp);
		//		MandoInspLog.iNGCnt++;
		//		bResult=false;
		//		pFrame->putListLog(sTemp);
		//		sTemp.Format("[MTF %s: %.3f]",  MtfPos[3],dBL_Post05);
		//		MandoInspLog.sNGList += sTemp;
		//	}
		//	else
		//	{
		//		sTemp.Format("MTF %s Spec OK :%.3f(Spec:%.2f)", MtfPos[3], dBL_Post05, dMTFSpecTemp);
		//		pFrame->putListLog(sTemp);
		//	}
		//	dMTFSpecTemp = MandoSfrSpec.dAASFR_Spec[10][sfrIndex];

		//	if(dBR_Post05 < dMTFSpecTemp)
		//	{
		//		sTemp.Format("MTF %s Spec NG: %.3f(Spec:%.2f)", MtfPos[4],dBR_Post05, dMTFSpecTemp);
		//		//! Main DisplayÈ­¸é Overlay NG List
		//		MandoInspLog.sDispNG[MandoInspLog.iNGCnt].Format("MTF %s Spec NG: %.3f(Spec:%.2f)",MtfPos[4], dBR_Post05, dMTFSpecTemp);
		//		MandoInspLog.iNGCnt++;
		//		bResult=false;
		//		pFrame->putListLog(sTemp);
		//		sTemp.Format("[MTF %s: %.3f]",  MtfPos[4],dBR_Post05);
		//		MandoInspLog.sNGList += sTemp;
		//	}
		//	else
		//	{
		//		sTemp.Format("MTF %s Spec OK :%.3f(Spec:%.2f)", MtfPos[4], dBR_Post05, dMTFSpecTemp);
		//		pFrame->putListLog(sTemp);
		//	}
		//	if(model.m_oc_x >ocMaxSpec || model.m_oc_x < ocMinSpec)
		//	{
		//		bResult=false;
		//		MandoInspLog.sDispNG[MandoInspLog.iNGCnt].Format("UVAfter_OC_X:%.1f(Spec:%.2f~%.2f)",model.m_oc_x, ocMinSpec, ocMaxSpec);
		//		MandoInspLog.iNGCnt++;
		//		sTemp.Format("[UVAfter_OC_X: %.1f(Spec:%.2f~%.2f)]", model.m_oc_x, ocMinSpec, ocMaxSpec);
		//		pFrame->putListLog(sTemp);
		//		MandoInspLog.sNGList += sTemp;
		//	}
		//	if(model.m_oc_y > ocMaxSpec || model.m_oc_y < ocMinSpec)
		//	{
		//		bResult=false;
		//		MandoInspLog.sDispNG[MandoInspLog.iNGCnt].Format("UVAfter_OC_Y:%.1f(Spec:%.2f~%.2f)",model.m_oc_y, ocMinSpec, ocMaxSpec);
		//		MandoInspLog.iNGCnt++;
		//		sTemp.Format("[UVAfter_OC_Y: %.1f(Spec:%.2f~%.2f)]", model.m_oc_y, ocMinSpec, ocMaxSpec);
		//		pFrame->putListLog(sTemp);
		//		MandoInspLog.sNGList += sTemp;
		//	}
		//}
	}
	Task.m_bOkFlag=(bResult)?1:-1;  
	if(Task.m_bOkFlag==-1)
	{
		MandoInspLog.bInspRes = false;
		NgList_Data.tmpNgStr = "PCB NG Manual Pass";
		bResult=false;
	}
	g_SaveLGITLog(0, "Sfr", m_pSFRProc->GetLogHeader(), m_pSFRProc->GetLogData());
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
	cvReleaseImage(&cvImgMtf);
	vision.drawOverlay(CCD);

	Task.sSaveImageName="";
	return bResult;

}


//20170214 ÀÌ¹°°Ë»ç °ü·ÃÃß°¡ lee
double CAABonderDlg::calcColorBalance(cv::Mat src, int nColorOrder)
{
	double ratioGG = 0.0;
	/*int nRoIsize = 100;
	int iRow=(src.rows/2)*2;
	int iCol=(src.cols/2)*2;

	cv::Mat ColorPlane0(iRow / 2, iCol / 2, CV_16UC1);
	cv::Mat ColorPlane1(iRow / 2, iCol / 2, CV_16UC1);
	cv::Mat ColorPlane2(iRow / 2, iCol / 2, CV_16UC1);
	cv::Mat ColorPlane3(iRow / 2, iCol / 2, CV_16UC1);

	for(int j = 0; j < iRow; j+=2)
	{
		for(int i = 0; i < iCol; i+=2)
		{
			ColorPlane0.at<ushort>(j/2, i/2) = src.at<ushort>(j+0, i+0);
			ColorPlane1.at<ushort>(j/2, i/2) = src.at<ushort>(j+0, i+1);
			ColorPlane2.at<ushort>(j/2, i/2) = src.at<ushort>(j+1, i+0);
			ColorPlane3.at<ushort>(j/2, i/2) = src.at<ushort>(j+1, i+1);
		}
	}
	double meanColorPlane0 = calcMeanRoI(ColorPlane0, nRoIsize);
	double meanColorPlane1 = calcMeanRoI(ColorPlane1, nRoIsize);
	double meanColorPlane2 = calcMeanRoI(ColorPlane2, nRoIsize);
	double meanColorPlane3 = calcMeanRoI(ColorPlane3, nRoIsize);
	
	double R, Gr, Gb, B;
	switch(nColorOrder)
	{
		case RGGB:
			R = meanColorPlane0;
			Gr = meanColorPlane1;
			Gb = meanColorPlane2;
			B = meanColorPlane3;
			break;
		case GRBG:
			Gr = meanColorPlane0;
			R = meanColorPlane1;
			B = meanColorPlane2;
			Gb = meanColorPlane3;
			break;
		case GBRG:
			Gb = meanColorPlane0;
			B = meanColorPlane1;
			R = meanColorPlane2;
			Gr = meanColorPlane3;
			break;
		case BGGR:
			B = meanColorPlane0;
			Gb = meanColorPlane1;
			Gr = meanColorPlane2;
			R = meanColorPlane3;
			break;

			default:
			break;
	}
	ratioGG = 100 * ( ( Gr - Gb ) / Gb );*/
	return ratioGG;
}

double CAABonderDlg::calcMeanRoI(cv::Mat src, int nRoIsize)
{
	cv::Mat RoI(src(cv::Rect(src.cols/2-nRoIsize/2, src.rows/2-nRoIsize/2, nRoIsize, nRoIsize)));
	cv::Scalar mean = cv::mean(RoI);
	return mean.val[0];
}

int CAABonderDlg::BlackSpotInsp(CMapData spec , IplImage* bmpImg, BYTE* stainImg)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	int res = R_RESULT_PASS;
	int nResultBlackSpot = 0;
	CString sTemp = "";

	TBlackSpotContrast stSpecBlackSpot;
	memset(&stSpecBlackSpot, 0x00, sizeof(TBlackSpotContrast)); 
	//
	stSpecBlackSpot.nBlockWidth =						spec["BLACKSPOT_STAIN_SPEC.BLOCKWIDTH.i"].i;					//32;
	stSpecBlackSpot.nBlockHeight =						spec["BLACKSPOT_STAIN_SPEC.BLOCKWIDTH.i"].i;					//32;
	stSpecBlackSpot.nClusterSize =						spec["BLACKSPOT_STAIN_SPEC.CLUSTERSIZE.i"].i;					//5;
	stSpecBlackSpot.nDefectInCluster =					spec["BLACKSPOT_STAIN_SPEC.DEFECTINCLUSTER.i"].i;			//5;
	stSpecBlackSpot.dDefectRatio =						spec["BLACKSPOT_STAIN_SPEC.DEFECTRATIO.d"].d;					//0.45000;
	stSpecBlackSpot.nMaxSingleDefectNum =				spec["BLACKSPOT_STAIN_SPEC.MAXSINGLEDEFECTNUM.i"].i;	//10000;
	stSpecBlackSpot.tCircleSpec.bEnableCircle =			spec["BLACKSPOT_STAIN_SPEC.ENABLECIRCLE.i"].i;					//
	stSpecBlackSpot.tCircleSpec.nPosOffsetX =			spec["BLACKSPOT_STAIN_SPEC.POSOFFSETX.i"].i;					//5;
	stSpecBlackSpot.tCircleSpec.nPosOffsetY =			spec["BLACKSPOT_STAIN_SPEC.POSOFFSETY.i"].i;					//5;
	stSpecBlackSpot.tCircleSpec.dRadiusRatioX =			spec["BLACKSPOT_STAIN_SPEC.RADIUSRATIOX.d"].d;				//0.5;
	stSpecBlackSpot.tCircleSpec.dRadiusRatioY =			spec["BLACKSPOT_STAIN_SPEC.RADIUSRATIOY.d"].d;				//0.5;
	stSpecBlackSpot.tMultiCircleSpec.bEnableMultiCircle =		0;
	stSpecBlackSpot.tMultiCircleSpec.dThreshold[0] =		0.0;
	stSpecBlackSpot.tMultiCircleSpec.dThreshold[1] =		0.0;
	stSpecBlackSpot.tMultiCircleSpec.dThreshold[2] =		0.0;
	stSpecBlackSpot.tMultiCircleSpec.dZoneSizeRatio[0] =		0.0;
	stSpecBlackSpot.tMultiCircleSpec.dZoneSizeRatio[1] =		0.0;
	stSpecBlackSpot.tMultiCircleSpec.dZoneSizeRatio[2] =		0.0;
	stSpecBlackSpot.tMultiCircleSpec.nBlobSize[0] =		0;
	stSpecBlackSpot.tMultiCircleSpec.nBlobSize[1] =		0;
	stSpecBlackSpot.tMultiCircleSpec.nBlobSize[2] =		0;
	//CACMISImageBlackSpotContrast *pInspectBlackSpot = new CACMISImageBlackSpotContrast; 
	//delete pInspectBlackSpot;   
	std::shared_ptr<CACMISImageBlackSpotContrastCommon> pInspectBlackSpot = std::make_shared<CACMISImageBlackSpotContrastCommon> ();
	pInspectBlackSpot->Inspect((BYTE*)stainImg,  gMIUDevice.nWidth, gMIUDevice.nHeight, stSpecBlackSpot, gMIUDevice.dTDATASPEC_n.eDataFormat, gMIUDevice.dTDATASPEC_n.eOutMode, gMIUDevice.dTDATASPEC_n.eSensorType,0);////¡Ú¡Ú¡Ú¡Ú
	
	nResultBlackSpot=pInspectBlackSpot->GetDefectBlobCount(); 
   
	totalStainNum +=nResultBlackSpot; 
	if (pInspectBlackSpot->GetSingleDefectCount() > stSpecBlackSpot.nMaxSingleDefectNum) 
	{
		res = R_FAIL_NOISE;
	}else if (pInspectBlackSpot->GetDefectBlobCount() > 0)
	{
		res = R_FAIL_BLACK_SPOT;
	}
	
	for (int i = 0; i < (int)pInspectBlackSpot->GetDefectBlobCount(); i++)
	{
		const RECT* rt1 = pInspectBlackSpot->GetDefectBlobRect(i);
		cvRectangle(bmpImg, cvPoint(rt1->left, rt1->top), cvPoint(rt1->right, rt1->bottom), CV_RGB(100, 255, 100),2);
		vision.boxlist[CCD].addList(rt1, PS_SOLID, M_COLOR_GREEN);
	} 
	const TDefectResult* pMaxResult = pInspectBlackSpot->GetMaxDefectResult();
	sTemp.Format("[BlackSpot-Contrast] Blob Count : %d, Result = %d, maxValue=%.02f",pInspectBlackSpot->GetDefectBlobCount(), res,pMaxResult->dValue);
	pFrame->putListLog(sTemp);
	return res;
}
int CAABonderDlg::LCBInsp(CMapData spec ,  IplImage* bmpImg ,BYTE* LCBstainImg)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	int nResult = R_RESULT_PASS;
	int nResultLCB=0;
	CString sTemp;
	TLCBSpec stSpecLCB;
	memset(&stSpecLCB, 0x00, sizeof(TLCBSpec));
	
	stSpecLCB.dCenterThreshold =				spec["LCB_STAIN_SPEC.CENTERTHRESHOLD.i"].i;
	stSpecLCB.dCornerThreshold =				spec["LCB_STAIN_SPEC.CORNERTHRESHOLD.i"].i;
	stSpecLCB.dEdgeThreshold =					spec["LCB_STAIN_SPEC.EDGETHRESHOLD.i"].i;
	stSpecLCB.nMaxSingleDefectNum =				spec["LCB_STAIN_SPEC.MAXSINGLEDEFECTNUM.i"].i;
	stSpecLCB.nMinDefectWidthHeight =			spec["LCB_STAIN_SPEC.MINDEFECTWIDTHHEIGHT.i"].i;	
	stSpecLCB.tCircleSpec.bEnableCircle =					spec["LCB_STAIN_SPEC.ENABLECIRCLE.i"].i;
	stSpecLCB.tCircleSpec.nPosOffsetX =						spec["LCB_STAIN_SPEC.POSOFFSETX.i"].i;
	stSpecLCB.tCircleSpec.nPosOffsetY =						spec["LCB_STAIN_SPEC.POSOFFSETY.i"].i;
	stSpecLCB.tCircleSpec.dRadiusRatioX =					spec["LCB_STAIN_SPEC.RADIUSRATIOX.d"].d;
	stSpecLCB.tCircleSpec.dRadiusRatioY =					spec["LCB_STAIN_SPEC.RADIUSRATIOY.d"].d;
	stSpecLCB.tMultiCircleSpec.bEnableMultiCircle =		0;
	stSpecLCB.tMultiCircleSpec.dThreshold[0] =		0.0;
	stSpecLCB.tMultiCircleSpec.dThreshold[1] =		0.0;
	stSpecLCB.tMultiCircleSpec.dThreshold[2] =		0.0;
	stSpecLCB.tMultiCircleSpec.dZoneSizeRatio[0] =		0.0;
	stSpecLCB.tMultiCircleSpec.dZoneSizeRatio[1] =		0.0;
	stSpecLCB.tMultiCircleSpec.dZoneSizeRatio[2] =		0.0;
	stSpecLCB.tMultiCircleSpec.nBlobSize[0] =		0;
	stSpecLCB.tMultiCircleSpec.nBlobSize[1] =		0;
	stSpecLCB.tMultiCircleSpec.nBlobSize[2] =		0;
	
	std::shared_ptr<CACMISImageStainLCBCommon> pInspectLCB = std::make_shared<CACMISImageStainLCBCommon>();
	pInspectLCB->Inspect((BYTE*)LCBstainImg, gMIUDevice.nWidth, gMIUDevice.nHeight, stSpecLCB, gMIUDevice.dTDATASPEC_n.eDataFormat, gMIUDevice.dTDATASPEC_n.eOutMode, gMIUDevice.dTDATASPEC_n.eSensorType,0);
	nResultLCB=pInspectLCB->GetDefectBlobCount();
    
	totalStainNum+=nResultLCB;	
	if (pInspectLCB->GetSingleDefectCount() > stSpecLCB.nMaxSingleDefectNum)
	{
		nResult = R_FAIL_NOISE;
	}
	else if (pInspectLCB->GetDefectBlobCount() > 0)
	{
		nResult = R_FAIL_STAIN;
	}	
#if (____MODEL_NAME == M_MPC ||____MODEL_NAME == M_COB_8M ||____MODEL_NAME == M_GEN2  ||____MODEL_NAME == M_TRINITY ) 
	//main,narrow 
			for( int j = 0 ; j < 3 ; j++ )
			{
				for( int i = 0 ; i < pInspectLCB->GetDefectCount((EImageRegion)j); i++ )
				{
					const TDefectResult* dr = pInspectLCB->GetDefectResult((EImageRegion)j,i);
					sTemp.Format("[LCB]Region:%d,i=%2d,x=%4d,y=%4d,value=%3.3f",j,i,dr->ptPos.x,dr->ptPos.y,dr->dValue);
					pFrame->putListLog(sTemp);
				}
			}
#elif  (____MODEL_NAME == XPENG_SIDE) 
	//wide
	const TDefectResult* pCenterMaxresult = pInspectLCB->GetMaxDefectResult(EIMAGEREGION_CIRCLE);
			int ocx = (int)pCenterMaxresult->dContrastMaxR;//(Áß½ÉXÁÂÇ¥);
			int ocy = (int)pCenterMaxresult->dContrastMaxGb;//(Áß½ÉYÁÂÇ¥);
			int radx =(int)pCenterMaxresult->dContrastMaxGr;//(XÃà ¹ÝÁö¸§);
			int rady =(int)pCenterMaxresult->dContrastMaxB;//(YÃà ¹ÝÁö¸§);
			cvEllipse(bmpImg, cvPoint(ocx, ocy), cvSize(radx, rady), 0, 0, 360, CV_RGB(255, 0, 0),2);
			vision.ellipselist[CCD].addList(CPoint(ocx-radx, ocy-rady),CPoint(ocx+radx, ocy+rady),PS_SOLID , M_COLOR_RED);
#endif
	
	for( int i = 0 ; i < pInspectLCB->GetDefectBlobCount() ; i++ ) 
	{
		const RECT* rt2 = pInspectLCB->GetDefectBlobRect(i);
		cvRectangle(bmpImg, cvPoint(rt2->left, rt2->top), cvPoint(rt2->right, rt2->bottom), CV_RGB(255, 0, 0),2,8);
		vision.boxlist[CCD].addList(rt2, PS_SOLID, M_COLOR_RED);
	}

	sTemp.Format("[LCB] SingleDefectCount=%d, BlobCount : %d, Result = %d",pInspectLCB->GetSingleDefectCount() ,pInspectLCB->GetDefectBlobCount(), nResult);
	pFrame->putListLog(sTemp);
#if (____MODEL_NAME == M_MPC || ____MODEL_NAME == M_COB_8M||____MODEL_NAME == M_GEN2||____MODEL_NAME == M_TRINITY) 
	//main,narrow 
	const TDefectResult* pCenterMaxResult = pInspectLCB->GetMaxDefectResult(EIMAGEREGION_CENTER);
	const TDefectResult* pEdgeMaxResult = pInspectLCB->GetMaxDefectResult(EIMAGEREGION_EDGE);
	const TDefectResult* pCornerMaxResult = pInspectLCB->GetMaxDefectResult(EIMAGEREGION_CORNER);

	sTemp.Format( "[LCB] Center maxValue=%.02f, Edge maxValue=%.02f, Corner maxValue=%.02f", pCenterMaxResult->dValue, pEdgeMaxResult->dValue,pCornerMaxResult->dValue);

#elif  (____MODEL_NAME == SONY_2M || ____MODEL_NAME == SALLEN_RIVIN) 
	//wide
	const TDefectResult* pCircleMaxResult = pInspectLCB->GetMaxDefectResult(EIMAGEREGION_CIRCLE);
  
	sTemp.Format( "[LCB] Circle maxValue=%.2lf, posX=%d, posY=%d , W=%d , H=%d , N=%d", 
	pCircleMaxResult->dValue, pCircleMaxResult->ptPos.x, pCircleMaxResult->ptPos.y , pCircleMaxResult->nWidth , pCircleMaxResult->nHeight , pCircleMaxResult->nCount);

#endif
	pFrame->putListLog(sTemp);
	return nResult;
}
int   CAABonderDlg::Blemish_YmeanInsp(CMapData spec ,  IplImage* bmpImg , BYTE* YstainImg)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	int nResult = R_RESULT_PASS;
	int nResultRUYmean = 0;
	CString sTemp = "";

	TRU_YmeanSpec stSpecYmean;
	memset(&stSpecYmean, 0x00, sizeof(TRU_YmeanSpec));

	stSpecYmean.nDefectBlockSize =								spec["YMEAN_STAIN_SPEC.DEFECTBLOCKSIZE.i"].i;
	stSpecYmean.nEdgeSize =										spec["YMEAN_STAIN_SPEC.EDGESIZE.i"].i;	
	stSpecYmean.fCornerThreshold =								spec["YMEAN_STAIN_SPEC.CORNERTHRESHOLD.i"].i;	
	stSpecYmean.fCenterThreshold =								spec["YMEAN_STAIN_SPEC.CENTERTHRESHOLD.i"].i;
	stSpecYmean.fEdgeThreshold =								spec["YMEAN_STAIN_SPEC.EDGETHRESHOLD.i"].i;	
	stSpecYmean.nLscBlockSize =									spec["YMEAN_STAIN_SPEC.LSCBLOCKSIZE.i"].i;	
	stSpecYmean.tCircleSpec.bEnableCircle =						spec["YMEAN_STAIN_SPEC.ENABLECIRCLE.i"].i;
	stSpecYmean.tCircleSpec.nPosOffsetX =						spec["YMEAN_STAIN_SPEC.POSOFFSETX.i"].i;
	stSpecYmean.tCircleSpec.nPosOffsetY =						spec["YMEAN_STAIN_SPEC.POSOFFSETY.i"].i;
	stSpecYmean.tCircleSpec.dRadiusRatioX =						spec["YMEAN_STAIN_SPEC.RADIUSRATIOX.d"].d;
	stSpecYmean.tCircleSpec.dRadiusRatioY =						spec["YMEAN_STAIN_SPEC.RADIUSRATIOY.d"].d;
	stSpecYmean.tMultiCircleSpec.bEnableMultiCircle =			0;
	stSpecYmean.tMultiCircleSpec.dThreshold[0] =				0.0;
	stSpecYmean.tMultiCircleSpec.dThreshold[1] =				0.0;
	stSpecYmean.tMultiCircleSpec.dThreshold[2] =				0.0;
	stSpecYmean.tMultiCircleSpec.dZoneSizeRatio[0] =			0.0;
	stSpecYmean.tMultiCircleSpec.dZoneSizeRatio[1] =			0.0;
	stSpecYmean.tMultiCircleSpec.dZoneSizeRatio[2] =			0.0;
	stSpecYmean.tMultiCircleSpec.nBlobSize[0] =					0;
	stSpecYmean.tMultiCircleSpec.nBlobSize[1] =					0;
	stSpecYmean.tMultiCircleSpec.nBlobSize[2] =					0;



	std::shared_ptr<CACMISImageStainRU_YmeanCommon> pInspectBlemish_Ymean = std::make_shared<CACMISImageStainRU_YmeanCommon>();//delete,x

	pInspectBlemish_Ymean->Inspect((BYTE*)YstainImg,gMIUDevice.nWidth, gMIUDevice.nHeight, stSpecYmean, gMIUDevice.dTDATASPEC_n.eDataFormat, gMIUDevice.dTDATASPEC_n.eOutMode, gMIUDevice.dTDATASPEC_n.eSensorType,0);
	nResultRUYmean = pInspectBlemish_Ymean->GetDefectBlobCount();
	totalStainNum+=nResultRUYmean;

	//=========================================================================================================
	const TDefectResult* center = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_CORNER);
	RECT rec;
	rec.left = center->dContrastMaxR * 2;
	rec.right= center->dContrastMaxGr * 2;
	rec.top = center->dContrastMaxGb * 2;
	rec.bottom = center->dContrastMaxB * 2;
	//
	int EdgeSize = stSpecYmean.nEdgeSize;
#if (____MODEL_NAME == M_MPC || ____MODEL_NAME == M_COB_8M ||____MODEL_NAME == M_GEN2||____MODEL_NAME == M_TRINITY) 
	//Çù°¢
	vision.linelist[CCD].addList(EdgeSize, EdgeSize, gMIUDevice.nWidth-EdgeSize, EdgeSize, PS_SOLID, M_COLOR_YELLOW);		//top
	vision.linelist[CCD].addList(EdgeSize, gMIUDevice.nHeight-EdgeSize, gMIUDevice.nWidth-EdgeSize, gMIUDevice.nHeight-EdgeSize, PS_SOLID, M_COLOR_YELLOW);		//bottom
	vision.linelist[CCD].addList(EdgeSize, EdgeSize, EdgeSize, gMIUDevice.nHeight-EdgeSize, PS_SOLID, M_COLOR_YELLOW);	//left
	vision.linelist[CCD].addList(gMIUDevice.nWidth-EdgeSize, EdgeSize, gMIUDevice.nWidth-EdgeSize, gMIUDevice.nHeight-EdgeSize, PS_SOLID, M_COLOR_YELLOW);	//right
	//
	cvLine(bmpImg, cvPoint(EdgeSize, EdgeSize),cvPoint(gMIUDevice.nWidth-EdgeSize, EdgeSize),CV_YELLOW,2);
	cvLine(bmpImg, cvPoint(EdgeSize, gMIUDevice.nHeight-EdgeSize),cvPoint( gMIUDevice.nWidth-EdgeSize, gMIUDevice.nHeight-EdgeSize),CV_YELLOW,2);
	cvLine(bmpImg, cvPoint(EdgeSize, EdgeSize),cvPoint(EdgeSize, gMIUDevice.nHeight-EdgeSize),CV_YELLOW,2);
	cvLine(bmpImg, cvPoint(gMIUDevice.nWidth-EdgeSize, EdgeSize),cvPoint(gMIUDevice.nWidth-EdgeSize, gMIUDevice.nHeight-EdgeSize),CV_YELLOW,2);
#elif  (____MODEL_NAME == SONY_2M || ____MODEL_NAME == SALLEN_RIVIN || ____MODEL_NAME == SALLEN_RIVIN_SPL) 
	//wide
#endif

	//=========================================================================================================
	if (pInspectBlemish_Ymean->GetSingleDefectCount() > 0)
	{
		nResult = R_FAIL_BLACK_SPOT;
	}
	for (int i = 0; i < pInspectBlemish_Ymean->GetDefectBlobCount(); i++)
	{
		const RECT* rt3= pInspectBlemish_Ymean->GetDefectBlobRect(i);
		cvRectangle(bmpImg, cvPoint(rt3->left, rt3->top), cvPoint(rt3->right, rt3->bottom), CV_RGB(0, 255, 255),2);
		vision.boxlist[CCD].addList(rt3, PS_SOLID, M_COLOR_CYAN);
	}
	cvSaveImage("D:/bmpImg.bmp", bmpImg);
	sTemp.Format(" RU Ymean Blob Count : %d, Result = %d", pInspectBlemish_Ymean->GetDefectBlobCount(), nResult);
	pFrame->putListLog(sTemp);
		
	sTemp.Format("[RU_Ymean] SingleDefectCount=%d, BlobCount=%d,  Result = %d", nResultRUYmean, pInspectBlemish_Ymean->GetDefectBlobCount(), nResult);
#if (____MODEL_NAME == M_MPC ||____MODEL_NAME == M_COB_8M  ||____MODEL_NAME == M_GEN2 ||____MODEL_NAME == M_TRINITY) 
	//Çù°¢
	const TDefectResult* pCenterMaxResult = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_CENTER);
	const TDefectResult* pEdgeMaxResult = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_EDGE);
	const TDefectResult* pCornerMaxResult = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_CORNER);
			
	sTemp.Format("[RU_Ymean]Center max=%.02f, Edge max=%.02f, Corner max=%.02f", pCenterMaxResult->dValue, pEdgeMaxResult->dValue, pCornerMaxResult->dValue );
#else
	//wide  SALLEN_RIVIN   DCY11_AVM DCY11_RVC
	const TDefectResult* pCircleMaxResult = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_CIRCLE);
	sTemp.Format("[RU_Ymean]Circle=%d, maxValue=%.02f, posX=%d, posY=%d", pInspectBlemish_Ymean->GetDefectCount(EIMAGEREGION_CIRCLE), pCircleMaxResult->dValue, pCircleMaxResult->ptPos.x, pCircleMaxResult->ptPos.y );
	int ocx = (int)pCircleMaxResult->dContrastMaxR;//(Áß½ÉXÁÂÇ¥);
	int ocy = (int)pCircleMaxResult->dContrastMaxGb;//(Áß½ÉYÁÂÇ¥);
	int radx =(int)pCircleMaxResult->dContrastMaxGr;//(XÃà ¹ÝÁö¸§);
	int rady =(int)pCircleMaxResult->dContrastMaxB;//(YÃà ¹ÝÁö¸§);
	cvEllipse(bmpImg, cvPoint(ocx, ocy), cvSize(radx, rady), 0, 0, 360, CV_RGB(0, 255, 255),2);
	vision.ellipselist[CCD].addList(CPoint(ocx-radx, ocy-rady),CPoint(ocx+radx, ocy+rady),PS_SOLID , M_COLOR_CYAN);
#endif

	return nResult;
}

double CAABonderDlg::GetDistortion(LPBYTE Rgb,int m_Width, int m_Height,CPoint Center)
{
	COLORREF *m_pRgb = (COLORREF *)Rgb;

	int iMarkSize=100;

	CPoint A11,A12,B1,B2,A21,A22;

	int X=(m_Width/2)-336;
	int Y=(m_Height/2)-239;
	int CY=(m_Height/2)-181;


	A11=GetCirclePos((LPBYTE)m_pRgb,m_Width, m_Height,CRect(Center.x-X-iMarkSize	,Center.y-Y-iMarkSize	, Center.x-X+iMarkSize	, Center.y-Y+iMarkSize),false);
	A12=GetCirclePos((LPBYTE)m_pRgb,m_Width, m_Height,CRect(Center.x-X-iMarkSize	,Center.y+Y-iMarkSize	, Center.x-X+iMarkSize	, Center.y+Y+iMarkSize),false);

	B1=GetCirclePos((LPBYTE)m_pRgb,m_Width, m_Height,CRect(Center.x-iMarkSize		,Center.y-CY-iMarkSize	, Center.x+iMarkSize		, Center.y-CY+iMarkSize));
	B2=GetCirclePos((LPBYTE)m_pRgb,m_Width, m_Height,CRect(Center.x-iMarkSize		,Center.y+CY-iMarkSize	, Center.x+iMarkSize		, Center.y+CY+iMarkSize));

	A21=GetCirclePos((LPBYTE)m_pRgb,m_Width, m_Height,CRect(Center.x+X-iMarkSize	,Center.y-Y-iMarkSize	, Center.x+X+iMarkSize	, Center.y-Y+iMarkSize),false);
	A22=GetCirclePos((LPBYTE)m_pRgb,m_Width, m_Height,CRect(Center.x+X-iMarkSize	,Center.y+Y-iMarkSize	, Center.x+X+iMarkSize	, Center.y+Y+iMarkSize),false);

	if(A11.x== -1 || A11.y==-1 ||A12.x== -1 || A12.y==-1 ||B1.x== -1 || B1.y==-1 ||B2.x== -1 || B2.y==-1 ||A21.x== -1 || A21.y==-1 ||A22.x== -1 || A22.y==-1)
	{
		return -999;
	}

	Mark_Area(Center.x-X-iMarkSize	,Center.y-Y-iMarkSize	, Center.x-X+iMarkSize	, Center.y-Y+iMarkSize, rgb(255,0,0), m_Width, m_Height, (LPBYTE)m_pRgb);
	Mark_Area(Center.x-iMarkSize		,Center.y-CY-iMarkSize	, Center.x+iMarkSize		, Center.y-CY+iMarkSize, rgb(255,0,0), m_Width, m_Height, (LPBYTE)m_pRgb);
	Mark_Area(Center.x+X-iMarkSize	,Center.y-Y-iMarkSize	, Center.x+X+iMarkSize	, Center.y-Y+iMarkSize, rgb(255,0,0), m_Width, m_Height, (LPBYTE)m_pRgb);

	Mark_Area(Center.x-X-iMarkSize	,Center.y+Y-iMarkSize	, Center.x-X+iMarkSize	, Center.y+Y+iMarkSize, rgb(255,0,0), m_Width, m_Height, (LPBYTE)m_pRgb);
	Mark_Area(Center.x-iMarkSize		,Center.y+CY-iMarkSize	, Center.x+iMarkSize		, Center.y+CY+iMarkSize, rgb(255,0,0), m_Width, m_Height, (LPBYTE)m_pRgb);
	Mark_Area(Center.x+X-iMarkSize	,Center.y+Y-iMarkSize	, Center.x+X+iMarkSize	, Center.y+Y+iMarkSize, rgb(255,0,0), m_Width, m_Height, (LPBYTE)m_pRgb);

	double A1 = std::sqrt( std::pow((double)A11.x - (double)A12.x, 2) + std::pow((double)A11.y - (double)A12.y, 2));
	double A2 = std::sqrt( std::pow((double)A21.x - (double)A22.x, 2) + std::pow((double)A21.y - (double)A22.y, 2));
	double B = std::sqrt( std::pow((double)B1.x - (double)B2.x, 2) + std::pow((double)B1.y - (double)B2.y, 2));

	double A = (A1 + A2) / 2;
	double TVdistortion = 100 * ( A - B ) / B;


	return TVdistortion;
}

double CAABonderDlg::GetFoV(LPBYTE Rgb,int Width, int Height,CPoint Center)
{
	COLORREF *rgb_p = (COLORREF *)Rgb;

	const double cols = Width;
	const double rows = Height;
	const double pixelsize = 0.00155;;//0.0025;
	const double f = 5.018;;//25.21;
	
	CPoint cross1,cross2;
	CPoint cpCross1,cpCross2;

	int X=(Width/2)-800;
	int Y=200;
	int W=Y*2;
	
	cross1=GetCrossPos(Center.x-X-W/2,Center.y-W/2,W,(LPBYTE)rgb_p,Width, Height,Center);		
	cross2=GetCrossPos(Center.x+X-W/2,Center.y-W/2,W,(LPBYTE)rgb_p,Width, Height,Center);		

	cross1=GetCrossPos2(Center.x-X-W/2,Center.y-W/2,W,(LPBYTE)rgb_p,Width, Height,Center);		
	cross2=GetCrossPos2(Center.x+X-W/2,Center.y-W/2,W,(LPBYTE)rgb_p,Width, Height,Center);		

	if(cross1.x	== -1 || cross1.y==-1 || cross2.x==-1 || cross2.y== -1)
	{
		return -999;
	}

	//Mark_Area(Center.x-X-W/2,Center.y-Y, Center.x-X+W/2,Center.y+W/2, RGB(0,0,255), Width, Height, (LPBYTE)rgb_p);//±âÁ¸
	//Mark_Area(Center.x+X-W/2,Center.y-Y, Center.x+X+W/2,Center.y+W/2, RGB(0,0,255), Width, Height, (LPBYTE)rgb_p);//±âÁ¸

	double y1 = std::sqrt(std::pow(cross1.x - cols/2, 2) + std::pow(cross1.y - rows/2, 2));
	double y2 = std::sqrt(std::pow(cross2.x - cols/2, 2) + std::pow(cross2.y - rows/2, 2));

	double FoV = std::atan2(y1 * pixelsize, f) + std::atan2(y2 * pixelsize, f);


	//		Mark_Area(150,m_Height/2-200, 150+400, (m_Height/2-200)+400, RGB(0,0,255), Width, Height, (LPBYTE)m_pRgb);/¿ø·¡ ÁÖ¼®Ã³¸®

	//Mark_Area(cross1.x,cross1.y, cross1.x+10, cross1.y+10, RGB(0,0,255), Width, Height, (LPBYTE)rgb_p);//±âÁ¸
	//Mark_Area(cross2.x,cross2.y, cross2.x+10, cross2.y+10, RGB(0,0,255), Width, Height, (LPBYTE)rgb_p);//±âÁ¸
	Mark_Area(cross1.x,cross1.y, cross1.x+2, cross1.y+2, RGB(0,0,255), Width, Height, (LPBYTE)rgb_p);
	Mark_Area(cross2.x,cross2.y, cross2.x+2, cross2.y+2, RGB(0,0,255), Width, Height, (LPBYTE)rgb_p);
	return FoV;
}


CPoint CAABonderDlg::GetCrossPos(int x,int y,int size,LPBYTE Rgb,int Width, int Height,CPoint cpCenter)
{
	COLORREF *rgb_p = (COLORREF *)Rgb;
	long index;

	CPoint LT_RB[2];
	CPoint RT_LB[2];
	CPoint Center[4];

	bool bCheckFirstLT_RB=false;
	bool bCheckFirstRT_LB=false;

	int LineColor=90;
	COLORREF rgb;

	int iMax=0;
	int iMin=255;

	int iLx[4]={-1,-1,-1,-1};
	int iLy[4]={-1,-1,-1,-1};

	int MinXValue=0;
	int MinXSeq=0;
	int MinYValue=0;
	int MinYSeq=0;
	
	int r,g,b;

	for(LineColor=60;LineColor<220;LineColor+=10)
	{
		for(int i=0;i<size;i++)
		{
			// Left Top to Right Bottom
			index = (y+i) * Width + (x+i);
			rgb=*(rgb_p + index );
			r=GetRValue(rgb);	g=GetGValue(rgb);	b=GetBValue(rgb);
//			if(GetRValue(rgb) < LineColor && GetGValue(rgb) < LineColor && GetBValue(rgb) < LineColor)
			if(GetGValue(rgb) < LineColor)
			{
				if(!bCheckFirstLT_RB)
				{
					LT_RB[0]=CPoint(x+i,y+i);
					bCheckFirstLT_RB=true;
				}
				else LT_RB[1]=CPoint(x+i,y+i);
			}
			Mark_Area(x+i,y+i,x+i,y+i, RGB(0,255,0), Width, Height, (LPBYTE)rgb_p);		// ´ë°¢¼± È®ÀÎ

			// Right Top to Left Bottom
			index = (y+i) * Width + (x+size-i);
			rgb=*(rgb_p + index );
			//			if(GetRValue(rgb) < LineColor && GetGValue(rgb) < LineColor && GetBValue(rgb) < LineColor)
			if(GetGValue(rgb) < LineColor)
			{
				if(!bCheckFirstRT_LB)
				{
					RT_LB[0]=CPoint(x+size-i,y+i);
					bCheckFirstRT_LB=true;
				}
				else RT_LB[1]=CPoint(x+size-i,y+i);
			}
			Mark_Area(x+size-i,y+i, x+size-i,y+i, RGB(0,0,255), Width, Height, (LPBYTE)rgb_p);		// ´ë°¢¼± È®ÀÎ
		}

		iLx[0]=abs(RT_LB[0].x-LT_RB[0].x);
		iLx[1]=abs(RT_LB[0].x-LT_RB[1].x);
		iLx[2]=abs(RT_LB[1].x-LT_RB[0].x);
		iLx[3]=abs(RT_LB[1].x-LT_RB[1].x);

		iLy[0]=abs(RT_LB[0].y-LT_RB[0].y);
		iLy[1]=abs(RT_LB[0].y-LT_RB[1].y);
		iLy[2]=abs(RT_LB[1].y-LT_RB[0].y);
		iLy[3]=abs(RT_LB[1].y-LT_RB[1].y);


		Center[0].x=(RT_LB[0].x+LT_RB[0].x)/2;
		Center[1].x=(RT_LB[0].x+LT_RB[1].x)/2;
		Center[2].x=(RT_LB[1].x+LT_RB[0].x)/2;
		Center[3].x=(RT_LB[1].x+LT_RB[1].x)/2;

		Center[0].y=(RT_LB[0].y+LT_RB[0].y)/2;
		Center[1].y=(RT_LB[0].y+LT_RB[1].y)/2;
		Center[2].y=(RT_LB[1].y+LT_RB[0].y)/2;
		Center[3].y=(RT_LB[1].y+LT_RB[1].y)/2;

		MinXValue=iLx[0];
		MinXSeq=0;
		MinYValue=iLy[0];
		MinYSeq=0;

		for(int i=1;i<4;i++)
		{
			if(iLx[i] < MinXValue)
			{
				MinXValue=iLx[i];
				MinXSeq=i;
			}

			if(iLy[i] < MinYValue)
			{
				MinYValue=iLy[i];
				MinYSeq=i;
			}
		}
		if(abs(cpCenter.y - Center[MinYSeq].y) < 10)break;
	}

	return CPoint(Center[MinXSeq].x,Center[MinYSeq].y);
}
CPoint CAABonderDlg::GetCrossPos2(int x,int y,int size,LPBYTE Rgb,int Width, int Height,CPoint cpCenter)
{
	COLORREF *rgb_p = (COLORREF *)Rgb;
	long index;

	CPoint VH[2];
	CPoint VL[2];
	CPoint HL[2];
	CPoint HR[2];
	CPoint Center;

	bool bIsVH=false;
	bool bIsVL=false;
	bool bIsHL=false;
	bool bIsHR=false;

	int LineColor=160;
	COLORREF rgb;

	int iMax=0;
	int iMin=255;

	int iLx[4]={-1,-1,-1,-1};
	int iLy[4]={-1,-1,-1,-1};

	int MinXValue=0;
	int MinXSeq=0;
	int MinYValue=0;
	int MinYSeq=0;

	int r,g,b;
	int iX,iY;

	for(LineColor=60;LineColor<120;LineColor+=10)
	{
		for(int i=0;i<size;i++)
		{
			// VH
			iX=x+i;
			iY=y;
			iX=iX<Width?iX:Width-1;
			iY=iY<Height?iY:Height-1;
			index = (iY) * Width + (iX);
			rgb=rgb_p[index];
			if(GetGValue(rgb) < LineColor)
			{
				if(!bIsVH)
				{
					VH[0]=CPoint(iX,iY);
					bIsVH=true;
				}
				else VH[1]=CPoint(iX,iY);
			}
//			Mark_Area(iX,iY,iX,iY, rgb(0,100,100), Width, Height, (LPBYTE)rgb_p);		// ¼¼·Î¼± È®ÀÎ

			// VL
			iX=x+i;
			iY=y+size-1;
			iX=iX<Width?iX:Width-1;
			iY=iY<Height?iY:Height-1;
			index = (iY) * Width + (iX);
			rgb=rgb_p[index];
			g=GetGValue(rgb);
			if(GetGValue(rgb) < LineColor)
			{
				if(!bIsVL)
				{
					VL[0]=CPoint(iX,iY);
					bIsVL=true;
				}
				else VL[1]=CPoint(iX,iY);
			}
			//Mark_Area(iX,iY,iX,iY, rgb(255,0,0), Width, Height, (LPBYTE)rgb_p);		// ¼¼·Î¼± È®ÀÎ


			// HL
			iX=x;
			iY=y+i;
			iX=iX<Width?iX:Width-1;
			iY=iY<Height?iY:Height-1;
			index = (iY) * Width + (iX);
			rgb=rgb_p[index];
			if(GetGValue(rgb) < LineColor)
			{
				if(!bIsHL)
				{
					HL[0]=CPoint(iX,iY);
					bIsHL=true;
				}
				else HL[1]=CPoint(iX,iY);
			}
//			Mark_Area(iX,iY,iX,iY, rgb(0,100,100), Width, Height, (LPBYTE)rgb_p);		// ¼¼·Î¼± È®ÀÎ

			// HR
			iX=x+size-1;
			iY=y+i;
			iX=iX<Width?iX:Width-1;
			iY=iY<Height?iY:Height-1;
			index = (iY) * Width + (iX);
			rgb=rgb_p[index];
			if(GetGValue(rgb) < LineColor)
			{
				if(!bIsHR)
				{
					HR[0]=CPoint(iX,iY);
					bIsHR=true;
				}
				else HR[1]=CPoint(iX,iY);
			}
//			Mark_Area(iX,iY,iX,iY, rgb(0,100,100), Width, Height, (LPBYTE)rgb_p);		// ¼¼·Î¼± È®ÀÎ

		}

		Center=CPoint((VH[0].x+VH[1].x+VL[0].x+VL[1].x)/4,(HL[0].y+HL[1].y+HR[0].y+HR[1].y)/4);

		if(abs(cpCenter.y - Center.y) < 20)break;
	}

		Mark_Area(VH[0].x,VH[0].y,VH[1].x,VH[1].y, rgb(0,255,100), Width, Height, (LPBYTE)rgb_p);		// Ã£Àº ¼± È®ÀÎ
		Mark_Area(VL[0].x,VL[0].y,VL[1].x,VL[1].y, rgb(0,255,100), Width, Height, (LPBYTE)rgb_p);		// Ã£Àº ¼± È®ÀÎ
		Mark_Area(HL[0].x,HL[0].y,HL[1].x,HL[1].y, rgb(0,255,100), Width, Height, (LPBYTE)rgb_p);		// Ã£Àº ¼± È®ÀÎ
		Mark_Area(HR[0].x,HR[0].y,HR[1].x,HR[1].y, rgb(0,255,100), Width, Height, (LPBYTE)rgb_p);		// Ã£Àº ¼± È®ÀÎ

	return Center;
}
bool CAABonderDlg::findFiducialMark(LPBYTE Rgb, int sizeX, int sizeY, CPoint* cpFiducialPos)
{
	CString sLangChange;
	CString sLog;

	COLORREF *ucImage = (COLORREF *)Rgb;
//	long index;

	int maxSize = 400;

	int aiHistX[5000];
	int aiHistY[5000];

	CRect crFiducialRoi[4];
	crFiducialRoi[0]=CRect(1270, 280,1270+200, 280+200);
	crFiducialRoi[1]=CRect(2590, 280,2590+200, 280+200);
	crFiducialRoi[2]=CRect(1270,2580,1270+200,2580+200);
	crFiducialRoi[3]=CRect(2590,2580,2590+200,2580+200);

	int _CIRCLE_MARK_GAP=800;	// 800

	for (int i=0 ; i<4 ; i++)
	{
		if ( crFiducialRoi[i].left < 0 || crFiducialRoi[i].top < 0 || crFiducialRoi[i].right > sizeX || crFiducialRoi[i].bottom > sizeY )
		{
			sLog.Format("[¸¶Å© #%d] ¿øÇü ¸¶Å© °Ë»ö ¿µ¿ªÀÌ Àß¸øµÇ¾ú½À´Ï´Ù.",i);
//			MSG_Display(sLog);
			return false;
		}

		int iSx, iSy, iEx, iEy;
		int x, y, iPos;
		int iMaxX = -1, iMinX = 9999;
		int iMaxY = -1, iMinY = 9999;
		int iMax_Left = -1 , iMax_Right = -1, iMax_Up = -1, iMax_Down = -1;

		int iMaxX2 = -1, iMinX2 = 9999;
		int iMaxY2 = -1, iMinY2 = 9999;
		int iMax_Left2 = -1 , iMax_Right2 = -1, iMax_Up2 = -1, iMax_Down2 = -1;

		int iSum;		
		int iGap;

		int iSum_Left, iSum_Right;
		int iSum_Top, iSum_Bottom;

		iSx = crFiducialRoi[i].left;
		iSy = crFiducialRoi[i].top;
		iEx = crFiducialRoi[i].right;
		iEy = crFiducialRoi[i].bottom;

		::memset(aiHistY, 0, sizeof(int)*(5000));
		::memset(aiHistX, 0, sizeof(int)*(5000));


		/* X¹æÇâ È÷½ºÅä±×·¥ */
		for (x=iSx ; x<iEx ; x++)
		{
			iPos = iSy * sizeX + x;
			iSum = 0;

			for (y=iSy ; y<iEy ; y++)
			{
				//				iSum += (ucImage[iPos]*ucImage[iPos]);
				iSum += (ucImage[iPos]);
				iPos += sizeX;
			}

			aiHistX[x-iSx] = iSum;
		}

		/* X¹æÇâºÎÅÍ ¿§Áö Ã£±â */
		for (x=iSx+5 ; x<iEx-5 ; x++)
		{
			iSum_Left  = aiHistX[x-iSx-4] + aiHistX[x-iSx-3] + aiHistX[x-iSx-2] + aiHistX[x-iSx-1];
			iSum_Right = aiHistX[x-iSx+4] + aiHistX[x-iSx+3] + aiHistX[x-iSx+2] + aiHistX[x-iSx+1];

			iGap = iSum_Left - iSum_Right;

			if (iGap>_CIRCLE_MARK_GAP &&  iGap > iMax_Left)
			{
				iMax_Left = iGap;
				iMinX = x;
			}

			iGap = iSum_Right - iSum_Left;

			if (iGap>_CIRCLE_MARK_GAP &&  iGap > iMax_Right)
			{
				iMax_Right = iGap;
				iMaxX = x;
			}
		}


		iMax_Left = 0;
		iMinX2 = 0;
		int tmpSx = iMaxX-_CIRCLE_MARK_GAP;
		int tmpEx = iMinX - 30;
		if(tmpSx < (iSx+5))
			tmpSx = (iSx+5);
		if(tmpEx > (iEx-5))
			tmpEx = (iSx-5);

		for (x=tmpSx ; x<iMaxX; x++)
		{
			iSum_Left  = aiHistX[x-iSx-4] + aiHistX[x-iSx-3] + aiHistX[x-iSx-2] + aiHistX[x-iSx-1];
			iSum_Right = aiHistX[x-iSx+4] + aiHistX[x-iSx+3] + aiHistX[x-iSx+2] + aiHistX[x-iSx+1];

			iGap = iSum_Left - iSum_Right;

			if (iGap>_CIRCLE_MARK_GAP &&  iGap > iMax_Left)
			{
				iMax_Left = iGap;
				iMinX2 = x;
			}
		}


		iMax_Right = 0;
		iMaxX2 = 0;

		tmpSx = iMinX + 30;
		tmpEx = iMinX + _CIRCLE_MARK_GAP;
		if(tmpSx < (iSx+5))
			tmpSx = (iSx+5);
		if(tmpEx > (iEx-5))
			tmpEx = (iEx-5);


		for (x=tmpSx ; x<tmpEx; x++)
		{
			iSum_Left  = aiHistX[x-iSx-4] + aiHistX[x-iSx-3] + aiHistX[x-iSx-2] + aiHistX[x-iSx-1];
			iSum_Right = aiHistX[x-iSx+4] + aiHistX[x-iSx+3] + aiHistX[x-iSx+2] + aiHistX[x-iSx+1];

			iGap = iSum_Right - iSum_Left;

			if (iGap>_CIRCLE_MARK_GAP && iGap > iMax_Right)
			{
				iMax_Right = iGap;
				iMaxX2 = x;
			}
		}


		if( iMaxX-iMinX>30 && iMaxX-iMinX<maxSize && iMinX>0)
		{
			iMinX = iMinX;
			iMaxX = iMaxX;
		}
		else if( iMaxX2-iMinX>30 && iMaxX2-iMinX<maxSize && iMinX>0 )
		{
			iMinX = iMinX;
			iMaxX = iMaxX2;
		}
		else if( iMaxX-iMinX2>30 && iMaxX-iMinX2<maxSize && iMinX2>0 )
		{
			iMinX = iMinX2;
			iMaxX = iMaxX;
		}

		/* Y ¹æÇâ È÷½ºÅä±×·¥ */
		for (y=iSy ; y<iEy ; y++)
		{
			iPos = y * sizeX + iMinX;
			iSum = 0;

			for (x=iMinX ; x<iMaxX; x++)
			{
				iSum += ucImage[iPos++];
			}

			aiHistY[y-iSy] = iSum;
		}

		for (y=iSy+5 ; y<iEy-5 ; y++)
		{
			iSum_Top	= aiHistY[y-iSy-4] + aiHistY[y-iSy-3] + aiHistY[y-iSy-2] + aiHistY[y-iSy-1];
			iSum_Bottom = aiHistY[y-iSy+4] + aiHistY[y-iSy+3] + aiHistY[y-iSy+2] + aiHistY[y-iSy+1];

			iGap = iSum_Top - iSum_Bottom;

			if ( iGap > iMax_Up )
			{
				iMax_Up = iGap;
				iMinY = y;
			}

			iGap = iSum_Bottom - iSum_Top;

			if ( iGap > iMax_Down )
			{
				iMax_Down = iGap;
				iMaxY = y;
			}
		}


		if ( iMinY >= iMaxY )
		{
			int iMinY2		= -9999;
			int iMaxY2		= -9999;
			int iMax_Up2	= 0;
			int iMax_Down2	= 0;

			int tmpSy = iMinY + 5;
			int tmpEy = iEy-5;

			for (y=tmpSy ; y<tmpEy ; y++)
			{
				iSum_Top	= aiHistY[y-iSy-4] + aiHistY[y-iSy-3] + aiHistY[y-iSy-2] + aiHistY[y-iSy-1];
				iSum_Bottom = aiHistY[y-iSy+4] + aiHistY[y-iSy+3] + aiHistY[y-iSy+2] + aiHistY[y-iSy+1];

				iGap = iSum_Bottom - iSum_Top;

				if ( iGap > iMax_Down2 && (iGap>_CIRCLE_MARK_GAP) )
				{
					iMax_Down2 = iGap;
					iMaxY2 = y;
				}
			}


			tmpSy = iSy+5;
			tmpEy = iMaxY - 5;

			for (y=tmpSy ; y<tmpEy ; y++)
			{
				iSum_Top	= aiHistY[y-iSy-4] + aiHistY[y-iSy-3] + aiHistY[y-iSy-2] + aiHistY[y-iSy-1];
				iSum_Bottom = aiHistY[y-iSy+4] + aiHistY[y-iSy+3] + aiHistY[y-iSy+2] + aiHistY[y-iSy+1];

				iGap = iSum_Top - iSum_Bottom;

				if ( iGap > iMax_Up2 && (iGap>_CIRCLE_MARK_GAP) )
				{
					iMax_Up2 = iGap;
					iMinY2 = y;
				}
			}

			if( (iMaxY-iMinY2)>50 && (iMaxY-iMinY2)<250)
			{
				iMinY = iMinY2;
			}
			else if( (iMaxY2-iMinY)>50 && (iMaxY2-iMinY)<250)
			{
				iMaxY = iMaxY2;
			}
		}
		else if( (iMaxY-iMinY) > _CIRCLE_MARK_GAP )
		{
			int iMaxY2		= -9999;
			int iMinY2		= -9999;
			int iMax_Up2	= 0;
			int iMax_Down2	= 0;

			int tmpSy = iMinY + 5;
			int tmpEy = iMaxY - 5;

			for (y=tmpSy ; y<tmpEy ; y++)
			{
				iSum_Top	= aiHistY[y-iSy-4] + aiHistY[y-iSy-3] + aiHistY[y-iSy-2] + aiHistY[y-iSy-1];
				iSum_Bottom = aiHistY[y-iSy+4] + aiHistY[y-iSy+3] + aiHistY[y-iSy+2] + aiHistY[y-iSy+1];

				iGap = iSum_Top - iSum_Bottom;

				if ( iGap > iMax_Up2 && (iGap>_CIRCLE_MARK_GAP) )
				{
					iMax_Up2 = iGap;
					iMinY2 = y;
				}

				iGap = iSum_Bottom - iSum_Top;

				if ( iGap > iMax_Down2 && (iGap>_CIRCLE_MARK_GAP) )
				{
					iMax_Down2 = iGap;
					iMaxY2 = y;
				}
			}

			if( (iMinY2>0) && ((iMaxY-iMinY2)>50 && (iMaxY-iMinY2)<250) )
			{
				iMinY = iMinY2;
			}
			else if ( (iMaxY2>0) && ((iMaxY2-iMinY)>50 && (iMaxY2-iMinY)<250) )
			{
				iMaxY = iMaxY2;
			}
		}


		if(iMinX>iMaxX || (iMaxX-iMinX>maxSize) )
		{
			sLog.Format("[¸¶Å© #%d] ÁÂ¿ì ÀÎ½Ä À§Ä¡°¡ ºñÁ¤»ó ÀÔ´Ï´Ù.",i+1);
//			MSG_Display(sLog);
			return false;
		}


		if(iMinY>iMaxY || (iMaxY-iMinY>maxSize) )
		{
			sLog.Format("[¸¶Å© #%d] ÁÂ¿ì ÀÎ½Ä À§Ä¡°¡ ºñÁ¤»ó ÀÔ´Ï´Ù.",i+1);
//			MSG_Display(sLog);
			return false;
		}


		if ( iMaxX < 0 || iMaxY < 0 || iMaxX > sizeX || iMaxY > sizeY	||
			iMinX < 0 || iMinY < 0 || iMinX > sizeX || iMinY > sizeY )
		{
			sLog.Format("[¸¶Å© #%d] Max, Min À§Ä¡¸¦ Ã£Áö ¸øÇß½À´Ï´Ù.",i+1);
//			MSG_Display(sLog);
			return false;
		}
		Mark_Area(iMinX, iMinY, iMaxX, iMaxY, RGB(100,100,0), sizeX, sizeY, (LPBYTE)ucImage);

		cpFiducialPos[i].x = (iMaxX+iMinX)/2;
		cpFiducialPos[i].y = (iMaxY+iMinY)/2;
	}

	return true;
}


CPoint CAABonderDlg::GetCirclePos(LPBYTE Rgb, int sizeX, int sizeY, CRect crFiducialRoi)
{
	CString sLangChange;
	CString sLog;

	COLORREF *ucImage = (COLORREF *)Rgb;
//	long index;

	int maxSize = 400;

	int aiHistX[5000];
	int aiHistY[5000];

	crFiducialRoi.top=crFiducialRoi.top<0?0:crFiducialRoi.top;
	crFiducialRoi.left=crFiducialRoi.left<0?0:crFiducialRoi.left;

	int _CIRCLE_MARK_GAP=800;	// 800


	if ( crFiducialRoi.left < 0 || crFiducialRoi.top < 0 || crFiducialRoi.right > sizeX || crFiducialRoi.bottom > sizeY )
	{
		sLog.Format("¿øÇü ¸¶Å© °Ë»ö ¿µ¿ªÀÌ Àß¸øµÇ¾ú½À´Ï´Ù.");
//		MSG_Display(sLog);
		return false;
	}

	int iSx, iSy, iEx, iEy;
	int x, y, iPos;
	int iMaxX = -1, iMinX = 9999;
	int iMaxY = -1, iMinY = 9999;
	int iMax_Left = -1 , iMax_Right = -1, iMax_Up = -1, iMax_Down = -1;

	int iMaxX2 = -1, iMinX2 = 9999;
	int iMaxY2 = -1, iMinY2 = 9999;
	int iMax_Left2 = -1 , iMax_Right2 = -1, iMax_Up2 = -1, iMax_Down2 = -1;

	int iSum;		
	int iGap;

	int iSum_Left, iSum_Right;
	int iSum_Top, iSum_Bottom;

	iSx = crFiducialRoi.left;
	iSy = crFiducialRoi.top;
	iEx = crFiducialRoi.right;
	iEy = crFiducialRoi.bottom;

	::memset(aiHistY, 0, sizeof(int)*(5000));
	::memset(aiHistX, 0, sizeof(int)*(5000));


	/* X¹æÇâ È÷½ºÅä±×·¥ */
	for (x=iSx ; x<iEx ; x++)
	{
		iPos = iSy * sizeX + x;
		iSum = 0;

		for (y=iSy ; y<iEy ; y++)
		{
			//				iSum += (ucImage[iPos]*ucImage[iPos]);
			iSum += (ucImage[iPos]);
			iPos += sizeX;
		}

		aiHistX[x-iSx] = iSum;
	}

	/* X¹æÇâºÎÅÍ ¿§Áö Ã£±â */
	for (x=iSx+5 ; x<iEx-5 ; x++)
	{
		iSum_Left  = aiHistX[x-iSx-4] + aiHistX[x-iSx-3] + aiHistX[x-iSx-2] + aiHistX[x-iSx-1];
		iSum_Right = aiHistX[x-iSx+4] + aiHistX[x-iSx+3] + aiHistX[x-iSx+2] + aiHistX[x-iSx+1];

		iGap = iSum_Left - iSum_Right;

		if (iGap>_CIRCLE_MARK_GAP &&  iGap > iMax_Left)
		{
			iMax_Left = iGap;
			iMinX = x;
		}

		iGap = iSum_Right - iSum_Left;

		if (iGap>_CIRCLE_MARK_GAP &&  iGap > iMax_Right)
		{
			iMax_Right = iGap;
			iMaxX = x;
		}
	}


	iMax_Left = 0;
	iMinX2 = 0;
	int tmpSx = iMaxX-_CIRCLE_MARK_GAP;
	int tmpEx = iMinX - 30;
	if(tmpSx < (iSx+5))
		tmpSx = (iSx+5);
	if(tmpEx > (iEx-5))
		tmpEx = (iSx-5);

	for (x=tmpSx ; x<iMaxX; x++)
	{
		iSum_Left  = aiHistX[x-iSx-4] + aiHistX[x-iSx-3] + aiHistX[x-iSx-2] + aiHistX[x-iSx-1];
		iSum_Right = aiHistX[x-iSx+4] + aiHistX[x-iSx+3] + aiHistX[x-iSx+2] + aiHistX[x-iSx+1];

		iGap = iSum_Left - iSum_Right;

		if (iGap>_CIRCLE_MARK_GAP &&  iGap > iMax_Left)
		{
			iMax_Left = iGap;
			iMinX2 = x;
		}
	}


	iMax_Right = 0;
	iMaxX2 = 0;

	tmpSx = iMinX + 30;
	tmpEx = iMinX + _CIRCLE_MARK_GAP;
	if(tmpSx < (iSx+5))
		tmpSx = (iSx+5);
	if(tmpEx > (iEx-5))
		tmpEx = (iEx-5);


	for (x=tmpSx ; x<tmpEx; x++)
	{
		iSum_Left  = aiHistX[x-iSx-4] + aiHistX[x-iSx-3] + aiHistX[x-iSx-2] + aiHistX[x-iSx-1];
		iSum_Right = aiHistX[x-iSx+4] + aiHistX[x-iSx+3] + aiHistX[x-iSx+2] + aiHistX[x-iSx+1];

		iGap = iSum_Right - iSum_Left;

		if (iGap>_CIRCLE_MARK_GAP && iGap > iMax_Right)
		{
			iMax_Right = iGap;
			iMaxX2 = x;
		}
	}


	if( iMaxX-iMinX>30 && iMaxX-iMinX<maxSize && iMinX>0)
	{
		iMinX = iMinX;
		iMaxX = iMaxX;
	}
	else if( iMaxX2-iMinX>30 && iMaxX2-iMinX<maxSize && iMinX>0 )
	{
		iMinX = iMinX;
		iMaxX = iMaxX2;
	}
	else if( iMaxX-iMinX2>30 && iMaxX-iMinX2<maxSize && iMinX2>0 )
	{
		iMinX = iMinX2;
		iMaxX = iMaxX;
	}

	/* Y ¹æÇâ È÷½ºÅä±×·¥ */
	for (y=iSy ; y<iEy ; y++)
	{
		iPos = y * sizeX + iMinX;
		iSum = 0;

		for (x=iMinX ; x<iMaxX; x++)
		{
			iSum += ucImage[iPos++];
		}

		aiHistY[y-iSy] = iSum;
	}

	for (y=iSy+5 ; y<iEy-5 ; y++)
	{
		iSum_Top	= aiHistY[y-iSy-4] + aiHistY[y-iSy-3] + aiHistY[y-iSy-2] + aiHistY[y-iSy-1];
		iSum_Bottom = aiHistY[y-iSy+4] + aiHistY[y-iSy+3] + aiHistY[y-iSy+2] + aiHistY[y-iSy+1];

		iGap = iSum_Top - iSum_Bottom;

		if ( iGap > iMax_Up )
		{
			iMax_Up = iGap;
			iMinY = y;
		}

		iGap = iSum_Bottom - iSum_Top;

		if ( iGap > iMax_Down )
		{
			iMax_Down = iGap;
			iMaxY = y;
		}
	}


	if ( iMinY >= iMaxY )
	{
		int iMinY2		= -9999;
		int iMaxY2		= -9999;
		int iMax_Up2	= 0;
		int iMax_Down2	= 0;

		int tmpSy = iMinY + 5;
		int tmpEy = iEy-5;

		for (y=tmpSy ; y<tmpEy ; y++)
		{
			iSum_Top	= aiHistY[y-iSy-4] + aiHistY[y-iSy-3] + aiHistY[y-iSy-2] + aiHistY[y-iSy-1];
			iSum_Bottom = aiHistY[y-iSy+4] + aiHistY[y-iSy+3] + aiHistY[y-iSy+2] + aiHistY[y-iSy+1];

			iGap = iSum_Bottom - iSum_Top;

			if ( iGap > iMax_Down2 && (iGap>_CIRCLE_MARK_GAP) )
			{
				iMax_Down2 = iGap;
				iMaxY2 = y;
			}
		}


		tmpSy = iSy+5;
		tmpEy = iMaxY - 5;

		for (y=tmpSy ; y<tmpEy ; y++)
		{
			iSum_Top	= aiHistY[y-iSy-4] + aiHistY[y-iSy-3] + aiHistY[y-iSy-2] + aiHistY[y-iSy-1];
			iSum_Bottom = aiHistY[y-iSy+4] + aiHistY[y-iSy+3] + aiHistY[y-iSy+2] + aiHistY[y-iSy+1];

			iGap = iSum_Top - iSum_Bottom;

			if ( iGap > iMax_Up2 && (iGap>_CIRCLE_MARK_GAP) )
			{
				iMax_Up2 = iGap;
				iMinY2 = y;
			}
		}

		if( (iMaxY-iMinY2)>50 && (iMaxY-iMinY2)<250)
		{
			iMinY = iMinY2;
		}
		else if( (iMaxY2-iMinY)>50 && (iMaxY2-iMinY)<250)
		{
			iMaxY = iMaxY2;
		}
	}
	else if( (iMaxY-iMinY) > _CIRCLE_MARK_GAP )
	{
		int iMaxY2		= -9999;
		int iMinY2		= -9999;
		int iMax_Up2	= 0;
		int iMax_Down2	= 0;

		int tmpSy = iMinY + 5;
		int tmpEy = iMaxY - 5;

		for (y=tmpSy ; y<tmpEy ; y++)
		{
			iSum_Top	= aiHistY[y-iSy-4] + aiHistY[y-iSy-3] + aiHistY[y-iSy-2] + aiHistY[y-iSy-1];
			iSum_Bottom = aiHistY[y-iSy+4] + aiHistY[y-iSy+3] + aiHistY[y-iSy+2] + aiHistY[y-iSy+1];

			iGap = iSum_Top - iSum_Bottom;

			if ( iGap > iMax_Up2 && (iGap>_CIRCLE_MARK_GAP) )
			{
				iMax_Up2 = iGap;
				iMinY2 = y;
			}

			iGap = iSum_Bottom - iSum_Top;

			if ( iGap > iMax_Down2 && (iGap>_CIRCLE_MARK_GAP) )
			{
				iMax_Down2 = iGap;
				iMaxY2 = y;
			}
		}

		if( (iMinY2>0) && ((iMaxY-iMinY2)>50 && (iMaxY-iMinY2)<250) )
		{
			iMinY = iMinY2;
		}
		else if ( (iMaxY2>0) && ((iMaxY2-iMinY)>50 && (iMaxY2-iMinY)<250) )
		{
			iMaxY = iMaxY2;
		}
	}

	Mark_Area(iMinX, iMinY, iMaxX, iMaxY, RGB(100,100,0), sizeX, sizeY, (LPBYTE)ucImage);

	return CPoint((iMaxX+iMinX)/2, (iMaxY+iMinY)/2);
}

CPoint CAABonderDlg::GetCirclePos(LPBYTE Rgb, int sizeX, int sizeY, CRect crFiducialRoi,bool bOver)
{
	CString sLangChange;
	CString sLog;

	COLORREF *ucImage = (COLORREF *)Rgb;
//	long index;

	int maxSize = 400;

	crFiducialRoi.top=crFiducialRoi.top<0?0:crFiducialRoi.top;
	crFiducialRoi.left=crFiducialRoi.left<0?0:crFiducialRoi.left;

	int aiHistX[5000];
	int aiHistY[5000];

	int _CIRCLE_MARK_GAP=800;	// 800


	if ( crFiducialRoi.left < 0 || crFiducialRoi.top < 0 || crFiducialRoi.right > sizeX || crFiducialRoi.bottom > sizeY )
	{
		sLog.Format("¿øÇü ¸¶Å© °Ë»ö ¿µ¿ªÀÌ Àß¸øµÇ¾ú½À´Ï´Ù.");
//		MSG_Display(sLog);
		return false;
	}

	int iSx, iSy, iEx, iEy;
	int x, y, iPos;
	int iMaxX = -1, iMinX = 9999;
	int iMaxY = -1, iMinY = 9999;
	int iMax_Left = -1 , iMax_Right = -1, iMax_Up = -1, iMax_Down = -1;

	int iMaxX2 = -1, iMinX2 = 9999;
	int iMaxY2 = -1, iMinY2 = 9999;
	int iMax_Left2 = -1 , iMax_Right2 = -1, iMax_Up2 = -1, iMax_Down2 = -1;

	int iSum;		
	int iGap;

	int iSum_Left, iSum_Right;
	int iSum_Top, iSum_Bottom;

	iSx = crFiducialRoi.left;
	iSy = crFiducialRoi.top;
	iEx = crFiducialRoi.right;
	iEy = crFiducialRoi.bottom;

	::memset(aiHistY, 0, sizeof(int)*(5000));
	::memset(aiHistX, 0, sizeof(int)*(5000));


	/* X¹æÇâ È÷½ºÅä±×·¥ */
	for (x=iSx ; x<iEx ; x++)
	{
		iPos = iSy * sizeX + x;
		iSum = 0;

		for (y=iSy ; y<iEy ; y++)
		{
			//				iSum += (ucImage[iPos]*ucImage[iPos]);
			iSum += bOver?(ucImage[iPos]):(rgb(255,255,255)-ucImage[iPos]);
			iPos += sizeX;
		}

		aiHistX[x-iSx] = iSum;
	}

	/* X¹æÇâºÎÅÍ ¿§Áö Ã£±â */
	for (x=iSx+5 ; x<iEx-5 ; x++)
	{
		iSum_Left  = aiHistX[x-iSx-4] + aiHistX[x-iSx-3] + aiHistX[x-iSx-2] + aiHistX[x-iSx-1];
		iSum_Right = aiHistX[x-iSx+4] + aiHistX[x-iSx+3] + aiHistX[x-iSx+2] + aiHistX[x-iSx+1];

		iGap = iSum_Left - iSum_Right;

		if (iGap>_CIRCLE_MARK_GAP &&  iGap > iMax_Left)
		{
			iMax_Left = iGap;
			iMinX = x;
		}

		iGap = iSum_Right - iSum_Left;

		if (iGap>_CIRCLE_MARK_GAP &&  iGap > iMax_Right)
		{
			iMax_Right = iGap;
			iMaxX = x;
		}
	}


	iMax_Left = 0;
	iMinX2 = 0;
	int tmpSx = iMaxX-_CIRCLE_MARK_GAP;
	int tmpEx = iMinX - 30;
	if(tmpSx < (iSx+5))
		tmpSx = (iSx+5);
	if(tmpEx > (iEx-5))
		tmpEx = (iSx-5);

	for (x=tmpSx ; x<iMaxX; x++)
	{
		iSum_Left  = aiHistX[x-iSx-4] + aiHistX[x-iSx-3] + aiHistX[x-iSx-2] + aiHistX[x-iSx-1];
		iSum_Right = aiHistX[x-iSx+4] + aiHistX[x-iSx+3] + aiHistX[x-iSx+2] + aiHistX[x-iSx+1];

		iGap = iSum_Left - iSum_Right;

		if (iGap>_CIRCLE_MARK_GAP &&  iGap > iMax_Left)
		{
			iMax_Left = iGap;
			iMinX2 = x;
		}
	}


	iMax_Right = 0;
	iMaxX2 = 0;

	tmpSx = iMinX + 30;
	tmpEx = iMinX + _CIRCLE_MARK_GAP;
	if(tmpSx < (iSx+5))
		tmpSx = (iSx+5);
	if(tmpEx > (iEx-5))
		tmpEx = (iEx-5);


	for (x=tmpSx ; x<tmpEx; x++)
	{
		iSum_Left  = aiHistX[x-iSx-4] + aiHistX[x-iSx-3] + aiHistX[x-iSx-2] + aiHistX[x-iSx-1];
		iSum_Right = aiHistX[x-iSx+4] + aiHistX[x-iSx+3] + aiHistX[x-iSx+2] + aiHistX[x-iSx+1];

		iGap = iSum_Right - iSum_Left;

		if (iGap>_CIRCLE_MARK_GAP && iGap > iMax_Right)
		{
			iMax_Right = iGap;
			iMaxX2 = x;
		}
	}


	if( iMaxX-iMinX>30 && iMaxX-iMinX<maxSize && iMinX>0)
	{
		iMinX = iMinX;
		iMaxX = iMaxX;
	}
	else if( iMaxX2-iMinX>30 && iMaxX2-iMinX<maxSize && iMinX>0 )
	{
		iMinX = iMinX;
		iMaxX = iMaxX2;
	}
	else if( iMaxX-iMinX2>30 && iMaxX-iMinX2<maxSize && iMinX2>0 )
	{
		iMinX = iMinX2;
		iMaxX = iMaxX;
	}

	/* Y ¹æÇâ È÷½ºÅä±×·¥ */
	for (y=iSy ; y<iEy ; y++)
	{
		iPos = y * sizeX + iMinX;
		iSum = 0;

		for (x=iMinX ; x<iMaxX; x++)
		{
			iSum += bOver?(ucImage[iPos]):(rgb(255,255,255)-ucImage[iPos]);
			iPos++;
		}

		aiHistY[y-iSy] = iSum;
	}

	for (y=iSy+5 ; y<iEy-5 ; y++)
	{
		iSum_Top	= aiHistY[y-iSy-4] + aiHistY[y-iSy-3] + aiHistY[y-iSy-2] + aiHistY[y-iSy-1];
		iSum_Bottom = aiHistY[y-iSy+4] + aiHistY[y-iSy+3] + aiHistY[y-iSy+2] + aiHistY[y-iSy+1];

		iGap = iSum_Top - iSum_Bottom;

		if ( iGap > iMax_Up )
		{
			iMax_Up = iGap;
			iMinY = y;
		}

		iGap = iSum_Bottom - iSum_Top;

		if ( iGap > iMax_Down )
		{
			iMax_Down = iGap;
			iMaxY = y;
		}
	}


	if ( iMinY >= iMaxY )
	{
		int iMinY2		= -9999;
		int iMaxY2		= -9999;
		int iMax_Up2	= 0;
		int iMax_Down2	= 0;

		int tmpSy = iMinY + 5;
		int tmpEy = iEy-5;

		for (y=tmpSy ; y<tmpEy ; y++)
		{
			iSum_Top	= aiHistY[y-iSy-4] + aiHistY[y-iSy-3] + aiHistY[y-iSy-2] + aiHistY[y-iSy-1];
			iSum_Bottom = aiHistY[y-iSy+4] + aiHistY[y-iSy+3] + aiHistY[y-iSy+2] + aiHistY[y-iSy+1];

			iGap = iSum_Bottom - iSum_Top;

			if ( iGap > iMax_Down2 && (iGap>_CIRCLE_MARK_GAP) )
			{
				iMax_Down2 = iGap;
				iMaxY2 = y;
			}
		}


		tmpSy = iSy+5;
		tmpEy = iMaxY - 5;

		for (y=tmpSy ; y<tmpEy ; y++)
		{
			iSum_Top	= aiHistY[y-iSy-4] + aiHistY[y-iSy-3] + aiHistY[y-iSy-2] + aiHistY[y-iSy-1];
			iSum_Bottom = aiHistY[y-iSy+4] + aiHistY[y-iSy+3] + aiHistY[y-iSy+2] + aiHistY[y-iSy+1];

			iGap = iSum_Top - iSum_Bottom;

			if ( iGap > iMax_Up2 && (iGap>_CIRCLE_MARK_GAP) )
			{
				iMax_Up2 = iGap;
				iMinY2 = y;
			}
		}

		if( (iMaxY-iMinY2)>50 && (iMaxY-iMinY2)<250)
		{
			iMinY = iMinY2;
		}
		else if( (iMaxY2-iMinY)>50 && (iMaxY2-iMinY)<250)
		{
			iMaxY = iMaxY2;
		}
	}
	else if( (iMaxY-iMinY) > _CIRCLE_MARK_GAP )
	{
		int iMaxY2		= -9999;
		int iMinY2		= -9999;
		int iMax_Up2	= 0;
		int iMax_Down2	= 0;

		int tmpSy = iMinY + 5;
		int tmpEy = iMaxY - 5;

		for (y=tmpSy ; y<tmpEy ; y++)
		{
			iSum_Top	= aiHistY[y-iSy-4] + aiHistY[y-iSy-3] + aiHistY[y-iSy-2] + aiHistY[y-iSy-1];
			iSum_Bottom = aiHistY[y-iSy+4] + aiHistY[y-iSy+3] + aiHistY[y-iSy+2] + aiHistY[y-iSy+1];

			iGap = iSum_Top - iSum_Bottom;

			if ( iGap > iMax_Up2 && (iGap>_CIRCLE_MARK_GAP) )
			{
				iMax_Up2 = iGap;
				iMinY2 = y;
			}

			iGap = iSum_Bottom - iSum_Top;

			if ( iGap > iMax_Down2 && (iGap>_CIRCLE_MARK_GAP) )
			{
				iMax_Down2 = iGap;
				iMaxY2 = y;
			}
		}

		if( (iMinY2>0) && ((iMaxY-iMinY2)>50 && (iMaxY-iMinY2)<250) )
		{
			iMinY = iMinY2;
		}
		else if ( (iMaxY2>0) && ((iMaxY2-iMinY)>50 && (iMaxY2-iMinY)<250) )
		{
			iMaxY = iMaxY2;
		}
	}

	Mark_Area(iMinX, iMinY, iMaxX, iMaxY, RGB(100,100,0), sizeX, sizeY, (LPBYTE)ucImage);

	return CPoint((iMaxX+iMinX)/2, (iMaxY+iMinY)/2);
}

void CAABonderDlg::Mark_Area(int x1, int y1, int x2, int y2, COLORREF color, int Width, int Height, LPBYTE Rgb)
{
	long index;
	COLORREF *rgb_p = (COLORREF *)Rgb;
	int thickness=5;
	for (int i = x1; i <= x2; i++)
	{
		for(int j=y1;j<y1+thickness;j++)
		{
			index = j * Width + i;
			if (index >= 0 && index < Width * Height) *(rgb_p + index ) = color;
		}

		for(int j=y2;j<y2+thickness;j++)
		{
			index = j * Width + i;
			if (index >= 0 && index < Width * Height) *(rgb_p + index ) = color;
		}
	}

	for (int i = y1; i <= y2; i++)
	{
		for(int j=x1;j<x1+thickness;j++)
		{
			index = Width * i + j;
			if (index >= 0 && index < Width * Height) *(rgb_p + index ) = color;
		}

		for(int j=x2;j<x2+thickness;j++)
		{
			index = Width * i + j;
			if (index >= 0 && index < Width * Height) *(rgb_p + index ) = color;
		}
	}
}

void CAABonderDlg::Mark_Cross(int x1, int y1, int x2, int y2, COLORREF color, int Width, int Height, LPBYTE Rgb)
{
	long index;
	COLORREF *rgb_p = (COLORREF *)Rgb;
	int thickness=5;

	for (int i = x1-(x2/2); i <= x1+(x2/2); i++)
	{
		for(int j=y1;j<y1+thickness;j++)
		{
			index = j * Width + i;
			if (index >= 0 && index < Width * Height) *(rgb_p + index ) = color;
		}
	}

	for (int i = y1-(y2/2); i <= y1+(y2/2); i++)
	{
		for(int j=x1;j<x1+thickness;j++)
		{
			index = Width * i + j;
			if (index >= 0 && index < Width * Height) *(rgb_p + index ) = color;
		}
	}
}


void CAABonderDlg::WriteRohm12Cbulk(int wRegAddr,unsigned char *wRegData,int len)
{
}


void CAABonderDlg::OnBnClickedButtonAlarm()
{
	ctrlSubDlg(ALARM_DLG);
	changeMainBtnColor(ALARM_DLG);
}
void CAABonderDlg::OnBnClickedButtonLight()
{
	//CLightDlg*			lightDlg = NULL;
	//if (m_bisLightBtn)	m_bisLightBtn = false;
	//else				m_bisLightBtn = true;

	ctrlSubDlg(IDD_DIALOG_LIGHT);
	changeMainBtnColor(IDD_DIALOG_LIGHT);

}

BOOL CAABonderDlg::DestroyWindow()
{
	g_ADOData.func_AA_DBDisConnect();

	return CDialogEx::DestroyWindow();
}


//! Added by LHW (2013/4/29, 2013/5/3)
//! UV Lamp¿¡¼­ÀÇ ¼ö½Å ¹®ÀÚ¿­ Ã³¸® º»Ã¼ ÇÔ¼ö
//! [ÀÔ·Â°ª]
//!  sRecv : ¼ö½ÅµÈ ¹®ÀÚ¿­, iCnt_Recv : ¸î ¹øÂ° ¼ö½Å ÀÛ¾÷À¸·Î È£ÃâµÇ¾ú´ÂÁö¸¦ ³ªÅ¸³½´Ù, iIndex_Machine : UV Lamp ÀåÄ¡ÀÇ ±¸ºÐ index
//! [ÁÖÀÇ »çÇ×]
//!  UV Lamp´Â RS-232C Command°¡ ÀûÀýÄ¡ ¸øÇÒ °æ¿ì, 'ERR' ¹®ÀÚ¿­À» PC·Î º¸³½´Ù. 

void CAABonderDlg::ReceiveStop()
{
//	unsigned int    errorCode;
//	if(gMIUDevice.CurrentState == TRUE)
//	{
//		for(int i = 0; i < 100; i++)
//		{
//			errorCode = MIUStop(gDeviceIndex);
//
//			if(errorCode != 0)
//			{
//				errorCode = MIUStop(gDeviceIndex);
//				Sleep(10);
//			}
//			else
//			{
//				break;
//			}
//		}
//
//		//StopDevice(gDeviceIndex);
//	}
//	
//
//	MIUSetMCLK(gDeviceIndex, 0.0f);
//	MIUSetIOPowerChannel(gDeviceIndex, 0.0f, 0);
//	MIUSetPowerChannel0(gDeviceIndex, 0.0f, 0);
//	MIUSetPowerChannel1(gDeviceIndex, 0.0f, 0);
//	MIUSetPowerChannel2(gDeviceIndex, 0.0f, 0);
//	MIUSetPowerChannel3(gDeviceIndex, 0.0f, 0);
//
////	MIUSetPowerOTP(gDeviceIndex, 0.0f, 0);
//
//
//	if((gDeviceIndex == 0 || gDeviceIndex == 2))
//	{
//		int nDeviceIndex = gDeviceIndex + 1;
//		if(gMIUDevice.CurrentState == TRUE)
//		{
//			for(int i = 0; i < 100; i++)
//			{
//				errorCode = MIUStop(nDeviceIndex);
//				if(errorCode != 0)
//				{
//					errorCode = MIUStop(nDeviceIndex);
//					Sleep(10);
//				}
//				else
//				{
//					break;
//				}
//			}
//
//			//StopDevice(nDeviceIndex);
//		}
//
//		MIUSetMCLK(nDeviceIndex, 0.0f);
//		MIUSetIOPowerChannel(nDeviceIndex, 0.0f, 0);
//		MIUSetPowerChannel0(nDeviceIndex, 0.0f, 0);
//		MIUSetPowerChannel1(nDeviceIndex, 0.0f, 0);
//		MIUSetPowerChannel2(nDeviceIndex, 0.0f, 0);
//		MIUSetPowerChannel3(nDeviceIndex, 0.0f, 0);
//
////		MIUSetPowerOTP(gDeviceIndex, 0.0f, 0);
//	}

}

void CAABonderDlg::func_ChipID_Draw()
{
	CString strID="";
	strID.Format("%s", Task.ChipID);

	m_labelCCD_ID.SetText(Task.ChipID); 
	m_labelCCD_ID.Invalidate();
}

void CAABonderDlg::changeMainBtnColor(int dlg)
{
	if (dlg == m_oldDlg)
	{
		m_bMainBtn_Main.m_iStateBtn = 1;
	}
	else
	{
		m_bMainBtn_Main.m_iStateBtn =0;
		m_bMainBtn_Model.m_iStateBtn =0;
		m_bMainBtn_Align.m_iStateBtn =0;
		m_bMainBtn_CCD.m_iStateBtn =0;
		m_bMainBtn_Motor.m_iStateBtn =0;
		m_bMainBtn_IO.m_iStateBtn =0;
		m_bMainBtn_Light.m_iStateBtn =0;
		m_bMainBtn_Alarm.m_iStateBtn =0;
	}


	switch(dlg)
	{
	case MAIN_DLG:
		if (m_bMainBtn_Main.m_iStateBtn)
			m_bMainBtn_Main.m_iStateBtn = 0;
		else
			m_bMainBtn_Main.m_iStateBtn = 1;
		break;

	case MODEL_DLG:
		if (m_bMainBtn_Model.m_iStateBtn)
			m_bMainBtn_Model.m_iStateBtn = 0;
		else
		{
			m_bMainBtn_Model.m_iStateBtn = 1;
			m_bMainBtn_Main.m_iStateBtn = 0;
		}
		break;

	case PCB_DLG:
		if (m_bMainBtn_Align.m_iStateBtn)
			m_bMainBtn_Align.m_iStateBtn = 0;
		else
		{
			m_bMainBtn_Align.m_iStateBtn = 1;
			m_bMainBtn_Main.m_iStateBtn = 0;
		}
		break;

	case CCD_DLG:
		if (m_bMainBtn_CCD.m_iStateBtn)
			m_bMainBtn_CCD.m_iStateBtn = 0;
		else
		{
			m_bMainBtn_CCD.m_iStateBtn = 1;
			m_bMainBtn_Main.m_iStateBtn = 0;
		}
		break;

	case MOTOR_DLG2:
		if (m_bMainBtn_Motor.m_iStateBtn)
			m_bMainBtn_Motor.m_iStateBtn = 0;
		else
		{
			m_bMainBtn_Motor.m_iStateBtn = 1;
			m_bMainBtn_Main.m_iStateBtn = 0;
		}
		break;

	case IO_DLG:
		if (m_bMainBtn_IO.m_iStateBtn)
			m_bMainBtn_IO.m_iStateBtn = 0;
		else
		{
			m_bMainBtn_IO.m_iStateBtn = 1;
			m_bMainBtn_Main.m_iStateBtn = 0;
		}
		break;
	case IDD_DIALOG_LIGHT:
		if (m_bMainBtn_Light.m_iStateBtn)
			m_bMainBtn_Light.m_iStateBtn = 0;
		else
		{
			m_bMainBtn_Light.m_iStateBtn = 1;		
			m_bMainBtn_Main.m_iStateBtn = 0;
		}
		break;
	case ALARM_DLG:
		if (m_bMainBtn_Alarm.m_iStateBtn)
			m_bMainBtn_Alarm.m_iStateBtn = 0;
		else
		{
			m_bMainBtn_Alarm.m_iStateBtn = 1;		
			m_bMainBtn_Main.m_iStateBtn = 0;
		}
		break;

		
	}

	m_bMainBtn_Main.Invalidate();
	m_bMainBtn_Model.Invalidate();
	m_bMainBtn_Align.Invalidate();
	m_bMainBtn_CCD.Invalidate();
	m_bMainBtn_Motor.Invalidate();
	m_bMainBtn_IO.Invalidate();
	m_bMainBtn_Alarm.Invalidate();
	m_bMainBtn_Light.Invalidate();

	m_oldDlg = dlg;
}

void CAABonderDlg::OnStnClickedLabelStatusServo()
{
	Dio.setAlarm(ALARM_ON);
}

bool CAABonderDlg::MIUCheck_process()
{
	CString logStr=""; 

	/*if(gMIUDevice.CurrentState !=0) 
	{
		MIU.Stop();
	}*/

	if (MIU.m_pBoard->IsGrabStarted())		//if ( gMIUDevice.CurrentState >= 2 )
	{
		MIU.Stop();
		Sleep(300);
		MIU.Close();
		Sleep(500);
	}

	Sleep(100);
	double ep = myTimer(true);

	if(m_bMiuRun)
	{
		logStr.Format("[ CCD ] µ¿ÀÛ ÁßÀÔ´Ï´Ù.");
		putListLog(logStr);
		delayMsg(logStr.GetBuffer(100), 3000, M_COLOR_RED);
		m_bMiuRun = false;
		return false ;
	}
	Sleep(100);
	if(gMIUDevice.bMIUOpen==0)		//if(!MIU.m_pBoard->IsConnected()) 
	{
		if(!MIU.Open())
		{
			m_bMiuRun = false;
			theApp.MainDlg->putListLog("	MIU Open ½ÇÆÐ.");
			return false;
		}
	}
	
	double time3 = myTimer(true);
	if(!MIU.Start())
	{ 
		m_bMiuRun = false;
		logStr.Format("MIU Start ½ÇÆÐ.");
		putListLog(logStr);
		LogSave(logStr);

		return false;
	}
	else
	{
		if (!bThreadCcmGrabRun)
		{
			CcmThreadStart();
		}
	}

	double time4 = myTimer(true);
	logStr.Format("MIU Start ¿Ï·á.");//logStr.Format("Start ¿Ï·á. [%.0f]", time4-time3);
	putListLog(logStr);

	/*for(int i=0; i<100; i++)
	{
		Sleep(1);
		checkMessage();
	}*/


	//float        fSensorFrameRate = 0;
	////MIUGetSensorFrameRate(0, &fSensorFrameRate);	//KYH ÁÖ¼® -> Frame¼ö°¡ ¾Èµé¾î¿È.
	//fSensorFrameRate = 5;	//ÀÓ½Ã Frame


	//if( !_finite(fSensorFrameRate) || fSensorFrameRate<3 )
	//{
	//	for(int i=0; i<100; i++)
	//	{
	//		Sleep(1);
	//		checkMessage();
	//		checkMessage();
	//	}

	//	MIUGetSensorFrameRate(0, &fSensorFrameRate);

	//	if( !_finite(fSensorFrameRate) || fSensorFrameRate<3 )
	//	{
	//		gMIUDevice.CurrentState = 0;

	//		MIU.Stop();					// 95 ~ 100 msec

	//		sLangChange.LoadStringA(IDS_STRING446);	//CCD µ¿¿µ»ó ½ÇÆÐ
	//		logStr.Format(sLangChange);
	//		delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);

	//		sLangChange.LoadStringA(IDS_STRING1424);	//Ä«¸Þ¶ó ¸ðµâ Åë½Å Close.
	//		pFrame->putListLog(sLangChange);

	//		m_bMiuRun = false;

	//		return false;
	//	}
	//}
	//else
	//{
	//	
	//	return true;
	//	//}
	//}
	gMIUDevice.CurrentState = 4;
	m_bMiuRun = true;
	return true;
}

int CAABonderDlg::NG_process(int iStep)
{
	CString sLog;
	int iRtnFunction = iStep;

	switch(iStep)
	{
	case  150000:

		if(Task.m_bNgUnload == true)
		{
			if(Task.m_bOkUnload == true)
				Task.m_bOkFlag = 1;
			else
				Task.m_bOkFlag = -1;

			iRtnFunction = 150100;

			m_btnNgOut.m_iStateBtn = 2;
			m_btnNgOut.Invalidate();
		}
		else
		{
			Task.m_bOkFlag = 0;
			m_btnNgOut.m_iStateBtn = 0;
			m_btnNgOut.Invalidate();

			iRtnFunction = -(Task.PausePCBStep);
			//Task.LensTask = -(Task.PauseLensStep);
		}
		break;

	case  150100:
		{
			double posLensX = fabs(motor.GetEncoderPos(Motor_Lens_X)-model.axis[Motor_Lens_X].pos[Bonding_Pos]);
			double posLensY = fabs(motor.GetEncoderPos(Motor_Lens_Y)-model.axis[Motor_Lens_Y].pos[Bonding_Pos]);
			double posLensZ = fabs(motor.GetCommandPos(Motor_Lens_Z)-model.axis[Motor_Lens_Z].pos[Bonding_Pos]);

			double posPcbX = fabs(motor.GetEncoderPos(Motor_PCB_X)-model.axis[Motor_PCB_X].pos[Bonding_Pos]);
			double posPcbY = fabs(motor.GetEncoderPos(Motor_PCB_Y)-model.axis[Motor_PCB_Y].pos[Bonding_Pos]);

			if ( (posLensX<3 && posLensY<3 && posLensZ<2 && posPcbX<3 && posPcbY<3)
				&& (Task.LensTask >= 30000) )
			{
				if(!/*gPassUV*/sysData.m_iUVPass == 1 && Task.m_bUvPassUnload == true)
				{// UV Shot -> Grip Open -> ZÃà »ó½Â->Lens ´ë±â À§Ä¡->PCB ¹èÃâ-> NG ¿äÃ» StepÀ¸·Î ÀÌµ¿
					iRtnFunction = 110000;
				}
				else
				{// Grip Open -> ZÃà »ó½Â->Lens ´ë±â À§Ä¡->PCB ¹èÃâ-> NG ¿äÃ» StepÀ¸·Î ÀÌµ¿
					iRtnFunction = 115000;
				}
			}
			else
			{
				Task.m_iRetry_Opt = 0;

				if(posLensX<3 && posLensY<3 && posLensZ<3)			// ZÃà »ó½Â->Lens ´ë±â À§Ä¡->PCB ¹èÃâ-> NG ¿äÃ» StepÀ¸·Î ÀÌµ¿
					iRtnFunction = 120000;
 				else
 					iRtnFunction = 121000;							// ZÃà »ó½Â Pass ÇÏ°í PCB ¹èÃâ-> NG ¿äÃ» StepÀ¸·Î ÀÌµ¿
			}
		}

		break;
	}

	return iRtnFunction;
}


void CAABonderDlg::OnBnClickedButtonNgOut()
{
	if(sysData.m_FreeRun == 1)
	{
		sLangChange.LoadStringA(IDS_STRING234);
		errMsg2(Task.AutoFlag, sLangChange);
		return;
	}


	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING364);
		putListLog(sLangChange);
		return;
	}


#ifdef	ON_LINE_MODE
	if(Task.AutoReday == 0 && Task.AutoFlag == 0)
	{
		sLangChange.LoadStringA(IDS_STRING1326);	//¿îÀü ÁØºñ°¡ µÇÁö ¾Ê¾Ò½À´Ï´Ù.
		errMsg2(Task.AutoFlag, sLangChange);
		return;
	}




	if(Dio.PCBvaccumOnCheck(true, false) == false)
	{
		errMsg2(Task.AutoFlag, "PCB ÈíÂø °¨Áö¿¡ ½ÇÆÐ Çß½À´Ï´Ù.");
		return;
	}
#endif

//	CNgList dlg;

	//if (dlg.DoModal() == IDOK)
	//{
	//	if(g_bMovingflag)
	//		return;

	//	//Task.LensTask	= Task.PauseLensStep;
	//	Task.PCBTask	= 150000;
	//	Task.PcbOnStage = 200;

	//	Task.m_iStart_Step_PCB = 10000;
	//	Task.m_iEnd_Step_PCB = 160000;
	//	Task.m_iStart_Step_LENS = 10000;
	//	Task.m_iEnd_Step_LENS = 60000;

	//	g_bMovingflag =true;
	//	Dio.setAlarm(AUTO_RUN);
	//	Task.AutoFlag = 1;
	//	AutoRunView(Task.AutoFlag);


	//	m_btnNgOut.m_iStateBtn = 2;
	//	m_btnNgOut.Invalidate();

	//	Task.PCBTaskTime = Task.LensTaskTime = myTimer(true);

	//	Task.AutoReday = 0;

	//	pThread_TaskPcb = ::AfxBeginThread(Thread_TaskPcb, this);
	//	//pThread_TaskLens = ::AfxBeginThread(Thread_TaskLens, this);
	//}
}


void CAABonderDlg::dispLotName()
{
	CString tmpStr;
	tmpStr.Format("%s", Task.LotNo);
	m_labelLotName.SetText(tmpStr);
}


void CAABonderDlg::OnBnClickedButtonPcbResult()
{
	if(Task.AutoFlag==1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//ÀÚµ¿ ¿îÀü Áß ÀÔ´Ï´Ù.
		delayMsg(sLangChange, 3000, M_COLOR_GREEN);
	}
	else if(Task.AutoFlag==0 || Task.AutoFlag==2)
	{
		if(Task.PcbOnStage==200)
		{
			Task.PcbOnStage = 1;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING1299);	//¾çºÒ ÆÇÁ¤°ªÀ» ÀÔ·Â ÇÏ¼¼¿ä. [¾çÇ° Yes / ºÒ·® No]
			if(askMsg(sLangChange))
				Task.m_bOkFlag = 1;
			else
				Task.m_bOkFlag = -1;

			Task.PcbOnStage = 200;
		}
	}
}

int CAABonderDlg::freeRun()
{
	if(g_bMovingflag)
		return -1;
	
	if (sysData.m_FreeRun == 1)
	{
		sysData.m_FreeRun	= 0;

		Task.AutoReday		= 0;
		Task.AutoFlag		= 0;
		AutoRunView(Task.AutoFlag);

		bThreadTaskPcb =0;			//	 Á¤Áö ÇÒ¶§ »ç¿ë 0:¾²·¹µå ºüÁ® ³ª¿È
		bThreadTaskLens =0;			//	 Á¤Áö ÇÒ¶§ »ç¿ë 0:¾²·¹µå ºüÁ® ³ª¿È

		sLangChange.LoadStringA(IDS_STRING1162);	//°øÈ¸ÀüÀ» Á¤Áö ÇÏ¿´½À´Ï´Ù.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
		return 0 ;
	}
	else if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//ÀÚµ¿ ¿îÀü Áß ÀÔ´Ï´Ù.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
		return -1;
	}
	else if(Task.AutoFlag == 2)
	{
		sLangChange.LoadStringA(IDS_STRING1361);	//ÀÏ½Ã Á¤Áö »óÅÂ ÀÔ´Ï´Ù.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
		return -1;
	}
	else if(Task.AutoFlag == 0)
	{
		if(Task.AutoReday == 0)
		{
			sLangChange.LoadStringA(IDS_STRING1326);	//¿îÀü ÁØºñ°¡ µÇÁö ¾Ê¾Ò½À´Ï´Ù.
			errMsg2(Task.AutoFlag, sLangChange);
			return -1;
		}

		sLangChange.LoadStringA(IDS_STRING1161);	//°øÈ¸ÀüÀ» ½ÃÀÛ ÇÏ½Ã°Ú½À´Ï±î?
		if(askMsg(sLangChange) == false)
			return 0;


		Task.LensTask	= 10000;
		Task.PCBTask	= 10000;

		//checkTablePcb();


		Task.m_iStart_Step_PCB = 10000;
		Task.m_iEnd_Step_PCB = 130000;
		Task.m_iStart_Step_LENS = 10000;
		Task.m_iEnd_Step_LENS = 60000;

		g_bMovingflag = true;

		Dio.setAlarm(AUTO_STOP);

		Task.AutoFlag = 1;
		sysData.m_FreeRun = 1;
		Task.AutoReday = 0;


		AutoRunView(Task.AutoFlag);
		Task.PCBTaskTime = Task.LensTaskTime = myTimer(true);

		Task.m_timeChecker.Init_Time();
		Task.m_timeChecker.Start_Time();

		pThread_TaskPcb = ::AfxBeginThread(Thread_TaskPcb, this);
		pThread_TaskLens = ::AfxBeginThread(Thread_TaskLens, this);
	}

	return true;
}


void CAABonderDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 999)
	{
		sLangChange.LoadStringA(IDS_STRING1404);
		if(Task.PcbOnStage == 200){
			GetDlgItem(IDC_BUTTON_PCB_RESULT)->SetWindowText(sLangChange);
			m_bPcbFinish.m_iStateBtn = 1;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING1403);
			GetDlgItem(IDC_BUTTON_PCB_RESULT)->SetWindowText(sLangChange);
			m_bPcbFinish.m_iStateBtn = 0;	
		}
		m_bPcbFinish.Invalidate();

		if(Task.m_bOkDispense == 1){
			sLangChange.LoadStringA(IDS_STRING1200);
			GetDlgItem(IDC_BUTTON_DISPENSE_RESULT)->SetWindowText(sLangChange);
			m_bDispenseFinish.m_iStateBtn = 1;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING1198);
			GetDlgItem(IDC_BUTTON_DISPENSE_RESULT)->SetWindowText(sLangChange);
			m_bDispenseFinish.m_iStateBtn = 0;
		}
		m_bDispenseFinish.Invalidate();

		if(Task.m_bOKLensPass == 1){
			sLangChange.LoadStringA(IDS_STRING717);
			GetDlgItem(IDC_BUTTON_LENS_PASS_RESULT)->SetWindowText(sLangChange);
			m_bLensPassFinish.m_iStateBtn = 1;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING716);
			GetDlgItem(IDC_BUTTON_LENS_PASS_RESULT)->SetWindowText(sLangChange);
			m_bLensPassFinish.m_iStateBtn = 0;
		}
		m_bLensPassFinish.Invalidate();

		if(sysData.m_iProductComp == 1){
			m_bProCompCheck.m_iStateBtn = 1;
		}
		else
		{
			m_bProCompCheck.m_iStateBtn = 0;
		}
		m_bProCompCheck.Invalidate();

		if(bThreadTaskReadyRun == true)
		{
			if(iReadyRunCnt%4<2)
				m_btnReady.m_iStateBtn = 0;
			else
				m_btnReady.m_iStateBtn = 1;

			m_btnReady.Invalidate();

			iReadyRunCnt++;
		}

		if(Task.AutoReday==1)
		{
			double pcbX = fabs(motor.GetEncoderPos(Motor_PCB_X) - model.axis[Motor_PCB_X].pos[Wait_Pos]);
			double pcbY = fabs(motor.GetEncoderPos(Motor_PCB_Y) - model.axis[Motor_PCB_Y].pos[Wait_Pos]);
			double LensX = fabs(motor.GetEncoderPos(Motor_Lens_X) - model.axis[Motor_Lens_X].pos[Wait_Pos]);
			double LensY = fabs(motor.GetEncoderPos(Motor_Lens_Y) - model.axis[Motor_Lens_Y].pos[Wait_Pos]);
			double LensZ = fabs(motor.GetCommandPos(Motor_Lens_Z) - model.axis[Motor_Lens_Z].pos[Wait_Pos]);

			if(pcbX>0.5 || pcbY>0.5 || LensX>0.5 || LensY>0.5 || LensZ>0.5)
			{
				Task.AutoReday = 0;

				m_btnReady.m_iStateBtn = 0;
				m_btnReady.Invalidate();
			}	
		}
		else if(alarmDlg != NULL)
		{
			if(g_AlarmFlag)				// ¾Ë¶÷ ¾øÀ» ¶§..
			{
				if (alarmDlg->IsWindowVisible())
				{
					if(alarmDlg->m_iAlarmKind==1)
						alarmDlg->m_btnAlarmMonitor.m_iStateBtn = 1;
					else
						alarmDlg->m_btnAlarmMonitor.m_iStateBtn = 0;

					m_bMainBtn_Alarm.m_iStateBtn = 1;
				}
				else
					m_bMainBtn_Alarm.m_iStateBtn = 0;

				g_AlarmCnt = 0;
			}
			else
			{
				if((g_AlarmCnt%4) < 2)
				{
					m_bMainBtn_Alarm.m_iStateBtn = 2;
					alarmDlg->m_btnAlarmMonitor.m_iStateBtn = 2;
				}
				else
				{
					if (alarmDlg->IsWindowVisible())
					{
						m_bMainBtn_Alarm.m_iStateBtn = 1;

						if(alarmDlg->m_iAlarmKind==1)
							alarmDlg->m_btnAlarmMonitor.m_iStateBtn = 1;
						else
							alarmDlg->m_btnAlarmMonitor.m_iStateBtn = 0;

						alarmDlg->m_btnAlarmMonitor.Invalidate();
					}
					else
						m_bMainBtn_Alarm.m_iStateBtn = 0;
				}

				g_AlarmCnt++;
			}

			alarmDlg->m_btnAlarmMonitor.Invalidate();
			m_bMainBtn_Alarm.Invalidate();

		}
	}

	if(nIDEvent == 9)
	{
		func_SocketControl_ConnectCheck();
	}

		motor.InDIO(0, curInDio[0]);
		motor.InDIO(2, curInDio[1]);

	CDialogEx::OnTimer(nIDEvent);
}


bool CAABonderDlg::MoveOffset_Prev_UV()
{
#if (____AA_WAY == PCB_TILT_AA)
	short axis[6] = {Motor_PCB_X, Motor_PCB_Y, Motor_PCB_Z, Motor_PCB_Xt, Motor_PCB_Yt, Motor_PCB_TH};
#elif (____AA_WAY == LENS_TILT_AA)
	short axis[6] = {Motor_Lens_X, Motor_Lens_Y, Motor_Lens_Z, Motor_Lens_Xt, Motor_Lens_Yt, Motor_PCB_TH};
#endif
	
	double dDes_Pos[6] = {0.0, };
	double offSetZ=0.0;

	//Task.SFR.dMaxPos[1~4]

	for(int i = 1; i < 5; i++)
	{
		offSetZ += Task.SFR.dMaxPos[i];
	}

	offSetZ = Task.SFR.dMaxPos[0]-(offSetZ/4);
	Task.dUvOffset_Aver = offSetZ * model.UV_Weight*-1;  

	if(Task.dUvOffset_Aver < (sysData.m_dOffset_Prev_UV_Z/2))
	{
		Task.dUvOffset_Aver = sysData.m_dOffset_Prev_UV_Z/2;
	}
	
	if(Task.dUvOffset_Aver > fabs(sysData.m_dOffset_Prev_UV_Z/2))
	{
		Task.dUvOffset_Aver = fabs(sysData.m_dOffset_Prev_UV_Z/2);
	}

	CString logStr;
	//logStr.Format("Uv Weight: %.3lf" , Task.dUvOffset_Aver);
	//putListLog(logStr);

	dDes_Pos[0] = motor.GetEncoderPos(axis[0]) + sysData.m_dOffset_Prev_UV_X;
	dDes_Pos[1] = motor.GetEncoderPos(axis[1]) + sysData.m_dOffset_Prev_UV_Y;
	//

	dDes_Pos[2] = motor.GetCommandPos(axis[2]) + sysData.m_dOffset_Prev_UV_Z;	// + Task.dUvOffset_Aver;

	
	//
	dDes_Pos[3] = motor.GetCommandPos(axis[3]) + sysData.m_dOffset_Prev_UV_Tx;
	dDes_Pos[4] = motor.GetCommandPos(axis[4]) + sysData.m_dOffset_Prev_UV_Ty;
	dDes_Pos[5] = motor.GetCommandPos(axis[5]) + sysData.m_dOffset_Prev_UV_Th;
	//
	motor.goMotorPos(6, axis, dDes_Pos, true);

	double iTime = myTimer(true);

	while (1)
	{
		if (myTimer(true) - iTime < MOTOR_MOVE_TIME)
		{
			if (motor.IsStopAxis(axis[0]) == true	&&
				motor.IsStopAxis(axis[1]) == true	&&
				motor.IsStopAxis(axis[2]) == true	&&
				motor.IsStopAxis(axis[3]) == true	&&
				motor.IsStopAxis(axis[4]) == true	&&
				motor.IsStopAxis(axis[5]) == true	)
				break;
		}
		else{
			return false;
		}
	}

	return true;
}


void CAABonderDlg::OnClickedLabelTitle()
{

	if (Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//ÀÚµ¿ ¿îÀü Áß ÀÔ´Ï´Ù.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
		return;
	}
	//UVCommand.Connect_Device(sysData.iCommPort[COMM_UV]);
	//UVCommand.UV_Shutter_PowerSet(95);//
#ifdef NORINDA_MODE
	g_FinalInspLog();
	AlignResultSave("1");
#endif
	

	

	if (bThreadUvRun == false)
	{
		//UVCommand2.UV_Shutter_Open();
		pThread_TaskUv = ::AfxBeginThread(Thread_TaskUv, this);
	}
	else
	{
		//UVCommand2.UV_Shutter_Close();
	}
	bThreadUvRun = !bThreadUvRun;

	Dio.setAlarm(ALARM_OFF);
}

void CAABonderDlg::DeleteOldData(int year, int month, int day)
{
	CString tmpStr;

	int deleteYear		= year;
	int deleteMonth		;
	int deleteDay		= day;

	if(sysData.m_Log_Retention_Period > 0)
	{
		deleteMonth		= month-sysData.m_Log_Retention_Period;
		while(deleteMonth < -12)
		{
			deleteMonth+=12;
			deleteYear--;
		}
	}
	else
	{
		deleteMonth		= month-1;
	}

	if(deleteMonth<1)
	{
		deleteMonth		= 12;
		deleteYear		= year-1;
	}

	int deleteYear2		= year;
	int deleteMonth2	= month-2;

	if(deleteMonth==0)
	{
		deleteMonth2	= 12;
		deleteYear2		= year-1;
	}
	else if(deleteMonth<0)
	{
		deleteMonth2	= 11;
		deleteYear2		= year-1;
	}


	CFileFind finder;
	BOOL IsFind;

	/*tmpStr.Format("%s\\%04d%02d\\%02d", ALARM_DIR, deleteYear, deleteMonth, deleteDay);
	IsFind = finder.FindFile(tmpStr);
	if(IsFind)
	{
		DeletePath(tmpStr);
		RemoveDirectory(tmpStr);
		return;
	}*/
	/*
	//20170615 ·Î±×´Â »èÁ¦ µ¥ÀÌÅÍ¿¡¼­ Á¦¿Ü ÁÖ¼®Ã³¸®
	tmpStr.Format("%s\\%04d%02d\\%02d", LOG_DIR, deleteYear, deleteMonth, deleteDay);
	IsFind = finder.FindFile(tmpStr);
	if(IsFind)
	{
		DeletePath(tmpStr);
		RemoveDirectory(tmpStr);
		return;
	}
	*/
	/*tmpStr.Format("%s\\%04d%02d\\%02d", IMG_DIR, deleteYear, deleteMonth, deleteDay);
	IsFind = finder.FindFile(tmpStr);
	if(IsFind)
	{
		DeletePath(tmpStr);
		RemoveDirectory(tmpStr);
		return;
	}*/
	/*
	//20170615 ·Î±×´Â »èÁ¦ µ¥ÀÌÅÍ¿¡¼­ Á¦¿Ü ÁÖ¼®Ã³¸®
	tmpStr.Format("%s\\%04d%02d\\%02d", RESULT_DIR, deleteYear, deleteMonth, deleteDay);
	IsFind = finder.FindFile(tmpStr);
	if(IsFind)
	{
		DeletePath(tmpStr);
		RemoveDirectory(tmpStr);
		return;
	}
	*/
	/*tmpStr.Format("%s\\%04d%02d\\%02d", BACKUP_DIR, deleteYear, deleteMonth, deleteDay);
	IsFind = finder.FindFile(tmpStr);
	if(IsFind)
	{
		DeletePath(tmpStr);
		RemoveDirectory(tmpStr);
		return;
	}

	tmpStr.Format("%s\\Backup\\%04d%02d\\%02d", BASE_DIR, deleteYear, deleteMonth, deleteDay);
	if(IsFind)
	{
		DeletePath(tmpStr);
		RemoveDirectory(tmpStr);
		return;
	}

	tmpStr.Format("%s\\%04d%02d", ALARM_DIR, deleteYear2, deleteMonth2);
	IsFind = finder.FindFile(tmpStr);
	if(IsFind)
	{
		DeletePath(tmpStr);
		RemoveDirectory(tmpStr);
		return;
	}*/
	/*
	//20170615 ·Î±×´Â »èÁ¦ µ¥ÀÌÅÍ¿¡¼­ Á¦¿Ü ÁÖ¼®Ã³¸®
	tmpStr.Format("%s\\%04d%02d", LOG_DIR, deleteYear2, deleteMonth2);
	IsFind = finder.FindFile(tmpStr);
	if(IsFind)
	{
		DeletePath(tmpStr);
		RemoveDirectory(tmpStr);
		return;
	}
	*/
	/*tmpStr.Format("%s\\%04d%02d", IMG_DIR, deleteYear2, deleteMonth2);
	IsFind = finder.FindFile(tmpStr);
	if(IsFind)
	{
		DeletePath(tmpStr);
		RemoveDirectory(tmpStr);
		return;
	}*/
	/*
	//20170615 ·Î±×´Â »èÁ¦ µ¥ÀÌÅÍ¿¡¼­ Á¦¿Ü ÁÖ¼®Ã³¸®
	tmpStr.Format("%s\\%04d%02d", RESULT_DIR, deleteYear2, deleteMonth2);
	IsFind = finder.FindFile(tmpStr);
	if(IsFind)
	{
		DeletePath(tmpStr);
		RemoveDirectory(tmpStr);
		return;
	}
	*/
	/*tmpStr.Format("%s\\%04d%02d", BACKUP_DIR, deleteYear2, deleteMonth2);
	IsFind = finder.FindFile(tmpStr);
	if(IsFind)
	{
		DeletePath(tmpStr);
		RemoveDirectory(tmpStr);
		return;
	}

	tmpStr.Format("%s\\Backup\\%04d%02d", BASE_DIR, deleteYear2, deleteMonth2);
	IsFind = finder.FindFile(tmpStr);
	if(IsFind)
	{
		DeletePath(tmpStr);
		RemoveDirectory(tmpStr);
		return;
	}*/
}

void CAABonderDlg::SetDigReference(int iMark)
{//Bright, Contrast ¼³Á¤

	long chRef;
	
	//if(iMark==0)			chRef = M_CH0_REF;
	//else if(iMark==1)		chRef = M_CH1_REF;
	if(iMark == PCB_Chip_MARK)				chRef = M_CH0_REF;	//CAM1
	else if(iMark == PCB_Holder_MARK)		chRef = M_CH0_REF;	//CAM1
	else if(iMark == LENS_Align_MARK)		chRef = M_CH1_REF;	//CAM2

#ifdef ON_LINE_VISION
	if(vision.MilDigitizer)
	{
		MdigReference(vision.MilDigitizer, M_BLACK_REF+chRef,0);//model.m_iBright[iMark]);
		MdigReference(vision.MilDigitizer, M_WHITE_REF+chRef, 0);//model.m_iContrast[iMark]);
	}
#endif
	//UpdateData(false);
}


void CAABonderDlg::InstantMarkDelete(int iMarkType)
{
	/*if((model.m_MarkSize[iMarkType][1].x == 0)|(model.m_MarkSize[iMarkType][1].y == 0))
		return;

	model.m_MarkCenter[iMarkType][1].x	=	0.0f;
	model.m_MarkCenter[iMarkType][1].y	=	0.0f;
	model.m_MarkSize[iMarkType][1].x	=	0;
	model.m_MarkSize[iMarkType][1].y	=	0;
	model.m_iLimitRate[iMarkType][1]	=	70;

	CString sDelFile;

#ifdef USE_GEOMETRIC
	MmodFree(vision.ModModel[iMarkType][1]);
	vision.ModModel[iMarkType][1] = M_NULL;
	
	if(iMarkType == PCB_Chip_MARK)			sDelFile.Format("%s\\Model\\%s\\LENS_Mark_%d.mod", DATA_DIR, model.mCurModelName, 1);
	else if(iMarkType == PCB_Holder_MARK)	sDelFile.Format("%s\\Model\\%s\\PCB_Mark_%d.mod", DATA_DIR, model.mCurModelName, 1);
	else									sDelFile.Format("%s\\Model\\%s\\LENS_Align_MARK_%d.mod", DATA_DIR, model.mCurModelName, 1);
#else
	MpatFree(vision.PatModel[iMarkType][m_iMarkNo]);
	vision.PatModel[iMarkType][m_iMarkNo] = M_NULL;

	if(iMarkType == PCB_Chip_MARK)			sDelFile.Format("%s\\Model\\%s\\LENS_Mark_%d.pat", DATA_DIR, model.mCurModelName, 1);
	else if(iMarkType == PCB_Holder_MARK)	sDelFile.Format("%s\\Model\\%s\\PCB_Mark_%d.pat", DATA_DIR, model.mCurModelName, 1);
	else									sDelFile.Format("%s\\Model\\%s\\LENS_Align_MARK_%d.pat", DATA_DIR, model.mCurModelName, 1);
#endif

	::DeleteFile(sDelFile);

	model.Save();*/
}

void CAABonderDlg::OnStnClickedLabelCcdRetry()
{
		CString logStr;
		sLangChange.LoadStringA(IDS_STRING1203);	//µ¿¿µ»ó Àç½Ãµµ È½¼ö¸¦ ÃÊ±âÈ­ ½ÃÅ°°Ú½À´Ï±î?
	if(askMsg(sLangChange))
	{
		work.m_iCoverUpDownCnt = 0;
	}

	logStr.Format("%d", work.m_iCoverUpDownCnt);
	m_labelCcdRetryCnt.SetText(logStr);
	m_labelCcdRetryCnt.Invalidate();
}


int	CAABonderDlg::procOisAf(int iStep)
{
	int iRtn;
	int iRtnFunction = 0;
	CString logStr;

	int code1 = model.code1;
	int code2 = model.code2;

	bool limitCheckFlag = false;

	if ( gMIUDevice.bMIUOpen == 0 || gMIUDevice.CurrentState != 4 )
	{
		sLangChange.LoadStringA(IDS_STRING447);
		errMsg2(Task.AutoFlag, sLangChange);	//CCD µ¿¿µ»ó Á¤Áö »óÅÂÀÔ´Ï´Ù 1
		return iStep;
	}

	iRtnFunction = iStep;

	switch (iStep)
	{
	case 50000:
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 50100;
		}
		break;

	case 50100:			/* ZÃà º»µù À§Ä¡ µµÂø ÈÄ */
		{
			if( (myTimer(true) - Task.PCBTaskTime) > model.strInfo_Cam[CCD].m_iDelayTime)
				iRtnFunction = 51000;
			else
				iRtnFunction = 50100;
		}
		break;


	case 51000:
		{
			MIU.MoveVCM_Code(0);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 51100;
		}
		break;

	case 51100:
		{
			if( (myTimer(true) - Task.PCBTaskTime) > model.strInfo_Cam[CCD].m_iDelayTime)
				iRtnFunction = 51200;
			else
				iRtnFunction = 51100;
		}
		break;

	case 51200:			/* ZÃà º»µù À§Ä¡ µµÂø ÈÄ */
		{
			iRtn = _getMTF(SFR_FIRST);

			Task.dFov[0] = Task.SFR.fImageAlign[0][0];
			sLangChange.LoadStringA(IDS_STRING458);	//CDOE %d, [DFOV %.03f] [Z %.03f]
			logStr.Format(sLangChange, 0, Task.dFov[0], motor.GetCommandPos((Motor_Lens_Z)));
			putListLog(logStr);

			if (iRtn>0)
				iRtnFunction = 52000;
			else
			{
				sLangChange.LoadStringA(IDS_STRING209);
				errMsg2(Task.AutoFlag, sLangChange);
				iRtnFunction = -51200;
			}
		}
		break;

	case 52000:		/* CODE1 °ª ÀÔ·Â ÈÄ sfr ÃøÁ¤ */
		{
			MIU.MoveVCM_Code(code1);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 52100;
		}
		break;

	case 52100:
		{
			if( (myTimer(true) - Task.PCBTaskTime) > model.strInfo_Cam[CCD].m_iDelayTime)
				iRtnFunction = 52200;
			else
				iRtnFunction = 52100;
		}
		break;

	case 52200:			/* ZÃà º»µù À§Ä¡ µµÂø ÈÄ */
		{
			iRtn = _getMTF(SFR_FIRST);

			Task.dFov[1] = Task.SFR.fImageAlign[1][0];
			sLangChange.LoadStringA(IDS_STRING458);	//CDOE %d, [DFOV %.03f] [Z %.03f]
			logStr.Format(sLangChange, 0, Task.dFov[0], motor.GetCommandPos((Motor_Lens_Z)));
			putListLog(logStr);

			if (iRtn>0)
				iRtnFunction = 53000;
			else
			{
				sLangChange.LoadStringA(IDS_STRING205);
				errMsg2(Task.AutoFlag, sLangChange);
				iRtnFunction = -52200;
			}
		}
		break;

	case 53000:		/* CODE2 °ª ÀÔ·Â ÈÄ sfr ÃøÁ¤ */
		{
			MIU.MoveVCM_Code(code2);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 53100;
		}
		break;

	case 53100:
		{
			if( (myTimer(true) - Task.PCBTaskTime) > model.strInfo_Cam[CCD].m_iDelayTime)
				iRtnFunction = 53200;
			else
				iRtnFunction = 53100;
		}
		break;

	case 53200:
		{
			iRtn = _getMTF(SFR_FIRST);

			Task.dFov[2] = Task.SFR.fImageAlign[2][0];
			sLangChange.LoadStringA(IDS_STRING458);	//CDOE %d, [DFOV %.03f] [Z %.03f]
			logStr.Format(sLangChange, 0, Task.dFov[0], motor.GetCommandPos((Motor_Lens_Z)));
			putListLog(logStr);

			if (iRtn>0)
				iRtnFunction = 54000;
			else
			{
				sLangChange.LoadStringA(IDS_STRING206);
				errMsg2(Task.AutoFlag, sLangChange);
				iRtnFunction = -53200;
			}
		}
		break;

	case 54000:		/* CODE2 °ª ÀÔ·Â ÈÄ sfr ÃøÁ¤ */
		{
			MIU.MoveVCM_Code(0);

			double dMovePos = model.axis[Motor_Lens_Z].pos[Bonding_Pos];// + model.dFov;

			motor.MoveAxis(Motor_Lens_Z, ABS, dMovePos, sysData.fMotorSpeed[Motor_Lens_Z], sysData.fMotorAccTime[Motor_Lens_Z]);

			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 54100;
		}
		break;


	case 54100:
		if ( motor.IsStopAxis(Motor_Lens_Z) )//&& motor.GetInposition(Motor_Lens_Z) )
		{
			double dCurPos = motor.GetCommandPos(Motor_Lens_Z);
			double dMovePos = model.axis[Motor_Lens_Z].pos[Bonding_Pos];// + model.dFov;

			if (fabs(dCurPos-dMovePos)<=0.003)
			{
				Task.PCBTaskTime = myTimer(true);
				iRtnFunction =  54200;
			}
		}
		
		if(myTimer(true)-Task.PCBTaskTime > 5000)
		{	
			sLangChange.LoadStringA(IDS_STRING868);	//OIS AF ñé LENS Z Motor offset ÀÌµ¿ ½Ã°£ ÃÊ°ú.
			errMsg2(Task.AutoFlag, sLangChange);
			iRtnFunction = -54100;
			break;
		}
		break;

	case 54200:
		{
			if( (myTimer(true) - Task.PCBTaskTime) > model.strInfo_Cam[CCD].m_iDelayTime)
				iRtnFunction = 54300;
			else
				iRtnFunction = 54200;
		}
		break;

	case 54300:
		{
			iRtn = _getMTF(SFR_FIRST);

			Task.dFov[3] = Task.SFR.fImageAlign[3][0];
			sLangChange.LoadStringA(IDS_STRING459);	//CDOE 0 (Offset ÀÌµ¿), [DFOV %.03f] [Z %.03f]
			logStr.Format(sLangChange, Task.dFov[3], motor.GetCommandPos(Motor_Lens_Z));
			putListLog(logStr);

			if (iRtn>0)
				iRtnFunction = 55000;
			else
			{
				sLangChange.LoadStringA(IDS_STRING208);
				errMsg2(Task.AutoFlag, sLangChange);
				iRtnFunction = -54300;
			}
		}
		break;

	case 55000:		/* 50 §­ CODE °ª °è»ê */
		{
			Task.m_iCnt_Step_AA_Total = 0;
			Task.codeResult = caclCodeVal(Task.dFov);

			MIU.MoveVCM_Code(Task.codeResult);

			double dMovePos = model.axis[Motor_Lens_Z].pos[Bonding_Pos];

			motor.MoveAxis(Motor_Lens_Z, ABS, dMovePos, sysData.fMotorSpeed[Motor_Lens_Z], sysData.fMotorAccTime[Motor_Lens_Z]);

			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 55100;
		}
		break;


	case 55100:
		if ( motor.IsStopAxis(Motor_Lens_Z))// && motor.GetInposition(Motor_Lens_Z) )
		{
			double dCurPos = motor.GetCommandPos(Motor_Lens_Z);
			double dMovePos = model.axis[Motor_Lens_Z].pos[Bonding_Pos];

			if (fabs(dCurPos-dMovePos)<=0.003)
			{
				Task.PCBTaskTime = myTimer(true);
				iRtnFunction =  55200;
			}
		}

		if(myTimer(true)-Task.PCBTaskTime > 5000)
		{
			sLangChange.LoadStringA(IDS_STRING207);
			errMsg2(Task.AutoFlag, sLangChange);
			iRtnFunction = -55100;
			break;
		}
		break;

	case 55200:
		{
			if( (myTimer(true) - Task.PCBTaskTime) > model.strInfo_Cam[CCD].m_iDelayTime)
				iRtnFunction = 55300;
			else
				iRtnFunction = 56000;
		}
		break;


	case 56000:
		{
			iRtnFunction = 60900;
		}

		break;
	}

	return iRtnFunction;
}


int	CAABonderDlg::caclCodeVal(double dFov[5])
{
	int code1 = model.code1;
	int code2 = model.code2;

	int rtnVal = 0;

	double calcA = (dFov[2]-dFov[1]) / (code2-code1);
	double calcB = dFov[1] - (calcA * code1);

	rtnVal = (int)((dFov[3] - calcB) / calcA + 0.5);

	return rtnVal;
}


bool CAABonderDlg::MoveOffset_Z()
{
	short axis[6] = {Motor_Lens_Z, };
	double dDes_Pos[6] = {0.0, };

	dDes_Pos[0] = motor.GetEncoderPos(Motor_Lens_Z) + sysData.m_dOffset_Prev_UV_Z;

	motor.goMotorPos(1, axis, dDes_Pos, true);

	double iTime = myTimer(true);

	while (1)
	{
		if (myTimer(true) - iTime < MOTOR_MOVE_TIME)
		{
			if (motor.IsStopAxis(Motor_Lens_Z) == true)
				break;
		}
		else{
			return false;
		}
	}

	return true;
}

int CAABonderDlg::TiltAlignLimitCheck(double dOffsetX, double dOffsetY)										// [Insptype] 0:Lens PreAlign 1:PCB PreAlign
{								
													// Return	0:NG,  1:Retry,  2:OK
	if (Task.PCBTask == 27250  || Task.PCBTask == 13100)
	{
		if (fabs(dOffsetX)  > model.axis[Motor_PCB_Xt].m_dLimit_Err || fabs(dOffsetY)  > model.axis[Motor_PCB_Yt].m_dLimit_Err) {
			return 0;
		}
		if (fabs(dOffsetX)  > model.axis[Motor_PCB_Xt].m_dLimit_OK || fabs(dOffsetY)  > model.axis[Motor_PCB_Yt].m_dLimit_OK) {
			return 1;
		}
	}
	else
	{
		if (fabs(dOffsetX)  > model.axis[Motor_Lens_Xt].m_dLimit_Err || fabs(dOffsetY)  > model.axis[Motor_Lens_Yt].m_dLimit_Err) {
			return 0;
		}
		if (fabs(dOffsetX)  > model.axis[Motor_Lens_Xt].m_dLimit_OK || fabs(dOffsetY)  > model.axis[Motor_Lens_Yt].m_dLimit_OK) {
			return 1;
		}
	}
	
	return 2; 
}

int CAABonderDlg::Test(int iStep)
{
	CString sLog;
	int iRtnFunction = iStep;
	CString logStr;

	switch(iStep)
	{
	case  160000:

		if(motor.LENS_Z_Motor_Move(Bonding_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING703);	//Lens Z Bonding À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]
			logStr.Format(sLangChange, iStep);
			putListLog(logStr);
			iRtnFunction = 160100;

		}
		else
		{	
			sLangChange.LoadStringA(IDS_STRING702);	//Lens Z Bonding À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]
			logStr.Format(sLangChange, iStep);
			putListLog(logStr);
			iRtnFunction = -160000;
		}
		break;

	case  160100:
		{
			if(Dio.LensMotorGrip(false,false))
			{		
				sLangChange.LoadStringA(IDS_STRING671);	//Lens Grip ÈÄÁø [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
				iRtnFunction = 160200;
			}
			else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
			{
				sLangChange.LoadStringA(IDS_STRING674);	//Lens Grip ÈÄÁø ½ÇÆÐ [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = -160100;
			}
		}

		break;
	case  160200:
		{
			if(Dio.LensMotorGripCheck(false,false))
			{
				sLangChange.LoadStringA(IDS_STRING675);	//Lens Grip ÈÄÁø È®ÀÎ ¿Ï·á [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 160400;
			}
			else
			{
				Dio.LensMotorGrip(false,false);
				sLangChange.LoadStringA(IDS_STRING674);	//Lens Grip ÈÄÁø È®ÀÎ ½ÇÆÐ [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
				iRtnFunction = 160300;
			}
		}

		break;
	case  160300:
		{
			if(Dio.LensMotorGripCheck(false,false))
			{
				sLangChange.LoadStringA(IDS_STRING675);	//Lens Grip ÈÄÁø È®ÀÎ ¿Ï·á [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 160400;
			}
			else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
			{
				sLangChange.LoadStringA(IDS_STRING674);	//Lens Grip ÈÄÁø ½ÇÆÐ [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = -160100;
			}
		}

		break;

	case  160400:
		{
			if(motor.LENS_Z_Motor_Move(Wait_Pos))
			{
				sLangChange.LoadStringA(IDS_STRING705);	//Lens Z ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 160500;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING704);	//Lens Z ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = -160400;
			}
		}

		break;
	case  160500:
		{
			if(motor.Pcb_Motor_Move(Defect_Pos))
			{
				sLangChange.LoadStringA(IDS_STRING934);	//PCB Defect À§Ä¡ ÀÌµ¿ ¿Ï·á [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 160600;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING933);	//PCB Defect À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = -160500;
			}
		}

		break;
	case  160600:
		{
			Sleep(2000);
			sLangChange.LoadStringA(IDS_STRING1352);	//ÀÌ¹° °Ë»ç ¿Ï·á [%d]
			logStr.Format(sLangChange, iStep);
			putListLog(logStr);
			iRtnFunction = 160700;
		}

		break;
	case  160700:
		{
			if(motor.Pcb_Motor_Move(Dark_Pos))
			{
				sLangChange.LoadStringA(IDS_STRING927);	//PCB Dark À§Ä¡ ÀÌµ¿ ¿Ï·á [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 160800;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING926);	//PCB Dark À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = -160700;
			}
		}

		break;
	case  160800:
		{
			if(Dio.PCBDark(false,false))
			{
				sLangChange.LoadStringA(IDS_STRING928);	//PCB Dark ÇÏ°­ [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 160900;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING930);	//PCB Dark ÇÏ°­ ½ÇÆÐ [%d]
				logStr.Format(sLangChange, iStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -160800;
			}
		}

		break;

	case  160900:
		{
			if(Dio.PCBDarkCheck(false,false))
			{
				Sleep(2000);
				sLangChange.LoadStringA(IDS_STRING919);	//PCB Dark °Ë»ç ¿Ï·á [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 161000;
			}

		}

		break;

	case  161000:
		{
			if(Dio.PCBDark(true,false))
			{
				sLangChange.LoadStringA(IDS_STRING920);	//PCB Dark »ó½Â [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
				iRtnFunction = 161100;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING922);	//PCB Dark »ó½Â ½ÇÆÐ [%d]
				logStr.Format(sLangChange, iStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -161000;
			}
		}

		break;

	case  161100:
		{
			if(Dio.PCBDarkCheck(true,false))
			{
				sLangChange.LoadStringA(IDS_STRING925);	//PCB Dark »ó½Â È®ÀÎ ¿Ï·á [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				
				iRtnFunction = 161200;
			}
			else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
			{
				sLangChange.LoadStringA(IDS_STRING924);	//PCB Dark »ó½Â È®ÀÎ ½ÇÆÐ [%d]
				logStr.Format(sLangChange, iStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -161100;
			}
		}

		break;

	case  161200:
		{
			if(motor.Pcb_Motor_Move(Wait_Pos))
			{
				sLangChange.LoadStringA(IDS_STRING987);	//PCB ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 161300;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING986);	//PCB ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]
				logStr.Format(sLangChange, iStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -161200;
			}
		}

		break;
	}

	return iRtnFunction;
}

void CAABonderDlg::OnBnClickedButtonDispenseResult()
{
	CString logStr="";
	if(Task.AutoFlag==1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//ÀÚµ¿ ¿îÀü Áß ÀÔ´Ï´Ù.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
	}
	else if(Task.AutoFlag==0 || Task.AutoFlag==2)
	{	
		logStr.Format("µµÆ÷ ¿Ï·á ¿©ºÎ ¼±ÅÃÇÏ¼¼¿ä! \n¿Ï·á = ¿¹(Y) / ¹Ì¿Ï·á = ¾Æ´Ï¿À(N)");	//µµÆ÷ ¿Ï·á ¿©ºÎ¸¦ ÀÔ·Â ÇÏ¼¼¿ä. \n[¿Ï·á ¼±ÅÃ Yes / ¹Ì¿Ï·á ¼±ÅÃ No]
		if(askMsg(logStr)){
			Task.m_bOkDispense = 1;
		}else{
			Task.m_bOkDispense = -1;
		}
	}
}

bool CAABonderDlg::func_MIU_ConnectLiveCheck()
{
	if( gMIUDevice.CurrentState != 4)
	{	
		sLangChange.LoadStringA(IDS_STRING1370);	//ÀÚµ¿ ¿îÀü Áß CCD µ¿¿µ»ó ¿µ»ó È¹µæ ½ÇÆÐ
		errMsg2(Task.AutoFlag, sLangChange);
		return false;
	}
	return true;
}

bool CAABonderDlg::func_Check_LaserValueErr(double dVal[4])
{
	//20150617
	//for(int iNo = 1; iNo < 4; iNo++)
	//{
	//	if( dVal[0] != dVal[1])	 return true;	//ÇÏ³ª¶óµµ ´Ù¸£¸é OK
	//}

	//return false;
	return true;
}

void CAABonderDlg::showLanConnect()
{
}

bool CAABonderDlg::ConnectToServer()
{
	CString sLog;
#ifdef		ON_LINE_LAN
	if ( m_Socket != NULL || m_SocketMes != NULL)
	{
		if(m_Socket != NULL)
		{
			m_Socket.ShutDown();
			m_Socket.Close();
		}
		else
		{
			m_SocketMes.ShutDown();
			m_SocketMes.Close();
		}
	}

	if( !m_Socket.Create() || !m_SocketMes.Create())
	{
		sLangChange.LoadStringA(IDS_STRING1085);	//Socket »ý¼º ½ÇÆÐ
		sLog.Format(sLangChange);
		putListLog(sLog);
		return false;
	}

	//SetTimer(10, 5000, NULL);

	CString sIPAdd;
	sIPAdd.Format("%s", SOCKET_MAIN);
	
	if( m_Socket.Connect(sIPAdd, 21000) == FALSE )
	{
		sLangChange.LoadStringA(IDS_STRING1276);	//¼­¹ö¿Í ¿¬°á ½ÇÆÐ
		sLog.Format(sLangChange);
		putListLog(sLog);
		//KillTimer(10);
		return false;
	}
	//else
	//{
	//	KillTimer(10);
	//}
#endif

	return true;
}


bool CAABonderDlg::CheckMessage(CString sMsg)
{
	// 	int iIndex = atoi(sMsg.Right(1));
	// 	if ( iIndex != 1)		return false;
	int startIndex, endIndex;
	CString sTemp, logStr, sSendData ;//&%s$
	int stxIndex = sMsg.Find("&", 0);
	int etxIndex = sMsg.Find("%", 0);
	sTemp		= sMsg.Mid(stxIndex+1, etxIndex - stxIndex -1);	
	
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if(stxIndex<0 || etxIndex<0 || stxIndex>etxIndex)
	{
		sLangChange.LoadStringA(IDS_STRING1337);	//ÀÌ´õ³Ý Data ¼ö½Å ÀÌ»ó
		sTemp.Format(sLangChange + _T(" - %s\n"), sMsg);
		errMsg2(Task.AutoFlag, sTemp);
		return false;
	}

	startIndex	= sMsg.Find("$", 0);
	endIndex	= sMsg.Find("_", startIndex+1);
	sTemp		= sMsg.Mid(startIndex+1, endIndex - startIndex -1);
	
	//sMsg.Format("[¼­¹ö -> Å¬¶óÀÌ¾ðÆ®] %s", sTemp);
	//putListLog(sMsg);

	if( lstrcmp(sTemp, "GETTILT") == 0 )
	{//GETTILT_¹ÙÄÚµå¸íÄª
		startIndex	= sMsg.Find("_", 0);
		endIndex	= sMsg.Find("%", startIndex+1);
		sTemp		= sMsg.Mid(startIndex+1, endIndex - startIndex -1);
		
		if( !g_ADOData.func_Get_RecordData(sTemp) )	//¹ÙÄÚµå ¸íÀ¸·Î DB °Ë»ö
		{
			sSendData = ("NAK");
			SendMessageToServer(sSendData);
		}
		else
		{
			sSendData = ("ACK");
			SendMessageToServer(sSendData);
		}
	}
	else if( lstrcmp(sTemp, "GETDATA") == 0)
	{
		startIndex	= sMsg.Find("_", 0);
		endIndex	= sMsg.Find("%", startIndex+1);
		sTemp		= sMsg.Mid(startIndex+1, endIndex - startIndex -1);

		if( g_ADOData.func_Get_RecordData(sTemp) == true )	//¹ÙÄÚµå ¸íÀ¸·Î DB °Ë»ö
		{
			sSendData.Format("ALIGNX_%.04lf", g_ADOData.adoRegist.dOffset_Align[0]);
			SendMessageToServer(sSendData);
			Sleep(200);
			sSendData.Format("ALIGNY_%.04lf", g_ADOData.adoRegist.dOffset_Align[1]);
			SendMessageToServer(sSendData);
			Sleep(200);
			sSendData.Format("ALIGNT_%.04lf", g_ADOData.adoRegist.dOffset_Align[2]);
			SendMessageToServer(sSendData);
			Sleep(200);
			sSendData.Format("TILTX_%.04lf", g_ADOData.adoRegist.dOffset_TiltTx);
			SendMessageToServer(sSendData);
			Sleep(200);
			sSendData.Format("TILTY_%.04lf", g_ADOData.adoRegist.dOffset_TiltTy);
			SendMessageToServer(sSendData);
		}
		else
		{
			sSendData = ("NAK");
			SendMessageToServer(sSendData);
		}
	}
	else if( lstrcmp(sTemp, "NAK") == 0 )
	{
		sSendData = ("NAK");
		SendMessageToServer(sSendData);
	}
	return true;
}


bool CAABonderDlg::SendMessageToServer(CString sMsg)
{
	CString sLog;
#ifndef		ON_LINE_LAN
	return true;
#endif

	if(!m_bisConnect)
	{
		sLangChange.LoadStringA(IDS_STRING562);
		delayMsg(sLangChange, 3000, M_COLOR_RED);	//Ether-Net ¿¬°á ÇØÁ¦ »óÅÂÀÔ´Ï´Ù
		return false;
	}

	CString sHead;
	sHead.Format("&AA%d$", sysData.m_iUnitNo);
	CString sEnd = "%";
	CString sSendMsg = sHead + sMsg +sEnd;

	if(sMsg.GetLength()==0)
	{
		return false;
	}

	int iRtn;
	int retry_cnt = 5;

	m_csLock_Socket.Lock();
	sLangChange.LoadStringA(IDS_STRING212);
	sLog.Format(sLangChange, sMsg);
	putListLog(sLog);

	Sleep(10);
	for(int i=0; i<retry_cnt; i++)
	{
		iRtn = m_Socket.Send(sSendMsg, sSendMsg.GetLength() * 2);
		SocketDataSave(SEND_DATA, sMsg);

		if ( iRtn >= 0 )
			break;

		for(int j=0; j<100; j++)
		{
			Sleep(1);
			checkMessage();
		}
	}

	m_csLock_Socket.Unlock();

	if ( iRtn < 0 )
	{
		CString sLog;
		sLangChange.LoadStringA(IDS_STRING1275);	//¼­¹ö·Î ¸Þ½ÃÁö Àü¼Û ½ÇÆÐ.
		sLog.Format(sLangChange + _T(" [%s]"), sSendMsg);
		delayMsg(sLog.GetBuffer(99), 3000, M_COLOR_RED);
		return false;
	}

	return true;
}

void CAABonderDlg::func_SocketControl_ConnectCheck()
{
}


void CAABonderDlg::CreateServer()
{/* ¼­¹ö »ý¼º */
	CString sLog;

	if ( m_ListenSocket.Create(21000, SOCK_STREAM) == TRUE )
	{
		if ( m_ListenSocket.Listen() == TRUE )
		{
			sLangChange.LoadStringA(IDS_STRING1273);	//¼­¹ö »ý¼º ÈÄ Á¢¼Ó ´ë±â Áß...
			putListLog(sLangChange);
		}
		else
		{		
			sLangChange.LoadStringA(IDS_STRING1274);	//¼­¹ö Á¢¼Ó ´ë±â ½ÇÆÐ
			putListLog(sLangChange);
		}
	}
	else
	{
		sLangChange.LoadStringA(IDS_STRING1272);	//¼­¹ö »ý¼º ½ÇÆÐ
		putListLog(sLangChange);
	}
}

void CAABonderDlg::CloseServer()
{/* ¼­¹ö ´Ý±â */
	POSITION pos;
	pos = m_ListenSocket.m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	while(pos != NULL)
	{
		pClient = (CClientSocket*)m_ListenSocket.m_ptrClientSocketList.GetNext(pos);

		if(pClient != NULL)
		{
			pClient->ShutDown();
			pClient->Close();

			delete pClient;
		}
	}

	m_ListenSocket.ShutDown();
	m_ListenSocket.Close();
}


void CAABonderDlg::CheckClientPosition(CSocket* pClient, CString sMsg)
{
	int iIndex = atoi(sMsg.Right(1)) - 1;

	if ( iIndex < 0 )
	{
		sLangChange.LoadStringA(IDS_STRING1379);		//Àß¸øµÈ Å¬¶óÀÌ¾ðÆ® Á¢¼ÓÀÔ´Ï´Ù.\n\n(IP ÁÖ¼Ò¸¦ È®ÀÎÇØ ÁÖ¼¼¿ä)
		putListLog(sLangChange);
		return;
	}

	m_pos[iIndex] = m_ListenSocket.m_ptrClientSocketList.Find(pClient);

	CString sTemp;
	sTemp.Format("AA PC #%d Connection", iIndex+1);
	putListLog(sTemp);


}


bool CAABonderDlg::SendMessageToClient(int iCh, CString sMsg)
{
	//m_csSendMsg.Lock();

	CString sTemp = "&INSP$" + sMsg + "%";

	if(m_pos[iCh] == NULL) return false;

	if ( m_ListenSocket.SendData(m_pos[iCh], sTemp) == false )
	{
		sTempLang.LoadStringA(IDS_STRING169);
		sLangChange.Format(sTempLang, iCh+1, sTemp);
		sTemp.Format(sLangChange);
		putListLog(sTemp);
		//m_csSendMsg.Unlock();
		return false;
	}
	sLangChange.LoadStringA(IDS_STRING213);
	sTemp.Format(sLangChange, iCh+1, sMsg);
	putListLog(sTemp);

	//m_csSendMsg.Unlock();
	return true;
}

void CAABonderDlg::ServerCheckMessage(CSocket* pClient, CString sMsg)
{
	m_csRcvMsg.Lock();

	CString sTemp, sCompair;
	int iIndex = -1;
	for (int i=0 ; i<2 ; i++)
	{
		if ( m_pos[i] == m_ListenSocket.m_ptrClientSocketList.Find(pClient) )
		{
			iIndex = i;
			break;
		}
	}

	if(iIndex== -1)
	{
		sLangChange.LoadStringA(IDS_STRING1251);	//¹ÞÀº ¸Þ¼¼Áö ÀÌ»ó.
		putListLog(sLangChange);
		m_csRcvMsg.Unlock();
		return;
	}

	int iCheckFirst = sMsg.Find('&', 0);
	int iCheckMiddle = sMsg.Find('$', 0);
	int iCheckLast = sMsg.Find('%', 0);

	if ( iCheckFirst < 0 || iCheckMiddle < 0 || iCheckLast < 0)
	{
		sLangChange.LoadStringA(IDS_STRING1251);	//¹ÞÀº ¸Þ¼¼Áö ÀÌ»ó.
		putListLog(sLangChange);
		m_csRcvMsg.Unlock();
		return;
	}

	int startIndex, endIndex;
	CString logStr, sSendData ;//&%s$
	int stxIndex = sMsg.Find("&", 0);
	int etxIndex = sMsg.Find("%", 0);
	sTemp		= sMsg.Mid(stxIndex+1, etxIndex - stxIndex -1);	
	
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if(stxIndex<0 || etxIndex<0 || stxIndex>etxIndex)
	{
		sLangChange.LoadStringA(IDS_STRING1337);	//ÀÌ´õ³Ý Data ¼ö½Å ÀÌ»ó
		sTemp.Format(sLangChange + _T(" - %s\n"), sMsg);
		errMsg2(Task.AutoFlag, sTemp);
		return;
	}

	startIndex	= sMsg.Find("$", 0);
	endIndex	= sMsg.Find("%", startIndex+1);
	sTemp		= sMsg.Mid(startIndex+1, endIndex - startIndex -1);
	
	startIndex	= sMsg.Find("$", 0);
	endIndex	= sMsg.Find("_", startIndex+1);
	sCompair	= sMsg.Mid(startIndex+1, endIndex - startIndex -1);

	if ( lstrcmp(sCompair, "TILTX") == 0 )
	{//TILTX_0.0245
		startIndex	= sMsg.Find("_", 0);
		endIndex	= sMsg.Find("%", startIndex+1);
		sTemp		= sMsg.Mid(startIndex+1, endIndex - startIndex -1);

		Task.dTiltingManual[0] = atof(sTemp);
		Task.iRecvLenCnt[0] = 1;
	}
	else if( lstrcmp(sCompair, "TILTY") == 0 )
	{//TILTY_0.0245
		startIndex	= sMsg.Find("_", 0);
		endIndex	= sMsg.Find("%", startIndex+1);
		sTemp		= sMsg.Mid(startIndex+1, endIndex - startIndex -1);

		Task.dTiltingManual[1] = atof(sTemp);
		Task.iRecvLenCnt[1] = 1;
	}
	else if( lstrcmp(sCompair, "ALIGNX") == 0 )
	{//º¸Á¤·® X
		startIndex	= sMsg.Find("_", 0);
		endIndex	= sMsg.Find("%", startIndex+1);
		sTemp		= sMsg.Mid(startIndex+1, endIndex - startIndex -1);
		Task.dAlignManual[0] = atof(sTemp);
		Task.iRecvLenCnt[2] = 1;
	}
	else if( lstrcmp(sCompair, "ALIGNY") == 0 )
	{//º¸Á¤·® Y
		startIndex	= sMsg.Find("_", 0);
		endIndex	= sMsg.Find("%", startIndex+1);
		sTemp		= sMsg.Mid(startIndex+1, endIndex - startIndex -1);
		Task.dAlignManual[1] = atof(sTemp);
		Task.iRecvLenCnt[3] = 1;
	}
	else if( lstrcmp(sCompair, "ALIGNT") == 0 )
	{//º¸Á¤·® T
		startIndex	= sMsg.Find("_", 0);
		endIndex	= sMsg.Find("%", startIndex+1);
		sTemp		= sMsg.Mid(startIndex+1, endIndex - startIndex -1);
		Task.dAlignManual[2] = atof(sTemp);
		Task.iRecvLenCnt[4] = 1;
	}
	else if( lstrcmp(sTemp, "NAK") == 0 )
	{//!! ¸ðµ¨ÀÌ ¾øÀ» °æ¿ì
		Task.iRecvLenACK[iIndex] = 0;
	}
	else if( lstrcmp(sTemp, "ACK") == 0 )
	{//!! ¸ðµ¨ Á¤º¸ ÀÖÀ» °æ¿ì
		Task.iRecvLenACK[iIndex] = 1;
	}
	sLangChange.LoadStringA(IDS_STRING210);
	sMsg.Format(sLangChange, iIndex+1, sTemp);

	if (lstrcmp(m_sOldRcvMsg, sTemp) != 0)
	{
		putListLog(sMsg);
		m_sOldRcvMsg = sTemp;
	}

	m_csRcvMsg.Unlock();
}


void CAABonderDlg::func_Control_StateChange(int iCh)
{
	
}


void CAABonderDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	double dExpandFactorX, dExpandFactorY;

	if( ( lensDlg!=NULL && lensDlg->IsWindowVisible()) || 
		(lensEdgeDlg!=NULL && lensEdgeDlg->IsWindowVisible()) || 
		(pcbDlg!=NULL && pcbDlg->IsWindowVisible()) || 
//		(pcbInspDlg!=NULL && pcbInspDlg->IsWindowVisible()) || 
		(motorDlg!=NULL && motorDlg->m_bCalcResol) || 
		(motorDlg2!=NULL && motorDlg2->m_bCalcResol) || 
		(motorDlg3!=NULL && motorDlg3->m_bCalcResol))
	{
		if (point.x>m_rectCamDispPos1.left &&
			point.x<m_rectCamDispPos1.right &&
			point.y>m_rectCamDispPos1.top &&
			point.y<m_rectCamDispPos1.bottom)
		{
			if (	lensDlg->IsWindowVisible())				m_iCurCamNo = CAM1; 
#if (____AA_WAY == PCB_TILT_AA)
			else if (lensEdgeDlg->IsWindowVisible())		m_iCurCamNo = CAM1;
#elif (____AA_WAY == LENS_TILT_AA)
			else if (lensEdgeDlg->IsWindowVisible())		m_iCurCamNo = CAM1;
#endif
			else if (pcbDlg->IsWindowVisible())				m_iCurCamNo = CAM1;
			else if ( motorDlg->IsWindowVisible())			m_iCurCamNo = motorDlg->m_iSelCam;
			else if ( motorDlg2->IsWindowVisible())			m_iCurCamNo = motorDlg2->m_iSelCam;
			else if ( motorDlg3->IsWindowVisible())			m_iCurCamNo = motorDlg3->m_iSelCam;			

			int iGap;
			if (m_iCurCamNo < 3)
			{
				dExpandFactorX = CAM_EXPAND_FACTOR_X;
				dExpandFactorY = CAM_EXPAND_FACTOR_Y;
				iGap = 20;
			}
			else
			{
				dExpandFactorX = (double)gMIUDevice.nWidth/SMALL_CCD_SIZE_X;
				dExpandFactorY = (double)gMIUDevice.nHeight/SMALL_CCD_SIZE_Y;
				iGap = 200;
			}

			m_ClickP.x = point.x - m_rectCamDispPos1.left;
			m_ClickP.y = point.y - m_rectCamDispPos1.top;

			if ( m_bMeasureDist )
			{
				m_bDrawMeasureLine = true;
			}
			else
			{
				m_bDrawFlag = true;

				if (m_ClickP.x * dExpandFactorX>m_rBox.left-iGap	&&
					m_ClickP.y * dExpandFactorY>m_rBox.top-iGap		&&
					m_ClickP.x * dExpandFactorX<m_rBox.right+iGap	&&
					m_ClickP.y * dExpandFactorY<m_rBox.bottom+iGap)
				{
					m_bBoxMoveFlag = true;
				}

				m_iMoveType = checkMousePos(point, m_rBox);
			}
		}
	}
	//else if (ccdDlg->m_pDefectDlg!=NULL && ccdDlg->m_pDefectDlg->IsWindowVisible() )
	//{
	//	m_iCurCamNo = 3;

	//	if (point.x>m_rectCamDispPos1.left &&
	//		point.x<m_rectCamDispPos1.right &&
	//		point.y>m_rectCamDispPos1.top &&
	//		point.y<m_rectCamDispPos1.bottom)
	//	{
	//		m_bDrawFlag = true;
	//		
	//		m_ClickP.x = point.x - m_rectCamDispPos1.left;
	//		m_ClickP.y = point.y - m_rectCamDispPos1.top;

	//		if ( m_bBox_CCD_Zoom == true )
	//		{
	//			if ( m_bState_CCD_Zoom == false )
	//			{
	//				//! ÇöÀç È­¸éÀÌ ÀüÃ¼ º¸±â »óÅÂÀÏ ¶§
	//				//! È®´ëÇÒ »ç°¢ ¿µ¿ª ¼³Á¤ ½ÃÀÛ

	//				int iGap;

	//				dExpandFactorX = (double)gMIUDevice.nWidth/SMALL_CAM_SIZE_X;
	//				dExpandFactorY = (double)gMIUDevice.nHeight/SMALL_CAM_SIZE_Y;
	//				iGap = 200;

	//				if (m_ClickP.x * dExpandFactorX>m_rBox.left-iGap	&&
	//					m_ClickP.y * dExpandFactorY>m_rBox.top-iGap		&&
	//					m_ClickP.x * dExpandFactorX<m_rBox.right+iGap	&&
	//					m_ClickP.y * dExpandFactorY<m_rBox.bottom+iGap)
	//				{
	//					m_bBoxMoveFlag = true;							
	//				}

	//				m_bBox_Acting_CCD_Zoom = true;

	//				m_iMoveType = checkMousePos(point, m_rBox);
	//			}
	//			else
	//			{
	//				if ( m_bPan_CCD_Zoom == true )
	//				{
	//					//! È®´ëµÈ »óÅÂ¿¡¼­ Mouse·Î È­¸é ÀÌµ¿ ½ÃÀÛ
	//					m_PanMoveP = m_ClickP;
	//					m_bActing_Pan_CCD_Zoom = true;
	//				}
	//			}							
	//		}
	//		else
	//		{
	//			//! Å¬¸¯ÇÑ ÁöÁ¡ÀÇ ¹à±â°ª Ç¥½Ã

	//			CPoint p, pShow;
	//			int pos, width;
	//			char szTmp[256];

	//			vision.clearOverlay(m_iCurCamNo);

	//			vision.MilBufferUpdate();

	//			width = MbufInquire(vision.MilGrabImageChild[4], M_PITCH, M_NULL);

	//			//! È®´ëµÈ »óÅÂ¿¡¼­µµ Å¬¸¯ÇÑ ÁöÁ¡¿¡ Á¤È®ÇÏ°Ô ½ÊÀÚ¼±À» ±×¸°´Ù. 
	//			if ( m_bState_CCD_Zoom == true )
	//			{
	//				p.x = m_ViewPos.x + m_ClickP.x;
	//				p.y = m_ViewPos.y + m_ClickP.y;
	//				pShow.x = m_ClickP.x * gMIUDevice.nWidth / SMALL_CAM_SIZE_X;
	//				pShow.y = m_ClickP.y * gMIUDevice.nHeight / SMALL_CAM_SIZE_Y;
	//			}
	//			else
	//			{
	//				p.x = m_ClickP.x * gMIUDevice.nWidth / SMALL_CAM_SIZE_X;
	//				p.y = m_ClickP.y * gMIUDevice.nHeight / SMALL_CAM_SIZE_Y;
	//				pShow.x = p.x;
	//				pShow.y = p.y;
	//			}

	//			vision.crosslist[m_iCurCamNo].addList(pShow, 150, M_COLOR_RED);

	//			pos = p.y * width + p.x;

	//			sprintf_s(szTmp, "(%d, %d) ==> RGB %d, %d, %d", p.x, p.y, vision.Image[3][pos], vision.Image[4][pos], vision.Image[5][pos]);
	//			vision.textlist[m_iCurCamNo].addList(50, 680, szTmp, M_COLOR_RED, 17, 7, "Arial");
	//			vision.drawOverlay(m_iCurCamNo, true);
	//		}			
	//	} 

	//	CDialogEx::OnLButtonDown(nFlags, point);
	//	return;
	//}
	//else if (ccdDlg->m_pOSChkDlg!=NULL && ccdDlg->m_pOSChkDlg->IsWindowVisible() || (ccdDlg->m_pSFRDlg!=NULL && ccdDlg->m_pSFRDlg->IsWindowVisible() ) )
	else if ((ccdDlg->m_pSFRDlg!=NULL && ccdDlg->m_pSFRDlg->IsWindowVisible() ) )
	
	{
		m_iCurCamNo = 3;

		/*if (point.x>m_rectCamDispPos1.left &&
			point.x<m_rectCamDispPos1.right &&
			point.y>m_rectCamDispPos1.top &&
			point.y<m_rectCamDispPos1.bottom)*/
			if (point.x>m_rectCcdDispPos.left &&
				point.x<m_rectCcdDispPos.right &&
				point.y>m_rectCcdDispPos.top &&
				point.y<m_rectCcdDispPos.bottom)
		{
			int iGap;

			dExpandFactorX = (double)gMIUDevice.nWidth/SMALL_CCD_SIZE_X;
			dExpandFactorY = (double)gMIUDevice.nHeight/SMALL_CCD_SIZE_Y;
			iGap = 200;

			m_ClickP.x = point.x - m_rectCcdDispPos.left;// m_rectCamDispPos1.left;
			m_ClickP.y = point.y - m_rectCcdDispPos.top;// m_rectCamDispPos1.top;

            if (vision.m_FovSetMode == true)
            {
                ccdDlg->m_pSFRDlg->m_nSelectIndexFOV = ccdDlg->m_pSFRDlg->GetSelectedFOVNo(point);
                ccdDlg->m_pSFRDlg->drawRectFOV(ccdDlg->m_pSFRDlg->m_nSelectIndexFOV);//DrawRectFov(m_nSelectIndexCCD);
                return;
            }
            else if (vision.m_SnrSetMode == true)
            {
                ccdDlg->m_pSFRDlg->m_nSelectIndexSNR = ccdDlg->m_pSFRDlg->GetSelectedSNRNo(point);
                ccdDlg->m_pSFRDlg->drawRectSNR(ccdDlg->m_pSFRDlg->m_nSelectIndexSNR);//   drawRectSnr(m_nSelectIndexCCD);
                return;
            }

			m_iNo_SFR = ccdDlg->m_pSFRDlg->checkNoSFR(point);


			//if (m_iNo_SFR>=0 && m_iNo_SFR < LAST_MARK_CNT)
			if (m_iNo_SFR >= 0 && m_iNo_SFR < model.mGlobalChartCount)
			{
				m_bBoxMoveFlag_CCD = true;

				m_rBox.left		= ccdDlg->m_pSFRDlg->m_iOffsetX_SFR[m_iNo_SFR];
				m_rBox.top		= ccdDlg->m_pSFRDlg->m_iOffsetY_SFR[m_iNo_SFR];
				m_rBox.right	= m_rBox.left + ccdDlg->m_pSFRDlg->m_iSizeX_SFR[m_iNo_SFR];
				m_rBox.bottom	= m_rBox.top + ccdDlg->m_pSFRDlg->m_iSizeY_SFR[m_iNo_SFR];

				m_iMoveType = checkMousePos(point, m_rBox);
			}
			//else if (m_iNo_SFR>=LAST_MARK_CNT && m_iNo_SFR<LAST_MARK_CNT+4)
			else if (m_iNo_SFR >= model.mGlobalChartCount && m_iNo_SFR<model.mGlobalChartCount + 4)
			{
				m_bBoxMoveFlag_CCD = true;

				m_rBox	= ccdDlg->m_pSFRDlg->m_rcRoiBox[m_iNo_SFR- model.mGlobalChartCount];//LAST_MARK_CNT];

				m_iMoveType = checkMousePos(point, m_rBox);
			}
		}
	}


	if(m_iCurCamNo<0 || m_iCurCamNo>3)
	{
		putListLog("¸¶¿ì½º Å¬¸¯ - Ä«¸Þ¶ó ¹øÈ£ ÀÌ»ó");
		return;
	}


	/*if (point.x>m_rectCamDispPos1.left &&
		point.x<m_rectCamDispPos1.right &&
		point.y>m_rectCamDispPos1.top &&
		point.y<m_rectCamDispPos1.bottom	&& !m_bMeasureDist && m_bBoxMoveFlag_CCD == false )
	{*/
	if (point.x>m_rectCcdDispPos.left &&
		point.x<m_rectCcdDispPos.right &&
		point.y>m_rectCcdDispPos.top &&
		point.y<m_rectCcdDispPos.bottom && !m_bMeasureDist && m_bBoxMoveFlag_CCD == false)
	{
		CPoint p;
		int pos = 0;
		int width = 0;
		int val = 0;
		char szTmp[256];

		vision.clearOverlay(m_iCurCamNo);

		if(m_iCurCamNo<3) 
		{
			p.x = (int)(m_ClickP.x * CAM_EXPAND_FACTOR_X + 0.5);
			p.y = (int)(m_ClickP.y * CAM_EXPAND_FACTOR_Y + 0.5);
			vision.crosslist[m_iCurCamNo].addList(p, 30, M_COLOR_RED);
			MbufCopy(vision.MilGrabImageChild[m_iCurCamNo], vision.MilProcImageChild[m_iCurCamNo]);
			width = MbufInquire(vision.MilProcImageChild[m_iCurCamNo], M_PITCH, M_NULL);
			pos = p.y * width + p.x;
			val = vision.MilImageBuffer[m_iCurCamNo][pos];

			sprintf_s(szTmp, "(%d, %d) %d", p.x, p.y, val);
			vision.textlist[m_iCurCamNo].addList(50, 700, szTmp, M_COLOR_RED, 17, 7, "Arial");

			if( Task.bManual_FindEpoxy == true)
			{
				Task.cpMEpoxyPos.x = p.x;
				Task.cpMEpoxyPos.y = p.y;
			}
		}
		else 
		{
			vision.MilBufferUpdate();

			width = MbufInquire(vision.MilGrabImageChild[4], M_PITCH, M_NULL);

			p.x = m_ClickP.x * gMIUDevice.nWidth / SMALL_CCD_SIZE_X;
			p.y = m_ClickP.y * gMIUDevice.nHeight / SMALL_CCD_SIZE_Y;

			vision.crosslist[m_iCurCamNo].addList(p, 150, M_COLOR_RED);
			 
			pos = p.y * width + p.x;

			sprintf_s(szTmp, "(%d, %d) ==> RGB %d, %d, %d", p.x, p.y, vision.MilImageBuffer[3][pos], vision.MilImageBuffer[4][pos], vision.MilImageBuffer[5][pos]);
			vision.textlist[m_iCurCamNo].addList(50, 680, szTmp, M_COLOR_RED, 17, 7, "Arial");
		}


        if (ccdDlg->m_pSFRDlg != NULL && ccdDlg->m_pSFRDlg->IsWindowVisible())
        {
            if (vision.m_FovSetMode == true)
            {
                ccdDlg->m_pSFRDlg->drawRectFOV(m_iNo_SFR);
            }
            else if (vision.m_FovSetMode == true)
            {
                ccdDlg->m_pSFRDlg->drawRectSNR(m_iNo_SFR);
            }
            else
            {
                ccdDlg->m_pSFRDlg->drawRectSFR(m_iNo_SFR);
            }
            
        }
		vision.drawOverlay(m_iCurCamNo, true);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CAABonderDlg::OnBnClickedButtonLensPassResult()
{
	CString logStr="";
	if(Task.AutoFlag==1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//ÀÚµ¿ ¿îÀü Áß ÀÔ´Ï´Ù.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
	}
	else if(Task.AutoFlag==0 || Task.AutoFlag==2)
	{	
		logStr.Format("LENS ³Ñ±è ¿Ï·á ¿©ºÎ¸¦ ¼±ÅÃÇÏ¼¼¿ä. Á¦Ç° À¯¹« È®ÀÎ ÇÏ¼¼¿ä..\n ³Ñ±è ¿Ï·á ¼±ÅÃ = Yes / ³Ñ±è ¹Ì¿Ï·á ¼±ÅÃ = No");
		if( askMsg(logStr))
		{
			if(!Dio.LensMotorGripCheck(true, false) )//Lens °¨Áö°¡ ¾ÈµÇ¾úÀ» °æ¿ì  
			{
				sLangChange.Format(_T("Lens Grip ÀüÁø»óÅÂ°¡ ¾Æ´Õ´Ï´Ù."));
				delayMsg(sLangChange.GetBuffer(99), 3000, M_COLOR_DARK_GREEN);
			}else
			{
				Task.m_bOKLensPass = 1;
			}
		}else
		{
			if(Dio.LensMotorGripCheck(true, false) )//Lens °¨Áö°¡ µÇ¾úÀ» °æ¿ì  
			{
				sLangChange.Format(_T("Lens Grip ÈÄÁø»óÅÂ°¡ ¾Æ´Õ´Ï´Ù."));
				delayMsg(sLangChange.GetBuffer(99), 3000, M_COLOR_DARK_GREEN);
				
			}else
			{
				if(Dio.LensMotorGripCheck(false, false) )
				{
					Task.m_bOKLensPass = -1;
				}else
				{
					sLangChange.Format(_T("Lens Grip ÈÄÁø»óÅÂ°¡ ¾Æ´Õ´Ï´Ù."));
					delayMsg(sLangChange.GetBuffer(99), 3000, M_COLOR_DARK_GREEN);
				}
			}
			
		}
			
	}
}
// µµÆ÷ °Ë»ç 
bool CAABonderDlg::_EpoxyFinddispense(int cam)
{
	unsigned char *m_Imagebuf;
	m_Imagebuf = (unsigned char *)malloc(CAM_SIZE_X*CAM_SIZE_Y);
	Sleep(500);
	vision.m_csGrab.Lock();
	Sleep(100);
	//MimFlip(vision.MilGrabImageChild[CAM1], vision.MilGrabImageChild[CAM1], M_HORIZONTAL, M_DEFAULT);
	MbufGet(vision.MilGrabImageChild[CAM1], m_Imagebuf);	// ÀÌ¹ÌÁö ÀüÃ¼ ÇÈ¼¿ ¹à±â°ª ¹öÆÛ ´ã±â

	vision.m_csGrab.Unlock();
	
	bool bRtn = true;

	bool bRtn1 = true;
	bool bRtn2 = true;
	bool bRtn3 = true;
	bool bRtn4 = true;
	//
	bool bRtn5 = true;
	bool bRtn6 = true;
	bool bRtn7 = true;
	bool bRtn8 = true;

	bRtn1 = _inspResignRect(false, CAM1, 0, 0, m_Imagebuf);		// left (¼¼·Î)
	bRtn2 = _inspResignRect(false, CAM1, 0, 1, m_Imagebuf);	// right (¼¼·Î)
	bRtn3 = _inspResignRect(false, CAM1, 1, 2, m_Imagebuf);	// top (°¡·Î)
	bRtn4 = _inspResignRect(false, CAM1, 1, 3, m_Imagebuf);	// bottom (°¡·Î)
	//bRtn = (bRtn1 && bRtn2 && bRtn3 && bRtn4);

	//bRtn1 = _inspResignHole(true, 0, 0);
	//bRtn = bRtn1;


	//
	free(m_Imagebuf);
	
	return bRtn;
}

bool CAABonderDlg::_inspResign(bool autoMode, int index, int dispMode)				// dispMode 0:ROI + °á°ú Ãâ·Â, 1:µµÆ÷ White ¿µ¿ª + Text Ãâ·Â, 2:µµÆ÷ White Edge Ãâ·Â
{
	int iCh = 0;
	if ( vision.getLiveMode() == 1 )
	{
		vision.getSnapImage(1);
	}


	if(Task.m_iStatus_Unit_Epoxy == 1)
	{
		saveInspImage(EPOXY_IMAGE_SAVE, index);
	}

	if(index<0 || index>>3)
	{
		sLangChange.LoadStringA(IDS_STRING950);	//PCB Index °ªÀÌ ºñÁ¤»ó ÀÔ´Ï´Ù.
		errMsg2(Task.AutoFlag, sLangChange);
		return false;
	}

		
	Task.m_bResign_Result[index] = false;

	float innerOffsetX	= (float)((model.m_ResinDrawSize.x*0.5) - model.m_dResinInspOffset[0].x);
	float innerOffsetY	= (float)((model.m_ResinDrawSize.y*0.5) - model.m_dResinInspOffset[0].y);
	float resignCheckX	= (float)((model.m_ResinDrawSize.x*0.5) + model.m_dResinInspOffset[1].x);
	float resignCheckY	= (float)((model.m_ResinDrawSize.y*0.5) + model.m_dResinInspOffset[1].y);
	int i_limit_rate	= model.m_iResinInspLimit;

	int margine = 5;

	char	szDispData[256];
	CString sTemp;
	double ep1, ep2;

	ep1 = myTimer(true);


	vision.clearOverlay();

	int width = MbufInquire(vision.MilProcImage[0], M_PITCH, NULL);


	int x, y, pos, sum, minVal, maxVal, avgVal;
	int sx, sy, ex, ey;
	CRect inRect, outRect;

	double centX = Task.d_mark_pos_x[1][index];
	double centY = Task.d_mark_pos_y[1][index];

	inRect.left		= (int)(centX - innerOffsetX / sysData.dCamResol[iCh].x - 0.5);
	inRect.right	= (int)(centX + innerOffsetX / sysData.dCamResol[iCh].x + 0.5);
	inRect.top		= (int)(centY - innerOffsetY / sysData.dCamResol[iCh].y + 0.5);
	inRect.bottom	= (int)(centY + innerOffsetY / sysData.dCamResol[iCh].y - 0.5);

	outRect.left	= (int)(centX - resignCheckX / sysData.dCamResol[iCh].x - 0.5);
	outRect.right	= (int)(centX + resignCheckX / sysData.dCamResol[iCh].x + 0.5);
	outRect.top		= (int)(centY - resignCheckY / sysData.dCamResol[iCh].y + 0.5);
	outRect.bottom	= (int)(centY + resignCheckY / sysData.dCamResol[iCh].y - 0.5);


	vision.crosslist[iCh].addList((int)centX, (int)centY, 30, M_COLOR_GREEN);				// µµÆ÷ °Ë»ç ¿µ¿ª..

	vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_RED);				// µµÆ÷ °Ë»ç ¿µ¿ª..
	vision.boxlist[iCh].addList(outRect, PS_SOLID, M_COLOR_GREEN);


	int Hist[CAM_SIZE_X];


	int inspStartPosX[CAM_SIZE_X]	= {0, };
	int inspEndPosX[CAM_SIZE_X]		= {0, };
	int inspStartPosY[CAM_SIZE_Y]	= {0, };
	int inspEndPosY[CAM_SIZE_Y]		= {0, };

	int startPoint[CAM_SIZE_X]		= {0, };
	int endPoint[CAM_SIZE_X]		= {0, };


	int leftVal = 0;
	int leftPos = 0;
	int rightVal = 0;
	int rightPos = 0;
	int topVal = 0;
	int topPos = 0;
	int bottomVal = 0;
	int bottomPos = 0;

	//////////////////////////////////////////////////////////////////////////////////
	// ÁÂÃø µµÆ÷ ¿µ¿ª °Ë»ç Start..
	sx = outRect.left; 
	ex = inRect.left;
	sy = (int)(inRect.top + fabs((outRect.top - inRect.top)*0.5));
	ey = (int)(inRect.bottom - fabs((outRect.bottom - inRect.bottom)*0.5));


	if (sx<0)					sx = 0;
	if (ex>=CAM_SIZE_X)			ex = CAM_SIZE_X - 1;
	if (sy<0)					sy = 0;
	if (ey>=CAM_SIZE_Y)			ey = CAM_SIZE_Y - 1;

	if((ex-sx)<10)
	{
		sTempLang.LoadStringA(IDS_STRING119);
		sLangChange.Format(sTempLang, sx, ex);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("Arial"));
		sLangChange.LoadStringA(IDS_STRING246);
		putListLog(sLangChange);
		return false;
	}

	if((ey-sy)<10)
	{
		sTempLang.LoadStringA(IDS_STRING120);
		sLangChange.Format(sTempLang, sy, ey);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("Arial"));
		sLangChange.LoadStringA(IDS_STRING247);
		putListLog(sLangChange);
		return false;
	}


	minVal = 255;
	maxVal = 0;
	avgVal = 0;

	memset(Hist, 0x00, sizeof(int)*CAM_SIZE_X);

	for(x=sx; x<ex; x++)
	{
		sum = 0;
		pos = sy*width + x;

		for(y=sy; y<ey; y++)
		{
			sum += vision.MilImageBuffer[1][pos];
			pos += width;
		}
		avgVal += sum;
		Hist[x] = sum;

		if(minVal > sum / (ey - sy))
			minVal = sum / (ey - sy);
		if(maxVal < sum / (ey - sy))
			maxVal = sum / (ey - sy);
	}

	avgVal = avgVal / ((ex-sx)*(ey-sy));

	if(dispMode)
	{
		sTempLang.LoadStringA(IDS_STRING121);
		sLangChange.Format(sTempLang, minVal, maxVal, avgVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 60, szDispData, M_COLOR_GREEN, 17, 8, _T("arialuni"));
	}


	if((maxVal-minVal)<70)
	{
		sTempLang.LoadStringA(IDS_STRING137);
		sLangChange.Format(sTempLang, minVal, maxVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sTempLang.LoadStringA(IDS_STRING357);
		sLangChange.Format(sTempLang, minVal, maxVal);
		_stprintf_s(szDispData, sLangChange);	//\n[µµÆ÷ °Ë»ç]\nÁÂÃø µµÆ÷ ¿µ¿ªÀÇ ¹à±â Â÷ÀÌ°¡ ³Ê¹« ÀÛ½À´Ï´Ù.\n(Min %d, Max %d)

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);

		return false;
	}

	if(avgVal>230)	//               µµÆ÷ ¿µ¿ªÀº ¾îµÎ ¿ö¾ß ÇÔ
	{
		sTempLang.LoadStringA(IDS_STRING135);
		sLangChange.Format(sTempLang, avgVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sTempLang.LoadStringA(IDS_STRING358);
		sLangChange.Format(sTempLang, avgVal);
		_stprintf_s(szDispData, sLangChange);	//\n[µµÆ÷ °Ë»ç]\nÁÂÃø µµÆ÷ ¿µ¿ªÀÌ ¿µ»óÀÌ ³Ê¹« ¹à½À´Ï´Ù.\n(Avg %d)

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);

		return false;
	}

	//if(maxVal<190)	  // ¹àÀº °÷À» °Ë»çÇÏ´Â°Ô ¾Æ´Ï°í ¾îµÎ¿î °÷À» °Ë»ç
	if(minVal > 150)
	{
		sTempLang.LoadStringA(IDS_STRING136);
		sLangChange.Format(sTempLang, minVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sTempLang.LoadStringA(IDS_STRING356);
		sLangChange.Format(sTempLang, minVal);
		_stprintf_s(szDispData, sLangChange);	//\n[µµÆ÷ °Ë»ç]\nÁÂÃø µµÆ÷ LineÀÌ ³Ê¹« ¹à½À´Ï´Ù.\n(Min %d)

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);

		return false;
	}

	


	int whiteVal	= (int)(minVal*1.3 + (avgVal * 0.3));//(maxVal*0.4 + avgVal*0.6);        // °ªÀ» ÃÖ´ëÇÑ ³·°Ô
	int whiteVal2	= whiteVal * 3;							// °ªÀ» ´õ ³·Ãç¾ßÇÔ
	int whiteVal3	= whiteVal * 3;


	int val_start1, val_start2, val_end1, val_end2;
	int startPos, endPos, startVal, endVal;

	bool findFlag;


	sy = inRect.top - 5;
	ey = inRect.bottom + 5;

	sx += 5;
	ex -= 5;

	if(sx>(ex-3))
	{
		sLangChange.LoadStringA(IDS_STRING248);
		_stprintf_s(szDispData, sLangChange, sx-5, ex+5);
		putListLog(szDispData);

		sx = (sx+ex)/2 - 1;
		ex = sx + 3;
	}


	for(y=sy; y<ey; y++)
	{
		pos = y*width + sx;

		startVal	= -9999;
		endVal		= -9999;
		startPos	= 9999;
		endPos		= 0;
		findFlag	= false;

		val_start1	= vision.MilImageBuffer[iCh][pos-1] + vision.MilImageBuffer[iCh][pos-2] + vision.MilImageBuffer[iCh][pos-3] + vision.MilImageBuffer[iCh][pos-4] + vision.MilImageBuffer[iCh][pos-5];
		val_end1	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos+1];

		val_start2	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos-1];
		val_end2	= vision.MilImageBuffer[iCh][pos+1] + vision.MilImageBuffer[iCh][pos+2] + vision.MilImageBuffer[iCh][pos+3] + vision.MilImageBuffer[iCh][pos+4] + vision.MilImageBuffer[iCh][pos+5];

		for(x=sx; x<ex; x++)
		{
			if( (vision.MilImageBuffer[iCh][pos]>=(vision.MilImageBuffer[iCh][pos-2]-margine)) && (vision.MilImageBuffer[1][pos-1]>=(vision.MilImageBuffer[iCh][pos-3]-margine)) && (vision.MilImageBuffer[iCh][pos-2]>=(vision.MilImageBuffer[iCh][pos-4]-margine)) &&
				vision.MilImageBuffer[iCh][pos-1]>=whiteVal && vision.MilImageBuffer[iCh][pos]<=whiteVal)
			{
				if( (val_end1-val_start1) > startVal)
				{
					startVal = val_end1-val_start1;
					startPos = x;
					inspStartPosX[x]++;

					startPoint[y] = x;
					
					if(dispMode==2)
						vision.crosslist[iCh].addList(x, y, 1, M_COLOR_MAGENTA);

					findFlag = true;
				}
			}

			if(	findFlag && 
				(	vision.MilImageBuffer[iCh][pos]>=(vision.MilImageBuffer[iCh][pos+2]-margine)) && (vision.MilImageBuffer[iCh][pos+1]>=(vision.MilImageBuffer[iCh][pos+3]-margine)) && (vision.MilImageBuffer[iCh][pos+2]>=(vision.MilImageBuffer[iCh][pos+4]-margine)) &&
				vision.MilImageBuffer[iCh][pos+1]>=whiteVal && vision.MilImageBuffer[iCh][pos]<=whiteVal)
			{
				if( (val_end2-val_start2) > endVal)
				{
					endVal = val_end2-val_start2;
					endPos = x;
					if(dispMode==2)
					{
						vision.crosslist[iCh].addList(x, y, 1, M_COLOR_BLUE);
					}

					endPoint[y] = x;

					inspEndPosX[x]++;
				}
			}
			pos++;

			val_start1	= val_start1 + vision.MilImageBuffer[iCh][pos-1] - vision.MilImageBuffer[iCh][pos-6];
			val_end1	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos+1];

			val_start2	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos-1];
			val_end2	= val_end2 - vision.MilImageBuffer[iCh][pos] + vision.MilImageBuffer[iCh][pos+5];
		}
	}


	startVal	= -9999;
	endVal		= -9999;
	startPos	= 9999;
	endPos		= 0;

	for(x=sx; x<ex; x++)
	{
		if(inspStartPosX[x]>startVal)
		{
			startVal = inspStartPosX[x];
			startPos = x;
		}

		if(inspEndPosX[x] > endVal)
		{
			endVal = inspEndPosX[x];
			endPos = x;
		}
	}

	if(startVal>endVal)
	{
		endVal		= -9999;
		endPos		= 0;

		for(x=startPos; x<ex; x++)
		{
			if(inspEndPosX[x] > endVal)
			{
				endVal = inspEndPosX[x];
				endPos = x;
			}
		}
	}
	else
	{
		startVal	= -9999;
		startPos	= 9999;

		for(x=sx; x<endPos; x++)
		{
			if(inspStartPosX[x]>startVal)
			{
				startVal = inspStartPosX[x];
				startPos = x;
			}
		}
	}


	//	memset(Hist, 0x00, sizeof(int)*CAM_SIZE_X);

	maxVal = 0;

	for (x=startPos; x<=endPos; x++)
	{
		if(Hist[x]>maxVal)
		{
			maxVal = Hist[x];
			leftPos = x;
		}
	}
	//	leftPos = (startPos + endPos + 1) / 2;
	if(leftPos == 0 || leftPos >= 270)
	{
		leftPos = 262;
	}

	leftVal = 0;
	for(y=sy; y<ey; y++)
	{
		pos = y*width + leftPos;

		startVal = vision.MilImageBuffer[iCh][pos] + vision.MilImageBuffer[iCh][pos+1];
		if(startVal <= whiteVal2)
		{
			leftVal++;
			continue;
		}

		startVal = vision.MilImageBuffer[iCh][pos-2] + vision.MilImageBuffer[iCh][pos-3];
		if(startVal <= whiteVal2)
		{
			leftVal++;
			continue;
		}

		startVal = vision.MilImageBuffer[iCh][pos+2] + vision.MilImageBuffer[iCh][pos+3];
		if(startVal <= whiteVal2)
		{
			leftVal++;
			continue;
		}
	}




	//! 2013. 06. 10. µµÆ÷ °Ë»ç.. Edge ÀÎ½Ä ¸¸ ÀÌ¿ëÇÏ¿© °Ë»ç ÇÏµµ·Ï Test..
// 	leftVal = 0;
// 	for(int y=sy; y<ey; y++)
// 	{
// 		if(startPoint[y]>sx && startPoint[y]<endPoint[y])
// 		{
// 			leftVal++;
// 		}
// 	}


	Task.m_line_cnt[0]	= leftVal;
	Task.m_line_pos[0]	= leftPos;
	Task.m_f_line_rate[0]= leftVal * 100.0f / (inRect.bottom - inRect.top - 20);

	if(Task.m_f_line_rate[0]>100)
		Task.m_f_line_rate[0] = 100;


	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	// ¿ìÃø µµÆ÷ ¿µ¿ª °Ë»ç Start..

	sx = inRect.right;
	ex = outRect.right;
	sy = (int)(inRect.top + fabs((outRect.top - inRect.top)*0.5));
	ey = (int)(inRect.bottom - fabs((outRect.bottom - inRect.bottom)*0.5));

	if (sx<0)					sx = 0;
	if (ex>=CAM_SIZE_X)			ex = CAM_SIZE_X - 1;
	if (sy<0)					sy = 0;
	if (ey>=CAM_SIZE_Y)			ey = CAM_SIZE_Y - 1;

	if((ex-sx)<10)
	{
		sTempLang.LoadStringA(IDS_STRING138);
		sLangChange.Format(sTempLang, sx, ex);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
		sLangChange.LoadStringA(IDS_STRING1317);	//¿ìÃø °Ë»ç ¿µ¿ª °¡·Î Æø ÀÌ»ó
		putListLog(sLangChange);
		return false;
	}

	if((ey-sy)<10)
	{
		sTempLang.LoadStringA(IDS_STRING139);
		sLangChange.Format(sTempLang, sy, ey);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sLangChange.LoadStringA(IDS_STRING1318);	//¿ìÃø °Ë»ç ¿µ¿ª ¼¼·Î Æø ÀÌ»ó
		putListLog(sLangChange);
		return false;
	}


	minVal = 255;
	maxVal = 0;
	avgVal = 0;

	memset(Hist, 0x00, sizeof(int)*CAM_SIZE_X);

	for(x=sx; x<ex; x++)
	{
		sum = 0;
		pos = sy*width + x;

		for(y=sy; y<ey; y++)
		{
			sum += vision.MilImageBuffer[iCh][pos];
			pos += width;
		}
		avgVal += sum;

		Hist[x] = sum;

		if(minVal > sum / (ey - sy))
			minVal = sum / (ey - sy);
		if(maxVal < sum / (ey - sy))
			maxVal = sum / (ey - sy);
	}

	avgVal = avgVal / ((ex-sx)*(ey-sy));


	if(dispMode)
	{
		sTempLang.LoadStringA(IDS_STRING140);
		sLangChange.Format(sTempLang, minVal, maxVal, avgVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 100, szDispData, M_COLOR_GREEN, 17, 8, _T("arialuni"));
	}

	if((maxVal-minVal)<70)
	{
		sTempLang.LoadStringA(IDS_STRING143);
		sLangChange.Format(sTempLang, minVal, maxVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sTempLang.LoadStringA(IDS_STRING354);
		sLangChange.Format(sTempLang, minVal, maxVal);
		_stprintf_s(szDispData, sLangChange);	//\n[µµÆ÷ °Ë»ç]\n¿ìÃø µµÆ÷ ¿µ¿ªÀÇ ¹à±â Â÷ÀÌ°¡ ³Ê¹« ÀÛ½À´Ï´Ù.\n(Min %d, Max %d)

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);
		return false;
	}

	if(avgVal>230)	
	{
		sTempLang.LoadStringA(IDS_STRING141);
		sLangChange.Format(sTempLang, avgVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sTempLang.LoadStringA(IDS_STRING355);
		sLangChange.Format(sTempLang, avgVal);
		_stprintf_s(szDispData, sLangChange);	//\n[µµÆ÷ °Ë»ç]\n¿ìÃø µµÆ÷ ¿µ¿ªÀÌ ³Ê¹« ¹à½À´Ï´Ù.\n(Avg %d)

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);
		return false;
	}

	//if(maxVal<190)
	if(minVal > 150)
	{
		sTempLang.LoadStringA(IDS_STRING142);
		sLangChange.Format(sTempLang, minVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sTempLang.LoadStringA(IDS_STRING102);
		sLangChange.Format(sTempLang, minVal);
		_stprintf_s(szDispData, sLangChange);	//\n[µµÆ÷ °Ë»ç]\n¿ìÃø µµÆ÷ LineÀÌ ³Ê¹« ¹à½À´Ï´Ù.\n(Min %d)

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);

		return false;
	}


	whiteVal	= (int)(minVal* 1.5 + avgVal*0.1);//(maxVal*0.4 + avgVal*0.6);
	whiteVal2	= whiteVal * 2;
	whiteVal3	= whiteVal * 3;


	sy = inRect.top - 5;
	ey = inRect.bottom + 5;

	sx += 5;
	ex -= 5;

	if(sx>(ex-3))
	{
		sLangChange.LoadStringA(IDS_STRING245);
		_stprintf_s(szDispData, sLangChange, sx-5, ex+5);
		putListLog(szDispData);

		sx = (sx+ex)/2 - 1;
		ex = sx + 3;
	}


	for(y=sy; y<ey; y++)
	{
		pos = y*width + sx;

		startVal	= -9999;
		endVal		= -9999;
		startPos	= 9999;
		endPos		= 0;
		findFlag	= false;

		val_start1	= vision.MilImageBuffer[iCh][pos-1] + vision.MilImageBuffer[iCh][pos-2] + vision.MilImageBuffer[iCh][pos-3] + vision.MilImageBuffer[iCh][pos-4] + vision.MilImageBuffer[iCh][pos-5];
		val_end1	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos+1];

		val_start2	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos-1];
		val_end2	= vision.MilImageBuffer[iCh][pos+1] + vision.MilImageBuffer[iCh][pos+2] + vision.MilImageBuffer[iCh][pos+3] + vision.MilImageBuffer[iCh][pos+4] + vision.MilImageBuffer[iCh][pos+5];

		for(x=sx; x<ex; x++)
		{
			if( (vision.MilImageBuffer[iCh][pos]>=(vision.MilImageBuffer[iCh][pos-2]-margine)) && (vision.MilImageBuffer[iCh][pos-1]>=(vision.MilImageBuffer[iCh][pos-3]-margine)) && (vision.MilImageBuffer[iCh][pos-2]>=(vision.MilImageBuffer[iCh][pos-4]-margine)) &&
				vision.MilImageBuffer[iCh][pos-1]>=whiteVal && vision.MilImageBuffer[iCh][pos]<=whiteVal)
			{
				if( (val_end1-val_start1) > startVal)
				{
					startVal = val_end1-val_start1;
					startPos = x;
					inspStartPosX[x]++;

					if(dispMode==2)
						vision.crosslist[iCh].addList(x, y, 1, M_COLOR_MAGENTA);

					findFlag = true;
				}
			}

			if(	findFlag && 
				(	vision.MilImageBuffer[iCh][pos]>=(vision.MilImageBuffer[iCh][pos+2]-margine)) && (vision.MilImageBuffer[iCh][pos+1]>=(vision.MilImageBuffer[iCh][pos+3]-margine)) && (vision.MilImageBuffer[iCh][pos+2]>=(vision.MilImageBuffer[iCh][pos+4]-margine)) &&
				vision.MilImageBuffer[iCh][pos+1]>=whiteVal && vision.MilImageBuffer[iCh][pos]<=whiteVal)
			{
				if( (val_end2-val_start2) > endVal)
				{
					endVal = val_end2-val_start2;
					endPos = x;
					inspEndPosX[x]++;

					if(dispMode==2)
						vision.crosslist[iCh].addList(x, y, 1, M_COLOR_BLUE);
				}
			}
			pos++;

			val_start1	= val_start1 + vision.MilImageBuffer[iCh][pos-1] - vision.MilImageBuffer[iCh][pos-6];
			val_end1	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos+1];

			val_start2	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos-1];
			val_end2	= val_end2 - vision.MilImageBuffer[iCh][pos] + vision.MilImageBuffer[iCh][pos+5];
		}
	}

	startVal	= -9999;
	endVal		= -9999;
	startPos	= 9999;
	endPos		= 0;

	for(x=sx; x<ex; x++)
	{
		if(inspStartPosX[x]>startVal)
		{
			startVal = inspStartPosX[x];
			startPos = x;
		}

		if(inspEndPosX[x] > endVal)
		{
			endVal = inspEndPosX[x];
			endPos = x;
		}
	}

	if(startVal>endVal)
	{
		endVal		= -9999;
		endPos		= 0;

		for(x=startPos; x<ex; x++)
		{
			if(inspEndPosX[x] > endVal)
			{
				endVal = inspEndPosX[x];
				endPos = x;
			}
		}
	}
	else
	{
		startVal	= -9999;
		startPos	= 9999;

		for(x=sx; x<endPos; x++)
		{
			if(inspStartPosX[x]>startVal)
			{
				startVal = inspStartPosX[x];
				startPos = x;
			}
		}
	}

	//	memset(Hist, 0x00, sizeof(int)*CAM_SIZE_X);

	maxVal = 0;

	for (x=startPos; x<=endPos; x++)
	{
		if(Hist[x]>maxVal)
		{
			maxVal = Hist[x];
			rightPos = x;
		}
	}
	if(rightPos == 0 || rightPos < 760)
	{
		rightPos = 780;
	}
	//	rightPos = (startPos + endPos + 1) / 2;

	rightVal = 0;
	for(y=sy; y<ey; y++)
	{
		pos = y*width + rightPos;

		startVal = vision.MilImageBuffer[iCh][pos] + vision.MilImageBuffer[iCh][pos+1];
		if(startVal <= whiteVal2)
		{
			rightVal++;
			continue;
		}

		startVal = vision.MilImageBuffer[iCh][pos-2] + vision.MilImageBuffer[iCh][pos-3];
		if(startVal <= whiteVal2)
		{
			rightVal++;
			continue;
		}

		startVal = vision.MilImageBuffer[iCh][pos+2] + vision.MilImageBuffer[iCh][pos+3];
		if(startVal <= whiteVal2)
		{
			rightVal++;
			continue;
		}
	}

	Task.m_line_cnt[1]	= rightVal;
	Task.m_line_pos[1]	= rightPos;
	Task.m_f_line_rate[1]= rightVal * 100.0f / (inRect.bottom - inRect.top - 20);

	if(Task.m_f_line_rate[1]>100)
		Task.m_f_line_rate[1] = 100;




	double resinRectSizeX = (rightPos - leftPos) * sysData.dCamResol[iCh].x;

	if (model.m_ResinDrawSize.x*0.8>resinRectSizeX || resinRectSizeX>model.m_ResinDrawSize.x*1.2)
	{
		sTempLang.LoadStringA(IDS_STRING349);
		sLangChange.Format(sTempLang, resinRectSizeX, model.m_ResinDrawSize.x);
		_stprintf_s(szDispData, sLangChange);	//\n[µµÆ÷ °Ë»ç]\nµµÆ÷ °¡·Î ÆøÀÌ ºñÁ¤»óÀÔ´Ï´Ù.\n[µµÆ÷ Æø %.01f mm, Spec %.01f mm

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);
	}



	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	// »óºÎ µµÆ÷ ¿µ¿ª °Ë»ç Start..

	sx = (int)(inRect.left + fabs((outRect.left - inRect.left)*0.5));
	ex = (int)(inRect.right - fabs((outRect.right - inRect.right)*0.5));
	sy = outRect.top;
	ey = inRect.top;


	//	vision.boxlist[iCh].addList(sx, sy, ex, ey, PS_SOLID, M_COLOR_GREEN);

	if (sx<0)					sx = 0;
	if (ex>=CAM_SIZE_X)			ex = CAM_SIZE_X - 1;
	if (sy<0)					sy = 0;
	if (ey>=CAM_SIZE_Y)			ey = CAM_SIZE_Y - 1;

	if((ex-sx)<10)
	{
		sTempLang.LoadStringA(IDS_STRING144);
		sLangChange.Format(sTempLang, sx, ex);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
		sLangChange.LoadStringA(IDS_STRING242);
		putListLog(sLangChange);
		return false;
	}

	if((ey-sy)<10)
	{
		sTempLang.LoadStringA(IDS_STRING145);
		sLangChange.Format(sTempLang, sy, ey);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
		sLangChange.LoadStringA(IDS_STRING243);
		putListLog(sLangChange);
		return false;
	}


	minVal = 255;
	maxVal = 0;
	avgVal = 0;

	memset(Hist, 0x00, sizeof(int)*CAM_SIZE_X);

	for(y=sy; y<ey; y++)
	{
		sum = 0;
		pos = y*width + sx;

		for(x=sx; x<ex; x++)
		{
			sum += vision.MilImageBuffer[iCh][pos];
			pos++;
		}
		avgVal += sum;

		Hist[y] = sum;

		if(minVal > sum / (ex - sx))				minVal = sum / (ex - sx);
		if(maxVal < sum / (ex - sx))				maxVal = sum / (ex - sx);
	}

	avgVal = avgVal / ((ex-sx) * (ey-sy));


	if(dispMode)
	{
		sTempLang.LoadStringA(IDS_STRING122);
		sLangChange.Format(sTempLang, minVal, maxVal, avgVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 140, szDispData, M_COLOR_GREEN, 17, 8, _T("arialuni"));
	}

	


	// ¹àÀº ¿µ¿ª°ú ¾îµÎ¿î ¿µ¿ªÀÇ ¹à±â Â÷°¡ 50 ÀÌ»óÀÌ°í, Æò±Õ ¹à±â´Â 200 ÀÌÇÏ·Î..
	if( (maxVal-minVal)<70)
	{
		sTempLang.LoadStringA(IDS_STRING148);
		sLangChange.Format(sTempLang, minVal, maxVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sTempLang.LoadStringA(IDS_STRING351);
		sLangChange.Format(sTempLang, minVal, maxVal);
		_stprintf_s(szDispData, sLangChange);	//\n[µµÆ÷ °Ë»ç]\n»óºÎ µµÆ÷ ¿µ¿ªÀÇ ¹à±â Â÷ÀÌ°¡ ³Ê¹« ÀÛ½À´Ï´Ù.\n(Min %d, Max %d)

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);

		return false;
	}

	if(avgVal>230 ) 	
	{
		sTempLang.LoadStringA(IDS_STRING146);
		sLangChange.Format(sTempLang, avgVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
		sTempLang.LoadStringA(IDS_STRING352);
		sLangChange.Format(sTempLang, avgVal);
		_stprintf_s(szDispData, sLangChange);	//\n[µµÆ÷ °Ë»ç]\n»óºÎ µµÆ÷ ¿µ¿ªÀÌ ³Ê¹« ¹à½À´Ï´Ù.\n(Avg %d)

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);

		return false;
	}

	//if(maxVal<190)	
	if(minVal > 150)
	{
		sTempLang.LoadStringA(IDS_STRING147);
		sLangChange.Format(sTempLang, minVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sTempLang.LoadStringA(IDS_STRING350);
		sLangChange.Format(sTempLang, minVal);
		_stprintf_s(szDispData, sLangChange);	//\n[µµÆ÷ °Ë»ç]\n»óºÎ µµÆ÷ LineÀÌ ³Ê¹« ¹à½À´Ï´Ù.\n(Min %d)
		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);

		return false;
	}


	whiteVal	= (int)(minVal*1.5 + avgVal*0.3);//(maxVal*0.9 + avgVal*0.1);
//	whiteVal	= (int)(maxVal*0.35 + avgVal*0.65);
	whiteVal2	= whiteVal * 3;
	whiteVal3	= whiteVal * 3;

	int width2	= 2*width;
	int width3	= 3*width;
	int width4	= 4*width;
	int width5	= 5*width;

	sx = inRect.left - 5;
	ex = inRect.right + 5;

	sy += 5;
	ey -= 5;

	if(sy>(ey-3))
	{
		sLangChange.LoadStringA(IDS_STRING244);
		_stprintf_s(szDispData, sLangChange, sy-5, ey+5);
		putListLog(szDispData);

		sy = (sy+ey)/2 - 1;
		ey = sy + 3;
	}


	for(x=sx; x<ex; x++)
	{
		pos = sy*width + x;

		startVal	= -9999;
		endVal		= -9999;
		startPos	= 9999;
		endPos		= 0;
		findFlag	= false;

		val_start1	= vision.MilImageBuffer[iCh][pos-width] + vision.MilImageBuffer[iCh][pos-width2] + vision.MilImageBuffer[iCh][pos-width3] + vision.MilImageBuffer[iCh][pos-width4] + vision.MilImageBuffer[iCh][pos-width5];
		val_end1	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos+width];

		val_start2	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos-width];
		val_end2	= vision.MilImageBuffer[iCh][pos+width] + vision.MilImageBuffer[iCh][pos+width2] + vision.MilImageBuffer[iCh][pos+width3] + vision.MilImageBuffer[iCh][pos+width4] + vision.MilImageBuffer[iCh][pos+width5];

		for(y=sy; y<ey; y++)
		{
			if( (vision.MilImageBuffer[iCh][pos]>=(vision.MilImageBuffer[iCh][pos-width2]-margine)) && (vision.MilImageBuffer[iCh][pos-width]>=(vision.MilImageBuffer[iCh][pos-width3]-margine)) && (vision.MilImageBuffer[iCh][pos-width2]>=(vision.MilImageBuffer[iCh][pos-width4]-margine)) &&
				vision.MilImageBuffer[iCh][pos-width]>=whiteVal && vision.MilImageBuffer[iCh][pos]<=whiteVal)
			{
				if( (val_end1-val_start1) > startVal)
				{
					startVal = val_end1-val_start1;
					startPos = y;
					inspStartPosY[y]++;

					if(dispMode==2)
						vision.crosslist[iCh].addList(x, y, 1, M_COLOR_MAGENTA);

					findFlag = true;
				}
			}

			if(	findFlag && 
				(	vision.MilImageBuffer[iCh][pos]>=(vision.MilImageBuffer[iCh][pos+width2]-margine)) && (vision.MilImageBuffer[iCh][pos+width]>=(vision.MilImageBuffer[iCh][pos+width3]-margine)) && (vision.MilImageBuffer[iCh][pos+width2]>=(vision.MilImageBuffer[iCh][pos+width4]-margine)) &&
				vision.MilImageBuffer[iCh][pos+width]>=whiteVal && vision.MilImageBuffer[iCh][pos]<=whiteVal)
			{
				if( (val_end2-val_start2) > endVal)
				{
					endVal = val_end2-val_start2;
					endPos = y;
					inspEndPosY[y]++;

					if(dispMode==2)
						vision.crosslist[iCh].addList(x, y, 1, M_COLOR_BLUE);
				}
			}
			pos += width;

			val_start1	= val_start1 + vision.MilImageBuffer[iCh][pos-1] - vision.MilImageBuffer[iCh][pos-6];
			val_end1	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos+1];

			val_start2	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos-1];
			val_end2	= val_end2 - vision.MilImageBuffer[iCh][pos] + vision.MilImageBuffer[iCh][pos+5];
		}
	}

	startVal	= -9999;
	endVal		= -9999;
	startPos	= 9999;
	endPos		= 0;

	for(y=sy; y<ey; y++)
	{
		if(inspStartPosY[y]>startVal)
		{
			startVal = inspStartPosY[y];
			startPos = y;
		}

		if(inspEndPosY[y] > endVal)
		{
			endVal = inspEndPosY[y];
			endPos = y;
		}
	}

	if(startVal>endVal)
	{
		endVal		= -9999;
		endPos		= 0;

		for(y=startPos; y<ey; y++)
		{
			if(inspEndPosY[y] > endVal)
			{
				endVal = inspEndPosY[y];
				endPos = y;
			}
		}
	}
	else
	{
		startVal	= -9999;
		startPos	= 9999;

		for(y=sy; y<endPos; y++)
		{
			if(inspStartPosY[y]>startVal)
			{
				startVal = inspStartPosY[y];
				startPos = y;
			}
		}
	}


	//	memset(Hist, 0x00, sizeof(int)*CAM_SIZE_X);

	maxVal = 0;

	for (y=startPos; y<=endPos; y++)
	{
		if(Hist[y]>maxVal)
		{
			maxVal = Hist[y];
			topPos = y;
		}
	}
	if(topPos == 0)
	{
		topPos = 92;
	}
	//	topPos = (startPos + endPos + 1) / 2;

	topVal = 0;
	for(x=sx; x<ex; x++)
	{
		pos = topPos*width + x;

		startVal = vision.MilImageBuffer[iCh][pos] + vision.MilImageBuffer[iCh][pos+width];
		if(startVal <= whiteVal2)
		{
			topVal++;
			continue;
		}

		startVal = vision.MilImageBuffer[iCh][pos-width2] + vision.MilImageBuffer[iCh][pos-width3];
		if(startVal <= whiteVal2)
		{
			topVal++;
			continue;
		}

		startVal = vision.MilImageBuffer[iCh][pos+width2] + vision.MilImageBuffer[iCh][pos+width3];
		if(startVal <= whiteVal2)
		{
			topVal++;
			continue;
		}
		continue;
	}

	Task.m_line_cnt[2]	= topVal;
	Task.m_line_pos[2]	= topPos;
	Task.m_f_line_rate[2]= topVal * 100.0f / (inRect.right - inRect.left - 20);

	if(Task.m_f_line_rate[2]>100)
		Task.m_f_line_rate[2] = 100;


	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	// ÇÏºÎ µµÆ÷ ¿µ¿ª °Ë»ç Start..
	sx = (int)(inRect.left + fabs((outRect.left - inRect.left)*0.5));
	ex = (int)(inRect.right - fabs((outRect.right - inRect.right)*0.5));
	sy = inRect.bottom;
	ey = outRect.bottom;


	//	vision.boxlist[iCh].addList(sx, sy, ex, ey, PS_SOLID, M_COLOR_GREEN);

	if (sx<0)					sx = 0;
	if (ex>=CAM_SIZE_X)			ex = CAM_SIZE_X - 1;
	if (sy<0)					sy = 0;
	if (ey>=CAM_SIZE_Y)			ey = CAM_SIZE_Y - 1;

	if((ex-sx)<10)
	{
		sTempLang.LoadStringA(IDS_STRING123);
		sLangChange.Format(sTempLang, sx, ex);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
		sLangChange.LoadStringA(IDS_STRING249);
		putListLog(sLangChange);
		return false;
	}

	if((ey-sy)<10)
	{
		sTempLang.LoadStringA(IDS_STRING124);
		sLangChange.Format(sTempLang, sy, ey);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
		sLangChange.LoadStringA(IDS_STRING250);
		putListLog(sLangChange);
		return false;
	}


	minVal = 255;
	maxVal = 0;
	avgVal = 0;

	memset(Hist, 0x00, sizeof(int)*CAM_SIZE_X);

	for(y=sy; y<ey; y++)
	{
		sum = 0;
		pos = y*width + sx;

		for(x=sx; x<ex; x++)
		{
			sum += vision.MilImageBuffer[iCh][pos];
			pos++;
		}
		avgVal += sum;

		Hist[y] = sum;

		if(minVal > sum / (ex - sx))				minVal = sum / (ex - sx);
		if(maxVal < sum / (ex - sx))				maxVal = sum / (ex - sx);
	}

	avgVal = avgVal / ((ex-sx) * (ey-sy));

	if(dispMode)
	{
		sTempLang.LoadStringA(IDS_STRING125);
		sLangChange.Format(sTempLang, minVal, maxVal, avgVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 180, szDispData, M_COLOR_GREEN, 17, 8, _T("arialuni"));
	}


	// ¹àÀº ¿µ¿ª°ú ¾îµÎ¿î ¿µ¿ªÀÇ ¹à±â Â÷°¡ 50 ÀÌ»óÀÌ°í, Æò±Õ ¹à±â´Â 200 ÀÌÇÏ·Î..
	if( (maxVal-minVal)<70)
	{
		sTempLang.LoadStringA(IDS_STRING127);
		sLangChange.Format(sTempLang, minVal, maxVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
		
		sTempLang.LoadStringA(IDS_STRING360);
		sLangChange.Format(sTempLang, minVal, maxVal);

		_stprintf_s(szDispData, sLangChange);	//\n[µµÆ÷ °Ë»ç]\nÇÏºÎ µµÆ÷ ¿µ¿ªÀÇ ¹à±â Â÷ÀÌ°¡ ³Ê¹« ÀÛ½À´Ï´Ù.\n(Min %d, Max %d)

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);

		return false;
	}

	if( avgVal>230 ) 	
	{
		sTempLang.LoadStringA(IDS_STRING125);
		sLangChange.Format(sTempLang, avgVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sTempLang.LoadStringA(IDS_STRING361);
		sLangChange.Format(sTempLang, avgVal);
		_stprintf_s(szDispData, sLangChange);	//\n[µµÆ÷ °Ë»ç]\nÇÏºÎ µµÆ÷ ¿µ¿ªÀÌ ³Ê¹« ¹à½À´Ï´Ù.\n(Avg %d)

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);

		return false;
	}

	//if(maxVal<190)	
	if(minVal > 150)
	{
		sTempLang.LoadStringA(IDS_STRING126);
		sLangChange.Format(sTempLang, minVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sTempLang.LoadStringA(IDS_STRING359);
		sLangChange.Format(sTempLang, minVal);
		_stprintf_s(szDispData, sLangChange);	//\n[µµÆ÷ °Ë»ç]\nÇÏºÎ µµÆ÷ LineÀÌ ³Ê¹« ¹à½À´Ï´Ù.\n(Min %d)

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);

		return false;
	}



	whiteVal	= (int)(minVal*2 + avgVal*0.5);//(maxVal*0.9 + avgVal*0.1);
//	whiteVal	= (int)(maxVal*0.4 + avgVal*0.6);
	whiteVal2	= whiteVal * 2;
	whiteVal3	= whiteVal * 3;

	width2	= 2*width;
	width3	= 3*width;
	width4	= 4*width;
	width5	= 5*width;

	sx = inRect.left - 5;
	ex = inRect.right + 5;

	sy += 5;
	ey -= 5;

	if(sy>(ey-3))
	{
		sLangChange.LoadStringA(IDS_STRING251);
		_stprintf_s(szDispData, sLangChange, sy-5, ey+5);
		putListLog(szDispData);

		sy = (sy+ey)/2 - 1;
		ey = sy + 3;
	}


	for(x=sx; x<ex; x++)
	{
		pos = sy*width + x;

		startVal	= -9999;
		endVal		= -9999;
		startPos	= 9999;
		endPos		= 0;
		findFlag	= false;

		val_start1	= vision.MilImageBuffer[iCh][pos-width] + vision.MilImageBuffer[iCh][pos-width2] + vision.MilImageBuffer[iCh][pos-width3] + vision.MilImageBuffer[iCh][pos-width4] + vision.MilImageBuffer[iCh][pos-width5];
		val_end1	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos+width];

		val_start2	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos-width];
		val_end2	= vision.MilImageBuffer[iCh][pos+width] + vision.MilImageBuffer[iCh][pos+width2] + vision.MilImageBuffer[iCh][pos+width3] + vision.MilImageBuffer[iCh][pos+width4] + vision.MilImageBuffer[iCh][pos+width5];

		for(y=sy; y<ey; y++)
		{
			if(val_end1<300 && (vision.MilImageBuffer[iCh][pos]>=(vision.MilImageBuffer[iCh][pos-width2]-margine)) && (vision.MilImageBuffer[iCh][pos-width]>=(vision.MilImageBuffer[iCh][pos-width3]-margine)) && (vision.MilImageBuffer[iCh][pos-width2]>=(vision.MilImageBuffer[iCh][pos-width4]-margine)) &&
				vision.MilImageBuffer[iCh][pos-width]>=whiteVal && vision.MilImageBuffer[iCh][pos]<=whiteVal)
			{
				if( (val_end1-val_start1) > startVal)
				{
					startVal = val_end1-val_start1;
					startPos = y;
					inspStartPosY[y]++;

					if(dispMode==2)
						vision.crosslist[iCh].addList(x, y, 1, M_COLOR_MAGENTA);

					findFlag = true;
				}
			}

			if(	findFlag && (val_start2<300) &&
				(	vision.MilImageBuffer[iCh][pos]>=(vision.MilImageBuffer[iCh][pos+width2]-margine)) && (vision.MilImageBuffer[iCh][pos+width]>=(vision.MilImageBuffer[iCh][pos+width3]-margine)) && (vision.MilImageBuffer[iCh][pos+width2]>=(vision.MilImageBuffer[iCh][pos+width4]-margine)) &&
				vision.MilImageBuffer[iCh][pos+width]>=whiteVal && vision.MilImageBuffer[iCh][pos]<=whiteVal)
			{
				if( (val_end2-val_start2) > endVal)
				{
					endVal = val_end2-val_start2;
					endPos = y;
					inspEndPosY[y]++;

					if(dispMode==2)
						vision.crosslist[iCh].addList(x, y, 1, M_COLOR_BLUE);
				}
			}
			pos += width;

			val_start1	= val_start1 + vision.MilImageBuffer[iCh][pos-1] - vision.MilImageBuffer[iCh][pos-6];
			val_end1	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos+1];

			val_start2	= vision.MilImageBuffer[iCh][pos+0] + vision.MilImageBuffer[iCh][pos-1];
			val_end2	= val_end2 - vision.MilImageBuffer[iCh][pos] + vision.MilImageBuffer[iCh][pos+5];
		}
	}
	//inspStartPosY[670] = 1;
	//inspEndPosY[670] = 1;

	startVal	= -9999;
	endVal		= -9999;
	startPos	= 9999;
	endPos		= 0;

	for(y=sy; y<ey; y++)
	{
		if(inspStartPosY[y]>startVal)
		{
			startVal = inspStartPosY[y];
			startPos = y;
		}

		if(inspEndPosY[y] > endVal)
		{
			endVal = inspEndPosY[y];
			endPos = y;
		}
	}

	if(startVal>endVal)
	{
		endVal		= -9999;
		endPos		= 0;

		for(y=startPos; y<ey; y++)
		{
			if(inspEndPosY[y] > endVal)
			{
				endVal = inspEndPosY[y];
				endPos = y;
			}
		}
	}
	else
	{
		startVal	= -9999;
		startPos	= 9999;

		for(y=sy; y<endPos; y++)
		{
			if(inspStartPosY[y]>startVal)
			{
				startVal = inspStartPosY[y];
				startPos = y;
			}
		}
	}


	//	memset(Hist, 0x00, sizeof(int)*CAM_SIZE_X);

	maxVal = 0;

	for (y=startPos; y<=endPos; y++)
	{
		if(Hist[y]>maxVal)
		{
			maxVal = Hist[y];
			bottomPos = y;
		}
	}

	if(bottomPos == 0 || bottomPos <= 640)
	{
		bottomPos = 670;
	}
	//	bottomPos = (startPos + endPos + 1) / 2;

	bottomVal = 0;
	for(x=sx; x<ex; x++)
	{
		pos = bottomPos*width + x;

		startVal = vision.MilImageBuffer[iCh][pos] + vision.MilImageBuffer[iCh][pos+width];
		if(startVal <= whiteVal2)
		{
			bottomVal++;
			continue;
		}

		startVal = vision.MilImageBuffer[iCh][pos-width2] + vision.MilImageBuffer[iCh][pos-width3];
		if(startVal <= whiteVal2)
		{
			bottomVal++;
			continue;
		}

		startVal = vision.MilImageBuffer[iCh][pos+width2] + vision.MilImageBuffer[iCh][pos+width3];
		if(startVal <= whiteVal2)
		{
			bottomVal++;
			continue;
		}
	}

	Task.m_line_cnt[3]	= bottomVal;
	Task.m_line_pos[3]	= bottomPos;
	Task.m_f_line_rate[3]= bottomVal * 100.0f / (inRect.right - inRect.left - 20);

	if(Task.m_f_line_rate[3]>100)
		Task.m_f_line_rate[3] = 100;


	Task.m_bResign_Result[index] = true;


	double resinRectSizeY = (bottomPos - topPos) * sysData.dCamResol[iCh].y;

	if (model.m_ResinDrawSize.y*0.8>resinRectSizeY || resinRectSizeY>model.m_ResinDrawSize.y*1.2)
	{
		sTempLang.LoadStringA(IDS_STRING349);
		sLangChange.Format(sTempLang, resinRectSizeX, model.m_ResinDrawSize.x);
		_stprintf_s(szDispData, sLangChange);	//\n[µµÆ÷ °Ë»ç]\nµµÆ÷ °¡·Î ÆøÀÌ ºñÁ¤»óÀÔ´Ï´Ù.\n[µµÆ÷ Æø %.01f mm, Spec %.01f mm

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);
		//		return false;
	}



	unsigned long color;

	for(int i=0; i<4; i++)
	{
		if(Task.m_f_line_rate[i]>=i_limit_rate)
			color = M_COLOR_GREEN;
		else
		{
			Task.m_bResign_Result[index] = false;
			color = M_COLOR_RED;
		}

		if (i==0)
		{
			vision.linelist[iCh].addList(Task.m_line_pos[0]-2, Task.m_line_pos[2],	Task.m_line_pos[0]-2, Task.m_line_pos[3], PS_SOLID, color);
			vision.linelist[iCh].addList(Task.m_line_pos[0]-1, Task.m_line_pos[2],	Task.m_line_pos[0]-1, Task.m_line_pos[3], PS_SOLID, color);
			vision.linelist[iCh].addList(Task.m_line_pos[0]-0, Task.m_line_pos[2],	Task.m_line_pos[0]-0, Task.m_line_pos[3], PS_SOLID, color);
		}
		else if(i==1)
		{
			vision.linelist[iCh].addList(Task.m_line_pos[1]+2, Task.m_line_pos[2],	Task.m_line_pos[1]+2, Task.m_line_pos[3], PS_SOLID, color);
			vision.linelist[iCh].addList(Task.m_line_pos[1]+1, Task.m_line_pos[2],	Task.m_line_pos[1]+1, Task.m_line_pos[3], PS_SOLID, color);
			vision.linelist[iCh].addList(Task.m_line_pos[1]+0, Task.m_line_pos[2],	Task.m_line_pos[1]+0, Task.m_line_pos[3], PS_SOLID, color);
		}
		else if(i==2)
		{
			vision.linelist[iCh].addList(Task.m_line_pos[0],	Task.m_line_pos[2]-2, Task.m_line_pos[1], Task.m_line_pos[2]-2, PS_SOLID, color);
			vision.linelist[iCh].addList(Task.m_line_pos[0],	Task.m_line_pos[2]-1, Task.m_line_pos[1], Task.m_line_pos[2]-1, PS_SOLID, color);
			vision.linelist[iCh].addList(Task.m_line_pos[0],	Task.m_line_pos[2]-0, Task.m_line_pos[1], Task.m_line_pos[2]-0, PS_SOLID, color);
		}
		else if(i==3)
		{
			vision.linelist[iCh].addList(Task.m_line_pos[0],	Task.m_line_pos[3]+2, Task.m_line_pos[1], Task.m_line_pos[3]+2, PS_SOLID, color);
			vision.linelist[iCh].addList(Task.m_line_pos[0],	Task.m_line_pos[3]+1, Task.m_line_pos[1], Task.m_line_pos[3]+1, PS_SOLID, color);
			vision.linelist[iCh].addList(Task.m_line_pos[0],	Task.m_line_pos[3]+0, Task.m_line_pos[1], Task.m_line_pos[3]+0, PS_SOLID, color);
		}


		if(i==0)			sprintf_s(szDispData,  "[ L ] %.01f", Task.m_f_line_rate[i]);
		else if(i==1)		sprintf_s(szDispData,  "[ R ] %.01f", Task.m_f_line_rate[i]);
		else if(i==2)		sprintf_s(szDispData,  "[ T ] %.01f", Task.m_f_line_rate[i]);
		else if(i==3)		sprintf_s(szDispData,  "[ B ] %.01f", Task.m_f_line_rate[i]);

		vision.textlist[iCh].addList(CAM_SIZE_X-150, 650+i*25, szDispData, color, 15, 8, "Arial Black");
	}


	ep2 = myTimer(true);

	sTemp.Format("Insp Time %d msec", (int)((ep2 - ep1)));
	vision.textlist[iCh].addList(50, (CAM_SIZE_Y-60), sTemp, M_COLOR_RED, 24, 10, "Arial");


	vision.drawOverlay(CAM1);
	vision.drawOverlay(CAM2);

	if (!Task.m_bResign_Result[index])
	{
		return false;
	}

	return true; 
}
bool CAABonderDlg::_inspResignHole(bool autoMode, int index, int dispMode)
{
	int iCh = CAM1;
	if ( vision.getLiveMode() == 1 )
	{
		vision.getSnapImage(CAM1);
	}

	if( Task.m_iStatus_Unit_Epoxy == 1)
	{
		saveInspImage(EPOXY_IMAGE_SAVE, index);
	}

	if(index<0 || index>>3)
	{
		sLangChange.LoadStringA(IDS_STRING950);	//PCB Index °ªÀÌ ºñÁ¤»ó ÀÔ´Ï´Ù.
		errMsg2(Task.AutoFlag, sLangChange);
		return false;
	}
	
	Task.m_bResign_Result[index] = false;
	//====================================================µµÆ÷°Ë»çÀÌ¹ÌÁö Ãß°¡
	double dReduceFactorX = 0.;
	double dReduceFactorY = 0.;
		
	dReduceFactorX = (double) autodispDlg->m_iSizeX_Client / CAM_SIZE_X;
	dReduceFactorY = (double)autodispDlg->m_iSizeY_Client / CAM_SIZE_Y;
	//MimResize(vision.MilGrabImageChild[1], vision.MilDefectImage, dReduceFactorX, dReduceFactorY, M_DEFAULT);
	MimResize(vision.MilGrabImageChild[CAM1], vision.MilOptImage, dReduceFactorX, dReduceFactorY, M_DEFAULT);////µµÆ÷ ÀÌ¹ÌÁö È­¸é¿¡ µð½ºÇÃ·¹ÀÌ 20180827_2
	//====================================================µµÆ÷°Ë»çÀÌ¹ÌÁö Ãß°¡
	//vision.drawOverlay(CCD);
	//end
	CPoint inRadius, outRadius;
	float inOffsetX		= (float)((model.m_ResinDrawSize.x*1) - model.m_dResinInspOffset[0].x);	//¾ÈÂÊ Offset
	float inOffsetY		= (float)((model.m_ResinDrawSize.y*1) - model.m_dResinInspOffset[0].y);
	float outOffsetX	= (float)((model.m_ResinDrawSize.x*1) + model.m_dResinInspOffset[1].x);	//¹Ù±ùÂÊ Offset
	float outOffsetY	= (float)((model.m_ResinDrawSize.y*1) + model.m_dResinInspOffset[1].y);
	inRadius.x	= (int)inOffsetX/ sysData.dCamResol[iCh].x;
	outRadius.x	= (int)outOffsetX/ sysData.dCamResol[iCh].x;		
	int i_limit_rate	= model.m_iResinInspLimit;

	int margine = 5;

	char	szDispData[256];
	CString sTemp="";
	double ep1=0.0;

	ep1 = myTimer(true);

	vision.clearOverlay();

	int x = 0;
	int	y = 0;
	int	pos = 0;
	int	pos2 = 0;
	int	minVal = 0;
	int	maxVal = 0;
	int	avgVal = 0;

	int sx = 0;
	int	sy = 0;
	int	ex = 0;
	int	ey = 0;
	//cellStr = m_clGridEpoxyOffset.GetItemText(1, 1);
	//model.dEpoxyOffset_X = (float)atof(cellStr);
	//cellStr = m_clGridEpoxyOffset.GetItemText(2, 1);
	//model.dEpoxyOffset_Y = (float)atof(cellStr);

	double centX = Task.d_mark_pos_x[PCB_Holder_MARK][0];
	double centY = Task.d_mark_pos_y[PCB_Holder_MARK][0];

	centX += model.dEpoxyOffset_X;
	centY += model.dEpoxyOffset_Y;

	CRect inRect, outRect;
	inRect.left		= (int)(centX - inOffsetX / sysData.dCamResol[iCh].x  );
	inRect.right	= (int)(centX + inOffsetX / sysData.dCamResol[iCh].x  );
	inRect.top		= (int)(centY - inOffsetY / sysData.dCamResol[iCh].y  );
	inRect.bottom	= (int)(centY + inOffsetY / sysData.dCamResol[iCh].y  );
																		  
	outRect.left	= (int)(centX - outOffsetX / sysData.dCamResol[iCh].x );
	outRect.right	= (int)(centX + outOffsetX / sysData.dCamResol[iCh].x );
	outRect.top		= (int)(centY - outOffsetY / sysData.dCamResol[iCh].y );
	outRect.bottom	= (int)(centY + outOffsetY / sysData.dCamResol[iCh].y );

	vision.crosslist[iCh].addList((int)centX, (int)centY, 30, M_COLOR_GREEN);	// µµÆ÷ °Ë»ç Áß½É À§Ä¡..
	vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_RED);				// µµÆ÷ °Ë»ç ¿µ¿ª..
	vision.boxlist[iCh].addList(outRect, PS_SOLID, M_COLOR_GREEN);


	int Hist[CAM_SIZE_X] = {0, };

	int inspStartPosX[CAM_SIZE_X]	= {0, };
	int inspEndPosX[CAM_SIZE_X]		= {0, };
	int inspStartPosY[CAM_SIZE_Y]	= {0, };
	int inspEndPosY[CAM_SIZE_Y]		= {0, };

	int startPoint[CAM_SIZE_X]		= {0, };
	int endPoint[CAM_SIZE_X]		= {0, };

	int leftVal = 0;
	int leftPos = 0;
	int rightVal = 0;
	int rightPos = 0;
	int topVal = 0;
	int topPos = 0;
	int bottomVal = 0;
	int bottomPos = 0;

	//////////////////////////////////////////////////////////////////////////////////
	// ¿ø ¿µ¿ª °Ë»ç Start..
	sx = (int)outRect.left;
	ex = (int)outRect.right;
	sy = (int)outRect.top;
	ey = (int)outRect.bottom;

	if (sx<0)					sx = 0;
	if (ex>=CAM_SIZE_X)			ex = CAM_SIZE_X - 1;
	if (sy<0)					sy = 0;
	if (ey>=CAM_SIZE_Y)			ey = CAM_SIZE_Y - 1;

	if( (outOffsetX-inOffsetX) / sysData.dCamResol[iCh].x < 10)
	{
		sTempLang.LoadStringA(IDS_STRING132);
		sLangChange.Format(sTempLang, inOffsetX, outOffsetX);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
		sLangChange.LoadStringA(IDS_STRING240);
		putListLog(sLangChange);
		return false;
	}
	if((outOffsetY-inOffsetY) / sysData.dCamResol[iCh].y < 10)
	{
		sTempLang.LoadStringA(IDS_STRING133);
		sLangChange.Format(sTempLang, outOffsetY, inOffsetY);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
		sLangChange.LoadStringA(IDS_STRING241);
		putListLog(sLangChange);
		return false;
	}
	

	//!! HoleÀ» Á¦¿ÜÇÑ ³ª¸ÓÁö´Â ¸ðµÎ 0x00À¸·Î Image º¯È¯
	double HoleW=0, HoleH=0;
	double Length=0;
	unsigned char *m_Imagebuf;
	m_Imagebuf = (unsigned char *)malloc(CAM_SIZE_X*CAM_SIZE_Y);

	MbufGet(vision.MilGrabImageChild[iCh], m_Imagebuf);	// ÀÌ¹ÌÁö ÀüÃ¼ ÇÈ¼¿ ¹à±â°ª ¹öÆÛ ´ã±â

	//for(int x = 0;x<CAM_SIZE_X;x++)
	//{
	//	for(int y = 0; y<CAM_SIZE_Y; y++)
	//	{
	//		pos = y*CAM_SIZE_X+x;

	//		HoleW = abs(centX - x);
	//		HoleH = abs(centY - y);
	//		Length = sqrt(pow(HoleW, 2) + pow(HoleH, 2));
	//	
	//		if( Length > outRadius.x || Length < inRadius.x )	//-- Á¤ ¿øÀÏ °æ¿ì¸¸ ÇÇÅ¸°í¶ó½º·Î Ã³¸®µÊ... 
	//		{//!! ¿ÜºÎHole°ú ³»ºÎ Hole À» Á¦¿ÜÇÏ°í ³ª¸ÓÁö ¹à±â°ªÀ» 0À¸·Î..
	//			m_Imagebuf[pos] = 0;
	//		}
	//	}
	//}

	
	//!¿ÜºÎ¿µ¿ª ±âÁØÀ¸·Î Buf¿¡ ´ãÀ½.
	CPoint cpCutSize;
	cpCutSize.x = ex-sx;
	cpCutSize.y = ey-sy;

	unsigned char	*cutimgBuf;
	cutimgBuf		= (unsigned char *)malloc(cpCutSize.x * cpCutSize.y);
	memset(cutimgBuf, 0, sizeof(cutimgBuf));

	//! ¿ÜºÎ¿µ¿ª¹öÆÛ¸¦ cv¿¡ ´ãÀ½.
	IplImage *bufImg, *dstImg, *srcImg, *polarImg;
	srcImg = cvCreateImage(cvSize(cpCutSize.x, cpCutSize.y),8,1);
	dstImg = cvCreateImage(cvSize(cpCutSize.x, cpCutSize.y),8,1);
	bufImg = cvCreateImage(cvSize(CAM_SIZE_X, CAM_SIZE_Y),8,1);
	memcpy(bufImg->imageData, m_Imagebuf, CAM_SIZE_X * CAM_SIZE_Y);

	cvSetImageROI(bufImg, cvRect(sx, sy, cpCutSize.x, cpCutSize.y));
	cvCopyImage(bufImg, srcImg);
	cvSaveImage("D:/1_cvPolarSrc.bmp", srcImg);	//===============

	//! cv Image Polar Ã³¸®
	double M = srcImg->width/log(srcImg->width/2.0);
	cvLogPolar(srcImg, dstImg, cvPoint2D32f(srcImg->width/2, srcImg->height/2), M, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);
	cvSaveImage("D:/2_cvPolardst.bmp", dstImg);	//===============

	//!°í¸® ¿µ¿ª¸¸ ¹öÆÛ¿¡ ´Ù½Ã ´ã±â -> °í¸® Å©±â·Î..
	int iEpoxySize = (outRect.right - inRect.right)*0.6;//outOffsetX -inOffsetX;//(inRect.left - outRect.left) * 1;
	unsigned char	*imgInsp; 
	imgInsp		= (unsigned char *)malloc(iEpoxySize * cpCutSize.y);
	sx =  cpCutSize.x-iEpoxySize;//¸¶Áö¸· Æø outRect.right -iEpoxySize;// 

	polarImg = cvCreateImage(cvSize(iEpoxySize, cpCutSize.y),8,1);
	cvSetImageROI(dstImg, cvRect(sx, 0, iEpoxySize, cpCutSize.y));

	cvCopyImage(dstImg, polarImg);
	cvSaveImage("D:/3_cvPolarInsp.bmp", dstImg);	//===============

	//!¹öÆÛ¿¡ ´Ù½Ã ´ã±â
	memcpy(imgInsp, polarImg->imageData , iEpoxySize*cpCutSize.y);

	//! IplImage ¸Þ¸ð¸® ÇØÁ¦
	
	cvReleaseImage( &polarImg );
	cvReleaseImage( &dstImg );
	cvReleaseImage( &srcImg );
	cvResetImageROI(bufImg);
	cvReleaseImage( &bufImg );

	///---------------------------------------------
	// °Ë»ç´Â unsigned char	*imgInsp·Î..
	CPoint inspSize;
	inspSize.x = iEpoxySize;
	inspSize.y = cpCutSize.y;

	minVal = 255;
	maxVal = 0;
	avgVal = 0;
	memset(Hist, 0x00, sizeof(int)*(CAM_SIZE_X) );
	int sum = 0;
	

	int iSpecOverLine = 0;
	int iContinuityLine = 0;

	for(y=0; y<inspSize.y; y++)
	{
		sum = 0;
		for(x=0; x<inspSize.x; x++)
		{
			pos = y * inspSize.x + x;
			sum += imgInsp[pos];
		}

		avgVal += sum;
		Hist[x] = sum;

		if(sum / inspSize.x < 255.0 * (model.m_dResinInspHoleSpec/100.0) )
		{
			iContinuityLine++;
			if( iContinuityLine*sysData.dCamResol[iCh].x > model.m_dResinInspGapLength  )
			{//! 5È¸ ¿¬¼ÓÀ¸·Î ³ª¿Ã °æ¿ì.
				sTempLang.LoadStringA(IDS_STRING134);
				sLangChange.Format(sTempLang, y);
				_stprintf_s(szDispData, sLangChange);
				vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
				return false;
			}
		}
		else								iContinuityLine = 0;

		if(sum / inspSize.x < 255.0*(model.m_dResinInspHoleSpec/100.0) )		iSpecOverLine++;
	}

	
	free(cutimgBuf);
	free(m_Imagebuf);
	free(imgInsp);

	return true;
}


bool CAABonderDlg::_inspResignRect(bool autoMode, int index, int iDirection, int iRectCnt)
{//KKYH 20150622 ¾Æ·¡ ÀÌÇÏ ÇÔ¼ö Ãß°¡
	int iCh = 1;

	if( Task.m_iStatus_Unit_Epoxy == 1)
	{
		saveInspImage(EPOXY_IMAGE_SAVE, index);
	}

	if(index<0 || index>>3)
	{
		sLangChange.LoadStringA(IDS_STRING950);	//PCB Index °ªÀÌ ºñÁ¤»ó ÀÔ´Ï´Ù.
		errMsg2(Task.AutoFlag, sLangChange);
		return false;
	}

	double centX = Task.d_mark_pos_x[PCB_Holder_MARK][0];
	double centY = Task.d_mark_pos_y[PCB_Holder_MARK][0];
	if(centX == 0 || centY ==0)
	{
		centX = CAM_SIZE_X/2;
		centY = CAM_SIZE_Y/2;
	}
	centX += model.dEpoxyOffset_X;
	centY += model.dEpoxyOffset_Y;
	int margine = 5;
//	char	szDispData[256];
	CString sTemp;
	double ep1;

	ep1 = myTimer(true);
	int x = 0;
	int	y = 0;
	int	pos = 0;
	int	pos2 = 0;
	int	minVal = 0;
	int	maxVal = 0;
	int	avgVal = 0;

	int iColorLevel=180;
	int iDispenseCnt=0;
	int iIsDispense;
	int iDispenseSpec=5;

	CPoint point;

	if(centX >= 0 || centY >= 0)
	{
		if(iDirection == 0)	// ¼¼·Î¹æÇâ
		{
			CRect inRect;
			inRect.left = (int)(centX + model.m_ResingRectStart[iRectCnt].x/sysData.dCamResol[iCh].x );
			inRect.top = (int)(centY + model.m_ResingRectStart[iRectCnt].y/sysData.dCamResol[iCh].y );
			inRect.right = (int)(centX + (model.m_ResingRectStart[iRectCnt].x+ model.m_ResingRectSize[iRectCnt].x)/sysData.dCamResol[iCh].x );
			inRect.bottom = (int)(centY + (model.m_ResingRectStart[iRectCnt].y+ model.m_ResingRectSize[iRectCnt].y)/sysData.dCamResol[iCh].y );


			//!! Image º¯È¯
			double HoleW=0, HoleH=0;
			double Length=0;
			unsigned char *m_Imagebuf;
			m_Imagebuf = (unsigned char *)malloc(CAM_SIZE_X*CAM_SIZE_Y);

			MbufGet(vision.MilGrabImageChild[iCh], m_Imagebuf);	// ÀÌ¹ÌÁö ÀüÃ¼ ÇÈ¼¿ ¹à±â°ª ¹öÆÛ ´ã±â

			//! °Ë»ç ¿µ¿ª³» ÇÈ¼¿ÀÇ ¹à±â °ª ±¸ÇÏ±â
			int iAllSumCnt = 0, iSumCnt = 0;				//! °Ë»ç ¿µ¿ª³»ÀÇ ¹à±â°ª ¾ò¾î¿Â ÇÈ¼¿ ÀüÃ¼ÀÇ Ä«¿îÆ®
			int sum = 0;									//! ÇÈ¼¿ ¹à±â°ªÀÇ ÇÕ
			int whiteVal = 0;								//! Æò±Õ ÇÈ¼¿ ¹à±â °ª º¯È¯

			for(y=inRect.top+1; y<inRect.bottom-1; y++)
			{
				iIsDispense=0;
				for(x=inRect.left+1; x<inRect.right-1; x++)
				{
					pos = y* CAM_SIZE_X + x;				//! ¹à±â °ªÀ» ¾òÀ» ÇÈ¼¿ÀÇ À§Ä¡ °ª
					if(m_Imagebuf[pos]>iColorLevel)
					{
						iIsDispense=1;
						break;
					}					
					else
					{
						point.x=x;
						point.y=y;
						vision.pixellist[iCh].addList(point, M_COLOR_MAGENTA);
					}
				}
				if(iIsDispense == 0)
				{
					iDispenseCnt++;
					if(iDispenseCnt>=iDispenseSpec)break;
				}
				else iDispenseCnt=0;
			}
			free(m_Imagebuf);
			if(iDispenseCnt >= iDispenseSpec)//-model.m_iResinInspRectSpec)
			{
				vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_RED);				// µµÆ÷ °Ë»ç ¿µ¿ª..
//				errMsg2(Task.AutoFlag, _T("µµÆ÷ ºÒ·® ¹ß°ß"));
				return false;
			}
			else
			{
				vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_GREEN);				// µµÆ÷ °Ë»ç ¿µ¿ª..
				return true;
			}
		}
		else	// °¡·Î¹æÇâ
		{  
			CRect inRect;
			inRect.left = (int)(centX + model.m_ResingRectStart[iRectCnt].x/sysData.dCamResol[iCh].x );
			inRect.top = (int)(centY + model.m_ResingRectStart[iRectCnt].y/sysData.dCamResol[iCh].y );
			inRect.right = (int)(centX + (model.m_ResingRectStart[iRectCnt].x+ model.m_ResingRectSize[iRectCnt].x)/sysData.dCamResol[iCh].x );
			inRect.bottom = (int)(centY + (model.m_ResingRectStart[iRectCnt].y+ model.m_ResingRectSize[iRectCnt].y)/sysData.dCamResol[iCh].y );

			//!! Image º¯È¯
			double HoleW=0, HoleH=0;
			double Length=0;
			unsigned char *m_Imagebuf;
			m_Imagebuf = (unsigned char *)malloc(CAM_SIZE_X*CAM_SIZE_Y);

			MbufGet(vision.MilGrabImageChild[iCh], m_Imagebuf);	// ÀÌ¹ÌÁö ÀüÃ¼ ÇÈ¼¿ ¹à±â°ª ¹öÆÛ ´ã±â

			//! °Ë»ç ¿µ¿ª³» ÇÈ¼¿ÀÇ ¹à±â °ª ±¸ÇÏ±â
			int iAllSumCnt = 0, iSumCnt = 0;				//! °Ë»ç ¿µ¿ª³»ÀÇ ¹à±â°ª ¾ò¾î¿Â ÇÈ¼¿ ÀüÃ¼ÀÇ Ä«¿îÆ®
			int sum = 0;									//! ÇÈ¼¿ ¹à±â°ªÀÇ ÇÕ
			int whiteVal = 0;								//! Æò±Õ ÇÈ¼¿ ¹à±â °ª º¯È¯

			for(x=inRect.left+1; x<inRect.right-1; x++)
			{
				iIsDispense=0;
				for(y=inRect.top+1; y<inRect.bottom-1; y++)
				{
					pos = y* CAM_SIZE_X + x;				//! ¹à±â °ªÀ» ¾òÀ» ÇÈ¼¿ÀÇ À§Ä¡ °ª
					if(m_Imagebuf[pos]>iColorLevel)
					{
						iIsDispense=1;
						//break;
					}					
					else
					{
						point.x=x;
						point.y=y;
						vision.pixellist[iCh].addList(point, M_COLOR_MAGENTA);
					}					//! ¹à±â °ª ¹öÆÛ¿¡ ÀúÀå
				}
				if(iIsDispense == 0)
				{
					iDispenseCnt++;
					//if(iDispenseCnt>=iDispenseSpec)break;
				}
				else iDispenseCnt=0;
			}
			free(m_Imagebuf);
			if(iDispenseCnt >= iDispenseSpec)//-model.m_iResinInspRectSpec)
			{
				vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_RED);				// µµÆ÷ °Ë»ç ¿µ¿ª..
//				errMsg2(Task.AutoFlag, _T("µµÆ÷ ºÒ·® ¹ß°ß"));
				return false;
			}
			else
			{
				vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_GREEN);				// µµÆ÷ °Ë»ç ¿µ¿ª..
				return true;
			}
		}



	}

	return true;
}


bool CAABonderDlg::_inspResignHole(bool autoMode, int index, int dispMode, int iCirCnt, unsigned char *m_Imagebuf)
{//KKYH 20150622 ¾Æ·¡ ÀÌÇÏ ÇÔ¼ö ¼öÁ¤
	int iCh = CAM2;

	if(index<0 || index>>3)
	{
		sLangChange.LoadStringA(IDS_STRING950);	//PCB Index °ªÀÌ ºñÁ¤»ó ÀÔ´Ï´Ù.
		errMsg2(Task.AutoFlag, sLangChange);
		return false;
	}

	Task.m_bResign_Result[index] = false;
	
	CPoint inRadius, outRadius;
	float inOffsetX		= (float)((model.m_ResinDrawSize.x*0.5) - model.m_dResinInspOffset[0].x);	//¾ÈÂÊ Offset
	float inOffsetY		= (float)((model.m_ResinDrawSize.y*0.5) - model.m_dResinInspOffset[0].y);
	float outOffsetX	= (float)((model.m_ResinDrawSize.x*0.5) + model.m_dResinInspOffset[1].x);	//¹Ù±ùÂÊ Offset
	float outOffsetY	= (float)((model.m_ResinDrawSize.y*0.5) + model.m_dResinInspOffset[1].y);
	inRadius.x	= (int)inOffsetX/ sysData.dCamResol[iCh].x;
	outRadius.x	= (int)outOffsetX/ sysData.dCamResol[iCh].x;		
	int i_limit_rate	= model.m_iResinInspLimit;

	int margine = 5;

	char	szDispData[256];
	CString sTemp;
	double ep1;

	ep1 = myTimer(true);

	//vision.clearOverlay();

	int x = 0;
	int	y = 0;
	int	pos = 0;
	int	pos2 = 0;
	int	minVal = 0;
	int	maxVal = 0;
	int	avgVal = 0;

	int sx = 0;
	int	sy = 0;
	int	ex = 0;
	int	ey = 0;

	double centX = 512 + model.m_CircleDrawCenter[iCirCnt].x;
	double centY = 384 + model.m_CircleDrawCenter[iCirCnt].y;
	//double centX = Task.d_mark_pos_x[PCB_Chip_MARK][0] + model.m_ResinDrawCenter[iCirCnt].x;
	//double centY = Task.d_mark_pos_y[PCB_Chip_MARK][0] + model.m_ResinDrawCenter[iCirCnt].y;
	//double dCenterX = centX + model.m_ResinDrawCenter[iCirCnt].x;
	//double dCenterY = centX + model.m_ResinDrawCenter[iCirCnt].y;
	CRect inRect, outRect;
	
	inRect.left		= (int)(centX - inOffsetX / sysData.dCamResol[iCh].x - 0.5);
	inRect.right	= (int)(centX + inOffsetX / sysData.dCamResol[iCh].x + 0.5);
	inRect.top		= (int)(centY - inOffsetY / sysData.dCamResol[iCh].y + 0.5);
	inRect.bottom	= (int)(centY + inOffsetY / sysData.dCamResol[iCh].y - 0.5);

	outRect.left	= (int)(centX - outOffsetX / sysData.dCamResol[iCh].x - 0.5);
	outRect.right	= (int)(centX + outOffsetX / sysData.dCamResol[iCh].x + 0.5);
	outRect.top		= (int)(centY - outOffsetY / sysData.dCamResol[iCh].y + 0.5);
	outRect.bottom	= (int)(centY + outOffsetY / sysData.dCamResol[iCh].y - 0.5);

	vision.crosslist[iCh].addList((int)centX, (int)centY, 30, M_COLOR_GREEN);	// µµÆ÷ °Ë»ç Áß½É À§Ä¡..
	vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_RED);				// µµÆ÷ °Ë»ç ¿µ¿ª..
	vision.boxlist[iCh].addList(outRect, PS_SOLID, M_COLOR_GREEN);


	int Hist[CAM_SIZE_X] = {0, };

	int inspStartPosX[CAM_SIZE_X]	= {0, };
	int inspEndPosX[CAM_SIZE_X]		= {0, };
	int inspStartPosY[CAM_SIZE_Y]	= {0, };
	int inspEndPosY[CAM_SIZE_Y]		= {0, };

	int startPoint[CAM_SIZE_X]		= {0, };
	int endPoint[CAM_SIZE_X]		= {0, };

	int leftVal = 0;
	int leftPos = 0;
	int rightVal = 0;
	int rightPos = 0;
	int topVal = 0;
	int topPos = 0;
	int bottomVal = 0;
	int bottomPos = 0;

	//////////////////////////////////////////////////////////////////////////////////
	// ¿ø ¿µ¿ª °Ë»ç Start..
	sx = (int)outRect.left;
	ex = (int)outRect.right;
	sy = (int)outRect.top;
	ey = (int)outRect.bottom;

	if (sx<0)					sx = 0;
	if (ex>=CAM_SIZE_X)			ex = CAM_SIZE_X - 1;
	if (sy<0)					sy = 0;
	if (ey>=CAM_SIZE_Y)			ey = CAM_SIZE_Y - 1;

	if( (outOffsetX-inOffsetX) / sysData.dCamResol[iCh].x < 10)
	{
		sprintf_s(szDispData, "[ ERROR ] Insp ROI size X Error. (%f~%f)", inOffsetX, outOffsetX);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, "Arial");
		putListLog("[µµÆ÷ °Ë»ç] °Ë»ç ¿µ¿ª °¡·Î Æø ÀÌ»ó");
		return false;
	}
	if((outOffsetY-inOffsetY) / sysData.dCamResol[iCh].y < 10)
	{
		sprintf_s(szDispData, "[ ERROR ] Insp ROI size Y Error. (%f~%f)", outOffsetY, inOffsetY);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, "Arial");

		putListLog("[µµÆ÷ °Ë»ç] °Ë»ç ¿µ¿ª ¼¼·Î Æø ÀÌ»ó");
		return false;
	}
	

	//!! HoleÀ» Á¦¿ÜÇÑ ³ª¸ÓÁö´Â ¸ðµÎ 0x00À¸·Î Image º¯È¯
	double HoleW=0, HoleH=0;
	double Length=0;

	//!¿ÜºÎ¿µ¿ª ±âÁØÀ¸·Î Buf¿¡ ´ãÀ½.
	CPoint cpCutSize;
	cpCutSize.x = ex-sx;
	cpCutSize.y = ey-sy;

	unsigned char	*cutimgBuf;
	cutimgBuf		= (unsigned char *)malloc(cpCutSize.x * cpCutSize.y);
	memset(cutimgBuf, 0, sizeof(cutimgBuf));

	//! ¿ÜºÎ¿µ¿ª¹öÆÛ¸¦ cv¿¡ ´ãÀ½.
	IplImage *bufImg, *dstImg, *srcImg, *polarImg;
	
	srcImg = cvCreateImage(cvSize(cpCutSize.x, cpCutSize.y),8,1);
	
	dstImg = cvCreateImage(cvSize(cpCutSize.x, cpCutSize.y),8,1);
	bufImg = cvCreateImage(cvSize(CAM_SIZE_X, CAM_SIZE_Y),8,1);
	memcpy(bufImg->imageData, m_Imagebuf, CAM_SIZE_X * CAM_SIZE_Y);

	cvSetImageROI(bufImg, cvRect(sx, sy, cpCutSize.x, cpCutSize.y));
	cvCopyImage(bufImg, srcImg);
	CString fileCirName;
	fileCirName.Format(_T("D:/1_cvPolarSrc_%d.bmp"), iCirCnt);
	cvSaveImage(fileCirName, srcImg);	//===============
	
	
	//! cv Image Polar Ã³¸®
	double M = srcImg->width/log(srcImg->width/2.0);
	//cvLogPolar(srcImg, dstImg, cvPoint2D32f(srcImg->width/2, srcImg->height/2), M, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);
	cvLogPolar(srcImg, dstImg, cvPoint2D32f(srcImg->width / 2, srcImg->height / 2), M, CV_WARP_INVERSE_MAP + CV_WARP_FILL_OUTLIERS);
	fileCirName.Format(_T("D:/2_cvPolardst_%d.bmp"), iCirCnt);
	cvSaveImage(fileCirName, dstImg);	//===============

	//!°í¸® ¿µ¿ª¸¸ ¹öÆÛ¿¡ ´Ù½Ã ´ã±â -> °í¸® Å©±â·Î..
	int iEpoxySize = (inRect.left - outRect.left) * 0.32;//°í¸® Å©±â ¼öÁ¤ 0.32
	int iBuffSize=0;
	unsigned char	*imgInsp;
	int iSizeY=0;
	int my = cpCutSize.y/4;
	if(iCirCnt < 1)
	{
		iSizeY = cpCutSize.y/4; //iSizeY=(iCirCnt < 2)?(cpCutSize.y/4):((cpCutSize.y/4)-10);  ¿ø·¡ ÀÖ´ø if¹®
	}else
	{
		iSizeY = (cpCutSize.y/4)-10;
	}
	
	iBuffSize = iEpoxySize * iSizeY;
	imgInsp		= (unsigned char *)malloc(iBuffSize);

	sx = cpCutSize.x-iEpoxySize;
	
	polarImg = cvCreateImage(cvSize(iEpoxySize, iSizeY),8,1);

	if(iCirCnt == 0)		{cvSetImageROI(dstImg, cvRect(sx, 0+1, iEpoxySize, iSizeY));}
	else if(iCirCnt == 1)	{cvSetImageROI(dstImg, cvRect(sx, my+8, iEpoxySize, iSizeY));}
	else if(iCirCnt == 2)	{cvSetImageROI(dstImg, cvRect(sx, my*3+5, iEpoxySize, iSizeY));}
	else if(iCirCnt == 3)	{cvSetImageROI(dstImg, cvRect(sx, my*2+10, iEpoxySize, iSizeY));}
	
	cvCopyImage(dstImg, polarImg);
	fileCirName.Format(_T("D:/3_cvPolarInsp_%d.bmp"), iCirCnt);
	cvSaveImage(fileCirName, polarImg);

	//!¹öÆÛ¿¡ ´Ù½Ã ´ã±â
	memcpy(imgInsp, polarImg->imageData , iBuffSize);
	//! IplImage ¸Þ¸ð¸® ÇØÁ¦

	cvReleaseImage( &polarImg );
	cvReleaseImage( &dstImg );
	cvReleaseImage( &srcImg );
	
	cvResetImageROI(bufImg);
	cvReleaseImage( &bufImg );
	///-----------------------------------------------------------------------------------------------------------------------------
	CPoint inspSize;
	inspSize.x = iEpoxySize;
	inspSize.y = iSizeY;//cpCutSize.y;

	minVal = 255;
	maxVal = 0;
	avgVal = 0;
	memset(Hist, 0x00, sizeof(int)*(CAM_SIZE_X));

	int sum = 0;
	int iSpecOverLine = 0;
	int iContinuityLine = 0;
	int iCutErrCount=0;
	double iCutLength=0.0;
	for(y=0; y<inspSize.y; y++)
	{
		sum = 0;
		for(x=0; x<inspSize.x; x++)
		{
			pos = y * inspSize.x + x;
			sum += imgInsp[pos];
		}
		avgVal += sum;
		Hist[x] = sum;
		if(sum / inspSize.x < 255.0 * (model.m_dResinInspHoleSpec/100.0) )//½ºÆåº¸´Ù ÀÛÀ¸¸é
		{
			iContinuityLine++;

			if(iContinuityLine*sysData.dCamResol[iCh].x > model.m_dResinInspGapLength)//! 5È¸ ¿¬¼ÓÀ¸·Î ³ª¿Ã °æ¿ì.
			{
				sTempLang.LoadStringA(IDS_STRING134);
				sLangChange.Format(sTempLang, y);
				_stprintf_s(szDispData, sLangChange);
				vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
				return false;
			}
		}
		iCutErrCount++;
		if(sum / inspSize.x > 255.0*(model.m_dResinInspHoleSpec/100.0) )		iSpecOverLine++;
	}
	//
	free(cutimgBuf);
	free(imgInsp);
	return true;
}

bool CAABonderDlg::_inspResignRect(bool autoMode, int index, int iDirection, int iRectCnt, unsigned char *m_Imagebuf)
{//KKYH 20150622 ¾Æ·¡ ÀÌÇÏ ÇÔ¼ö Ãß°¡
	int iCh = index;

	if( Task.m_iStatus_Unit_Epoxy == 1)
	{
		saveInspImage(EPOXY_IMAGE_SAVE, index);
	}

	if(index<0 || index>>3)
	{
		sLangChange.LoadStringA(IDS_STRING950);	//PCB Index °ªÀÌ ºñÁ¤»ó ÀÔ´Ï´Ù.
		errMsg2(Task.AutoFlag, sLangChange);
		return false;
	}

	double centX = Task.d_mark_pos_x[PCB_Chip_MARK][0];	//PCB_Holder_MARK
	double centY = Task.d_mark_pos_y[PCB_Chip_MARK][0];
	//CAM_SIZE_X/2, CAM_SIZE_Y/2
	int margine = 3;
//	char	szDispData[256];
	CString sTemp;
	double ep1;

	ep1 = myTimer(true);
	int x = 0;
	int	y = 0;
	int	pos = 0;
	int	pos2 = 0;
	int	minVal = 0;
	int	maxVal = 0;
	int	avgVal = 0;

	int iColorLevel=model.m_ResingInspLevel;
	int iDispenseCnt=0;
	int iIsDispense=0;
	int iDispenseSpec=10;

	
	int Ewidth = 0;
	int Eheight = 0;
	bool pass = true;
	if (centX == 0 )
	{
		centX = 1024.0 / 2;
	}
	if (centY == 0)
	{
		centY = 768.0 / 2;
	}
		CRect inRect;
		/*inRect.left = (int)(centX + model.m_ResingRectStart[iRectCnt].x/sysData.dCamResol[iCh].x );
		inRect.top = (int)(centY + model.m_ResingRectStart[iRectCnt].y/sysData.dCamResol[iCh].y );
		inRect.right = (int)(centX + (model.m_ResingRectStart[iRectCnt].x+ model.m_ResingRectSize[iRectCnt].x)/sysData.dCamResol[iCh].x );
		inRect.bottom = (int)(centY + (model.m_ResingRectStart[iRectCnt].y+ model.m_ResingRectSize[iRectCnt].y)/sysData.dCamResol[iCh].y );*/

		
		

		inRect.left = (int)(centX + model.m_ResingRectStart[iRectCnt].x);
		inRect.top = (int)(centY + model.m_ResingRectStart[iRectCnt].y);
		inRect.right = (int)(centX + (model.m_ResingRectStart[iRectCnt].x+ model.m_ResingRectSize[iRectCnt].x));
		inRect.bottom = (int)(centY + (model.m_ResingRectStart[iRectCnt].y+ model.m_ResingRectSize[iRectCnt].y));
		Ewidth = inRect.right - inRect.left;
		Eheight = inRect.bottom - inRect.top;
			   
		iDispenseSpec = (model.m_iResinInspRectSpec *  Ewidth) / 100;//±æÀÌ ´ëºñ ¹éºÐÀ²

		//! °Ë»ç ¿µ¿ª³» ÇÈ¼¿ÀÇ ¹à±â °ª ±¸ÇÏ±â
		int iAllSumCnt = 0, iSumCnt = 0;				//! °Ë»ç ¿µ¿ª³»ÀÇ ¹à±â°ª ¾ò¾î¿Â ÇÈ¼¿ ÀüÃ¼ÀÇ Ä«¿îÆ®
		int sum = 0;									//! ÇÈ¼¿ ¹à±â°ªÀÇ ÇÕ
		int whiteVal = 0;								//! Æò±Õ ÇÈ¼¿ ¹à±â °ª º¯È¯

		CPoint point;
		if(iDirection == 0)	// ¼¼·Î¹æÇâ
		{
			for(y=inRect.top+1; y<inRect.bottom-1; y++)
			{
				iIsDispense=0;
				iDispenseCnt=0;
				for(x=inRect.left+1; x<inRect.right-1; x++)
				{
					pos = y* CAM_SIZE_X + x;				//! ¹à±â °ªÀ» ¾òÀ» ÇÈ¼¿ÀÇ À§Ä¡ °ª
					if(m_Imagebuf[pos]>model.m_ResingInspLevel)
					{
						iDispenseCnt++;
					}					
					else
					{
						point.x=x;
						point.y=y;
						vision.pixellist[iCh].addList(point, M_COLOR_MAGENTA);
					}					//! ¹à±â °ª ¹öÆÛ¿¡ ÀúÀå
				}
				if(iDispenseCnt < iDispenseSpec)//°¡·Î·Î ¹ÌµµÆ÷¿µ¿ªÀÌ Æò±ÕÆÛ¼¾Æ®Áö¿¡ ¸ø ¹ÌÄ¥¶§
				{
					pass = false;
					
				}

			}
		}else// °¡·Î¹æÇâ
		{
			inRect.left = (int)(centX + model.m_ResingRectStart[iRectCnt].x);
			inRect.top = (int)(centY + model.m_ResingRectStart[iRectCnt].y);
			inRect.right = (int)(centX + (model.m_ResingRectStart[iRectCnt].x+ model.m_ResingRectSize[iRectCnt].x));
			inRect.bottom = (int)(centY + (model.m_ResingRectStart[iRectCnt].y+ model.m_ResingRectSize[iRectCnt].y));
			Ewidth = inRect.right - inRect.left;
			Eheight = inRect.bottom - inRect.top;
			   
		iDispenseSpec = (model.m_iResinInspRectSpec *  Eheight) / 100;//±æÀÌ ´ëºñ ¹éºÐÀ²
			for(x=inRect.left+1; x<inRect.right-1; x++)
			{
				iIsDispense=0;
				iDispenseCnt=0;
				for(y=inRect.top+1; y<inRect.bottom-1; y++)
				{
					pos = y* CAM_SIZE_X + x;				//! ¹à±â °ªÀ» ¾òÀ» ÇÈ¼¿ÀÇ À§Ä¡ °ª
					if(m_Imagebuf[pos]>model.m_ResingInspLevel)
					{
						iDispenseCnt++;
					}					
					else
					{
						point.x=x;
						point.y=y;
						vision.pixellist[iCh].addList(point, M_COLOR_MAGENTA);
					}					//! ¹à±â °ª ¹öÆÛ¿¡ ÀúÀå
				}
				if(iDispenseCnt < iDispenseSpec)//°¡·Î·Î ¹ÌµµÆ÷¿µ¿ªÀÌ Æò±ÕÆÛ¼¾Æ®Áö¿¡ ¸ø ¹ÌÄ¥¶§
				{
					pass = false;
				}

			}
		}
		if(pass)
		{
			vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_GREEN);				// µµÆ÷ °Ë»ç ¿µ¿ª..
			return true;
		}else
		{
			vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_RED);				// µµÆ÷ °Ë»ç ¿µ¿ª..
			return false;
		}
	
	return true;
}

bool CAABonderDlg::_inspResignHole(bool autoMode, int index, int iRectCnt,unsigned char *m_Imagebuf)
{//KKYH 20150622 ¾Æ·¡ ÀÌÇÏ ÇÔ¼ö ¼öÁ¤
	int iCh = CAM2;

	if( Task.m_iStatus_Unit_Epoxy == 1)
	{
		saveInspImage(EPOXY_IMAGE_SAVE, index);
	}

	if(index<0 || index>3)
	{
		sLangChange.LoadStringA(IDS_STRING950);	//PCB Index °ªÀÌ ºñÁ¤»ó ÀÔ´Ï´Ù.
		errMsg2(Task.AutoFlag, sLangChange);
		return false;
	}


	Task.m_bResign_Result[index] = false;

	CPoint inRadius, outRadius;
	float inOffsetX		= (float)((model.m_ResinDrawSize.x*0.5) - model.m_dResinInspOffset[0].x);	//¾ÈÂÊ Offset
	float inOffsetY		= (float)((model.m_ResinDrawSize.y*0.5) - model.m_dResinInspOffset[0].y);
	float outOffsetX	= (float)((model.m_ResinDrawSize.x*0.5) + model.m_dResinInspOffset[1].x);	//¹Ù±ùÂÊ Offset
	float outOffsetY	= (float)((model.m_ResinDrawSize.y*0.5) + model.m_dResinInspOffset[1].y);
	inRadius.x	= (int)(inOffsetX/ sysData.dCamResol[iCh].x);
	outRadius.x	= (int)(outOffsetX/ sysData.dCamResol[iCh].x);		
	int i_limit_rate	= model.m_iResinInspLimit;

	int margine = 5;

	char	szDispData[256];
	CString sTemp;
	double ep1;

	ep1 = myTimer(true);

	int x = 0;
	int	y = 0;
	int	pos = 0;
	int	pos2 = 0;
	int	minVal = 0;
	int	maxVal = 0;
	int	avgVal = 0;

	int sx = 0;
	int	sy = 0;
	int	ex = 0;
	int	ey = 0;

	double centX = 512 + model.m_CircleDrawCenter[iRectCnt].x + Task.d_Align_offset_x[PCB_Holder_MARK];
	double centY = 384 + model.m_CircleDrawCenter[iRectCnt].y + Task.d_Align_offset_y[PCB_Holder_MARK];

	CRect inRect, outRect;
	inRect.left		= (int)(centX - inOffsetX / sysData.dCamResol[iCh].x - 0.5);
	inRect.right	= (int)(centX + inOffsetX / sysData.dCamResol[iCh].x + 0.5);
	inRect.top		= (int)(centY - inOffsetY / sysData.dCamResol[iCh].y + 0.5);
	inRect.bottom	= (int)(centY + inOffsetY / sysData.dCamResol[iCh].y - 0.5);

	outRect.left	= (int)(centX - outOffsetX / sysData.dCamResol[iCh].x - 0.5);
	outRect.right	= (int)(centX + outOffsetX / sysData.dCamResol[iCh].x + 0.5);
	outRect.top		= (int)(centY - outOffsetY / sysData.dCamResol[iCh].y + 0.5);
	outRect.bottom	= (int)(centY + outOffsetY / sysData.dCamResol[iCh].y - 0.5);

	vision.crosslist[iCh].addList((int)centX, (int)centY, 30, M_COLOR_GREEN);	// µµÆ÷ °Ë»ç Áß½É À§Ä¡..
	vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_RED);				// µµÆ÷ °Ë»ç ¿µ¿ª..
	vision.boxlist[iCh].addList(outRect, PS_SOLID, M_COLOR_GREEN);

	int inspStartPosX[CAM_SIZE_X]	= {0, };
	int inspEndPosX[CAM_SIZE_X]		= {0, };
	int inspStartPosY[CAM_SIZE_Y]	= {0, };
	int inspEndPosY[CAM_SIZE_Y]		= {0, };

	int startPoint[CAM_SIZE_X]		= {0, };
	int endPoint[CAM_SIZE_X]		= {0, };

	int leftVal = 0;
	int leftPos = 0;
	int rightVal = 0;
	int rightPos = 0;
	int topVal = 0;
	int topPos = 0;
	int bottomVal = 0;
	int bottomPos = 0;

	//////////////////////////////////////////////////////////////////////////////////
	// ¿ø ¿µ¿ª °Ë»ç Start..
	sx = (int)outRect.left;
	ex = (int)outRect.right;
	sy = (int)outRect.top;
	ey = (int)outRect.bottom;

	if (sx<0)					sx = 0;
	if (ex>=CAM_SIZE_X)			ex = CAM_SIZE_X - 1;
	if (sy<0)					sy = 0;
	if (ey>=CAM_SIZE_Y)			ey = CAM_SIZE_Y - 1;

	if( (outOffsetX-inOffsetX) / sysData.dCamResol[iCh].x < 10)
	{
		sTempLang.LoadStringA(IDS_STRING132);
		sLangChange.Format(sTempLang, inOffsetX, outOffsetX);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
		sLangChange.LoadStringA(IDS_STRING240);
		putListLog(sLangChange);
		return false;
	}
	if((outOffsetY-inOffsetY) / sysData.dCamResol[iCh].y < 10)
	{
		sTempLang.LoadStringA(IDS_STRING133);
		sLangChange.Format(sTempLang, outOffsetY, inOffsetY);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
		sLangChange.LoadStringA(IDS_STRING241);
		putListLog(sLangChange);
		return false;
	}


	int iSpecOverLine = 0;
	int iCnt=0;

	{
		float sa=model.m_ResingStartAngle[iRectCnt];
		float ea=model.m_ResingFinishAngle[iRectCnt];
		float r=outRadius.x;
		float r2=inRadius.x;
		float sx,sy,ex,ey;
		float step=2*PI/(r*PI*1.5);


		CPoint point;

		if(ea<sa)
		{
			for(float theta=(sa/360)*(2*PI);theta<((360/360)*(2*PI));theta+=step)
			{
				sx=centX + r*sin(theta);
				sy=centY + r*cos(theta);

				ex=centX + r2*sin(theta);
				ey=centY + r2*cos(theta);
				if(!_inspLine(sx,sy,ex,ey,m_Imagebuf))
				{
					iSpecOverLine++;

					point.x=sx;
					point.y=sy;
					vision.pixellist[CAM2].addList(point, M_COLOR_RED);
				}

			}
			for(float theta=(0/360)*(2*PI);theta<((ea/360)*(2*PI));theta+=step)
			{
				sx=centX + r*sin(theta);
				sy=centY + r*cos(theta);

				ex=centX + r2*sin(theta);
				ey=centY + r2*cos(theta);
				if(!_inspLine(sx,sy,ex,ey,m_Imagebuf))
				{
					iSpecOverLine++;

					point.x=sx;
					point.y=sy;
					vision.pixellist[CAM2].addList(point, M_COLOR_RED);
				}
			}
		}
		else
		{
			for(float theta=(sa/360)*(2*PI);theta<((ea/360)*(2*PI));theta+=step)
			{
				sx=centX + r*sin(theta);
				sy=centY + r*cos(theta);

				ex=centX + r2*sin(theta);
				ey=centY + r2*cos(theta);

				if(!_inspLine(sx,sy,ex,ey,m_Imagebuf))
				{
					iSpecOverLine++;

					point.x=sx;
					point.y=sy;
					vision.pixellist[CAM2].addList(point, M_COLOR_RED);
				}
			}
		}
	}
	return iSpecOverLine<1;
}

bool CAABonderDlg::_inspLine(int sx, int sy, int ex, int ey, unsigned char *m_Imagebuf)
{
	int w=abs(ex-sx);
	int h=abs(ey-sy);

	int x,y;
	int dx=(ex>sx?1:-1);
	int dy=(ey>sy?1:-1);

	int iColorLevel=model.m_ResingInspLevel;
	int iDispenseCnt=0;
	int iIsDispense=0;
	int iDispenseSpec=10;
	int pos;
	CPoint point;

	point.x=sx;
	point.y=sy;
	vision.pixellist[CAM2].addList(point, M_COLOR_CYAN);

	point.x=ex;
	point.y=ey;
	vision.pixellist[CAM2].addList(point, M_COLOR_BLUE);
	 
	bool bSetPoint=true;
	
	if(w>=h)
	{
		for(int i=0;i<w;i++)
		{
			x=sx+i*dx;
			y=sy+i*((float)h/(float)w)*dy;

			pos = y* CAM_SIZE_X + x;				//! ¹à±â °ªÀ» ¾òÀ» ÇÈ¼¿ÀÇ À§Ä¡ °ª
			if(m_Imagebuf[pos]>iColorLevel)
			{
				iDispenseCnt++;
			}
			else
			{
				point.x=x;
				point.y=y;
				if(!bSetPoint)
				{
					vision.pixellist[CAM2].addList(point, M_COLOR_MAGENTA);
					bSetPoint=true;
				}
				m_Imagebuf[pos]=0;
			}
			//			m_Imagebuf[pos]=0;
		}
	}
	else
	{
		for(int i=0;i<h;i++)
		{
			x=sx+i*((float)w/(float)h)*dx;
			y=sy+i*dy;
			pos = y* CAM_SIZE_X + x;				//! ¹à±â °ªÀ» ¾òÀ» ÇÈ¼¿ÀÇ À§Ä¡ °ª
			if(m_Imagebuf[pos]>iColorLevel)
			{
				iDispenseCnt++;
			}
			else
			{
				point.x=x;
				point.y=y;
				if(!bSetPoint)
				{
					vision.pixellist[CAM2].addList(point, M_COLOR_MAGENTA);
					bSetPoint=true;
				}
				m_Imagebuf[pos]=0;
			}
			//			m_Imagebuf[pos]=0;
		}
	}

	return iDispenseCnt>=iDispenseSpec;
}


void CAABonderDlg::OnBnClickedButtonMinimize()
{
	SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
}

void CAABonderDlg::func_Set_SFR_N_Type()
{
		dSFR_N_4_PositionX = model.m_Line_Pulse;
		dSFR_N_8_PositionX = model.m_Line_Pulse;
}


void CAABonderDlg::func_Socket_Barcode()
{
	if(bThreadSocket == true)	return;
	//! Data ÃÊ±âÈ­
	Task.iRecvLenACK[0] = Task.iRecvLenACK[1] = 0;
	Task.dTiltingManual[0] = Task.dTiltingManual[1] = 0.0;
	Task.dAlignManual[0] = Task.dAlignManual[0] = Task.dAlignManual[0] = 0.0;
	Task.iRecvLenCnt[0] = Task.iRecvLenCnt[1] = Task.iRecvLenCnt[2] = Task.iRecvLenCnt[3] = Task.iRecvLenCnt[4] = -1;

	pThread_SocketRead = ::AfxBeginThread(Thread_Socket_ReadingData, this);
}

void CAABonderDlg::OnBnClickedButtonTimeCheck()
{		
	CString str, strTime;
	GetDlgItem(IDC_BUTTON_TIME_CHECK)->GetWindowText(str);

	sLangChange.LoadStringA(IDS_STRING538); //EPOXY TIME START
	if(str == sLangChange)
	{
		sLangChange.LoadStringA(IDS_STRING1269);	//»ý»êÀ» ½ÃÀÛÇÏ½Ã°Ú½À´Ï±î?
		if(askMsg(sLangChange))
		{
			today = CTime::GetCurrentTime();

			work.m_Epoxy_Time_Flag = 1;
			work.m_Epoxy_Time_Check_Year = today.GetYear();
			work.m_Epoxy_Time_Check_Month = today.GetMonth();
			work.m_Epoxy_Time_Check_Day = today.GetDay();
			work.m_Epoxy_Time_Check_Hour = today.GetHour();
			work.m_Epoxy_Time_Check_Min = today.GetMinute();
			work.Save();

			sLangChange.LoadStringA(IDS_STRING1451);
			GetDlgItem(IDC_BUTTON_TIME_CHECK)->SetWindowText(sLangChange);
			m_EpoxyTimeCheck.m_iStateBtn = 1;
		}
	}
	else
	{
		sLangChange.LoadStringA(IDS_STRING1270);
		if(askMsg(sLangChange))	//"»ý»êÀ» Á¾·á ÇÏ½Ã°Ú½À´Ï±î?"
		{
			work.m_Epoxy_Time_Flag = 0;

			work.m_Epoxy_Time_Check_Year = 0;
			work.m_Epoxy_Time_Check_Month = 0;
			work.m_Epoxy_Time_Check_Day = 0;
			work.m_Epoxy_Time_Check_Hour = 0;
			work.m_Epoxy_Time_Check_Min = 0;

			work.Save();
			sLangChange.LoadStringA(IDS_STRING538);
			GetDlgItem(IDC_BUTTON_TIME_CHECK)->SetWindowText(sLangChange);
			m_EpoxyTimeCheck.m_iStateBtn = 0;
		}
	}

	m_EpoxyTimeCheck.Invalidate();
}


bool CAABonderDlg::EpoxyTimeCheck(bool TimeFlag)
{
	// 20141119 LHC - ÀÚµ¿¿îÀü Àü¿¡ ¿¡Æø½Ã ±³Ã¼ ½Ã°£ Ã¼Å©ÇÏ±â
	CString str;
	int ResultTimeHour, ResultTimeMin;

	if(work.m_Epoxy_Time_Flag == 1)
	{
		CTime today2;
		CTime StartTime(work.m_Epoxy_Time_Check_Year, work.m_Epoxy_Time_Check_Month, work.m_Epoxy_Time_Check_Day, work.m_Epoxy_Time_Check_Hour, work.m_Epoxy_Time_Check_Min, 0);
		CTimeSpan ChkTime;
		today2 = CTime::GetCurrentTime();
		if(sysData.m_Epoxy_Change_Count < 60)
		{
			ResultTimeHour = 0;
			ResultTimeMin = sysData.m_Epoxy_Change_Count;
		}
		else
		{
			ResultTimeHour = sysData.m_Epoxy_Change_Count / 60;
			ResultTimeMin = sysData.m_Epoxy_Change_Count % 60;
		}
		
		today2 -= CTimeSpan(0, ResultTimeHour, ResultTimeMin,0);

		ChkTime = StartTime - today2;

		if(ChkTime <= 0)
		{
			Dio.setAlarm(ALARM_ON);
			sLangChange.LoadStringA(IDS_STRING540);	//Epoxy ±³Ã¼ÇÑÁö %dºÐÀÌ Áö³µ½À´Ï´Ù\n ±³Ã¼ ¿Ï·á ÈÄ YES ¹öÆ°À» ´­·¯ÁÖ¼¼¿ä.\n [YES : ±³Ã¼ ¿Ï·á NO : ±³Ã¼ ¹Ì¿Ï·á]
			str.Format(sLangChange,sysData.m_Epoxy_Change_Count);
		
			if(askMsg(str))
			{
				today = CTime::GetCurrentTime();
				work.m_Epoxy_Time_Flag = 1;
				work.m_Epoxy_Time_Check_Year = today.GetYear();
				work.m_Epoxy_Time_Check_Month = today.GetMonth();
				work.m_Epoxy_Time_Check_Day = today.GetDay();
				work.m_Epoxy_Time_Check_Hour = today.GetHour();
				work.m_Epoxy_Time_Check_Min = today.GetMinute();
				work.Save();
				Dio.setAlarm(ALARM_OFF);
			}
			else
			{			
				sLangChange.LoadStringA(IDS_STRING538);
				GetDlgItem(IDC_BUTTON_TIME_CHECK)->SetWindowText(sLangChange);
				work.m_Epoxy_Time_Flag = 0;
				work.m_Epoxy_Time_Check_Year = 0;
				work.m_Epoxy_Time_Check_Month = 0;
				work.m_Epoxy_Time_Check_Day = 0;
				work.m_Epoxy_Time_Check_Hour = 0;
				work.m_Epoxy_Time_Check_Min = 0;
				m_EpoxyTimeCheck.m_iStateBtn = 0;
				m_EpoxyTimeCheck.Invalidate();
				work.Save();
				Dio.setAlarm(ALARM_OFF);
				return false;
			}
		}
	}

	return true;
}


void CAABonderDlg::OnStnClickedLabelLotName()
{
	CKeyPadDlg keyDlg;
	CString strTemp;
	sLangChange.LoadStringA(IDS_STRING757);	//Lot¸íÀ» ÀÔ·ÂÇÏ¼¼¿ä.
	keyDlg.m_strKeyPad = sLangChange;

	if (keyDlg.DoModal()==IDOK)
	{
		strTemp = keyDlg.GetstrKeypad();
		int strLength = strTemp.GetLength();
		if(strLength<1)
		{
			sLangChange.LoadStringA(IDS_STRING758);	//Lot¸íÀÌ ºñÁ¤»óÀÔ´Ï´Ù.
			errMsg2(Task.AutoFlag, sLangChange);
			return;
		}
		sprintf_s(Task.LotNo, strTemp, sizeof(strTemp) );
		sprintf_s(Task.sNum, strTemp, sizeof(strTemp) );

		m_labelLotName.SetText(strTemp);
	}
	else
	{
		return;
	}
}

//141201 ¹Ú¼®Çö [SocketCommunication] Added
void CAABonderDlg::OnBnClickedServerStart()
{
	//func_HidataSocket_ServerStart(8000);
}

void CAABonderDlg::OnBnClickedServerStop()
{
	//func_HidataSocket_ServerEnd();
}


void CAABonderDlg::OnBnClickedClientConnect()
{
	MESConnectToServer();
	/*
	CString sLog;
	if(m_SocketMes.Connect(sysData.m_Mes_Ip_Number, sysData.m_Mes_Port_Number) == FALSE)
	{
		sLangChange.LoadStringA(IDS_STRING771);	//"MES¿Í ¿¬°á ½ÇÆÐ"
		sLog.Format(sLangChange);
		putListLog(sLog);
	}
	else
	{
		sLangChange.LoadStringA(IDS_STRING770);	//"MES¿Í ¿¬°á ¼º°ø"
		sLog.Format(sLangChange);
		putListLog(sLog);
	}*/
}


void CAABonderDlg::OnBnClickedClientDisconnect()
{
	//func_HidataSocket_ClientDisConnect();
}


void CAABonderDlg::OnBnClickedClientSend()
{
	//func_HiSocket_ClientSend("T*");
}

void CAABonderDlg::MESConnectToServer()
{
	CString sLog;
	if (m_SocketMes != NULL)
	{
		m_SocketMes.ShutDown();
		Sleep(1000);
		m_SocketMes.Close();
	}
	if(!m_SocketMes.Create())
	{
		sLangChange.LoadStringA(IDS_STRING1085);	//Socket »ý¼º ½ÇÆÐ
		sLog.Format(sLangChange);
		putListLog(sLog);
		return;
	}
	if(sysData.m_iProductComp == 1)
	{
		if(m_SocketMes.Connect(sysData.m_Mes_Ip_Number, sysData.m_Mes_Comp_Port_Number) == FALSE)
		{
			sLog.Format("MES ¿¬°á ½ÇÆÐ. MES»óÅÂ,IP, PORT È®ÀÎ ÇØ ÁÖ¼¼¿ä.");
			putListLog(sLog);
			m_labelMes.SetBkColor(M_COLOR_RED);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING770);	//"MES¿Í ¿¬°á ¼º°ø"
			sLog.Format(sLangChange);
			putListLog(sLog);
			m_labelMes.SetBkColor(M_COLOR_GREEN);
		}
		m_labelMes.Invalidate();
	}
	else
	{
		if(m_SocketMes.Connect(sysData.m_Mes_Ip_Number, sysData.m_Mes_Port_Number) == FALSE)
		{
			//sLangChange.LoadStringA(IDS_STRING772);	//MES¿Í ¿¬°á ½ÇÆÐ. MES»óÅÂ ¹× IP, PORT ¹øÈ£¸¦ È®ÀÎ ÇØ ÁÖ¼¼¿ä.
			sLog.Format("MES ¿¬°á ½ÇÆÐ. MES»óÅÂ,IP, PORT È®ÀÎ ÇØ ÁÖ¼¼¿ä.");
			putListLog(sLog);
			m_labelMes.SetBkColor(M_COLOR_RED);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING770);	//"MES¿Í ¿¬°á ¼º°ø"
			sLog.Format(sLangChange);
			putListLog(sLog);
			m_labelMes.SetBkColor(M_COLOR_GREEN);
			
		}
		m_labelMes.Invalidate();
	}

}


int CAABonderDlg::RunProc_ProductLoading(int iUseStep)
{//! ÀÛ¾÷ÀÚ Á¦Ç° ÅõÀÔ ÀÚµ¿ Step		(10000 ~ 10999)
	int iRtnFunction = iUseStep;
	CString logStr = "", sBarCode="";
	bool bChk = true;

	switch (iUseStep)
	{
	case 10000://! DoorÄ¿Æ° Lift Open.
		Task.interlockLens = Task.interlockPcb = 0;
		iRtnFunction = 10010;
		break;
	case 10010:
		iRtnFunction = 10030;
		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
		break;
	case 10030:
		iRtnFunction = 10050;
		break;
	case 10050:
		//LightControl.ctrlLedVolume(LIGHT_OC_6500K, 0);		// Align¸¸ Á¶¸í ON
		Dio.LaserCylinderUPDOWN(true, false);

		logStr.Format("LASER ½Ç¸°´õ »ó½Â ¿Ï·á[%d]", iRtnFunction);
		theApp.MainDlg->putListLog(logStr);
		Task.PCBTaskTime = myTimer(true);

		iRtnFunction = 10060;
		break;

	case 10060:
		if (Dio.LaserCylinderCheck(true, false))
		{
			logStr.Format("LASER ½Ç¸°´õ »ó½Â È®ÀÎ ¿Ï·á[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 10100;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER ½Ç¸°´õ »ó½Â ½Ã°£ ÃÊ°ú..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -10060;
		}
		break;

	case 10100://! Chip Á¤º¸ Draw
		func_ChipID_Draw();

		iRtnFunction = 10110;

		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
		logStr.Format("		Start ¹öÆ°À» ´­·¯ÁÖ¼¼¿ä[%d]", iRtnFunction);
		putListLog(logStr);
		m_btnStart.m_iStateBtn = 4;
		m_btnStart.Invalidate();
		break;
	case 10110://! Start P/B Button Push ´ë±â. 
		if (Dio.StartPBOnCheck(true, false))// || Start_Btn_On)	//ÇÁ·Î±×·¥¿¡ start ¹öÆ° Ãß°¡
		{
			iRtnFunction = 10165;
			MbufClear(vision.MilOptImage, 0);
			m_btnStart.m_iStateBtn = 0;
			m_btnStart.Invalidate();
			Start_Btn_On = false;
			if (Task.m_bOKLensPass == 1)   //Lens ³Ñ±è ¿Ï·á»óÅÂ
			{
				if (!askMsg("·»Áî ³Ñ±è»óÅÂÀÔ´Ï´Ù. \n°è¼Ó ÁøÇàÇÏ½Ã°Ú½À´Ï±î?") == IDOK)
				{
					iRtnFunction = 10000;
					logStr.Format("		loading Step [%d]", iRtnFunction);
					putListLog(logStr);
					break;
				}
			}
			logStr.Format("		loading Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		break;
	case 10165://! Start P/B Button Push ´ë±â. 
		//Dio.StartPBLampOn(false);
		sBarCode.Format("%s", Task.ChipID);
		if (sysData.m_iProductComp == 1)
		{
			logStr.Format("====%s ¿ÏÁ¦Ç° °Ë»ç start====", Task.ChipID);
		}
		else
		{
			logStr.Format("====%s AA start====", Task.ChipID);
		}
		vision.clearOverlay(CCD);
		vision.drawOverlay(CCD);
		putListLog("	");
		putListLog(logStr);
		putListLog("	");
		iRtnFunction = 10170;
		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
		break;
	case 10170://! Start P/B Button Push ´ë±â.
	{
		bool bflag = true;

		Task.d_Align_offset_x[LENS_Align_MARK] = 0;
		Task.d_Align_offset_y[LENS_Align_MARK] = 0;
		Task.d_Align_offset_th[LENS_Align_MARK] = 0;

		/*bflag = EpoxyTimeCheck(bEpoxyTimeChk);
		if(!bflag)
		{
			iRtnFunction = -10170;
			break;
		}*/
		MandoInspLog.func_InitData();
		Task.func_TactTimeInit();
		dispGrid();
		Task.m_timeChecker.Measure_Time(1);	//Start ¹öÆ° Å¬¸¯ ½Ã°£
		func_ChipID_Draw();
		MandoInspLog.bInspRes = true;
		vision.clearOverlay(CCD);
		int iSocketCount = 0;
		//logStr.Format("%d", sysData.m_Socket_Count);
		iSocketCount = atoi(logStr);
		iSocketCount++;
		sysData.m_Socket_Count = iSocketCount;
		sysData.Save();

		iRtnFunction = 10180;
		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
	}
	break;
	case 10180:
		if (motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB Z Wait À§Ä¡ ÀÌµ¿ ¿Ï·á [%d]", iRtnFunction);
			putListLog(logStr);
			iRtnFunction = 10200;
			logStr.Format("		loading Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("PCB Z Wait À§Ä¡ ÀÌµ¿ ¿Ï·á ½ÇÆÐ [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -10180;
		}
		break;
	case 10200:
		if (motor.Lens_Tilt_Motor_Move(Wait_Pos))
		{
			iRtnFunction = 10202;
			logStr.Format("		Ready Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING745);	//LENSºÎ Tx, TyÃà ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ
			logStr.Format(sLangChange);
			putListLog(logStr);
			iRtnFunction = -10200;
		}
		break;

	case 10202:
		if (Dio.LaserCylinderCheck(true, false) == false)
		{
			logStr.Format("LASER ½Ç¸°´õ »ó½Â È®ÀÎ ½ÇÆÐ[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = -10202;
			break;
		}
		iRtnFunction = 10205;
		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
		break;

	case 10205:
		if (sysData.m_iProductComp == 1)
		{
			iRtnFunction = 10250;
			logStr.Format("		loading Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		else
		{
			iRtnFunction = 10210;
			logStr.Format("		loading Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		break;

	case 10210:
		if (Task.m_bOKLensPass == 1)   //Lens ³Ñ±è ¿Ï·á»óÅÂ
		{
			iRtnFunction = 10250;
			break;
		}

		iRtnFunction = 10250;
		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
		break;

	case 10250:
		iRtnFunction = 10400;
		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
		break;
	case 10400:	//! ¹ÙÄÚµå Á¤º¸ Check

		////////////////////////////////
		MandoInspLog.func_InitData();	//-- Log ÃÊ±âÈ­
		sBarCode.Format("%s", Task.ChipID);

		if (sBarCode == "EMPTY")//¹ÙÄÚµåÁ¤º¸°¡ ¾øÀ» °æ¿ì È®ÀÎ Message
		{
			logStr.Format("¹ÙÄÚµå Á¤º¸°¡ ¾ø½À´Ï´Ù. \n [ %s ]¸íÀ¸·Î ÁøÇà ÇÏ½Ã°Ú½À´Ï±î?", Task.ChipID);
			if (askMsg(logStr) == IDOK)
			{
				sBarCode.Format("%s", Task.ChipID);
			}
			else
			{
				iRtnFunction = 10100;
				logStr.Format("		loading Step [%d]", iRtnFunction);
				putListLog(logStr);
				break;
			}
		}
		else
		{
			if (!g_ADOData.func_AATaskToRecordID(modelList.curModelName, Task.ChipID)) 
			{
				logStr.Format("		loading Step [%d]", iRtnFunction);
				putListLog(logStr);
#if (____AA_WAY == PCB_TILT_AA)
				/*if(sysData.m_iProductComp==1)
				{
					logStr.Format("ÀÏÄ¡ÇÏ´Â ¾ÆÀÌµð°¡ ¾ø½À´Ï´Ù.[%d]", iUseStep);
				}else
				{
					logStr.Format("DataBase »ý¼º ½ÇÆÐ.[%d]\n MS Office¸¦ ´Ý¾ÆÁÖ¼¼¿ä.", iUseStep);
				}
				errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = -10400;
				break;*/
#endif		

			}
		}


		iRtnFunction = 10440;

		break;
	case 10440:
		if (sysData.m_FreeRun == 0 && sysData.m_iProductComp != 1)
		{
			pThread_MIUCheck = ::AfxBeginThread(Thread_MIUCheck, this);

		}
		iRtnFunction = 10500;
		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
		break;
	case 10500:
		iRtnFunction = 10900;
		break;

		break;
	case 10550:
		//±×¸³È®ÀÎ
		if (Dio.PcbGripCheck(true, false) == true)
		{
			logStr.Format(_T("PCB GRIP È®ÀÎ ¿Ï·á [%d]"), iRtnFunction);
			iRtnFunction = 10600;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format(_T("PCB GRIP ½ÇÆÐ [%d]"), iRtnFunction);
			iRtnFunction = -10550;
			errMsg2(Task.AutoFlag, logStr);
		}
		break;
	case 10600:
		//ÅÏ
		Sleep(1000);
		if (Dio.PcbTurn(false, false) == true)
		{
			logStr.Format(_T("PCB RETURN ¿Ï·á [%d]"), iRtnFunction);
			iRtnFunction = 10650;
		}
		else
		{
			logStr.Format(_T("PCB RETURN ½ÇÆÐ [%d]"), iRtnFunction);
			iRtnFunction = -10600;
			errMsg2(Task.AutoFlag, logStr);
		}
		putListLog(logStr);
		Task.PCBTaskTime = myTimer(true);
		
		break;
	case 10650:
		//ÅÏÈ®ÀÎ
		if (Dio.PcbTurnCheck(false, false) == true)
		{
			logStr.Format(_T("PCB RETURN È®ÀÎ ¿Ï·á [%d]"), iRtnFunction);
			iRtnFunction = 10700;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format(_T("PCB RETURN È®ÀÎ ½ÇÆÐ [%d]"), iRtnFunction);
			iRtnFunction = -10650;
			errMsg2(Task.AutoFlag, logStr);
		}
		
		break;
	case 10700:
		//PCB ·Îµù ÈÄ È®ÀÎ ´­·¯ÁÖ¼¼¿ä
		if (askMsg("PCB ·Îµù ÈÄ È®ÀÎ ´­·¯ÁÖ¼¼¿ä") == IDOK)
		{
			iRtnFunction = 10900;
		}
		
		break;
	case 10900:
		Task.PcbOnStage = 100;
		Task.m_b_AA_Retry_Flag = false;

		logStr.Format("%d", Task.m_iPcbPickupNo);
		m_labelPickupNoPcb.SetText(logStr);
		m_labelPickupNoPcb.Invalidate();
		
		MandoInspLog.sBarcodeID.Format("%s", Task.ChipID);	//¸¸µµ Â÷·®¿ëCamera °Ë»ç Log ÀúÀå
		iLaser_Pos = 0;
		

		if (sysData.m_iIrChartUse == 1)
		{
			IrvAlignLed.Irv_Light_SetValue(model.m_iLedValue[LEDDATA_TOP1_CHART], IR_CHART);
		}
		else
		{
			LightControl.ctrlLedVolume(LIGHT_TOP_CHART1, model.m_iLedValue[LEDDATA_TOP1_CHART]);
			LightControl.ctrlLedVolume(LIGHT_TOP_CHART2, model.m_iLedValue[LEDDATA_TOP2_CHART]);
			LightControl.ctrlLedVolume(LIGHT_TOP_CHART3, model.m_iLedValue[LEDDATA_TOP3_CHART]);
			LightControl.ctrlLedVolume(LIGHT_TOP_CHART4, model.m_iLedValue[LEDDATA_TOP4_CHART]);
			LightControl.ctrlLedVolume(LIGHT_LEFT_CHART, model.m_iLedValue[LEDDATA_CHART_L]);
			LightControl.ctrlLedVolume(LIGHT_RIGHT_CHART, model.m_iLedValue[LEDDATA_CHART_R]);
		}

		


#if (____AA_WAY == PCB_TILT_AA)
		if (Task.m_bOKLensPass == 1)   //Lens ³Ñ±è ¿Ï·á»óÅÂ
		{
			logStr.Format("		Lens ±×¸³ ÁøÇà [%d]", iRtnFunction);
			putListLog(logStr);
			iRtnFunction = 15500;// 10250;
			break;
		}
#endif
		iRtnFunction = 11000;
		logStr.Format("		loading Step [%d]" , iRtnFunction);
		putListLog(logStr);
		break;

	default:
		sLangChange.LoadStringA(IDS_STRING1007);	//PCB Á¦Ç° ÅõÀÔ Thread Step ¹øÈ£ ºñÁ¤»ó . 
		logStr.Format("PCB Á¦Ç° ÅõÀÔ Thread Step ¹øÈ£ ºñÁ¤»ó [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}

	return iRtnFunction;
}
int CAABonderDlg::RunProc_Smini_Holder_Epoxy_Process(int iUseStep)  //17000 ~ 18000
{
	int iRtnFunction = iUseStep;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;

	ctrlSubDlg(MAIN_DLG);
	m_iCurCamNo = 0;
	setCamDisplay(1, 0);
	changeMainBtnColor(MAIN_DLG);
	switch (iUseStep)
	{
	case 17000:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING1012);	//PCB_ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ.
			logStr.Format(sLangChange, iUseStep);
			putListLog(logStr);
			iRtnFunction = -17000;
			break;
		}

		iRtnFunction = 17020;
		break;
	case 17020: //Dispenser -ZÃà ÇÏ°­
		if (motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			iRtnFunction = 17030;
		}
		else
		{
			logStr.Format("PCB Z Wait À§Ä¡ ÀÌµ¿ ¿Ï·á ½ÇÆÐ [%d]", iUseStep);
			putListLog(logStr);
			errMsg2(Task.AutoFlag, logStr);

			iRtnFunction = -17020;
		}
		break;
	case 17030:  //µµÆ÷ À§Ä¡ ÀÌµ¿
		LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align Á¶¸í OFF
		if (motor.Pcb_Motor_Move(PDispense_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]))
		{
			sLangChange.LoadStringA(IDS_STRING935);	//PCB Dispense º¸Á¤ À§Ä¡ ÀÌµ¿ ¿Ï·á-º¸Á¤·® X:%.03lf,Y:%.03lf,T:%.03lf
			logStr.Format(sLangChange, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]);
			putListLog(logStr);
			iRtnFunction = 17040;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING938);	//PCB Dispense À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -17030;
		}
		break;
	case 17040: //Dispenser-z Ãà »ó½Â
		if (motor.PCB_Z_Motor_Move(PDispense_Pos))
		{
			iRtnFunction = 17050;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING977);	//PCB Z Dispense À§Ä¡ ÀÌµ¿ ½ÇÆÐ
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -17040;
		}
		break;

	case 17050: // µð½ºÆæ½º µ¿ÀÛ
		if (motor.func_Epoxy_Point_Draw())//4Æ÷ÀÎÆ® µµÆ÷
		{
			//Task.m_bOkDispense = 1;
			SYSTEMTIME SysTime;
			::GetLocalTime(&SysTime);
			CString timeData = "";
			CString sTemp = "";
			int iEpoxyShot = 0;
			sTemp.Format("%d", sysData.m_Epoxy_Shot);
			iEpoxyShot = atoi(sTemp);
			iEpoxyShot++;
			sysData.m_Epoxy_Shot = iEpoxyShot;
			timeData.Format("%02d%02d%02d", SysTime.wDay, SysTime.wHour, SysTime.wMinute);
			sysData.m_Epoxy_Last_Time = atoi(timeData);
			sysData.Save();
			sLangChange.LoadStringA(IDS_STRING937);	//PCB Dispense ¿Ï·á[%d]
			logStr.Format(sLangChange, iUseStep);
			putListLog(logStr);
			iRtnFunction = 17070;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING936);	//PCB Dispense ½ÇÆÐ[%d]
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -17050;
		}
		break;
	case 17070:
		if (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y))
		{
			LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_SENSOR]);//µµÆ÷ °Ë»çÀü ¾ó¶óÀÎ¸¶Å©¿ë ¶óÀÌÆ®
			iRtnFunction = 17100;
		}
		break;
	case 17100:
		iRtnFunction = 17200;
		break;
	case 17200:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCBºÎ zÃà ¸ðÅÍ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			logStr.Format(sLangChange, iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -17200;
			break;
		}
		else
		{
			logStr.Format("PCB Z ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
			putListLog(logStr);
		}
		iRtnFunction = 17250;
		break;
	case 17250:
		iRtnFunction = 17900;
		break;
	case 17900:
		iRtnFunction = 18000;
		break;
	default:
		logStr.Format("PCB Smini Thread Step ¹øÈ£ ºñÁ¤»ó . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	return iRtnFunction;
}
int CAABonderDlg::RunProc_Smini_PcbLoad_Process(int iUseStep)  //16000 ~ 17000
{
	int iRtnFunction = iUseStep;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;

	ctrlSubDlg(MAIN_DLG);
	m_iCurCamNo = 0;
	setCamDisplay(1, 0);
	changeMainBtnColor(MAIN_DLG);

	switch (iUseStep)
	{
	case 16000:
		//

		if (!motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("Lens_ZÃà ´ëÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ");
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -16000;
			break;
		}

		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -16000;
			break;
		}

		if (!motor.Pcb_Motor_Move(Pcb_Loading_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], 0))
		{
			logStr.Format("PCB %s ÀÌµ¿ ½ÇÆÐ", MotorPosName[Pcb_Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -16000;
			break;
		}
		iRtnFunction = 16100;

		break;
	case 16100:
		if (!Dio.LensMotorGrip(false, false))
		{
			logStr.Format("Lens Unit Gripper ÇØÁ¦ ½ÇÆÐ[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = -16100;
			break;
		}
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 16150;
		break;

	case 16150:
		if (Dio.LensMotorGripCheck(false, false))
		{
			iRtnFunction = 16200;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			iRtnFunction = -16150;
		}
		break;

	case 16200: // °ø±Þ
		if (!motor.Lens_Tilt_Motor_Move(Pcb_Loading_Pos))
		{
			logStr.Format("Lens %s Tx,TyÀÌµ¿ ½ÇÆÐ", MotorPosName[Pcb_Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -16200;
			break;
		}
		if (!motor.Lens_Motor_MoveXY(Pcb_Loading_Pos))
		{
			logStr.Format("Lens %s X,Y ÀÌµ¿ ½ÇÆÐ", MotorPosName[Pcb_Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -16200;
			break;
		}
		logStr.Format("Lens °ø±Þ À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 16250;
		break;

	case 16250:

		//------//
		if (!motor.PCB_Z_Motor_Move(Pcb_Loading_Pos))
		{
			logStr.Format("PCB loading Z ÀÌµ¿ ½ÇÆÐ");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -16250;
		}
		else
		{
			iRtnFunction = 16252;
		}
		//iRtnFunction = 16252;
		break;
	case 16252:
		
		if (motor.LENS_Z_Motor_Move(Pcb_Loading_Pos))
		{
			iRtnFunction = 16255;
		}
		else
		{
			logStr.Format("Lens Z À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -16252;
		}
		//iRtnFunction = 16255;
		break;

	case 16255:
		iRtnFunction = 16300;
		break;
	case 16300: // ¸ðÅÍ ±×¸³
		if (Dio.LensMotorGrip(true, false))
		{
			Task.m_bOKLensPass = 1;
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 16350;
		}
		else
		{
			Task.m_bOKLensPass = -1;
			logStr.Format("Lens ±×¸³ ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -16300;
		}
		//iRtnFunction = 16350;
		break;

	case 16350://¸ðÅÍ ±×¸³ È®ÀÎ
		if (Dio.LensMotorGripCheck(true, false))
		{
			logStr.Format("Lens ±×¸³ ¿Ï·á[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 16360;

			Sleep(800);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("Lens Gripper ÀüÁø ¼¾¼­ ½Ã°£ ÃÊ°ú..[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -16350;
		}
		//iRtnFunction = 16360;
		break;
	case 16360:
		if (askMsg("Lens Grip Check") == IDOK)
		{
			iRtnFunction = 16400;
		}
		else
		{
			iRtnFunction = 16360;
		}
		//------//

		//iRtnFunction = 16400;
		break;
	case 16400:
		if (!motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("Lens Z ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -16400;
			break;
		}
		//
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB waiting Z ÀÌµ¿ ½ÇÆÐ");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -16400;
			break;
		}
		iRtnFunction = 16450;
		
		break;

	case 16450:
		if (!motor.Lens_Motor_MoveXY(Wait_Pos))
		{
			logStr.Format("Lens %s X,Y ÀÌµ¿ ½ÇÆÐ", MotorPosName[Wait_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -16450;
			break;
		}
		iRtnFunction = 16452;
		break;
	case 16452:
		
		if (motor.Pcb_Motor_Move(Wait_Pos))			//PCBºÎ ¸ðÅÍ ´ë±âÀ§Ä¡ ÀÌµ¿
		{
			iRtnFunction = 16455;
			logStr.Format("Pcb x,y ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á [%d]", iUseStep);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("Pcb x,y ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = -16452;
		}
		break;

	case 16455:
		//askMsg("OUT MODULE PLZ");
		if (askMsg("Sensor Place Empty?") == IDOK)
		{
			iRtnFunction = 16800;
		}
		else
		{
			iRtnFunction = 16455;
		}
		//iRtnFunction = 16800;
		break;
	case 16800:

		iRtnFunction = 17000;
		logStr.Format("PCB Epoxy Waiting[%d]", iUseStep);
		putListLog(logStr);
		break;
	default:
		logStr.Format("PCB Smini Thread Step ¹øÈ£ ºñÁ¤»ó . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	return iRtnFunction;
}
int CAABonderDlg::RunProc_Smini_PcbUnLoad_Process(int iUseStep)  //18000 ~ 19000
{
	int iRtnFunction = iUseStep;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;

	ctrlSubDlg(MAIN_DLG);
	m_iCurCamNo = 0;
	setCamDisplay(1, 0);
	changeMainBtnColor(MAIN_DLG);

	switch (iUseStep)
	{
	case 18000:
		if (!motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("Lens_ZÃà ´ëÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ");
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -18000;
			break;
		}

		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -18000;
			break;
		}

		if (!motor.Pcb_Motor_Move(Pcb_Loading_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], 0))
		{
			logStr.Format("PCB %s ÀÌµ¿ ½ÇÆÐ", MotorPosName[Pcb_Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -18000;
			break;
		}
		iRtnFunction = 18100;
		break;
	case 18100:

		iRtnFunction = 18200;
		break;

	case 18200: // °ø±Þ
		if (!motor.Lens_Tilt_Motor_Move(Pcb_Loading_Pos))
		{
			logStr.Format("Lens %s Tx,TyÀÌµ¿ ½ÇÆÐ", MotorPosName[Pcb_Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -18200;
			break;
		}
		if (!motor.Lens_Motor_MoveXY(Pcb_Loading_Pos))
		{
			logStr.Format("Lens %s X,Y ÀÌµ¿ ½ÇÆÐ", MotorPosName[Pcb_Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -18200;
			break;
		}
		logStr.Format("Lens °ø±Þ À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 18250;


		break;

	case 18250:

		//----//
		if (!motor.PCB_Z_Motor_Move(Pcb_Loading_Pos))
		{
			logStr.Format("PCB loading Z ÀÌµ¿ ½ÇÆÐ");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -18250;
		}
		else
		{
			iRtnFunction = 18252;
		}
		//iRtnFunction = 18252;
		break;
	case 18252:
		if (motor.LENS_Z_Motor_Move(Pcb_Loading_Pos))
		{
			iRtnFunction = 18255;
		}
		else
		{
			logStr.Format("Lens Z À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -18252;
		}
		//iRtnFunction = 18255;
		break;

	case 18255:
		iRtnFunction = 18300;
		break;
	case 18300: // ¸ðÅÍ ¾ð±×¸³
		if (Dio.LensMotorGrip(false, false))
		{
			Task.m_bOKLensPass = 1;
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 18350;
		}
		else
		{
			Task.m_bOKLensPass = -1;
			logStr.Format("Lens UnGrip ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -18300;
		}
		//iRtnFunction = 18350;
		break;

	case 18350://¸ðÅÍ ¾ð±×¸³ È®ÀÎ
		if (Dio.LensMotorGripCheck(false, false))
		{
			logStr.Format("Lens UnGrip ¿Ï·á[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 18400;

			Sleep(800);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("Lens Gripper ÈÄÁø ¼¾¼­ ½Ã°£ ÃÊ°ú..[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -18350;
		}
		//----//

		//iRtnFunction = 18400;
		break;
	case 18400:
		if (!motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("Lens Z ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -18400;
		}
		//
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB waiting Z ÀÌµ¿ ½ÇÆÐ");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -18400;
		}
		iRtnFunction = 18450;
		break;
	case 18450:
		if (!motor.Lens_Motor_MoveXY(Wait_Pos))		//<------ °£¼·¾øÀ¸¸é »©ÀÚ
		{
			logStr.Format("Lens %s X,Y ÀÌµ¿ ½ÇÆÐ", MotorPosName[Wait_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -18450;
			break;
		}

		iRtnFunction = 18900;
		break;
	case 18900:

		iRtnFunction = 18990;
		break;
	case 18990:

		iRtnFunction = 19000;
		logStr.Format("PCB Unload Complete[%d]", iUseStep);
		putListLog(logStr);
		break;
	default:
		logStr.Format("PCB Smini Thread Step ¹øÈ£ ºñÁ¤»ó . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	return iRtnFunction;
}
int CAABonderDlg::RunProc_Smini_Uv_Process(int iUseStep)  //19000 ~ 20000
{
	int iRtnFunction = iUseStep;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;

	ctrlSubDlg(MAIN_DLG);
	m_iCurCamNo = 0;
	setCamDisplay(1, 0);
	changeMainBtnColor(MAIN_DLG);

	switch (iUseStep)
	{
	case 19000:

		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -19000;
			break;
		}
		iRtnFunction = 19050;
		break;
	case 19050:
		iRtnFunction = 19330;
		break;
	case 19330:
		iRtnFunction = 19350;
		break;
	case 19350:
		iRtnFunction = 19400;
		break;
	case 19400:
		if (motor.Pcb_Motor_Move(Holder_Uv_Pos))			//PCBºÎ ¸ðÅÍ ´ë±âÀ§Ä¡ ÀÌµ¿
		{
			iRtnFunction = 19420;
			logStr.Format("Pcb x,y Holder_Uv_Pos ÀÌµ¿ ¿Ï·á [%d]", iUseStep);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("Pcb x,y Holder_Uv_Pos ÀÌµ¿ ½ÇÆÐ [%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = -19400;
		}
		break;
	case 19420:
		iRtnFunction = 19430;
		break;
	case 19430:
		if (!motor.PCB_Z_Motor_Move(Holder_Uv_Pos))
		{
			logStr.Format("PCB Holder_Uv_Pos Z ÀÌµ¿ ½ÇÆÐ");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19430;
			break;
		}
		iRtnFunction = 19440;
		break;
	case 19440:
		if (Dio.UVCylinder_UPDOWN(false, false))
		{
			logStr.Format("UV ½Ç¸°´õ ÇÏ°­ ¿Ï·á[%d]", iUseStep);
			putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 19450;
		}
		else
		{
			logStr.Format("UV ½Ç¸°´õ ÇÏ°­ ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19440;
		}
		break;
	case 19450:
		if (Dio.UVCylinderCheck(false, false))
		{
			logStr.Format("UV ½Ç¸°´õ ÇÏ°­ È®ÀÎ ¿Ï·á[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 19600;

			Sleep(800);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("UV ½Ç¸°´õ ÇÏ°­ ½Ã°£ ÃÊ°ú..[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19450;
		}
		break;
	case 19600:
		//uv ½ÃÀÛ
		UVCommand2.UV_Shutter_Open();
		Sleep(10);

		logStr.Format("===> Holder UV ON");
		putListLog(logStr);
		Sleep(model.UV_Time);

		UVCommand2.UV_Shutter_Close();
		logStr.Format("===> Holder UV OFF [%d]", iUseStep);
		putListLog(logStr);

		iRtnFunction = 19630;//
		break;
	case 19630:
		if (askMsg("Uv ReTry?") == IDOK)
		{
			iRtnFunction = 19600;
		}
		else
		{
			iRtnFunction = 19660;
		}
		break;
	case 19660:
		if (Dio.UVCylinder_UPDOWN(true, false))
		{
			logStr.Format("UV ½Ç¸°´õ »ó½Â ¿Ï·á[%d]", iUseStep);
			putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 19670;
		}
		else
		{
			logStr.Format("UV ½Ç¸°´õ »ó½Â ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19660;
		}
		break;
	case 19670:
		if (Dio.UVCylinderCheck(true, false))
		{
			logStr.Format("UV ½Ç¸°´õ »ó½Â È®ÀÎ ¿Ï·á[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 19680;

			Sleep(800);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("UV ½Ç¸°´õ »ó½Â ½Ã°£ ÃÊ°ú..[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19670;
		}
		break;
	case 19680:
		/*if (askMsg("UV complete. continue?") == IDOK)
		{
			iRtnFunction = 19690;
		}
		else
		{
			iRtnFunction = 19680;
		}
		break;*/
		iRtnFunction = 19690;
		break;
	case 19690:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB waiting Z ÀÌµ¿ ½ÇÆÐ");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19690;
		}
		iRtnFunction = 19700;
		break;
	case 19700:

		iRtnFunction = 19750;
		break;
	case 19750:
		if (askMsg("Áö±×Á¦°Å ÈÄ  È®ÀÎ ´­·¯ÁÖ¼¼¿ä") == IDOK)
		{
			iRtnFunction = 19760;
			Sleep(5000);
		}
		break;
	case 19760:
		if (!motor.Lens_Motor_MoveXY(Wait_Pos))
		{
			logStr.Format("Lens %s X,Y ÀÌµ¿ ½ÇÆÐ", MotorPosName[Wait_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -19760;
			break;
		}
		logStr.Format("Lens °ø±Þ À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 19800;
		break;
	case 19800:
		if (Dio.PcbTurn(true, false) == true)
		{
			logStr.Format(_T("PCB TURN [%d]"), iRtnFunction);
			iRtnFunction = 19850;
			Task.PCBTaskTime = myTimer(true);
		}
		else
		{
			logStr.Format(_T("PCB TURN ½ÇÆÐ [%d]"), iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19800;
		}
		break;
	case 19850:
		if (Dio.PcbTurnCheck(true, false) == true)
		{
			logStr.Format(_T("PCB TURN È®ÀÎ ¿Ï·á [%d]"), iRtnFunction);
			iRtnFunction = 19900;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format(_T("PCB TURN ½ÇÆÐ [%d]"), iRtnFunction);
			iRtnFunction = -19850;
			errMsg2(Task.AutoFlag, logStr);
		}
		break;
	case 19900:
		if (Dio.ContactUpDown(true, false) == true)
		{
			logStr.Format(_T("Contact UP ¼º°ø [%d]"), iRtnFunction);
			iRtnFunction = 19910;
			Task.PCBTaskTime = myTimer(true);
		}
		else
		{
			logStr.Format(_T("Contact UP ½ÇÆÐ [%d]"), iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19900;
		}
		break;
	case 19910:
		if (Dio.ContactUpDownCheck(true, false) == true)
		{
			logStr.Format(_T("Contact UP È®ÀÎ ¿Ï·á [%d]"), iRtnFunction);
			iRtnFunction = 19930;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format(_T("Contact UP È®ÀÎ ½ÇÆÐ [%d]"), iRtnFunction);
			iRtnFunction = -19910;
			errMsg2(Task.AutoFlag, logStr);
		}
		break;
	case 19930:
		iRtnFunction = 19990;
		break;
	case 19990:
		iRtnFunction = 26000;
		logStr.Format("PCB Unload Complete[%d]", iUseStep);
		putListLog(logStr);
		break;
	default:
		logStr.Format("PCB Smini Thread Step ¹øÈ£ ºñÁ¤»ó . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	return iRtnFunction;
}
int CAABonderDlg::RunProc_PCBOutsideAlign(int iUseStep)
{
	//! PCB ¿ÜºÎ Align Step(11000 ~ 14999)					//!! MIU_Initialize -> PCB Holder Align À§Ä¡
	int iRtnFunction = iUseStep;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;		//PCB ¿µ»ó È¹µæ Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;

	//ctrlSubDlg(MAIN_DLG);		//20161212  ccd start ¼öÁ¤
	m_iCurCamNo =0;
	setCamDisplay(1, 0);
	changeMainBtnColor(MAIN_DLG);

	switch( iUseStep )
	{
	case 11000:
		/////////////////////////
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 11500;
		break;

	case 11500: 
		iRtnFunction = 12000;
		break;
	case 12000://! ¹ÙÄÚµå DB Data ÀúÀå
		/*if( !g_ADOData.func_AATaskToRecordID(modelList.curModelName, Task.ChipID) )
		{
			logStr.Format("DataBase »ý¼º ½ÇÆÐ.[%d]\n MS Office¸¦ ´Ý¾ÆÁÖ¼¼¿ä.", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -12000;
			break;
		}*/
		iRtnFunction = 13000;	
		break;

	case 13000:
		if (Task.m_bOKLensPass == 1)   //Lens ³Ñ±è ¿Ï·á»óÅÂ
		{
			iRtnFunction = 14100;
			break;
		}

		Task.m_iRetry_Opt = 0;
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_HOLDER]);		// Align¸¸ Á¶¸í ON
		Task.d_Align_offset_x[PCB_Holder_MARK] = 0;
		Task.d_Align_offset_y[PCB_Holder_MARK] = 0;
		Task.d_Align_offset_th[PCB_Holder_MARK] = 0;
		if(motor.Pcb_Motor_Move(Holder_Pos))
		{
			Task.PCBTaskTime = myTimer(true);
			logStr.Format("Pcb Holder À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
  			putListLog(logStr);
  			iRtnFunction = 13300;
		}
		else
		{
			logStr.Format("Pcb Holder À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -13000;
		}
		break;

	case 13300:
		if(motor.PCB_Z_Motor_Move(Holder_Pos))
		{
			Task.PCBTaskTime = myTimer(true);
			logStr.Format("Pcb Holder ZÀ§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
  			putListLog(logStr);
			//151109 ¿µÁø ÀÓ½Ã µô·¹ÀÌ
			Sleep(1000);
  			iRtnFunction = 13500;
		}
		else
		{
			logStr.Format("Pcb Holder ZÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -13300;
		}
		break;
	case 13500:
		if((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 13700;
		}
		break;
	case 13700: // lens Align

		if (Task.m_iRetry_Opt > iAlignRetry)
		{
			logStr.Format("PCB Holder Àç°Ë»ç %d È¸ ½ÇÆÐ[%d]\n º¸Á¤ ¹ÌÀû¿ëÀ¸·Î ÁøÇàÇÏ½Ã°Ú½À´Ï±î?", Task.m_iRetry_Opt, iUseStep);
			if ( askMsg(logStr) == IDOK)
			{
				Task.d_Align_offset_x[PCB_Holder_MARK] = 0;
				Task.d_Align_offset_y[PCB_Holder_MARK] = 0;
				Task.d_Align_offset_th[PCB_Holder_MARK] = 0;
				iRtnFunction = 14100;
			}
			else
			{
				Task.m_iRetry_Opt = 0;
				iRtnFunction = 13700;
				logStr.Format("PCB Holder Àç°Ë»ç ½ÃÀÛ[%d]", iUseStep);
				putListLog(logStr);
				//logStr.Format("PCB Holder Àç°Ë»ç %d È¸ ½ÇÆÐ[%d]", Task.m_iRetry_Opt, iUseStep);
				//errMsg2(Task.AutoFlag,logStr);
			}
			
			break;
		}
		
		offsetX = offsetY = offsetTh = 0.0;
		iRtn = procCamAlign(CAM2-1, PCB_Holder_MARK, false, offsetX, offsetY, offsetTh);
		offsetTh = 0.0;
		saveInspImage(PCB_IMAGE_SAVE, Task.m_iRetry_Opt);

		Task.m_iRetry_Opt++;
		offsetTh = 0;//holder x,yÃà¸¸
		if (iRtn == 0)
		{
			Task.d_Align_offset_x[PCB_Holder_MARK] -= offsetX;
			Task.d_Align_offset_y[PCB_Holder_MARK] -= offsetY;
			Task.d_Align_offset_th[PCB_Holder_MARK] += offsetTh;

			logStr.Format("Pcb Holder : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]);//0.000
//			Sleep(500);
			//pcbDlg->m_labelAlignResult.SetText(logStr);
			//pcbDlg->m_labelAlignResult.Invalidate();

			putListLog(logStr);

			int iRtnVal = AlignLimitCheck(CAM2-1, offsetX, offsetY, offsetTh);
			if(iRtnVal==1)
			{
				iRtnFunction = 14000;				// º¸Á¤ ÀÌµ¿
			}
			else if(iRtnVal==2 )
			{
				sLangChange.LoadStringA(IDS_STRING654);	//Lens Align [%d] ¿Ï·á [%d]
				logStr.Format("		Pcb Holder [%d] ¿Ï·á [%d]", Task.m_iRetry_Opt, iUseStep);
				putListLog(logStr);

				InstantMarkDelete(PCB_Holder_MARK);			//ÀÓ½Ã µî·Ï ¸¶Å© »èÁ¦

				logStr.Format("		Pcb Holder Complete : %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]);//0.000);
				AlignResultSave(logStr);	
				Task.m_iRetry_Opt = 0;
				iRtnFunction = 14100;				// ¿Ï·á
			}
			else
			{
				logStr.Format("Pcb Holder [%d] º¸Á¤ ¹üÀ§ ÃÊ°ú[%d]", Task.m_iRetry_Opt, iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				AlignLogSave(logStr);
				InstantMarkDelete(PCB_Holder_MARK);
				iRtnFunction = -13700;				// NG
			}
		}
		else
		{
			logStr.Format("Pcb Holder [%d] ¸¶Å© ÀÎ½Ä ½ÇÆÐ[%d]", Task.m_iRetry_Opt, iUseStep);
			putListLog(logStr);
			Sleep(300);
			iRtnFunction = 13700;					//  Àç°Ë»ç 
		}
		break;

	case 14000://!! º¸Á¤·® ÀÌµ¿.- Retry
		if(motor.Pcb_Holder_Align_Move(Holder_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 13500;
		}
		else
		{
			logStr.Format("Pcb Holder º¸Á¤ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -14000;
		}
		break;
	case 14100:	
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format(" PCB ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -14100;	
			break;
		}
#if (____AA_WAY == PCB_TILT_AA)
		iRtnFunction = 15000; 
#elif (____AA_WAY == LENS_TILT_AA)
		iRtnFunction = 26000;
		
#endif

		break;
	default:
		logStr.Format("PCB Á¦Ç° ÅõÀÔ Thread Step ¹øÈ£ ºñÁ¤»ó . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	return iRtnFunction;
}

////// AA Flow Step

int CAABonderDlg::RunProc_LensNewPassPickup(int iUseStep)
{//! Lens Pickup¿¡ Á¦Ç° ³Ñ±è Step	(15000 ~ 16600)
	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = "";
	double	dFinalDelay = (double)(model.strInfo_Cam[3].m_iDelayTime);
	bool bChk = true; 
	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;

	switch( iUseStep )
	{
		case 15000:
			if(!motor.LENS_Z_Motor_Move(Wait_Pos)) 
			{
				logStr.Format("Lens_ZÃà ´ëÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ");
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -15000;	
				break;
			}

			if(!motor.PCB_Z_Motor_Move(Wait_Pos))
			{
				logStr.Format("PCB ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -15000;	
				break;
			}

			if( Task.m_bOKLensPass != 1 )//! Lens ³Ñ±è ¹Ì¿Ï·á ÀÏ °æ¿ì	 
			{
				if( !motor.Pcb_Motor_Move(Loading_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], 0) )
				{
					logStr.Format("PCB %s ÀÌµ¿ ½ÇÆÐ", MotorPosName[Loading_Pos]);
					delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
					iRtnFunction = -15000;	
					break;
				}
				iRtnFunction = 15100;	
			}else
			{
				iRtnFunction = 15500;	 
			}
			break;
	case 15100:
		if(!Dio.LensMotorGrip(false, false))
		{
			logStr.Format("Lens Unit Gripper ÇØÁ¦ ½ÇÆÐ[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = -15100;
			break;
		}
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 15150;
		break;

	case 15150:
		if(Dio.LensMotorGripCheck(false, false))
		{
			iRtnFunction = 15200;
		}
		else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			iRtnFunction = -15150;
		}
		break;

	case 15200: // °ø±Þ
		if(!motor.Lens_Tilt_Motor_Move(Loading_Pos))
		{
			logStr.Format("Lens %s Tx,TyÀÌµ¿ ½ÇÆÐ", MotorPosName[Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -15200;
			break;
		}
		if(!motor.Lens_Motor_MoveXY(Loading_Pos))
		{
			logStr.Format("Lens %s X,Y ÀÌµ¿ ½ÇÆÐ", MotorPosName[Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -15200;
			break;
		}
		logStr.Format("Lens °ø±Þ À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 15250;
		break;

	case 15250: 
		if(!motor.PCB_Z_Motor_Move(Loading_Pos))
		{
			logStr.Format("PCB loading Z ÀÌµ¿ ½ÇÆÐ");
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -15250;
		}else
		{
			iRtnFunction = 15252;
		}
		break;
	case 15252:
		if(motor.LENS_Z_Motor_Move(Loading_Pos))
		{
			iRtnFunction = 15255;
		}
		else
		{
			logStr.Format("Lens Z À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -15252;
		}
		break;

	case 15255:
		iRtnFunction = 15300;
		break;
	case 15300: // ¸ðÅÍ ±×¸³
		if(Dio.LensMotorGrip(true,false))
		{
			Task.m_bOKLensPass = 1;
			Task.PCBTaskTime = myTimer(true);
 			iRtnFunction = 15350;
			Sleep(500);
			Dio.LensMotorGrip(false, false);
			Sleep(500);
			Dio.LensMotorGrip(true, false);
		}
		else
		{
			Task.m_bOKLensPass = -1;
			logStr.Format("Lens ±×¸³ ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -15300;
		}
		break;

	case 15350://¸ðÅÍ ±×¸³ È®ÀÎ
		if(Dio.LensMotorGripCheck(true, false))
		{
			logStr.Format("Lens ±×¸³ ¿Ï·á[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 15400;

			Sleep(800);
		}
		else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("Lens Gripper ÀüÁø ¼¾¼­ ½Ã°£ ÃÊ°ú..[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -15350;
		}
		break;
	case 15400:
		iRtnFunction = 15450;
		//Sleep(400);	//½Ç¸°´õ »ç¿ëÀ¸·Î PCB Stage UnGrip½Ã ¼¾¼­ °¨Áö ÈÄ, Delay
		/*if(Dio.HolderGrip(false,false))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 15450;
		}
		else
		{
			logStr.Format("Lens Gripper ÇØÁ¦ ½ÇÆÐ..[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);

			iRtnFunction = -15400;
		}*/
		break;

	case 15450:
		iRtnFunction = 15452;
		/*if(Dio.HolderGripCheck(false, false) )
		{
			iRtnFunction = 15452;
		}
		else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("PCB Stage Lens Gripper ÇØÁ¦ ½ÇÆÐ..[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);

			iRtnFunction = -15450;
		}*/
		//Sleep(600);
		break;
	case 15452:

		iRtnFunction = 15455;
		break;

	case 15455:

		iRtnFunction = 15500;
		break;
	case 15500: // ´ë±â À§Ä¡ ÀÌµ¿		//·»Áî ±×¸³»óÅÂ¿¡¼­ ½ÃÀÛÇÏ¸é ÀÌ¸® ¿Â´Ù ¼öÁ¤ÇÏÁö¸¶

		if(!motor.LENS_Z_Motor_Move(Wait_Pos))
		{	
			logStr.Format("Lens Z ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -15500;
		}
		//
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB waiting Z ÀÌµ¿ ½ÇÆÐ");
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -15500;
		}
		iRtnFunction = 15550;
		break;
	case 15550:
		if (motor.Pcb_Motor_Move(Wait_Pos))			//PCBºÎ ¸ðÅÍ ´ë±âÀ§Ä¡ ÀÌµ¿
		{
			iRtnFunction = 15800;
			logStr.Format("Pcb x,y ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á [%d]", iUseStep);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("Pcb x,y ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = -15550;
		}
		break;
	case 15800:

		Task.LensTask = 30000;	//pcb AAÀÏ¶§ lens ´Â Align ÀÌµ¿ÈÄ laser
		iRtnFunction = 15900;
		logStr.Format("PCB Epoxy Waiting[%d]", iUseStep);
		putListLog(logStr);
		break;
	case 15900:
		//pcb ´Â 
		//lens°¡ Align , laser¸¶Ä¡°í 
		//º»µùÀ§Ä¡·Î ºüÁö±â Àü±îÁö ´ë±â À§Ä¡·Î°¡¼­ ´ë±âÇØ¾ßµÈ´Ù.

		if (Task.interlockLens == 1)
		{
			Task.interlockLens = 0;
			iRtnFunction = 26000;
		}
		break;
default:
		logStr.Format("Lens °ø±Þ µ¿ÀÛ Thread Step ¹øÈ£ ºñÁ¤»ó . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}

	return iRtnFunction;
}
int CAABonderDlg::RunProc_LensLoading(int iUseStep)
{//! Lens Pickup¿¡ Á¦Ç° ³Ñ±è Step	(15000 ~ 16600)
	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = "";
	double	dFinalDelay = (double)(model.strInfo_Cam[3].m_iDelayTime);
	bool bChk = true; 
	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;
	//Task.m_bOKLensPass = -1;//PCB¾ð·Îµå °úÁ¤¿¡¼­ ³Ñ±è»óÅÂ·Î ¸¸µé¾î³õÀ¸¹Ç·Î ÃÊ±âÈ­.

	switch( iUseStep )
	{
		case 40000:
			if(!motor.LENS_Z_Motor_Move(Wait_Pos)) 
			{
				logStr.Format("Lens_ZÃà ´ëÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ");
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -40000;	
				break;
			}

			if(!motor.PCB_Z_Motor_Move(Wait_Pos))
			{
				logStr.Format("PCB ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -40000;	
				break;
			}
			 
			if( Task.m_bOKLensPass != 1 )//! Lens ³Ñ±è ¹Ì¿Ï·á ÀÏ °æ¿ì	 
			{

				Task.d_Align_offset_x[LENS_Align_MARK] = 0.0;
				Task.d_Align_offset_y[LENS_Align_MARK] = 0.0;
				if( !motor.Pcb_Motor_Move(Loading_Pos, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], 0) )
				{
					logStr.Format("PCB %s ÀÌµ¿ ½ÇÆÐ", MotorPosName[Loading_Pos]);
					delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
					iRtnFunction = -40000;	
					break;
				}
				//M_TRINITY ¸ðµ¨Àº ·¹ÀÌÀúÈÄ Æ¿Æ® ½ÉÇØ¼­ º»µùÀ§Ä¡°¥¶§ ´Ù½Ã ·¹ÀÌÀú Æ¿Æ®µÈ tx,ty °ªÀ¸·Î ÀÌµ¿ 
				if (!motor.Pcb_Tilt_Motor_Move(Loading_Pos))
				{
					logStr.Format("PCB %s ÀÌµ¿ ½ÇÆÐ", MotorPosName[Loading_Pos]);
					delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
					iRtnFunction = -40000;
					break;
				}
				iRtnFunction = 40100;	
			}else
			{
				iRtnFunction = 40500;	 
			}
			break;
	case 40100:
		if(!Dio.LensMotorGrip(false, false))
		{
			logStr.Format("Lens Unit Gripper ÇØÁ¦ ½ÇÆÐ[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = -40100;
			break;
		}
		Task.PCBTaskTime = myTimer(true);
		iRtnFunction = 40150;
		break;

	case 40150:
		if(Dio.LensMotorGripCheck(false, false)){
			iRtnFunction = 40170;
			Task.PCBTaskTime = myTimer(true);
		}
		else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			iRtnFunction = -40150;
		}
		break;
	case 40170:
		iRtnFunction = 40200;
		break;

	case 40200: // °ø±Þ 
		if (!motor.Lens_Tilt_Motor_Move(Loading_Pos))
		{
			logStr.Format("Lens %s Tx,TyÀÌµ¿ ½ÇÆÐ", MotorPosName[Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -40200;
			break;
		}
		if (!motor.Lens_Motor_MoveXY(Loading_Pos))
		{
			logStr.Format("Lens %s X,Y ÀÌµ¿ ½ÇÆÐ", MotorPosName[Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -40200;
			break;
		}

		logStr.Format("Lens °ø±Þ À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 40250;
		break;
	case 40250:
		if (!motor.PCB_Z_Motor_Move(Loading_Pos))
		{
			logStr.Format("PCB loading Z ÀÌµ¿ ½ÇÆÐ");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40250;
			break;
		}
		if (!motor.LENS_Z_Motor_Move(Loading_Pos))
		{
			logStr.Format("Lens Z À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40250;
			break;
		}
		iRtnFunction = 40252;
		break;
	case 40252:
		Dio.LensMotorGrip(true, false);
		Sleep(500);
		iRtnFunction = 40255;
		break;

	case 40255:
		Dio.LensMotorGrip(false, false);
		Sleep(300);
		iRtnFunction = 40300;
		break;
	case 40300: // ¸ðÅÍ ±×¸³
		if(Dio.LensMotorGrip(true,false))
		{
			Task.m_bOKLensPass = 1;
			Task.PCBTaskTime = myTimer(true);
 			iRtnFunction = 40350;
		}
		else
		{
			Task.m_bOKLensPass = -1;
			logStr.Format("Lens ±×¸³ ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -40300;
		}
		break;

	case 40350://¸ðÅÍ ±×¸³ È®ÀÎ
		if(Dio.LensMotorGripCheck(true, false))
		{
			logStr.Format("Lens ±×¸³ ¿Ï·á[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 40400;
		}
		else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("Lens Gripper ÀüÁø ¼¾¼­ ½Ã°£ ÃÊ°ú..[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -40350;
		}
		break;

	case 40400:
		Sleep(500);	//½Ç¸°´õ »ç¿ëÀ¸·Î PCB Stage UnGrip½Ã ¼¾¼­ °¨Áö ÈÄ, Delay 0.2s
		iRtnFunction = 40455;
		break;

	case 40455:
		iRtnFunction = 40500;
		break;
	case 40500: // ´ë±â À§Ä¡ ÀÌµ¿
		if(!motor.LENS_Z_Motor_Move(Wait_Pos))
		{	
			logStr.Format("Lens Z ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -40500;
			break;
		}
		//
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB waiting Z ÀÌµ¿ ½ÇÆÐ");
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -40500;
			break;
		}
		iRtnFunction = 40550;
		break;
	case 40550:
		//ºÒÇÊ¿ä µ¿ÀÛ »ý·«.
		/*if (!motor.Lens_Motor_MoveXY(Wait_Pos))
		{
			logStr.Format("LensºÎ ¸ðÅÍ ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = -40550;
		}
		else
		{
			iRtnFunction = 40900;
		}*/
		iRtnFunction = 40900;
		break;
	case 40900:
		iRtnFunction = 41000;
		break;
default:
		logStr.Format("Lens °ø±Þ µ¿ÀÛ Thread Step ¹øÈ£ ºñÁ¤»ó . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}

	return iRtnFunction;
}

int	CAABonderDlg::RunProc_LaserMeasure(int iUseStep)
{
	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;	//PCB ¿µ»ó È¹µæ Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser ÃøÁ¤Àü Delay
	switch( iUseStep )
	{
		case 27000:
			Task.oldLaserTx = 0.1;
			Task.oldLaserTy = 0.1;
			iLaser_Pos = 0;// 4;//4¹øºÎÅÍ°¡ laser out point
			LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align¸¸ Á¶¸í ON
			if(!motor.PCB_Z_Motor_Move(Wait_Pos)) 
			{
				logStr.Format("[¿îÀü ÁØºñ] PCB ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -27000;	
				break;
			}

			//if ((sysData.m_iDispensePass == 1) || (Task.m_bOkDispense == 1))
			//{
			//	iRtnFunction = 30000;	//-- µµÆ÷ Pass Mode ÀÌ°Å³ª, µµÆ÷ ¿Ï·á ÀÏ °æ¿ì
			//	break;
			//}

			iRtnFunction = 27100;// 27350;//<-ÀÓ½Ã ÆÐ½º   Original-> 27100; 
			break;

		case 27100:
			if(motor.Pcb_Motor_Move_Laser(iLaser_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
			{
				Task.PCBTaskTime = myTimer(true);
				logStr.Format("Laser º¯À§ ÃøÁ¤ ¿ÜºÎ À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
  				putListLog(logStr);
  				iRtnFunction = 27150;
			}
			else
			{
				logStr.Format("Laser º¯À§ ÃøÁ¤ ¿ÜºÎ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
				errMsg2(Task.AutoFlag,logStr);
 				iRtnFunction = -27100;
			}
			break;

		case 27150:
			if(iLaser_Pos==0)
			{
				if(!motor.PCB_Z_Motor_Move(Laser_Pcb_Pos)) 
				{
					logStr.Format("PCB Laser_Pcb_Pos Z ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
					putListLog(logStr);
					errMsg2(Task.AutoFlag, logStr);
					iRtnFunction = -27150;
					break;
				}
				else
				{
					logStr.Format("PCB Laser_Pcb_Pos Z ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
					putListLog(logStr);
				}
			}

			if (iLaser_Pos == 0) { iRtnFunction = 27160; }//Ã³À½¿¡¸¸ ½Ç¸°´õ ÇÏ°­
			else { iRtnFunction = 27200; }//Ã³À½¾Æ´Ï¸é ¹Ù·Î °Ë»ç·Î.
			Task.PCBTaskTime = myTimer(true);
			break;
			//laser ½Ç¸°´õ ÇÏ°­
		case 27160:
			if (Dio.LaserCylinderUPDOWN(false, false))
			{
				logStr.Format("LASER ½Ç¸°´õ ÇÏ°­ ¿Ï·á[%d]", iRtnFunction);
				theApp.MainDlg->putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
				iRtnFunction = 27170;
			}
			else
			{
				logStr.Format("LASER ½Ç¸°´õ ÇÏ°­ ½ÇÆÐ [%d]", iRtnFunction);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27160;
			}
			break;

		case 27170:
			if (Dio.LaserCylinderCheck(false, false))
			{
				logStr.Format("LASER ½Ç¸°´õ ÇÏ°­ È®ÀÎ ¿Ï·á[%d]", iRtnFunction);
				theApp.MainDlg->putListLog(logStr);
				iRtnFunction = 27200;

				Sleep(300);
			}
			else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
			{
				logStr.Format("LASER ½Ç¸°´õ ÇÏ°­ ½Ã°£ ÃÊ°ú..[%d]", iRtnFunction);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27170;
			}
			break;
			//laser½Ç¸°´õ ÇÏ°­ END
		case 27200:
			if((myTimer(true) - Task.PCBTaskTime) > iLaserDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
			{
				Sleep(200);
				iRtnFunction = 27250;
			}
			break;
		case 27250:
			
			Keyence.func_CL3000_Scan(Task.m_Laser_Point[iLaser_Pos]);
			Sleep(100);
			logStr.Format("º¯ À§¼¾¼­ %lf - %d À§Ä¡[%d]",Task.m_Laser_Point[iLaser_Pos],  iLaser_Pos+1, iUseStep );
  			putListLog(logStr);
			LaserPos[iLaser_Pos].x = motor.GetEncoderPos(Motor_PCB_X);
			LaserPos[iLaser_Pos].y = motor.GetEncoderPos(Motor_PCB_Y);
			LaserValue[iLaser_Pos] = Task.m_Laser_Point[iLaser_Pos];

			if(Task.m_Laser_Point[iLaser_Pos] != 0)
			{
				if(iLaser_Pos == 3)
				{
					if( func_Check_LaserValueErr(LaserValue) == false)	
					{
						logStr.Format("Laser ÃøÁ¤°ªÀÌ»ó: %.04lf, %.04lf, %.04lf, %.04lf [%d]", LaserValue[0], LaserValue[1], LaserValue[2], LaserValue[3], iUseStep);
						errMsg2(Task.AutoFlag,logStr);
 						iRtnFunction = -27250;
						break;
					}

					if(_calcLaserTilt(LaserPos, LaserValue, Task.d_Align_offset_xt[PCB_Chip_MARK], Task.d_Align_offset_yt[PCB_Chip_MARK]))
					{
						Task.m_dataOffset_x[0] =  Task.d_Align_offset_xt[PCB_Chip_MARK];
						Task.m_dataOffset_y[0] =  Task.d_Align_offset_yt[PCB_Chip_MARK];
						logStr.Format("	º¸Á¤·® Tx: %.04lf, Ty: %.04lf", Task.d_Align_offset_xt[PCB_Chip_MARK], Task.d_Align_offset_yt[PCB_Chip_MARK] );
  						putListLog(logStr);

						int Rnd = TiltAlignLimitCheck(Task.d_Align_offset_xt[PCB_Chip_MARK],Task.d_Align_offset_yt[PCB_Chip_MARK]);
						if(Rnd == 2)
						{
							if( Task.m_dataOffset_x[0] == 0 || Task.m_dataOffset_y[0] == 0 )
							{
								logStr.Format("Laser º¯À§ Â÷ °ª ÀÌ»ó ¹ß»ý..Xt : %lf, Yt : %lf",Task.m_dataOffset_x[0], Task.m_dataOffset_y[0]);
								//errMsg2(Task.AutoFlag, logStr);
								//iRtnFunction = -27250;
								//break;
							}
							if( !g_ADOData.func_AATaskToRecordLaser(Task.ChipID, Task.m_dataOffset_x[0], Task.m_dataOffset_y[0], Task.m_Laser_Point) )
							{
								logStr.Format("DataBase Laser º¯À§ ÃøÁ¤ Data ±â·Ï ½ÇÆÐ.[%d]\n MS Office¸¦ ´Ý¾ÆÁÖ¼¼¿ä.", iUseStep);
								errMsg2(Task.AutoFlag,logStr);
								iRtnFunction = -27250;
								break; 
							}
							Task.m_PcbLaserAfterTx = motor.GetCommandPos(Motor_PCB_Xt);
							Task.m_PcbLaserAfterTy = motor.GetCommandPos(Motor_PCB_Yt);

							logStr.Format("	PCB Laser tx:%lf , ty:%lf [%d]", model.axis[Motor_PCB_Xt].pos[Laser_Pcb_Pos], model.axis[Motor_PCB_Yt].pos[Laser_Pcb_Pos], iUseStep);
							putListLog(logStr);
							logStr.Format("	PCB Laser After tx:%lf , ty:%lf [%d]", Task.m_PcbLaserAfterTx, Task.m_PcbLaserAfterTy, iUseStep);
							putListLog(logStr);
							iRtnFunction = 27310;//->Laser½Ç¸°´õ»ó½Â, 27350;->PCB WaitPos
							iLaser_Pos++;
						}
						else if (Rnd == 1)
						{
							iRtnFunction = 27300;
						}
						else
						{
							logStr.Format("Laser Tilt  º¸Á¤°ª Limit¸¦ ÃÊ°ú ÇÏ¿´½À´Ï´Ù.[%d]", iUseStep);
							errMsg2(Task.AutoFlag,logStr);
 							iRtnFunction = -27250;
						}
					}
				}
				else
				{
					iRtnFunction = 27100;
					iLaser_Pos++;
				}
			}
			else
			{
				iRtnFunction = 27250;
			}
			break;
		case 27300:
			if(motor.Pcb_Move_Tilt( Task.d_Align_offset_xt[PCB_Chip_MARK], Task.d_Align_offset_yt[PCB_Chip_MARK] ))
			{
				iRtnFunction = 27100;
				logStr.Format("PCB Xt : %lf, Yt : %lf [%d]", Task.d_Align_offset_xt[PCB_Chip_MARK], Task.d_Align_offset_yt[PCB_Chip_MARK] ,  iUseStep);
				putListLog(logStr);
				iLaser_Pos = 0;
			}
			else
			{
				logStr.Format("PCB tx,tyÃà  À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27300;
			}
			break;

			//laser ½Ç¸°´õ »ó½Â
		case 27310:
			if (Dio.LaserCylinderUPDOWN(true, false))
			{
				logStr.Format("LASER ½Ç¸°´õ »ó½Â ¿Ï·á[%d]", iRtnFunction);
				theApp.MainDlg->putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
				iRtnFunction = 27320;
			}
			else
			{
				logStr.Format("LASER ½Ç¸°´õ »ó½Â ½ÇÆÐ [%d]", iRtnFunction);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27310;
			}
			break;
		case 27320:
			if (Dio.LaserCylinderCheck(true, false))
			{
				logStr.Format("LASER ½Ç¸°´õ »ó½Â È®ÀÎ ¿Ï·á[%d]", iRtnFunction);
				theApp.MainDlg->putListLog(logStr);
				iRtnFunction = 27350;
				Sleep(300);
			}
			else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
			{
				logStr.Format("LASER ½Ç¸°´õ »ó½Â ½Ã°£ ÃÊ°ú..[%d]", iRtnFunction);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27320;
			}
			break;
			//laser½Ç¸°´õ »ó½Â END

		case 27350:
			if(motor.PCB_Z_Motor_Move(Wait_Pos))
			{
				logStr.Format("PCB ZÃà ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
				putListLog(logStr);
				iRtnFunction = 27600;
			}
			else
			{
				logStr.Format("PCB ZÃà ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27350;
			}
			
			break;
		case 27600:

			iRtnFunction = 27650;
			break;
		case 27650:
			//Sensor Align Ãß°¡
			//
			offsetX = offsetY = offsetTh = 0.0;
			iRtnFunction = 27750;
			break;
		case 27750:
			Task.m_iRetry_Opt = 0;
			iRtnFunction = 27800;
			break;
		case 27800:
			ctrlSubDlg(MAIN_DLG);
			m_iCurCamNo = 0;
			LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_SENSOR]);		// Align¸¸ Á¶¸í ON

			if (motor.Pcb_Motor_Move(Align_Pos))
			{
				if (motor.PCB_Z_Motor_Move(Align_Pos))
				{
					logStr.Format("PCB Sensor Align À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
					putListLog(logStr);
					Task.d_Align_offset_x[PCB_Chip_MARK] = 0;
					Task.d_Align_offset_y[PCB_Chip_MARK] = 0;
					Task.d_Align_offset_th[PCB_Chip_MARK] = 0;
					iRtnFunction = 27850;
					Task.PCBTaskTime = myTimer(true);
				}
			}
			else
			{
				logStr.Format("PCB Sensor Align À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27960;
			}
			break;
		case 27850:
			if ((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
			{
				iRtnFunction = 27900;
			}
			break;
		case 27900:
			if (Task.m_iRetry_Opt>iAlignRetry)
			{
				logStr.Format("PCB Align(*Laser*) Àç°Ë»ç %d È¸ ½ÇÆÐ[%d]\n º¸Á¤ ¹ÌÀû¿ëÀ¸·Î ÁøÇàÇÏ½Ã°Ú½À´Ï±î?", Task.m_iRetry_Opt, iUseStep);
				Task.m_iRetry_Opt = 0;
				Task.d_Align_offset_x[PCB_Chip_MARK] = 0.0;
				Task.d_Align_offset_y[PCB_Chip_MARK] = 0.0;
				Task.d_Align_offset_th[PCB_Chip_MARK] = 0.0;
				if (askMsg(logStr) == IDOK)
				{
					iRtnFunction = 27960;
					logStr.Format("PCB Align ¹Ìº¸Á¤ ÁøÇà[%d]", iUseStep);
					putListLog(logStr);
				}
				else
				{
					iRtnFunction = 27900;
					logStr.Format("PCB Align Àç°Ë»ç ½ÃÀÛ[%d]", iUseStep);
					putListLog(logStr);
					Sleep(500);
				}
				break;
			}

			offsetX = offsetY = offsetTh = 0.0;

			iRtn = procCamAlign(CAM2 - 1, PCB_Chip_MARK, false, offsetX, offsetY, offsetTh);

			saveInspImage(CHIP_IMAGE_SAVE, Task.m_iRetry_Opt);
			Task.m_iRetry_Opt++;
			//offsetTh=0;
			if (iRtn == 0)
			{
				Task.d_Align_offset_x[PCB_Chip_MARK] -= offsetX;
				Task.d_Align_offset_y[PCB_Chip_MARK] -= offsetY;
				Task.d_Align_offset_th[PCB_Chip_MARK] -= offsetTh;

				Task.dPcbAlignTh = Task.d_Align_offset_th[PCB_Chip_MARK];
				logStr.Format("PCB Sensor Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
				putListLog(logStr);

				int iRtnVal = AlignLimitCheck(CAM2 - 1, offsetX, offsetY, offsetTh);

				if (iRtnVal == 1)
				{
					iRtnFunction = 27950;				// º¸Á¤ ÀÌµ¿
				}
				else if (iRtnVal == 2)
				{
					logStr.Format("		PCB Sensor Align [%d] ¿Ï·á [%d]", Task.m_iRetry_Opt, iUseStep);
					putListLog(logStr);
					logStr.Format("		PCB Sensor Align Complete : %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
					AlignResultSave(logStr);
					Task.m_iRetry_Opt = 0;

					Task.m_timeChecker.Measure_Time(4);	//Laser º¯À§ ÃøÁ¤ ½ÃÀÛ ½Ã°£

					
					iRtnFunction = 27960;		// ¿Ï·á
					LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align¸¸ Á¶¸í ON 
					LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, 0);		// Align¸¸ Á¶¸í ON
				}
				else
				{
					logStr.Format("PCB Sensor Align [%d] º¸Á¤ ¹üÀ§ ÃÊ°ú", Task.m_iRetry_Opt, iUseStep);
					errMsg2(Task.AutoFlag, logStr);
					AlignLogSave(logStr);
					iRtnFunction = -27900;				// NG
				}
			}
			else
			{
				logStr.Format("PCB Sensor Align(*Laser*) [%d] ¸¶Å© ÀÎ½Ä ½ÇÆÐ[%d]", Task.m_iRetry_Opt, iUseStep);
				putListLog(logStr);
				Sleep(500);
				iRtnFunction = 27900;				//  Àç°Ë»ç 
			}
			break;
		case 27950:
			if (motor.Pcb_Holder_Align_Move(Align_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
			{
				Task.PCBTaskTime = myTimer(true);
				iRtnFunction = 27850;
			}
			else
			{
				logStr.Format("PCB Sensor Align º¸Á¤ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27950;
			}
			break;
		case 27960:
			if (!motor.PCB_Z_Motor_Move(Wait_Pos))
			{
				logStr.Format("PCBºÎ ZÃà ´ë±â À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27960;
				break;
			}
			else
			{
				logStr.Format("PCBºÎ ZÃà ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
				putListLog(logStr);
				iRtnFunction = 27970;
			}
			
			break;
		case 27970:
			iRtnFunction = 27990;
		case 27990:
			iRtnFunction = 30000;
			break;
		default:
		logStr.Format("Pcb Thread Step ¹øÈ£ ºñÁ¤»ó .[%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	return iRtnFunction;
}
int	CAABonderDlg::RunProc_SensorAlign(int iUseStep)
{
	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = _T("");
	bool bChk = true;
	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;	//PCB ¿µ»ó È¹µæ Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser ÃøÁ¤Àü Delay
	switch( iUseStep )
	{
	case 26000:
		offsetX = offsetY = offsetTh = 0.0;
		if(!motor.PCB_Z_Motor_Move(Wait_Pos)) 
		{
			logStr.Format("		PCB ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -26000;	
			break;
		}
		iRtnFunction = 26050;
		break;
	case 26050:
		Task.m_iRetry_Opt = 0;
		iRtnFunction = 26600;
		break;

	case 26600:
		//Task.LensTaskTime = myTimer(true);
		iRtnFunction = 26610;
		break;

	case 26610:
		//ctrlSubDlg(MAIN_DLG);
		m_iCurCamNo =0;
		//SetDigReference(PCB_Chip_MARK);
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_SENSOR]);		// Align¸¸ Á¶¸í ON

		if(motor.Pcb_Motor_Move(Align_Pos))
		{
			if(motor.PCB_Z_Motor_Move(Align_Pos))
			{
				logStr.Format("PCB Sensor Align À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
  				putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
				Task.d_Align_offset_x[PCB_Chip_MARK] = 0;
				Task.d_Align_offset_y[PCB_Chip_MARK] = 0;
				Task.d_Align_offset_th[PCB_Chip_MARK] = 0;
				iRtnFunction = 26615;
			}
		}
		else
		{
			logStr.Format("PCB Sensor Align À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -26610;
		}
		break;
	case 26615:
		if((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			iRtnFunction = 26620;
		}
		break;
	case 26620: 
		if (Task.m_iRetry_Opt>iAlignRetry)
		{
			logStr.Format("PCB Align(*SensorAlign*) Àç°Ë»ç %d È¸ ½ÇÆÐ[%d]\n º¸Á¤ ¹ÌÀû¿ëÀ¸·Î ÁøÇàÇÏ½Ã°Ú½À´Ï±î?", Task.m_iRetry_Opt, iUseStep);
			Task.d_Align_offset_x[PCB_Chip_MARK] = 0.0;
			Task.d_Align_offset_y[PCB_Chip_MARK] = 0.0;
			Task.d_Align_offset_th[PCB_Chip_MARK] = 0.0;
			Task.m_iRetry_Opt = 0;
			if ( askMsg(logStr) == IDOK)
			{
				iRtnFunction = 26629;
			}
			else
			{
				iRtnFunction = 26620;
				logStr.Format("PCB Align Àç°Ë»ç ½ÃÀÛ[%d]", iUseStep);
				putListLog(logStr);
			}
			break;
			
			break;
		}

		offsetX = offsetY = offsetTh = 0.0;

		iRtn = procCamAlign(CAM2-1, PCB_Chip_MARK, false, offsetX, offsetY, offsetTh);
		saveInspImage(CHIP_IMAGE_SAVE, Task.m_iRetry_Opt);
		Task.m_iRetry_Opt++;

		//offsetTh=0;
		if (iRtn == 0)
		{
			Task.d_Align_offset_x[PCB_Chip_MARK] -= offsetX;
			Task.d_Align_offset_y[PCB_Chip_MARK] -= offsetY;
			Task.d_Align_offset_th[PCB_Chip_MARK] -= offsetTh;
			

			Task.dPcbAlignTh = Task.d_Align_offset_th[PCB_Chip_MARK];

			logStr.Format("PCB Sensor Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
			//pcbDlg->m_labelAlignResult.SetText(logStr);
			//pcbDlg->m_labelAlignResult.Invalidate();

			putListLog(logStr);

			int iRtnVal = AlignLimitCheck(CAM2-1, offsetX, offsetY, offsetTh);

			if(iRtnVal==1)
			{
				iRtnFunction = 26625;				// º¸Á¤ ÀÌµ¿
			}
			else if(iRtnVal==2 )
			{
				logStr.Format("		PCB Sensor Align [%d] ¿Ï·á [%d]", Task.m_iRetry_Opt, iUseStep);
				putListLog(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);			//ÀÓ½Ã µî·Ï ¸¶Å© »èÁ¦

				logStr.Format("		PCB Sensor Align Complete : %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
				AlignResultSave(logStr);	
				Task.m_iRetry_Opt = 0;

				Task.m_timeChecker.Measure_Time(4);	//Laser º¯À§ ÃøÁ¤ ½ÃÀÛ ½Ã°£
				
				iRtnFunction = 26629;		// ¿Ï·á//iRtnFunction = 16630;				// ¿Ï·á
				LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align¸¸ Á¶¸í ON
			}
			else
			{
				logStr.Format("PCB Sensor Align [%d] º¸Á¤ ¹üÀ§ ÃÊ°ú", Task.m_iRetry_Opt, iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				AlignLogSave(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);
				iRtnFunction = -26620;				// NG
			}
		}
		else
		{
			logStr.Format("PCB Sensor Align [%d] ¸¶Å© ÀÎ½Ä ½ÇÆÐ[%d]", Task.m_iRetry_Opt, iUseStep);
			putListLog(logStr);
			Sleep(300);
			iRtnFunction = 26620;				//  Àç°Ë»ç 
		}
		break;
	case 26625://- Sensor Align Retry
		if(motor.Pcb_Holder_Align_Move(Align_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 26615;
		}
		else
		{
			logStr.Format("PCB Sensor Align º¸Á¤ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -26625;
		}
		break;
	case 26629:
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCBºÎ zÃà ¸ðÅÍ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			logStr.Format(sLangChange, iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -26629;	
			break;
		}
		iRtnFunction = 26990;
		break;
	case 26990://! Lens-ZÃà ´ë±â À§Ä¡ ÀÌµ¿
#if (____AA_WAY == PCB_TILT_AA)
		if(sysData.m_iEpoxyLaserInspPass == 1)
		{
			iRtnFunction = 30000;	//µµÆ÷ÀÌµ¿
		}
		else
		{
			iRtnFunction = 27000;	//LaserÃøÁ¤ ->Align -> µµÆ÷
		}
#elif (____AA_WAY == LENS_TILT_AA)
		if (sysData.m_iEpoxyLaserInspPass == 1)
		{
			iRtnFunction = 30000;//Laser ÃøÁ¤ Pass
		}else
		{
			iRtnFunction = 27000;
		}
#endif
		break;
	default:
		logStr.Format("Lens ¿ÜºÎ Align Thread Step ¹øÈ£ ºñÁ¤»ó [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	logStr.Empty();
	return iRtnFunction;
}

int	CAABonderDlg::RunProc_LensAlign(int iUseStep)			//(35000 ~ 39000)
{
	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;	//PCB ¿µ»ó È¹µæ Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser ÃøÁ¤Àü Delay
	switch( iUseStep )
	{
	case 35000:
		checkMessage();
		offsetX = offsetY = offsetTh = 0.0;
		if(!motor.PCB_Z_Motor_Move(Wait_Pos)) 
		{
			logStr.Format("		PCB ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -35000;	
			break;
		}
		iRtnFunction = 35050;
		break;
	case 35050:
		Task.m_iRetry_Opt = 0;
		//iRtnFunction = 35100;  //BENZ_3M JUMP
		iRtnFunction = 35400;
		break;

	case 35100:
		Task.m_bOKLensPass = -1;//PCB¾ð·Îµå °úÁ¤¿¡¼­ ³Ñ±è»óÅÂ·Î ¸¸µé¾î³õÀ¸¹Ç·Î ÃÊ±âÈ­.
		Task.d_Align_offset_x[LENS_Align_MARK] = 0;
		Task.d_Align_offset_y[LENS_Align_MARK] = 0;
		Task.d_Align_offset_th[LENS_Align_MARK] = 0;


		if( Task.m_bOKLensPass != 1 )//! Lens ³Ñ±è ¹Ì¿Ï·á ÀÏ °æ¿ì	 
		{
			iRtnFunction = 35150;
		}else
		{
			logStr.Format("Lens ³Ñ±è ¿Ï·á·Î Lens Align Pass![%d]", iUseStep);
  			putListLog(logStr);
			iRtnFunction = 35400;
		}
		break;

	case 35150:
		ctrlSubDlg(MAIN_DLG);
		m_iCurCamNo =0;
		//SetDigReference(LENS_Align_MARK);
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_LENS]);		// Align¸¸ Á¶¸í ON
		if(motor.Pcb_Motor_Move(Lens_Pos))
		{
			if(motor.PCB_Z_Motor_Move(Lens_Pos))
			{
				logStr.Format("PCB Lens_Pos Align À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
  				putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
				
				iRtnFunction = 35200;
			}
		}
		else
		{
			logStr.Format("PCB Sensor Align À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -35150;
		}
		break;
	case 35200:
		checkMessage();
		if((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
		{
			iRtnFunction = 35250;
		}
		break;

	case 35250: 

		if (Task.m_iRetry_Opt>iAlignRetry)
		{
			logStr.Format("PCB Align Àç°Ë»ç %d È¸ ½ÇÆÐ[%d]\n º¸Á¤ ¹ÌÀû¿ëÀ¸·Î ÁøÇàÇÏ½Ã°Ú½À´Ï±î?", Task.m_iRetry_Opt, iUseStep);
			if ( askMsg(logStr) == IDOK)
			{
				Task.d_Align_offset_x[LENS_Align_MARK] = 0;
				Task.d_Align_offset_y[LENS_Align_MARK] = 0;
				Task.d_Align_offset_th[LENS_Align_MARK] = 0;
				iRtnFunction = 35400;
			}
			else
			{
				Task.m_iRetry_Opt = 0;
				iRtnFunction = 35250;
				logStr.Format("PCB Lens Align Àç°Ë»ç ½ÃÀÛ[%d]", iUseStep);
				putListLog(logStr);

				//iRtnFunction = -35250;
				//logStr.Format("PCB Align Àç°Ë»ç %d È¸ ½ÇÆÐ[%d]", Task.m_iRetry_Opt, iUseStep);
				//errMsg2(Task.AutoFlag,logStr);
			}
			break;
		}

		offsetX = offsetY = offsetTh = 0.0;

		iRtn = procCamAlign(CAM1, LENS_Align_MARK, false, offsetX, offsetY, offsetTh);
		saveInspImage(LENS_IMAGE_SAVE, Task.m_iRetry_Opt);
		Task.m_iRetry_Opt++;
		offsetTh=0;
		if (iRtn == 0)
		{
			Task.d_Align_offset_x[LENS_Align_MARK] -= offsetX;
			Task.d_Align_offset_y[LENS_Align_MARK] -= offsetY;
			Task.d_Align_offset_th[LENS_Align_MARK] = 0.0;//offsetTh;

			Task.dPcbAlignTh = Task.d_Align_offset_th[LENS_Align_MARK];

			logStr.Format("PCB Lens Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
			//pcbDlg->m_labelAlignResult.SetText(logStr);
			//pcbDlg->m_labelAlignResult.Invalidate();

			putListLog(logStr);

			int iRtnVal = AlignLimitCheck(CAM1, offsetX, offsetY, offsetTh);

			if(iRtnVal==1)
			{
				iRtnFunction = 35300;				// º¸Á¤ ÀÌµ¿
			}
			else if(iRtnVal==2 )
			{
				logStr.Format("		PCB Lens Align [%d] ¿Ï·á [%d]", Task.m_iRetry_Opt, iUseStep);
				putListLog(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);			//ÀÓ½Ã µî·Ï ¸¶Å© »èÁ¦

				logStr.Format("		PCB Lens Align Complete : %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
				AlignResultSave(logStr);	
				Task.m_iRetry_Opt = 0;

				Task.m_timeChecker.Measure_Time(4);	//Laser º¯À§ ÃøÁ¤ ½ÃÀÛ ½Ã°£
				
				iRtnFunction = 35400;		// ¿Ï·á
			}
			else
			{
				logStr.Format("PCB Lens Align [%d] º¸Á¤ ¹üÀ§ ÃÊ°ú", Task.m_iRetry_Opt, iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				AlignLogSave(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);
				iRtnFunction = -35250;				// NG
			}
		}
		else
		{
			logStr.Format("PCB Lens Align [%d] ¸¶Å© ÀÎ½Ä ½ÇÆÐ[%d]", Task.m_iRetry_Opt, iUseStep);
			putListLog(logStr);

			iRtnFunction = 35200;				//  Àç°Ë»ç 
			Sleep(300);
		}
		break;

	case 35300://- Sensor Align Retry
		if(motor.Pcb_Holder_Align_Move(Lens_Pos, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 35200;
		}
		else
		{
			logStr.Format("PCB Sensor Align º¸Á¤ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -35300;
		}
		break;
	case 35400:
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCBºÎ zÃà ¸ðÅÍ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			logStr.Format(sLangChange, iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -35400;	
			break;
		}
		iRtnFunction = 35500;
		break;
	case 35500://! Lens-ZÃà ´ë±â À§Ä¡ ÀÌµ¿
		iRtnFunction = 39000;
		break;
	default:
		logStr.Format("Lens ¿ÜºÎ Align Thread Step ¹øÈ£ ºñÁ¤»ó [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	logStr.Empty();
	return iRtnFunction;
}

int CAABonderDlg::RunProc_EpoxyNewResing(int iUseStep)
{//! Epoxy µ¿ÀÛ ¹× µµÆ÷ °Ë»ç Step(17000 ~ 17999)
	int iRtnFunction = iUseStep;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;		//PCB ¿µ»ó È¹µæ Delay
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser ÃøÁ¤Àü Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;

	switch( iUseStep )
	{
		case 30000:
			if(!motor.PCB_Z_Motor_Move(Wait_Pos))
			{
				sLangChange.LoadStringA(IDS_STRING1012);	//PCB_ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ.
				logStr.Format(sLangChange, iUseStep);
  				putListLog(logStr);
				iRtnFunction = -30000;
				break;
			}

			if((sysData.m_iDispensePass == 1) || (Task.m_bOkDispense == 1) )	
			{
					iRtnFunction = 31000;	//-- µµÆ÷ Pass Mode ÀÌ°Å³ª, µµÆ÷ ¿Ï·á ÀÏ °æ¿ì
			}else																
			{
					iRtnFunction = 30020;
			}
			break;
		case 30020: //Dispenser -ZÃà ÇÏ°­
			if(motor.PCB_Z_Motor_Move(Wait_Pos))
			{
  				iRtnFunction = 30030;
			}
			else
			{
				logStr.Format("PCB Z Wait À§Ä¡ ÀÌµ¿ ¿Ï·á ½ÇÆÐ [%d]", iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = -30020;
			}
			break;
	case 30030:  //µµÆ÷ À§Ä¡ ÀÌµ¿
		LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align Á¶¸í OFF
		if(motor.Pcb_Motor_Move(Dispense_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			sLangChange.LoadStringA(IDS_STRING935);	//PCB Dispense º¸Á¤ À§Ä¡ ÀÌµ¿ ¿Ï·á-º¸Á¤·® X:%.03lf,Y:%.03lf,T:%.03lf
			logStr.Format(sLangChange, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
  			putListLog(logStr);
  			iRtnFunction = 30040;
		}
		else
  		{
			sLangChange.LoadStringA(IDS_STRING938);	//PCB Dispense À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%
  			logStr.Format(sLangChange + _T("d]"), iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -30030;
  		}
		break;
	case 30040: //Dispenser-z Ãà »ó½Â
		if(motor.PCB_Z_Motor_Move(Dispense_Pos))
		{
			iRtnFunction = 30050;
		}else
		{
			sLangChange.LoadStringA(IDS_STRING977);	//PCB Z Dispense À§Ä¡ ÀÌµ¿ ½ÇÆÐ
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -30040;
		}
		break;

	case 30050: // µð½ºÆæ½º µ¿ÀÛ

		if (sysData.nEpoxyIndex == CRICLE_EPOXY)
		{
			bChk = motor.func_Epoxy_CircleDraw();
		}
		else if (sysData.nEpoxyIndex == RECT_EPOXY)
		{
			bChk = motor.func_Epoxy_Draw();
		}
		else if (sysData.nEpoxyIndex == POLYGON_EPOXY)
		{
			motor.func_Epoxy_Rect_Circle_Draw();
		}
		else if (sysData.nEpoxyIndex == POINT_EPOXY)
		{
			//point Çü
		}

		if(bChk)
		{
			Task.m_bOkDispense = 1;

			logStr.Format("PCB Dispense ¿Ï·á[%d]", iUseStep);
  			putListLog(logStr);

  			iRtnFunction = 30070;
		}
		else
		{
			logStr.Format("PCB Dispense ½ÇÆÐ [%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -30050;
		}
		break;
	case 30070:
		if(motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y))
		{
			LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_SENSOR]);//µµÆ÷ °Ë»çÀü ¾ó¶óÀÎ¸¶Å©¿ë ¶óÀÌÆ®
			iRtnFunction = 30100;
		}
		break;
	case 30100:
		iRtnFunction = 31000;
		break;
	case 31000:
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCBºÎ zÃà ¸ðÅÍ À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			logStr.Format(sLangChange, iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -31000;
			break;
		}else
		{
			logStr.Format("PCB Z ´ë±â À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iUseStep);
  			putListLog(logStr);
		}
		iRtnFunction = 34000;
		/*if( sysData.m_iDispenseInspPass == 0 )	
		{
			iRtnFunction = 32000;

		}else									
		{
			iRtnFunction = 34000;
		}*/
		break;

	case 32000: // µµÆ÷°Ë»ç ÀÌµ¿
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_HOLDER]);		// Align¸¸ Á¶¸í ON
		if (motor.Pcb_Motor_Move(Holder_Pos))
		{
  			iRtnFunction = 32500;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING989);	//PCB µµÆ÷°Ë»ç À§Ä¡ ÀÌµ¿ ½ÇÆÐ
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -32000;
		}
		break;

	case 32500:// ÀÌµ¿ÈÄ, ÀÏÁ¤ ½Ã°£ Áö¿¬ ¹× ¸¶Å© È®ÀÎ
		if(motor.PCB_Z_Motor_Move(Holder_Pos))

		{
			Task.PCBTaskTime = myTimer(true);
			sLangChange.LoadStringA(IDS_STRING990);	//PCB µµÆ÷°Ë»ç À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]
			logStr.Format(sLangChange, iUseStep);
  			putListLog(logStr);
			iRtnFunction = 33000;
		}else
		{
			sLangChange.LoadStringA(IDS_STRING989);	//PCB µµÆ÷°Ë»ç À§Ä¡ ÀÌµ¿ ½ÇÆÐ
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -32500;
		}
		break;
	case 33000: //µµÆ÷°Ë»ç zÃà ÀÌµ¿
		if( (myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)) )
		{
			Sleep(300);
			int iMarkNo = 0;
			CString sTemp="";

			vision.clearOverlay(CAM1);

			if(vision.getLiveMode())
			{
				vision.getSnapImage(CAM1);
				vision.setLiveMode(true);
			}

			double ep1 = 0.0;
			MappTimer(M_TIMER_RESET, &ep1);
			iMarkNo = vision.findMark(CAM1, PCB_Holder_MARK);

			double ep2 = 0.0;
			MappTimer(M_TIMER_READ, &ep2);
			sTemp.Format(" %.3lf sec", ep2 - ep1);
			vision.textlist[CAM1].addList((CAM_SIZE_X-140), (CAM_SIZE_Y-60), sTemp, M_COLOR_GREEN, 24, 10, "Arial");

			vision.drawOverlay(CAM1);

			if (iMarkNo!=-1)
			{
		#ifdef USE_GEOMETRIC
				MgraColor(M_DEFAULT, M_COLOR_GREEN);
				MmodDraw(M_DEFAULT, vision.ModResult[PCB_Holder_MARK][iMarkNo], vision.MilOverlayImage[CAM1], M_DRAW_BOX+M_DRAW_POSITION+M_DRAW_EDGES+M_DRAW_AXIS, M_DEFAULT, M_DEFAULT);

		#endif
			}
			CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
			pFrame->m_rBox.left = 0;
			pFrame->m_rBox.top = 0;
			pFrame->m_rBox.right = 0;
			pFrame->m_rBox.bottom = 0;
			iRtnFunction = 33500;
		}
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_EPOXY_INSP]);
		break;
	case 33500: //µµÆ÷°Ë»ç                                Ãß°¡ÇØ¾ßÇÔ
		Sleep(100);
		vision.clearOverlay(CAM1);
		bChk = _EpoxyFinddispense(CAM1);
		vision.drawOverlay(CAM1);
		if(!bChk)
		{
			putListLog("Epoxy µµÆ÷ °Ë»ç ½ÇÆÐ");
			//logStr.Format("Epoxy Table ÁÂÇ¥ (X:%.03f, Y:%.03f, ¥È:%.03f)", motor.GetEncoderPos(Motor_PCB_X), motor.GetEncoderPos(Motor_PCB_Y), motor.GetEncoderPos(Motor_PCB_TH));
			//putListLog(logStr);
			logStr.Format("Epoxy µµÆ÷ °Ë»ç ½ÇÆÐ. \nÀÚµ¿ ¿îÀü ÁøÇà ÇÏ½Ã°Ú½À´Ï±î?");
			//_stprintf_s(motor.m_szLogMsg, "Epoxy µµÆ÷ °Ë»ç ½ÇÆÐ. \nÀÚµ¿ ¿îÀü ÁøÇà ÇÏ½Ã°Ú½À´Ï±î?");

			Dio.setAlarm(ALARM_ON);
			if (askMsg(logStr))
			{
				Task.PCBTaskTime = myTimer(true);
				Dio.setAlarm(ALARM_OFF);
				Dio.setAlarm(AUTO_RUN);
			}
			else
			{
				Dio.setAlarm(AUTO_STOP);
				logStr.Format("Epoxy µµÆ÷ °Ë»ç ½ÇÆÐ[%d]", iUseStep);
				iRtnFunction = -33500;
				break;
			}
		}
		Task.m_timeChecker.Measure_Time(3);	//µµÆ÷ ¿Ï·á ½Ã°£
		Task.m_dTime_Epoxy = Task.m_timeChecker.m_adTime[3] - Task.m_timeChecker.m_adTime[2];
		dispGrid();

		logStr.Format("PCB µµÆ÷°Ë»ç ¿Ï·á[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 34000;
		break;
	case 34000:
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING1012);	//PCB_ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ.
			logStr.Format(sLangChange, iUseStep);
  			putListLog(logStr);
			iRtnFunction = -34000;
			break;
		}
#if (____AA_WAY == PCB_TILT_AA)
		iRtnFunction = 39000;
#elif (____AA_WAY == LENS_TILT_AA)
		iRtnFunction = 35000;
#endif
		break;
	default:
		sLangChange.LoadStringA(IDS_STRING939);	//PCB Dispenser Thread Step ¹øÈ£ ºñÁ¤»ó .
		logStr.Format(sLangChange + _T(" [%d]"), iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}

	return iRtnFunction;
}

int CAABonderDlg::RunProc_InspAAPos(int iUseStep) 
{
	//! Lens Pickup¿¡ Á¦Ç° ³Ñ±è Step	(18000 ~ 19999)

	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = "";
	double	dFinalDelay = (double)(model.strInfo_Cam[3].m_iDelayTime);
	bool bChk = true;
	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;

	switch( iUseStep )
	{
		case 39000:
#if (____AA_WAY == PCB_TILT_AA)
			iRtnFunction = 41000;
#elif (____AA_WAY == LENS_TILT_AA)
			iRtnFunction = 40000;
#endif		
			break;
			vision.clearOverlay(CAM1);
			vision.drawOverlay(CAM1);
            //if (sysData.m_iDefectInspPass == 1)	// DEFECT °Ë»ç PASS
            //{
            //    iRtnFunction = 39999;
            //}
            //else
            //{
            //    iRtnFunction = 39100;
            //}
			//if(model.m_IsUseBrightDefectTest == 1)	// ±¤¿ø Defect Picxel °Ë»ç À¯¹«

			iRtnFunction = 39700;//aa¶§ defect Pass
			break;
		case 39100:
			LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, model.m_iLedValue[LEDDATA_6500K]);
			if( !motor.Pcb_Motor_Move(OC_6500K_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]) )
			{
				logStr.Format("PCBºÎ ¸ðÅÍ 0C 6500K °Ë»ç À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iUseStep);
				putListLog(logStr);
				iRtnFunction = -39100;
			}
			else
			{
				iRtnFunction = 39300;
			}
			break;
		case 39300:
			if( !motor.PCB_Z_Motor_Move(OC_6500K_Pos) )
			{
				logStr.Format(_T("PCB Z axis 0C 6500K Pos Move Failed [%d]"), iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = -39300;
				break;
			}
			else
			{
				logStr.Format(_T("PCB Z axis 0C 6500K Pos Move Complete [%d]"), iUseStep);
				//errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = 39350;
				break;
			}


		case 39350: //UVÀü OC Defect °Ë»ç 
            Sleep(100);
			MIU.func_Set_InspImageCopy(MID_6500K_RAW);
            logStr.Format("MID_6500K_RAW SAVE OK [%d]", iUseStep);
            putListLog(logStr);
            Sleep(500);

			LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, 0);
			iRtnFunction = 39400;
			break;

		case 39400:
			if( !motor.PCB_Z_Motor_Move(Wait_Pos) )
			{
				logStr.Format(_T("PCB Z axis Defect Pos Move Failed [%d]"), iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = -39400;
				break;
			}
			else
			{
				logStr.Format(_T("PCB Z axis Defect Pos Move Complete [%d]"), iUseStep);
				//errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = 39450;
				break;
			}

		case 39450:	//Dark °Ë»ç À§Ä¡ ÀÌµ¿
			iRtnFunction = 39500;
			//if(model.m_IsUseDarkDefectTest == 1)	// ¾Ï¸· Defect Picxel °Ë»ç À¯¹«
			//{
			//	iRtnFunction = 18102;				//¾Ï¸· °Ë»ç À§Ä¡ ÀÌµ¿
			//}
			//else
			//{
			//	iRtnFunction = 18115;				
			//}
			break;

		case 39500:	//Dark °Ë»ç À§Ä¡ ÀÌµ¿
			if( !motor.Pcb_Motor_Move(Dark_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]) )
			{
				sLangChange.LoadStringA(IDS_STRING1022);	//PCBºÎ ¸ðÅÍ Dark °Ë»ç À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
				logStr.Format(sLangChange, iUseStep);
				putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
				iRtnFunction = -39500;
			}
			else
			{
				LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, 0);
				LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);
				iRtnFunction = 39600; 
			}
			break;

		case 39600:
			if( !motor.PCB_Z_Motor_Move(Dark_Pos) )
			{
				logStr.Format(_T("PCB Z axis Dark Pos Move Failed [%d]"), iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = -39600;
			}
			else
			{
				logStr.Format(_T("PCB Z axis Dark Pos Move Complete [%d]"), iUseStep);
				//errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = 39620;
			}
			break;
	
		case 39620:
			Sleep(500);
			MIU.func_Set_InspImageCopy(LOW_LEVEL_RAW);
			logStr.Format("LOW_LEVEL_RAW SAVE OK [%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 39650;
			break;
		case 39650:	
			if(g_clPriInsp.func_Insp_Defect(MIU.vDefectMidBuffer_6500K, MIU.vDefectLowBuffer, false) == false)
			{	
                logStr.Format("[AUTO] Defect °Ë»ç ½ÇÆÐ [%d]", iUseStep);
				MandoInspLog.bInspRes = false;
			}

			logStr.Format("[AUTO] Defect °Ë»ç ¿Ï·á [%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 39700;
			break;

		case 39700:
			if( !motor.PCB_Z_Motor_Move(Wait_Pos) )
			{
				logStr.Format(_T("PCB Z axis Wait Pos Move Failed [%d]"), iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = -39700;
			}
			else 
			{
				iRtnFunction = 39850;
			}
			break;
		case 39850:
			iRtnFunction = 39900;
			Task.PCBTaskTime = myTimer(true);
			break;
		case 39900: // º»µù À§Ä¡ ÀÌµ¿
			iRtnFunction = 39999; 	
			break;
		case 39999:			//¿©±â±îÁö¿È 180706_1
			//if( func_MIU_ConnectLiveCheck() == false)		//MIU ¿µ»ó Check
			//{
			//	iRtnFunction = -39999;
			//	break;
			//}
#if (____AA_WAY == PCB_TILT_AA)
			iRtnFunction = 41000;
#elif (____AA_WAY == LENS_TILT_AA)
			iRtnFunction = 40000;
#endif		
			break;
		default:
			logStr.Format( "Pcb Align µ¿ÀÛ Thread Step ¹øÈ£ ºñÁ¤»ó . [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -1;
			break;
	}
	logStr.Empty();
	return iRtnFunction;
}

int CAABonderDlg::RunProc_Bonding_Pos_GO(int iUseStep)
{//! Lens Pickup¿¡ Á¦Ç° ³Ñ±è Step	(18000 ~ 19999)
	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = "";
	double	dFinalDelay = (double)(model.strInfo_Cam[3].m_iDelayTime);
	bool bChk = true;
	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;

	switch( iUseStep )
	{
		case 41000:
			if (sysData.m_iIrChartUse == 1)
			{
				IrvAlignLed.Irv_Light_SetValue(model.m_iLedValue[LEDDATA_TOP1_CHART], IR_CHART);
			}
			else
			{

			}

		

			iRtnFunction = 41050;
			break;
		case 41050:
			if( !motor.PCB_Z_Motor_Move(Wait_Pos) )
			{
				logStr.Format(_T("PCB Z axis Wait Pos Move Failed [%d]"), iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = -41050;
			}
			else
			{
				iRtnFunction = 41100;
			}
			break;
		case 41100:
			iRtnFunction = 41150; 	
			Task.PCBTaskTime = myTimer(true);
			break;
		case 41150: // º»µù À§Ä¡ ÀÌµ¿
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 41200;
			break;
		case 41200:
			if (motor.Lens_Motor_MoveXY(Bonding_Pos))
			{

#if (____AA_WAY == LENS_TILT_AA)
				if (!motor.Lens_Tilt_Motor_Move(Bonding_Pos))
				{
					logStr.Format("Lens %s Tx,TyÀÌµ¿ ½ÇÆÐ", MotorPosName[Bonding_Pos]);
					delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
					iRtnFunction = -41200;
					break; 
				}

#endif
				logStr.Format("Lens Bonding À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iRtnFunction);
				putListLog(logStr);
				Task.interlockPcb = 1;
				iRtnFunction = 41250;
			}
			else
			{
				logStr.Format("Lens Bonding À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iRtnFunction);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -41200;
				break;
			}
			break;
		case 41250:
#if (____AA_WAY == PCB_TILT_AA)
			if (motor.Pcb_Motor_Move(Bonding_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
			{
				sLangChange.LoadStringA(IDS_STRING913);	//"PCB Bonding À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]"
				logStr.Format(sLangChange, iUseStep);
				putListLog(logStr);
				iRtnFunction = 41300;
				if (!motor.Pcb_Tilt_Motor_Move(Bonding_Pos))
				{
					logStr.Format("PCB tx,ty Bonding_Pos ÀÌµ¿ ½ÇÆÐ");
					delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
					iRtnFunction = -41250;
					break;
				}
				break;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING912);	//PCB Bonding À§Ä¡ ÀÌµ¿ ½ÇÆÐ
				logStr.Format(sLangChange + _T("[%d]"), iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -41250;
				break;
			}
#elif	(____AA_WAY == LENS_TILT_AA)
			if (motor.Pcb_Motor_Move(Bonding_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
			{
				sLangChange.LoadStringA(IDS_STRING913);	//"PCB Bonding À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]"
				logStr.Format(sLangChange, iUseStep);
				putListLog(logStr);
				iRtnFunction = 41300;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING912);	//PCB Bonding À§Ä¡ ÀÌµ¿ ½ÇÆÐ
				logStr.Format(sLangChange + _T("[%d]"), iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -41250;
			}
#endif
			break;
		case 41300:
			if (motor.Pcb_Tilt_Trinity_Move(Task.m_PcbLaserAfterTx, Task.m_PcbLaserAfterTy))
			{
				iRtnFunction = 41310;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING912);	//PCB Bonding À§Ä¡ ÀÌµ¿ ½ÇÆÐ
				logStr.Format(sLangChange + _T("[%d]"), iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -41250;
			}
			

			break;
		case 41310:
			if(Task.interlockPcb == 1)		//pcbÆ¿Æ®ÀÏ °æ¿ì  ·»Áî ·¹ÀÌÀú,¾ó¶óÀÎ ÀÛ¾÷ ³¡³­ÈÄ º»µùÀ§Ä¡ ¿Ã¶§±îÁö ±â´Ù·Á¾ßµÅ¼­
			{
				iRtnFunction = 41350;
			}else if((myTimer(true) - Task.PCBTaskTime) > 50000)
			{
				logStr.Format("Lens Bonding À§Ä¡ ÀÌµ¿ ½Ã°£ ÃÊ°ú[%d]", iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = -41310;
			}
			break;
		case 41350:
			iRtnFunction = 41400;

			break;
		case 41400:
			//if( func_MIU_ConnectLiveCheck() == false)		//MIU ¿µ»ó Check
			//{
			//	iRtnFunction = -41400;
			//	break;
			//}

			setCamDisplay(3, 1);
			iRtnFunction = 48000;

			break;
		case 48000:
			LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align¸¸ Á¶¸í ON

			logStr.Format("Loading End[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 49000;
			break;
			//
		default:
			logStr.Format( "Pcb Align µ¿ÀÛ Thread Step ¹øÈ£ ºñÁ¤»ó . [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -1;
			break;
	}
	logStr.Empty();
	return iRtnFunction;
}
int CAABonderDlg::RunProc_LENS_AlignLaserMeasure(int iLensStep)
{
	int iRtnFunction = iLensStep;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;		//PCB ¿µ»ó È¹µæ Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;

	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser ÃøÁ¤Àü Delay

	switch (iLensStep)
	{
	case 30000:
		Task.oldLaserTx = 0.1;
		Task.oldLaserTy = 0.1;
		Task.d_Align_offset_x[LENS_Align_MARK] = 0;
		Task.d_Align_offset_y[LENS_Align_MARK] = 0;
		Task.d_Align_offset_th[LENS_Align_MARK] = 0;
		//
		Task.m_iRetry_Opt = 0;
		if (motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 30100;
		}
		else
		{
			logStr.Format("Lens_ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ.[%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -30000;
		}

		break;
	case 30100:
#if (____AA_WAY == PCB_TILT_AA)
		iRtnFunction = 30500;
#elif (____AA_WAY == LENS_TILT_AA)
		if (motor.Lens_Motor_MoveXY(Wait_Pos))
		{
			iRtnFunction = 43000;//41900;		//·»ÁîÂÊ Ä«¸Þ¶ó ¾øÀ½
		}
		else
		{
			logStr.Format("Lens ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iLensStep);
			putListLog(logStr);
			iRtnFunction = -30050;
		}
		break;
		
#endif

		break;
	case 30500:
		iRtnFunction = 30600;

		break;
	case 30600:
		iRtnFunction = 30900;

		break;
	case 30900:
#if (____AA_WAY == PCB_TILT_AA)
		if (sysData.m_iLaserInspPass == 1)	//·»Áî
		{
			iRtnFunction = 41100;			//Laser ÃøÁ¤ Pass
		}
		else
		{
			iRtnFunction = 31000;		// 31300;//¾ó¶óÀÎ ÀÓ½Ã ÆÐ½º
		}

#elif (____AA_WAY == LENS_TILT_AA)
		iRtnFunction = 43000;//41900;		//·»ÁîÂÊ Ä«¸Þ¶ó ¾øÀ½
#endif

		break;
	case 31000:
		if (motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 31050;
		}
		else
		{
			logStr.Format("Lens_ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ.[%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31000;
		}

		
		
		break;

	case 31050:
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_LENS]);
		if (motor.Lens_Motor_MoveXY(Lens_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			logStr.Format("Lens Align À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iLensStep);
			putListLog(logStr);
			iRtnFunction = 31060;
		}
		else
		{
			logStr.Format("Lens Align À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31050;
		}
		break;
	case 31060:
		if (motor.LENS_Z_Motor_Move(Lens_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			logStr.Format("Lens Align Z À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iLensStep);
			putListLog(logStr);
			iRtnFunction = 31080;
		}
		else
		{
			logStr.Format("Lens Align Z À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31060;
		}
		break;

	case 31080:
		checkMessage();
		if ((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_Lens_X) && motor.IsStopAxis(Motor_Lens_Y)))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 31100;
		}
		break;

	case 31100: // lens Align
		if (Task.m_iRetry_Opt > iAlignRetry)
		{
			logStr.Format("Lens Align Àç°Ë»ç %d È¸ ½ÇÆÐ[%d]\n º¸Á¤ ¹ÌÀû¿ëÀ¸·Î ÁøÇàÇÏ½Ã°Ú½À´Ï±î?", Task.m_iRetry_Opt, iLensStep);
			if (askMsg(logStr) == IDOK)
			{
				Task.d_Align_offset_x[LENS_Align_MARK] = 0;
				Task.d_Align_offset_y[LENS_Align_MARK] = 0;
				Task.d_Align_offset_th[LENS_Align_MARK] = 0;
				iRtnFunction = 31300;
			}
			else
			{
				iRtnFunction = -31100;
				logStr.Format("Lens Align Àç°Ë»ç %d È¸ ½ÇÆÐ[%d]", Task.m_iRetry_Opt, iLensStep);
				errMsg2(Task.AutoFlag, logStr);
			}
			break;
		}

		offsetX = offsetY = offsetTh = 0.0;
		Sleep(200);
		iRtn = procCamAlign(CAM2 - 1, LENS_Align_MARK, false, offsetX, offsetY, offsetTh);
		saveInspImage(LENS_IMAGE_SAVE, Task.m_iRetry_Opt);

		Task.m_iRetry_Opt++;
		offsetY = 0;
		if (iRtn == 0)
		{
			Task.d_Align_offset_x[LENS_Align_MARK] -= offsetX;
			Task.d_Align_offset_y[LENS_Align_MARK] += offsetY;
			Task.d_Align_offset_th[LENS_Align_MARK] = 0;//+= offsetTh;	//Lens´Â º¸Á¤·® Theta°¡ ¾øÀ½

			logStr.Format("Lens Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
			//pcbDlg->m_labelAlignResult.SetText(logStr);
			//pcbDlg->m_labelAlignResult.Invalidate();

			putListLog(logStr);

			int iRtnVal = AlignLimitCheck(CAM2, offsetX, offsetY, 0);
			if (iRtnVal == 1)
			{
				iRtnFunction = 31200;				// º¸Á¤ ÀÌµ¿
			}
			else if (iRtnVal == 2)
			{
				logStr.Format("		Lens Align [%d] ¿Ï·á [%d]", Task.m_iRetry_Opt, iLensStep);
				putListLog(logStr);
				InstantMarkDelete(LENS_Align_MARK);			//ÀÓ½Ã µî·Ï ¸¶Å© »èÁ¦

				logStr.Format("		Lens Align Complete : %d, %.03f, %.03f, %.03f", Task.m_iLensPickupNoCentering, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
				AlignResultSave(logStr);
				Task.m_iRetry_Opt = 0;
				iRtnFunction = 31250;// 31300;				// ¿Ï·á
			}
			else
			{
				logStr.Format("Lens Align [%d] º¸Á¤ ¹üÀ§ ÃÊ°ú[%d]", Task.m_iRetry_Opt, iLensStep);
				errMsg2(Task.AutoFlag, logStr);
				AlignLogSave(logStr);
				InstantMarkDelete(LENS_Align_MARK);
				iRtnFunction = -31100;				// NG
			}
		}
		else
		{
			logStr.Format("Lens Align [%d] ¸¶Å© ÀÎ½Ä ½ÇÆÐ[%d]", Task.m_iRetry_Opt, iLensStep);
			putListLog(logStr);
			iRtnFunction = 31100;					//  Àç°Ë»ç 
		}
		break;

	case 31200://!! º¸Á¤·® ÀÌµ¿.- Retry
		if (motor.Lens_Motor_Align_Move(Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 31080;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING657);	//Lens Align º¸Á¤ À§Ä¡ ÀÌµ¿ ½ÇÆÐ
			logStr.Format(sLangChange + _T("[%d]"), iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31200;
		}
		break;
	case 31250:
		LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);
		if (motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 31300;
		}
		else
		{
			logStr.Format("Lens_ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ.[%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31250;
		}
		break;
	case 31300://! Lens-ZÃà ´ë±â À§Ä¡ ÀÌµ¿
		iLaser_Pos = 0;
		if (motor.Lens_Motor_Move_Laser(iLaser_Pos, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]))
		{
			Task.LensTaskTime = myTimer(true);
			logStr.Format("Laser º¯À§ ÃøÁ¤ ¿ÜºÎ À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iLensStep);	//Laser º¯À§ ÃøÁ¤ ¿ÜºÎ À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]
			putListLog(logStr);
			iRtnFunction = 40000;
		}
		else
		{
			logStr.Format("Laser º¯À§ ÃøÁ¤ ¿ÜºÎ À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31300;
		}
		break;
	case 40000://! Laser-ZÃà ¿ÜºÎ ÃøÁ¤À§Ä¡ ÀÌµ¿
		if (motor.LENS_Z_Motor_Move(Laser_Lens_Pos))
		{
			Task.LensTaskTime = myTimer(true); 
			iRtnFunction = 40100;
		}
		else
		{
			logStr.Format("Laser-ZÃà º¯À§ ÃøÁ¤ ¿ÜºÎ À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40000;
		}
		break;

	case 40100:
		iRtnFunction = 40200;
		break;

	case 40200://! Lens Stage ¿ÜºÎ Laser ÃøÁ¤À§Ä¡ ÀÌµ¿ 
		if (motor.Lens_Motor_Move_Laser(iLaser_Pos, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]))
		{
			Task.LensTaskTime = myTimer(true);
			logStr.Format("Laser º¯À§ ÃøÁ¤ ¿ÜºÎ À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]", iLensStep);	//Laser º¯À§ ÃøÁ¤ ¿ÜºÎ À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]
			putListLog(logStr);
			if (iLaser_Pos == 0) { iRtnFunction = 40210; }//Ã³À½¸¸ ÇÏ°­¸í·É.
			else { iRtnFunction = 40300; }
		}
		else
		{
			logStr.Format("Laser º¯À§ ÃøÁ¤ ¿ÜºÎ À§Ä¡ ÀÌµ¿ ½ÇÆÐ [%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40200;
		}
		break;

		//Laser ½Ç¸°´õ ÇÏ°­
	case 40210:
		if (Dio.LaserCylinderUPDOWN(false, false))
		{
			logStr.Format("LASER ½Ç¸°´õ ÇÏ°­ ¿Ï·á[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 40220;
		}
		else
		{
			logStr.Format("LASER ½Ç¸°´õ ÇÏ°­ ½ÇÆÐ [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40210;
		}
		break;

	case 40220:
		if (Dio.LaserCylinderCheck(false, false))
		{
			logStr.Format("LASER ½Ç¸°´õ ÇÏ°­ È®ÀÎ ¿Ï·á[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 40300;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER ½Ç¸°´õ ÇÏ°­ ½Ã°£ ÃÊ°ú..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40220;
		}
		break;
	case 40300://! ÃøÁ¤ Àü Delay
		checkMessage();
		if ((myTimer(true) - Task.LensTaskTime) > iLaserDelay && (motor.IsStopAxis(Motor_Lens_X) && motor.IsStopAxis(Motor_Lens_Y)))	//ÃÖÁÖÀÓ´Ô, Delay È®ÀÎ
		{
			iRtnFunction = 41000;
		}
		break;
		//Laser ½Ç¸°´õ ÇÏ°­ END

	case 41000: //¿ÜºÎ º¯À§ °¨Áö (ÀúÀå µû·Î ÇÏ°í Align ÀÌµ¿)
		//Keyence.func_LT9030_Scan(Task.m_Laser_Point[iLaser_Pos]);
		Keyence.func_CL3000_Scan(Task.m_Laser_Point[iLaser_Pos]); 
		logStr.Format("º¯À§ ¼¾¼­ %lf[%d] - ¿ÜºÎ - %d À§Ä¡", Task.m_Laser_Point[iLaser_Pos], iLensStep, iLaser_Pos+ 1);
		putListLog(logStr);
		LaserPos[iLaser_Pos].x = motor.GetEncoderPos(Motor_Lens_X) - sysData.dDataset[Motor_Lens_X];
		LaserPos[iLaser_Pos].y = motor.GetEncoderPos(Motor_Lens_Y) - sysData.dDataset[Motor_Lens_Y];
		LaserValue[iLaser_Pos] = Task.m_Laser_Point[iLaser_Pos];
		if (Task.m_Laser_Point[iLaser_Pos] != 0)
		{
			if (iLaser_Pos == 3)
			{
				if (func_Check_LaserValueErr(LaserValue) == false)
				{
					sLangChange.LoadStringA(IDS_STRING639);	//Laser ÃøÁ¤°ª ÀÌ»ó..¿ÜºÎ º¯À§ ÃøÁ¤°ªÀÌ ÀÌ»ó..[%d]\n ÃøÁ¤°ª:%.04lf, %.04lf, %.04lf, %.04lf
					logStr.Format(sLangChange, iLensStep, LaserValue[0], LaserValue[1], LaserValue[2], LaserValue[3]);
					errMsg2(Task.AutoFlag, logStr);
					iRtnFunction = -41000;
					break;
				}

				if (_calcLaserTilt(LaserPos, LaserValue, Task.d_Align_offset_xt[LENS_Align_MARK], Task.d_Align_offset_yt[LENS_Align_MARK]))
				{
					Task.m_dataOffset_x[LENS_Align_MARK] = Task.d_Align_offset_xt[LENS_Align_MARK];
					Task.m_dataOffset_y[LENS_Align_MARK] = Task.d_Align_offset_yt[LENS_Align_MARK];
	

					sLangChange.LoadStringA(IDS_STRING1264);		//º¸Á¤·® Tx: %.04lf, Ty: %.04lf
					logStr.Format(_T("		") + sLangChange, Task.d_Align_offset_xt[LENS_Align_MARK], Task.d_Align_offset_yt[LENS_Align_MARK]);
					putListLog(logStr);

					int Rnd = TiltAlignLimitCheck(Task.m_dataOffset_x[LENS_Align_MARK], Task.m_dataOffset_y[LENS_Align_MARK]);
					//20160117 ÀÌÇü¼® ¼öÁ¤

					if (Rnd == 2)
					{

						Task.m_timeChecker.Measure_Time(5);	//Laser º¯À§ ÃøÁ¤ ¿Ï·á ½Ã°£
						Task.m_dTime_LaserDpm = Task.m_timeChecker.m_adTime[5] - Task.m_timeChecker.m_adTime[4];
						dispGrid();
						MandoInspLog.dTilteOffset[0] = Task.m_dataOffset_x[LENS_Align_MARK];	//¸¸µµ Â÷·®¿ëCamera °Ë»ç Log ÀúÀå
						MandoInspLog.dTilteOffset[1] = Task.m_dataOffset_y[LENS_Align_MARK];


						//20141217 LHC - DataBase¿¡ 0°ªÀÌ ÀúÀåµÇ´Â °æ¿ì°¡ »ý°Ü 0°ªÀÌ µé¾î°¥ °æ¿ì ÀúÀåÇÏÁö¾Ê°í ¾Ë¶÷Ä¡µµ·Ï.
						//±è¿µÈ£ 20150602 FraneeGrabber  °ü·Ã ÀÓ½Ã Pass
						if (Task.m_dataOffset_x[LENS_Align_MARK] == 0 || Task.m_dataOffset_y[LENS_Align_MARK] == 0)
						{
							sLangChange.LoadStringA(IDS_STRING627);	//Laser º¯À§ Â÷ °ª ÀÌ»ó ¹ß»ý..
							logStr.Format(sLangChange + _T("Xt : %lf, Yt : %lf"), Task.m_dataOffset_x[LENS_Align_MARK], Task.m_dataOffset_y[LENS_Align_MARK]);
							errMsg2(Task.AutoFlag, logStr);
							iRtnFunction = -41000;
							break;
						}
						if (!g_ADOData.func_AATaskToRecordLaser(Task.ChipID, Task.m_dataOffset_x[LENS_Align_MARK], Task.m_dataOffset_y[LENS_Align_MARK], Task.m_Laser_Point))
						{
							sLangChange.LoadStringA(IDS_STRING489);	//DataBase Laser º¯À§ ÃøÁ¤ Data ±â·Ï ½ÇÆÐ.[%d]\n MS Office¸¦ ´Ý¾ÆÁÖ¼¼¿ä.
							logStr.Format(sLangChange, iLensStep);
							errMsg2(Task.AutoFlag, logStr);
							iRtnFunction = -41000;
							break;
						}

						LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_LENS]);		// Align¸¸ Á¶¸í ON
						Task.LensTaskTime = myTimer(true);
						iRtnFunction = 41100;
						iLaser_Pos++;

					}
					else if (Rnd == 1)
					{
						iRtnFunction = 41050;
					}
					else
					{
						logStr.Format("Laser Tilt  º¸Á¤°ª Limit¸¦ ÃÊ°ú ÇÏ¿´½À´Ï´Ù.[%d]", iLensStep);
						errMsg2(Task.AutoFlag, logStr);
						iRtnFunction = -41000;
					}
				}
			}
			else
			{
				iRtnFunction = 40200;
				iLaser_Pos++;
			}
		}
		else
		{
			iRtnFunction = 41000;
		}
		break;

	case 41050: // ÀÌµ¿  º¯À§°ªÀÌ 0À¸·Î ¸¸µé±â À§ÇØ ÀÌµ¿
		if (motor.Lens_Move_Tilt(Task.d_Align_offset_xt[LENS_Align_MARK], Task.d_Align_offset_yt[LENS_Align_MARK]))
		{
			iRtnFunction = 40200; 
			iLaser_Pos = 0;//4;

		}
		break;

	case 41100:
		iRtnFunction = 41110;
		break;

		//laser½Ç¸°´õ »ó½Â
	case 41110:
		if (Dio.LaserCylinderUPDOWN(true, false))
		{
			logStr.Format("LASER ½Ç¸°´õ »ó½Â ¿Ï·á[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 41120;
		}
		else
		{
			logStr.Format("LASER ½Ç¸°´õ »ó½Â ½ÇÆÐ [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41110;
		}
		break;

	case 41120:
		if (Dio.LaserCylinderCheck(true, false))
		{
			logStr.Format("LASER ½Ç¸°´õ »ó½Â È®ÀÎ ¿Ï·á[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 41200;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER ½Ç¸°´õ »ó½Â ½Ã°£ ÃÊ°ú..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41120;
		}
		break;
		//laser½Ç¸°´õ »ó½ÂEND

	case 41200://! Lens-ZÃà ´ë±â À§Ä¡ ÀÌµ¿
		if (motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 41850;;//// 41300; //41850;//ÀÓ½Ã ·»Áî align pass
								 //iRtnFunction = 41300;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING739);	//Lens_ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ
			logStr.Format(sLangChange + _T("[%d]"), iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41200;
		}
		break;

	case 41300: 
		if (motor.Lens_Motor_MoveXY(Lens_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			sLangChange.LoadStringA(IDS_STRING659);	//Lens Align ÀÌµ¿ [%d]
			logStr.Format(sLangChange, iLensStep);
			putListLog(logStr);
			iRtnFunction = 41400;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING660);	//Lens Align ÀÌµ¿ ½ÇÆÐ[%d]
			logStr.Format(sLangChange, iLensStep);
			putListLog(logStr);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41300;
		}
		
		break;
	case 41400:

		if (motor.LENS_Z_Motor_Move(Lens_Pos))
		{
			Task.d_Align_offset_x[LENS_Align_MARK] = 0;
			Task.d_Align_offset_y[LENS_Align_MARK] = 0;
			Task.d_Align_offset_th[LENS_Align_MARK] = 0;
			Task.LensTaskTime = myTimer(true);
			sLangChange.LoadStringA(IDS_STRING661);	//Lens Align À§Ä¡ ÀÌµ¿ ¿Ï·á[%d]
			logStr.Format(sLangChange, iLensStep);
			putListLog(logStr);
			iRtnFunction = 41500;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING660);	//Lens Align À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			logStr.Format(sLangChange, iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41400;
		}
		break;

	case 41500:
		checkMessage();
		if ((myTimer(true) - Task.LensTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_Lens_X) && motor.IsStopAxis(Motor_Lens_Y)))
		{
			iRtnFunction = 41600;
			Task.m_iRetry_Opt = 0;
		}
		break;

	case 41600://! LENS Align(º¯À§ÃøÁ¤ º¸Á¤ÈÄ, Shift °è»ê ¿ëµµ)
		if (sysData.m_FreeRun == 1)
		{
			for (int i = 0; i<100; i++)
			{
				Sleep(10);
				checkMessage();
			}

			iRtnFunction = 41700;
			Task.d_Align_offset_x[LENS_Align_MARK] = 0;
			Task.d_Align_offset_y[LENS_Align_MARK] = 0;
			Task.d_Align_offset_th[LENS_Align_MARK] = 0;
			break;
		}
		if (Task.m_iRetry_Opt > iAlignRetry)
		{
			logStr.Format("Lens Align Àç°Ë»ç %d È¸ ½ÇÆÐ[%d]\n º¸Á¤ ¹ÌÀû¿ëÀ¸·Î ÁøÇàÇÏ½Ã°Ú½À´Ï±î?", Task.m_iRetry_Opt, iLensStep);
			if (askMsg(logStr) == IDOK)
			{
				Task.d_Align_offset_x[LENS_Align_MARK] = 0;
				Task.d_Align_offset_y[LENS_Align_MARK] = 0;
				Task.d_Align_offset_th[LENS_Align_MARK] = 0;
				iRtnFunction = 41800;
			}
			else
			{
				iRtnFunction = -41600;
				logStr.Format("Lens Align Àç°Ë»ç %d È¸ ½ÇÆÐ[%d]", Task.m_iRetry_Opt, iLensStep);
				errMsg2(Task.AutoFlag, logStr);
			}
			
			break;
		}
		offsetX = offsetY = offsetTh = 0.0;
		Sleep(200);
		iRtn = procCamAlign(CAM2-1, LENS_Align_MARK, false, offsetX, offsetY, offsetTh);

		saveInspImage(LENS_IMAGE_SAVE, Task.m_iRetry_Opt);

		Task.m_iRetry_Opt++;

		if (iRtn == 0)
		{
			Task.d_Align_offset_x[LENS_Align_MARK] -= offsetX;
			Task.d_Align_offset_y[LENS_Align_MARK] += offsetY;
			Task.d_Align_offset_th[LENS_Align_MARK] = 0;//+= offsetTh;	//Lens´Â º¸Á¤·® Theta°¡ ¾øÀ½

			sLangChange.LoadStringA(IDS_STRING651);	//Lens Align : %.3lf %.3lf %.3lf
			logStr.Format("Lens Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
			//			Sleep(500);
			//pcbDlg->m_labelAlignResult.SetText(logStr);
			//pcbDlg->m_labelAlignResult.Invalidate();

			putListLog(logStr);

			int iRtnVal = AlignLimitCheck(CAM2, offsetX, offsetY, 0);
			if (iRtnVal == 1)
			{
				iRtnFunction = 41700;				// º¸Á¤ ÀÌµ¿
			}
			else if (iRtnVal == 2)
			{
				sLangChange.LoadStringA(IDS_STRING654);	//Lens Align [%d] ¿Ï·á [%d]
				logStr.Format(_T("		") + sLangChange, Task.m_iRetry_Opt, iLensStep);
				putListLog(logStr);

				InstantMarkDelete(LENS_Align_MARK);			//ÀÓ½Ã µî·Ï ¸¶Å© »èÁ¦

				sLangChange.LoadStringA(IDS_STRING655);	//Lens Align Complete : %d, %.03f, %.03f, %.03f
				logStr.Format(_T("		") + sLangChange, Task.m_iLensPickupNoCentering, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
				AlignResultSave(logStr);
				Task.m_iRetry_Opt = 0;
				iRtnFunction = 41800;				// ¿Ï·á
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING653);	//Lens Align [%d] º¸Á¤ ¹üÀ§ ÃÊ°ú
				logStr.Format(sLangChange + _T("[%d]"), Task.m_iRetry_Opt, iLensStep);
				errMsg2(Task.AutoFlag, logStr);
				AlignLogSave(logStr);
				InstantMarkDelete(LENS_Align_MARK);
				iRtnFunction = -41600;				// NG
			}
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING652);	//Lens Align [%d] ¸¶Å© ÀÎ½Ä ½ÇÆÐ[%d]
			logStr.Format(sLangChange, Task.m_iRetry_Opt, iLensStep);
			putListLog(logStr);

			iRtnFunction = 41600;					//  Àç°Ë»ç 
		}
		break;

	case 41700://!! º¸Á¤·® ÀÌµ¿.- Retry
		if (motor.Lens_Motor_Align_Move(Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 41500;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING657);	//Lens Align º¸Á¤ À§Ä¡ ÀÌµ¿ ½ÇÆÐ
			logStr.Format(sLangChange + _T("[%d]"), iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41700;
		}
	case 41800: // °ª ÀúÀå2

		//!!!!! 1Â÷ Align º¸Á¤·® ÆíÂ÷ ÀúÀå °ø½Ä(PCB<-> Lens Sensor°£)
		Task.m_dataOffset_x[1] = Task.d_Align_offset_x[LENS_Align_MARK];
		Task.m_dataOffset_y[1] = Task.d_Align_offset_y[LENS_Align_MARK];
		//		Task.m_dataOffset_th[1] = Task.d_Align_offset_th[LENS_Align_MARK] - Task.d_Align_offset_th[PCB_Chip_MARK];

		//sLangChange.LoadStringA(IDS_STRING962);	//PCB Sensor_Holder°£ Align ÆíÂ÷°ª : X:%.03f, Y:%.03f, ¥È:%.04f [%d]
		//logStr.Format(_T("		") + sLangChange, Task.m_dataOffset_x[1], Task.m_dataOffset_y[1],Task.m_dataOffset_th[1], iLensStep);
		//putListLog(logStr);

		//¸¸µµ Â÷·®¿ëCamera °Ë»ç Log ÀúÀå
		MandoInspLog.dLensOffset[0] = Task.m_dataOffset_x[1];
		MandoInspLog.dLensOffset[1] = Task.m_dataOffset_y[1];
		MandoInspLog.dLensOffset[2] = Task.m_dataOffset_th[1];


		iRtnFunction = 41850;
		//if (Dio.CamLaserSlinderMove(false, true))
		//{
		//	logStr.Format("Laser/Cam ½Ç¸°´õ ÈÄÁø ¿Ï·á[%d]", iLensStep);
		//	//putListLog(logStr);
		//	Task.LensTaskTime = myTimer(true);
		//	iRtnFunction = 41850;
		//}
		//else
		//{
		//	logStr.Format("Laser/Cam ½Ç¸°´õ ÈÄÁø ½ÇÆÐ[%d]", iLensStep);
		//	errMsg2(Task.AutoFlag, logStr);
		//	iRtnFunction = -41800;
		//}

		// 20141217 LHC - sensor_holder°£ align ÆíÂ÷ °ª¿¡ 0°¡ µé¾î°¡¸é Á¤ÁöÇÏ°Ô²û
		//±è¿µÈ£ 20150602 FraneeGrabber  °ü·Ã ÀÓ½Ã Pass
		//if(Task.m_dataOffset_x[1] == 0 || Task.m_dataOffset_y[1] == 0 || Task.m_dataOffset_th[1] == 0)
		//{
		//	sLangChange.LoadStringA(IDS_STRING963);	//PCB Sensor_Holder°£ Align ÆíÂ÷°ª ÀÌ»ó ¹ß»ý : X:%.03f, Y:%.03f, ¥È:%.04f [%d]
		//	logStr.Format(_T("		") + sLangChange, Task.m_dataOffset_x[1], Task.m_dataOffset_y[1],Task.m_dataOffset_th[1], iLensStep);
		//	putListLog(logStr);
		//	errMsg2(Task.AutoFlag,logStr);
		//	iRtnFunction = -41800;
		//	break;
		//}

		//-- ADO DB ÀúÀå
		//g_ADOData.func_AATaskToRecordAlign(Task.ChipID, MandoInspLog.dLensOffset[0], MandoInspLog.dLensOffset[1], MandoInspLog.dLensOffset[2]);

		break;
	case 41850:
		iRtnFunction = 41870;
		//if (Dio.CamLaserSlinderMove(false, true))
		//{
		//	logStr.Format("Laser/Cam ½Ç¸°´õ ÈÄÁø ¿Ï·á[%d]", iLensStep);
		//	//putListLog(logStr);
		//	Task.LensTaskTime = myTimer(true);
		//	iRtnFunction = 41870;
		//}
		//else
		//{
		//	logStr.Format("Laser/Cam ½Ç¸°´õ ÈÄÁø ½ÇÆÐ[%d]", iLensStep);
		//	errMsg2(Task.AutoFlag, logStr);
		//	iRtnFunction = -41850;
		//}
		break;
	case 41870:
		iRtnFunction = 41900;
		//if (Dio.CamLaserSlinderCheck(false, false))
		//{
		//	logStr.Format("Laser/Cam ½Ç¸°´õ ÈÄÁø È®ÀÎ ¿Ï·á[%d]", iLensStep);
		//	putListLog(logStr);
		//	iRtnFunction = 41900;
		//}
		//else if ((myTimer(true) - Task.LensTaskTime) > 10000)//IO_DELAY_TIME)
		//{
		//	logStr.Format("Laser/Cam ½Ç¸°´õ ÈÄÁø ½Ã°£ ÃÊ°ú[%d]", iLensStep);
		//	errMsg2(Task.AutoFlag, logStr);
		//	iRtnFunction = -41870;
		//}
		break;
	case 41900://! Lens-ZÃà ´ë±â À§Ä¡ ÀÌµ¿
		if (motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 41950;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING739);	//Lens_ZÃà ´ë±âÀ§Ä¡ ÀÌµ¿ ½ÇÆÐ
			logStr.Format(sLangChange + _T("[%d]"), iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41900;
		}
		break;
	case 41950:
		iRtnFunction = 42000;

		
		break;
	case 42000: // º»µù À§Ä¡ ÀÌµ¿
		if (motor.Lens_Motor_MoveXY(Bonding_Pos, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK]))
		{
			sLangChange.LoadStringA(IDS_STRING659);	//Lens Align ÀÌµ¿ [%d]
			logStr.Format(sLangChange, iRtnFunction);
			putListLog(logStr);
			iRtnFunction = 42200;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING664);	//Lens Bonding À§Ä¡ ÀÌµ¿ ½ÇÆÐ[%d]
			logStr.Format(sLangChange, iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -42000;
		}
		break;
	case 42200:
		//Á×½º ¸ðµ¨  , lens º»µùÀ§Ä¡¿¡¼­ zÃà ´ë±âÀ§Ä¡¿¡¼­ pcb º»µùÀ§Ä¡·Î ¸øµé¾î¿Í¼­ zÃà Á» µé¾î¿Ã·Á³ö¾ßµÊ
		iRtnFunction = 43000;
		break;
	case 42400:
		if ((myTimer(true) - Task.LensTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_Lens_Z)))
		{
			iRtnFunction = 43000;
		}
		break;
	case 43000:
		Task.interlockPcb = 1;
		Task.interlockLens = 1;
		iRtnFunction = 50000;
		break;
	default:
		sLangChange.LoadStringA(IDS_STRING628);	//Laser º¯À§ ÃøÁ¤ Thread Step ¹øÈ£ ºñÁ¤»ó .
		logStr.Format(sLangChange + _T("[%d]"), iLensStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}

	return iRtnFunction;
}

void CAABonderDlg::AdjustDisplaySize(unsigned char* pFrameBuffer, unsigned char* p2byteBuffer, RawImgInfo* pRawImgInfo)
{
	if( pRawImgInfo->nSensorWidth != pRawImgInfo->nDisplaySizeX || pRawImgInfo->nSensorHeight != pRawImgInfo->nDisplaySizeY )
	{
		for( int y = pRawImgInfo->nDisplayStartPosY ; y < pRawImgInfo->nDisplayStartPosY+pRawImgInfo->nDisplaySizeY ; y++ )
		{
			memcpy( p2byteBuffer + (y-pRawImgInfo->nDisplayStartPosY)*pRawImgInfo->nDisplaySizeX*2, 
				&pFrameBuffer[0]+(y*pRawImgInfo->nSensorWidth*2 + pRawImgInfo->nDisplayStartPosX), pRawImgInfo->nDisplaySizeX*2 );
		}
	}
}

int CAABonderDlg::LoadImageData(unsigned char* pDataBuffer, const char * _Filename)
{
	FILE* _File;
	fopen_s(&_File, _Filename, "rb");
	fseek(_File, 0, SEEK_END);
	int _lengthByte = ftell(_File);
	rewind(_File);

	fread((BYTE*)pDataBuffer, sizeof(BYTE), _lengthByte, _File);
	fclose(_File);

	return _lengthByte;
}

bool	CAABonderDlg::func_inspVisual()	// Distortion,Blemish,Blotch,Stain,Defect Pixel,Optical Centering,Distortion Centering°Ë»ç ¿ÜºÎDLL »ç¿ë
{
	return true;
}

void CAABonderDlg::OnBnClickedButtonProcomp()
{
	if(Task.AutoFlag==1 )
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//ÀÚµ¿ ¿îÀü Áß ÀÔ´Ï´Ù.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
	}
	else if(Task.AutoFlag==0 || Task.AutoFlag==2)
	{	
		if(sysData.m_iProductComp == 0)
		{
			sLangChange.LoadStringA(IDS_STRING1308);	//¿ÏÁ¦Ç° ¸ðµå·Î º¯°æ ÇÏ½Ã°Ú½À´Ï±î?
			if(askMsg(sLangChange))
			{
				MIU.INI_LOAD();
				sysData.m_iProductComp = 1;
				sysData.Save();
			}
		}
		else if(sysData.m_iProductComp == 1)	
		{
			sLangChange.LoadStringA(IDS_STRING376);
			if(askMsg(sLangChange))
			{
				MIU.INI_LOAD();
				m_bProComp = false;
				sysData.m_iProductComp = 0;
				sysData.Save();
			}
		} 
	}
}


void CAABonderDlg::UnpackRaw10(unsigned char *src, unsigned short *dst, int rows, int cols)
{
	int nNewWidth = cols * 5 / 4;

	for(int j = 0; j < rows; j++)
	{
		for(int i = 0; i < cols; i += 4)
		{
			dst[j * cols + i + 0] = ((unsigned short)src[j * nNewWidth + (i * 5 / 4) + 0] << 2) | ((unsigned short)(src[j * nNewWidth + (i * 5 / 4) + 4] & 0xC0) >> 6);
			dst[j * cols + i + 1] = ((unsigned short)src[j * nNewWidth + (i * 5 / 4) + 1] << 2) | ((unsigned short)(src[j * nNewWidth + (i * 5 / 4) + 4] & 0x30) >> 4);
			dst[j * cols + i + 2] = ((unsigned short)src[j * nNewWidth + (i * 5 / 4) + 2] << 2) | ((unsigned short)(src[j * nNewWidth + (i * 5 / 4) + 4] & 0x0C) >> 2);
			dst[j * cols + i + 3] = ((unsigned short)src[j * nNewWidth + (i * 5 / 4) + 3] << 2) | ((unsigned short)(src[j * nNewWidth + (i * 5 / 4) + 4] & 0x03) >> 0);
		}
	}
}

int I2C_Read(int iAddr)
{
	unsigned short iValue = 0;

	//MIUReadRegister(gDeviceIndex, (MIU_REG_TYPE)REG_IIC1, (unsigned short)iAddr, (unsigned short*)&iValue);

	return iValue;
}


int I2C_Write(int iAddr,int iValue)
{
	return 1;//MIUWriteRegister(gDeviceIndex, (MIU_REG_TYPE)REG_IIC1, iAddr, iValue);
}


CString CAABonderDlg::getSensorID()
{
	char LotID[255];
	char Wafer[255];
	char Chip[255];
	char Fab[255];
	char Spec[255];
	char SensorID[255];

	I2C_Write(0x0A02, 0x1F);
	I2C_Write(0x0A00, 0x01);
	Sleep(100);
//	while(I2C_Read(0x0A01) != 0x01);

	sprintf(LotID, "%02X%02X%02X%02X%02X", I2C_Read(0x0A22)
				, I2C_Read(0x0A23)
				, I2C_Read(0x0A24)
				, I2C_Read(0x0A25)
				, I2C_Read(0x0A26));
	sprintf(Wafer, "%02X", I2C_Read(0x0A27));
	sprintf(Chip, "%02X%02X", I2C_Read(0x0A28), I2C_Read(0x0A29));
	sprintf(Fab, "%02X%02X", I2C_Read(0x0A2A), I2C_Read(0x0A2B));
	sprintf(Spec, "%02X", I2C_Read(0x0A2C));

	sprintf(SensorID, "%s%s%s%s%s", LotID, Wafer, Chip, Fab, Spec);
	
	CString sValue;
	sValue=SensorID;
	return sValue;
}


bool CAABonderDlg::func_Insp_CurrentMeasure(bool bLogDraw, bool bAutoMode)
{
	return false;
}


void CAABonderDlg::OnStnClickedLabelId()
{
	CKeyPadDlg keyDlg;
	CString strTemp;
	strTemp.Format("IDÀ» ÀÔ·ÂÇÏ¼¼¿ä.");
	//keyDlg.m_strKeyPad = strTemp;
	keyDlg.m_strKeyPad.Format("%s", Task.ChipID);
	if (keyDlg.DoModal()==IDOK)
	{
		strTemp = keyDlg.GetstrKeypad();
		int strLength = strTemp.GetLength();
		if(strLength<1)
		{
			strTemp.Format("ID¸íÀÌ ºñÁ¤»óÀÔ´Ï´Ù.");
			errMsg2(Task.AutoFlag, strTemp);
			return;
		}
		Task.m_bPBStart = 1;	//¹ÙÄÚµå Á¤»óÀûÀ¸·Î ÀÐÇô Á³À»¶§.		 
		sprintf_s(Task.ChipID, strTemp, sizeof(strTemp) );
		sprintf_s(Task.sNum, strTemp, sizeof(strTemp) );

		 m_labelCCD_ID.SetText(strTemp);
	}
	else
	{
		return;
	}
}



void CAABonderDlg::OnBnClickedAutorunStart()
{
	Start_Btn_On = true;
	Sleep(1000);
	Start_Btn_On = false;
}


void CAABonderDlg::OnBnClickedDoorOpen()
{
	Dio.DoorLift(true, false);

	
}
bool CAABonderDlg::uart_ThreadFn()
{
	pThread_CCDCheck = ::AfxBeginThread(Thread_CCDCheck, this);
	
	return false;
}


void CAABonderDlg::OnBnClickedDoorClose()
{
	Dio.DoorLift(false, false);//
}


void CAABonderDlg::OnStnClickedLabelStatusMes()
{
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
	CString sMsg = "";

	barcode.func_Comm_Close();
	sMsg.Format("Barcode Close");
	Task.bConnectBarcode = false;
	theApp.MainDlg->putListLog(sMsg);
	Sleep(600);
	//
	MESConnectToServer();
}


void CAABonderDlg::OnStnClickedLabelModelname()
{
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.

//	CString sTemp;
//	CString str;
//	model.d_SIDE_MODEL_CH++;
//	if (model.d_SIDE_MODEL_CH > 1)
//	{
//		model.d_SIDE_MODEL_CH = 0;
//	}
//	if (model.d_SIDE_MODEL_CH == SIDE_FRONT)
//	{
//		sTemp.Format(" SIDE FRONT");
//	}
//	else
//	{
//		sTemp.Format("SIDE BACK");
//	}
//	DispCurModelName(sTemp);// model.name);
//
//	str.Format("SIDE MODEL CHANGE [%s]", sTemp);
//	theApp.MainDlg->putListLog(str);
//	model.Save();
//}
}


void CAABonderDlg::ChartRoiReset()
{
#ifdef ON_LINE_VISION
	ccdDlg->m_pSFRDlg->ComboxValueSet();
	vision.clearOverlay(CCD);
	ccdDlg->m_pSFRDlg->drawRectSFR();
	vision.drawOverlay(CCD);
#endif
}
void CAABonderDlg::OnBnClickedButtonSminiOqmode()
{
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
	if (Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//ÀÚµ¿ ¿îÀü Áß ÀÔ´Ï´Ù.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
	}
	if (sysData.m_iSminiOQMOde == 0)
	{
		sysData.m_iSminiOQMOde = 1;
		m_bSminiOQCheck.m_iStateBtn = 1;
	}
	else
	{
		sysData.m_iSminiOQMOde = 0;
		m_bSminiOQCheck.m_iStateBtn = 0;
	}
	m_bSminiOQCheck.Invalidate();
}
