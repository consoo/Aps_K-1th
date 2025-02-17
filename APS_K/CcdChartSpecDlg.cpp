// CcdSfrSpecDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CcdChartSpecDlg.h"
// CCcdChartSpecDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCcdChartSpecDlg, CDialogEx)

CCcdChartSpecDlg::CCcdChartSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCcdChartSpecDlg::IDD, pParent)
{
}

CCcdChartSpecDlg::~CCcdChartSpecDlg()
{
}

void CCcdChartSpecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_TOP_BAR, m_clColorStaticTopBar);
	/*DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT, m_clColorStaticCenterSfrLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, m_clColorStaticCenterSfrLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT, m_clColorStaticCornerSfrLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, m_clColorStaticCornerSfrLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT, m_clColorStaticCornerSfrDeviationLimit);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, m_clColorStaticCornerSfrDeviationLimitVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_XY, m_clColorStaticOCXY);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, m_clColorStaticOCXYVal);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_TH, m_clColorStaticOCTh);
	DDX_Control(pDX, IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, m_clColorStaticOCThVal);*/

    DDX_Control(pDX, IDC_STATIC_CCD_HFOV_MIN, m_clColorStaticHfovMin);
    DDX_Control(pDX, IDC_STATIC_CCD_HFOV_MAX, m_clColorStaticHfovMax);
    DDX_Control(pDX, IDC_STATIC_CCD_HFOV_MIN_VAL, m_clColorStaticHfovMinVal);
    DDX_Control(pDX, IDC_STATIC_CCD_HFOV_MAX_VAL, m_clColorStaticHfovMaxVal);

    DDX_Control(pDX, IDC_STATIC_CCD_VFOV_MIN, m_clColorStaticVfovMin);
    DDX_Control(pDX, IDC_STATIC_CCD_VFOV_MAX, m_clColorStaticVfovMax);
    DDX_Control(pDX, IDC_STATIC_CCD_VFOV_MIN_VAL, m_clColorStaticVfovMinVal);
    DDX_Control(pDX, IDC_STATIC_CCD_VFOV_MAX_VAL, m_clColorStaticVfovMaxVal);

    DDX_Control(pDX, IDC_STATIC_CCD_DISTORTION_MIN, m_clColorStaticDistortionMin);
    DDX_Control(pDX, IDC_STATIC_CCD_DISTORTION_MAX, m_clColorStaticDistortionMax);
    DDX_Control(pDX, IDC_STATIC_CCD_DISTORTION_MIN_VAL, m_clColorStaticDistortionMinVal);
    DDX_Control(pDX, IDC_STATIC_CCD_DISTORTION_MAX_VAL, m_clColorStaticDistortionMaxVal);

	DDX_Control(pDX, IDC_BUTTON_CCD_SFR_SPEC_SAVE, m_clColorButtonSave);
	DDX_Control(pDX, IDC_BUTTON_CCD_SFR_SPEC_CLOSE, m_clColorButtonClose);
}

BEGIN_MESSAGE_MAP(CCcdChartSpecDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_CHART_GRID_SPEC, &CCcdChartSpecDlg::OnNMDblClickedChartSpec)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_SAVE, &CCcdChartSpecDlg::OnBnClickedButtonCcdChartSpecSave)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_CLOSE, &CCcdChartSpecDlg::OnBnClickedButtonCcdChartSpecClose)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal)
    ON_STN_CLICKED(IDC_STATIC_CCD_HFOV_MIN_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdHfovMinVal)
    ON_STN_CLICKED(IDC_STATIC_CCD_HFOV_MAX_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdHfovMaxVal)
    ON_STN_CLICKED(IDC_STATIC_CCD_VFOV_MIN_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdVfovMinVal)
    ON_STN_CLICKED(IDC_STATIC_CCD_VFOV_MAX_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdVfovMaxVal)
    ON_STN_CLICKED(IDC_STATIC_CCD_DISTORTION_MIN_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdDistortionMinVal)
    ON_STN_CLICKED(IDC_STATIC_CCD_DISTORTION_MAX_VAL, &CCcdChartSpecDlg::OnStnClickedStaticCcdDistortionMaxVal)
END_MESSAGE_MAP()


// CCcdChartSpecDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::OnPaint()
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
BOOL CCcdChartSpecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	// �׸��� �ʱ�ȭ
	this->InitChartGridCtrl();
	this->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::InitCtrl()
{
	// ��Ʈ ����
	m_clFontSmall.CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontMid.CreateFont(23, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	m_clFontBig.CreateFont(32, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));

	//m_clColorStaticTopBar.SetBkColor(RGB_COLOR_BLACK);
	//m_clColorStaticTopBar.SetTextColor(RGB_COLOR_WHITE);
	
	/*m_clColorStaticCenterSfrLimit.SetBkColor(RGB_DLG_BG);
	m_clColorStaticCenterSfrLimit.SetTextColor(RGB_COLOR_WHITE);
	m_clColorStaticCenterSfrLimit.SetFont(&m_clFontSmall);

	m_clColorStaticCenterSfrLimitVal.SetBkColor(RGB_COLOR_WHITE);
	m_clColorStaticCenterSfrLimitVal.SetFont(&m_clFontMid);*/
/*
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
	m_clColorStaticOCThVal.SetFont(&m_clFontMid);*/

    m_clColorStaticHfovMin.SetBkColor(RGB_DLG_BG);
    m_clColorStaticHfovMin.SetTextColor(RGB_COLOR_WHITE);
    m_clColorStaticHfovMin.SetFont(&m_clFontSmall);

    m_clColorStaticHfovMax.SetBkColor(RGB_DLG_BG);
    m_clColorStaticHfovMax.SetTextColor(RGB_COLOR_WHITE);
    m_clColorStaticHfovMax.SetFont(&m_clFontSmall);
    m_clColorStaticHfovMinVal.SetBkColor(RGB_COLOR_WHITE).SetFont(&m_clFontMid);
    m_clColorStaticHfovMaxVal.SetBkColor(RGB_COLOR_WHITE).SetFont(&m_clFontMid);

    m_clColorStaticVfovMin.SetBkColor(RGB_DLG_BG);
    m_clColorStaticVfovMin.SetTextColor(RGB_COLOR_WHITE);
    m_clColorStaticVfovMin.SetFont(&m_clFontSmall);

    m_clColorStaticVfovMax.SetBkColor(RGB_DLG_BG);
    m_clColorStaticVfovMax.SetTextColor(RGB_COLOR_WHITE);
    m_clColorStaticVfovMax.SetFont(&m_clFontSmall);
    m_clColorStaticVfovMinVal.SetBkColor(RGB_COLOR_WHITE).SetFont(&m_clFontMid);
    m_clColorStaticVfovMaxVal.SetBkColor(RGB_COLOR_WHITE).SetFont(&m_clFontMid);

    m_clColorStaticDistortionMin.SetBkColor(RGB_DLG_BG);
    m_clColorStaticDistortionMin.SetTextColor(RGB_COLOR_WHITE);
    m_clColorStaticDistortionMin.SetFont(&m_clFontSmall);

    m_clColorStaticDistortionMax.SetBkColor(RGB_DLG_BG);
    m_clColorStaticDistortionMax.SetTextColor(RGB_COLOR_WHITE);
    m_clColorStaticDistortionMax.SetFont(&m_clFontSmall);
    m_clColorStaticDistortionMinVal.SetBkColor(RGB_COLOR_WHITE).SetFont(&m_clFontMid);
    m_clColorStaticDistortionMaxVal.SetBkColor(RGB_COLOR_WHITE).SetFont(&m_clFontMid);
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::InitChartGridCtrl()
{
	TCHAR* pszRow[] = { _T("") ,
		_T("dRealGapX"), _T("dRealGapY"), _T("nFiducialMarkType"),
		_T("dModuleChartDistance"), _T("nDistortionAlrotithmType") 
	};
	
	TCHAR* pszCol[] = { _T("Chart"), _T("spec") };

	CRect rect;
	int i, j;
	int DLG = IDC_STATIC_CCD_CHART_GRID_SPEC;

	CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
	int SpecRow = ChartMaxCount;//�Ʒ�
	int SpecCol = 2;//��
	int margin = 4;
	int gridHeight = 25;
	int gridWidth1 = 180;
	int gridWidth2 = 110;
	int totalWidth = gridWidth1 + (gridWidth2*(SpecCol - 1));
	//
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);

	rect.right = totalWidth + margin;
	rect.bottom = (gridHeight*SpecRow) + margin;
	pWnd->MoveWindow(rect.left, rect.top, rect.right, rect.bottom);//���̾�α��� ũ��� ��ġ�� ������ ���� �Լ�.


	GetDlgItem(DLG)->GetWindowRect(rect);
	ScreenToClient(rect);
    m_clGridChartSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

	m_clGridChartSpec.SetTextBkColor(RGB_COLOR_WHITE);
	m_clGridChartSpec.SetFixedBkColor(GRID_COLOR_TITLE);
	m_clGridChartSpec.SetFixedTextColor(RGB_COLOR_WHITE);
	m_clGridChartSpec.SetReference_Setting();
	m_clGridChartSpec.EnableSelection(FALSE);
	m_clGridChartSpec.SetRowCount(SpecRow);
	m_clGridChartSpec.SetColumnCount(SpecCol);
	m_clGridChartSpec.SetFixedRowCount(1);
	m_clGridChartSpec.SetFixedColumnCount(1);

	for (i = 0; i < SpecRow; i++)
	{
        m_clGridChartSpec.SetRowHeight(i, gridHeight);
        m_clGridChartSpec.SetItemText(i, 0, pszRow[i]);

		for (j = 0; j < SpecCol; j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
                    m_clGridChartSpec.SetColumnWidth(j, gridWidth1);
				}
				else
				{
                    m_clGridChartSpec.SetColumnWidth(j, gridWidth2);
				}

                m_clGridChartSpec.SetItemText(i, j, pszCol[j]);
			}

            m_clGridChartSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}
