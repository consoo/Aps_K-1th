#pragma once


// CBarcodeDlg ��ȭ �����Դϴ�.

class CBarcodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBarcodeDlg)

public:
	CBarcodeDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBarcodeDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_BARCODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedBtnBarcodePortOpen();
	afx_msg void OnBnClickedBtnBarcodePortClose();
	afx_msg void OnBnClickedBtnBarcodeReade();
	CString m_Edit_strBarcode;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnBarcodeReadeStop();
};
