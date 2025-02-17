#include "StdAfx.h"
#include "Comm_RS232C.h"
//#include "../../AA Bonder.h"
//#include "../../AA BonderDlg.h"

bool	g_Comm_bReady_UV;

CCommList_RS232C::CCommList_RS232C(void)
{
	m_arrList_ComPort.RemoveAll();
	m_arrList_ComPort.FreeExtra();
}

CCommList_RS232C::~CCommList_RS232C(void)
{
	m_arrList_ComPort.RemoveAll();
	m_arrList_ComPort.FreeExtra();
}

//! [���]   PC�� Serial Port�� �˾Ƴ��� m_arrComPortList�� �����Ѵ�. 
//! [��ȯ��] �۾��� �Ϸ��ϸ� true�� ��ȯ�Ѵ�. 
bool CCommList_RS232C::GetList_SerialPort()
{
	HKEY hResKey = NULL;

	//! Registry�� �����Ѵ�. 	
	long nRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hResKey);

	//! Registry�� ���� ����
	if ( nRet != ERROR_SUCCESS )
	{
		return false;
	}

	//! ��� �ʱ�ȭ
	m_arrList_ComPort.RemoveAll();
	m_arrList_ComPort.FreeExtra();

	//! ����, ���� �ʱ�ȭ
	DWORD i = 0, dwKeyType = 0;
	TCHAR czKeyName[8192], czKeyValue[8192];
	DWORD dwKeyNameBufSize = (DWORD)(sizeof(czKeyName));
	DWORD dwKeyValueBufSize = (DWORD)(sizeof(czKeyValue));
	CString szAddValue = _T("");

	//! Registry���� PC�� Serial Port�� �˾Ƴ��� m_arrComPortList�� �����Ѵ�. 
	for ( i = 0; ; i++ )
	{
		dwKeyNameBufSize = (DWORD)(sizeof(czKeyName));
		dwKeyValueBufSize = (DWORD)(sizeof(czKeyValue));

		nRet = RegEnumValue(hResKey, i, czKeyName, &dwKeyNameBufSize, NULL, &dwKeyType, (LPBYTE)czKeyValue, &dwKeyValueBufSize);

		if ( nRet == ERROR_SUCCESS )
		{
			szAddValue.Format(_T("%s"), czKeyValue);
			m_arrList_ComPort.Add(szAddValue);
		}
		else
		{
			//! ERROR_NO_MORE_ITEMS : �� �̻� �о�� ���� ���� ��
			break;
		}
	}

	//! Registry ���� �ڵ��� �ݴ´�.
	RegCloseKey(hResKey);

	return true;
}


CCommHelper_Setup_RS232C::CCommHelper_Setup_RS232C(void)
{
}

CCommHelper_Setup_RS232C::~CCommHelper_Setup_RS232C(void)
{
}

bool CCommHelper_Setup_RS232C::GetValue_BaudRate(int iIndex_BaudRate, DWORD &dwRetBaudRate, CString *psItemName)
{
	bool bRet = true;

	switch(iIndex_BaudRate)
	{
	case 0:
		{
			dwRetBaudRate = CBR_1200;

			if ( psItemName != NULL )
			{
				*psItemName = _T("1200");
			}
		}
		break;
	case 1:
		{
			dwRetBaudRate = CBR_2400;

			if ( psItemName != NULL )
			{
				*psItemName = _T("2400");
			}
		}
		break;
	case 2:
		{
			dwRetBaudRate = CBR_4800;

			if ( psItemName != NULL )
			{
				*psItemName = _T("4800");
			}
		}
		break;
	case 3:
		{
			dwRetBaudRate = CBR_9600;

			if ( psItemName != NULL )
			{
				*psItemName = _T("9600");
			}
		}
		break;
	case 4:
		{
			dwRetBaudRate = CBR_19200;

			if ( psItemName != NULL )
			{
				*psItemName = _T("19200");
			}
		}
		break;
	case 5:
		{
			dwRetBaudRate = CBR_38400;

			if ( psItemName != NULL )
			{
				*psItemName = _T("38400");
			}
		}
		break;
	case 6:
		{
			dwRetBaudRate = CBR_115200;

			if ( psItemName != NULL )
			{
				*psItemName = _T("115200");
			}
		}
		break;		
	default:
		{
			dwRetBaudRate = CBR_38400;
			bRet = false;

			if ( psItemName != NULL )
			{
				*psItemName = _T("");
			}
		}
		break;
	}

	return bRet;
}

bool CCommHelper_Setup_RS232C::GetValue_DataBit(int iIndex_DataBit, BYTE  &byRetDataBit, CString *psItemName)
{
	bool bRet = true;

	switch(iIndex_DataBit)
	{
	case 0:
		{
			byRetDataBit = 5;
		}
		break;
	case 1:
		{
			byRetDataBit = 6;
		}
		break;
	case 2:
		{
			byRetDataBit = 7;
		}
		break;
	case 3:
		{
			byRetDataBit = 8;
		}
		break;
	default:
		{
			byRetDataBit = 8;
			bRet = false;
		}
		break;
	}

	if ( psItemName != NULL )
	{
		if ( bRet == true )
		{
			int iDataBit = (int)(byRetDataBit);
			psItemName->Format(_T("%d"), iDataBit);
		}
		else
		{
			*psItemName = _T("");
		}
	}

	return bRet;
}

bool CCommHelper_Setup_RS232C::GetValue_StopBit(int iIndex_StopBit, BYTE  &byRetStopBit, CString *psItemName)
{
	bool bRet = true;

	switch(iIndex_StopBit)
	{
	case 0:
		{
			byRetStopBit = ONESTOPBIT;

			if ( psItemName != NULL )
			{
				*psItemName = _T("1");
			}
		}
		break;
	case 1:
		{
			byRetStopBit = TWOSTOPBITS;

			if ( psItemName != NULL )
			{
				*psItemName = _T("2");
			}
		}
		break;
	default:
		{
			byRetStopBit = ONESTOPBIT;
			bRet = false;

			if ( psItemName != NULL )
			{
				*psItemName = _T("");
			}
		}
		break;
	}
	
	return bRet;
}