//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::SetUnit(int nUnit)
{
	m_nUnit = nUnit;
}

//-----------------------------------------------------------------------------
//
//	OnShowWindow
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow == TRUE)
	{
		this->ShowChartSpec();
	}
}

//-----------------------------------------------------------------------------
//
//	SFR SPEC ������ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::ShowChartSpec()
{
	TCHAR szData[SIZE_OF_100BYTE];
	int i;

    _stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), model.m_dChartHfovMin);
    m_clColorStaticHfovMinVal.SetWindowText(szData);
    _stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), model.m_dChartHfovMax);
    m_clColorStaticHfovMaxVal.SetWindowText(szData);

    _stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), model.m_dChartVfovMin);
    m_clColorStaticVfovMinVal.SetWindowText(szData);
    _stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), model.m_dChartVfovMax);
    m_clColorStaticVfovMaxVal.SetWindowText(szData);

    _stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), model.m_dChartDistortionMin);
    m_clColorStaticDistortionMinVal.SetWindowText(szData);
    _stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), model.m_dChartDistortionMax);
    m_clColorStaticDistortionMaxVal.SetWindowText(szData);
	
	/*_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), model.m_dLimitSFRCent);
	m_clColorStaticCenterSfrLimitVal.SetWindowText(szData);

	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), model.m_dLimitSFRSide);
	m_clColorStaticCornerSfrLimitVal.SetWindowText(szData);

	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), g_clSysData.m_dLimitSFRBalance[m_nUnit]);
	m_clColorStaticCornerSfrDeviationLimitVal.SetWindowText(szData);

	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), g_clSysData.m_clOCSpec[m_nUnit].x);
	m_clColorStaticOCXYVal.SetWindowText(szData);

	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), g_clSysData.m_dOCSpecTh[m_nUnit]);
	m_clColorStaticOCThVal.SetWindowText(szData);
*/
	for (i = 0; i < ChartMaxCount-1; i++)
	{
        m_clGridChartSpec.SetItemText(i + 1, 1, model.m_ChartSpec[i],3);
	}

    m_clGridChartSpec.Invalidate();
}
//---------------------------------------------------------------------------
//
//	CENTER SFR LIMIT
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal()
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
void CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal()
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
void CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal()
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
void CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal()
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
void CCcdChartSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal()
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
void CCcdChartSpecDlg::OnBnClickedButtonCcdChartSpecSave()
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
	sTemp.Format("CHART SPEC �����͸� �����Ͻðڽ��ϱ�?");
	if (!askMsg(sTemp))return;

	/*if (g_ShowMsgModal(_T("Ȯ��"), _T("DEFECT/UNIFORM SPEC �����͸� �����Ͻðڽ��ϱ�?"), RGB_COLOR_BLUE) == false)
		return;*/

	this->GetChartSpec();

	//g_clSysData.Save();
	//g_clMandoSfrSpec[m_nUnit].Save();
	model.AcmisDataSave();
	theApp.MainDlg->putListLog(_T("[INFO] BLEMISH SPEC ����"));// , 0, m_nUnit);
}

