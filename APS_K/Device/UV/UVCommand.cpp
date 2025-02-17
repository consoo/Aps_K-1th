#include "StdAfx.h"
#include "UVCommand.h"

CUVCommand::CUVCommand(void)
{
	m_iRecvSize = 0;
	memset(m_acRecvStorage, 0, sizeof(m_acRecvStorage));
		
	m_iNo_Machine = -1;

	m_iMode_Control = 0;
	m_iMode_Program = 1;
	m_dOutputIntensity = 0.;

	//! [���� ����] �ݵ��, '%'�� ������ �����Ͽ� ����Ѵ�. 
	m_iUnit_Output_Intensity = 1;

	m_bFlag_Request_INP = false;

	m_bFlag_Ready = false;
	m_bFlag_Stable = false;

	Init_Buffer_Input_Pin();
}


CUVCommand::~CUVCommand(void)
{
	//! UV Lamp RS-232C ��� ����
	Close_Device();
}

//! [���] Serial RS-232C ��� ����
//! [���� ����] �ݵ��, SetReceiveProcPtr �Լ��� ���� ó���� �� Dialog�� GUI Ŭ������ �����ϰ�, �� �Լ��� ȣ���Ѵ�. 
bool CUVCommand::Connect_Device(int iPort)
{
	CString sPort;
	sPort.Format("COM%d", iPort);

	if ( sPort.GetLength() < 4 )
	{
		return false;
	}
	if ( sPort.Find(_T("COM"), 0) < 0 )
	{
		return false;
	}

	//! �ʱ�ȭ
	Init_Buffer_Input_Pin();

	m_bFlag_Ready = false;
	m_bFlag_Stable = false;
	//m_iNo_Machine = iNoMachine;

	//! �Ŵ��� 32 �������� ���뿡 ���� �Է�
	strSetupInfo_RS232C strSerial;
	strSerial.InitInfo();
	strSerial.dwBaudRate = CBR_9600;
	strSerial.byDataBit    = 8;
	strSerial.byStopBit    = ONESTOPBIT;
	strSerial.byParityBit  = NOPARITY;
	strSerial.sPortName    = sPort;
	strSerial.eFlowControl = EHandshakeOff;
	strSerial.bFlag_Close_COM_Port   = true;
	strSerial.bFlag_Use_CPU_Affinity = false;
	strSerial.bFlag_Recv_ThreadStart = true;
	strSerial.iTimeOut_Close_Thread  = 3000;
	int iReturn = OpenPort(&strSerial);

	if ( iReturn < 0 )
	{

		return false;
	}	
	
	return true;
}

//! [���] Serial RS-232C ��� ��ħ
void CUVCommand::Close_Device()
{	
	//! ������ ���� ���ʹ� �ݰ�, Lamp�� ���д�. 
	UV_Shutter_Close();
	UV_Lamp_Off();

	//! UV Lamp Front Panel���� �����ϴ� ������� ����
	Set_Control_Mode_Front_Panel();

	//! RS-232C ����� ��ģ��. 
	ClosePort();

	m_iNo_Machine = -1;
	m_bFlag_Ready = false;
	m_bFlag_Stable = false;
}

//! ���� ó�� Thread�� �����ϰ� ����, ��ó�� 1���� ����ȴ�. 
 void CUVCommand::ReadyDevice()
 {
	 int iRet = 0;

	 //! PC���� RS-232C ������� �����ϴ� ������� ����
	 iRet = Set_Control_Mode_RS232C();
	 Sleep(50);
	 //! Output Intensity�� RS-232C�� ���� �����ϱ� ���Ͽ� 'Step 1'�� �����Ѵ�.
	 iRet = Set_Program_Mode_1_Step();
	 Sleep(50);
	 //! UV ���� ���⸦ �����Ѵ�. 
	// iRet = Set_Output_Intensity_1_Step(model.UV_Intensity);
	 Sleep(50);
	 //Time ����
	 iRet = Set_Shutter_AutoTime_1_Step(model.UV_Time/1000);
	 Sleep(50);

	 //! ������ ���� ���ʹ� �ݵ�� �ݾƵд�. 
	 UV_Shutter_Close();
	 Sleep(50);
	 //! UV Lamp�� �Ҵ�. Lamp ����ȭ�� 5�� ������ �ɸ��Ƿ�, ���α׷��� ������ ���� Lamp�� �ѵд�. 
	 iRet = UV_Lamp_On();
	 Sleep(50);
	 //! ���� ��ġ�� ���¸� �˾Ƴ���. 
	 iRet = Ask_Input_Pin();

	 //! UV Lamp �⺻���� �۵� �غ� �Ϸ�
	 m_bFlag_Ready = true;
 }

 //! ���� ó���� ��ĥ�� ����, 1ȸ ����ȴ�. 
 void CUVCommand::PostProcRecvData()
 {
	 /*if ( m_bFlag_Request_INP == true )
	 {
		 Ask_Output_Intensity();

		 m_bFlag_Request_INP = false;
	 }*/
 }

