#pragma once

#include "Label/Label.h"

// CAskMsgDlg2 ��ȭ �����Դϴ�.

class CAskMsgDlg2 : public CDialogEx
{
	DECLARE_DYNAMIC(CAskMsgDlg2)

public:
	CAskMsgDlg2(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAskMsgDlg2();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ASK2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString		m_sMsg;
	CLabel		m_labelMsg;

	int			m_iSelectNo;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonYes();
	afx_msg void OnBnClickedButtonNo();
	afx_msg void OnBnClickedButtonNo2();
};
