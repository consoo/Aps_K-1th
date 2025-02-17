#include "stdafx.h"

#include "MIU.h"

#include "../AA Bonder.h"
#include "../AA BonderDlg.h"
#include "../RawToBmp.h"
#include "ConvertColor.h"
#include <winbase.h>

#define	BUFFER_COUNT	10
#define PAYLOAD_SIZE	1020
#define	BGGR				1	//BGBG
#define RGGB				2	//RGRG
#define GBRG				3	//GBGB
#define GRBG				4	//GRGR
#define RCCC				5	//RCCC
#define CCCR				6	
#define CRCC				7
#define	CCRC				8


CMeasureTimeElapsed::CMeasureTimeElapsed()
{
	Init_Time();
}

CMeasureTimeElapsed::~CMeasureTimeElapsed()
{
}

void CMeasureTimeElapsed::Init_Time()
{
	int i = 0;
	for ( i = 0; i < iBufSize_Measure_Time_Elapsed; i++ )
	{
		m_adTime[i] = 0.;
	}
	m_bFlag_Valid_Freq = false;
}

bool CMeasureTimeElapsed::Start_Time()
{
	if ( QueryPerformanceFrequency(&m_liFreq) == TRUE )
	{
		m_bFlag_Valid_Freq = true;
	}
	else
	{
		m_bFlag_Valid_Freq = false;		
	}

    if ( QueryPerformanceCounter(&m_liStartCounter) == FALSE )
	{
		m_bFlag_Valid_Freq = false;
	}

	return m_bFlag_Valid_Freq;
}

double CMeasureTimeElapsed::Measure_Time(int iIndex_Time)
{
	double dRet = -1.;

	if ( m_bFlag_Valid_Freq == false )
	{
		return dRet;
	}
	if ( iIndex_Time < 0 || iIndex_Time >= iBufSize_Measure_Time_Elapsed )
	{
		return dRet;
	}

	if ( QueryPerformanceCounter( &(m_aliCounter[iIndex_Time]) ) == TRUE )
	{
		dRet = ((double)(m_aliCounter[iIndex_Time].QuadPart - m_liStartCounter.QuadPart)) / ((double)m_liFreq.QuadPart);
		dRet *= 1000.;
	}
	
	m_adTime[iIndex_Time] = dRet;

	return dRet;
}


CMIU::CMIU()
{
	

	
}

CMIU::~CMIU()
{
	cvReleaseImage(&pImageBuf[0]);
	cvReleaseImage(&pImageBuf[1]);
	cvReleaseImage(&pImageBuf[2]);
	//delete[] gMIUDevice.pBuffer;

	cvBufferFree();	//~CMIU()
	SensorList_Free();

	free(pImageTemp2);

	//---------------------------------------------20180503 �߰�
	

	if (m_hBoardLibrary)
	{
		pfnReleaseBoardControl fnReleaseBoard = (pfnReleaseBoardControl)GetProcAddress(m_hBoardLibrary, "ReleaseBoardControl");
		if( fnReleaseBoard )
			fnReleaseBoard(m_pBoard);

		FreeLibrary(m_hBoardLibrary);
		m_hBoardLibrary = NULL;
		m_pBoard = NULL;
	}
	
	if (vChartBuffet)
		delete vChartBuffet;
	vChartBuffet = NULL;

	if (vChart_Second_Buffet)
		delete vChart_Second_Buffet;
	vChart_Second_Buffet = NULL;
	
	if (m_pFrameRawBuffer)
		delete m_pFrameRawBuffer;
	m_pFrameRawBuffer = NULL;

	if (m_pFrameBMPBuffer)
		delete m_pFrameBMPBuffer;
	m_pFrameBMPBuffer = NULL;
}
void CMIU::ImageBufferSet()
{
	
	INI_LOAD();
	//
	//
	//
	int iRet = SelectSensor();
	


	//! CCD Camera Sensor ���� ��� ���� ���� ����
	cvBufferAlloc();		//ImageBufferSet

	
}
void CMIU::setInterface()
{
	m_pBoard = NULL;
	m_hBoardLibrary = NULL;
	m_pFrameRawBuffer = NULL;
	m_pFrameBMPBuffer = NULL;
	vChartBuffet = NULL;
	vChart_Second_Buffet = NULL;
	m_strIniFileName = "";
	m_cBoardIndex = 0;
	//--
	m_bFlag_Color_Covert_Ext = false;
	m_bFlag_Mil_Convert = true;
	m_bFlag_Averaging = false;

	gDeviceIndex = 0;
	gMIUDevice.iImageSave = 0;
	int i = 0;

	::GetLocalTime(&(gMIUDevice.TimeGrab));
	memset(&gMIUDevice, 0, sizeof(MIU_DEVICE));


	for (i = 0; i < iBufSize_CCM_Temp; i++)
	{
		memset(&(m_aTemp[i]), 0, sizeof(MIU_DEVICE));
		::GetLocalTime(&(m_aTemp[i].TimeGrab));
	}


	//������� �ڡڡڡڡڡ�
	//===================================================================================================================
	m_hBoardLibrary = LoadLibrary("GrabberDLL.dll");
	if (m_hBoardLibrary == NULL)
	{
		AfxMessageBox("[ERROR] Cannot Load Board Library");
	}

	pfnGetBoardControl fnGetInterface = (pfnGetBoardControl)GetProcAddress(m_hBoardLibrary, "GetBoardControl");
	if (fnGetInterface)
	{
		m_pBoard = (*fnGetInterface)(BOARD_TYPE_LAON);
		if (m_pBoard == NULL)
		{
			AfxMessageBox("[ERROR] GetBoardControl Load Failed!");
		}
	}
	else
	{
		AfxMessageBox("[ERROR] Cannot Get Board Interface");
	}
	m_iDeviceCount = 2;
	memset(m_acIndexDevice, 0, 4);

	m_iIndexDevice = 1;

	m_pSID = NULL;
	m_iSensorListCnt = 0;

	for (i = 0; i < 3; i++)
	{
		m_acvChildImage[i] = NULL;
		pImageBuf[i] = NULL;
	}

	Init_Grab_Time();

	Init_Index_Grab();

	m_iSensorType = 1;

	m_cInputCurrent = 150;

	m_ubBurst = FALSE;

	sLogMessage = _T("");

	gMIUDevice.bMIUOpen = 0;
	gMIUDevice.bMIUInit = 0;
	gMIUDevice.bReadyVCM = 0;
	gMIUDevice.CurrentState = 0;

	gMIUDevice.nWidth = 0;
	gMIUDevice.nHeight = 0;

	gMIUDevice.fCcdSize = 1.75f;
	gMIUDevice.nDataFormat = 0;
	gMIUDevice.nOutMode = 0;
	gMIUDevice.nSensorType = 0;

	// by PRI	
	m_bFlagImgReady = 0;
	m_bFlagImgUsed = 0;
	Init_Display_Count();
	m_bFlagTestPattern_test = 0;
	/////////////////////////////////////////////////////////
	iImgUpdateFlag = 0;

	bFlag_RawImageSave = false;
	ImageBufferSet();

	
}
void CMIU::INI_LOAD()
{
	char szTemp[MAX_PATH];
	CString uiFile=_T("");
	uiFile.Format("%s\\%s\\Initialize\\UIConfig\\UIConfig.ini", AA_MODEL_LOAD_DIR, modelList.curModelName);			//uiFile.Empty();
	theApp.MainDlg->putListLog(uiFile);

	m_cBoardIndex = GetPrivateProfileInt("UIConfig", "BoardIndex", 0, uiFile);
	CString iniFolder = "";
	GetPrivateProfileString("UIConfig", "GrabberINI", "Default.ini", szTemp, MAX_PATH, uiFile);
	selectedIniFile.Format("%s\\%s\\Initialize\\%s", AA_MODEL_LOAD_DIR, modelList.curModelName, szTemp);
	m_pBoard->SetINIFile(selectedIniFile);



	
}
void CMIU::disConnectedInit()
{
	//theApp.MainDlg->m_bMiuRun = false; 
	gMIUDevice.CurrentState = 0;
	gMIUDevice.bMIUOpen = 0;
	gMIUDevice.bMIUInit = 0;
	gMIUDevice.bReadyVCM = 0;
	//Init_Index_Grab();
	//theApp.MainDlg->m_labelUsbLive.SetBkColor(M_COLOR_RED);
	//theApp.MainDlg->m_labelUsbLive.Invalidate();
}
void CMIU::Init_Index_Grab()
{
	//! CCM ���� Thread ���̸� ������ �ִ� index �ʱ�ȭ
	//! [���� ����] Start�� �ϱ� ������ ���� ����� ���� ���ϵ��� ���� (-1)�� �����Ѵ�. 
	m_iIndex_Grab_Working = -1;
	m_iIndex_Grab_Ready = -1;
	m_iIndex_Grab_Used = -1;

	m_iIndex_Cvt_Clr_Working = -1;		
	m_iIndex_Cvt_Clr_Used = -1;

	m_iIndex_Cvt_Mil_Working = -1;		
	m_iIndex_Cvt_Mil_Used = -1;

	m_iIndex_Display_Working = -1;
}

void CMIU::Start_Index_Grab()
{
	//! �ݵ��, ������ Play�� �ʱ�ȭ �Ѵ�. 
	int i = 0;
	/*for ( i = 0; i < iBufSize_CCM_Temp; i++ )
	{
		m_aTemp[i].CurrentState = 4;
	}*/

	//! CCM ���� Thread ���̸� ������ �ִ� index �ʱ�ȭ
	//! [���� ����] 
	//!	  ���� ��� ������ ������ 'm_iIndex_Grab_Working'�� '0'���� �ʱ�ȭ �ϴµ��� ���� �����Ѵ�. 
	//!   �ٸ� index���� ��� (-1)�� �ʱ�ȭ�Ǿ�� �Ѵ�.
	m_iIndex_Grab_Working = 0;
	m_iIndex_Grab_Ready = -1;
	m_iIndex_Grab_Used = -1;

	m_iIndex_Cvt_Clr_Working = -1;		
	m_iIndex_Cvt_Clr_Used = -1;

	m_iIndex_Cvt_Mil_Working = -1;		
	m_iIndex_Cvt_Mil_Used = -1;

	m_iIndex_Display_Working = -1;
}

//! Thread�� ��� �ð� �ʱ�ȭ
void CMIU::Init_Grab_Time()
{
	int i = 0;
	for ( i = 0; i < iBufSize_CCM_Time; i++ )
	{
		m_adTime_Grab[i] = 0.;
		m_aMeasureTime[i].Init_Time();
	}
}

//! Thread�� ��� �ð� �ʱ�ȭ
bool CMIU::Init_Grab_Time(int fi_iIndex_Time_Grab, bool fi_bFlag_Init_Temp)
{
	if ( fi_iIndex_Time_Grab < 0 || fi_iIndex_Time_Grab >= iBufSize_CCM_Time )
	{
		return false;
	}

	m_aMeasureTime[fi_iIndex_Time_Grab].Init_Time();

	if ( fi_bFlag_Init_Temp == true )
	{
		m_adTime_Grab[fi_iIndex_Time_Grab] = 0.;
	}

	return true;
}

//! Thread�� ��� �ð� ���� ����
bool CMIU::Start_Grab_Time(int fi_iIndex_Time_Grab)
{
	if ( fi_iIndex_Time_Grab < 0 || fi_iIndex_Time_Grab >= iBufSize_CCM_Time )
	{
		return false;
	}

	bool bRet = m_aMeasureTime[fi_iIndex_Time_Grab].Start_Time();

	return bRet;
}

//! Thread�� ��� �ð� ����, ���� msec
double CMIU::Measure_Grab_Time(int fi_iIndex_Time_Grab)
{
	double dRet = -1.;

	if ( fi_iIndex_Time_Grab < 0 || fi_iIndex_Time_Grab >= iBufSize_CCM_Time )
	{
		return dRet;
	}

	bool bRet = m_aMeasureTime[fi_iIndex_Time_Grab].IsSuccess_Start_Time();
	if ( bRet == false )
	{
		return dRet;
	}
	
	dRet = m_aMeasureTime[fi_iIndex_Time_Grab].Measure_Time(0);
	
	m_adTime_Grab[fi_iIndex_Time_Grab] = dRet;

	return dRet;
}

//! Sensor ����� �޸𸮸� ����
void CMIU::SensorList_Free()
{
	if ( m_iSensorListCnt > 0 && m_pSID != NULL )
	{
		int i = 0;

		for ( i = 0; i < m_iSensorListCnt; i++ )
		{
			if ( m_pSID[i].pInfo != NULL )
			{
				delete [] m_pSID[i].pInfo;
				m_pSID[i].pInfo = NULL;
			}
		}

		delete [] m_pSID;
		m_pSID = NULL;
	}
}

//! USB 3.0�� ����� Device Search
int CMIU::Search_Device()
{
	/*int iErrorCode = MIU_OK;
	char cDeiveCount = 0;
	memset(m_acIndexDevice, 0, 2);
	iErrorCode = MIUGetDeviceList(&cDeiveCount, m_acIndexDevice);
	if ( iErrorCode != MIU_OK )
	{
		m_iDeviceCount = 1;
		sLogMessage.Format(_T("MIU Device - MIUGetDeviceList Error (Code : 0x%x)"), iErrorCode);
		return 0;
	}

	m_iDeviceCount = cDeiveCount;*/

	return m_iDeviceCount;
}
//! LPMC-500 Frame Grabber�� �����Ѵ�.
bool CMIU::Open()
{ 
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString logStr = "";
	pFrame->putListLog("		MIU Open ����.");
	int	errorCode = 0;
	
	gMIUDevice.bMIUOpen = 0;
	gMIUDevice.bMIUInit = 0;
	gMIUDevice.bReadyVCM = 0;
	gMIUDevice.CurrentState = 0;

	CString strErrCount="";
	
	int _temp = 1;
	errorCode = m_pBoard->OpenBoard(m_cBoardIndex);

	if (errorCode)
	{	
		
		switch(errorCode)
		{
		case 1:
			strErrCount.Format("OpenBoard Error: ERR_BOARD_GET_LIST[%d]", errorCode);
			break;
		case 2:
			strErrCount.Format("OpenBoard Error: ERR_BOARD_NO_DEVICE[%d]", errorCode);
			break;
		case 3:
			strErrCount.Format("OpenBoard Error: ERR_BOARD_OPEN[%d]", errorCode);
			break;
		case 4:
			strErrCount.Format("OpenBoard Error: ERR_BOARD_INIT[%d]", errorCode);
			break;
		case 5:
			strErrCount.Format("OpenBoard Error: ERR_BOARD_SET_POWER_5_12[%d]", errorCode);
			break;
		case 6:
			strErrCount.Format("OpenBoard Error: ERR_BOARD_GRAB_START[%d]", errorCode);
			break;
		case 7:
			strErrCount.Format("OpenBoard Error: ERR_BOARD_I2C_WRITE[%d]", errorCode);
			break;
		case 8:
			strErrCount.Format("OpenBoard Error: ERR_BOARD_I2C_READ[%d]", errorCode);
			break;
		case 9:
			strErrCount.Format("OpenBoard Error: ERR_BOARD_BOOT_CAMERA[%d]", errorCode);
			break;
		case 10:
			strErrCount.Format("OpenBoard Error: ERR_BOARD_CLOSE[%d]", errorCode);
			break;
		case 11:
			strErrCount.Format("OpenBoard Error: ERR_BOARD_LOAD_MODEL[%d]", errorCode);
			break;
		}
		AfxMessageBox(strErrCount);
		return false;
	}
	else
	{
		pFrame->putListLog("MIU Open �Ϸ�.");
		strErrCount.Format("OpenBoard Success");
		
	}

	//
	//! [���� ����] ������ ���Ŀ��� ����� �� ���� �������̹Ƿ�, �ݵ�� �ʱ�ȭ �Ѵ�. 
	gMIUDevice.IMX135.bFlag_Valid = 0;

	//! Laon People������ �亯 (2013/3/5)
	//!  - 0x00001006 Error�� MIUOpenDevice�Լ� ȣ��� ��ȯ�ϴ� ������ 
	//!    �ٸ� ���α׷����� �̹� Open�Ǿ� �ִ� ���¿��� �ٽ� Open�Ϸ��� �� �� �߻��Ǵ� Error �Դϴ�.

	gMIUDevice.bMIUOpen = 1;

	//! �ʱ⿡�� ����� ������ �����Ѵ�. 
	//bool bRet = SelectSensor();
	
	//! Thread�� ���� ��� ��� �ð� ���� �ʱ�ȭ
	Init_Grab_Time();

	//! Display�� Frame Rate ������ �ʱ�ȭ
	Init_Display_Count();
	//AfxMessageBox(strErrCount);
	logStr.Format("[MIU] Open �Ϸ�.");
	return true;
}

//! LPMC-500 Frame Grabber���� ������ �����Ѵ�. 
bool CMIU::Close()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	CString logStr;
	logStr.Format("  Close ����.");
	MiuLogSave(logStr);

	int iRet = 0;
	bool bRtnFlag = true;
	if (m_pBoard->IsGrabStarted())//if ( gMIUDevice.CurrentState >= 2 )
	{
		Stop();
		Sleep(500);
	}
	//! ���� ����� �����Ѵ�. (CCD�� ������ ������ ����)

	iRet = m_pBoard->CloseBoard();

	INI_LOAD();
	if(iRet != BOARD_SUCCESS)
	{
		return false;
	}
	theApp.MainDlg->m_labelUsbLive.SetBkColor(M_COLOR_RED);
	theApp.MainDlg->m_labelUsbLive.Invalidate();
	Sleep(200);

	//! Flag�� �ʱ�ȭ
	gMIUDevice.bMIUOpen = 0;
	gMIUDevice.bMIUInit = 0;
	gMIUDevice.bReadyVCM = 0;

	//! [���� ����] ������ ������ ���Ŀ��� ����� �� ���� �������̹Ƿ�, �ݵ�� �ʱ�ȭ �Ѵ�. 
	gMIUDevice.IMX135.bFlag_Valid = 0;

