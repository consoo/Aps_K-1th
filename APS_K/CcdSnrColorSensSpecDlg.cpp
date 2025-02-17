// CcdSfrSpecDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CcdSnrColorSensSpecDlg.h"
// CCcdSnrColorSensSpecDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCcdSnrColorSensSpecDlg, CDialogEx)

CCcdSnrColorSensSpecDlg::CCcdSnrColorSensSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCcdSnrColorSensSpecDlg::IDD, pParent)
{
}

CCcdSnrColorSensSpecDlg::~CCcdSnrColorSensSpecDlg()
{
}

void CCcdSnrColorSensSpecDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CCcdSnrColorSensSpecDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_SNR_SPEC_GRID_SPEC, &CCcdSnrColorSensSpecDlg::OnNMDblClickedSnrSpec)
	ON_NOTIFY(NM_DBLCLK, IDC_STATIC_CCD_COLORSENS_SPEC_GRID_SPEC, &CCcdSnrColorSensSpecDlg::OnNMDblClickedColorSensitivitySpec)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_SAVE, &CCcdSnrColorSensSpecDlg::OnBnClickedButtonCcdSfrSpecSave)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR_SPEC_CLOSE, &CCcdSnrColorSensSpecDlg::OnBnClickedButtonCcdSfrSpecClose)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CENTER_SFR_LIMIT_VAL, &CCcdSnrColorSensSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_LIMIT_VAL, &CCcdSnrColorSensSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_CORNER_SFR_DEVIATION_LIMIT_VAL, &CCcdSnrColorSensSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_XY_VAL, &CCcdSnrColorSensSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal)
	ON_STN_CLICKED(IDC_STATIC_CCD_SFR_SPEC_OC_TH_VAL, &CCcdSnrColorSensSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal)
END_MESSAGE_MAP()


// CCcdSnrColorSensSpecDlg �޽��� ó�����Դϴ�.

//-----------------------------------------------------------------------------
//
//	OnPaint
//
//-----------------------------------------------------------------------------
void CCcdSnrColorSensSpecDlg::OnPaint()
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
BOOL CCcdSnrColorSensSpecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��Ʈ�� �ʱ�ȭ
	this->InitCtrl();

	// �׸��� �ʱ�ȭ
	this->InitSnrGridCtrl();
	//this->InitColorSensitivityGridCtrl();
	this->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��Ʈ�� �ʱ�ȭ
//
//-----------------------------------------------------------------------------
void CCcdSnrColorSensSpecDlg::InitCtrl()
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
void CCcdSnrColorSensSpecDlg::InitSnrGridCtrl()
{
	TCHAR* pszRow[] = { _T("") ,
		_T("dSNRThreshold") , _T("MIN SPEC") , _T("MAX SPEC")// _T("dSNRThreshold"), _T("dDRThreshold")
	};

	TCHAR* pszCol[] = { _T("SNR"), _T("Spec") };

	CRect rect;
	int i, j;
	int DLG = IDC_STATIC_CCD_SNR_SPEC_GRID_SPEC;

	CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
	int SpecRow = SnrMaxCount;//�Ʒ�
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
    m_clGridSnrSpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

	m_clGridSnrSpec.SetTextBkColor(RGB_COLOR_WHITE);
	m_clGridSnrSpec.SetFixedBkColor(GRID_COLOR_TITLE);
	m_clGridSnrSpec.SetFixedTextColor(RGB_COLOR_WHITE);
	m_clGridSnrSpec.SetReference_Setting();
	m_clGridSnrSpec.EnableSelection(FALSE);
	m_clGridSnrSpec.SetRowCount(SpecRow);
	m_clGridSnrSpec.SetColumnCount(SpecCol);
	m_clGridSnrSpec.SetFixedRowCount(1);
	m_clGridSnrSpec.SetFixedColumnCount(1);

	for (i = 0; i < SpecRow; i++)
	{
        m_clGridSnrSpec.SetRowHeight(i, gridHeight);
        m_clGridSnrSpec.SetItemText(i, 0, pszRow[i]);

		for (j = 0; j < SpecCol; j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
                    m_clGridSnrSpec.SetColumnWidth(j, gridWidth1);
				}
				else
				{
                    m_clGridSnrSpec.SetColumnWidth(j, gridWidth2);
				}

                m_clGridSnrSpec.SetItemText(i, j, pszCol[j]);
			}

            m_clGridSnrSpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}