bool CCommHelper_Setup_RS232C::GetValue_ParityBit(int iIndex_ParityBit, BYTE  &byRetParityBit, CString *psItemName)
{
	bool bRet = true;

	switch(iIndex_ParityBit)
	{
	case 0:
		{
			byRetParityBit = NOPARITY;

			if ( psItemName != NULL )
			{
				*psItemName = _T("None");
			}
		}
		break;
	case 1:
		{
			byRetParityBit = ODDPARITY;

			if ( psItemName != NULL )
			{
				*psItemName = _T("Odd");
			}
		}
		break;
	case 2:
		{
			byRetParityBit = EVENPARITY;

			if ( psItemName != NULL )
			{
				*psItemName = _T("Even");
			}
		}
		break;
	default:
		{
			byRetParityBit = NOPARITY;
			bRet= false;

			if ( psItemName != NULL )
			{
				*psItemName = _T("");
			}
		}
		break;
	}

	return bRet;
}

int CCommHelper_Setup_RS232C::GetIndex_BaudRate(DWORD dwBaudRate)
{
	int iIndex_Ret = -1;

	if ( dwBaudRate == CBR_1200 )
	{
		iIndex_Ret = 0;
	}
	else if ( dwBaudRate == CBR_2400 )
	{
		iIndex_Ret = 1;
	}
	else if ( dwBaudRate == CBR_4800 )
	{
		iIndex_Ret = 2;
	}
	else if ( dwBaudRate == CBR_9600 )
	{
		iIndex_Ret = 3;
	}
	else if ( dwBaudRate == CBR_19200 )
	{
		iIndex_Ret = 4;
	}
	else if ( dwBaudRate == CBR_38400 )
	{
		iIndex_Ret = 5;
	}
	else if ( dwBaudRate == CBR_115200 )
	{
		iIndex_Ret = 6;
	}
	
	return iIndex_Ret;
}

int CCommHelper_Setup_RS232C::GetIndex_DataBit(BYTE byDataBit)
{
	int iIndex_Ret = -1;

	if ( byDataBit == 5 )
	{
		iIndex_Ret = 0;
	}
	else if ( byDataBit == 6 )
	{
		iIndex_Ret = 1;
	}
	else if ( byDataBit == 7 )
	{
		iIndex_Ret = 2;
	}
	else if ( byDataBit == 8 )
	{
		iIndex_Ret = 3;
	}

	return iIndex_Ret;
}

int CCommHelper_Setup_RS232C::GetIndex_StopBit(BYTE byStopBit)
{
	int iIndex_Ret = -1;

	if ( byStopBit == TWOSTOPBITS )
	{
		iIndex_Ret = 1;
	}
	else if ( byStopBit == ONESTOPBIT )
	{
		iIndex_Ret = 0;
	}

	return iIndex_Ret;
}

int CCommHelper_Setup_RS232C::GetIndex_ParityBit(BYTE byParityBit)
{
	int iIndex_Ret = -1;

	if ( byParityBit == NOPARITY )
	{
		iIndex_Ret = 0;
	}
	else if ( byParityBit == ODDPARITY )
	{
		iIndex_Ret = 1;
	}
	else if ( byParityBit == EVENPARITY )
	{
		iIndex_Ret = 2;
	}

	return iIndex_Ret;
}

//! [���]   Baud Rate ���� ���������� ��ȯ
//! [�Է°�] 'CBR_'�� ���۵Ǵ� Baud Rate �����
//! [��ȯ��] Baud Rate ������ ��
int CCommHelper_Setup_RS232C::GetBaudRateValue_Integer_FromConstant(DWORD dwBaudRate)
{
	int iRet = 38400;

	switch(dwBaudRate)
	{
	case CBR_1200:
		{
			iRet = 1200;
		}
		break;
	case CBR_2400:
		{
			iRet = 2400;
		}
		break;
	case CBR_4800:
		{
			iRet = 4800;
		}
		break;
	case CBR_9600:
		{
			iRet = 9600;
		}
		break;
	case CBR_19200:
		{
			iRet = 19200;
		}
		break;
	case CBR_38400:
		{
			iRet = 38400;
		}
		break;
	case CBR_115200:
		{
			iRet = 115200;
		}
		break;
	}

	return iRet;
}

//! [���] ������ ���� BYTE �迭�� �����ͷ� ��ȯ
//! [�Է°�] iInput : ������ ��, pbyOutputBuf : BYTE �迭, ����� ū �迭�� �Է��ؾ� �Ѵ�. 
//!			 iOutputBufSize : pbyOutputBuf�� ũ��
//! [��ȯ��] �۾��� �Ϸ�Ǹ� pbyOutputBuf�� �Էµ� �������� ũ�⸦ ��ȯ�Ѵ�. ������ �߻��ϸ� ������ ��ȯ�Ѵ�. 
int CCommHelper_Setup_RS232C::Convert_Integer_To_ArrayByte(int iInput, BYTE* pbyOutputBuf, int iOutputBufSize)
{
	if ( pbyOutputBuf == NULL || iOutputBufSize <= 0 )
	{
		return -1;
	}

	TCHAR czInteger[64];
	memset(czInteger, 0x00, sizeof(czInteger));
	_itot_s(iInput, czInteger, 10);	//! Convert an integer to a string, 10���� Base radix ���

	int iLength = _tcsclen(czInteger);

	if ( iLength >= iOutputBufSize )
	{
		return -2;
	}

	int i = 0;
	for ( i = 0; i < iLength; i++ )
	{
		pbyOutputBuf[i] = (BYTE)(czInteger[i]);
	}

	return iLength;
}