//	m_cPcie.DeviceReset() ;
	if(pFrame->m_bMiuRun)
	{
		pFrame->m_bMiuRun=false;
	}
	
	return bRtnFlag;
}

//! LPMC-800 Frame Grabber���� ����ϴ� CCD Sensor�� �����Ѵ�. 
//! SensorList������ index�� �̿��Ͽ�, ����� CCD Sensor�� �˷��־�� �Ѵ�.
unsigned short ChecksumCalc(unsigned short* arr, unsigned int size)
{
	unsigned int tmp_checksum = 0, i;
	unsigned short checksum = 0;
	for (i = 0; i < size; i++)
	{
		tmp_checksum += arr[i];
	}
	checksum = (unsigned short)(tmp_checksum & 0x000000ff);
	return checksum;
}
void CMIU::ISPmodeCheck()
{
	unsigned int errorCode = 0, i;
	unsigned short ReadByteCount;
	unsigned short AddressByteCount;
	unsigned short SlaveAddress;
	unsigned short TempAdress1[22] = { 0x33, 0x47, 0x0F, 0x00, 0x00, 0x00, 0x53, 0x00, 0x80, 0x02 ,0x00,0x00,0x00,0x00,0xFA,0x00,0x00, 0x00 ,0x00,0x00,0x00, 0x00 };
	unsigned short TempAdress2[11] = { 0x33, 0x51, 0x07, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00 };
	unsigned char TempData1[6] = { 0, };// = new unsigned short[6];//unsigned char TempData1[6] = { 0, };

	unsigned char TempData2[19] = { 0, };
	unsigned short checksum = 0;
	SlaveAddress = 0x6D;
	AddressByteCount = 22;
	ReadByteCount = 6;
	checksum = ChecksumCalc(TempAdress1, 22);
	TempAdress1[21] = checksum;
	//errorCode = I2CMABurstRead(SlaveAddress, TempAdress1, AddressByteCount, ReadByteCount, TempData1);
	errorCode = m_pBoard->ReadI2CMultiAddrBurst(SlaveAddress, TempAdress1, AddressByteCount, TempData1, ReadByteCount);
	//WriteI2CMultiAddrBurst
	//(unsigned short nSlaveAddr, unsigned short* nAddr, unsigned char nAddrByteCount, unsigned char* pData, unsigned short nWriteByteCount) = 0;

	if (errorCode)
	{
		CString ErrorMessage;
		ErrorMessage.Format(_T("BurstRead Fail!!! (Code : 0x%x)"), errorCode);
		AfxMessageBox(ErrorMessage);
	}
	TempAdress2[3] = TempData1[3];
	checksum = ChecksumCalc(TempAdress2, 11);
	TempAdress2[10] = checksum;
	AddressByteCount = 11;
	ReadByteCount = 19;

	errorCode = m_pBoard->ReadI2CMultiAddrBurst(SlaveAddress, TempAdress2, AddressByteCount, TempData2, ReadByteCount);
	if (!errorCode)
	{
		CString csFormat, csFormat1, csFormat2, csFormat3, csTemp1, csTemp2;
		for (i = 0; i < ReadByteCount; i++)
		{
			csTemp1.Format("%02X", TempData2[i]);
			csFormat1 += csTemp1;
		}
		csTemp2.Format("%02X", TempData2[15]);
		csFormat2 += csTemp2;
		csFormat = _T("ISP Mode:0x") + csFormat2 + _T(" , Read Data:") + csFormat1;
		//m_ctrlEditReadData.SetWindowTextA(csFormat);
		if (csFormat2 == _T("01")) csFormat3 = _T(" : rgb888_hdr");
		else if (csFormat2 == _T("02")) csFormat3 = _T(" : raw20");
		else if (csFormat2 == _T("03")) csFormat3 = _T(" : rgb888_linear");
		csFormat = _T("ISP Mode(0x") + csFormat2 + _T(")") + csFormat3;
		theApp.MainDlg->putListLog("rgb="+csFormat);
		//QueryResMsg(csFormat);
	}
	else
	{
		CString ErrorMessage;
		ErrorMessage.Format(_T("BurstRead Fail!!! (Code : 0x%x)"), errorCode);
		AfxMessageBox(ErrorMessage);
	}
	return; 
}
void CMIU::SwitchLinearToHDRMode()
{
	unsigned int errorCode = 0, i;
	unsigned short ReadByteCount, DataByteCount;
	unsigned short AddressByteCount;
	unsigned short SlaveAddress;
	unsigned short TempAdress1[18] = { 0x33, 0x47, 0x0B, 0x00, 0x00, 0x00, 0x12, 0x00, 0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00 };
	//unsigned short TempAdress1[18] = { 0x33, 0x47, 0x0B, 0x00, 0x00, 0x00, 0x12, 0x00, 0x80, 0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x00, 0x00 };
	unsigned char TempData1[9] = { 0, };
	unsigned short checksum = 0;
	SlaveAddress = 0x6D;
	AddressByteCount = 18;
	ReadByteCount = 9;
	checksum = ChecksumCalc(TempAdress1, 18);
	TempAdress1[17] = checksum;
	//errorCode = I2CMABurstRead(SlaveAddress, TempAdress1, AddressByteCount, ReadByteCount, TempData1);
	errorCode = m_pBoard->ReadI2CMultiAddrBurst(SlaveAddress, TempAdress1, AddressByteCount, TempData1, ReadByteCount);
	if (errorCode)
	{
		CString ErrorMessage;
		ErrorMessage.Format(_T("BurstRead Fail!!! (Code : 0x%x)"), errorCode);
		AfxMessageBox(ErrorMessage);
	}
	//unsigned short TempAddr[2] = { 0x02,0xD6 };
	unsigned short TempAddr = 0x02D6;
	unsigned short ___TempAddr[2] = { 0x02, 0xD6};
	unsigned char TempData3[1] = { 0x9C, };
	unsigned char TempData4[1] = { 0x80, };
	unsigned short SlaveTempAddr = 0xC4;
	Sleep(1500);
	//SerFsyncControl(SER_SLAVE_ADDR, TempAddr, TempData3);
	//errorCode = m_pBoard->WriteI2CBurst(SlaveTempAddr, TempAddr, 2, TempData3, 1);
	//
	unsigned int nSlaveAddress = 0x62;// 0xC4;
	unsigned int nAddress = 0x02D6;
	unsigned char pData[5];
	pData[0] = 0x9C;
	//
	//errorCode = m_pBoard->WriteI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress, 2, pData, (unsigned short)1);
	errorCode = m_pBoard->WriteI2CBurst(SlaveTempAddr, TempAddr, 2, TempData3, 1);
	Sleep(50);
	//SerFsyncControl(SER_SLAVE_ADDR, TempAddr, TempData4);
	errorCode = m_pBoard->WriteI2CBurst(SlaveTempAddr, TempAddr, 2, TempData4, 1);
	Sleep(50); 
	//SerFsyncControl(SER_SLAVE_ADDR, TempAddr, TempData3);
	errorCode = m_pBoard->WriteI2CBurst(SlaveTempAddr, TempAddr, 2, TempData3, 1);
	Sleep(1000);
	ISPmodeCheck();

}
void CMIU::SwitchHDRToLinearMode()
{
	unsigned int errorCode = 0, i;
	unsigned short ReadByteCount;
	unsigned short DataByteCount;
	unsigned short AddressByteCount;
	unsigned short SlaveAddress;
	unsigned short TempAdress1[18] = { 0x33, 0x47, 0x0B, 0x00, 0x00, 0x00, 0x12, 0x00, 0x80, 0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x00, 0x00 };
	unsigned char TempData1[9] = { 0, };
	unsigned short checksum = 0;
	SlaveAddress = 0x6D;
	AddressByteCount = 18;
	ReadByteCount = 9;
	checksum = ChecksumCalc(TempAdress1, 18); 
	TempAdress1[17] = checksum;
	errorCode = m_pBoard->ReadI2CMultiAddrBurst(SlaveAddress, TempAdress1, AddressByteCount, TempData1, ReadByteCount);
	if (errorCode)
	{
		CString ErrorMessage;
		ErrorMessage.Format(_T("BurstRead Fail!!! (Code : 0x%x)"), errorCode);
		AfxMessageBox(ErrorMessage);
	}
	unsigned short TempAddr = 0x02D6;// { 0x02, 0xD6 };//TempAddr[2] = { 0x02,0xD6 };
	unsigned char TempData3[1] = { 0x9C };
	unsigned char TempData4[1] = { 0x80 };
	unsigned short SlaveTempAddr = 0x62;// 0xC4;
	Sleep(1500);
	//SerFsyncControl(SER_SLAVE_ADDR, TempAddr, TempData3);//WriteI2CBurst , 1  0c4
	errorCode = m_pBoard->WriteI2CBurst(SlaveTempAddr, TempAddr, 2, TempData3, 1);
	Sleep(50);
	//SerFsyncControl(SER_SLAVE_ADDR, TempAddr, TempData4);
	errorCode = m_pBoard->WriteI2CBurst(SlaveTempAddr, TempAddr, 2, TempData4, 1);
	Sleep(50);
	//SerFsyncControl(SER_SLAVE_ADDR, TempAddr, TempData3);
	errorCode = m_pBoard->WriteI2CBurst(SlaveTempAddr, TempAddr, 2, TempData3, 1);
	Sleep(1000);
	ISPmodeCheck();
}
bool CMIU::SelectSensor()
{
	gMIUDevice.nWidth =  m_pBoard->GetWidth(); 
	gMIUDevice.nHeight =  m_pBoard->GetHeight();
	gMIUDevice.iPixelFormat = m_pBoard->GetDataFormat();
	gMIUDevice.nFrameImageSize = m_pBoard->GetFrameRawSize();

	////gMIUDevice.dTDATASPEC_n.eOutMode = OUTMODE_BAYER_GBRG;		//M_MPC ��
	gMIUDevice.dTDATASPEC_n.eOutMode = (EOUTMODE)m_pBoard->GetOutMode();

	
	gMIUDevice.dTDATASPEC_n.eDataFormat = (EDATAFORMAT)m_pBoard->GetDataFormat();		//EDATAFORMAT �� DATAFORMAT_YUV�϶� +5
	gMIUDevice.dTDATASPEC_n.eSensorType = (ESENSORTYPE) m_pBoard->GetSensorType();
    

	return true;
}


void CMIU::GetImagePixelFormat(char* szPixelFormat, unsigned int* nPixelFormat)
{
	int nSelectIndex = 0;
	CString strPixelFormat;
	strPixelFormat.Format("%s", szPixelFormat); 

	
	if(strPixelFormat == "MIU_MONO8")						// Mono buffer format defines
	{
		nSelectIndex = 0; // Combo box index
		*nPixelFormat = MIU_MONO8;
	}
	else if(strPixelFormat == "MIU_MONO10_PACKED")
	{
		nSelectIndex = 1;
		*nPixelFormat = MIU_MONO10_PACKED;
	}
	else if(strPixelFormat == "MIU_MONO12_PACKED")
	{
		nSelectIndex = 2;
		*nPixelFormat = MIU_MONO12_PACKED;
	}
	else if(strPixelFormat == "MIU_MONO14")
	{
		nSelectIndex = 3;
		*nPixelFormat = MIU_MONO14;
	}
	else if(strPixelFormat == "MIU_BAYERGR8")				// Bayer buffer format defines
	{
		nSelectIndex = 4;
		*nPixelFormat = MIU_BAYERGR8;
	}
	else if(strPixelFormat == "MIU_BAYERRG8")
	{
		nSelectIndex = 5;
		*nPixelFormat = MIU_BAYERRG8;
	}
	else if(strPixelFormat == "MIU_BAYERGB8")
	{
		nSelectIndex = 6;
		*nPixelFormat = MIU_BAYERGB8;
	}
	else if(strPixelFormat == "MIU_BAYERBG8")
	{
		nSelectIndex = 7;
		*nPixelFormat = MIU_BAYERBG8;
	}
	else if(strPixelFormat == "MIU_BAYERGR10_PACKED")
	{
		nSelectIndex = 8;
		*nPixelFormat = MIU_BAYERGR10_PACKED;
	}
	else if(strPixelFormat == "MIU_BAYERRG10_PACKED")
	{
		nSelectIndex = 9;
		*nPixelFormat = MIU_BAYERRG10_PACKED;
	}
	else if(strPixelFormat == "MIU_BAYERGB10_PACKED")
	{
		nSelectIndex = 10;
		*nPixelFormat = MIU_BAYERGB10_PACKED;
	}
	else if(strPixelFormat == "MIU_BAYERBG10_PACKED")
	{
		nSelectIndex = 11;
		*nPixelFormat = MIU_BAYERBG10_PACKED;
	}
	else if(strPixelFormat == "MIU_BAYERGR12_PACKED")
	{
		nSelectIndex = 12;
		*nPixelFormat = MIU_BAYERGR12_PACKED;
	}
	else if(strPixelFormat == "MIU_BAYERRG12_PACKED")
	{
		nSelectIndex = 13;
		*nPixelFormat = MIU_BAYERRG12_PACKED;
	}
	else if(strPixelFormat == "MIU_BAYERGB12_PACKED")
	{
		nSelectIndex = 14;
		*nPixelFormat = MIU_BAYERGB12_PACKED;
	}
	else if(strPixelFormat == "MIU_BAYERBG12_PACKED")
	{
		nSelectIndex = 15;
		*nPixelFormat = MIU_BAYERBG12_PACKED;
	}
	else if(strPixelFormat == "MIU_RGB8_PACKED")		// RGB Packed buffer format defines
	{
		nSelectIndex = 16;
		*nPixelFormat = MIU_RGB8_PACKED;
	}
	else if(strPixelFormat == "MIU_BGR8_PACKED")
	{
		nSelectIndex = 17;
		*nPixelFormat = MIU_BGR8_PACKED;
	}
	else if(strPixelFormat == "MIU_RGB565")
	{
		nSelectIndex = 18;
		*nPixelFormat = MIU_RGB565;
	}
	else if(strPixelFormat == "MIU_BGR565")
	{
		nSelectIndex = 19;
		*nPixelFormat = MIU_BGR565;
	}
	else if(strPixelFormat == "MIU_YUV422_PACKED")		// YUV Packed buffer format defines
	{
		nSelectIndex = 20;
		*nPixelFormat = MIU_YUV422_PACKED;
	}
	else if(strPixelFormat == "MIU_YUV422_YUYV_PACKED")
	{
		nSelectIndex = 21;
		*nPixelFormat = MIU_YUV422_YUYV_PACKED;
	}
	else
	{
		AfxMessageBox("Check the Image Pixel Format!");
	}
}

//! ���� ����
bool CMIU::cvBufferAlloc()
{
	CString logStr;
	int iWidth = m_pBoard->GetWidth();
	int iHeight = m_pBoard->GetHeight();

	//! ���� ������ ��ȿ�� �˻�	
	if ( gMIUDevice.nWidth <= 0 || gMIUDevice.nHeight <= 0 )
	{
		logStr.Format("  ���� ���� �̻�.");
		theApp.MainDlg->putListLog(logStr);
		logStr.Empty();
		return false;
	}

	if ( gMIUDevice.imageItp != NULL || 
		 m_acvChildImage[0] != NULL || m_acvChildImage[1] != NULL || m_acvChildImage[2] != NULL ||
		pImageBuf[0] != NULL || pImageBuf[1] != NULL || pImageBuf[2] != NULL||
		m_acvChildImage[0] != NULL || m_acvChildImage[1] != NULL || m_acvChildImage[2] != NULL)
	{
		//! ������ ������ ���۴� �����Ѵ�.
		cvBufferFree();
	}

	
	if (m_pFrameRawBuffer != NULL)
	{
		delete[] m_pFrameRawBuffer; 
		m_pFrameRawBuffer = NULL;
	}

	if (m_pFrameBMPBuffer != NULL)
	{
		delete[] m_pFrameBMPBuffer;
		m_pFrameBMPBuffer = NULL;
	}
	if (vChartBuffet != NULL)
	{
		delete[] vChartBuffet;
		vChartBuffet = NULL;
	}
	if (vChart_Second_Buffet != NULL)
	{
		delete[] vChart_Second_Buffet;
		vChart_Second_Buffet = NULL;
	}
	if (vDefectMidBuffer_6500K != NULL)
	{
		delete[] vDefectMidBuffer_6500K;
		vDefectMidBuffer_6500K = NULL;
	}
	if (vDefectLowBuffer != NULL)
	{
		delete[] vDefectLowBuffer;
		vDefectLowBuffer = NULL;
	}
	if (vDefectMidBuffer_2800K != NULL)
	{
		delete[] vDefectMidBuffer_2800K;
		vDefectMidBuffer_2800K = NULL;
	}
	
	m_pFrameRawBuffer = new BYTE[m_pBoard->GetFrameRawSize()];
	vChartBuffet = new BYTE[m_pBoard->GetFrameRawSize()];
	vChart_Second_Buffet = new BYTE[m_pBoard->GetFrameRawSize()];
	vDefectMidBuffer_6500K = new BYTE[m_pBoard->GetFrameRawSize()];
	vDefectLowBuffer = new BYTE[m_pBoard->GetFrameRawSize()];
	vDefectMidBuffer_2800K = new BYTE[m_pBoard->GetFrameRawSize()];

	//bmp
	m_pFrameBMPBuffer = new BYTE[m_pBoard->GetFrameBMPSize()];
	memset(m_pFrameBMPBuffer, 0, m_pBoard->GetFrameBMPSize());
	//
	//raw
	memset(m_pFrameRawBuffer, 0, m_pBoard->GetFrameRawSize());
	memset(vChartBuffet, 0, m_pBoard->GetFrameRawSize());
	memset(vChart_Second_Buffet, 0, m_pBoard->GetFrameRawSize());
	memset(vDefectMidBuffer_6500K, 0, m_pBoard->GetFrameRawSize());
	memset(vDefectLowBuffer, 0, m_pBoard->GetFrameRawSize());
	memset(vDefectMidBuffer_2800K, 0, m_pBoard->GetFrameRawSize());

	gMIUDevice.imageItp = cvCreateImage(cvSize(gMIUDevice.nWidth,gMIUDevice.nHeight),IPL_DEPTH_8U,3);

	if ( m_bFlag_Mil_Convert == true )
	{
		m_acvChildImage[0] = cvCreateImage(cvSize(gMIUDevice.nWidth,gMIUDevice.nHeight), IPL_DEPTH_8U, 1);
		m_acvChildImage[1] = cvCreateImage(cvSize(gMIUDevice.nWidth,gMIUDevice.nHeight), IPL_DEPTH_8U, 1);
		m_acvChildImage[2] = cvCreateImage(cvSize(gMIUDevice.nWidth,gMIUDevice.nHeight), IPL_DEPTH_8U, 1);
	}	

	pImageBuf[0] = cvCreateImage(cvSize(iWidth, iHeight), IPL_DEPTH_8U, 1);
	pImageBuf[1] = cvCreateImage(cvSize(iWidth, iHeight), IPL_DEPTH_8U, 1);
	pImageBuf[2] = cvCreateImage(cvSize(iWidth, iHeight), IPL_DEPTH_8U, 1);

	//! <-------------------------------------------------------------------------------------------------------------------
	Init_Index_Grab();
	//! <-------------------------------------------------------------------------------------------------------------------

	
	logStr.Format("  ���ۻ��� �Ϸ�.");
	theApp.MainDlg->putListLog(logStr);

	logStr.Empty();
	return true;	
}

