#pragma once

#include "ExButton/exbutton.h"
#include "MSFlexGrid.h"

// CMotorHomDlg ��ȭ �����Դϴ�.

class CMotorHomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMotorHomDlg)

public:

	bool	timerRunFlag;

	CMotorHomDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMotorHomDlg();


	CMSFlexGrid	m_gridMotor;

	void setInterface();
	void dispMotorStatus();


	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MOTOR_HOME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonServoOnAll();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonServoOffAll();
	afx_msg void OnBnClickedButtonHome();
	afx_msg void OnBnClickedButtonHomeStop();
	afx_msg void OnBnClickedButtonReset();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonMotor0();
	afx_msg void OnBnClickedButtonMotionInit();
};
