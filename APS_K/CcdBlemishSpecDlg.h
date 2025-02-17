#pragma once

#include "afxwin.h"
#include "Utility\GridCtrl\GridCtrl.h"
#include "Utility\ButtonEx\ButtonEx.h"
#include "Label\Label.h"

// CCcdBlemishSpecDlg ��ȭ �����Դϴ�.

class CCcdBlemishSpecDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCcdBlemishSpecDlg)

public:
	CCcdBlemishSpecDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCcdBlemishSpecDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CCD_BLEMISH_SPEC};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	void SetUnit(int nUnit);
    const int BlemishMaxCount = 21;
    const int LcbMaxCount = 20;
    const int YmeanMaxCount = 26;

private:
	void InitCtrl();
    void InitBlemishGridCtrl();
    void InitLcbGridCtrl();
    void InitYmeanGridCtrl();

    void ShowBlemishSpec();
    void GetBlemishSpec();
    void ShowLcbSpec();
    void GetLcbSpec();
    void ShowYmeanSpec();
    void GetYmeanSpec();
private:
    CGridCtrl m_clGridBlemishSpec;
    CGridCtrl m_clGridLcbSpec;
    CGridCtrl m_clGridYmenaSpec;
	CGridCtrl m_clGridUniformitySpec;

	CFont m_clFontSmall;
	CFont m_clFontMid;
	CFont m_clFontBig;

	int m_nUnit;

public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	afx_msg void OnNMDblClickedUniformitySpec(NMHDR* pNMHDR, LRESULT* pResult);

    afx_msg void OnNMDblClickedSfrBlockspot(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMDblClickedSfrLcb(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMDblClickedSfrYmean(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal();
	afx_msg void OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal();
	afx_msg void OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal();
	afx_msg void OnStnClickedStaticCcdSfrSpecOcXyVal();
	afx_msg void OnStnClickedStaticCcdSfrSpecOcThVal();
	afx_msg void OnBnClickedButtonCcdSfrSpecSave();
	afx_msg void OnBnClickedButtonCcdSfrSpecClose();
	
public:
	CLabel m_clColorStaticTopBar;
    CLabel m_clColorStaticCenterSfrLimit;
    CLabel m_clColorStaticCenterSfrLimitVal;
    CLabel m_clColorStaticCornerSfrLimit;
    CLabel m_clColorStaticCornerSfrLimitVal;
    CLabel m_clColorStaticCornerSfrDeviationLimit;
    CLabel m_clColorStaticCornerSfrDeviationLimitVal;
    CLabel m_clColorStaticOCXY;
    CLabel m_clColorStaticOCXYVal;
    CLabel m_clColorStaticOCTh;
    CLabel m_clColorStaticOCThVal;
	CExButton m_clColorButtonSave;
	CExButton m_clColorButtonClose;
};