//! ���� ����
void CMIU::cvBufferFree()
{
	int i = 0;
	//CString logStr;
	////.Format("  ���� ���� ����.");
	//theApp.MainDlg->putListLog(logStr);

	//! [���� ����] ���� ��游 �����ؾ��Ѵ�. CCD�� �������� ���� �ȵȴ�.
	//Stop(false);
	//
	if ( gMIUDevice.imageItp != NULL )
	{
		cvReleaseImage(&gMIUDevice.imageItp);
		gMIUDevice.imageItp = NULL;
	}
	for (i = 0; i < 3; i++)
	{
		if (m_acvChildImage[i] != NULL)
		{
			cvReleaseImage(&m_acvChildImage[i]);
			m_acvChildImage[i] = NULL;
		}

		if (pImageBuf[i] != NULL)
		{
			cvReleaseImage(&pImageBuf[i]);
			pImageBuf[i] = NULL;
		}
	}

	//logStr.Format("  ���� ���� �Ϸ�.");
	//theApp.MainDlg->putListLog(logStr);
}


//! Open CV �̹��� ----> Mil Color �̹��� ��ȯ
bool CMIU::cvCopyToMil(MIL_ID* f_pDestBuf)
{
	if ( f_pDestBuf == NULL )
	{
		return false;
	}
	if ( m_bFlag_Mil_Convert == false )
	{
		return false;
	}
	if ( m_acvChildImage[0] == NULL || m_acvChildImage[1] == NULL || m_acvChildImage[2] == NULL )
	{
		cvBufferAlloc();		//cvCopyToMil
	}

	//! RGB ä�� �и�..
	cvCvtPixToPlane(gMIUDevice.imageItp, m_acvChildImage[0], m_acvChildImage[1], m_acvChildImage[2], NULL);

	MbufPut(f_pDestBuf[0], m_acvChildImage[2]->imageData);
	MbufPut(f_pDestBuf[1], m_acvChildImage[1]->imageData);
	MbufPut(f_pDestBuf[2], m_acvChildImage[0]->imageData);

	return true;
}

//! Mil Color �̹��� ----> Open CV �̹���
//! bFlag_Green : true�̸� ��� ������ ���۸� ���
bool CMIU::MilCopyToCv(MIL_ID* f_pDestBuf)
{
	if ( f_pDestBuf == NULL )
	{
		return false;
	}
	if ( m_bFlag_Mil_Convert == false )
	{
		return false;
	}
	if ( m_acvChildImage[0] == NULL || m_acvChildImage[1] == NULL || m_acvChildImage[2] == NULL )
	{
		cvBufferAlloc();	//MilCopyToCv
	}	

	MbufGet(f_pDestBuf[2], m_acvChildImage[0]->imageData);				// 2.6 msec
	MbufGet(f_pDestBuf[1], m_acvChildImage[1]->imageData);
	MbufGet(f_pDestBuf[0], m_acvChildImage[2]->imageData);
	
	//! ���� ��ȯ �� RGB --> BGR �� ���� ���� ����..

	cvCvtPlaneToPix(m_acvChildImage[2], m_acvChildImage[1], m_acvChildImage[0], NULL, gMIUDevice.imageItp);		// 57.5msec

	return true;
}


bool CMIU::CCM_Init()
{
	bool bRet = false;

	bRet = CcmInit();

	return bRet;
}

//! LPMC-500 Frame Grabber�� �ʱ�ȭ
bool CMIU::MIU_Init()
{
//	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
//	CString logStr;
//	logStr.Format("[MIU] MIU_Init ȣ��.");
//	MiuLogSave(logStr);
//
//	//! ���� ������ ��ȿ�� �˻�
//	if ( m_iDeviceCount <= 0 )
//	{
//		return false;
//	}
//	
//	if ( m_pSID == NULL || m_iSensorListCnt <= 0 )
//	{
//		return false;
//	}	
//	if ( m_iSensor < 0 || m_iSensor >= m_iSensorListCnt )
//	{
//		return false;
//	}
//	if ( m_pSID[m_iSensor].pInfo == NULL || m_pSID[m_iSensor].nInfoCount <= 0 )
//	{
//		return false;
//	}
//	if ( m_iSensorType < 0 || m_iSensorType >= m_pSID[m_iSensor].nInfoCount )
//	{
//		return false;
//	}
//
//	if ( gMIUDevice.bMIUOpen != 1 )
//	{
//		sLogMessage.Format(_T("LPMC ������ Ȯ���ϼ���."));
//		return false;
//	}
//
//	//! CCD Sensor�� �ʱ�ȭ ����
//	gMIUDevice.CurrentState = 0;
//
//	char            szFile_Name_1[MAX_PATH]= {0};
//	char            szFile_Name[MAX_PATH]= {0};
//	char			szTemp[MAX_PATH];	
//	CString			strSectionValue;
//
//	//ISJ Data Load
//	//! CCD Sensor�� ini ������ ���� ��θ��� �˾Ƴ���.
//	sprintf_s(szFile_Name, "%s\\%s_%s.ini", MIU_DIR, 
//		                                    m_pSID[m_iSensor].szSensorName, 
//											m_pSID[m_iSensor].pInfo[m_iSensorType].szResolution);
//
//#ifdef USE_MIU_OPEN_TACT
//	asTime[iCnt_Time].Format("FILE Ȯ�� �Ϸ� - %.1f sec.", timeChecker.m_adTime[iCnt_Time] );
//	iCnt_Time++;
//#endif
//
//
//	//! CCD Sensor�� ini ���Ͽ��� ���� ������ �о�´�.
//			gMIUDevice.InitialValue.SensorMode = GetPrivateProfileInt("MIUConfiguration", "SensorMode" ,0 , szFile_Name);
//			gMIUDevice.InitialValue.nWidth = GetPrivateProfileInt("MIUConfiguration", "SensorWidth", 0, szFile_Name);
//			gMIUDevice.InitialValue.nHeight = GetPrivateProfileInt("MIUConfiguration", "SensorHeight", 0, szFile_Name);
//			gMIUDevice.nWidth = gMIUDevice.InitialValue.nWidth;
//			gMIUDevice.nHeight = gMIUDevice.InitialValue.nHeight;
//	
//			gMIUDevice.nMaxWidth = GetPrivateProfileInt("MIUConfiguration", "MAXWidth", 4000, szFile_Name);;			// Max Width
//			gMIUDevice.nMaxHeight = GetPrivateProfileInt("MIUConfiguration", "MAXHeight", 3000, szFile_Name);;			// Max Height   
//	
//			gMIUDevice.InitialValue.MCLKOnOff = GetPrivateProfileInt("MIUConfiguration", "MCLKOnOff" ,1 , szFile_Name);
//			gMIUDevice.InitialValue.MCLKSelection = GetPrivateProfileInt("MIUConfiguration", "MCLKSelection" ,1 , szFile_Name);
//			GetPrivateProfileString("MIUConfiguration","MCLK","0.0", szTemp, MAX_PATH, szFile_Name);
//			strSectionValue.Format("%s",szTemp);
//			sscanf(strSectionValue.GetBuffer(0),"%f",&gMIUDevice.InitialValue.MCLK);
//			//m_strMCLK.Format("%.2f",gMIUDevice.InitialValue.MCLK);
//	
//			gMIUDevice.InitialValue.PCLKInversion = GetPrivateProfileInt("MIUConfiguration", "PCLKInversion", 0, szFile_Name);
//			GetPrivateProfileString("MIUConfiguration","IICDeviceID","", szTemp, MAX_PATH, szFile_Name);
//			strSectionValue.Format("%s",szTemp);
//			sscanf(strSectionValue.GetBuffer(0),"%x",&gMIUDevice.InitialValue.IICDeviceID);
//			gMIUDevice.InitialValue.IICMode = GetPrivateProfileInt("MIUConfiguration", "IICMode", 0, szFile_Name);
//			gMIUDevice.InitialValue.IICSpeed = GetPrivateProfileInt("MIUConfiguration", "IICSpeed", 0, szFile_Name);
//			gMIUDevice.InitialValue.IICReadRestart = GetPrivateProfileInt("MIUConfiguration", "IICReadRestart", 1, szFile_Name);
//			gMIUDevice.InitialValue.IICReadRestartInterval = GetPrivateProfileInt("MIUConfiguration", "IICReadRestartInterval", 0, szFile_Name);
//			gMIUDevice.InitialValue.IICSCKPinCheck = GetPrivateProfileInt("MIUConfiguration", "IICSCKPinCheck", 0, szFile_Name);
//			gMIUDevice.InitialValue.IICAddressLength = GetPrivateProfileInt("MIUConfiguration", "IICAddressLength", 0, szFile_Name);
//			gMIUDevice.InitialValue.IICDataLength = GetPrivateProfileInt("MIUConfiguration", "IICDataLength", 0, szFile_Name);
//
//			gMIUDevice.InitialValue.MIPILaneEnable = GetPrivateProfileInt("MIUConfiguration", "MIPILaneEnable", 1, szFile_Name);
//			GetPrivateProfileString("MIUConfiguration","MIPIDataType","0x2B", szTemp, MAX_PATH, szFile_Name);
//			strSectionValue.Format("%s",szTemp);
//			sscanf(strSectionValue.GetBuffer(0),"%x",&gMIUDevice.InitialValue.MIPIDataType);
//	
//			gMIUDevice.InitialValue.MIPI8bitMode = GetPrivateProfileInt("MIUConfiguration", "MIPI8bitMode", 0, szFile_Name);
//
//			GetPrivateProfileString("MIUConfiguration","MIUIOVoltage","0.0", szTemp, MAX_PATH, szFile_Name);
//			strSectionValue.Format("%s",szTemp);
//			sscanf(strSectionValue.GetBuffer(0),"%f",&gMIUDevice.InitialValue.MIUIOVoltage);
//
//			gMIUDevice.InitialValue.FirstPowerChannel = GetPrivateProfileInt("MIUConfiguration", "1stPowerChannel", 0, szFile_Name);
//
//			GetPrivateProfileString("MIUConfiguration","1stPowerVoltage","0.0", szTemp, MAX_PATH, szFile_Name);
//			strSectionValue.Format("%s",szTemp);
//			sscanf(strSectionValue.GetBuffer(0),"%f",&gMIUDevice.InitialValue.FirstPowerVoltage);
//
//			gMIUDevice.InitialValue.SecondPowerChannel = GetPrivateProfileInt("MIUConfiguration", "2ndPowerChannel", 0, szFile_Name);
//
//			GetPrivateProfileString("MIUConfiguration","2ndPowerVoltage","0.0", szTemp, MAX_PATH, szFile_Name);
//			strSectionValue.Format("%s",szTemp);
//			sscanf(strSectionValue.GetBuffer(0),"%f",&gMIUDevice.InitialValue.SecondPowerVoltage);
//
//			gMIUDevice.InitialValue.ThirdPowerChannel = GetPrivateProfileInt("MIUConfiguration", "3rdPowerChannel", 0, szFile_Name);
//
//			GetPrivateProfileString("MIUConfiguration","3rdPowerVoltage","0.0", szTemp, MAX_PATH, szFile_Name);
//			strSectionValue.Format("%s",szTemp);
//			sscanf(strSectionValue.GetBuffer(0),"%f",&gMIUDevice.InitialValue.ThirdPowerVoltage);
//
//			gMIUDevice.InitialValue.FourthPowerChannel = GetPrivateProfileInt("MIUConfiguration", "4thPowerChannel", 0, szFile_Name);
//
//			GetPrivateProfileString("MIUConfiguration","4thPowerVoltage","0.0", szTemp, MAX_PATH, szFile_Name);
//			strSectionValue.Format("%s",szTemp);
//			sscanf(strSectionValue.GetBuffer(0),"%f",&gMIUDevice.InitialValue.FourthPowerVoltage);
//
//			gMIUDevice.InitialValue.FifthPowerChannel = GetPrivateProfileInt("MIUConfiguration", "5thPowerChannel", 0, szFile_Name);
//
//			GetPrivateProfileString("MIUConfiguration","5thPowerVoltage","0.0", szTemp, MAX_PATH, szFile_Name);
//			strSectionValue.Format("%s",szTemp);
//			sscanf(strSectionValue.GetBuffer(0),"%f",&gMIUDevice.InitialValue.FifthPowerVoltage);
//
//			gMIUDevice.InitialValue.Power5VoltOnOff = GetPrivateProfileInt("MIUConfiguration", "5VoltPowerOnOff", 0, szFile_Name);
//			gMIUDevice.InitialValue.Power12VoltOnOff = GetPrivateProfileInt("MIUConfiguration", "12VoltPowerOnOff", 0, szFile_Name);
//
//			gMIUDevice.InitialValue.InitialSkipCount = GetPrivateProfileInt("MIUConfiguration", "InitialSkipCount", 0, szFile_Name);
//			gMIUDevice.InitialValue.PreviewSkipCount = GetPrivateProfileInt("MIUConfiguration", "PreviewSkipCount", 0, szFile_Name);
//
//			gMIUDevice.InitialValue.ParallelSamplingMode = GetPrivateProfileInt("MIUConfiguration", "ParallelSamplingMode", 0, szFile_Name);
//			gMIUDevice.InitialValue.ParallelBitsPerPixel = GetPrivateProfileInt("MIUConfiguration", "ParallelBitsPerPixel", 0, szFile_Name);
//			gMIUDevice.InitialValue.ParallelPixelComponent = GetPrivateProfileInt("MIUConfiguration", "ParallelPixelComponent", 0, szFile_Name);
//			gMIUDevice.InitialValue.ParallelBitShift = GetPrivateProfileInt("MIUConfiguration", "ParallelBitShift", 0, szFile_Name);
//		
//	//! CCD Sensor�� �ʱ� �������� �����Ѵ�.  
//	int iErrorCode = MIUInitialize(gDeviceIndex, gMIUDevice.InitialValue);
//	if ( iErrorCode != MIU_OK )
//	{
//		sLogMessage.Format(_T("'MIUInitialize'�Լ� ���� [%d]"), iErrorCode);
//
//		logStr.Format("[MIU[ MIU_Init ����. CCD ������ �ٽ� ������ �ּ���.");
//		MiuLogSave(logStr);
//
//		pFrame->m_bMiuRun = false;
//
//		Close();
//
//		return false;
//	}
//
//#ifdef USE_MIU_OPEN_TACT
//	timeChecker.Measure_Time(iCnt_Time);
//	asTime[iCnt_Time].Format("MIU Init �Ϸ� - %.1f sec.", timeChecker.m_adTime[iCnt_Time] - timeChecker.m_adTime[iCnt_Time-1] );
//	iCnt_Time++;
//#endif
//
//
//	gMIUDevice.bMIUInit = 1;
//
//	logStr.Format("[MIU] MIU_Init �Ϸ�.");
//	MiuLogSave(logStr);

	return true;
}


