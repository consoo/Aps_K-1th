// CcdSfrSpecDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CcdRiOcSpecDlg.h"
// CCcdRiOcSpecDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCcdRiOcSpecDlg, CDialogEx)

CCcdRiOcSpecDlg::CCcdRiOcSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCcdRiOcSpecDlg::IDD, pParent)
{
}

CCcdRiOcSpecDlg::~CCcdRiOcSpecDlg()
{
}

void CCcdRiOcSpecDlg::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, m_clColorStaticSaturationMinVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_TH, m_clColorStaticOCTh);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, m_clColorStaticSaturationMaxVal);

	DDX_Control(pDX, IDC_BUTTON_CCD_SFR_SPEC_SAVE, m_clColorButtonSave);
	DDX_Control(pDX, IDC_BUTTON_CCD_SFR_SPEC_CLOSE, m_clColorButtonClose);
}


BEGIN_MESSAGE_MAP(CCcdRiOcSpecDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_OC_SPEC_GRID_SPEC, &CCcdRiOcSpecDlg::OnNMDblClickedUniformitySpec)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_SAVE, &CCcdRiOcSpecDlg::OnBnClickedButtonCcdSfrSpecSave)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_CLOSE, &CCcdRiOcSpecDlg::OnBnClickedButtonCcdSfrSpecClose)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, &CCcdRiOcSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, &CCcdRiOcSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, &CCcdRiOcSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, &CCcdRiOcSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, &CCcdRiOcSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal)
END_MESSAGE_MAP()


// CCcdRiOcSpecDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::OnPaint()
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
BOOL CCcdRiOcSpecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	// �׸��� �ʱ�ȭ
	this->InitSaturationGridCtrl();
	this->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::InitCtrl()
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

	m_clColorStaticSaturationMinVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticSaturationMinVal.SetFont(&m_clFontMid);

	m_clColorStaticOCTh.SetBkColor(RGB_DLG_BG);
	m_clColorStaticOCTh.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticOCTh.SetFont(&m_clFontSmall);

	m_clColorStaticSaturationMaxVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticSaturationMaxVal.SetFont(&m_clFontMid);

}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::InitSaturationGridCtrl()
{
	TCHAR* pszRow[] = { _T("") ,
        _T("nFiducialMarkNum"),_T("nFiducialMarkType")
	};

	TCHAR* pszCol[] = { _T(" "), _T("Min") , _T("Max") , _T("Threshold") };

	CRect rect;
	int i, j;
	int DLG = IDC_STATIC_CCD_OC_SPEC_GRID_SPEC;

	CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
	int SpecRow = OCMaxCount;//�Ʒ�
	int SpecCol = 4;//��
	int margin = 4;
	int gridHeight = 23;
	int gridWidth1 = 120;
	int gridWidth2 = 70;
	int totalWidth = gridWidth1 + (gridWidth2*(SpecCol - 1));
	//
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);

	rect.right = totalWidth + margin;
	rect.bottom = (gridHeight*SpecRow) + margin;
	pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.


	GetDlgItem(DLG)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_clGridOCSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

	m_clGridOCSpec.SetTextBkColor(RGB_COLOR_WHITE);
	m_clGridOCSpec.SetFixedBkColor(GRID_COLOR_TITLE);
	m_clGridOCSpec.SetFixedTextColor(RGB_COLOR_WHITE);
	m_clGridOCSpec.SetReference_Setting();
	m_clGridOCSpec.EnableSelection(FALSE);
	m_clGridOCSpec.SetRowCount(SpecRow);
	m_clGridOCSpec.SetColumnCount(SpecCol);
	m_clGridOCSpec.SetFixedRowCount(1);
	m_clGridOCSpec.SetFixedColumnCount(1);



	m_clGridOCSpec.SetItemText(1, 0, "Center_Top");
	m_clGridOCSpec.SetItemText(2, 0, "Center_Bottom");
	m_clGridOCSpec.SetItemText(3, 0, "Center_Left");
	m_clGridOCSpec.SetItemText(4, 0, "Center_Right");
	//
	m_clGridOCSpec.SetItemText(5, 0, "0_4_LT_H");
	m_clGridOCSpec.SetItemText(6, 0, "0_4_LT_V");
	m_clGridOCSpec.SetItemText(7, 0, "0_4_RT_H");
	m_clGridOCSpec.SetItemText(8, 0, "0_4_RT_V");
	m_clGridOCSpec.SetItemText(9, 0, "0_4_LB_H");
	m_clGridOCSpec.SetItemText(10, 0, "0_4_LB_V");
	m_clGridOCSpec.SetItemText(11, 0, "0_4_RB_H");
	m_clGridOCSpec.SetItemText(12, 0, "0_4_RB_V");
	m_clGridOCSpec.SetItemText(13, 0, "0_4_LT_H");
	m_clGridOCSpec.SetItemText(14, 0, "0_4_LT_V");
	m_clGridOCSpec.SetItemText(15, 0, "0_4_RT_H");
	m_clGridOCSpec.SetItemText(16, 0, "0_4_RT_V");
	m_clGridOCSpec.SetItemText(17, 0, "0_4_LB_H");
	m_clGridOCSpec.SetItemText(18, 0, "0_4_LB_V");
	m_clGridOCSpec.SetItemText(19, 0, "0_4_RB_H");
	m_clGridOCSpec.SetItemText(20, 0, "0_4_RB_V");
	//
	m_clGridOCSpec.SetItemText(21, 0, "0_8_LT_T");
	m_clGridOCSpec.SetItemText(22, 0, "0_8_LT_B");
	m_clGridOCSpec.SetItemText(23, 0, "0_8_LT_L");
	m_clGridOCSpec.SetItemText(24, 0, "0_8_LT_R");
	m_clGridOCSpec.SetItemText(25, 0, "0_8_RT_T");
	m_clGridOCSpec.SetItemText(26, 0, "0_8_RT_B");
	m_clGridOCSpec.SetItemText(27, 0, "0_8_RT_L");
	m_clGridOCSpec.SetItemText(28, 0, "0_8_RT_R");
	m_clGridOCSpec.SetItemText(29, 0, "0_8_LB_T");
	m_clGridOCSpec.SetItemText(30, 0, "0_8_LB_B");
	m_clGridOCSpec.SetItemText(31, 0, "0_8_LB_L");
	m_clGridOCSpec.SetItemText(32, 0, "0_8_LB_R");
	m_clGridOCSpec.SetItemText(33, 0, "0_8_RB_T");
	m_clGridOCSpec.SetItemText(34, 0, "0_8_RB_B");
	m_clGridOCSpec.SetItemText(35, 0, "0_8_RB_L");
	m_clGridOCSpec.SetItemText(36, 0, "0_8_RB_R");
	for (i = 0; i < SpecRow; i++)
	{
		m_clGridOCSpec.SetRowHeight(i, gridHeight);
		//m_clGridOCSpec.SetItemText(i, 0, pszRow[i]);

		for (j = 0; j < SpecCol; j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
					m_clGridOCSpec.SetColumnWidth(j, gridWidth1);
				}
				else
				{
					m_clGridOCSpec.SetColumnWidth(j, gridWidth2);
				}

				m_clGridOCSpec.SetItemText(i, j, pszCol[j]);
			}

			m_clGridOCSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}