void CCcdSnrColorSensSpecDlg::InitColorSensitivityGridCtrl()
{
	TCHAR* pszRow[] = { _T("") 
        ,_T("dSpecDevMaxRG"), _T("dSpecDevMinRG"),_T("dSpecDevMaxBG"), 
        _T("dSpecDevMinBG"), _T("dSpecDevMaxGrGb"),_T("dSpecDevMinGrGb")
        , _T("nAdjustType"),_T("nTypicalValueType")
	};

	TCHAR* pszCol[] = { _T("Color Sensitivity"), _T("Spec") };

	CRect rect;
	int i, j;
	int DLG = IDC_STATIC_CCD_COLORSENS_SPEC_GRID_SPEC;

	CWnd *pWnd = (CWnd*)GetDlgItem(DLG);
	int SpecRow = ColorSensitivityMaxCount;//�Ʒ�
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
    m_clGridColorSensitivitySpec.Create(rect, this, DLG, WS_TABSTOP | WS_VISIBLE);

	m_clGridColorSensitivitySpec.SetTextBkColor(RGB_COLOR_WHITE);
	m_clGridColorSensitivitySpec.SetFixedBkColor(GRID_COLOR_TITLE);
	m_clGridColorSensitivitySpec.SetFixedTextColor(RGB_COLOR_WHITE);
	m_clGridColorSensitivitySpec.SetReference_Setting();
	m_clGridColorSensitivitySpec.EnableSelection(FALSE);
	m_clGridColorSensitivitySpec.SetRowCount(SpecRow);
	m_clGridColorSensitivitySpec.SetColumnCount(SpecCol);
	m_clGridColorSensitivitySpec.SetFixedRowCount(1);
	m_clGridColorSensitivitySpec.SetFixedColumnCount(1);

	for (i = 0; i < SpecRow; i++)
	{
        m_clGridColorSensitivitySpec.SetRowHeight(i, gridHeight);
        m_clGridColorSensitivitySpec.SetItemText(i, 0, pszRow[i]);

		for (j = 0; j < SpecCol; j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
                    m_clGridColorSensitivitySpec.SetColumnWidth(j, gridWidth1);
				}
				else
				{
                    m_clGridColorSensitivitySpec.SetColumnWidth(j, gridWidth2);
				}

                m_clGridColorSensitivitySpec.SetItemText(i, j, pszCol[j]);
			}

            m_clGridColorSensitivitySpec.SetItemFormat(i, j, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}
//-----------------------------------------------------------------------------
//
//	���� ����
//
//-----------------------------------------------------------------------------
void CCcdSnrColorSensSpecDlg::SetUnit(int nUnit)
{
	m_nUnit = nUnit;
}

//-----------------------------------------------------------------------------
//
//	OnShowWindow
//
//-----------------------------------------------------------------------------
void CCcdSnrColorSensSpecDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow == TRUE)
	{
		this->ShowSnrSpec();
		//this->ShowColorSensitivitySpec();
	}
}

//-----------------------------------------------------------------------------
//
//	SFR SPEC ������ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdSnrColorSensSpecDlg::ShowSnrSpec()
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

	for (i = 0; i < SnrMaxCount-1; i++)
	{
        m_clGridSnrSpec.SetItemText(i + 1, 1, (int)model.m_SnrSpec[i], 3);
	}
    m_clGridSnrSpec.Invalidate();
}
//-----------------------------------------------------------------------------
//
//	UNIFORMITY SPEC ������ ǥ��
//
//-----------------------------------------------------------------------------
void CCcdSnrColorSensSpecDlg::ShowColorSensitivitySpec()
{
	TCHAR szData[SIZE_OF_100BYTE];
	int i;

	for (i = 0; i < ColorSensitivityMaxCount-1; i++)
	{
        m_clGridColorSensitivitySpec.SetItemText(i + 1, 1, (int)model.m_ColorSensitivitySpec[i], 3);
	}
    m_clGridColorSensitivitySpec.Invalidate();
}

