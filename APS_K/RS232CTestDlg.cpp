// RS232CTestDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AA Bonder.h"
#include "RS232CTestDlg.h"
#include "afxdialogex.h"


// CRS232CTestDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CRS232CTestDlg, CDialogEx)

CRS232CTestDlg::CRS232CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRS232CTestDlg::IDD, pParent)
{

}

CRS232CTestDlg::~CRS232CTestDlg()
{
}

void CRS232CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_RS232C, m_cbCOMPort);
	DDX_Control(pDX, IDC_COMBO_RS232C_002, m_cbCOMPort002);
}


BEGIN_MESSAGE_MAP(CRS232CTestDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_RS232C_SEND, &CRS232CTestDlg::OnBnClickedButtonRs232cSend)
	ON_BN_CLICKED(IDCANCEL, &CRS232CTestDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_RS232C_CON, &CRS232CTestDlg::OnClickedButtonRs232cCon)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_RS232C_CON_002, &CRS232CTestDlg::OnClickedButtonRs232cCon002)
	ON_BN_CLICKED(IDC_BUTTON_RS232C_SEND_002, &CRS232CTestDlg::OnClickedButtonRs232cSend002)
END_MESSAGE_MAP()


// CRS232CTestDlg �޽��� ó�����Դϴ�.


BOOL CRS232CTestDlg::Create(UINT nIDTemplate, CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	//return CDialog::Create(IDD, pParentWnd);
	return CDialog::Create(nIDTemplate, pParentWnd);
}

BOOL CRS232CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	int i = 0;
	CString sSetTemp = _T("");

	m_cbCOMPort.ResetContent();
	m_cbCOMPort002.ResetContent();
	for ( i = 0; i < 15; i++ )
	{
		sSetTemp.Format(_T("COM%d"), i + 1);

		m_cbCOMPort.AddString(sSetTemp);
		m_cbCOMPort002.AddString(sSetTemp);
	}
	m_cbCOMPort.SetCurSel(0);
	m_cbCOMPort002.SetCurSel(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CRS232CTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//! 'Esc', 'Enter' Ű�� Dialog�� ������ �ʵ��� �����Ѵ�. 
	if ( (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) || 
	     (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) )
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CRS232CTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	m_cbCOMPort.ResetContent();
	m_cbCOMPort002.ResetContent();
}


void CRS232CTestDlg::OnBnClickedButtonRs232cSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString sSend = _T("");

	GetString_FromUI(IDC_EDIT_RS232C_SEND, this, sSend);

	sSend.TrimLeft(_T(" "));
	sSend.TrimRight(_T(" "));
	if ( sSend.GetLength() <= 0 )
	{
		return;
	}

	int iRet = UVCommand.SendData_UV_Controller(sSend);
	if ( iRet < 0 )
	{
		CString sError = _T("");
		sError.Format(_T("Error %d"), iRet);
		AfxMessageBox(sError);
	}
}

void CRS232CTestDlg::OnClickedButtonRs232cSend002()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString sSend = _T("");

	GetString_FromUI(IDC_EDIT_RS232C_SEND, this, sSend);

	sSend.TrimLeft(_T(" "));
	sSend.TrimRight(_T(" "));
	if ( sSend.GetLength() <= 0 )
	{
		return;
	}

	int iRet = LightControl.SendData_Light_Controller(sSend);
	if ( iRet < 0 )
	{
		CString sError = _T("");
		sError.Format(_T("Error %d"), iRet);
		AfxMessageBox(sError);
	}
}

void CRS232CTestDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnCancel();
	this->ShowWindow(SW_HIDE);
}

void CRS232CTestDlg::AddText(CString sInput)
{
	AppendString_InEditCtrl(IDC_EDIT_RS232C_RECV, this, sInput, true);
}

void CRS232CTestDlg::OnClickedButtonRs232cCon()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if ( UVCommand.IsConnected() == false )
	{
		int iGetCurSel = m_cbCOMPort.GetCurSel();
		if ( iGetCurSel == CB_ERR )
		{
			return;
		}

		CString sGetTemp = _T("");
		m_cbCOMPort.GetLBText(iGetCurSel, sGetTemp);	
		sGetTemp.TrimLeft(_T(" "));	

		if ( UVCommand.Connect_Device(sGetTemp) == true )
		{
			SetString_to_UI(IDC_BUTTON_RS232C_CON, this, _T("Disconnect"));
		}
		else
		{
			SetString_to_UI(IDC_BUTTON_RS232C_CON, this, _T("Connect"));
		}
	}
	else
	{
		UVCommand.Close_Device();
		SetString_to_UI(IDC_BUTTON_RS232C_CON, this, _T("Connect"));
	}
}

void CRS232CTestDlg::OnClickedButtonRs232cCon002()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if ( LightControl.IsConnected() == false )
	{
		int iGetCurSel = m_cbCOMPort002.GetCurSel();
		if ( iGetCurSel == CB_ERR )
		{
			return;
		}

		CString sGetTemp = _T("");
		m_cbCOMPort002.GetLBText(iGetCurSel, sGetTemp);	
		sGetTemp.TrimLeft(_T(" "));	

		if ( LightControl.Connect_Device(sGetTemp) == true )
		{
			SetString_to_UI(IDC_BUTTON_RS232C_CON_002, this, _T("Disconnect"));
		}
		else
		{
			SetString_to_UI(IDC_BUTTON_RS232C_CON_002, this, _T("Connect"));
		}
	}
	else
	{
		LightControl.Close_Device();
		SetString_to_UI(IDC_BUTTON_RS232C_CON_002, this, _T("Connect"));
	}
}

void CRS232CTestDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	if ( UVCommand.IsConnected() == false )
	{
		SetString_to_UI(IDC_BUTTON_RS232C_CON, this, _T("Connect"));
	}
	else
	{
		SetString_to_UI(IDC_BUTTON_RS232C_CON, this, _T("Disconnect"));
	}

	if ( LightControl.IsConnected() == false )
	{
		SetString_to_UI(IDC_BUTTON_RS232C_CON_002, this, _T("Connect"));
	}
	else
	{
		SetString_to_UI(IDC_BUTTON_RS232C_CON_002, this, _T("Disconnect"));
	}
}
