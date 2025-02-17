// ConnectSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "../AA Bonder.h"
#include "../AA BonderDlg.h"
#include "ConnectSocket.h"


// CConnectSocket

CConnectSocket::CConnectSocket()
{
}

CConnectSocket::~CConnectSocket()
{
}


// CConnectSocket ��� �Լ�


void CConnectSocket::OnClose(int nErrorCode)
{
	ShutDown();
	Close();

	CSocket::OnClose(nErrorCode);

	AfxMessageBox(_T("ERROR: Disconnected from server!"));
	::PostQuitMessage(0);
}


void CConnectSocket::OnReceive(int nErrorCode)
{
	TCHAR szBuffer[1024];
	::ZeroMemory(szBuffer, sizeof(szBuffer));

	if(Receive(szBuffer, sizeof(szBuffer)) > 0)
	{
		CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

		if ( lstrcmp(szBuffer, "IDCHK") == 0 )
		{
			CString sTemp = "ID";
			pFrame->m_Socket.Send(sTemp, lstrlen(sTemp) * 2);
		}
	}

	CSocket::OnReceive(nErrorCode);
}


void CConnectSocket::OnConnect(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CSocket::OnConnect(nErrorCode);
}


BOOL CConnectSocket::OnMessagePending()
{
	MSG Message;

	if ( ::PeekMessage(&Message, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE) == TRUE )
	{
		if ( Message.wParam == 10 )
		{
			::PeekMessage(&Message, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
			CancelBlockingCall();
		}
	}

	return CSocket::OnMessagePending();
}
