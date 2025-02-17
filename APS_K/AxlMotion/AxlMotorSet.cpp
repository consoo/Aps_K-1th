#include "StdAfx.h"
#include "AxlMotorSet.h"

extern bool bThreadTaskReadyRun;

 


CAxlMotorSet::CAxlMotorSet(void)
{
	int i = 0;
	for ( i = 0; i < MAX_MOTOR_NO; i++ )
	{
		m_bOrgFlag[i] = false;
	}

	bAxdConnect = true;
	//! Motor ���� ���� ���� ������ ���� ��� (Motion Controller Library���� �����ϴ� ���)
	m_sMotionSettingFilePath = _T("C:/Program Files/EzSoftware UC/EzSoftware/MotionSetting.mot");
	//m_sMotionSettingFilePath = "C:\\Program Files\\EzSoftware RM\\EzSoftware\\MotionSetting.mot";
}

CAxlMotorSet::~CAxlMotorSet(void)
{
	Axl_Close();
}




bool CAxlMotorSet::InDIO(int moduleNo, int &inVal)
{
	DWORD ret = 0;
	DWORD val = 0x00;

#ifdef ON_LINE_MOTOR
	ret = AxdiReadInportDword(moduleNo, 0, &val);
#endif

	inVal = ~val & 0xFFFFFFFF;

	if(ret)
		return false;

	return true;
}


bool CAxlMotorSet::OutDIO(int moduleNo, int no)
{
	DWORD ret = 0;
	if(moduleNo == 1)
	{
		moduleNo = 2;
	}

#ifdef ON_LINE_MOTOR
	ret = AxdoWriteOutportDword(moduleNo+1, 0, ~no & 0xFFFFFFFF);
#endif

	if(ret)
	{
		Sleep(5);
		checkMessage();

		ret = AxdoWriteOutportDword(moduleNo+1, 0, ~no & 0xFFFFFFFF);

		if(ret)
		{
			CString tmpStr;
			sTempLang.LoadStringA(IDS_STRING888);
			sLangChange.Format(sTempLang, ret, no);
			tmpStr.Format(sLangChange);			//OUTPUT ��ȣ ���� ���� 1-2
			errMsg2(Task.AutoFlag, tmpStr);

			return false;
		}
	}

	curOutDioByte[moduleNo][0] = no & 0xFF;
	curOutDioByte[moduleNo][1] = (no>>8) & 0xFF;
	curOutDioByte[moduleNo][2] = (no>>16) & 0xFF;
	curOutDioByte[moduleNo][3] = (no>>24) & 0xFF;

	return true;
}


bool CAxlMotorSet::OutDIO(int moduleNo, int chNo, int no)
{

	if(moduleNo<0 || moduleNo == 1 || moduleNo>=3)
	{
		CString tmpStr;
		sTempLang.LoadStringA(IDS_STRING497);
		sLangChange.Format(sTempLang, moduleNo);
		tmpStr.Format(sLangChange);	//DIO ��� ��� ��ȣ[%d]�� �������Դϴ�.
		errMsg2(Task.AutoFlag, tmpStr);

		return false;
	}

	if(chNo<0 || chNo>=4)
	{
		CString tmpStr;
		sTempLang.LoadStringA(IDS_STRING498);
		sLangChange.Format(sTempLang, moduleNo);
		tmpStr.Format(sLangChange);	//DIO ��� ü�� ��ȣ[%d]�� �������Դϴ�.
		errMsg2(Task.AutoFlag, tmpStr);

		return false;
	}

	DWORD ret = 0;

#ifdef ON_LINE_MOTOR
	ret = AxdoWriteOutportByte(moduleNo+1, chNo, (~no) & 0xFF);

	if(ret)
	{
		Sleep(5);
		checkMessage();

		ret = AxdoWriteOutportByte(moduleNo+1, chNo, (~no) & 0xFF);

		if(ret)
		{
			CString tmpStr;
			sTempLang.LoadStringA(IDS_STRING889);
			sLangChange.Format(sTempLang, ret, chNo, no);
			tmpStr.Format(sLangChange);	//OUTPUT ��ȣ ���� ���� 2-2
			errMsg2(Task.AutoFlag, tmpStr);

			return false;
		}
	}
#endif

	curOutDioByte[moduleNo][chNo] = no & 0xFF;

	return true;
}

bool CAxlMotorSet::GetOutputIO(int moduleNo, int& val)
{
	val = 0;
	DWORD ret = 0;
	DWORD dwValue = 0;

#ifdef ON_LINE_MOTOR
	ret = AxdoReadOutportDword(moduleNo+1, 0, &dwValue);
#endif

	val = ~dwValue & 0xFFFFFFFF;

	if(ret)
		return false;

	return true;
}


bool CAxlMotorSet::Axl_Init()
{
	int i = 0;
	for ( i = 0; i < MAX_MOTOR_NO; i++ )
		m_bOrgFlag[i] = false;

	DWORD dwRet = 0;

#ifdef	ON_LINE_MOTOR
	dwRet = AxlOpen(0);		// 0 : �����Ⱚ..
	if ( dwRet != AXT_RT_SUCCESS )
	{
		_tcscpy_s(m_szLogMsg, _T("��� ���� �ʱ�ȭ�� ���� �Ͽ����ϴ�."));
		LogSave(m_szLogMsg);
		return false;
	}

	//! ��ϵ� AXL ��밡�� ������ ������ Ȯ���Ѵ�.
	long lBoardCounts = 0;
	AxlGetBoardCount(&lBoardCounts);
	if ( lBoardCounts <1)// <= 0 )
	{
		_tcscpy_s(m_szLogMsg, _T("��� ���� �ν��� ���� �Ͽ����ϴ�."));
		LogSave(m_szLogMsg);
		return false;
	}

	TCHAR* pszPath = NULL;
	pszPath = m_sMotionSettingFilePath.GetBuffer( m_sMotionSettingFilePath.GetLength() + 0 );

	if ( IsExist_Path(m_sMotionSettingFilePath) == true )
	{
		//! ��� ���� �ʱ� ���� ������ ���Ͽ��� �о� �����Ѵ�. 
		if ( AxmMotLoadParaAll(pszPath) != AXT_RT_SUCCESS )
		{
			_tcscpy_s(m_szLogMsg, _T("��� �Ķ���� ������ �����ϴ�."));
			LogSave(m_szLogMsg);
			return false;
		}
	}
	else
	{
		//! ��� ���� �ʱ� ���� ���Ͽ� ���� �Ѵ�. 
		AxmMotSaveParaAll(pszPath);    
	}

	m_sMotionSettingFilePath.ReleaseBuffer(); 
#endif

	//! �ʱ� ����
	AmpDisableAll();
	int iRet_Axis = Axl_Axisconfig();
	
	int iRet_DIO  = Axl_DIO_Config();
	bool bRet_DIO_Reset = Axl_DIO_Reset();

	return true;
}




//! ��� DIO ��⿡ �ʱⰪ�� �Է��Ѵ�. 
bool CAxlMotorSet::Axl_DIO_Reset()
{
	//! ���̺귯���� �ʱ�ȭ �Ǿ� �ִ��� Ȯ���Ѵ�.

#ifdef	ON_LINE_MOTOR
	//! DIO ����� �ִ��� Ȯ���Ѵ�.
	DWORD dwStatus = STATUS_NOTEXIST;
	AxdInfoIsDIOModule(&dwStatus);
	if(dwStatus != STATUS_EXIST)
	{
		_tcscpy_s(m_szLogMsg, _T("IO ����� ��� �ν��� ���� �Ͽ����ϴ�."));
		LogSave(m_szLogMsg);
		return false;
	}

	// DIO����� ������ Ȯ��
	long lModuleCounts = 0, lCount;
	AxdInfoGetModuleCount(&lModuleCounts);

	// DIO����� ����� ��ȣ������ Low�� �ʱ� �����Ѵ�.
	for( int ModuleNo = 0; ModuleNo < lModuleCounts; ModuleNo++ )
	{
		lCount = 0;
		AxdInfoGetInputCount(ModuleNo, &lCount);
		if(lCount > 0)
		{
			AxdiLevelSetInportDword(ModuleNo, 0, 0); 
		}

		lCount = 0;
		AxdInfoGetOutputCount(ModuleNo, &lCount);
		if(lCount > 0)
		{
			AxdoLevelSetOutportDword(ModuleNo, 0, 0);
		}
	}
#endif

	return true;
}


//! ��� ��ü�� Axis�� �ʱ� ������ �����Ѵ�. 
//! [���� ����] 
//!    1) �ݵ��, 'Axl_Init' �Լ��� 'AxmMotLoadParaAll' �Լ� ȣ�� �Ŀ� �� �Լ��� ȣ���Ͽ��� �Ѵ�. 
//!    2) PCI-R1604�� A4N, A5N��������̺��� ���(PM����) Limit/Home/Servo On/Inposition�� Level�� ���� �Ͻ� �� ���� High�Ǵ� Low�� ������ �Ǿ��ֽ��ϴ�. 
//!		  Limit ������ A/B���� �ٲٰ��� �Ҷ��� ��������̺��� �Ķ���͸� �ٲپ�� �մϴ�.
int CAxlMotorSet::Axl_Axisconfig() 
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

#ifdef	ON_LINE_MOTOR

	DWORD Status = 0;
	DWORD dwReturn = 0;

	//! ���峻�� ���ͷ�Ʈ ����� ���� ��Ų��. (RTEX ���� ����)
	AxlInterruptDisable();

	//! �ý��ۿ� ������ �� ������ Ȯ��
	long lAxisCount = 0;
	AxmInfoGetAxisCount(&lAxisCount);
	if ( lAxisCount != MAX_MOTOR_NO+1)//�ӽ�
	{
		sLangChange.LoadStringA(IDS_STRING1231);	//���� ����̺� ���� ����ġ.
		_stprintf_s(m_szLogMsg, sLangChange);
		pFrame->putListLog(m_szLogMsg);
		return -4;
	}

	int    iFailed = -1;

	for(int i=0; i<MAX_MOTOR_NO; i++)
	{

		dwReturn = AxmInfoIsInvalidAxisNo(i);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sTempLang.LoadStringA(IDS_STRING161);
			sLangChange.Format(sTempLang, MotorName[i]);
			_stprintf_s(m_szLogMsg, sLangChange);
			pFrame->putListLog(m_szLogMsg);
			iFailed = i;
			continue;
		}


		dwReturn = AxmMotSetMoveUnitPerPulse(i, 1, 1);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sTempLang.LoadStringA(IDS_STRING162);
			sLangChange.Format(sTempLang, MotorName[i]);
			_stprintf_s(m_szLogMsg, sLangChange);
			pFrame->putListLog(m_szLogMsg);
			iFailed = i;	
		}


		dwReturn = AxmMotSetMinVel(i, 1);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sTempLang.LoadStringA(IDS_STRING163);
			sLangChange.Format(sTempLang, MotorName[i]);
			_stprintf_s(m_szLogMsg, sLangChange);
			pFrame->putListLog(m_szLogMsg);
			iFailed = i;			
		}

#ifdef MOTOR_ACC_TYPE_SEC
		dwReturn = AxmMotSetAccelUnit(i, SEC);	//KYH UNIT_SEC2
#else
		dwReturn = AxmMotSetAccelUnit(i, UNIT_SEC2);	//KYH UNIT_SEC2
#endif
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sTempLang.LoadStringA(IDS_STRING160);
			sLangChange.Format(sTempLang, MotorName[i]);
			_stprintf_s(m_szLogMsg, sLangChange);
			pFrame->putListLog(m_szLogMsg);
			iFailed = i;			
		}

		dwReturn = AxmMotSetProfileMode(i, ASYM_S_CURVE_MODE);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sTempLang.LoadStringA(IDS_STRING158);
			sLangChange.Format(sTempLang, MotorName[i]);
			_stprintf_s(m_szLogMsg, sLangChange);
			pFrame->putListLog(m_szLogMsg);
			iFailed = i;
		}

		dwReturn = AxmMotSetAbsRelMode(i, POS_ABS_MODE);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sTempLang.LoadStringA(IDS_STRING166);
			sLangChange.Format(sTempLang, MotorName[i]);
			_stprintf_s(m_szLogMsg, sLangChange);
			pFrame->putListLog(m_szLogMsg);
			iFailed = i;			
		}

		DWORD	dwHomLevel;
		if (i == Motor_PCB_TH || i == Motor_PCB_Xt || i == Motor_PCB_Yt || i == Motor_Lens_Yt ||i == Motor_Lens_Xt)
		{
			dwReturn = AxmHomeSetSignalLevel(i, HIGH);
		}
		else
		{
			dwReturn = AxmHomeSetSignalLevel(i, LOW);//����
		}
		
		dwReturn = AxmHomeGetSignalLevel(i, &dwHomLevel);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sTempLang.LoadStringA(IDS_STRING155);
			sLangChange.Format(sTempLang, MotorName[i]);
			_stprintf_s(m_szLogMsg, sLangChange);
			pFrame->putListLog(m_szLogMsg);
			iFailed = i;			
		}

		DWORD	dwMaxVel; 
		dwMaxVel = AxmMotSetMaxVel(i, MOTOR_SPEED_MAX[i] * sysData.fMotorResol[i]);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			sTempLang.LoadStringA(IDS_STRING168);
			sLangChange.Format(sTempLang, MotorName[i]);
			_stprintf_s(m_szLogMsg, sLangChange);
			pFrame->putListLog(m_szLogMsg);
			iFailed = i;			
		}

		//! OneHighLowHigh      1�޽� ���, PULSE(Active High)  +����(DIR=Low)  / -����(DIR=High)
		//! OneHighHighLow      1�޽� ���, PULSE(Active High)  +����(DIR=High) / -����(DIR=Low)
		//! OneLowLowHigh       1�޽� ���, PULSE(Active Low)   +����(DIR=Low)  / -����(DIR=High)
		//! OneLowHighLow       1�޽� ���, PULSE(Active Low)   +����(DIR=High) / -����(DIR=Low)
		//! TwoCcwCwHigh        2�޽� ���, PULSE(CW:-����)  DIR(CCW:+����), Active High
		//! TwoCcwCwLow         2�޽� ���, PULSE(CW:-����)  DIR(CCW:+����), Active Low
		//! TwoCwCcwHigh        2�޽� ���, PULSE(CW:+����)  DIR(CCW:-����), Active High
		//! TwoCwCcwLow         2�޽� ���, PULSE(CW:+����)  DIR(CCW:-����), Active Low		

		if( MOTOR_TYPE[i] == STEPING)				// Step ���� ����̺�� ��� ���, ����Ʈ ���� Ʋ��.
		{

			dwReturn = AxmSignalSetInpos(i, UNUSED);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sTempLang.LoadStringA(IDS_STRING156);
				sLangChange.Format(sTempLang, MotorName[i]);
				_stprintf_s(m_szLogMsg, sLangChange);
				pFrame->putListLog(m_szLogMsg);
				iFailed = i;
			}
	
			dwReturn = AxmMotSetPulseOutMethod(i, TwoCcwCwHigh);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sTempLang.LoadStringA(IDS_STRING154);
				sLangChange.Format(sTempLang, MotorName[i]);
				_stprintf_s(m_szLogMsg, sLangChange);
				pFrame->putListLog(m_szLogMsg);
				iFailed = i;
			}


			dwReturn = AxmSignalSetStop(i, EMERGENCY_STOP, HIGH);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sTempLang.LoadStringA(IDS_STRING164);
				sLangChange.Format(sTempLang, MotorName[i]);
				_stprintf_s(m_szLogMsg, sLangChange);
				pFrame->putListLog(m_szLogMsg);
				iFailed = i;			
			}
			if (i == Motor_PCB_Z)
			{
				dwReturn = AxmSignalSetLimit(i, EMERGENCY_STOP, HIGH, HIGH);
			}
			else
			{
				dwReturn = AxmSignalSetLimit(i, EMERGENCY_STOP, LOW, LOW);//low ����
			}
			
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sTempLang.LoadStringA(IDS_STRING157);
				sLangChange.Format(sTempLang, MotorName[i]);
				_stprintf_s(m_szLogMsg, sLangChange);
				pFrame->putListLog(m_szLogMsg);
				iFailed = i;
			}

			dwReturn = AxmMotSetEncInputMethod(i, ObverseSqr4Mode);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sTempLang.LoadStringA(IDS_STRING153);
				sLangChange.Format(sTempLang, MotorName[i]);
				_stprintf_s(m_szLogMsg, sLangChange);
				pFrame->putListLog(m_szLogMsg);
				iFailed = i;			
			}


			dwReturn = AxmSignalSetServoOnLevel(i, HIGH);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sTempLang.LoadStringA(IDS_STRING159);
				sLangChange.Format(sTempLang, MotorName[i]);
				_stprintf_s(m_szLogMsg, sLangChange);
				pFrame->putListLog(m_szLogMsg);
				iFailed = i;
			}
		}
		else
		{
			dwReturn = AxmSignalSetInpos(i, HIGH);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sTempLang.LoadStringA(IDS_STRING156);
				sLangChange.Format(sTempLang, MotorName[i]);
				_stprintf_s(m_szLogMsg, sLangChange);
				pFrame->putListLog(m_szLogMsg);
				iFailed = i;
			}

			dwReturn = AxmSignalSetStop(i, EMERGENCY_STOP, LOW);
			if ( dwReturn != AXT_RT_SUCCESS )
			{

				sTempLang.LoadStringA(IDS_STRING164);
				sLangChange.Format(sTempLang, MotorName[i]);
				_stprintf_s(m_szLogMsg, sLangChange);
				pFrame->putListLog(m_szLogMsg);
				iFailed = i;			
			}

			dwReturn = AxmSignalSetLimit(i, EMERGENCY_STOP, HIGH, HIGH);
			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sTempLang.LoadStringA(IDS_STRING156);
				sLangChange.Format(sTempLang, MotorName[i]);
				_stprintf_s(m_szLogMsg, sLangChange);
				pFrame->putListLog(m_szLogMsg);
				iFailed = i;
			}
		}

		dwReturn = AxmSignalSetServoAlarm(i, HIGH);
		if ( dwReturn != AXT_RT_SUCCESS )
		{
			iFailed = i;			
		}
	}

	if ( iFailed >= 0 )
	{
		return (-1000 - iFailed);
	}

#endif

	return 1;
}

//! ��� ��ü�� DIO ������ �ʱ� ������ �����Ѵ�. 
//! [���� ����] �ݵ��, 'Axl_Init' �Լ��� 'AxmMotLoadParaAll' �Լ� ȣ�� �Ŀ� �� �Լ��� ȣ���Ͽ��� �Ѵ�. 
int CAxlMotorSet::Axl_DIO_Config()
{
#ifdef	ON_LINE_MOTOR
	DWORD Status = 0;
	DWORD dwReturn = 0;

	int i = 0, j = 0;
	long lModuleCounts = 0, lInputCounts = 0, lOutputCounts = 0;
	int ModuleNo = 0;

	DWORD uLevel = HIGH, dwStatus;

	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	//DWORD dwStatus2;
	//DWORD Code = AxlOpenNoReset(7);
	//Code = AxdInfoIsDIOModule(&dwStatus2);
	//! ��ü �ý��ۿ� �����Ǿ� �ִ� DIO����� ������ Ȯ��
	AxdInfoGetModuleCount(&lModuleCounts);

	if(lModuleCounts != 2)
	{
		bAxdConnect = false;
		sLangChange.LoadStringA(IDS_STRING194);
		sTempLang.Format(sLangChange, lModuleCounts);
		_stprintf_s(m_szLogMsg, sTempLang);
		pFrame->putListLog(m_szLogMsg);
	}


	//! �Է� ���..
	AxdInfoGetInputCount(0, &lInputCounts);
	if ( lInputCounts == 32 )
	{
		for ( j=0; j<lInputCounts; j++ )
			AxdiLevelSetInportBit(0, j, HIGH);
	}
	else
	{
		bAxdConnect = false;
		sLangChange.LoadStringA(IDS_STRING1489);	//[IO ��� ] �Է� ��� Ȯ�� ������ [%d EA] .
		sprintf_s(m_szLogMsg, sLangChange, lInputCounts);
		pFrame->putListLog(m_szLogMsg);
		return 0;
	}


	//! ��� ���..
	AxdInfoGetOutputCount(1, &lOutputCounts);
	if ( lOutputCounts == 32 )
	{
		for ( j = 0; j < lOutputCounts; j++ )
			AxdoLevelSetOutportBit(1, j, HIGH);
	}
	else
	{
		bAxdConnect = false;
		sLangChange.LoadStringA(IDS_STRING195);
		_stprintf_s(m_szLogMsg, sLangChange, lOutputCounts);	//"[IO ��� ] ��� ��� Ȯ�� ������ [%d EA] ."
		pFrame->putListLog(m_szLogMsg);
		return 0;
	}
#endif

	return 1;
}

void CAxlMotorSet::Axl_Close()
{
	int i = 0;

#ifdef ON_LINE_MOTOR

	//! ��� Motor�� �����Ѵ�. 
	for(i=0; i<MAX_MOTOR_NO; i++)
	{
		AxmMoveSStop(i);		
	}

	TCHAR* pszPath = NULL;
	pszPath = m_sMotionSettingFilePath.GetBuffer( m_sMotionSettingFilePath.GetLength() + 1 );
	//! ��� ���� ���� �������� ���Ͽ� ���� �Ѵ�. 
	AxmMotSaveParaAll(pszPath);
	m_sMotionSettingFilePath.ReleaseBuffer();

	AxlClose();

#endif

	for ( i = 0; i < MAX_MOTOR_NO; i++ )
	{
		m_bOrgFlag[i] = false;
	}
}

int CAxlMotorSet::Start_Home_Search(int iAxis)
{//! Home �˻� �۾� ����
	m_criHomeProc.Lock();

	DWORD dwReturn = 0;
	DWORD level;

	AxmSignalIsServoOn(iAxis, &level);
	if ( level != ENABLE )
	{
		AxmSignalServoOn(iAxis, ENABLE);	

		for(int i=0; i<20; i++)
		{
			Sleep(5);
			CheckHandle();
		}

		AxmSignalIsServoOn(iAxis, &level);
		if ( level != ENABLE )
		{
			sLangChange.LoadStringA(IDS_STRING1235);
			sMsg.Format(_T("[ %s ] ") + sLangChange, MotorName[iAxis]);		//���� ���� �˻� ���� ON ����.!!
			errMsg2(Task.AutoFlag, sMsg);
			m_criHomeProc.Unlock();
			return -3;
		}
	}


	//! +++++++++++++++++++++++++++++++++++++++++++++++++++++
	//!    DIR_CCW    00h     (-) ����
	//!    DIR_CW     01h     (+) ����
	//! [lHmsig]
	//!    PosEndLimit  00h   +Elm(End limit) +���� ����Ʈ ���� ��ȣ
	//!    NegEndLimit  01h   -Elm(End limit) -���� ����Ʈ ���� ��ȣ
	//!    HomeSensor   04h   IN0(ORG)  ���� ���� ��ȣ
	//! [dwZphas]
	//!    DISABLE      00h   Z�� ���� ������� ����
	//!    + ����       01h   Z�� ���� +������
	//!    - ����       02h   Z�� ���� -������
	
	long    lHmDir = DIR_CW;			//! �����˻��� �ʱ⿡ ������ ���⼳��
	DWORD   lHmsig =  HomeSensor;		//! ���� �˻��� ��� �� ��ȣ����
	DWORD   dwZphas = DISABLE;			//! 1�� �����˻� �Ϸ� �� ���ڴ� Z�� ���� ���� ����
	double  dwHClrTim = 2000.0;			//! �����˻� �Ϸ� �� ������ġ�� Encoder��ġ�� Clear�ϱ� ���� ����ϴ� �ð����� [mSec����]
	double  dwHOffset = 0.0;			//! �����˻��� �Ϸ�� �� �ⱸ �������� �̵� �� ���� �缳�� �� ��ġ
	
	lHmDir = MOTOR_HOME_DIR[iAxis];
	lHmsig = MOTOR_HOME_SIG[iAxis];

	dwReturn = AxmHomeSetMethod(iAxis, lHmDir, lHmsig, dwZphas, dwHClrTim, dwHOffset);
	if ( dwReturn != AXT_RT_SUCCESS )
	{
		sLangChange.LoadStringA(IDS_STRING1236);
		sMsg.Format(_T("[ %s ] ") + sLangChange, MotorName[iAxis]);		//���� ���� �˻� ���� ����
		errMsg2(Task.AutoFlag, sMsg);
		m_criHomeProc.Unlock();
		return -4;
	}


	dwReturn = SetHomeSearchSpeed(iAxis);	//���� �˻� �ӵ� ����
	if ( dwReturn != AXT_RT_SUCCESS )
	{
		sLangChange.LoadStringA(IDS_STRING1236);		//���� ���� �ӵ� ���� ����
		sMsg.Format(_T("[ %s ] ") + sLangChange, MotorName[iAxis]);errMsg2(Task.AutoFlag, sMsg);
		errMsg2(Task.AutoFlag, sMsg);
		m_criHomeProc.Unlock();
		return -6;
	}

	dwReturn = AxmHomeSetStart(iAxis);
	if ( dwReturn != AXT_RT_SUCCESS )
	{
		//! Error Code Ȯ��
		CString sError = _T("");
		GetResult_Home_Search(iAxis, sError);
		errMsg2(Task.AutoFlag, sMsg);
		m_criHomeProc.Unlock();
		return -7;
	}

	m_criHomeProc.Unlock();
	return 1;
}


DWORD CAxlMotorSet::SetHomeSearchSpeed(int iAxis)
 {//! ���� �˻��� ���Ǵ� �ӵ� ����
 /*
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // ������ ������ �����ϰ� �˻��ϱ� ���� ���� �ܰ��� �������� �����Ѵ�. �̶� �� ���ǿ� ��� �� �ӵ��� �����Ѵ�. 
 // �� �ӵ����� �������� ���� �����˻� �ð���, �����˻� ���е��� �����ȴ�. 
 // �� ���Ǻ� �ӵ����� ������ �ٲ㰡�鼭 �� ���� �����˻� �ӵ��� �����ϸ� �ȴ�. 
 // (�ڼ��� ������ AxmMotSaveParaAll ���� �κ� ����)
 // �����˻��� ���� �ӵ��� �����ϴ� �Լ�
 // [dVelFirst]- 1�������ӵ�   [dVelSecond]-�����ļӵ�   [dVelThird]- ������ �ӵ�  [dvelLast]- index�˻��� �����ϰ� �˻��ϱ�����. 
 // [dAccFirst]- 1���������ӵ� [dAccSecond]-�����İ��ӵ� 
 */
	DWORD dwReturn = 0;
	double dVelFirst = 20000.;		
	double dVelSecond = 10000.;
	double dVelThird = 4000.;
	double dvelLast = 500.;	

	double dAccFirst = 0.1;		
	double dAccSecond = 0.1;


	double dAccCalFirst = 0.0, dAccCalSecond = 0.0;
//! ���� �ӵ� ���� �Ұ�.
#ifndef ORG_INITIAL_SETTING	
		switch(iAxis)
	{
	case Motor_PCB_X:			//! PCB ���� X��
		dVelFirst = 40000.;		dVelSecond = 20000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_PCB_Y:			//! PCB ���� Y��
		dVelFirst = 50000.;		dVelSecond = 20000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_PCB_TH:			//! PCB ���� THETA��
		dVelFirst = 5000.;		dVelSecond = 4000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_PCB_Xt:			//! PCB ���� TX��
		dVelFirst = 5000;		dVelSecond = 3000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_PCB_Yt:			//! PCB ���� TY��
		dVelFirst = 5000;		dVelSecond = 3000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_PCB_Z:			//! PCB Z��
		dVelFirst = 10000.;		dVelSecond = 3000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_Lens_X:			//! LENS ���� X��
		dVelFirst = 40000.;		dVelSecond = 10000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_Lens_Y:			//! LENS ���� Y��
		dVelFirst = 50000.;		dVelSecond = 10000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_Lens_Z:			//! LENS ���� Z��
		dVelFirst = 10000.;		dVelSecond = 3000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_Lens_Xt:			//! LENS ���� TX��
		dVelFirst = 5000;		dVelSecond = 3000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_Lens_Yt:			//! LENS ���� TY��
		dVelFirst = 5000;		dVelSecond = 3000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	default://��ȿ���� �ʴ� ���� ��ȣ
		dwReturn = AXT_RT_INVALID_MODULE_POS;
		break;	
	}

#else		//!!!!!!!!! �ʱ� Setting ��, ���� ���� �˻� �ӵ� �����.
	switch(iAxis)
	{
	case Motor_PCB_X:			//! PCB ���� X��
		dVelFirst = 10000.;		dVelSecond = 5000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_PCB_Y:			//! PCB ���� Y��
		dVelFirst = 10000.;		dVelSecond = 5000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_PCB_TH:			//! PCB ���� THETA��
		dVelFirst = 3000.;		dVelSecond = 2000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_PCB_Xt:			//! PCB ���� TX��
		dVelFirst = 2000;		dVelSecond = 1000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_PCB_Yt:			//! PCB ���� TY��
		dVelFirst = 2000;		dVelSecond = 1000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_PCB_Z:			//! DISPENSER Z��
		dVelFirst = 3000.;		dVelSecond = 2000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_Lens_X:			//! LENS ���� X��
		dVelFirst = 3000.;		dVelSecond = 2000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_Lens_Y:			//! LENS ���� Y��
		dVelFirst = 3000.;		dVelSecond = 2000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_Lens_Z:			//! LENS ���� Z��
		dVelFirst = 3000.;		dVelSecond = 2000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_Lens_Xt:			//! LENS ���� TX��
		dVelFirst = 500;		dVelSecond = 400.;		dVelThird = 200;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;

	case Motor_Lens_Yt:			//! LENS ���� TY��
		dVelFirst = 2000;		dVelSecond = 1000.;		dVelThird = 500;		dvelLast = 50.;		dAccFirst = 0.1;	dAccSecond = 0.1;
		break;


	default://��ȿ���� �ʴ� ���� ��ȣ
		dwReturn = AXT_RT_INVALID_MODULE_POS;
		break;	
	}
#endif
	
	
#ifdef MOTOR_ACC_TYPE_SEC
	dAccCalFirst = dAccFirst;
	dAccCalSecond = dAccSecond;
#else
	dAccCalFirst = dAccFirst * (9.8 * 1000 *  sysData.fMotorResol[iAxis]);
	dAccCalSecond = dAccFirst * (9.8 * 1000 *  sysData.fMotorResol[iAxis]);		//! ���� 
#endif	

	if( dwReturn == 0 )
	{
		dwReturn = AxmHomeSetVel(iAxis, dVelFirst, dVelSecond, dVelThird, dvelLast, dAccCalFirst, dAccCalSecond);
	}
	
	return dwReturn;
}

//! Home �˻� �۾� ���̸� false�̰�, �׷��� ������ true�� ��ȯ
int CAxlMotorSet::IsCompleted_Home_Search(int axis)
{
	DWORD dwReturn = 0;
	DWORD uHomeResult;

	AxmHomeGetResult(axis, &uHomeResult);
	//! ���� �����˻��� �������� ���
	if ( uHomeResult == HOME_SEARCHING )		return 2;
	if ( uHomeResult == HOME_SUCCESS )			return 1; 
	if ( uHomeResult == HOME_ERR_AMP_FAULT)		return -1;
	if ( uHomeResult == HOME_ERR_NOT_DETECT)	return -2;
	if ( uHomeResult == HOME_ERR_NEG_LIMIT)		return -3;
	if ( uHomeResult == HOME_ERR_POS_LIMIT)		return -4;
	if ( uHomeResult == HOME_ERR_UNKNOWN)		return -5;

	return 0;
}

//! Home �˻� �߿� ������ ���� ��Ŵ
void CAxlMotorSet::Stop_Home_Search(int iAxis)
{//! Home �˻� �߿� ������ ���� ��Ŵ
	AxmMoveSStop(iAxis);
}


//! Home �˻� �۾� ���� ���� ����, ���� %�� ��ȯ�ȴ�. 
int CAxlMotorSet::GetRate_Home_Search(int axis)
{
	DWORD dwReturn = 0;
	DWORD uHomeStepNumber,uHomeMainStepNumber;

	//! �����˻� ���۵Ǹ� �������� Ȯ���� �� �ִ�. �����˻��� �Ϸ�Ǹ� �������ο� ������� 100�� ��ȯ�ϰ� �ȴ�.
	//! uHomeMainStepNumber�� ��Ʈ���� ��� ������,�����̺갡 �����ϴ��� Ȯ���� �� �̿��Ѵ�.
	//!  (�������� ������ ���� 0�� ����, �����̺굿���� ���� 10�� �����Ѵ�.)
	dwReturn = AxmHomeGetRate(axis, &uHomeMainStepNumber, &uHomeStepNumber);

	//! �����˻� ������(����: %)
	return uHomeStepNumber;
}

//! Home �˻� �۾� ��� ���ڿ��� �˾Ƴ���. 
bool CAxlMotorSet::GetResult_Home_Search(int axis, CString &sResult)
{
	DWORD uHomeResult;
	AxmHomeGetResult(axis, &uHomeResult);

	bool bRet = false;

	switch(uHomeResult)
	{
	case HOME_SUCCESS:
		{
			sResult = _T("���� �˻��� ���������� ���� ���� ���");
			bRet = true;
		}
		break;
	case HOME_SEARCHING:
		{
			sResult = _T("���� �����˻��� �������� ���");
		}
		break;
	case HOME_ERR_GNT_RANGE:
		{
			sResult = _T("��Ʈ�� �������� Master��� Slave���� �����˻� ����� ������ OffsetRange�� ��������");
		}
		break;
	case HOME_ERR_USER_BREAK:
		{
			sResult = _T("���� �˻��� ����ڰ� ���� ����� �������� ���");
		}
		break;
	case HOME_ERR_VELOCITY:
		{
			sResult = _T("���� �˻��� �ӵ� ���� �� �� �� ���� 0���� �۰ų� �������");
		}
		break;
	case HOME_ERR_AMP_FAULT:
		{
			sResult = _T("���� �˻��� ������ �˶��� �߻��� ���");
		}
		break;
	case HOME_ERR_NEG_LIMIT:
		{
			sResult = _T("(+)�������� �������� ���� �� (-)����Ʈ ������ ������ ���");
		}
		break;
	case HOME_ERR_POS_LIMIT:
		{
			sResult = _T("(-)�������� �������� ���� �� (+)����Ʈ ������ ������ ���");
		}
		break;
	case HOME_ERR_NOT_DETECT:
		{
			sResult = _T("���������� �������� �������");
		}
		break;
	case HOME_ERR_UNKNOWN:
		{
			sResult = _T("�˼����� ä��(��) ��ȣ(0 ~ (�ִ���� - 1))�� �����˻��� �����Ϸ� �������");
		}
		break;
	default:
		{
			sResult = _T("Error");
		}
		break;
	}//! switch(uHomeResult)

	return bRet;
}

//! Ư�� ��ġ (Limit ������ Home ����)���� �ڵ����� ���ߵ��� �Ͽ� ���͸� ������ �� ����Ѵ�.
//! ���� �࿡�� ������ ��ȣ�� �����Ѵ�
//! ����Ʈ ������ȣ�� ���� ������ȣ, ���� ����̹��� Z�� ��ȣ �Ǵ� ���� �Է� ��ȣ���� Edge�� ������ �� �ִ�.
//! �ַ� �����˻� ���� �Լ� ���� �� ����ϴ� �Լ��̴�.
//! [���� ����] 
//!		1) End Limit ������ ã�� ���, ��ȣ�� Active Level ���¸� �����Ѵ�.
//!     2) Emergency Stop���� ����� ���, �������� ���õǸ� ������ �ӵ��� ���� �������ϰԵȴ�.
bool CAxlMotorSet::MoveAxis_To_Sensor(int axis, strMove_Input_Signal_Search *pstrInput)
{
	//! �Է°� Ȯ��
	if ( pstrInput == NULL )
	{
		return false;
	}

	double dVel   = pstrInput->dVel;
	double dAccel = pstrInput->dAccel;

	long lDetectSignal = -1;
	long lSignalEdge = -1;
	long lSignalMethod = -1;

	//! lDetectSignal ����
	if ( pstrInput->bEndLimit_Negative == true )
	{
		lDetectSignal = NegEndLimit;
	}
	if ( pstrInput->bEndLimit_Positive == true )
	{
		lDetectSignal = PosEndLimit;
	}
	if ( pstrInput->bHomeSensor == true )
	{
		lDetectSignal = HomeSensor;
	}
	if ( pstrInput->bPhase_Encoder_Z == true )
	{
		lDetectSignal = EncodZPhase;
	}

	//! lSignalEdge ����
	if ( pstrInput->bEdge_Down == true )
	{
		lSignalEdge = SIGNAL_DOWN_EDGE;
	}
	if ( pstrInput->bEdge_Up == true )
	{
		lSignalEdge = SIGNAL_UP_EDGE;
	}

	//! lSignalMethod ����
	if ( pstrInput->bEmergency == true )
	{
		lSignalMethod = EMERGENCY_STOP;
	}
	if ( pstrInput->bSlowDown == true )
	{
		lSignalMethod = SLOWDOWN_STOP;
	}

	//! �Է°� Ȯ��
	if ( lDetectSignal == NegEndLimit )
		dVel = -1 * fabs(pstrInput->dVel);
	else
		dVel = fabs(pstrInput->dVel);


	if ( dAccel < 0. )
	{
		return false;
	}
	if ( lDetectSignal < 0 )
	{
		return false;
	}
	if ( lSignalEdge < 0 )
	{
		return false;
	}
	if ( lSignalMethod < 0 )
	{
		return false;
	}

	DWORD dwReturn = 0;

	//! ���� �࿡�� ������ ��ȣ�� ������ ������ ���͸� �����δ�. 
	dwReturn = AxmMoveSignalSearch(axis, dVel, dAccel, lDetectSignal, lSignalEdge, lSignalMethod);
	if ( dwReturn != AXT_RT_SUCCESS )
	{
		return false;
	}

	return true;
}

//! Added by LHW (2013/4/16)
//! GUI ȭ�鿡�� ������ ���̸�ŭ �����δ�. (��� �̵�)
//! GUI ȭ�鿡�� ���� ���Ǵ� ����̶� ���� �Լ��� �����. 
bool CAxlMotorSet::Move_From_GUI(int axis, int iID_GUI, CWnd* pParentWnd, bool bPlus)
{
	if ( pParentWnd == NULL )
	{
		return false;
	}
	if ( g_bMovingflag == true )
	{
		return false;
	}
	g_bMovingflag = true;

	CString sGetTemp = _T("");
	GetString_FromUI(iID_GUI, pParentWnd, sGetTemp);

	double dGetTemp = atof(sGetTemp);
	if ( dGetTemp > 3. )
	{
		dGetTemp = 3.;
	}
	else if (dGetTemp < -3 )
	{
		dGetTemp = -3;
	}

	if ( bPlus == false )
	{
		dGetTemp *= (-1.);
	}

	MoveAxis(axis, REL, dGetTemp, 4, 20);

	g_bMovingflag =false;
	
	return true;
}

