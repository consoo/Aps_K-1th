// AskMsgDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AA BonderDlg.h"
#include "AskMsgDlg.h"
#include "afxdialogex.h"


// CAskMsgDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAskMsgDlg, CDialogEx)

	CAskMsgDlg::CAskMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAskMsgDlg::IDD, pParent)
{
}

CAskMsgDlg::~CAskMsgDlg()
{
}

void CAskMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LABEL_MESSAGE, m_labelMsg);
}


BEGIN_MESSAGE_MAP(CAskMsgDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_YES, &CAskMsgDlg::OnBnClickedButtonYes)
	ON_BN_CLICKED(IDC_BUTTON_NO, &CAskMsgDlg::OnBnClickedButtonNo)
END_MESSAGE_MAP()


// CAskMsgDlg �޽��� ó�����Դϴ�.


BOOL CAskMsgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_labelMsg
		.SetBkColor(M_COLOR_LIGHT_BLUE)
		.SetText(m_sMsg)
		.SetFontBold(TRUE)
		.SetFontSize(20);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CAskMsgDlg::OnBnClickedButtonYes()
{
	CDialogEx::OnOK();
}


void CAskMsgDlg::OnBnClickedButtonNo()
{
	CDialogEx::OnCancel();
}
