#pragma once

//! RS-232C Serial ��� Ŭ����
//! Maked by LHW (2013/2/6), Visual C++ 2010���� ����
//! ������ �۾� ��¥ : 2013/5/3

//! ����ϴ� RS-232C Serial Port (COM1, COM2, ...)���� CComm_RS232C Ŭ������ ����� Ŭ������ ���� ����Ѵ�. 
//! CComm_RS232C Ŭ������ RS-232C Serial ����� �⺻���� ���¸� �ٷ��. 

//! CComm_RS232C Ŭ������ ����� Ŭ���������� �ش� Port�� �����Ǵ� ����� Packet Protocol ó���� �����Ѵ�. 


#include <afxmt.h>	//! CCriticalSection ������ �ʿ�

//! [���� ����] ��� ���� STX, ETX�� ������� �ʱ⵵ �Ѵ�. (����� Protocol ���� Ȯ�� �ʿ�)
//! ���ڿ� ��Ŷ�� ���ۿ� ���Ǵ� Ascii ����
#define STX	0x02
//! ���ڿ� ��Ŷ�� ��ħ�� ���Ǵ� Ascii ����
#define ETX 0x03


//! PC�� Serial Port ����� �˾Ƴ���. 
//! ���α׷� ��ü���� 1���� ��ü�� ������ �ȴ�.
class CCommList_RS232C
{
public:
	CCommList_RS232C(void);
	~CCommList_RS232C(void);

public:
	CStringArray m_arrList_ComPort;	//! PC�� Serial Port ���

	bool GetList_SerialPort();		//! PC�� Serial Port�� �˾Ƴ��� m_arrList_ComPort�� �����Ѵ�. 
};


//! RS-232C Serial ��� ������ �����ִ� Ŭ����
//!  - ��� �Լ��� Ư�� index�� �Է��ϸ�, CComm_RS232C Ŭ�������� ����� �� �ִ�RS-232C �������� ��ȯ�Ѵ�. 
//!  - GUI�� Combo Box�� �Բ� ����ϸ�, ���ϰ� ����� �� �ִ�. 
//! # RS-232C ��/���� ������ ó���� ������ �Լ� ����
class CCommHelper_Setup_RS232C
{
public:
	CCommHelper_Setup_RS232C(void);
	~CCommHelper_Setup_RS232C(void);

public:
	//! index (0 based) ----->> CComm_RS232C Ŭ�������� ����� �� �ִ�RS-232C ������
	//! [���� ����] 
	//!   1) �ش� ���� ������, false�� ��ȯ
	//!	  2) psItemName�� CString ������ �Էµǰ�, �Լ� ��ȯ���� true�̸�, �ش� ���ڿ��� ��ȯ�ȴ�. �� ���ڿ��� Combo Box�� ������ �� �ִ�.
	bool GetValue_BaudRate (int iIndex_BaudRate,  DWORD &dwRetBaudRate,  CString *psItemName = NULL);	
	bool GetValue_DataBit  (int iIndex_DataBit,   BYTE  &byRetDataBit,   CString *psItemName = NULL);
	bool GetValue_StopBit  (int iIndex_StopBit,   BYTE  &byRetStopBit,   CString *psItemName = NULL);
	bool GetValue_ParityBit(int iIndex_ParityBit, BYTE  &byRetParityBit, CString *psItemName = NULL);

	//! CComm_RS232C Ŭ�������� ����� �� �ִ�RS-232C ������ ----->> index (0 based)
	//! [���� ����] �ش� ���� ������, ������ ��ȯ
	int GetIndex_BaudRate (DWORD dwBaudRate);	
	int GetIndex_DataBit  (BYTE  byDataBit);
	int GetIndex_StopBit  (BYTE  byStopBit);
	int GetIndex_ParityBit(BYTE  byParityBit);

	//! Baud Rate ���� ���������� ��ȯ
	int GetBaudRateValue_Integer_FromConstant(DWORD dwBaudRate);

	//! ������ ���� BYTE �迭�� �����ͷ� ��ȯ
	int Convert_Integer_To_ArrayByte(int iInput, BYTE* pbyOutputBuf, int iOutputBufSize);
};