//! CRC ��ȣ ����(�޽� ����� ��� ����⿡ ������ ���)
//! Ư�� �������� ��� ������ �Ϸ�� �������� �Ǵ� ����Ʈ������ ������ ��쿡 
//! �ܺο��� CRC(Current Remaining Clear)��ȣ�� �޾� �������� ������ �ִ� �ܿ� �޽��� �����־�� �ϴ� ��찡 �߻��Ѵ�.
//! [���� ����] CRC ��ȣ ���� ����� �޽� ����� ��������(PCI-N804/404, RTEX-PM)������ ����� �� �ִ� ����̴�.
bool CAxlMotorSet::ActCRC(int axis)
{
	DWORD dwReturn = 0;
	long  BoardNo = 0;
	long  ModulePos = 0;
	DWORD ModuleID = 0;

	DWORD uLevel, uMethod;


	//! ModuleID (Module�� ����)
	//! 'AXHS.h' ���� : 'C:\Program Files\EzSoftware RM\AXL(Library)\C, C++' �����ȿ� ���� (AXT_FUNC_MODULE)	
	//!      AXT_SMC_R1V04    = 0x09,   // CAMC-QI, 1 Axis, For RTEX SLAVE only	
	dwReturn = AxmInfoGetAxis(axis, &BoardNo, &ModulePos, &ModuleID);
	if ( dwReturn != AXT_RT_SUCCESS )
	{
		return false;
	}
	if ( ModuleID != AXT_SMC_R1V04 )
	{
		return false;
	}

	//! ���� �࿡ CRC ��ȣ ��� ���� ������ ��ȯ �Ѵ�.
	dwReturn = AxmCrcGetMaskLevel(axis, &uLevel, &uMethod);
	if ( dwReturn != AXT_RT_SUCCESS )
	{
		return false;
	}
	if ( uLevel == UNUSED )
	{
		//! CRC ��ȣ ������
		return false;
	}

	//! ���� �࿡ CRC ��ȣ�� ���������� ����� �߻���Ų��. (���� �ð� ���ȸ� �ۿ��Ŵ)
	AxmCrcSetOutput(axis, ENABLE);
	Sleep(10);
	AxmCrcSetOutput(axis, DISABLE);

	return true;
}

bool CAxlMotorSet::AmpDisable(int axis)
{
	m_bOrgFlag[axis] = false;
	double dDecel = 0.;

#ifdef ON_LINE_MOTOR

	AxmMoveStop(axis, dDecel);	
	AxmSignalServoOn(axis, DISABLE);

	
	if (axis != Motor_Lens_Xt && axis != Motor_Lens_Yt && axis != Motor_PCB_Xt && axis != Motor_PCB_Yt&& axis != Motor_PCB_Z&& axis != Motor_Lens_Z&& axis != Motor_PCB_TH){
		AxmSignalServoAlarmReset(axis, ENABLE);
	}
#endif

	return true;
}
bool CAxlMotorSet::AmpDisableAll()
{
	bool rtnVal = true;
	double dDecel = 0.;

	for(int i=0; i<MAX_MOTOR_NO; i++)
	{
		m_bOrgFlag[i] = false;

#ifdef ON_LINE_MOTOR
		AxmMoveStop(i, dDecel);		
		AxmSignalServoOn(i, DISABLE);
	
		if (i != Motor_Lens_Xt && i != Motor_Lens_Yt&& i != Motor_PCB_Xt && i != Motor_PCB_Yt&& i != Motor_PCB_Z&& i != Motor_Lens_Z&& i != Motor_PCB_TH)
			AxmSignalServoAlarmReset (i, ENABLE);
	}

	Sleep(500);

	for(int i=0; i<MAX_MOTOR_NO; i++)
	{
		if (i != Motor_Lens_Xt && i != Motor_Lens_Yt && i != Motor_PCB_Xt && i != Motor_PCB_Yt&& i != Motor_PCB_Z&& i != Motor_Lens_Z&& i != Motor_PCB_TH)
			AxmSignalServoAlarmReset (i, DISABLE);
#endif
	}

	return true;
}
bool CAxlMotorSet::AmpEnable(int axis)
{
	DWORD level = ENABLE;

#ifdef ON_LINE_MOTOR

	StopAxis(axis);

	//! �̹� Servo On �������� Ȯ���Ѵ�. 
	AxmSignalIsServoOn(axis, &level);
	if ( level == ENABLE )
	{
		return true;
	}
	m_bOrgFlag[axis] = false;
	AxmSignalServoOn(axis, ENABLE);	


	Sleep(100);

	AxmSignalIsServoOn(axis, &level);
	if ( level != ENABLE )
	{
		return false;
	}
#endif

	return true;
}
bool CAxlMotorSet::AmpEnableAll()
{
	bool rtnVal = true;
	DWORD level = ENABLE;
	int i = 0;


	StopAxisAll();
	Sleep(100);
	int iRet_Axis = Axl_Axisconfig();

	if(iRet_Axis != 1)
	{
		sLangChange.LoadStringA(IDS_STRING1281);	//���� ���� ����̺� �ʱ�ȭ ����.
		errMsg2(Task.AutoFlag, sLangChange);
		return	false;
	}


	for ( i = 0; i < MAX_MOTOR_NO; i++ )
	{
		m_bOrgFlag[i] = false;

#ifdef ON_LINE_MOTOR

		AxmSignalIsServoOn(i, &level);
		if ( level == ENABLE )
		{
			continue;
		}

		AxmSignalServoOn(i, ENABLE);	

		AxmSignalIsServoOn(i, &level);
		if ( level != ENABLE )
		{
			rtnVal = false;
		}
#endif		
	}

	Sleep(100);

	//! 1���� Servo On ���а� ������, �ٽ� �õ��Ѵ�. 
	if ( rtnVal == false )
	{
		rtnVal = true;

		for ( i = 0; i < MAX_MOTOR_NO; i++ )
		{
#ifdef ON_LINE_MOTOR
			//! �̹� Servo On �������� Ȯ���Ѵ�. 
			AxmSignalIsServoOn(i, &level);
			if ( level == ENABLE )
			{
				continue;
			}

			AxmSignalServoOn(i, ENABLE);	

			Sleep(100);

			AxmSignalIsServoOn(i, &level);
			if ( level != ENABLE )
			{
				rtnVal = false;
			}
#endif		
		}
	}//! if ( rtnVal == false )

	if(!rtnVal)
	{
		TCHAR dispStr[100];
		_stprintf_s(dispStr, "AmpEnableAll ����.");	//AmpEnableAll ����.
		LogSave(dispStr);
	}

	return rtnVal;
}


bool CAxlMotorSet::AmpFaultReset(int axis)
{
	m_bOrgFlag[axis] = false;
	double dDecel = 0.;

#ifdef ON_LINE_MOTOR

	DWORD level;

	AxmMoveStop(axis, dDecel);	
	AxmSignalServoOn(axis, DISABLE);

	AxmSignalServoAlarmReset(axis, ENABLE);

	Sleep(100);

	AxmSignalServoAlarmReset(axis, DISABLE);
	AxmSignalServoOn(axis, ENABLE);	

	Sleep(100);

	AxmSignalIsServoOn(axis, &level);
	if ( level != ENABLE )
	{
		return false;
	}
#endif

	return true;
}
bool CAxlMotorSet::AmpFaultResetAll()
{
	int i = 0;

	for(i=0; i<MAX_MOTOR_NO; i++)
	{
		AmpDisable(i);
	}

	Sleep(1000);

	for( i=0; i<MAX_MOTOR_NO; i++)
	{
		AmpEnable(i);
	}

	return true;
}

int	CAxlMotorSet::AmpFaultCheck()
{
	DWORD upStatus;
	int iAmpFaultFlag = 0;

	for(int i=0; i<MAX_MOTOR_NO; i++)
	{
		// ���� �˶� ��ȣ Ȯ��
		AxmSignalReadServoAlarm(i, &upStatus);

		if(upStatus == ACTIVE)
		{
			iAmpFaultFlag += 0x01<<i;
			m_bOrgFlag[i] = true;
		}
	}

	Task.iAmpFaultFlag = iAmpFaultFlag;
	return iAmpFaultFlag;
}


//! Motor�� ��ǥ ������ �����ߴ��� Ȯ��
bool CAxlMotorSet::GetInposition(int axis)
{
#ifdef ON_LINE_MOTOR

	DWORD dwRet = 0;
	DWORD use;

	//! Inposition ��ȣ ���� Ȯ��
	dwRet = AxmSignalGetInpos(axis, &use);
	if ( dwRet != AXT_RT_SUCCESS )
	{
		return false;
	}
	if ( use == UNUSED )
	{
		return false;
	}

	//! Inpositon ��ȣ�� �Է� Ȯ��
	DWORD upStatus = INACTIVE;
	dwRet = AxmSignalReadInpos(axis, &upStatus);
	if ( dwRet != AXT_RT_SUCCESS )
	{
		return false;
	}
	if ( upStatus == INACTIVE )
	{
		return false;
	}	
#endif
	return true;
}

bool CAxlMotorSet::GetInpositionPos(int iAxis, double dPos)
{//! ���� ���� Motor�� ��ǥ ������ �����ߴ��� Ȯ��( Encord������ Ȯ��)
	DWORD dwRet = 0, use = 0;

	if( !IsStopAxis(iAxis) || fabs(GetEncoderPos(iAxis) - dPos) > 0.005 )	return false;

	return true;
}

bool CAxlMotorSet::GetHomeSensor(int axis)
{
#ifdef ON_LINE_MOTOR
	DWORD dwRet = 0;
	int nAxis = axis;
	//DWORD state;

	//dwRet = AxmHomeGetSignalLevel(axis, &state);
	//if ( dwRet != AXT_RT_SUCCESS )
	//{
	//	return false;
	//}

	//int ret = home_switch(axis);
	//return ret ? true : false;
	//! Home ��ȣ�� �Է� Ȯ��
	DWORD upStatus = 0;
	dwRet = AxmHomeReadSignal(nAxis, &upStatus);
	if ( dwRet != AXT_RT_SUCCESS )
	{
		return false;
	}
	if ( upStatus == INACTIVE )
	{
		return false;
	}
#endif
	return true;
}
bool CAxlMotorSet::GetPosiSensor(int axis)
{
#ifdef ON_LINE_MOTOR
	DWORD dwRet = 0;
	DWORD StopMode = 0;
	DWORD PositiveLevel = 0;
	DWORD NegativeLevel = 0;
	int nAxis = axis;
	dwRet = AxmSignalGetLimit(nAxis, &StopMode, &PositiveLevel, &NegativeLevel);
	if ( dwRet != AXT_RT_SUCCESS )
	{
		return false;
	}
	if ( PositiveLevel == UNUSED )
	{
		//! ��� ����		
		return false;
	}

	//int ret = pos_switch(axis);
	//return ret ? true : false;
	//! ����Ʈ ���� �Է� ���� Ȯ��
	DWORD upPositiveLevel = 0;
	DWORD upNegativeLevel = 0;
	dwRet = AxmSignalReadLimit(nAxis, &upPositiveLevel, &upNegativeLevel);
	if ( dwRet != AXT_RT_SUCCESS )
	{
		return false;
	}
	if ( upPositiveLevel == ACTIVE )
	{
		return true;
	}
#endif

	return false;
}
bool CAxlMotorSet::GetNegaSensor(int axis)
{
#ifdef ON_LINE_MOTOR
	DWORD dwRet = 0;
	DWORD StopMode = 0;
	DWORD PositiveLevel = 0;
	DWORD NegativeLevel = 0;
	int nAxis = axis;
	dwRet = AxmSignalGetLimit(nAxis, &StopMode, &PositiveLevel, &NegativeLevel);
	if ( dwRet != AXT_RT_SUCCESS )
	{
		return false;
	}
	if ( NegativeLevel == UNUSED )
	{
		//! ��� ����		
		return false;
	}

	//int ret = neg_switch(axis);
	//return ret ? true : false;
	//! ����Ʈ ���� �Է� ���� Ȯ��
	DWORD upPositiveLevel = 0;
	DWORD upNegativeLevel = 0;
	dwRet = AxmSignalReadLimit(nAxis, &upPositiveLevel, &upNegativeLevel);
	if ( dwRet != AXT_RT_SUCCESS )
	{
		return false;
	}
	if ( upNegativeLevel == ACTIVE )
	{
		return true;
	}
#endif

	return false;
}
//! ���� �˶��� �߻��ϸ� true�� ��ȯ
bool CAxlMotorSet::GetAmpFault(int axis)
{
#ifdef	ON_LINE_MOTOR

	DWORD dwRet = 0;
	DWORD use = 0;

	int nAxis = axis;
	dwRet = AxmSignalGetServoAlarm(nAxis, &use);
	if ( dwRet != AXT_RT_SUCCESS )
	{		
		return true;
	}
	if ( use == UNUSED )
	{
		return true;
	}

	DWORD upStatus = 0;
	// ���� �˶� ��ȣ Ȯ��
	dwRet = AxmSignalReadServoAlarm(nAxis, &upStatus);
	if ( dwRet != AXT_RT_SUCCESS )
	{
		m_bOrgFlag[nAxis] = false;
		return true;
	}
	if(upStatus == ACTIVE)
	{
		m_bOrgFlag[nAxis] = false;
		return true;
	}
#endif

	return false;
}

bool CAxlMotorSet::GetAmpEnable(int axis)
{
	DWORD upStatus;
// Servo �˶� Ȯ��.
	AxmSignalIsServoOn(axis, &upStatus);	// upStatus  1: Servo On
	if(upStatus == ACTIVE)
	{
		return true;
	}

	m_bOrgFlag[axis] = false;
	return false;
}


bool CAxlMotorSet::GetAmpEnableAll()
{
	DWORD upStatus;
	bool rtnVal = true;

	for(int i=0; i<MAX_MOTOR_NO; i++)
	{
		AxmSignalIsServoOn(i, &upStatus);	// upStatus  1: Servo On
		if(upStatus != ACTIVE)
		{
			m_bOrgFlag[i] = false;
			rtnVal = false;
		}
	}

	return rtnVal;
}


double CAxlMotorSet::GetEncoderPos(int axis)
{
	double pos = 0;
#ifdef ON_LINE_MOTOR
	if (axis == Motor_Lens_Xt || axis == Motor_Lens_Yt || axis == Motor_PCB_Xt || axis == Motor_PCB_Yt || axis == Motor_PCB_Z || axis == Motor_Lens_Z || axis == Motor_PCB_TH){
		AxmStatusGetCmdPos(axis, &pos);	
	}else{
		AxmStatusGetActPos(axis, &pos);
	}
#endif

	pos /= sysData.fMotorResol[axis];

	return (float)pos;
}

double CAxlMotorSet::GetCommandPos(int axis)
{
	double pos = 0;

#ifdef ON_LINE_MOTOR

	AxmStatusGetCmdPos(axis, &pos);	
#endif

	pos /= sysData.fMotorResol[axis];

	return (float)pos;
}

double CAxlMotorSet::GetEncoderPos_Disp(int axis)
{
	double pos = 0;
	int mAxis = axis;
#ifdef ON_LINE_MOTOR
	AxmStatusGetActPos(mAxis, &pos);
#endif

	pos /= sysData.fMotorResol[mAxis];
	pos -= sysData.dDataset[mAxis];

	return (float)pos;
}

double CAxlMotorSet::GetCommandPos_Disp(int axis)
{
	double pos = 0.0;

	int mAxis = axis;
#ifdef ON_LINE_MOTOR
	AxmStatusGetCmdPos(mAxis, &pos);
#endif

	pos /= sysData.fMotorResol[mAxis];
	pos -= sysData.dDataset[mAxis];

	return (float)pos;
}

//! Motor�� ���������� true�� ��ȯ
bool CAxlMotorSet::IsStopAxis(int axis)
{
#ifdef ON_LINE_MOTOR

	//! ���� ���� ���¸� Ȯ��
	DWORD Status;
	DWORD dwRet = AxmStatusReadInMotion(axis, &Status);
	if ( dwRet != AXT_RT_SUCCESS )
	{
		return false;
	}
	if (Status == FALSE)
	{
		//! ���Ͱ� �����Ͽ����ϴ�.
		return true;
	}
#endif

	return false;
}



bool CAxlMotorSet::StopAxis(int axis)
{
#ifdef ON_LINE_MOTOR
	
	AxmMoveSStop(axis);
	Stop_Home_Search(axis);

	CTimeChecker timeChk;
	timeChk.Init_Time();
	timeChk.Start_Time();

	double t_duration = 0.;
	
	while(!IsStopAxis(axis))
	{
		DoEvents();

		t_duration = timeChk.Measure_Time(0);
		
		if(t_duration>1000)
		{
			//clear_status(axis);

			sTempLang.LoadStringA(IDS_STRING167);
			sLangChange.Format(sTempLang, MotorName[axis]);
			TCHAR temp[200];
			wsprintf(temp,sLangChange);
			CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
			pFrame->putListLog(temp);
			LogSave(temp);

			return false;
		}

		Sleep(5);
	}	
#endif

	return true;
}
bool CAxlMotorSet::StopAxisAll()
{
	int i;
	bool runFlag[MAX_MOTOR_NO] = {0, };

#ifdef ON_LINE_MOTOR
	for(i=0; i<MAX_MOTOR_NO; i++)
	{
		//! ���� ���� ���� ó���� ����ڰ� �Է��� ���ӵ���ŭ ��� ������ �����ϸ鼭 �����ϴ� �Լ��̴�. 
		AxmMoveSStop(i);
		Stop_Home_Search(i);

		runFlag[i] = 1;
	}
#endif

	CTimeChecker timeChk;

	timeChk.Init_Time();
	timeChk.Start_Time();
	double t_duration = 0.0;

	while(1)
	{
		t_duration = timeChk.Measure_Time(0);

		if(t_duration>1000)
		{
			LogSave("All Motor Stop Time Exceed. [1 sec]");
			return false;
		}

		for(i=0; i<MAX_MOTOR_NO; i++)
		{
			if (runFlag[i])
			{

#ifdef ON_LINE_MOTOR
				if(!IsStopAxis(i))
#endif
				{
					runFlag[i] = 0;
					break;
				}
			}
		}

		if(i==MAX_MOTOR_NO)
			break;

		DoEvents();

		Sleep(5);
	}
	
	return true;
}



BOOL CAxlMotorSet::JogMove(int axis, double dVel)
{// ���߿� �� ���� ����� ���.
	if(GetAmpFault(axis))
		return FALSE;

	double dAcc = (short)(sysData.fMotorAccTime[axis]);
	int ret = 0;
	DWORD dwRet = 0;
#ifdef	ON_LINE_MOTOR
	dwRet = AxmMoveVel(axis, dVel, dAcc, dAcc);
#endif
	ret = dwRet;

	return ret;
}
BOOL CAxlMotorSet::JogStop(int axis)
{
	//++ ������ ���� ������ ���������մϴ�.
	AxmMoveSStop(axis);

#ifdef	ON_LINE_MOTOR
	CTimeChecker timeChk;

	timeChk.Init_Time();
	timeChk.Start_Time();
	double t_duration = 0.0;

	while(!IsStopAxis(axis))
	{
		DoEvents();

		t_duration = timeChk.Measure_Time(0);

		if(t_duration > 1000)
		{
			LogSave("JOG Move Stop Time Exceed. [1 sec]");
			return false;
		}
	}
#endif

	return true;
}

bool CAxlMotorSet::DispenseGoMotorPos(int no, short axis[MAX_MOTOR_NO], double pos[MAX_MOTOR_NO], double dVel)
{
	long   axis2[MAX_MOTOR_NO] = { 0, };
	double pos2[MAX_MOTOR_NO] = { 0, };
	double vel[MAX_MOTOR_NO] = { 0, };
	double acc[MAX_MOTOR_NO] = { 0, };
	double dec[MAX_MOTOR_NO] = { 0, };

	int cnt = 0;
	double curPos = 0.0;

	for (int i = 0; i<no; i++)
	{
		if (!m_bOrgFlag[axis[i]])
			return false;

		if (!IsStopAxis(axis[i]))
		{
			CString tmpStr = "";
			sLangChange.LoadStringA(IDS_STRING1239);
			tmpStr.Format(_T("[%s] ") + sLangChange + _T(" [goMotoPos]"), MotorName[axis[i]]);
			errMsg2(Task.AutoFlag, tmpStr);
			return false;
		}

		if (axis[i] == Motor_PCB_Z || axis[i] == Motor_Lens_Z || axis[i] == Motor_Lens_Xt || axis[i] == Motor_Lens_Yt || axis[i] == Motor_PCB_Xt || axis[i] == Motor_PCB_Yt || axis[i] == Motor_PCB_TH) {
			curPos = GetCommandPos(axis[i]);
		}
		else {
			curPos = GetEncoderPos(axis[i]);
		}

		//! Motor�� �̵� ��ǥ ��ġ, �ӵ�, ���ӵ�/���ӵ� ���� �˾Ƴ���. 
		axis2[cnt] = axis[i];
		pos2[cnt] = pos[i] * sysData.fMotorResol[axis[i]];

		vel[cnt] = dVel * sysData.fMotorResol[axis[i]];//sysData.fMotorSpeed[axis[i]] * sysData.fMotorResol[axis[i]];

		acc[cnt] = sysData.fMotorAccTime[axis[i]]; //! ���� ���ӵ�
		dec[cnt] = sysData.fMotorAccTime[axis[i]];//sysData.iMotorDecTime[axis[i]];

		if (fabs(pos[i] - curPos)<0.001) { 			// �̵��� ��ġ���� 5um �̻� ���̰� �������� �������� ����.
			continue;
		}
		else {
			cnt++;
		}
	}

	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
#ifdef ON_LINE_MOTOR
	AxmMoveStartMultiPos(cnt, axis2, pos2, vel, acc, dec);
	Sleep(10);
#endif

	return true;
}
bool CAxlMotorSet::goMotorPos(int no, short axis[MAX_MOTOR_NO], double pos[MAX_MOTOR_NO], bool bAutoRun)
{
	long   axis2[MAX_MOTOR_NO] = {0, };
	double pos2[MAX_MOTOR_NO] = {0, };
	double vel[MAX_MOTOR_NO] = {0, };
	double acc[MAX_MOTOR_NO] = {0, };
	double dec[MAX_MOTOR_NO] = {0, };

	int cnt = 0;
	double curPos = 0.0;

	for(int i=0; i<no; i++)
	{
		if(!m_bOrgFlag[axis[i]])
			return false;

		if(!IsStopAxis(axis[i]))
		{
			CString tmpStr="";
			sLangChange.LoadStringA(IDS_STRING1239);
			tmpStr.Format(_T("[%s] ") + sLangChange + _T(" [goMotoPos]"), MotorName[axis[i]]);
			errMsg2(Task.AutoFlag, tmpStr);
			return false;
		}

		if (axis[i] == Motor_PCB_Z || axis[i] == Motor_Lens_Z || axis[i] == Motor_Lens_Xt || axis[i] == Motor_Lens_Yt || axis[i] == Motor_PCB_Xt || axis[i] == Motor_PCB_Yt || axis[i] == Motor_PCB_TH){
			curPos = GetCommandPos(axis[i]);
		}else{
			curPos = GetEncoderPos(axis[i]);
		}
		
		//! Motor�� �̵� ��ǥ ��ġ, �ӵ�, ���ӵ�/���ӵ� ���� �˾Ƴ���. 
		axis2[cnt] = axis[i];
		pos2[cnt] = pos[i] * sysData.fMotorResol[axis[i]];
		if ( bAutoRun == true )
		{ 
			vel[cnt] = sysData.fMotorSpeed[axis[i]] * sysData.fMotorResol[axis[i]];
		}
		else
		{
			vel[cnt] = sysData.fMotorSpeed[axis[i]] * sysData.fMotorResol[axis[i]];
		}
		acc[cnt] = sysData.fMotorAccTime[axis[i]]; //! ���� ���ӵ�
		dec[cnt] = sysData.fMotorAccTime[axis[i]];//sysData.iMotorDecTime[axis[i]];

		if( fabs(pos[i]-curPos)<0.001){ 			// �̵��� ��ġ���� 5um �̻� ���̰� �������� �������� ����.
			continue;
		}else{
			cnt++;
		}
	}

	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
#ifdef ON_LINE_MOTOR
	AxmMoveStartMultiPos(cnt, axis2, pos2, vel, acc, dec);
	Sleep(10);
#endif

	return true;
}

bool CAxlMotorSet::goMotorZPos(int no, short axis[MAX_MOTOR_NO],double pos[MAX_MOTOR_NO],double dVel, bool bAutoRun)
{
	long   axis2[MAX_MOTOR_NO] = {0, };
	double pos2[MAX_MOTOR_NO] = {0, };
	double vel[MAX_MOTOR_NO] = {0, };
	double acc[MAX_MOTOR_NO] = {0, };
	double dec[MAX_MOTOR_NO] = {0, };

	int cnt =0;
	double curPos;

	for(int i=0; i<no; i++)
	{
		if(!m_bOrgFlag[axis[i]])
			return false;

		if(!IsStopAxis(axis[i]))
		{
			CString tmpStr;
			sLangChange.LoadStringA(IDS_STRING1239);
			tmpStr.Format(_T("[%s] ") + sLangChange + _T(" [goMotoPos]"), MotorName[axis[i]]);
			errMsg2(Task.AutoFlag, tmpStr);
			return false;
		}

		if (axis[i] == Motor_Lens_Xt || axis[i] == Motor_Lens_Yt || axis[i] == Motor_PCB_Xt || axis[i] == Motor_PCB_Yt || axis[i] == Motor_PCB_Z || axis[i] == Motor_Lens_Z){
			curPos = GetCommandPos(axis[i]);
		}else{
			curPos = GetEncoderPos(axis[i]); //GetCommandPos(axis[i]);
		}
		
		//! Motor�� �̵� ��ǥ ��ġ, �ӵ�, ���ӵ�/���ӵ� ���� �˾Ƴ���. 
		axis2[cnt] = axis[i];
		pos2[cnt] = pos[i] * sysData.fMotorResol[axis[i]];
		if ( bAutoRun == true )
		{
			vel[cnt] = dVel * sysData.fMotorResol[axis[i]];//sysData.fMotorSpeed[axis[i]] * sysData.fMotorResol[axis[i]];
		}
		else
		{
			vel[cnt] = dVel * sysData.fMotorResol[axis[i]];//sysData.fMotorSpeed[axis[i]] * sysData.fMotorResol[axis[i]];
		}
		acc[cnt] = sysData.fMotorAccTime[axis[i]]; //! ���� ���ӵ�
		dec[cnt] = sysData.fMotorAccTime[axis[i]];//sysData.iMotorDecTime[axis[i]];

		if( fabs(pos[i]-curPos)<0.003) 			// �̵��� ��ġ���� 5um �̻� ���̰� �������� �������� ����.
			continue;
		else
			cnt++;
	}

	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
#ifdef ON_LINE_MOTOR
	AxmMoveStartMultiPos(cnt, axis2, pos2, vel, acc, dec);
	Sleep(10);
#endif

	return true;
}
bool CAxlMotorSet::MoveAxis(int axis, bool AbSFlag, double dPos, double dVel, float iAcc)
{	// abs 1:absolute, 0:increment
	if(GetAmpFault(axis))
	{
		m_bOrgFlag[axis] = false;
		sLangChange.LoadStringA(IDS_STRING1453);
		LogSave(sLangChange);		//"AMP Alarm �߻�. MoveAxis ���� ����."
		return false;
	}

	if(!GetAmpEnable(axis))
	{
		m_bOrgFlag[axis] = false;
	
		sLangChange.LoadStringA(IDS_STRING1226);
		CString tmpStr;
		tmpStr.Format(_T("[%s] ") + sLangChange, MotorName[axis]);
		errMsg2(Task.AutoFlag, tmpStr);
		return false;
	}

	if(AbSFlag == ABS)
	{
		double curPos;

		if (axis == Motor_PCB_Z || axis == Motor_Lens_Z || axis == Motor_Lens_Xt || axis == Motor_Lens_Yt || axis == Motor_PCB_Xt || axis == Motor_PCB_Yt || axis == Motor_PCB_Z || axis == Motor_PCB_TH){
			curPos = GetCommandPos(axis);
		}else{
			curPos = GetEncoderPos(axis);
		}
		if(fabs(curPos - dPos) < 0.0001)	
		{
			return true;
		}

		if(m_bOrgFlag[axis]==false)
		{
			TCHAR	motorMSG[256];
			sTempLang.LoadStringA(IDS_STRING367);
			sLangChange.Format(sTempLang, axis+1);
			wsprintf(motorMSG, sLangChange);
			return false;
		}
	}
	else if(AbSFlag==REL)
	{
		if (axis == Motor_PCB_Z || axis == Motor_Lens_Z || axis == Motor_Lens_Xt || axis == Motor_Lens_Yt || axis == Motor_PCB_Xt || axis == Motor_PCB_Yt || axis == Motor_PCB_Z || axis == Motor_PCB_TH){
			dPos += GetCommandPos(axis);
		}else{
			dPos += GetEncoderPos(axis);
		}
	}

	dPos *= sysData.fMotorResol[axis];
	if(dPos>0)					dPos = (int)(dPos+0.5);

	long   axisAll[MAX_MOTOR_NO] = {0, };
	double posAll[MAX_MOTOR_NO] = {0, };
	double velAll[MAX_MOTOR_NO] = {0, };
	double accAll[MAX_MOTOR_NO] = {0, };
	double decAll[MAX_MOTOR_NO] = {0, };

	axisAll[0] = axis;
	posAll[0] = dPos;
	velAll[0] = dVel * sysData.fMotorResol[axis];
	accAll[0] = sysData.fMotorAccTime[axis]; //! ���� ���ӵ�
	decAll[0] = sysData.fMotorAccTime[axis];

	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
	//AxmMoveStartMultiPos(1, axisAll, posAll, velAll, accAll, decAll);
	AxmMoveStartPos(axis, dPos, velAll[0], accAll[0], decAll[0]);

	Sleep(10);
	return true;
}


bool CAxlMotorSet::MoveAxisMulti(int iAxisCnt, short iAxis[MAX_MOTOR_NO], double pos[MAX_MOTOR_NO], int iSpeed)
{//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�
	long   axis2[MAX_MOTOR_NO] = {0, };
	double pos2[MAX_MOTOR_NO] = {0, };
	double vel[MAX_MOTOR_NO] = {0, };
	double acc[MAX_MOTOR_NO] = {0, };
	double dec[MAX_MOTOR_NO] = {0, };
	CString tmpStr;
	int iCnt =0;
	double curPos;

	for(int i=0; i<iAxisCnt; i++)
	{
		if(!m_bOrgFlag[iAxis[i]])
		{
			return false;
		}

		if(!IsStopAxis(iAxis[i]))
		{
			sLangChange.LoadStringA(IDS_STRING1239);
			tmpStr.Format(_T("[ %s ] ") + sLangChange + _T(" [MoveAxisMulti]"), MotorName[ iAxis[i] ]);
			errMsg2(Task.AutoFlag, tmpStr);
			return false;
		}

		if(MOTOR_TYPE[ iAxis[i] ] == STEPING)	curPos = GetCommandPos(iAxis[i]);
		else									curPos = GetEncoderPos(iAxis[i]); 

		
		//! Motor�� �̵� ��ǥ ��ġ, �ӵ�, ���ӵ�/���ӵ� ���� �˾Ƴ���. 
		axis2[iCnt] = iAxis[i];
		pos2[iCnt] = pos[i] * sysData.fMotorResol[iAxis[i]];
		vel[iCnt] = sysData.fMotorSpeed[iAxis[i]] * sysData.fMotorResol[iAxis[i]];

#ifdef MOTOR_ACC_TYPE_SEC
		acc[iCnt] = sysData.fMotorAccTime[ iAxis[i] ];	//! ���ӵ�
		dec[iCnt] = sysData.fMotorAccTime[ iAxis[i] ];	//! ���ӵ�
#else
/*���ӵ� ���� (G)
-> 9.8m/s^2  X 1000mm/m   X   2000plses/mm     =      pls/s^2   */
		acc[iCnt] = sysData.fMotorAccTime[ iAxis[i] ] * (9.8 * 1000 *  sysData.fMotorResol[ iAxis[i] ]);//! ���ӵ� 
		dec[iCnt] = sysData.fMotorDecTime[ iAxis[i] ] * (9.8 * 1000 *  sysData.fMotorResol[ iAxis[i] ]);//! ���ӵ�
#endif

		if( fabs(pos2[i]-curPos) < 0.005) 			// �̵��� ��ġ���� 5um �̻� ���̰� �������� �������� ����.
			continue;
		else
			iCnt++;
	}

	if( iCnt == 0 )	return true;	//�̵� ������ ���� ��� Return

	//m_csMoveMulti.Lock();
	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
#ifdef ON_LINE_MOTOR
	AxmMoveStartMultiPos(iCnt, axis2, pos2, vel, acc, dec);
#endif

	//m_csMoveMulti.Unlock();

	return true;
}

//! ���ӵ� = �ӵ�/(���� �ð�)���� ���
double CAxlMotorSet::CalcAccFromAccTime(double dVel, double dTime)
{
	double dRet = 0.;

	if ( fabs(dTime) < 0.001 )
	{
		//! ���� �ð��� �ʹ� ª�Ƽ� ������ ���� �ð��� 2 sec�� ����
		dRet = dVel / 2.;
	}
	else
	{
		dRet = dVel / dTime;
		dRet *= 1000.; //! ���� ��ȯ msec -> sec
	}

	return dRet;
}

int CAxlMotorSet::HomeSubReqProcess(int iAxis, int iStep, double &checkTime)
{
	CString sMsg;	
	int iRtnStep = iStep;
	double curTime = myTimer(true); 

	switch(iStep)
	{
	case 100:
		m_bOrgFlag[iAxis] = false;
		if( Start_Home_Search(iAxis) < 0 )
		{
			sLangChange.LoadStringA(IDS_STRING322);
			wsprintf(m_szLogMsg, sLangChange, MotorName[iAxis]);	//"[����] %s (HOM) ���� ���� ��� ����."
			iRtnStep = -100;
			break;
		}

		checkTime = myTimer(true);
		iRtnStep = 200;
		break;

	case 200:
		{ 
			bool bHomeProc=true, bHomeError=true;

			int iHomFlag = motor.IsCompleted_Home_Search(iAxis);
			if( iHomFlag < 1 )	
				bHomeError = false;				//���� ���� Error ���� �� ���
			if( iHomFlag == 2 )	bHomeProc = false;				//���� ���� ���� ���� ���.
			if( iHomFlag == 1 )
			{//! ���������� ������ �˻� �Ǿ��� ���
				iRtnStep = 500;
			}
	
			if( bHomeError == false )	
			{//! ������ �˶�(���� ���� ����)
				sLangChange.LoadStringA(IDS_STRING328);
				wsprintf(m_szLogMsg, sLangChange, MotorName[iAxis], iStep);		//"[����] %s ���� ���� Error �߻� ����.[STEP:%d]"
				iRtnStep = -200;
				break;
			}
			else if( bHomeProc == false )
			{//! ���� ������ ���� �ȳ� ���
				if((curTime - checkTime) > MOTOR_ORIGIN_TIME)
				{
					sLangChange.LoadStringA(IDS_STRING329);
					wsprintf(m_szLogMsg, sLangChange, MotorName[iAxis], iStep);	//"[����] %s ���� ���� �̵� �ð� �ʰ�..[STEP:%d]"
					iRtnStep = -200;
				}
			}
		}
		break;

	case 500:
		m_bOrgFlag[iAxis] = true;
		iRtnStep = 1000;
		break;

	}

	return iRtnStep;
}

