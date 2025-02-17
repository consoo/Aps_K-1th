




// AA BonderDlg.cpp : ���� ����
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



#include <mmsystem.h>				// ��Ƽ �̵�� Ÿ�̸�..
#pragma comment(lib, "winmm.lib")
MMRESULT		m_mmResult;			//��Ƽ�̵��Ÿ�̸�..



//CInformDlg*		InformDlg;
//CForceAlignDlg* g_pFoceDlg = NULL;
//CTiltingManualDlg* TiltingManualdlg;//Manual ƿ�� ���� �Է�â


//CDPoint		LaserPos[4];
//double		LaserValue[4];
//CPoint cpFiducialPos[4];
//CRect crFiducialRoi[4];

//bool bInsCenter = false;	


/* Thread */
/************************************************************************/

//! Modified by LHW (2013/2/25)
//! ���� ��� �ӵ��� �ø��� ���Ͽ� ���� ���� Thread�� ������ ����� �۾��Ѵ�.
//! 4���� Thread�� �׻� ¦�� �Ǿ�, ����/���� �Ǿ�� �Ѵ�.
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


/* ���� ���� */
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
	//		theApp.MainDlg->putListLog("USB ���� ȹ�� TimeOut �߻�");
	//		//MIU.Close();					// 95 ~ 100 msec
	//		errMsg2(Task.AutoFlag, "USB ���� ȹ�� TimeOut �߻�.");
	//	}
	//	break;

	//case MIU_EVENT_ATTACHED:
	//	{
	//		theApp.MainDlg->putListLog("USB attached Event");
	//		TRACE("USB attached Event \n");
	//		/*if(!MIU.Open())	
	//		{
	//			theApp.MainDlg->m_bMiuRun = false;
	//			theApp.MainDlg->putListLog("MIU Open ����.");
	//		}*/
	//		//int iSize_Device = MIU.Search_Device();
	//		//if ( iSize_Device > 0 )
	//		//{
	//		//	if ( gMIUDevice.bMIUOpen == 0 )
	//		//	{
	//		//		//! LPMC-500 Frame Grabber�� PC�� USB 3.0���� ����Ǿ�, PC���� �νĵǾ��� ��
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
	//		//	errMsg2(Task.AutoFlag, "USB ī�޶� ��� ������ ���� �Ǿ����ϴ�.");
	//		//	pFrame->m_labelUsbModule.Invalidate();

	//		//	//! LPMC-500 Frame Grabber�� PC���� ������ ������ ��
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
		//point ��
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
//! Servo ����̹��� Alarm Ȯ��
//! [���� ����] PCI-R1604, PCI-R1604-MLII�� ����� ��������̹� �࿡�� ���
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
	DWORD upAlarmCode;	//! Ȯ�ε� �˶� �ڵ�

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

			//! Servo Alarm ���� Ȯ��
			if ( motor.GetAmpFault(lAxisNo) == false )
			{
				continue;
			}

			//! Servo ����̹��� Alarm�� Ȯ���� �� �ִ� ���� Ȯ��
			dwRet = AxmInfoGetAxis(lAxisNo, &BoardNo, &ModulePos, &ModuleID);
			if ( dwRet != AXT_RT_SUCCESS )
			{
				continue;
			}
			if ( ModuleID != AXT_SMC_R1V04A4 && ModuleID != AXT_SMC_R1V04A5 )
			{
				continue;
			}

			//! �������� �˶� ����Ȯ���� ��û�Ѵ�.
			dwRet = AxmStatusRequestServoAlarm(lAxisNo);

			//! �������� �˶� ����Ȯ�� ��û�� ���� ���� ����� Ȯ���Ѵ�.
			//! uReturnMode : �Լ� ���� ������ ����
			//! - [0] �Լ� ���� �� �ٷ� ��ȯ
			//! - [1] ���������κ��� �˶� �ڵ带 ���� �� ���� ��ȭ���� ����
			//! - [2] ���������κ��� �˶� �ڵ带 ���� �� ���� ��ȭ���� ������ ���α׷� Blocking ���� ����
			uReturnMode = 0;
			dwRet = AxmStatusReadServoAlarm(lAxisNo, uReturnMode, &upAlarmCode);
			if ( dwRet != AXT_RT_SUCCESS )
			{
				//! [4210] AXT_RT_MOTION_READ_ALARM_WAITING : ���������κ��� �˶� �ڵ尡 ���޵��� ����
				//! [4211] AXT_RT_MOTION_READ_ALARM_NO_REQUEST : �˶� �ڵ� �б� ��û ���� ����.
				//! [4212] AXT_RT_MOTION_READ_ALARM_TIMEOUT : �˶� �ڵ� �б��� �ð� �ʰ� (1�� �̻�)
				//! [4213] AXT_RT_MOTION_READ_ALARM_FAILED : �˶� �ڵ� �б� ����(��Ʈ��ũ ���� ������)
				continue;
			}

			//! �������� �˶� �ڵ带 ����Ͽ� �˶� ���뿡 ���� ���ڿ� ���� Ȯ���Ѵ�.
			//! �˶� �ڵ忡 ���� ���ڿ��� �ùٸ��� ǥ���ϱ� ���ؼ��� ���̺귯���� ���� �����Ǵ� �˶� ���ڿ� �϶� ������ 
			//! Axl.dll�� ���� ������ �ְų� OS ��ġ ���丮 ���� System32 �������� �����Ͽ��� �Ѵ�. 
			//! �׷��� ���� ��� �Լ� ���� ����� ��AXT_RT_MOTION_READ_ALARM_FILES���� ���ϵȴ�. 
			//! �˶� �ڵ忡 ���� ���ڿ� ������ EzSoftwre ��ġ �������� ��AXL(Library)\Library\DefFile���� �ִ�.
			//!  (Alarm-A4N.def, Alarm-A5N.def)
			dwRet = AxmStatusGetServoAlarmString(lAxisNo, upAlarmCode, iBufSize_GetAlarm, szGetAlarm);
			if ( dwRet != AXT_RT_SUCCESS )
			{
				//! [4220] AXT_RT_MOTION_READ_ALARM_UNKNOWN : �߸��� �˶� �ڵ�
				//! [4221] AXT_RT_MOTION_READ_ALARM_FILES : �˶� �ڵ忡 �ش��ϴ� ���ڿ� ������ �ε���� ����.
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
//! CCM ���� ���� Thread
//! [���� ����] Model�� ����Ǹ�, Thread�� �ݰ�, �ٽ� ������Ѿ� �մϴ�.
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
		sMessage.Format(_T("MIL ���� ���� ȣ�� ũ�Ⱑ �������Դϴ�."));
		errMsg2(Task.AutoFlag,sMessage);
		return 1;
	}

	vision.MiuBufferFree();

	if(!vision.MiuBufferAlloc(gMIUDevice.nWidth, gMIUDevice.nHeight))
	{
		errMsg2(Task.AutoFlag,"MIL ���� ���� ���� ����.");
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
				//! �ش� ������ ������ ���� ȭ�鿡 ǥ������ ����
				Sleep(5);
				continue;
			}


			//! ���� ��� ������ ���°� �ӽ� ���� 
			//! [���� ����] 1 Frame�� �� ���� ���� Display�ϴ� ��ɿ� �ʿ�, Process�� ���� ������ �߿��ϴ�.
			if ( gMIUDevice.CurrentState == 3)
			{
				gMIUDevice.CurrentState = 2;
			}

			//! ���� ��� ���� Thread�� ��� �ð� ���� ����
			MIU.Init_Grab_Time(0);
			MIU.Start_Grab_Time(0);

			//! ���� ��� �ð��� �˾Ƴ���.
			::GetLocalTime( &(MIU.m_aTemp[ MIU.m_iIndex_Grab_Working ].TimeGrab) );
			try
			{
				//! ���� ���
				if (MIU.m_pBoard->GetFrame(MIU.m_pFrameRawBuffer, MIU.m_pFrameBMPBuffer))
				{
					//gMIUDevice.imageGrayItp->imageData = (char*)MIU.m_pFrameBMPBuffer; 
					gMIUDevice.imageItp->imageData = (char*)MIU.m_pFrameBMPBuffer;
					//cvSaveImage("D:\\m_pFrameBMPBuffer.bmp", gMIUDevice.imageGrayItp);	 
					MIU.Grab_StopImage();

					//! ���� ��� ���� Thread�� ��� �ð� ����
					MIU.Measure_Grab_Time(0);

					//! ���� ��� ���� Thread�� ��� �ð� ���� ����
					MIU.Init_Grab_Time(1);
					MIU.Start_Grab_Time(1);

					//! ���� ����� ��ġ��, �ش� ������ ���� index�� �˷��� �Ŀ�, 
					MIU.m_iIndex_Grab_Ready = MIU.m_iIndex_Grab_Working;
					//! ���� ������ ������ ����� �� �ֵ���, index ����
					(MIU.m_iIndex_Grab_Working)++;
					//! ������ ũ�⸦ �Ѿ��, ���� index�� �ʱ�ȭ
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
//! CCM�ο��� Color ��ȯ �۾��� Thread
//! [���� ����] Model�� ����Ǹ�, Thread�� �ݰ�, �ٽ� ������Ѿ� �մϴ�.
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


			//! ���� ��� ������ ���°� �ӽ� ����
			//! [���� ����] 1 Frame�� �� ���� ���� Display�ϴ� ��ɿ� �ʿ�, Process�� ���� ������ �߿��ϴ�.
			//MIU.m_aTemp[ MIU.m_iIndex_Cvt_Clr_Working ].CurrentState = MIU.m_aTemp[ MIU.m_iIndex_Grab_Ready ].CurrentState;

			//! ���� ó������ ����, ���� ������ �����Ƿ�, 

			//! ���� ��� ���� Thread�� ��� �ð� ����
			MIU.Measure_Grab_Time(1);

			//! ���� ��� ���� Thread�� ��� �ð� ���� ����
			MIU.Init_Grab_Time(2);
			MIU.Start_Grab_Time(2);

			MIU.m_iIndex_Cvt_Clr_Working = MIU.m_iIndex_Grab_Ready;

			MIU.Measure_Grab_Time(2);

			//! ���� ��� ���� Thread�� ��� �ð� ���� ����
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
//! CCM�ο����� Open CV --> Mil �̹��� ���� ��ȯ�� Thread
//! [���� ����] Model�� ����Ǹ�, Thread�� �ݰ�, �ٽ� ������Ѿ� �մϴ�.
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

			//! ���� ��� ������ ���°� �ӽ� ����
			//! [���� ����] 1 Frame�� �� ���� ���� Display�ϴ� ��ɿ� �ʿ�, Process�� ���� ������ �߿��ϴ�.
			//MIU.m_aTemp[ MIU.m_iIndex_Cvt_Mil_Working ].CurrentState = MIU.m_aTemp[ MIU.m_iIndex_Cvt_Clr_Used ].CurrentState;

			MIU.m_iIndex_Cvt_Mil_Working = MIU.m_iIndex_Cvt_Clr_Used;

			//! ���� ��� ���� Thread�� ��� �ð� ����
			MIU.Measure_Grab_Time(3);

			//! ���� ��� ���� Thread�� ��� �ð� ���� ����
			MIU.Init_Grab_Time(4);
			MIU.Start_Grab_Time(4);

			//! RGB ä�� �и�..
			vision.m_acsGrabLock[1].Lock();



			//! ���� ��� �ð��� �����Ѵ�.
			CopySystemTime(MIU.m_aTemp[ MIU.m_iIndex_Cvt_Mil_Working ].TimeGrab, vision.m_aTimeGrab[1]);


			vision.m_acsGrabLock[1].Unlock();

			MIU.Measure_Grab_Time(4);

			//! ���� ��� ���� Thread�� ��� �ð� ���� ����
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
//! CCM�ο��� Color ��ȯ �۾��� Thread (MbufBayer �Լ� ���)
//! [���� ����] Model�� ����Ǹ�, Thread�� �ݰ�, �ٽ� ������Ѿ� �մϴ�.
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

			//! ���� ��� ���� Thread�� ��� �ð� ����
			MIU.Measure_Grab_Time(3);

			//! ���� ��� ���� Thread�� ��� �ð� ���� ����
			MIU.Init_Grab_Time(4);
			MIU.Start_Grab_Time(4);

			//! MbufBayer �Լ� ���, Bayer �̹��� -->> Color �̹���

			vision.m_acsGrabLock[1].Lock();
			//! ���� ��� �ð��� �����Ѵ�.
			CopySystemTime(vision.m_aTimeBayer[ vision.m_iIndex_Cvt_Clr_Ready ], vision.m_aTimeGrab[1]);
			//vision.m_bFlag_Use_WB_Coeff = true;


			//if ( vision.m_bFlag_Use_WB_Coeff == true )
			//{
			//	MbufBayer( vision.MilBayerImage[ vision.m_iIndex_Cvt_Clr_Ready ], vision.MilGrabImage[1], vision.MilWBCoefficients, M_BAYER_RG );				
			//}
			//else 
			//{
			//	//! 'M_ADAPTIVE'�� �����ϸ�, 500 msec�� �ð��� �ɸ���.
			//	MbufBayer( vision.MilBayerImage[ vision.m_iIndex_Cvt_Clr_Ready ], vision.MilGrabImage[1], M_DEFAULT, M_BAYER_RG );		
			//}

			vision.m_csProcImageCV.Unlock();
			vision.m_acsGrabLock[1].Unlock();

			MIU.Measure_Grab_Time(4);

			//! ���� ��� ���� Thread�� ��� �ð� ���� ����
			MIU.Init_Grab_Time(5);
			MIU.Start_Grab_Time(5);

			vision.m_iIndex_Cvt_Clr_Used = vision.m_iIndex_Cvt_Clr_Ready;

			//! Thread_Ccm_Display�� �۵���Ű�� ���ؼ� �ʿ��ϴ�. 
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
//! CCM ���� Display�� Thread
//! [���� ����] Model�� ����Ǹ�, Thread�� �ݰ�, �ٽ� ������Ѿ� �մϴ�.
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

			//! ���� ��� ���� Thread�� ��� �ð� ����
			MIU.Measure_Grab_Time(5);

			MIU.Init_Grab_Time(6);
			MIU.Start_Grab_Time(6);

			//! ���� ��� ������ ���°� �ӽ� ����
			//! [���� ����] 1 Frame�� �� ���� ���� Display�ϴ� ��ɿ� �ʿ�, Process�� ���� ������ �߿��ϴ�.

			//! ���� ó������ ����, ���� ������ �����Ƿ�, 

			dReduceFactorX = (double)SMALL_CCD_SIZE_X / gMIUDevice.nWidth;
			dReduceFactorY = (double)SMALL_CCD_SIZE_Y / gMIUDevice.nHeight;

			MIU.m_iIndex_Display_Working = MIU.m_iIndex_Cvt_Mil_Used;

			pFrame->Update_CCD_Display();

			MIU.Measure_Grab_Time(6);

			//! Display�� Frame Rate ����� ���� ȣ���Ѵ�. 
			MIU.Add_Display_Count();	

			if ( bFlag_First_Grab_Display == false )
			{
				//! Thread�� �����ǰ� ����, ù��°�� ���� ��濡 �����ϴ�.
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

//! Auto Exposure Time ��ü Thread
//! Auto Exposure Time ���� ���� �߿� Flag ������ ���ؼ� �����Ѵ�. 
//! (���� ��� ���� ���θ� Ȯ���ϱ� ���� ���̴�.)
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
		sLangChange.LoadStringA(IDS_STRING1368);	//"�ڵ� ���� �� �Դϴ�."
		delayMsg(sLangChange);
		return 0;
	}

	if( Task.AutoFlag == 2 )
	{
		sLangChange.LoadStringA(IDS_STRING1362);	//"�Ͻ� ���� �� �Դϴ�."
		errMsg2(Task.AutoFlag, sLangChange);
		return 0;
	}

	if ( bThreadTaskReadyRun == true)
	{
		sLangChange.LoadStringA(IDS_STRING1322);	//"���� �غ� ���� �� �Դϴ�."
		errMsg2(Task.AutoFlag, sLangChange);
		return 0;
	}

	if(g_bMovingflag)
	{
		sLangChange.LoadStringA(IDS_STRING1324);	//"���� �غ� ���� - ��� ���� �� �Դϴ�."
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
			sLangChange.LoadStringA(IDS_STRING402);	//"AutoRunFlag OFF ���� . Lens�� step :-1"
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
				Task.ReadyTask = pFrame->Ready_process(Task.ReadyTask);		/* ���� �ε� -> ���� ����� -> ���� ���� ��ġ */	
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING309);	//"[���� �غ�]���� �غ� �Ϸ� �߽��ϴ�."
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
		sLangChange.LoadStringA(IDS_STRING1321);	//"���� �غ� ���� �Ϸ�."
		logStr.Format(sLangChange);
		pFrame->putListLog(logStr);
	}
	else
	{
		sLangChange.LoadStringA(IDS_STRING1325);	//	"���� �غ� ����."
		logStr.Format(sLangChange);
		pFrame->putListLog(logStr);
	}

	if(Task.ReadyTask == 19900)
	{
		bThreadTaskReadyRun = false;		// �����尡 ���� �ߴ��� Ȯ��..

		pFrame->AutoRunView(3);
	}
	else
	{
		bThreadTaskReadyRun = false;		// �����尡 ���� �ߴ��� Ȯ��..

		pFrame->AutoRunView(0);
	}

	Task.AutoFlag = 0;

	bThreadTaskReady = false;			// ������ ���� ���� ��..
	bThreadTaskReadyRun = false;		// �����尡 ���� �ߴ��� Ȯ��..

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
		logStr.Format("CCD ��� ���� �ʱ�ȭ ����.\n ��ǰ ���� ���� �� ��ǰ �ҷ� Ȯ�� �ϼ���.");
		errMsg2(Task.AutoFlag, logStr);
		bThread_MIUCheckRun=false;
		return false;
	}
	Sleep(300);

	///MIU.SwitchHDRToLinearMode();		//zoox �𵨸�

	bThread_MIUCheckRun=false;

	if(Task.PCBTask > 60000 && Task.MUICheckflag == false)
	{
		//sLangChange.LoadStringA(IDS_STRING446);	//"CCD ������ ����."
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
// ���� ���� ���� Ȯ��..

		Task.iHomeErrorFlag = motor.HomeCheck();

		//if(Task.iHomeErrorFlag)
		//{
		//	if(!delayDlg->IsWindowVisible())
		//	{
		//		//sLangChange.LoadStringA(IDS_STRING1399);	//��ü ���� ���͸� �Ϸ����� �� �߽��ϴ�.
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
					sLangChange.LoadStringA(IDS_STRING1399);	//��ü ���� ���͸� �Ϸ����� �� �߽��ϴ�.
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
// ���� ����̺� �˶� Display

		int iAmpfault = 0;
		//for(int i=0;i<MAX_MOTOR_NO;i++)
		//{
		//	//if(motor.GetAmpFault(i))
		//	//{
		//	//	iAmpfault += i+1;
		//	//	if(!delayDlg->IsWindowVisible())
		//	//	{
		//	//		//sLangChange.LoadStringA(IDS_STRING1053);	//Servo Alarm �� ���� �Ǿ� �ֽ��ϴ�.
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
// Limit ���� ���� Display
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
			//	sTempLang.LoadStringA(IDS_STRING152);	//[%s] AMP �˶� ����
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
			Task.AutoFlag = 0;			//	�Ͻ������� ����� 0: �Ͻ����� 1: Run	
			pFrame->AutoRunView(Task.AutoFlag);
			bThreadTaskPcb =0;			//	 ���� �Ҷ� ��� 0:������ ���� ����
			bThreadTaskLens =0;			//	 ���� �Ҷ� ��� 0:������ ���� ����
			bThreadTaskLens_Align =0;			//	 ���� �Ҷ� ��� 0:������ ���� ����
		}

		if(iOldLimitFlag != Task.iLimitErrorFlag)
		{
			if(Task.iLimitErrorFlag ==1 && Task.AutoFlag)
			{
				Dio.setAlarm(ALARM_ON);
				Task.AutoFlag = 0;			//	�Ͻ������� ����� 0: �Ͻ����� 1: Run	
				pFrame->AutoRunView(Task.AutoFlag);
				bThreadTaskPcb =0;			//	 ���� �Ҷ� ��� 0:������ ���� ����
				bThreadTaskLens =0;			//	 ���� �Ҷ� ��� 0:������ ���� ����
				bThreadTaskLens_Align =0;			//	 ���� �Ҷ� ��� 0:������ ���� ����
			}

			iOldLimitFlag = Task.iLimitErrorFlag;
		}

		////////////////////////////////////////////////////////////////////////////////
		// Door Open ����..
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
				Task.iDoorFlag2 = 1;//door ���� �� �ݰ�������
			}
			else
			{
				pFrame->m_LabelDoor.SetBkColor(M_COLOR_GREEN);
				Task.iDoorFlag2 = 0;
			}
			pFrame->m_LabelDoor.Invalidate();	
		}
		iOldDoorOpenFlag = Task.iDoorFlag;


//-- ������� �߿� PCB ������ ���� ������.. ���� �������� �������� ����
		//if(Task.AutoFlag == 0 && gMIUDevice.CurrentState == 4)
		//{
		//	if( !func_Insp_CurrentMeasure(false))
		//	{//������ ����
		//		gMIUDevice.CurrentState = 3;

		//		pFrame->m_csLock_Miu.Lock();
		//		if(!MIU.Stop())
		//		{
		//			pFrame->putListLog(MIU.sLogMessage);
		//			delayMsg(MIU.sLogMessage.GetBuffer(999));
		//		}
		//		else
		//		{
		//			sLangChange.LoadStringA(IDS_STRING1424);	//ī�޶� ��� ��� Close.
		//			pFrame->putListLog(sLangChange);
		//		}

		//		pFrame->m_bMiuRun = false;
		//		pFrame->m_csLock_Miu.Unlock();
		//	}
		//}

