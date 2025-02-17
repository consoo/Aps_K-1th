#pragma once


// CListenSocket ��� ����Դϴ�.

class CListenSocket : public CAsyncSocket
{
public:
	CListenSocket();
	virtual ~CListenSocket();

	
	CPtrList	m_ptrClientSocketList;

	virtual void OnAccept(int nErrorCode);
	void CloseClientSocket(CAsyncSocket* pClient);
	bool SendData(POSITION pos, CString sMessage);
	bool SendDataAll(CString sMessage);
};
