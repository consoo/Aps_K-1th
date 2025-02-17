#include "StdAfx.h"
#include "LightControl.h"

CLightControl::CLightControl(void)
{
	m_iRecvSize = 0;
	memset(m_acRecvStorage, 0, sizeof(m_acRecvStorage));
		
	m_iNo_Machine = -1;
}


CLightControl::~CLightControl(void)
{
	Close_Device();
}

//! RS-232C 통신이 연결된 후에 맨 처음 1번 만 실행해야 할 기능을 구현하다. 
void CLightControl::ReadyDevice()
{
}

//! 수신 처리 본체 함수
void CLightControl::ProcessReceiveData(void *pProcPtr, BYTE *pbyBuff, int iSize, bool bFlag_Active, int iCnt_Recv)
{
	if ( m_iNo_Machine < 0 )
	{
		return;
	}
	if ( pProcPtr == NULL || pbyBuff == NULL || iSize <= 0 )
	{
		return;
	}
	
	TCHAR acRecvTmp[8192];
	memset(acRecvTmp, 0, sizeof(acRecvTmp));
	_tcscpy_s(acRecvTmp, m_acRecvStorage);	//! 저번 함수 호출 때에 남은 데이터를 가져온다. 

	int iRecvCnt = m_iRecvSize;
	if ( iRecvCnt < 0 )
	{
		iRecvCnt = 0;
	}

	int i = 0;
	for ( i = 0; i < iSize; i++ )
	{
		if ( (pbyBuff[i] == ']')  && (iRecvCnt == 0) )		//! 시작
		{
			memset(acRecvTmp, 0, sizeof(acRecvTmp));
			iRecvCnt = 0;			
		}
		else if ( (pbyBuff[i] == ']') && (iRecvCnt > 0) )	//! 마침
		{
			acRecvTmp[iRecvCnt] = 0x00;
			iRecvCnt++;

			if ( bFlag_Active == true )
			{
				//! 준비된 수신 문자열을 실제로 사용한다. 
				OutputRecvData(pProcPtr, acRecvTmp, iRecvCnt, iCnt_Recv);
			}

			memset(acRecvTmp, 0, sizeof(acRecvTmp));
			iRecvCnt = 0;			
		}
		else
		{
			acRecvTmp[iRecvCnt] = (TCHAR)(pbyBuff[i]);	//! pbyBuff -> acRecvTmp 버퍼로 데이터를 옮긴다. 
			iRecvCnt++;
		}
	}

	acRecvTmp[iRecvCnt] = 0x00;
	m_iRecvSize = iRecvCnt;

	memset(m_acRecvStorage, 0, sizeof(m_acRecvStorage));
	_tcscpy_s(m_acRecvStorage, acRecvTmp);	//! 처리하고 남은 데이터를 다음 함수 호출때를 위해 저장해 둔다. 
}

//! [기능] 수신 받은 문자열을 Parsing하거나, 다른 곳에 전달한다.  
void CLightControl::OutputRecvData(void *pProcPtr, TCHAR *pcBuff, int iDataSize, int iCnt_Recv)
{
	//! TCHAR* -> CString 
	CString sRecv = _T("");
	sRecv.Format(_T("%s"), pcBuff);

	int iStringLength = (int)(sRecv.GetLength());
	
	if ( pProcPtr != NULL )
	{
	}
}

//! 수신 처리 후 후처리 함수
void CLightControl::PostProcRecvData()
{
}

//! 조명 컨트롤러에 연결
//! [주의 사항] 수신 데이터가 있을 경우에는, 반드시, SetReceiveProcPtr 함수로 수신 처리를 할 Dialog나 GUI 클래스를 설정하고, 이 함수를 호출한다. 
bool CLightControl::Connect_Device(CString sPort, int iNoMachine)
{
	if ( iNoMachine < 0 )
	{
		return false;
	}
	/*if ( sPort.GetLength() < 8 )
	{
		return false;
	}*/
	if ( sPort.Find(_T("COM"), 0) < 0 )
	{
		return false;
	}

	strSetupInfo_RS232C strSerial;
	strSerial.InitInfo();
	strSerial.dwBaudRate   = CBR_19200;
	strSerial.byDataBit    = 8;
	strSerial.byStopBit = ONESTOPBIT;
	strSerial.byParityBit  = NOPARITY;
	strSerial.sPortName    = sPort;
	strSerial.eFlowControl = EHandshakeOff;
	strSerial.bFlag_Close_COM_Port   = true;
	strSerial.bFlag_Use_CPU_Affinity = false;
	strSerial.bFlag_Recv_ThreadStart = false;	//! 수신 데이터 처리용 Thread를 생성하지 않음
	strSerial.iTimeOut_Close_Thread  = 3000;
	int iReturn = OpenPort(&strSerial);

	if ( iReturn < 0 )
	{
		return false;
	}

	m_iNo_Machine = iNoMachine;
			

	return true;
}