//-----------------------------------------------------------------------------
//
//	CENTER SFR LIMIT
//
//-----------------------------------------------------------------------------
void CCcdSnrColorSensSpecDlg::OnStnClickedStaticCcdSfrSpecCenterSfrLimitVal()
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
void CCcdSnrColorSensSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrLimitVal()
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
void CCcdSnrColorSensSpecDlg::OnStnClickedStaticCcdSfrSpecCornerSfrDeviationLimitVal()
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
void CCcdSnrColorSensSpecDlg::OnStnClickedStaticCcdSfrSpecOcXyVal()
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
void CCcdSnrColorSensSpecDlg::OnStnClickedStaticCcdSfrSpecOcThVal()
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
void CCcdSnrColorSensSpecDlg::OnBnClickedButtonCcdSfrSpecSave()
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
	sTemp.Format("SNR SPEC �����͸� �����Ͻðڽ��ϱ�?");
	if (!askMsg(sTemp))return;


	//if (g_ShowMsgModal(_T("Ȯ��"), _T("DEFECT/UNIFORM SPEC �����͸� �����Ͻðڽ��ϱ�?"), RGB_COLOR_BLUE) == false)
	//	return;

	this->GetSnrSpec();
    //.this->GetColorSensitivitySpec();
	//g_clSysData.Save();
	//g_clMandoSfrSpec[m_nUnit].Save();
	model.AcmisDataSave();
	theApp.MainDlg->putListLog(_T("[INFO] SNR/COLOR SENSITIVITY SPEC ����"));//, 0, m_nUnit);
}

//-----------------------------------------------------------------------------
//
//	SFR ������ -> �޸𸮿� ����
//
//-----------------------------------------------------------------------------
void CCcdSnrColorSensSpecDlg::GetSnrSpec()
{
	CString sData = _T("");
	int i;

	for (i = 0; i < SnrMaxCount-1; i++)
	{
		sData = m_clGridSnrSpec.GetItemText(i + 1, 1);
		model.m_SnrSpec[i] = _ttof((TCHAR*)(LPCTSTR)sData);
	}

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
//	UNIFORMITY ������ -> �޸𸮿� ����
//
//-----------------------------------------------------------------------------
void CCcdSnrColorSensSpecDlg::GetColorSensitivitySpec()
{
	CString sData = _T("");
	int i;

	for (i = 0; i < ColorSensitivityMaxCount-1; i++)
	{
		sData = m_clGridColorSensitivitySpec.GetItemText(i + 1, 1);
		model.m_ColorSensitivitySpec[i] = _ttof((TCHAR*)(LPCTSTR)sData);
	}
	//g_clSysData.Save();
}

//-----------------------------------------------------------------------------
//
//	��ư : CLOSE
//
//-----------------------------------------------------------------------------
void CCcdSnrColorSensSpecDlg::OnBnClickedButtonCcdSfrSpecClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------
void CCcdSnrColorSensSpecDlg::OnNMDblClickedSnrSpec(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	CString sData = _T("");
	int nRow, nCol;

	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;

	if (nRow >= 1 && nCol >= 1)
	{
		sData = m_clGridSnrSpec.GetItemText(nRow, nCol);
		//CKeyPadDlg keyDlg;
		//if (keyDlg.DoModal() == IDOK)
		CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)sData);

		if (dlg.DoModal() == IDOK)
		{
			sData.Format(_T("%.2lf"), atof(dlg.GetstrNum())); //keyDlg.GetstrKeypad());
            m_clGridSnrSpec.SetItemText(nRow, nCol, sData);

            m_clGridSnrSpec.Invalidate();
		}

	}
}

//-----------------------------------------------------------------------------
//
//	�׸��� ��Ʈ�� ���� Ŭ��
//
//-----------------------------------------------------------------------------
void CCcdSnrColorSensSpecDlg::OnNMDblClickedColorSensitivitySpec(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGridView = (NM_GRIDVIEW*)pNMHDR;
	CString sData = _T("");
	int nRow, nCol;

	nRow = pNMGridView->iRow;
	nCol = pNMGridView->iColumn;

	if (nRow >= 1 && nCol >= 1)
	{
		sData = m_clGridColorSensitivitySpec.GetItemText(nRow, nCol);
		CKeyPadDlg keyDlg;
		if (keyDlg.DoModal() == IDOK)
		{
			sData.Format(_T("%s"), keyDlg.GetstrKeypad());
            m_clGridColorSensitivitySpec.SetItemText(nRow, nCol, sData);

            m_clGridColorSensitivitySpec.Invalidate();
			}

	}
}

//-----------------------------------------------------------------------------
//
//	OnLButtonDown
//
//-----------------------------------------------------------------------------
void CCcdSnrColorSensSpecDlg::OnLButtonDown(UINT nFlags, CPoint point)
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
BOOL CCcdSnrColorSensSpecDlg::PreTranslateMessage(MSG* pMsg)
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