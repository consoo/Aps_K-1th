// CcdSfrSpecDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CcdSfrSpecDlg.h"
// CCcdSfrSpecDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCcdSfrSpecDlg, CDialogEx)

CCcdSfrSpecDlg::CCcdSfrSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCcdSfrSpecDlg::IDD, pParent)
{
}

CCcdSfrSpecDlg::~CCcdSfrSpecDlg()
{
}

void CCcdSfrSpecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_TOP_BAR, m_clColorStaticTopBar);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT, m_clColorStaticCenterSfrLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, m_clColorStaticCenterSfrLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT, m_clColorStaticCornerSfrLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, m_clColorStaticCornerSfrLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT, m_clColorStaticCornerSfrDeviationLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, m_clColorStaticCornerSfrDeviationLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_XY, m_clColorStaticOCXY);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, m_clColorStaticOCXYVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_TH, m_clColorStaticOCTh);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, m_clColorStaticOCThVal);
	DDX_Control(pDX, IDC_BUTTON_CCD_SFR_SPEC_SAVE, m_clColorButtonSave);
	DDX_Control(pDX, IDC_BUTTON_CCD_SFR_SPEC_CLOSE, m_clColorButtonClose);
}


BEGIN_MESSAGE_MAP(CCcdSfrSpecDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_SFR_SPEC_GRID_SPEC, &CCcdSfrSpecDlg::OnNMDblClickedSfrSpec)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_SAVE, &CCcdSfrSpecDlg::OnBnClickedButtonCcdSfrSpecSave)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_CLOSE, &CCcdSfrSpecDlg::OnBnClickedButtonCcdSfrSpecClose)	
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, &CCcdSfrSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, &CCcdSfrSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, &CCcdSfrSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, &CCcdSfrSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, &CCcdSfrSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal)
END_MESSAGE_MAP()


// CCcdSfrSpecDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
}

