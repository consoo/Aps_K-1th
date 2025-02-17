#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "Label/Label.h"
#include "Library/AutoRepeat/AutoRepeatButton.h"
#include "ExButton/exbutton.h"
// CDataSet ��ȭ �����Դϴ�.

class CDataSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDataSet)

public:
	CDataSet(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDataSet();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_DATASET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void initMotorGrid();
	void setInterface();
	void changeMotorNo(int motorNo);

	int m_iMotorSpeed;
	int m_iSelMotor;
	CLabel	m_labelMotorName;
	//CMSFlexGrid	m_gridMotor;
	double m_fMotorJogSpeed;
	bool OLD_JogPlusflag;
	bool OLD_JogMinusflag;

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_EVENTSINK_MAP()
	void DblClickMsflexgridMotor();
	afx_msg void OnBnClickedButtonMotorSave();
	afx_msg void OnBnClickedButtonMotorOrigin();
	afx_msg void OnBnClickedButtonServoOn();
	afx_msg void OnBnClickedButtonSerboOff();
	afx_msg void OnBnClickedButtonAlarmReset();
	afx_msg void OnBnClickedButtonTest2();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedRadioMotorSpeed1();
	afx_msg void OnBnClickedRadioMotorSpeed2();
	afx_msg void OnBnClickedRadioMotorSpeed3();
	afx_msg void OnBnClickedButtonJogMinus();
	afx_msg void OnBnClickedButtonJogStop();
	afx_msg void OnBnClickedButtonJogPlus();
	CAutoRepeatButton m_JogPlus;
	CAutoRepeatButton m_JogMinus;

	afx_msg void OnStnClickedStaticMotorspeed();
	CLabel m_LabelMotorSpeed;

	public:
	//Grid
	CGridCtrl m_clGridDataSet;
	void	InitGridCtrl_DataSet();
	void	ShowGridCtrl_DataSet();

	int DataSetRow;
	int DataSetCol;
	
	//
	afx_msg void OnDBClickedGridDataSet(NMHDR* pNMHDR, LRESULT* pResult);
};