CComm_RS232C::CComm_RS232C(void)
{
	//! �ʱ�ȭ
	m_hComm = NULL;

	m_strCommSetupInfo.InitInfo();
	m_strCommStateInfo.InitInfo();

	m_osReceive.hEvent = NULL;
	m_osSend.hEvent = NULL;

	m_pThread_RecvComm = NULL;

	m_pReceiveProcPtr = NULL;	

	m_bFlag_Stop = false;	
}

CComm_RS232C::~CComm_RS232C(void)
{
	m_csReceiveProcPtr.Lock();
	m_pReceiveProcPtr = NULL;
	m_csReceiveProcPtr.Unlock();

	ClosePort();
}

//! [���] 'SetThreadAffinityMask' �Լ��� Mask ���ڰ��� ��ȯ�޴´�. 
//! [�Է°�] piBuf_CPU_Use : int �迭, CPU�� ����ϸ� 1, ������� ������ 0, 
//!							 0 �� index�� 1�� CPU�� ����Ų��. 1�� index�� 2�� CPU, ...
//!			 iBufSize_CPU_Use : piBuf_CPU_Use �迭���� ����� �������� ����
//! [��ȯ��] 'SetThreadAffinityMask' �Լ��� Mask ���ڰ�, ������ �߻��ϸ� 0�� ��ȯ�Ѵ�. 
DWORD CComm_RS232C::GetMask_CPU_Affinity(int *piBuf_CPU_Use, int iBufSize_CPU_Use)
{
	if ( piBuf_CPU_Use == NULL )
	{
		return 0;
	}

	//! �ý����� ����� �˾Ƴ���. 
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	//! CPU ������ ���� ũ�⸦ Ȯ���Ѵ�. 
	int iCPU_Size = (int)(sysInfo.dwNumberOfProcessors);
	if ( iBufSize_CPU_Use <= 0 || iBufSize_CPU_Use > iCPU_Size )
	{
		return 0;
	}

	int i = 0;
	DWORD dwRet = 0;

	iBufSize_CPU_Use--;
	for ( i = iBufSize_CPU_Use; i >= 0; i-- )
	{
		if ( piBuf_CPU_Use[i] != 0 )	//! CPU�� ����� ���
		{
			dwRet = dwRet | (0x01 << (iBufSize_CPU_Use - i));
		}
	}

	return dwRet;
}

int	CComm_RS232C::Set_CPU_Affinity(int *piBuf_CPU_Use, int iBufSize_CPU_Use)
{
	//! ����, Thread�� �������� �ʾ���
	if ( m_pThread_RecvComm == NULL )
	{
		return -1;
	}

	if ( piBuf_CPU_Use == NULL )
	{
		return -2;
	}

	//! �ý����� ����� �˾Ƴ���. 
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	//! CPU ������ ���� ũ�⸦ Ȯ���Ѵ�. 
	int iCPU_Size = (int)(sysInfo.dwNumberOfProcessors);
	if ( iCPU_Size <= 0 )
	{
		return -3;
	}

	//! CPU�� 1 Core �̹Ƿ� �� ����� ������� �ʴ´�.
	if ( iCPU_Size == 1 )
	{
		return 0;
	}
	
	//! ��� ������ CPU�� ������ �˾Ƴ���.
	int iSize_Possible_CPU_Core = 0;
	if ( iCPU_Size > intBufSize_RS232C_CPU_Core )
	{
		iSize_Possible_CPU_Core = intBufSize_RS232C_CPU_Core;
	}
	else
	{
		iSize_Possible_CPU_Core = iCPU_Size;
	}

	//! GetMask_CPU_Affinity �Լ��� �Է��� ����
	int aiCPU[intBufSize_RS232C_CPU_Core];
	memset(aiCPU, 0, sizeof(aiCPU));

	//! GetMask_CPU_Affinity �Լ��� �Է��� ���� ����
	//! ���ÿ� ���ۿ��� ������ ��� CPU ������ Ȯ��
	int i = 0;
	int iCnt_Use_CPU = 0;
	for ( i = 0; i < iSize_Possible_CPU_Core; i++ )
	{
		if ( i >= iBufSize_CPU_Use )
		{
			break;
		}
		if ( piBuf_CPU_Use[i] != 0 )
		{
			aiCPU[i] = 1;
			iCnt_Use_CPU++;
		}
	}

	//! ���� PC���� ����� CPU�� ���� ���
	if ( iCnt_Use_CPU <= 0 )
	{
		//! �� ������ CPU�� ����ϵ��� ���� ����
		aiCPU[iSize_Possible_CPU_Core - 1] = 1;
	}

	DWORD dwMaskValue = GetMask_CPU_Affinity(aiCPU, iSize_Possible_CPU_Core);

	//! Process�� �۵���ų CPU�� �����Ѵ�.  
	DWORD dwRet = ::SetThreadAffinityMask(m_pThread_RecvComm->m_hThread, dwMaskValue);
	if ( dwRet <= 0 )
	{
		DWORD dwError = GetLastError();

		//! The handle is invalid
		//! Thread�� �������� ���� Thread ��ü �Լ����� �ߴܵ� ����̴�.
		if ( dwError == ERROR_INVALID_HANDLE )
		{
			return -4;
		}

		//! 'dwMaskValue' �Է°��� ������ ����
		if ( dwError == ERROR_INVALID_PARAMETER )
		{
			return -5;
		}

		return -6;
	}

	return 1;
}

