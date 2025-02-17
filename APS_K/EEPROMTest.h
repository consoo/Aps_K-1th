#pragma once

#include "Label\Label.h"
#include "ExButton\exbutton.h"
// CEEPROMTest ��ȭ �����Դϴ�.

class CEEPROMTest : public CDialogEx
{
	DECLARE_DYNAMIC(CEEPROMTest)

public:
	CEEPROMTest(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEEPROMTest();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EEPROM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CExButton	m_Button_Ascii;
	CExButton	m_Button_Float;
	CExButton	m_Button_Dec;
	CExButton	m_Button_Hex;
public:
	void SetInterface();
	CString GetErrorStr(int errorCode);
	void FuncModeChange(int type);
public:
	CString	m_Str_SlaveAddr;
	CString	m_Str_Addr;
	CString	m_Str_Data_Hex;
	CString	m_Str_Data_Ch;
	//
	BYTE	m_Str_AddrLength;
	BYTE	m_Str_DataLength;

public:


	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnRead();
	afx_msg void OnBnClickedBtnMiuOpen();
	afx_msg void OnBnClickedBtnMiuStop();
	afx_msg void OnBnClickedBtnMiuClose();
	afx_msg void OnBnClickedBtnWrite();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnOutputAscii();
	afx_msg void OnBnClickedBtnOutputFloat();
	afx_msg void OnBnClickedBtnOutputDec();
	afx_msg void OnBnClickedBtnOutputHex();
};