//! [���] RS-232C Serial ������� ���� ���ڿ��� �޾�, STX, STX ���ڸ� ���� ���ڷ� �Ͽ�, 
//!		   1���� ���ڿ��� �����Ͽ� OutputRecvData �Լ��� �����Ѵ�. 
//! [���� ����] 
//!       ������, UV ��ġ�� STX, ETX ���� ���ڸ� �������� �����Ƿ�, �����Ѵ�. 
//!	      ���� ���ڿ����� '0x0D'�� ������ ���������, 1���� ���ڿ� �������� ������ �ִ�.
//!       Front Panel ���� ����̸�, ������ 'ERR' ���ڿ��� ���� �ȴ�. ��, 'CNTQ'������ 'ERR' �ڿ� 'CNT0' ���ڿ��� �´�. 
void CUVCommand::ProcessReceiveData(void *pProcPtr, BYTE *pbyBuff, int iSize, bool bFlag_Active, int iCnt_Recv)
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
	_tcscpy_s(acRecvTmp, m_acRecvStorage);	//! ���� �Լ� ȣ�� ���� ���� �����͸� �����´�. 

	int iRecvCnt = m_iRecvSize;
	if ( iRecvCnt < 0 )
	{
		iRecvCnt = 0;
	}

	int i = 0;
	for ( i = 0; i < iSize; i++ )
	{
		if ( (pbyBuff[i] == UV_STX)  && (iRecvCnt == 0) )		//! ����
		{
			memset(acRecvTmp, 0, sizeof(acRecvTmp));
			iRecvCnt = 0;			
		}
		else if ( (pbyBuff[i] == UV_ETX) && (iRecvCnt > 0) )	//! ��ħ
		{
			acRecvTmp[iRecvCnt] = 0x00;
			iRecvCnt++;

			if ( bFlag_Active == true )
			{
				//! �غ�� ���� ���ڿ��� ������ ����Ѵ�. 
				OutputRecvData(pProcPtr, acRecvTmp, iRecvCnt, iCnt_Recv);
			}

			memset(acRecvTmp, 0, sizeof(acRecvTmp));
			iRecvCnt = 0;			
		}
		else
		{
			acRecvTmp[iRecvCnt] = (TCHAR)(pbyBuff[i]);	//! pbyBuff -> acRecvTmp ���۷� �����͸� �ű��. 
			iRecvCnt++;
		}
	}

	acRecvTmp[iRecvCnt] = 0x00;
	m_iRecvSize = iRecvCnt;

	memset(m_acRecvStorage, 0, sizeof(m_acRecvStorage));
	_tcscpy_s(m_acRecvStorage, acRecvTmp);	//! ó���ϰ� ���� �����͸� ���� �Լ� ȣ�⶧�� ���� ������ �д�. 	
}

//! [���] ���� ���� ���ڿ��� Parsing�ϰų�, �ٸ� ���� �����Ѵ�. �� �Լ��� �Էµ� ���ڿ��� STX, ETX ������ ���̴�. 
void CUVCommand::OutputRecvData(void *pProcPtr, TCHAR *pcBuff, int iDataSize, int iCnt_Recv)
{
	//! TCHAR* -> CString 
	CString sRecv = _T("");
	sRecv.Format(_T("%s"), pcBuff);

	int iStringLength = (int)(sRecv.GetLength());
	
	/*if ( pProcPtr != NULL )
	{
		((CAABonderDlg*)pProcPtr)->Parse_From_UV_Lamp(sRecv, iCnt_Recv, m_iNo_Machine);
	}*/
}

