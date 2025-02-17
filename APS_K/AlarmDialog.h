#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "Label/Label.h"
#include "Library/AutoRepeat/AutoRepeatButton.h"
#include "ExButton/exbutton.h"
#include "Utility\GridCtrl\GridCtrl.h"

// CAlarmDialog ��ȭ �����Դϴ�.


enum ALARM_PAGE{e_AlarmHistory = 0,e_AlarmMoniter, e_NgList};

class CAlarmDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAlarmDialog)

public:
	CAlarmDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAlarmDialog();


	int m_iAlarmKind;
	CExButton m_btnAlarmMonitor;

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ALARM };

protected:
	//CMSFlexGrid	m_gridAlarm[3];

	CExButton m_btnAlarmHistory;
	CExButton m_btnNgList;


	CString *m_sAlarmList[3];

	int m_iAlarmPage[3];
	int m_iAlarmCnt[3];
	int m_icurPage[3];


protected:
	void setinterface();
	void initGrid();
	void DrawGrid(int flag);	// 0�� ���� 1�� ä��
	void PushAlarm();			// 10�������� �� ���� 1�������� ���� 2�������� 1���� �־ ������ �������� �ٽ� ����д�.
	void SaveAlarm();
	void NowAlarmShow();
	void LoadAlarm();
	void LoadAlarm_History();
	void LoadAlarm_NgList();

	void SelectAlarm();
	bool checkAlarm();



	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void AlarmAdd(CString msg, int iAlarmKind = e_AlarmHistory);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButtonBack();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonBuzzerOff();
	afx_msg void OnBnClickedButtonNgList();
	afx_msg void OnBnClickedButtonAlarmMonitor();
	afx_msg void OnBnClickedButtonAlarmHistory();
	afx_msg void OnBnClickedButtonReset();
public:
	//grid
	CGridCtrl m_clGridAlarm;

	void	InitGridCtrl_Alarm();
	void	ShowGridCtrl_Alarm();

	int AlarmRow;
	int AlarmCol;
	//
	afx_msg void OnDBClickedGridAlarm(NMHDR* pNMHDR, LRESULT* pResult);
};