//! �帧 ���� ���� (Handshaking)
// 1) No handshaking, so data is always send even if the receiver
//    cannot handle the data anymore. This can lead to data loss,
//    when the sender is able to transmit data faster then the
//    receiver can handle.
// 2) Hardware handshaking, where the RTS/CTS lines are used to
//    indicate if data can be sent. This mode requires that both
//    ports and the cable support hardware handshaking. Hardware
//    handshaking is the most reliable and efficient form of
//    handshaking available, but is hardware dependant.
// 3) Software handshaking, where the XON/XOFF characters are used
//    to throttle the data. A major drawback of this method is that
//    these characters cannot be used for data anymore.
typedef enum
{
	EHandshakeUnknownAdd		= -1,	// Unknown
	EHandshakeOffAdd			=  0,	// No handshaking
	EHandshakeHardwareAdd		=  1,	// Hardware handshaking (RTS/CTS)
	EHandshakeSoftwareAdd		=  2	// Software handshaking (XON/XOFF)
}EHandshakeAdd;

//! CPU Core ��� ���� ������ ũ��
const int intBufSize_RS232C_CPU_Core = 30;

//! RS-232C Serial ����� �ϴ� ��񸶴��� ���� ���� ����ü
struct stringSetupInfo_RS232C
{
	//! COM Port �̸�
	CString sPortName;

	//! RS-232C ��� ����
	DWORD	dwBaudRate;
	BYTE	byDataBit;
	BYTE	byStopBit;
	BYTE	byParityBit;

	//! �帧 ����
	EHandshakeAdd   eFlowControl;	
	
	//! ������ COM Port ó�� ����
	//! true�̸�, ������ ���� �ִ� COM Port�� �ݰ�, �� �����Ѵ�. 
	bool	bFlag_Close_COM_Port;
	//! bFlag_Close_COM_Port�� true�� ��, Thread�� �ݴµ� �־����� �ð�, ���� msec
	//! ClosePort() �Լ������� ���ȴ�.
	int		iTimeOut_Close_Thread;

	//! Thread ����
	bool    bFlag_Recv_ThreadStart; //! true�̸�, RS-232C COM Port�� ���� ����, �ٷ� ���� Thread�� �����Ѵ�. 
	int		aiCPU[intBufSize_RS232C_CPU_Core];
	bool	bFlag_Use_CPU_Affinity;	//! aiCPU ������ ������ �̿��Ͽ�, Ư�� CPU Core���� ���� Thread ����

	stringSetupInfo_RS232C()
	{
		InitInfo();
	}

	void InitInfo()
	{
		sPortName   = _T("COM1");

		dwBaudRate  = CBR_38400;
		byDataBit   = 8;
		byStopBit   = ONESTOPBIT;
		byParityBit = NOPARITY;

		eFlowControl = EHandshakeUnknownAdd;		
		
		bFlag_Close_COM_Port = false;
		iTimeOut_Close_Thread = 3000;

		bFlag_Recv_ThreadStart = false;
		int i = 0;
		for ( i = 0; i < intBufSize_RS232C_CPU_Core; i++ )
		{
			aiCPU[i] = 0;
		}
		bFlag_Use_CPU_Affinity = false;
	}

	stringSetupInfo_RS232C& operator=(const stringSetupInfo_RS232C &input)	//! ������ ������
	{
		sPortName   = input.sPortName;

		dwBaudRate  = input.dwBaudRate;
		byDataBit   = input.byDataBit;
		byStopBit   = input.byStopBit;
		byParityBit = input.byParityBit;

		eFlowControl = input.eFlowControl;		
		
		bFlag_Close_COM_Port = input.bFlag_Close_COM_Port;
		iTimeOut_Close_Thread = input.iTimeOut_Close_Thread;

		bFlag_Recv_ThreadStart = input.bFlag_Recv_ThreadStart;
		int i = 0;
		for ( i = 0; i < intBufSize_RS232C_CPU_Core; i++ )
		{
			aiCPU[i] = input.aiCPU[i];
		}
		bFlag_Use_CPU_Affinity = input.bFlag_Use_CPU_Affinity;

		return *this;
	}