int CAxlMotorSet::HomeSubProcess(int axis, int iStep, double &checkTime)
{
	////////////////////////////////////////////////////////
	// Lens X	Home (+)����
	// Lens Y	Home (+)����
	// Lens Z	Home (+)����
	// Lens Tx	  -	 
	// Lens Ty	Home (-)����
	// PCB X	Home (-)����
	// PCB Y	Home (-)����
	// PCB Z	  -	 
	////////////////////////////////////////////////////////
	CString sMsg="";	
	int iRtnStep = iStep;
	
	CString sLog="";
	DWORD dwReturn = 0;
	DWORD statusMechanical = 0;
	DWORD statusMotion = 0;

	double dAcc = sysData.fMotorAccTime[axis]*3;
	double dVel = sysData.fMotorResol[axis]*20;
	double curTime = myTimer(true); 

	if( (axis == Motor_Lens_Xt) || (axis == Motor_Lens_Yt) || (axis == Motor_PCB_Xt) || (axis == Motor_PCB_Yt) || (axis == Motor_Lens_Z) || (axis == Motor_PCB_TH))
	{
		dVel /= 4;
	}


	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	if((axis == Motor_Lens_X) || (axis == Motor_Lens_Y) || (axis == Motor_Lens_Z) || (axis == Motor_PCB_Z || axis == Motor_PCB_TH)  ||
		(axis == Motor_Lens_Xt) || (axis == Motor_Lens_Yt) /*|| (axis == Motor_PCB_TH)*/)
	{// Home (+)����
		switch(iStep)
		{
		case 100:
			dwReturn = AxmMoveSignalSearch(axis, dVel, dAcc, PosEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);
			Sleep(5);

			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sLangChange.LoadStringA(IDS_STRING316);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (+) �̵� ���� ����."
				iRtnStep = -200;
			}
			else
			{
				checkTime = myTimer(true);
				iRtnStep = 200;
			}
			break;
		
		case 200:
//			if(IsStopAxis(axis))
			if(GetPosiSensor(axis) && IsStopAxis(axis))
			{
				if(axis == Motor_PCB_Z)
				{
					dwReturn = AxmMoveSignalSearch(axis, -dVel/4, dAcc, HomeSensor, SIGNAL_DOWN_EDGE, SLOWDOWN_STOP);
					if ( dwReturn != AXT_RT_SUCCESS )
					{
						sLangChange.LoadStringA(IDS_STRING324);
						wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (HOM) �̵� ���� ����."
						iRtnStep = -200;
					}
					else
					{
						checkTime = myTimer(true);
						iRtnStep = 300;
					}
				}
				else
				{
					dwReturn = AxmMoveSignalSearch(axis, -dVel, dAcc, HomeSensor, SIGNAL_DOWN_EDGE, SLOWDOWN_STOP);
					if ( dwReturn != AXT_RT_SUCCESS )
					{
						sLangChange.LoadStringA(IDS_STRING324);	//[����] %s (HOM) �̵� ���� ����.
						wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);
						iRtnStep = -200;
					}
					else
					{
						checkTime = myTimer(true);
						iRtnStep = 300;
					}
				}
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				StopAxis(axis);
				sLangChange.LoadStringA(IDS_STRING315);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (+) ���� ���� �ð� �ʰ�"
				iRtnStep = -200;
			}
			break;

		case 300:
//			if(IsStopAxis(axis))
			if(!GetHomeSensor(axis) && IsStopAxis(axis))
			{
				dwReturn = AxmMoveSignalSearch(axis, dVel/20, dAcc, HomeSensor, SIGNAL_UP_EDGE, EMERGENCY_STOP);
				if ( dwReturn != AXT_RT_SUCCESS )
				{
					sLangChange.LoadStringA(IDS_STRING320);
					wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (HOM) �˻� ���� ����."
					iRtnStep = -300;
				}
				else
				{
					checkTime = myTimer(true);
					iRtnStep = 400;
				}
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				StopAxis(axis);
				sLangChange.LoadStringA(IDS_STRING323);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (HOM) �̵� �ð� �ʰ�"
				iRtnStep = -300;
			}
			break;

		case 400:
//			if(IsStopAxis(axis))
			if(GetHomeSensor(axis) && IsStopAxis(axis))
			{
				checkTime = myTimer(true);
				iRtnStep = 500;
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				StopAxis(axis);
				sLangChange.LoadStringA(IDS_STRING319);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (HOM) �˻� �ð� �ʰ�"
				iRtnStep = -400;
			}
			break;

		case 500:
			if((curTime - checkTime) > 2000)
			{
				AxmStatusSetActPos(axis, 0.);
				AxmStatusSetCmdPos(axis, 0.);
				m_bOrgFlag[axis] = true;
				iRtnStep = 1000;
			}
			break;
		}
	}
	else if( (axis == Motor_PCB_X) || (axis == Motor_PCB_Y) || (axis == Motor_PCB_TH))
	{// Home (-)����
		switch(iStep)
		{
		case 100:
			//dwReturn = AxmMoveSignalSearch(axis, dVel, dAcc, HomeSensor, SIGNAL_DOWN_EDGE, EMERGENCY_STOP);
			dwReturn = AxmMoveSignalSearch(axis, -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);
			Sleep(5);

			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sLangChange.LoadStringA(IDS_STRING313);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (-) �̵� ���� ����."
				iRtnStep = -200;
			}
			else
			{
				checkTime = myTimer(true);
				iRtnStep = 200;
			}
			break;

		case 200:
			if(GetNegaSensor(axis) && IsStopAxis(axis))
			{
				// PNT 0829..
				if((axis != Motor_Lens_Xt) && (axis != Motor_Lens_Yt) && (axis != Motor_PCB_Xt) && (axis != Motor_PCB_Yt) && (axis != Motor_PCB_TH))	//-- ������ ���Ͱ� �ƴ� ��츸 ��ġ Setting
				{
					AxmStatusSetActPos(axis, 0.);
					AxmStatusSetCmdPos(axis, 0.);
				}

				dwReturn = AxmMoveSignalSearch(axis, dVel, dAcc, HomeSensor, SIGNAL_DOWN_EDGE, SLOWDOWN_STOP);

				if ( dwReturn != AXT_RT_SUCCESS )
				{
					sLangChange.LoadStringA(IDS_STRING324);
						wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (HOM) �̵� ���� ����."
					iRtnStep = -200;
				}
				else
				{
					checkTime = myTimer(true);
					if((axis != Motor_Lens_Xt) && (axis != Motor_Lens_Yt) && (axis != Motor_PCB_Xt) && (axis != Motor_PCB_Yt) && (axis != Motor_PCB_TH))		iRtnStep = 300;
					else																				iRtnStep = 301;
				}
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				StopAxis(axis);
				sLangChange.LoadStringA(IDS_STRING311);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (-) ���� ���� �ð� �ʰ�"
				iRtnStep = -200;
			}

			break;


		case 300:
			if((axis != Motor_Lens_Xt) && (axis != Motor_Lens_Yt) && (axis != Motor_PCB_Xt) && (axis != Motor_PCB_Yt) && (axis != Motor_PCB_TH))	//-- ������ ���Ͱ� �ƴ� ��츸 ��ġ Setting
			{
				if( GetEncoderPos(axis) > 100.0 ) //wook 0826  100mm
				//if( !GetHomeSensor(axis) && IsStopAxis(axis))
				{
					StopAxis(axis);
					sLangChange.LoadStringA(IDS_STRING326);
					wsprintf(m_szLogMsg, sLangChange, MotorName[axis],(int)GetEncoderPos(axis));	//"[����] %s Home ���� ���� �Ÿ�(%02d [mm]) �ʰ�"
					iRtnStep = -300;
					break;
				}
				if((curTime - checkTime) > MOTOR_MOVE_TIME)
				{
					StopAxis(axis);
					sLangChange.LoadStringA(IDS_STRING321);
					wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//[����] %s (HOM) ���� ���� �ð� �ʰ�"
					iRtnStep = -300;
					break;
				}
				if(!IsStopAxis(axis)) 
					break;
			}

			iRtnStep++;
			break;

		case 301:
			if(IsStopAxis(axis))
//			if(!GetHomeSensor(axis) && IsStopAxis(axis))
			{
				dwReturn = AxmMoveSignalSearch(axis, -dVel/20, dAcc, HomeSensor, SIGNAL_UP_EDGE, EMERGENCY_STOP);

				if ( dwReturn != AXT_RT_SUCCESS )
				{
					sLangChange.LoadStringA(IDS_STRING320);
					wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (HOM) �˻� ���� ����."
					iRtnStep = -iRtnStep;
				}
				else
				{
					checkTime = myTimer(true);

					iRtnStep = 500;
				}
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				StopAxis(axis);
				sLangChange.LoadStringA(IDS_STRING323);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (HOM) �̵� �ð� �ʰ�"
				iRtnStep = -iRtnStep;
			}
			break;

		case 500:
			//if(IsStopAxis(axis))
			if(GetHomeSensor(axis) && IsStopAxis(axis))
			{
				if (axis == Motor_Lens_Xt || axis == Motor_Lens_Yt || axis == Motor_PCB_Xt || axis == Motor_PCB_Yt || axis == Motor_PCB_TH)	//������ ������ ��� AMP OFF ���� ���� Setting
				{
					AmpDisable(axis);
					checkTime = myTimer(true);
					iRtnStep = 510;	//--> ������ ���͸� 
					break;
				}

				checkTime = myTimer(true);
				iRtnStep = 600;
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				sLangChange.LoadStringA(IDS_STRING319);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (HOM) �˻� �ð� �ʰ�"
				iRtnStep = -500;
			}
			break;
		case 510:
			if( GetAmpEnable(axis) == false)
			{
				iRtnStep = 600;
				break;
			}
			else if((curTime - checkTime) > IO_CHECK_TIME) //3�ʸ�
			{
				StopAxis(axis);
				sLangChange.LoadStringA(IDS_STRING327);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s Servo Off ����"
				iRtnStep = -600;
			}
			break;

		case 600:
			if((curTime - checkTime) > 2000)
			{
				AxmStatusSetActPos(axis, 0.);
				AxmStatusSetCmdPos(axis, 0.);

				if(axis == Motor_Lens_Xt || axis == Motor_Lens_Yt || axis == Motor_PCB_Xt || axis == Motor_PCB_Yt || axis == Motor_PCB_TH)	//������ ������ ��� AMP OFF ���� ���� Setting
				{
					AmpEnable(axis);
				}
				m_bOrgFlag[axis] = true;
				iRtnStep = 1000;
			}
			break;
		}
	}
	//else if((axis == Motor_Lens_Xt) ||(axis == Motor_Lens_Xt) )
	else if( (axis == Motor_PCB_Yt) || (axis == Motor_Lens_Xt) || (axis == Motor_Lens_Yt) || (axis == Motor_PCB_Xt) || (axis == Motor_PCB_TH))
	{// - Limit �� Home
		switch(iStep)
		{
		case 100:
			
			dwReturn = AxmMoveSignalSearch(axis, -dVel, dAcc, NegEndLimit, SIGNAL_UP_EDGE, SLOWDOWN_STOP);
			Sleep(5);

			if ( dwReturn != AXT_RT_SUCCESS )
			{
				sLangChange.LoadStringA(IDS_STRING313);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (-) �̵� ���� ����."
				iRtnStep = -100;
			}
			else
			{
				checkTime = myTimer(true);
				iRtnStep = 200;
			}
			break;

		case 200:
//			if(IsStopAxis(axis))
			if(GetNegaSensor(axis) && IsStopAxis(axis))
			{
				CString sLog;
				sTempLang.LoadStringA(IDS_STRING109);
				sLangChange.Format(sTempLang, MotorName[axis]);
				sLog.Format(sLangChange);
				pFrame->putListLog(sLog);
			
				checkTime = myTimer(true);
				iRtnStep = 250;
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				StopAxis(axis);
				sLangChange.LoadStringA(IDS_STRING317);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (+)���� �̵� ����"
				iRtnStep = -200;
			}
			break;

		case 250:
			if( IsStopAxis(axis) )
			{
				double moveVal = 2;
				double moveDist = 0.5;
				dwReturn = MoveAxis(axis, false, moveDist, moveVal, (float)dAcc);	
				if ( dwReturn != TRUE ){
					sLangChange.LoadStringA(IDS_STRING325);
					wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s +0.5mm �̵� ���� ���� (Down)."
					iRtnStep = -250;
				} else {
					checkTime = myTimer(true);
					iRtnStep = 251;
				}
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				StopAxis(axis);
				sLangChange.LoadStringA(IDS_STRING317);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (+)���� �̵� ����"
				iRtnStep = -250;
			}
			break;
		case 251:
			if( IsStopAxis(axis) )
			{
				if( GetHomeSensor(axis) )
				{//Home������ ��� ���� ��� Ȩ������ ���������� +1mm�� �̵�
					checkTime = myTimer(true);
					iRtnStep = 260;
				}
				else 
				{
					checkTime = myTimer(true);
					iRtnStep = 250;
				}
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				StopAxis(axis);
				sLangChange.LoadStringA(IDS_STRING317);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (+)���� �̵� ����"
				iRtnStep = -251;
			}
			break;

		case 260:
			if(GetHomeSensor(axis) && IsStopAxis(axis))
			{//Home ������ ��� ���� ���¶�� �ȵ��ö����� +�̵�
				double moveVal = 2;
				double moveDist = 0.5;
				dwReturn = MoveAxis(axis, false, moveDist, moveVal, (float)dAcc);	
				if ( dwReturn != TRUE ){
					sLangChange.LoadStringA(IDS_STRING325);
					wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s +0.5mm �̵� ���� ���� (Down)."
					iRtnStep = -260;
				} else {
					checkTime = myTimer(true);
					iRtnStep = 261;
				}
			}
			else if( !GetHomeSensor(axis) && IsStopAxis(axis) )
			{
				checkTime = myTimer(true);
				iRtnStep = 300;
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				StopAxis(axis);
				sLangChange.LoadStringA(IDS_STRING317);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (+)���� �̵� ����"
				iRtnStep = -260;
			}
			break;
		case 261:
			if( IsStopAxis(axis) )
			{
				if( !GetHomeSensor(axis) )
				{//Home������ Off�� ��� ���� Step
					checkTime = myTimer(true);
					iRtnStep = 300;
				}
				else 
				{
					checkTime = myTimer(true);
					iRtnStep = 260;
				}
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				StopAxis(axis);
				sLangChange.LoadStringA(IDS_STRING317);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (+)���� �̵� ����"
				iRtnStep = -261;
			}
			break;

		case 300:
//			if(IsStopAxis(axis))
			if(!GetNegaSensor(axis) && IsStopAxis(axis))
			{
				CString sLog;
				sTempLang.LoadStringA(IDS_STRING110);
				sLangChange.Format(sTempLang, MotorName[axis]);
				sLog.Format(sLangChange);
				pFrame->putListLog(sLog);

				dwReturn = AxmMoveSignalSearch(axis, -dVel/20, dAcc, HomeSensor, SIGNAL_UP_EDGE, EMERGENCY_STOP);
				Sleep(5);

				if ( dwReturn != AXT_RT_SUCCESS )
				{
					sLangChange.LoadStringA(IDS_STRING312);
					wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (-) �̵� ���� ���� (Up)."
					iRtnStep = -300;
				}
				else
				{
					checkTime = myTimer(true);
					iRtnStep = 400;
				}
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				StopAxis(axis);
				sLangChange.LoadStringA(IDS_STRING314);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (-) �ν� �ð� �ʰ�"
				iRtnStep = -300;
			}
			break;

		case 400:
			if(GetHomeSensor(axis) && IsStopAxis(axis))
			{

				checkTime = myTimer(true);
				iRtnStep = 500;
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				StopAxis(axis);
				sLangChange.LoadStringA(IDS_STRING318);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s (H) �ν� �ð� �ʰ�2"
				iRtnStep = -400;
			}
			break;

		case 500:
			if((curTime - checkTime) > 1000)
			{
				if (axis == Motor_Lens_Xt || axis == Motor_Lens_Yt || axis == Motor_PCB_Xt || axis == Motor_PCB_Yt || axis == Motor_PCB_TH)
				{//-- �������� ���� Off�� Set
					AmpDisable(axis);
					Sleep(100);
					checkTime = myTimer(true);
					iRtnStep = 600;
					break;
				}
				AxmStatusSetActPos(axis, 0.);
				AxmStatusSetCmdPos(axis, 0.);

				m_bOrgFlag[axis] = true;
				iRtnStep = 1000;
			}
			break;
		case 600:
			if(GetAmpEnable(axis) == false)
			{
				AxmStatusSetActPos(axis, 0.);
				AxmStatusSetCmdPos(axis, 0.);
				Sleep(100);
				AmpEnable(axis);
				m_bOrgFlag[axis] = true;
				iRtnStep = 1000;
			}
			else if((curTime - checkTime) > 10000/*IO_CHECK_TIME*/) //3�ʸ�
			{
				StopAxis(axis);
				sLangChange.LoadStringA(IDS_STRING327);
				wsprintf(m_szLogMsg, sLangChange, MotorName[axis]);	//"[����] %s Servo Off ����"
				iRtnStep = -600;
			}
			checkMessage();
		}
	}
	return iRtnStep;
}

bool CAxlMotorSet::HomeProcess(int axis)
{
	if ( axis < 0 || axis >= MAX_MOTOR_NO )		return false;

	if ( IsStopAxis(axis) == false )
	{//! Motor�� ���� �������� Ȯ��
		CString tmpStr;
		sLangChange.LoadStringA(IDS_STRING310);
		tmpStr.Format(sLangChange, MotorName[axis]);
		errMsg2(Task.AutoFlag, tmpStr);
		return false;
	}

	if(!AmpEnable(Motor_Lens_Z))
	{
		sLangChange.LoadStringA(IDS_STRING706);	//Lens Z ���� ���� On ����
		errMsg2(Task.AutoFlag, sLangChange);
		return false;
	}

	if(!AmpEnable(axis))
	{
		sLangChange.LoadStringA(IDS_STRING1280);	//���� On ���� ����
		errMsg2(Task.AutoFlag, sLangChange);
		return false;
	}


	m_bOrgFlag[axis] = false;
	bool bChk_Amp_Fault   = GetAmpFault(axis);

	if ( bChk_Amp_Fault == true )
	{
		AmpFaultReset(axis);
		Sleep(100);
	}

	if(axis!=Motor_Lens_Z && (!GetAmpEnable(Motor_Lens_Z)))
	{
		sLangChange.LoadStringA(IDS_STRING728);	//Lens �� Z���� Servo OFF �����Դϴ�.
		errMsg2(Task.AutoFlag, sLangChange);
		return false;
	}

	CString sLog;
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	sLangChange.LoadStringA(IDS_STRING165);
	sLog.Format(sLangChange, MotorName[axis]);	//[%s] ���� ���� ���� ����.
	pFrame->putListLog(sLog);


	CTimeChecker timeChk;
	double t_duration = 0.;
	double curTime, checkTime, OrginTime;

	double subTime = 0;
	int iStep = 100;
	int iHomeRtnStep = 0;

	OrginTime = myTimer(true);

	while (1)
	{
		bool bChk_Amp = GetAmpFault(axis);
		if(bChk_Amp)
		{
			sLangChange.LoadStringA(IDS_STRING1278);	//���� Amp Fault �߻�.
			errMsg2(Task.AutoFlag, sLangChange);
			return false;
		}

		bChk_Amp = GetAmpEnable(axis);
		if(!bChk_Amp)
		{//! �������� Servo Off ������ ������ ������ �˶�.
			if(!(axis == Motor_Lens_Xt && iHomeRtnStep == 600) &&
				!(axis == Motor_Lens_Yt && iHomeRtnStep == 600) &&
				!(axis == Motor_PCB_Yt && iHomeRtnStep == 600) &&
				!(axis == Motor_PCB_Xt && iHomeRtnStep == 600) &&
				!(axis == Motor_PCB_Z && iHomeRtnStep == 600) &&
				!(axis == Motor_Lens_Z && iHomeRtnStep == 600) &&
				!(axis == Motor_PCB_TH && iHomeRtnStep == 600))
			{
				sLangChange.LoadStringA(IDS_STRING1279);	//���� Off �߻�.
				errMsg2(Task.AutoFlag, sLangChange);
				return false;
			}
		}

		curTime = myTimer(true);

		switch(iStep)
		{
		case 100:
			checkTime = myTimer(true);
			if( m_bOrgFlag[Motor_Lens_Z] == true )
			{
				LENS_Z_Motor_Move(Wait_Pos);;
				iStep = 400;
			}
			else
			{
				AmpEnable(Motor_Lens_Z);
				iStep = 400;
			}
			break;

		/*case 200:
			if(Dio.PCBCoverCloseCheck(true, false))
			{
				Dio.BackLightOn(false, false);
				checkTime = myTimer(true);
				iStep = 300;
			}
			else 
			{
				sLangChange.LoadStringA(IDS_STRING339);
				errMsg2(Task.AutoFlag, sLangChange);
				iStep = -200;
			}
			break;

		case 300:
			if(Dio.BackLightOnCheck(false, false))
			{*/
			//	
			//}
			//else if((curTime - checkTime) > IO_CHECK_TIME)
			//{
			//	sLangChange.LoadStringA(IDS_STRING330);
			//	errMsg2(Task.AutoFlag, sLangChange);	//"[����] Back Light ���� ����"
			//	iStep = -400;
			//}
			//break;

		case 400:
			if( IsStopAxis(Motor_Lens_Z) )
			{
				checkTime = myTimer(true);
				iStep = 900;
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				sLangChange.LoadStringA(IDS_STRING337);
				errMsg2(Task.AutoFlag, sLangChange);
				iStep = -400;
			}
			break;

		//case 500:
		//	if(m_bOrgFlag[Motor_Laser_Z] == true)
		//	{
		//		Laser_Z_Motor_Move(Wait_Pos);;
		//		iStep = 510;
		//	}
		//	else
		//	{
		//		AmpEnable(Motor_Laser_Z);
		//		iStep = 510;
		//	}
		//	break;

		//case 510:
		//	if( IsStopAxis(Motor_Laser_Z) )
		//	{
		//		iHomeRtnStep = 100;	
		//		checkTime = myTimer(true);
		//		iStep = 900;
		//	}
		//	else if((curTime - checkTime) > MOTOR_MOVE_TIME)
		//	{
		//		sLangChange.LoadStringA(IDS_STRING332);
		//		errMsg2(Task.AutoFlag, sLangChange);
		//		iStep = -500;
		//	}
		//	break;

		case 900:
			if(m_bOrgFlag[axis] == true)
				iStep = 1000;
			break;
		}//main switch


		if(iHomeRtnStep >= 100 && iHomeRtnStep < 1000)
		{
			iHomeRtnStep = HomeSubReqProcess(axis, iHomeRtnStep, subTime);
			if(iHomeRtnStep < 0)
			{
				StopAxis(axis);
				errMsg2(Task.AutoFlag, m_szLogMsg);
				return false;
			}
		}

		if(iHomeRtnStep == 1000)
			break;

	

		if(iHomeRtnStep < 0 || iHomeRtnStep > 1000 ||
			iStep < 0 )
		{
			sLangChange.LoadStringA(IDS_STRING340);
			errMsg2(Task.AutoFlag, sLangChange);	//("[����] ���� ���� ���� ����"
			return false;
		}

		if((curTime - OrginTime) > MOTOR_ORIGIN_TIME)
		{
			StopAxisAll();
			sLangChange.LoadStringA(IDS_STRING341);
			errMsg2(Task.AutoFlag, sLangChange);	//"[����] ���� ���� �ð� �ʰ�"
			return false;
		}

		checkMessage();
		Sleep(5);
		checkMessage();
	}

	return true;
}


bool CAxlMotorSet::HomeProcessAll()
{
	CString sLog="";
	CTimeChecker timeChk;
	double t_duration = 0.0;


	sLog.Format("��ü ���͸� ���� ���� �մϴ�.");	//��ü ���� ���� ���� ����.
	theApp.MainDlg->putListLog(sLog);

	sLangChange.LoadStringA(IDS_STRING1332);//"���� ������"
	theApp.MainDlg->m_labelHom.SetBkColor(M_COLOR_GRAY);
	theApp.MainDlg->m_labelHom.SetText(sLangChange);
	theApp.MainDlg->m_labelHom.Invalidate();
	
	theApp.MainDlg->m_btnOrigin.m_iStateBtn = 0;
	theApp.MainDlg->m_btnOrigin.Invalidate();


	StopAxisAll();
	Sleep(10);

	for (int i = Motor_PCB_X; i <= Motor_Lens_Z; i++)//for (int i= Motor_Lens_X ; i<= Motor_PCB_TH ; i++)
	{
		m_bOrgFlag[i] = false;
		 
		if ( IsStopAxis(i) == false )
		{
			sLangChange.LoadStringA(IDS_STRING1244);
			sLog.Format(_T("[%s] ") + sLangChange, MotorName[i]);	//���Ͱ� ���� �� �Դϴ�
			errMsg2(Task.AutoFlag, sLog);
			return false;
		}

		bool bChk_Amp_Fault   = GetAmpFault(i);

		if ( bChk_Amp_Fault == true )
		{
			sLog.Format("[%s] ���� Home AmpFault ����", MotorName[i]);		//���� Home AmpFault ����
			errMsg2(Task.AutoFlag, sLog);
			return false;
		}
	}

	if(!AmpEnableAll())
	{
		sLog.Format("���� ���� �� ��ü ���� On ����");
		errMsg2(Task.AutoFlag, sLog);	//"���� ���� �� ��ü ���� On ����"
		return false;
	}

	int MainStep = 100;
	int iStep[MAX_MOTOR_NO];
	double iTime[MAX_MOTOR_NO];

	for(int i=0; i<MAX_MOTOR_NO;i++)
	{
		iStep[i] = 0;	// 100���� ����
		iTime[i] = 0;
	}

	double dAcc = 0.0;
	double dVel = 0.0;

	double curTime = 0.0;
	double checkTime = 0.0;
	double OriginStartTime = 0.0;
	double dSpeed = 15.0f;
	OriginStartTime = myTimer(true);
	CString logStr = _T("");
	while(true)
	{
		int bChk_Amp = AmpFaultCheck();
		if(bChk_Amp)
		{
			sLog.Format("���� Amp Fault �߻�.");	//���� Amp Fault �߻�.
			errMsg2(Task.AutoFlag, sLog);
			return false;
		}
		for(int i =0;i<MAX_MOTOR_NO;i++)
		{
			if(!GetAmpEnable(i))
			{
				if(!(i == Motor_Lens_Xt  && iStep[Motor_Lens_Xt] == 600) &&
					!(i == Motor_Lens_Yt  && iStep[Motor_Lens_Yt] == 600) &&
					!(i == Motor_PCB_Xt  && iStep[Motor_PCB_Xt] == 600) &&
					!(i == Motor_PCB_Yt  && iStep[Motor_PCB_Yt] == 600) &&
					!(i == Motor_PCB_Z  && iStep[Motor_PCB_Z] == 600) &&
					!(i == Motor_Lens_Z  && iStep[Motor_Lens_Z] == 600) &&
					!(i == Motor_PCB_TH  && iStep[Motor_PCB_TH] == 600))
				{
					sLangChange.LoadStringA(IDS_STRING1279);	//���� Off �߻�.
					errMsg2(Task.AutoFlag, sLangChange);
					return false;
				}
			}
		}

		curTime = myTimer(true);

		motor.InDIO(0, curInDio[0]);
		motor.InDIO(2, curInDio[1]);
		switch(MainStep)
		{
			case 100:

			MainStep = 210;
			Task.PCBTaskTime = myTimer(true);
			break;
		
		case 200:
			if (Dio.UVCylinderCheck(true, false))
			{
				sLog.Format("UV �Ǹ��� ��� Ȯ�� �Ϸ�[%d]", MainStep);
				theApp.MainDlg->putListLog(sLog);
				MainStep = 210;

				Sleep(800);
			}
			else if ((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				sLog.Format("UV �Ǹ��� ��� �ð� �ʰ�..[%d]", MainStep);
				errMsg2(Task.AutoFlag, sLog);
				MainStep = -200;
			}
			break;

			//Laser �Ǹ��� ���
		case 210:
			if (Dio.LaserCylinderUPDOWN(true, false))
			{
				sLog.Format("LASER �Ǹ��� ��� �Ϸ�[%d]", MainStep);
				theApp.MainDlg->putListLog(sLog);
				checkTime = myTimer(true);
				MainStep = 220;
			}
			else
			{
				sLog.Format("LASER �Ǹ��� ��� ���� [%d]", MainStep);
				errMsg2(Task.AutoFlag, sLog);
				MainStep = -210;
			}
			break;

		case 220:
			if (Dio.LaserCylinderCheck(true, false))
			{
				sLog.Format("LASER �Ǹ��� ��� Ȯ�� �Ϸ�[%d]", MainStep);
				theApp.MainDlg->putListLog(sLog);
				MainStep = 250;

				Sleep(300);
			}
			else if ((curTime - checkTime) > 3000)
			{
				sLog.Format("LASER �Ǹ��� ��� �ð� �ʰ�..[%d]", MainStep);
				errMsg2(Task.AutoFlag, sLog);
				MainStep = -220;
			}
			break;
			//Laser �Ǹ��� ��� END

		case 250:

			MainStep = 300;
			break;
		case 300:
			dAcc = sysData.fMotorAccTime[Motor_Lens_Z];
			dVel = 5 * sysData.fMotorResol[Motor_Lens_Z];
			AxmMoveVel(Motor_Lens_Z, dVel, dAcc, dAcc);
			checkTime = myTimer(true);
			MainStep = 400;
			break;
		case 400:
			if(GetPosiSensor(Motor_Lens_Z) && IsStopAxis(Motor_Lens_Z))
			{
				dAcc = sysData.fMotorAccTime[Motor_PCB_Z];
				dVel = 5 * sysData.fMotorResol[Motor_PCB_Z];
				AxmMoveVel(Motor_PCB_Z, -dVel, dAcc, dAcc);		// ���� �̵�
				checkTime = myTimer(true);
				MainStep = 405;//410;
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				//sLangChange.LoadStringA(IDS_STRING337);
				errMsg2(Task.AutoFlag, "[����] Lens Z�� (+)�̵� ����");
				MainStep = -400;
			}
			break;

		case 405://! PCB Z�� (-) Limit ��ġ �̵�
			if(GetNegaSensor(Motor_PCB_Z) && IsStopAxis(Motor_PCB_Z))
			{
				dAcc = sysData.fMotorAccTime[Motor_PCB_Y];
				dVel = 40 * sysData.fMotorResol[Motor_PCB_Y];
				AxmMoveVel(Motor_PCB_Y, -dVel, dAcc, dAcc);		// ���� �̵�
				checkTime = myTimer(true);
				MainStep = 410;
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				//sLangChange.LoadStringA(IDS_STRING338);
				sLangChange.Format("PCB Z-Axis Move Fail");
				errMsg2(Task.AutoFlag, sLangChange);
				MainStep = -405;
			}
			break;

		case 410://! Lens Y�� (+) Limit ��ġ �̵�
			if (GetHomeSensor(Motor_PCB_Y))
			{
				StopAxis(Motor_PCB_Y);
			}
			if((GetHomeSensor(Motor_PCB_Y) ||GetNegaSensor(Motor_PCB_Y)) && IsStopAxis(Motor_PCB_Y))
			
			{
				dAcc = sysData.fMotorAccTime[Motor_Lens_Y];
				dVel = 40 * sysData.fMotorResol[Motor_Lens_Y];
				AxmMoveVel(Motor_Lens_Y, dVel, dAcc, dAcc);
				checkTime = myTimer(true);
				MainStep = 500;
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				sLangChange.LoadStringA(IDS_STRING338);
				errMsg2(Task.AutoFlag, sLangChange);
				MainStep = -410;
			}
			break;

		case 500://! Lens X�� (+) Limit ��ġ �̵�
			if (GetHomeSensor(Motor_Lens_Y))
			{
				StopAxis(Motor_Lens_Y);
			}
			if((GetHomeSensor(Motor_Lens_Y) || GetPosiSensor(Motor_Lens_Y) )&& IsStopAxis(Motor_Lens_Y))
			{
				dAcc = sysData.fMotorAccTime[Motor_Lens_X];
				dVel = 20 * sysData.fMotorResol[Motor_Lens_X];
				AxmMoveVel(Motor_Lens_X, dVel, dAcc, dAcc);
				checkTime = myTimer(true);
				MainStep = 550;
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				sLangChange.LoadStringA(IDS_STRING336);
				errMsg2(Task.AutoFlag, sLangChange);
				MainStep = -500;
			}
			break;

		case 550://! PCB Y�� (-) Limit ��ġ �̵�
			if (GetHomeSensor(Motor_Lens_X))
			{
				StopAxis(Motor_Lens_X);
			}
			if((GetHomeSensor(Motor_Lens_X) || GetPosiSensor(Motor_Lens_X) )&& IsStopAxis(Motor_Lens_X))
			{
				dAcc = sysData.fMotorAccTime[Motor_PCB_X];
				dVel = 20 * sysData.fMotorResol[Motor_PCB_X];
				AxmMoveVel(Motor_PCB_X, -dVel, dAcc, dAcc);
				checkTime = myTimer(true);
				MainStep = 600;
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				sLangChange.LoadStringA(IDS_STRING335);
				errMsg2(Task.AutoFlag, sLangChange);
				MainStep = -550; 
			}
			break;
		case 600:
			if (GetHomeSensor(Motor_PCB_X))
			{
				StopAxis(Motor_PCB_X);
			}
			if((GetHomeSensor(Motor_PCB_X) || GetNegaSensor(Motor_PCB_X) )&& IsStopAxis(Motor_PCB_X))
			{
				/*dAcc = sysData.fMotorAccTime[Motor_Lens_Yt];
				dVel = 5 * sysData.fMotorResol[Motor_Lens_Yt];
				AxmMoveVel(Motor_Lens_Yt, dVel, dAcc, dAcc);*/
				/*dAcc = sysData.fMotorAccTime[Motor_PCB_Yt];
				dVel = 5 * sysData.fMotorResol[Motor_PCB_Yt];
				AxmMoveVel(Motor_PCB_Yt, -dVel, dAcc, dAcc);*/

				iStep[Motor_Lens_Y] = 100;	// Lens Y ���� ����
				checkTime = myTimer(true);
				MainStep = 800;
			}
			else if((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				errMsg2(Task.AutoFlag, "[����] Pcb X�� (+)�̵� ����");
				MainStep = -550;
			}
			break;
		case 650:
			//
			//if (GetPosiSensor(Motor_Lens_Yt) && IsStopAxis(Motor_Lens_Yt))
			if (GetNegaSensor(Motor_PCB_Yt) && IsStopAxis(Motor_PCB_Yt))
			{
				/*dAcc = sysData.fMotorAccTime[Motor_PCB_Yt];
				dVel = 5 * sysData.fMotorResol[Motor_PCB_Yt];
				AxmMoveVel(Motor_PCB_Yt, -dVel, dAcc, dAcc);
*/
				
			}
			else if ((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				errMsg2(Task.AutoFlag, "[����] Lens ty�� (-)�̵� ����");
				MainStep = -650;
			}
			
			break;
		case 700: 
			if (GetNegaSensor(Motor_PCB_Yt) && IsStopAxis(Motor_PCB_Yt))
			{
				iStep[Motor_Lens_Y] = 100;	// Lens Y ���� ����
				checkTime = myTimer(true);
				MainStep = 800;
			}
			else if ((curTime - checkTime) > MOTOR_MOVE_TIME)
			{
				sLangChange.LoadStringA(IDS_STRING335);
				errMsg2(Task.AutoFlag, "[����] Pcb ty�� (-)�̵� ����");
				MainStep = -700;
			}
			break;
		case 800:
			//if(m_bOrgFlag[Motor_Lens_Y] == true)
			//{// ��ü Home ����
				iStep[Motor_PCB_Y] = 100;	// PCB Y ���� ����
				iStep[Motor_PCB_X] = 100;
				iStep[Motor_Lens_X] = 100;	
				iStep[Motor_Lens_Z] = 100;	
				iStep[Motor_Lens_Xt] = 100;	
				iStep[Motor_Lens_Yt] = 100;	
				iStep[Motor_PCB_TH] = 100;	
				iStep[Motor_PCB_Xt] = 100;	
				iStep[Motor_PCB_Yt] = 100;	
				iStep[Motor_PCB_Z] = 100;
				MainStep = 900;
			//}
			break;

		case 900:
			if((m_bOrgFlag[Motor_Lens_X] == true) && (m_bOrgFlag[Motor_Lens_Y] == true) && (m_bOrgFlag[Motor_Lens_Z] == true) && (m_bOrgFlag[Motor_Lens_Xt] == true) && 
				(m_bOrgFlag[Motor_Lens_Yt] == true) && (m_bOrgFlag[Motor_PCB_X] == true) && (m_bOrgFlag[Motor_PCB_Y] == true) && (m_bOrgFlag[Motor_PCB_TH] == true) && 
				(m_bOrgFlag[Motor_PCB_Xt] == true) && (m_bOrgFlag[Motor_PCB_Yt] == true) && (m_bOrgFlag[Motor_PCB_Z] == true))
			{//! ��ü ���� �˻� �Ϸ� Check
				MainStep = 1000;
			}
			break;
		}//main switch

		for(int i=0; i< MAX_MOTOR_NO;i++)
		{
			if(iStep[i] >= 100 && iStep[i] < 1000) 
			{
				iStep[i] = HomeSubReqProcess(i, iStep[i], iTime[i]);
				if(iStep[i] < 0)
				{
					StopAxisAll();
					errMsg2(Task.AutoFlag, m_szLogMsg);
					return false;
				}
			}
		}

		if(MainStep == 1000)	break;
		if(MainStep < 0 || MainStep > 1000)
		{
			sLangChange.LoadStringA(IDS_STRING343);
			errMsg2(Task.AutoFlag, sLangChange);	//[����] ��ü ���� ���� ����

			sLangChange.LoadStringA(IDS_STRING1333);
			theApp.MainDlg->m_labelHom.SetText(sLangChange);	//"�������� ����"
			theApp.MainDlg->m_labelHom.SetBkColor(M_COLOR_RED);
			theApp.MainDlg->m_labelHom.Invalidate();

			theApp.MainDlg->m_btnOrigin.m_iStateBtn =2;
			theApp.MainDlg->m_btnOrigin.Invalidate();

			return false;
		}

		if((curTime - OriginStartTime) > MOTOR_ORIGIN_TIME*1000)
		{
			StopAxisAll();
			errMsg2(Task.AutoFlag, "[����] ��ü ���� ���� �ð� �ʰ�");	//[����] ��ü ���� ���� �ð� �ʰ�

			theApp.MainDlg->m_labelHom.SetText("�������� ����");	//"�������� ����"
			theApp.MainDlg->m_labelHom.SetBkColor(M_COLOR_RED);
			theApp.MainDlg->m_labelHom.Invalidate();

			theApp.MainDlg->m_btnOrigin.m_iStateBtn =2;
			theApp.MainDlg->m_btnOrigin.Invalidate();

			return false;
		}
		Sleep(5);
	}

	theApp.MainDlg->m_labelHom.SetText("�������� �Ϸ�");	//"�������� �Ϸ�"
	theApp.MainDlg->m_labelHom.SetBkColor(M_COLOR_GREEN);
	theApp.MainDlg->m_labelHom.Invalidate();

	theApp.MainDlg->m_btnOrigin.m_iStateBtn =1;
	theApp.MainDlg->m_btnOrigin.Invalidate();

	return true;
}

int CAxlMotorSet::HomeCheck()
{
	int iHomeCheckFlag = 0;

	for(int i=0; i<MAX_MOTOR_NO; i++)
	{
		if (bThreadTaskReadyRun == true && i == Motor_Lens_Xt&& i == Motor_Lens_Yt)//if (bThreadTaskReadyRun == true && i==Motor_Lens_Yt && i==Motor_PCB_Yt&& i==Motor_PCB_Xt)	
			continue;
		if(m_bOrgFlag[i]==false)
			iHomeCheckFlag += 0x01 << i;
	}

	Task.iHomeErrorFlag = iHomeCheckFlag;

	return iHomeCheckFlag;
}

bool CAxlMotorSet::Lens_Motor_Align_Move(double offsetX, double offsetY, double offsetTh)
{
	double eTime;
	short axis[4];	/* axis[0] : X , axis[1] : Y , axis[2] : Z , axis[3] : Theta */
	double pos[4];	/* axis[0] : X , axis[1] : Y , axis[2] : Z , axis[3] : Theta */

	char logMsg[1000];

	double d_dist_X		= model.axis[Motor_Lens_X].pos[Lens_Pos] + offsetX ;	
	double d_dist_Y		= model.axis[Motor_Lens_Y].pos[Lens_Pos] + offsetY;	
	
	for (int i=Motor_Lens_X; i<=Motor_Lens_Y; i++)
	{
		if(i==Motor_Lens_Z)
			continue;


		if(!m_bOrgFlag[i])
		{
			sLangChange.LoadStringA(IDS_STRING198);
			sTempLang.Format(sLangChange, MotorName[i]);
			_stprintf_s(logMsg, sTempLang);	//"[Lens ��] %s ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�."
			errMsg2(Task.AutoFlag, logMsg);

			return false;
		}

		axis[i-Motor_Lens_X]	= i;
				
		if(i==Motor_Lens_X)				pos[i-Motor_Lens_X]	= model.axis[i].pos[Lens_Pos] + offsetX;		/* Align ��ġ���� �̵� */
		else if(i==Motor_Lens_Y)		pos[i-Motor_Lens_X]	= model.axis[i].pos[Lens_Pos] + offsetY;		/* Align ��ġ���� �̵� */
	}


	double MotorPos[2];
// 	MotorPos[0] = GetEncoderPos(Motor_Lens_X) - model.axis[Motor_Lens_X].pos[Align_Pos];
// 	MotorPos[1] = GetEncoderPos(Motor_Lens_Y) - model.axis[Motor_Lens_Y].pos[Align_Pos];

	MotorPos[0] = GetEncoderPos(Motor_Lens_X);
	MotorPos[1] = GetEncoderPos(Motor_Lens_Y);

	if(fabs(MotorPos[0]-model.axis[Motor_Lens_X].pos[Lens_Pos]) > model.axis[Motor_Lens_X].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING196);
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_Lens_X].m_dLimit_Err, model.axis[Motor_Lens_X].pos[Lens_Pos], MotorPos[0]);
		errMsg2(Task.AutoFlag, logMsg);
		return false;
	}

	if(fabs(MotorPos[1]-model.axis[Motor_Lens_Y].pos[Lens_Pos]) > model.axis[Motor_Lens_Y].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING197);
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_Lens_Y].m_dLimit_Err, model.axis[Motor_Lens_Y].pos[Lens_Pos], MotorPos[1]);
		errMsg2(Task.AutoFlag, logMsg);
		return false;
	}


	double startTime = myTimer(true);

	int step=0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	while(1)
	{
		switch(step)
		{
		case 0:			
			goMotorPos(2, axis, pos);			// X, Y
			step = 10;
			break;

		case 10:
			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if( IsStopAxis(Motor_Lens_X) && GetInposition(Motor_Lens_X) )		step = 101;
			break;

		case 101:
			if( IsStopAxis(Motor_Lens_Y) && GetInposition(Motor_Lens_Y) )		step = 1000;
			break;		
			
		default:
			break;
		}

		if(step==1000)
			break;


		eTime = myTimer(true);
		if(eTime-startTime > MOTOR_MOVE_TIME)
		{
			if(step>100)
			{
				int motorNo = step - 100;	
				sLangChange.LoadStringA(IDS_STRING723);	//Lens �� [%s ����] �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorName[motorNo]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING734);	//Lens �� �̵� ������
				_stprintf_s(logMsg, sLangChange);
			}
			errMsg2(Task.AutoFlag, logMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}

	return true;
}


bool CAxlMotorSet::Lens_Tilt_Motor_Move(int index, double offsetX, double offsetY)
{
	char logMsg[1000];
	double eTime;
	short axis[3];
	double pos[3];
	int axis_cnt =0;
	double dOffsetLimit = 1.0;
	
	for (int i= Motor_Lens_Yt; i<= Motor_Lens_Xt; i++)
	{
		if(!m_bOrgFlag[i])
		{
			sTempLang.LoadStringA(IDS_STRING108);
			sLangChange.Format(sTempLang, MotorName[i]);
			_stprintf_s(logMsg, sLangChange);
			errMsg2(Task.AutoFlag, logMsg);
			return false;
		}

		/*axis[i-Motor_Lens_Xt]		= i;
	
		if(i==Motor_Lens_Xt)				pos[i-Motor_Lens_Xt]	= model.axis[i].pos[index] + offsetX;
		else if(i==Motor_Lens_Yt)			pos[i-Motor_Lens_Yt]	= model.axis[i].pos[index] + offsetY;*/
	}


	double MotorPos[2];
	
	MotorPos[0] = GetCommandPos(Motor_Lens_Xt) - model.axis[Motor_Lens_Xt].pos[index];
	MotorPos[1] = GetCommandPos(Motor_Lens_Yt) - model.axis[Motor_Lens_Yt].pos[index];
	if(fabs(MotorPos[0]) > model.axis[Motor_Lens_Xt].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING688);	//LENS TX ���Ͱ� Limit�� ���� �����ϴ�. [Encoder %.03lf, Teaching %.03lf]
		_stprintf_s(logMsg, sLangChange, GetCommandPos(Motor_Lens_Xt), model.axis[Motor_Lens_Xt].pos[index]);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}

	if(fabs(MotorPos[1]) > model.axis[Motor_Lens_Yt].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING692);	//LENS TY ���Ͱ� Limit�� ���� �����ϴ�. [Encoder %.03lf, Teaching %.03lf]
		_stprintf_s(logMsg, sLangChange, GetCommandPos(Motor_Lens_Yt), model.axis[Motor_Lens_Yt].pos[index]);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}
	axis[0]		= Motor_Lens_Xt;
	axis[1]		= Motor_Lens_Yt;

	pos[0]	= model.axis[Motor_Lens_Xt].pos[index];// + offsetX;
	pos[1]	= model.axis[Motor_Lens_Yt].pos[index];// + offsetX;

	int step = 0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	double startTime;

	while(1)
	{
		switch(step)
		{
		case 0:									// 2�� ���� ��� ��ġ�� �̵� ���..
			startTime = myTimer(true);
			goMotorPos(2, axis, pos);
			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if( IsStopAxis(Motor_Lens_Xt) )			step = 101;
			break;

		case 101:
			if ( IsStopAxis(Motor_Lens_Yt) )		step = 1000;
			break;

		default:
			break;
		}

		if(step==1000)
			break;
		 

		eTime = myTimer(true);
		if(eTime-startTime > MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_Lens_Xt);
			StopAxis(Motor_Lens_Yt);

			if(step>100)
			{
				int motorNo = step - 100;
				sLangChange.LoadStringA(IDS_STRING723);	//Lens �� [%s ����] �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorName[motorNo]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING735);	//LENS �� ƿ���� �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange);
			}

			errMsg2(Task.AutoFlag,logMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}

	return true;
}

