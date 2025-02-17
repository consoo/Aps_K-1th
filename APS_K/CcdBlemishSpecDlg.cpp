// CcdSfrSpecDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CcdBlemishSpecDlg.h"
// CCcdBlemishSpecDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCcdBlemishSpecDlg, CDialogEx)

CCcdBlemishSpecDlg::CCcdBlemishSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCcdBlemishSpecDlg::IDD, pParent)
{
}

CCcdBlemishSpecDlg::~CCcdBlemishSpecDlg()
{
}

void CCcdBlemishSpecDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CCcdBlemishSpecDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
    ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_BLEMISH_SPEC_GRID_SPEC, &CCcdBlemishSpecDlg::OnNMDblClickedSfrBlockspot)
    ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_LCB_SPEC_GRID_SPEC, &CCcdBlemishSpecDlg::OnNMDblClickedSfrLcb)
    ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_YMEAN_SPEC_GRID_SPEC, &CCcdBlemishSpecDlg::OnNMDblClickedSfrYmean)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_SAVE, &CCcdBlemishSpecDlg::OnBnClickedButtonCcdSfrSpecSave)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_CLOSE, &CCcdBlemishSpecDlg::OnBnClickedButtonCcdSfrSpecClose)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, &CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, &CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, &CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, &CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, &CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal)
END_MESSAGE_MAP()


// CCcdBlemishSpecDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::OnPaint()
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
BOOL CCcdBlemishSpecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	// �׸��� �ʱ�ȭ
    this->InitBlemishGridCtrl();
    this->InitLcbGridCtrl();
    this->InitYmeanGridCtrl();

    
	this->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::InitCtrl()
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
void CCcdBlemishSpecDlg::InitBlemishGridCtrl()
{
    TCHAR* pszRow[] = { _T("") ,
        _T("nBlockWidth"), _T("nBlockHeight"), _T("nClusterSize"),
        _T("nDefectInCluster"), _T("dDefectRatio"),
        _T("bEnableCircle"), _T("nPosOffsetX"), _T("nPosOffsetY"),
        _T("dRadiusRatioX"), _T("dRadiusRatioY"), _T("dThresholdRatio"),
        _T("dROIRange"), _T("nUsedFixedCircle") ,_T("bEnableMultiCircle"),
        _T("dZoneSizeRatio1") ,_T("dZoneSizeRatio2"), _T("dZoneSizeRatio3"),
        _T("dThreshold1") ,_T("dThreshold2"), _T("dThreshold3")
    };

    TCHAR* pszCol[] = { _T("Blemish"), _T("Spec") };

    CRect rect;
    int i, j;
    int DLG = IDC_STATIC_CCD_BLEMISH_SPEC_GRID_SPEC;

    CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
    int SpecRow = BlemishMaxCount;//�Ʒ�
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
    m_clGridBlemishSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

    m_clGridBlemishSpec.SetTextBkColor(RGB_COLOR_WHITE);
    m_clGridBlemishSpec.SetFixedBkColor(GRID_COLOR_TITLE);
    m_clGridBlemishSpec.SetFixedTextColor(RGB_COLOR_WHITE);
    m_clGridBlemishSpec.SetReference_Setting();
    m_clGridBlemishSpec.EnableSelection(FALSE);
    m_clGridBlemishSpec.SetRowCount(SpecRow);
    m_clGridBlemishSpec.SetColumnCount(SpecCol);
    m_clGridBlemishSpec.SetFixedRowCount(1);
    m_clGridBlemishSpec.SetFixedColumnCount(1);

    for (i = 0; i < SpecRow; i++)
    {
        m_clGridBlemishSpec.SetRowHeight(i, gridHeight);
        m_clGridBlemishSpec.SetItemText(i, 0, pszRow[i]);

        for (j = 0; j < SpecCol; j++)
        {
            if (i == 0)
            {
                if (j == 0)
                {
                    m_clGridBlemishSpec.SetColumnWidth(j, gridWidth1);
                }
                else
                {
                    m_clGridBlemishSpec.SetColumnWidth(j, gridWidth2);
                }

                m_clGridBlemishSpec.SetItemText(i, j, pszCol[j]);
            }

            m_clGridBlemishSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }
}
void CCcdBlemishSpecDlg::InitLcbGridCtrl()
{
    TCHAR* pszRow[] = { _T("") ,
        _T("AdCenterThreshold"), _T("AdEdgeThreshold"), _T("AdCornerThreshold"),
         _T("AnMinDefectWidthHeigh"),
        _T("AbEnableCircle"), _T("AnPosOffsetX"), _T("AnPosOffsetY"),
        _T("AdRadiusRatioX"), _T("AdRadiusRatioY"), _T("AdThresholdRatio"),
        _T("AdROIRange"), _T("AnUsedFixedCircle"), _T("AbEnableMultiCircle"),
        _T("dZoneSizeRatio1") ,_T("dZoneSizeRatio2"), _T("dZoneSizeRatio3"),
        _T("dThreshold1") ,_T("dThreshold2"), _T("dThreshold3")
    };

    TCHAR* pszCol[] = { _T("Lcb"), _T("Spec") };

    CRect rect;
    int i, j;
    int DLG = IDC_STATIC_CCD_LCB_SPEC_GRID_SPEC;

    CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
    int SpecRow = LcbMaxCount;//�Ʒ�
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
    m_clGridLcbSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

    m_clGridLcbSpec.SetTextBkColor(RGB_COLOR_WHITE);
    m_clGridLcbSpec.SetFixedBkColor(GRID_COLOR_TITLE);
    m_clGridLcbSpec.SetFixedTextColor(RGB_COLOR_WHITE);
    m_clGridLcbSpec.SetReference_Setting();
    m_clGridLcbSpec.EnableSelection(FALSE);
    m_clGridLcbSpec.SetRowCount(SpecRow);
    m_clGridLcbSpec.SetColumnCount(SpecCol);
    m_clGridLcbSpec.SetFixedRowCount(1);
    m_clGridLcbSpec.SetFixedColumnCount(1);

    for (i = 0; i < SpecRow; i++)
    {
        m_clGridLcbSpec.SetRowHeight(i, gridHeight);
        m_clGridLcbSpec.SetItemText(i, 0, pszRow[i]);

        for (j = 0; j < SpecCol; j++)
        {
            if (i == 0)
            {
                if (j == 0)
                {
                    m_clGridLcbSpec.SetColumnWidth(j, gridWidth1);
                }
                else
                {
                    m_clGridLcbSpec.SetColumnWidth(j, gridWidth2);
                }

                m_clGridLcbSpec.SetItemText(i, j, pszCol[j]);
            }

            m_clGridLcbSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }
}
void CCcdBlemishSpecDlg::InitYmeanGridCtrl()
{
    TCHAR* pszRow[] = { _T("") ,
        _T("AnEdgeSize"), _T("AdCenterThreshold"), _T("AdEdgeThreshold"), _T("AdCornerThreshold"),
        _T("AnDefecBlockSize"), _T("AnLscBlockSize"), _T("AnCalcType"),
        _T("AnnMaxRecursiveCount"), _T("AdMaxDefectSize"), _T("AdPixelSize"),
        _T("AbEnableCircle"), _T("AnPosOffsetX"), _T("AnPosOffsetY"),
        _T("AdRadiusRatioX"), _T("AdRadiusRatioY"), _T("AdThresholdRatio"),
        _T("AdROIRange"), _T("AnUsedFixedCircle"), _T("AbEnableMultiCircle"),
        _T("dZoneSizeRatio1") ,_T("dZoneSizeRatio2"), _T("dZoneSizeRatio3"),
        _T("dThreshold1") ,_T("dThreshold2"), _T("dThreshold3")
    };

    TCHAR* pszCol[] = { _T("Ymean"), _T("Spec") };

    CRect rect;
    int i, j;
    int DLG = IDC_STATIC_CCD_YMEAN_SPEC_GRID_SPEC;

    CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
    int SpecRow = YmeanMaxCount;//�Ʒ�
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
    m_clGridYmenaSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

    m_clGridYmenaSpec.SetTextBkColor(RGB_COLOR_WHITE);
    m_clGridYmenaSpec.SetFixedBkColor(GRID_COLOR_TITLE);
    m_clGridYmenaSpec.SetFixedTextColor(RGB_COLOR_WHITE);
    m_clGridYmenaSpec.SetReference_Setting();
    m_clGridYmenaSpec.EnableSelection(FALSE);
    m_clGridYmenaSpec.SetRowCount(SpecRow);
    m_clGridYmenaSpec.SetColumnCount(SpecCol);
    m_clGridYmenaSpec.SetFixedRowCount(1);
    m_clGridYmenaSpec.SetFixedColumnCount(1);

    for (i = 0; i < SpecRow; i++)
    {
        m_clGridYmenaSpec.SetRowHeight(i, gridHeight);
        m_clGridYmenaSpec.SetItemText(i, 0, pszRow[i]);

        for (j = 0; j < SpecCol; j++)
        {
            if (i == 0)
            {
                if (j == 0)
                {
                    m_clGridYmenaSpec.SetColumnWidth(j, gridWidth1);
                }
                else
                {
                    m_clGridYmenaSpec.SetColumnWidth(j, gridWidth2);
                }

                m_clGridYmenaSpec.SetItemText(i, j, pszCol[j]);
            }

            m_clGridYmenaSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }
}
//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::SetUnit(int nUnit)
{
	m_nUnit = nUnit;
}

//-----------------------------------------------------------------------------
//
//	OnShowWindow
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow == TRUE)
	{
        this->ShowBlemishSpec();
        this->ShowLcbSpec();
        this->ShowYmeanSpec();
	}
}

//-----------------------------------------------------------------------------
//
//	SFR SPEC ������ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::ShowBlemishSpec()
{
	TCHAR szData[SIZE_OF_100BYTE];
	int i;

	
	/*_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), model.m_dLimitSFRCent);
	m_clColorStaticCenterSfrLimitVal.SetWindowText(szData);

	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), model.m_dLimitSFRSide);
	m_clColorStaticCornerSfrLimitVal.SetWindowText(szData);

	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), g_clSysData.m_dLimitSFRBalance[m_nUnit]);
	m_clColorStaticCornerSfrDeviationLimitVal.SetWindowText(szData);

	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), g_clSysData.m_clOCSpec[m_nUnit].x);
	m_clColorStaticOCXYVal.SetWindowText(szData);

	_stprintf_s(szData, SIZE_OF_100BYTE, _T("%.03lf"), g_clSysData.m_dOCSpecTh[m_nUnit]);
	m_clColorStaticOCThVal.SetWindowText(szData);*/

	for (i = 0; i < BlemishMaxCount-1; i++)
	{
		m_clGridBlemishSpec.SetItemText(i + 1, 1, model.m_BlemishSpec[i],3);
		//m_clGridBlemishSpec.SetItemText(i + 1, 2, model.m_LcbSpec[i],3);
		//m_clGridBlemishSpec.SetItemText(i + 1, 3, model.m_YmeanSpec[i],3);
	}

	m_clGridBlemishSpec.Invalidate();
}
void CCcdBlemishSpecDlg::ShowLcbSpec()
{
    TCHAR szData[SIZE_OF_100BYTE];
    int i;

    for (i = 0; i < LcbMaxCount-1; i++)
    {
        m_clGridLcbSpec.SetItemText(i + 1, 1, model.m_LcbSpec[i], 3);
    }

    m_clGridLcbSpec.Invalidate();
}
void CCcdBlemishSpecDlg::ShowYmeanSpec()
{
    TCHAR szData[SIZE_OF_100BYTE];
    int i;

    for (i = 0; i < YmeanMaxCount-1; i++)
    {
        m_clGridYmenaSpec.SetItemText(i + 1, 1, model.m_YmeanSpec[i], 3);
    }

    m_clGridYmenaSpec.Invalidate();
}


//-----------------------------------------------------------------------------
//
//	SFR ������ -> �޸𸮿� ����
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::GetBlemishSpec()
{
    CString sData = _T("");
    int i;

    for (i = 0; i < BlemishMaxCount - 1; i++)
    {
        sData = m_clGridBlemishSpec.GetItemText(i + 1, 1);
        model.m_BlemishSpec[i] = _ttof((TCHAR*)(LPCTSTR)sData);

    }
}
void CCcdBlemishSpecDlg::GetLcbSpec()
{
    CString sData = _T("");
    int i;

    for (i = 0; i < LcbMaxCount-1; i++)
    {
        sData = m_clGridLcbSpec.GetItemText(i + 1, 1);
        model.m_LcbSpec[i] = _ttof((TCHAR*)(LPCTSTR)sData);
    }
    //g_clSysData.Save();
}
void CCcdBlemishSpecDlg::GetYmeanSpec()
{
    CString sData = _T("");
    int i;

    for (i = 0; i < YmeanMaxCount - 1; i++)
    {
        sData = m_clGridYmenaSpec.GetItemText(i + 1, 1);
        model.m_YmeanSpec[i] = _ttof((TCHAR*)(LPCTSTR)sData);
    }
    //g_clSysData.Save();
}
//---------------------------------------------------------------------------
//
//	CENTER SFR LIMIT
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal()
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
void CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal()
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
void CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal()
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
void CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal()
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
void CCcdBlemishSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal()
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
void CCcdBlemishSpecDlg::OnBnClickedButtonCcdSfrSpecSave()
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
	sTemp.Format("BLEMISH SPEC �����͸� �����Ͻðڽ��ϱ�?");
	if (!askMsg(sTemp))return;
	//if (g_ShowMsgModal(_T("Ȯ��"), _T("DEFECT/UNIFORM SPEC �����͸� �����Ͻðڽ��ϱ�?"), RGB_COLOR_BLUE) == false)
		//return;

    this->GetBlemishSpec();
    this->GetLcbSpec();
    this->GetYmeanSpec();

	//g_clSysData.Save();
	//g_clMandoSfrSpec[m_nUnit].Save();
	model.AcmisDataSave();
	theApp.MainDlg->putListLog(_T("[INFO] BLEMISH SPEC ����"));// , 0, m_nUnit);
}