//! [���] Thread �۵��� �����Ѵ�.
int CComm_RS232C::StartThread(bool bFlag_Use_CPU_Affinity, int *piBuf_CPU_Use, int iBufSize_CPU_Use)
{
	if ( m_pThread_RecvComm != NULL )
	{
		return -1;
	}

	//! Thread�� ���� �۵� ���� ��
	if ( m_strCommStateInfo.bFlag_Thread_Operate == true )
	{
		return -2;
	}

	//! RS-232C COM Port�� ������ �ʾ��� ��
	if ( m_strCommStateInfo.bFlag_Connected == false )
	{
		return -3;
	}

	if ( bFlag_Use_CPU_Affinity == true )
	{
		if ( piBuf_CPU_Use == NULL || iBufSize_CPU_Use <= 0 )
		{
			return -4;
		}
	}

	//! ���� ���� �ʱ�ȭ
	m_strCommStateInfo.InitInfo_Thread();
	m_pThread_RecvComm = NULL;
	m_bFlag_Stop = false;

	//! Thread ����
	m_pThread_RecvComm = AfxBeginThread(m_Thread_Receive_Comm, this, THREAD_PRIORITY_NORMAL, 0, 0);

	if ( m_pThread_RecvComm == NULL )
	{
		return -5;
	}

	//! Ư�� CPU Core���� �۵��ϴ� ����� ������� �ʴ´�.
	if ( bFlag_Use_CPU_Affinity == false )
	{
		return 1;
	}

	int iRet = Set_CPU_Affinity(m_strCommSetupInfo.aiCPU, intBufSize_RS232C_CPU_Core);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}

//! [���] ���߿� Thread �۵��� ����ϴ�. Thread�� ���ߴ� ���� ��ٸ��� �ʴ´�.
int CComm_RS232C::Stop_Thread()
{
	if ( m_pThread_RecvComm == NULL )
	{
		return -1;
	}

	//! Thread ���� Flag�� On�Ѵ�. 
	m_bFlag_Stop = true;

	m_pThread_RecvComm = NULL;

	return 1;
}

//! [���]   Thread�� ���ߴ� ���� ��ٸ���. 
//! [�Է°�] iTimeOut : ��� �ð��� msec ������ �Է��մϴ�. 
int CComm_RS232C::Stop_Thread_Waiting(int iTimeOut)
{
	if ( m_pThread_RecvComm == NULL )
	{
		return -1;
	}

	//! Thread�� �̹� ���߾� �ִ� ���
	if ( m_strCommStateInfo.bFlag_Thread_Operate == false )
	{
		return 1;
	}

	//! Thread ���� Flag�� On�Ѵ�. 
	m_bFlag_Stop = true;

	bool bRet = true;	//! true�̸� ���� �ð��ȿ� ���� ����� ����
	int iTimeCounter = 0, iWaitTime = 0;
	MSG msg;

	for ( iTimeCounter = iTimeOut;; )
	{
		if ( iTimeOut > 0 )
		{
			iWaitTime = iTimeOut;

			//! iTimeOut ���� �ʹ� ũ�� ���α׷��� ���߾� �����Ƿ�, 100 msec ��ŭ �߶� �ݺ������� ó���Ѵ�. 
			if ( iWaitTime > 100 )
			{
				iWaitTime = 100;
			}

			Sleep(iWaitTime);
		}
		else
		{
			iWaitTime = 0;
		}

		if ( m_strCommStateInfo.bFlag_Thread_Operate == false )
		{
			break;
		}

		//! ���α׷��� Holding���� �ʵ��� �޽��� ó��
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

			if ( msg.message == 0 )
			{
				break;
			}
		}

		if ( iTimeOut > 0 )
		{
			iTimeOut -= iWaitTime;	//! ��� �ð��� ���� ������. 

			if ( iTimeOut <= 0 )	//! �Է��� ��� �ð��� ������ ���
			{
				bRet = false;
				break;
			}
		}
		else
		{
			//! ��� �ð��� ���� ��� 
			bRet = false;
			break;
		}
	}

	if ( bRet == true )
	{
		m_pThread_RecvComm = NULL;

		TRACE(_T("CComm_RS232C::Stop_Thread_Waiting [%s] Stop OK \n"), m_strCommSetupInfo.sPortName);
	}
	else
	{
		TRACE(_T("CComm_RS232C::Stop_Thread_Waiting [%s] Stop Error \n"), m_strCommSetupInfo.sPortName);

		return -2;
	}

	return 1;
}

//! [���] ���� �����͸� ó���� Ŭ������ instance�� �����͸� �����Ѵ�. 
//! [�Է°�] pInput : Ŭ������ void ������ 
void CComm_RS232C::SetReceiveProcPtr(void* pInput)
{
	m_csReceiveProcPtr.Lock();
	m_pReceiveProcPtr = pInput;
	m_csReceiveProcPtr.Unlock();
}