bool CAxlMotorSet::Lens_Motor_Conti_Down(int index, bool waitFlag)
{
	double eTime = 0.0;
	short axis[1];
	double pos[1];
	CString logStr = "";
	double LensDownHeight = 1.0;
	LensDownHeight = fabs(model.m_ContiDownOffset);// (double)atof(logStr);
	double dWaitZPos = model.axis[Motor_Lens_Z].pos[Wait_Pos];
	if (LensDownHeight > 15.0)
	{
		LensDownHeight = 15.0;
	}
	axis[0] = index;
	pos[0] = GetEncoderPos(index) - LensDownHeight;
	logStr.Format("%s ���� �̵� �մϴ�.", MotorPosName[index]);
	int step = 0;
	double startTime = 0.0;
	goMotorPos(1, axis, pos);

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	while (waitFlag)
	{
		switch (step)
		{
		case 0:
			startTime = myTimer(true);
			step = 100;
			break;
		case 100:								// �̵� �Ϸ� Ȯ��..
			if (IsStopAxis(index)) //if (IsStopAxis(Motor_Lens_Z))
			{
				step = 1000;
			}
			break;

		default:
			break;
		}
		if (step == 1000) {
			break;
		}
		eTime = myTimer(true);
		if ((eTime - startTime)>MOTOR_MOVE_TIME)
		{
			if (step>100)
			{
				logStr.Format("%s �̵� �ð� �ʰ�", MotorPosName[index]);
			}
			else
			{
				logStr.Format("%s �̵� �� ������", MotorPosName[index]);
			}
			errMsg2(Task.AutoFlag, logStr);
			return false;
		}
		Sleep(5);
	}
	return true;
}
bool CAxlMotorSet::Lens_Motor_Move(int index, double offsetX, double offsetY)
{
	double eTime;
	short axis[6];
	double pos[6];

	char logMsg[1000];

	for (int i=0; i<MAX_MOTOR_NO; i++)
	{
		if(!m_bOrgFlag[i])
		{
			sLangChange.LoadStringA(IDS_STRING1246);	//���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
			_stprintf_s(logMsg, _T("%s ") + sLangChange, MotorName[i]);
			errMsg2(Task.AutoFlag,logMsg);
			return false;
		}
	}

	if(index<Wait_Pos || index>Bonding_Pos)
	{
		sLangChange.LoadStringA(IDS_STRING729);	//Lens �� ���� �̵� ��ġ�� �������Դϴ�.
		_stprintf_s(logMsg, sLangChange + _T(" [index : %d]"), index);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}


	int step=0;
	double startTime = 0.0;
	

	bool b_Z_Move_Flag = false;
	double d_dist_X = model.axis[Motor_Lens_X].pos[index] - GetEncoderPos(Motor_Lens_X);
	double d_dist_Y = model.axis[Motor_Lens_Y].pos[index] - GetEncoderPos(Motor_Lens_Y);

	if (fabs(d_dist_X)>model.axis[Motor_Lens_X].m_dLimit_Err || fabs(d_dist_Y)>model.axis[Motor_Lens_Y].m_dLimit_Err)
	{
		axis[0] = Motor_Lens_Z;		// Z, Tilx X, Tilt Y �� ��� ��ġ �̵� �� X, Y, ��, Tx, Ty ����, ���� �Ϸ� �� Z ����..
		axis[1] = Motor_Lens_Xt;						
		axis[2] = Motor_Lens_Yt;						

		pos[0] = model.axis[Motor_Lens_Z].pos[index];
		pos[1] = model.axis[Motor_Lens_Xt].pos[index];
		pos[2] = model.axis[Motor_Lens_Yt].pos[index];
		

		step = 0;

		while(1)
		{
			switch(step)
			{
			case 0:									// 5�� ���� ��� ��ġ�� �̵� ���..
				goMotorPos(3, axis, pos);			// Z, Xt, Yt
				startTime = myTimer(true);
				step = 100;
				break;

			case 100:								// �̵� �Ϸ� Ȯ��..
				if ( IsStopAxis(Motor_Lens_Z) )		step = 101;
				break;

			case 101:
				if ( IsStopAxis(Motor_Lens_Xt))		step = 102;
				break;

			case 102:
				if ( IsStopAxis(Motor_Lens_Yt) )	step = 1000;
				break;
				
			default:
				break;
			}

			if(step==1000)
				break;


			eTime = myTimer(true);
			if(eTime-startTime>MOTOR_MOVE_TIME)
			{
				if(step>100)
				{
					int motorNo = step - 100;
					sLangChange.LoadStringA(IDS_STRING722);	//Lens �� [%s ����] %s �̵� �ð� �ʰ�
					_stprintf_s(logMsg, sLangChange, MotorName[motorNo], MotorPosName[index]);
				}
				else
				{
					sLangChange.LoadStringA(IDS_STRING720);	//Lens �� %s �̵� ������
					_stprintf_s(logMsg, sLangChange, MotorPosName[index]);
				}
				errMsg2(Task.AutoFlag, logMsg);

				return false;
			}

			Sleep(5);
			checkMessage();
		}
	}

	for (int i=Motor_Lens_X; i<= Motor_Lens_Z; i++)
	{
		//clear_status(i);
		if(!m_bOrgFlag[i])
		{
			sTempLang.LoadStringA(IDS_STRING108);
			sLangChange.Format(sTempLang, MotorName[i]);
			_stprintf_s(logMsg, sLangChange);
			errMsg2(Task.AutoFlag,logMsg);
			return false;
		}

		axis[i-Motor_Lens_X]	= i;

		if(index == Bonding_Pos)								// Bonding_Pos �̵� �� Align ��ġ���� �ν��� x, y, Theta ������ ����..
		{
			if (fabs(offsetX)>model.axis[Motor_Lens_X].m_dLimit_Err || fabs(offsetY)>model.axis[Motor_Lens_Y].m_dLimit_Err)
			{
				sLangChange.LoadStringA(IDS_STRING733);	//Lens �� �������� �������Դϴ�
				sprintf_s(logMsg, sLangChange, offsetX, offsetY);
				errMsg2(Task.AutoFlag, logMsg);
				return false;
			}

			if(i==Motor_Lens_X)				pos[i-Motor_Lens_X]	= model.axis[i].pos[index] + offsetX;
			else if(i==Motor_Lens_Y)		pos[i-Motor_Lens_X]	= model.axis[i].pos[index] + offsetY;
			else if(i==Motor_Lens_Z)		pos[i-Motor_Lens_X] = model.axis[i].pos[index];
			else							pos[i-Motor_Lens_X]	= model.axis[i].pos[index];
		}
		else
			pos[i-Motor_Lens_X]	= model.axis[i].pos[index];
	}

	int b_bonding_move_flag = 0;

	if (index == Bonding_Pos || index == Loading_Pos)
	{
		b_bonding_move_flag = 1;
	}
	else
	{
		double d_x_pos = abs(GetEncoderPos(Motor_Lens_X) - model.axis[Motor_Lens_X].pos[Bonding_Pos]);
		double d_y_pos = abs(GetEncoderPos(Motor_Lens_Y) - model.axis[Motor_Lens_Y].pos[Bonding_Pos]);
		double d_x2_pos = abs(GetEncoderPos(Motor_Lens_X) - model.axis[Motor_Lens_X].pos[Loading_Pos]);
		double d_y2_pos = abs(GetEncoderPos(Motor_Lens_Y) - model.axis[Motor_Lens_Y].pos[Loading_Pos]);

		// PCB x, y ���Ͱ� ���� ��ġ ��ó�� ������..
		if( (d_x_pos<model.axis[Motor_Lens_X].m_dLimit_Err) && (d_y_pos<model.axis[Motor_Lens_Y].m_dLimit_Err) || (d_x2_pos<model.axis[Motor_Lens_X].m_dLimit_Err) && (d_y2_pos<model.axis[Motor_Lens_Y].m_dLimit_Err))
		{
			b_bonding_move_flag = 2;
		}
		else		b_bonding_move_flag = 0;
	}

	sLangChange.LoadStringA(IDS_STRING721);//Lens �� %s�� �̵� �մϴ�.	
	_stprintf_s(logMsg, sLangChange, MotorPosName[index]);
//	delayMsg(logMsg, 10000, M_COLOR_BLUE);


	startTime = myTimer(true);
	step=0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif
	
	while(1) 
	{
		switch(step)
		{
		case 0:									// Lens ���� ��� ��ġ�� �̵� ���..
			// 1: ������ġ ����, 2: ���� ��ġ���� ���� ��, 0:xy�� ���� ������ ��.
			if(b_bonding_move_flag==1)
			{
				goMotorPos(1, axis, pos);				// x ���ͺ��� �̵�
			}
			else if(b_bonding_move_flag==2)
			{
				goMotorPos(1, axis+1, pos+1);			// Y ���ͺ��� �̵�
			}
			else if(b_bonding_move_flag==0)
			{
				goMotorPos(2, axis, pos);
			}
			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if( ( IsStopAxis(Motor_Lens_X) && GetInposition(Motor_Lens_X) ) && 
				( IsStopAxis(Motor_Lens_Y) && GetInposition(Motor_Lens_Y) ))
			{
				if(b_bonding_move_flag==1)
				{
					goMotorPos(1, axis+1, pos+1);		// Y �̵�..
				}
				else if(b_bonding_move_flag==2)
				{
					goMotorPos(1, axis, pos);			// X �̵�..
				}
				else
					goMotorPos(2, axis, pos);
				step = 101;
			}

			break;

		case 101:
			if( ( IsStopAxis(Motor_Lens_X) && GetInposition(Motor_Lens_X) ) && 
			    ( IsStopAxis(Motor_Lens_Y) && GetInposition(Motor_Lens_Y) ) )
				step = 1000;
			break;
			
		default:
			break;
		}

		if(step==1000)
			break;


		eTime = myTimer(true);
		if(eTime-startTime > MOTOR_MOVE_TIME) 
		{
			if(step>100)
			{
				int motorNo = step - 100;
				sLangChange.LoadStringA(IDS_STRING722);	//Lens �� [%s ����] %s �̵� �ð� �ʰ�
					_stprintf_s(logMsg, sLangChange, MotorName[motorNo], MotorPosName[index]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING720);	//Lens �� %s �̵� ������
					_stprintf_s(logMsg, sLangChange, MotorPosName[index]);
			}
			errMsg2(Task.AutoFlag, logMsg);

			return false;
		}

		Sleep(5);
	}

	return true;
}

bool CAxlMotorSet::Lens_Motor_MoveXY(int index, double offsetX, double offsetY)
{
	double eTime = 0.0;
	short axis[6];
	double pos[6];

	char logMsg[1000];
	CString logStr = _T("");
	for (int i=0; i<MAX_MOTOR_NO; i++)
	{
		if(!m_bOrgFlag[i])
		{
			sLangChange.LoadStringA(IDS_STRING1246);	//���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
			_stprintf_s(logMsg, _T("%s ") + sLangChange, MotorName[i]);
			errMsg2(Task.AutoFlag,logMsg);
			return false;
		}
	}

	if(index<Wait_Pos || index>ContiTailDelay_Pos)
	{
		sLangChange.LoadStringA(IDS_STRING729);	//Lens �� ���� �̵� ��ġ�� �������Դϴ�.
		_stprintf_s(logMsg, sLangChange + _T(" [index : %d]"), index);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}
	if(index==Bonding_Pos)
	{
		double dMotor = GetEncoderPos(Motor_PCB_Y)/*GetCommandPos(Motor_PCB_Y)*/ - model.axis[Motor_PCB_Y].pos[index];

		if(dMotor > -model.axis[Motor_PCB_Y].m_dLimit_Err)
		{
			//bool b_pcb_push_state = Dio.PCBCoverCloseCheck(true, false);
			//if (b_pcb_push_state == false)
			//{
			//	sLangChange.LoadStringA(IDS_STRING1026);	//PCB�� Ŀ�� ������ ���� �ֽ��ϴ�.
			//	_stprintf_s(logMsg, sLangChange);
			//	errMsg2(Task.AutoFlag, logMsg);
			//	return false;
			//}
		}

		//Dio.BackLightOn(false, true);
	}

	int step=0;
	double startTime = 0.0;
	double lensPosX = 0.0;
	double lensPosY = 0.0;
	
	double lensBondPosX = 0.0;
	double lensBondPosY = 0.0;

	double lensCurrentPosY = 0.0;
	bool b_Z_Move_Flag = false;
	double d_dist_X = model.axis[Motor_Lens_X].pos[index] - GetEncoderPos(Motor_Lens_X);
	double d_dist_Y = model.axis[Motor_Lens_Y].pos[index] - GetEncoderPos(Motor_Lens_Y);


	axis[0] = Motor_Lens_X;						
	axis[1] = Motor_Lens_Y;						

	pos[0] = model.axis[Motor_Lens_X].pos[index];
	pos[1] = model.axis[Motor_Lens_Y].pos[index];

	step = 0;

	int MoveType = 0;//0 = x,y���� �̵� , 1 = y�� ����  , 2 = x�� ���� �̵�
#if 0
	lensPosX = (float)model.axis[Motor_Lens_X].pos[Loading_Pos] - (float)GetEncoderPos(Motor_Lens_X);
	lensPosY = (float)model.axis[Motor_Lens_Y].pos[Loading_Pos] - (float)GetEncoderPos(Motor_Lens_Y);

	lensBondPosX = (float)model.axis[Motor_Lens_X].pos[Bonding_Pos] - (float)GetEncoderPos(Motor_Lens_X);
	lensBondPosY = (float)model.axis[Motor_Lens_Y].pos[Bonding_Pos] - (float)GetEncoderPos(Motor_Lens_Y);

	lensCurrentPosY = (float)GetEncoderPos(Motor_Lens_Y);
	if (fabs(lensPosX) < 1.1 && fabs(lensPosY) < 1.1)
	{
		MoveType = 1;//���� ��ġ�� �ε���ġ�� y�� ���� �ڷ� �����ߵȴ�.����׸��۰� ��� ���ΰ� �������־.
	}
	else if (fabs(lensBondPosX) < 1.1 && fabs(lensBondPosY) < 1.1)
	{
		MoveType = 1;//���� ��ġ�� ������ġ�� y�� ���� �ڷ� �����ߵȴ�.����׸��۰� ��� ���ΰ� �������־.
	}
	else
	{
		if (index == Loading_Pos)
		{
			MoveType = 2;//x����� �̵��� y�� �̵� , ���� ������ ���� ���� ���̷� �׸��۰� ���ƾߵż� x���� ���߰� y�� ����
		}else if (index == Bonding_Pos)
		{
			if (lensCurrentPosY < (float)model.axis[Motor_Lens_Y].pos[Bonding_Pos])
			{
				//cl�� ���� pcb�ʿ� �ֵ�.
				//���⼭ ���������� y�����
				MoveType = 1;
			}
			else
			{
				MoveType = 2;
			}
		}
		
		else if (index == Wait_Pos)
		{
			MoveType = 2;// 1;//y�� ���� �̵��� x�� �̵�
		}
		else
		{

			MoveType = 1;
		}
	}
#endif
	while(1)
	{
		switch(step)
		{
		case 0:									// 2�� ���� ��� ��ġ�� �̵� ���..
			 if (MoveType == 1)
			{
				goMotorPos(1, axis+1, pos+1);//y�̵�
			}
			else if (MoveType == 2)
			{
				goMotorPos(1, axis, pos);//x�̵�
			}
			else
			{
				goMotorPos(2, axis, pos);//x, y ���� �̵�
			}
			
			startTime = myTimer(true);
			step = 100;
			break;
		case 100:
			if (MoveType == 1)
			{
				if (IsStopAxis(Motor_Lens_Y))
				{
					goMotorPos(1, axis, pos);
					step = 101;
				}
				
			}
			else if (MoveType == 2)
			{
				if (IsStopAxis(Motor_Lens_X))
				{
					goMotorPos(1, axis+1, pos+1);
					step = 101;
				}
			}
			else
			{
				if (IsStopAxis(Motor_Lens_X))
				{
					step = 101;
				}
			}
			
			break;

		case 101:
			if (MoveType == 1)
			{
				if (IsStopAxis(Motor_Lens_X))
				{
					step = 1000;
				}
			}
			else if (MoveType == 2)
			{
				if (IsStopAxis(Motor_Lens_Y))
				{
					step = 1000;
				}
			}
			else
			{
				if (IsStopAxis(Motor_Lens_Y))
				{
					step = 1000;
				}
			}
			
			break;
				
		default:
			break;
		}

		if(step==1000)
			break;


		eTime = myTimer(true);
		if(eTime-startTime>MOTOR_MOVE_TIME)
		{
			if(step>100)
			{
				int motorNo = step - 100;
				_stprintf_s(logMsg, "Lens �� [%s ����] %s �̵� �ð� �ʰ�", MotorName[motorNo], MotorPosName[index]);
			}
			else
			{
				_stprintf_s(logMsg, "Lens �� %s �̵� ������", MotorPosName[index]);
			}
			errMsg2(Task.AutoFlag, logMsg);

			return false;
		}

		Sleep(5);
	}

	return true;

}


bool CAxlMotorSet::Pcb_Motor_Align_Move(double offsetX, double offsetY, double offsetTh)
{
	char logMsg[1000];
	double eTime;
	short axis[4];
	double pos[4];
	int axis_cnt =0;
	double dOffsetLimit = 1.0;

	
	for (int i=Motor_PCB_X; i<=Motor_PCB_TH; i++)
	{
		if (i == Motor_PCB_Z)continue;
		if(!m_bOrgFlag[i])
		{
			sLangChange.LoadStringA(IDS_STRING1001);	//PCB �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
			_stprintf_s(logMsg, _T("%s ") + sLangChange, MotorName[i]);
			errMsg2(Task.AutoFlag, logMsg);
			return false;
		}

		axis[i-Motor_PCB_X]		= i;
	
		if(i==Motor_PCB_X)				pos[i-Motor_PCB_X]	= model.axis[i].pos[Align_Pos] + offsetX;
		else if(i==Motor_PCB_Y)			pos[i-Motor_PCB_X]	= model.axis[i].pos[Align_Pos] + offsetY;
		else if(i==Motor_PCB_TH)		pos[i-Motor_PCB_X]	= model.axis[i].pos[Align_Pos] + offsetTh;
	}

	double MotorPos[2];
	MotorPos[0] = GetEncoderPos(Motor_PCB_X) - model.axis[Motor_PCB_X].pos[Align_Pos];
	MotorPos[1] = GetEncoderPos(Motor_PCB_Y) - model.axis[Motor_PCB_Y].pos[Align_Pos];

	if(fabs(MotorPos[0]) > model.axis[Motor_PCB_X].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING970);	//PCB X ���Ͱ� Align ���� Limit�� ���� �����ϴ�. [Encoder %.03lf, Teaching %.03lf]
		_stprintf_s(logMsg, sLangChange, GetEncoderPos(Motor_PCB_X), model.axis[Motor_PCB_X].pos[Align_Pos]);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}

	if(fabs(MotorPos[1]) > model.axis[Motor_PCB_Y].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING973);	//PCB Y ���Ͱ� Align ���� Limit�� ���� �����ϴ�. [Encoder %.03lf, Teaching %.03lf]
		_stprintf_s(logMsg, sLangChange, GetEncoderPos(Motor_PCB_Y), model.axis[Motor_PCB_Y].pos[Align_Pos]);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}

	int step=0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif


	double startTime;

	while(1)
	{
		switch(step)
		{
		case 0:									// 3�� ���� ��� ��ġ�� �̵� ���..
			startTime = myTimer(true);
			goMotorPos(3, axis, pos);
			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if( IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X) )			step = 101;
			break;

		case 101:
			if ( IsStopAxis(Motor_PCB_Y) && GetInposition(Motor_PCB_Y) )		step = 102;
			break;

		case 102:
			if ( IsStopAxis(Motor_PCB_TH) )		step = 1000;
			break;

		default:
			break;
		}

		if(step==1000)
			break;


		eTime = myTimer(true);
		if(eTime-startTime > MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_Y);
			StopAxis(Motor_PCB_TH);

			if(step>100)
			{
				int motorNo = step - 100;
				sLangChange.LoadStringA(IDS_STRING1482);	//PCB �� [%s ����] �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorName[motorNo]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING994);	//PCB �� Align ���� �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange);
			}

			errMsg2(Task.AutoFlag,logMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}


#ifdef ON_LINE_MOTOR
	double curPos = 0.0;
	for (int i=Motor_PCB_X; i<=Motor_PCB_TH; i++)
	{
		if (i == Motor_PCB_Z)continue;
		if (MOTOR_TYPE[i] == STEPING)
		{
			curPos = GetCommandPos(axis[i - Motor_PCB_X]);
		}
		else
		{
			curPos = GetEncoderPos(axis[i - Motor_PCB_X]);
		}

		double cmdPos = GetCommandPos(axis[i-Motor_PCB_X]);

		double errVal = fabs(pos[i-Motor_PCB_X] - curPos);

		if (errVal>IN_POSITION_VAL)
		{
			sLangChange.LoadStringA(IDS_STRING995);	//PCB �� Align ���� �̵� �� %s ���� ��ġ ���� �߻�
			_stprintf_s(logMsg, sLangChange, MotorName[i], pos[i], curPos, cmdPos);
			LogSave(logMsg);

			checkMessage();


			for(int j=0; j<10; j++)
			{
				Sleep(5);
				checkMessage();
			}

			if (MOTOR_TYPE[i] == STEPING)
			{
				curPos = GetCommandPos(axis[i - Motor_PCB_X]);
			}
			else
			{
				curPos = GetEncoderPos(axis[i - Motor_PCB_X]);
			}
			cmdPos = GetCommandPos(axis[i-Motor_PCB_X]);
			 
			errVal = fabs(pos[i-Motor_PCB_X] - curPos);

			if (errVal>IN_POSITION_VAL)
			{
				sLangChange.LoadStringA(IDS_STRING995);	//PCB �� Align ���� �̵� �� %s ���� ��ġ ���� �߻�
				_stprintf_s(logMsg, sLangChange, MotorName[i], pos[i], curPos, cmdPos);
				errMsg2(Task.AutoFlag, logMsg);

				return false;
			}		

			return false;
		}
	}
#endif


	return true;
}

bool CAxlMotorSet::Dispense_XY_Motor_Move(int index, double dDispenseSpeed, double offsetX, double offsetY, double offsetTh)
{
	char logMsg[1000];
	double eTime = 0.0;
	short axis[3];
	double pos[3];
	int axis_cnt = 0;

	if (!m_bOrgFlag[Motor_PCB_X] || !m_bOrgFlag[Motor_PCB_Y] || !m_bOrgFlag[Motor_PCB_TH])
	{
		sLangChange.LoadStringA(IDS_STRING1001);	//PCB �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
		_stprintf_s(logMsg, sLangChange);
		errMsg2(Task.AutoFlag, logMsg);
		return false;
	}

	if (index<Wait_Pos || index >= MAX_POS_NO)
	{
		sLangChange.LoadStringA(IDS_STRING1000);	//PCB �� ���� �̵� ��ġ�� �������Դϴ�
		_stprintf_s(logMsg, sLangChange, index);
		LogSave(logMsg);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}

	if (Dio.LaserCylinderCheck(false, false))
	{
		sLangChange.LoadStringA(IDS_STRING1497);	//LASER �Ǹ����� �ϰ� ���� �Դϴ�.
		_stprintf_s(logMsg, sLangChange);
		errMsg2(Task.AutoFlag, logMsg);
		return false;
	}




	double zPos = 0.0;

	
	if (fabs(offsetX)>model.axis[Motor_PCB_X].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING997);	//PCB �� X �� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_PCB_X].m_dLimit_Err, offsetX);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}

	if (fabs(offsetY)>model.axis[Motor_PCB_Y].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING998);	//PCB �� Y �� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�.
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_PCB_Y].m_dLimit_Err, offsetY);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}

	if (fabs(offsetTh)>model.axis[Motor_PCB_TH].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING999);	//PCB �� ���� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_PCB_TH].m_dLimit_Err, offsetTh);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}

	axis[0] = Motor_PCB_X;
	axis[1] = Motor_PCB_Y;
	axis[2] = Motor_PCB_TH;

	pos[0] = model.axis[Motor_PCB_X].pos[index] + offsetX;
	pos[1] = model.axis[Motor_PCB_Y].pos[index] + offsetY;
	pos[2] = model.axis[Motor_PCB_TH].pos[index] + offsetTh;

	_stprintf_s(logMsg, "PCB X,Y %s�� �̵� �մϴ�.", MotorPosName[index]);
	//	delayMsg(logMsg, 10000, M_COLOR_BLUE);


	int step = 0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	double startTime;

	int b_bonding_move_flag = 1;
	while (1)
	{
		switch (step)
		{
		case 0:									// 3�� ���� ��� ��ġ�� �̵� ���..
			startTime = myTimer(true);

			DispenseGoMotorPos(3, axis, pos, dDispenseSpeed);

			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if ((IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X)) &&
				(IsStopAxis(Motor_PCB_Y) && GetInposition(Motor_PCB_Y)) &&
				(IsStopAxis(Motor_PCB_TH)))
			{
				step = 101;
			}

			break;

		case 101:

			step = 1000;
			break;

		default:
			break;
		}

		if (step == 1000)
			break;


		eTime = myTimer(true);
		if (eTime - startTime > MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_Y);
			StopAxis(Motor_PCB_TH);

			if (step>100)
			{
				int motorNo = step - 100;
				sLangChange.LoadStringA(IDS_STRING1479);	//PCB �� [%s ����] %s �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorName[motorNo], MotorPosName[index]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING992);	//PCB �� %s �̵� ������
				_stprintf_s(logMsg, sLangChange, MotorPosName[index]);
			}

			errMsg2(Task.AutoFlag, logMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}

	return true;
}
bool CAxlMotorSet::Pcb_Motor_Move(int index, double offsetX, double offsetY, double offsetTh)
{
	char logMsg[1000];
	double eTime = 0.0;
	short axis[3];
	double pos[3];
	int axis_cnt = 0;

	if(!m_bOrgFlag[Motor_PCB_X] || !m_bOrgFlag[Motor_PCB_Y] || !m_bOrgFlag[Motor_PCB_TH])
	{
		sLangChange.LoadStringA(IDS_STRING1001);	//PCB �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
		_stprintf_s(logMsg, sLangChange);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}

	if(index<Wait_Pos || index>=MAX_POS_NO)
	{
		sLangChange.LoadStringA(IDS_STRING1000);	//PCB �� ���� �̵� ��ġ�� �������Դϴ�
		_stprintf_s(logMsg, sLangChange, index);
		LogSave(logMsg);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}

	if (Dio.LaserCylinderCheck(false, false))
	{
		sLangChange.LoadStringA(IDS_STRING1497);	//LASER �Ǹ����� �ϰ� ���� �Դϴ�.
		_stprintf_s(logMsg, sLangChange);
		errMsg2(Task.AutoFlag, logMsg);
		return false;
	}

	if(index == Bonding_Pos)
	{
		//bool  b_PCB_cover_push_down = Dio.PCBCoverCloseCheck(true, false);
		//if(b_PCB_cover_push_down==false)
		//{
		//	sLangChange.LoadStringA(IDS_STRING1026);	//PCB�� Ŀ�� ������ ���� �ֽ��ϴ�.
		//	_stprintf_s(logMsg, sLangChange);
		//	errMsg2(Task.AutoFlag,logMsg);

		//	return false;
		//}
	}


	double MotorPos[3];
	MotorPos[0] = fabs(GetEncoderPos(Motor_Lens_X) - model.axis[Motor_Lens_X].pos[Bonding_Pos]);
	MotorPos[1] = fabs(GetEncoderPos(Motor_Lens_Y) - model.axis[Motor_Lens_Y].pos[Bonding_Pos]);
	MotorPos[2] = fabs(GetCommandPos(Motor_Lens_Z) - model.axis[Motor_Lens_Z].pos[Wait_Pos]);

	// Lens�� X,Y ���� ��ġ�� ��� , Lens Z �� Ȯ�� 
	if(MotorPos[0] < model.axis[Motor_Lens_X].m_dLimit_Err && MotorPos[1] < model.axis[Motor_Lens_Y].m_dLimit_Err)
	{
		if (MotorPos[2] > 1)		// PCB�� ���� ��ġ�� �̵� �Ҷ� Lens Z�� ��� ��ġ Ȯ��
		{
			//sLangChange.LoadStringA(IDS_STRING719);	//Lens ���Ͱ� ���� ��ġ�� �ֽ��ϴ�.
			//_stprintf_s(logMsg, sLangChange);
			errMsg2(Task.AutoFlag, "Lens Z�� ��Ⱑ �ƴմϴ�.");
			return false;
		}

			
	}


	double zPos = 0.0;
	
	// ������ġ�� ��� ���ų�, ���� ���� X �� ���� �̵� �� Y ���� �̵� ����..
	//�����Ӵ� Ȯ��.
	int b_bonding_move_flag = 0;

	if (index == Bonding_Pos || index == Loading_Pos)
	{
		b_bonding_move_flag = 1;
	}
	else
	{
		double d_x_pos = abs(GetEncoderPos(Motor_PCB_X) - model.axis[Motor_PCB_X].pos[Bonding_Pos]);
		double d_y_pos = abs(GetEncoderPos(Motor_PCB_Y) - model.axis[Motor_PCB_Y].pos[Bonding_Pos]);
		double d_x2_pos = abs(GetEncoderPos(Motor_PCB_X) - model.axis[Motor_PCB_X].pos[Loading_Pos]);
		double d_y2_pos = abs(GetEncoderPos(Motor_PCB_Y) - model.axis[Motor_PCB_Y].pos[Loading_Pos]);

		// PCB x, y ���Ͱ� ���� ��ġ ��ó�� ������..
		if( (d_x_pos<model.axis[Motor_PCB_X].m_dLimit_Err) && (d_y_pos<model.axis[Motor_PCB_Y].m_dLimit_Err) || (d_x2_pos<model.axis[Motor_PCB_X].m_dLimit_Err) && (d_y2_pos<model.axis[Motor_PCB_Y].m_dLimit_Err))
		{
			b_bonding_move_flag = 2;
		}
		//b_bonding_move_flag = 2;
		else		b_bonding_move_flag = 0;
	}

	if(fabs(offsetX)>model.axis[Motor_PCB_X].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING997);	//PCB �� X �� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_PCB_X].m_dLimit_Err, offsetX);
		errMsg2(Task.AutoFlag,logMsg);

		return false;
	}

	if(fabs(offsetY)>model.axis[Motor_PCB_Y].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING998);	//PCB �� Y �� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�.
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_PCB_Y].m_dLimit_Err, offsetY);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}

	if(fabs(offsetTh)>model.axis[Motor_PCB_TH].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING999);	//PCB �� ���� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_PCB_TH].m_dLimit_Err, offsetTh);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}

	if (index == Bonding_Pos && sysData.m_iProductComp == 1)//if (index == Bonding_Pos && (Task.dAAPcbMotor[0] != 0 && Task.dAAPcbMotor[1] != 0 && Task.dAAPcbMotor[2]!=0))
	{
		axis[0] = Motor_PCB_X;
		axis[1] = Motor_PCB_Y;
		axis[2] = Motor_PCB_TH;
#if (____AA_WAY == PCB_TILT_AA)
		pos[0]	= Task.dAAPcbMotor[0] + offsetX;	//PCB X access ����� ��
		pos[1]	= Task.dAAPcbMotor[1] + offsetY;	//PCB Y
		pos[2]	= Task.dAAPcbMotor[2] + offsetTh;	//PCB TH 
#elif (____AA_WAY == LENS_TILT_AA)
		pos[0]	= model.axis[Motor_PCB_X].pos[index] + offsetX;
		pos[1]	= model.axis[Motor_PCB_Y].pos[index] + offsetY;
		pos[2]	= model.axis[Motor_PCB_TH].pos[index] + offsetTh;
#endif
	}
	else
	{
		axis[0] = Motor_PCB_X;
		axis[1] = Motor_PCB_Y;
		axis[2] = Motor_PCB_TH;

		pos[0]	= model.axis[Motor_PCB_X].pos[index] + offsetX;
		pos[1]	= model.axis[Motor_PCB_Y].pos[index] + offsetY;
		pos[2]	= model.axis[Motor_PCB_TH].pos[index] + offsetTh;
		
	}

	sLangChange.LoadStringA(IDS_STRING993);	//PCB �� %s�� �̵� �մϴ�.
	_stprintf_s(logMsg, sLangChange, MotorPosName[index]);
	//	delayMsg(logMsg, 10000, M_COLOR_BLUE);


	int step=0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	double startTime;


	while(1)
	{
		switch(step)
		{
		case 0:									// 3�� ���� ��� ��ġ�� �̵� ���..
			startTime = myTimer(true);

			// 1: ������ġ ����, 2: ���� ��ġ���� ���� ��, 0:xy�� ���� ������ ��.
			if(b_bonding_move_flag==1)
			{
				goMotorPos(1, axis, pos);				// x ����
				goMotorPos(1, axis + 2, pos + 2);			// Th ����
				//if(index!=Loading_Pos)
				//{
				//	goMotorPos(1, axis+2, pos+2);			// Th ����
				//}
				
			}
			else if(b_bonding_move_flag==2)
			{
				goMotorPos(1, axis+1, pos+1);			// Y ����
			}
			else if(b_bonding_move_flag==0)
			{
				goMotorPos(3, axis, pos);
			}

			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if( ( IsStopAxis(Motor_PCB_X) ) && 
				( IsStopAxis(Motor_PCB_Y)) && 
				( IsStopAxis(Motor_PCB_TH) ))//&& GetInposition(Motor_PCB_TH) ) )
			{
				if(b_bonding_move_flag==1)
				{
					goMotorPos(1, axis+1, pos+1);		// Y �̵�..
				}
				else if(b_bonding_move_flag==2)
				{
					goMotorPos(1, axis, pos);			// X �̵�..
					goMotorPos(1, axis+2, pos+2);		// Th �̵�..
				}
				else
					goMotorPos(3, axis, pos);
				step = 101;
			}

			break;

		case 101:
			if( ( IsStopAxis(Motor_PCB_X)  ) && 
			    ( IsStopAxis(Motor_PCB_Y)  ) )
				step = 102;
			break;

		case 102:
			if( IsStopAxis(Motor_PCB_TH))// && GetInposition(Motor_PCB_TH) )
				step = 1000;
			break;

		default:
			break;
		}

		if(step==1000)
			break;


		eTime = myTimer(true);
		if(eTime-startTime > MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_Y);
			StopAxis(Motor_PCB_TH);

			if(step>100)
			{
				int motorNo = step - 100;
				sLangChange.LoadStringA(IDS_STRING1479);	//PCB �� [%s ����] %s �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorName[motorNo], MotorPosName[index]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING992);	//PCB �� %s �̵� ������
				_stprintf_s(logMsg, sLangChange, MotorPosName[index]);
			}

			errMsg2(Task.AutoFlag,logMsg);

			return false;
		}

		Sleep(5);
		checkMessage();  
	}
	
	return true;
}


