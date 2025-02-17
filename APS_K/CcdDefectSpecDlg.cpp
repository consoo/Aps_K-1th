// CcdSfrSpecDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CcdDefectSpecDlg.h"
// CCcdDefectSpecDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCcdDefectSpecDlg, CDialogEx)

CCcdDefectSpecDlg::CCcdDefectSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCcdDefectSpecDlg::IDD, pParent)
{
}

CCcdDefectSpecDlg::~CCcdDefectSpecDlg()
{
}

void CCcdDefectSpecDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CCcdDefectSpecDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_DEFECT_SPEC_GRID_SPEC, &CCcdDefectSpecDlg::OnNMDblClickedSfrSpec)
	ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_UNIFORMITY_SPEC_GRID_SPEC, &CCcdDefectSpecDlg::OnNMDblClickedUniformitySpec)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_SAVE, &CCcdDefectSpecDlg::OnBnClickedButtonCcdSfrSpecSave)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_CLOSE, &CCcdDefectSpecDlg::OnBnClickedButtonCcdSfrSpecClose)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, &CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, &CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, &CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, &CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, &CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal)
END_MESSAGE_MAP()


// CCcdDefectSpecDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::OnPaint()
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
BOOL CCcdDefectSpecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	// �׸��� �ʱ�ȭ
	this->InitGridCtrl();
	//this->InitUniformityGridCtrl();
	this->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::InitCtrl()
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
void CCcdDefectSpecDlg::InitGridCtrl()
{
	/*TCHAR* pszRow[] = { _T("") ,
         _T("BlockSizeDark"), _T("BlockSizeBright"),
		_T("ThresholdTypeHot"), _T("ThresholdTypeDark"), _T("ThresholdTypeBright"),
		_T("DefectThresholdHot"), _T("DefectThresholdDark"), _T("DefectThresholdBright"),
		_T("MaxHotDefectNum"), _T("MaxDarkDefectNum"), _T("MaxBrightDefectNum"),
		_T("MaxHotClusterNum") ,_T("MaxDarkClusterNum"), _T("MaxBrightClusterNum"),
		_T("EnableChannel") ,	_T("DefectInCouplet"),			 _T("DefectInCluster"),
		_T("DefectInClusterChannel"),_T("specColumnFPN")
	};*/
	TCHAR* pszRow[] = { _T("") ,
		_T("BlockSizeHot"),_T("BlockSizeDark"), _T("BlockSizeBright"),
		_T("ThresholdTypeHot"), _T("ThresholdTypeDark"), _T("ThresholdTypeBright"),
		_T("DefectThresholdHot"), _T("DefectThresholdDark"), _T("DefectThresholdBright"),
		_T("MaxHotDefectNum"), _T("MaxDarkDefectNum"), _T("MaxBrightDefectNum"),
		_T("MaxHotClusterNum") ,_T("MaxDarkClusterNum"), _T("MaxBrightClusterNum"),	//15��
		_T("EnableChannel") ,	 _T("DefectInCluster"),	//2��
		_T("DefectInClusterChannel"),	//_T("specColumnFPN"),
		_T("EnableCircle") ,
		_T("PosOffsetX"),	_T("PosOffsetY") ,
		_T("RadiusRatioX"),	_T("RadiusRatioY") ,
		_T("ThresholdRatio"),	_T("ROIRange") ,	 _T("UsediFixedCircle")		//10��
	};
	//
	TCHAR* pszCol[] = { _T("Defect"), _T("Spec") };

	CRect rect;
	int i, j;
	int DLG = IDC_STATIC_CCD_DEFECT_SPEC_GRID_SPEC;

	CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
	DefectMaxCount = sizeof(DEFECT_SPEC_NAME) / sizeof(DEFECT_SPEC_NAME[1]) + 1;
	int SpecRow = DefectMaxCount;//�Ʒ�
	int SpecCol = 2;//��
	int margin = 4;
	int gridHeight = 25;
	int gridWidth1 = 140;
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
	m_clGridDefectSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

	m_clGridDefectSpec.SetTextBkColor(RGB_COLOR_WHITE);
	m_clGridDefectSpec.SetFixedBkColor(GRID_COLOR_TITLE);
	m_clGridDefectSpec.SetFixedTextColor(RGB_COLOR_WHITE);
	m_clGridDefectSpec.SetReference_Setting();
	m_clGridDefectSpec.EnableSelection(FALSE);
	m_clGridDefectSpec.SetRowCount(SpecRow);
	m_clGridDefectSpec.SetColumnCount(SpecCol);
	m_clGridDefectSpec.SetFixedRowCount(1);
	m_clGridDefectSpec.SetFixedColumnCount(1);

	for (i = 0; i < SpecRow; i++)
	{
		m_clGridDefectSpec.SetRowHeight(i, gridHeight);
		m_clGridDefectSpec.SetItemText(i, 0, pszRow[i]);

		for (j = 0; j < SpecCol; j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
					m_clGridDefectSpec.SetColumnWidth(j, gridWidth1);
				}
				else
				{
					m_clGridDefectSpec.SetColumnWidth(j, gridWidth2);
				}

				m_clGridDefectSpec.SetItemText(i, j, pszCol[j]);
			}

			m_clGridDefectSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}