//! [���] ���� Thread ��ü �Լ�
UINT m_Thread_Receive_Comm(LPVOID pParam)
{
	CComm_RS232C *pSerial = (CComm_RS232C*)pParam;

	if ( pSerial == NULL )
	{
		return 0;
	}
	if ( pSerial->m_hComm == NULL )
	{
		return 0;
	}

	int         iCnt_Recv_Action = 0;
	DWORD		dwEvent;
	OVERLAPPED	os;
	bool		bOK = true;
	BYTE		byRecvBuff[4096];		//! ���� ���� 
	DWORD		dwReceive = 0;			//! ������ Byte ũ�� 
	void		*pReceiveProcPtr = NULL;//! m_pReceiveProcPtr�� �����͸� �ӽ� ����, �۾� ���߿� �����Ͱ� ����Ǵ� �Ϳ� ����Ѵ�. 

	memset(&os, 0, sizeof(OVERLAPPED));

	os.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if ( os.hEvent == NULL )
	{
		bOK = false;
	}

	if ( ::SetCommMask(pSerial->m_hComm, EV_RXCHAR) == FALSE )
	{
		bOK = false;
	}

	if ( bOK == false )	//! Thread ���� ���� ����
	{
		return 0;
	}

	//! Thread�� ������ ���������� �����Ѵ�.
	pSerial->m_strCommStateInfo.bFlag_Thread_Operate = true;

	//! RS-232C ����� ����ǰ� �� ó�� 1���� ����ȴ�.
	pSerial->ReadyDevice();

	while(1)
	{
		if ( pSerial == NULL )
		{
			break;
		}
		if ( pSerial->IsConnected() == false )
		{
			break;
		}
		if ( pSerial->m_bFlag_Stop == true )		//! Thread�� �����.
		{
			break;
		}
		
		//! COM Port�� ������ �����Ͱ� ���⸦ ��ٸ���. 
		//! �� �������� Pending �ȴ�. 
		::WaitCommEvent( pSerial->m_hComm, &dwEvent, NULL );	

		//! COM Port�� �����Ͱ� ���ŵǾ��� �� 
		if ( (dwEvent & EV_RXCHAR) == EV_RXCHAR )
		{
			iCnt_Recv_Action = 0;

			//! COM Port���� ������ �ִ¸�ŭ �д´�. 
			do
			{
				if ( pSerial == NULL )
				{
					break;
				}
				if ( pSerial->IsConnected() == false )
				{
					break;
				}
				if ( pSerial->m_bFlag_Stop == true )	//! Thread�� �����.
				{
					break;
				}
								
				memset(byRecvBuff, 0x00, 4096);
				//! byRecvBuff�� ���ŵ� �����͸� �ӽ� �����Ѵ�. 
				dwReceive = pSerial->ReceiveComm( byRecvBuff, sizeof(byRecvBuff) );

				if (dwReceive > 0 && g_Comm_bReady_UV==false)
				{
					CString tmp;
					tmp.Format("%s", byRecvBuff);
					int index = tmp.Find("CNT1OK");

					if(index>=0)
					{
						g_Comm_bReady_UV = true;
					}
				}
				//! ���� ������ m_pReceiveProcPtr �����͸� �����´�. 
				pSerial->m_csReceiveProcPtr.Lock();
				pReceiveProcPtr = pSerial->m_pReceiveProcPtr;
				pSerial->m_csReceiveProcPtr.Unlock();

				if ( pSerial->m_strCommStateInfo.bFlag_Recv_Acting_In_Thread == true )
				{
					//! ���� �����͸� ó���ϴ� ���� ��

					Sleep(10);
					continue;
				}

				//! byRecvBuff�� �����͸� �����Ͽ�, CComm_RS232C Ŭ�������� ��ӹ��� Ŭ�������� ó���ϵ��� �Ѵ�. 
				if ( pReceiveProcPtr != NULL )
				{
					pSerial->m_strCommStateInfo.bFlag_Recv_Acting_In_Thread = true;

					if ( pSerial->m_strCommStateInfo.bFlag_Thread_Active == false )
					{
						//! �������� ������ �����Ѵ�. 
					}
					else
					{
	//					pSerial->ProcessReceiveData( pReceiveProcPtr, byRecvBuff, (int)dwReceive, pSerial->m_strCommStateInfo.bFlag_Thread_Active, iCnt_Recv_Action );
					}

					iCnt_Recv_Action++;

					pSerial->m_strCommStateInfo.bFlag_Recv_Acting_In_Thread = false;
				}

			}while(dwReceive);

			//! ���� ó���� ����ġ�� ������ ��û �۾�
// LHC			pSerial->PostProcRecvData();
		}
	}//! while(1)

	if ( os.hEvent != NULL )
	{
		::CloseHandle(os.hEvent);
	}

	//! Thread�� ����Ǿ����� Ȯ�� �����Ѵ�. 
	if ( pSerial != NULL )
	{
		pSerial->m_strCommStateInfo.bFlag_Thread_Operate = false;
	}

	return 1;
}

bool CComm_RS232C::IsConnected()
{
	return m_strCommStateInfo.bFlag_Connected;
}

CString CComm_RS232C::GetCurrent_PortName()
{
	return m_strCommSetupInfo.sPortName;
}

DWORD CComm_RS232C::GetCurrent_BaudRate()
{
	return m_strCommSetupInfo.dwBaudRate;
}

BYTE CComm_RS232C::GetCurrent_DataBit()
{
	return m_strCommSetupInfo.byDataBit;
}

BYTE CComm_RS232C::GetCurrent_StopBit()
{
	return m_strCommSetupInfo.byStopBit;
}

BYTE CComm_RS232C::GetCurrent_ParityBit()
{
	return m_strCommSetupInfo.byParityBit;
}