//! CCD Camera Sensor�� �ʱ�ȭ�� �����Ѵ�.
bool CMIU::CCD_Init()
{
//	int errorCode;
//	CString logStr;
//	logStr.Format("[MIU] CCD_Init ȣ��.");
//	MiuLogSave(logStr);
//
//	//! ���� ������ ��ȿ�� �˻�
//	if ( m_iDeviceCount <= 0 )
//	{
//	return false;
//	}
//	if ( m_pSID == NULL || m_iSensorListCnt <= 0 )
//	{
//		return false;
//	}	
//	if ( m_iSensor < 0 || m_iSensor >= m_iSensorListCnt )
//	{
//		return false;
//	}
//	if ( m_pSID[m_iSensor].pInfo == NULL || m_pSID[m_iSensor].nInfoCount <= 0 )
//	{
//		return false;
//	}
//	if ( m_iSensorType < 0 || m_iSensorType >= m_pSID[m_iSensor].nInfoCount )
//	{
//		return false;
//	}
//
//	// 	if ( gMIUDevice.bMIUOpen != 1 )
//	// 	{
//	// 		sLogMessage.Format(_T("LPMC ������ Ȯ���ϼ���."));
//	// 		return false;
//	// 	}
//
//	if ( gMIUDevice.bMIUInit != 1 )
//	{
//		sLogMessage.Format(_T("LPMC Frame Grabber�� �ʱ�ȭ �Ͻʽÿ�."));
//		return false;
//	}
//
//	logStr.Format("[MIU] CCD_Init ����.");
//	MiuLogSave(logStr);
//
//	char            szFile_Name_1[MAX_PATH]= {0};
//	char            szFile_Name[MAX_PATH]= {0};
//	char			szTemp[MAX_PATH];
//	CStdioFile      flInitialFile;
//	CFileException  e;
//	CString         szLineRead;
//	char            fInitMode = 0;
//	unsigned int    nAddress, nData;
//	int				errorCount = 0;
//	BYTE			BurstWriteData[2048];
//	int				BurstWriteLength = 0;
//	int				BurstWriteLengthaddr = 0;
//	unsigned short  addr, data;
//	unsigned int	bustSleep	= 0;
//
//	int				i = 0;
//	bool			bFlag_Error_Display = false;
//	bool			bFlag_Error = false;
//
//	//int             iErrorCode = MIU_OK;
//
//	//! CCD Sensor�� ini ������ ���� ��θ��� �˾Ƴ���.
//	sprintf_s(szFile_Name, "%s\\%s_%s.ini", MIU_DIR, 
//			  m_pSID[m_iSensor].szSensorName, 
//			  m_pSID[m_iSensor].pInfo[m_iSensorType].szResolution);
//
//	//! CCD Sensor�� ini ������ ���� �� �ֵ��� Open �մϴ�.
//	if(!flInitialFile.Open(szFile_Name, CFile::modeRead, &e))
//	{
//		sLogMessage.Format(_T("CCD Sensor�� ini ������ ���� �� �����ϴ�.{%s}"), szFile_Name);
//		return false;
//	}
//
//	//! ���� Flag �ʱ�ȭ
//	bFlag_Error = false;
//
//	//! CCD Sensor�� ini ���Ͽ��� Registry ���� ������ �о�ͼ�, 
//	//! LPMC-500 Frame Grabber ('[MIU] �׸�')�� CCD Sensor ('[Sensor] �׸�')�� Registry�� ������ �����մϴ�.
//
//#ifdef USE_MIU_OPEN_TACT
//	timeChecker.Measure_Time(iCnt_Time);
//	asTime[iCnt_Time].Format("CCD Init File Open Ȯ�� While�� ���� - %.1f sec.", timeChecker.m_adTime[iCnt_Time] - timeChecker.m_adTime[iCnt_Time-1] );
//	iCnt_Time++;
//#endif
//
//	flInitialFile.SeekToBegin();
//	while( flInitialFile.ReadString(szLineRead) )
//	{		
//		if((szLineRead.Find("[MIU]")!=-1)&&(szLineRead.Find("[MIU]")<2))
//		{
//			fInitMode = 1;
//			
//		}
//		else if((szLineRead.Find("[Sensor]")!=-1)&&(szLineRead.Find("[Sensor]")<2))
//		{
//			fInitMode = 2;				
//		}
//		else if(((szLineRead.Find("Sleep")!=-1)&&((szLineRead.Find("Sleep")<2)))||((szLineRead.Find("SLEEP")!=-1)&&((szLineRead.Find("SLEEP")<2))))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nData);
//			if(BurstWriteLength == 0)
//			{
//				////////////////////////// 20130711 TRACE �߰�
//				TRACE("Sleep = %d \n",nData);
//				Sleep(nData);
//				bustSleep = 0;
//			}
//			else
//			{
//				bustSleep = nData;
//
//			}			
//		}
//		else if(((szLineRead.Find("RESET")!=-1)&&((szLineRead.Find("RESET")<2)))||((szLineRead.Find("reset")!=-1)&&((szLineRead.Find("reset")<2))))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nData);
//			MIUWriteRegister(gDeviceIndex, REG_MIU, 0x04, (unsigned short) nData);			
//		}
//		else if(((szLineRead.Find("ENABLE")!=-1)&&((szLineRead.Find("ENABLE")<2)))||((szLineRead.Find("enable")!=-1)&&((szLineRead.Find("enable")<2))))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nData);
//			MIUWriteRegister(gDeviceIndex, REG_MIU, 0x05, (unsigned short) nData);			
//		}
//		else if(((szLineRead.Find("GPIO0")!=-1)&&((szLineRead.Find("GPIO0")<2)))||((szLineRead.Find("gpio0")!=-1)&&((szLineRead.Find("gpio0")<2))))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nData);
//			MIUSetGPIOStatus(gDeviceIndex, 0, nData);			
//		}
//		else if(((szLineRead.Find("GPIO1")!=-1)&&((szLineRead.Find("GPIO1")<2)))||((szLineRead.Find("gpio1")!=-1)&&((szLineRead.Find("gpio1")<2))))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nData);
//			MIUSetGPIOStatus(gDeviceIndex, 1, nData);				
//		}
//		else if(((szLineRead.Find("GPIO2")!=-1)&&((szLineRead.Find("GPIO2")<2)))||((szLineRead.Find("gpio2")!=-1)&&((szLineRead.Find("gpio2")<2))))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nData);
//			MIUSetGPIOStatus(gDeviceIndex, 2, nData);				
//		}
//		else if(((szLineRead.Find("GPIO3")!=-1)&&((szLineRead.Find("GPIO3")<2)))||((szLineRead.Find("gpio3")!=-1)&&((szLineRead.Find("gpio3")<2))))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nData);
//			MIUSetGPIOStatus(gDeviceIndex, 3, nData);				
//		}
//		else if(((szLineRead.Find("GPO0")!=-1)&&((szLineRead.Find("GPO0")<2)))||((szLineRead.Find("gpo0")!=-1)&&((szLineRead.Find("gpo0")<2))))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nData);
//			MIUSetGPOStatus(gDeviceIndex, 0, nData);			
//		}
//		else if(((szLineRead.Find("GPO1")!=-1)&&((szLineRead.Find("GPO1")<2)))||((szLineRead.Find("gpo1")!=-1)&&((szLineRead.Find("gpo1")<2))))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nData);
//			MIUSetGPOStatus(gDeviceIndex, 0, nData);			
//		}
//		else if(((szLineRead.Find("GPIOMODE0")!=-1)&&((szLineRead.Find("GPIOMODE0")<2)))||((szLineRead.Find("gpiomode0")!=-1)&&((szLineRead.Find("gpiomode0")<2))))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nData);
//			MIUSetGPIOMode(gDeviceIndex, 0, nData);		
//		}
//		else if(((szLineRead.Find("GPIOMODE1")!=-1)&&((szLineRead.Find("GPIOMODE1")<2)))||((szLineRead.Find("gpiomode1")!=-1)&&((szLineRead.Find("gpiomode1")<2))))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nData);
//			MIUSetGPIOMode(gDeviceIndex, 1, nData);		
//		}
//		else if(((szLineRead.Find("GPIOMODE2")!=-1)&&((szLineRead.Find("GPIOMODE2")<2)))||((szLineRead.Find("gpiomode2")!=-1)&&((szLineRead.Find("gpiomode2")<2))))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nData);
//			MIUSetGPIOMode(gDeviceIndex, 2, nData);		
//		}
//		else if(((szLineRead.Find("GPIOMODE3")!=-1)&&((szLineRead.Find("GPIOMODE3")<2)))||((szLineRead.Find("gpiomode3")!=-1)&&((szLineRead.Find("gpiomode3")<2))))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nData);
//			MIUSetGPIOMode(gDeviceIndex, 3, nData);		
//		}
//		else if(((szLineRead.Find("BULK")!=-1)&&((szLineRead.Find("BULK")<2)))||((szLineRead.Find("Bulk")!=-1)&&((szLineRead.Find("Bulk")<2))))
//		{
//			unsigned int	nCount; 
//			unsigned char	pData[256];
//			CString	 strInput;
//
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nCount);
//
//			switch(nCount)
//			{
//				case 1:
//					sscanf(szLineRead.GetBuffer(0),"%s%d %x",szTemp,&nCount, pData);
//					break;
//				case 2:
//					sscanf(szLineRead.GetBuffer(0),"%s%d %x %x",szTemp,&nCount, &pData[0], &pData[1]);
//					break;
//				case 3:
//					sscanf(szLineRead.GetBuffer(0),"%s%d %x %x %x",szTemp,&nCount, &pData[0], &pData[1], &pData[2]);
//					break;
//				case 4:
//					sscanf(szLineRead.GetBuffer(0),"%s%d %x %x %x %x",szTemp,&nCount, &pData[0], &pData[1], &pData[2], &pData[3]);
//					break;
//				case 5:
//					sscanf(szLineRead.GetBuffer(0),"%s%d %x %x %x %x %x",szTemp,&nCount, &pData[0], &pData[1], &pData[2], &pData[3], &pData[4]);
//					break;
//				default:
//					break;
//			}
//			if(fInitMode == 2)
//			{
//				errorCode = MIUI2CBurstWrite(gDeviceIndex, gMIUDevice.InitialValue.IICDeviceID, pData[0], 1, &pData[1], nCount-1);
//				if(errorCode)
//				{
//					errorCount++;
//					TRACE("Error Code : 0x%x\n", errorCode);
//				}
//			}
//
//		}
//		else if(((szLineRead.Find("BITFIELD")!=-1)&&((szLineRead.Find("BITFIELD")<2)))||((szLineRead.Find("Bitfield")!=-1)&&((szLineRead.Find("Bitfield")<2))))
//		{
//			unsigned int	bitfield; 
//			unsigned int    ReadValue;
//			unsigned char	value;
//			CString	 strInput;
//			sscanf(szLineRead.GetBuffer(0),"%s %x %x %d",nAddress,&bitfield, &value);
//			if(fInitMode == 1)
//			{				
//				MIUReadRegister(gDeviceIndex, REG_MIU, (unsigned short) nAddress, (unsigned short*) &ReadValue);
//			}
//			else if(fInitMode == 2)
//			{
//				MIUReadRegister(gDeviceIndex, REG_IIC1, (unsigned short) nAddress, (unsigned short*) &ReadValue);
//			}
//			
//			if(value == 0)
//			{
//				ReadValue &= bitfield ^ 0xFFFFFFFF;
//			}
//			else
//			{
//				ReadValue |= bitfield;
//			}
//
//			if(fInitMode == 1)
//			{
//				TRACE("MIU Addr %x, DAta %x \n", nAddress, ReadValue);
//				MIUWriteRegister(gDeviceIndex, REG_MIU, (unsigned short) nAddress, (unsigned short) ReadValue);
//			}
//			else if(fInitMode == 2)
//			{
//				TRACE("Sensor Addr %x, DAta %x \n", nAddress, ReadValue);
//				MIUWriteRegister(gDeviceIndex, REG_IIC1, (unsigned short) nAddress, (unsigned short) ReadValue);
//				Sleep(1);
//			}		
//		}
//		else if(((szLineRead.Find("0x")!=-1)||(szLineRead.Find("0X")!=-1))&&(szLineRead.Find("0")==0))
//		{
//			sscanf_s(szLineRead.GetBuffer(0),"%x%x",&nAddress,&nData);
//            
//			if(fInitMode == 1)
//			{				
//				TRACE("MIU Addr %x, DAta %x \n", nAddress, nData);
//				errorCode = MIUWriteRegister(gDeviceIndex, REG_MIU, (unsigned short) nAddress, (unsigned short) nData);
//				if(errorCode)
//				{
//					errorCount++;
//					TRACE("Error Code : 0x%x\n", errorCode);
//				}
//			}
//			else if(fInitMode == 2)
//			{
//				// 0722
//				if(m_Burst == TRUE &&  (nAddress == 0x0F12  || nAddress == 0x0F14) )
//				{
//					if (BurstWriteLength == 0)
//					{
//						memset(BurstWriteData, 0, sizeof(BurstWriteData));
//
//						BurstWriteData[0] = nData >> 8 ;
//						BurstWriteData[1] = nData & 0xFF ;
//
//						BurstWriteLength = 2;
//						BurstWriteLengthaddr = 2;
//					}
//					else
//					{
//						BurstWriteData[BurstWriteLength] = nData >> 8 ;
//						BurstWriteData[BurstWriteLength+1] = nData & 0xFF ;
//						BurstWriteLength += 2;
//					}
//				
//					if (BurstWriteLength == 2046)
//					{						
//						errorCode = MIUI2CBurstWrite(gDeviceIndex, gMIUDevice.InitialValue.IICDeviceID, (unsigned short)nAddress, BurstWriteLengthaddr, BurstWriteData, BurstWriteLength);
//
//						if(errorCode) 
//						{
//							 errorCount++;
//							 TRACE("Burst Write Fail Error Code : 0x%x\n", errorCode);
//						 }
//						else
//						{
//							TRACE("Burst Write Ok  : %d Byte \n", BurstWriteLength);
//						}
//						BurstWriteLength = 0;
//						BurstWriteLengthaddr = 0;
//					}				
//				}
//				else
//				{
//					if (BurstWriteLength > 0 )
//					{						
//						if (BurstWriteLength == 2)
//						{
//							data =   (unsigned short) (BurstWriteData[0] << 8) + BurstWriteData[1];
//							TRACE("Sensor Addr %x, DAta %x \n", nAddress, data);
//							errorCode = MIUWriteRegister(gDeviceIndex, REG_IIC1, (unsigned short)nAddress, (unsigned short) data);
//							if(errorCode)
//							{
//								errorCount++;
//								TRACE("Error Code : 0x%x\n", errorCode);
//							}
//						}
//						else
//						{						
//							errorCode = MIUI2CBurstWrite(gDeviceIndex, gMIUDevice.InitialValue.IICDeviceID, (unsigned short)nAddress, BurstWriteLengthaddr, BurstWriteData, BurstWriteLength);
//							if(errorCode) 
//							{
//								 errorCount++;
//								 TRACE("Burst Write Fail Error Code : 0x%x\n", errorCode);
//							 }
//							else
//							{
//								TRACE("Burst Write Ok  : %d Byte \n", BurstWriteLength);
//							}
//							//Sleep(400);
//						}
//
//						if(bustSleep > 0)
//						{
//							////////////////////////// 20130711 TRACE �߰�
//							TRACE("burst Sleep = %d \n",bustSleep);
//							Sleep(bustSleep); 
//							bustSleep = 0;
//						}
//
//						BurstWriteLength = 0;
//						BurstWriteLengthaddr = 0;
//						//Sleep(100);
//					}
//
//					TRACE("Sensor Addr %x, DAta %x \n", nAddress, nData);
//					errorCode = MIUWriteRegister(gDeviceIndex, REG_IIC1, (unsigned short) nAddress, (unsigned short) nData);
//					if(errorCode)
//					{
//						errorCount++;
//						TRACE("Error Code : 0x%x\n", errorCode);
//					}
//				}
//			}					
//		}
//		else if(((szLineRead.Find("I2C_MODE")!=-1)||(szLineRead.Find("I2C_Mode")!=-1))&&(szLineRead.Find("I")==0))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%d",szTemp,&nData);
//			MIUSetI2CMode(gDeviceIndex, nData);
//			Sleep(100);
//		}
//		else if((szLineRead.Find("I2CID")!=-1)&&(szLineRead.Find("I")==0))
//		{
//			sscanf(szLineRead.GetBuffer(0),"%s%x",szTemp,&nData);
//			MIUSetI2CID(gDeviceIndex, nData);
//		}
//	}//! while
//
//#ifdef USE_MIU_OPEN_TACT
//	timeChecker.Measure_Time(iCnt_Time);
//	asTime[iCnt_Time].Format("While�� ���� - %.1f sec.", timeChecker.m_adTime[iCnt_Time] - timeChecker.m_adTime[iCnt_Time-1] );
//	iCnt_Time++;
//#endif
//
//
//
//	//! Registry ó�� �� ���� �߻�
//	if ( bFlag_Error == true )
//	{
//		//	return false;
//	}
//
//	// 0722
//	if (BurstWriteLength > 0 )
//	{		
//		if (BurstWriteLength == 2)
//		{
//			data =   (unsigned short) (BurstWriteData[0] << 8) + BurstWriteData[1];
//			TRACE("Sensor Addr %x, DAta %x \n", nAddress, data);
//			errorCode = MIUWriteRegister(gDeviceIndex, REG_IIC1, (unsigned short)nAddress, (unsigned short) data);
//			if(errorCode)
//			{
//				errorCount++;
//				TRACE("Error Code : 0x%x\n", errorCode);
//			}
//		}
//		else
//		{		
//			errorCode = MIUI2CBurstWrite(gDeviceIndex, gMIUDevice.InitialValue.IICDeviceID, (unsigned short)nAddress, BurstWriteLengthaddr, BurstWriteData, BurstWriteLength);
//			if(errorCode) 
//			{
//				 errorCount++;
//				 TRACE("Burst Write Fail Error Code : 0x%x\n", errorCode);
//			 }
//			else
//			{
//				TRACE("Burst Write Ok  : %d Byte \n", BurstWriteLength);
//			}
//		}
//
//		////////////////////////// 20130711 �߰�
//		if(bustSleep > 0)
//		{
//			TRACE("burst Sleep = %d \n",bustSleep);
//			Sleep(bustSleep); 
//			bustSleep = 0;
//			
//		}
//
//		BurstWriteLength = 0;
//		BurstWriteLengthaddr = 0;
//		//Sleep(100);
//	}
//
//#ifdef USE_MIU_OPEN_TACT
//	timeChecker.Measure_Time(iCnt_Time);
//	asTime[iCnt_Time].Format("CCD Init �Ϸ� - %.1f sec.", timeChecker.m_adTime[iCnt_Time] - timeChecker.m_adTime[iCnt_Time-1] );
//	iCnt_Time++;
//#endif
//	 
//
//	//! Registry ó�� �� ���� �߻�
//	if ( bFlag_Error == true )
//	{
//		//	return false;
//	}
//
//	//! CCD Sensor�� �ʱ�ȭ �Ϸ�
////	gMIUDevice.CurrentState = 1;
//	if(errorCount != 0)
//	{
//		CString strErrCount;
//		strErrCount.Format("I2C & Register Error Count : %d", errorCount);
//		errMsg2(Task.AutoFlag, strErrCount);	
//		return false;
//	}
//	logStr.Format("[MIU] CCD_Init �Ϸ�.");
//	MiuLogSave(logStr);
//
	return true;
}
bool CMIU::CcmInit()
{
	int iUnit=0;
	CString logStr;
	logStr.Format("[MIU] CcmInit ����.");
	MiuLogSave(logStr);

	sLogMessage = "";
	//! ���� ������ ��ȿ�� �˻�
	if ( m_iDeviceCount <= 0 )
	{
		return false;
	}
	if ( iUnit < 0 /*|| iUnit >= m_iDeviceCount*/ )
	{
		return false;
	}
	if ( m_pSID == NULL || m_iSensorListCnt <= 0 )
	{
		return false;
	}	
	if ( m_iSensor < 0 || m_iSensor >= m_iSensorListCnt )
	{
		return false;
	}
	if ( m_pSID[m_iSensor].pInfo == NULL || m_pSID[m_iSensor].nInfoCount <= 0 )
	{
		return false;
	}
	if ( m_iSensorType < 0 || m_iSensorType >= m_pSID[m_iSensor].nInfoCount )
	{
		return false;
	}

	if ( gMIUDevice.bMIUOpen != 1 )
	{
		sLogMessage.Format(_T("LPMC ������ Ȯ���ϼ���."));
		return false;
	}

	bool bRet = false;
	bool bFlag_Grab_Stop = false;
	if ( gMIUDevice.CurrentState > 1 )
	{
		bFlag_Grab_Stop = true;

		//! ���� ��� ����
		//! [���� ����] ���� ��游 �����ؾ��Ѵ�. CCD�� �������� ���� �ȵȴ�.
		bRet = Stop( false);
	}

/////////////////////////////////////////////////
	//! Frame Grabber�� �ʱ�ȭ �Ѵ�.
	bRet = MIU_Init();
	if ( bRet == false )
	{
		return false;
	}	
	
	Sleep(10);	
// 	//! CCD Camera Sensor�� �ʱ�ȭ �Ѵ�.
	//====================================================================================================================CCD_Init
	bRet = CCD_Init();
	if ( bRet == false )
	{
		return false;
	}
////////////////////////////////////////////////
	//! ���� ����� ���� �����Ѵ�. 
	if ( bFlag_Grab_Stop == true )
	{
		bRet = Start();
	}
	
	logStr.Format("[MIU] CcmInit �Ϸ�.");
	MiuLogSave(logStr);

	return true;
}