void CCcdDefectSpecDlg::InitUniformityGridCtrl()
{
    TCHAR* pszRow[] = { _T("") ,_T("nGridSizeX"), _T("nGridSizeY"),
                     _T("nColorSpaceType"),_T("nMaxDiffType"),
        _T("specMaxDiffRG"), _T("specMaxDiffBG"), _T("specMaxDiffRB")

	};
	TCHAR* pszCol[] = { _T("Uniformity"), _T("Spec") };

	CRect rect;
	int i, j;
	int DLG = IDC_STATIC_CCD_UNIFORMITY_SPEC_GRID_SPEC;

	CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
	int SpecRow = UniformityMaxCount;//�Ʒ�
	int SpecCol = 2;//��
	int margin = 4;
	int gridHeight = 25;
	int gridWidth1 = 140;
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
	m_clGridUniformitySpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

	m_clGridUniformitySpec.SetTextBkColor(RGB_COLOR_WHITE);
	m_clGridUniformitySpec.SetFixedBkColor(GRID_COLOR_TITLE);
	m_clGridUniformitySpec.SetFixedTextColor(RGB_COLOR_WHITE);
	m_clGridUniformitySpec.SetReference_Setting();
	m_clGridUniformitySpec.EnableSelection(FALSE);
	m_clGridUniformitySpec.SetRowCount(SpecRow);
	m_clGridUniformitySpec.SetColumnCount(SpecCol);
	m_clGridUniformitySpec.SetFixedRowCount(1);
	m_clGridUniformitySpec.SetFixedColumnCount(1);

	for (i = 0; i < SpecRow; i++)
	{
		m_clGridUniformitySpec.SetRowHeight(i, gridHeight);
		m_clGridUniformitySpec.SetItemText(i, 0, pszRow[i]);

		for (j = 0; j < SpecCol; j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
					m_clGridUniformitySpec.SetColumnWidth(j, gridWidth1);
				}
				else
				{
					m_clGridUniformitySpec.SetColumnWidth(j, gridWidth2);
				}

				m_clGridUniformitySpec.SetItemText(i, j, pszCol[j]);
			}

			m_clGridUniformitySpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}
//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::SetUnit(int nUnit)
{
	m_nUnit = nUnit;
}

//-----------------------------------------------------------------------------
//
//	OnShowWindow
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow == TRUE)
	{
		this->ShowDefectSpec();
		//this->ShowUniformitySpec();
	}
}

//-----------------------------------------------------------------------------
//
//	SFR SPEC ������ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::ShowDefectSpec()
{
	TCHAR szData[SIZE_OF_100BYTE];
	int i;

	for (i = 0; i < DefectMaxCount-1; i++)
	{
		m_clGridDefectSpec.SetItemText(i + 1, 1, model.m_DefectSpec[i], 3);
	}
	m_clGridDefectSpec.Invalidate();
}
//-----------------------------------------------------------------------------
//
//	UNIFORMITY SPEC ������ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::ShowUniformitySpec()
{
	TCHAR szData[SIZE_OF_100BYTE];
	int i;

	for (i = 0; i < UniformityMaxCount-1; i++)
	{
		m_clGridUniformitySpec.SetItemText(i + 1, 1, model.m_UniformSpec[i], 3);
	}
	m_clGridUniformitySpec.Invalidate();
}

