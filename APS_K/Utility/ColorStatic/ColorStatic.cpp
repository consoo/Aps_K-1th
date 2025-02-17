// ColorStatic.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ColorStatic.h"


// CColorStatic

IMPLEMENT_DYNAMIC(CColorStatic, CStatic)

CColorStatic::CColorStatic()
{
	// Set default parent window and notification message
	m_pParent = NULL;

	// Set default foreground text
	m_crTextColor = ::GetSysColor(COLOR_BTNTEXT);

	// Set default background text
	m_crBkColor = ::GetSysColor(COLOR_BTNFACE);

	// Set default background brush
	m_brBkgnd.CreateSolidBrush(m_crBkColor);
}

CColorStatic::~CColorStatic()
{
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CColorStatic �޽��� ó�����Դϴ�.



//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
HBRUSH CColorStatic::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	pDC->SetTextColor(m_crTextColor);
	pDC->SetBkColor(m_crBkColor);

	// TODO:  �θ� ó���Ⱑ ȣ����� ���� ��� Null�� �ƴ� �귯�ø� ��ȯ�մϴ�.
	return (HBRUSH)m_brBkgnd;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CColorStatic::OnDestroy()
{
	CStatic::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// Destroy resources
	m_brBkgnd.DeleteObject();
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CColorStatic::SetTextColor(COLORREF crTextColor)
{
	// Set new foreground color
	if (crTextColor != 0xffffffff)
	{
		m_crTextColor = crTextColor;
	}
	else // Set default foreground color
	{
		m_crTextColor = ::GetSysColor(COLOR_BTNTEXT);
	}

	// Repaint control
	Invalidate();
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CColorStatic::SetBkColor(COLORREF crBkColor)
{
	// Set new background color
	if (crBkColor != 0xffffffff)
	{
		m_crBkColor = crBkColor;
	}
	else // Set default background color
	{
		m_crBkColor = ::GetSysColor(COLOR_BTNFACE);
	}

	m_brBkgnd.DeleteObject();
	m_brBkgnd.CreateSolidBrush(m_crBkColor);

	// Repaint control
	Invalidate();
}