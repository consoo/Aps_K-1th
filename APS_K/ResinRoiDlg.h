#pragma once

#include "afxcmn.h"
#include "afxwin.h"

#include "Label/Label.h"
#include "Library/AutoRepeat/AutoRepeatButton.h"
#include "ExButton/exbutton.h"


// CResinRoiDlg ��ȭ �����Դϴ�.

class CResinRoiDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResinRoiDlg)

public:
	CResinRoiDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CResinRoiDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_RESIN_ROI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	void	dispMotorPos();
	void	func_GridInit_ArcRoi();
	void	func_GridInit_LineRoi();
	//CMSFlexGrid m_gridArcRoi;		//��ȣ���� Model Data Draw
	//CMSFlexGrid m_gridLineRoi;		//��ȣ���� Model Data Draw

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonResinRoiSave();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void DblClickMsflexgridArcRoi();
	void DblClickMsflexgridLineRoi();
	void displayData_ResinRoi();
	void getData_ResinRoi();
};