	bool Copy(stringSetupInfo_RS232C *pstrInput)
	{
		if ( pstrInput == NULL )
		{
			return false;
		}

		sPortName   = pstrInput->sPortName;

		dwBaudRate  = pstrInput->dwBaudRate;
		byDataBit   = pstrInput->byDataBit;
		byStopBit   = pstrInput->byStopBit;
		byParityBit = pstrInput->byParityBit;

		eFlowControl = pstrInput->eFlowControl;		
		
		bFlag_Close_COM_Port  = pstrInput->bFlag_Close_COM_Port;
		iTimeOut_Close_Thread = pstrInput->iTimeOut_Close_Thread;

		bFlag_Recv_ThreadStart = pstrInput->bFlag_Recv_ThreadStart;
		int i = 0;
		for ( i = 0; i < intBufSize_RS232C_CPU_Core; i++ )
		{
			aiCPU[i] = pstrInput->aiCPU[i];
		}
		bFlag_Use_CPU_Affinity = pstrInput->bFlag_Use_CPU_Affinity;

		return true;
	}
};

//! RS-232C Serial ��� ���� ����ü
struct stringStateInfo_RS232C
{
	//! COM Port�� �������� true�� ����
	bool bFlag_Connected;
	//! ���� �۽� �۾� ���̸� true�� �����Ѵ�.
	bool bFlag_Sending;
	
	//! ���� Thread�� �۵� ���̸� true�� ����
	bool bFlag_Thread_Operate;
	//! true�̸�,  ���� Thread�� ���� �����ϵ��� �Ѵ�. 
	//! false�̸�, ���� Thread�� Stop������ ������ �������� ������ ���� �ʵ��� �Ѵ�.
	bool bFlag_Thread_Active;
	//! ���� ���� Thread���� ���� �۾� ���̸� true�� �����Ѵ�.
	bool bFlag_Recv_Acting_In_Thread;

	stringStateInfo_RS232C()
	{
		InitInfo();
	}

	void InitInfo()
	{
		InitInfo_Comm();

		InitInfo_Thread();
	}

	//! RS-232C ��� ���� �ʱ�ȭ
	void InitInfo_Comm()
	{
		bFlag_Connected   = false;
		bFlag_Sending     = false;
	}

	//! Thread ���� �ʱ�ȭ
	void InitInfo_Thread()
	{
		bFlag_Thread_Operate        = false;
		bFlag_Thread_Active         = true;
		bFlag_Recv_Acting_In_Thread = false;
	}

	stringStateInfo_RS232C& operator=(const stringStateInfo_RS232C &input)	//! ������ ������
	{
		bFlag_Connected   = input.bFlag_Connected;
		bFlag_Sending     = input.bFlag_Sending;

		bFlag_Thread_Operate        = input.bFlag_Thread_Operate;
		bFlag_Thread_Active         = input.bFlag_Thread_Active;
		bFlag_Recv_Acting_In_Thread = input.bFlag_Recv_Acting_In_Thread;

		return *this;
	}
};

//! RS-232C Serial ��� Ŭ����
//! RS-232C Serial ����� �ϴ� ��� (COM Port)���� �� Ŭ������ ����� Ŭ������ ��ü�� �����ؾ� �Ѵ�. 
class CComm_RS232C
{
public:
	CComm_RS232C(void);
	~CComm_RS232C(void);

private:
	// Ư�� CPU���� Thread�� ����� ��, ��� CPU�� �����ϱ� ���� Mask ó�� ����
	DWORD				GetMask_CPU_Affinity(int *piBuf_CPU_Use, int iBufSize_CPU_Use);
	int					Set_CPU_Affinity(int *piBuf_CPU_Use, int iBufSize_CPU_Use);

	CWinThread			*m_pThread_RecvComm;				//! ���� Thread ��ü
	friend UINT			m_Thread_Receive_Comm(LPVOID pParam);	//! Thread ��ü �Լ�, ���� �����͸� ó���Ѵ�. 	

protected:
	HANDLE		        m_hComm;			//! ��� ��Ʈ ���� �ڵ�
	
