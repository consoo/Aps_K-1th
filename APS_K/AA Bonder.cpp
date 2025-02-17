
// AA Bonder.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "AA Bonder.h"
#include "AA BonderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAABonderApp

BEGIN_MESSAGE_MAP(CAABonderApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	ON_BN_CLICKED(IDSAVE, &CAABonderApp::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CAABonderApp::OnBnClickedOk)
END_MESSAGE_MAP()


// CAABonderApp ����

CAABonderApp::CAABonderApp()
{
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
	
	CString cMsg;
	/*if(GetFileAttributes(DATA_DIR) == -1)
	{
		cMsg=DATA_DIR;
		cMsg=cMsg+"������ �������� �ʾҽ��ϴ�.\n"+cMsg+"������ ȯ�漳�� ������ �����ϼ���.";
		MessageBox(NULL,cMsg,NULL,0);
		exit(1);
	}*/

	if(GetFileAttributes(BASE_DIR) == -1)
	{
		cMsg=BASE_DIR;
		cMsg=cMsg+"������ �������� �ʾҽ��ϴ�.\n"+cMsg+"������ ȯ�漳�� ������ �����ϼ���.";
		MessageBox(NULL,cMsg,NULL,0);
		exit(1);
	}

	/*if(GetFileAttributes(DATA_DIR) == -1)
	{
		cMsg=DATA_DIR;
		cMsg=cMsg+"������ �������� �ʾҽ��ϴ�.\n"+cMsg+"������ ȯ�漳�� ������ �����ϼ���.";
		MessageBox(NULL,cMsg,NULL,0);
		exit(1);
	}*/

	if(GetFileAttributes(LOG_DIR) == -1)
	{
		cMsg=LOG_DIR;
		cMsg=cMsg+"������ �������� �ʾҽ��ϴ�.\n"+cMsg+"������ ȯ�漳�� ������ �����ϼ���.";
		MessageBox(NULL,cMsg,NULL,0);
		exit(1);
	}

}


// ������ CAABonderApp ��ü�Դϴ�.

CAABonderApp theApp;


// CAABonderApp �ʱ�ȭ

BOOL CAABonderApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�.
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	

	HANDLE hMutex;
	hMutex = CreateMutex(NULL, FALSE, "AA Bonder*.exe");
	
	if(GetLastError()==ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		return false;
	}

	//! Added by LHW (2013/4/23)
	if ( !AfxSocketInit() )
	{
		AfxMessageBox(_T("Windows Socket Init Failed. "));
		return FALSE;
	}


	AfxEnableControlContainer();

	// ��ȭ ���ڿ� �� Ʈ�� �� �Ǵ�
	// �� ��� �� ��Ʈ���� ���ԵǾ� �ִ� ��� �� �����ڸ� ����ϴ�.
	CShellManager *pShellManager = new CShellManager;

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));


	CAABonderDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ���⿡ [Ȯ��]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ���⿡ [���]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}

	// ������ ���� �� �����ڸ� �����մϴ�.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�  ���� ���α׷��� ���� �� �ֵ��� FALSE��
	// ��ȯ�մϴ�.
	return FALSE;
}



void CAABonderApp::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CAABonderApp::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

