// HeaderCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "HeaderCtrlEx.h"
#include "MemDC_Header.h"
#include "afxtempl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx

CHeaderCtrlEx::CHeaderCtrlEx()
{
	m_clrBack = ::GetSysColor(COLOR_3DFACE);
	m_clrText = ::GetSysColor(COLOR_BTNTEXT);
}

CHeaderCtrlEx::~CHeaderCtrlEx()
{
	if (m_font.GetSafeHandle())
		m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CHeaderCtrlEx, CHeaderCtrl)
	//{{AFX_MSG_MAP(CHeaderCtrlEx)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(HDM_INSERTITEMA, OnInsertItem)
	ON_MESSAGE(HDM_INSERTITEMW, OnInsertItem)
	ON_MESSAGE(HDM_DELETEITEM, OnDeleteItem)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx message handlers

void CHeaderCtrlEx::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CHeaderCtrl::PreSubclassWindow();
}

void CHeaderCtrlEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CMemDC_Header memDC(&dc);
	Draw(&memDC);
	// Do not call CHeaderCtrl::OnPaint() for painting messages
}

void CHeaderCtrlEx::SetFont(CFont *pFont, BOOL bRedraw)
{
	m_font.Attach(pFont->m_hObject);
	//Do not call CHeaderCtrl::SetFont(pFont, bRedraw);
}

LRESULT CHeaderCtrlEx::OnInsertItem(WPARAM wParam, LPARAM lParam)
{
	int nIndex = (int)wParam; 
	LPHDITEM phdi = (LPHDITEM)lParam;
	m_sTitle.InsertAt(nIndex, _T(""));
	return Default();
}

LRESULT CHeaderCtrlEx::OnDeleteItem(WPARAM wParam, LPARAM lParam)
{
	int nIndex = (int)wParam;
	m_sTitle.RemoveAt(nIndex);
	return Default();
}

void CHeaderCtrlEx::Draw(CDC *pDC)
{
	CRect rectClip;
	if (pDC->GetClipBox(&rectClip) == ERROR)
		return;

	CString sTitle;
	CRect rectClient, rectItem, upperRect;
	GetClientRect(&rectClient);

    pDC->FillSolidRect(rectClip, m_clrBack);

	int iItems = GetItemCount();
	ASSERT(iItems >= 0);

	CPen penHighLight(PS_SOLID, 1, RGB(255,255,255));
	CPen penShadow(PS_SOLID, 1, RGB(128,128,128));
	CPen* pPen = pDC->GetCurrentPen();
	CFont* pFont = pDC->SelectObject(GetFont());

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_clrText);

	int iWidth = 0;

	for (int i = 0; i < iItems; i++)
	{
		int iItem = OrderToIndex(i);

		TCHAR szText[512];

		HDITEM hditem;
		hditem.mask = HDI_WIDTH|HDI_FORMAT|HDI_TEXT|HDI_IMAGE|HDI_BITMAP;
		hditem.pszText = szText;
		hditem.cchTextMax = sizeof(szText);
		VERIFY(GetItem(iItem, &hditem));

		VERIFY(GetItemRect(iItem, rectItem));

		if (rectItem.right >= rectClip.left || rectItem.left <= rectClip.right)
		{
			/////////////////////////////////////////////////////////////////
			//�� �۵�
			DRAWITEMSTRUCT disItem;
			disItem.CtlType = ODT_HEADER;
			disItem.CtlID = GetDlgCtrlID();
			disItem.itemID = iItem;
			disItem.itemAction = ODA_DRAWENTIRE;
			disItem.itemState = 0;
			disItem.hwndItem = m_hWnd;
			disItem.hDC = pDC->m_hDC;
			disItem.rcItem = rectItem;
			disItem.itemData = 0;
			DrawItem(&disItem);
			//
			/////////////////////////////////////////////////////////////////

			if ((!sTitle.IsEmpty() && sTitle != m_sTitle[i]) || //���� ������ �ٲ�� ���
				i==(iItems-1))	//�������� ���
			{	//���� ���� ����
				if (i==(iItems-1) && !m_sTitle[i].IsEmpty())
				{	//������ ���� ó��
					if (sTitle.IsEmpty())
					{	//������ �ϳ��� ���ؼ� ������ ������ ������ ���
						upperRect = rectItem;
						sTitle = m_sTitle[i];
					}
					else //������ ���� �������� ���Եǵ��� ��
						upperRect.right = rectItem.right;
				}
				if (!sTitle.IsEmpty())
				{	//���� ���� ���
					CFont *oldFont = NULL;
					if (m_font.GetSafeHandle())
						oldFont = pDC->SelectObject(&m_font);
					else
						pDC->SelectStockObject(DEFAULT_GUI_FONT);
					upperRect.bottom = (rectItem.top+rectItem.bottom)/2;
					pDC->DrawText(sTitle, upperRect, DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);
					if (m_font.GetSafeHandle())
						pDC->SelectObject(oldFont);
					else
						pDC->SelectStockObject(SYSTEM_FONT);
					sTitle.Empty();
				}
			} 
			{
				if (sTitle.IsEmpty())
				{
					upperRect = rectItem;
					sTitle = m_sTitle[i];
				}
				else
					upperRect.right = rectItem.right;
			}

			//�Ʒ� �� ���Ҽ� - Ÿ��Ʋ�� ������ ���
			if (!m_sTitle[i].IsEmpty())
			{ //������ �����ϸ�..
				pDC->SelectObject(&penShadow);
				pDC->MoveTo(rectItem.left, (rectItem.top+rectItem.bottom)/2-1);
				pDC->LineTo(rectItem.right, (rectItem.top+rectItem.bottom)/2-1);

				pDC->SelectObject(&penHighLight);
				pDC->MoveTo(rectItem.left, (rectItem.top+rectItem.bottom)/2);
				pDC->LineTo(rectItem.right, (rectItem.top+rectItem.bottom)/2);
			}
			//���� ���м�
			if (!m_sTitle[i].IsEmpty() && //Ÿ��Ʋ�� �����ϰ�
				(i+1<m_sTitle.GetSize() && m_sTitle[i] == m_sTitle[i+1])) //���� ����� �����ϴٸ�
			{ //���� ���� ���м�
				pDC->SelectObject(&penShadow);
				pDC->MoveTo(rectItem.right-1, (rectItem.top+rectItem.bottom)/2+1);
				pDC->LineTo(rectItem.right-1, rectItem.bottom-2);

				pDC->SelectObject(&penHighLight);
				pDC->MoveTo(rectItem.right, (rectItem.top+rectItem.bottom)/2+1);
				pDC->LineTo(rectItem.right, rectItem.bottom-2);
			} 
			else if (i < iItems)
			{	//�Ϲ� ���� ���м�
				pDC->SelectObject(&penShadow);
				pDC->MoveTo(rectItem.right-1, rectItem.top+2);
				pDC->LineTo(rectItem.right-1, rectItem.bottom-2);

				pDC->SelectObject(&penHighLight);
				pDC->MoveTo(rectItem.right, rectItem.top+2);
				pDC->LineTo(rectItem.right, rectItem.bottom-2);
			}
		}

		iWidth += hditem.cxy;
	}

	//iWidth = ���� �������� ��ü ���� �ǹ�
	if (iWidth > 0)
	{
		rectClient.right += 20; //��ü�� �ƿ츣�� �簢�� ����(������ ������ �ʵ���)
		//rectClient.right = rectClient.left + iWidth;
		pDC->Draw3dRect(rectClient, RGB(255,255,255), RGB(128,128,128));
	}

	pDC->SelectObject(pFont);
	pDC->SelectObject(pPen);

	penHighLight.DeleteObject();
	penShadow.DeleteObject();
}

void CHeaderCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct->CtlType == ODT_HEADER);

	HDITEM hdi;
	TCHAR  lpBuffer[256]={0};

	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 256;

	int nIndex = lpDrawItemStruct->itemID;
	GetItem(lpDrawItemStruct->itemID, &hdi);
	CRect rect = lpDrawItemStruct->rcItem;
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CFont *oldFont = NULL;
	if (m_font.GetSafeHandle())
		oldFont = pDC->SelectObject(&m_font);
	else
		pDC->SelectStockObject(DEFAULT_GUI_FONT);

	if (!m_sTitle[nIndex].IsEmpty()) //������ �����ϸ�..
	   rect.top = (rect.top+rect.bottom)/2+1;

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//���� ���� ó��
	CStringArray arrTitle;
	TCHAR seps[] = _T("\n");
	TCHAR* next_token;
	TCHAR *token = _tcstok_s(lpBuffer, seps, &next_token);
	while (token)
	{
		arrTitle.Add(token);
		token = _tcstok_s(NULL, seps, &next_token);
	}
	CRect titleRect = rect;
	for (int i=0; i<arrTitle.GetSize(); i++)
	{
		titleRect.top = rect.top + rect.Height()/(int)arrTitle.GetSize() * i;
		titleRect.bottom = titleRect.top + rect.Height()/(int)arrTitle.GetSize();
		pDC->DrawText(arrTitle[i], titleRect, DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);
	}

	arrTitle.RemoveAll();
	arrTitle.FreeExtra();
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////

	if (m_font.GetSafeHandle())
		pDC->SelectObject(oldFont);
	else
		pDC->SelectStockObject(SYSTEM_FONT);
}

void CHeaderCtrlEx::SetTitle(int nIndex, CString sTitle)
{
	ASSERT(nIndex >= 0 && nIndex < m_sTitle.GetSize());
	m_sTitle.SetAt(nIndex, sTitle);
}

void CHeaderCtrlEx::SetHeight(int nHeight)
{	//Header�� ���� ������ ������ ��Ʈ�� ���� �޶�����.
	//��Ʈ�� ������ �� �ش� ��Ʈ�� �����Ѵ�.
	//���� ����� ���� ��Ʈ�� SetFont���� ������ ��Ʈ�� ����ȴ�.
	nHeight -= 6; //���� ����
	CFont font;
	font.CreatePointFont((int)(nHeight/1.4)*10, _T("MS Serif"));
	CHeaderCtrl::SetFont(&font);
	font.Detach();
}