//! [���] UV Controller�� �����͸� �����Ѵ�. 
//! [�Է°�] pbySend : ���� ������, iSendSize : ���� �������� ũ��
//! [��ȯ��] ���ۿ� �����ϸ� '1'�� ��ȯ�ϰ�, �׷��� ������ ������ ��ȯ�Ѵ�. 
//! [���� ����] STX, ETX ���ڸ� ������ ���� �����͸� �Է����� �ʴ´�. STX, ETX ���ڴ� �� �Լ����� �ڵ����� ���Խ�Ų��. 
int CUVCommand::SendData_UV_Controller(BYTE *pbySend, int iSendSize)
{	
	if ( pbySend == NULL || iSendSize <= 0 )
	{
		return -1;
	}
	
	BYTE bySendBuf[256];
	memset(bySendBuf, 0x00, sizeof(bySendBuf));

	int i = 0;
	DWORD dwSendCnt = 0;

	//! ���� UV Lamp ��ġ���� �׽�Ʈ�� �� ���, ���� ���ڸ� ���� 1���� �ٿ��� �Ѵ�. (2013/5/3)
	//! �տ� '0x0D'�� ������, 'ERR' ���ڿ��� ���ŵȴ�. 

	//bySendBuf[dwSendCnt++] = 0x0D;
	for ( i = 0; i < iSendSize; i++ )
	{
		bySendBuf[dwSendCnt++] = pbySend[i];
	}
	bySendBuf[dwSendCnt++] = 0x0D;

	DWORD dwRet = WriteComm(bySendBuf, dwSendCnt);
	if ( dwRet < dwSendCnt )
	{
		return -3;
	}

	return 1;
}

//! [���] UV Controller�� �����͸� �����Ѵ�. 
//! [�Է°�] sSend : ���� ������, ���� ������ ũ��� ���ο��� ó���Ѵ�. 
//! [��ȯ��] ���ۿ� �����ϸ� '1'�� ��ȯ�ϰ�, �׷��� ������ ������ ��ȯ�Ѵ�. 
//! [���� ����] STX, ETX ���ڸ� ������ ���� �����͸� �Է����� �ʴ´�. STX, ETX ���ڴ� �� �Լ����� �ڵ����� ���Խ�Ų��. 
int CUVCommand::SendData_UV_Controller(CString sSend)
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

	int iRet = SendData_UV_Controller(bySendBuffer, iStringSize);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}

void CUVCommand::Set_Flag_UV_Lamp_Stable()
{
	m_bFlag_Stable = true;
}

//! UV Lamp�� �⺻���� ���� �غ� ��ġ�� true�� ��ȯ
bool CUVCommand::IsReady()
{
	if ( m_iNo_Machine < 0 )
	{
		return false;
	}

	return m_bFlag_Ready;
}

//! UV Lamp�� ����ȭ�Ǹ� true�� ��ȯ
bool CUVCommand::IsStable_UV_Lamp()
{
	if ( IsReady() == false )
	{
		return false;
	}

	return m_bFlag_Stable;
}