//! ���� ��� ����
//! �̹� ���� ����� ���۵� ���¶��, ������ Display ����
bool CMIU::Start()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString logStr;
	logStr.Format("MIU Start ����.");
	theApp.MainDlg->putListLog(logStr);
	MiuLogSave(logStr);

#ifdef USE_MIU_OPEN_TACT
	bMiuLiveFrameFlag = false;
#endif
	if ( m_iDeviceCount <= 0 )
	{
		return false;
	}
	if(!m_pBoard->IsConnected())
	{
		logStr.Format("[CCD] Grabber Board ���� ����");
		pFrame->putListLog(logStr);
		if(!Open())					// 85 msec.
		{
			pFrame->putListLog("	MIU Open ����.");
			return false;
		}
		Sleep(500);
	}
	if ( gMIUDevice.CurrentState == 0 )
	{
		sLogMessage = _T("ī�޶� ��� �ʱ�ȭ�� ���� �մϴ�.");

#ifdef USE_MIU_OPEN_TACT
		timeChecker.Measure_Time(iCnt_Time);
		asTime[iCnt_Time].Format("���� CCM_Init(iUnit) - %.1f sec.", timeChecker.m_adTime[iCnt_Time] - timeChecker.m_adTime[iCnt_Time-1] );
		iCnt_Time++;
#endif

	}

#ifdef USE_MIU_OPEN_TACT
	timeChecker.Measure_Time(iCnt_Time);
	asTime[iCnt_Time].Format("���� Init_Display_Count() - %.1f sec.", timeChecker.m_adTime[iCnt_Time] - timeChecker.m_adTime[iCnt_Time-1] );
	iCnt_Time++;
#endif
	if ( gMIUDevice.CurrentState < 2 )
	{ 		
		logStr.Format("[MIU] Start Buffer Alloc. [State : %d] ", gMIUDevice.CurrentState);
		MiuLogSave(logStr);
		
#ifdef USE_MIU_OPEN_TACT
		timeChecker.Measure_Time(iCnt_Time);
		asTime[iCnt_Time].Format("���� cvBufferAlloc(iUnit) - %.1f sec.", timeChecker.m_adTime[iCnt_Time] - timeChecker.m_adTime[iCnt_Time-1] );
		iCnt_Time++;
#endif

#ifdef USE_MIU_OPEN_TACT
		timeChecker.Measure_Time(iCnt_Time);
		asTime[iCnt_Time].Format("���� Start_Index_Grab(iUnit) - %.1f sec.", timeChecker.m_adTime[iCnt_Time] - timeChecker.m_adTime[iCnt_Time-1] );
		iCnt_Time++;
#endif

		Start_Index_Grab();

		//int iErrorCode = MIU_OK;
		
		//! Max Size�� ��� ����. 
#ifdef USE_MIU_OPEN_TACT
		timeChecker.Measure_Time(iCnt_Time);
		asTime[iCnt_Time].Format("���� MIUInitializeImageValue(iUnit - %.1f sec.", timeChecker.m_adTime[iCnt_Time] - timeChecker.m_adTime[iCnt_Time-1] );
		iCnt_Time++;
#endif

#ifdef USE_MIU_OPEN_TACT
		timeChecker.Measure_Time(iCnt_Time);
		asTime[iCnt_Time].Format("���� MIUStart(iUnit) - %.1f sec.", timeChecker.m_adTime[iCnt_Time] - timeChecker.m_adTime[iCnt_Time-1] );
		iCnt_Time++;
#endif
		//! ���� ��� ����
		//iErrorCode = MIUStart(gDeviceIndex);
		if(m_pBoard->GrabStart()==BOARD_SUCCESS)
		{
			logStr.Format("[MIU] Start �Ϸ�.");
			MiuLogSave(logStr);
		}else
		{
			return false;
		}

	}

#ifdef USE_MIU_OPEN_TACT
	bMiuLiveFrameFlag = true;
	timeChecker.Measure_Time(iCnt_Time);
	asTime[iCnt_Time].Format("MIUStart �Ϸ� - %.1f sec.", timeChecker.m_adTime[iCnt_Time] - timeChecker.m_adTime[iCnt_Time-1] );
	iCnt_Time++;
#endif
	return true;
}
//! [���� ����] 
//!		bFlag_PowerOff�� true�̸� CCD Sensor�� ������ ����. 
//!		�̰��� �� ���������� ���ǵ��� ���� �ؾ� �Ѵ�. 
//!		�ֳ��ϸ�, ������ ����, �ʱ�ȭ�� �ٽ� �ؾ߸� ������ �ٽ� �� �� �ִ�.
//!		(�ʱ�ȭ�Ϸ���, CMIU::CcmInit �Լ� �ٽ� ȣ���ؾ� ��)
bool CMIU::Stop(bool fi_bFlag_PowerOff)
{

	if (m_pBoard->IsGrabStarted())//if ( gMIUDevice.CurrentState >= 2 )
	{
		m_pBoard->GrabStop();
	}
	
	//! Flag�� �ʱ�ȭ
	gMIUDevice.bMIUOpen = 0;
	gMIUDevice.CurrentState = 0;

	//! ���� ��� Thread�� �۵��� ���߰� �Ѵ�. 
	Init_Index_Grab();

	if(theApp.MainDlg->m_bMiuRun)
	{
		theApp.MainDlg->m_bMiuRun=false;
	}
	theApp.MainDlg->m_labelUsbLive.SetBkColor(M_COLOR_RED);
	theApp.MainDlg->m_labelUsbLive.Invalidate();
	return true;
}

//! Display Frame Rate ������ �Լ�
//! [���� ����] �ݵ��, Display ���Ŀ� ȣ���Ѵ�. (Display�� ���� ���� ��쿡�� ���� ��� ���� ���� �ܰ迡�� ȣ���Ѵ�.)
void CMIU::Add_Display_Count()
{
	(m_strDisplayDiag.iCnt_Display)++;
	
	double dElapsed = m_aMeasureTimeForDisplayFrameRate.Measure_Time(0);

	double dDuration = fabs(dElapsed - m_strDisplayDiag.dGrabStart);

	if ( dDuration > 0.001 )
	{
		m_fRate_Display = ((float)(m_strDisplayDiag.iCnt_Display)) / (float)dDuration;
	}
	else
	{
		m_fRate_Display = 0.f;
	}

	if ( m_strDisplayDiag.iCnt_Display > 50 )
	{
		m_strDisplayDiag.iCnt_Display = 0;
		m_strDisplayDiag.dGrabStart = m_aMeasureTimeForDisplayFrameRate.Measure_Time(0);
	}
}

//! Display Frame Rate �ʱ�ȭ
void CMIU::Init_Display_Count()
{
	m_aMeasureTimeForDisplayFrameRate.Init_Time();
	m_aMeasureTimeForDisplayFrameRate.Start_Time();	
	m_strDisplayDiag.dGrabStart = m_aMeasureTimeForDisplayFrameRate.Measure_Time(0);

	m_strDisplayDiag.iCnt_Display = 0;

	m_fRate_Display = 0.f;

	fGrapStatTickCount=GetTickCount();
}


int CMIU::OsTest_PinMapping(unsigned char ucSensorMode, float* TestResult)
{
	float tempData[OS_PIN_COUNT];

	memcpy(tempData,  TestResult, sizeof(tempData));
	memset(TestResult,         0, sizeof(tempData));
	
	if ( ucSensorMode == 1) // Parallel
	{
		TestResult[0] = tempData[48];
		TestResult[1] = tempData[49];
		TestResult[2] = tempData[50];
		TestResult[3] = tempData[51];
		TestResult[4] = tempData[53];
		TestResult[5] = tempData[54];
		TestResult[6] = tempData[4];
		TestResult[7] = tempData[16];
		TestResult[8] = tempData[1];
		TestResult[9] = tempData[15];
		TestResult[10] = tempData[53];
		TestResult[11] = tempData[54];
		TestResult[12] = tempData[21];
		TestResult[13] = tempData[36];
		TestResult[14] = tempData[27];
		TestResult[15] = tempData[28];
		TestResult[16] = tempData[20];
		TestResult[17] = tempData[43];
		TestResult[18] = tempData[31];
		TestResult[19] = tempData[45];
		TestResult[20] = tempData[33];
		TestResult[21] = tempData[34];
		TestResult[22] = tempData[30];
		TestResult[23] = tempData[32];
		TestResult[24] = tempData[37];
		TestResult[25] = tempData[38];
		TestResult[26] = tempData[39];
		TestResult[27] = tempData[40];
		TestResult[28] = tempData[22];
		TestResult[29] = tempData[42];
		TestResult[30] = tempData[53];
		TestResult[31] = tempData[54];
		TestResult[32] = tempData[0];
		TestResult[33] = tempData[24];
		TestResult[34] = tempData[5];
		TestResult[35] = tempData[8];
		TestResult[36] = tempData[44];
		TestResult[37] = tempData[6];
		TestResult[38] = tempData[52];

		TestResult[39] = 0; // Pin 40 Not Test
		// SPI
		TestResult[40] = tempData[7];
		TestResult[41] = tempData[19];
		TestResult[42] = tempData[9];
		TestResult[43] = tempData[18];
	}
	else //Mipi
	{
		TestResult[0] = tempData[48];
		TestResult[1] = tempData[49];
		TestResult[2] = tempData[50];
		TestResult[3] = tempData[51];
		TestResult[4] = tempData[53];
		TestResult[5] = tempData[54];
		TestResult[6] = tempData[4];
		TestResult[7] = tempData[13];
		TestResult[8] = tempData[1];
		TestResult[9] = tempData[41];
		TestResult[10] = tempData[53];
		TestResult[11] = tempData[54];
		TestResult[12] = tempData[16];
		TestResult[13] = tempData[25];
		TestResult[14] = tempData[15];
		TestResult[15] = tempData[26];
		TestResult[16] = tempData[53];
		TestResult[17] = tempData[54];
		TestResult[18] = tempData[2];
		TestResult[19] = tempData[35];
		TestResult[20] = tempData[3];
		TestResult[21] = tempData[10];
		TestResult[22] = tempData[53];
		TestResult[23] = tempData[54];
		TestResult[24] = tempData[17];
		TestResult[25] = tempData[29];
		TestResult[26] = tempData[14];
		TestResult[27] = tempData[23];
		TestResult[28] = tempData[53];
		TestResult[29] = tempData[54];
		TestResult[30] = tempData[53];
		TestResult[31] = tempData[12];
		TestResult[32] = tempData[53];
		TestResult[33] = tempData[11];
		TestResult[34] = tempData[0];
		TestResult[35] = tempData[54];
		TestResult[36] = tempData[53];
		TestResult[37] = tempData[54];
		TestResult[38] = tempData[52];

		TestResult[39] = 0; // Pin 40 Not Test

		// SPI
		TestResult[40] = tempData[7];
		TestResult[41] = tempData[19];
		TestResult[42] = tempData[9];
		TestResult[43] = tempData[18];
	}

	return 1;//MIU_OK;
}

//! Color ��ȯ�� ���
//! *   Shift10BitMode : Test �� -> 10 bit �� 8bit�� shift  
//!	*
//!	*   @param      pImage, [in] Image Pointer
//!	*   @param      nWidth, [in] Image Width
//!	*	@param		nHeight, [in] Image Height  
void CMIU::Shift10BitMode(unsigned char* pImage, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight)
{
	unsigned int i,j,k;
	unsigned int nByteWidth;
	nByteWidth = nWidth * 5 / 4 ;

	unsigned int *piDest;
	piDest = (unsigned int *)pDest;

	nWidth >>= 2;

	for(i=0; i<nHeight; i++)
	{
		for(j=0, k=0; j<nWidth; j++, k+=5)
		{
			piDest[j] = (pImage[k+3]<<24) +  (pImage[k+2]<<16) +  (pImage[k+1]<<8) +  (pImage[k]);
		}

		piDest += nWidth;
		pImage += nByteWidth;
	}
}

//! Color ��ȯ�� ���
//! *   Shift12BitMode : Test �� -> 12 bit �� 8bit�� shift  
//!	*
//!	*   @param      pImage, [in] Image Pointer
//!	*   @param      nWidth, [in] Image Width
//!	*	@param		nHeight, [in] Image Height 
void CMIU::Shift12BitMode(unsigned char* pImage, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight)
{
	unsigned int	i,j,k;
	unsigned int	nByteWidth;
	unsigned short *psDest;

	nByteWidth = nWidth * 3 / 2 ;
	psDest = (unsigned short *)pDest;
	nWidth >>= 1;
	for(i=0; i<nHeight; i++)
	{
		for(j=0, k = 0; j<nWidth; j++,k+=3)
		{
			psDest[j] =  (pImage[k+1]<<8) +  (pImage[k]);			
		}
		psDest += nWidth;
		pImage += nByteWidth;
	}
}

//! Color ��ȯ�� ���
void CMIU::CopyBuffer(unsigned char* pImage, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight)
{
	unsigned int	i,j;

	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			pDest[j] =  pImage[j];			
		}
		pDest += nWidth;
		pImage += nWidth;
	}
}

//! CCD Sensor�� ����
//! IMX135-0APH5-L CMOS, Sony, 2013/3/13
//! ���� Gain ����
bool CMIU::SetRegister_WhiteBalance_Red_IMX135(CString fi_sGain)
{
	if ( m_iDeviceCount <= 0 )
	{
		return false;
	}
	if ( m_iIndexDevice < 0 )
	{
		return false;
	}
	if ( gMIUDevice.bMIUOpen != 1 )
	{
		sLogMessage.Format(_T("LPMC Check Connect"));
		return false;
	}
	if ( fi_sGain.GetLength() <= 0 )
	{
		return false;
	}

	//! �Է°��� 2���� 16���� ���ڷ� ��ȯ�Ѵ�. 
	int iInteger = 0;
	double dDecimalPoint = 0;

	GetInteger_From_Double(fi_sGain, iInteger, dDecimalPoint);

	int iDecimalPoint = (int)(dDecimalPoint * 256.);

	long lGain_MSB = Convert_Decimal_To_Hex(iInteger);
	long lGain_LSB = Convert_Decimal_To_Hex(iDecimalPoint);

	int iErrorCode = 0;

	CString sAddress1 = _T("0x0210"), sAddress2 = _T("0x0211");

	long lAddress1 = HexString_to_Num(sAddress1);
	long lAddress2 = HexString_to_Num(sAddress2);

	//iErrorCode = MIUWriteRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress1, (unsigned short)lGain_MSB);
	//if ( iErrorCode != MIU_OK )
	//{
	//	return false;
	//}
	//iErrorCode = MIUWriteRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress2, (unsigned short)lGain_LSB);
	//if ( iErrorCode != MIU_OK )
	//{
	//	return false;
	//}
	
	return true;
}

