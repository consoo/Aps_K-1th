#pragma once


// CEditDlg ��ȭ �����Դϴ�.

class CEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditDlg)

public:
	CEditDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEditDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CString getTextOnEdit();

	CString m_strText;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
