// PassWordDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "PassWordDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CPassWordDlg, CDialogEx)

	CPassWordDlg::CPassWordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassWordDlg::IDD, pParent)
{

}

CPassWordDlg::~CPassWordDlg()
{
}

void CPassWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PASSWORD, m_labelPassWord);
}


BEGIN_MESSAGE_MAP(CPassWordDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPassWordDlg::OnBnClickedOk)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CPassWordDlg �޽��� ó�����Դϴ�.


void CPassWordDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


BOOL CPassWordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_labelPassWord
		.SetBkColor(GRID_COLOR_GREEN)
		.SetTextColor(M_COLOR_BLACK)
		.SetFontBold(TRUE)
		.SetFontSize(24);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CPassWordDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CString word;
	float PassData;
	word.Format("");

	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)word);
	if(dlg.DoModal() == IDOK)
	{
		CString curStr = dlg.GetstrNum();
		PassData = (float)atof(curStr);

		if(PassData == sysData.Password)
			CDialog::OnOK();
		else
			CDialog::OnCancel();
	}	
}



