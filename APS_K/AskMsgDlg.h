#pragma once

#include "Label/Label.h"

// CAskMsgDlg ��ȭ �����Դϴ�.

class CAskMsgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAskMsgDlg)

public:
	CAskMsgDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAskMsgDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CString		m_sMsg;
	CLabel		m_labelMsg;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonYes();
	afx_msg void OnBnClickedButtonNo();
};