//-----------------------------------------------------------------------------
//
//	���̾�α� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
BOOL CCcdSfrSpecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	// �׸��� �ʱ�ȭ
	this->InitGridCtrl();

	this->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::InitCtrl()
{
	// ��Ʈ ����
	m_clFontSmall.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontMid.CreateFont(23, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontBig.CreateFont(32, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

	//m_clColorStaticTopBar.SetBkColor(RGB_COLOR_BLACK);
	//m_clColorStaticTopBar.SetTextColor(RGB_COLOR_WHITE);
	
	m_clColorStaticCenterSfrLimit.SetBkColor(RGB_DLG_BG);
	m_clColorStaticCenterSfrLimit.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticCenterSfrLimit.SetFont(&m_clFontSmall);

	m_clColorStaticCenterSfrLimitVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticCenterSfrLimitVal.SetFont(&m_clFontMid);

	m_clColorStaticCornerSfrLimit.SetBkColor(RGB_DLG_BG);
	m_clColorStaticCornerSfrLimit.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticCornerSfrLimit.SetFont(&m_clFontSmall);

	m_clColorStaticCornerSfrLimitVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticCornerSfrLimitVal.SetFont(&m_clFontMid);

	m_clColorStaticCornerSfrDeviationLimit.SetBkColor(RGB_DLG_BG);
	m_clColorStaticCornerSfrDeviationLimit.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticCornerSfrDeviationLimit.SetFont(&m_clFontSmall);

	m_clColorStaticCornerSfrDeviationLimitVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticCornerSfrDeviationLimitVal.SetFont(&m_clFontMid);

	m_clColorStaticOCXY.SetBkColor(RGB_DLG_BG);
	m_clColorStaticOCXY.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticOCXY.SetFont(&m_clFontSmall);

	m_clColorStaticOCXYVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticOCXYVal.SetFont(&m_clFontMid);

	m_clColorStaticOCTh.SetBkColor(RGB_DLG_BG);
	m_clColorStaticOCTh.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticOCTh.SetFont(&m_clFontSmall);

	m_clColorStaticOCThVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticOCThVal.SetFont(&m_clFontMid);

}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::InitGridCtrl()
{
	TCHAR* pszRow[] = { _T("") ,
                        _T("CL_00F_V"), _T("CR_OOF_V"), _T("CT_00F_H"), _T("CB_00F_H"), 
                        _T("LT_04F_V"),_T("LT_04F_H"), _T("RT_04F_V"), _T("RT_04F_H"),
                        _T("LB_04F_V"), _T("LB_04F_H"), _T("RB_04F_V"), _T("RB_04F_H"),
                        _T("LT_07F_V") ,_T("LT_07F_H"), _T("RT_07F_V"), _T("RT_07F_H"),
                        _T("LB_07F_V"), _T("LB_07F_H"),  _T("RB_07F_V") ,_T("RB_07F_H"),  
    };
	
	TCHAR* pszCol[] = { _T("FIELD"), _T("Spec") };

    CRect rect;
    int i, j;
    int DLG = IDC_STATIC_CCD_SFR_SPEC_GRID_SPEC;

    CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
    int SpecRow = MAX_SFR_INSP_CNT+1;//�Ʒ�
    int SpecCol = 2;//��
    int margin = 4;
    int gridHeight = 25;
    int gridWidth1 = 110;
    int gridWidth2 = 90;
    int totalWidth = gridWidth1 + (gridWidth2*(SpecCol - 1));
    //
    pWnd->GetWindowRect(rect);
    ScreenToClient(rect);

    rect.right = totalWidth + margin;
    rect.bottom = (gridHeight*SpecRow) + margin;
    pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.


	GetDlgItem(DLG)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_clGridSfrSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

	m_clGridSfrSpec.SetTextBkColor(RGB_COLOR_WHITE);
	m_clGridSfrSpec.SetFixedBkColor(GRID_COLOR_TITLE);
	m_clGridSfrSpec.SetFixedTextColor(RGB_COLOR_WHITE);
	m_clGridSfrSpec.SetReference_Setting();
	m_clGridSfrSpec.EnableSelection(FALSE);
	m_clGridSfrSpec.SetRowCount(SpecRow);
	m_clGridSfrSpec.SetColumnCount(SpecCol);
	m_clGridSfrSpec.SetFixedRowCount(1);
	m_clGridSfrSpec.SetFixedColumnCount(1);

	for (i = 0; i < SpecRow; i++)
	{
		m_clGridSfrSpec.SetRowHeight(i, gridHeight);
		m_clGridSfrSpec.SetItemText(i, 0, pszRow[i]);

		for (j = 0; j < SpecCol; j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
					m_clGridSfrSpec.SetColumnWidth(j, gridWidth1);
				}
				else
				{
					m_clGridSfrSpec.SetColumnWidth(j, gridWidth2);
				}

				m_clGridSfrSpec.SetItemText(i, j, pszCol[j]);
			}

			m_clGridSfrSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}

//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::SetUnit(int nUnit)
{
	m_nUnit = nUnit;
}

//-----------------------------------------------------------------------------
//
//	OnShowWindow
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow == TRUE)
	{
		this->ShowSfrSpec();
	}
}

//-----------------------------------------------------------------------------
//
//	SFR ������ -> �޸𸮿� ����
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::GetSfrSpec()
{
   
}

//-----------------------------------------------------------------------------
//
//	SFR SPEC ������ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::ShowSfrSpec()
{
	
}

//-----------------------------------------------------------------------------
//
//	CENTER SFR LIMIT
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticCenterSfrLimitVal.GetWindowText(sPos);
	//CKeyPadDlg keyDlg;
	//if (keyDlg.DoModal() == IDOK)
	CString celData;
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);

	if (dlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%.3lf"), atof(dlg.GetstrNum()));// keyDlg.GetstrKeypad());
		m_clColorStaticCenterSfrLimitVal.SetWindowText(sPos);
	}


}

