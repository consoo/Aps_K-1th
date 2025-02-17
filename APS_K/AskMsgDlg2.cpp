// AskMsgDlg2.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AA Bonder.h"
#include "AskMsgDlg2.h"
#include "afxdialogex.h"


// CAskMsgDlg2 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAskMsgDlg2, CDialogEx)

CAskMsgDlg2::CAskMsgDlg2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAskMsgDlg2::IDD, pParent)
{

}

CAskMsgDlg2::~CAskMsgDlg2()
{
}

void CAskMsgDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LABEL_MESSAGE, m_labelMsg);
}


BEGIN_MESSAGE_MAP(CAskMsgDlg2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_YES, &CAskMsgDlg2::OnBnClickedButtonYes)
	ON_BN_CLICKED(IDC_BUTTON_NO, &CAskMsgDlg2::OnBnClickedButtonNo)
	ON_BN_CLICKED(IDC_BUTTON_NO2, &CAskMsgDlg2::OnBnClickedButtonNo2)
END_MESSAGE_MAP()


// CAskMsgDlg2 �޽��� ó�����Դϴ�.


BOOL CAskMsgDlg2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_labelMsg
		.SetBkColor(M_COLOR_LIGHT_BLUE)
		.SetText(m_sMsg)
		.SetFontBold(TRUE)
		.SetFontSize(20);

	m_iSelectNo = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CAskMsgDlg2::OnBnClickedButtonYes()
{
	m_iSelectNo = 1;
	CDialogEx::OnOK();
}


void CAskMsgDlg2::OnBnClickedButtonNo()
{
	m_iSelectNo = -1;
	CDialogEx::OnOK();
}


void CAskMsgDlg2::OnBnClickedButtonNo2()
{
	m_iSelectNo = 0;
	CDialogEx::OnOK();
}
