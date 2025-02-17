#pragma once


// CCcdDlg ��ȭ �����Դϴ�.

#include "ExButton/exbutton.h"

//! ���� ���õ� �ڽ� Dialog�� ��Ÿ����. 
#define	CCD_VIEW_DEFECT		0
#define CCD_VIEW_OSCHECK	1
#define CCD_VIEW_SFR		2
#define CCD_VIEW_INSP_MODE	3

//class CCCDOSChkDlg;
class CCCDSFRDlg;
class CCCDInspModeDlg;


class CCcdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCcdDlg)

public:
	CCcdDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCcdDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CCD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:


	//! ��ư ��Ʈ
	CFont m_ftNormal;
	CFont m_ftSelected;
	bool CreateFont_Child_Dlg_Btn();

	int     m_iMode_View;	//! ���� ���õ� �ڽ� Dialog�� ��Ÿ���� ���� (CCD_VIEW_BOARD ~ CCD_VIEW_TEST)

	int     Make_Child_Dialog();
	void    Delete_Child_Dialog();

	void	setInterface();
	void    setInterface_Child_Dialog();

	
	bool    SetVisible_Child_Dialog(int iMode_View, bool bFlag_Visible);

public:
	void    Hide_All_Child_Dialog();	
	void    SetView_Child_Dialog(int iMode_View);

	//CCCDOSChkDlg *m_pOSChkDlg;
	CCCDSFRDlg	 *m_pSFRDlg;
	CCCDInspModeDlg *m_pInspModeDlg;


public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();

	CExButton m_btnOSChk;
	CExButton m_btnSFR;
	CExButton m_btnDefect;
	CExButton m_btnInspMode;

	afx_msg void OnBnClickedButtonCcdOscheck();
	afx_msg void OnBnClickedButtonCcdSfr();
	afx_msg void OnBnClickedButtonCcdDefect();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void SetAlignData_Tilt(double dx, double dy);
	void SetAlignData_Shift(double dx, double dy);
	void DisplayFrameRate(int iFrameRate);
	afx_msg void OnBnClickedButtonMandoInsp();
};
