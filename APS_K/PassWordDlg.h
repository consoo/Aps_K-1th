#pragma once


#include "Label/Label.h"
// CPassWordDlg ��ȭ �����Դϴ�.

class CPassWordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPassWordDlg)

public:
	CPassWordDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPassWordDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

	CLabel	m_labelPassWord;

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