bool CMIU::SetRegister_WhiteBalance_Green_IMX135(CString fi_sGain)
{
	if ( m_iDeviceCount <= 0 )
	{
		return false;
	}
	if ( m_iIndexDevice < 0 )
	{
		return false;
	}
	if ( gMIUDevice.bMIUOpen != 1 )
	{
		sLogMessage.Format(_T("LPMC Check Connect"));
		return false;
	}
	if ( fi_sGain.GetLength() <= 0 )
	{
		return false;
	}

	//! �Է°��� 2���� 16���� ���ڷ� ��ȯ�Ѵ�. 
	int iInteger = 0;
	double dDecimalPoint = 0;

	GetInteger_From_Double(fi_sGain, iInteger, dDecimalPoint);

	int iDecimalPoint = (int)(dDecimalPoint * 256.);

	long lGain_MSB = Convert_Decimal_To_Hex(iInteger);
	long lGain_LSB = Convert_Decimal_To_Hex(iDecimalPoint);

	int iErrorCode = 0;

	//! Gr
	CString sAddress1 = _T("0x020E"), sAddress2 = _T("0x020F");

	long lAddress1 = HexString_to_Num(sAddress1);
	long lAddress2 = HexString_to_Num(sAddress2);

	/*iErrorCode = MIUWriteRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress1, (unsigned short)lGain_MSB);
	if ( iErrorCode != MIU_OK )
	{
		return false;
	}
	iErrorCode = MIUWriteRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress2, (unsigned short)lGain_LSB);
	if ( iErrorCode != MIU_OK )
	{
		return false;
	}*/

	//! Gb
	sAddress1 = _T("0x0214"), sAddress2 = _T("0x0215");

	lAddress1 = HexString_to_Num(sAddress1);
	lAddress2 = HexString_to_Num(sAddress2);

	/*iErrorCode = MIUWriteRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress1, (unsigned short)lGain_MSB);
	if ( iErrorCode != MIU_OK )
	{
		return false;
	}
	iErrorCode = MIUWriteRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress2, (unsigned short)lGain_LSB);
	if ( iErrorCode != MIU_OK )
	{
		return false;
	}*/

	return true;
}

bool CMIU::SetRegister_WhiteBalance_Blue_IMX135(CString fi_sGain)
{
	if ( m_iDeviceCount <= 0 )
	{
		return false;
	}
	if ( m_iIndexDevice < 0 )
	{
		return false;
	}
	if ( gMIUDevice.bMIUOpen != 1 )
	{
		sLogMessage.Format(_T("LPMC Check Connect"));
		return false;
	}
	if ( fi_sGain.GetLength() <= 0 )
	{
		return false;
	}

	//! �Է°��� 2���� 16���� ���ڷ� ��ȯ�Ѵ�. 
	int iInteger = 0;
	double dDecimalPoint = 0;

	GetInteger_From_Double(fi_sGain, iInteger, dDecimalPoint);

	int iDecimalPoint = (int)(dDecimalPoint * 256.);

	long lGain_MSB = Convert_Decimal_To_Hex(iInteger);
	long lGain_LSB = Convert_Decimal_To_Hex(iDecimalPoint);

	int iErrorCode = 0;

	CString sAddress1 = _T("0x0212"), sAddress2 = _T("0x0213");

	long lAddress1 = HexString_to_Num(sAddress1);
	long lAddress2 = HexString_to_Num(sAddress2);

	/*iErrorCode = MIUWriteRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress1, (unsigned short)lGain_MSB);
	if ( iErrorCode != MIU_OK )
	{
		return false;
	}
	iErrorCode = MIUWriteRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress2, (unsigned short)lGain_LSB);
	if ( iErrorCode != MIU_OK )
	{
		return false;
	}*/

	return true;
}

bool CMIU::SetRegister_WhiteBalance_Red_IMX135(double fi_dGain)
{
	CString sInput = _T("");
	sInput.Format(_T("%g"), fi_dGain);
	bool bRet = SetRegister_WhiteBalance_Red_IMX135(sInput);
	return bRet;
}

bool CMIU::SetRegister_WhiteBalance_Green_IMX135(double fi_dGain)
{
	CString sInput = _T("");
	sInput.Format(_T("%g"), fi_dGain);
	bool bRet = SetRegister_WhiteBalance_Green_IMX135(sInput);
	return bRet;
}

bool CMIU::SetRegister_WhiteBalance_Blue_IMX135(double fi_dGain)
{
	CString sInput = _T("");
	sInput.Format(_T("%g"), fi_dGain);
	bool bRet = SetRegister_WhiteBalance_Blue_IMX135(sInput);
	return bRet;
}

//! Exposure Time ���� (Added by LHW, 2013/3/14)
//! CCD Sensor�� ���� ���� ������ �����´�.
bool CMIU::GetCurrentInfo_Exposure_From_CCD_IMX135()
{
	return true;
	//if ( m_iDeviceCount <= 0 )
	//{
	//	return false;
	//}
	//if ( m_iIndexDevice < 0 )
	//{
	//	return false;
	//}
	//if ( gMIUDevice.bMIUOpen != 1 )
	//{
	//	sLogMessage.Format(_T("LPMC Check Connect"));
	//	return false;
	//}

	//bool bRet = true;

	//int iErrorCode = 0;
	//CString sAddress1 = _T(""), sAddress2 = _T("");
	//long lAddress1 = 0, lAddress2 = 0;
	//int iData1 = 0, iData2 = 0, iData = 0;

	////unsigned short iPrePLLClk_OP_Div;	//! Register Address : 0x0305
	////unsigned short iPLL_OP_MPY;		//! Register Address : 0x030C, 0x030D
	////unsigned short iVTSYCK_Div;		//! Register Address : 0x0303
	////unsigned short iVTPXCK_Div;		//! Register Address : 0x0301

	////unsigned short iFrame_Length;		//! Register Address : 0x0340, 0x0341
	////unsigned short iLine_Length;		//! Register Address : 0x0342, 0x0343

	////unsigned short iIntegrationTime_Coarse;	//! Register Address : 0x0202, 0x0203
	////unsigned short iIntegrationTime_Fine;		//! Register Address : 0x0200, 0x0201 (Read Only)

	//sAddress1 = _T("0x0305");	
	//lAddress1 = HexString_to_Num(sAddress1);	
	//iErrorCode = MIUReadRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress1, (unsigned short*)&iData1);
	//if ( iData1 <= 0 )
	//{
	//	bRet = false;
	//	iData1 = 1;
	//}
	//gMIUDevice.IMX135.iPrePLLClk_OP_Div = (unsigned short)iData1;
	//
	//sAddress1 = _T("0x030C");
	//sAddress2 = _T("0x030D");
	//lAddress1 = HexString_to_Num(sAddress1);
	//lAddress2 = HexString_to_Num(sAddress2);
	//iErrorCode = MIUReadRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress1, (unsigned short*)&iData1);
	//iErrorCode = MIUReadRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress2, (unsigned short*)&iData2);
	//Merge_Integer_From_Hex_8bit(iData1, iData2, iData);
	//if ( iData <= 0 )
	//{
	//	bRet = false;
	//	iData = 1;
	//}
	//gMIUDevice.IMX135.iPLL_OP_MPY = (unsigned short)iData;

	//sAddress1 = _T("0x0303");	
	//lAddress1 = HexString_to_Num(sAddress1);	
	//iErrorCode = MIUReadRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress1, (unsigned short*)&iData1);
	//if ( iData1 <= 0 )
	//{
	//	bRet = false;
	//	iData1 = 1;
	//}
	//gMIUDevice.IMX135.iVTSYCK_Div = (unsigned short)iData1;

	//sAddress1 = _T("0x0301");	
	//lAddress1 = HexString_to_Num(sAddress1);	
	//iErrorCode = MIUReadRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress1, (unsigned short*)&iData1);
	//if ( iData1 <= 0 )
	//{
	//	bRet = false;
	//	iData1 = 1;
	//}
	//gMIUDevice.IMX135.iVTPXCK_Div = (unsigned short)iData1;

	//sAddress1 = _T("0x0340");
	//sAddress2 = _T("0x0341");
	//lAddress1 = HexString_to_Num(sAddress1);
	//lAddress2 = HexString_to_Num(sAddress2);
	//iErrorCode = MIUReadRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress1, (unsigned short*)&iData1);
	//iErrorCode = MIUReadRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress2, (unsigned short*)&iData2);
	//Merge_Integer_From_Hex_8bit(iData1, iData2, iData);
	//if ( iData <= 0 )
	//{
	//	bRet = false;
	//	iData = 1;
	//}
	//gMIUDevice.IMX135.iFrame_Length = (unsigned short)iData;

	//sAddress1 = _T("0x0342");
	//sAddress2 = _T("0x0343");
	//lAddress1 = HexString_to_Num(sAddress1);
	//lAddress2 = HexString_to_Num(sAddress2);
	//iErrorCode = MIUReadRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress1, (unsigned short*)&iData1);
	//iErrorCode = MIUReadRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress2, (unsigned short*)&iData2);
	//Merge_Integer_From_Hex_8bit(iData1, iData2, iData);
	//if ( iData <= 0 )
	//{
	//	bRet = false;
	//	iData = 1;
	//}
	//gMIUDevice.IMX135.iLine_Length = (unsigned short)iData;

	//sAddress1 = _T("0x0202");
	//sAddress2 = _T("0x0203");
	//lAddress1 = HexString_to_Num(sAddress1);
	//lAddress2 = HexString_to_Num(sAddress2);
	//iErrorCode = MIUReadRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress1, (unsigned short*)&iData1);
	//iErrorCode = MIUReadRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress2, (unsigned short*)&iData2);
	//Merge_Integer_From_Hex_8bit(iData1, iData2, iData);
	//if ( iData <= 0 )
	//{
	//	bRet = false;
	//	iData = 1;
	//}
	//gMIUDevice.IMX135.iIntegrationTime_Coarse = (unsigned short)iData;

	//sAddress1 = _T("0x0200");
	//sAddress2 = _T("0x0201");
	//lAddress1 = HexString_to_Num(sAddress1);
	//lAddress2 = HexString_to_Num(sAddress2);
	//iErrorCode = MIUReadRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress1, (unsigned short*)&iData1);
	//iErrorCode = MIUReadRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress2, (unsigned short*)&iData2);
	//Merge_Integer_From_Hex_8bit(iData1, iData2, iData);
	//if ( iData <= 0 )
	//{
	//	bRet = false;
	//	iData = 1;
	//}
	//gMIUDevice.IMX135.iIntegrationTime_Fine = (unsigned short)iData;

	//if ( bRet == false )
	//{
	//	return false;
	//}

	////! CCD Sensor�� �Ŵ����� �����Ͽ� �Ʒ��� ������ �˾Ƴ���.
	////! IMX135-0APH5-L Data Sheet 
	////! Clock System Diagram (Page 25 ~ 28)
	////! Frame Rate Calculation Formula (Page 42)
	////! Electronic shutter and integration time settings (Page 43)

	//double dValue_VTPXCK = 0;
	//dValue_VTPXCK  = gMIUDevice.InitialValue.MCLK * 1000. * 1000.;	//! CCD Sensor�� Main Clock���� �����´�.
	//dValue_VTPXCK /= (double)(gMIUDevice.IMX135.iPrePLLClk_OP_Div);
	//dValue_VTPXCK *= (double)(gMIUDevice.IMX135.iPLL_OP_MPY);
	//dValue_VTPXCK /= ((double)(gMIUDevice.IMX135.iVTSYCK_Div * gMIUDevice.IMX135.iVTPXCK_Div));

	//double dTime_Line = 0.;
	//dTime_Line = ((double)(gMIUDevice.IMX135.iLine_Length)) / (2. * dValue_VTPXCK);

	//double dValue_Temp = 0.;
	//dValue_Temp = ((double)(gMIUDevice.IMX135.iIntegrationTime_Fine)) * 2. / ((double)(gMIUDevice.IMX135.iLine_Length));

	//double dTime_Exposure_Min = 0., dTime_Exposure_Max = 0., dTime_Exposure_Current = 0.;

	//dTime_Exposure_Min     = dTime_Line * (1. + dValue_Temp);
	//dTime_Exposure_Max     = dTime_Line * (((double)(gMIUDevice.IMX135.iFrame_Length - 4)) + dValue_Temp);
	//dTime_Exposure_Current = dTime_Line * (((double)(gMIUDevice.IMX135.iIntegrationTime_Coarse)) + dValue_Temp);

	////! sec ---> usec ���� ��ȯ
	//gMIUDevice.IMX135.iExposureTime_Min     = (int)(dTime_Exposure_Min * 1000. * 1000.);
	//gMIUDevice.IMX135.iExposureTime_Max     = (int)(dTime_Exposure_Max * 1000. * 1000.);
	//gMIUDevice.IMX135.iExposureTime_Current = (int)(dTime_Exposure_Current * 1000. * 1000.);

	////! ������ ��� ����
	//gMIUDevice.IMX135.bFlag_Valid = 1;

	//return bRet;
}
void CMIU::SaveSfrImage(unsigned char* ucImage, int pitch, int sizeX, int sizeY, int index, CRect rcRoi)
{
	TCHAR szPos[SIZE_OF_1K];
	int mSizeX = 0;
	int mSizeY = 0;

	mSizeX = sizeX;
	mSizeY = sizeY;

	MbufAlloc2d(vision.MilSystem, mSizeX, mSizeY, 8 + M_UNSIGNED, M_IMAGE + M_PROC, &vision.MilSfrTargetImage);

	BOOL IsFind;
	CFileFind clFinder;
	TCHAR szPath[SIZE_OF_1K];
	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\%s\\SfrCrop"), BASE_LOG_DIR, model.mCurModelName);
	if (clFinder.FindFile(szPath) == FALSE)
	{
		CreateDirectory(szPath, NULL);
	}

	_stprintf_s(szPos, SIZE_OF_1K, _T("%s\\%s\\SfrCrop\\Crop_%d.bmp"), BASE_LOG_DIR,model.mCurModelName, index);
	MbufChild2d(vision.MilProcImageChild[3], rcRoi.left, rcRoi.top, mSizeX, mSizeY, &vision.MilSfrTargetImage);
	MbufSave(szPos, vision.MilSfrTargetImage);


	//
	//
	//MbufFree(MilSfrTargetImage);

}
//! Exposure Time ���� (Added by LHW, 2013/3/14)
//! fi_iExposureTime : Exposure �ð�, ���� usec
bool CMIU::Set_ExposureTime_To_CCD_IMX135(int fi_iExposureTime)
{
	//if ( m_iDeviceCount <= 0 )
	//{
	//	return false;
	//}
	//if ( m_iIndexDevice < 0 )
	//{
	//	return false;
	//}
	//if ( gMIUDevice.bMIUOpen != 1 )
	//{
	//	sLogMessage.Format(_T("LPMC Check Connect"));
	//	return false;
	//}

	//if ( gMIUDevice.IMX135.bFlag_Valid != 1 )
	//{
	//	sLogMessage.Format(_T("IMX135 Sensor Data Not Used."));
	//	return false;
	//}

	//if ( fi_iExposureTime < gMIUDevice.IMX135.iExposureTime_Min || 
	//	 fi_iExposureTime > gMIUDevice.IMX135.iExposureTime_Max )
	//{
	//	return false;
	//}

	//if ( gMIUDevice.IMX135.iPrePLLClk_OP_Div <= 0 || 
	//	 gMIUDevice.IMX135.iVTSYCK_Div <= 0 || 
	//	 gMIUDevice.IMX135.iVTPXCK_Div <= 0 || 
	//	 gMIUDevice.IMX135.iLine_Length <= 0 )
	//{
	//	return false;
	//}
	//
	////! CCD Sensor�� �Ŵ����� �����Ͽ� �Ʒ��� ������ �˾Ƴ���.
	////! IMX135-0APH5-L Data Sheet 
	////! Clock System Diagram (Page 25 ~ 28)
	////! Frame Rate Calculation Formula (Page 42)
	////! Electronic shutter and integration time settings (Page 43)

	//double dValue_VTPXCK = 0;
	//dValue_VTPXCK  = gMIUDevice.InitialValue.MCLK * 1000. * 1000.;	//! CCD Sensor�� Main Clock���� �����´�.
	//dValue_VTPXCK /= (double)(gMIUDevice.IMX135.iPrePLLClk_OP_Div);
	//dValue_VTPXCK *= (double)(gMIUDevice.IMX135.iPLL_OP_MPY);
	//dValue_VTPXCK /= ((double)(gMIUDevice.IMX135.iVTSYCK_Div * gMIUDevice.IMX135.iVTPXCK_Div));

	//double dTime_Line = 0.;
	//dTime_Line = ((double)(gMIUDevice.IMX135.iLine_Length)) / (2. * dValue_VTPXCK);

	//double dValue_Temp = 0.;
	//dValue_Temp = ((double)(gMIUDevice.IMX135.iIntegrationTime_Fine)) * 2. / ((double)(gMIUDevice.IMX135.iLine_Length));

	//if ( fi_iExposureTime <= ((int)dTime_Line) )
	//{
	//	return false;
	//}

	//double dTime_Coarse = 0.;
	//double dTime_Exposure = ((double)fi_iExposureTime) / (1000. * 1000);	//! ���� ��ȯ, usec ---> sec
	//dTime_Coarse = dTime_Exposure / dTime_Line;
	//dTime_Coarse -= dValue_Temp;

	//int iTime_Coarse = (int)(dTime_Coarse + 0.5);

	//if ( iTime_Coarse < 1 )
	//{
	//	iTime_Coarse = 1;
	//}
	//if ( iTime_Coarse > (gMIUDevice.IMX135.iFrame_Length - 4) )
	//{
	//	iTime_Coarse = gMIUDevice.IMX135.iFrame_Length - 4;
	//}

	//int iErrorCode = 0;
	//CString sAddress1 = _T(""), sAddress2 = _T("");
	//long lAddress1 = 0, lAddress2 = 0;
	//int iData1 = 0, iData2 = 0;

	//Separate_Hex_From_Integer_8bit(iTime_Coarse, iData1, iData2);

	////! COARSE_INTEG_TIME ����

	//sAddress1 = _T("0x0202");
	//sAddress2 = _T("0x0203");

	//lAddress1 = HexString_to_Num(sAddress1);
	//lAddress2 = HexString_to_Num(sAddress2);

	//iErrorCode = MIUWriteRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress1, (unsigned short)iData1);
	//if ( iErrorCode != MIU_OK )
	//{
	//	return false;
	//}
	//iErrorCode = MIUWriteRegister(m_iIndexDevice, REG_IIC1, (unsigned short)lAddress2, (unsigned short)iData2);
	//if ( iErrorCode != MIU_OK )
	//{
	//	return false;
	//}

	//gMIUDevice.IMX135.iExposureTime_Current = fi_iExposureTime;

	return true;
}