//! ��� ���� �Լ�
//! [���] RS-232C Serial ����� �ϱ� ���� ��� ��Ʈ�� ����. 
int CComm_RS232C::OpenPort(stringSetupInfo_RS232C *pstrSetupInfo)
{
	if ( pstrSetupInfo == NULL )
	{
		return -1;
	}

	if ( pstrSetupInfo->bFlag_Close_COM_Port == true )
	{
		if ( m_strCommStateInfo.bFlag_Connected == true )
		{
			//! ���� ��Ʈ�� ���� ������, �ݰ� �� �����Ѵ�. 
			ClosePort(pstrSetupInfo->iTimeOut_Close_Thread);
		}
	}
	else
	{
		if ( m_strCommStateInfo.bFlag_Connected == true )
		{
			return -2;
		}
	}

	CString sGetPortName = pstrSetupInfo->sPortName;

	sGetPortName.TrimLeft();
	sGetPortName.TrimRight();

	int iLen = sGetPortName.GetLength();

	//! �Է°� ��ȿ�� �˻�
	if ( iLen <= 0 )
	{
		return -3;
	}
	if ( pstrSetupInfo->dwBaudRate <= 0 )
	{
		return -4;
	}
	if ( pstrSetupInfo->byDataBit <= 0 )
	{
		return -5;
	}

	CString sPortName = _T("");

	if ( iLen > 4 )
	{
		//! COM10���� Ŭ ���
		sPortName = _T("\\\\.\\") + sGetPortName;
	}
	else
	{
		sPortName = sGetPortName;
	}

	//! Overlapped structure �ʱ�ȭ 
	m_osReceive.Offset = 0;
	m_osReceive.OffsetHigh = 0;
	m_osReceive.hEvent = NULL;

	m_osReceive.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if ( m_osReceive.hEvent == NULL )
	{
		ClosePort(pstrSetupInfo->iTimeOut_Close_Thread);
		return -6;
	}

	m_osSend.Offset = 0;
	m_osSend.OffsetHigh = 0;
	m_osSend.hEvent = NULL;

	m_osSend.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if ( m_osSend.hEvent == NULL )
	{
		ClosePort(pstrSetupInfo->iTimeOut_Close_Thread);
		return -7;
	}

	//! RS-232C ��Ʈ ���� 
	m_hComm = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if ( m_hComm == INVALID_HANDLE_VALUE )
	{
		DWORD dwError = ::GetLastError();
		ClosePort(pstrSetupInfo->iTimeOut_Close_Thread);
		return -8;
	}

	//! ��Ʈ ���� ���� 

	::SetCommMask(m_hComm, EV_RXCHAR);	//! EV_RXCHAR Event ���� (Data is received on input)

	::SetupComm(m_hComm, 4096, 4096);	//! InQueue, OutQueue ũ�� ���� 

	::PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);	//! COM Port ���� ���� ���� 

	//! Timeout ���� (Non-blocking ���� Read Timeout ����)
	// 1) Blocking reads, which will cause the 'Read' method to block
	//    until the requested number of bytes have been read. This is
	//    useful if you know how many data you will receive.
	// 2) Non-blocking reads, which will read as many bytes into your
	//    buffer and returns almost immediately. This is often the
	//    preferred setting.
	COMMTIMEOUTS timeOuts;
	timeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeOuts.ReadTotalTimeoutMultiplier = 0;
	timeOuts.ReadTotalTimeoutConstant = 0;
	timeOuts.WriteTotalTimeoutMultiplier = 2 * CBR_9600 / pstrSetupInfo->dwBaudRate;
	timeOuts.WriteTotalTimeoutConstant = 0;
	::SetCommTimeouts(m_hComm, &timeOuts);

	//! dcb ����, ��Ʈ�� �������� ��� ����ϴ� DCB ����ü�� ���� 
	DCB dcb;
	dcb.DCBlength = sizeof(DCB);

	::GetCommState(m_hComm, &dcb);	//! ���� ������ ���� �����´�. 

	//! the CTS (clear-to-send) signal 
	//! the DSR (data-set-ready) signal 
	//! The RTS (request-to-send) flow control

	//! �帧 ����
	switch(pstrSetupInfo->eFlowControl)
	{
	case EHandshakeHardwareAdd:
		{
			dcb.fOutxCtsFlow = TRUE;					// Enable CTS monitoring
			dcb.fOutxDsrFlow = TRUE;					// Enable DSR monitoring
			dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;	// Enable DTR handshaking
			dcb.fOutX = FALSE;							// Disable XON/XOFF for transmission
			dcb.fInX = FALSE;							// Disable XON/XOFF for receiving
			dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;	// Enable RTS handshaking
		}
		break;
	case EHandshakeSoftwareAdd:
		{
			dcb.fOutxCtsFlow = FALSE;					// Disable CTS (Clear To Send)
			dcb.fOutxDsrFlow = FALSE;					// Disable DSR (Data Set Ready)
			dcb.fDtrControl = DTR_CONTROL_DISABLE;		// Disable DTR (Data Terminal Ready)
			dcb.fOutX = TRUE;							// Enable XON/XOFF for transmission
			dcb.fInX = TRUE;							// Enable XON/XOFF for receiving
			dcb.fRtsControl = RTS_CONTROL_DISABLE;		// Disable RTS (Ready To Send)
		}
		break;	
	default:
		{
			//! 'EHandshakeOff' ����
			dcb.fOutxCtsFlow = FALSE;					// Disable CTS monitoring
			dcb.fOutxDsrFlow = FALSE;					// Disable DSR monitoring
			dcb.fDtrControl = DTR_CONTROL_DISABLE;		// Disable DTR monitoring
			dcb.fOutX = FALSE;							// Disable XON/XOFF for transmission
			dcb.fInX = FALSE;							// Disable XON/XOFF for receiving
			dcb.fRtsControl = RTS_CONTROL_DISABLE;		// Disable RTS (Ready To Send)
		}
		break;
	}//! switch(pstrSetupInfo->eFlowControl)
	
	//! If this member is TRUE, binary mode is enabled. 
	//! Windows does not support nonbinary mode transfers, so this member must be TRUE.
	dcb.fBinary = TRUE;
	//! The minimum number of bytes in use allowed in the input buffer before flow control is activated to allow transmission by the sender. 
	//! This assumes that either XON/XOFF, RTS, or DTR input flow control is specified in the fInX, fRtsControl, or fDtrControl members. 
	dcb.XonLim = 0;
	//! The minimum number of free bytes allowed in the input buffer before flow control is activated to inhibit the sender. 
	//! Note that the sender may transmit characters after the flow control signal has been activated, so this value should never be zero. 
	//! This assumes that either XON/XOFF, RTS, or DTR input flow control is specified in the fInX, fRtsControl, or fDtrControl members. 
	//! The maximum number of bytes in use allowed is calculated by subtracting this value from the size, in bytes, of the input buffer. 
	dcb.XoffLim = 0;
	//! If this member is TRUE, the communications driver is sensitive to the state of the DSR signal. 
	//! The driver ignores any bytes received, unless the DSR modem input line is high. 
	dcb.fDsrSensitivity = 0;
	//! If this member is TRUE, the driver terminates all read and write operations with an error status if an error occurs. 
	//! The driver will not accept any further communications operations until the application has acknowledged the error by calling the ClearCommError function. 
	dcb.fAbortOnError = 0; 

	//! �Լ� ���ڷ� �Է��� ��Ʈ ������ �Է� 
	dcb.BaudRate = pstrSetupInfo->dwBaudRate;
	dcb.ByteSize = pstrSetupInfo->byDataBit;
	dcb.StopBits = pstrSetupInfo->byStopBit;
	dcb.Parity   = pstrSetupInfo->byParityBit;

	//! ��Ʈ�� �� ������ dcb ����ü�� �ݿ� ��Ų��. 
	if ( ::SetCommState(m_hComm, &dcb) == FALSE )
	{
		ClosePort(pstrSetupInfo->iTimeOut_Close_Thread);
		return -9;
	}

	//! ������ ��� ������ ����
	m_strCommSetupInfo.Copy(pstrSetupInfo);

	//! �ʱ�ȭ
	m_strCommStateInfo.InitInfo();
	m_pThread_RecvComm = NULL;	

	//! Flag �� ����
	m_strCommStateInfo.bFlag_Connected = true;
	if ( m_strCommSetupInfo.bFlag_Recv_ThreadStart == false )
	{
		//! RS-232C ����� ����ǰ� �� ó�� 1���� ����ȴ�.
		ReadyDevice();

		return 1;
	}

	//! ���� Thread ����
	int iRet = StartThread(m_strCommSetupInfo.bFlag_Use_CPU_Affinity, m_strCommSetupInfo.aiCPU, intBufSize_RS232C_CPU_Core);
	if ( iRet < 0 )
	{
		return (-100 + iRet);
	}
	return 1;
}

