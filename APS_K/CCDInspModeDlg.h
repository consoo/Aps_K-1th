#pragma once

#include "CcdSfrSpecDlg.h"
#include "CcdDefectSpecDlg.h"
#include "CcdRiOcSpecDlg.h"
#include "CcdBlemishSpecDlg.h"
#include "CcdChartSpecDlg.h"
#include "CcdRISpecDlg.h"
#include "CcdSnrColorSensSpecDlg.h"

// CCCDInspModeDlg ��ȭ �����Դϴ�.

class CCCDInspModeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCCDInspModeDlg)

public:
	CCCDInspModeDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCCDInspModeDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MANDO_INSP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
    
public:
    CCcdSfrSpecDlg m_clSfrSpecDlg;
    CCcdDefectSpecDlg m_clDefectSpecDlg;
    CCcdRiOcSpecDlg m_clRiOcSpecDlg;
    CCcdBlemishSpecDlg m_clBlemishSpecDlg;
    CCcdChartSpecDlg m_clChartSpecDlg;
    CCcdRISpecDlg m_clRiSpecDlg;
    CCcdSnrColorSensSpecDlg m_clSnrColorSensSpecDlg;
public:
	bool func_Check_MIU_Mode();

	afx_msg void OnBnClickedBtnInspPixelDefect();
	afx_msg void OnBnClickedBtnInspStain();
	afx_msg void OnBnClickedBtnInspUniformity();
	afx_msg void OnBnClickedBtnInspCurrent();
	afx_msg void OnBnClickedBtnInspTestPattern();
	afx_msg void OnBnClickedBtnInspMtf();
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedBtnSpecDefect();
    afx_msg void OnBnClickedBtnSpecOc();
    afx_msg void OnBnClickedBtnSpecBlemish();
    afx_msg void OnBnClickedBtnSpecFov();
    afx_msg void OnBnClickedButtonCcdRelativeiSpec();
    afx_msg void OnBnClickedBtnSpecSnr();
	afx_msg void OnBnClickedBtnInspColorUniformity();
	afx_msg void OnBnClickedBtnInspRi();
	afx_msg void OnBnClickedBtnInspSnr();
	afx_msg void OnBnClickedBtnInspColorSensitivity();
	afx_msg void OnBnClickedBtnInspFovDistortion();
	afx_msg void OnBnClickedBtnSpecMtf();
    afx_msg void OnBnClickedBtn6500kRawLoad();
    afx_msg void OnBnClickedBtnInspDistortion();
	afx_msg void OnBnClickedBtnInspSaturation();
};
