#pragma once


#include "Resource.h"
#include "Label/Label.h"

// CInformDlg ��ȭ �����Դϴ�.

class CInformDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInformDlg)

public:
	CInformDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInformDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_INFORM };

	CString		titleStr;
	CString		m_sMsg;
	COLORREF	color;
	COLORREF	bkColor;

	CLabel		m_labelInformMsg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonClose();
	virtual BOOL OnInitDialog();
};