//! 연결 해제
void CLightControl::Close_Device()
{
	ClosePort();

	m_iNo_Machine = -1;
}

//! 문자열 직접 전송
int CLightControl::SendData_Light_Controller(BYTE *pbySend, int iSendSize)
{
	if ( pbySend == NULL || iSendSize <= 0 )
	{
		return -1;
	}
	if ( m_iNo_Machine < 0 )
	{
		return -2;
	}
	
	BYTE bySendBuf[256];
	memset(bySendBuf, 0x00, sizeof(bySendBuf));

	int i = 0;
	DWORD dwSendCnt = 0;
	
	//bySendBuf[dwSendCnt++] = 0x0D;
	for ( i = 0; i < iSendSize; i++ )
	{
		bySendBuf[dwSendCnt++] = pbySend[i];
	}
	//bySendBuf[dwSendCnt++] = 0x0D;

	DWORD dwRet = WriteComm(bySendBuf, dwSendCnt);
	if ( dwRet < dwSendCnt )
	{
		return -3;
	}

	return 1;
}

//! 문자열 직접 전송
int CLightControl::SendData_Light_Controller(CString sSend)
{
	int iStringSize = sSend.GetLength();
	if ( iStringSize <= 0 )
	{
		return -1;
	}

	BYTE bySendBuffer[256];
	memset(bySendBuffer, 0x00, sizeof(bySendBuffer));

	int i = 0;
	for ( i = 0; i < iStringSize; i++ )
	{
		bySendBuffer[i] = (BYTE)(sSend.GetAt(i));
	}

	int iRet = SendData_Light_Controller(bySendBuffer, iStringSize);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}

//! Channel On/Off
//! [주의 사항] 'iNoChannel' 값은 조명 컨트롤러의 실제 채널 입력값이다. 
int CLightControl::SetChannel_OnOff(int iNoChannel, bool bSwitch_On)
{
	CString sSend = _T("");
	sSend.Format(_T("]%02d"), iNoChannel);

	if ( bSwitch_On == true )
	{
		sSend += _T("1");
	}
	else
	{
		sSend += _T("0");
	}

	int iRet = SendData_Light_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}

//! 각 Channel의 밝기값(PWM값) 조절
//! [주의 사항] 'iNoChannel' 값은 조명 컨트롤러의 실제 채널 입력값이다. 
int CLightControl::SetChannel_Value(int iNoChannel, int iValue)
{
	if ( iValue < 0 )
	{
		iValue = 0;
	}
	if ( iValue > 255 )
	{
		iValue = 255;
	}

	CString sSend = _T("");
	sSend.Empty();
	sSend.Format(_T("[%02d%03d"), iNoChannel, iValue);//sSend.Format(_T("[%02d%03d"), iNoChannel, iValue);

	int iRet = SendData_Light_Controller(sSend);
	if ( iRet < 0 )
	{ 
		return (-10 + iRet);
	}

	return 1;
}

void CLightControl::ctrlLedVolume(int iChannel, int iValue)
{
	int chNo = 0;
#ifdef USE_LED_CTRL_BACKDU_____
    chNo = iChannel + 1;
    SetChannel_Value(chNo, iValue); 
#else
	chNo = iChannel + 1;
	SetChannel_OnOff(chNo, true);
	Sleep(5);
	SetChannel_Value(chNo, iValue);
#endif
}

bool CLightControl::allControl(int iChannel , bool onOff)
{
	byte sendData[100];
	memset(sendData, 0x00, sizeof(sendData));
	CString temp = _T("");
	temp.Format("%X", 255);
	sendData[0] = 0x02;
	sendData[1] = 0x50;
	//
	if (onOff == true)
	{
		sendData[2] = model.m_iLedValue[LEDDATA_TOP1_CHART];
		sendData[3] = model.m_iLedValue[LEDDATA_TOP1_CHART];
		sendData[4] = model.m_iLedValue[LEDDATA_TOP1_CHART];
		sendData[5] = model.m_iLedValue[LEDDATA_TOP1_CHART];
	}
	
	//
	sendData[6] = 0x03;
	int iRet = SendData_Light_Controller(sendData , 7);
	if (iRet < 0)
	{
		return false;
	}

	return true;
}