    OVERLAPPED	        m_osReceive;		//! ��Ʈ ���� Overlapped structure
	OVERLAPPED	        m_osSend;			//! ��Ʈ ���� Overlapped structure

	void*				m_pReceiveProcPtr;	  //! ���� ���ڿ��� ���� Ŭ������ ������
	CCriticalSection	m_csReceiveProcPtr;	  //! m_pReceiveProcPtr�� ��ȣ��
	
	bool				m_bFlag_Stop;		  //! true�̸� �۾� ���� ���� Thread�� �����Ѵ�. �ʱⰪ�� false�̴�.

	//! RS-232C ����� ����� �Ŀ� �� ó�� 1�� �� �����ؾ� �� ����� �����ϴ�. 
	void ReadyDevice() ;

	//! ���ŵ� ���ڿ��� ���� m_pReceiveProcPtr�� �����Ѵ�. 
	//! CComm_RS232C Ŭ������ ����� Ŭ�������� ������ �ش�. 
	//! [���� ����] 
	//!		1) ���� Thread_Receive_Comm �Լ����� ȣ��Ǵ� �Լ��̴�. "�ٸ� �������� �� �Լ��� ȣ���ϸ� �ȵȴ� !!!!!!!!!!!! "
	//!     2) ������, �� Ŭ������ ����� Ŭ�������� �ݵ�� ���� ����� �����ؾ� �Ѵ�. ��, ���� �����͸� �޾� ó���ϴ� ����� �����ؾ� �Ѵ�. 
	//!		   �̴� ������ Ŭ���� ������ �����ϸ鼭, �ð� ���� ���� ���� �������� ó���� �����ϰ� �Ѵ�. 
	//! [�Է°�]
	//!		pProcPtr    : ���� ������ m_pReceiveProcPtr
	//!		pbyBuff     : ���ŵ� ���ڿ�
    //!     iSize       : ���ŵ� ���ڿ��� ũ��
	//!		bActiveFlag : true�̸� ���� ���ڿ��� STX, ETX�� �°� �����ϳ�, ���������� ó�������� �ʴ´�. 
	//!     iCnt_Recv   : ���� ó�� ȸ��
// ����LHC	virtual void ProcessReceiveData(void *pProcPtr, BYTE *pbyBuff, int iSize, bool bFlag_Active, int iCnt_Recv) = 0;

	//! ���� ó���� ����ġ�� ������ ��û �۾�
//	virtual void PostProcRecvData() = 0;

public:	
	stringSetupInfo_RS232C m_strCommSetupInfo;	//! ����� ����� ���� ���� ����
	stringStateInfo_RS232C m_strCommStateInfo;	//! ����� ����ǰ� ������ ���°�

	//! ���ŵ� �����͸� ���������� ó���ϴ� Ŭ������ �����Ѵ�. 
	//! m_pReceiveProcPtr�� �����Ѵ�.
	void    SetReceiveProcPtr(void* pInput);
	
	//! Thread ���� �Լ�
	int		StartThread(bool bFlag_Use_CPU_Affinity, int *piBuf_CPU_Use = NULL, int iBufSize_CPU_Use = 0);
	int		Stop_Thread();
	int		Stop_Thread_Waiting(int iTimeOut = 0);

	//! �� ���� ���� ��ȯ
	bool    IsConnected();
	CString GetCurrent_PortName();
	DWORD	GetCurrent_BaudRate();
	BYTE	GetCurrent_DataBit();
	BYTE	GetCurrent_StopBit();
	BYTE	GetCurrent_ParityBit();

	//! ��� ���� �Լ�
	int     OpenPort(stringSetupInfo_RS232C *pstrSetupInfo);

	//! �۽� �۾� �Լ�
	DWORD   WriteComm(CString szSend, bool bSTX_ETX = false);
	DWORD   WriteComm(BYTE *pbyBuff, DWORD nSizeToSend, bool bSTX_ETX = false);

	//! ���� �۾� �Լ�
	DWORD   ReceiveComm(BYTE *pbyBuff, DWORD nSizeToReceive);

	//! ��� ��ħ �Լ�
	void    ClosePort();
	void    ClosePort(int iTimeOut);
};