//! �۽� �۾� �Լ�
//! [���]   ���� ������ ��Ʈ�� szSend�� ������ ����. 
//! [�Է°�] szSend : �۽��� ���ڿ�, bSTX_ETX : true�̸� �۽��Ϸ��� �������� ��, �ڷ� STX (0x02), ETX (0x03) ���ڸ� ���δ�. 
//! [��ȯ��] ������ �۽��� Byte ������ ��ȯ�Ѵ�. 
//! [����]   �۽��� ���ڿ��� ũ�⿡ ������ �ִ�. ����� ������, ���� �������� 8190�ڱ��� �۽��� �� �ִ�. 
DWORD CComm_RS232C::WriteComm(CString szSend, bool bSTX_ETX)
{
    int iStringSize = szSend.GetLength();
	if ( iStringSize <= 0 )
	{
		return 0;
	}
	
	const DWORD dwSendBufferSize = 8192;
	BYTE bySendBuffer[dwSendBufferSize];
	DWORD j, dwSendDataSize = (DWORD)iStringSize;

	//! �۽��� ���ڿ��� �ʹ� �� ��
	if ( dwSendBufferSize < dwSendDataSize )
	{
		return 0;
	}

	//! CString -> BYTE �迭 
	for ( j = 0; j < dwSendDataSize; j++ )
	{
		bySendBuffer[j] = (BYTE)(szSend.GetAt(j));
	}

	return WriteComm(bySendBuffer, dwSendDataSize, bSTX_ETX);
}

//! �۽� �۾� �Լ�
//! [���]   ���� ������ ��Ʈ�� pbyBuff�� ������ nSizeToReceive��ŭ ����. 
//! [�Է°�] pbyBuff : RS-232C Serial ������� �۽��� �����Ͱ� �ִ� ����, nSizeToSend : �۽��� �������� ũ��, ���� Byte ���� 
//!			 bSTX_ETX : true�̸� �۽��Ϸ��� �������� ��, �ڷ� STX (0x02), ETX (0x03) ���ڸ� ���δ�. 
//! [��ȯ��] ������ �۽��� Byte ������ ��ȯ�Ѵ�. 
//! [����]   �۽��� ���ڿ��� ũ�⿡ ������ �ִ�. ����� ������, ���� �������� 8190�ڱ��� �۽��� �� �ִ�. 
DWORD CComm_RS232C::WriteComm(BYTE *pbyBuff, DWORD nSizeToSend, bool bSTX_ETX)
{
	DWORD dwWritten = 0, dwError = 0, dwErrorFlags = 0;
	COMSTAT comstat;

	if ( m_hComm == NULL )	//! ��Ʈ�� ��ȿ�� �˻� 
	{
		return 0;
	}
	if ( pbyBuff == NULL || nSizeToSend <= 0 )	//! �۽� �������� ��ȿ�� �˻� 
	{
		return 0;
	}
	if ( m_strCommStateInfo.bFlag_Connected == false )//! ��Ʈ�� ���� ���� ���� ������ �� 
	{
		return 0;
	}

	if ( m_strCommStateInfo.bFlag_Sending == true )	//! ���� �۽� �۾� ���� �� 
	{
		return 0;
	}

	m_strCommStateInfo.bFlag_Sending = true;

	const DWORD dwSendBuffSize = 8192;
	BYTE bySendBuff[dwSendBuffSize];
	DWORD i, dwSendSize = nSizeToSend;

	if ( bSTX_ETX == true )
	{
		dwSendSize += 2;
	}
	
	if ( dwSendSize > dwSendBuffSize )	//! �۽��Ϸ��� �����Ͱ� ������ ũ�� ���� ũ��. 
	{
		m_strCommStateInfo.bFlag_Sending = false;
		return 0;
	}

	if ( bSTX_ETX == true )
	{
		bySendBuff[0] = STX;
		for ( i = 0; i < nSizeToSend; i++ )
		{
			bySendBuff[i + 1] = pbyBuff[i];
		}
		bySendBuff[i + 1] = ETX;
	}
	else
	{
		for ( i = 0; i < dwSendSize; i++ )
		{
			bySendBuff[i] = pbyBuff[i];
		}
	}

	if ( ::WriteFile( m_hComm, bySendBuff, dwSendSize, &dwWritten, &m_osSend ) == FALSE )
	{
		//! ���� �۽��� ���ڰ� ������ ���
		if ( ::GetLastError() == ERROR_IO_PENDING )
		{
			//! ���� ���ڰ� ���� �ְų� ������ ���ڰ� ���� ���� ��� Overapped IO�� Ư���� ���� 
			//! ERROR_IO_PENDING ���� �޽����� ���޵ȴ�. 
			//! �̶��� timeouts�� ������ �ð���ŭ ��ٷ��ش�. 
			while( ::GetOverlappedResult( m_hComm, &m_osSend, &dwWritten, TRUE ) == FALSE )
			{
				dwError = ::GetLastError();
				if ( dwError != ERROR_IO_INCOMPLETE )
				{
					::ClearCommError( m_hComm, &dwErrorFlags, &comstat );
					break;
				}
			}
		}
		else
		{
			dwWritten = 0;
			::ClearCommError( m_hComm, &dwErrorFlags, &comstat );
		}
	}

	m_strCommStateInfo.bFlag_Sending = false;
	
	//! ��Ʈ�� ������ �۽ŵ� ������ ��ȯ�Ѵ�. 
	return dwWritten;
}

