#pragma once

#include "afxcmn.h"
#include "afxwin.h"

#include "Label/Label.h"
#include "Library/AutoRepeat/AutoRepeatButton.h"
#include "ExButton/exbutton.h"
#include "Utility\GridCtrl\GridCtrl.h"

// CEpoxyDlg ��ȭ �����Դϴ�.

class CEpoxyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEpoxyDlg)

public:
	CEpoxyDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEpoxyDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_EPOXYSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	
	DECLARE_MESSAGE_MAP()

public:
	void	dispMotorPos();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonMotorPosSave();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()

public:
	//Grid
	CGridCtrl m_clGridEpoxyBasic;
	CGridCtrl m_clGridEpoxyCircleSet;
	CGridCtrl m_clGridEpoxyLength;
	CGridCtrl m_clGridEpoxyCircleInsp;
	CGridCtrl m_clGridEpoxyRectInsp;
	CGridCtrl m_clGridEpoxyOffset;
	CGridCtrl m_clGridCircleRadiusOffset;
	CGridCtrl m_clGridEpoxyCircleInspArea;

	void	InitGridCtrl_EpoxyCircleInspArea();
	void	InitGridCtrl_EpoxyBasicSet();
	void	InitGridCtrl_EpoxyCircleSet();
	void	InitGridCtrl_EpoxyLength();
	void	InitGridCtrl_EpoxyCircleInsp();
	void	InitGridCtrl_EpoxyRectInsp();
	void	InitGridCtrl_EpoxyOffset();
	void	InitGridCtrl_CircleRadiusOffset();//�ٰ��� �� ������ 4��
	void	ShowGridCtrl_Epoxy();

	int EpoxySetRow;
	int EpoxySetCircleRow;
	int EpoxyLengthRow;
	int EpoxyCircleInspRow;
	int EpoxyRectInspRow;
	int EpoxyOffsetRow;
	int EpoxyCircleAreaRow;//EpoxyCircleAreaCol;

	int EpoxySetCol;
	int EpoxySetCircleCol;
	int EpoxyLengthCol;
	int EpoxyCircleInspCol;
	int EpoxyCircleAreaCol;
	int EpoxyRectInspCol;
	int EpoxyOffsetCol;
	
	//
	afx_msg void OnDBClickedGridEpoxySet(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDBClickedGridEpoxyCircleSet(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDBClickedGridEpoxyLength(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDBClickedGridEpoxyCircleInsp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDBClickedGridEpoxyRectInsp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDBClickedGridEpoxyOffset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDBClickedGridCircleRadiusOffset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDBClickedGridCircleInspSet(NMHDR* pNMHDR, LRESULT* pResult);
	
};