//-- ���ڵ� Reading
		CString sBarCode = "";
		if( barcode.func_Barcode_Read(sBarCode) ) 
		{	//���ڵ尡 ���� ������...
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
				{//Loading ���� �� �ν� Ȯ�α����� Data Task�� Push
					Task.m_bPBStart = 1;	//���ڵ� ���������� ���� ������.		 
					sprintf_s(Task.ChipID, sData, sizeof(sData) );
					pFrame->func_ChipID_Draw();	
				}
			}
			else if( Task.AutoFlag == 0 )
			{//����(����)���¿����� �׻� Push
				Task.m_bPBStart = 1;	//���ڵ� ���������� ���� ������.		
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

	//sprintf_s(logMsg, "���ڵ� AA DATA ��� Reading ����..");
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
			//sprintf_s(logMsg, "THREAD ���ۿ䱸.");
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
			{//AA2���� �Ѵ�� ��ġ �� ��� Data �䱸
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
				//sprintf_s(logMsg, "AA���� ��ġ ���� �������� �ʽ��ϴ�.");
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
			{//Data ���� �Ϸ� 
				sLangChange.LoadStringA(IDS_STRING1087);	//Socket ��� ���� �Ϸ�.
				sprintf_s(logMsg, sLangChange);
				pFrame->putListLog(logMsg);
				iRtnStep = 999;
				bThreadSocket = false;
			}
			else if(myTimer(true) - dSocketTime > 10000)
			{
				sLangChange.LoadStringA(IDS_STRING1086);	//Socket ��� ���� �ð� �ʰ�.
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
	//	sprintf_s(logMsg, "���ڵ� AA DATA ��� ����..");
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
	
	sTemp.Format("��ü ���� ���� �����մϴ�");	//��ü ���� ���� �����մϴ�
	delayMsg(sTemp, 50000, M_COLOR_DARK_GREEN);
	theApp.MainDlg->putListLog(sTemp);
	Sleep(100);
	//
	memset(Task.ChipID, 0x00, 256);
	sprintf_s(Task.ChipID, "EMPTY");
	Task.m_bPBStart = 0;	//���ڵ� ���� �ʱ�ȭ
	theApp.MainDlg->func_ChipID_Draw();



	bool bHomeComplete = false;
	bHomeComplete = motor.HomeProcessAll();

	if (bHomeComplete)
	{
		Dio.setAlarm(ALARM_OFF);
		sLangChange.LoadStringA(IDS_STRING1397);	//��ü ���� ���� �Ϸ�
		sTemp.Format(sLangChange);
		delayMsg(sTemp.GetBuffer(99), 50000, M_COLOR_GREEN);
	}
	else
	{
		sLangChange.LoadStringA(IDS_STRING1393);	//��ü ���� ���� ����
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
//		errMsg2(Task.AutoFlag, "PCB �����尡 ���� �� �Դϴ�.");
		pFrame->putListLog("PCB �����尡 ���� �� �Դϴ�.");
		return 0;
	}

	int i_alarm_flag = pFrame->checkAutoRunPcbAlarm(Task.PCBTask);

	if(i_alarm_flag)
	{
		logStr.Format("[���� ����] %s", pFrame->sz_PCB_Error_Msg);
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
		if(Task.AutoFlag != 1)			//0:���� 1: �ڵ� 
		{
			sLangChange.LoadStringA(IDS_STRING403);
			logStr.Format(sLangChange);	//AutoRunFlag OFF ���� . PCB�� step :-1
			pFrame->putListLog(logStr);
			break;
		}
		
		if( Task.PCBTask >= 12500)
		{
			//if( Dio.StartPBOnCheck(true, false) )
			//{
			//	//logStr.Format(_T("�ڵ� ���� �� ���� Door Stop ����ġ�� OFF ���� �Դϴ�."));
			//	sLangChange.LoadStringA(IDS_STRING1373);	//�ڵ� ���� �� ���� Start/Stop ����ġ�� �۵�..\n �ڵ���带 �Ͻ� ���� �մϴ�.
			//	logStr.Format(sLangChange);
			//	errMsg2(Task.AutoFlag, logStr);
			//	break;
			//}
		}
		//if( (sysData.m_iFront == 0))
		//{
		//	if( (Dio.LightCurtainOnCheck(true, false)) && (Task.PCBTask > 11000) )
		//	{//���� Ŀư ���������� ���� �ɶ�
		//		sLangChange.LoadStringA(IDS_STRING1372);	//�ڵ� ���� �� ���� Door Area ����.. �ڵ���� �Ͻ� ����.
		//		logStr.Format(sLangChange);
		//		errMsg2(Task.AutoFlag, logStr);
		//		break;
		//	}
		//}
		//

		////////////////////////////////////////////////////////////////////////////////
		// Door Open ����..
		if( (Task.iDoorFlag2 == 1) && (sysData.iDoorInterlock == 1) )
		{
			logStr.Format("�ڵ� ���� �� Door Open �˶� �߻�.. �ڵ���� �Ͻ� ����.");
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////������
		if ( Task.PCBTask >= 10000 && Task.PCBTask < 11000 )
		{
			Task.PCBTask = pFrame->RunProc_ProductLoading(Task.PCBTask); //! ����� ��ǰ Loading	
		}
		else if ( Task.PCBTask >= 11000 && Task.PCBTask < 49000 )
		{
			if(sysData.m_iProductComp == 1)
			{
					Task.PCBTask = pFrame->procProductComplete(Task.PCBTask);	// �ϼ�ǰ �˻� �� ���
			}else
			{
#if (____AA_WAY == PCB_TILT_AA)
				if( Task.PCBTask >= 11000 && Task.PCBTask < 15000 )			Task.PCBTask = pFrame->RunProc_PCBOutsideAlign(Task.PCBTask);		//! PCB �ܺ� Align Step		
				else if( Task.PCBTask >= 15000 && Task.PCBTask < 26000 )	Task.PCBTask = pFrame->RunProc_LensNewPassPickup(Task.PCBTask);		//! Lens Pickup�� ��ǰ �ѱ� Step
				else if( Task.PCBTask >= 26000 && Task.PCBTask < 27000 )	Task.PCBTask = pFrame->RunProc_SensorAlign(Task.PCBTask);		//! Sensor Align Step + Laser ����
				else if (Task.PCBTask >= 27000 && Task.PCBTask < 29000)		Task.PCBTask = pFrame->RunProc_LaserMeasure(Task.PCBTask);			//pcb ������ ����
				else if( Task.PCBTask >= 30000 && Task.PCBTask < 39000 )	Task.PCBTask = pFrame->RunProc_EpoxyNewResing(Task.PCBTask);		//! Epoxy ���� �� ���� �˻� Step
				else if( Task.PCBTask >= 39000 && Task.PCBTask <= 40000 )	Task.PCBTask = pFrame->RunProc_InspAAPos(Task.PCBTask);			//! Defect �˻��� ������ġ Step
#elif (____AA_WAY == LENS_TILT_AA)
				if( Task.PCBTask >= 11000 && Task.PCBTask < 15000 )			Task.PCBTask = pFrame->RunProc_PCBOutsideAlign(Task.PCBTask);					//Ȧ�� �����
				else if( Task.PCBTask >= 26000 && Task.PCBTask < 27000 )	Task.PCBTask = pFrame->RunProc_SensorAlign(Task.PCBTask);						//���� �����
				else if( Task.PCBTask >= 27000 && Task.PCBTask < 28000 )	Task.PCBTask = pFrame->RunProc_LaserMeasure(Task.PCBTask);						//pcb ������ ����
				else if( Task.PCBTask >= 30000 && Task.PCBTask < 35000 )	Task.PCBTask = pFrame->RunProc_EpoxyNewResing(Task.PCBTask);					//����
				else if( Task.PCBTask >= 35000 && Task.PCBTask < 39000 )	Task.PCBTask = pFrame->RunProc_LensAlign(Task.PCBTask);							//���� �����
				else if (Task.PCBTask >= 39000 && Task.PCBTask < 40000)		Task.PCBTask = pFrame->RunProc_InspAAPos(Task.PCBTask);							//��ũ,����˻�
				else if( Task.PCBTask >= 40000 && Task.PCBTask < 41000 )	Task.PCBTask = pFrame->RunProc_LensLoading(Task.PCBTask);						//���� �ε�
#endif
				else if( Task.PCBTask >= 41000 && Task.PCBTask < 49000 )	Task.PCBTask = pFrame->RunProc_Bonding_Pos_GO(Task.PCBTask);					//���� �ε�
			}
		}
		else if ( Task.PCBTask >= 49000 && Task.PCBTask < 50000 )
		{
			Task.PCBTask = 60000;
			Task.iMTFCnt = 0;
		}
		else if ( Task.PCBTask >= 60000 && Task.PCBTask <80000 )
		{
			Task.PCBTask = pFrame->procAutoFocus(Task.PCBTask);			//Active Align ����
		}
		else if ( Task.PCBTask >= 80000 && Task.PCBTask <110000 )
		{
			Task.iMTFCnt = 0;
			Task.bFirstAA = true;
			Task.PCBTask = 60200;	//O.C Align ���ϰ�... 2�� AA�� ...
			MIU.func_Set_InspImageCopy(CHART_RAW);			//1�� AA�� 
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
			//if(!bThreadTaskLens)	//Dio.setAlarm(ALARM_OFF);	// Lens  ������ ���߸� Yellow	
			Dio.setAlarm(ALARM_OFF);	// Lens  ������ ���߸� Yellow
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
	logStr.Format("PCB AA-Bonding ���� ������ ����.");
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
//		errMsg2(Task.AutoFlag, "Lens �����尡 ���� �� �Դϴ�.");
		return 0;
	}

	int i_alarm_flag = pFrame->checkAutoRunLensAlarm(Task.LensTask);

	if(i_alarm_flag)
	{
		sLangChange.LoadStringA(IDS_STRING297); //[���� ����]
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
		if(Task.AutoFlag != 1)			//0:���� 1: �ڵ� 
		{
			sLangChange.LoadStringA(IDS_STRING403);
			logStr.Format(sLangChange);	//AutoRunFlag OFF ���� . Lens�� step :-1
			pFrame->putListLog(logStr);
			break;
		}
		
		if( Task.LensTask >= 12500)
		{
			//if( Dio.StartPBOnCheck(true, false) )
			//{
			//	//logStr.Format(_T("�ڵ� ���� �� ���� Door Stop ����ġ�� OFF ���� �Դϴ�."));
			//	sLangChange.LoadStringA(IDS_STRING1373);	//�ڵ� ���� �� ���� Start/Stop ����ġ�� �۵�..\n �ڵ���带 �Ͻ� ���� �մϴ�.
			//	logStr.Format(sLangChange);
			//	errMsg2(Task.AutoFlag, logStr);
			//	break;
			//}
		}
		if( (sysData.m_iFront == 0) )
		{
			if( (Dio.LightCurtainOnCheck(true, false)) && (Task.LensTask > 11000) )
			{//���� Ŀư ���������� ���� �ɶ�
				sLangChange.LoadStringA(IDS_STRING1372);	//�ڵ� ���� �� ���� Door Area ����.. �ڵ���� �Ͻ� ����.
				logStr.Format(sLangChange);
				errMsg2(Task.AutoFlag, logStr);
				break;
			}
		}
		////////////////////////////////////////////////////////////////////////////////
		if( (Task.iDoorFlag2 == 1) && (sysData.iDoorInterlock == 1) )
		{
			sLangChange.LoadStringA(IDS_STRING1371);	//�ڵ� ���� �� Door Open �˶� �߻�.. �ڵ���� �Ͻ� ����.
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
		
//�ڡڡڡ�///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		if(Task.LensTask >= 30000 && Task.LensTask < 50000)
		{
			Task.LensTask = pFrame->RunProc_LENS_AlignLaserMeasure(Task.LensTask);		//! LENS Align Step
		}

//�ڡڡڡ�///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
			//if(!bThreadTaskLens)	//Dio.setAlarm(ALARM_OFF);	// Lens  ������ ���߸� Yellow	
//			Dio.setAlarm(ALARM_OFF);	// Lens  ������ ���߸� Yellow
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
	sLangChange.LoadStringA(IDS_STRING900);	//"Lens AA-Bonding ���� ������ ����.
	logStr.Format(_T("Lens AA-Bonding ���� ������ ����."));//sLangChange);
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
{//----- ��¥�� ���÷� Check�Ͽ� ���ʿ��� �����͸� ������Ų��...
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

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CAABonderDlg ��ȭ ����




CAABonderDlg::CAABonderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAABonderDlg::IDD, pParent)
	, m_iCcd(0)
	, bEpoxyTimeChk(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//! <---------------------------------------------------------------------------
	//! Added by LHW (2013/2/5, 2013/3/6)
	m_iMode_Mouse_Box = 0;	//! Mouse�� �簢 ���� �����ϴ� �۾��� ���� ��, 0 based	
		

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


// CAABonderDlg �޽��� ó����

BOOL CAABonderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.
	Start_Btn_On = false;

	LogSave("���α׷� ����.");

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
		errMsg2(Task.AutoFlag, "ī�޶� ���� ���� �ʱ�ȭ ����");
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

	//! MbufBayer �Լ� ��� ���ο� ����, MIU ����� ���� ���� ����� �����ؾ� �Ѵ�.
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
		sprintf_s(logStr, "���� �ʱ�ȭ�� ���� �Ͽ����ϴ�.");
		errMsg2(Task.AutoFlag, logStr);

		sprintf_s(logStr, "���� �ʱ�ȭ�� ���� �Ͽ����ϴ�.");
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

	/* ���� ���� �� DB */
	if(!g_ADOData.func_AA_DBConnect())
	{
		putListLog(" [ ACCESS DB ] Open Fail!");
	}

	//MESConnectToServer();

	/*���� �ð� Timer*/
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
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}
void CAABonderDlg::Rs232Init()
{
	SerialPortList.GetList_SerialPort();
	// ���ڵ� ��Ʈ ����
	CString logStr="";
	if( !barcode.func_Comm_Open(sysData.iCommPort[COMM_BARCODE], sysData.iBaudRate[COMM_BARCODE]) )
	{
		sLangChange.LoadStringA(IDS_STRING263);	//[���ڵ� ������] ��� ���� ����
		logStr.Format("[Barcode] ��� ���� ���� : COM%d" , sysData.iCommPort[COMM_BARCODE]);

		//delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
		putListLog(logStr);
		Task.bConnectBarcode = false;
	}
	else
	{
		//sLangChange.LoadStringA(IDS_STRING262);	//[���ڵ� ������] ��� ���� ����
		logStr.Format("[Barcode] ��� ���� ���� : COM%d", sysData.iCommPort[COMM_BARCODE]);
		Task.bConnectBarcode = true;
		putListLog(logStr);
	}
	bool bRet_Con_RS232C = false;
	CString sCommPort = _T("");
 
	if (sysData.m_iIrChartUse == 1)
	{
		if (!IrvAlignLed.Irv_RS232_CommOpen(sysData.iCommPort[COMM_LIGHT2], sysData.iBaudRate[COMM_LIGHT2], IR_CHART))
		{

			logStr.Format("[IR CHART] ��� ���� ���� : COM%d", sysData.iCommPort[COMM_LIGHT2]);
			//delayMsg(logStr.GetBuffer(99), 50000, M_COLOR_RED);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("[IR CHART] ��� ���� ���� : COM%d", sysData.iCommPort[COMM_LIGHT2]);
			putListLog(logStr);
			Sleep(100);
			IrvAlignLed.IrvLight_Power(1, IR_CHART);
		}

		if (!IrvAlignLed.Irv_RS232_CommOpen(sysData.iCommPort[COMM_LIGHT1], sysData.iBaudRate[COMM_LIGHT1], IR_OC))
		{

			logStr.Format("[IR OC] ��� ���� ���� : COM%d", sysData.iCommPort[COMM_LIGHT1]);
			//delayMsg(logStr.GetBuffer(99), 50000, M_COLOR_RED);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("[IR OC] ��� ���� ���� : COM%d", sysData.iCommPort[COMM_LIGHT1]);
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

		//���� 2
		sCommPort.Format("COM%d", sysData.iCommPort[COMM_LIGHT2]);
		LightControlSecond.SetReceiveProcPtr(this);
		bRet_Con_RS232C = LightControlSecond.Connect_Device(sCommPort, 0);

		//���� 3

		sCommPort.Format("COM%d", sysData.iCommPort[COMM_LIGHT3]);
		LightControlthird.SetReceiveProcPtr(this);
		bRet_Con_RS232C = LightControlthird.Connect_Device(sCommPort, 0);

	}
	

	
	

	
   
   


	

#ifdef ON_LINE_KEYENCE

	if( !Keyence.func_RS232_CommOpen(sysData.iCommPort[COMM_LASER], sysData.iBaudRate[COMM_LASER]) )
	{
		sLangChange.LoadStringA(IDS_STRING275);
		logStr.Format("[���� ������] ��� ���� ���� : COM%d",sysData.iCommPort[COMM_LASER]);
		//delayMsg(logStr.GetBuffer(99), 50000, M_COLOR_RED);
		putListLog(logStr);
	}
	else
	{
		if( !Keyence.func_LT9030_KeyLock(true))
		{
			logStr.Format("[���� ������] ��� �̻� �߻�..Lock ���� Fail..Cable�� Ȯ�� �ϼ���.");
			//delayMsg(logStr.GetBuffer(99), 50000, M_COLOR_RED);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("[���� ������] ��� ���� ���� : COM%d", sysData.iCommPort[COMM_LASER]);
			putListLog(logStr);
		}
		
	}
#endif
	
	if (!UVCommand.Connect_Device(sysData.iCommPort[COMM_UV]))
	{
		logStr.Format("[UV] ��� ���� ���� : COM%d", sysData.iCommPort[COMM_UV]);
		putListLog(logStr);
	}
	else
	{
		logStr.Format("[UV] ��� ���� ���� : COM%d", sysData.iCommPort[COMM_UV]);
		putListLog(logStr);
		Sleep(100);
		//UVCommand.UV_Shutter_Open();//
		UVCommand.UV_Shutter_PowerSet(model.UV_Power);// 95);//
		logStr.Format("	#1 UV POWER SET: %d", model.UV_Power);
		putListLog(logStr);

	}
	if (!UVCommand2.Connect_Device(sysData.iCommPort[COMM_UV2]))
	{
		logStr.Format("[UV 2] ��� ���� ���� : COM%d", sysData.iCommPort[COMM_UV2]);
		putListLog(logStr);
	}
	else
	{
		logStr.Format("[UV 2] ��� ���� ���� : COM%d", sysData.iCommPort[COMM_UV2]);
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CAABonderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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
		putListLog("���� �̵����Դϴ�.");
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
		// 20151006 ���� ����ī�޶� �߰��� ���� ����
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

	//ccd���� ��ǥ ����
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

	

	g_iCCDCamView = 5;		// ȭ�� ũ�� ����
							/* ListBox */
	wndpl.rcNormalPosition.left = m_rectCamDispPos1.left;
	wndpl.rcNormalPosition.right = m_rectCamDispPos2.left;// -10;// m_rectCamDispPos1.right + ;
	wndpl.rcNormalPosition.top = m_rectCamDispPos1.bottom + 145;
	wndpl.rcNormalPosition.bottom = MAIN_DLG_SIZE_Y - 85;
	GetDlgItem(IDC_LIST_LOG)->MoveWindow(&wndpl.rcNormalPosition);

	/* ���콺 Ŀ�� */
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
	/* ��ư ��� */
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
	//--------------------------------------------------------------------------��� �޴� ����
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
	//�ð�
	
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
		.SetText(sLangChange) //��������
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


	sLangChange.LoadStringA(IDS_STRING441);//"CCD ������" 
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
	sLangChange.LoadStringA(IDS_STRING756);	//"Lot ��"
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

	Make_Child_Dialog();		// ����, CCD �ʱ�ȭ
}

//! Added by LHW (2013/3/27)
//! �̹� �˻翡�� �̹��� Ȯ�� ���¿����� Pan ��� �߰��� �����ϰ� �ϱ� ���� �Լ��� �и�
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

		//! �̹��� ���� ��ġ Ȯ��
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
		if(g_iCCDCamView == 0)		// �»�
		{
			m_ViewPos.x = 0;
			m_ViewPos.y = 0;
		}
		else if(g_iCCDCamView == 1)	// ���
		{
			m_ViewPos.x = gMIUDevice.nWidth - SMALL_CCD_SIZE_X;
			m_ViewPos.y = 0;
		}
		else if(g_iCCDCamView == 2)		//����
		{
			m_ViewPos.x = 0;
			m_ViewPos.y = gMIUDevice.nHeight - SMALL_CCD_SIZE_Y;
		}
		else if(g_iCCDCamView == 3)		//����
		{
			m_ViewPos.x = gMIUDevice.nWidth - SMALL_CCD_SIZE_X;
			m_ViewPos.y = gMIUDevice.nHeight - SMALL_CCD_SIZE_Y;
		}
		else if(g_iCCDCamView == 4)		// �߽�
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
			if(g_iCCDCamView >= 0 && g_iCCDCamView <= 4)	// Ȯ�� ����
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
			if(g_iCCDCamView >= 0 && g_iCCDCamView <= 4)	// Ȯ�� ����
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
			if(g_iCCDCamView >= 0 && g_iCCDCamView <= 4)	// Ȯ�� ����
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
	sLangChange.LoadStringA(IDS_STRING1408);	//�����Ͻðڽ��ϱ�?
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
		//! UV Lamp Front Panel���� �����ϴ� ������� ����
		//! ������ ���� ���ʹ� �ݰ�, Lamp�� ���д�. 
		//UVCommand.UV_Shutter_Close();
		//Sleep(200);
		//UVCommand.UV_Lamp_Off();
		//Sleep(200);
		//UVCommand.Set_Control_Mode_Front_Panel();
		//Sleep(100);		
		////! UV Lamp RS-232C ��� ����
		//UVCommand.Close_Device();
		
		//! Added by LHW (2013/5/3)
		//! ���� ��Ʈ�ѷ� RS-232C ��� ����
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
				sLangChange.LoadStringA(IDS_STRING1432);	//��� ���� ��� �ð��� �ʰ� �Ͽ����ϴ�. ���� ���� �մϴ�.
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
				sLangChange.LoadStringA(IDS_STRING1432);	//��� ���� ��� �ð��� �ʰ� �Ͽ����ϴ�. ���� ���� �մϴ�.
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
				sLangChange.LoadStringA(IDS_STRING1432);	//��� ���� ��� �ð��� �ʰ� �Ͽ����ϴ�. ���� ���� �մϴ�.
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
				sLangChange.LoadStringA(IDS_STRING1432);	//��� ���� ��� �ð��� �ʰ� �Ͽ����ϴ�. ���� ���� �մϴ�.
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
				sLangChange.LoadStringA(IDS_STRING1432);	//��� ���� ��� �ð��� �ʰ� �Ͽ����ϴ�. ���� ���� �մϴ�.
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
				sLangChange.LoadStringA(IDS_STRING1432);	//��� ���� ��� �ð��� �ʰ� �Ͽ����ϴ�. ���� ���� �մϴ�.
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
				sLangChange.LoadStringA(IDS_STRING1432);	//��� ���� ��� �ð��� �ʰ� �Ͽ����ϴ�. ���� ���� �մϴ�.
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
				sLangChange.LoadStringA(IDS_STRING1432);	//��� ���� ��� �ð��� �ʰ� �Ͽ����ϴ�. ���� ���� �մϴ�.
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

//! ���� ȭ�鿡 ǥ�õǾ� �ִ� �簢 ������ ������ ����
void CAABonderDlg::Change_Mode_Mouse_Box(int iMode_Mouse_Box)
{
	//! ���� ���õ� ���� ���� �簢 ���� ǥ�ø� �ϴ� ������ �����Ѵ�. 
	COLORREF clrBoxArea = GetColor_Mouse_Box(iMode_Mouse_Box);

	vision.ChangeColor_Box(m_iCurCamNo, clrBoxArea);
	vision.drawOverlay(m_iCurCamNo);
}

//! �Էµ� ���� �´� �簢 ������ ������ ��ȯ
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
			//! �簢 ������ ���� ��ġ���� Ȯ���Ѵ�.

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
				//! CCD OffLine ������ ��
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

				/* �̵� */
				if (m_iMoveType==MOVE_ALL)
				{
					m_rBox.left		+= iMoveX;
					m_rBox.top		+= iMoveY;
					m_rBox.right	+= iMoveX;
					m_rBox.bottom	+= iMoveY;
				}
				/* �� ũ�� */
				else if (m_iMoveType==MOVE_WIDTH_LEFT)
				{
					m_rBox.left		+= iMoveX;
				}
				/* �� ũ�� */
				else if (m_iMoveType==MOVE_WIDTH_RIGHT)
				{
					m_rBox.right	+= iMoveX;
				}
				/* �� ũ�� */
				else if (m_iMoveType==MOVE_HEIGHT_TOP)
				{
					m_rBox.top		+= iMoveY;
				}
				/* �� ũ�� */
				else if (m_iMoveType==MOVE_HEIGHT_BOTTOM)
				{
					m_rBox.bottom	+= iMoveY;
				}
				/* �»� ũ�� */
				else if (m_iMoveType==MOVE_NW)
				{
					m_rBox.left		+= iMoveX;
					m_rBox.top		+= iMoveY;
				}
				/* ��� ũ�� */
				else if (m_iMoveType==MOVE_NE)
				{
					m_rBox.top		+= iMoveY;
					m_rBox.right	+= iMoveX;
				}
				/* ���� ũ�� */
				else if (m_iMoveType==MOVE_SW)
				{
					m_rBox.left		+= iMoveX;
					m_rBox.bottom	+= iMoveY;
				}
				/* ���� ũ�� */
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

				//! ���� ���õ� ���� ���� �簢 ���� ǥ�ø� �ϴ� ������ �����Ѵ�. 
				COLORREF clrBoxArea = GetColor_Mouse_Box(m_iMode_Mouse_Box);

				//! �簢 ������ �׸���. 
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

				//! ���� ���õ� ���� ���� �簢 ���� ǥ�ø� �ϴ� ������ �����Ѵ�. 
				COLORREF clrBoxArea = GetColor_Mouse_Box(m_iMode_Mouse_Box);

				//! �簢 ������ �׸���. 
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

			/* �̵� */
			if (m_iMoveType==MOVE_ALL)
			{
				m_rBox.left		+= iMoveX;
				m_rBox.top		+= iMoveY;
				m_rBox.right	+= iMoveX;
				m_rBox.bottom	+= iMoveY;
			}
			/* �� ũ�� */
			else if (m_iMoveType==MOVE_WIDTH_LEFT)
			{
				m_rBox.left		+= iMoveX;
			}
			/* �� ũ�� */
			else if (m_iMoveType==MOVE_WIDTH_RIGHT)
			{
				m_rBox.right	+= iMoveX;
			}
			/* �� ũ�� */
			else if (m_iMoveType==MOVE_HEIGHT_TOP)
			{
				m_rBox.top		+= iMoveY;
			}
			/* �� ũ�� */
			else if (m_iMoveType==MOVE_HEIGHT_BOTTOM)
			{
				m_rBox.bottom	+= iMoveY;
			}
			/* �»� ũ�� */
			else if (m_iMoveType==MOVE_NW)
			{
				m_rBox.left		+= iMoveX;
				m_rBox.top		+= iMoveY;
			}
			/* ��� ũ�� */
			else if (m_iMoveType==MOVE_NE)
			{
				m_rBox.top		+= iMoveY;
				m_rBox.right	+= iMoveX;
			}
			/* ���� ũ�� */
			else if (m_iMoveType==MOVE_SW)
			{
				m_rBox.left		+= iMoveX;
				m_rBox.bottom	+= iMoveY;
			}
			/* ���� ũ�� */
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

			if (m_iNo_SFR < model.mGlobalChartCount)	//if(m_iNo_SFR<LAST_MARK_CNT)					// �簢 ��ũ ��ġ..
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
			else if (m_iNo_SFR < model.mGlobalChartCount + 4)//else if(m_iNo_SFR<LAST_MARK_CNT+4)			// ���� ��ũ �˻� ��ġ..
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
	/* �̵����̸� Return */
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
	/* �̵����̸� Return */
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
		
		// 20140905 Overlay Box ��ġ �̵� �� Box ������ �󸶳� ������ �־�� ���콺 Ŀ�� ��� ���� �Ÿ� Ȯ�� ��..
		//iGap = 50;
		iGap = int(dExpandFactorX * 5);
	}

	CPoint point = p;

	int iRtn = -1;

	point.x -= m_rectCamDispPos1.left;
	point.y -= m_rectCamDispPos1.top;

	p.x = (int)(point.x * dExpandFactorX + 0.5);
	p.y = (int)(point.y * dExpandFactorY + 0.5);

	/* �ڽ� �̵� */
	if (p.x>rcTemp.left+iGap	&&
		p.x<rcTemp.right-iGap	&&
		p.y>rcTemp.top+iGap		&&
		p.y<rcTemp.bottom-iGap)
	{
		::SetCursor(m_hCursor_Move);
	}
	/* �� ũ�� */
	else if (p.y>rcTemp.top+iGap && p.y<rcTemp.bottom-iGap && p.x>rcTemp.left-iGap && p.x<rcTemp.left+iGap)
	{
		::SetCursor(m_hCursor_Width);
	}
	/* �� ũ�� */
	else if (p.y>rcTemp.top+iGap && p.y<rcTemp.bottom-iGap && p.x>rcTemp.right-iGap && p.x<rcTemp.right+iGap)
	{
		::SetCursor(m_hCursor_Width);
	}
	/* �� ũ�� */
	else if (p.x>rcTemp.left+iGap && p.x<rcTemp.right-iGap && p.y>rcTemp.top-iGap && p.y<rcTemp.top+iGap)
	{
		::SetCursor(m_hCursor_Height);
	}
	/* �� ũ�� */
	else if (p.x>rcTemp.left+iGap && p.x<rcTemp.right-iGap && p.y>rcTemp.bottom-iGap && p.y<rcTemp.bottom+iGap)
	{
		::SetCursor(m_hCursor_Height);
	}
	/* �»� ũ�� */
	else if (p.x>rcTemp.left-iGap && p.x<rcTemp.left+iGap && p.y>rcTemp.top-iGap && p.y<rcTemp.top+iGap)
	{
		::SetCursor(m_hCursor_NWSE);
	}
	/* ��� ũ�� */
	else if (p.x>rcTemp.right-iGap && p.x<rcTemp.right+iGap && p.y>rcTemp.top-iGap && p.y<rcTemp.top+iGap)
	{
		::SetCursor(m_hCursor_NESW);
	}
	/* ���� ũ�� */
	else if (p.x>rcTemp.left-iGap && p.x<rcTemp.left+iGap && p.y>rcTemp.bottom-iGap && p.y<rcTemp.bottom+iGap)
	{
		::SetCursor(m_hCursor_NESW);
	}
	/* ���� ũ�� */
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
		// 20140905 Overlay Box ��ġ �̵� �� Box ������ �󸶳� ������ �־�� ���콺 Ŀ�� ��� ���� �Ÿ� Ȯ�� ��..
		//iGap = 50;
		iGap = int(dExpandFactorX * 5);
	}

	CPoint point = p;

	int iRtn = -1;

	point.x -= m_rectCamDispPos1.left;
	point.y -= m_rectCamDispPos1.top;

	p.x = (int)(point.x * dExpandFactorX + 0.5);
	p.y = (int)(point.y * dExpandFactorY + 0.5);

	/* �ڽ� �̵� */
	if (p.x>rcTemp.left+iGap	&&
		p.x<rcTemp.right-iGap	&&
		p.y>rcTemp.top+iGap		&&
		p.y<rcTemp.bottom-iGap)
	{
		iRtn = MOVE_ALL;
	}
	/* �� ũ�� */
	else if (p.y>rcTemp.top+iGap && p.y<rcTemp.bottom-iGap && p.x>rcTemp.left-iGap && p.x<rcTemp.left+iGap)
	{
		iRtn = MOVE_WIDTH_LEFT;
	}
	/* �� ũ�� */
	else if (p.y>rcTemp.top+iGap && p.y<rcTemp.bottom-iGap && p.x>rcTemp.right-iGap && p.x<rcTemp.right+iGap)
	{
		iRtn = MOVE_WIDTH_RIGHT;
	}
	/* �� ũ�� */
	else if (p.x>rcTemp.left+iGap && p.x<rcTemp.right-iGap && p.y>rcTemp.top-iGap && p.y<rcTemp.top+iGap)
	{
		iRtn = MOVE_HEIGHT_TOP;
	}
	/* �� ũ�� */
	else if (p.x>rcTemp.left+iGap && p.x<rcTemp.right-iGap && p.y>rcTemp.bottom-iGap && p.y<rcTemp.bottom+iGap)
	{
		iRtn = MOVE_HEIGHT_BOTTOM;
	}
	/* �»� ũ�� */
	else if (p.x>rcTemp.left-iGap && p.x<rcTemp.left+iGap && p.y>rcTemp.top-iGap && p.y<rcTemp.top+iGap)
	{
		iRtn = MOVE_NW;
	}
	/* ��� ũ�� */
	else if (p.x>rcTemp.right-iGap && p.x<rcTemp.right+iGap && p.y>rcTemp.top-iGap && p.y<rcTemp.top+iGap)
	{
		iRtn = MOVE_NE;
	}
	/* ���� ũ�� */
	else if (p.x>rcTemp.left-iGap && p.x<rcTemp.left+iGap && p.y>rcTemp.bottom-iGap && p.y<rcTemp.bottom+iGap)
	{
		iRtn = MOVE_SW;
	}
	/* ���� ũ�� */
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
		sLangChange.LoadStringA(IDS_STRING1395);	//"��ü ���� ���� ���� - �ڵ����� ��
		sTemp.Format(sLangChange);
		delayMsg(sTemp.GetBuffer(99), 3000, M_COLOR_DARK_GREEN);
		return;
	}

	if(g_bMovingflag)
	{
		sLangChange.LoadStringA(IDS_STRING1394);	//��ü ���� ���� ���� - ���� �̵� ��
		sTemp.Format(sLangChange);
		delayMsg(sTemp.GetBuffer(99), 3000, M_COLOR_DARK_GREEN);
		return;
	}


	bool home_process_run_flag = true;
	g_bMovingflag =true;
	sLangChange.LoadStringA(IDS_STRING1398);	//��ü ���� ���͸� ���� �Ͻðڽ��ϱ�?
	sTemp.Format("��ü ���� ���͸� ���� �Ͻðڽ��ϱ�?");	//��ü ���� ���͸� ���� �Ͻðڽ��ϱ�?
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

//! Auto Exposure Time Thread ����
void CAABonderDlg::CCMThreadStart_Auto_Exposure_Time()
{
	pThread_CCM_Auto_Exposure_Time = NULL;

	pThread_CCM_Auto_Exposure_Time = ::AfxBeginThread(Thread_Ccm_Auto_Exposure_Time,  this);
}

//! Auto White Balance Thread ����
void CAABonderDlg::CCMThreadStart_Auto_WhiteBalance()
{
}

double CAABonderDlg::HolderThetaCalc(double _theta , int index)
{
	//�߽����� ȸ������ ���� Ư������Ʈ(x,y)�� �̵��Ÿ� ���ϱ�
	double temp;
	temp = 0.0;
	_theta = _theta * (M_PI/180);
	switch(index)
	{
		case 0://x��
			temp = (model.holderx-((model.holderx*cos(_theta)) - (model.holdery*sin(_theta))))*-1;
			//((model.holderx-centerX)*cos(_theta) - (model.holdery-centerY)*sin(_theta) + centerX)
			break;

		case 1://y��
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
			putListLog("Lens�� X, Y ������ �̵� Limit�� �ʰ� �Ͽ����ϴ�.");
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
			sTemp.Format("����ǰ Align �˻� NG");
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
				sLangChange.LoadStringA(IDS_STRING656);	//Lens Align �˻� NG.
				sTemp.Format(sLangChange);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING906);	//PCB Align �˻� NG.
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
	
		sLangChange.LoadStringA(IDS_STRING1162);	//���� Shift (%.01f, %.01f) �˻� �Ϸ�. %.01lf msec
		sTmpLog.Format(sLangChange, fShiftX, fShiftY, eTime-ep);
		putListLog(sTmpLog);
	}
	else
	{
		saveInspImage(OPTIC_IMAGE_SAVE, Task.PCBTask);
		sLangChange.LoadStringA(IDS_STRING1165);		//���� �˻� ����.
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

	//--- ȸ������ Degree�� ����ǹǷ�...
	dTheta = (sysData.dCamAngle[iCamNo].x*M_PI) / 180.0;
	dCosVal = cos(dTheta);
	dSinVal = sin(dTheta);

	dCx = (dCosVal*(dFindX-dBaseX))-(dSinVal*(dBaseY-dFindY));
	dCx = dCx + dBaseX;

	//--- ȸ������ Degree�� ����ǹǷ�...
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
	sLangChange.LoadStringA(IDS_STRING1482);	//������ : %.3lf %.3lf %.3lf
	sTemp.Format(sLangChange, dOffsetX, dOffsetY, dOffsetTh);
	vision.textlist[camNo].addList(20, (CAM_SIZE_Y-50), sTemp, M_COLOR_CYAN, 24, 10, "Arial Black");

	return true;
}

/////////////////////// ���� �۾� �� �Դϴ�~~
int CAABonderDlg::checkAutoRunLensAlarm(int fi_step)					// 0:����, 1:���� ���� ���� ���� �� �� �ֵ���, 2:������ ����
{
	////////////////////////////////////////////////////////////////////////////////
	// ���� AmpFault

	motor.AmpFaultCheck();
	if(Task.iAmpFaultFlag)
	{
		m_labelServo.SetBkColor(M_COLOR_RED);
		m_labelServo.Invalidate();
		sLangChange.LoadStringA(IDS_STRING1327);	//���� �� AMP Fault�� ���� �Ǿ����ϴ�.
		_stprintf_s(sz_PCB_Error_Msg, sLangChange);
		return 2;
	}


	////////////////////////////////////////////////////////////////////////////////
	// ���� ���� ���� Ȯ��..

	motor.HomeCheck();

	if(Task.iHomeErrorFlag)
	{
		m_labelHom.SetBkColor(M_COLOR_RED);
		m_labelHom.Invalidate();
		sLangChange.LoadStringA(IDS_STRING1331);	//���� ���� ������ ���� �Ǿ����ϴ�.
		_stprintf_s(sz_PCB_Error_Msg, sLangChange);
		return 2;
	}
	
	//motor.InDIO(0, curInDio[0]);
	return 0;
}


int CAABonderDlg::checkAutoRunPcbAlarm(int fi_step)					// 0:����, 1:���� ���� ���� ���� �� �� �ֵ���, 2:������ ����
{
	////////////////////////////////////////////////////////////////////////////////
	// ���� AmpFault

	bool testFlag = true;

	motor.AmpFaultCheck();
	if(Task.iAmpFaultFlag)
	{
		m_labelServo.SetBkColor(M_COLOR_RED);
		m_labelServo.Invalidate();
		sLangChange.LoadStringA(IDS_STRING1327);	//���� �� AMP Fault�� ���� �Ǿ����ϴ�.
		_stprintf_s(sz_PCB_Error_Msg, sLangChange);
		return 2;
	}


	////////////////////////////////////////////////////////////////////////////////
	// ���� ���� ���� Ȯ��..

	motor.HomeCheck();

	if(Task.iHomeErrorFlag)
	{
		m_labelHom.SetBkColor(M_COLOR_RED);
		m_labelHom.Invalidate();

		sLangChange.LoadStringA(IDS_STRING1331);	//���� ���� ������ ���� �Ǿ����ϴ�.
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
			putListLog("Auto Focusing �� Z Motor �̵� �ð� �ʰ�.");
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

	//gMIUDevice.CurrentState = 3;	//��������
	vision.clearOverlay(CCD);
	vision.MilBufferUpdate();	//ù�忡�� ������ Image�� ����� ���� �߻����� �ʱ� �ѹ� ���� ������


	Task.getROI();				// ���� ��ũ ��ġ �ν�..
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
            bRtn = Task.getROI_SFR(Mode);				// �簢�� Box ��ġ �ν�..
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
			/////m_stSFRSpec.tSFRConfig.eFrequencyUnit = ESFRFreq_LinePairPerMilliMeter;	//�����ڸ�
			m_stSFRSpec.tSFRConfig.eFrequencyUnit = ESFRFreq_CyclePerPixel;		//�Ҽ���
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
			_stprintf_s(szLog, SIZE_OF_1K, _T("[SFR ����] ���� ��ũ �ν� ����"));
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

	// SFR����� ������� ���� �ִ�ġ�� ���� 
	// ����� SONY1
#ifndef SFR_COMP_MAX
	for (int i = 0; i<model.mGlobalSmallChartCount; i++)
	{
		fMax_SFR_N4[i] = fMax_SFR_N4[i] / iCnt_Focus;
		//logStr.Format("SFR(%d) = %f", i, fMax_SFR_N4[i]);
		_stprintf_s(szLog, SIZE_OF_1K, _T("SFR(%d) = %f"), i, fMax_SFR_N4[i]);
		putListLog(logStr);
	}
#endif

	//Plus �� 
	//[0]CENTER
	//[1]TOP , [2]BOTTOM , [3]LEFT , [5]RIGHT
	//
	//��������
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
	//new ���
	for (i = 0; i < 5; i++)	//AA ���� ����
	{
		nPartternRoiIndex = model.sfrElem.m_AATiltRoiIndex[i];
		mAvrSfrData = 0.0;
		mTempPosX = 0.0;
		mTempPosY = 0.0;
		mAvrIndex = 0;
		for (j = 0; j < 4; j++)		//���� ������ T,B,L,R ����
		{
			if (model.sfrElem.m_AATiltCalRoi[nPartternRoiIndex][j] == 1)	//aa��� �������� üũ��������
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

	//end new ���
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
			_stprintf_s(szLog, SIZE_OF_1K, _T("	OC ���� = X: %.3lf, Y: %.3lf   Spec(%.03lf, %.03lf)"), Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y, sysData.m_dOcSpec.x, sysData.m_dOcSpec.y);
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
		Task.SFR.fSfrN4[Task.m_iCnt_Step_AA_Total][4] > model.strInfo_AF1.m_fLimit_MTF )	/* ���� MTF ���� ����Ʈ �̻��̸� fine pitch �̵� */
	{
		Task.m_bFlag_Fine_Move = true;
	}
	Task.m_iCnt_Step_AA_Total++;


	
	if ( Mode != SFR_FINAL )
	{
		autodispDlg->DrawGraph(2);		/* ���� ��ġ SFR ������ �׷��� �׸��� */
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
				putListLog("[Active Align] Peak ���� ��ġ ��� ����");
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
					LogSave("ERROR ==========> ��Ŀ�� ���� �ּ� �ڽ¹� ��� ��� �̻�.");
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
			logStr.Format("CCD ������ ���� �����Դϴ�.");
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
			MandoInspLog.bInspRes = true;	//60000 �ʱ�ȭ
			Task.m_timeChecker.Measure_Time(6);	//AA ���� �ð�
			iRtnFunction = 60010;
			logStr.Format("	AA Step [%d]" , iRtnFunction);
			putListLog(logStr);
		}
		break;
	case  60010:
		if(bThread_MIUCheckRun == false)		//	CCD ���� ������ ���߸� ����... step = 12000 ����
		{
			iRtnFunction = 60015;
			logStr.Format("	AA Step [%d]" , iRtnFunction);
			putListLog(logStr);
		}else
		{
			logStr.Format("CCD ���� Open �������Դϴ�[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -60010;
		}
		break;
	case  60015:
		if (motor.LENS_Z_Motor_Move(Bonding_Pos))
		{
			logStr.Format("Lens Z Bonding ��ġ �̵� �Ϸ�[%d]", iStep);
			putListLog(logStr);
			iRtnFunction = 60020;

		}
		else
		{
			logStr.Format("Lens Z Bonding ��ġ �̵� ���� [%d]", iStep);
			putListLog(logStr);
			iRtnFunction = -60015;
		}
		break;
	case 60020:
		if( !motor.PCB_Z_Motor_Move(Bonding_Pos) )
		{
			logStr.Format("PCB Z�� Bonding ��ġ �̵� ����[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -60020;
			break;
		}else 
		{
			iRtnFunction = 60030;
			logStr.Format("PCB Z�� Bonding ��ġ �̵� �Ϸ�[%d]", iRtnFunction);
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
				logStr.Format("PCB�� x,y ���� ��ġ�� ���� �Ϸ�[%d]", iStep);
				putListLog(logStr);
				iRtnFunction = 60040;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
				break;
			}
			else
			{
				logStr.Format("Bonding ��ġ �̵� ����[%d]", iRtnFunction);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -60030;
				break;
			}
		}
		break;
	
	case 60040:		/* PCB Z���� ������ġ �̵� */
		{
			iRtn = _checkPcbMotor();
			if ( iRtn > 0 )
			{
				logStr.Format("Z ���� ���� ��ġ �̵�");
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
						putListLog("Auto Focusing �� LENS Z Motor �̵� �ð� �ʰ�.");
						iRtnFunction = -60040;
						break;
					}
				}
			}
			else
			{
				putListLog("PCB �� ������ġ�� �������� ���߽��ϴ�.");
				iRtnFunction = -60040;
			}
		}
		break;
	case 60070:		/* SFR ���� �������� �϶��� ��, Z���� �̵� */
		{
			double dCurPos = 0.0;
			double dMovePos = model.axis[TITLE_MOTOR_Z].pos[Bonding_Pos] + model.m_dMove_Offset_Z;
			//�������� �϶��� model.m_dMove_Offset_Z��ŭ z���� �����δ� 
			if(fabs(model.m_dMove_Offset_Z) > 0.2)
			{
				logStr.Format("z�� �̵�ġ Limit �ʰ� : Z : %.3lf", dMovePos);
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
					putListLog("Auto Focusing �� Z Motor �̵� �ð� �ʰ�.");
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

			iRtn = _moveZMotor(dMoveDist, model.strInfo_AF1.m_dMoveVel);		/* Z���� Rough �̵� */ 
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
				if(sysData.m_i1stAaEbable)	//1�� AA�� �����
				{
					iRtn = _getMTF(SFR_FINAL);

					if(iRtn > 0)
					{
						Task.m_iCnt_Second_AA_Start = Task.m_iCnt_Step_AA_Total;
						Task.m_iDrawBarStep = Task.m_iCnt_Step_AA_Total;
						Task.m_iCnt_Step_AA = 0;

						autodispDlg->DrawBarGraph();
						//MIU.func_Set_InspImageCopy(CHART_RAW);			//1�� AA�� 
						Task.sfrResult = func_MTF(MIU.vChartBuffet);		//1�� AA�� ����
						if( Task.sfrResult )
						{ 
							logStr.Format("1�� AA ��� Spec OK.");
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
							//	//saveSfrSumLog();//���� Data log ����
							//	logStr.Format("1�� AA ��� Spec OK.");
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
				logStr.Format("Tx, Ty ������ 1 �̻� - Z 2�� �Ÿ� ��� [Tx %.03f, Ty %.03f]", Task.SFR.dTilt_X, Task.SFR.dTilt_Y );
				dMovePos = -2 * sysData.m_dMove_MaxPos_Fine;
			}*/

			iRtn = _moveZMotor(dMovePos, model.strInfo_AF1.m_dMoveVel);		/* Z���� Rough �̵� */

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

	case 60900:			/* Z�� ���� ��ġ ���� �� */
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

	case 60910:	// AA�� Lens X/Y, PCB Theta ����
		
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
					logStr.Format("		 ���� Shift. Spec(%.03lf, %.03lf) In : X %.3lf, Y %.3lf", 
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
					logStr.Format("		 ���� Shift.  X %.3lf, Y %.3lf ,Spec(%.03lf, %.03lf)", Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y, sysData.m_dOcSpec.x, sysData.m_dOcSpec.y);
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
			logStr.Format("SFR ���� ȹ�� ����.");	//SFR ���� ȹ�� ����.
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
				Task.m_iCnt_Step_AA++;		/* Auto Focus rough ���� Ƚ�� ���� */

				if (Task.bFirstAA)
				{
					autodispDlg->DrawGraph(1);		/* Z ���� ��ġ�� SFR ������ �׷��� �׸��� */
				}
				else
				{
					autodispDlg->DrawGraph(0);		/* Z ���� ��ġ�� SFR ������ �׷��� �׸��� */
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

	case 63000:		/* �ʱ� 3�� ������ Ȯ�� �� ���� ���̸� Z�� ���� �̵� */
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

 			iRtn = _checkMaxSfrPos(0);		/* MAX �� ã�� */

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
										logStr.Format("Z Down ��ġ �̵� �Ϸ�");
										putListLog(logStr);
										break;
									}
								}
							}
							else
							{
								logStr.Format("Z Down ��ġ �̵� �ð� �ʰ�");
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
										logStr.Format("Z Down ����ġ �̵� �Ϸ�");
										putListLog(logStr);
										break;
									}
								}
							}
							else
							{
								logStr.Format("Z Down ����ġ �̵� �ð� �ʰ�"); 
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
					logStr.Format("Tilt ������ Limit �ʰ� : Tx : %.4f Ty : %.4f", Task.SFR.dTilt_X, Task.SFR.dTilt_Y);
					errMsg2(Task.AutoFlag, logStr);
					iRtnFunction = -64000;
				}
				else
				{
					if (Task.bFirstAA == true)
					{
						//2��
						logStr.Format("	AA Step 2��[%d]", iRtnFunction);
						putListLog(logStr);
					}
					else
					{
						//1��
						logStr.Format("	AA Step 1��[%d]", iRtnFunction);
						putListLog(logStr);
					}
					iRtnFunction = 64200;		/* Focusing �Ϸ� */	
					logStr.Format("	AA Step [%d]" , iRtnFunction);
					putListLog(logStr);
				}
			}
			else{
				iRtnFunction = 60100;		/* �ٽ� ���� */
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
			}

			if (Task.m_iCnt_Step_AA>model.strInfo_AF1.m_iStepCnt+5)
			{
				logStr.Format("SFR ���� �ִ� Ƚ�� �ʰ�.");
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -64000;	/* ī��Ʈ �ʰ��� ���� */
			}

			double z_move_dist = fabs(model.axis[TITLE_MOTOR_Z].pos[Bonding_Pos] - motor.GetCommandPos(TITLE_MOTOR_Z));

			if (z_move_dist > 1.0)//0.5
			{
				logStr.Format("[SFR ����] Z ���� �ϰ� �Ÿ� �ʰ� (1.0mm �̻� ���� �� �����ϴ�.)");
				errMsg2(Task.AutoFlag, logStr);

				iRtnFunction = -64000;
			}
		}
		break;

	case 64200:		/* Trough Focus �� Center SFR �� MAX ��ġ�� Z���� �̵� */
		{
			double dCurPos		= 0.0;
			double dMovePos		= Task.SFR.dMaxPos[0];//-3.970

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

			double z_move_dist = 0.0;
			//PCB Z�s�� ���̹Ƿ� Tilt������ PCB-Z�s�� �ϱ� ������.
			//
			//2�� �Ѿ�鼭 Z�� ���� �κ�
			//Lens�� Z�� ���
			//Pcb�� Z�� �ϰ�
#if (____AA_WAY == PCB_TILT_AA)
			z_move_dist = fabs(model.axis[TITLE_MOTOR_Z].pos[Bonding_Pos] - dMovePos) * -1;	//1�� ->2 ���̵�
#elif (____AA_WAY == LENS_TILT_AA)
			z_move_dist = fabs(model.axis[TITLE_MOTOR_Z].pos[Bonding_Pos] - dMovePos) * 1;	//1�� ->2 ���̵�
#endif
			if (z_move_dist > 1.0) 
			{
				logStr.Format("[SFR ����] Z ���� �ϰ� �Ÿ� �ʰ� (1.0mm �̻� ���� �� �����ϴ�.)");
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
					putListLog("Auto Focusing �� LENS Z Motor �̵� �ð� �ʰ�.");
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

	case 64700:			/* Tilt ���� �� Delay */
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

	case 65000:			/* Tilt �� MTF ���� */
		{
			//Task.bFirstAA=true;
			if ( Task.bFirstAA == true )//! Active Alignment 2��° ���� 
			{
				Sleep(200);
				iRtn = _getMTF(SFR_FINAL);

				Task.m_iDrawBarStep = Task.m_iCnt_Step_AA_Total;
				autodispDlg->DrawBarGraph();

				if ( iRtn > 0 )
				{
					if (!_calcImageAlignment())
					{//-- Align ���� Limit(������ �� �����...1.0)
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
								logStr.Format("ImageCenter ���� Shift.  X %.3lf, Y %.3lf", Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
								putListLog(logStr);
								Sleep(200);
								iRtnFunction = 65500;
								logStr.Format("	AA Step [%d]", iRtnFunction);
								putListLog(logStr);
								break;
							}
							else
							{
								logStr.Format("ImageCenter ���� Shift. Spec(%.03lf, %.03lf) In : X %.3lf, Y %.3lf", sysData.m_dOcSpec.x, sysData.m_dOcSpec.y, Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
								putListLog(logStr);
							}
						}
					}

					bool bMtfCheck2 = true;
					vision.MilBufferUpdate();
					Sleep(500);
					MIU.func_Set_InspImageCopy(UV_BEFORE_CHART);			//AA UV BEFORE
					Task.sfrResult = func_MTF( MIU.vChartBuffet);				// [AA UV BEFORE] #1 MIU.m_pFrameRawBuffer);// 
					MandoInspLog.func_LogSave_UVBefore();	// UV�� Log Data ����
				
					autodispDlg->DrawBarGraph();
					if(!Task.sfrResult && bMtfCheck2)
					{
						saveInspImage(AA_NG_IMAGE_SAVE, Task.m_iCnt_Step_AA_Total);
						logStr.Format("UV�� SFR �˻� NG.  \nAA RETRY �Ͻðڽ��ϱ�?");//logStr.Format("SFR ���� �˻� NG.  \nAA RETRY �Ͻðڽ��ϱ�?");
						if(askMsg(logStr))
						{
							iRtnFunction = 60000;
							logStr.Format("	AA Step [%d]" , iRtnFunction);
							putListLog(logStr); 
							break;
						}
					}
					
					//saveSfrSumLog();//���� Data log ����

					double dCurPosX, dCurPosY, dCurPosTH, dCurPosTX, dCurPosTY, dCurPosZ;
					dCurPosX = motor.GetEncoderPos(Motor_PCB_X);
					dCurPosY = motor.GetEncoderPos(Motor_PCB_Y);
					dCurPosTH = motor.GetCommandPos(Motor_PCB_TH);
					dCurPosTX = motor.GetCommandPos(Motor_PCB_Xt);
					dCurPosTY = motor.GetCommandPos(Motor_PCB_Yt);
					dCurPosZ = motor.GetCommandPos(Motor_PCB_Z);
					if( !g_ADOData.func_AATaskToAAPcbMoter(Task.ChipID, dCurPosX, dCurPosY, dCurPosTH, dCurPosTX, dCurPosTY, dCurPosZ) )
					{
						logStr.Format("DataBase ���� ����.[%d]\n MS Office�� �ݾ��ּ���.", iStep);
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
					logStr.Format("SFR ���� �˻� ����.");
					errMsg2(Task.AutoFlag, logStr);
					iRtnFunction = -65000;
				}
			}
			else
			{//! Active Alignment 1��° ����
				iRtn = _getMTF(SFR_FINAL);
				if ( iRtn > 0 )
				{
					if ( !_calcImageAlignment() )	//����� //����AA
					{//-- Align ���� Limit(������ �� �����...1.0)
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
						{//X/Y �������� Spec In�� ��� ���� Step
							sLangChange.LoadStringA(IDS_STRING1302);	//���� Shift. Spec(%.03lf, %.03lf) In : X %.3lf, Y %.3lf
							logStr.Format(_T("		 ") + sLangChange, sysData.m_dOcSpec.x, sysData.m_dOcSpec.y, Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
							putListLog(logStr);
							iRtnFunction = 69999;
							logStr.Format("	AA Step [%d]" , iRtnFunction);
							putListLog(logStr);
							break;
						}
						else
						{
							sLangChange.LoadStringA(IDS_STRING1301);	//���� Shift.  X %.3lf, Y %.3lf
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
					sLangChange.LoadStringA(IDS_STRING1074);	//SFR ���� ȹ�� ����.
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

	case 69000:								// ���� SFR �˻� ��� ����..
		{									// SFR �˻� NG �߻� �� 1ȸ ��˻� �� ��� ����.
			if(myTimer(true)-Task.PCBTaskTime > model.strInfo_Cam[CCD].m_iDelayTime*2 || (myTimer(true)<Task.PCBTaskTime))
			{
				iRtn = _getMTF(SFR_FINAL);		/* MTF�� ȹ�� */
				 
				Task.m_iDrawBarStep = Task.m_iCnt_Step_AA_Total;
				autodispDlg->DrawBarGraph();
				
				if (iRtn>0)
				{
					if(Task.m_b_AA_Retry_Flag==false)
					{
						Task.m_b_AA_Retry_Flag = true;		// TRUE�� ��� ���������� �˻� ��� NG �� �˶� �߻�..
						iRtnFunction = 60000;				// 60000���� 1, 2�� AA �˻� ��� �ʱ�ȭ..
						logStr.Format("	AA Step [%d]" , iRtnFunction);
						putListLog(logStr);
					}
					else
					{
						saveInspImage(AA_NG_IMAGE_SAVE, Task.m_iCnt_Step_AA_Total);
						Dio.setAlarm(ALARM_ON);

						logStr.Format("SFR ���� �˻� NG. �ڵ� ���� �Ͻðڽ��ϱ�?");
						if(askMsg(sLangChange))
						{
							Dio.setAlarm(ALARM_OFF);
							logStr.Format("NG ���� �Ͻðڽ��ϱ�. (��:NG, �ƴϿ�:OK)");
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
							Task.AutoFlag = 2;		// �Ͻ�����
							AutoRunView(Task.AutoFlag);
							iRtnFunction = -65000;
						}
					}
				}
				else
				{
					logStr.Format("SFR ���� �˻� (2��) ����.");
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
				logStr.Format("DataBase ���� ����.[%d]\n MS Office�� �ݾ��ּ���.", iStep);
				errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = -69999;
				break;
			}
			if (Task.bFirstAA == true)
			{
				Task.m_timeChecker.Measure_Time(7);	//AA �Ϸ� �ð�
				Task.m_dTime_FineAA = Task.m_timeChecker.m_adTime[5] - Task.m_timeChecker.m_adTime[4];
				dispGrid();

				iRtnFunction = 110000;
				logStr.Format("	AA Step [%d]" , iRtnFunction);
				putListLog(logStr);
			}
			else
			{
				//procOptical Aling����...
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

//--Laser ���������� Tilting���� ��� - 
//-- ���� ����� ������ 4��и� �������� 2��и�->1��и�->4��и�->3��и����� ���
bool CAABonderDlg::_calcLaserTilt(CDPoint dMotorPos[4], double dLaser[4], double &TX, double &TY)
{
	CString strLog = "";
	double offsetTx = 1.0;
	double offsetTy = 1.0;
	double tempLaser = 0.0;
	double Length[4];
	// ======================= ���
	//���Ͱ� �Ÿ�(�غ�)
	Length[0] = abs(dMotorPos[0].y - dMotorPos[3].y);  // left  
	Length[1] = abs(dMotorPos[0].x - dMotorPos[1].x);  // top
	Length[2] = abs(dMotorPos[1].y - dMotorPos[2].y);  // Right 
	Length[3] = abs(dMotorPos[2].x - dMotorPos[3].x);  // bottom
	
	// ======================= X,Y ���( �߽� ������)
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
	Height = AvgRight - AvgLeft;    //+ - ��ȣ �ȸ����� ���� �ٲٱ�
	radian = atan(Height / Width);
	theta = radian * 180 / M_PI;
	//-------------------------------------------------------
	tempLaser = (-theta + Task.oldLaserTy);
	double perValue = 100 * (tempLaser / Task.oldLaserTy);
	if (fabs(perValue) > 100 || Task.oldLaserTy == 0)
	{
		offsetTy = 1.0;//����
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
	// ======================= ���� ���� ��� (TX)
	Width = Length[0];                   // left

    Height = AvgBottom - AvgTop;
//	Height = AvgBottom - AvgTop;	//+ - ��ȣ �ȸ����� ���� �ٲٱ�
	
	radian = atan(Height / Width);
	theta = radian * 180 / M_PI;
	//-------------------------------------------------------
	tempLaser = (-theta + Task.oldLaserTx);
	perValue = 100 * (tempLaser / Task.oldLaserTx);
	if (fabs(perValue) > 100 || Task.oldLaserTx == 0)
	{
		offsetTx = 1.0;//����
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
		//x����
		dTopZPos_A = (Task.SFR.dMaxPos[1] + Task.SFR.dMaxPos[2]) / 2;
		dBottomZPos_A = (Task.SFR.dMaxPos[3] + Task.SFR.dMaxPos[4]) / 2;

		dHeight_A = dBottomZPos_A - dTopZPos_A;
		dTopPixelPos_A = (Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[1]][1].y + Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[2]][2].y) / 2;
		dBottomPixelPos_A = (Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[3]][3].y + Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[4]][4].y) / 2;

		dWidth_A = (dBottomPixelPos_A - dTopPixelPos_A) * model.m_dSize_CCD_Cell / 1000;	// CCD Cell ���ش�

		dTiltRad_A = atan(dHeight_A / dWidth_A);
		dTiltDeg_A = dTiltRad_A * 180.0 / M_PI;
	}
	else
	{
		//+����
		//  + ����Ʈ�� ƿƮ�� ���
		dTopZPos_A = Task.SFR.dMaxPos[1];
		dBottomZPos_A = Task.SFR.dMaxPos[2];

		dHeight_A = dBottomZPos_A - dTopZPos_A;
		dTopPixelPos_A = Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[1]][1].y;
		dBottomPixelPos_A = Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[2]][2].y;

		dWidth_A = (dBottomPixelPos_A - dTopPixelPos_A) * model.m_dSize_CCD_Cell / 1000;	//* CCD Cell ���ش�

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
		//x����
		dLeftZPos_A = (Task.SFR.dMaxPos[1] + Task.SFR.dMaxPos[3]) / 2;
		dRightZPos_A = (Task.SFR.dMaxPos[2] + Task.SFR.dMaxPos[4]) / 2;

		dHeight_A = dRightZPos_A - dLeftZPos_A;

		dLeftPixelPos_A = (Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[1]][1].x + Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[3]][3].x) / 2;
		dRightPixelPos_A = (Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[2]][2].x + Task.SFR.Sfr_pos[Task.SFR.iMaxIndex[4]][4].x) / 2;

		dWidth_A = (dRightPixelPos_A - dLeftPixelPos_A) * model.m_dSize_CCD_Cell / 1000;		// CCD Cell ���ش�

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

		dWidth_A = (dRightPixelPos_A - dLeftPixelPos_A) * model.m_dSize_CCD_Cell / 1000;		//* CCD Cell ���ش�

		dTiltRad_A = atan(dHeight_A / dWidth_A);
		dTiltDeg_A = dTiltRad_A * 180.0 / M_PI;
	}

	return dTiltDeg_A;
}

int CAABonderDlg::_procTiltY()
{
	if(Task.SFR.dTilt_Y == 0.0f)return 1;
  	CString logStr="";

	/* ��갪�� ���� ���� ���� �̵� */
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
					putListLog("Tilting PCB Yt Motor �̵� ������");
					errMsg2(Task.AutoFlag, sLangChange);
					return -1;
				}
			}
		}
		else
		{
			putListLog("Tilting PCB Yt Motor �̵� �ð� �ʰ�.");	
			return -1;
		}
	}
	logStr.Empty();
	return 1;
}


int	CAABonderDlg::_procTiltX()
{
  	CString logStr="";

	/* ��갪�� ���� ���� ���� �̵� */
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
					logStr.Format("Tilting Xt Motor �̵� ������  (Limit ���� Ȯ��).");
					putListLog(logStr);
					errMsg2(Task.AutoFlag, logStr);
					return -1;
				}
			}
		}
		else
		{
			putListLog("Tilting Xt Motor �̵� �ð� �ʰ�.");
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
			sLangChange.LoadStringA(IDS_STRING725);	//Lens �� X, Y ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
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
		case 0:									// 3�� ���� ��� ��ġ�� �̵� ���..
			ep = myTimer(true);
			motor.goMotorPos(2, axis, pos);
			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
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
				sLangChange.LoadStringA(IDS_STRING723);	//Lens �� [%s ����] �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorName[motorNo]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING724);	//Lens �� X, Y ���� ���� ���� �̵� ����.
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
		errMsg2(Task.AutoFlag, sLangChange);	//CCD ������ ���� �����Դϴ�. 3
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

				if (saveSfrLog(OC_INSP) == true)			// ���� ���� ����
					iRtnFunction = 132000;
				else
					iRtnFunction = -131000;
			}
			else
			{
				iRtnFunction = -131000;

				sLangChange.LoadStringA(IDS_STRING1173);	//���� ���͸� ���� �˻� ���� [%d]
				sLog.Format(sLangChange, iStep);
				errMsg2(Task.AutoFlag, sLog);
			}
		}

		break;

	case 132000:
		{
			int iCnt = 0;
			Dio.BackLightOn(false);		/* Back Light ���� */
			LightControl.ctrlLedVolume(LIGHT_OC_6500K, 0);
			while (1)
			{
				if (iCnt<10)
				{
					//if (Dio.BackLightOnCheck(false))		/* Back Light ���� üũ */
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
		sLangChange.LoadStringA(IDS_STRING862);	//OC ���� �˻� Step ��ȣ�� �������Դϴ�. [%d]
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
			sLog.Format("AA after PCB Z : %f , set Offset Z : %f", motor.GetCommandPos(TITLE_MOTOR_Z),sysData.m_dOffset_Prev_UV_Z);	//AA�� Lens Z : %f , ���� Offset Z : %f
			putListLog(sLog);

			MoveOffset_Prev_UV();	/* PCB X, Y, Z, Tx, Ty, PCB Th UV �� offset �̵� */
			
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

	case 110600:									// 20140612 ���� �߰�.. UV ��ȭ �� SFR ����.
		{      
			
		//	sLangChange.LoadStringA(IDS_STRING864);	//Offset Z ���� �� Lens Z : %f
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
		
	case 110650:					//UV�� MTF �˻� ����
		{
			Task.bInsCenter = true;

			_calcImageAlignment();	 
			Task.bInsCenter = false;
			
			iRtnFunction = 110700;	

		}
		break;


	case 110700:	// Lens Z  UV �� offset �̵� 
// 20150730 HJH Offset �ߺ� �������� ����
//		MoveOffset_Z();	/* Lens Z  UV �� offset �̵� */

		iRtnFunction =111000;
		Task.PCBTaskTime = myTimer(true);
		break;

	case 111000:	// UV ��ȭ
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
		autodispDlg->dispCurTaskOnLabel(1, sLangChange);//"UV ��ȭ �Ϸ�"
		
		Task.m_timeChecker.Measure_Time(8);	//UV �Ϸ� �ð�
		Task.m_dTime_UV = Task.m_timeChecker.m_adTime[8] - Task.m_timeChecker.m_adTime[7];


		if(myTimer(true) - Task.PCBTaskTime> 100)  // Delay üũ
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
			Task.PcbOnStage = 200;			// �˻� �Ϸ�.
				
			iRtnFunction = 115300;
			
			//if(!Dio.HolderGrip(false, true))				//Lens Gripper open
			//{
			//	sLog.Format("Lens Grip Open ���� [%d]", iStep);
			//	errMsg2(Task.AutoFlag, sLog);
			//	iRtnFunction = -115000;
			//}
			//else
			//{
			//	Task.m_bOKLensPass = -1;//���� �ѱ� �Ϸ� Lamp ���
			//	Task.PCBTaskTime = myTimer(true);
			//	iRtnFunction = 115300;
			//}
			
		}

		break;

	case 115300:		
		if(!Dio.LensMotorGrip(false, true))
		{

			sLog.Format("Lens Unit Gripper ���� ����[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = -115300;
			break;
		}
		Task.m_bOKLensPass = -1;//���� �ѱ� �Ϸ� Lamp ���
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
			sLog.Format("Laser/Cam �Ǹ��� ���� ����[%d]", iStep);
			errMsg2(Task.AutoFlag,sLog);
 			iRtnFunction = -115400;
		}*/
		break;
	case 115450:
		iRtnFunction = 115500;
		//if(Dio.CamLaserSlinderCheck(false, false))
		//{
		//	sLog.Format("Laser/Cam �Ǹ��� ���� �Ϸ�[%d]", iStep);
		//	putListLog(sLog);
		//	iRtnFunction = 115500;
		//}
		//else if((myTimer(true) - Task.PCBTaskTime) > 10000)//IO_DELAY_TIME)
		//{
		//	sLog.Format("Laser/Cam �Ǹ��� ���� �ð� �ʰ�[%d]", iStep);
		//	errMsg2(Task.AutoFlag,sLog);
		//	iRtnFunction = -115450;
		//}
		break;
	case 115500: //UV�Ϸ��� �����˻�
		Task.m_bOkFlag = 0;
		work.m_iCnt_Output++;
		work.Save();
		iRtnFunction = 120000;
		_getMTF(SFR_AFTER_UV);  
		break;


	default:
		sLangChange.LoadStringA(IDS_STRING1120);	//UV Process Step ��ȣ�� �������Դϴ�.
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

    case 122000:	//����ǰ �˻縸 �ش� (Final �˻��� X/Y/T ����)
        Sleep(dFinalDelay);
        iRtn = _getMTF(SFR_FINAL);
        Sleep(500);
        if (iRtn > 0)
        {
            if (!_calcImageAlignment())	//-- Align ���� Limit(������ �� �����...1.0)
            {
                sLog.Format("NG.  X : %.3lf Y : %.3lf", Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
                putListLog(sLog);
                iRtnFunction = -122000;
                break;
            }
            else
            {

                //ƿ�ð� �ȸ��� ��� ���� �����ϰ� �ݺ���.. 3ȸ ������ �ϰ� �ȸ����� Spec IN ��Ű��.., 
                if (Task.m_iFineThCnt > 3)	Task.m_dShift_IMG_TH = 0.0;
                if (fabs(Task.m_dShift_IMG_TH) <= sysData.dSpec_OC_Theta)//X/Y �������� Spec In�� ��� ���� Step
                {
                    sLog.Format("	���� Shift. Spec In(%.03lf) In : T: %.3lf", sysData.dSpec_OC_Theta, Task.m_dShift_IMG_TH);
                    putListLog(sLog);
                    iRtnFunction = 122500;
                }
                else
                {
                    sLog.Format("	���� Shift(Spec : %.03lf).  T: %.3lf", sysData.dSpec_OC_Theta, Task.m_dShift_IMG_TH);
                    putListLog(sLog);
                    Task.m_iFineThCnt++;
                    iRtnFunction = 122100;
                }
            }
        }
        else
        {
            errMsg2(Task.AutoFlag, "MTF ���� �� SFR ���� ȹ�� ����..");
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
    case 122500:	//Final �� �˻� - Ȯ�ο�
		setCamDisplay(3, 1);
		Sleep(dFinalDelay);
		//dms�� �̹��� ����
		MIU.func_Set_InspImageCopy(EOL_CHART);	//����ǰ ���
		Sleep(300);
		iRtnFunction = 122520;
		break;
	case 122520:
        Task.bInsCenter = true;
        
		_calcImageAlignment();

        Task.bInsCenter = false;

		Task.sfrResult = func_MTF(MIU.vChartBuffet);//EOL ȭ�� #1

        if (Task.sfrResult == true)
        {
            sLangChange.LoadStringA(IDS_STRING229);
            putListLog("[�˻�] MTF �˻� ����.");// Optic_Axis,
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
			sLangChange.LoadStringA(IDS_STRING305);	//PCB�� z�� ���� ��ġ �̵� ����[%d]
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
			sLangChange.LoadStringA(IDS_STRING747);	//Lens�� z�� ���� ��ġ �̵� ����[%d]
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
		if (sysData.m_iDefectInspPass == 1)	// �̹��˻� ����
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
			sLog.Format("PCB�� Dark ��ġ �̵� ����[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = -123100;
		}
		else
		{
			sLog.Format("PCB�� Dark ��ġ �̵� �Ϸ�[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = 123200;
		}
		break;
	case 123200:
		if (!motor.PCB_Z_Motor_Move(Dark_Pos))
		{
			sLog.Format("PCB�� Dark Z ��ġ �̵� ����[%d]", iStep);
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
			sLog.Format("PCB Z DAR ����Ȯ�� �ð� �ʰ�[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -123200;
		}
		break;
	case 123400:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB�� z�� ���� ��ġ �̵� ����[%d]
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
            sLangChange.LoadStringA(IDS_STRING1023);	//PCB�� ���� Defect �˻� ��ġ �̵� ����[%d]
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

            //6500k �̹��� ����
			Sleep(300);
			MIU.func_Set_InspImageCopy(MID_6500K_RAW);
            //ccdDlg->m_pSFRDlg->Raw_ImageSave(MIU.m_pFrameRawBuffer, MID_6500K_RAW);
            //MID_6500K_RAW �̹��� ����

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
		if (sysData.m_iDefectInspPass == 1)	// �̹��˻� ����
		{
			sLog.Format(_T("[PASS] Defect Insp"), iStep); 
			putListLog(sLog);
		}
		else
		{
			if (g_clPriInsp.func_Insp_Defect(MIU.vDefectMidBuffer_6500K, MIU.vDefectLowBuffer, true) == true)	//EOL
			{
				putListLog("[�˻�] Defect �˻� ����.");
			}
			else
			{
				sLog.Format("[�˻�] Defect �˻�  Error[%d]", iStep);
				putListLog(sLog);
				MandoInspLog.bInspRes = false;
			}
		}
		if (sysData.m_iStaintInspPass == 1)	// �̹��˻� ����
		{
			sLog.Format(_T("[PASS] Stain Insp"), iStep);
			putListLog(sLog);

		}
		else
		{
			if (g_clPriInsp.func_Insp_Stain(MIU.vDefectMidBuffer_6500K, false) == true)
			{
				putListLog("[�˻�] Stain �˻� ����.");
			}
			else
			{
				sLog.Format("[�˻�] Stain �˻�  Error[%d]", iStep);
				putListLog(sLog);
				MandoInspLog.bInspRes = false;
			}
		}
        iRtnFunction = 127000;
        break;
    case 127000:
        if (motor.PCB_Z_Motor_Move(Wait_Pos))
        {
            sLog.Format("PCB Z�� ��� ��ġ �̵� �Ϸ�[%d]", iStep);
            putListLog(sLog);
            iRtnFunction = 127600;
        }
        else
        {
            sLog.Format("PCB Z�� ��� ��ġ �̵� ����[%d]", iStep);
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
				sLog.Format("CCD ���� Close ���� [%d]", iStep);	//CCD ���� Close ����[%d]
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
        MandoInspLog.func_LogSave_UVAfter();	//���� ������Camera �˻� Log ����(�˻�/������ ���� -> ����Log)
        //! �˻� Count ó��
        g_FinalInspLog();
		//inspMES();
		//MESCommunication.Messave();
        work.iInspCnt_Output++;
        if (MandoInspLog.bInspRes == false)	work.iInspCnt_NG_Output++;
        dispInspResGrid();
        work.Save();
        work.Load();

        Task.m_timeChecker.Measure_Time(13);	//Final �˻� �Ϸ� �ð�
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
            MandoInspLog.func_DrawNG_Overlay(CCD);	//ȭ�� Overlay�� NG List Draw
            work.m_iCnt_NG_Output++;
        }
        vision.drawOverlay(CCD, true);

        //ccd���� ��ȯ

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
            sLangChange.LoadStringA(IDS_STRING748);	//Lens�� ���� ��� ��ġ �̵� ����[%d]
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
            sLangChange.LoadStringA(IDS_STRING987);	//PCB ��� ��ġ �̵� �Ϸ� [%d]
            sLog.Format(sLangChange, iStep);
            putListLog(sLog);
            iRtnFunction = 129350;
        }
        else
        {
            sLangChange.LoadStringA(IDS_STRING986);	//PCB ��� ��ġ �̵� ���� [%d]
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
            sLog.Format("PCB TX,TY ��� ��ġ �̵� �Ϸ� [%d]", iStep);
            putListLog(sLog);
            iRtnFunction = 129500;
        }
        else
        {
            sLog.Format("PCB TX,TY �����ġ �̵� ����[%d]", iStep);
            errMsg2(Task.AutoFlag, sLog);
            iRtnFunction = -129400;
        }
        break;

    case 129500:
        Task.m_bOkDispense = -1;	//���� ���� �Ϸ� Lamp ���
        putListLog("		");
        putListLog("		");
        sLog.Format("			END =======%s", Task.ChipID);
        putListLog(sLog);
        putListLog("		");
        putListLog("		");
        Task.m_bPBStart = 0;	//���ڵ� ���� �ʱ�ȭ
        if (true)
        {
            memset(Task.ChipID, 0x00, 256);
            sprintf_s(Task.ChipID, "EMPTY");
            func_ChipID_Draw();
        }

        Task.m_timeChecker.Measure_Time(14);	//����ǰ ���� �ð�
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
	bool bUseDispense = true;	//���� Pass ����
    
	if( (sysData.m_iDispensePass == 1) )
	{
		bUseDispense = false;//���� Pass Mode �̰ų�, ���� �Ϸ� �� ���
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
	case 122000:	//����ǰ �˻縸 �ش� (Final �˻��� X/Y/T ����)
		Sleep(dFinalDelay);
		iRtn = _getMTF(SFR_FINAL);
		if ( iRtn > 0 )
		{
			if ( !_calcImageAlignment())	//-- Align ���� Limit(������ �� �����...1.0)
			{
				sLog.Format("NG.  X : %.3lf Y : %.3lf", Task.m_dShift_IMG_X, Task.m_dShift_IMG_Y);
				putListLog(sLog);
				iRtnFunction = -122000;
				break;
			}
			else
			{
				//ƿ�ð� �ȸ��� ��� ���� �����ϰ� �ݺ���.. 3ȸ ������ �ϰ� �ȸ����� Spec IN ��Ű��..,
				if( Task.m_iFineThCnt > 5)	Task.m_dShift_IMG_TH = 0.0;	
				if( fabs(Task.m_dShift_IMG_TH) <= sysData.dSpec_OC_Theta )//X/Y �������� Spec In�� ��� ���� Step
				{
					sLog.Format("	���� Shift. Spec In(%.03lf) In : T: %.3lf",sysData.dSpec_OC_Theta, Task.m_dShift_IMG_TH);
					putListLog(sLog);
					iRtnFunction = 122500;
				}
				else
				{
					sLog.Format("	���� Shift(Spec : %.03lf).  T: %.3lf",sysData.dSpec_OC_Theta, Task.m_dShift_IMG_TH);
					putListLog(sLog);
					if( bUseDispense == false ) //AAB, ���� �̻���� ���->���� Shift���ϰ� ����
					{
						iRtnFunction = 122500;
					}
					else//���� Shift ����
					{
						Task.m_iFineThCnt++;
						iRtnFunction = 122100;
					}
				}
			}
		}
		else
		{
			if (askMsg("����?") == IDOK)
			{
				iRtnFunction = 122500;
				break;
			}
			else
			{
				errMsg2(Task.AutoFlag, "MTF ���� �� SFR ���� ȹ�� ����..");
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
	case 122500:	//Final �� �˻� - Ȯ�ο�
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
			putListLog("[�˻�] MTF �˻� ����.");// Optic_Axis,
		}
		else
		{
			putListLog("[�˻�] MTF �˻� ����.");// Optic_Axis,
		}
		Task.m_iDrawBarStep = Task.m_iCnt_Step_AA_Total;
		autodispDlg->DrawBarGraph();
		iRtnFunction = 122550;

		break;
	case 122550:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB�� z�� ���� ��ġ �̵� ����[%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = -122550;
			break;
		}

		if (!motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING747);	//Lens�� z�� ���� ��ġ �̵� ����[%d]
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
		if (sysData.m_iDefectInspPass == 1)	// �̹��˻� ����
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
			sLog.Format("PCB�� Dark ��ġ �̵� ����[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = -122850;
		}
		else
		{
			sLog.Format("PCB�� Dark ��ġ �̵� �Ϸ�[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = 122900;
		}
		
		break;
	case 122900:
		if (!motor.PCB_Z_Motor_Move(Dark_Pos))
		{
			sLog.Format("PCB�� Dark Z ��ġ �̵� ����[%d]", iStep);
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
			sLog.Format("PCB Z DAR ����Ȯ�� �ð� �ʰ�[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -122900;
		}
		
		break;
	case 122970:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB�� z�� ���� ��ġ �̵� ����[%d]
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
			sLangChange.LoadStringA(IDS_STRING1023);	//PCB�� ���� Defect �˻� ��ġ �̵� ����[%d]
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
			sLog.Format("PCB Z 6500K ����Ȯ�� �ð� �ʰ�[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -123100;
		}
		break;
	case 124000:
		Sleep(dFinalDelay);
		if (sysData.m_iDefectInspPass == 1)	// �̹��˻� ����
		{
			sLog.Format(_T("[PASS] Defect Insp"), iStep);
			putListLog(sLog);
		}
		else
		{
			if (g_clPriInsp.func_Insp_Defect(MIU.vDefectMidBuffer_6500K, MIU.vDefectLowBuffer, true) == true)	//AA
			{
				putListLog("[�˻�] Defect �˻� ����.");
			}
			else
			{
				sLog.Format("[�˻�] Defect �˻�  Error[%d]", iStep);
				putListLog(sLog);
				MandoInspLog.bInspRes = false;
			}
		}
		if (sysData.m_iStaintInspPass == 1)	// �̹��˻� ����
		{
			sLog.Format(_T("[PASS] Stain Insp"), iStep);
			putListLog(sLog);
			
		}
		else
		{
			if (g_clPriInsp.func_Insp_Stain(MIU.vDefectMidBuffer_6500K, false) == true)
			{
				putListLog("[�˻�] Stain �˻� ����.");
			}
			else
			{
				sLog.Format("[�˻�] Stain �˻�  Error[%d]", iStep);
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
				sLog.Format("CCD ���� Close ���� [%d]", iStep);	//CCD ���� Close ����[%d]
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
			sLog.Format("PCB Z�� ��� ��ġ �̵� �Ϸ�[%d]", iStep);
			putListLog(sLog);
			iRtnFunction = 127000;
		}
		else
		{
			sLog.Format("PCB Z�� ��� ��ġ �̵� ����[%d]", iStep);
			sLog.Format(sLangChange, iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -125000;
		}
		break;
	case 127000:
		//MIU.m_acsInspImage.Unlock();
		Task.m_iRetry_Opt = 0; 
		MandoInspLog.func_LogSave_UVAfter();	//���� ������Camera �˻� Log ����(�˻�/������ ���� -> ����Log)
		/*inspMES();
		MESCommunication.Messave();*/
		//! �˻� Count ó��
		work.iInspCnt_Output++;	
		if(MandoInspLog.bInspRes == false)	work.iInspCnt_NG_Output++;
		dispInspResGrid();
		work.Save();
		work.Load();

		Task.m_timeChecker.Measure_Time(13);	//Final �˻� �Ϸ� �ð�
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
			MandoInspLog.func_DrawNG_Overlay(CCD);	//ȭ�� Overlay�� NG List Draw
			work.m_iCnt_NG_Output++;
		}
		vision.drawOverlay(CCD, true);

		//ccd���� ��ȯ

		ctrlSubDlg(MAIN_DLG);
		setCamDisplay(3, 1);
		changeMainBtnColor(MAIN_DLG);
		iRtnFunction = 127400;
		break;

	case 127400:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLog.Format("PCB Z�� ��� ��ġ �̵� ����[%d]", iStep);
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
			sLangChange.LoadStringA(IDS_STRING987);	//PCB ��� ��ġ �̵� �Ϸ� [%d]
			sLog.Format(sLangChange, iStep);
			putListLog(sLog);
			iRtnFunction = 127900;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING986);	//PCB ��� ��ġ �̵� ���� [%d]
			sLog.Format(sLangChange, iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -127500;
		}
		break;
	case 127900:

		//////Dio.PcbGrip(false, false);   //�������?
		
		iRtnFunction = 128000;
		break;
	case 128000:
		if(!motor.Lens_Motor_MoveXY(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING748);	//Lens�� ���� ��� ��ġ �̵� ����[%d]
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
			sLangChange.LoadStringA(IDS_STRING745);	//LENS�� Tx, Ty�� ��� ��ġ �̵� ����
			sLog.Format(sLangChange);
			putListLog(sLog);	
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -128200;
		}
		break;
	case 128500:
		if( motor.Pcb_Tilt_Motor_Move(Wait_Pos))
		{
			sLog.Format("PCB TX,TY ��� ��ġ �̵� �Ϸ� [%d]", iStep);
			putListLog(sLog);
			iRtnFunction = 129000;
		}
		else
		{
			sLog.Format("PCB TX,TY �����ġ �̵� ����[%d]", iStep);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -128500;
		}
		break;
	case 129000:
		iRtnFunction = 129500;
		break;
	case 129500:
		Task.m_bOkDispense = -1;	//���� ���� �Ϸ� Lamp ���
		putListLog("		");
		putListLog("		");
		sLog.Format("			END =======%s" , Task.ChipID);
		putListLog(sLog);
		putListLog("		");
		putListLog("		");
		putListLog("		");
		putListLog("		");
		Task.m_bPBStart = 0;	//���ڵ� ���� �ʱ�ȭ
		if(true)
		{
			memset(Task.ChipID, 0x00, 256);
			sprintf_s(Task.ChipID, "EMPTY");
			func_ChipID_Draw();
		}

		Task.m_timeChecker.Measure_Time(14);	//����ǰ ���� �ð�
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
			if(Task.m_bOKLensPass==1)   //Lens �ѱ� �Ϸ����
			{ 
				iRtnFunction = 10160;
				sLog.Format("		Ready Step [%d]" , iRtnFunction);
				putListLog(sLog);
			}else
			{
				sLog.Format("[���� �غ�] Lens Grip ���� ���� �Դϴ�[%d]\n����ѱ���� Ȯ�ιٶ��ϴ�", iStep);
				errMsg2(Task.AutoFlag, sLog);
				iRtnFunction = -10150;
				break;
			}
		}else
		{
			if(!Dio.LensMotorGripCheck(false, false))
			{
				sLog.Format("[���� �غ�]Lens Grip ���� Ȯ�� ���� [%d]", iStep);
				errMsg2(Task.AutoFlag, sLog);
				iRtnFunction = -10150;
			}else
			{
				if(Task.m_bOKLensPass==1)   //Lens �ѱ� �Ϸ����
				{ 
					sLog.Format("[���� �غ�]Lens Grip ���� ���� [%d]]\n����ѱ���� Ȯ�ιٶ��ϴ�", iStep);
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
		
		//Laser �Ǹ��� �ʱ�ȭ
	case 10160:
		if (Dio.LaserCylinderUPDOWN(true, false))
		{
			sLog.Format("LASER �Ǹ��� ��� �Ϸ�[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(sLog);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 10170;
		}
		else
		{
			sLog.Format("LASER �Ǹ��� ��� ���� [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -10160;
		}
		break;

	case 10170:
		if (Dio.LaserCylinderCheck(true, false))
		{
			sLog.Format("LASER �Ǹ��� ��� Ȯ�� �Ϸ�[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(sLog);
			iRtnFunction = 10200;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			sLog.Format("LASER �Ǹ��� ��� �ð� �ʰ�..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, sLog);
			iRtnFunction = -10170;
		}
		break;
		//Laser �Ǹ��� �ʱ�ȭ END

	case 10200:
		if (motor.PCB_Z_Motor_Move(Wait_Pos))		//PCB Z�� ��� ��ġ �̵�
		{
			iRtnFunction = 10300;
			sLog.Format("		Ready Step [%d]", iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING305);
			sLog.Format(sLangChange, iStep);	//"[���� �غ�] PCB Z�� �����ġ �̵� ����[%d]"
			putListLog(sLog);
			iRtnFunction = -10200;
		}
		
		break;

	case 10300:
		if (motor.LENS_Z_Motor_Move(Wait_Pos))		//Lens Z�� ��� ��ġ
		{
			iRtnFunction = 10400;
			sLog.Format("		Ready Step [%d]", iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING303);
			sLog.Format(sLangChange, iStep);	//"[���� �غ�] Lens Z�� �����ġ �̵� ����[%d]"
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
		if(motor.Pcb_Motor_Move(Wait_Pos))			//PCB�� ���� �����ġ �̵�
		{
			iRtnFunction = 10700;
			sLog.Format("		Ready Step [%d]" , iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING306);
			sLog.Format(sLangChange, iStep);	//"[���� �غ�] PCB �����ġ �̵� ����[%d]"
			putListLog(sLog);
			iRtnFunction = -10600;
		}
		break;
	case 10700:
		if(motor.Lens_Motor_MoveXY(Wait_Pos))			//Lens�� ���� ��� ��ġ �̵�(Lens X/Y/Z��)
		{
			iRtnFunction = 10750;
			sLog.Format("		Ready Step [%d]" , iRtnFunction);
			putListLog(sLog);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING304);
			sLog.Format(sLangChange, iStep);	//"[���� �غ�] Lens �����ġ �̵� ����[%d]"
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
			sLangChange.LoadStringA(IDS_STRING1017);	//PCB�� Tx, Ty�� ��� ��ġ �̵� ����
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
			sLangChange.LoadStringA(IDS_STRING745);	//LENS�� Tx, Ty�� ��� ��ġ �̵� ����
			sLog.Format(sLangChange);
			putListLog(sLog);	
			iRtnFunction = -10800;
		}
		break;

	case  10900:
			///////////////////////
		//if( !Dio.LensGrip(true, true) )
		//{		//-- Lens ���� ���¿��� Grip ���� �ȵ� ��� NG
		//	sLangChange.LoadStringA(IDS_STRING300);
		//	sLog.Format(sLangChange, iStep);	//"[���� �غ�] Lens Grip ���� ���� [%d]"
		//	putListLog(sLog);
		//	iRtnFunction = -10900;
		//	break;
		//}			
		//else if( Dio.LensGripCheck(false, false) )			//Lens ������ �ȵǾ��� ���  
		//{//-- Lens Grip ���� ����
		//if(! Dio.HolderGrip(false,false) )    //-- Lens ���� �ȵǰ�/ Grip ���� �ȵ� ��� NG
		//{  
		//	sLangChange.LoadStringA(IDS_STRING301);
		//	sLog.Format(sLangChange, iStep);	//"[���� �غ�] Lens Grip ���� ���� [%d]"
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
		iRtnFunction = 15000;	//While�� ����
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



// MilProcImageChild[4] �̿� �˻� ����..
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


	if(centAvg < 20/*60*/)	//KYH Mando Camera ���� ����
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

	if(centAvg - upperAvg < 10/*20*/)	//KYH Mando Camera ���� ����
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
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;	//PCB ���� ȹ�� Delay
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser ������ Delay
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
		case 11000:	// PCB MIU ����	
			Sleep(1000);
			Task.oldLaserTx = 0.1;
			Task.oldLaserTy = 0.1;
			Task.PCBTaskTime = myTimer(true);
			if (sysData.m_FreeRun == 0)
			{
				bThread_MIUCheckRun = true;
				if (!MIUCheck_process() || gMIUDevice.CurrentState != 4)
				{
					sLangChange.LoadStringA(IDS_STRING452);	//CCD ��� ���� �ʱ�ȭ ����.[%d]\n ��ǰ ���� ���� �� ��ǰ �ҷ� Ȯ�� �ϼ���.
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
			//Ȯ���ʿ� norinda
			Task.m_bPBStart = 1;
			//if( Task.m_bPBStart ==  1 )
			//{
			//	func_Socket_Barcode();
			//}
			//else
			//{
			//	//! ���ڵ尡 ���� ������ ��� ��� ����.
			//	Task.iRecvLenACK[0] = Task.iRecvLenACK[1] = 0;
			//	Task.dTiltingManual[0] = Task.dTiltingManual[1] = 0.0;
			//	Task.dAlignManual[0] = Task.dAlignManual[0] = Task.dAlignManual[0] = 0.0;
			//}
			iRtnFunction = 12100;
			break;

	case 12100:

		Task.m_iRetry_Opt = 0;
		//SetDigReference(PCB_Holder_MARK);
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_HOLDER]);		// Align�� ���� ON

		Task.d_Align_offset_x[PCB_Chip_MARK] = 0;
		Task.d_Align_offset_y[PCB_Chip_MARK] = 0;
		Task.d_Align_offset_th[PCB_Chip_MARK] = 0;

		if(motor.Pcb_Motor_Move(CompleteAlign_Pos))
		{
			Task.PCBTaskTime = myTimer(true);
			logStr.Format("Pcb ����ǰ Align ��ġ �̵� �Ϸ�[%d]", iStep);
  			putListLog(logStr);
  			iRtnFunction = 12200;
		}
		else
		{
			logStr.Format("Pcb ����ǰ Align ��ġ �̵� ����[%d]", iStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -12100;
		}
		break;
	case 12200:
		if(motor.PCB_Z_Motor_Move(CompleteAlign_Pos))
		{
			Task.PCBTaskTime = myTimer(true);
			logStr.Format("Pcb ����ǰ Align Z��ġ �̵� �Ϸ�[%d]", iStep);
  			putListLog(logStr);
  			iRtnFunction = 12400;
		}
		else
		{
			logStr.Format("Pcb ����ǰ Align Z��ġ �̵� ����[%d]", iStep);
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
			logStr.Format("PCB ����ǰ Align ��˻� %d ȸ ����[%d]\n ���� ���������� �����Ͻðڽ��ϱ�?", Task.m_iRetry_Opt, iStep);
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
				logStr.Format("PCB Align ��˻� %d ȸ ����[%d]", Task.m_iRetry_Opt, iStep);
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

			logStr.Format("PCB ����ǰ Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
			//pcbDlg->m_labelAlignResult.SetText(logStr);
			//pcbDlg->m_labelAlignResult.Invalidate();
			putListLog(logStr);

			int iRtnVal = AlignLimitCheck(CAM1, offsetX, offsetY, offsetTh); // PCB����� �����̴� �κ��̶� Limit Check �Ҷ� CAM1��

			if(iRtnVal==1)
			{
				iRtnFunction = 12700;				// ���� �̵�
			}
			else if(iRtnVal==2)
			{
				iLaser_Pos = 0;
				logStr.Format("		PCB ����ǰ Align [%d] �Ϸ� [%d]", Task.m_iRetry_Opt, iStep);
				putListLog(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);
				logStr.Format("		PCB ����ǰ Align Complete :  %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
				AlignResultSave(logStr);	
				Task.m_iRetry_Opt = 0;
				Task.dSocketRecvTime = myTimer(true);
				iRtnFunction = 12800;
				
			}
			else
			{
				logStr.Format("PCB ����ǰ Align [%d] ���� ���� �ʰ�[%d]", Task.m_iRetry_Opt, iStep);
				errMsg2(Task.AutoFlag,logStr);
				AlignLogSave(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);
				iRtnFunction = -12500;	
			}
		}
		else
		{
			logStr.Format("PCB ����ǰ Align [%d] ��ũ �ν� ����[%d]", Task.m_iRetry_Opt, iStep);
			putListLog(logStr);
			iRtnFunction = 12500;				//  ��˻� 
		}
		break;

	case 12700://!! ������ �̵�.- Retry
		if(motor.Pcb_Holder_Align_Move(CompleteAlign_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 12200;
		}
		else
		{
			logStr.Format("Pcb ����ǰ Align ���� ��ġ �̵� ����[%d]", iStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -12700;
		}
		break;
	case 12800:
		//LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		//������ �˻��� off
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB�� z�� ���� ��ġ �̵� ����[%d]
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
		//�ܺ� ���������� Read
#if (____AA_WAY == PCB_TILT_AA) 
		if (g_ADOData.func_Get_RecordData(Task.ChipID))//DB�� Data�� ���� ���
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
			logStr.Format("PCB ȭ��˻� TX(%.2lf), TY(%.2lf)", Task.dAAPcbMotor[3] , Task.dAAPcbMotor[4]);
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
			//logStr.Format("��ġ �� �����ϴ�.\ bonding ��ġ�� ���� �Ͻðڽ��ϱ�?");	//��ü ���� ���͸� ���� �Ͻðڽ��ϱ�?
			//if (askMsg(logStr))
			//{

			//	
			//	break;
			//}
			/*logStr.Format("��ġ �� �����ϴ�.[%d]", iStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -12850;*/
			//��ġ �� ���� ��� ���� ƼĪ�� â����.
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
			logStr.Format("�������� Data �Է� �� �ڵ���� ���� ����[%d]", iStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -12870;
			break;
		}
		else if ( Task.m_iRtnTiltingDlg == 1 )
		{
			logStr.Format("�ܺ� ���� Tilting �Ϸ� ��ġ : Tx=%.03lf, Ty=%.03lf", Task.dTiltingManual[0], Task.dTiltingManual[1]);
  			putListLog(logStr);
			
			Task.m_timeChecker.Measure_Time(4);	//Laser ���� ���� ���� �ð�
			//iRtnFunction = 12900;//pcb aa�� ������ �ϵ��� 200708

			iRtnFunction = 12900;

		}
		break;
	case 12900:	
		if (sysData.m_iLaserInspPass == 1)		//����ǰ
		{
			iRtnFunction = 13300;		//����ǰ�� ������ �н�
			break;
		}

		
		if(motor.Pcb_Motor_Move_Laser(iLaser_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			if(motor.PCB_Z_Motor_Move(Com_Laser_Pos))//Laser_Lens_Pos))
			{
				Task.PCBTaskTime = myTimer(true);
				logStr.Format("Laser ���� ���� �ܺ� ��ġ �̵� �Ϸ�[%d]", iStep);
  				putListLog(logStr);
				iRtnFunction = 12910;// 12950;
			} else {
				logStr.Format("PCB Com_Laser_Pos Z �̵� ����[%d]", iStep);
				putListLog(logStr);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -12900;
				break;
			}
		}
		else
		{
			logStr.Format("Laser ���� ���� �ܺ� ��ġ �̵� ����[%d]", iStep);
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
			logStr.Format("LASER �Ǹ��� �ϰ� �Ϸ�[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 12920;
		}
		else
		{
			logStr.Format("LASER �Ǹ��� �ϰ� ���� [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -12910;
		}
		break;

	case 12920:
		if (Dio.LaserCylinderCheck(false, false))
		{
			logStr.Format("LASER �Ǹ��� �ϰ� Ȯ�� �Ϸ�[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 12950;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER �Ǹ��� �ϰ� �ð� �ʰ�..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -12920;
		}
		break;
		//laser�Ǹ��� �ϰ� END
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
		logStr.Format("���� ���� %lf[%d] - �ܺ� - %d ��ġ", Task.m_Laser_Point[iLaser_Pos], iStep, iLaser_Pos+1);
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
					logStr.Format("Laser ������ �̻�..�ܺ� ���� �������� �̻�..[%d]\n ������:%.04lf, %.04lf, %.04lf, %.04lf ", iStep, LaserValue[0], LaserValue[1], LaserValue[2], LaserValue[3]);
					errMsg2(Task.AutoFlag,logStr);
 					iRtnFunction = -13100;
					break;
				}

				if(_calcLaserTilt(LaserPos, LaserValue, Task.d_Align_offset_xt[0], Task.d_Align_offset_yt[0]))
				{
				/*	logStr.Format("		������ Tx: %.04lf, Ty: %.04lf", Task.d_Align_offset_xt[0], Task.d_Align_offset_yt[0] );
  					putListLog(logStr);
					logStr.Format("		���� ������ ���� Tx: %.04lf, Ty: %.04lf", Task.dTiltingManual[0], Task.dTiltingManual[1] );
  					putListLog(logStr);
					int Rnd = TiltAlignLimitCheck(Task.d_Align_offset_xt[0] + Task.dTiltingManual[0], Task.d_Align_offset_yt[0] + Task.dTiltingManual[1] );
					*/

					
					//Task.m_dataOffset_x[0] =  Task.d_Align_offset_xt[PCB_Holder_MARK];
					//Task.m_dataOffset_y[0] =  Task.d_Align_offset_yt[PCB_Holder_MARK];
					logStr.Format("		������ Tx: %.04lf, Ty: %.04lf", Task.d_Align_offset_xt[0], Task.d_Align_offset_yt[0] );
  					putListLog(logStr);
					logStr.Format("		�ܺ� ������ ���� Tx: %.04lf, Ty: %.04lf", Task.dTiltingManual[0], Task.dTiltingManual[1] );
  					putListLog(logStr);
					//int Rnd = TiltAlignLimitCheck(Task.d_Align_offset_xt[PCB_Holder_MARK],Task.d_Align_offset_yt[PCB_Holder_MARK]);
					//int Rnd = TiltAlignLimitCheck(Task.d_Align_offset_xt[0] + Task.dTiltingManual[0], Task.d_Align_offset_yt[0] + Task.dTiltingManual[1] );
					int Rnd = TiltAlignLimitCheck(Task.d_Align_offset_xt[0], Task.d_Align_offset_yt[0]);
					if(Rnd == 2)
					{
						Task.m_timeChecker.Measure_Time(5);	//Laser ���� ���� �Ϸ� �ð�
						Task.m_dTime_LaserDpm = Task.m_timeChecker.m_adTime[5] - Task.m_timeChecker.m_adTime[4]; 
						//dispGrid();
						iRtnFunction = 13300;	
						
					}
					else if (Rnd == 1)
					{
						iRtnFunction = 13200;	//������ �̵�		
					}
					else
					{
						logStr.Format("Laser Tilt  ������ Limit�� �ʰ� �Ͽ����ϴ�.[%d]", iStep);
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
			logStr.Format("LASER �Ǹ��� ��� �Ϸ�[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 13310;
		}
		else
		{
			logStr.Format("LASER �Ǹ��� ��� ���� [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -13300;
		}
		break;
	case 13310:
		if (Dio.LaserCylinderCheck(true, false))
		{
			logStr.Format("LASER �Ǹ��� ��� Ȯ�� �Ϸ�[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 13350;
			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER �Ǹ��� ��� �ð� �ʰ�..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -13310;
		}
		break;
		//laser�Ǹ��� ��� END
	case 13350:	
		if(motor.PCB_Z_Motor_Move(Wait_Pos)) 
		{
			logStr.Format("PCB Z ��� ��ġ �̵� �Ϸ�[%d]", iRtnFunction);
  			putListLog(logStr);
  			iRtnFunction = 14000;
		}
		else
		{
			logStr.Format("PCB Z ��� ��ġ �̵� ����[%d]", iRtnFunction);
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

		iRtnFunction = 14400;				// �Ϸ�
		break;
	case 14400:	
		iRtnFunction = 14500;
		break;
	case 14500:	
		if(motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB Z ��� ��ġ �̵� �Ϸ�[%d]", iStep);
  			putListLog(logStr);
  			iRtnFunction = 15200;
		}
		else
		{
			logStr.Format("PCB Z ��� ��ġ �̵� ����[%d]", iStep);
  			putListLog(logStr);
  			iRtnFunction = -14500;
		}
		break;
	case 15200:	
		iRtnFunction = 19600;
		break;	
		  
	case 19600: // ���� ��ġ �̵� 
		//if(motor.Pcb_Motor_Move(Bonding_Pos, Task.d_Align_offset_x[PCB_Holder_MARK] + Task.dAlignManual[0], Task.d_Align_offset_y[PCB_Holder_MARK] + Task.dAlignManual[1], Task.d_Align_offset_th[PCB_Holder_MARK] + Task.dAlignManual[2]) )
  		if(motor.Pcb_Motor_Move(Bonding_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
  		{
			Task.PCBTaskTime = myTimer(true);
  			logStr.Format("PCB Bonding ���� ��ġ �̵� �Ϸ�[%d]", iStep);
  			putListLog(logStr);
  			iRtnFunction = 19700;
  		}
  		else
  		{
  			logStr.Format("PCB Bonding ��ġ �̵� ����[%d]", iStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -19600;
  		}
		break;
	case 19700:
		iRtnFunction = 19800;//pcb aa�� ������ �ϵ��� 200708
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
	case 19800: //z�� ���� ��ġ �̵�
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



			//if( func_MIU_ConnectLiveCheck() == false)		//MIU ���� Check
			//{
			//	iRtnFunction = -19900;
			//	break;
			//}
			setCamDisplay(3, 1);

			iRtnFunction = 120000;//����ǰ �˻�
		}
		break;
		
	default:
		sLangChange.LoadStringA(IDS_STRING1004);	//PCB ����ǰ �˻� Thread Step ��ȣ ������ . [%d]
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
		sLangChange.LoadStringA(IDS_STRING1374);	//�ڵ��������� �ƴմϴ�.
		errMsg2(Task.AutoFlag, sLangChange);
		return;
	}

	Dio.setAlarm(AUTO_STOP);


	//if(sysData.m_FreeRun == 1)
	//{
	//	Task.AutoFlag		= 0;			//	// 0:���� 1:�ڵ� 2: �Ͻ����� 20130405	
	//	Task.PausePCBStep	= 0;
	//}
	//else
	//{
		Task.AutoFlag		= 2;			//	// 0:���� 1:�ڵ� 2: �Ͻ����� 20130405	
		Task.PausePCBStep	= Task.PCBTask;
		//Task.PauseLensStep	= Task.LensTask;
	//}

	AutoRunView(Task.AutoFlag);
	Task.AutoReday = 0;

	bThreadTaskPcb =0;			//	 ���� �Ҷ� ��� 0:������ ���� ����
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
		putListLog("MIU Stop ����.");
		MIU.Close();					// 95 ~ 100 msec//MIU.Close();					// 95 ~ 100 msec
		putListLog("MIU Stop �Ϸ�.");
	}
	DisableButton(Task.AutoFlag);
	g_bMovingflag=false;
}



void CAABonderDlg::OnBnClickedButtonAutorun()
{
	if(g_bMovingflag){
		putListLog("	�����̵���!");
		return;
	}

	if(Task.AutoFlag == 1)
	{
		return;
	}
	if ( bThreadTaskReadyRun == true)
	{
		putListLog("	�����غ����Դϴ�!");
		return;
	}
	if(Task.AutoReday == 0 && Task.AutoFlag == 0)
	{
#ifndef	ON_LINE_MODE
		Task.AutoFlag = 1;
#endif
		sLangChange.LoadStringA(IDS_STRING1326);	//���� �غ� ���� �ʾҽ��ϴ�.
		errMsg2(Task.AutoFlag, sLangChange);
		return;
	}


	if( Dio.StartPBOnCheck(true, false) && Task.AutoFlag != 2 )
	{//����ġ�� ������ �ְ�, �Ͻ� ���� ���°� �ƴ� ��츸 Check
		sLangChange.LoadStringA(IDS_STRING1383);	//���� Door Start/Stop ����ġ�� On ���� �Դϴ�.
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
	{// 0:���� 1:�ڵ� 2: �Ͻ����� 20130405
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
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
		return;
	}

	if(Task.AutoFlag==2)
	{
		sLangChange.LoadStringA(IDS_STRING1362);	//"�Ͻ� ���� �� �Դϴ�."
		delayMsg(sLangChange, 3000, M_COLOR_RED);
		return;
	}

	//if(Dio.PCBCoverCloseCheck(true, false) == false)
	//{
	//	CString sTemp;
	//	sLangChange.LoadStringA(IDS_STRING1026);	//PCB�� Ŀ�� ������ ���� �ֽ��ϴ�.
	//	sTemp.Format(sLangChange);
	//	delayMsg(sTemp.GetBuffer(99), 3000, M_COLOR_DARK_GREEN);
	//	return ;
	//}
	CString sTemp="";
	Dio.StartPBLampOn(false);

	/*if (!Dio.CamLaserSlinderMove(false, false))
	{
		sTemp.Format("Laser/Cam �Ǹ��� ���� ����");
		errMsg2(Task.AutoFlag, sTemp);
		return;
	}
	if (!Dio.CamLaserSlinderCheck(false, true))
	{
		sTemp.Format("Laser/Cam �Ǹ��� ���� Ȯ�� ����");
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
	//		sLangChange.LoadStringA(IDS_STRING1471);	//���� �غ� ����. Door Open ����..
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
{//���� ������ Camer �˻� ��� 
	dispInspResGrid();
}

void CAABonderDlg::dispInspResGrid()
{

}

void CAABonderDlg::InitGridCtrl_Result()
{
	CRect rect;
	CWnd *pWnd= (CWnd*)GetDlgItem(IDC_STATIC_RESULT_GRID); 
	ResultRow = 8;//�Ʒ�
	ResultCol = 2;//��
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
	pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.

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
	m_clGridResult.SetItemText(1, 0, "���귮");
	m_clGridResult.SetItemText(2, 0, "NG ����");
	m_clGridResult.SetItemText(3, 0, "TOTAL T/T");
	m_clGridResult.SetItemText(4, 0, "Dispensing T/T");
	m_clGridResult.SetItemText(5, 0, "Laser ���� T/T");
	m_clGridResult.SetItemText(6, 0, "AA T/T");
	m_clGridResult.SetItemText(7, 0, "�˻� T/T");


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
		sTemp.Format("���귮�� �ʱ�ȭ �Ͻðڽ��ϱ�?");
		if ( askMsg(sTemp.GetBuffer()) == IDOK )
		{
	
			work.m_iCnt_Output = 0;
			work.m_iCnt_NG_Output = 0;
			// 20141103 LHC - ���귮 �ʱ�ȭ�� T/T �ʱ�ȭ
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
	MESCommunication.m_dMesDeltaOC[0] = model.m_oc_x;//delta �� ������
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


	if(fabs(dShiftX) > 3.0 || fabs(dShiftY) > 3.0 || fabs(dAng) > 20.0)//3.0)		// ���ǰ�...
	{

		if(!Task.bInsCenter)
		{
			errMsg2(Task.AutoFlag, "Image Center �̹��� ����� ������ ����Ʈ ����.");
			return false;
		}
		else
		{
			sLog.Format("Image Center ���� error");		//Image Center ���� error
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
  		sLog.Format("[�˻�] Image Center [X=%f, Y=%f, T=%f]", dShiftX, dShiftY, dAng);
		putListLog(sLog);
	}

	Task.m_dShift_IMG_X = dShiftX;
	Task.m_dShift_IMG_Y = dShiftY;
	sLog.Empty();
	
	return true;
}


bool CAABonderDlg::_MotorMove_IMG_Align()//���� sfr
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


	motor.MoveAxis(Motor_PCB_TH, REL, Task.m_dShift_IMG_TH * dicTH, sysData.fMotorSpeed[Motor_PCB_TH], sysData.fMotorAccTime[Motor_PCB_TH]);//���� Ȯ���ؾߵ�
	Sleep(10);

	while (1)
	{
		if ( ( motor.IsStopAxis(Motor_PCB_TH)))// && motor.GetInposition(Motor_PCB_TH) ) )
		{
			break;
		}
		else if ( myTimer(true)-ep > 1000 )
		{
			sLangChange.LoadStringA(IDS_STRING1357);	//�̹��� ����� PCB Theta�� ���� �̵� �ð� �ʰ�.
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
		sLangChange.LoadStringA(IDS_STRING1463);//"�ڵ� ���� �� ��� �Ұ�"
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
	int iIndex_Check = model.m_iCnt_Check_SFR;//�Ķ����â ���� , ����3
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
		// 20140905 Overlay Box ��ġ �̵� �� Box ������ �󸶳� ������ �־�� ���콺 Ŀ�� ��� ���� �Ÿ� Ȯ�� ��..
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



// readMode�� ���� 8M (Y475A), 13M (Y472A) ����.
// 0 : 13M CCD. Address 0x3B02 - 0����, 0x3B00 -1�� Write �� 0x3B24���� Read.. 
// 1 :  8M CCD. Address 0x01������ 0x09���� ������� Read.. (0x05 ����)

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
	//else if(readMode == 1)				// 8M CCD. Address 0x01������ 0x09���� ������� Read.. (0x05 ����)
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

void CAABonderDlg::AutoRunView(int curstate)		// Display��
{// ����:0 �ڵ�����:1 �Ͻ�����:2 �����غ�:3 
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
		//double Input_Spec[4]={0.0625,0.125,0.25,0.5};1/16 , 1/8, 1/4, 1/2 ���ļ��� SFR �� ���
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


bool CAABonderDlg::checkDarkDefect_PreUV()		// Dark�˻� ��� Ȯ�ο�.	// Defect Pixel�˻� �ܺ�DLL ���
{
	return true;
}

bool CAABonderDlg::checkLightDefect_PreUV()		// ���� ��� Ȯ�ο�.
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
		sTemp.Format(" -------------MTF ���� �˻�");	
	}else
	{
		sTemp.Format(" -------------MTF �˻� [step:%d]", Task.PCBTask);	
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
    Task.getROI();				// ���� ��ũ ��ġ �ν�..
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

	////m_stSFRSpec.tSFRConfig.eFrequencyUnit = ESFRFreq_LinePairPerMilliMeter;	//�����ڸ�
	m_stSFRSpec.tSFRConfig.eFrequencyUnit = ESFRFreq_CyclePerPixel;//�Ҽ���
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

    //bool bRtn = Task.getROI_SFR(MTF_INSP);				// �簢�� Box ��ġ �ν�..
    //if (!bRtn)
    //{
    //    if (Task.AutoFlag == 1) {
    //        saveInspImage(AA_CIRCLE_NG_IMAGE_SAVE, Task.m_iCnt_Step_AA_Total);
    //    }
    //    sLog.Format("[SFR ����] ���� ��ũ �ν� ����");
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
	
	if (Task.PCBTask == 65000)	// UV�� MTF�� ��� �Ǵܡڡڡڡڡڡڡڡڡڡ�
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
				//! Main Displayȭ�� Overlay NG List
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
		
		
		if(sysData.m_iMTFUvInsp==1 && sysData.m_iProductComp == 0)//uv�� �˻� ���� (1)
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
			//AA Retry �� ���� �˻�  , UV �� �˻��϶��� Pass�ż� �߰���
			/*if(dCenter<MandoSfrSpec.dAASFR_Spec[0][sfrIndex] || dTL05<MandoSfrSpec.dAASFR_Spec[4][sfrIndex] || dTR05<MandoSfrSpec.dAASFR_Spec[6][sfrIndex]
			|| dBL05<MandoSfrSpec.dAASFR_Spec[8][sfrIndex] || dBR05<MandoSfrSpec.dAASFR_Spec[10][sfrIndex])
			{
				return false;
			}*/
		}
		//==================================================================================================================================
	}
	else
	{ 	// UV�� MTF�� ��� �Ǵܡڡڡڡڡڡڡڡڡڡ�
		sfrIndex = 1;
		if(sysData.m_iProductComp == 1)
		{
			sfrIndex = 2;
		}
		if (dindex == 1)
		{
			sfrIndex = 3;
		}
		//uv�� ����
		for (int i = 0; i<model.mGlobalSmallChartCount; i++)
		{
			dMTFSpecTemp = MandoSfrSpec.dAASFR_Spec[i][sfrIndex];
			if (MandoInspLog.dMTF_PostUV[i] < dMTFSpecTemp)
			{
				sTemp.Format("MTF UV AFTER [%d] Spec NG: %lf (Spec:%.2f)", i, MandoInspLog.dMTF_PostUV[i], dMTFSpecTemp);
				//! Main Displayȭ�� Overlay NG List
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
		//		//! Main Displayȭ�� Overlay NG List
		//		MandoInspLog.sDispNG[MandoInspLog.iNGCnt].Format("MTF ����ǰ[%d]: %.2f(Spec:%.2f)",i, MandoInspLog.dMTF_PostUV[i],MandoSfrSpec.dAASFR_Spec[i][sfrIndex]);
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
		
		//=================uv��
		

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
		

		//if(sysData.m_iMTFUvInsp==0 || sysData.m_iProductComp == 1)//uv�� �˻� ���� (0)
		//{
		//	dMTFSpecTemp = MandoSfrSpec.dAASFR_Spec[1][sfrIndex]; 
		//	if(dCenter_Post < dMTFSpecTemp)
		//	{
		//		sTemp.Format("MTF %s Spec NG :%.3f(Spec:%.2f)", MtfPos[0],dCenter_Post, dMTFSpecTemp);
		//		//! Main Displayȭ�� Overlay NG List
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
		//		//! Main Displayȭ�� Overlay NG List
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
		//		//! Main Displayȭ�� Overlay NG List
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
		//		//! Main Displayȭ�� Overlay NG List
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
		//		//! Main Displayȭ�� Overlay NG List
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


//20170214 �̹��˻� �����߰� lee
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
	pInspectBlackSpot->Inspect((BYTE*)stainImg,  gMIUDevice.nWidth, gMIUDevice.nHeight, stSpecBlackSpot, gMIUDevice.dTDATASPEC_n.eDataFormat, gMIUDevice.dTDATASPEC_n.eOutMode, gMIUDevice.dTDATASPEC_n.eSensorType,0);////�ڡڡڡ�
	
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
			int ocx = (int)pCenterMaxresult->dContrastMaxR;//(�߽�X��ǥ);
			int ocy = (int)pCenterMaxresult->dContrastMaxGb;//(�߽�Y��ǥ);
			int radx =(int)pCenterMaxresult->dContrastMaxGr;//(X�� ������);
			int rady =(int)pCenterMaxresult->dContrastMaxB;//(Y�� ������);
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
	//����
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
	//����
	const TDefectResult* pCenterMaxResult = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_CENTER);
	const TDefectResult* pEdgeMaxResult = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_EDGE);
	const TDefectResult* pCornerMaxResult = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_CORNER);
			
	sTemp.Format("[RU_Ymean]Center max=%.02f, Edge max=%.02f, Corner max=%.02f", pCenterMaxResult->dValue, pEdgeMaxResult->dValue, pCornerMaxResult->dValue );
#else
	//wide  SALLEN_RIVIN   DCY11_AVM DCY11_RVC
	const TDefectResult* pCircleMaxResult = pInspectBlemish_Ymean->GetMaxDefectResult(EIMAGEREGION_CIRCLE);
	sTemp.Format("[RU_Ymean]Circle=%d, maxValue=%.02f, posX=%d, posY=%d", pInspectBlemish_Ymean->GetDefectCount(EIMAGEREGION_CIRCLE), pCircleMaxResult->dValue, pCircleMaxResult->ptPos.x, pCircleMaxResult->ptPos.y );
	int ocx = (int)pCircleMaxResult->dContrastMaxR;//(�߽�X��ǥ);
	int ocy = (int)pCircleMaxResult->dContrastMaxGb;//(�߽�Y��ǥ);
	int radx =(int)pCircleMaxResult->dContrastMaxGr;//(X�� ������);
	int rady =(int)pCircleMaxResult->dContrastMaxB;//(Y�� ������);
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

	//Mark_Area(Center.x-X-W/2,Center.y-Y, Center.x-X+W/2,Center.y+W/2, RGB(0,0,255), Width, Height, (LPBYTE)rgb_p);//����
	//Mark_Area(Center.x+X-W/2,Center.y-Y, Center.x+X+W/2,Center.y+W/2, RGB(0,0,255), Width, Height, (LPBYTE)rgb_p);//����

	double y1 = std::sqrt(std::pow(cross1.x - cols/2, 2) + std::pow(cross1.y - rows/2, 2));
	double y2 = std::sqrt(std::pow(cross2.x - cols/2, 2) + std::pow(cross2.y - rows/2, 2));

	double FoV = std::atan2(y1 * pixelsize, f) + std::atan2(y2 * pixelsize, f);


	//		Mark_Area(150,m_Height/2-200, 150+400, (m_Height/2-200)+400, RGB(0,0,255), Width, Height, (LPBYTE)m_pRgb);/���� �ּ�ó��

	//Mark_Area(cross1.x,cross1.y, cross1.x+10, cross1.y+10, RGB(0,0,255), Width, Height, (LPBYTE)rgb_p);//����
	//Mark_Area(cross2.x,cross2.y, cross2.x+10, cross2.y+10, RGB(0,0,255), Width, Height, (LPBYTE)rgb_p);//����
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
			Mark_Area(x+i,y+i,x+i,y+i, RGB(0,255,0), Width, Height, (LPBYTE)rgb_p);		// �밢�� Ȯ��

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
			Mark_Area(x+size-i,y+i, x+size-i,y+i, RGB(0,0,255), Width, Height, (LPBYTE)rgb_p);		// �밢�� Ȯ��
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
//			Mark_Area(iX,iY,iX,iY, rgb(0,100,100), Width, Height, (LPBYTE)rgb_p);		// ���μ� Ȯ��

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
			//Mark_Area(iX,iY,iX,iY, rgb(255,0,0), Width, Height, (LPBYTE)rgb_p);		// ���μ� Ȯ��


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
//			Mark_Area(iX,iY,iX,iY, rgb(0,100,100), Width, Height, (LPBYTE)rgb_p);		// ���μ� Ȯ��

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
//			Mark_Area(iX,iY,iX,iY, rgb(0,100,100), Width, Height, (LPBYTE)rgb_p);		// ���μ� Ȯ��

		}

		Center=CPoint((VH[0].x+VH[1].x+VL[0].x+VL[1].x)/4,(HL[0].y+HL[1].y+HR[0].y+HR[1].y)/4);

		if(abs(cpCenter.y - Center.y) < 20)break;
	}

		Mark_Area(VH[0].x,VH[0].y,VH[1].x,VH[1].y, rgb(0,255,100), Width, Height, (LPBYTE)rgb_p);		// ã�� �� Ȯ��
		Mark_Area(VL[0].x,VL[0].y,VL[1].x,VL[1].y, rgb(0,255,100), Width, Height, (LPBYTE)rgb_p);		// ã�� �� Ȯ��
		Mark_Area(HL[0].x,HL[0].y,HL[1].x,HL[1].y, rgb(0,255,100), Width, Height, (LPBYTE)rgb_p);		// ã�� �� Ȯ��
		Mark_Area(HR[0].x,HR[0].y,HR[1].x,HR[1].y, rgb(0,255,100), Width, Height, (LPBYTE)rgb_p);		// ã�� �� Ȯ��

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
			sLog.Format("[��ũ #%d] ���� ��ũ �˻� ������ �߸��Ǿ����ϴ�.",i);
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


		/* X���� ������׷� */
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

		/* X������� ���� ã�� */
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

		/* Y ���� ������׷� */
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
			sLog.Format("[��ũ #%d] �¿� �ν� ��ġ�� ������ �Դϴ�.",i+1);
//			MSG_Display(sLog);
			return false;
		}


		if(iMinY>iMaxY || (iMaxY-iMinY>maxSize) )
		{
			sLog.Format("[��ũ #%d] �¿� �ν� ��ġ�� ������ �Դϴ�.",i+1);
//			MSG_Display(sLog);
			return false;
		}


		if ( iMaxX < 0 || iMaxY < 0 || iMaxX > sizeX || iMaxY > sizeY	||
			iMinX < 0 || iMinY < 0 || iMinX > sizeX || iMinY > sizeY )
		{
			sLog.Format("[��ũ #%d] Max, Min ��ġ�� ã�� ���߽��ϴ�.",i+1);
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
		sLog.Format("���� ��ũ �˻� ������ �߸��Ǿ����ϴ�.");
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


	/* X���� ������׷� */
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

	/* X������� ���� ã�� */
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

	/* Y ���� ������׷� */
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
		sLog.Format("���� ��ũ �˻� ������ �߸��Ǿ����ϴ�.");
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


	/* X���� ������׷� */
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

	/* X������� ���� ã�� */
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

	/* Y ���� ������׷� */
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
//! UV Lamp������ ���� ���ڿ� ó�� ��ü �Լ�
//! [�Է°�]
//!  sRecv : ���ŵ� ���ڿ�, iCnt_Recv : �� ��° ���� �۾����� ȣ��Ǿ������� ��Ÿ����, iIndex_Machine : UV Lamp ��ġ�� ���� index
//! [���� ����]
//!  UV Lamp�� RS-232C Command�� ����ġ ���� ���, 'ERR' ���ڿ��� PC�� ������. 

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
		logStr.Format("[ CCD ] ���� ���Դϴ�.");
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
			theApp.MainDlg->putListLog("	MIU Open ����.");
			return false;
		}
	}
	
	double time3 = myTimer(true);
	if(!MIU.Start())
	{ 
		m_bMiuRun = false;
		logStr.Format("MIU Start ����.");
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
	logStr.Format("MIU Start �Ϸ�.");//logStr.Format("Start �Ϸ�. [%.0f]", time4-time3);
	putListLog(logStr);

	/*for(int i=0; i<100; i++)
	{
		Sleep(1);
		checkMessage();
	}*/


	//float        fSensorFrameRate = 0;
	////MIUGetSensorFrameRate(0, &fSensorFrameRate);	//KYH �ּ� -> Frame���� �ȵ���.
	//fSensorFrameRate = 5;	//�ӽ� Frame


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

	//		sLangChange.LoadStringA(IDS_STRING446);	//CCD ������ ����
	//		logStr.Format(sLangChange);
	//		delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);

	//		sLangChange.LoadStringA(IDS_STRING1424);	//ī�޶� ��� ��� Close.
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
				{// UV Shot -> Grip Open -> Z�� ���->Lens ��� ��ġ->PCB ����-> NG ��û Step���� �̵�
					iRtnFunction = 110000;
				}
				else
				{// Grip Open -> Z�� ���->Lens ��� ��ġ->PCB ����-> NG ��û Step���� �̵�
					iRtnFunction = 115000;
				}
			}
			else
			{
				Task.m_iRetry_Opt = 0;

				if(posLensX<3 && posLensY<3 && posLensZ<3)			// Z�� ���->Lens ��� ��ġ->PCB ����-> NG ��û Step���� �̵�
					iRtnFunction = 120000;
 				else
 					iRtnFunction = 121000;							// Z�� ��� Pass �ϰ� PCB ����-> NG ��û Step���� �̵�
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
		sLangChange.LoadStringA(IDS_STRING1326);	//���� �غ� ���� �ʾҽ��ϴ�.
		errMsg2(Task.AutoFlag, sLangChange);
		return;
	}




	if(Dio.PCBvaccumOnCheck(true, false) == false)
	{
		errMsg2(Task.AutoFlag, "PCB ���� ������ ���� �߽��ϴ�.");
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
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
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
			sLangChange.LoadStringA(IDS_STRING1299);	//��� �������� �Է� �ϼ���. [��ǰ Yes / �ҷ� No]
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

		bThreadTaskPcb =0;			//	 ���� �Ҷ� ��� 0:������ ���� ����
		bThreadTaskLens =0;			//	 ���� �Ҷ� ��� 0:������ ���� ����

		sLangChange.LoadStringA(IDS_STRING1162);	//��ȸ���� ���� �Ͽ����ϴ�.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
		return 0 ;
	}
	else if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
		return -1;
	}
	else if(Task.AutoFlag == 2)
	{
		sLangChange.LoadStringA(IDS_STRING1361);	//�Ͻ� ���� ���� �Դϴ�.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
		return -1;
	}
	else if(Task.AutoFlag == 0)
	{
		if(Task.AutoReday == 0)
		{
			sLangChange.LoadStringA(IDS_STRING1326);	//���� �غ� ���� �ʾҽ��ϴ�.
			errMsg2(Task.AutoFlag, sLangChange);
			return -1;
		}

		sLangChange.LoadStringA(IDS_STRING1161);	//��ȸ���� ���� �Ͻðڽ��ϱ�?
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
			if(g_AlarmFlag)				// �˶� ���� ��..
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
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
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
	//20170615 �α״� ���� �����Ϳ��� ���� �ּ�ó��
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
	//20170615 �α״� ���� �����Ϳ��� ���� �ּ�ó��
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
	//20170615 �α״� ���� �����Ϳ��� ���� �ּ�ó��
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
	//20170615 �α״� ���� �����Ϳ��� ���� �ּ�ó��
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
{//Bright, Contrast ����

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
		sLangChange.LoadStringA(IDS_STRING1203);	//������ ��õ� Ƚ���� �ʱ�ȭ ��Ű�ڽ��ϱ�?
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
		errMsg2(Task.AutoFlag, sLangChange);	//CCD ������ ���� �����Դϴ� 1
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

	case 50100:			/* Z�� ���� ��ġ ���� �� */
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

	case 51200:			/* Z�� ���� ��ġ ���� �� */
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

	case 52000:		/* CODE1 �� �Է� �� sfr ���� */
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

	case 52200:			/* Z�� ���� ��ġ ���� �� */
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

	case 53000:		/* CODE2 �� �Է� �� sfr ���� */
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

	case 54000:		/* CODE2 �� �Է� �� sfr ���� */
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
			sLangChange.LoadStringA(IDS_STRING868);	//OIS AF �� LENS Z Motor offset �̵� �ð� �ʰ�.
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
			sLangChange.LoadStringA(IDS_STRING459);	//CDOE 0 (Offset �̵�), [DFOV %.03f] [Z %.03f]
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

	case 55000:		/* 50 �� CODE �� ��� */
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
			sLangChange.LoadStringA(IDS_STRING703);	//Lens Z Bonding ��ġ �̵� �Ϸ�[%d]
			logStr.Format(sLangChange, iStep);
			putListLog(logStr);
			iRtnFunction = 160100;

		}
		else
		{	
			sLangChange.LoadStringA(IDS_STRING702);	//Lens Z Bonding ��ġ �̵� ���� [%d]
			logStr.Format(sLangChange, iStep);
			putListLog(logStr);
			iRtnFunction = -160000;
		}
		break;

	case  160100:
		{
			if(Dio.LensMotorGrip(false,false))
			{		
				sLangChange.LoadStringA(IDS_STRING671);	//Lens Grip ���� [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
				iRtnFunction = 160200;
			}
			else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
			{
				sLangChange.LoadStringA(IDS_STRING674);	//Lens Grip ���� ���� [%d]
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
				sLangChange.LoadStringA(IDS_STRING675);	//Lens Grip ���� Ȯ�� �Ϸ� [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 160400;
			}
			else
			{
				Dio.LensMotorGrip(false,false);
				sLangChange.LoadStringA(IDS_STRING674);	//Lens Grip ���� Ȯ�� ���� [%d]
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
				sLangChange.LoadStringA(IDS_STRING675);	//Lens Grip ���� Ȯ�� �Ϸ� [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 160400;
			}
			else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
			{
				sLangChange.LoadStringA(IDS_STRING674);	//Lens Grip ���� ���� [%d]
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
				sLangChange.LoadStringA(IDS_STRING705);	//Lens Z ��� ��ġ �̵� �Ϸ� [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 160500;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING704);	//Lens Z ��� ��ġ �̵� ���� [%d]
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
				sLangChange.LoadStringA(IDS_STRING934);	//PCB Defect ��ġ �̵� �Ϸ� [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 160600;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING933);	//PCB Defect ��ġ �̵� ���� [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = -160500;
			}
		}

		break;
	case  160600:
		{
			Sleep(2000);
			sLangChange.LoadStringA(IDS_STRING1352);	//�̹� �˻� �Ϸ� [%d]
			logStr.Format(sLangChange, iStep);
			putListLog(logStr);
			iRtnFunction = 160700;
		}

		break;
	case  160700:
		{
			if(motor.Pcb_Motor_Move(Dark_Pos))
			{
				sLangChange.LoadStringA(IDS_STRING927);	//PCB Dark ��ġ �̵� �Ϸ� [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 160800;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING926);	//PCB Dark ��ġ �̵� ���� [%d]
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
				sLangChange.LoadStringA(IDS_STRING928);	//PCB Dark �ϰ� [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 160900;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING930);	//PCB Dark �ϰ� ���� [%d]
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
				sLangChange.LoadStringA(IDS_STRING919);	//PCB Dark �˻� �Ϸ� [%d]
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
				sLangChange.LoadStringA(IDS_STRING920);	//PCB Dark ��� [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
				iRtnFunction = 161100;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING922);	//PCB Dark ��� ���� [%d]
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
				sLangChange.LoadStringA(IDS_STRING925);	//PCB Dark ��� Ȯ�� �Ϸ� [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				
				iRtnFunction = 161200;
			}
			else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
			{
				sLangChange.LoadStringA(IDS_STRING924);	//PCB Dark ��� Ȯ�� ���� [%d]
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
				sLangChange.LoadStringA(IDS_STRING987);	//PCB ��� ��ġ �̵� �Ϸ� [%d]
				logStr.Format(sLangChange, iStep);
				putListLog(logStr);
				iRtnFunction = 161300;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING986);	//PCB ��� ��ġ �̵� ���� [%d]
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
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
	}
	else if(Task.AutoFlag==0 || Task.AutoFlag==2)
	{	
		logStr.Format("���� �Ϸ� ���� �����ϼ���! \n�Ϸ� = ��(Y) / �̿Ϸ� = �ƴϿ�(N)");	//���� �Ϸ� ���θ� �Է� �ϼ���. \n[�Ϸ� ���� Yes / �̿Ϸ� ���� No]
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
		sLangChange.LoadStringA(IDS_STRING1370);	//�ڵ� ���� �� CCD ������ ���� ȹ�� ����
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
	//	if( dVal[0] != dVal[1])	 return true;	//�ϳ��� �ٸ��� OK
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
		sLangChange.LoadStringA(IDS_STRING1085);	//Socket ���� ����
		sLog.Format(sLangChange);
		putListLog(sLog);
		return false;
	}

	//SetTimer(10, 5000, NULL);

	CString sIPAdd;
	sIPAdd.Format("%s", SOCKET_MAIN);
	
	if( m_Socket.Connect(sIPAdd, 21000) == FALSE )
	{
		sLangChange.LoadStringA(IDS_STRING1276);	//������ ���� ����
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
		sLangChange.LoadStringA(IDS_STRING1337);	//�̴��� Data ���� �̻�
		sTemp.Format(sLangChange + _T(" - %s\n"), sMsg);
		errMsg2(Task.AutoFlag, sTemp);
		return false;
	}

	startIndex	= sMsg.Find("$", 0);
	endIndex	= sMsg.Find("_", startIndex+1);
	sTemp		= sMsg.Mid(startIndex+1, endIndex - startIndex -1);
	
	//sMsg.Format("[���� -> Ŭ���̾�Ʈ] %s", sTemp);
	//putListLog(sMsg);

	if( lstrcmp(sTemp, "GETTILT") == 0 )
	{//GETTILT_���ڵ��Ī
		startIndex	= sMsg.Find("_", 0);
		endIndex	= sMsg.Find("%", startIndex+1);
		sTemp		= sMsg.Mid(startIndex+1, endIndex - startIndex -1);
		
		if( !g_ADOData.func_Get_RecordData(sTemp) )	//���ڵ� ������ DB �˻�
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

		if( g_ADOData.func_Get_RecordData(sTemp) == true )	//���ڵ� ������ DB �˻�
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
		delayMsg(sLangChange, 3000, M_COLOR_RED);	//Ether-Net ���� ���� �����Դϴ�
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
		sLangChange.LoadStringA(IDS_STRING1275);	//������ �޽��� ���� ����.
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
{/* ���� ���� */
	CString sLog;

	if ( m_ListenSocket.Create(21000, SOCK_STREAM) == TRUE )
	{
		if ( m_ListenSocket.Listen() == TRUE )
		{
			sLangChange.LoadStringA(IDS_STRING1273);	//���� ���� �� ���� ��� ��...
			putListLog(sLangChange);
		}
		else
		{		
			sLangChange.LoadStringA(IDS_STRING1274);	//���� ���� ��� ����
			putListLog(sLangChange);
		}
	}
	else
	{
		sLangChange.LoadStringA(IDS_STRING1272);	//���� ���� ����
		putListLog(sLangChange);
	}
}

void CAABonderDlg::CloseServer()
{/* ���� �ݱ� */
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
		sLangChange.LoadStringA(IDS_STRING1379);		//�߸��� Ŭ���̾�Ʈ �����Դϴ�.\n\n(IP �ּҸ� Ȯ���� �ּ���)
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
		sLangChange.LoadStringA(IDS_STRING1251);	//���� �޼��� �̻�.
		putListLog(sLangChange);
		m_csRcvMsg.Unlock();
		return;
	}

	int iCheckFirst = sMsg.Find('&', 0);
	int iCheckMiddle = sMsg.Find('$', 0);
	int iCheckLast = sMsg.Find('%', 0);

	if ( iCheckFirst < 0 || iCheckMiddle < 0 || iCheckLast < 0)
	{
		sLangChange.LoadStringA(IDS_STRING1251);	//���� �޼��� �̻�.
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
		sLangChange.LoadStringA(IDS_STRING1337);	//�̴��� Data ���� �̻�
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
	{//������ X
		startIndex	= sMsg.Find("_", 0);
		endIndex	= sMsg.Find("%", startIndex+1);
		sTemp		= sMsg.Mid(startIndex+1, endIndex - startIndex -1);
		Task.dAlignManual[0] = atof(sTemp);
		Task.iRecvLenCnt[2] = 1;
	}
	else if( lstrcmp(sCompair, "ALIGNY") == 0 )
	{//������ Y
		startIndex	= sMsg.Find("_", 0);
		endIndex	= sMsg.Find("%", startIndex+1);
		sTemp		= sMsg.Mid(startIndex+1, endIndex - startIndex -1);
		Task.dAlignManual[1] = atof(sTemp);
		Task.iRecvLenCnt[3] = 1;
	}
	else if( lstrcmp(sCompair, "ALIGNT") == 0 )
	{//������ T
		startIndex	= sMsg.Find("_", 0);
		endIndex	= sMsg.Find("%", startIndex+1);
		sTemp		= sMsg.Mid(startIndex+1, endIndex - startIndex -1);
		Task.dAlignManual[2] = atof(sTemp);
		Task.iRecvLenCnt[4] = 1;
	}
	else if( lstrcmp(sTemp, "NAK") == 0 )
	{//!! ���� ���� ���
		Task.iRecvLenACK[iIndex] = 0;
	}
	else if( lstrcmp(sTemp, "ACK") == 0 )
	{//!! �� ���� ���� ���
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
	//				//! ���� ȭ���� ��ü ���� ������ ��
	//				//! Ȯ���� �簢 ���� ���� ����

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
	//					//! Ȯ��� ���¿��� Mouse�� ȭ�� �̵� ����
	//					m_PanMoveP = m_ClickP;
	//					m_bActing_Pan_CCD_Zoom = true;
	//				}
	//			}							
	//		}
	//		else
	//		{
	//			//! Ŭ���� ������ ��Ⱚ ǥ��

	//			CPoint p, pShow;
	//			int pos, width;
	//			char szTmp[256];

	//			vision.clearOverlay(m_iCurCamNo);

	//			vision.MilBufferUpdate();

	//			width = MbufInquire(vision.MilGrabImageChild[4], M_PITCH, M_NULL);

	//			//! Ȯ��� ���¿����� Ŭ���� ������ ��Ȯ�ϰ� ���ڼ��� �׸���. 
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
		putListLog("���콺 Ŭ�� - ī�޶� ��ȣ �̻�");
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
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
	}
	else if(Task.AutoFlag==0 || Task.AutoFlag==2)
	{	
		logStr.Format("LENS �ѱ� �Ϸ� ���θ� �����ϼ���. ��ǰ ���� Ȯ�� �ϼ���..\n �ѱ� �Ϸ� ���� = Yes / �ѱ� �̿Ϸ� ���� = No");
		if( askMsg(logStr))
		{
			if(!Dio.LensMotorGripCheck(true, false) )//Lens ������ �ȵǾ��� ���  
			{
				sLangChange.Format(_T("Lens Grip �������°� �ƴմϴ�."));
				delayMsg(sLangChange.GetBuffer(99), 3000, M_COLOR_DARK_GREEN);
			}else
			{
				Task.m_bOKLensPass = 1;
			}
		}else
		{
			if(Dio.LensMotorGripCheck(true, false) )//Lens ������ �Ǿ��� ���  
			{
				sLangChange.Format(_T("Lens Grip �������°� �ƴմϴ�."));
				delayMsg(sLangChange.GetBuffer(99), 3000, M_COLOR_DARK_GREEN);
				
			}else
			{
				if(Dio.LensMotorGripCheck(false, false) )
				{
					Task.m_bOKLensPass = -1;
				}else
				{
					sLangChange.Format(_T("Lens Grip �������°� �ƴմϴ�."));
					delayMsg(sLangChange.GetBuffer(99), 3000, M_COLOR_DARK_GREEN);
				}
			}
			
		}
			
	}
}
// ���� �˻� 
bool CAABonderDlg::_EpoxyFinddispense(int cam)
{
	unsigned char *m_Imagebuf;
	m_Imagebuf = (unsigned char *)malloc(CAM_SIZE_X*CAM_SIZE_Y);
	Sleep(500);
	vision.m_csGrab.Lock();
	Sleep(100);
	//MimFlip(vision.MilGrabImageChild[CAM1], vision.MilGrabImageChild[CAM1], M_HORIZONTAL, M_DEFAULT);
	MbufGet(vision.MilGrabImageChild[CAM1], m_Imagebuf);	// �̹��� ��ü �ȼ� ��Ⱚ ���� ���

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

	bRtn1 = _inspResignRect(false, CAM1, 0, 0, m_Imagebuf);		// left (����)
	bRtn2 = _inspResignRect(false, CAM1, 0, 1, m_Imagebuf);	// right (����)
	bRtn3 = _inspResignRect(false, CAM1, 1, 2, m_Imagebuf);	// top (����)
	bRtn4 = _inspResignRect(false, CAM1, 1, 3, m_Imagebuf);	// bottom (����)
	//bRtn = (bRtn1 && bRtn2 && bRtn3 && bRtn4);

	//bRtn1 = _inspResignHole(true, 0, 0);
	//bRtn = bRtn1;


	//
	free(m_Imagebuf);
	
	return bRtn;
}

bool CAABonderDlg::_inspResign(bool autoMode, int index, int dispMode)				// dispMode 0:ROI + ��� ���, 1:���� White ���� + Text ���, 2:���� White Edge ���
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
		sLangChange.LoadStringA(IDS_STRING950);	//PCB Index ���� ������ �Դϴ�.
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


	vision.crosslist[iCh].addList((int)centX, (int)centY, 30, M_COLOR_GREEN);				// ���� �˻� ����..

	vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_RED);				// ���� �˻� ����..
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
	// ���� ���� ���� �˻� Start..
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
		_stprintf_s(szDispData, sLangChange);	//\n[���� �˻�]\n���� ���� ������ ��� ���̰� �ʹ� �۽��ϴ�.\n(Min %d, Max %d)

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);

		return false;
	}

	if(avgVal>230)	//               ���� ������ ��� ���� ��
	{
		sTempLang.LoadStringA(IDS_STRING135);
		sLangChange.Format(sTempLang, avgVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sTempLang.LoadStringA(IDS_STRING358);
		sLangChange.Format(sTempLang, avgVal);
		_stprintf_s(szDispData, sLangChange);	//\n[���� �˻�]\n���� ���� ������ ������ �ʹ� ����ϴ�.\n(Avg %d)

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);

		return false;
	}

	//if(maxVal<190)	  // ���� ���� �˻��ϴ°� �ƴϰ� ��ο� ���� �˻�
	if(minVal > 150)
	{
		sTempLang.LoadStringA(IDS_STRING136);
		sLangChange.Format(sTempLang, minVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sTempLang.LoadStringA(IDS_STRING356);
		sLangChange.Format(sTempLang, minVal);
		_stprintf_s(szDispData, sLangChange);	//\n[���� �˻�]\n���� ���� Line�� �ʹ� ����ϴ�.\n(Min %d)

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);

		return false;
	}

	


	int whiteVal	= (int)(minVal*1.3 + (avgVal * 0.3));//(maxVal*0.4 + avgVal*0.6);        // ���� �ִ��� ����
	int whiteVal2	= whiteVal * 3;							// ���� �� �������
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




	//! 2013. 06. 10. ���� �˻�.. Edge �ν� �� �̿��Ͽ� �˻� �ϵ��� Test..
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
	// ���� ���� ���� �˻� Start..

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
		sLangChange.LoadStringA(IDS_STRING1317);	//���� �˻� ���� ���� �� �̻�
		putListLog(sLangChange);
		return false;
	}

	if((ey-sy)<10)
	{
		sTempLang.LoadStringA(IDS_STRING139);
		sLangChange.Format(sTempLang, sy, ey);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sLangChange.LoadStringA(IDS_STRING1318);	//���� �˻� ���� ���� �� �̻�
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
		_stprintf_s(szDispData, sLangChange);	//\n[���� �˻�]\n���� ���� ������ ��� ���̰� �ʹ� �۽��ϴ�.\n(Min %d, Max %d)

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
		_stprintf_s(szDispData, sLangChange);	//\n[���� �˻�]\n���� ���� ������ �ʹ� ����ϴ�.\n(Avg %d)

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
		_stprintf_s(szDispData, sLangChange);	//\n[���� �˻�]\n���� ���� Line�� �ʹ� ����ϴ�.\n(Min %d)

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
		_stprintf_s(szDispData, sLangChange);	//\n[���� �˻�]\n���� ���� ���� �������Դϴ�.\n[���� �� %.01f mm, Spec %.01f mm

		if(autoMode)
			errMsg2(false,szDispData);
		else
			putListLog(szDispData);
	}



	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	// ��� ���� ���� �˻� Start..

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

	


	// ���� ������ ��ο� ������ ��� ���� 50 �̻��̰�, ��� ���� 200 ���Ϸ�..
	if( (maxVal-minVal)<70)
	{
		sTempLang.LoadStringA(IDS_STRING148);
		sLangChange.Format(sTempLang, minVal, maxVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));

		sTempLang.LoadStringA(IDS_STRING351);
		sLangChange.Format(sTempLang, minVal, maxVal);
		_stprintf_s(szDispData, sLangChange);	//\n[���� �˻�]\n��� ���� ������ ��� ���̰� �ʹ� �۽��ϴ�.\n(Min %d, Max %d)

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
		_stprintf_s(szDispData, sLangChange);	//\n[���� �˻�]\n��� ���� ������ �ʹ� ����ϴ�.\n(Avg %d)

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
		_stprintf_s(szDispData, sLangChange);	//\n[���� �˻�]\n��� ���� Line�� �ʹ� ����ϴ�.\n(Min %d)
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
	// �Ϻ� ���� ���� �˻� Start..
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


	// ���� ������ ��ο� ������ ��� ���� 50 �̻��̰�, ��� ���� 200 ���Ϸ�..
	if( (maxVal-minVal)<70)
	{
		sTempLang.LoadStringA(IDS_STRING127);
		sLangChange.Format(sTempLang, minVal, maxVal);
		_stprintf_s(szDispData, sLangChange);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, _T("arialuni"));
		
		sTempLang.LoadStringA(IDS_STRING360);
		sLangChange.Format(sTempLang, minVal, maxVal);

		_stprintf_s(szDispData, sLangChange);	//\n[���� �˻�]\n�Ϻ� ���� ������ ��� ���̰� �ʹ� �۽��ϴ�.\n(Min %d, Max %d)

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
		_stprintf_s(szDispData, sLangChange);	//\n[���� �˻�]\n�Ϻ� ���� ������ �ʹ� ����ϴ�.\n(Avg %d)

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
		_stprintf_s(szDispData, sLangChange);	//\n[���� �˻�]\n�Ϻ� ���� Line�� �ʹ� ����ϴ�.\n(Min %d)

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
		_stprintf_s(szDispData, sLangChange);	//\n[���� �˻�]\n���� ���� ���� �������Դϴ�.\n[���� �� %.01f mm, Spec %.01f mm

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
		sLangChange.LoadStringA(IDS_STRING950);	//PCB Index ���� ������ �Դϴ�.
		errMsg2(Task.AutoFlag, sLangChange);
		return false;
	}
	
	Task.m_bResign_Result[index] = false;
	//====================================================�����˻��̹��� �߰�
	double dReduceFactorX = 0.;
	double dReduceFactorY = 0.;
		
	dReduceFactorX = (double) autodispDlg->m_iSizeX_Client / CAM_SIZE_X;
	dReduceFactorY = (double)autodispDlg->m_iSizeY_Client / CAM_SIZE_Y;
	//MimResize(vision.MilGrabImageChild[1], vision.MilDefectImage, dReduceFactorX, dReduceFactorY, M_DEFAULT);
	MimResize(vision.MilGrabImageChild[CAM1], vision.MilOptImage, dReduceFactorX, dReduceFactorY, M_DEFAULT);////���� �̹��� ȭ�鿡 ���÷��� 20180827_2
	//====================================================�����˻��̹��� �߰�
	//vision.drawOverlay(CCD);
	//end
	CPoint inRadius, outRadius;
	float inOffsetX		= (float)((model.m_ResinDrawSize.x*1) - model.m_dResinInspOffset[0].x);	//���� Offset
	float inOffsetY		= (float)((model.m_ResinDrawSize.y*1) - model.m_dResinInspOffset[0].y);
	float outOffsetX	= (float)((model.m_ResinDrawSize.x*1) + model.m_dResinInspOffset[1].x);	//�ٱ��� Offset
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

	vision.crosslist[iCh].addList((int)centX, (int)centY, 30, M_COLOR_GREEN);	// ���� �˻� �߽� ��ġ..
	vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_RED);				// ���� �˻� ����..
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
	// �� ���� �˻� Start..
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
	

	//!! Hole�� ������ �������� ��� 0x00���� Image ��ȯ
	double HoleW=0, HoleH=0;
	double Length=0;
	unsigned char *m_Imagebuf;
	m_Imagebuf = (unsigned char *)malloc(CAM_SIZE_X*CAM_SIZE_Y);

	MbufGet(vision.MilGrabImageChild[iCh], m_Imagebuf);	// �̹��� ��ü �ȼ� ��Ⱚ ���� ���

	//for(int x = 0;x<CAM_SIZE_X;x++)
	//{
	//	for(int y = 0; y<CAM_SIZE_Y; y++)
	//	{
	//		pos = y*CAM_SIZE_X+x;

	//		HoleW = abs(centX - x);
	//		HoleH = abs(centY - y);
	//		Length = sqrt(pow(HoleW, 2) + pow(HoleH, 2));
	//	
	//		if( Length > outRadius.x || Length < inRadius.x )	//-- �� ���� ��츸 ��Ÿ��󽺷� ó����... 
	//		{//!! �ܺ�Hole�� ���� Hole �� �����ϰ� ������ ��Ⱚ�� 0����..
	//			m_Imagebuf[pos] = 0;
	//		}
	//	}
	//}

	
	//!�ܺο��� �������� Buf�� ����.
	CPoint cpCutSize;
	cpCutSize.x = ex-sx;
	cpCutSize.y = ey-sy;

	unsigned char	*cutimgBuf;
	cutimgBuf		= (unsigned char *)malloc(cpCutSize.x * cpCutSize.y);
	memset(cutimgBuf, 0, sizeof(cutimgBuf));

	//! �ܺο������۸� cv�� ����.
	IplImage *bufImg, *dstImg, *srcImg, *polarImg;
	srcImg = cvCreateImage(cvSize(cpCutSize.x, cpCutSize.y),8,1);
	dstImg = cvCreateImage(cvSize(cpCutSize.x, cpCutSize.y),8,1);
	bufImg = cvCreateImage(cvSize(CAM_SIZE_X, CAM_SIZE_Y),8,1);
	memcpy(bufImg->imageData, m_Imagebuf, CAM_SIZE_X * CAM_SIZE_Y);

	cvSetImageROI(bufImg, cvRect(sx, sy, cpCutSize.x, cpCutSize.y));
	cvCopyImage(bufImg, srcImg);
	cvSaveImage("D:/1_cvPolarSrc.bmp", srcImg);	//===============

	//! cv Image Polar ó��
	double M = srcImg->width/log(srcImg->width/2.0);
	cvLogPolar(srcImg, dstImg, cvPoint2D32f(srcImg->width/2, srcImg->height/2), M, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);
	cvSaveImage("D:/2_cvPolardst.bmp", dstImg);	//===============

	//!�� ������ ���ۿ� �ٽ� ��� -> �� ũ���..
	int iEpoxySize = (outRect.right - inRect.right)*0.6;//outOffsetX -inOffsetX;//(inRect.left - outRect.left) * 1;
	unsigned char	*imgInsp; 
	imgInsp		= (unsigned char *)malloc(iEpoxySize * cpCutSize.y);
	sx =  cpCutSize.x-iEpoxySize;//������ �� outRect.right -iEpoxySize;// 

	polarImg = cvCreateImage(cvSize(iEpoxySize, cpCutSize.y),8,1);
	cvSetImageROI(dstImg, cvRect(sx, 0, iEpoxySize, cpCutSize.y));

	cvCopyImage(dstImg, polarImg);
	cvSaveImage("D:/3_cvPolarInsp.bmp", dstImg);	//===============

	//!���ۿ� �ٽ� ���
	memcpy(imgInsp, polarImg->imageData , iEpoxySize*cpCutSize.y);

	//! IplImage �޸� ����
	
	cvReleaseImage( &polarImg );
	cvReleaseImage( &dstImg );
	cvReleaseImage( &srcImg );
	cvResetImageROI(bufImg);
	cvReleaseImage( &bufImg );

	///---------------------------------------------
	// �˻�� unsigned char	*imgInsp��..
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
			{//! 5ȸ �������� ���� ���.
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
{//KKYH 20150622 �Ʒ� ���� �Լ� �߰�
	int iCh = 1;

	if( Task.m_iStatus_Unit_Epoxy == 1)
	{
		saveInspImage(EPOXY_IMAGE_SAVE, index);
	}

	if(index<0 || index>>3)
	{
		sLangChange.LoadStringA(IDS_STRING950);	//PCB Index ���� ������ �Դϴ�.
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
		if(iDirection == 0)	// ���ι���
		{
			CRect inRect;
			inRect.left = (int)(centX + model.m_ResingRectStart[iRectCnt].x/sysData.dCamResol[iCh].x );
			inRect.top = (int)(centY + model.m_ResingRectStart[iRectCnt].y/sysData.dCamResol[iCh].y );
			inRect.right = (int)(centX + (model.m_ResingRectStart[iRectCnt].x+ model.m_ResingRectSize[iRectCnt].x)/sysData.dCamResol[iCh].x );
			inRect.bottom = (int)(centY + (model.m_ResingRectStart[iRectCnt].y+ model.m_ResingRectSize[iRectCnt].y)/sysData.dCamResol[iCh].y );


			//!! Image ��ȯ
			double HoleW=0, HoleH=0;
			double Length=0;
			unsigned char *m_Imagebuf;
			m_Imagebuf = (unsigned char *)malloc(CAM_SIZE_X*CAM_SIZE_Y);

			MbufGet(vision.MilGrabImageChild[iCh], m_Imagebuf);	// �̹��� ��ü �ȼ� ��Ⱚ ���� ���

			//! �˻� ������ �ȼ��� ��� �� ���ϱ�
			int iAllSumCnt = 0, iSumCnt = 0;				//! �˻� �������� ��Ⱚ ���� �ȼ� ��ü�� ī��Ʈ
			int sum = 0;									//! �ȼ� ��Ⱚ�� ��
			int whiteVal = 0;								//! ��� �ȼ� ��� �� ��ȯ

			for(y=inRect.top+1; y<inRect.bottom-1; y++)
			{
				iIsDispense=0;
				for(x=inRect.left+1; x<inRect.right-1; x++)
				{
					pos = y* CAM_SIZE_X + x;				//! ��� ���� ���� �ȼ��� ��ġ ��
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
				vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_RED);				// ���� �˻� ����..
//				errMsg2(Task.AutoFlag, _T("���� �ҷ� �߰�"));
				return false;
			}
			else
			{
				vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_GREEN);				// ���� �˻� ����..
				return true;
			}
		}
		else	// ���ι���
		{  
			CRect inRect;
			inRect.left = (int)(centX + model.m_ResingRectStart[iRectCnt].x/sysData.dCamResol[iCh].x );
			inRect.top = (int)(centY + model.m_ResingRectStart[iRectCnt].y/sysData.dCamResol[iCh].y );
			inRect.right = (int)(centX + (model.m_ResingRectStart[iRectCnt].x+ model.m_ResingRectSize[iRectCnt].x)/sysData.dCamResol[iCh].x );
			inRect.bottom = (int)(centY + (model.m_ResingRectStart[iRectCnt].y+ model.m_ResingRectSize[iRectCnt].y)/sysData.dCamResol[iCh].y );

			//!! Image ��ȯ
			double HoleW=0, HoleH=0;
			double Length=0;
			unsigned char *m_Imagebuf;
			m_Imagebuf = (unsigned char *)malloc(CAM_SIZE_X*CAM_SIZE_Y);

			MbufGet(vision.MilGrabImageChild[iCh], m_Imagebuf);	// �̹��� ��ü �ȼ� ��Ⱚ ���� ���

			//! �˻� ������ �ȼ��� ��� �� ���ϱ�
			int iAllSumCnt = 0, iSumCnt = 0;				//! �˻� �������� ��Ⱚ ���� �ȼ� ��ü�� ī��Ʈ
			int sum = 0;									//! �ȼ� ��Ⱚ�� ��
			int whiteVal = 0;								//! ��� �ȼ� ��� �� ��ȯ

			for(x=inRect.left+1; x<inRect.right-1; x++)
			{
				iIsDispense=0;
				for(y=inRect.top+1; y<inRect.bottom-1; y++)
				{
					pos = y* CAM_SIZE_X + x;				//! ��� ���� ���� �ȼ��� ��ġ ��
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
					}					//! ��� �� ���ۿ� ����
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
				vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_RED);				// ���� �˻� ����..
//				errMsg2(Task.AutoFlag, _T("���� �ҷ� �߰�"));
				return false;
			}
			else
			{
				vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_GREEN);				// ���� �˻� ����..
				return true;
			}
		}



	}

	return true;
}


bool CAABonderDlg::_inspResignHole(bool autoMode, int index, int dispMode, int iCirCnt, unsigned char *m_Imagebuf)
{//KKYH 20150622 �Ʒ� ���� �Լ� ����
	int iCh = CAM2;

	if(index<0 || index>>3)
	{
		sLangChange.LoadStringA(IDS_STRING950);	//PCB Index ���� ������ �Դϴ�.
		errMsg2(Task.AutoFlag, sLangChange);
		return false;
	}

	Task.m_bResign_Result[index] = false;
	
	CPoint inRadius, outRadius;
	float inOffsetX		= (float)((model.m_ResinDrawSize.x*0.5) - model.m_dResinInspOffset[0].x);	//���� Offset
	float inOffsetY		= (float)((model.m_ResinDrawSize.y*0.5) - model.m_dResinInspOffset[0].y);
	float outOffsetX	= (float)((model.m_ResinDrawSize.x*0.5) + model.m_dResinInspOffset[1].x);	//�ٱ��� Offset
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

	vision.crosslist[iCh].addList((int)centX, (int)centY, 30, M_COLOR_GREEN);	// ���� �˻� �߽� ��ġ..
	vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_RED);				// ���� �˻� ����..
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
	// �� ���� �˻� Start..
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
		putListLog("[���� �˻�] �˻� ���� ���� �� �̻�");
		return false;
	}
	if((outOffsetY-inOffsetY) / sysData.dCamResol[iCh].y < 10)
	{
		sprintf_s(szDispData, "[ ERROR ] Insp ROI size Y Error. (%f~%f)", outOffsetY, inOffsetY);
		vision.textlist[iCh].addList(50, 690, szDispData, M_COLOR_RED, 17, 8, "Arial");

		putListLog("[���� �˻�] �˻� ���� ���� �� �̻�");
		return false;
	}
	

	//!! Hole�� ������ �������� ��� 0x00���� Image ��ȯ
	double HoleW=0, HoleH=0;
	double Length=0;

	//!�ܺο��� �������� Buf�� ����.
	CPoint cpCutSize;
	cpCutSize.x = ex-sx;
	cpCutSize.y = ey-sy;

	unsigned char	*cutimgBuf;
	cutimgBuf		= (unsigned char *)malloc(cpCutSize.x * cpCutSize.y);
	memset(cutimgBuf, 0, sizeof(cutimgBuf));

	//! �ܺο������۸� cv�� ����.
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
	
	
	//! cv Image Polar ó��
	double M = srcImg->width/log(srcImg->width/2.0);
	//cvLogPolar(srcImg, dstImg, cvPoint2D32f(srcImg->width/2, srcImg->height/2), M, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);
	cvLogPolar(srcImg, dstImg, cvPoint2D32f(srcImg->width / 2, srcImg->height / 2), M, CV_WARP_INVERSE_MAP + CV_WARP_FILL_OUTLIERS);
	fileCirName.Format(_T("D:/2_cvPolardst_%d.bmp"), iCirCnt);
	cvSaveImage(fileCirName, dstImg);	//===============

	//!�� ������ ���ۿ� �ٽ� ��� -> �� ũ���..
	int iEpoxySize = (inRect.left - outRect.left) * 0.32;//�� ũ�� ���� 0.32
	int iBuffSize=0;
	unsigned char	*imgInsp;
	int iSizeY=0;
	int my = cpCutSize.y/4;
	if(iCirCnt < 1)
	{
		iSizeY = cpCutSize.y/4; //iSizeY=(iCirCnt < 2)?(cpCutSize.y/4):((cpCutSize.y/4)-10);  ���� �ִ� if��
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

	//!���ۿ� �ٽ� ���
	memcpy(imgInsp, polarImg->imageData , iBuffSize);
	//! IplImage �޸� ����

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
		if(sum / inspSize.x < 255.0 * (model.m_dResinInspHoleSpec/100.0) )//���庸�� ������
		{
			iContinuityLine++;

			if(iContinuityLine*sysData.dCamResol[iCh].x > model.m_dResinInspGapLength)//! 5ȸ �������� ���� ���.
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
{//KKYH 20150622 �Ʒ� ���� �Լ� �߰�
	int iCh = index;

	if( Task.m_iStatus_Unit_Epoxy == 1)
	{
		saveInspImage(EPOXY_IMAGE_SAVE, index);
	}

	if(index<0 || index>>3)
	{
		sLangChange.LoadStringA(IDS_STRING950);	//PCB Index ���� ������ �Դϴ�.
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
			   
		iDispenseSpec = (model.m_iResinInspRectSpec *  Ewidth) / 100;//���� ��� �����

		//! �˻� ������ �ȼ��� ��� �� ���ϱ�
		int iAllSumCnt = 0, iSumCnt = 0;				//! �˻� �������� ��Ⱚ ���� �ȼ� ��ü�� ī��Ʈ
		int sum = 0;									//! �ȼ� ��Ⱚ�� ��
		int whiteVal = 0;								//! ��� �ȼ� ��� �� ��ȯ

		CPoint point;
		if(iDirection == 0)	// ���ι���
		{
			for(y=inRect.top+1; y<inRect.bottom-1; y++)
			{
				iIsDispense=0;
				iDispenseCnt=0;
				for(x=inRect.left+1; x<inRect.right-1; x++)
				{
					pos = y* CAM_SIZE_X + x;				//! ��� ���� ���� �ȼ��� ��ġ ��
					if(m_Imagebuf[pos]>model.m_ResingInspLevel)
					{
						iDispenseCnt++;
					}					
					else
					{
						point.x=x;
						point.y=y;
						vision.pixellist[iCh].addList(point, M_COLOR_MAGENTA);
					}					//! ��� �� ���ۿ� ����
				}
				if(iDispenseCnt < iDispenseSpec)//���η� �̵��������� ����ۼ�Ʈ���� �� ��ĥ��
				{
					pass = false;
					
				}

			}
		}else// ���ι���
		{
			inRect.left = (int)(centX + model.m_ResingRectStart[iRectCnt].x);
			inRect.top = (int)(centY + model.m_ResingRectStart[iRectCnt].y);
			inRect.right = (int)(centX + (model.m_ResingRectStart[iRectCnt].x+ model.m_ResingRectSize[iRectCnt].x));
			inRect.bottom = (int)(centY + (model.m_ResingRectStart[iRectCnt].y+ model.m_ResingRectSize[iRectCnt].y));
			Ewidth = inRect.right - inRect.left;
			Eheight = inRect.bottom - inRect.top;
			   
		iDispenseSpec = (model.m_iResinInspRectSpec *  Eheight) / 100;//���� ��� �����
			for(x=inRect.left+1; x<inRect.right-1; x++)
			{
				iIsDispense=0;
				iDispenseCnt=0;
				for(y=inRect.top+1; y<inRect.bottom-1; y++)
				{
					pos = y* CAM_SIZE_X + x;				//! ��� ���� ���� �ȼ��� ��ġ ��
					if(m_Imagebuf[pos]>model.m_ResingInspLevel)
					{
						iDispenseCnt++;
					}					
					else
					{
						point.x=x;
						point.y=y;
						vision.pixellist[iCh].addList(point, M_COLOR_MAGENTA);
					}					//! ��� �� ���ۿ� ����
				}
				if(iDispenseCnt < iDispenseSpec)//���η� �̵��������� ����ۼ�Ʈ���� �� ��ĥ��
				{
					pass = false;
				}

			}
		}
		if(pass)
		{
			vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_GREEN);				// ���� �˻� ����..
			return true;
		}else
		{
			vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_RED);				// ���� �˻� ����..
			return false;
		}
	
	return true;
}

bool CAABonderDlg::_inspResignHole(bool autoMode, int index, int iRectCnt,unsigned char *m_Imagebuf)
{//KKYH 20150622 �Ʒ� ���� �Լ� ����
	int iCh = CAM2;

	if( Task.m_iStatus_Unit_Epoxy == 1)
	{
		saveInspImage(EPOXY_IMAGE_SAVE, index);
	}

	if(index<0 || index>3)
	{
		sLangChange.LoadStringA(IDS_STRING950);	//PCB Index ���� ������ �Դϴ�.
		errMsg2(Task.AutoFlag, sLangChange);
		return false;
	}


	Task.m_bResign_Result[index] = false;

	CPoint inRadius, outRadius;
	float inOffsetX		= (float)((model.m_ResinDrawSize.x*0.5) - model.m_dResinInspOffset[0].x);	//���� Offset
	float inOffsetY		= (float)((model.m_ResinDrawSize.y*0.5) - model.m_dResinInspOffset[0].y);
	float outOffsetX	= (float)((model.m_ResinDrawSize.x*0.5) + model.m_dResinInspOffset[1].x);	//�ٱ��� Offset
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

	vision.crosslist[iCh].addList((int)centX, (int)centY, 30, M_COLOR_GREEN);	// ���� �˻� �߽� ��ġ..
	vision.boxlist[iCh].addList(inRect, PS_SOLID, M_COLOR_RED);				// ���� �˻� ����..
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
	// �� ���� �˻� Start..
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

			pos = y* CAM_SIZE_X + x;				//! ��� ���� ���� �ȼ��� ��ġ ��
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
			pos = y* CAM_SIZE_X + x;				//! ��� ���� ���� �ȼ��� ��ġ ��
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
	//! Data �ʱ�ȭ
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
		sLangChange.LoadStringA(IDS_STRING1269);	//������ �����Ͻðڽ��ϱ�?
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
		if(askMsg(sLangChange))	//"������ ���� �Ͻðڽ��ϱ�?"
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
	// 20141119 LHC - �ڵ����� ���� ������ ��ü �ð� üũ�ϱ�
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
			sLangChange.LoadStringA(IDS_STRING540);	//Epoxy ��ü���� %d���� �������ϴ�\n ��ü �Ϸ� �� YES ��ư�� �����ּ���.\n [YES : ��ü �Ϸ� NO : ��ü �̿Ϸ�]
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
	sLangChange.LoadStringA(IDS_STRING757);	//Lot���� �Է��ϼ���.
	keyDlg.m_strKeyPad = sLangChange;

	if (keyDlg.DoModal()==IDOK)
	{
		strTemp = keyDlg.GetstrKeypad();
		int strLength = strTemp.GetLength();
		if(strLength<1)
		{
			sLangChange.LoadStringA(IDS_STRING758);	//Lot���� �������Դϴ�.
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

//141201 �ڼ��� [SocketCommunication] Added
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
		sLangChange.LoadStringA(IDS_STRING771);	//"MES�� ���� ����"
		sLog.Format(sLangChange);
		putListLog(sLog);
	}
	else
	{
		sLangChange.LoadStringA(IDS_STRING770);	//"MES�� ���� ����"
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
		sLangChange.LoadStringA(IDS_STRING1085);	//Socket ���� ����
		sLog.Format(sLangChange);
		putListLog(sLog);
		return;
	}
	if(sysData.m_iProductComp == 1)
	{
		if(m_SocketMes.Connect(sysData.m_Mes_Ip_Number, sysData.m_Mes_Comp_Port_Number) == FALSE)
		{
			sLog.Format("MES ���� ����. MES����,IP, PORT Ȯ�� �� �ּ���.");
			putListLog(sLog);
			m_labelMes.SetBkColor(M_COLOR_RED);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING770);	//"MES�� ���� ����"
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
			//sLangChange.LoadStringA(IDS_STRING772);	//MES�� ���� ����. MES���� �� IP, PORT ��ȣ�� Ȯ�� �� �ּ���.
			sLog.Format("MES ���� ����. MES����,IP, PORT Ȯ�� �� �ּ���.");
			putListLog(sLog);
			m_labelMes.SetBkColor(M_COLOR_RED);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING770);	//"MES�� ���� ����"
			sLog.Format(sLangChange);
			putListLog(sLog);
			m_labelMes.SetBkColor(M_COLOR_GREEN);
			
		}
		m_labelMes.Invalidate();
	}

}


int CAABonderDlg::RunProc_ProductLoading(int iUseStep)
{//! �۾��� ��ǰ ���� �ڵ� Step		(10000 ~ 10999)
	int iRtnFunction = iUseStep;
	CString logStr = "", sBarCode="";
	bool bChk = true;

	switch (iUseStep)
	{
	case 10000://! DoorĿư Lift Open.
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
		//LightControl.ctrlLedVolume(LIGHT_OC_6500K, 0);		// Align�� ���� ON
		Dio.LaserCylinderUPDOWN(true, false);

		logStr.Format("LASER �Ǹ��� ��� �Ϸ�[%d]", iRtnFunction);
		theApp.MainDlg->putListLog(logStr);
		Task.PCBTaskTime = myTimer(true);

		iRtnFunction = 10060;
		break;

	case 10060:
		if (Dio.LaserCylinderCheck(true, false))
		{
			logStr.Format("LASER �Ǹ��� ��� Ȯ�� �Ϸ�[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 10100;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER �Ǹ��� ��� �ð� �ʰ�..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -10060;
		}
		break;

	case 10100://! Chip ���� Draw
		func_ChipID_Draw();

		iRtnFunction = 10110;

		logStr.Format("		loading Step [%d]", iRtnFunction);
		putListLog(logStr);
		logStr.Format("		Start ��ư�� �����ּ���[%d]", iRtnFunction);
		putListLog(logStr);
		m_btnStart.m_iStateBtn = 4;
		m_btnStart.Invalidate();
		break;
	case 10110://! Start P/B Button Push ���. 
		if (Dio.StartPBOnCheck(true, false))// || Start_Btn_On)	//���α׷��� start ��ư �߰�
		{
			iRtnFunction = 10165;
			MbufClear(vision.MilOptImage, 0);
			m_btnStart.m_iStateBtn = 0;
			m_btnStart.Invalidate();
			Start_Btn_On = false;
			if (Task.m_bOKLensPass == 1)   //Lens �ѱ� �Ϸ����
			{
				if (!askMsg("���� �ѱ�����Դϴ�. \n��� �����Ͻðڽ��ϱ�?") == IDOK)
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
	case 10165://! Start P/B Button Push ���. 
		//Dio.StartPBLampOn(false);
		sBarCode.Format("%s", Task.ChipID);
		if (sysData.m_iProductComp == 1)
		{
			logStr.Format("====%s ����ǰ �˻� start====", Task.ChipID);
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
	case 10170://! Start P/B Button Push ���.
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
		Task.m_timeChecker.Measure_Time(1);	//Start ��ư Ŭ�� �ð�
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
			logStr.Format("PCB Z Wait ��ġ �̵� �Ϸ� [%d]", iRtnFunction);
			putListLog(logStr);
			iRtnFunction = 10200;
			logStr.Format("		loading Step [%d]", iRtnFunction);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("PCB Z Wait ��ġ �̵� �Ϸ� ���� [%d]", iRtnFunction);
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
			sLangChange.LoadStringA(IDS_STRING745);	//LENS�� Tx, Ty�� ��� ��ġ �̵� ����
			logStr.Format(sLangChange);
			putListLog(logStr);
			iRtnFunction = -10200;
		}
		break;

	case 10202:
		if (Dio.LaserCylinderCheck(true, false) == false)
		{
			logStr.Format("LASER �Ǹ��� ��� Ȯ�� ����[%d]", iRtnFunction);
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
		if (Task.m_bOKLensPass == 1)   //Lens �ѱ� �Ϸ����
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
	case 10400:	//! ���ڵ� ���� Check

		////////////////////////////////
		MandoInspLog.func_InitData();	//-- Log �ʱ�ȭ
		sBarCode.Format("%s", Task.ChipID);

		if (sBarCode == "EMPTY")//���ڵ������� ���� ��� Ȯ�� Message
		{
			logStr.Format("���ڵ� ������ �����ϴ�. \n [ %s ]������ ���� �Ͻðڽ��ϱ�?", Task.ChipID);
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
					logStr.Format("��ġ�ϴ� ���̵� �����ϴ�.[%d]", iUseStep);
				}else
				{
					logStr.Format("DataBase ���� ����.[%d]\n MS Office�� �ݾ��ּ���.", iUseStep);
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
		//�׸�Ȯ��
		if (Dio.PcbGripCheck(true, false) == true)
		{
			logStr.Format(_T("PCB GRIP Ȯ�� �Ϸ� [%d]"), iRtnFunction);
			iRtnFunction = 10600;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format(_T("PCB GRIP ���� [%d]"), iRtnFunction);
			iRtnFunction = -10550;
			errMsg2(Task.AutoFlag, logStr);
		}
		break;
	case 10600:
		//��
		Sleep(1000);
		if (Dio.PcbTurn(false, false) == true)
		{
			logStr.Format(_T("PCB RETURN �Ϸ� [%d]"), iRtnFunction);
			iRtnFunction = 10650;
		}
		else
		{
			logStr.Format(_T("PCB RETURN ���� [%d]"), iRtnFunction);
			iRtnFunction = -10600;
			errMsg2(Task.AutoFlag, logStr);
		}
		putListLog(logStr);
		Task.PCBTaskTime = myTimer(true);
		
		break;
	case 10650:
		//��Ȯ��
		if (Dio.PcbTurnCheck(false, false) == true)
		{
			logStr.Format(_T("PCB RETURN Ȯ�� �Ϸ� [%d]"), iRtnFunction);
			iRtnFunction = 10700;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format(_T("PCB RETURN Ȯ�� ���� [%d]"), iRtnFunction);
			iRtnFunction = -10650;
			errMsg2(Task.AutoFlag, logStr);
		}
		
		break;
	case 10700:
		//PCB �ε� �� Ȯ�� �����ּ���
		if (askMsg("PCB �ε� �� Ȯ�� �����ּ���") == IDOK)
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
		
		MandoInspLog.sBarcodeID.Format("%s", Task.ChipID);	//���� ������Camera �˻� Log ����
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
		if (Task.m_bOKLensPass == 1)   //Lens �ѱ� �Ϸ����
		{
			logStr.Format("		Lens �׸� ���� [%d]", iRtnFunction);
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
		sLangChange.LoadStringA(IDS_STRING1007);	//PCB ��ǰ ���� Thread Step ��ȣ ������ . 
		logStr.Format("PCB ��ǰ ���� Thread Step ��ȣ ������ [%d]", iUseStep);
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
			sLangChange.LoadStringA(IDS_STRING1012);	//PCB_Z�� �����ġ �̵� ����.
			logStr.Format(sLangChange, iUseStep);
			putListLog(logStr);
			iRtnFunction = -17000;
			break;
		}

		iRtnFunction = 17020;
		break;
	case 17020: //Dispenser -Z�� �ϰ�
		if (motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			iRtnFunction = 17030;
		}
		else
		{
			logStr.Format("PCB Z Wait ��ġ �̵� �Ϸ� ���� [%d]", iUseStep);
			putListLog(logStr);
			errMsg2(Task.AutoFlag, logStr);

			iRtnFunction = -17020;
		}
		break;
	case 17030:  //���� ��ġ �̵�
		LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align ���� OFF
		if (motor.Pcb_Motor_Move(PDispense_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]))
		{
			sLangChange.LoadStringA(IDS_STRING935);	//PCB Dispense ���� ��ġ �̵� �Ϸ�-������ X:%.03lf,Y:%.03lf,T:%.03lf
			logStr.Format(sLangChange, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]);
			putListLog(logStr);
			iRtnFunction = 17040;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING938);	//PCB Dispense ��ġ �̵� ����[%d]
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -17030;
		}
		break;
	case 17040: //Dispenser-z �� ���
		if (motor.PCB_Z_Motor_Move(PDispense_Pos))
		{
			iRtnFunction = 17050;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING977);	//PCB Z Dispense ��ġ �̵� ����
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -17040;
		}
		break;

	case 17050: // ���潺 ����
		if (motor.func_Epoxy_Point_Draw())//4����Ʈ ����
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
			sLangChange.LoadStringA(IDS_STRING937);	//PCB Dispense �Ϸ�[%d]
			logStr.Format(sLangChange, iUseStep);
			putListLog(logStr);
			iRtnFunction = 17070;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING936);	//PCB Dispense ����[%d]
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -17050;
		}
		break;
	case 17070:
		if (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y))
		{
			LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_SENSOR]);//���� �˻��� ����θ�ũ�� ����Ʈ
			iRtnFunction = 17100;
		}
		break;
	case 17100:
		iRtnFunction = 17200;
		break;
	case 17200:
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB�� z�� ���� ��ġ �̵� ����[%d]
			logStr.Format(sLangChange, iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -17200;
			break;
		}
		else
		{
			logStr.Format("PCB Z ��� ��ġ �̵� �Ϸ�[%d]", iUseStep);
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
		logStr.Format("PCB Smini Thread Step ��ȣ ������ . [%d]", iUseStep);
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
			logStr.Format("Lens_Z�� ����ġ �̵� ����");
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -16000;
			break;
		}

		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB Z�� �����ġ �̵� ����[%d]", iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -16000;
			break;
		}

		if (!motor.Pcb_Motor_Move(Pcb_Loading_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], 0))
		{
			logStr.Format("PCB %s �̵� ����", MotorPosName[Pcb_Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -16000;
			break;
		}
		iRtnFunction = 16100;

		break;
	case 16100:
		if (!Dio.LensMotorGrip(false, false))
		{
			logStr.Format("Lens Unit Gripper ���� ����[%d]", iUseStep);
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

	case 16200: // ����
		if (!motor.Lens_Tilt_Motor_Move(Pcb_Loading_Pos))
		{
			logStr.Format("Lens %s Tx,Ty�̵� ����", MotorPosName[Pcb_Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -16200;
			break;
		}
		if (!motor.Lens_Motor_MoveXY(Pcb_Loading_Pos))
		{
			logStr.Format("Lens %s X,Y �̵� ����", MotorPosName[Pcb_Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -16200;
			break;
		}
		logStr.Format("Lens ���� ��ġ �̵� �Ϸ�[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 16250;
		break;

	case 16250:

		//------//
		if (!motor.PCB_Z_Motor_Move(Pcb_Loading_Pos))
		{
			logStr.Format("PCB loading Z �̵� ����");
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
			logStr.Format("Lens Z ��ġ �̵� ���� [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -16252;
		}
		//iRtnFunction = 16255;
		break;

	case 16255:
		iRtnFunction = 16300;
		break;
	case 16300: // ���� �׸�
		if (Dio.LensMotorGrip(true, false))
		{
			Task.m_bOKLensPass = 1;
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 16350;
		}
		else
		{
			Task.m_bOKLensPass = -1;
			logStr.Format("Lens �׸� ���� [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -16300;
		}
		//iRtnFunction = 16350;
		break;

	case 16350://���� �׸� Ȯ��
		if (Dio.LensMotorGripCheck(true, false))
		{
			logStr.Format("Lens �׸� �Ϸ�[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 16360;

			Sleep(800);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("Lens Gripper ���� ���� �ð� �ʰ�..[%d]", iUseStep);
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
			logStr.Format("Lens Z ��� ��ġ �̵� ���� [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -16400;
			break;
		}
		//
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB waiting Z �̵� ����");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -16400;
			break;
		}
		iRtnFunction = 16450;
		
		break;

	case 16450:
		if (!motor.Lens_Motor_MoveXY(Wait_Pos))
		{
			logStr.Format("Lens %s X,Y �̵� ����", MotorPosName[Wait_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -16450;
			break;
		}
		iRtnFunction = 16452;
		break;
	case 16452:
		
		if (motor.Pcb_Motor_Move(Wait_Pos))			//PCB�� ���� �����ġ �̵�
		{
			iRtnFunction = 16455;
			logStr.Format("Pcb x,y ��� ��ġ �̵� �Ϸ� [%d]", iUseStep);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("Pcb x,y ��� ��ġ �̵� ���� [%d]", iUseStep);
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
		logStr.Format("PCB Smini Thread Step ��ȣ ������ . [%d]", iUseStep);
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
			logStr.Format("Lens_Z�� ����ġ �̵� ����");
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -18000;
			break;
		}

		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB Z�� �����ġ �̵� ����[%d]", iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -18000;
			break;
		}

		if (!motor.Pcb_Motor_Move(Pcb_Loading_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], 0))
		{
			logStr.Format("PCB %s �̵� ����", MotorPosName[Pcb_Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -18000;
			break;
		}
		iRtnFunction = 18100;
		break;
	case 18100:

		iRtnFunction = 18200;
		break;

	case 18200: // ����
		if (!motor.Lens_Tilt_Motor_Move(Pcb_Loading_Pos))
		{
			logStr.Format("Lens %s Tx,Ty�̵� ����", MotorPosName[Pcb_Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -18200;
			break;
		}
		if (!motor.Lens_Motor_MoveXY(Pcb_Loading_Pos))
		{
			logStr.Format("Lens %s X,Y �̵� ����", MotorPosName[Pcb_Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -18200;
			break;
		}
		logStr.Format("Lens ���� ��ġ �̵� �Ϸ�[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 18250;


		break;

	case 18250:

		//----//
		if (!motor.PCB_Z_Motor_Move(Pcb_Loading_Pos))
		{
			logStr.Format("PCB loading Z �̵� ����");
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
			logStr.Format("Lens Z ��ġ �̵� ���� [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -18252;
		}
		//iRtnFunction = 18255;
		break;

	case 18255:
		iRtnFunction = 18300;
		break;
	case 18300: // ���� ��׸�
		if (Dio.LensMotorGrip(false, false))
		{
			Task.m_bOKLensPass = 1;
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 18350;
		}
		else
		{
			Task.m_bOKLensPass = -1;
			logStr.Format("Lens UnGrip ���� [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -18300;
		}
		//iRtnFunction = 18350;
		break;

	case 18350://���� ��׸� Ȯ��
		if (Dio.LensMotorGripCheck(false, false))
		{
			logStr.Format("Lens UnGrip �Ϸ�[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 18400;

			Sleep(800);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("Lens Gripper ���� ���� �ð� �ʰ�..[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -18350;
		}
		//----//

		//iRtnFunction = 18400;
		break;
	case 18400:
		if (!motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("Lens Z ��� ��ġ �̵� ���� [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -18400;
		}
		//
		if (!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB waiting Z �̵� ����");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -18400;
		}
		iRtnFunction = 18450;
		break;
	case 18450:
		if (!motor.Lens_Motor_MoveXY(Wait_Pos))		//<------ ���������� ����
		{
			logStr.Format("Lens %s X,Y �̵� ����", MotorPosName[Wait_Pos]);
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
		logStr.Format("PCB Smini Thread Step ��ȣ ������ . [%d]", iUseStep);
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
			logStr.Format("PCB Z�� �����ġ �̵� ����[%d]", iUseStep);
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
		if (motor.Pcb_Motor_Move(Holder_Uv_Pos))			//PCB�� ���� �����ġ �̵�
		{
			iRtnFunction = 19420;
			logStr.Format("Pcb x,y Holder_Uv_Pos �̵� �Ϸ� [%d]", iUseStep);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("Pcb x,y Holder_Uv_Pos �̵� ���� [%d]", iUseStep);
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
			logStr.Format("PCB Holder_Uv_Pos Z �̵� ����");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19430;
			break;
		}
		iRtnFunction = 19440;
		break;
	case 19440:
		if (Dio.UVCylinder_UPDOWN(false, false))
		{
			logStr.Format("UV �Ǹ��� �ϰ� �Ϸ�[%d]", iUseStep);
			putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 19450;
		}
		else
		{
			logStr.Format("UV �Ǹ��� �ϰ� ���� [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19440;
		}
		break;
	case 19450:
		if (Dio.UVCylinderCheck(false, false))
		{
			logStr.Format("UV �Ǹ��� �ϰ� Ȯ�� �Ϸ�[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 19600;

			Sleep(800);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("UV �Ǹ��� �ϰ� �ð� �ʰ�..[%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19450;
		}
		break;
	case 19600:
		//uv ����
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
			logStr.Format("UV �Ǹ��� ��� �Ϸ�[%d]", iUseStep);
			putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 19670;
		}
		else
		{
			logStr.Format("UV �Ǹ��� ��� ���� [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19660;
		}
		break;
	case 19670:
		if (Dio.UVCylinderCheck(true, false))
		{
			logStr.Format("UV �Ǹ��� ��� Ȯ�� �Ϸ�[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 19680;

			Sleep(800);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("UV �Ǹ��� ��� �ð� �ʰ�..[%d]", iUseStep);
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
			logStr.Format("PCB waiting Z �̵� ����");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19690;
		}
		iRtnFunction = 19700;
		break;
	case 19700:

		iRtnFunction = 19750;
		break;
	case 19750:
		if (askMsg("�������� ��  Ȯ�� �����ּ���") == IDOK)
		{
			iRtnFunction = 19760;
			Sleep(5000);
		}
		break;
	case 19760:
		if (!motor.Lens_Motor_MoveXY(Wait_Pos))
		{
			logStr.Format("Lens %s X,Y �̵� ����", MotorPosName[Wait_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -19760;
			break;
		}
		logStr.Format("Lens ���� ��ġ �̵� �Ϸ�[%d]", iUseStep);
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
			logStr.Format(_T("PCB TURN ���� [%d]"), iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19800;
		}
		break;
	case 19850:
		if (Dio.PcbTurnCheck(true, false) == true)
		{
			logStr.Format(_T("PCB TURN Ȯ�� �Ϸ� [%d]"), iRtnFunction);
			iRtnFunction = 19900;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format(_T("PCB TURN ���� [%d]"), iRtnFunction);
			iRtnFunction = -19850;
			errMsg2(Task.AutoFlag, logStr);
		}
		break;
	case 19900:
		if (Dio.ContactUpDown(true, false) == true)
		{
			logStr.Format(_T("Contact UP ���� [%d]"), iRtnFunction);
			iRtnFunction = 19910;
			Task.PCBTaskTime = myTimer(true);
		}
		else
		{
			logStr.Format(_T("Contact UP ���� [%d]"), iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -19900;
		}
		break;
	case 19910:
		if (Dio.ContactUpDownCheck(true, false) == true)
		{
			logStr.Format(_T("Contact UP Ȯ�� �Ϸ� [%d]"), iRtnFunction);
			iRtnFunction = 19930;
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format(_T("Contact UP Ȯ�� ���� [%d]"), iRtnFunction);
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
		logStr.Format("PCB Smini Thread Step ��ȣ ������ . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	return iRtnFunction;
}
int CAABonderDlg::RunProc_PCBOutsideAlign(int iUseStep)
{
	//! PCB �ܺ� Align Step(11000 ~ 14999)					//!! MIU_Initialize -> PCB Holder Align ��ġ
	int iRtnFunction = iUseStep;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;		//PCB ���� ȹ�� Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;

	//ctrlSubDlg(MAIN_DLG);		//20161212  ccd start ����
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
	case 12000://! ���ڵ� DB Data ����
		/*if( !g_ADOData.func_AATaskToRecordID(modelList.curModelName, Task.ChipID) )
		{
			logStr.Format("DataBase ���� ����.[%d]\n MS Office�� �ݾ��ּ���.", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -12000;
			break;
		}*/
		iRtnFunction = 13000;	
		break;

	case 13000:
		if (Task.m_bOKLensPass == 1)   //Lens �ѱ� �Ϸ����
		{
			iRtnFunction = 14100;
			break;
		}

		Task.m_iRetry_Opt = 0;
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_HOLDER]);		// Align�� ���� ON
		Task.d_Align_offset_x[PCB_Holder_MARK] = 0;
		Task.d_Align_offset_y[PCB_Holder_MARK] = 0;
		Task.d_Align_offset_th[PCB_Holder_MARK] = 0;
		if(motor.Pcb_Motor_Move(Holder_Pos))
		{
			Task.PCBTaskTime = myTimer(true);
			logStr.Format("Pcb Holder ��ġ �̵� �Ϸ�[%d]", iUseStep);
  			putListLog(logStr);
  			iRtnFunction = 13300;
		}
		else
		{
			logStr.Format("Pcb Holder ��ġ �̵� ����[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -13000;
		}
		break;

	case 13300:
		if(motor.PCB_Z_Motor_Move(Holder_Pos))
		{
			Task.PCBTaskTime = myTimer(true);
			logStr.Format("Pcb Holder Z��ġ �̵� �Ϸ�[%d]", iUseStep);
  			putListLog(logStr);
			//151109 ���� �ӽ� ������
			Sleep(1000);
  			iRtnFunction = 13500;
		}
		else
		{
			logStr.Format("Pcb Holder Z��ġ �̵� ����[%d]", iUseStep);
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
			logStr.Format("PCB Holder ��˻� %d ȸ ����[%d]\n ���� ���������� �����Ͻðڽ��ϱ�?", Task.m_iRetry_Opt, iUseStep);
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
				logStr.Format("PCB Holder ��˻� ����[%d]", iUseStep);
				putListLog(logStr);
				//logStr.Format("PCB Holder ��˻� %d ȸ ����[%d]", Task.m_iRetry_Opt, iUseStep);
				//errMsg2(Task.AutoFlag,logStr);
			}
			
			break;
		}
		
		offsetX = offsetY = offsetTh = 0.0;
		iRtn = procCamAlign(CAM2-1, PCB_Holder_MARK, false, offsetX, offsetY, offsetTh);
		offsetTh = 0.0;
		saveInspImage(PCB_IMAGE_SAVE, Task.m_iRetry_Opt);

		Task.m_iRetry_Opt++;
		offsetTh = 0;//holder x,y�ุ
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
				iRtnFunction = 14000;				// ���� �̵�
			}
			else if(iRtnVal==2 )
			{
				sLangChange.LoadStringA(IDS_STRING654);	//Lens Align [%d] �Ϸ� [%d]
				logStr.Format("		Pcb Holder [%d] �Ϸ� [%d]", Task.m_iRetry_Opt, iUseStep);
				putListLog(logStr);

				InstantMarkDelete(PCB_Holder_MARK);			//�ӽ� ��� ��ũ ����

				logStr.Format("		Pcb Holder Complete : %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]);//0.000);
				AlignResultSave(logStr);	
				Task.m_iRetry_Opt = 0;
				iRtnFunction = 14100;				// �Ϸ�
			}
			else
			{
				logStr.Format("Pcb Holder [%d] ���� ���� �ʰ�[%d]", Task.m_iRetry_Opt, iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				AlignLogSave(logStr);
				InstantMarkDelete(PCB_Holder_MARK);
				iRtnFunction = -13700;				// NG
			}
		}
		else
		{
			logStr.Format("Pcb Holder [%d] ��ũ �ν� ����[%d]", Task.m_iRetry_Opt, iUseStep);
			putListLog(logStr);
			Sleep(300);
			iRtnFunction = 13700;					//  ��˻� 
		}
		break;

	case 14000://!! ������ �̵�.- Retry
		if(motor.Pcb_Holder_Align_Move(Holder_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 13500;
		}
		else
		{
			logStr.Format("Pcb Holder ���� ��ġ �̵� ����[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -14000;
		}
		break;
	case 14100:	
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format(" PCB Z�� �����ġ �̵� ����[%d]", iUseStep);
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
		logStr.Format("PCB ��ǰ ���� Thread Step ��ȣ ������ . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	return iRtnFunction;
}

////// AA Flow Step

int CAABonderDlg::RunProc_LensNewPassPickup(int iUseStep)
{//! Lens Pickup�� ��ǰ �ѱ� Step	(15000 ~ 16600)
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
				logStr.Format("Lens_Z�� ����ġ �̵� ����");
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -15000;	
				break;
			}

			if(!motor.PCB_Z_Motor_Move(Wait_Pos))
			{
				logStr.Format("PCB Z�� �����ġ �̵� ����[%d]", iUseStep);
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -15000;	
				break;
			}

			if( Task.m_bOKLensPass != 1 )//! Lens �ѱ� �̿Ϸ� �� ���	 
			{
				if( !motor.Pcb_Motor_Move(Loading_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], 0) )
				{
					logStr.Format("PCB %s �̵� ����", MotorPosName[Loading_Pos]);
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
			logStr.Format("Lens Unit Gripper ���� ����[%d]", iUseStep);
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

	case 15200: // ����
		if(!motor.Lens_Tilt_Motor_Move(Loading_Pos))
		{
			logStr.Format("Lens %s Tx,Ty�̵� ����", MotorPosName[Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -15200;
			break;
		}
		if(!motor.Lens_Motor_MoveXY(Loading_Pos))
		{
			logStr.Format("Lens %s X,Y �̵� ����", MotorPosName[Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -15200;
			break;
		}
		logStr.Format("Lens ���� ��ġ �̵� �Ϸ�[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 15250;
		break;

	case 15250: 
		if(!motor.PCB_Z_Motor_Move(Loading_Pos))
		{
			logStr.Format("PCB loading Z �̵� ����");
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
			logStr.Format("Lens Z ��ġ �̵� ���� [%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -15252;
		}
		break;

	case 15255:
		iRtnFunction = 15300;
		break;
	case 15300: // ���� �׸�
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
			logStr.Format("Lens �׸� ���� [%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -15300;
		}
		break;

	case 15350://���� �׸� Ȯ��
		if(Dio.LensMotorGripCheck(true, false))
		{
			logStr.Format("Lens �׸� �Ϸ�[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 15400;

			Sleep(800);
		}
		else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("Lens Gripper ���� ���� �ð� �ʰ�..[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -15350;
		}
		break;
	case 15400:
		iRtnFunction = 15450;
		//Sleep(400);	//�Ǹ��� ������� PCB Stage UnGrip�� ���� ���� ��, Delay
		/*if(Dio.HolderGrip(false,false))
		{
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 15450;
		}
		else
		{
			logStr.Format("Lens Gripper ���� ����..[%d]", iUseStep);
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
			logStr.Format("PCB Stage Lens Gripper ���� ����..[%d]", iUseStep);
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
	case 15500: // ��� ��ġ �̵�		//���� �׸����¿��� �����ϸ� �̸� �´� ����������

		if(!motor.LENS_Z_Motor_Move(Wait_Pos))
		{	
			logStr.Format("Lens Z ��� ��ġ �̵� ���� [%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -15500;
		}
		//
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB waiting Z �̵� ����");
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -15500;
		}
		iRtnFunction = 15550;
		break;
	case 15550:
		if (motor.Pcb_Motor_Move(Wait_Pos))			//PCB�� ���� �����ġ �̵�
		{
			iRtnFunction = 15800;
			logStr.Format("Pcb x,y ��� ��ġ �̵� �Ϸ� [%d]", iUseStep);
			putListLog(logStr);
		}
		else
		{
			logStr.Format("Pcb x,y ��� ��ġ �̵� ���� [%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = -15550;
		}
		break;
	case 15800:

		Task.LensTask = 30000;	//pcb AA�϶� lens �� Align �̵��� laser
		iRtnFunction = 15900;
		logStr.Format("PCB Epoxy Waiting[%d]", iUseStep);
		putListLog(logStr);
		break;
	case 15900:
		//pcb �� 
		//lens�� Align , laser��ġ�� 
		//������ġ�� ������ ������ ��� ��ġ�ΰ��� ����ؾߵȴ�.

		if (Task.interlockLens == 1)
		{
			Task.interlockLens = 0;
			iRtnFunction = 26000;
		}
		break;
default:
		logStr.Format("Lens ���� ���� Thread Step ��ȣ ������ . [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}

	return iRtnFunction;
}
int CAABonderDlg::RunProc_LensLoading(int iUseStep)
{//! Lens Pickup�� ��ǰ �ѱ� Step	(15000 ~ 16600)
	int iRtnFunction = iUseStep;
	int iRtn = 0;
	CString logStr = "";
	double	dFinalDelay = (double)(model.strInfo_Cam[3].m_iDelayTime);
	bool bChk = true; 
	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;
	//Task.m_bOKLensPass = -1;//PCB��ε� �������� �ѱ���·� ���������Ƿ� �ʱ�ȭ.

	switch( iUseStep )
	{
		case 40000:
			if(!motor.LENS_Z_Motor_Move(Wait_Pos)) 
			{
				logStr.Format("Lens_Z�� ����ġ �̵� ����");
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -40000;	
				break;
			}

			if(!motor.PCB_Z_Motor_Move(Wait_Pos))
			{
				logStr.Format("PCB Z�� �����ġ �̵� ����[%d]", iUseStep);
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -40000;	
				break;
			}
			 
			if( Task.m_bOKLensPass != 1 )//! Lens �ѱ� �̿Ϸ� �� ���	 
			{

				Task.d_Align_offset_x[LENS_Align_MARK] = 0.0;
				Task.d_Align_offset_y[LENS_Align_MARK] = 0.0;
				if( !motor.Pcb_Motor_Move(Loading_Pos, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], 0) )
				{
					logStr.Format("PCB %s �̵� ����", MotorPosName[Loading_Pos]);
					delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
					iRtnFunction = -40000;	
					break;
				}
				//M_TRINITY ���� �������� ƿƮ ���ؼ� ������ġ���� �ٽ� ������ ƿƮ�� tx,ty ������ �̵� 
				if (!motor.Pcb_Tilt_Motor_Move(Loading_Pos))
				{
					logStr.Format("PCB %s �̵� ����", MotorPosName[Loading_Pos]);
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
			logStr.Format("Lens Unit Gripper ���� ����[%d]", iUseStep);
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

	case 40200: // ���� 
		if (!motor.Lens_Tilt_Motor_Move(Loading_Pos))
		{
			logStr.Format("Lens %s Tx,Ty�̵� ����", MotorPosName[Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -40200;
			break;
		}
		if (!motor.Lens_Motor_MoveXY(Loading_Pos))
		{
			logStr.Format("Lens %s X,Y �̵� ����", MotorPosName[Loading_Pos]);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -40200;
			break;
		}

		logStr.Format("Lens ���� ��ġ �̵� �Ϸ�[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 40250;
		break;
	case 40250:
		if (!motor.PCB_Z_Motor_Move(Loading_Pos))
		{
			logStr.Format("PCB loading Z �̵� ����");
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40250;
			break;
		}
		if (!motor.LENS_Z_Motor_Move(Loading_Pos))
		{
			logStr.Format("Lens Z ��ġ �̵� ���� [%d]", iUseStep);
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
	case 40300: // ���� �׸�
		if(Dio.LensMotorGrip(true,false))
		{
			Task.m_bOKLensPass = 1;
			Task.PCBTaskTime = myTimer(true);
 			iRtnFunction = 40350;
		}
		else
		{
			Task.m_bOKLensPass = -1;
			logStr.Format("Lens �׸� ���� [%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -40300;
		}
		break;

	case 40350://���� �׸� Ȯ��
		if(Dio.LensMotorGripCheck(true, false))
		{
			logStr.Format("Lens �׸� �Ϸ�[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 40400;
		}
		else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("Lens Gripper ���� ���� �ð� �ʰ�..[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -40350;
		}
		break;

	case 40400:
		Sleep(500);	//�Ǹ��� ������� PCB Stage UnGrip�� ���� ���� ��, Delay 0.2s
		iRtnFunction = 40455;
		break;

	case 40455:
		iRtnFunction = 40500;
		break;
	case 40500: // ��� ��ġ �̵�
		if(!motor.LENS_Z_Motor_Move(Wait_Pos))
		{	
			logStr.Format("Lens Z ��� ��ġ �̵� ���� [%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -40500;
			break;
		}
		//
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			logStr.Format("PCB waiting Z �̵� ����");
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -40500;
			break;
		}
		iRtnFunction = 40550;
		break;
	case 40550:
		//���ʿ� ���� ����.
		/*if (!motor.Lens_Motor_MoveXY(Wait_Pos))
		{
			logStr.Format("Lens�� ���� ��� ��ġ �̵� ����[%d]", iUseStep);
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
		logStr.Format("Lens ���� ���� Thread Step ��ȣ ������ . [%d]", iUseStep);
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
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;	//PCB ���� ȹ�� Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser ������ Delay
	switch( iUseStep )
	{
		case 27000:
			Task.oldLaserTx = 0.1;
			Task.oldLaserTy = 0.1;
			iLaser_Pos = 0;// 4;//4�����Ͱ� laser out point
			LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align�� ���� ON
			if(!motor.PCB_Z_Motor_Move(Wait_Pos)) 
			{
				logStr.Format("[���� �غ�] PCB Z�� �����ġ �̵� ����[%d]", iUseStep);
				delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
				iRtnFunction = -27000;	
				break;
			}

			//if ((sysData.m_iDispensePass == 1) || (Task.m_bOkDispense == 1))
			//{
			//	iRtnFunction = 30000;	//-- ���� Pass Mode �̰ų�, ���� �Ϸ� �� ���
			//	break;
			//}

			iRtnFunction = 27100;// 27350;//<-�ӽ� �н�   Original-> 27100; 
			break;

		case 27100:
			if(motor.Pcb_Motor_Move_Laser(iLaser_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
			{
				Task.PCBTaskTime = myTimer(true);
				logStr.Format("Laser ���� ���� �ܺ� ��ġ �̵� �Ϸ�[%d]", iUseStep);
  				putListLog(logStr);
  				iRtnFunction = 27150;
			}
			else
			{
				logStr.Format("Laser ���� ���� �ܺ� ��ġ �̵� ����[%d]", iUseStep);
				errMsg2(Task.AutoFlag,logStr);
 				iRtnFunction = -27100;
			}
			break;

		case 27150:
			if(iLaser_Pos==0)
			{
				if(!motor.PCB_Z_Motor_Move(Laser_Pcb_Pos)) 
				{
					logStr.Format("PCB Laser_Pcb_Pos Z �̵� ����[%d]", iUseStep);
					putListLog(logStr);
					errMsg2(Task.AutoFlag, logStr);
					iRtnFunction = -27150;
					break;
				}
				else
				{
					logStr.Format("PCB Laser_Pcb_Pos Z �̵� �Ϸ�[%d]", iUseStep);
					putListLog(logStr);
				}
			}

			if (iLaser_Pos == 0) { iRtnFunction = 27160; }//ó������ �Ǹ��� �ϰ�
			else { iRtnFunction = 27200; }//ó���ƴϸ� �ٷ� �˻��.
			Task.PCBTaskTime = myTimer(true);
			break;
			//laser �Ǹ��� �ϰ�
		case 27160:
			if (Dio.LaserCylinderUPDOWN(false, false))
			{
				logStr.Format("LASER �Ǹ��� �ϰ� �Ϸ�[%d]", iRtnFunction);
				theApp.MainDlg->putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
				iRtnFunction = 27170;
			}
			else
			{
				logStr.Format("LASER �Ǹ��� �ϰ� ���� [%d]", iRtnFunction);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27160;
			}
			break;

		case 27170:
			if (Dio.LaserCylinderCheck(false, false))
			{
				logStr.Format("LASER �Ǹ��� �ϰ� Ȯ�� �Ϸ�[%d]", iRtnFunction);
				theApp.MainDlg->putListLog(logStr);
				iRtnFunction = 27200;

				Sleep(300);
			}
			else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
			{
				logStr.Format("LASER �Ǹ��� �ϰ� �ð� �ʰ�..[%d]", iRtnFunction);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27170;
			}
			break;
			//laser�Ǹ��� �ϰ� END
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
			logStr.Format("�� ������ %lf - %d ��ġ[%d]",Task.m_Laser_Point[iLaser_Pos],  iLaser_Pos+1, iUseStep );
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
						logStr.Format("Laser �������̻�: %.04lf, %.04lf, %.04lf, %.04lf [%d]", LaserValue[0], LaserValue[1], LaserValue[2], LaserValue[3], iUseStep);
						errMsg2(Task.AutoFlag,logStr);
 						iRtnFunction = -27250;
						break;
					}

					if(_calcLaserTilt(LaserPos, LaserValue, Task.d_Align_offset_xt[PCB_Chip_MARK], Task.d_Align_offset_yt[PCB_Chip_MARK]))
					{
						Task.m_dataOffset_x[0] =  Task.d_Align_offset_xt[PCB_Chip_MARK];
						Task.m_dataOffset_y[0] =  Task.d_Align_offset_yt[PCB_Chip_MARK];
						logStr.Format("	������ Tx: %.04lf, Ty: %.04lf", Task.d_Align_offset_xt[PCB_Chip_MARK], Task.d_Align_offset_yt[PCB_Chip_MARK] );
  						putListLog(logStr);

						int Rnd = TiltAlignLimitCheck(Task.d_Align_offset_xt[PCB_Chip_MARK],Task.d_Align_offset_yt[PCB_Chip_MARK]);
						if(Rnd == 2)
						{
							if( Task.m_dataOffset_x[0] == 0 || Task.m_dataOffset_y[0] == 0 )
							{
								logStr.Format("Laser ���� �� �� �̻� �߻�..Xt : %lf, Yt : %lf",Task.m_dataOffset_x[0], Task.m_dataOffset_y[0]);
								//errMsg2(Task.AutoFlag, logStr);
								//iRtnFunction = -27250;
								//break;
							}
							if( !g_ADOData.func_AATaskToRecordLaser(Task.ChipID, Task.m_dataOffset_x[0], Task.m_dataOffset_y[0], Task.m_Laser_Point) )
							{
								logStr.Format("DataBase Laser ���� ���� Data ��� ����.[%d]\n MS Office�� �ݾ��ּ���.", iUseStep);
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
							iRtnFunction = 27310;//->Laser�Ǹ������, 27350;->PCB WaitPos
							iLaser_Pos++;
						}
						else if (Rnd == 1)
						{
							iRtnFunction = 27300;
						}
						else
						{
							logStr.Format("Laser Tilt  ������ Limit�� �ʰ� �Ͽ����ϴ�.[%d]", iUseStep);
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
				logStr.Format("PCB tx,ty��  ��ġ �̵� ����[%d]", iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27300;
			}
			break;

			//laser �Ǹ��� ���
		case 27310:
			if (Dio.LaserCylinderUPDOWN(true, false))
			{
				logStr.Format("LASER �Ǹ��� ��� �Ϸ�[%d]", iRtnFunction);
				theApp.MainDlg->putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
				iRtnFunction = 27320;
			}
			else
			{
				logStr.Format("LASER �Ǹ��� ��� ���� [%d]", iRtnFunction);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27310;
			}
			break;
		case 27320:
			if (Dio.LaserCylinderCheck(true, false))
			{
				logStr.Format("LASER �Ǹ��� ��� Ȯ�� �Ϸ�[%d]", iRtnFunction);
				theApp.MainDlg->putListLog(logStr);
				iRtnFunction = 27350;
				Sleep(300);
			}
			else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
			{
				logStr.Format("LASER �Ǹ��� ��� �ð� �ʰ�..[%d]", iRtnFunction);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27320;
			}
			break;
			//laser�Ǹ��� ��� END

		case 27350:
			if(motor.PCB_Z_Motor_Move(Wait_Pos))
			{
				logStr.Format("PCB Z�� ��� ��ġ �̵� �Ϸ�[%d]", iUseStep);
				putListLog(logStr);
				iRtnFunction = 27600;
			}
			else
			{
				logStr.Format("PCB Z�� ��� ��ġ �̵� ����[%d]", iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27350;
			}
			
			break;
		case 27600:

			iRtnFunction = 27650;
			break;
		case 27650:
			//Sensor Align �߰�
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
			LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_SENSOR]);		// Align�� ���� ON

			if (motor.Pcb_Motor_Move(Align_Pos))
			{
				if (motor.PCB_Z_Motor_Move(Align_Pos))
				{
					logStr.Format("PCB Sensor Align ��ġ �̵� �Ϸ�[%d]", iUseStep);
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
				logStr.Format("PCB Sensor Align ��ġ �̵� ���� [%d]", iUseStep);
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
				logStr.Format("PCB Align(*Laser*) ��˻� %d ȸ ����[%d]\n ���� ���������� �����Ͻðڽ��ϱ�?", Task.m_iRetry_Opt, iUseStep);
				Task.m_iRetry_Opt = 0;
				Task.d_Align_offset_x[PCB_Chip_MARK] = 0.0;
				Task.d_Align_offset_y[PCB_Chip_MARK] = 0.0;
				Task.d_Align_offset_th[PCB_Chip_MARK] = 0.0;
				if (askMsg(logStr) == IDOK)
				{
					iRtnFunction = 27960;
					logStr.Format("PCB Align �̺��� ����[%d]", iUseStep);
					putListLog(logStr);
				}
				else
				{
					iRtnFunction = 27900;
					logStr.Format("PCB Align ��˻� ����[%d]", iUseStep);
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
					iRtnFunction = 27950;				// ���� �̵�
				}
				else if (iRtnVal == 2)
				{
					logStr.Format("		PCB Sensor Align [%d] �Ϸ� [%d]", Task.m_iRetry_Opt, iUseStep);
					putListLog(logStr);
					logStr.Format("		PCB Sensor Align Complete : %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
					AlignResultSave(logStr);
					Task.m_iRetry_Opt = 0;

					Task.m_timeChecker.Measure_Time(4);	//Laser ���� ���� ���� �ð�

					
					iRtnFunction = 27960;		// �Ϸ�
					LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align�� ���� ON 
					LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, 0);		// Align�� ���� ON
				}
				else
				{
					logStr.Format("PCB Sensor Align [%d] ���� ���� �ʰ�", Task.m_iRetry_Opt, iUseStep);
					errMsg2(Task.AutoFlag, logStr);
					AlignLogSave(logStr);
					iRtnFunction = -27900;				// NG
				}
			}
			else
			{
				logStr.Format("PCB Sensor Align(*Laser*) [%d] ��ũ �ν� ����[%d]", Task.m_iRetry_Opt, iUseStep);
				putListLog(logStr);
				Sleep(500);
				iRtnFunction = 27900;				//  ��˻� 
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
				logStr.Format("PCB Sensor Align ���� ��ġ �̵� ����[%d]", iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27950;
			}
			break;
		case 27960:
			if (!motor.PCB_Z_Motor_Move(Wait_Pos))
			{
				logStr.Format("PCB�� Z�� ��� ��ġ �̵� ����[%d]", iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -27960;
				break;
			}
			else
			{
				logStr.Format("PCB�� Z�� ��� ��ġ �̵� �Ϸ�[%d]", iUseStep);
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
		logStr.Format("Pcb Thread Step ��ȣ ������ .[%d]", iUseStep);
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
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;	//PCB ���� ȹ�� Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser ������ Delay
	switch( iUseStep )
	{
	case 26000:
		offsetX = offsetY = offsetTh = 0.0;
		if(!motor.PCB_Z_Motor_Move(Wait_Pos)) 
		{
			logStr.Format("		PCB Z�� �����ġ �̵� ����[%d]", iUseStep);
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
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_SENSOR]);		// Align�� ���� ON

		if(motor.Pcb_Motor_Move(Align_Pos))
		{
			if(motor.PCB_Z_Motor_Move(Align_Pos))
			{
				logStr.Format("PCB Sensor Align ��ġ �̵� �Ϸ�[%d]", iUseStep);
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
			logStr.Format("PCB Sensor Align ��ġ �̵� ���� [%d]", iUseStep);
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
			logStr.Format("PCB Align(*SensorAlign*) ��˻� %d ȸ ����[%d]\n ���� ���������� �����Ͻðڽ��ϱ�?", Task.m_iRetry_Opt, iUseStep);
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
				logStr.Format("PCB Align ��˻� ����[%d]", iUseStep);
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
				iRtnFunction = 26625;				// ���� �̵�
			}
			else if(iRtnVal==2 )
			{
				logStr.Format("		PCB Sensor Align [%d] �Ϸ� [%d]", Task.m_iRetry_Opt, iUseStep);
				putListLog(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);			//�ӽ� ��� ��ũ ����

				logStr.Format("		PCB Sensor Align Complete : %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
				AlignResultSave(logStr);	
				Task.m_iRetry_Opt = 0;

				Task.m_timeChecker.Measure_Time(4);	//Laser ���� ���� ���� �ð�
				
				iRtnFunction = 26629;		// �Ϸ�//iRtnFunction = 16630;				// �Ϸ�
				LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align�� ���� ON
			}
			else
			{
				logStr.Format("PCB Sensor Align [%d] ���� ���� �ʰ�", Task.m_iRetry_Opt, iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				AlignLogSave(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);
				iRtnFunction = -26620;				// NG
			}
		}
		else
		{
			logStr.Format("PCB Sensor Align [%d] ��ũ �ν� ����[%d]", Task.m_iRetry_Opt, iUseStep);
			putListLog(logStr);
			Sleep(300);
			iRtnFunction = 26620;				//  ��˻� 
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
			logStr.Format("PCB Sensor Align ���� ��ġ �̵� ����[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -26625;
		}
		break;
	case 26629:
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB�� z�� ���� ��ġ �̵� ����[%d]
			logStr.Format(sLangChange, iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -26629;	
			break;
		}
		iRtnFunction = 26990;
		break;
	case 26990://! Lens-Z�� ��� ��ġ �̵�
#if (____AA_WAY == PCB_TILT_AA)
		if(sysData.m_iEpoxyLaserInspPass == 1)
		{
			iRtnFunction = 30000;	//�����̵�
		}
		else
		{
			iRtnFunction = 27000;	//Laser���� ->Align -> ����
		}
#elif (____AA_WAY == LENS_TILT_AA)
		if (sysData.m_iEpoxyLaserInspPass == 1)
		{
			iRtnFunction = 30000;//Laser ���� Pass
		}else
		{
			iRtnFunction = 27000;
		}
#endif
		break;
	default:
		logStr.Format("Lens �ܺ� Align Thread Step ��ȣ ������ [%d]", iUseStep);
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
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;	//PCB ���� ȹ�� Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser ������ Delay
	switch( iUseStep )
	{
	case 35000:
		checkMessage();
		offsetX = offsetY = offsetTh = 0.0;
		if(!motor.PCB_Z_Motor_Move(Wait_Pos)) 
		{
			logStr.Format("		PCB Z�� �����ġ �̵� ����[%d]", iUseStep);
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
		Task.m_bOKLensPass = -1;//PCB��ε� �������� �ѱ���·� ���������Ƿ� �ʱ�ȭ.
		Task.d_Align_offset_x[LENS_Align_MARK] = 0;
		Task.d_Align_offset_y[LENS_Align_MARK] = 0;
		Task.d_Align_offset_th[LENS_Align_MARK] = 0;


		if( Task.m_bOKLensPass != 1 )//! Lens �ѱ� �̿Ϸ� �� ���	 
		{
			iRtnFunction = 35150;
		}else
		{
			logStr.Format("Lens �ѱ� �Ϸ�� Lens Align Pass![%d]", iUseStep);
  			putListLog(logStr);
			iRtnFunction = 35400;
		}
		break;

	case 35150:
		ctrlSubDlg(MAIN_DLG);
		m_iCurCamNo =0;
		//SetDigReference(LENS_Align_MARK);
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_LENS]);		// Align�� ���� ON
		if(motor.Pcb_Motor_Move(Lens_Pos))
		{
			if(motor.PCB_Z_Motor_Move(Lens_Pos))
			{
				logStr.Format("PCB Lens_Pos Align ��ġ �̵� �Ϸ�[%d]", iUseStep);
  				putListLog(logStr);
				Task.PCBTaskTime = myTimer(true);
				
				iRtnFunction = 35200;
			}
		}
		else
		{
			logStr.Format("PCB Sensor Align ��ġ �̵� ���� [%d]", iUseStep);
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
			logStr.Format("PCB Align ��˻� %d ȸ ����[%d]\n ���� ���������� �����Ͻðڽ��ϱ�?", Task.m_iRetry_Opt, iUseStep);
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
				logStr.Format("PCB Lens Align ��˻� ����[%d]", iUseStep);
				putListLog(logStr);

				//iRtnFunction = -35250;
				//logStr.Format("PCB Align ��˻� %d ȸ ����[%d]", Task.m_iRetry_Opt, iUseStep);
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
				iRtnFunction = 35300;				// ���� �̵�
			}
			else if(iRtnVal==2 )
			{
				logStr.Format("		PCB Lens Align [%d] �Ϸ� [%d]", Task.m_iRetry_Opt, iUseStep);
				putListLog(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);			//�ӽ� ��� ��ũ ����

				logStr.Format("		PCB Lens Align Complete : %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
				AlignResultSave(logStr);	
				Task.m_iRetry_Opt = 0;

				Task.m_timeChecker.Measure_Time(4);	//Laser ���� ���� ���� �ð�
				
				iRtnFunction = 35400;		// �Ϸ�
			}
			else
			{
				logStr.Format("PCB Lens Align [%d] ���� ���� �ʰ�", Task.m_iRetry_Opt, iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				AlignLogSave(logStr);
				//InstantMarkDelete(PCB_Chip_MARK);
				iRtnFunction = -35250;				// NG
			}
		}
		else
		{
			logStr.Format("PCB Lens Align [%d] ��ũ �ν� ����[%d]", Task.m_iRetry_Opt, iUseStep);
			putListLog(logStr);

			iRtnFunction = 35200;				//  ��˻� 
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
			logStr.Format("PCB Sensor Align ���� ��ġ �̵� ����[%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
			iRtnFunction = -35300;
		}
		break;
	case 35400:
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB�� z�� ���� ��ġ �̵� ����[%d]
			logStr.Format(sLangChange, iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -35400;	
			break;
		}
		iRtnFunction = 35500;
		break;
	case 35500://! Lens-Z�� ��� ��ġ �̵�
		iRtnFunction = 39000;
		break;
	default:
		logStr.Format("Lens �ܺ� Align Thread Step ��ȣ ������ [%d]", iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}
	logStr.Empty();
	return iRtnFunction;
}

int CAABonderDlg::RunProc_EpoxyNewResing(int iUseStep)
{//! Epoxy ���� �� ���� �˻� Step(17000 ~ 17999)
	int iRtnFunction = iUseStep;
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;		//PCB ���� ȹ�� Delay
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser ������ Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;

	switch( iUseStep )
	{
		case 30000:
			if(!motor.PCB_Z_Motor_Move(Wait_Pos))
			{
				sLangChange.LoadStringA(IDS_STRING1012);	//PCB_Z�� �����ġ �̵� ����.
				logStr.Format(sLangChange, iUseStep);
  				putListLog(logStr);
				iRtnFunction = -30000;
				break;
			}

			if((sysData.m_iDispensePass == 1) || (Task.m_bOkDispense == 1) )	
			{
					iRtnFunction = 31000;	//-- ���� Pass Mode �̰ų�, ���� �Ϸ� �� ���
			}else																
			{
					iRtnFunction = 30020;
			}
			break;
		case 30020: //Dispenser -Z�� �ϰ�
			if(motor.PCB_Z_Motor_Move(Wait_Pos))
			{
  				iRtnFunction = 30030;
			}
			else
			{
				logStr.Format("PCB Z Wait ��ġ �̵� �Ϸ� ���� [%d]", iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = -30020;
			}
			break;
	case 30030:  //���� ��ġ �̵�
		LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align ���� OFF
		if(motor.Pcb_Motor_Move(Dispense_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
		{
			sLangChange.LoadStringA(IDS_STRING935);	//PCB Dispense ���� ��ġ �̵� �Ϸ�-������ X:%.03lf,Y:%.03lf,T:%.03lf
			logStr.Format(sLangChange, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]);
  			putListLog(logStr);
  			iRtnFunction = 30040;
		}
		else
  		{
			sLangChange.LoadStringA(IDS_STRING938);	//PCB Dispense ��ġ �̵� ����[%
  			logStr.Format(sLangChange + _T("d]"), iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -30030;
  		}
		break;
	case 30040: //Dispenser-z �� ���
		if(motor.PCB_Z_Motor_Move(Dispense_Pos))
		{
			iRtnFunction = 30050;
		}else
		{
			sLangChange.LoadStringA(IDS_STRING977);	//PCB Z Dispense ��ġ �̵� ����
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -30040;
		}
		break;

	case 30050: // ���潺 ����

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
			//point ��
		}

		if(bChk)
		{
			Task.m_bOkDispense = 1;

			logStr.Format("PCB Dispense �Ϸ�[%d]", iUseStep);
  			putListLog(logStr);

  			iRtnFunction = 30070;
		}
		else
		{
			logStr.Format("PCB Dispense ���� [%d]", iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -30050;
		}
		break;
	case 30070:
		if(motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y))
		{
			LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_SENSOR]);//���� �˻��� ����θ�ũ�� ����Ʈ
			iRtnFunction = 30100;
		}
		break;
	case 30100:
		iRtnFunction = 31000;
		break;
	case 31000:
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING305);	//PCB�� z�� ���� ��ġ �̵� ����[%d]
			logStr.Format(sLangChange, iUseStep);
			delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
			iRtnFunction = -31000;
			break;
		}else
		{
			logStr.Format("PCB Z ��� ��ġ �̵� �Ϸ�[%d]", iUseStep);
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

	case 32000: // �����˻� �̵�
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_HOLDER]);		// Align�� ���� ON
		if (motor.Pcb_Motor_Move(Holder_Pos))
		{
  			iRtnFunction = 32500;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING989);	//PCB �����˻� ��ġ �̵� ����
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -32000;
		}
		break;

	case 32500:// �̵���, ���� �ð� ���� �� ��ũ Ȯ��
		if(motor.PCB_Z_Motor_Move(Holder_Pos))

		{
			Task.PCBTaskTime = myTimer(true);
			sLangChange.LoadStringA(IDS_STRING990);	//PCB �����˻� ��ġ �̵� �Ϸ�[%d]
			logStr.Format(sLangChange, iUseStep);
  			putListLog(logStr);
			iRtnFunction = 33000;
		}else
		{
			sLangChange.LoadStringA(IDS_STRING989);	//PCB �����˻� ��ġ �̵� ����
			logStr.Format(sLangChange + _T("[%d]"), iUseStep);
			errMsg2(Task.AutoFlag,logStr);
 			iRtnFunction = -32500;
		}
		break;
	case 33000: //�����˻� z�� �̵�
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
	case 33500: //�����˻�                                �߰��ؾ���
		Sleep(100);
		vision.clearOverlay(CAM1);
		bChk = _EpoxyFinddispense(CAM1);
		vision.drawOverlay(CAM1);
		if(!bChk)
		{
			putListLog("Epoxy ���� �˻� ����");
			//logStr.Format("Epoxy Table ��ǥ (X:%.03f, Y:%.03f, ��:%.03f)", motor.GetEncoderPos(Motor_PCB_X), motor.GetEncoderPos(Motor_PCB_Y), motor.GetEncoderPos(Motor_PCB_TH));
			//putListLog(logStr);
			logStr.Format("Epoxy ���� �˻� ����. \n�ڵ� ���� ���� �Ͻðڽ��ϱ�?");
			//_stprintf_s(motor.m_szLogMsg, "Epoxy ���� �˻� ����. \n�ڵ� ���� ���� �Ͻðڽ��ϱ�?");

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
				logStr.Format("Epoxy ���� �˻� ����[%d]", iUseStep);
				iRtnFunction = -33500;
				break;
			}
		}
		Task.m_timeChecker.Measure_Time(3);	//���� �Ϸ� �ð�
		Task.m_dTime_Epoxy = Task.m_timeChecker.m_adTime[3] - Task.m_timeChecker.m_adTime[2];
		dispGrid();

		logStr.Format("PCB �����˻� �Ϸ�[%d]", iUseStep);
		putListLog(logStr);
		iRtnFunction = 34000;
		break;
	case 34000:
		if(!motor.PCB_Z_Motor_Move(Wait_Pos))
		{
			sLangChange.LoadStringA(IDS_STRING1012);	//PCB_Z�� �����ġ �̵� ����.
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
		sLangChange.LoadStringA(IDS_STRING939);	//PCB Dispenser Thread Step ��ȣ ������ .
		logStr.Format(sLangChange + _T(" [%d]"), iUseStep);
		errMsg2(Task.AutoFlag, logStr);
		iRtnFunction = -1;
		break;
	}

	return iRtnFunction;
}

int CAABonderDlg::RunProc_InspAAPos(int iUseStep) 
{
	//! Lens Pickup�� ��ǰ �ѱ� Step	(18000 ~ 19999)

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
            //if (sysData.m_iDefectInspPass == 1)	// DEFECT �˻� PASS
            //{
            //    iRtnFunction = 39999;
            //}
            //else
            //{
            //    iRtnFunction = 39100;
            //}
			//if(model.m_IsUseBrightDefectTest == 1)	// ���� Defect Picxel �˻� ����

			iRtnFunction = 39700;//aa�� defect Pass
			break;
		case 39100:
			LightControlthird.ctrlLedVolume(LIGHT_OC_6500K, model.m_iLedValue[LEDDATA_6500K]);
			if( !motor.Pcb_Motor_Move(OC_6500K_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]) )
			{
				logStr.Format("PCB�� ���� 0C 6500K �˻� ��ġ �̵� ����[%d]", iUseStep);
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


		case 39350: //UV�� OC Defect �˻� 
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

		case 39450:	//Dark �˻� ��ġ �̵�
			iRtnFunction = 39500;
			//if(model.m_IsUseDarkDefectTest == 1)	// �ϸ� Defect Picxel �˻� ����
			//{
			//	iRtnFunction = 18102;				//�ϸ� �˻� ��ġ �̵�
			//}
			//else
			//{
			//	iRtnFunction = 18115;				
			//}
			break;

		case 39500:	//Dark �˻� ��ġ �̵�
			if( !motor.Pcb_Motor_Move(Dark_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]) )
			{
				sLangChange.LoadStringA(IDS_STRING1022);	//PCB�� ���� Dark �˻� ��ġ �̵� ����[%d]
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
                logStr.Format("[AUTO] Defect �˻� ���� [%d]", iUseStep);
				MandoInspLog.bInspRes = false;
			}

			logStr.Format("[AUTO] Defect �˻� �Ϸ� [%d]", iUseStep);
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
		case 39900: // ���� ��ġ �̵�
			iRtnFunction = 39999; 	
			break;
		case 39999:			//��������� 180706_1
			//if( func_MIU_ConnectLiveCheck() == false)		//MIU ���� Check
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
			logStr.Format( "Pcb Align ���� Thread Step ��ȣ ������ . [%d]", iUseStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -1;
			break;
	}
	logStr.Empty();
	return iRtnFunction;
}

int CAABonderDlg::RunProc_Bonding_Pos_GO(int iUseStep)
{//! Lens Pickup�� ��ǰ �ѱ� Step	(18000 ~ 19999)
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
		case 41150: // ���� ��ġ �̵�
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 41200;
			break;
		case 41200:
			if (motor.Lens_Motor_MoveXY(Bonding_Pos))
			{

#if (____AA_WAY == LENS_TILT_AA)
				if (!motor.Lens_Tilt_Motor_Move(Bonding_Pos))
				{
					logStr.Format("Lens %s Tx,Ty�̵� ����", MotorPosName[Bonding_Pos]);
					delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
					iRtnFunction = -41200;
					break; 
				}

#endif
				logStr.Format("Lens Bonding ��ġ �̵� �Ϸ�[%d]", iRtnFunction);
				putListLog(logStr);
				Task.interlockPcb = 1;
				iRtnFunction = 41250;
			}
			else
			{
				logStr.Format("Lens Bonding ��ġ �̵� ����[%d]", iRtnFunction);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -41200;
				break;
			}
			break;
		case 41250:
#if (____AA_WAY == PCB_TILT_AA)
			if (motor.Pcb_Motor_Move(Bonding_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
			{
				sLangChange.LoadStringA(IDS_STRING913);	//"PCB Bonding ��ġ �̵� �Ϸ�[%d]"
				logStr.Format(sLangChange, iUseStep);
				putListLog(logStr);
				iRtnFunction = 41300;
				if (!motor.Pcb_Tilt_Motor_Move(Bonding_Pos))
				{
					logStr.Format("PCB tx,ty Bonding_Pos �̵� ����");
					delayMsg(logStr.GetBuffer(99), 3000, M_COLOR_RED);
					iRtnFunction = -41250;
					break;
				}
				break;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING912);	//PCB Bonding ��ġ �̵� ����
				logStr.Format(sLangChange + _T("[%d]"), iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -41250;
				break;
			}
#elif	(____AA_WAY == LENS_TILT_AA)
			if (motor.Pcb_Motor_Move(Bonding_Pos, Task.d_Align_offset_x[PCB_Chip_MARK], Task.d_Align_offset_y[PCB_Chip_MARK], Task.d_Align_offset_th[PCB_Chip_MARK]))
			{
				sLangChange.LoadStringA(IDS_STRING913);	//"PCB Bonding ��ġ �̵� �Ϸ�[%d]"
				logStr.Format(sLangChange, iUseStep);
				putListLog(logStr);
				iRtnFunction = 41300;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING912);	//PCB Bonding ��ġ �̵� ����
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
				sLangChange.LoadStringA(IDS_STRING912);	//PCB Bonding ��ġ �̵� ����
				logStr.Format(sLangChange + _T("[%d]"), iUseStep);
				errMsg2(Task.AutoFlag, logStr);
				iRtnFunction = -41250;
			}
			

			break;
		case 41310:
			if(Task.interlockPcb == 1)		//pcbƿƮ�� ���  ���� ������,����� �۾� ������ ������ġ �ö����� ��ٷ��ߵż�
			{
				iRtnFunction = 41350;
			}else if((myTimer(true) - Task.PCBTaskTime) > 50000)
			{
				logStr.Format("Lens Bonding ��ġ �̵� �ð� �ʰ�[%d]", iUseStep);
				errMsg2(Task.AutoFlag,logStr);
				iRtnFunction = -41310;
			}
			break;
		case 41350:
			iRtnFunction = 41400;

			break;
		case 41400:
			//if( func_MIU_ConnectLiveCheck() == false)		//MIU ���� Check
			//{
			//	iRtnFunction = -41400;
			//	break;
			//}

			setCamDisplay(3, 1);
			iRtnFunction = 48000;

			break;
		case 48000:
			LightControlthird.ctrlLedVolume(LIGHT_PCB, 0);		// Align�� ���� ON

			logStr.Format("Loading End[%d]", iUseStep);
			putListLog(logStr);
			iRtnFunction = 49000;
			break;
			//
		default:
			logStr.Format( "Pcb Align ���� Thread Step ��ȣ ������ . [%d]", iUseStep);
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
	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;		//PCB ���� ȹ�� Delay
	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
	int iRtn = 0;
	CString logStr = "";
	bool bChk = true;
	double offsetX = 0.0, offsetY = 0.0, offsetTh = 0.0;

	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser ������ Delay

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
			logStr.Format("Lens_Z�� �����ġ �̵� ����.[%d]", iLensStep);
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
			iRtnFunction = 43000;//41900;		//������ ī�޶� ����
		}
		else
		{
			logStr.Format("Lens �����ġ �̵� ����[%d]", iLensStep);
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
		if (sysData.m_iLaserInspPass == 1)	//����
		{
			iRtnFunction = 41100;			//Laser ���� Pass
		}
		else
		{
			iRtnFunction = 31000;		// 31300;//����� �ӽ� �н�
		}

#elif (____AA_WAY == LENS_TILT_AA)
		iRtnFunction = 43000;//41900;		//������ ī�޶� ����
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
			logStr.Format("Lens_Z�� �����ġ �̵� ����.[%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31000;
		}

		
		
		break;

	case 31050:
		LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_LENS]);
		if (motor.Lens_Motor_MoveXY(Lens_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			logStr.Format("Lens Align ��ġ �̵� �Ϸ�[%d]", iLensStep);
			putListLog(logStr);
			iRtnFunction = 31060;
		}
		else
		{
			logStr.Format("Lens Align ��ġ �̵� ����[%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31050;
		}
		break;
	case 31060:
		if (motor.LENS_Z_Motor_Move(Lens_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			logStr.Format("Lens Align Z ��ġ �̵� �Ϸ�[%d]", iLensStep);
			putListLog(logStr);
			iRtnFunction = 31080;
		}
		else
		{
			logStr.Format("Lens Align Z ��ġ �̵� ����[%d]", iLensStep);
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
			logStr.Format("Lens Align ��˻� %d ȸ ����[%d]\n ���� ���������� �����Ͻðڽ��ϱ�?", Task.m_iRetry_Opt, iLensStep);
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
				logStr.Format("Lens Align ��˻� %d ȸ ����[%d]", Task.m_iRetry_Opt, iLensStep);
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
			Task.d_Align_offset_th[LENS_Align_MARK] = 0;//+= offsetTh;	//Lens�� ������ Theta�� ����

			logStr.Format("Lens Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
			//pcbDlg->m_labelAlignResult.SetText(logStr);
			//pcbDlg->m_labelAlignResult.Invalidate();

			putListLog(logStr);

			int iRtnVal = AlignLimitCheck(CAM2, offsetX, offsetY, 0);
			if (iRtnVal == 1)
			{
				iRtnFunction = 31200;				// ���� �̵�
			}
			else if (iRtnVal == 2)
			{
				logStr.Format("		Lens Align [%d] �Ϸ� [%d]", Task.m_iRetry_Opt, iLensStep);
				putListLog(logStr);
				InstantMarkDelete(LENS_Align_MARK);			//�ӽ� ��� ��ũ ����

				logStr.Format("		Lens Align Complete : %d, %.03f, %.03f, %.03f", Task.m_iLensPickupNoCentering, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
				AlignResultSave(logStr);
				Task.m_iRetry_Opt = 0;
				iRtnFunction = 31250;// 31300;				// �Ϸ�
			}
			else
			{
				logStr.Format("Lens Align [%d] ���� ���� �ʰ�[%d]", Task.m_iRetry_Opt, iLensStep);
				errMsg2(Task.AutoFlag, logStr);
				AlignLogSave(logStr);
				InstantMarkDelete(LENS_Align_MARK);
				iRtnFunction = -31100;				// NG
			}
		}
		else
		{
			logStr.Format("Lens Align [%d] ��ũ �ν� ����[%d]", Task.m_iRetry_Opt, iLensStep);
			putListLog(logStr);
			iRtnFunction = 31100;					//  ��˻� 
		}
		break;

	case 31200://!! ������ �̵�.- Retry
		if (motor.Lens_Motor_Align_Move(Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 31080;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING657);	//Lens Align ���� ��ġ �̵� ����
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
			logStr.Format("Lens_Z�� �����ġ �̵� ����.[%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31250;
		}
		break;
	case 31300://! Lens-Z�� ��� ��ġ �̵�
		iLaser_Pos = 0;
		if (motor.Lens_Motor_Move_Laser(iLaser_Pos, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]))
		{
			Task.LensTaskTime = myTimer(true);
			logStr.Format("Laser ���� ���� �ܺ� ��ġ �̵� �Ϸ�[%d]", iLensStep);	//Laser ���� ���� �ܺ� ��ġ �̵� �Ϸ�[%d]
			putListLog(logStr);
			iRtnFunction = 40000;
		}
		else
		{
			logStr.Format("Laser ���� ���� �ܺ� ��ġ �̵� ���� [%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -31300;
		}
		break;
	case 40000://! Laser-Z�� �ܺ� ������ġ �̵�
		if (motor.LENS_Z_Motor_Move(Laser_Lens_Pos))
		{
			Task.LensTaskTime = myTimer(true); 
			iRtnFunction = 40100;
		}
		else
		{
			logStr.Format("Laser-Z�� ���� ���� �ܺ� ��ġ �̵� ���� [%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40000;
		}
		break;

	case 40100:
		iRtnFunction = 40200;
		break;

	case 40200://! Lens Stage �ܺ� Laser ������ġ �̵� 
		if (motor.Lens_Motor_Move_Laser(iLaser_Pos, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]))
		{
			Task.LensTaskTime = myTimer(true);
			logStr.Format("Laser ���� ���� �ܺ� ��ġ �̵� �Ϸ�[%d]", iLensStep);	//Laser ���� ���� �ܺ� ��ġ �̵� �Ϸ�[%d]
			putListLog(logStr);
			if (iLaser_Pos == 0) { iRtnFunction = 40210; }//ó���� �ϰ����.
			else { iRtnFunction = 40300; }
		}
		else
		{
			logStr.Format("Laser ���� ���� �ܺ� ��ġ �̵� ���� [%d]", iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40200;
		}
		break;

		//Laser �Ǹ��� �ϰ�
	case 40210:
		if (Dio.LaserCylinderUPDOWN(false, false))
		{
			logStr.Format("LASER �Ǹ��� �ϰ� �Ϸ�[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 40220;
		}
		else
		{
			logStr.Format("LASER �Ǹ��� �ϰ� ���� [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40210;
		}
		break;

	case 40220:
		if (Dio.LaserCylinderCheck(false, false))
		{
			logStr.Format("LASER �Ǹ��� �ϰ� Ȯ�� �Ϸ�[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 40300;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER �Ǹ��� �ϰ� �ð� �ʰ�..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -40220;
		}
		break;
	case 40300://! ���� �� Delay
		checkMessage();
		if ((myTimer(true) - Task.LensTaskTime) > iLaserDelay && (motor.IsStopAxis(Motor_Lens_X) && motor.IsStopAxis(Motor_Lens_Y)))	//�����Ӵ�, Delay Ȯ��
		{
			iRtnFunction = 41000;
		}
		break;
		//Laser �Ǹ��� �ϰ� END

	case 41000: //�ܺ� ���� ���� (���� ���� �ϰ� Align �̵�)
		//Keyence.func_LT9030_Scan(Task.m_Laser_Point[iLaser_Pos]);
		Keyence.func_CL3000_Scan(Task.m_Laser_Point[iLaser_Pos]); 
		logStr.Format("���� ���� %lf[%d] - �ܺ� - %d ��ġ", Task.m_Laser_Point[iLaser_Pos], iLensStep, iLaser_Pos+ 1);
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
					sLangChange.LoadStringA(IDS_STRING639);	//Laser ������ �̻�..�ܺ� ���� �������� �̻�..[%d]\n ������:%.04lf, %.04lf, %.04lf, %.04lf
					logStr.Format(sLangChange, iLensStep, LaserValue[0], LaserValue[1], LaserValue[2], LaserValue[3]);
					errMsg2(Task.AutoFlag, logStr);
					iRtnFunction = -41000;
					break;
				}

				if (_calcLaserTilt(LaserPos, LaserValue, Task.d_Align_offset_xt[LENS_Align_MARK], Task.d_Align_offset_yt[LENS_Align_MARK]))
				{
					Task.m_dataOffset_x[LENS_Align_MARK] = Task.d_Align_offset_xt[LENS_Align_MARK];
					Task.m_dataOffset_y[LENS_Align_MARK] = Task.d_Align_offset_yt[LENS_Align_MARK];
	

					sLangChange.LoadStringA(IDS_STRING1264);		//������ Tx: %.04lf, Ty: %.04lf
					logStr.Format(_T("		") + sLangChange, Task.d_Align_offset_xt[LENS_Align_MARK], Task.d_Align_offset_yt[LENS_Align_MARK]);
					putListLog(logStr);

					int Rnd = TiltAlignLimitCheck(Task.m_dataOffset_x[LENS_Align_MARK], Task.m_dataOffset_y[LENS_Align_MARK]);
					//20160117 ������ ����

					if (Rnd == 2)
					{

						Task.m_timeChecker.Measure_Time(5);	//Laser ���� ���� �Ϸ� �ð�
						Task.m_dTime_LaserDpm = Task.m_timeChecker.m_adTime[5] - Task.m_timeChecker.m_adTime[4];
						dispGrid();
						MandoInspLog.dTilteOffset[0] = Task.m_dataOffset_x[LENS_Align_MARK];	//���� ������Camera �˻� Log ����
						MandoInspLog.dTilteOffset[1] = Task.m_dataOffset_y[LENS_Align_MARK];


						//20141217 LHC - DataBase�� 0���� ����Ǵ� ��찡 ���� 0���� �� ��� ���������ʰ� �˶�ġ����.
						//�迵ȣ 20150602 FraneeGrabber  ���� �ӽ� Pass
						if (Task.m_dataOffset_x[LENS_Align_MARK] == 0 || Task.m_dataOffset_y[LENS_Align_MARK] == 0)
						{
							sLangChange.LoadStringA(IDS_STRING627);	//Laser ���� �� �� �̻� �߻�..
							logStr.Format(sLangChange + _T("Xt : %lf, Yt : %lf"), Task.m_dataOffset_x[LENS_Align_MARK], Task.m_dataOffset_y[LENS_Align_MARK]);
							errMsg2(Task.AutoFlag, logStr);
							iRtnFunction = -41000;
							break;
						}
						if (!g_ADOData.func_AATaskToRecordLaser(Task.ChipID, Task.m_dataOffset_x[LENS_Align_MARK], Task.m_dataOffset_y[LENS_Align_MARK], Task.m_Laser_Point))
						{
							sLangChange.LoadStringA(IDS_STRING489);	//DataBase Laser ���� ���� Data ��� ����.[%d]\n MS Office�� �ݾ��ּ���.
							logStr.Format(sLangChange, iLensStep);
							errMsg2(Task.AutoFlag, logStr);
							iRtnFunction = -41000;
							break;
						}

						LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_LENS]);		// Align�� ���� ON
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
						logStr.Format("Laser Tilt  ������ Limit�� �ʰ� �Ͽ����ϴ�.[%d]", iLensStep);
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

	case 41050: // �̵�  �������� 0���� ����� ���� �̵�
		if (motor.Lens_Move_Tilt(Task.d_Align_offset_xt[LENS_Align_MARK], Task.d_Align_offset_yt[LENS_Align_MARK]))
		{
			iRtnFunction = 40200; 
			iLaser_Pos = 0;//4;

		}
		break;

	case 41100:
		iRtnFunction = 41110;
		break;

		//laser�Ǹ��� ���
	case 41110:
		if (Dio.LaserCylinderUPDOWN(true, false))
		{
			logStr.Format("LASER �Ǹ��� ��� �Ϸ�[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			Task.PCBTaskTime = myTimer(true);
			iRtnFunction = 41120;
		}
		else
		{
			logStr.Format("LASER �Ǹ��� ��� ���� [%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41110;
		}
		break;

	case 41120:
		if (Dio.LaserCylinderCheck(true, false))
		{
			logStr.Format("LASER �Ǹ��� ��� Ȯ�� �Ϸ�[%d]", iRtnFunction);
			theApp.MainDlg->putListLog(logStr);
			iRtnFunction = 41200;

			Sleep(300);
		}
		else if ((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
		{
			logStr.Format("LASER �Ǹ��� ��� �ð� �ʰ�..[%d]", iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41120;
		}
		break;
		//laser�Ǹ��� ���END

	case 41200://! Lens-Z�� ��� ��ġ �̵�
		if (motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 41850;;//// 41300; //41850;//�ӽ� ���� align pass
								 //iRtnFunction = 41300;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING739);	//Lens_Z�� �����ġ �̵� ����
			logStr.Format(sLangChange + _T("[%d]"), iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41200;
		}
		break;

	case 41300: 
		if (motor.Lens_Motor_MoveXY(Lens_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			sLangChange.LoadStringA(IDS_STRING659);	//Lens Align �̵� [%d]
			logStr.Format(sLangChange, iLensStep);
			putListLog(logStr);
			iRtnFunction = 41400;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING660);	//Lens Align �̵� ����[%d]
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
			sLangChange.LoadStringA(IDS_STRING661);	//Lens Align ��ġ �̵� �Ϸ�[%d]
			logStr.Format(sLangChange, iLensStep);
			putListLog(logStr);
			iRtnFunction = 41500;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING660);	//Lens Align ��ġ �̵� ����[%d]
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

	case 41600://! LENS Align(�������� ������, Shift ��� �뵵)
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
			logStr.Format("Lens Align ��˻� %d ȸ ����[%d]\n ���� ���������� �����Ͻðڽ��ϱ�?", Task.m_iRetry_Opt, iLensStep);
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
				logStr.Format("Lens Align ��˻� %d ȸ ����[%d]", Task.m_iRetry_Opt, iLensStep);
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
			Task.d_Align_offset_th[LENS_Align_MARK] = 0;//+= offsetTh;	//Lens�� ������ Theta�� ����

			sLangChange.LoadStringA(IDS_STRING651);	//Lens Align : %.3lf %.3lf %.3lf
			logStr.Format("Lens Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
			//			Sleep(500);
			//pcbDlg->m_labelAlignResult.SetText(logStr);
			//pcbDlg->m_labelAlignResult.Invalidate();

			putListLog(logStr);

			int iRtnVal = AlignLimitCheck(CAM2, offsetX, offsetY, 0);
			if (iRtnVal == 1)
			{
				iRtnFunction = 41700;				// ���� �̵�
			}
			else if (iRtnVal == 2)
			{
				sLangChange.LoadStringA(IDS_STRING654);	//Lens Align [%d] �Ϸ� [%d]
				logStr.Format(_T("		") + sLangChange, Task.m_iRetry_Opt, iLensStep);
				putListLog(logStr);

				InstantMarkDelete(LENS_Align_MARK);			//�ӽ� ��� ��ũ ����

				sLangChange.LoadStringA(IDS_STRING655);	//Lens Align Complete : %d, %.03f, %.03f, %.03f
				logStr.Format(_T("		") + sLangChange, Task.m_iLensPickupNoCentering, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
				AlignResultSave(logStr);
				Task.m_iRetry_Opt = 0;
				iRtnFunction = 41800;				// �Ϸ�
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING653);	//Lens Align [%d] ���� ���� �ʰ�
				logStr.Format(sLangChange + _T("[%d]"), Task.m_iRetry_Opt, iLensStep);
				errMsg2(Task.AutoFlag, logStr);
				AlignLogSave(logStr);
				InstantMarkDelete(LENS_Align_MARK);
				iRtnFunction = -41600;				// NG
			}
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING652);	//Lens Align [%d] ��ũ �ν� ����[%d]
			logStr.Format(sLangChange, Task.m_iRetry_Opt, iLensStep);
			putListLog(logStr);

			iRtnFunction = 41600;					//  ��˻� 
		}
		break;

	case 41700://!! ������ �̵�.- Retry
		if (motor.Lens_Motor_Align_Move(Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 41500;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING657);	//Lens Align ���� ��ġ �̵� ����
			logStr.Format(sLangChange + _T("[%d]"), iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41700;
		}
	case 41800: // �� ����2

		//!!!!! 1�� Align ������ ���� ���� ����(PCB<-> Lens Sensor��)
		Task.m_dataOffset_x[1] = Task.d_Align_offset_x[LENS_Align_MARK];
		Task.m_dataOffset_y[1] = Task.d_Align_offset_y[LENS_Align_MARK];
		//		Task.m_dataOffset_th[1] = Task.d_Align_offset_th[LENS_Align_MARK] - Task.d_Align_offset_th[PCB_Chip_MARK];

		//sLangChange.LoadStringA(IDS_STRING962);	//PCB Sensor_Holder�� Align ������ : X:%.03f, Y:%.03f, ��:%.04f [%d]
		//logStr.Format(_T("		") + sLangChange, Task.m_dataOffset_x[1], Task.m_dataOffset_y[1],Task.m_dataOffset_th[1], iLensStep);
		//putListLog(logStr);

		//���� ������Camera �˻� Log ����
		MandoInspLog.dLensOffset[0] = Task.m_dataOffset_x[1];
		MandoInspLog.dLensOffset[1] = Task.m_dataOffset_y[1];
		MandoInspLog.dLensOffset[2] = Task.m_dataOffset_th[1];


		iRtnFunction = 41850;
		//if (Dio.CamLaserSlinderMove(false, true))
		//{
		//	logStr.Format("Laser/Cam �Ǹ��� ���� �Ϸ�[%d]", iLensStep);
		//	//putListLog(logStr);
		//	Task.LensTaskTime = myTimer(true);
		//	iRtnFunction = 41850;
		//}
		//else
		//{
		//	logStr.Format("Laser/Cam �Ǹ��� ���� ����[%d]", iLensStep);
		//	errMsg2(Task.AutoFlag, logStr);
		//	iRtnFunction = -41800;
		//}

		// 20141217 LHC - sensor_holder�� align ���� ���� 0�� ���� �����ϰԲ�
		//�迵ȣ 20150602 FraneeGrabber  ���� �ӽ� Pass
		//if(Task.m_dataOffset_x[1] == 0 || Task.m_dataOffset_y[1] == 0 || Task.m_dataOffset_th[1] == 0)
		//{
		//	sLangChange.LoadStringA(IDS_STRING963);	//PCB Sensor_Holder�� Align ������ �̻� �߻� : X:%.03f, Y:%.03f, ��:%.04f [%d]
		//	logStr.Format(_T("		") + sLangChange, Task.m_dataOffset_x[1], Task.m_dataOffset_y[1],Task.m_dataOffset_th[1], iLensStep);
		//	putListLog(logStr);
		//	errMsg2(Task.AutoFlag,logStr);
		//	iRtnFunction = -41800;
		//	break;
		//}

		//-- ADO DB ����
		//g_ADOData.func_AATaskToRecordAlign(Task.ChipID, MandoInspLog.dLensOffset[0], MandoInspLog.dLensOffset[1], MandoInspLog.dLensOffset[2]);

		break;
	case 41850:
		iRtnFunction = 41870;
		//if (Dio.CamLaserSlinderMove(false, true))
		//{
		//	logStr.Format("Laser/Cam �Ǹ��� ���� �Ϸ�[%d]", iLensStep);
		//	//putListLog(logStr);
		//	Task.LensTaskTime = myTimer(true);
		//	iRtnFunction = 41870;
		//}
		//else
		//{
		//	logStr.Format("Laser/Cam �Ǹ��� ���� ����[%d]", iLensStep);
		//	errMsg2(Task.AutoFlag, logStr);
		//	iRtnFunction = -41850;
		//}
		break;
	case 41870:
		iRtnFunction = 41900;
		//if (Dio.CamLaserSlinderCheck(false, false))
		//{
		//	logStr.Format("Laser/Cam �Ǹ��� ���� Ȯ�� �Ϸ�[%d]", iLensStep);
		//	putListLog(logStr);
		//	iRtnFunction = 41900;
		//}
		//else if ((myTimer(true) - Task.LensTaskTime) > 10000)//IO_DELAY_TIME)
		//{
		//	logStr.Format("Laser/Cam �Ǹ��� ���� �ð� �ʰ�[%d]", iLensStep);
		//	errMsg2(Task.AutoFlag, logStr);
		//	iRtnFunction = -41870;
		//}
		break;
	case 41900://! Lens-Z�� ��� ��ġ �̵�
		if (motor.LENS_Z_Motor_Move(Wait_Pos))
		{
			Task.LensTaskTime = myTimer(true);
			iRtnFunction = 41950;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING739);	//Lens_Z�� �����ġ �̵� ����
			logStr.Format(sLangChange + _T("[%d]"), iLensStep);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -41900;
		}
		break;
	case 41950:
		iRtnFunction = 42000;

		
		break;
	case 42000: // ���� ��ġ �̵�
		if (motor.Lens_Motor_MoveXY(Bonding_Pos, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK]))
		{
			sLangChange.LoadStringA(IDS_STRING659);	//Lens Align �̵� [%d]
			logStr.Format(sLangChange, iRtnFunction);
			putListLog(logStr);
			iRtnFunction = 42200;
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING664);	//Lens Bonding ��ġ �̵� ����[%d]
			logStr.Format(sLangChange, iRtnFunction);
			errMsg2(Task.AutoFlag, logStr);
			iRtnFunction = -42000;
		}
		break;
	case 42200:
		//�׽� ��  , lens ������ġ���� z�� �����ġ���� pcb ������ġ�� �����ͼ� z�� �� ���÷����ߵ�
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
		sLangChange.LoadStringA(IDS_STRING628);	//Laser ���� ���� Thread Step ��ȣ ������ .
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

bool	CAABonderDlg::func_inspVisual()	// Distortion,Blemish,Blotch,Stain,Defect Pixel,Optical Centering,Distortion Centering�˻� �ܺ�DLL ���
{
	return true;
}

void CAABonderDlg::OnBnClickedButtonProcomp()
{
	if(Task.AutoFlag==1 )
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 3000, M_COLOR_RED);
	}
	else if(Task.AutoFlag==0 || Task.AutoFlag==2)
	{	
		if(sysData.m_iProductComp == 0)
		{
			sLangChange.LoadStringA(IDS_STRING1308);	//����ǰ ���� ���� �Ͻðڽ��ϱ�?
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
	strTemp.Format("ID�� �Է��ϼ���.");
	//keyDlg.m_strKeyPad = strTemp;
	keyDlg.m_strKeyPad.Format("%s", Task.ChipID);
	if (keyDlg.DoModal()==IDOK)
	{
		strTemp = keyDlg.GetstrKeypad();
		int strLength = strTemp.GetLength();
		if(strLength<1)
		{
			strTemp.Format("ID���� �������Դϴ�.");
			errMsg2(Task.AutoFlag, strTemp);
			return;
		}
		Task.m_bPBStart = 1;	//���ڵ� ���������� ���� ������.		 
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
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
