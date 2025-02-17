#pragma once


#include "Resource.h"
#include "Label/Label.h"


// CDelayMsgDlg ��ȭ �����Դϴ�.

class CDelayMsgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDelayMsgDlg)

public:
	CDelayMsgDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDelayMsgDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_DELAY };

	int timerRunFlag;

	int			delay;
	CTime		showTime;
	CTime		curTime;
	CString		titleStr;
	COLORREF	color;
	COLORREF	bkColor;

	CLabel		m_labelDelayMsg;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