int CUVCommand::Set_Control_Mode_Front_Panel()
{
	CString sSend = _T("");

	sSend = _T("CNT0");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Set_Control_Mode_RS232C()
{
	CString sSend = _T("");

	sSend = _T("CNT1");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Ask_Control_Mode()
{
	CString sSend = _T("");

	sSend = _T("CNTQ");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}

int CUVCommand::Set_Program_Mode_1_Step()
{
	CString sSend = _T("");

	sSend = _T("PROG_1STEP");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Set_Program_Mode_7_Steps()
{
	CString sSend = _T("");

	sSend = _T("PROG_7STEP");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Ask_Program_Mode()
{
	CString sSend = _T("");
#ifdef	ON_LINE_UV_LAMP
	sSend = _T("PROG?");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	for (int i=0; i<10; i++)
	{
		::Sleep(20);
		checkMessage();
	}

	if (iRet == 0)
	{
		return -1;
	}
#endif

	return 1;
}
	
int CUVCommand::UV_Lamp_On()
{
	CString sSend = _T("");

	sSend = _T("L1");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::UV_Lamp_Off()
{
	CString sSend = _T("");

	sSend = _T("L0");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::UV_Shutter_PowerSet(int value)
{
	CString sSend = _T("");

	sSend.Format(":EP,%d\n", value);
	int iRet = SendData_UV_Controller(sSend);
	if (iRet < 0)
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::UV_Shutter_Open()
{
	CString sSend = _T("");

	sSend = _T(":EONT\n");
	//sSend.AppendFormat("%d\n", value);
	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::UV_Shutter_Close()
{
	CString sSend = _T("");

	//sSend = _T("S0");
	sSend = _T(":E0\n");
	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}

int CUVCommand::Set_Shutter_AutoTime_1_Step(double fi_dTime)
{
	CString sSend = _T("");

	if ( fi_dTime < 0 || fi_dTime > 999.9 )
	{
		return -1;
	}

	CString sFormat = _T(""), sNumber = _T("");
	sFormat = _T("%05.1f");
	sNumber.Format(sFormat, fi_dTime);
	sSend.Format(_T("AT%s"), sNumber);

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Ask_Shutter_AutoTime_1_Step(double *fo_pdTime)
{
	if ( fo_pdTime == NULL )
	{
		return -1;
	}

	CString sSend = _T("");

	sSend = _T("ATQ");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}

int CUVCommand::Start_Shutter_Auto_Operation_1_Step()
{
	CString sSend = _T("");

	sSend = _T("ATS");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Ask_RemainTime_Shutter_Auto_Operation_1_Step(double *fo_pdTime)
{
	if ( fo_pdTime == NULL )
	{
		return -1;
	}

	CString sSend = _T("");

	sSend = _T("ATT");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}

int CUVCommand::Set_Diaphragm_Stop()
{
	CString sSend = _T("");

	sSend = _T("INT0");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Set_Diaphragm_Up()
{
	CString sSend = _T("");

	sSend = _T("INT1");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Set_Diaphragm_Down()
{
	CString sSend = _T("");

	sSend = _T("INT2");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Set_Diaphragm_Up_1_Point()
{
	CString sSend = _T("");

	sSend = _T("INT3");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Set_Diaphragm_Down_1_Point()
{
	CString sSend = _T("");

	sSend = _T("INT4");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}

int CUVCommand::Set_Output_Intensity_1_Step(double fi_dInput)
{
	CString sSend = _T("");

	CString sFormat = _T(""), sNumber = _T("");
	
	if ( m_iUnit_Output_Intensity == 1 )
	{
		//! ���� %

		if ( fi_dInput < 0. )
		{
			fi_dInput = 0.;
		}
		if ( fi_dInput > 100. )
		{
			fi_dInput = 100.;
		}
		
		sNumber.Format(_T("%03d"), (int)fi_dInput);
	}
	else
	{
		//! ���� W

		if ( fi_dInput < 0. )
		{
			fi_dInput = 0.;
		}
		
		sFormat = _T("%3.2f");
		sNumber.Format(sFormat, fi_dInput);
	}

	sSend.Format(_T("INTSET%s"), sNumber);

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Ask_Output_Intensity()
{
	CString sSend = _T("");

	sSend = _T("INTQ");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}

int CUVCommand::Reset_UV_Lamp_Operation_Time()
{
	CString sSend = _T("");

	sSend = _T("LAMPCLR");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Ask_UV_Lamp_Operation_time()
{
	CString sSend = _T("");

	sSend = _T("LIF");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}

int CUVCommand::Set_Cure_Program_Info_7_Steps(int fi_iNoProgram, sreSetupInfo_UV_Program *fi_pstrInput)
{	
	if ( fi_iNoProgram < 1 || fi_iNoProgram > 9 )
	{
		return -1;
	}
	if ( fi_pstrInput == NULL )
	{
		return -2;
	}

	int iRet = fi_pstrInput->IsValid();

	if ( iRet < 0 )
	{
		return (-1000 + iRet);
	}

	CString sSend = _T("");
	CString sParameter = _T("");
	CString sFormat = _T(""), sNumber = _T(""), sTime = _T("");
	int i = 0;

	for ( i = 0; i < iBufSize_Step_In_Cure; i++ )
	{
		if ( fi_pstrInput->iUnit_Output_Intensity == 1 )
		{
			//! %
			sNumber.Format(_T("%03d"), (int)(fi_pstrInput->adIntensity[i]));
		}
		else
		{
			//! W
			sFormat = _T("%3.2f");
			sNumber.Format(sFormat, fi_pstrInput->adIntensity[i]);
		}

		sFormat = _T("%05.1f");		
		sTime.Format(sFormat, fi_pstrInput->adTime[i]);

		sParameter += sNumber;
		sParameter += _T(",");

		sParameter += sTime;
		sParameter += _T(",");
	}//! for ( i = 0; i < iBufSize_Step_In_Cure; i++ )

	sParameter.TrimRight(_T(","));

	sSend.Format(_T("CURE%d%d%d:%s"), fi_iNoProgram, fi_pstrInput->iModel_Shutter, fi_pstrInput->iUnit_Output_Intensity, sParameter);

	iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Ask_Cure_Program_Info_7_Steps(int fi_iNoProgram)
{
	if ( fi_iNoProgram < 1 || fi_iNoProgram > 9 )
	{
		return -1;
	}

	CString sSend = _T("");

	sSend.Format(_T("CUREQ%d"), fi_iNoProgram);

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Start_Cure_Program_7_Steps(int fi_iNoProgram)
{
	if ( fi_iNoProgram < 1 || fi_iNoProgram > 9 )
	{
		return -1;
	}

	CString sSend = _T("");

	sSend.Format(_T("START%d"), fi_iNoProgram);

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Stop_Cure_Program_7_Steps()
{	
	CString sSend = _T("");

	sSend = _T("STOP");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}
int CUVCommand::Ask_Cure_Program_Progress_7_Steps()
{	
	CString sSend = _T("");

	sSend = _T("STPQ");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}

//! 'INP' ���� Flag ���� �ʱ�ȭ
//! RS-232C ��� �����ؼ��� ������ ����, ����� ���Ǹ� ���� Flag ������ ����ϱ� ���� (Ư��, �ߺ� ó�� ����)
void CUVCommand::Init_Buffer_Input_Pin()
{
	int i = 0;
	for ( i = 0; i < iBufSize_INP_Bit; i++ )
	{
		m_aiBuf_INP_Bit[i] = 0;
	}
}

//! 'INP' Command�� UV ��ġ�� ������ ������ �پ��� ���� ������ �˾Ƴ���. 
//! 'CAABonderDlg::Parse_From_UV_Lamp' �Լ��� 'INP' �׸񿡼� ���� ó��
int CUVCommand::Ask_Input_Pin()
{
	CString sSend = _T("");

	sSend = _T("INP");

	int iRet = SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		return (-10 + iRet);
	}

	return 1;
}

//! NG �� ���� ���ڿ��� �˾Ƴ���. 
//! UV ��ġ������ ���� ���ڿ����� 'NG' ������ ������ �ǹ̸� �̿��Ѵ�. 
CString CUVCommand::GetExplanationNG(CString sNumberNG)
{
	CString sRet = _T("");
	sRet.Format(_T("NG Number [%s]"), sNumberNG);

	if ( sNumberNG.GetLength() != 2 )
	{
		return sRet;
	}	

	sNumberNG.TrimLeft(_T("0"));
	sNumberNG.TrimRight(_T("0"));

	int iNumberNG = _ttoi(sNumberNG);

	switch(iNumberNG)
	{
	case 1:
		{
			sRet += _T(" : ");
			sRet += _T("Lamp trigger being applied. ");
		}
		break;
	case 2:
		{
			sRet += _T(" : ");
			sRet += _T("Memory cure being executed. ");
		}
		break;
	case 3:
		{
			sRet += _T(" : ");
			sRet += _T("Shutter auto function being executed. ");
		}
		break;
	case 4:
		{
			sRet += _T(" : ");
			sRet += _T("Optical feedback unit unconnected. ");
		}
		break;
	case 5:
		{
			sRet += _T(" : ");
			sRet += _T("Error not reaching memory cure setting of output intensity(W) ");
		}
		break;
	}//! switch(iNumberNG)

	return sRet;
}