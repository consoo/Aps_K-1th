#pragma once
#include "Comm_RS232C.h"
class CKeyenceSensor
{
public:
	CKeyenceSensor(void);
	~CKeyenceSensor(void);

	DWORD				dwReceive ;			//! ������ Byte ũ��
	CComm_RS232C		serial;

	bool func_RS232_CommOpen(int iPort, int iBaudRate);
	void func_RS232_CommClose();

	bool func_LT9030_Connect(void);
	bool func_RS232_ScanCheck(CString &sComm);

	bool func_CL3000_Scan(double &laserVal);
	bool func_LT9030_Scan(double &laserVal);				//���� ���� Reading
	bool func_LT9030_ReScan(CString &sRtnComm);				//���� ���� �� Reading
	bool func_LT9030_AutoZeroSet(int iCh, bool bOn);		//Auto Zero Setting ON/OFF
	bool func_LT9030_KeyLock(bool bOn);						//Key Lock ON/OFF


	bool	bThreadHand;
	HANDLE	m_hThreadComm;		//THREAD �ڵ�

	bool func_CreateCommLT9030(void);
};

DWORD ThreadCommLT9030(CKeyenceSensor* pKeyence);