//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::SetUnit(int nUnit)
{
	m_nUnit = nUnit;
}

//-----------------------------------------------------------------------------
//
//	OnShowWindow
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow == TRUE)
	{
		this->ShowOcSpec();
	}
}

//-----------------------------------------------------------------------------
//
//	UNIFORMITY SPEC ������ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::ShowOcSpec()
{
	TCHAR szData[SIZE_OF_100BYTE];
	int i;

	for (i = 0; i < 36; i++)//for (i = 0; i < MTF_INSP_CNT; i++)
	{
		m_clGridOCSpec.SetItemText(i + 1, 1, (int)model.m_SaturationMin[i], 3);
		m_clGridOCSpec.SetItemText(i + 1, 2, (int)model.m_SaturationMax[i], 3);
		m_clGridOCSpec.SetItemText(i + 1, 3, (int)model.m_SaturationThreshold[i], 3);
	}
	m_clGridOCSpec.Invalidate();


	/*_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), model.m_dStaturationMin);
	m_clColorStaticSaturationMinVal.SetWindowText(szData);
	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), model.m_dStaturationMax);
	m_clColorStaticSaturationMaxVal.SetWindowText(szData);*/
}

//-----------------------------------------------------------------------------
//
//	CENTER SFR LIMIT
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticCenterSfrLimitVal.GetWindowText(sPos);
	CKeyPadDlg keyDlg;
	if (keyDlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%s"), keyDlg.GetstrKeypad());
		m_clColorStaticCenterSfrLimitVal.SetWindowText(sPos);
	}
}

