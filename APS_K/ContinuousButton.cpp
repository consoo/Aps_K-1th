// ContinuousButton.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AA Bonder.h"
#include "ContinuousButton.h"


// CContinuousButton

IMPLEMENT_DYNAMIC(CContinuousButton, CButton)

CContinuousButton::CContinuousButton()
{
}

CContinuousButton::~CContinuousButton()
{
}


BEGIN_MESSAGE_MAP(CContinuousButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CContinuousButton �޽��� ó�����Դϴ�.




void CContinuousButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SetTimer(100, 300, NULL);
	GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
	CButton::OnLButtonDown(nFlags, point);
}


void CContinuousButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	KillTimer(100);
	KillTimer(200);
	KillTimer(300);
	CButton::OnLButtonUp(nFlags, point);
}


void CContinuousButton::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(nIDEvent == 100){
		GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
		KillTimer(100);
		SetTimer(200, 100, NULL);
	}
	else if(nIDEvent == 200){
		GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
		KillTimer(200);
		SetTimer(300, 30, NULL);
	}
	else if(nIDEvent == 300){
		GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
	}
	CButton::OnTimer(nIDEvent);
}
