// EditDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AA Bonder.h"
#include "EditDlg.h"
#include "afxdialogex.h"


// CEditDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEditDlg, CDialogEx)

CEditDlg::CEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditDlg::IDD, pParent)
{

}

CEditDlg::~CEditDlg()
{
}

void CEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEditDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEditDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CEditDlg �޽��� ó�����Դϴ�.


void CEditDlg::OnBnClickedOk()
{
	GetDlgItem(IDC_EDIT_TEXT)->GetWindowText(m_strText);

	CDialogEx::OnOK();
}


void CEditDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


CString CEditDlg::getTextOnEdit()
{
	return m_strText;
}