
// AA Bonder.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once
#include "AA BonderDlg.h"
#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.

// CAABonderApp:
// �� Ŭ������ ������ ���ؼ��� AA Bonder.cpp�� �����Ͻʽÿ�.
//

class CAABonderApp : public CWinApp
{
public:
	CAABonderApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	CAABonderDlg *MainDlg;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedOk();
};

extern CAABonderApp theApp;