bool CAxlMotorSet::LENS_Z_Motor_Move(int index, bool evasionMode)
{
	double eTime = 0.0;
	short axis[1];
	double pos[1];

	char logMsg[1000];

	double lensPosX = 0.0;
	double lensPosY = 0.0;
		
	if(index == Loading_Pos) 
	{
		bool b_lens_grip_closed = Dio.LensMotorGripCheck(false, false);

		if(b_lens_grip_closed==false)
		{
			sLangChange.LoadStringA(IDS_STRING666);	//Lens Grip Close �����Դϴ�
			errMsg2(Task.AutoFlag, sLangChange);
			return false;
		}

	}
	
	if(!m_bOrgFlag[Motor_Lens_Z])
	{
		sLangChange.LoadStringA(IDS_STRING726);	//Lens �� Z ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
		_stprintf_s(logMsg, sLangChange);
		errMsg2(Task.AutoFlag,logMsg);

		return false;
	}

	if(index == Loading_Pos)
	{
		for(int i=Motor_Lens_X; i<= Motor_Lens_Xt; i++)
		{
			double dCheckPos = 0.0;

			if (i == Motor_Lens_Xt || i == Motor_Lens_Yt)		dCheckPos = GetCommandPos(i);
			else						dCheckPos = GetEncoderPos(i);	//GetCommandPos(i);
	 
			if(fabs(dCheckPos-model.axis[i].pos[Loading_Pos])>1)
			{
				sLangChange.LoadStringA(IDS_STRING731);	//Lens �� ���Ͱ� Loading ��ġ�� ��� �ֽ��ϴ�.
				_stprintf_s(logMsg, sLangChange);
				errMsg2(Task.AutoFlag, logMsg);
				return false;
			}

		}

		pos[0]	= model.axis[Motor_Lens_Z].pos[Loading_Pos];
	}
	else if(index == Bonding_Pos)
	{
		for(int i=Motor_Lens_X; i<= Motor_Lens_Xt; i++)
		{
			double dCheckPos = 0.0;

			if (i == Motor_Lens_Xt || i == Motor_Lens_Yt)
				dCheckPos = GetCommandPos(i);
			else
				dCheckPos = GetEncoderPos(i);	
			
			if(i == Motor_Lens_Z)		// z�� ���� ��ġ�� ���� ��� �ٽ� ���� ��ġ�� �̵� �϶� ���� ��� error �߻�.
			{
				if(fabs(dCheckPos-model.axis[i].pos[Bonding_Pos])< model.axis[i].m_dLimit_Err)
				{
					sLangChange.LoadStringA(IDS_STRING727);	//Lens �� Z�� ���Ͱ� Bonding ��ġ�� �ֽ��ϴ�.
					_stprintf_s(logMsg, sLangChange);
					errMsg2(Task.AutoFlag,logMsg);
					return false;
				}
			}
			else	// Lens �ٸ� ����..
			{
				if(fabs(dCheckPos-model.axis[i].pos[Bonding_Pos]) > model.axis[i].m_dLimit_Err)
				{
					sLangChange.LoadStringA(IDS_STRING730);	//Lens �� ���Ͱ� Bonding ��ġ�� ��� �ֽ��ϴ�.
					_stprintf_s(logMsg, sLangChange);
					errMsg2(Task.AutoFlag, logMsg);

					return false;
				}
			}
		}
		
		pos[0]	= model.axis[Motor_Lens_Z].pos[Bonding_Pos];
	}
	else
	{
		
		pos[0] = model.axis[Motor_Lens_Z].pos[index];
	}
	axis[0] = Motor_Lens_Z;

	sLangChange.LoadStringA(IDS_STRING1480);	//Lens �� %s ���� �̵� �մϴ�.
	_stprintf_s(logMsg, sLangChange, MotorPosName[Motor_Lens_Z]);


	int step=0;
	
	double startTime;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	while(1)
	{
		switch(step)
		{
		case 0:
			startTime = myTimer(true);
			goMotorPos(1, axis, pos);
			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if(IsStopAxis(Motor_Lens_Z))
				step = 1000;
			break;

		default:
			break;
		}

		if(step==1000)
			break;


		eTime = myTimer(true);
		if((eTime-startTime)>MOTOR_MOVE_TIME)
		{
			if(step>100)
			{
				sLangChange.LoadStringA(IDS_STRING723);	//Lens �� [%s ����] �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorPosName[Motor_Lens_Z]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING720);	//Lens �� %s �̵� ������
				_stprintf_s(logMsg, sLangChange, MotorPosName[Motor_Lens_Z]);
			}

			errMsg2(Task.AutoFlag, logMsg);

			return false;
		}

		checkMessage();
		Sleep(5);
	}

	return true;
}
bool CAxlMotorSet::func_Epoxy_Rect_Circle_Draw()
{
	int step = 0;
	double startTime = 0.0;
	double eTime = 0.0;
	double curTime = 0.0;
	DWORD dwReturn = 0;
	long axis[2];
	double acc = 0.0;
	double dec = 0.0;
	double MoveStartTime = 0.0;
	long lSize = 2, lCoordinate = 0;
	double dTimeAngle = 90.0;
	double dEndDeg1ToTime = 0.0;	//1���� �ɸ��� �ð� 
	CString sMsg = "";
	axis[0] = Motor_PCB_X; //���ȣ
	axis[1] = Motor_PCB_Y;
	double dMidPos[2];
	double dEndPos[2];
	acc = model.dAcc;		//! �����ӵ�
	dec = model.dAcc;
	//
	dEndDeg1ToTime = (( (model.dRadiusLength * 1) * M_PI) / (sqrt(model.dEpoxySpeed  * model.dEpoxySpeed /2)) ) /360;
	//1���� �ɸ��� �ð� = ������ / �ӵ� / 360
	while(1)
	{
		
			switch(step)
			{
			case 0:
				if(Straight_Dispense_MoveAxis(Motor_PCB_X, REL, (model.dDisLength[0]) * -1, model.dEpoxySpeed, model.dAcc))//bottom Line
				{
					startTime = myTimer(true);
					step = 100;
				}
				break;
					
			case 100:									
				curTime = myTimer(true);	//���� �ð��� �� ���� On���� ��� �ð��� ����
				if( curTime-startTime > model.dOnTime * 1000 )	//���� Delay �ð� ��� �� ����
				{
					step = 110;
				}			
				break;

			case 110:									
				startTime = myTimer(true);
				if( Dio.EpoxyDischargeOn(true, true) )		//Epoxy ���� ON 
				{
					step = 120;
				}
				else 
				{
					sLangChange.LoadStringA(IDS_STRING188);
					sMsg.Format(sLangChange);	//"[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� ON ��ȣ"
					errMsg2(Task.AutoFlag, sMsg);
					step = -0;
				}
				break;
			case 120:
				if( IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y))
				{
					Pcb_Motor_CircleMove(dTimeAngle, false, 100);//��1 �����ϴ�
					
					step = 130;
				}
				
				break;
			case 130:
				if( IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) )
				{
					AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
					AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);
					if(Straight_Dispense_MoveAxis(Motor_PCB_Y, REL, (model.dDisLength[1]) * -1, model.dEpoxySpeed, model.dAcc))//right Line
					{
						step = 135;
					}
				}
				break;
			case 135:
				if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y))
				{
					//�� �׸����� ����
					if (Straight_Dispense_MoveAxis(Motor_PCB_X, REL, (model.dDisLength[4]) * 1, model.dEpoxySpeed, model.dAcc))
					{
						step = 140;
					}
				}
				break;
			case 140:
				//��2 �������
				if( IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) )
				{
					Pcb_Motor_CircleMove(dTimeAngle, false, 200);//���� ���
					
					step = 145;
				}
				break;
			case 145:
				if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y))
				{
					//�� �׸����� ����
					if (Straight_Dispense_MoveAxis(Motor_PCB_Y, REL, (model.dDisLength[5]) * -1, model.dEpoxySpeed, model.dAcc))
					{
						step = 150;
					}
				}
				break;
			case 150:
				if( IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) )
				{
					AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
					AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);
					if(Straight_Dispense_MoveAxis(Motor_PCB_X, REL, (model.dDisLength[2]) * 1, model.dEpoxySpeed, model.dAcc))//top Line
					{
						step = 160;
					}
				}
				break;
			case 160:
				//��3 �������
				if( IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) ) 
				{
					Pcb_Motor_CircleMove(dTimeAngle, false, 300);	//���� ���
					
					step = 170;
				}
				break;
			case 170:
				if( IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) )
				{
					AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
					AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);
					if(Straight_Dispense_MoveAxis(Motor_PCB_Y, REL, (model.dDisLength[3]) * 1, model.dEpoxySpeed, model.dAcc))//left Line
					{
						step = 175;
						
					}
				}
				break;
			case 175:
				if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y))
				{
					//�� �׸����� ����
					if (Straight_Dispense_MoveAxis(Motor_PCB_X, REL, (model.dDisLength[6]) * -1, model.dEpoxySpeed, model.dAcc))
					{
						step = 180;
					}
				}
				break;
			case 180:
				if( IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) ) 
				{
					Pcb_Motor_CircleMove(dTimeAngle, false, 400);	//���� �ϴ�
					step = 190;
					
					MoveStartTime = myTimer(true);
				}
				break;
			case 190:
				if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y))
				{
					//�� �׸����� ����
					if (Straight_Dispense_MoveAxis(Motor_PCB_Y, REL, (model.dDisLength[7]) * 1, model.dEpoxySpeed, model.dAcc))
					{
						step = 200;
					}
				}
				break;
			case 200:// ���� ������ ���� OFF Time ó��

				step = 400;
				break;
			case 400:
				if( IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) )
				{
					curTime = myTimer(true); 
					if( (dEndDeg1ToTime*dTimeAngle - model.dOffTime) * 1000  <  (curTime - MoveStartTime))
					{
						if( !Dio.EpoxyDischargeOn(false, false) )		//Epoxy ���� Off
						{
							sLangChange.LoadStringA(IDS_STRING187);
							sMsg.Format(sLangChange);	//"[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ"
							errMsg2(Task.AutoFlag, sMsg);
							return false;
						}
						Dio.EpoxyDischargeOn(false, false);
						Sleep(model.dOffTime);
					
						step = 410;	
					}
				}
				break;
			case 410:
				if( ( IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X) ) && 
					( IsStopAxis(Motor_PCB_Y) && GetInposition(Motor_PCB_Y) ) )
				{
					AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
					AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);

					if(Straight_Dispense_MoveAxis(Motor_PCB_X, REL, model.dDisLength[0] * -1 * 0.5, model.dEpoxySpeed, model.dAcc))	//�߰� ����
					{
						step = 500;	
					}	
				}
				break;
			case 500: //���� ���� �Ϸ���� ���
				if( ( IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X) ) && 
					( IsStopAxis(Motor_PCB_Y) && GetInposition(Motor_PCB_Y) ) )
					{
						step = 1000;
					}
				break;
			default:
				step = 1000;
				break;
			}	

		if(step==1000)		break;

		eTime = myTimer(true);
		if(eTime-startTime > 2000000)
		{
			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_Y);

			if(step>100)
			{
				int motorNo = step - 100;
				sMsg.Format("[Dispence����][%s,%s ����] Circle ���� �̵� �ð� �ʰ�", MotorName[Motor_PCB_X], MotorName[Motor_PCB_Y]);
			} else
			{
				sMsg.Format("PCB �� Dispence ���� Circle �̵� ������");
			}
			errMsg2(Task.AutoFlag, sMsg);

			return false;
		}

		Sleep(5);
	}

	return true;
}



bool CAxlMotorSet::func_Trinity_Epoxy_Rect_Draw()
{
	int step = 0;
	double startTime = 0.0;
	double eTime = 0.0;
	double curTime = 0.0;
	DWORD dwReturn = 0;
	long axis[2];
	double acc = 0.0;
	double dec = 0.0;
	double MoveStartTime = 0.0;
	long lSize = 2, lCoordinate = 0;
	double dTimeAngle = 90.0;
	double dEndDeg1ToTime = 0.0;	//1���� �ɸ��� �ð� 
	CString sMsg = "";
	axis[0] = Motor_PCB_X; //���ȣ
	axis[1] = Motor_PCB_Y;
	double dMidPos[2];
	double dEndPos[2];
	acc = model.dAcc;		//! �����ӵ�
	dec = model.dAcc;
	//
	dEndDeg1ToTime = (((model.dRadiusLength * 1) * M_PI) / (sqrt(model.dEpoxySpeed  * model.dEpoxySpeed / 2))) / 360;
	//1���� �ɸ��� �ð� = ������ / �ӵ� / 360
	while (1)
	{

		switch (step)
		{
		case 0:
			if (model.dOnTime == 0.0)
			{
				Dio.EpoxyDischargeOn(true, false);
			}
			if(Trinityt_Dispense_MoveAxis())
			{
				startTime = myTimer(true);
				step = 100;
			}
			break;

		case 100:
			if (model.dOnTime == 0.0)
			{
				step = 400;
				break;
			}
			curTime = myTimer(true);	//���� �ð��� �� ���� On���� ��� �ð��� ����
			if (curTime - startTime > model.dOnTime * 1000)	//���� Delay �ð� ��� �� ����
			{
				step = 200;
			}
			break;

		case 200:
			startTime = myTimer(true);
			if (Dio.EpoxyDischargeOn(true, true))		//Epoxy ���� ON 
			{
				step = 400;// 4000;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING188);
				sMsg.Format(sLangChange);	//"[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� ON ��ȣ"
				errMsg2(Task.AutoFlag, sMsg);
				step = -0;
			}
			break;
		case 400:
			if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y))
			{
				if (!Dio.EpoxyDischargeOn(false, false))		//Epoxy ���� Off
				{
					sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ");
					errMsg2(Task.AutoFlag, sMsg);
					return false;
				}
				step = 500;
				//curTime = myTimer(true);
				//if ((dEndDeg1ToTime*dTimeAngle - model.dOffTime) * 1000  <  (curTime - MoveStartTime))
				//{
				//	if (!Dio.EpoxyDischargeOn(false, false))		//Epoxy ���� Off
				//	{
				//		sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ");
				//		errMsg2(Task.AutoFlag, sMsg);
				//		return false;
				//	}
				//	step = 410;
				//}
			}
		case 500:
			if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y))
			{

				if (Straight_Dispense_MoveAxis(Motor_PCB_X, REL, model.dDisLength[3] * -1 * 0.2, model.dEpoxySpeed, model.dAcc))	//�߰� ����
				{
					step = 550;
				}
			}
			break;
		case 550:
			if ((IsStopAxis(Motor_PCB_X)) && (IsStopAxis(Motor_PCB_Y)))
			{
				step = 1000;
			}
			break;

			//----------------------------------------------------------------------------------------------------------------------------
			//----------------------------------------------------------------------------------------------------------------------------
		case 1000:
			
			step = 5000;
			break;
		default:
			step = 5000;
			break;
		}

		if (step == 5000)		break;

		eTime = myTimer(true);
		if (eTime - startTime > 2000000)
		{
			AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
			AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);

			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_Y);

			if (step>100)
			{
				int motorNo = step - 100;
				sMsg.Format("[Dispence����]Circle ���� �̵� �ð� �ʰ�");
			}
			else
			{
				sMsg.Format("PCB �� Dispence ���� Circle �̵� ������");
			}
			errMsg2(Task.AutoFlag, sMsg);

			return false;
		}

		Sleep(5);
	}

	return true;
}


bool CAxlMotorSet::func_Trinity_Epoxy_Circle_Draw()
{
	int step = 0;
	double startTime = 0.0;
	double eTime = 0.0;
	double curTime = 0.0;
	DWORD dwReturn = 0;
	long axis[2];
	double acc = 0.0;
	double dec = 0.0;
	double MoveStartTime = 0.0;
	long lSize = 2, lCoordinate = 0;
	double dTimeAngle = 90.0;
	double dEndDeg1ToTime = 0.0;	//1���� �ɸ��� �ð� 
	CString sMsg = "";
	axis[0] = Motor_PCB_X; //���ȣ
	axis[1] = Motor_PCB_Y;
	double dMidPos[2];
	double dEndPos[2];
	acc = model.dAcc;		//! �����ӵ�
	dec = model.dAcc;
	//
	dEndDeg1ToTime = (((model.dRadiusLength * 1) * M_PI) / (sqrt(model.dEpoxySpeed  * model.dEpoxySpeed / 2))) / 360;

	Task.bTrinityDoubleDispense = false;
	//1���� �ɸ��� �ð� = ������ / �ӵ� / 360
	while (1)
	{

		switch (step)
		{
		case 0:
			startTime = myTimer(true);
			step = 1000;
			break;
		case 1000:
			step = 2000;
			break;
			//
			//----------------------------------------------------------------------------------------------------------------------------
			//----------------------------------------------------------------------------------------------------------------------------
		case 2000:
			//�ι�° ��
			step = 2100; 
			break;
		case 2100:
			if (IsStopAxis(Motor_PCB_Z))
			{
				if (motor.Dispense_XY_Motor_Move(Dispense_Pos2, 10.0))
				{
					step = 2200;
				}
			}
			break;
		case 2200:
			if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) && IsStopAxis(Motor_PCB_Z))
			{
				if (motor.Dispense_Z_Motor_Move(Dispense_Pos2, 5.0))
				{
					step = 2250; 
				}
			}
			break;

		case 2250:
			if (Task.bTrinityDoubleDispense == true) 
			{
				if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) && IsStopAxis(Motor_PCB_Z))
				{
					//Z�� �ϴ� ȸ��
					Sleep(500);
					if (Straight_Dispense_MoveAxis(Motor_PCB_Z, REL, -0.4, model.dEpoxySpeed, model.dAcc))  
					{
						step = 2300;
					}
				}
			}
			else
			{
				
				step = 2300;
			}
			break;
		case 2300:

			startTime = myTimer(true);
			if (Dio.EpoxyDischargeOn2(true, true))		//Epoxy ���� ON 
			{
				step = 2350;
			}
			else
			{
				sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� ON ��ȣ");
				errMsg2(Task.AutoFlag, sMsg);
				step = 0;
			}
			break;

		case 2350:
			if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) && IsStopAxis(Motor_PCB_Z))
			{
				Pcb_Motor_Twister_Move(180, false, 2);//ȸ���� 180���� , ������ �÷����鼭 ����
				step = 2400;

				MoveStartTime = myTimer(true);
			}
			break;
		case 2400:
			curTime = myTimer(true);
			if ((IsStopAxis(Motor_PCB_X)) && (IsStopAxis(Motor_PCB_Y)))
			{
				AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
				AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);
				if (!Dio.EpoxyDischargeOn2(false, false))		//Epoxy ���� Off
				{
					sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ");
					errMsg2(Task.AutoFlag, sMsg);
					return false;
				}
				step = 2500;
			}
			break;
		case 2500:
			if (Task.bTrinityDoubleDispense == false)
			{
				if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) && IsStopAxis(Motor_PCB_Z))
				{
					//Z�� �ϴ� ȸ��
					if (Straight_Dispense_MoveAxis(Motor_PCB_Z, REL, -3.0, model.dEpoxySpeed, model.dAcc))
					{
						Task.bTrinityDoubleDispense = true;	//�� ����
						step = 2000;
						break;
					}
				} 
				step = 2500;
				break;
			}
			else
			{
				step = 3000;
				break;
			}
			break;
			//
			//----------------------------------------------------------------------------------------------------------------------------
			//----------------------------------------------------------------------------------------------------------------------------
		case 3000:
			Task.bTrinityDoubleDispense = false;
			step = 3050;
			break;
		case 3050:
			
			//����° ��
			if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) && IsStopAxis(Motor_PCB_Z))
			{
				//Z�� �ϴ� ȸ��
				if (Straight_Dispense_MoveAxis(Motor_PCB_Z, REL, 3.0 * -1, model.dEpoxySpeed, model.dAcc))
				{
					step = 3100;
				}
			}
			break;
		case 3100:
			if (IsStopAxis(Motor_PCB_Z))
			{
				if (motor.Dispense_XY_Motor_Move(Dispense_Pos3, 10.0))
				{
					step = 3200;
				}
			}
			break;
		case 3200:
			if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) && IsStopAxis(Motor_PCB_Z))
			{
				if (motor.Dispense_Z_Motor_Move(Dispense_Pos3, 5.0))
				{
					step = 3250;
				}
			}
			break;
		case 3250:
			if (Task.bTrinityDoubleDispense == true)
			{
				if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) && IsStopAxis(Motor_PCB_Z))
				{
					//Z�� �ϴ� ȸ��
					Sleep(500);
					if (Straight_Dispense_MoveAxis(Motor_PCB_Z, REL, -0.4, model.dEpoxySpeed, model.dAcc))
					{
						step = 3300;
						break;
					}
				}
			}
			else
			{

				step = 3300;
			}
			break;
		case 3300:
			startTime = myTimer(true);
			if (Dio.EpoxyDischargeOn2(true, true))		//Epoxy ���� ON 
			{
				step = 3400;
			}
			else
			{
				sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� ON ��ȣ");
				errMsg2(Task.AutoFlag, sMsg);
				step = 0;
			}
			break;
		case 3400:
			if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) && IsStopAxis(Motor_PCB_Z))
			{
				Pcb_Motor_Twister_Move(180, false, 3);//ȸ���� 180���� , ������ �÷����鼭 ����
				step = 3500;

				MoveStartTime = myTimer(true);
			}
			break;
		
		case 3500: //���� ���� �Ϸ���� ��� 
				   //if ((IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X)) &&(IsStopAxis(Motor_PCB_Y) && GetInposition(Motor_PCB_Y)))
			if ((IsStopAxis(Motor_PCB_X)) && (IsStopAxis(Motor_PCB_Y)))
			{
				AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
				AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);

				if (!Dio.EpoxyDischargeOn2(false, false))		//Epoxy ���� Off
				{
					sMsg.Format("[Dispence] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ");
					errMsg2(Task.AutoFlag, sMsg);
					return false;
				}
				step = 4000;
			}
			break;
		case 4000:
			if (Task.bTrinityDoubleDispense == false)
			{
				if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) && IsStopAxis(Motor_PCB_Z))
				{
					Task.bTrinityDoubleDispense = true;
					step = 3050; 
					break;
				}
			}
			else
			{
				step = 5000;
				break;
			}
			break;
		default:
			step = 5000;
			break;
		}

		if (step == 5000)		break;

		eTime = myTimer(true);
		if (eTime - startTime > 2000000) 
		{
			AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
			AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);

			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_Y);

			if (step>100)
			{
				int motorNo = step - 100;
				sMsg.Format("[Dispence����]Circle ���� �̵� �ð� �ʰ�");
			}
			else
			{
				sMsg.Format("PCB �� Dispence ���� Circle �̵� ������");
			}
			errMsg2(Task.AutoFlag, sMsg);

			return false;
		}

		Sleep(5);
	}

	return true;
}
bool CAxlMotorSet::func_New_Epoxy_Rect_Arck_Draw()
{
	int step = 0;
	double startTime = 0.0;
	double eTime = 0.0;
	double curTime = 0.0;
	DWORD dwReturn = 0;
	long axis[2];
	double acc = 0.0;
	double dec = 0.0;
	double MoveStartTime = 0.0;
	long lSize = 2, lCoordinate = 0;
	double dTimeAngle = 90.0;
	double dEndDeg1ToTime = 0.0;	//1���� �ɸ��� �ð� 
	CString sMsg = "";
	axis[0] = Motor_PCB_X; //���ȣ
	axis[1] = Motor_PCB_Y;
	double dMidPos[2];
	double dEndPos[2];
	acc = model.dAcc;		//! �����ӵ�
	dec = model.dAcc;
	//
	dEndDeg1ToTime = (((model.dRadiusLength * 1) * M_PI) / (sqrt(model.dEpoxySpeed  * model.dEpoxySpeed / 2))) / 360;
	//1���� �ɸ��� �ð� = ������ / �ӵ� / 360
	while (1)
	{

		switch (step)
		{
		case 0:
			if (model.dOnTime == 0.0)
			{
				Dio.EpoxyDischargeOn(true, false);
			}
			if (Continue_Line_Arck_Dispense() )//bottom Line
			{
				startTime = myTimer(true);
				step = 100;
			}
			break;

		case 100:
			if (model.dOnTime == 0.0)
			{
				step = 150;
				break;
			}
			curTime = myTimer(true);	//���� �ð��� �� ���� On���� ��� �ð��� ����
			if (curTime - startTime > model.dOnTime * 1000)	//���� Delay �ð� ��� �� ����
			{
				step = 110;
			}
			break;

		case 110:
			startTime = myTimer(true);
			if (Dio.EpoxyDischargeOn(true, false))		//Epoxy ���� ON 
			{
				step = 120;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING188);
				sMsg.Format(sLangChange);	//"[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� ON ��ȣ"
				errMsg2(Task.AutoFlag, sMsg);
				step = -0;
			}
			break;
		case 150:
			step = 200;
			break;
		case 200:

			step = 400;
			break;
		case 400:
			if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y))
			{
				curTime = myTimer(true);
				if (!Dio.EpoxyDischargeOn(false, false))		//Epoxy ���� Off
				{
					sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ");	//"[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ"
					errMsg2(Task.AutoFlag, sMsg);
					return false;
				}
				step = 410;
				
				//if ((dEndDeg1ToTime*dTimeAngle - model.dOffTime) * 1000  <  (curTime - MoveStartTime))
				//{
				//	if (!Dio.EpoxyDischargeOn(false, false))		//Epoxy ���� Off
				//	{
				//		sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ");	//"[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ"
				//		errMsg2(Task.AutoFlag, sMsg);
				//		return false;
				//	}
				//	Dio.EpoxyDischargeOn(false, false);
				//	Sleep(model.dOffTime);

				//	step = 410;
				//}
			}
			break;
		case 410:
			if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y))
			{

				if (Straight_Dispense_MoveAxis(Motor_PCB_X, REL, model.dDisLength[4] * -1 * 0.2, model.dEpoxySpeed, model.dAcc))	//�߰� ����
				{
					step = 500;
				}
			}
			break;
		case 500: //���� ���� �Ϸ���� ���
			if ((IsStopAxis(Motor_PCB_X) ) &&(IsStopAxis(Motor_PCB_Y) ))
			{
				step = 1000;
			}
			break;
		default:
			step = 1000;
			break;
		}

		if (step == 1000)		break;

		eTime = myTimer(true);
		if (eTime - startTime > 2000000)
		{
			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_Y);

			if (step>100)
			{
				int motorNo = step - 100;
				sMsg.Format("[Dispence����][%s,%s ����] Circle ���� �̵� �ð� �ʰ�", MotorName[Motor_PCB_X], MotorName[Motor_PCB_Y]);
			}
			else
			{
				sMsg.Format("PCB �� Dispence ���� Circle �̵� ������");
			}
			errMsg2(Task.AutoFlag, sMsg);

			return false;
		}

		Sleep(5);
	}

	return true;
}

bool CAxlMotorSet::func_Epoxy_Draw()
{
	int step=0;
	double startTime=0.0;
	double eTime = 0.0;
	double curTime = 0.0;
	double MoveStartTime = 0.0;
	DWORD dwReturn = 0;
	long axis[2];
	double acc, dec;
	long lSize = 2, lCoordinate = 0;
	CString sMsg;
	axis[0] = Motor_PCB_X; //���ȣ
	axis[1] = Motor_PCB_Y;
	
	
	acc = model.dAcc;		//! �����ӵ�
	dec = model.dAcc;

	while(1)
	{
		switch(step)
		{
		case 0:
			if(Straight_Dispense_MoveAxis(Motor_PCB_X, REL, (model.dDisLength[0]) * -1, model.dEpoxySpeed, model.dAcc))
			{
				startTime = myTimer(true);
				step = 100;
			}
			break;
					
		case 100:									
			curTime = myTimer(true);	//���� �ð��� �� ���� On���� ��� �ð��� ����
			if( curTime-startTime > model.dOnTime * 100 )	//���� Delay �ð� ��� �� ����
			{
				step = 110;
			}			
			break;

		case 110:									
			startTime = myTimer(true);
			if( Dio.EpoxyDischargeOn(true, true) )		//Epoxy ���� ON 
			{
				step = 130;
			}
			else 
			{
				sLangChange.LoadStringA(IDS_STRING188);
				sMsg.Format(sLangChange);	//"[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� ON ��ȣ"
				errMsg2(Task.AutoFlag, sMsg);
				step = -0;
			}
			break;

		case 130:
			if( IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) )
			{
				if(Straight_Dispense_MoveAxis(Motor_PCB_Y, REL, (model.dDisLength[1]) * -1, model.dEpoxySpeed, model.dAcc))
				{
					step = 150;
				}
			}
			break;

		case 150:
			if( IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) )
			{
				if(Straight_Dispense_MoveAxis(Motor_PCB_X, REL, (model.dDisLength[2]), model.dEpoxySpeed, model.dAcc))
				{
					step = 170;
				}
			}
			break;

		case 170:
			if( IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y) )
			{
				if(Straight_Dispense_MoveAxis(Motor_PCB_Y, REL, (model.dDisLength[3]), model.dEpoxySpeed, model.dAcc))
				{
					MoveStartTime = myTimer(true);
					step = 400;
				}
			}
			break;

		case 400:
			step = 450;
			//curTime = myTimer(true);
			//if (curTime - MoveStartTime > model.dOffTime * 1000)	//���� Delay �ð� ��� �� ���� �̵�
			//{
			//	if (!Dio.EpoxyDischargeOn(false, false))		//Epoxy ���� Off
			//	{
			//		sLangChange.LoadStringA(IDS_STRING187);
			//		sMsg.Format(sLangChange);	//"[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ"
			//		errMsg2(Task.AutoFlag, sMsg);
			//		Dio.EpoxyDischargeOn(false, false);

			//		return false;
			//	}
			//	Dio.EpoxyDischargeOn(false, false);
			//	step = 450;
			//}
				
			break;
		case 450:
			if ((IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X)) &&
				(IsStopAxis(Motor_PCB_Y) && GetInposition(Motor_PCB_Y)))
			{
				//curTime = myTimer(true);
				if (!Dio.EpoxyDischargeOn(false, false))		//Epoxy ���� Off
				{
					sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ");	
					errMsg2(Task.AutoFlag, sMsg);
					return false;
				}
				Dio.EpoxyDischargeOn(false, false);
				//Sleep(model.dOffTime);
				Straight_Dispense_MoveAxis(Motor_PCB_X, REL, model.dDisLength[0] * -1 * 0.25, model.dEpoxySpeed, model.dAcc);	//�߰� ����
				step = 500;
			}
			break;
		case 500: //���� ���� �Ϸ���� ���
			if( ( IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X) ) && 
				( IsStopAxis(Motor_PCB_Y) && GetInposition(Motor_PCB_Y) ) ){
				step = 1000;	
				}
			break;
		default:
			step = 1000;
			break;
		}

		if(step==1000)		break;

		eTime = myTimer(true);
		if(eTime-startTime > 2000000)
		{
			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_Y);

			if(step>100){
				int motorNo = step - 100;
				sLangChange.LoadStringA(IDS_STRING189);
				sMsg.Format(sLangChange, MotorName[Motor_PCB_X], MotorName[Motor_PCB_Y]);	//"[Dispence����][%s,%s ����] Circle ���� �̵� �ð� �ʰ�"
			} else {
				sLangChange.LoadStringA(IDS_STRING996);
				sMsg.Format(sLangChange);
			}
			errMsg2(Task.AutoFlag, sMsg);

			return false;
		}

		Sleep(5);
	}

	return true;

}

bool CAxlMotorSet::func_Epoxy_CircleDraw(bool CONTI)//Dispenser ���� ���� �ϸ� ��ȣ ���� ����
{
	int step=0;
	double startTime;
	double MoveStartTime;
	double eTime, curTime;
	CString sMsg;
	double dEndDeg1ToTime = 0.0;	//1���� �ɸ��� �ð� 
	double dTimeAngle = 0.0;
	double dDrawAng = 0.0;
	double dDrawLength = 0.0;
	int SizeCheck = 0;
	int dCircleSpeed = 0.0;
	double dCircleOffTime = 0.0;
	if (CONTI  == true)
	{
		//SizeCheck = 999;
		//dDrawLength = model.dConti_RadiusLength;
		//dDrawAng = model.dConti_Angle;	//conti���϶� ������ �׸���
		//dCircleSpeed = model.dSmallSpeed;
		//dCircleOffTime = model.dSmallOffTime;
	}

	dDrawLength = model.dRadiusLength;
	dDrawAng = model.dAngle;
	dCircleSpeed = model.dCircleSpeed;
	dCircleOffTime = model.dOffTime;
	//dConti_RadiusLength
	//dConti_Angle
	//----------------------------------------------------------------------------
	bool mOffCheck = false;
	if (dDrawAng > 360)	//if (model.dAngle > 360)
	{
		dTimeAngle = 360;		//Epoxy ���� Off Time�� ���
	}
	else
	{
		dTimeAngle = dDrawAng;
	}

	//1���� �ɸ��� �ð� = (������ * �ӵ�) / 360 /   ����/����
	//dEndDeg1ToTime = (( (model.dRadiusLength * 1) * M_PI) / (sqrt(model.dCircleSpeed  * model.dCircleSpeed/2)) ) / 360;	//1���� �ɸ��� �ð� = ������ / �ӵ� / 360
	  dEndDeg1ToTime = (( (dDrawLength * 1) * M_PI) / (sqrt(dCircleSpeed  * dCircleSpeed /2)) ) / 360;	//1���� �ɸ��� �ð� = ������ / �ӵ� / 360
	//dEndDeg1ToTime = (( (model.dRadiusLength * 1) * M_PI) / sqrt(model.dSpeed) ) / 360;	//1���� �ɸ��� �ð� = ������ / �ӵ� / 360

	while(1)
	{
		switch(step)
		{
		case 0:									
			startTime = myTimer(true);
			if( Dio.EpoxyDischargeOn(true, true) )		//Epoxy ���� ON 
			{
				step = 100;
			}
			else 
			{
				sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� ON ��ȣ");
				errMsg2(Task.AutoFlag, sMsg);
				return false;
			}
			break;
		case 100:								
			curTime = myTimer(true);	//���� �ð��� �� ���� On���� ��� �ð��� ����
			if( curTime-startTime > model.dOnTime * 1000 )	//���� Delay �ð� ��� �� ���� �̵�
			{
				Pcb_Motor_CircleMove(dTimeAngle, false , SizeCheck);		//���� �׸���
				MoveStartTime = myTimer(true);
				step = 200;
			}
			break;
		case 200:// ���� ������ ���� OFF Time ó��
			curTime = myTimer(true);
			//1����ð�*���� - (��߽ð�-����ð�) < �����ð�
			//if( (dEndDeg1ToTime*dTimeAngle - model.dOffTime) *1000 <  (curTime - MoveStartTime) )	//�Ϸ��� ���� �ð��� ���� OFF

			if (((dEndDeg1ToTime*dTimeAngle) - dCircleOffTime) * 1000 < (curTime - MoveStartTime)  && mOffCheck == false)
			{
				mOffCheck = true;
				Dio.EpoxyDischargeOn(false, false);
			}
			if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y)) 
			{
				if( !Dio.EpoxyDischargeOn(false, false/*true*/) )		//Epoxy ���� Off
				{
					sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ");
					errMsg2(Task.AutoFlag, sMsg);
					Dio.EpoxyDischargeOn(false, false);
					
					AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
					AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);
					return false;
				}
				else
				{
					if(dDrawAng > 360)	step = 400;	//360�� ���� ���� ��� �� ȸ��(��ȣ�������� 360������ ����,)
					else					step = 500;	//360�� ���� ���� ��� ���̻� ȸ�� ����
				}
			}
			break;

		case 400:
			if( ( IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X) ) && 
				( IsStopAxis(Motor_PCB_Y) && GetInposition(Motor_PCB_Y) ) )
			{
				Pcb_Motor_CircleMove(dDrawAng - dTimeAngle, true , SizeCheck);	//�߰� ���� ����
				step = 500;	
			}
			break;

		case 500: //���� ���� �Ϸ���� ���
			if( ( IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X) ) && 
				( IsStopAxis(Motor_PCB_Y) && GetInposition(Motor_PCB_Y) ) ){
				AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
				AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);
				step = 1000;	
				}
			break;
		default:
			step = 1000;
			break;
		}

		if(step==1000)		break;

		eTime = myTimer(true);
		if(eTime-startTime > 100000)
		{
			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_Y);

			if(step>100){
				int motorNo = step - 100;
				sMsg.Format("[Dispence����][%s,%s ����] Circle ���� �̵� �ð� �ʰ�", MotorName[Motor_PCB_X], MotorName[Motor_PCB_Y]);
			} else {
				sMsg.Format("PCB �� Dispence ���� Circle �̵� ������");
			}
			errMsg2(Task.AutoFlag, sMsg);

			AxmMotSetMoveUnitPerPulse(Motor_PCB_X,1,1);	//Unit/Pulse �������·� ����
			AxmMotSetMoveUnitPerPulse(Motor_PCB_Y,1,1);

			return false;
		}

		Sleep(5);
		checkMessage();
	}

	AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
	AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);
	return true;
}