//-----------------------------------------------------------------------------
//
//	CORNER SFR LIMIT
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticCornerSfrLimitVal.GetWindowText(sPos);
	CKeyPadDlg keyDlg;
	if (keyDlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%s"), keyDlg.GetstrKeypad());
		m_clColorStaticCornerSfrLimitVal.SetWindowText(sPos);
	}

}

//-----------------------------------------------------------------------------
//
//	�ڳʺ� SFR ���� LIMIT
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticCornerSfrDeviationLimitVal.GetWindowText(sPos);
	CKeyPadDlg keyDlg;
	if (keyDlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%s"), keyDlg.GetstrKeypad());
		m_clColorStaticCornerSfrDeviationLimitVal.SetWindowText(sPos);
	}
}

//-----------------------------------------------------------------------------
//
//	����˻� XY
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticSaturationMinVal.GetWindowText(sPos);
	//CKeyPadDlg keyDlg;
	//if (keyDlg.DoModal() == IDOK)
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)sPos);

	if (dlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%.3lf"), atof(dlg.GetstrNum())); //keyDlg.GetstrKeypad());
		m_clColorStaticSaturationMinVal.SetWindowText(sPos);
	}

}

//-----------------------------------------------------------------------------
//
//	����˻� TH
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticSaturationMaxVal.GetWindowText(sPos);
	//CKeyPadDlg keyDlg;
	//if (keyDlg.DoModal() == IDOK)
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)sPos);

	if (dlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%.3lf"), atof(dlg.GetstrNum())); //keyDlg.GetstrKeypad());
		m_clColorStaticSaturationMaxVal.SetWindowText(sPos);
	}


}

//-----------------------------------------------------------------------------
//
//	��ư : SAVE
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::OnBnClickedButtonCcdSfrSpecSave()
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
	sTemp.Format("SATURATION SPEC �����͸� �����Ͻðڽ��ϱ�?");
	if (!askMsg(sTemp))return;
		
	//if (g_ShowMsgModal(_T("Ȯ��"), _T("OC SPEC �����͸� �����Ͻðڽ��ϱ�?"), RGB_COLOR_BLUE) == false)
		

	this->GetOcSpec();

	model.AcmisDataSave();
	theApp.MainDlg->putListLog(_T("[INFO] SATURATION SPEC ����"));// , 0, m_nUnit);

}

//-----------------------------------------------------------------------------
//
//	UNIFORMITY ������ -> �޸𸮿� ����
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::GetOcSpec()
{
	CString sData = _T("");
	int i;

	for (i = 0; i < 36; i++)//for (i = 0; i < MTF_INSP_CNT; i++)
	{
		sData = m_clGridOCSpec.GetItemText(i + 1, 1);
		model.m_SaturationMin[i] = _ttof((TCHAR*)(LPCTSTR)sData);
		sData = m_clGridOCSpec.GetItemText(i + 1, 2);
		model.m_SaturationMax[i] = _ttof((TCHAR*)(LPCTSTR)sData);
		sData = m_clGridOCSpec.GetItemText(i + 1, 3);
		model.m_SaturationThreshold[i] = _ttof((TCHAR*)(LPCTSTR)sData);
	}

	/*m_clColorStaticSaturationMinVal.GetWindowText(sData);
	model.m_dStaturationMin = _ttof((TCHAR*)(LPCTSTR)sData);
	m_clColorStaticSaturationMaxVal.GetWindowText(sData);
	model.m_dStaturationMax = _ttof((TCHAR*)(LPCTSTR)sData);*/
	//g_clSysData.Save();
}

//-----------------------------------------------------------------------------
//
//	��ư : CLOSE
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::OnBnClickedButtonCcdSfrSpecClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::OnNMDblClickedUniformitySpec(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	CString sData = _T("");
	int nRow, nCol;

	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;

	if (nRow >= 1 && nCol >= 1)
	{
		sData = m_clGridOCSpec.GetItemText(nRow, nCol);
		//CKeyPadDlg keyDlg;
		//if (keyDlg.DoModal() == IDOK)
		CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)sData);

		if (dlg.DoModal() == IDOK)
		{
			sData.Format(_T("%.3lf"), atof(dlg.GetstrNum())); //keyDlg.GetstrKeypad());
			m_clGridOCSpec.SetItemText(nRow, nCol, sData);

			m_clGridOCSpec.Invalidate();
		}

	}
}

//-----------------------------------------------------------------------------
//
//	OnLButtonDown
//
//-----------------------------------------------------------------------------
void CCcdRiOcSpecDlg::OnLButtonDown(UINT nFlags, CPoint point)
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
BOOL CCcdRiOcSpecDlg::PreTranslateMessage(MSG* pMsg)
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