#pragma once
#include "Label/Label.h"
#include "ExButton/exbutton.h"
// CTiltingManualDlg ��ȭ �����Դϴ�.

class CTiltingManualDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTiltingManualDlg)

public:
	CTiltingManualDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTiltingManualDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MANUAL_TILTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CExButton m_btnOk;
	CExButton m_btnStop;
	
	CLabel	edit_TiltingTX;
	CLabel	edit_TiltingTY;

	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedEditTiltingX();
	afx_msg void OnStnClickedEditTiltingY();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonStop();
};