//-----------------------------------------------------------------------------
//
//	SFR ������ -> �޸𸮿� ����
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::GetChartSpec()
{
	CString sData = _T("");
	int i;

	for (i = 0; i < ChartMaxCount-1; i++)
	{
		sData = m_clGridChartSpec.GetItemText(i + 1, 1);
		model.m_ChartSpec[i] = _ttof((TCHAR*)(LPCTSTR)sData);
	}

    m_clColorStaticHfovMinVal.GetWindowText(sData);
    model.m_dChartHfovMin = _ttof((TCHAR*)(LPCTSTR)sData);
    m_clColorStaticHfovMaxVal.GetWindowText(sData);
    model.m_dChartHfovMax = _ttof((TCHAR*)(LPCTSTR)sData);

    m_clColorStaticVfovMinVal.GetWindowText(sData);
    model.m_dChartVfovMin = _ttof((TCHAR*)(LPCTSTR)sData);
    m_clColorStaticVfovMaxVal.GetWindowText(sData);
    model.m_dChartVfovMax = _ttof((TCHAR*)(LPCTSTR)sData);


    m_clColorStaticDistortionMinVal.GetWindowText(sData);
    model.m_dChartDistortionMin = _ttof((TCHAR*)(LPCTSTR)sData);
    m_clColorStaticDistortionMaxVal.GetWindowText(sData);
    model.m_dChartDistortionMax = _ttof((TCHAR*)(LPCTSTR)sData);

	/*GetDlgItem(IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL)->GetWindowText(sData);
	model.m_dLimitSFRCent = _ttof((TCHAR*)(LPCTSTR)sData);

	GetDlgItem(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL)->GetWindowText(sData);
	model.m_dLimitSFRSide = _ttof((TCHAR*)(LPCTSTR)sData);

	GetDlgItem(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL)->GetWindowText(sData);
	g_clSysData.m_dLimitSFRBalance[m_nUnit] = _ttof((TCHAR*)(LPCTSTR)sData);

	GetDlgItem(IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL)->GetWindowText(sData);
	g_clSysData.m_clOCSpec[m_nUnit].x = _ttof((TCHAR*)(LPCTSTR)sData);
	g_clSysData.m_clOCSpec[m_nUnit].y = _ttof((TCHAR*)(LPCTSTR)sData);

	GetDlgItem(IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL)->GetWindowText(sData);
	g_clSysData.m_dOCSpecTh[m_nUnit] = _ttof((TCHAR*)(LPCTSTR)sData);*/

	//g_clSysData.Save();
}

//-----------------------------------------------------------------------------
//
//	��ư : CLOSE
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::OnBnClickedButtonCcdChartSpecClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::OnNMDblClickedChartSpec(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	CString sData = _T("");
	int nRow, nCol;

	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;

	if (nRow >= 1 && nCol >= 1)
	{
		sData = m_clGridChartSpec.GetItemText(nRow, nCol);
		CKeyPadDlg keyDlg;
		if (keyDlg.DoModal() == IDOK)
		{
			sData.Format(_T("%s"), keyDlg.GetstrKeypad());
            m_clGridChartSpec.SetItemText(nRow, nCol, sData);

            m_clGridChartSpec.Invalidate();
		}


	}
}

//-----------------------------------------------------------------------------
//
//	OnLButtonDown
//
//-----------------------------------------------------------------------------
void CCcdChartSpecDlg::OnLButtonDown(UINT nFlags, CPoint point)
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
BOOL CCcdChartSpecDlg::PreTranslateMessage(MSG* pMsg)
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

void CCcdChartSpecDlg::OnStnClickedStaticCcdHfovMinVal()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sPos = _T("");

    m_clColorStaticHfovMinVal.GetWindowText(sPos);
	CKeyPadDlg keyDlg;
	if (keyDlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%s"), keyDlg.GetstrKeypad());
        m_clColorStaticHfovMinVal.SetWindowText(sPos);
    }


}


void CCcdChartSpecDlg::OnStnClickedStaticCcdHfovMaxVal()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sPos = _T("");

    m_clColorStaticHfovMaxVal.GetWindowText(sPos);
	CKeyPadDlg keyDlg;
	if (keyDlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%s"), keyDlg.GetstrKeypad());
        m_clColorStaticHfovMaxVal.SetWindowText(sPos);
    }

 
}


void CCcdChartSpecDlg::OnStnClickedStaticCcdVfovMinVal()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sPos = _T("");

    m_clColorStaticVfovMinVal.GetWindowText(sPos);
	CKeyPadDlg keyDlg;
	if (keyDlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%s"), keyDlg.GetstrKeypad());
        m_clColorStaticVfovMinVal.SetWindowText(sPos);
    }

}


void CCcdChartSpecDlg::OnStnClickedStaticCcdVfovMaxVal()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sPos = _T("");

    m_clColorStaticVfovMaxVal.GetWindowText(sPos);
	CKeyPadDlg keyDlg;
	if (keyDlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%s"), keyDlg.GetstrKeypad());
        m_clColorStaticVfovMaxVal.SetWindowText(sPos);
    }

}


void CCcdChartSpecDlg::OnStnClickedStaticCcdDistortionMinVal()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sPos = _T("");

    m_clColorStaticDistortionMinVal.GetWindowText(sPos);
	CKeyPadDlg keyDlg;
	if (keyDlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%s"), keyDlg.GetstrKeypad());
        m_clColorStaticDistortionMinVal.SetWindowText(sPos);
    }

}


void CCcdChartSpecDlg::OnStnClickedStaticCcdDistortionMaxVal()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString sPos = _T("");

    m_clColorStaticDistortionMaxVal.GetWindowText(sPos);
	CKeyPadDlg keyDlg;
	if (keyDlg.DoModal() == IDOK)
	{
		sPos.Format(_T("%s"), keyDlg.GetstrKeypad());
        m_clColorStaticDistortionMaxVal.SetWindowText(sPos);
    }

  
}