bool CAxlMotorSet::func_Epoxy_Point_Draw() 
{
	int step = 0;
	double startTime = 0.0;
	double eTime = 0.0;
	double curTime = 0.0;
	DWORD dwReturn = 0;
	long axis[2];
	double acc, dec;
	double dMovePos = 0.0;
	double dMoveZGap = 4.0;
	long lSize = 2, lCoordinate = 0;
	CString sMsg;
	axis[0] = Motor_PCB_X; //���ȣ
	axis[1] = Motor_PCB_Y;


	acc = model.dAcc;		//! �����ӵ�
	dec = model.dAcc;

	while (1)
	{

		switch (step)
		{
		case 0:
			startTime = myTimer(true);
			if (Dio.EpoxyDischargeOn(true, true) == false)		//Epoxy ���� ON 
			{
				sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� ON ��ȣ");
				errMsg2(Task.AutoFlag, sMsg);
				step = -0;
				break;
			}
			step = 100;
			Task.PCBTaskTime = myTimer(true); 
			break;
		case 100:
			if (myTimer(true) - Task.PCBTaskTime > model.dSmallOffTime * 1000)
			{
				if (Dio.EpoxyDischargeOn(false, false) == false)		//Epoxy ���� Off
				{
					sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ");
					errMsg2(Task.AutoFlag, sMsg);
					return false;
				}
				step = 110;
			}
			break;
		case 110:
			//z�� �ϰ�
			dMovePos = motor.GetCommandPos(Motor_PCB_Z) - dMoveZGap;
			motor.MoveAxis(Motor_PCB_Z, ABS, dMovePos, sysData.fMotorSpeed[Motor_PCB_Z], sysData.fMotorAccTime[Motor_PCB_Z]);
			step = 120;
			Task.PCBTaskTime = myTimer(true);
			break;
		case 120:
			//z�� ���� Ȯ��
			if ((motor.IsStopAxis(Motor_PCB_Z)))
			{
				step = 150;
			}
			else if ((myTimer(true) - Task.PCBTaskTime) > 5000)
			{
				sMsg.Format("[Dispence����] pcb Z ���� �̻� - %d", step);
				errMsg2(Task.AutoFlag, sMsg);
				step = -120;
				break;
			}
			break;
		case 150:
			//2��° Point �̵�
			if (Straight_Dispense_MoveAxis(Motor_PCB_X, REL, (model.dDisLength[0]) * -1, model.dEpoxySpeed, model.dAcc))
			{
				startTime = myTimer(true);
				step = 160;
			}
			break;
		case 160:
			if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y))
			{
				step = 200;
			}
			break;
		case 200:
			//z�� ���
			dMovePos = motor.GetCommandPos(Motor_PCB_Z) + dMoveZGap;
			motor.MoveAxis(Motor_PCB_Z, ABS, dMovePos, sysData.fMotorSpeed[Motor_PCB_Z], sysData.fMotorAccTime[Motor_PCB_Z]);
			Task.PCBTaskTime = myTimer(true);
			step = 210;
			break;
		case 210:
			//z�� ����Ȯ��
			if ((motor.IsStopAxis(Motor_PCB_Z)))
			{
				step = 250;
			}
			else if ((myTimer(true) - Task.PCBTaskTime) > 5000)
			{
				sMsg.Format("[Dispence����] pcb Z ���� �̻� - %d", step);
				errMsg2(Task.AutoFlag, sMsg);
				step = -210;
				break;
			}
			break;
		case 250:
			if (Dio.EpoxyDischargeOn(true, true) == false)		//Epoxy ���� ON 
			{
				sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� ON ��ȣ");
				errMsg2(Task.AutoFlag, sMsg);
				step = -250;
				break;
			}
			step = 260;
			startTime = myTimer(true);
			break;
		case 260:
			if (myTimer(true) - startTime > model.dSmallOffTime * 1000)
			{
				if (Dio.EpoxyDischargeOn(false, false) == false)		//Epoxy ���� Off
				{
					sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ");
					errMsg2(Task.AutoFlag, sMsg);
					return false;
				}
				step = 270;
			}
			break;
		case 270:
			//z�� �ϰ�
			dMovePos = motor.GetCommandPos(Motor_PCB_Z) - dMoveZGap;
			motor.MoveAxis(Motor_PCB_Z, ABS, dMovePos, sysData.fMotorSpeed[Motor_PCB_Z], sysData.fMotorAccTime[Motor_PCB_Z]);
			Task.PCBTaskTime = myTimer(true);
			step = 280;
			break;
		case 280:
			//z�� ���� Ȯ��
			if ((motor.IsStopAxis(Motor_PCB_Z)))
			{
				step = 300;
			}
			else if ((myTimer(true) - Task.PCBTaskTime) > 5000)
			{
				sMsg.Format("[Dispence����] pcb Z ���� �̻� - %d" , step);
				errMsg2(Task.AutoFlag, sMsg);
				step = -280;
				break;
			}
			break;
		case 300:
			//2��° Point �̵�
			if (Straight_Dispense_MoveAxis(Motor_PCB_Y, REL, (model.dDisLength[1]) * -1, model.dEpoxySpeed, model.dAcc))
			{
				startTime = myTimer(true);
				step = 310;
			}
			break;
		case 310:
			if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y))
			{
				step = 350;
			}
			break;
		case 350:
			//z�� ���
			dMovePos = motor.GetCommandPos(Motor_PCB_Z) + dMoveZGap;
			motor.MoveAxis(Motor_PCB_Z, ABS, dMovePos, sysData.fMotorSpeed[Motor_PCB_Z], sysData.fMotorAccTime[Motor_PCB_Z]);
			Task.PCBTaskTime = myTimer(true);
			step = 360;
			break;
		case 360:
			//z�� ����Ȯ��
			if ((motor.IsStopAxis(Motor_PCB_Z)))
			{
				step = 400;
			}
			else if ((myTimer(true) - Task.PCBTaskTime) > 5000)
			{
				sMsg.Format("[Dispence����] pcb Z ���� �̻� - %d", step);
				errMsg2(Task.AutoFlag, sMsg);
				step = -360;
				break;
			}
			break;
		case 400:
			if (Dio.EpoxyDischargeOn(true, true) == false)		//Epoxy ���� ON 
			{
				sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� ON ��ȣ");
				errMsg2(Task.AutoFlag, sMsg);
				step = -400;
				break;
			}
			step = 410;
			startTime = myTimer(true);
			break;
		case 410:
			curTime = myTimer(true);	//���� �ð��� �� ���� On���� ��� �ð��� off
			if (curTime - startTime > model.dSmallOffTime * 1000)
			{
				if (Dio.EpoxyDischargeOn(false, false) == false)		//Epoxy ���� Off
				{
					sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ");
					errMsg2(Task.AutoFlag, sMsg);
					return false;
				}
				step = 420;
			}
			break;
		case 420:
			//z�� �ϰ�
			dMovePos = motor.GetCommandPos(Motor_PCB_Z) - dMoveZGap;
			motor.MoveAxis(Motor_PCB_Z, ABS, dMovePos, sysData.fMotorSpeed[Motor_PCB_Z], sysData.fMotorAccTime[Motor_PCB_Z]);
			Task.PCBTaskTime = myTimer(true);
			step = 430;
			break;
		case 430:
			//z�� �ϰ� Ȯ��
			if ((motor.IsStopAxis(Motor_PCB_Z)))
			{
				step = 450;
			}
			else if ((myTimer(true) - Task.PCBTaskTime) > 5000)
			{
				sMsg.Format("[Dispence����] pcb Z ���� �̻� - %d", step);
				errMsg2(Task.AutoFlag, sMsg);
				step = -430;
				break;
			}
			break;
		case 450:
			//2��° Point �̵�
			if (Straight_Dispense_MoveAxis(Motor_PCB_X, REL, (model.dDisLength[2]) * 1, model.dEpoxySpeed, model.dAcc))
			{
				startTime = myTimer(true);
				step = 460;
			}
			break;
		case 460:
			if (IsStopAxis(Motor_PCB_X) && IsStopAxis(Motor_PCB_Y))
			{
				step = 470;
			}
			break;
		case 470:
			//z�� ���
			dMovePos = motor.GetCommandPos(Motor_PCB_Z) + dMoveZGap;
			motor.MoveAxis(Motor_PCB_Z, ABS, dMovePos, sysData.fMotorSpeed[Motor_PCB_Z], sysData.fMotorAccTime[Motor_PCB_Z]);
			Task.PCBTaskTime = myTimer(true);
			step = 480;
			break;
		case 480:
			//z�� ��� Ȯ��
			if ((motor.IsStopAxis(Motor_PCB_Z)))
			{
				step = 500;
			}
			else if ((myTimer(true) - Task.PCBTaskTime) > 5000)
			{
				sMsg.Format("[Dispence����] pcb Z ���� �̻� - %d", step);
				errMsg2(Task.AutoFlag, sMsg);
				step = -480;
				break;
			}
			break;
		case 500:
			if (Dio.EpoxyDischargeOn(true, true) == false)		//Epoxy ���� ON 
			{
				sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� ON ��ȣ");
				errMsg2(Task.AutoFlag, sMsg);
				step = -500;
				break;
			}
			step = 510;
			startTime = myTimer(true);
			break;
		case 510:
			if (myTimer(true) - startTime > model.dSmallOffTime * 1000)
			{
				if (Dio.EpoxyDischargeOn(false, false) == false)		//Epoxy ���� Off
				{
					sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ");
					errMsg2(Task.AutoFlag, sMsg);
					return false;
				}
				step = 600;
			}
			break;
		case 600:
			if ((IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X)) &&
				(IsStopAxis(Motor_PCB_Y) && GetInposition(Motor_PCB_Y)))
			{
				if (!Dio.EpoxyDischargeOn(false, false))		//Epoxy ���� Off
				{
					sMsg.Format("[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� OFF ��ȣ");
					errMsg2(Task.AutoFlag, sMsg);
					return false;
				}
				step = 1000;
			}
			break;
		default:
			step = 1000;
			break;
		}

		if (step == 1000)		break;

		eTime = myTimer(true);
		if (eTime - startTime > 2000000)
		{
			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_Y);

			if (step>100) {
				int motorNo = step - 100;
				sLangChange.LoadStringA(IDS_STRING189);
				sMsg.Format(sLangChange, MotorName[Motor_PCB_X], MotorName[Motor_PCB_Y]);	//"[Dispence����][%s,%s ����] Circle ���� �̵� �ð� �ʰ�"
			}
			else {
				sLangChange.LoadStringA(IDS_STRING996);
				sMsg.Format(sLangChange);
			}
			errMsg2(Task.AutoFlag, sMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}

	return true;

}

bool CAxlMotorSet::Pcb_Motor_Twister_Move(double dAngle, bool bWait, int Pos)
{	
	//��ȣ ���� ����(PCB X/Y��) -> bWait=true�� ��� �Ϸ� ���� ���� Check,
	//**** bWait=false �� ��� Unit/Pulse�� ������� ������ ������ �ܺο��� ����� ��.,,,
	DWORD dwReturn = 0;
	long axis[2];
	double acc = 0.0;
	double dec = 0.0;
	double dMidPos[2];
	double dEndPos[2];
	long lSize = 2, lCoordinate = 0, eTime;

	CString sMsg = _T("");
	//TRINITY
	bool TrinityCircleChange = false;
	int TrinityAlgle = 0;
	double TrinityCircleRadius = 0.0;

	if (!m_bOrgFlag[Motor_PCB_X] || !m_bOrgFlag[Motor_PCB_Y])
	{
		sLangChange.LoadStringA(IDS_STRING1001);
		sMsg.Format(sLangChange);	//PCB �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
		errMsg2(Task.AutoFlag, sMsg);
		return false;
	}


	axis[0] = Motor_PCB_X; //���ȣ
	axis[1] = Motor_PCB_Y;


	acc = model.dAcc;		//! �����ӵ�
	dec = model.dAcc;
	double TEMPRadiusLength = 0.0;
	long MOTOR_DIR = DIR_CCW;
	//dMidPos[0] = 0;	//���� �߽� ��ġ X	
	//dMidPos[1] = (sqrt(model.dRadiusLength * model.dRadiusLength + model.dRadiusLength * model.dRadiusLength) * -1 ) * 0.53;//0.65;	//				Y

	double offsetR1_X = 0.0;
	double offsetR2_X = 0.0;
	double offsetR3_X = 0.0;
	double offsetR4_X = 0.0;
	double offsetR1_Y = 0.0;
	double offsetR2_Y = 0.0;
	double offsetR3_Y = 0.0;
	double offsetR4_Y = 0.0;

	double CircleRadius = 0.0;
	double CircleSpeed = 0.0;

	CircleRadius = model.dRadiusLength;
	CircleSpeed = model.dTwister_Speed;	// model.dEpoxySpeed;


	//motor x,y resolution �� ���Ͽ� ū ���� �־��ָ� �˴ϴ�..
	//���� resolution ���� x : 5000  y : 1000 �̹Ƿ� 5000�� �־� �־����ϴ�.
	//���������� �����ϱ����� �޽� �����Ͻð� ���������� �ٷ� 1:1�� �ٲپ� �־�� �մϴ�..
	//�׷��� ������ ���Ͱ� �̻��ϰ� �����մϴ�..
	// 

	///////////////////////////////////////////////////////////////////////////////////////////////////

	AxmContiWriteClear(lCoordinate);
	AxmContiSetAxisMap(lCoordinate, lSize, axis);
	double dRadius[3][4] = {
		{ 0.2 , 0.4 , 0.6 , 0.8 } ,
		{ 0.2 , 0.4 , 0.8 , 1.0 } ,  
		{ 0.5 , 1.2 , 2.4 , 3.6 }
	};
	AxmContiSetAbsRelMode(lCoordinate, 1);		//������ġ ����(1) , ������ǥ(0)
	CircleSpeed = 2;
	AxmContiBeginNode(lCoordinate);
	if (Pos == 3) 
	{
		//ū ��
		CircleSpeed = model.dTwister_Speed * sysData.fMotorResol[Motor_PCB_X];
	}
	else
	{
		//���� ��
		CircleSpeed = (model.dTwister_Speed * 0.8) * sysData.fMotorResol[Motor_PCB_X];
	}
	
	int dic = -1;
	for (int i = 0; i < 4; i++)
	{ 
		if (i == 3)
		{
			if (Pos == 3) {
				dAngle = 350;
			}
			else
			{
				dAngle = 270;
			}
			
		}
		TrinityCircleRadius = dRadius[Pos - 1][i];	// 0.5; 
		dMidPos[0] = 0.0; 
		dMidPos[1] =((sqrt(TrinityCircleRadius * TrinityCircleRadius + TrinityCircleRadius * TrinityCircleRadius) * dic) * 0.5) * 10000.0;
		AxmCircleAngleMove(lCoordinate, axis, dMidPos, dAngle, CircleSpeed, acc, dec, MOTOR_DIR);
		dic *= -1;
	} 
	
	AxmContiEndNode(lCoordinate);
	// �����ġ ��������.
	AxmContiSetAbsRelMode(lCoordinate, 1);
	// ���Ӻ������� ����.
	AxmContiStart(lCoordinate, 0, 0);


	int step = 0;
	double startTime = 0.0;
	while (1)
	{
		switch (step)
		{
		case 0:
			startTime = myTimer(true);
			if (bWait == true)	step = 100;	//�Ϸ� Check
			else				step = 1000; //����
			break;
		case 100:								// �̵� �Ϸ� Ȯ��..
			if (IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X))
			{
				AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����
				AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);
				step = 1000;
			}
			break;
		default:
			break;
		}

		if (step == 1000)		break;

		eTime = myTimer(true);
		if (eTime - startTime > 30000)
		{
			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_Y);

			if (step>100) 
			{
				int motorNo = step - 100;
				sMsg.Format("[Dispence����] Circle ���� �̵� �ð� �ʰ�");	//"[Dispence����][%s,%s ����] Circle ���� �̵� �ð� �ʰ�"
			}
			else 
			{
				sMsg.Format("PCB �� Dispence ���� Circle �̵� ������");
			}
			errMsg2(Task.AutoFlag, sMsg);

			AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����
			AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);
			return false;
		}
		Sleep(5);
	}
	if (bWait == true)
	{
		AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
		AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);
	}

	return true;
}

bool CAxlMotorSet::Pcb_Motor_CircleMove(double dAngle, bool bWait, int Pos)
{	//��ȣ ���� ����(PCB X/Y��) -> bWait=true�� ��� �Ϸ� ���� ���� Check,
	//**** bWait=false �� ��� Unit/Pulse�� ������� ������ ������ �ܺο��� ����� ��.,,,
	DWORD dwReturn = 0;
	long axis[2];
	double acc, dec;
	double dMidPos[2];
	double dEndPos[2];
	long lSize = 2, lCoordinate = 0, eTime;

	CString sMsg = _T("");
	//TRINITY
	bool TrinityCircleChange = false;
	int TrinityAlgle = 0;
	double TrinityCircleRadius = 0.0;
	if (!m_bOrgFlag[Motor_PCB_X] || !m_bOrgFlag[Motor_PCB_Y])
	{
		sLangChange.LoadStringA(IDS_STRING1001);
		sMsg.Format(sLangChange);	//PCB �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
		errMsg2(Task.AutoFlag, sMsg);
		return false;
	}


	axis[0] = Motor_PCB_X; //���ȣ
	axis[1] = Motor_PCB_Y;


	acc = model.dAcc;		//! �����ӵ�
	dec = model.dAcc;
	double TEMPRadiusLength = 0.0;
	long MOTOR_DIR;
	//dMidPos[0] = 0;	//���� �߽� ��ġ X	
	//dMidPos[1] = (sqrt(model.dRadiusLength * model.dRadiusLength + model.dRadiusLength * model.dRadiusLength) * -1 ) * 0.53;//0.65;	//				Y

	double offsetR1_X = 0.0;
	double offsetR2_X = 0.0;
	double offsetR3_X = 0.0;
	double offsetR4_X = 0.0;
	double offsetR1_Y = 0.0;
	double offsetR2_Y = 0.0;
	double offsetR3_Y = 0.0;
	double offsetR4_Y = 0.0;

	double CircleRadius = 0.0;
	double CircleSpeed = 0.0;
	if (Pos == 999)
	{
		CircleRadius = model.dConti_RadiusLength;//dConti_RadiusLength
		CircleSpeed = model.dSmallSpeed;
	}
	else if (Pos == 100 || Pos == 200 || Pos == 300 || Pos == 400)
	{
		CircleRadius = model.dConti_RadiusLength;//dConti_RadiusLength
		CircleSpeed = model.dSmallSpeed;
	}
	else
	{
		CircleRadius = model.dRadiusLength;
		CircleSpeed = model.dCircleSpeed;	// model.dEpoxySpeed;
	}

	offsetR1_X = model.dCircleRadiusCenterOffsetX[0];
	offsetR2_X = model.dCircleRadiusCenterOffsetX[1];
	offsetR3_X = model.dCircleRadiusCenterOffsetX[2];
	offsetR4_X = model.dCircleRadiusCenterOffsetX[3];
	//
	offsetR1_Y = model.dCircleRadiusCenterOffsetY[0];
	offsetR2_Y = model.dCircleRadiusCenterOffsetY[1];
	offsetR3_Y = model.dCircleRadiusCenterOffsetY[2];
	offsetR4_Y = model.dCircleRadiusCenterOffsetY[3];

	if (offsetR1_X > 10.0)offsetR1_X = 10.0;
	if (offsetR2_X > 10.0)offsetR2_X = 10.0;
	if (offsetR3_X > 10.0)offsetR3_X = 10.0;
	if (offsetR4_X > 10.0)offsetR4_X = 10.0;
	if (offsetR1_X < -10.0)offsetR1_X = -10.0;
	if (offsetR2_X < -10.0)offsetR2_X = -10.0;
	if (offsetR3_X < -10.0)offsetR3_X = -10.0;
	if (offsetR4_X < -10.0)offsetR4_X = -10.0;
	//
	if (offsetR1_Y > 10.0)offsetR1_Y = 10.0;
	if (offsetR2_Y > 10.0)offsetR2_Y = 10.0;
	if (offsetR3_Y > 10.0)offsetR3_Y = 10.0;
	if (offsetR4_Y > 10.0)offsetR4_Y = 10.0;
	if (offsetR1_Y < -10.0)offsetR1_Y = -10.0;
	if (offsetR2_Y < -10.0)offsetR2_Y = -10.0;
	if (offsetR3_Y < -10.0)offsetR3_Y = -10.0;
	if (offsetR4_Y < -10.0)offsetR4_Y = -10.0;
	switch (Pos)
	{
	case 0:
		//���� ���� ���� ���
		dMidPos[0] = 0;	//���� �߽� ��ġ X	
		dMidPos[1] = (sqrt(CircleRadius * CircleRadius + CircleRadius * CircleRadius) * -1) * 0.5;//Y
		MOTOR_DIR = DIR_CCW;
		break;
		//=======================================================================================================================================
		//=======================================================================================================================================
	case 100://�����ϴ�
		dMidPos[0] = model.dCircleRadiusCenterOffsetX[0];
		dMidPos[1] = model.dCircleRadiusCenterOffsetY[0];
		dEndPos[0] = model.dCircleRadiusEndOffsetX[0];
		dEndPos[1] = model.dCircleRadiusEndOffsetY[0];
		MOTOR_DIR = DIR_CCW;


		break;
	case 200://�������
		//
		dMidPos[0] = model.dCircleRadiusCenterOffsetX[1];
		dMidPos[1] = model.dCircleRadiusCenterOffsetY[1];
		dEndPos[0] = model.dCircleRadiusEndOffsetX[1];
		dEndPos[1] = model.dCircleRadiusEndOffsetY[1];
		MOTOR_DIR = DIR_CW;

		break;
	case 300://�������
		dMidPos[0] = model.dCircleRadiusCenterOffsetX[2];
		dMidPos[1] = model.dCircleRadiusCenterOffsetY[2];
		dEndPos[0] = model.dCircleRadiusEndOffsetX[2];
		dEndPos[1] = model.dCircleRadiusEndOffsetY[2];
		MOTOR_DIR = DIR_CCW;

		break;
	case 400://�����ϴ�
		dMidPos[0] = model.dCircleRadiusCenterOffsetX[3];
		dMidPos[1] = model.dCircleRadiusCenterOffsetY[3];
		dEndPos[0] = model.dCircleRadiusEndOffsetX[3];
		dEndPos[1] = model.dCircleRadiusEndOffsetY[3];
		MOTOR_DIR = DIR_CW;

		break;
	
	case 999:
		//���� ���� ���� ��� conti
		dMidPos[0] = 0;	//���� �߽� ��ġ X	 
		dMidPos[1] = (sqrt(CircleRadius * CircleRadius + CircleRadius * CircleRadius) * -1) * 0.5;//Y
		MOTOR_DIR = DIR_CCW;
		break;
	default:
		return false;
		break;
	}
	//20150603 - motor x,y resolution �� ���Ͽ� ū ���� �־��ָ� �˴ϴ�..
	//���� resolution ���� x : 5000  y : 1000 �̹Ƿ� 5000�� �־� �־����ϴ�.
	//���������� �����ϱ����� �޽� �����Ͻð� ���������� �ٷ� 1:1�� �ٲپ� �־�� �մϴ�..
	//�׷��� ������ ���Ͱ� �̻��ϰ� �����մϴ�..
	// 

	dwReturn = AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 10000);	//Unit/Pulse ���� - X
	dwReturn = AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 10000);	//					Y 

	//���� ������ AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);   <-- �߰��Ͽ����ϴ�.
	//�ؿ��κп� Ȯ���Ͻø� �˴ϴ�...
	///////////////////////////////////////////////////////////////////////////////////////////////////

	AxmContiWriteClear(lCoordinate);
	AxmContiSetAxisMap(lCoordinate, lSize, axis);

	AxmContiSetAbsRelMode(lCoordinate, 1);		//������ġ ����(1) , ������ǥ(0)
	

	//AxmCircleAngleMove(lCoordinate, axis, dMidPos, dAngle, model.dEpoxySpeed, acc, dec, DIR_CCW);  //�ݽð���� ���׸��� ���:DIR_CCW

	if (Pos == 0)
	{
		AxmCircleAngleMove(lCoordinate, axis, dMidPos, dAngle, CircleSpeed, acc, dec, MOTOR_DIR);
	}
	else
	{
		//�ݿ�
		AxmCircleCenterMove(lCoordinate, axis, dMidPos, dEndPos, CircleSpeed, acc, dec, MOTOR_DIR);
	}
	// 20151006 ���� �޶��� ���� ������� ���� ������������ ��ũ������������ ����
	//AxmCirclePointMove(lCoordinate, axis, double *dMidPos, double *dEndPos,  model.dSpeed, acc, dec, 0);
	int step=0;
	double startTime;
	while(1)
	{
		switch(step)
		{
		case 0:									
			startTime = myTimer(true);
			if( bWait == true)	step = 100;	//�Ϸ� Check
			else				step = 1000; //����
			break;
		case 100:								// �̵� �Ϸ� Ȯ��..
			if(IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X))
			{
				step = 1000;
			}
			break;
		default:
			break;
		}

		if(step==1000)		break;

		eTime = myTimer(true);
		if(eTime-startTime > 30000)
		{
			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_Y);

			if(step>100){
				int motorNo = step - 100;
				sMsg.Format("[Dispence����][%s,%s ����] Circle ���� �̵� �ð� �ʰ�", MotorName[Motor_PCB_X], MotorName[Motor_PCB_Y]);	//"[Dispence����][%s,%s ����] Circle ���� �̵� �ð� �ʰ�"
			} else {
				sMsg.Format("PCB �� Dispence ���� Circle �̵� ������");
			}
			errMsg2(Task.AutoFlag, sMsg);

			AxmMotSetMoveUnitPerPulse(Motor_PCB_X,1,1);	//Unit/Pulse �������·� ����
			AxmMotSetMoveUnitPerPulse(Motor_PCB_Y,1,1);
			return false;
		}
		Sleep(5);
		checkMessage();
	}
	if( bWait == true)	
	{
		AxmMotSetMoveUnitPerPulse(Motor_PCB_X, 1, 1);	//Unit/Pulse �������·� ����(1:1 ��Ī����)
		AxmMotSetMoveUnitPerPulse(Motor_PCB_Y, 1, 1);
	}

	return true;
}

bool CAxlMotorSet::checkPcbMotorPos(int index)
{
	double posX		= GetEncoderPos(Motor_PCB_X)  - model.axis[Motor_PCB_X].pos[index];
	double posY		= GetEncoderPos(Motor_PCB_Y)  - model.axis[Motor_PCB_Y].pos[index];
	double posTh	= GetCommandPos(Motor_PCB_TH) - model.axis[Motor_PCB_TH].pos[index];

	if( fabs(posX)<0.1 && fabs(posY)<0.1 && fabs(posTh)<0.1)
		return true;
	else
		return false;
}


bool CAxlMotorSet::checkLensMotorPos(int index)
{
	return true;
}
bool CAxlMotorSet::PCB_Z_V_Motor_Move(int index,double dVel)
{
	double eTime;
	short axis[1];
	double pos[1];

	char logMsg[1000];

	
	if(!m_bOrgFlag[Motor_PCB_Z])
	{
		sLangChange.LoadStringA(IDS_STRING555);	//PCB �� Z ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
		_stprintf_s(logMsg, sLangChange);
		errMsg2(Task.AutoFlag,logMsg);

		return false;
	}

	/*if(sysData.m_iProductComp == 1 && index == Bonding_Pos && !(Task.dAAPcbMotor[0] == 0 && Task.dAAPcbMotor[1] == 0 && Task.dAAPcbMotor[2] == 0 && Task.dAAPcbMotor[3] == 0 && Task.dAAPcbMotor[4] == 0 && Task.dAAPcbMotor[5] == 0))
	{
		pos[0]	= Task.dAAPcbMotor[5];
		axis[0] = Motor_PCB_Z;
	}
	else
	{*/
		pos[0]	= model.axis[Motor_PCB_Z].pos[index];
		axis[0] = Motor_PCB_Z;
	//}
	

	sLangChange.LoadStringA(IDS_STRING991);	//"PCB �� %s ���� �̵� �մϴ�.
	_stprintf_s(logMsg, sLangChange, MotorPosName[Motor_PCB_Z]);


	int step=0;
	
	double startTime;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	while(1)
	{
		switch(step)
		{
		case 0:
			startTime = myTimer(true);
			//goMotorPos(1, axis, pos);
			goMotorZPos(1, axis,pos,dVel);
			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if(IsStopAxis(Motor_PCB_Z))
				step = 1000;
			break;

		default:
			break;
		}

		if(step==1000)
			break;


		eTime = myTimer(true);
		if((eTime-startTime)>MOTOR_MOVE_TIME)
		{
			if(step>100)
			{
				sLangChange.LoadStringA(IDS_STRING1479);	//PCB �� [%s ����] �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorPosName[Motor_PCB_Z]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING992);	//PCB �� %s �̵� ������
				_stprintf_s(logMsg, sLangChange, MotorPosName[Motor_PCB_Z]);
			}

			errMsg2(Task.AutoFlag, logMsg);

			return false;
		}

		checkMessage();
		Sleep(5);
	}

	return true;
}
bool CAxlMotorSet::Dispense_Z_Motor_Move(int index, double dVel)
{
	double eTime;
	short axis[1];
	double pos[1];

	char logMsg[1000];


	if (!m_bOrgFlag[Motor_PCB_Z])
	{
		sLangChange.LoadStringA(IDS_STRING555);	//PCB �� Z ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
		_stprintf_s(logMsg, sLangChange);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}

	pos[0] = model.axis[Motor_PCB_Z].pos[index];
	axis[0] = Motor_PCB_Z;

	/*pos[0]	= model.axis[Motor_PCB_Z].pos[index];
	axis[0] = Motor_PCB_Z;*/

	sLangChange.LoadStringA(IDS_STRING991);	//"PCB �� %s ���� �̵� �մϴ�.
	_stprintf_s(logMsg, sLangChange, MotorPosName[Motor_PCB_Z]);


	int step = 0;

	double startTime;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	while (1)
	{
		switch (step)
		{
		case 0:
			startTime = myTimer(true);
			DispenseGoMotorPos(1, axis, pos, dVel);
			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if (IsStopAxis(Motor_PCB_Z))
				step = 1000;
			break;

		default:
			break;
		}

		if (step == 1000)
			break;


		eTime = myTimer(true);
		if ((eTime - startTime)>MOTOR_MOVE_TIME)
		{
			if (step>100)
			{
				sLangChange.LoadStringA(IDS_STRING1479);	//PCB �� [%s ����] �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorPosName[Motor_PCB_Z]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING992);	//PCB �� %s �̵� ������
				_stprintf_s(logMsg, sLangChange, MotorPosName[Motor_PCB_Z]);
			}

			errMsg2(Task.AutoFlag, logMsg);

			return false;
		}

		checkMessage();
		Sleep(5);
	}

	return true;
}
bool CAxlMotorSet::PCB_Z_Motor_Move(int index)
{
	double eTime;
	short axis[1];
	double pos[1];

	char logMsg[1000];

	
	if(!m_bOrgFlag[Motor_PCB_Z])
	{
		sLangChange.LoadStringA(IDS_STRING555);	//PCB �� Z ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
		_stprintf_s(logMsg, sLangChange);
		errMsg2(Task.AutoFlag,logMsg);

		return false;
	}

	if(sysData.m_iProductComp == 1 && index == Bonding_Pos)//if(sysData.m_iProductComp == 1  && !(Task.dAAPcbMotor[0] == 0 && Task.dAAPcbMotor[1] == 0 && Task.dAAPcbMotor[2] == 0 && Task.dAAPcbMotor[3] == 0 && Task.dAAPcbMotor[4] == 0 && Task.dAAPcbMotor[5] == 0))
	{
#if (____AA_WAY == PCB_TILT_AA)
		pos[0] = Task.dAAPcbMotor[5];
		axis[0] = Motor_PCB_Z;
#elif (____AA_WAY == LENS_TILT_AA)
		pos[0] = model.axis[Motor_PCB_Z].pos[index];
		axis[0] = Motor_PCB_Z;
#endif
		
	}
	else
	{
		pos[0]	= model.axis[Motor_PCB_Z].pos[index];
		axis[0] = Motor_PCB_Z;
	}
	
	/*pos[0]	= model.axis[Motor_PCB_Z].pos[index];
	axis[0] = Motor_PCB_Z;*/

	sLangChange.LoadStringA(IDS_STRING991);	//"PCB �� %s ���� �̵� �մϴ�.
	_stprintf_s(logMsg, sLangChange, MotorPosName[Motor_PCB_Z]);


	int step=0;
	
	double startTime;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	while(1)
	{
		switch(step)
		{
		case 0:
			startTime = myTimer(true);
			goMotorPos(1, axis, pos);
			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if(IsStopAxis(Motor_PCB_Z))
				step = 1000;
			break;

		default:
			break;
		}

		if(step==1000)
			break;


		eTime = myTimer(true);
		if((eTime-startTime)>MOTOR_MOVE_TIME)
		{
			if(step>100)
			{
				sLangChange.LoadStringA(IDS_STRING1479);	//PCB �� [%s ����] �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorPosName[Motor_PCB_Z]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING992);	//PCB �� %s �̵� ������
				_stprintf_s(logMsg, sLangChange, MotorPosName[Motor_PCB_Z]);
			}

			errMsg2(Task.AutoFlag, logMsg);

			return false;
		}

		checkMessage();
		Sleep(5);
	}

	return true;
}

bool CAxlMotorSet::Pcb_Motor_Move_Laser(int Pos, double offsetX, double offsetY, double offsetTh)
{
	char logMsg[1000];
	double eTime = 0.0;
	short axis[3];
	double pos[3];
	int axis_cnt =0;

	if(!m_bOrgFlag[Motor_PCB_X] || !m_bOrgFlag[Motor_PCB_Y] || !m_bOrgFlag[Motor_PCB_TH])
	{
		sLangChange.LoadStringA(IDS_STRING1001);	//PCB �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
		_stprintf_s(logMsg, sLangChange);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}

	double MotorPos[3];
	MotorPos[0] = fabs(GetEncoderPos(Motor_Lens_X) - model.axis[Motor_Lens_X].pos[Bonding_Pos]);
	MotorPos[1] = fabs(GetEncoderPos(Motor_Lens_Y) - model.axis[Motor_Lens_Y].pos[Bonding_Pos]);
	MotorPos[2] = fabs(GetCommandPos(Motor_Lens_Z) - model.axis[Motor_Lens_Z].pos[Wait_Pos]);

	// Lens�� X,Y ���� ��ġ�� ��� , Lens Z �� Ȯ�� 
	if(MotorPos[0] < model.axis[Motor_Lens_X].m_dLimit_Err && MotorPos[1] < model.axis[Motor_Lens_Y].m_dLimit_Err)
	{
		if (MotorPos[2] > 1)		// PCB�� ���� ��ġ�� �̵� �Ҷ� Lens Z�� ��� ��ġ Ȯ��
		{
			//sLangChange.LoadStringA(IDS_STRING719);	//Lens ���Ͱ� ���� ��ġ�� �ֽ��ϴ�.
			//_stprintf_s(logMsg, sLangChange);
			errMsg2(Task.AutoFlag, "Lens Z�� ��Ⱑ �ƴմϴ�.");
			return false;
		}


	}


	double zPos = 0;
	

	if(fabs(offsetX)>model.axis[Motor_PCB_X].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING997);	//PCB �� X �� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_PCB_X].m_dLimit_Err, offsetX);
		errMsg2(Task.AutoFlag,logMsg);

		return false;
	}

	if(fabs(offsetY)>model.axis[Motor_PCB_Y].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING998);	//PCB �� Y �� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�.
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_PCB_Y].m_dLimit_Err, offsetY);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}

	if(fabs(offsetTh)>model.axis[Motor_PCB_TH].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING999);	//PCB �� ���� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_PCB_TH].m_dLimit_Err, offsetTh);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}


	axis[0] = Motor_PCB_X;
	axis[1] = Motor_PCB_Y;
	axis[2] = Motor_PCB_TH;
	int pMotorPos = -1;
	if (sysData.m_iProductComp == 1)
	{
		pMotorPos = Com_Laser_Pos;

	}
	else
	{
		pMotorPos = Laser_Pcb_Pos;
	}
	if( Pos == 0)
	{
		pos[0]	= model.axis[Motor_PCB_X].pos[pMotorPos] + offsetX;
		pos[1]	= model.axis[Motor_PCB_Y].pos[pMotorPos] + offsetY;
		pos[2]	= model.axis[Motor_PCB_TH].pos[pMotorPos] + offsetTh;
	}
	else if( Pos == 1)
	{
		if (sysData.m_iProductComp == 1)
		{
			pos[0] = model.axis[Motor_PCB_X].pos[pMotorPos] + offsetX - model.m_OutPos_X[0];
			pos[1] = model.axis[Motor_PCB_Y].pos[pMotorPos] + offsetY - model.m_OutPos_Y[0];
		}
		else
		{
			pos[0] = model.axis[Motor_PCB_X].pos[pMotorPos] + offsetX - model.m_PcbLaserPos_X[0];
			pos[1] = model.axis[Motor_PCB_Y].pos[pMotorPos] + offsetY - model.m_PcbLaserPos_Y[0];
		}
		
		pos[2]	= model.axis[Motor_PCB_TH].pos[pMotorPos] + offsetTh;
	}
	else if( Pos == 2)
	{
		if (sysData.m_iProductComp == 1)
		{
			pos[0] = model.axis[Motor_PCB_X].pos[pMotorPos] + offsetX - model.m_OutPos_X[1];
			pos[1] = model.axis[Motor_PCB_Y].pos[pMotorPos] + offsetY - model.m_OutPos_Y[1];
		}
		else
		{
			pos[0] = model.axis[Motor_PCB_X].pos[pMotorPos] + offsetX - model.m_PcbLaserPos_X[1];
			pos[1] = model.axis[Motor_PCB_Y].pos[pMotorPos] + offsetY - model.m_PcbLaserPos_Y[1];
		}
		
		pos[2]	= model.axis[Motor_PCB_TH].pos[pMotorPos] + offsetTh;
	}
	else if( Pos == 3)
	{
		if (sysData.m_iProductComp == 1)
		{
			pos[0] = model.axis[Motor_PCB_X].pos[pMotorPos] + offsetX - model.m_OutPos_X[2];
			pos[1] = model.axis[Motor_PCB_Y].pos[pMotorPos] + offsetY - model.m_OutPos_Y[2];
		}
		else
		{
			pos[0] = model.axis[Motor_PCB_X].pos[pMotorPos] + offsetX - model.m_PcbLaserPos_X[2];
			pos[1] = model.axis[Motor_PCB_Y].pos[pMotorPos] + offsetY - model.m_PcbLaserPos_Y[2];
		}
		
		pos[2]	= model.axis[Motor_PCB_TH].pos[pMotorPos] + offsetTh;
	}
	
	_stprintf_s(logMsg, "PCB �� %s�� �̵� �մϴ�.", MotorPosName[pMotorPos]);
	//	delayMsg(logMsg, 10000, M_COLOR_BLUE);


	int step=0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	double startTime;


	while(1)
	{
		switch(step)
		{
		case 0:									// 3�� ���� ��� ��ġ�� �̵� ���..
			startTime = myTimer(true);

			goMotorPos(3, axis, pos);

			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if( ( IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X) ) && 
				( IsStopAxis(Motor_PCB_Y) && GetInposition(Motor_PCB_Y) ) && 
				( IsStopAxis(Motor_PCB_TH)))// && GetInposition(Motor_PCB_TH) ) )
			{
				step = 101;
			}

			break;

		case 101:
			if( ( IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X) ) && 
			    ( IsStopAxis(Motor_PCB_Y) && GetInposition(Motor_PCB_Y) ) )
				step = 102;
			break;

		case 102:
			if( IsStopAxis(Motor_PCB_TH))// && GetInposition(Motor_PCB_TH) )
				step = 1000;
			break;

		default:
			break;
		}

		if(step==1000)
			break;


		eTime = myTimer(true);
		if(eTime-startTime > MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_Y);
			StopAxis(Motor_PCB_TH);

			if(step>100)
			{
				int motorNo = step - 100;
				sprintf_s(logMsg, "PCB �� [%s ����] %s �̵� �ð� �ʰ�", MotorName[motorNo], MotorPosName[pMotorPos]);
			}
			else
			{
				_stprintf_s(logMsg, "PCB �� %s �̵� ������", MotorPosName[pMotorPos]);
			}

			errMsg2(Task.AutoFlag,logMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}
	return true;
}