//-----------------------------------------------------------------------------
//
//	CORNER SFR LIMIT
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticCornerSfrLimitVal.GetWindowText(sPos);
	//CKeyPadDlg keyDlg;
	//if (keyDlg.DoModal() == IDOK)
	CString celData;
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);

	if (dlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%.3lf"), atof(dlg.GetstrNum()));// keyDlg.GetstrKeypad());
		m_clColorStaticCornerSfrLimitVal.SetWindowText(sPos);
	}


}

//-----------------------------------------------------------------------------
//
//	�ڳʺ� SFR ���� LIMIT
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticCornerSfrDeviationLimitVal.GetWindowText(sPos);
	//CKeyPadDlg keyDlg;
	//if (keyDlg.DoModal() == IDOK)
	CString celData;
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);

	if (dlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%.3lf"), atof(dlg.GetstrNum()));// keyDlg.GetstrKeypad());
		m_clColorStaticCornerSfrDeviationLimitVal.SetWindowText(sPos);
	}


}

//-----------------------------------------------------------------------------
//
//	����˻� XY
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticOCXYVal.GetWindowText(sPos);
	//CKeyPadDlg keyDlg;
	//if (keyDlg.DoModal() == IDOK)
	CString celData;
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);

	if (dlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%.3lf"), atof(dlg.GetstrNum()));// keyDlg.GetstrKeypad());
		m_clColorStaticOCXYVal.SetWindowText(sPos);
	}


}

//-----------------------------------------------------------------------------
//
//	����˻� TH
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticOCThVal.GetWindowText(sPos);
	//CKeyPadDlg keyDlg;
	//if (keyDlg.DoModal() == IDOK)
	CString celData;
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);

	if (dlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%.3lf"), atof(dlg.GetstrNum()));// keyDlg.GetstrKeypad());
		m_clColorStaticOCThVal.SetWindowText(sPos);
	}


}

//-----------------------------------------------------------------------------
//
//	��ư : SAVE
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::OnBnClickedButtonCcdSfrSpecSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1463);
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	if (Task.AutoFlag == 2)
	{
		sLangChange.LoadStringA(IDS_STRING1468);
		delayMsg(sLangChange, 2000, M_COLOR_RED);// �Ͻ� ���� �� ��� �Ұ�
		return;
	}
	CString sTemp;
	sTemp.Format("SFR SPEC �����͸� �����Ͻðڽ��ϱ�?");
	if (!askMsg(sTemp))return;


	//if (g_ShowMsgModal(_T("Ȯ��"), _T("SFR SPEC �����͸� �����Ͻðڽ��ϱ�?"), RGB_COLOR_BLUE) == false)
	//	return;

	this->GetSfrSpec();

	//g_clSysData.Save();
	//g_clMandoSfrSpec[m_nUnit].Save();
	model.AcmisDataSave();

	theApp.MainDlg->putListLog(_T("[INFO] SFR SPEC ����"));// , 0, m_nUnit);
}

//-----------------------------------------------------------------------------
//
//	��ư : CLOSE
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::OnBnClickedButtonCcdSfrSpecClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::OnNMDblClickedSfrSpec(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	CString sData = _T("");
	int nRow, nCol;

	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;

	if (nRow >= 1 && nCol >= 1)
	{
		sData = m_clGridSfrSpec.GetItemText(nRow, nCol);
		//CKeyPadDlg keyDlg;
		//if (keyDlg.DoModal() == IDOK)
		CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)sData);

		if (dlg.DoModal() == IDOK)
		{
			sData.Format(_T("%.1lf"), atof(dlg.GetstrNum()));
			m_clGridSfrSpec.SetItemText(nRow, nCol, sData);

			m_clGridSfrSpec.Invalidate();
		}

	}
}

//-----------------------------------------------------------------------------
//
//	OnLButtonDown
//
//-----------------------------------------------------------------------------
void CCcdSfrSpecDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);

	CDialogEx::OnLButtonDown(nFlags, point);
}

//-----------------------------------------------------------------------------
//
//	PreTranslateMessage
//
//-----------------------------------------------------------------------------
BOOL CCcdSfrSpecDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_LBUTTONDOWN)
	{
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}