//-----------------------------------------------------------------------------
//
//	��ư : CLOSE
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::OnBnClickedButtonCcdSfrSpecClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::OnNMDblClickedSfrBlockspot(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
    CString sData = _T("");
    int nRow, nCol;

    nRow = pNMGridView->iRow;
    nCol = pNMGridView->iColumn;

    if (nRow >= 1 && nCol >= 1)
    {
        sData = m_clGridBlemishSpec.GetItemText(nRow, nCol);
		/*CKeyPadDlg keyDlg;
		if (keyDlg.DoModal() == IDOK)
		{
			sData.Format(_T("%s"), keyDlg.GetstrKeypad());*/
		CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)sData);
		if (dlg.DoModal() == IDOK)
		{
			float f_num = (float)atof(dlg.GetstrNum());
			sData.Format(_T("%.3f"), f_num);
            m_clGridBlemishSpec.SetItemText(nRow, nCol, sData);

            m_clGridBlemishSpec.Invalidate();
        }


    }
}

void CCcdBlemishSpecDlg::OnNMDblClickedSfrLcb(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
    CString sData = _T("");
    int nRow, nCol;

    nRow = pNMGridView->iRow;
    nCol = pNMGridView->iColumn;

    if (nRow >= 1 && nCol >= 1)
    {
        sData = m_clGridLcbSpec.GetItemText(nRow, nCol);
		/*CKeyPadDlg keyDlg;
		if (keyDlg.DoModal() == IDOK)
		{
			sData.Format(_T("%s"), keyDlg.GetstrKeypad());*/
		CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)sData);
		if (dlg.DoModal() == IDOK)
		{
			float f_num = (float)atof(dlg.GetstrNum());
			sData.Format(_T("%.3f"), f_num);
            m_clGridLcbSpec.SetItemText(nRow, nCol, sData);

            m_clGridLcbSpec.Invalidate();
        }

     
    }
}

void CCcdBlemishSpecDlg::OnNMDblClickedSfrYmean(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
    CString sData = _T("");
    int nRow, nCol;

    nRow = pNMGridView->iRow;
    nCol = pNMGridView->iColumn;

    if (nRow >= 1 && nCol >= 1)
    {
        sData = m_clGridYmenaSpec.GetItemText(nRow, nCol);
		/*CKeyPadDlg keyDlg;
		if (keyDlg.DoModal() == IDOK)
		{
			sData.Format(_T("%s"), keyDlg.GetstrKeypad());*/
		CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)sData);
		if (dlg.DoModal() == IDOK)
		{
			float f_num = (float)atof(dlg.GetstrNum());
			sData.Format(_T("%.3f"), f_num);
            m_clGridYmenaSpec.SetItemText(nRow, nCol, sData);

            m_clGridYmenaSpec.Invalidate();
        }

    }
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//
//	OnLButtonDown
//
//-----------------------------------------------------------------------------
void CCcdBlemishSpecDlg::OnLButtonDown(UINT nFlags, CPoint point)
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
BOOL CCcdBlemishSpecDlg::PreTranslateMessage(MSG* pMsg)
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