bool CAxlMotorSet::Lens_Motor_Move_Laser(int Pos, double offsetX, double offsetY, double offsetTh)
{
	char logMsg[1000];
	double eTime;
	short axis[2];
	double pos[2];
	int axis_cnt =0;

	if(!m_bOrgFlag[Motor_PCB_X] || !m_bOrgFlag[Motor_PCB_Y] || !m_bOrgFlag[Motor_PCB_TH])
	{
		sLangChange.LoadStringA(IDS_STRING1001);	//PCB �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
		_stprintf_s(logMsg, sLangChange);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}

	double MotorPos[3];
	MotorPos[0] = fabs(GetEncoderPos(Motor_Lens_X) - model.axis[Motor_Lens_X].pos[Bonding_Pos]);
	MotorPos[1] = fabs(GetEncoderPos(Motor_Lens_Y) - model.axis[Motor_Lens_Y].pos[Bonding_Pos]);
	//MotorPos[2] = fabs(GetEncoderPos(Motor_Lens_Z) - model.axis[Motor_Lens_Z].pos[Wait_Pos]);

	// Lens�� X,Y ���� ��ġ�� ��� , Lens Z �� Ȯ�� 
	if(MotorPos[0] < model.axis[Motor_Lens_X].m_dLimit_Err && MotorPos[1] < model.axis[Motor_Lens_Y].m_dLimit_Err)
	{
		//if(Dio.PCBCoverCloseCheck(true, false) == false)
		//{
		//	sLangChange.LoadStringA(IDS_STRING1026);	//PCB�� Ŀ�� ������ ���� �ֽ��ϴ�.
		//	_stprintf_s(logMsg, sLangChange);
		//	errMsg2(Task.AutoFlag,logMsg);
		//	return false;
		//}

		if(MotorPos[2] > 1)		// PCB�� ���� ��ġ�� �̵� �Ҷ� Lens Z�� ��� ��ġ Ȯ��
		{
			sLangChange.LoadStringA(IDS_STRING719);	//Lens ���Ͱ� ���� ��ġ�� �ֽ��ϴ�.
			_stprintf_s(logMsg, sLangChange);
			errMsg2(Task.AutoFlag,logMsg);
			return false;
		}
	}


	double zPos = 0;
	

	if(fabs(offsetX)>model.axis[Motor_Lens_X].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING997);	//PCB �� X �� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_PCB_X].m_dLimit_Err, offsetX);
		errMsg2(Task.AutoFlag,logMsg);

		return false;
	}

	if(fabs(offsetY)>model.axis[Motor_Lens_Y].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING998);	//PCB �� Y �� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�.
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_PCB_Y].m_dLimit_Err, offsetY);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}


	axis[0] = Motor_Lens_X;
	axis[1] = Motor_Lens_Y;
	//axis[2] = Motor_Lens_TH;
	if(fabs(model.m_OutPos_X[0])>55||model.m_OutPos_X[1]>55||model.m_OutPos_Y[0]>55||model.m_OutPos_Y[1]>55){return false;}
	if( Pos == 0)
	{
		pos[0]	= model.axis[Motor_Lens_X].pos[Laser_Lens_Pos] + offsetX;
		pos[1]	= model.axis[Motor_Lens_Y].pos[Laser_Lens_Pos] + offsetY;
	}
	else if( Pos == 1)
	{
		pos[0]	= model.axis[Motor_Lens_X].pos[Laser_Lens_Pos] + offsetX - model.m_OutPos_X[0];
		pos[1]	= model.axis[Motor_Lens_Y].pos[Laser_Lens_Pos] + offsetY - model.m_OutPos_Y[0];
	}
	else if( Pos == 2)
	{
		pos[0]	= model.axis[Motor_Lens_X].pos[Laser_Lens_Pos] + offsetX - model.m_OutPos_X[1];
		pos[1]	= model.axis[Motor_Lens_Y].pos[Laser_Lens_Pos] + offsetY - model.m_OutPos_Y[1];
	}
	else if( Pos == 3)
	{
		pos[0]	= model.axis[Motor_Lens_X].pos[Laser_Lens_Pos] + offsetX - model.m_OutPos_X[2];
		pos[1]	= model.axis[Motor_Lens_Y].pos[Laser_Lens_Pos] + offsetY - model.m_OutPos_Y[2];
	}
	_stprintf_s(logMsg, "Lens �� %s�� �̵� �մϴ�.", MotorPosName[Laser_Lens_Pos]);
		//delayMsg(logMsg, 10000, M_COLOR_BLUE);


	int step=0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	double startTime;


	while(1)
	{
		switch(step)
		{
		case 0:									// 3�� ���� ��� ��ġ�� �̵� ���..
			startTime = myTimer(true);
			goMotorPos(2, axis, pos);
			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if( ( IsStopAxis(Motor_Lens_X) && GetInposition(Motor_Lens_X) ) && 
				( IsStopAxis(Motor_Lens_Y) && GetInposition(Motor_Lens_Y) ) )
			{
				step = 101;
			}

			break;

		case 101:
			if( ( IsStopAxis(Motor_Lens_X) && GetInposition(Motor_Lens_X) ) && 
			    ( IsStopAxis(Motor_Lens_Y) && GetInposition(Motor_Lens_Y) ) )
				step = 102;
			break;

		case 102:
				step = 1000;
			break;

		default:
			break;
		}

		if(step==1000)
			break;


		eTime = myTimer(true);
		if(eTime-startTime > MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_Lens_X);
			StopAxis(Motor_Lens_Y);
			if(step>100)
			{
				int motorNo = step - 100;
				sprintf_s(logMsg, "Lens �� [%s ����] %s �̵� �ð� �ʰ�", MotorName[motorNo], MotorPosName[Laser_Lens_Pos]);
			}
			else
			{
				_stprintf_s(logMsg, "Lens �� %s �̵� ������", MotorPosName[Laser_Lens_Pos]);
				
			}

			errMsg2(Task.AutoFlag,logMsg);

			return false;
		}

		Sleep(5);
	}
	return true;
}


bool CAxlMotorSet::Pcb_Move_Tilt(double offsetX, double offsetY)
{
	char logMsg[1000];
	double eTime;
	short axis[3];
	double pos[3];
	int axis_cnt =0;

	if(!m_bOrgFlag[Motor_PCB_Xt] || !m_bOrgFlag[Motor_PCB_Yt])
	{
		sLangChange.LoadStringA(IDS_STRING1001);	//PCB �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
		_stprintf_s(logMsg, sLangChange);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}

	if(fabs(offsetX)>model.axis[Motor_PCB_Xt].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING997);	//PCB �� X �� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_PCB_X].m_dLimit_Err, offsetX);
		errMsg2(Task.AutoFlag,logMsg);

		return false;
	}

	if(fabs(offsetY)>model.axis[Motor_PCB_Yt].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING998);	//PCB �� Y �� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�.
		_stprintf_s(logMsg, sLangChange, model.axis[Motor_PCB_Y].m_dLimit_Err, offsetY);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}

	axis[0] = Motor_PCB_Xt;
	axis[1] = Motor_PCB_Yt;
	if (sysData.m_iProductComp == 1)
	{
		//pos[0] = offsetX;
		//pos[1] = offsetY;
		pos[0] = GetCommandPos(Motor_PCB_Xt) + offsetX;
		pos[1] = GetCommandPos(Motor_PCB_Yt) + offsetY;
	}
	else
	{
		pos[0] = GetCommandPos(Motor_PCB_Xt) + offsetX;
		pos[1] = GetCommandPos(Motor_PCB_Yt) + offsetY;
	}
	//pos[0] = GetEncoderPos(Motor_PCB_Xt) + offsetX;
	//pos[1] = GetEncoderPos(Motor_PCB_Yt) + offsetY;

	int step=0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	double startTime;


	while(1)
	{
		switch(step)
		{
		case 0:									// 3�� ���� ��� ��ġ�� �̵� ���..
			startTime = myTimer(true);

			goMotorPos(2, axis, pos);

			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if( ( IsStopAxis(Motor_PCB_Xt) /*&& GetInposition(Motor_PCB_Xt)*/ ) && 
				( IsStopAxis(Motor_PCB_Yt) /*&& GetInposition(Motor_PCB_Yt)*/ ) )
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
		if(eTime-startTime > MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_Xt);
			StopAxis(Motor_PCB_Yt);

			if(step>100)
			{
				int motorNo = step - 100;
				sLangChange.LoadStringA(IDS_STRING1482);	//PCB �� [%s ����] �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorName[motorNo]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING1483);	//PCB �� �̵� ������
				_stprintf_s(logMsg, sLangChange);
			}

			errMsg2(Task.AutoFlag,logMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}
	
	return true;
}

bool CAxlMotorSet::Lens_Move_Tilt(double offsetX, double offsetY)
{
	char logMsg[1000];
	double eTime;
	short axis[3];
	double pos[3];
	int axis_cnt =0;

	if(!m_bOrgFlag[Motor_Lens_Xt] || !m_bOrgFlag[Motor_Lens_Yt])
	{
		sLangChange.LoadStringA(IDS_STRING1001);	//PCB �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
		_stprintf_s(logMsg, sLangChange);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}

	if(fabs(offsetX)>model.axis[Motor_Lens_Xt].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING997);	//PCB �� X �� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�
		_stprintf_s(logMsg, _T("Lens TX-Axis Limit") );// model.axis[Motor_Lens_Xt].m_dLimit_Err, offsetX);
		errMsg2(Task.AutoFlag,logMsg);

		return false;
	}

	if(fabs(offsetY)>model.axis[Motor_Lens_Yt].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING998);	//PCB �� Y �� ������ ��� ����[�� %.01f]�� �ʰ� �Ͽ����ϴ�.
		_stprintf_s(logMsg, _T("Lens TY-Axis Limit"));// , model.axis[Motor_Lens_Y].m_dLimit_Err, offsetY);
		errMsg2(Task.AutoFlag, logMsg);

		return false;
	}

	axis[0] = Motor_Lens_Xt;
	axis[1] = Motor_Lens_Yt;

	pos[0] = GetCommandPos(Motor_Lens_Xt) + offsetX;
	pos[1]	= GetEncoderPos(Motor_Lens_Yt) - offsetY;

	int step=0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	double startTime;


	while(1)
	{
		switch(step)
		{
		case 0:									// 3�� ���� ��� ��ġ�� �̵� ���..
			startTime = myTimer(true);

			goMotorPos(2, axis, pos);

			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if( ( IsStopAxis(Motor_Lens_Xt) /*&& GetInposition(Motor_PCB_Xt)*/ ) && 
				( IsStopAxis(Motor_Lens_Yt) /*&& GetInposition(Motor_PCB_Yt)*/ ) )
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
		if(eTime-startTime > MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_Lens_Xt);
			StopAxis(Motor_Lens_Yt);

			if(step>100)
			{
				int motorNo = step - 100;
				sLangChange.LoadStringA(IDS_STRING1482);	//PCB �� [%s ����] �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorName[motorNo]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING1483);	//PCB �� �̵� ������
				_stprintf_s(logMsg, sLangChange);
			}

			errMsg2(Task.AutoFlag,logMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}
	
	return true;
}

bool CAxlMotorSet::Pcb_Holder_Align_Move(int index, double offsetX, double offsetY, double offsetTh)
{
	char logMsg[1000];
	double eTime;
	short axis[4];
	double pos[4];
	int axis_cnt =0;
	double dOffsetLimit = 1.0;

	CString strlog;
	for (int i=Motor_PCB_X; i<=Motor_PCB_TH; i++)
	{
		if(!m_bOrgFlag[i])
		{
			sLangChange.LoadStringA(IDS_STRING1001);	//PCB �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
			_stprintf_s(logMsg, _T("%s ") + sLangChange, MotorName[i]);
			errMsg2(Task.AutoFlag, logMsg);
			return false;
		}
		 
		axis[i-Motor_PCB_X]		= i;
	
		if(i==Motor_PCB_X)				pos[i-Motor_PCB_X]	= model.axis[i].pos[index] + offsetX;
		else if (i == Motor_PCB_Y)		pos[i - Motor_PCB_X] = model.axis[i].pos[index] + offsetY;
		else if (i == Motor_PCB_Z)		pos[i - Motor_PCB_X] = model.axis[i].pos[index];
		else if(i==Motor_PCB_TH)		pos[i-Motor_PCB_X]	= model.axis[i].pos[index] + offsetTh;
	}

	double MotorPos[2];
	MotorPos[0] = GetEncoderPos(Motor_PCB_X) - model.axis[Motor_PCB_X].pos[index];
	MotorPos[1] = GetEncoderPos(Motor_PCB_Y) - model.axis[Motor_PCB_Y].pos[index];

	if(fabs(MotorPos[0]) > model.axis[Motor_PCB_X].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING970);	//PCB X ���Ͱ� Align ���� Limit�� ���� �����ϴ�. [Encoder %.03lf, Teaching %.03lf]
		_stprintf_s(logMsg, sLangChange, GetEncoderPos(Motor_PCB_X), model.axis[Motor_PCB_X].pos[Align_Pos]);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}

	if(fabs(MotorPos[1]) > model.axis[Motor_PCB_Y].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING973);	//PCB Y ���Ͱ� Align ���� Limit�� ���� �����ϴ�. [Encoder %.03lf, Teaching %.03lf]
		_stprintf_s(logMsg, sLangChange, GetEncoderPos(Motor_PCB_Y), model.axis[Motor_PCB_Y].pos[Align_Pos]);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}
	//20170215 holder �κ� th���� �߰�
	if(fabs(MotorPos[1]) > model.axis[Motor_PCB_TH].m_dLimit_Err)//
	{
		sLangChange.LoadStringA(IDS_STRING973);	//PCB Y ���Ͱ� Align ���� Limit�� ���� �����ϴ�. [Encoder %.03lf, Teaching %.03lf]
		_stprintf_s(logMsg, "PCB TH ���Ͱ� Align ���� Limit�� ���� �����ϴ�. [Encoder %.03lf, Teaching %.03lf]", GetCommandPos(Motor_PCB_TH), model.axis[Motor_PCB_TH].pos[Align_Pos]);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}

	int step=0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif


	double startTime;

	while(1)
	{
		switch(step)
		{
		case 0:									// 3�� ���� ��� ��ġ�� �̵� ���..
			startTime = myTimer(true);
			goMotorPos(4, axis, pos);
			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if( IsStopAxis(Motor_PCB_X) && GetInposition(Motor_PCB_X) )			step = 101;
			break;

		case 101:
			if ( IsStopAxis(Motor_PCB_Y) && GetInposition(Motor_PCB_Y) )		step = 102;
			break;

		case 102:
			//if ( IsStopAxis(Motor_PCB_TH) && GetInposition(Motor_PCB_TH) )		step = 1000;
			if (IsStopAxis(Motor_PCB_Z))		step = 103;
			break;
		case 103:
			//if ( IsStopAxis(Motor_PCB_TH) && GetInposition(Motor_PCB_TH) )		step = 1000;
			if (IsStopAxis(Motor_PCB_TH))		step = 1000;
			break;

		default:
			break;
		}

		if(step==1000)
			break;


		eTime = myTimer(true);
		if(eTime-startTime > MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_Y);
			StopAxis(Motor_PCB_Z);
			StopAxis(Motor_PCB_TH);

			if(step>100)
			{
				int motorNo = step - 100;
				//sLangChange.LoadStringA(IDS_STRING1482);	//PCB �� [%s ����] �̵� �ð� �ʰ�
				//_stprintf_s(logMsg, sLangChange, MotorName[motorNo]);
				strlog.Format("PCB �� [%s ����] �̵� �ð� �ʰ�", MotorName[motorNo]);
			}
			else
			{
				//sLangChange.LoadStringA(IDS_STRING994);	//PCB �� Align ���� �̵� �ð� �ʰ�
				//_stprintf_s(logMsg, sLangChange);
				strlog.Format("PCB �� Align ���� �̵� �ð� �ʰ�");
			}

			errMsg2(Task.AutoFlag, strlog);

			return false;
		}

		Sleep(5);
		checkMessage();
	}


#ifdef ON_LINE_MOTOR
	double curPos = 0.0;
	for (int i=Motor_PCB_X; i<=Motor_PCB_TH; i++)
	{
		if (i == Motor_PCB_Z)continue;
		if (MOTOR_TYPE[i] == STEPING)
		{
			curPos = GetCommandPos(axis[i - Motor_PCB_X]);
		}
		else
		{
			curPos = GetEncoderPos(axis[i - Motor_PCB_X]);
		}
		
		double cmdPos = GetCommandPos(axis[i-Motor_PCB_X]); 

		double errVal = fabs(pos[i-Motor_PCB_X] - curPos);

		if (errVal>5.5)//IN_POSITION_VAL)
		{
			sLangChange.LoadStringA(IDS_STRING995);	//PCB �� Align ���� �̵� �� %s ���� ��ġ ���� �߻�
			_stprintf_s(logMsg, sLangChange, MotorName[i], pos[i], curPos, cmdPos);
			strlog.Format("PCB �� Align ���� �̵� �� %s ���� ��ġ ���� �߻� (��갪:%.03lf, Encoder %.03lf, Command %.03lf)",
				MotorName[i], pos[i], curPos, cmdPos);
			//LogSave(logMsg);

			//checkMessage();


			for(int j=0; j<10; j++)
			{
				Sleep(5);
				checkMessage();
			}

			if (MOTOR_TYPE[i] == STEPING)
			{
				curPos = GetCommandPos(axis[i - Motor_PCB_X]);
			}
			else
			{
				curPos = GetEncoderPos(axis[i - Motor_PCB_X]);
			}
			
			cmdPos = GetCommandPos(axis[i-Motor_PCB_X]);

			errVal = fabs(pos[i-Motor_PCB_X] - curPos);

			if (errVal>5.5)//IN_POSITION_VAL)
			{
				sLangChange.LoadStringA(IDS_STRING995);	//PCB �� Align ���� �̵� �� %s ���� ��ġ ���� �߻�
				_stprintf_s(logMsg, sLangChange, MotorName[i], pos[i], curPos, cmdPos);
				errMsg2(Task.AutoFlag, logMsg);

				return false;
			}		

			return false;
		}
	}
#endif


	return true;
}

