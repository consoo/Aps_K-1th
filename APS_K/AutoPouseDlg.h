#pragma once


// CAutoPouseDlg ��ȭ �����Դϴ�.

class CAutoPouseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoPouseDlg)

public:
	CAutoPouseDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAutoPouseDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_AUTORUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