//! OIS (VCM) ��� �غ�
//! [���� ����] �ݵ��, Frame Grabber�� Open, Initialize �� �ڿ� ����ؾ� �Ѵ�. (Ŭ���� ���� Open, MIU_Init �Լ� ȣ�� �� ��� ����)
bool CMIU::ReadyVCM()
{
	if ( m_iDeviceCount <= 0 )
	{
		return false;
	}
	if ( m_iIndexDevice < 0 )
	{
		return false;
	}
	if ( gMIUDevice.bMIUOpen != 1 )
	{
		sLogMessage.Format(_T("LPMC ������ Ȯ���ϼ���."));
		return false;
	}
	if ( gMIUDevice.bMIUInit != 1 )
	{
		sLogMessage.Format(_T("LPMC Frame Grabber�� �ʱ�ȭ �Ͻʽÿ�."));
		return false;
	}

	//MIUSetI2CIDOIS(m_iIndexDevice, 0x0C);	//! IIC Slave Address ���� 7bit
	//										//! 0x0C�� ���� Slave Device Address ��
	//										//! Write = 0x18 , Read =0x19 �̰� ���� ���� 7bit�� ������ �����ϵ��� �Ǿ� ����
	//										//! VCM Driver �Ŵ��� (Ver. 1.93) 8 page ����

	//MIUSetI2CModeOIS(m_iIndexDevice, 0);	//! Address �� Data Length ����
	//										//! 0 : 8bit address, 8bit data, 1 : 8bit address, 16bit data
	//										//! 2 : 16bit address, 8bit data, 3 : 16bit address, 16bit data

	gMIUDevice.bReadyVCM = 1;

	return true;
}

//! OIS, VCM Driver�� ���� �����͸� �Է��Ѵ�. 
bool CMIU::SetValue_VCM_Direct(BYTE fi_Byte1, BYTE fi_Byte2)
{
	if ( m_iDeviceCount <= 0 )
	{
		return false;
	}
	if ( m_iIndexDevice < 0 )
	{
		return false;
	}
	if ( gMIUDevice.bMIUOpen != 1 )
	{
		sLogMessage.Format(_T("LPMC ������ Ȯ���ϼ���."));
		return false;
	}
	if ( gMIUDevice.bMIUInit != 1 )
	{
		sLogMessage.Format(_T("LPMC Frame Grabber�� �ʱ�ȭ �Ͻʽÿ�."));
		return false;
	}
	if ( gMIUDevice.bReadyVCM != 1 )
	{
		sLogMessage.Format(_T("LPMC Frame Grabber���� VCM �κ��� �ʱ�ȭ �Ͻʽÿ�."));
		return false;
	}

	//! MIUWriteOIS(char iDeviceIndex, unsigned short nAddress, unsigned short nData);
	//! MIUWriteOIS �Լ����� nAddress�� BYTE1, nData�� BYTE2 ���� (VCM Driver �Ŵ��� Ver 1.93 page 7 ����)
//	int iRet = MIUWriteOIS(m_iIndexDevice, fi_Byte1, fi_Byte2);

	return true;
}

//! �Է°��� VCM�� Data ���� ������ �ڿ� ��ȯ�Ѵ�. 
void CMIU::Apply_Data_Code_VCM(BYTE fi_Byte1, BYTE fi_Byte2, BYTE &fo_Byte1, BYTE &fo_Byte2, int fi_code)
{
	//! (VCM Driver �Ŵ��� Ver 1.93 page 7�� 8.2 Register Format ����)
	BYTE dataH = 0, dataL = 0;
	dataH = (fi_code>>4)&0x3f;
	dataL = (fi_code<<4)&0xf0;

	//! Data bit���� ���� �ʱ�ȭ�� �Ŀ� ����� bit���� ����
	fo_Byte1 = (fi_Byte1 & 0xc0) | dataH;
	fo_Byte2 = (fi_Byte2 & 0x0f) | dataL;
}

//! �Է°����� VCM�� Data���� �߷�����.
void CMIU::Get_Data_Code_VCM(BYTE fi_Byte1, BYTE fi_Byte2, int &fo_code)
{
	//! (VCM Driver �Ŵ��� Ver 1.93 page 7�� 8.2 Register Format ����)
	BYTE dataH = 0, dataL = 0;

	dataH = fi_Byte1 & 0x3f;
	dataL = fi_Byte2 & 0xf0;

	fo_code = (dataH << 4) | (dataL >> 4);
}

//! OIS, VCM Driver�� D0 ~ D9 bit�� ���� �����͸� �Է��Ѵ�. 
//! [�Է°�]
//!  code : �Է¿�) 0 mA(code = 0), 40 mA (code = 341) , 60 mA (code = 511) 
//!  bFlag_Get : true�̸� ������ VCM �������� �����ͼ� �۾��Ѵ�. 
bool CMIU::MoveVCM_Code(int code)
{
	if ( code < 0 )
	{
		code = 0;
	}
	if ( code >= 1024 )
	{
		code = 1023;
	}
	//MIUSetI2CIDOIS(0, 0x0C);  // IIC Slave Address ���� 7bit
	//MIUSetI2CModeOIS(0, 0);   // Address �� Data Length ����


	BYTE dataH = (code>>4)&0x3f;
	BYTE dataL = (code<<4)&0xf0;

//	MIUWriteOIS(0, dataH, dataL);
	
	return true;
}

//! OIS, VCM Driver�� D0 ~ D9 bit�� ���� �����͸� �Է��Ѵ�.
//! [�Է°�]
//!  current : MoveVCM_Code�ʹ� �޸�, 'mA ����'�� ���������� �Է��Ѵ�.  
bool CMIU::MoveVCM_Current(int current)
{
	//! D[9:0] : Data input
	//! Output current = (D[9:0]/1023) X 120mA
	//! (VCM Driver �Ŵ��� Ver 1.93 page 7 ����)
	int code = (int)(((double)current / 120.) * 1023.);

	bool bRet = MoveVCM_Code(code);
	if ( bRet == false )
	{
		return false;
	}

	return true;
}

void CMIU::ReadRawImg(CString strInFileName,int nSizeX,int nSizeY,int nChannel,IplImage *outImage)
{
    FILE *fp;
    int i,j,k;
    char strFileName_c[256];
    strcpy(strFileName_c, strInFileName.GetBuffer());

	if(fopen_s(&fp,strFileName_c, "rb") ==0 )
	{
		unsigned char *temp;
		temp=(unsigned char *)calloc(sizeof(unsigned char),nSizeX*nSizeY);
		
		for(i=0;i<nSizeX;i++)
		{
			fread(temp,nSizeX*nChannel,sizeof(unsigned char),fp);

			for(j=0;j<nSizeY;j++)
			{
				for(k=0; k<nChannel; k++)
				{
					outImage->imageData[i*nSizeX*nChannel + j*nChannel+k] = temp[j*nChannel+k];
				}
			}
		}
		free(temp);

	}
	fclose(fp);
}

void CMIU::ReadRawImg2(CString strInFileName,int nSizeX,int nSizeY,int nChannel,IplImage *outImage)
{
  //   FILE *fp;
  //   int i,j,k;
  //   size_t size_i;
  //   TCHAR strFileName_tc[256];
  //   char strFileName_c[256];
  //   _tcscpy_s(strFileName_tc, strInFileName.GetLength()+1, strInFileName);
	 //wcstombs_s(&size_i, strFileName_c, (size_t)sizeof(strFileName_c), strFileName_tc, (size_t)_tcslen( strInFileName.GetLength() )+1 );

  //   if( fopen_s( &fp,strFileName_c, "rb" ) ==0 )
  //   {
  //       unsigned char *temp;
  //       temp=(unsigned char *)calloc(sizeof(unsigned char),nSizeX*nSizeY);
  //       for(i=0;i<nSizeX;i++)
  //       {
  //           fread(temp,nSizeX*nChannel,sizeof(unsigned char),fp);
  //           for(j=0;j<nSizeY;j++)
  //               for(k=0;k<nChannel;k++)
  //                   outImage->imageData[i*nSizeX*nChannel + j*nChannel+k]=temp[j*nChannel+k];
  //       }
  //       free(temp);
  //   }
  //   fclose(fp);
 }

int CMIU::Grab_StopImage()//norinda
{
	cvSplit(gMIUDevice.imageItp,  pImageBuf[0], pImageBuf[1], pImageBuf[2], NULL);


	MbufPut(vision.MilGrabImageChild[3], pImageBuf[2]->imageData);
	MbufPut(vision.MilGrabImageChild[4], pImageBuf[1]->imageData);
	MbufPut(vision.MilGrabImageChild[5], pImageBuf[0]->imageData);

	//
	return 1;
}


void CMIU::PRI_12BitToRaw8 ( unsigned char *p12Bit, unsigned char *pRawBit8 )	// 1280 x 964 12bit -> 1280 x 960 8bit
{
	int		x, y;
	int		m_iHeight, m_iWidth;
//	FILE *fp = fopen("h:\\test.dat","wt");
//	FILE *fp1 = fopen("h:\\test_8.dat","wt");


	m_iWidth = m_pSID->pInfo->nOutputWidth ;
	m_iHeight	= m_pSID->pInfo->nOutputHeight;
	
	
	for(y=0 ; y<m_iHeight ; y++)
	{

		
		for(x=0 ; x<m_iWidth/2 ; x=x++)
		{			
			pRawBit8[y*m_iWidth + x*2] = (p12Bit[(y+2)*m_iWidth*2 + 4*x+0]) ;		
			pRawBit8[y*m_iWidth + x*2+1 ] = (p12Bit[(y+2)*m_iWidth*2 + 4*x+2]);	
				

			/*fprintf (fp,"%02x%02x %02x%02x ", p12Bit[y*m_iWidth*2 + 4*x],p12Bit[y*m_iWidth*2 + 4*x+1], p12Bit[y*m_iWidth*2 + 4*x+2],p12Bit[y*m_iWidth*2 + 4*x+3]   );
//			fprintf (fp,"%02x%02x %02x%02x ", p12Bit[y*m_iWidth*2 + 2*x],p12Bit[y*m_iWidth*2 + 2*x+1], p12Bit[y*m_iWidth*2 + 2*x+2],p12Bit[y*m_iWidth*2 + 2*x+3]   );


			fprintf (fp1,"%02x %02x ", pRaw8[y*m_iWidth + x*2], pRaw8[y*m_iWidth + x*2+1 ]);

			if ( pRaw8[y*m_iWidth + x*2] == 0x3f || pRaw8[y*m_iWidth + x*2] == 0x7f || pRaw8[y*m_iWidth + x*2+1] == 0x3f || pRaw8[y*m_iWidth + x*2] == 0x7f  ) {
				printf(" ");
			}*/
		
		}				
			
//		fprintf (fp,"\n");						
//		fprintf (fp1,"\n");			
		
		
		/////////
	}

}




int CMIU::StopLive () {

	DWORD	waitRet ;
	DWORD	dwCode =0 ;

	if ( m_bThreadUsed )
	{
		m_bThreadUsed = false ;	
		Sleep(100) ;
		if(m_hImgViewThread) 
		{		
			waitRet = WaitForSingleObject(m_hImgViewThread, 1000) ;
			if(waitRet == WAIT_TIMEOUT)
			{
				GetExitCodeThread(m_hImgViewThread, &dwCode) ;
				if(dwCode == STILL_ACTIVE)
				{
					TerminateThread(m_hImgViewThread, 0) ;					
				}
			}			
		}
	}

	return true;
}

void AdjustDisplaySize2(unsigned char* pFrameBuffer, unsigned char* p2byteBuffer, RawImgInfo* pRawImgInfo)
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