bool CAxlMotorSet::moveEpoxyCleanPos(int iIndex, bool waitFlag)
{
	char logMsg[1000];
	double eTime;
	short axis[4] = {Motor_PCB_X, Motor_PCB_Y, Motor_PCB_TH};
	double pos[4];
	int iIndex_X, iIndex_Y;
	int iCntX, iCntY;

	if(!m_bOrgFlag[Motor_PCB_X] || !m_bOrgFlag[Motor_PCB_Y] || !m_bOrgFlag[Motor_PCB_TH])
	{
		sLangChange.LoadStringA(IDS_STRING1001);	//PCB �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
		_stprintf_s(logMsg, sLangChange);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}
	if ( (motor.IsStopAxis(Motor_PCB_X) == false)		||
		(motor.IsStopAxis(Motor_PCB_Y) == false)		)
	{
		sLangChange.LoadStringA(IDS_STRING551);	//Epoxy�� X, Y �� ���Ͱ� ���� ���Դϴ�.
		_stprintf_s(logMsg, sLangChange);
		errMsg2(Task.AutoFlag, logMsg);
		return false;
	}


	if(!PCB_Z_Motor_Move(Wait_Pos))
		return false;


	iCntX = (int)(model.Cleanpad[0]);
	iCntY = (int)(model.Cleanpad[1]);

	if(iCntX == 0 && iCntY == 0)
	{
		sLangChange.LoadStringA(IDS_STRING466);
		_stprintf_s(logMsg, sLangChange);	//clean pad ũ�Ⱑ 0 �Դϴ�!!
		errMsg2(Task.AutoFlag, logMsg);
		return false;
	}
	if (iIndex >= iCntX*iCntY)
	{
		model.Cleancnt = 0;
		iIndex = 0;
	}

	iIndex_X = iIndex % iCntX;
	iIndex_Y = iIndex / iCntX;

	pos[0] = model.axis[Motor_PCB_X].pos[Clean_Pos] - iIndex_X;
	pos[1] = model.axis[Motor_PCB_Y].pos[Clean_Pos] + iIndex_Y;
	pos[2] = model.axis[Motor_PCB_TH].pos[Clean_Pos];

	goMotorPos(3, axis, pos);

	int step=100;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	eTime = myTimer(true);
	while (waitFlag)
	{
		switch(step)
		{
		case 100:		
			if(motor.IsStopAxis(Motor_PCB_X))
				step = 200;
			break;
		case 200:
			if(motor.IsStopAxis(Motor_PCB_Y))
				step = 300;
			break;
		case 300:
			if(motor.IsStopAxis(Motor_PCB_TH))
				step = 1000;
			break;
		default:
			break;
		}

		if(step==1000)
			break;


		if(myTimer(true) - eTime>MOTOR_MOVE_TIME)
		{
			motor.StopAxis(Motor_PCB_X);
			motor.StopAxis(Motor_PCB_Y);
			motor.StopAxis(Motor_PCB_TH);

			sLangChange.LoadStringA(IDS_STRING550);
			_stprintf_s(logMsg, sLangChange);	//"Epoxy�� X, Y �� Clean ��ġ �̵� ����"
			errMsg2(Task.AutoFlag,logMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}

	return true;
}

bool CAxlMotorSet::Pcb_Tilt_Align_Move(double offsetX, double offsetY)
{
	char logMsg[1000];
	double eTime;
	short axis[3];
	double pos[3];
	int axis_cnt =0;
	double dOffsetLimit = 1.0;

	
	for (int i = Motor_PCB_Yt; i <= Motor_PCB_Xt; i++)
		//for (int i=Motor_PCB_Xt; i<=Motor_PCB_Yt; i++)
	{
		if(!m_bOrgFlag[i])
		{
			sLangChange.LoadStringA(IDS_STRING1001);	//PCB �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
			_stprintf_s(logMsg, _T("%s ") + sLangChange, MotorName[i]);
			errMsg2(Task.AutoFlag, logMsg);
			return false;
		}

		axis[i- Motor_PCB_Yt]		= i;
	
		if(i==Motor_PCB_Xt)					pos[i- Motor_PCB_Yt]	= model.axis[i].pos[Laser_Lens_Pos] + offsetX;
		else if(i==Motor_PCB_Yt)			pos[i- Motor_PCB_Yt]	= model.axis[i].pos[Laser_Lens_Pos] + offsetY;
	}

	double MotorPos[2];
	MotorPos[0] = GetCommandPos(Motor_PCB_Xt) - model.axis[Motor_PCB_Xt].pos[Laser_Lens_Pos];
	MotorPos[1] = GetCommandPos(Motor_PCB_Yt) - model.axis[Motor_PCB_Yt].pos[Laser_Lens_Pos];

	if(fabs(MotorPos[0]) > model.axis[Motor_PCB_Xt].m_dLimit_Err)
	{	
		sLangChange.LoadStringA(IDS_STRING1490);
		sprintf_s(logMsg, sLangChange, GetCommandPos(Motor_PCB_Xt), model.axis[Motor_PCB_Xt].pos[Laser_Lens_Pos]);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}

	if(fabs(MotorPos[1]) > model.axis[Motor_PCB_Yt].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING1491);
		sprintf_s(logMsg, sLangChange, GetCommandPos(Motor_PCB_Yt), model.axis[Motor_PCB_Yt].pos[Laser_Lens_Pos]);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}

	int step=0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif


	double startTime;

	while(1)
	{
		switch(step)
		{
		case 0:									// 3�� ���� ��� ��ġ�� �̵� ���..
			startTime = myTimer(true);
			goMotorPos(2, axis, pos);
			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if( IsStopAxis(Motor_PCB_Xt) )		step = 101;
			break;

		case 101:
			if ( IsStopAxis(Motor_PCB_Yt) )		step = 1000;
			break;

		default:
			break;
		}

		if(step==1000)
			break;


		eTime = myTimer(true);
		if(eTime-startTime > MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_Xt);
			StopAxis(Motor_PCB_Yt);

			if(step>100)
			{
				int motorNo = step - 100;
				sLangChange.LoadStringA(IDS_STRING1482);	//PCB �� [%s ����] �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorName[motorNo]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING994);	//PCB �� Align ���� �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange);
			}

			errMsg2(Task.AutoFlag,logMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}


#ifdef ON_LINE_MOTOR
	for (int i = Motor_PCB_Yt; i <= Motor_PCB_Xt; i++)
	//for (int i=Motor_PCB_Xt; i<=Motor_PCB_Yt; i++)
	{
		double curPos = GetEncoderPos(axis[i- Motor_PCB_Yt]);
		double cmdPos = GetCommandPos(axis[i- Motor_PCB_Yt]);

		double errVal = fabs(pos[i- Motor_PCB_Yt] - curPos);

		if (errVal>IN_POSITION_VAL)
		{
			sLangChange.LoadStringA(IDS_STRING995);	//PCB �� Align ���� �̵� �� %s ���� ��ġ ���� �߻�
			_stprintf_s(logMsg, sLangChange, MotorName[i], pos[i], curPos, cmdPos);
			LogSave(logMsg);

			checkMessage();


			for(int j=0; j<10; j++)
			{
				Sleep(5);
				checkMessage();
			}


			curPos = GetEncoderPos(axis[i- Motor_PCB_Yt]);
			cmdPos = GetCommandPos(axis[i- Motor_PCB_Yt]);

			errVal = fabs(pos[i- Motor_PCB_Yt] - curPos);

			if (errVal>IN_POSITION_VAL)
			{
				sLangChange.LoadStringA(IDS_STRING995);	//PCB �� Align ���� �̵� �� %s ���� ��ġ ���� �߻�
				_stprintf_s(logMsg, sLangChange, MotorName[i], pos[i], curPos, cmdPos);
				errMsg2(Task.AutoFlag, logMsg);

				return false;
			}		

			return false;
		}
	}
#endif


	return true;
}

bool CAxlMotorSet::Pcb_Tilt_Trinity_Move(double offsetX, double offsetY)
{
	char logMsg[1000];
	double eTime;
	short axis[3];
	double pos[3] = { 0.0, };
	int axis_cnt = 0;
	double dOffsetLimit = 1.0;


	for (int i = Motor_PCB_Yt; i <= Motor_PCB_Xt; i++)
		//for (int i=Motor_PCB_Xt; i<=Motor_PCB_Yt; i++)
	{
		if (!m_bOrgFlag[i])
		{
			sLangChange.LoadStringA(IDS_STRING1001);	//PCB �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
			_stprintf_s(logMsg, _T("%s ") + sLangChange, MotorName[i]);
			errMsg2(Task.AutoFlag, logMsg);
			return false;
		}

		/*axis[i-Motor_PCB_Yt]		= i;

		if(i==Motor_PCB_Yt)					pos[i-Motor_PCB_Yt]	= model.axis[i].pos[index] + offsetX;
		else if(i==Motor_PCB_Yt)			pos[i-Motor_PCB_Yt]	= model.axis[i].pos[index] + offsetY;*/
	}


	//double MotorPos[2] = { 0.0, };
	//MotorPos[0] = GetCommandPos(Motor_PCB_Xt) - offsetX;// model.axis[Motor_PCB_Xt].pos[index];
	//MotorPos[1] = GetCommandPos(Motor_PCB_Yt) - offsetY;// model.axis[Motor_PCB_Yt].pos[index];

	//if (fabs(MotorPos[0]) > model.axis[Motor_PCB_Xt].m_dLimit_Err)
	//{
	//	sLangChange.LoadStringA(IDS_STRING1490);
	//	sprintf_s(logMsg, sLangChange, GetCommandPos(Motor_PCB_Xt), model.axis[Motor_PCB_Xt].pos[Laser_Pcb_Pos]);
	//	errMsg2(Task.AutoFlag, logMsg);
	//	return false;
	//}

	//if (fabs(MotorPos[1]) > model.axis[Motor_PCB_Yt].m_dLimit_Err)
	//{
	//	sLangChange.LoadStringA(IDS_STRING1491);
	//	sprintf_s(logMsg, sLangChange, GetCommandPos(Motor_PCB_Yt), model.axis[Motor_PCB_Yt].pos[Laser_Pcb_Pos]);
	//	errMsg2(Task.AutoFlag, logMsg);
	//	return false;
	//}
	axis[0] = Motor_PCB_Xt;
	axis[1] = Motor_PCB_Yt;

	pos[0] = offsetX;		// model.axis[Motor_PCB_Xt].pos[index];// + offsetX;
	pos[1] = offsetY;		// model.axis[Motor_PCB_Yt].pos[index];// + offsetX;
	int step = 0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif


	double startTime;

	while (1)
	{
		switch (step)
		{
		case 0:									// 2�� ���� ��� ��ġ�� �̵� ���..
			startTime = myTimer(true);
			goMotorPos(2, axis, pos);//goMotorPos(2, axis, pos);
			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if (IsStopAxis(Motor_PCB_Xt))		step = 101;
			break;

		case 101:
			if (IsStopAxis(Motor_PCB_Yt))		step = 1000;
			break;

		default:
			break;
		}

		if (step == 1000)
			break;


		eTime = myTimer(true);
		if (eTime - startTime > MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_Xt);
			StopAxis(Motor_PCB_Yt);

			if (step>100)
			{
				int motorNo = step - 100;
				sLangChange.LoadStringA(IDS_STRING1482);	//PCB �� [%s ����] �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorName[motorNo]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING1002);	//PCB �� ƿ���� �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange);
			}

			errMsg2(Task.AutoFlag, logMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}

	return true;
}
 




bool CAxlMotorSet::Pcb_Tilt_Motor_Move(int index, double offsetX, double offsetY)
{
	char logMsg[1000];
	double eTime;
	short axis[3];
	double pos[3] = {0.0, };
	int axis_cnt =0;
	double dOffsetLimit = 1.0;

	
	for (int i = Motor_PCB_Yt; i <= Motor_PCB_Xt; i++)
	//for (int i=Motor_PCB_Xt; i<=Motor_PCB_Yt; i++)
	{
		if(!m_bOrgFlag[i])
		{
			sLangChange.LoadStringA(IDS_STRING1001);	//PCB �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
			_stprintf_s(logMsg, _T("%s ") + sLangChange, MotorName[i]);
			errMsg2(Task.AutoFlag, logMsg);
			return false;
		}

		/*axis[i-Motor_PCB_Yt]		= i;
	
		if(i==Motor_PCB_Yt)					pos[i-Motor_PCB_Yt]	= model.axis[i].pos[index] + offsetX;
		else if(i==Motor_PCB_Yt)			pos[i-Motor_PCB_Yt]	= model.axis[i].pos[index] + offsetY;*/
	}


	double MotorPos[2] = {0.0, };
	MotorPos[0] = GetCommandPos(Motor_PCB_Xt) - model.axis[Motor_PCB_Xt].pos[index];
	MotorPos[1] = GetCommandPos(Motor_PCB_Yt) - model.axis[Motor_PCB_Yt].pos[index];

	if(fabs(MotorPos[0]) > model.axis[Motor_PCB_Xt].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING1490);
		sprintf_s(logMsg, sLangChange, GetCommandPos(Motor_PCB_Xt), model.axis[Motor_PCB_Xt].pos[Laser_Pcb_Pos]);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}

	if(fabs(MotorPos[1]) > model.axis[Motor_PCB_Yt].m_dLimit_Err)
	{
		sLangChange.LoadStringA(IDS_STRING1491);
		sprintf_s(logMsg, sLangChange, GetCommandPos(Motor_PCB_Yt), model.axis[Motor_PCB_Yt].pos[Laser_Pcb_Pos]);
		errMsg2(Task.AutoFlag,logMsg);
		return false;
	}
	if(sysData.m_iProductComp == 1 && index == Bonding_Pos)
	{
		axis[0] = Motor_PCB_Xt;
		axis[1] = Motor_PCB_Yt;
		pos[0]	= Task.dAAPcbMotor[3];	 //PCB TX 
		pos[1]	= Task.dAAPcbMotor[4];	 //PCB TY 
	}else
	{
		axis[0] = Motor_PCB_Xt;
		axis[1] = Motor_PCB_Yt;

		pos[0]	=  model.axis[Motor_PCB_Xt].pos[index];// + offsetX;
		pos[1]	=  model.axis[Motor_PCB_Yt].pos[index];// + offsetX;
	}
	int step=0;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif


	double startTime;

	while(1)
	{
		switch(step)
		{
		case 0:									// 2�� ���� ��� ��ġ�� �̵� ���..
			startTime = myTimer(true);
			goMotorPos(2, axis, pos);//goMotorPos(2, axis, pos);
			step = 100;
			break;

		case 100:								// �̵� �Ϸ� Ȯ��..
			if( IsStopAxis(Motor_PCB_Xt) )		step = 101;
			break;

		case 101:
			if ( IsStopAxis(Motor_PCB_Yt) )		step = 1000;
			break;

		default:
			break;
		}

		if(step==1000)
			break;


		eTime = myTimer(true);
		if(eTime-startTime > MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_Xt);
			StopAxis(Motor_PCB_Yt);

			if(step>100)
			{
				int motorNo = step - 100;
				sLangChange.LoadStringA(IDS_STRING1482);	//PCB �� [%s ����] �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange, MotorName[motorNo]);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING1002);	//PCB �� ƿ���� �̵� �ð� �ʰ�
				_stprintf_s(logMsg, sLangChange);
			}

			errMsg2(Task.AutoFlag,logMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}

	return true;
}




bool CAxlMotorSet::moveSensorContactPos(bool waitFlag)
{
	double eTime;
	short axis[4] = {Motor_PCB_X, Motor_PCB_Y, Motor_PCB_TH, Motor_PCB_Z};//short axis[2] = {Motor_PCB_Y, Motor_Epoxy_Z};
	double pos[4];

	if( checkOriginStatus(3, axis) == false )
	{
		sLangChange.LoadStringA(IDS_STRING552);	//Epoxy�� X, Y, Th �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�.
		_stprintf_s(m_szLogMsg, sLangChange);
		errMsg2(Task.AutoFlag, m_szLogMsg);
		return false;
	}

	if ( (IsStopAxis(Motor_PCB_X) == false) || (IsStopAxis(Motor_PCB_Y) == false) )
	{
		sLangChange.LoadStringA(IDS_STRING551);	//Epoxy�� X, Y �� ���Ͱ� ���� ���Դϴ�.
		_stprintf_s(m_szLogMsg, sLangChange);
		errMsg2(Task.AutoFlag, m_szLogMsg);
		return false;
	}

	if(!moveWaitPos_Z(true))	return false;

	pos[0] = model.axis[Motor_PCB_X].pos[Contact_Pos]; 
	pos[1] = model.axis[Motor_PCB_Y].pos[Contact_Pos];
	pos[2] = model.axis[Motor_PCB_TH].pos[Contact_Pos];
	goMotorPos(3, axis, pos);

	while(1)
	{
		if( (IsStopAxis(Motor_PCB_X)) && IsStopAxis(Motor_PCB_Y) && IsStopAxis(Motor_PCB_TH) ) break; 
	}

	pos[0] = model.axis[Motor_PCB_X].pos[Contact_Pos]; 
	pos[1] = model.axis[Motor_PCB_Y].pos[Contact_Pos];
	pos[2] = model.axis[Motor_PCB_TH].pos[Contact_Pos];
	pos[3] = model.axis[Motor_PCB_Z].pos[Contact_Pos] - 1.0;
	goMotorPos(4, axis, pos);

	int step=100;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	eTime = myTimer(true);
	while (waitFlag)
	{
		switch(step)
		{
		case 100:		
			if(IsStopAxis(Motor_PCB_Y))
				step = 200;
			break;

		case 200:
			if (IsStopAxis(Motor_PCB_Z) == true)
				step = 1000;
		default:
			break;
		}

		if(step==1000)
			break;


		if(myTimer(true) - eTime>MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_Y);
			StopAxis(Motor_PCB_Z);
			
			sLangChange.LoadStringA(IDS_STRING554);	//Epoxy�� Y, Z�� ���� ���� ��ġ �̵� ����
			_stprintf_s(m_szLogMsg, sLangChange);
			errMsg2(Task.AutoFlag,m_szLogMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}

	return true;
}




bool CAxlMotorSet::checkOriginStatus(int iCnt, short* axis)
{
	CString sLog;

	for (int i=0 ; i<iCnt ; i++)
	{
		if ( GetAmpFault(axis[i]) != false )
		{
			sLangChange.LoadStringA(IDS_STRING1245);
			sLog.Format(_T("%s ") + sLangChange, MotorName[axis[i]]);	//���Ͱ� �˶� �����Դϴ�.
			errMsg2(Task.AutoFlag, sLog);
			return false;
		}
	}

	for (int i=0 ; i<iCnt ; i++)
	{
		if ( m_bOrgFlag[axis[i]] == false )
		{
			sLangChange.LoadStringA(IDS_STRING1246);
			sLog.Format(_T("%s ") + sLangChange, MotorName[axis[i]]);	//���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�
			errMsg2(Task.AutoFlag, sLog);
			return false;
		}
	}

	return true;
}

bool CAxlMotorSet::moveWaitPos_Z(bool waitFlag)
{
	double eTime;
	short axis[2];
	double pos[2];

	if(!m_bOrgFlag[Motor_PCB_Z])
	{	
		sLangChange.LoadStringA(IDS_STRING555);
		_stprintf_s(m_szLogMsg, sLangChange);	//"Epoxy�� Z �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�."
		errMsg2(Task.AutoFlag, m_szLogMsg);
		return false;
	}

	if ( IsStopAxis(Motor_PCB_Z) == false )
	{
		sLangChange.LoadStringA(IDS_STRING558);
		_stprintf_s(m_szLogMsg, sLangChange);	//"Epoxy�� Z�� ���Ͱ� ���� ���Դϴ�."
		errMsg2(Task.AutoFlag, m_szLogMsg);
		return false;
	}

	axis[0] = Motor_PCB_Z;
	pos[0] = model.axis[Motor_PCB_Z].pos[Wait_Pos];

	goMotorPos(1, axis, pos);

	int step=100;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	eTime = myTimer(true);
	while (waitFlag)
	{
		switch(step)
		{
		case 100:		
			if(IsStopAxis(Motor_PCB_Z))
				step = 1000;
			break;

		default:
			break;
		}

		if(step==1000)
			break;


		if((myTimer(true) - eTime) > MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_Z);
			sLangChange.LoadStringA(IDS_STRING556);
			_stprintf_s(m_szLogMsg, sLangChange);	//"Epoxy�� Z�� ��� ��ġ �̵� ����"
			errMsg2(Task.AutoFlag,m_szLogMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}
	return true;
}



bool CAxlMotorSet::movePointDischargePos(bool waitFlag)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	double eTime;
	short axis[3] = {Motor_PCB_X, Motor_PCB_Y, Motor_PCB_TH};
	double pos[3];

	if( checkOriginStatus(3, axis) == false )
	{
		sLangChange.LoadStringA(IDS_STRING1246);
		_stprintf_s(m_szLogMsg, sLangChange);	//"PCB�� X, Th, Y �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�."
		errMsg2(Task.AutoFlag, m_szLogMsg);
		return false;
	}

	if ( (IsStopAxis(Motor_PCB_X) == false)		||
		 (IsStopAxis(Motor_PCB_TH) == false)	||
		 (IsStopAxis(Motor_PCB_Y) == false)		)
	{
		sLangChange.LoadStringA(IDS_STRING1019);
		_stprintf_s(m_szLogMsg, sLangChange);	//"PCB�� X, Th, Y �� ���Ͱ� ���� ���Դϴ�."
		errMsg2(Task.AutoFlag, m_szLogMsg);
		return false;
	}

	if(!moveWaitPos_Z(true))
		return false;

	pos[0]	= model.axis[Motor_PCB_X].pos[Calc_Pos];
	pos[1]	= model.axis[Motor_PCB_Y].pos[Calc_Pos];
	pos[2]	= model.axis[Motor_PCB_TH].pos[Calc_Pos];

	//startMoveAxis(3, axis, pos);
	goMotorPos(3, axis, pos);

	int step=100;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	eTime = myTimer(true);
	while (waitFlag)
	{
		switch(step)
		{
		case 100:		
			if(IsStopAxis(Motor_PCB_X))
				step = 200;
			break;
		case 200:
			if(IsStopAxis(Motor_PCB_TH))
				step = 300;
			break;
		case 300:
			if(IsStopAxis(Motor_PCB_Y))
				step = 1000;
			break;
		default:
			break;
		}

		if(step==1000)
			break;


		if(myTimer(true) - eTime>MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_TH);
			StopAxis(Motor_PCB_Y);

			sLangChange.LoadStringA(IDS_STRING1018);
			_stprintf_s(m_szLogMsg, sLangChange);		//"PCB�� X, Th, Y �� Point ���� ��ġ �̵� ����"
			errMsg2(Task.AutoFlag,m_szLogMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}

	return true;
}

//���� calc2

bool CAxlMotorSet::movePointDischarge2Pos(bool waitFlag)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	double eTime;
	short axis[3] = {Motor_PCB_X, Motor_PCB_Y, Motor_PCB_TH};
	double pos[3];

	if( checkOriginStatus(3, axis) == false )
	{
		sLangChange.LoadStringA(IDS_STRING1246);
		_stprintf_s(m_szLogMsg, sLangChange);	//"PCB�� X, Th, Y �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�."
		errMsg2(Task.AutoFlag, m_szLogMsg);
		return false;
	}

	if ( (IsStopAxis(Motor_PCB_X) == false)		||
		 (IsStopAxis(Motor_PCB_TH) == false)	||
		 (IsStopAxis(Motor_PCB_Y) == false)		)
	{
		sLangChange.LoadStringA(IDS_STRING1019);
		_stprintf_s(m_szLogMsg, sLangChange);	//"PCB�� X, Th, Y �� ���Ͱ� ���� ���Դϴ�."
		errMsg2(Task.AutoFlag, m_szLogMsg);
		return false;
	}

	if(!moveWaitPos_Z(true))
		return false;

	pos[0]	= model.axis[Motor_PCB_X].pos[Calc2_Pos];
	pos[1]	= model.axis[Motor_PCB_Y].pos[Calc2_Pos];
	pos[2]	= model.axis[Motor_PCB_TH].pos[Calc2_Pos];

	//startMoveAxis(3, axis, pos);
	goMotorPos(3, axis, pos);

	int step=100;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	eTime = myTimer(true);
	while (waitFlag)
	{
		switch(step)
		{
		case 100:		
			if(IsStopAxis(Motor_PCB_X))
				step = 200;
			break;
		case 200:
			if(IsStopAxis(Motor_PCB_TH))
				step = 300;
			break;
		case 300:
			if(IsStopAxis(Motor_PCB_Y))
				step = 1000;
			break;
		default:
			break;
		}

		if(step==1000)
			break;


		if(myTimer(true) - eTime>MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_X);
			StopAxis(Motor_PCB_TH);
			StopAxis(Motor_PCB_Y);

			sLangChange.LoadStringA(IDS_STRING1018);
			_stprintf_s(m_szLogMsg, sLangChange);		//"PCB�� X, Th, Y �� Point ���� ��ġ �̵� ����"
			errMsg2(Task.AutoFlag,m_szLogMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}

	return true;
}

bool CAxlMotorSet::moveDispensePos_Z(int iPcbNo, bool waitFlag)
{
	double eTime;
	short axis[2];
	double pos[2];

	if(!m_bOrgFlag[Motor_PCB_Z])
	{
		sLangChange.LoadStringA(IDS_STRING555);
		_stprintf_s(m_szLogMsg, sLangChange);	//"Epoxy�� Z �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�."
		errMsg2(Task.AutoFlag, m_szLogMsg);
		return false;
	}

	if ( IsStopAxis(Motor_PCB_Z) == false )
	{
		sLangChange.LoadStringA(IDS_STRING558);
		_stprintf_s(m_szLogMsg, sLangChange);	//"Epoxy�� Z�� ���Ͱ� ���� ���Դϴ�."
		errMsg2(Task.AutoFlag, m_szLogMsg);
		return false;
	}

	axis[0] = Motor_PCB_Z;
	pos[0] = model.axis[Motor_PCB_Z].pos[Dispense_Pos] + model.offset.f_epoxy_z_dispense_offset[iPcbNo];

	//startMoveAxis(1, axis, pos);
	goMotorPos(1, axis, pos);

	int step=100;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	eTime = myTimer(true);
	while (waitFlag)
	{
		switch(step)
		{
		case 100:		
			if(IsStopAxis(Motor_PCB_Z))
				step = 1000;
			break;

		default:
			break;
		}

		if(step==1000)
			break;


		if((myTimer(true) - eTime) > MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_Z);
			sLangChange.LoadStringA(IDS_STRING557);
			_stprintf_s(m_szLogMsg, sLangChange);	//"Epoxy�� Z�� ���� ��ġ �̵� ����"
			errMsg2(Task.AutoFlag,m_szLogMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}
	return true;
}

bool CAxlMotorSet::movePointDischargePos_Z(bool waitFlag)
{
	double eTime;
	short axis[1] = {Motor_PCB_Z};
	double pos[1];

	if( checkOriginStatus(1, axis) == false )
	{
		sLangChange.LoadStringA(IDS_STRING555);
		_stprintf_s(m_szLogMsg, sLangChange);	//"Epoxy�� Z �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�."
		errMsg2(Task.AutoFlag, m_szLogMsg);
		return false;
	}

	if ( (IsStopAxis(Motor_PCB_Z) == false) )
	{
		sLangChange.LoadStringA(IDS_STRING558);
		_stprintf_s(m_szLogMsg, sLangChange);	//"Epoxy�� Z�� ���Ͱ� ���� ���Դϴ�."
		errMsg2(Task.AutoFlag, m_szLogMsg);
		return false;
	}

	pos[0] = model.axis[Motor_PCB_Z].pos[Calc_Pos];

	//startMoveAxis(1, axis, pos);
	goMotorPos(1, axis, pos);

	int step=100;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	eTime = myTimer(true);
	while (waitFlag)
	{
		switch(step)
		{
		case 100:		
			if(IsStopAxis(Motor_PCB_Z))
				step = 1000;
			break;
		default:
			break;
		}

		if(step==1000)
			break;


		if(myTimer(true) - eTime>MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_Y);
			sLangChange.LoadStringA(IDS_STRING553);
			_stprintf_s(m_szLogMsg, sLangChange);		//"Epoxy�� Y �� Point ���� ��ġ �̵� ����"
			errMsg2(Task.AutoFlag,m_szLogMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}

	return true;
}

//���� calc Z

bool CAxlMotorSet::movePointDischarge2Pos_Z(bool waitFlag)
{
	double eTime;
	short axis[1] = {Motor_PCB_Z};
	double pos[1];

	if( checkOriginStatus(1, axis) == false )
	{
		sLangChange.LoadStringA(IDS_STRING555);
		_stprintf_s(m_szLogMsg, sLangChange);	//"Epoxy�� Z �� ���Ͱ� ���� ���͸� ���� �ʾҽ��ϴ�."
		errMsg2(Task.AutoFlag, m_szLogMsg);
		return false;
	}

	if ( (IsStopAxis(Motor_PCB_Z) == false) )
	{
		sLangChange.LoadStringA(IDS_STRING558);
		_stprintf_s(m_szLogMsg, sLangChange);	//"Epoxy�� Z�� ���Ͱ� ���� ���Դϴ�."
		errMsg2(Task.AutoFlag, m_szLogMsg);
		return false;
	}

	pos[0] = model.axis[Motor_PCB_Z].pos[Calc2_Pos];

	//startMoveAxis(1, axis, pos);
	goMotorPos(1, axis, pos);

	int step=100;

#ifndef	ON_LINE_MOTOR
	step = 1000;
#endif

	eTime = myTimer(true);
	while (waitFlag)
	{
		switch(step)
		{
		case 100:		
			if(IsStopAxis(Motor_PCB_Z))
				step = 1000;
			break;
		default:
			break;
		}

		if(step==1000)
			break;


		if(myTimer(true) - eTime>MOTOR_MOVE_TIME)
		{
			StopAxis(Motor_PCB_Y);
			
			sLangChange.LoadStringA(IDS_STRING553);
			_stprintf_s(m_szLogMsg, sLangChange);		//"Epoxy�� Y �� Point ���� ��ġ �̵� ����"
			errMsg2(Task.AutoFlag,m_szLogMsg);

			return false;
		}

		Sleep(5);
		checkMessage();
	}

	return true;
}
bool CAxlMotorSet::func_Epoxy_Left_StraightDraw()
{//���� ���� 
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	int step=0;
//	double startTime;
//	double MoveStartTime;
	//double eTime, curTime;
	int iEpoxyDelay = model.iEpoxyDelay;
	CString sMsg;

	while(1)
	{
		switch(step)
		{
		case 0:
			if( Dio.EpoxyDischargeOn2(true))
			{
				Sleep(500);
				step = 40;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING188);
				sMsg.Format(sLangChange);	//"[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� ON ��ȣ"
				errMsg2(Task.AutoFlag, sMsg);
				return false;
			}
			break;

		case 40:		
			if(Straight_Dispense_MoveAxis(Motor_PCB_Y, REL, model.dEpoxySize, model.dEpoxySpeed, model.dAcc))
			{
				double ep = myTimer(true);

				while (1)
				{
					if ( myTimer(true)-ep < 5000)
					{
						if ( motor.IsStopAxis(Motor_PCB_Y) && motor.GetInposition(Motor_PCB_Y) )
						{
							sLangChange.LoadStringA(IDS_STRING192);
							sMsg.Format(sLangChange);	//"[Dispense����] Epoxy Y�� �̵� �Ϸ�"
							pFrame->putListLog(sMsg);
							step = 50;
							break;
						}
					}
					else
					{
						sLangChange.LoadStringA(IDS_STRING191);
						sMsg.Format(sLangChange);		//"[Dispense����] Epoxy Y�� �̵� ����"
						errMsg2(Task.AutoFlag, sMsg);
						return false;
					}
				}
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING191);
				sMsg.Format(sLangChange);		//"[Dispense����] Epoxy Y�� �̵� ����"
				errMsg2(Task.AutoFlag, sMsg);
				return false;
			}
			break;

		/*case 45:		
			if(motor.Pcb_Motor_Move(Holder_Uv_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]))
			{
				sMsg.Format("[Dispense����] Epoxy Y�� ������ġ �̵� �Ϸ�");
				pFrame->putListLog(sMsg);
				step = 50;
			}
			else
			{
				sMsg.Format("[Dispense����] Epoxy Y�� ������ġ �̵� ����");
				return false;
			}
			break;*/
		case 50:
			if(StopAxis(Motor_PCB_Y))
			{
				Dio.EpoxyDischargeOn2(false, false);
				sLangChange.LoadStringA(IDS_STRING193);
				sMsg.Format(sLangChange);	//"[Dispense����] Epoxy ���� Off �Ϸ�"
				pFrame->putListLog(sMsg);
				Sleep(iEpoxyDelay);
			}
			else
			{
				Dio.EpoxyDischargeOn2(false, false);
			}
			step = 1000;
			break;
		
		default:
			step = 1000;
			break;
		}

		if(step==1000)		break;
	}

	return true;
}

bool CAxlMotorSet::func_Epoxy_Right_StraightDraw()
{//���� ���� 
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	int step=0;
//	double startTime;
//	double MoveStartTime;
//	double eTime, curTime;
	int iEpoxyDelay = model.iEpoxyDelay;
	CString sMsg;

	while(1)
	{
		switch(step)
		{
		case 0:
			if( Dio.EpoxyDischargeOn2(true))
			{
				Sleep(500);
				step = 40;
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING188);
				sMsg.Format(sLangChange);	//"[Dispence����] Epoxy ���� ��ȣ �̻� �߻�_���� ON ��ȣ"
				errMsg2(Task.AutoFlag, sMsg);
				return false;
			}
			break;

		case 40:		
			if(Straight_Dispense_MoveAxis(Motor_PCB_Y, REL, model.dEpoxySize, model.dEpoxySpeed, model.dAcc))
			{
				double ep = myTimer(true);

				while (1)
				{
					if ( myTimer(true)-ep < 5000)
					{
						if ( motor.IsStopAxis(Motor_PCB_Y) && motor.GetInposition(Motor_PCB_Y) )
						{
							sLangChange.LoadStringA(IDS_STRING192);
							sMsg.Format(sLangChange);	//"[Dispense����] Epoxy Y�� �̵� �Ϸ�"
							pFrame->putListLog(sMsg);
							step = 50;
							break;
						}
					}
					else
					{
						sLangChange.LoadStringA(IDS_STRING191);
						sMsg.Format(sLangChange);		//"[Dispense����] Epoxy Y�� �̵� ����"
						errMsg2(Task.AutoFlag, sMsg);
						return false;
					}
				}
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING191);
				sMsg.Format(sLangChange);		//"[Dispense����] Epoxy Y�� �̵� ����"
				errMsg2(Task.AutoFlag, sMsg);
				return false;
			}
			break;

		/*case 45:		
			if(motor.Pcb_Motor_Move(Holder_Uv_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]))
			{
				sMsg.Format("[Dispense����] Epoxy Y�� ������ġ �̵� �Ϸ�");
				pFrame->putListLog(sMsg);
				step = 50;
			}
			else
			{
				sMsg.Format("[Dispense����] Epoxy Y�� ������ġ �̵� ����");
				return false;
			}
			break;*/

		case 50:
			if(StopAxis(Motor_PCB_Y))
			{
				Dio.EpoxyDischargeOn2(false, false);

				sLangChange.LoadStringA(IDS_STRING193);
				sMsg.Format(sLangChange);	//"[Dispense����] Epoxy ���� Off �Ϸ�"
				pFrame->putListLog(sMsg);
				Sleep(iEpoxyDelay);
			}
			else
			{
				Dio.EpoxyDischargeOn2(false, false);
			}
			step = 1000;
			break;
		
		default:
			step = 1000;
			break;
		}

		if(step==1000)		break;
	}

	return true;
}

bool CAxlMotorSet::Trinityt_Dispense_MoveAxis()
{	// abs 1:absolute, 0:increment\



	//if (AbSFlag == ABS)
	//{
	//	double curPos;
	//	if (axis == Motor_Lens_Xt || axis == Motor_Lens_Yt || axis == Motor_PCB_Xt || axis == Motor_PCB_Yt)
	//		curPos = GetCommandPos(axis);	//GetCommandPos(axis);
	//	else
	//		curPos = GetEncoderPos(axis);	//GetCommandPos(axis);
	//	if (fabs(curPos - dPos) < 0.0001)
	//	{
	//		return true;
	//	}
	//	if (m_bOrgFlag[axis] == false)
	//	{
	//		TCHAR	motorMSG[256];
	//		sTempLang.LoadStringA(IDS_STRING367);
	//		sLangChange.Format(sTempLang, axis + 1);
	//		wsprintf(motorMSG, sLangChange);
	//		return false;
	//	}
	//}
	//else if (AbSFlag == REL)
	//{
	//	if (axis == Motor_Lens_Xt || axis == Motor_Lens_Yt || axis == Motor_PCB_Xt || axis == Motor_PCB_Yt)
	//		dPos += GetCommandPos(axis);
	//	else
	//		dPos += GetEncoderPos(axis);	//GetCommandPos(axis);
	//}
	//dPos *= sysData.fMotorResol[axis];
	//if (dPos>0)					dPos = (int)(dPos + 0.5);

	long   axisAll[MAX_MOTOR_NO] = { 0, };
	double posAll[MAX_MOTOR_NO] = { 0, };
	double velAll[MAX_MOTOR_NO] = { 0, };
	double accAll[MAX_MOTOR_NO] = { 0, };
	double decAll[MAX_MOTOR_NO] = { 0, };


	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.

	//210716
	//
	//
	long MOTOR_DIR = 0;
	double dAngle = 0.0;
	double CircleSpeed = 0.0;
	double TrinityCircleRadius = 0.0;
	double dPos[2];
	double dMidPos[2];
	double dEndPos[2];
	dMidPos[0] = dMidPos[1] = 0.0;
	dEndPos[0] = dEndPos[1] = 0.0;
	dPos[0] = dPos[1] = 0.0;
	int cnt = 2;
	long    lAxis[2];
	lAxis[0] = Motor_PCB_X; //���ȣ
	lAxis[1] = Motor_PCB_Y;

	long    lPosSize = 2;
	long  lCoordinate = 0;

	AxmContiWriteClear(lCoordinate);
	AxmContiSetAxisMap(lCoordinate, lPosSize, lAxis);

	// �����ġ �������.
	AxmContiSetAbsRelMode(lCoordinate, 1);
	AxmContiBeginNode(lCoordinate);
	//
	axisAll[0] = Motor_PCB_X;
	axisAll[1] = Motor_PCB_Y;
	velAll[0] = model.dEpoxySpeed * sysData.fMotorResol[axisAll[0]];
	velAll[1] = model.dEpoxySpeed * sysData.fMotorResol[axisAll[1]];
	accAll[0] = accAll[1] = model.dAcc;
	decAll[0] = decAll[1] = model.dAcc;

	//===================================================================================================================================
	//���� 1 
	//�ϴ� ����
	posAll[0] = posAll[1] = 0.0;
	posAll[0] = model.dDisLength[0] * -1;
	posAll[0] *= sysData.fMotorResol[Motor_PCB_X];
	//
	if (posAll[0]>0) posAll[0] = (int)(posAll[0] + 0.5);
	posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];
	
	
	accAll[0] = accAll[1] = model.dAcc;   
	decAll[0] = decAll[1] = model.dAcc;
	velAll[0] = model.dEpoxySpeed * sysData.fMotorResol[axisAll[0]];
	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]); 
	//===================================================================================================================================
	//���� 2
	//���� �ϴ� Ŀ��
	CircleSpeed = model.dConti_Speed * sysData.fMotorResol[axisAll[0]];
	dMidPos[0] = model.dCircleRadiusCenterOffsetX[0]*10000; 
	dMidPos[1] = model.dCircleRadiusCenterOffsetY[0] * 10000;
	dEndPos[0] = model.dCircleRadiusEndOffsetX[0] * 10000;
	dEndPos[1] = model.dCircleRadiusEndOffsetY[0] * 10000;   
	MOTOR_DIR = DIR_CCW;
	AxmCircleCenterMove(lCoordinate, axisAll, dMidPos, dEndPos, CircleSpeed, accAll[0], decAll[0], MOTOR_DIR);
	//===================================================================================================================================
	//���� 3
	//���� ����
	axisAll[0] = Motor_PCB_X;
	axisAll[1] = Motor_PCB_Y; 
	posAll[0] = posAll[1] = 0.0;
	posAll[0] *= sysData.fMotorResol[Motor_PCB_X];

	posAll[1] = model.dDisLength[1] * -1; 
	posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];
	if (posAll[1]>0)					posAll[1] = (int)(posAll[1] + 0.5);

	velAll[0] = model.dEpoxySpeed * sysData.fMotorResol[axisAll[0]];
	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 4
	//������� Ŀ�� �� ����
	posAll[0] = posAll[1] = 0.0;
	posAll[0] = model.dDisLength[4] * 1;
	posAll[0] *= sysData.fMotorResol[Motor_PCB_X];
	//
	if (posAll[0]>0) posAll[0] = (int)(posAll[0] + 0.5);
	posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];


	accAll[0] = accAll[1] = model.dAcc;
	decAll[0] = decAll[1] = model.dAcc;

	velAll[0] = model.dConti_Speed * sysData.fMotorResol[axisAll[0]];

	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 5
	//������� Ŀ��
	CircleSpeed = model.dConti_Speed * sysData.fMotorResol[axisAll[0]];
	dMidPos[0] = model.dCircleRadiusCenterOffsetX[1] * 10000;
	dMidPos[1] = model.dCircleRadiusCenterOffsetY[1] * 10000; 
	dEndPos[0] = model.dCircleRadiusEndOffsetX[1] * 10000;
	dEndPos[1] = model.dCircleRadiusEndOffsetY[1] * 10000;
	MOTOR_DIR = DIR_CW;
	AxmCircleCenterMove(lCoordinate, axisAll, dMidPos, dEndPos, CircleSpeed, accAll[0], decAll[0], MOTOR_DIR);
	//===================================================================================================================================
	//���� 6
	//������� Ŀ�� �� ����
	axisAll[0] = Motor_PCB_X;
	axisAll[1] = Motor_PCB_Y;
	posAll[0] = posAll[1] = 0.0;
	posAll[0] *= sysData.fMotorResol[Motor_PCB_X];

	posAll[1] = model.dDisLength[5] * -1;
	posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];
	if (posAll[1]>0)					posAll[1] = (int)(posAll[1] + 0.5);

	velAll[0] = model.dConti_Speed * sysData.fMotorResol[axisAll[0]];
	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);

	//===================================================================================================================================
	//���� 7
	//��� ����
	posAll[0] = posAll[1] = 0.0;

	posAll[0] = model.dDisLength[2] * 1;
	posAll[0] *= sysData.fMotorResol[Motor_PCB_X];
	//
	if (posAll[0]>0)					posAll[0] = (int)(posAll[0] + 0.5);
	posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];

	velAll[0] = model.dEpoxySpeed * sysData.fMotorResol[axisAll[0]];
	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 8
	//���� ��� Ŀ��
	CircleSpeed = model.dConti_Speed * sysData.fMotorResol[axisAll[0]];
	dMidPos[0] = model.dCircleRadiusCenterOffsetX[2] * 10000;
	dMidPos[1] = model.dCircleRadiusCenterOffsetY[2] * 10000;
	dEndPos[0] = model.dCircleRadiusEndOffsetX[2] * 10000;
	dEndPos[1] = model.dCircleRadiusEndOffsetY[2] * 10000;
	MOTOR_DIR = DIR_CCW;
	AxmCircleCenterMove(lCoordinate, axisAll, dMidPos, dEndPos, CircleSpeed, accAll[0], decAll[0], MOTOR_DIR);
	//===================================================================================================================================
	//���� 9
	//���� ����
	posAll[0] = posAll[1] = 0.0;
	posAll[0] *= sysData.fMotorResol[Motor_PCB_X];

	posAll[1] = model.dDisLength[3] * 1;
	posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];
	if (posAll[1]>0)					posAll[1] = (int)(posAll[1] + 0.5);

	velAll[0] = model.dEpoxySpeed * sysData.fMotorResol[axisAll[0]];
	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 10
	//������� Ŀ�� �� ����
	posAll[0] = posAll[1] = 0.0;

	posAll[0] = model.dDisLength[6] * -1;
	posAll[0] *= sysData.fMotorResol[Motor_PCB_X];
	//
	if (posAll[0]>0)					posAll[0] = (int)(posAll[0] + 0.5);
	posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];

	velAll[0] = model.dConti_Speed * sysData.fMotorResol[axisAll[0]];
	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 11
	//���� �ϴ� Ŀ��
	CircleSpeed = model.dConti_Speed * sysData.fMotorResol[axisAll[0]];
	dMidPos[0] = model.dCircleRadiusCenterOffsetX[3] * 10000;
	dMidPos[1] = model.dCircleRadiusCenterOffsetY[3] * 10000;
	dEndPos[0] = model.dCircleRadiusEndOffsetX[3] * 10000;
	dEndPos[1] = model.dCircleRadiusEndOffsetY[3] * 10000;
	MOTOR_DIR = DIR_CW;
	AxmCircleCenterMove(lCoordinate, axisAll, dMidPos, dEndPos, CircleSpeed, accAll[0], decAll[0], MOTOR_DIR);
	//===================================================================================================================================
	//���� 12
	//������� Ŀ�� �� ����
	posAll[0] = posAll[1] = 0.0;
	posAll[0] *= sysData.fMotorResol[Motor_PCB_X];

	posAll[1] = model.dDisLength[7] * 1;
	posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];
	if (posAll[1]>0)					posAll[1] = (int)(posAll[1] + 0.5);

	velAll[0] = model.dConti_Speed * sysData.fMotorResol[axisAll[0]];
	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]); 

	 
	//===================================================================================================================================
	//
	AxmContiEndNode(lCoordinate);
	// �����ġ ��������.
	AxmContiSetAbsRelMode(lCoordinate, 1);
	// ���Ӻ������� ����.
	AxmContiStart(lCoordinate, 0, 0);

	
	Sleep(10);
	return true;
}
bool CAxlMotorSet::Continue_Line_Arck_Dispense()
{
	long   axisAll[MAX_MOTOR_NO] = { 0, };
	double posAll[MAX_MOTOR_NO] = { 0, };
	double velAll[MAX_MOTOR_NO] = { 0, };
	double accAll[MAX_MOTOR_NO] = { 0, };
	double decAll[MAX_MOTOR_NO] = { 0, };


	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.

	//210716
	//
	//
	long MOTOR_DIR = 0;
	double dAngle = 0.0;
	double CircleSpeed = 0.0;
	double TrinityCircleRadius = 0.0;
	double dPos[2];
	double dMidPos[2];
	double dEndPos[2];
	dMidPos[0] = dMidPos[1] = 0.0;
	dEndPos[0] = dEndPos[1] = 0.0;
	//dPos[0] = dPos[1] = 0.0;
	int cnt = 2;
	long    lAxis[2];
	lAxis[0] = Motor_PCB_X; //���ȣ
	lAxis[1] = Motor_PCB_Y;

	long    lPosSize = 2;
	long  lCoordinate = 0;

	AxmContiWriteClear(lCoordinate);
	AxmContiSetAxisMap(lCoordinate, lPosSize, lAxis);

	// �����ġ �������.
	AxmContiSetAbsRelMode(lCoordinate, 1);
	AxmContiBeginNode(lCoordinate);
	//
	axisAll[0] = Motor_PCB_X;
	axisAll[1] = Motor_PCB_Y;
	velAll[0] = model.dEpoxySpeed * sysData.fMotorResol[axisAll[0]];
	velAll[1] = model.dEpoxySpeed * sysData.fMotorResol[axisAll[1]];
	accAll[0] = accAll[1] = model.dAcc;
	decAll[0] = decAll[1] = model.dAcc;

	//===================================================================================================================================
	//���� 1 
	//�ϴ� ����
	posAll[0] = posAll[1] = 0.0;
	posAll[0] = model.dDisLength[4] * -1;
	posAll[0] *= sysData.fMotorResol[Motor_PCB_X];
	//
	if (posAll[0]>0) posAll[0] = (int)(posAll[0] + 0.5);
	posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];


	accAll[0] = accAll[1] = model.dAcc;
	decAll[0] = decAll[1] = model.dAcc;
	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 2
	//���� �ϴ� Ŀ��
	CircleSpeed = model.dCircleSpeed * sysData.fMotorResol[axisAll[0]]; 
	dMidPos[0] = model.dCircleRadiusCenterOffsetX[0] * 10000;
	dMidPos[1] = model.dCircleRadiusCenterOffsetY[0] * 10000;
	dEndPos[0] = model.dCircleRadiusEndOffsetX[0] * 10000;
	dEndPos[1] = model.dCircleRadiusEndOffsetY[0] * 10000;
	MOTOR_DIR = DIR_CCW;

	AxmCircleCenterMove(lCoordinate, axisAll, dMidPos, dEndPos, CircleSpeed, accAll[0], decAll[0], MOTOR_DIR);
	//===================================================================================================================================
	//���� 3
	//���� ����
	axisAll[0] = Motor_PCB_X;
	axisAll[1] = Motor_PCB_Y;
	posAll[0] = posAll[1] = 0.0;
	posAll[0] *= sysData.fMotorResol[Motor_PCB_X];

	posAll[1] = model.dDisLength[5] * -1;
	posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];
	if (posAll[1]>0)					posAll[1] = (int)(posAll[1] + 0.5);

	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 4
	//������� Ŀ�� �� ����
	//posAll[0] = posAll[1] = 0.0;
	//posAll[0] = model.dDisLength[4] * 1;
	//posAll[0] *= sysData.fMotorResol[Motor_PCB_X];
	////
	//if (posAll[0]>0) posAll[0] = (int)(posAll[0] + 0.5);
	//posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];


	//accAll[0] = accAll[1] = model.dAcc;
	//decAll[0] = decAll[1] = model.dAcc;
	//AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 5
	//������� Ŀ��
	CircleSpeed = model.dCircleSpeed * sysData.fMotorResol[axisAll[0]];
	dMidPos[0] = model.dCircleRadiusCenterOffsetX[1] * 10000;
	dMidPos[1] = model.dCircleRadiusCenterOffsetY[1] * 10000;
	dEndPos[0] = model.dCircleRadiusEndOffsetX[1] * 10000;
	dEndPos[1] = model.dCircleRadiusEndOffsetY[1] * 10000; 
	MOTOR_DIR = DIR_CCW;
	AxmCircleCenterMove(lCoordinate, axisAll, dMidPos, dEndPos, CircleSpeed, accAll[0], decAll[0], MOTOR_DIR);
	//===================================================================================================================================
	//���� 6
	//������� Ŀ�� �� ����
	/*axisAll[0] = Motor_PCB_X;
	axisAll[1] = Motor_PCB_Y; 
	posAll[0] = posAll[1] = 0.0;
	posAll[0] *= sysData.fMotorResol[Motor_PCB_X];

	posAll[1] = model.dDisLength[5] * -1;
	posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];
	if (posAll[1]>0)					posAll[1] = (int)(posAll[1] + 0.5);

	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);*/

	//===================================================================================================================================
	//���� 7
	//��� ����
	posAll[0] = posAll[1] = 0.0;

	posAll[0] = model.dDisLength[6] * 1;
	posAll[0] *= sysData.fMotorResol[Motor_PCB_X];
	//
	if (posAll[0]>0)					posAll[0] = (int)(posAll[0] + 0.5);
	posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];

	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 8
	//���� ��� Ŀ��
	CircleSpeed = model.dCircleSpeed * sysData.fMotorResol[axisAll[0]];
	dMidPos[0] = model.dCircleRadiusCenterOffsetX[2] * 10000;
	dMidPos[1] = model.dCircleRadiusCenterOffsetY[2] * 10000;
	dEndPos[0] = model.dCircleRadiusEndOffsetX[2] * 10000;
	dEndPos[1] = model.dCircleRadiusEndOffsetY[2] * 10000;
	MOTOR_DIR = DIR_CCW;
	AxmCircleCenterMove(lCoordinate, axisAll, dMidPos, dEndPos, CircleSpeed, accAll[0], decAll[0], MOTOR_DIR);
	//===================================================================================================================================
	//���� 9
	//���� ����
	posAll[0] = posAll[1] = 0.0;
	posAll[0] *= sysData.fMotorResol[Motor_PCB_X];

	posAll[1] = model.dDisLength[7] * 1;
	posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];
	if (posAll[1]>0)					posAll[1] = (int)(posAll[1] + 0.5);


	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 10
	//������� Ŀ�� �� ����
	//posAll[0] = posAll[1] = 0.0;

	//posAll[0] = model.dDisLength[6] * -1;
	//posAll[0] *= sysData.fMotorResol[Motor_PCB_X];
	////
	//if (posAll[0]>0)					posAll[0] = (int)(posAll[0] + 0.5);
	//posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];

	//AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);
	//===================================================================================================================================
	//���� 11
	//���� �ϴ� Ŀ��
	CircleSpeed = model.dCircleSpeed * sysData.fMotorResol[axisAll[0]];
	dMidPos[0] = model.dCircleRadiusCenterOffsetX[3] * 10000;
	dMidPos[1] = model.dCircleRadiusCenterOffsetY[3] * 10000;
	dEndPos[0] = model.dCircleRadiusEndOffsetX[3] * 10000;
	dEndPos[1] = model.dCircleRadiusEndOffsetY[3] * 10000;
	MOTOR_DIR = DIR_CCW;
	AxmCircleCenterMove(lCoordinate, axisAll, dMidPos, dEndPos, CircleSpeed, accAll[0], decAll[0], MOTOR_DIR);
	//===================================================================================================================================
	//���� 12
	//������� Ŀ�� �� ����
	//���� ����ߵż� �ּ�ó���� 
	/*posAll[0] = posAll[1] = 0.0;
	posAll[0] *= sysData.fMotorResol[Motor_PCB_X];

	posAll[1] = model.dDisLength[7] * 1;
	posAll[1] *= sysData.fMotorResol[Motor_PCB_Y];
	if (posAll[1]>0)					posAll[1] = (int)(posAll[1] + 0.5);


	AxmLineMove(lCoordinate, posAll, velAll[0], accAll[0], decAll[0]);*/ 


	//===================================================================================================================================
	//
	AxmContiEndNode(lCoordinate);
	// �����ġ ��������.
	AxmContiSetAbsRelMode(lCoordinate, 1);
	// ���Ӻ������� ����.
	AxmContiStart(lCoordinate, 0, 0);

	 
	Sleep(10);
	return true;
}

bool CAxlMotorSet::Straight_Dispense_MoveAxis(int axis, bool AbSFlag, double dPos, double dVel, float iAcc)
{	// abs 1:absolute, 0:increment\

	if(GetAmpFault(axis))
	{
		m_bOrgFlag[axis] = false;
		sLangChange.LoadStringA(IDS_STRING1453);
		LogSave(sLangChange);		//"AMP Alarm �߻�. MoveAxis ���� ����."
		return false;
	}

	if(!GetAmpEnable(axis))
	{
		m_bOrgFlag[axis] = false;
	
		CString tmpStr;
		sLangChange.LoadStringA(IDS_STRING1226);
		tmpStr.Format(_T("[%s] ") + sLangChange, MotorName[axis]);
		errMsg2(Task.AutoFlag, tmpStr);
		return false;
	}

	if(AbSFlag == ABS)
	{
		double curPos;

		if (axis == Motor_Lens_Xt || axis == Motor_Lens_Yt || axis == Motor_PCB_Xt || axis == Motor_PCB_Yt)
			curPos = GetCommandPos(axis);	//GetCommandPos(axis);
		else
			curPos = GetEncoderPos(axis);	//GetCommandPos(axis);

		if(fabs(curPos - dPos) < 0.0001)	
		{
			return true;
		}

		if(m_bOrgFlag[axis]==false)
		{
			TCHAR	motorMSG[256];
			sTempLang.LoadStringA(IDS_STRING367);
			sLangChange.Format(sTempLang,axis+1);
			wsprintf(motorMSG, sLangChange);
			return false;
		}
	}
	else if(AbSFlag==REL)
	{
		if (axis == Motor_Lens_Xt || axis == Motor_Lens_Yt || axis == Motor_PCB_Xt || axis == Motor_PCB_Yt)
			dPos += GetCommandPos(axis);
		else
			dPos += GetEncoderPos(axis);	//GetCommandPos(axis);
	}

	dPos *= sysData.fMotorResol[axis];
	if(dPos>0)					dPos = (int)(dPos+0.5);

	long   axisAll[MAX_MOTOR_NO] = {0, };
	double posAll[MAX_MOTOR_NO] = {0, };
	double velAll[MAX_MOTOR_NO] = {0, };
	double accAll[MAX_MOTOR_NO] = {0, };
	double decAll[MAX_MOTOR_NO] = {0, };

	axisAll[0] = axis;
	posAll[0] = dPos;
	velAll[0] = dVel * sysData.fMotorResol[axis];
	accAll[0] = model.dAcc; //! ���� ���ӵ�
	decAll[0] = model.dAcc;

	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�. 
	//! �� �Լ��� ����ϸ� ���� ���� ���� ���ÿ� �۾��� �����Ѵ�. 
	//! �� �Լ��� ���� ���� ���⸦ ���߾� �۾��� �����ؾ� �ϴ� ��쿡 ����Ѵ�. 
	//! �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
	//AxmMoveStartMultiPos(1, axisAll, posAll, velAll, accAll, decAll);
	AxmMoveStartPos(axis, dPos, velAll[0], accAll[0], decAll[0]); 
	Sleep(10);
	return true;
}