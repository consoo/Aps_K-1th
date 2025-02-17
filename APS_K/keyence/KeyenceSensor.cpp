#include "StdAfx.h"
#include "KeyenceSensor.h"
CKeyenceSensor::CKeyenceSensor(void)
{
	m_hThreadComm = NULL;
}


CKeyenceSensor::~CKeyenceSensor(void)
{
}

bool CKeyenceSensor::func_RS232_CommOpen(int iPort, int iBaudRate)
{
	CString sPort;
	sPort.Format("COM%d", iPort);
	int iReturn;

	stringSetupInfo_RS232C stringSerial;
	stringSerial.InitInfo();
	stringSerial.dwBaudRate   = (DWORD)iBaudRate;//CBR_9600;
	stringSerial.byDataBit    = 8;
	stringSerial.byStopBit    = ONESTOPBIT;
	stringSerial.byParityBit  = NOPARITY;
	stringSerial.sPortName    = sPort;
	stringSerial.eFlowControl = EHandshakeUnknownAdd;
	stringSerial.bFlag_Close_COM_Port   = true;
	stringSerial.bFlag_Use_CPU_Affinity = false;
	stringSerial.bFlag_Recv_ThreadStart = false;	//! ���� ������ ó���� Thread�� �������� ����
	stringSerial.iTimeOut_Close_Thread  = 3000;
	iReturn = serial.OpenPort(&stringSerial);

	if ( iReturn < 0 )
	{
		return false;
	}

	return true;
}

void CKeyenceSensor::func_RS232_CommClose()
{
	serial.ClosePort(300);
}


bool CKeyenceSensor::func_LT9030_Connect(void)
{
	if( !func_RS232_CommOpen(sysData.iCommPort[COMM_LASER], sysData.iBaudRate[COMM_LASER]) )	return false;

	return true;
}

