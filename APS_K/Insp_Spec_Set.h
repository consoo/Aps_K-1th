#pragma once


// CInsp_Spec_Set ��ȭ �����Դϴ�.

class CInsp_Spec_Set : public CDialogEx
{
	DECLARE_DYNAMIC(CInsp_Spec_Set)

public:
	CInsp_Spec_Set(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInsp_Spec_Set();

	void getData();
	void putData();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_INSP_SPEC_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSave();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnEnChangeDefectpixelDefectpixeltotalBright();
	afx_msg void OnEnChangeDefectpixelDefectpixeltotalDark();
	afx_msg void OnEnChangeDefectpixelDefectpixeltotalHot();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