void CMIU::func_Set_InspImageCopy(int nType)
{
    CString str;
    if (nType == MID_6500K_RAW)
    {
        //Mid-level (6500K)
        str.Format("Mid_level_6500K");
       // vDefectMidBuffer_6500K = m_pFrameRawBuffer;
		memcpy(vDefectMidBuffer_6500K, m_pFrameRawBuffer, m_pBoard->GetFrameRawSize());
    }
    else if (nType == LOW_LEVEL_RAW)
    {
        //dark �̹���
        //low-level (Dark)
        str.Format("low_level_DARK");
        //vDefectLowBuffer = m_pFrameRawBuffer;
		memcpy(vDefectLowBuffer, m_pFrameRawBuffer, m_pBoard->GetFrameRawSize());
    }
    else if (nType == MID_2800K_RAW)
    {
        //Mid-level (2800K)  --Color Sensitivity �� ���(2800K+6500K)
        str.Format("Mid_level_2800K");
       // vDefectMidBuffer_2800K = m_pFrameRawBuffer;
		memcpy(vDefectMidBuffer_2800K, m_pFrameRawBuffer, m_pBoard->GetFrameRawSize());
    }
    else if (nType == CHART_RAW)
    {
        str.Format("CHART1");
		//vChartBuffet = m_pFrameRawBuffer;
		memcpy(vChartBuffet, m_pFrameRawBuffer, m_pBoard->GetFrameRawSize());
    }
	else if (nType == UV_BEFORE_CHART)
	{
		str.Format("UV_BEFORE_CHART");
		//vChartBuffet = m_pFrameRawBuffer;
		memcpy(vChartBuffet, m_pFrameRawBuffer, m_pBoard->GetFrameRawSize());
	}
	else if (nType == UV_AFTER_CHART)
	{
		str.Format("UV_AFTER_CHART");
		//vChartBuffet = m_pFrameRawBuffer;
		memcpy(vChartBuffet, m_pFrameRawBuffer, m_pBoard->GetFrameRawSize());
	}
	else if (nType == EOL_CHART)
	{
		str.Format("EOL_CHART");
		//vChartBuffet = m_pFrameRawBuffer;
		memcpy(vChartBuffet, m_pFrameRawBuffer, m_pBoard->GetFrameRawSize());
	}
	
	else if (nType == CHART_SECOND_RAW)
	{
		str.Format("CHART2");
		//vChart_Second_Buffet = m_pFrameRawBuffer;
		memcpy(vChart_Second_Buffet, m_pFrameRawBuffer, m_pBoard->GetFrameRawSize());
	}
	
    else
    {
        //��� �ʱ�ȭ?
		memset(vChartBuffet, 0, m_pBoard->GetFrameRawSize());
		memset(vChart_Second_Buffet, 0, m_pBoard->GetFrameRawSize());
		memset(vDefectMidBuffer_6500K, 0, m_pBoard->GetFrameRawSize());
        memset(vDefectLowBuffer, 0, m_pBoard->GetFrameRawSize());
        memset(vDefectMidBuffer_2800K, 0, m_pBoard->GetFrameRawSize());
    }
    RawImageSave(str);

	str.Empty();
}
void CMIU::RawImageSave(CString nType)
{
    TCHAR szPath[SIZE_OF_1K];
    TCHAR szFilePath[SIZE_OF_1K];
    SYSTEMTIME stSysTime;
    CFileFind clFinder;
    CString sFileMode = _T("");
    CString strLog;
    CFile clFile;
    int i = 0;
    ::GetLocalTime(&stSysTime);

    /*if (_tcslen(g_clTaskWork[m_nUnit].m_szLotID) <= 0)
        _tcscpy_s(g_clTaskWork[m_nUnit].m_szLotID, SIZE_OF_100BYTE, _T("(NULL)"));

    if (_tcslen(g_clTaskWork[m_nUnit].m_szChipID) <= 0)
        _tcscpy_s(g_clTaskWork[m_nUnit].m_szChipID, SIZE_OF_100BYTE, _T("(NULL)"));*/

    if (clFinder.FindFile(BASE_LOG_DIR) == FALSE)
        CreateDirectory(BASE_LOG_DIR, NULL);

    _stprintf_s(szPath, SIZE_OF_1K, _T("%s\\%s"), BASE_LOG_DIR , model.mCurModelName);
    if (clFinder.FindFile(szPath) == FALSE)
        CreateDirectory(szPath, NULL);

    _stprintf_s(szPath, SIZE_OF_1K, _T("%s\\%s\\%04d%02d"), BASE_LOG_DIR, model.mCurModelName, stSysTime.wYear, stSysTime.wMonth);
    if (clFinder.FindFile(szPath) == FALSE)
        CreateDirectory(szPath, NULL);

    _stprintf_s(szPath, SIZE_OF_1K, _T("%s\\%s\\%04d%02d\\Image"), BASE_LOG_DIR, model.mCurModelName, stSysTime.wYear, stSysTime.wMonth);
    if (clFinder.FindFile(szPath) == FALSE)
        CreateDirectory(szPath, NULL); 


    _stprintf_s(szFilePath, SIZE_OF_1K, _T("%s\\%s_%s_%d_%02d%02d%02d%03d.raw"), szPath, Task.ChipID, (LPTSTR)(LPCTSTR)nType, gMIUDevice.dTDATASPEC_n.eDemosaicMethod, stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond, stSysTime.wMilliseconds);

    CFile           savefile;
    CFileException  e;

    if (!savefile.Open(szFilePath, CFile::modeCreate | CFile::modeWrite, &e))
    {
        strLog.Format("File open fail:");
        theApp.MainDlg->putListLog(strLog);
        return;
    }
    savefile.Write(m_pFrameRawBuffer, m_pBoard->GetFrameRawSize());
    savefile.Close();


	sFileMode.Empty();
	strLog.Empty();
}
void CMIU::RawToBmp2(unsigned char *pRaw8,unsigned char *pBMP,int image_width,int image_height,int bayer_format)
{ 
	unsigned char *pBmpBuffer;
	unsigned char *pImageData;
	unsigned char *pImageTemp;
	unsigned char *pImageTemp1;
	unsigned char *pImageTemp2;

	unsigned char		r, g, b, temp;
	int			i, j;

	int			height, width;

	unsigned char *pTargetBuffer, *pSourceBuffer;

	height = image_height;
	width = image_width;

	pTargetBuffer = pBMP;
	pSourceBuffer = pRaw8;

	pBmpBuffer = pBMP;
	pImageData = pRaw8 + image_width*(image_height-1);

	switch(bayer_format) 
	{
	case GRBG:				//GRGR
		//BGBG
		//GRGR
		//BGBG
		for (i=0; i<image_height-2; i+=2) 
		{
			pImageTemp = pImageData;						
			pImageTemp1 = pImageData - image_width;		
			pImageTemp2 = pImageData - image_width*2;
			for (j=0; j<image_width-2; j+=2) 
			{
				r =	pImageTemp1[1]; 					
				b =  (pImageTemp[0] +pImageTemp[2]+pImageTemp2[0]+pImageTemp2[2]) >> 2; 
				g = (pImageTemp[1] +pImageTemp1[0]+pImageTemp1[2]+pImageTemp2[1]) >> 2;
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;							
				pImageTemp  += 1;   		
				pImageTemp1 += 1;							
				pImageTemp2 += 1;	

				r = (pImageTemp1[0]+pImageTemp1[2]) >> 1;							
				b = (pImageTemp[1]+pImageTemp2[1]) >> 1;
				g = pImageTemp1[1];
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
				pImageTemp += 1;    
				pImageTemp1 += 1;
				pImageTemp2 += 1;

			}
			for (j=0; j<2; j++) 
			{	//2's dummy
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;	
			}
			pImageData -= image_width;
			pImageTemp = pImageData;						
			pImageTemp1 = pImageData - image_width;		
			pImageTemp2 = pImageData - image_width*2;

			for (j=0; j<image_width-2; j+=2) {
				r = (pImageTemp[1]+pImageTemp2[1]) >> 1;							
				b = (pImageTemp1[0]+pImageTemp1[2]) >> 1;
				g = pImageTemp1[1];
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;	
				pImageTemp += 1;							
				pImageTemp1 += 1;							
				pImageTemp2 += 1;

				b = pImageTemp1[1];
				g = (pImageTemp[1]+pImageTemp1[0]+pImageTemp1[2]+pImageTemp2[1]) >> 2;
				r = (pImageTemp[0]+pImageTemp[2]+pImageTemp2[0]+pImageTemp2[2]) >> 2;							  				
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;	
				pImageTemp += 1; 
				pImageTemp1 += 1;
				pImageTemp2 += 1;				
			}
			for (j=0; j<2; j++) 
			{   //2's dummy
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;	
			}
			pImageData -= image_width;
		} //end for Y axis
		for (i=0; i<image_width*3; i++)	pBMP[image_width*3*(image_height-2)+i] = pBMP[image_width*3*(image_height-3)+i];
		for (i=0; i<image_width*3; i++)	pBMP[image_width*3*(image_height-1)+i] = pBMP[image_width*3*(image_height-3)+i];

		break;
	case RGGB:					//RGRG
		//GBGB
		//RGRG
		//GBGB
		for (i=0; i<image_height-2; i+=2) 
		{
			pImageTemp = pImageData;						
			pImageTemp1 = pImageData - image_width;		
			pImageTemp2 = pImageData - image_width*2;
			for (j=0; j<image_width-2; j+=2) 
			{
				r = (pImageTemp1[0]+pImageTemp1[2]) >> 1;							
				b = (pImageTemp[1]+pImageTemp2[1]) >> 1;
				g = pImageTemp1[1];
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
				pImageTemp  += 1;   		
				pImageTemp1 += 1;							
				pImageTemp2 += 1;	

				r =	pImageTemp1[1]; 					
				b =  (pImageTemp[0] +pImageTemp[2]+pImageTemp2[0]+pImageTemp2[2]) >> 2; 
				g = (pImageTemp[1] +pImageTemp1[0]+pImageTemp1[2]+pImageTemp2[1]) >> 2;
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;

				*pBmpBuffer++ = r;
				pImageTemp += 1;    
				pImageTemp1 += 1;
				pImageTemp2 += 1;
			}
			for (j=0; j<2; j++) 
			{	//2's dummy
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
			}
			pImageData -= image_width;
			pImageTemp = pImageData;						
			pImageTemp1 = pImageData - image_width;		
			pImageTemp2 = pImageData - image_width*2;

			for (j=0; j<image_width-2; j+=2) {
				b = pImageTemp1[1];
				g = (pImageTemp[1]+pImageTemp1[0]+pImageTemp1[2]+pImageTemp2[1]) >> 2;
				r = (pImageTemp[0]+pImageTemp[2]+pImageTemp2[0]+pImageTemp2[2]) >> 2;							  				
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
				pImageTemp += 1;							
				pImageTemp1 += 1;							
				pImageTemp2 += 1;

				r =	(pImageTemp[1]+pImageTemp2[1]) >> 1;							
				b = (pImageTemp1[0]+pImageTemp1[2]) >> 1;
				g = pImageTemp1[1];
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;

				*pBmpBuffer++ = r;
				pImageTemp += 1; 
				pImageTemp1 += 1;
				pImageTemp2 += 1;
			}
			for (j=0; j<2; j++) 
			{   //2's dummy
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
			}
			pImageData -= image_width;
		} //end for Y axis
		for (i=0; i<image_width*3; i++)	pBMP[image_width*3*(image_height-2)+i] = pBMP[image_width*3*(image_height-3)+i];
		for (i=0; i<image_width*3; i++)	pBMP[image_width*3*(image_height-1)+i] = pBMP[image_width*3*(image_height-3)+i];
		break;

	case BGGR:					//BGBG
		//GRGR
		//BGBG
		//GRGR
		for (i=0; i<image_height-2; i+=2) 
		{
			pImageTemp = pImageData;						
			pImageTemp1 = pImageData - image_width;		
			pImageTemp2 = pImageData - image_width*2;
			for (j=0; j<image_width-2; j+=2) 
			{
				b = (pImageTemp1[0]+pImageTemp1[2]) >> 1;
				g = pImageTemp1[1];									
				r = (pImageTemp[1]+pImageTemp2[1]) >> 1;
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
				pImageTemp  += 1;   		
				pImageTemp1 += 1;							
				pImageTemp2 += 1;

				b = pImageTemp1[1];
				g = (pImageTemp[1]+pImageTemp1[0]+pImageTemp1[2]+pImageTemp2[1]) >> 2;
				r = (pImageTemp[0]+pImageTemp[2]+pImageTemp2[0]+pImageTemp2[2]) >> 2;							  				
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
				pImageTemp += 1;    
				pImageTemp1 += 1;
				pImageTemp2 += 1;
			}
			for (j=0; j<2; j++) 
			{	//2's dummy
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
			}
			pImageData -=image_width;
			pImageTemp = pImageData;						
			pImageTemp1 = pImageData - image_width;		
			pImageTemp2 = pImageData - image_width*2;

			for	(j=0; j<image_width-2; j+=2) 
			{
				r =	pImageTemp1[1]; 					
				b =  (pImageTemp[0] +pImageTemp[2]+pImageTemp2[0]+pImageTemp2[2]) >> 2; 
				g = (pImageTemp[1] +pImageTemp1[0]+pImageTemp1[2]+pImageTemp2[1]) >> 2;
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
				pImageTemp += 1;							
				pImageTemp1 += 1;							
				pImageTemp2 += 1;	

				r = (pImageTemp1[0]+pImageTemp1[2]) >> 1;							
				b = (pImageTemp[1]+pImageTemp2[1]) >> 1;
				g = pImageTemp1[1]; 
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
				pImageTemp += 1; 
				pImageTemp1 += 1;
				pImageTemp2 += 1;
			}
			for (j=0; j<2; j++) 
			{   //2's dummy
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
			}
			pImageData -= image_width;
		} //end for Y
		for (i=0; i<image_width*3; i++)	pBMP[image_width*3*(image_height-2)+i] = pBMP[image_width*3*(image_height-3)+i];
		for (i=0; i<image_width*3; i++)	pBMP[image_width*3*(image_height-1)+i] = pBMP[image_width*3*(image_height-3)+i];

		break;
	case GBRG:					//GBGB
		//RGRG
		//GBGB
		//RGRG
		for (i=0; i<image_height-2; i+=2) 
		{
			pImageTemp = pImageData;						
			pImageTemp1 = pImageData - image_width;		
			pImageTemp2 = pImageData - image_width*2;
			for (j=0; j<image_width-2; j+=2) 
			{
				b = pImageTemp1[1];
				g = (pImageTemp[1]+pImageTemp1[0]+pImageTemp1[2]+pImageTemp2[1]) >> 2;
				r = (pImageTemp[0]+pImageTemp[2]+pImageTemp2[0]+pImageTemp2[2]) >> 2;
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
				pImageTemp  += 1;   		
				pImageTemp1 += 1;							
				pImageTemp2 += 1;	

				b = (pImageTemp1[0]+pImageTemp1[2]) >> 1;
				g = pImageTemp1[1];
				r = (pImageTemp[1]+pImageTemp2[1]) >> 1;
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
				pImageTemp += 1;
				pImageTemp1 += 1;
				pImageTemp2 += 1;
			}
			for (j=0; j<2; j++) 
			{	//2's dummy
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
			}
			pImageData -= image_width;
			pImageTemp = pImageData;						
			pImageTemp1 = pImageData - image_width;		
			pImageTemp2 = pImageData - image_width*2;

			for (j=0; j<image_width-2; j+=2) 
			{
				r = (pImageTemp1[0]+pImageTemp1[2]) >> 1;							
				b = (pImageTemp[1]+pImageTemp2[1]) >> 1;
				g = pImageTemp1[1]; 		
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
				pImageTemp += 1;							
				pImageTemp1 += 1;							
				pImageTemp2 += 1;

				r =	pImageTemp1[1]; 					
				b =  (pImageTemp[0] +pImageTemp[2]+pImageTemp2[0]+pImageTemp2[2]) >> 2; 
				g = (pImageTemp[1] +pImageTemp1[0]+pImageTemp1[2]+pImageTemp2[1]) >> 2;
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
				pImageTemp += 1; 
				pImageTemp1 += 1;
				pImageTemp2 += 1;
			}
			for (j=0; j<2; j++) {   //2's dummy
				*pBmpBuffer++ = b;
				*pBmpBuffer++ = g;
				*pBmpBuffer++ = r;
			}
			pImageData -= image_width;
		} //end for (m_ImageSize.xy)
		for (i=0; i<image_width*3; i++)	pBMP[image_width*3*(image_height-2)+i] = pBMP[image_width*3*(image_height-3)+i];
		for (i=0; i<image_width*3; i++)	pBMP[image_width*3*(image_height-1)+i] = pBMP[image_width*3*(image_height-3)+i];

		break;
		case RCCC :
		for (i = 0; i < height; i++)
			for (j = 0; j < width; j++)
			{
				if(i==0 && j==0)  // top_left
				{
					
				   *(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp = (BYTE)((*(pSourceBuffer + 1) + *(pSourceBuffer + width)) / 2);
				   *(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
				   *(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
				   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
				}
				else if(i==0 && ( (j+1) & 0x1 ))  // top
				{

					*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp =  (BYTE)((*(pSourceBuffer + j+1) + *(pSourceBuffer + j-1) + *(pSourceBuffer + width + j)) / 3);
					*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
					*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
				   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
				}
				else if( ((i+1) & 0x01 ) && j==0) // left
				{
					*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp =  (BYTE)((*(pSourceBuffer + i*width +1) + *(pSourceBuffer + (i-1)*width) + *(pSourceBuffer + (i+1)*width)) / 3);
					*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
					*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
				   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
				}
				else if( ((i+1) & 0x01 ) && ( (j+1) & 0x1 ) )
				{
					*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp =  (BYTE)((*(pSourceBuffer + i*width +j+1) + *(pSourceBuffer + (i-1)*width + j) + *(pSourceBuffer + (i+1)*width +j) + *(pSourceBuffer + i*width +j-1)) >> 2);
					*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
					*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
				   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
				}
				else
				{
					*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp = *(pSourceBuffer + i * width + j);
					*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
					*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
				   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
				}
			}
			break;
			case CRCC :
				for (i = 0; i < height; i++)
					for (j = 0; j < width; j++)
					{
						if(i==0 && j==(width-1))  // top_right
						{
					
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp = (BYTE)((*(pSourceBuffer + j - 1) + *(pSourceBuffer + width + j)) / 2);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
						else if(i==0 && ( j & 0x1 ))  // top
						{

							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp =  (BYTE)((*(pSourceBuffer + j+1) + *(pSourceBuffer + j-1) + *(pSourceBuffer + width + j)) / 3);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
						else if( ((i+1) & 0x01 ) && j==(width-1)) // right
						{
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp =  (BYTE)((*(pSourceBuffer + i*width + j -1) + *(pSourceBuffer + (i-1)*width + j ) + *(pSourceBuffer + (i+1)*width + j)) / 3);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
						else if( ((i+1) & 0x01 ) && ( j & 0x1 ) )
						{
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp =  (BYTE)((*(pSourceBuffer + i*width +j+1) + *(pSourceBuffer + (i-1)*width + j) + *(pSourceBuffer + (i+1)*width +j) + *(pSourceBuffer + i*width +j-1)) >> 2);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
						else
						{
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp = *(pSourceBuffer + i * width + j);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
					}


				break;

			case CCRC :
				for (i = 0; i < height; i++)
					for (j = 0; j < width; j++)
					{
						if(i==(height-1) && j==0)  // bottom_left
						{
					
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp = (BYTE)((*(pSourceBuffer + (i-1)*width + j) + *(pSourceBuffer + i*width + j + 1)) / 2);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
						else if(i==(height-1) && ( (j+1) & 0x1 ))  // bottom
						{

							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp =  (BYTE)((*(pSourceBuffer + (i-1)*width + j) + *(pSourceBuffer + i*width + j + 1) + *(pSourceBuffer + i*width + j - 1)) / 3);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
						else if( (i & 0x01 ) && j==0) // left
						{
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp =  (BYTE)((*(pSourceBuffer + i*width +1) + *(pSourceBuffer + (i-1)*width) + *(pSourceBuffer + (i+1)*width)) / 3);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
						else if( (i & 0x01 ) && ( (j+1) & 0x1 ) )
						{
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp =  (BYTE)((*(pSourceBuffer + i*width +j+1) + *(pSourceBuffer + (i-1)*width + j) + *(pSourceBuffer + (i+1)*width +j) + *(pSourceBuffer + i*width +j-1)) / 4);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
						else
						{
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp = *(pSourceBuffer + i * width + j);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
					}

				break;
			case CCCR :

				for (i = 0; i < height; i++)
					for (j = 0; j < width; j++)
					{
						if(i==(height-1) && j==(width-1))  // bottom_right
						{
					
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp = (BYTE)((*(pSourceBuffer + (i-1)*width + j) + *(pSourceBuffer + i*width +j-1)) / 2);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
						else if(i==(height-1) && ( j & 0x1 ))  // bottom
						{

							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp =  (BYTE)((*(pSourceBuffer + (i-1)*width + j) + *(pSourceBuffer + i*width + j + 1) + *(pSourceBuffer + i*width + j - 1)) / 3);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
						else if( (i & 0x01 ) && j==(width-1)) // right
						{
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp =  (BYTE)((*(pSourceBuffer + i*width + j -1) + *(pSourceBuffer + (i-1)*width + j ) + *(pSourceBuffer + (i+1)*width + j)) / 3);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
						else if( (i & 0x01 ) && ( j & 0x1 ) )
						{
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp =  (BYTE)((*(pSourceBuffer + i*width +j+1) + *(pSourceBuffer + (i-1)*width + j) + *(pSourceBuffer + (i+1)*width +j) + *(pSourceBuffer + i*width +j-1)) / 4);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
						else
						{
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3) = temp = *(pSourceBuffer + i * width + j);
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 1)  = temp;
							*(pTargetBuffer + (height-1-i) * width * 3 + j * 3 + 2)  = temp;
						   //*(m_pInterpolationBuffer + i * width + j)	=	temp;
						}
					}
			
			break;
			default :

				break;

	}  

}

#define YUV422_MIRROR		5	//Cb0 Y0 Cr0 Y1 / Cb2 Y2 Cr2 Y3
#define YUV422				6	//Cb0 Y0 Cr0 Y1 / Cb2 Y2 Cr2 Y3
#define YUV422_Y			7	//Cb0 Y0 Cr0 Y1 / Cb2 Y2 Cr2 Y3
#define YUV422_2			8	//Y0 Cb0 Y1 Cr0 / Y2 Cb2 Y3 Cr2
#define YUV422_Y2			9	//Cb0 Y0 Cr0 Y1 / Cb2 Y2 Cr2 Y3