bool CKeyenceSensor::func_RS232_ScanCheck(CString &sComm)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	BYTE	byRecvBuff[4096];		//! ���� ����
	memset(byRecvBuff, 0x00, 4096);
	CString sMsgBuff="",Err="",sMsg="";
	double startTime, eTime;
	sComm = "";

	startTime = myTimer(true);
	CString sRecv="";
	while(1)
	{
		Sleep(300);
		dwReceive = serial.ReceiveComm( byRecvBuff, sizeof(byRecvBuff) );  // ����� �ޱ�
		
		if(dwReceive > 0 )
		{		
			sRecv.Format("%s", byRecvBuff);	
			sComm = sRecv;
			sMsgBuff = sRecv;
			sMsgBuff = sMsgBuff.Left(2);
			
		if(sMsgBuff == "ER")
		{
			Err = sRecv.Mid(6,2);
			if(Err == _T("00"))	
			{
				sLangChange.LoadStringA(IDS_STRING278);	//"[���� ������]Command Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(Err == _T("01"))
			{
				sLangChange.LoadStringA(IDS_STRING287);	//"[���� ������]Status Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(Err == _T("02"))
			{
				sLangChange.LoadStringA(IDS_STRING284);	//"[���� ������]Parity Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(Err == _T("03"))
			{
				sLangChange.LoadStringA(IDS_STRING285);	//"[���� ������]Receive Buffer Full Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(Err == _T("04"))			
			{
				sLangChange.LoadStringA(IDS_STRING286);	//"[���� ������]Send Buff Full Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(Err == _T("05"))
			{
				sLangChange.LoadStringA(IDS_STRING280);	//"[���� ������]Framing Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(Err == _T("20"))	
			{
				sLangChange.LoadStringA(IDS_STRING279);	//"[���� ������]Command Length Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(Err == _T("21"))
			{
				sLangChange.LoadStringA(IDS_STRING283);	//"[���� ������]Parameter Count Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(Err == _T("22"))
			{
				sLangChange.LoadStringA(IDS_STRING281);	//"[���� ������]Invalid Parameter Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(Err == _T("88"))			
			{
				sLangChange.LoadStringA(IDS_STRING288);	//"[���� ������]Timout  Error �߻�"
				sMsg.Format(sLangChange);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING282);	//"[���� ������]Other  Error �߻�"
				sMsg.Format(sLangChange);
			}

			delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
			pFrame->putListLog(sMsg);
			return false;
		}
				break;
	}


		eTime = myTimer(true);
		if(eTime-startTime > 5000)
		{
			CString sMsg;
			sMsg.Format("Laser �������� ��� �̻�.. Recv ���� �ʰ�..");
			//delayMsg(sMsg.GetBuffer(99), 5000, M_COLOR_RED);
			pFrame->putListLog(sMsg);
			return false;
		}

	}

	return true;
}

bool CKeyenceSensor::func_CL3000_Scan(double &laserVal)
{
	CString sSendMsg = _T("");
	CString sRtnComm = _T("");
	CString sMsgBuff = _T("");
	CString ErrMsg = _T("");
	CString sMsg = _T("");
	CString sResult = _T("");
	sSendMsg =	_T("M");//0
	sSendMsg +=	_T("M");
	sSendMsg +=	_T(",");
	sSendMsg += _T("1");
	sSendMsg += _T(",");
	//
	sSendMsg += _T("1");//5
	sSendMsg += _T("1");
	sSendMsg += _T("0");
	sSendMsg += _T("0");
	sSendMsg += _T("0");
	sSendMsg += _T("0");//10
	sSendMsg += _T("0");
	sSendMsg += _T("0");
	sSendMsg += _T("\r");	// END(CR)

	serial.WriteComm( (unsigned char*)(LPCTSTR)sSendMsg, sSendMsg.GetLength() );   // Scan ��ɾ� ����
	//AfxMessageBox(sSendMsg);
	Sleep(100);

	BYTE	byRecvBuff[4096];		//! ���� ����
	memset(byRecvBuff, 0x00, 4096);

	dwReceive = serial.ReceiveComm( byRecvBuff, sizeof(byRecvBuff) );  // ����� �ޱ�
	sRtnComm = byRecvBuff;

	//-------------------------------------------------------------------------------------------
	double dValue[2];
	//CString szMsg[MAX_LANG_COUNT];
	CString sPacket = _T("");
	CString sToken = _T("");
	CString sToken2 = _T("");
	//int i;
	sPacket = (char*)byRecvBuff;
	dValue[0] = dValue[1] = 0.0;
	AfxExtractSubString(sToken, sPacket, 0, _T(','));
	if (sToken == _T("MM"))
	{
		// HEAD 1
		AfxExtractSubString(sToken2, sPacket, 2, _T(','));
		if (sToken2 == _T("0"))
		{
			AfxExtractSubString(sToken, sPacket, 1, _T(','));
			dValue[0] = _ttof((TCHAR*)(LPCTSTR)sToken);
			/*if ((g_clTaskWork[0].m_nLaserTiltIndex >= 0) && (g_clTaskWork[0].m_nLaserTiltIndex < 4))
			{
				g_clTaskWork[0].m_dLaserVal[g_clTaskWork[0].m_nLaserTiltIndex] = dValue[0];
				g_clTaskWork[0].m_bRecvLaserTilt = true;

			}*/
		}


		// HEAD 2
		AfxExtractSubString(sToken2, sPacket, 4, _T(','));
		if (sToken2 == _T("0"))
		{
			AfxExtractSubString(sToken, sPacket, 3, _T(','));

			dValue[1] = _ttof((TCHAR*)(LPCTSTR)sToken);
			/*if ((g_clTaskWork[1].m_nLaserTiltIndex >= 0) && (g_clTaskWork[1].m_nLaserTiltIndex < 4))
			{
				g_clTaskWork[1].m_dLaserVal[g_clTaskWork[1].m_nLaserTiltIndex] = dValue[1];
				g_clTaskWork[1].m_bRecvLaserTilt = true;
			}*/
		}

	}
	//------------------------------------------------------------------------------
	//AfxMessageBox(sRtnComm);
	sMsgBuff = sRtnComm.Left(2);
	if(sMsgBuff == "ER")
	{
		ErrMsg = sRtnComm.Mid(6,2);
		if(ErrMsg == _T("00"))	
			{
				sLangChange.LoadStringA(IDS_STRING278);	//"[���� ������]Command Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(ErrMsg == _T("01"))
			{
				sLangChange.LoadStringA(IDS_STRING287);	//"[���� ������]Status Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(ErrMsg == _T("02"))
			{
				sLangChange.LoadStringA(IDS_STRING284);	//"[���� ������]Parity Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(ErrMsg == _T("03"))
			{
				sLangChange.LoadStringA(IDS_STRING285);	//"[���� ������]Receive Buffer Full Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(ErrMsg == _T("04"))	
			{
				sLangChange.LoadStringA(IDS_STRING286);	//"[���� ������]Send Buff Full Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(ErrMsg == _T("05"))
			{
				sLangChange.LoadStringA(IDS_STRING280);	//"[���� ������]Framing Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(ErrMsg == _T("20"))	
			{
				sLangChange.LoadStringA(IDS_STRING279);	//"[���� ������]Command Length Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(ErrMsg == _T("21"))
			{
				sLangChange.LoadStringA(IDS_STRING283);	//"[���� ������]Parameter Count Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(ErrMsg == _T("22"))
			{
				sLangChange.LoadStringA(IDS_STRING281);	//"[���� ������]Invalid Parameter Error �߻�"
				sMsg.Format(sLangChange);
			}
			else if(ErrMsg == _T("88"))			
			{
				sLangChange.LoadStringA(IDS_STRING288);	//"[���� ������]Timout  Error �߻�"
				sMsg.Format(sLangChange);
			}
			else
			{
				sLangChange.LoadStringA(IDS_STRING282);	//"[���� ������]Other  Error �߻�"
				sMsg.Format(sLangChange);
			}
		return false;
	}
	else
	{
		sMsgBuff = sRtnComm.Left(4);  //����Ŀ�ǵ� �տ� 4�ڸ� ���� (�ؿ� �ٿ��� ��ȣ �ɷ����� ���ؼ�)
		sResult = sMsgBuff.Right(1); // ��ȣ�� ���� �ɷ���.
		sResult += sRtnComm.Mid(4,7); //���� �� ������ 4��°���� 12��°���� (�����ٺ�ȣ + ������)
	}

	laserVal = atof(sResult);
	laserVal = dValue[0];// aserVal / 1000;

	return true;
}

bool CKeyenceSensor::func_LT9030_Scan(double &laserVal)
{
	CString sSendMsg = _T("");
	CString sRtnComm = _T("");
	CString sMsgBuff = _T("");
	CString ErrMsg = _T("");
	CString sMsg = _T("");
	CString sResult = _T("");
	sSendMsg = _T("M");
	sSendMsg += _T("0");//sUseCh;		//0:Out1 + Out2
	sSendMsg += _T(",");
	sSendMsg += _T("1");	//0:����ġ, 1:����ġ+����
	sSendMsg += _T("\r");	// END(CR)

	serial.WriteComm((unsigned char*)(LPCTSTR)sSendMsg, sSendMsg.GetLength());   // Scan ��ɾ� ����
																				 //AfxMessageBox(sSendMsg);
	Sleep(100);

	BYTE	byRecvBuff[4096];		//! ���� ����
	memset(byRecvBuff, 0x00, 4096);

	dwReceive = serial.ReceiveComm(byRecvBuff, sizeof(byRecvBuff));  // ����� �ޱ�
	sRtnComm = byRecvBuff;
	//AfxMessageBox(sRtnComm);
	sMsgBuff = sRtnComm.Left(2);
	if (sMsgBuff == "ER")
	{
		ErrMsg = sRtnComm.Mid(6, 2);
		if (ErrMsg == _T("00"))
		{
			sLangChange.LoadStringA(IDS_STRING278);	//"[���� ������]Command Error �߻�"
			sMsg.Format(sLangChange);
		}
		else if (ErrMsg == _T("01"))
		{
			sLangChange.LoadStringA(IDS_STRING287);	//"[���� ������]Status Error �߻�"
			sMsg.Format(sLangChange);
		}
		else if (ErrMsg == _T("02"))
		{
			sLangChange.LoadStringA(IDS_STRING284);	//"[���� ������]Parity Error �߻�"
			sMsg.Format(sLangChange);
		}
		else if (ErrMsg == _T("03"))
		{
			sLangChange.LoadStringA(IDS_STRING285);	//"[���� ������]Receive Buffer Full Error �߻�"
			sMsg.Format(sLangChange);
		}
		else if (ErrMsg == _T("04"))
		{
			sLangChange.LoadStringA(IDS_STRING286);	//"[���� ������]Send Buff Full Error �߻�"
			sMsg.Format(sLangChange);
		}
		else if (ErrMsg == _T("05"))
		{
			sLangChange.LoadStringA(IDS_STRING280);	//"[���� ������]Framing Error �߻�"
			sMsg.Format(sLangChange);
		}
		else if (ErrMsg == _T("20"))
		{
			sLangChange.LoadStringA(IDS_STRING279);	//"[���� ������]Command Length Error �߻�"
			sMsg.Format(sLangChange);
		}
		else if (ErrMsg == _T("21"))
		{
			sLangChange.LoadStringA(IDS_STRING283);	//"[���� ������]Parameter Count Error �߻�"
			sMsg.Format(sLangChange);
		}
		else if (ErrMsg == _T("22"))
		{
			sLangChange.LoadStringA(IDS_STRING281);	//"[���� ������]Invalid Parameter Error �߻�"
			sMsg.Format(sLangChange);
		}
		else if (ErrMsg == _T("88"))
		{
			sLangChange.LoadStringA(IDS_STRING288);	//"[���� ������]Timout  Error �߻�"
			sMsg.Format(sLangChange);
		}
		else
		{
			sLangChange.LoadStringA(IDS_STRING282);	//"[���� ������]Other  Error �߻�"
			sMsg.Format(sLangChange);
		}
		return false;
	}
	else
	{
		sMsgBuff = sRtnComm.Left(4);  //����Ŀ�ǵ� �տ� 4�ڸ� ���� (�ؿ� �ٿ��� ��ȣ �ɷ����� ���ؼ�)
		sResult = sMsgBuff.Right(1); // ��ȣ�� ���� �ɷ���.
		sResult += sRtnComm.Mid(4, 7); //���� �� ������ 4��°���� 12��°���� (�����ٺ�ȣ + ������)
	}

	laserVal = atof(sResult);
	laserVal = laserVal / 1000;

	return true;
}
bool CKeyenceSensor::func_LT9030_ReScan(CString &sRtnComm)
{
	CString sSendMsg = _T("");
	sRtnComm = "";

	sSendMsg =	_T("M");
	sSendMsg +=	_T("R");	
	sSendMsg += _T("\r");	// END(CR)

	serial.WriteComm( (unsigned char*)(LPCTSTR)sSendMsg, sSendMsg.GetLength() );   // Scan ��ɾ� ����

	if( func_RS232_ScanCheck(sRtnComm) == false)	return false;

	return true;
}


bool CKeyenceSensor::func_LT9030_AutoZeroSet(int iCh, bool bOn)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString sUseCh = _T(""), sRtnMsg = _T(""), sCmd = "", sAck;
	sUseCh.Format("%d", iCh);
	
	if( bOn == true )	sCmd = _T("V");		//Auto Zero Set On�� ���
	else				sCmd = _T("W");		//				OFF

	CString sSendMsg = _T("");
	sSendMsg =	sCmd;
	sSendMsg +=	sUseCh;			//0:Out1 + Out2, 1:Out1, 2:Out2
	sSendMsg += _T("\r");		// END(CR)

	serial.WriteComm( (unsigned char*)(LPCTSTR)sSendMsg, sSendMsg.GetLength() );   // Scan ��ɾ� ����

	if( func_RS232_ScanCheck(sRtnMsg) == false ) return false;

	sAck = sCmd + sUseCh + _T("\r");
	if( sRtnMsg == sAck )				return true;

	CString sMsg;
	sLangChange.LoadStringA(IDS_STRING633);		//"Laser �������� ���� �̻�.. Zero Set ����.."
	sMsg.Format(sLangChange);

	delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
	pFrame->putListLog(sMsg);

	return false;
}

bool CKeyenceSensor::func_LT9030_KeyLock(bool bOn)
{
	CString sRtnMsg = _T(""), sCmd = "", sAck="", sInput="";

	if( bOn == true )	sCmd = _T("1");		//LOCK On�� ���
	else				sCmd = _T("0");		//	   OFF

	CString sSendMsg = _T("");
	sSendMsg =	_T("K");
	sSendMsg +=	_T("L");
	sSendMsg +=	_T(",");
	sSendMsg +=	sCmd;			//0:Out1 + Out2, 1:Out1, 2:Out2
	sSendMsg += _T("\r");		// END(CR)

	serial.WriteComm( (unsigned char*)(LPCTSTR)sSendMsg, sSendMsg.GetLength() );   // Scan ��ɾ� ����

	if( func_RS232_ScanCheck(sRtnMsg) == false ) return false;

	sAck = sSendMsg.Left(2);//�۽� Command�� ����
	sInput = sRtnMsg.Left(2);
	if( sInput == sAck )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CKeyenceSensor::func_CreateCommLT9030(void)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString sMsg;
	DWORD dwThreadID;

	if( serial.m_strCommStateInfo.bFlag_Connected == false)
	{
		sLangChange.LoadStringA(IDS_STRING289);	//"[��������] LT-9030 ���� Thread ���� ����.. RS-232C ��� Close����.!!"
		sMsg.Format(sLangChange);

		delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
		pFrame->putListLog(sMsg);
		return false;
	}
	
	bThreadHand = true;
	m_hThreadComm = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadCommLT9030, 
		this, 0, &dwThreadID);

	return true;
}


DWORD ThreadCommLT9030(CKeyenceSensor* pKeyence)
{
	//BOOL bReadStat = TRUE;
	//INT	 iErrCnt = 0;
	//INT  iErrTimeOver = 0;
	//long lRet = 0;
	//CString sMsg;

	//while( pKeyence->serial.m_strCommStateInfo.bFlag_Connected )
	//{
	////	//if( pKeyence->bThreadHand  == false )	
	////	if(!bReadStat) {
	////		if(iErrCnt < 5) {
	////			iErrCnt++;
	////			bReadStat = TRUE;
	////		}
	////		else 
	////		{
	////			if((GetTickCount() - iErrTimeOver) > 3000) {
	////				bReadStat = TRUE;
	////				iErrTimeOver = GetTickCount();
	////				iErrCnt = 0;
	////			}
	////		}
	////	} 
	////	else 
	////	{

	////	}

	//}
	return 1;
}