//! ���� �۾� �Լ�
//! [���]   ������ ��Ʈ�κ��� nSizeToReceive��ŭ�� �����͸� �о pbyBuff�� �����Ѵ�. 
//! [�Է°�] pbyBuff : RS-232C Serial ������� ������ �����͸� ���� ����, 
//!			 nSizeToReceive : ���ŵ� �����Ϳ��� �о�鿩 pbyBuff�� ������ �������� ũ��, ���� Byte ���� 
//! [��ȯ��] ������ ���ŵǾ� ������ Byte ������ ��ȯ�Ѵ�. 
DWORD CComm_RS232C::ReceiveComm(BYTE *pbyBuff, DWORD nSizeToReceive)
{
//! [���� ����] dwReaded���� 0���� �ʱ�ȭ���� ������ Release Static ������ �������� ������ �߻��Ѵ�. 
	DWORD	dwRead = 0, dwReaded = 0, dwError = 0, dwErrorFlags = 0;
	COMSTAT	comstat;

	if ( m_hComm == NULL )	//! ��Ʈ�� ��ȿ�� �˻� 
	{
		return 0;
	}
	if ( pbyBuff == NULL || nSizeToReceive <= 0 )	//! ���� ������ ������ ��ȿ�� �˻� 
	{
		return 0;
	}
	if ( m_strCommStateInfo.bFlag_Connected == false )	//! ��Ʈ�� ���� ���� ���� ������ �� 
	{
		return 0;
	}

	//! System Queue�� ������ Byte ������ �̸� �д´�. 
	::ClearCommError( m_hComm, &dwErrorFlags, &comstat );
	dwRead = comstat.cbInQue;

	if ( dwRead > 0 )	//! System Queue�� ���ŵ� �����Ͱ� ���� �� 
	{
		if ( ::ReadFile( m_hComm, pbyBuff, nSizeToReceive, &dwReaded, &m_osReceive ) == FALSE )
		{
			//! ������ �����Ͱ� ���� ������ �� 
			if ( ::GetLastError() == ERROR_IO_PENDING )
			{
				//! timeouts�� ������ �ð���ŭ ��ٷ��ش�. 
				while( ::GetOverlappedResult( m_hComm, &m_osReceive, &dwReaded, TRUE ) == FALSE )
				{
					dwError = ::GetLastError();
					if ( dwError != ERROR_IO_INCOMPLETE )
					{
						::ClearCommError( m_hComm, &dwErrorFlags, &comstat );
						break;
					}
				}
			}
			else
			{
				dwRead = 0;
				::ClearCommError( m_hComm, &dwErrorFlags, &comstat );
			}
		}
	}

	//! ������ �о���� Byte ������ ��ȯ�Ѵ�. 
	return dwReaded;
}

//! ��� ��ħ �Լ�
void CComm_RS232C::ClosePort()
{
	ClosePort(m_strCommSetupInfo.iTimeOut_Close_Thread);
}

//! ��� ��ħ �Լ�
//! [���]   RS-232C Serial ��� ��Ʈ�� �ݴ´�. Thread, Event ���� �����Ѵ�. 
//! [�Է°�] iTimeOut : ���� Thread �����ϴ� �۾��� ��� �ð�, ���� msec 
void CComm_RS232C::ClosePort(int iTimeOut)
{
	//! ���� Thread �۵��� ������Ű�� ���� Flag ����
	m_bFlag_Stop = true;

	if ( m_hComm != NULL )
	{
		//! Mask ����, ����ϴ� WaitCommEvent�� ����
		::SetCommMask(m_hComm, 0);
		//! ��Ʈ ����, ��� ���� �����͸� ����ش�. 
		::PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	}

	int iRet = Stop_Thread_Waiting(iTimeOut);

	if ( m_osSend.hEvent != NULL )
	{
		::CloseHandle(m_osSend.hEvent);
		m_osSend.hEvent = NULL;
	}

	if ( m_osReceive.hEvent != NULL )
	{
		::CloseHandle(m_osReceive.hEvent);
		m_osReceive.hEvent = NULL;
	}

	if ( m_hComm != NULL )
	{		
		//! �ڵ� �ݱ� 
		::CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	m_strCommStateInfo.bFlag_Connected = false;
}
void CComm_RS232C::ReadyDevice()
{
	// �ʿ���� LHC
}