//-----------------------------------------------------------------------------
//
//	CENTER SFR LIMIT
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal()
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
void CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal()
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
void CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal()
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
void CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticOCXYVal.GetWindowText(sPos);
	CKeyPadDlg keyDlg;
	if (keyDlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%s"), keyDlg.GetstrKeypad());
		m_clColorStaticOCXYVal.SetWindowText(sPos);
	}

}

//-----------------------------------------------------------------------------
//
//	����˻� TH
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos = _T("");

	m_clColorStaticOCThVal.GetWindowText(sPos);
	CKeyPadDlg keyDlg;
	if (keyDlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%s"), keyDlg.GetstrKeypad());
		m_clColorStaticOCThVal.SetWindowText(sPos);
	}


}

//-----------------------------------------------------------------------------
//
//	��ư : SAVE
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::OnBnClickedButtonCcdSfrSpecSave()
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
	sTemp.Format("DEFECT/UNIFORM SPEC �����͸� �����Ͻðڽ��ϱ�?");
	if (!askMsg(sTemp))return;

	//if (g_ShowMsgModal(_T("Ȯ��"), _T("DEFECT/UNIFORM SPEC �����͸� �����Ͻðڽ��ϱ�?"), RGB_COLOR_BLUE) == false)
	//	return;

	this->GetDefectSpec();
    //this->GetUniformitySpec();
	//g_clSysData.Save();
	//g_clMandoSfrSpec[m_nUnit].Save();
	model.AcmisDataSave();
	theApp.MainDlg->putListLog(_T("[INFO] DEFECT/UNIFORM SPEC ����"));// , 0, m_nUnit);
}

//-----------------------------------------------------------------------------
//
//	SFR ������ -> �޸𸮿� ����
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::GetDefectSpec()
{
	CString sData = _T("");
	int i;

	for (i = 0; i < DefectMaxCount-1; i++)
	{
		sData = m_clGridDefectSpec.GetItemText(i + 1, 1);
		model.m_DefectSpec[i] = _ttof((TCHAR*)(LPCTSTR)sData);
	}
}
//-----------------------------------------------------------------------------
//
//	UNIFORMITY ������ -> �޸𸮿� ����
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::GetUniformitySpec()
{
	CString sData = _T("");
	int i;

	for (i = 0; i < UniformityMaxCount-1; i++)
	{
		sData = m_clGridUniformitySpec.GetItemText(i + 1, 1);
		model.m_UniformSpec[i] = _ttof((TCHAR*)(LPCTSTR)sData);
	}
	//g_clSysData.Save();
}

//-----------------------------------------------------------------------------
//
//	��ư : CLOSE
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::OnBnClickedButtonCcdSfrSpecClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::OnNMDblClickedSfrSpec(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	CString sData = _T("");
	int nRow, nCol;

	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;

	if (nRow >= 1 && nCol >= 1)
	{
		sData = m_clGridDefectSpec.GetItemText(nRow, nCol);

		CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)sData);
		if (dlg.DoModal() == IDOK)
		{
			float f_num = (float)atof(dlg.GetstrNum());
			sData.Format(_T("%.3f"), f_num);
			m_clGridDefectSpec.SetItemText(nRow, nCol, sData);

			m_clGridDefectSpec.Invalidate();
		}

	}
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::OnNMDblClickedUniformitySpec(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	CString sData = _T("");
	int nRow, nCol;

	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;

	if (nRow >= 1 && nCol >= 1)
	{
		sData = m_clGridUniformitySpec.GetItemText(nRow, nCol);
		CKeyPadDlg keyDlg;
		if (keyDlg.DoModal() == IDOK)
		{
			sData.Format(_T("%s"), keyDlg.GetstrKeypad());
			m_clGridUniformitySpec.SetItemText(nRow, nCol, sData);

			m_clGridUniformitySpec.Invalidate();
		}


	}
}

//-----------------------------------------------------------------------------
//
//	OnLButtonDown
//
//-----------------------------------------------------------------------------
void CCcdDefectSpecDlg::OnLButtonDown(UINT nFlags, CPoint point)
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
BOOL CCcdDefectSpecDlg::PreTranslateMessage(MSG* pMsg)
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