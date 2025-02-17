#pragma once


// CDisplacementDlg ��ȭ �����Դϴ�.

class CDisplacementDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDisplacementDlg)

public:
	CDisplacementDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDisplacementDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_DISPLACEMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	afx_msg void OnBnClickedBtnCommOpen();
	afx_msg void OnBnClickedBtnCommClose();
	afx_msg void OnBnClickedBtnScan();
	afx_msg void OnBnClickedBtnDlgClose();
	afx_msg void OnBnClickedBtnZeroOn();
	afx_msg void OnBnClickedBtnZeroOff();
	afx_msg void OnBnClickedBtnKeyLock();
	afx_msg void OnBnClickedBtnKeyUnlock();

	double m_Edit_ResultVal;
};
