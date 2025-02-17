// AutorunDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AutoDispDlg.h"

//! ====================================================================================
//! �׷��� ��Ʈ�� Ŭ������ ���Խ�Ų��. 
#include "Graph2D/Graph2DWnd/Graph2DWnd.h"
//! ====================================================================================
// CAutoDispDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAutoDispDlg, CDialogEx)

	CAutoDispDlg::CAutoDispDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoDispDlg::IDD, pParent)
{
	int i = 0, j = 0;
	for ( i = 0; i < BUFFER_SIZE_GRAPH_2D; i++ )
	{
		m_apGraphWnd[i] = NULL;	

		m_aiIndex_Overlap_Cursor[i] = 0;

		m_aiCount_Visible[i] = BUFFER_SIZE_GRAPH_OVERLAP;

		for ( j = 0; j < BUFFER_SIZE_GRAPH_OVERLAP; j++ )
		{
			Init_Buffser(i, j);

			m_abFlag_Visible[i][j] = true;					
		}		
	}
}

CAutoDispDlg::~CAutoDispDlg()
{
}

void CAutoDispDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_CUR_TASK, m_label_Cur_Task);
	DDX_Control(pDX, IDC_LABEL_GRAPH1, m_label_Graph1);
	DDX_Control(pDX, IDC_LABEL_GRAPH2, m_label_Graph2);
	DDX_Control(pDX, IDC_LABEL_GRAPH3, m_label_Graph3);
	DDX_Control(pDX, IDC_LABEL_GRAPH5, m_label_Graph5);
	DDX_Control(pDX, IDC_LABEL_DISP3, m_label_Disp_Opt);
	DDX_Control(pDX, IDC_LABEL_DISP4, m_label_Disp_Defect);
}


BEGIN_MESSAGE_MAP(CAutoDispDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAutoDispDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAutoDispDlg::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()

	ON_MESSAGE(WM_GRAPH2D_CURSOR_EVENT, OnCursorEventMsgInPlot)	//! Added by LHW (2013/2/22)

	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CAutoDispDlg �޽��� ó�����Դϴ�.


void CAutoDispDlg::OnBnClickedOk(){}
void CAutoDispDlg::OnBnClickedCancel(){}

BOOL CAutoDispDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	setInterface();

	bool bRet = MakeGraphWnd();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CAutoDispDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
}


void CAutoDispDlg::setInterface()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	WINDOWPLACEMENT wndpl;

	/*wndpl.rcNormalPosition = pFrame->m_rectCamDispPos2;
	wndpl.rcNormalPosition.left = pFrame->m_rectCamDispPos2.right+1;
	wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left +SMALL_CAM_SIZE_X +2;
	wndpl.rcNormalPosition.top -= SUB_DLG_OFFSET_Y - 10;
	wndpl.rcNormalPosition.bottom = MAIN_DLG_SIZE_Y-85;*/


	wndpl.rcNormalPosition = pFrame->m_rectCamDispPos2;
	wndpl.rcNormalPosition.left = pFrame->m_rectCamDispPos2.left + 100;
	wndpl.rcNormalPosition.top -= SUB_DLG_OFFSET_Y - 10;
	wndpl.rcNormalPosition.bottom = MAIN_DLG_SIZE_Y - 5 - 95;
	wndpl.rcNormalPosition.right = MAIN_DLG_SIZE_X;
	this->MoveWindow(&wndpl.rcNormalPosition);

	UpdateData(false);

	m_label_Cur_Task
		.SetBkColor(GRID_COLOR_GREEN)
		.SetFontBold(TRUE)
		.SetFontSize(24)
		.SetText("");

	sLangChange.LoadStringA(IDS_STRING1094);
	m_label_Graph1
		.SetBkColor(M_COLOR_WHITE)
		.SetFontBold(TRUE)
		.SetFontSize(16)
		.SetText(sLangChange);

	sLangChange.LoadStringA(IDS_STRING1067);
	m_label_Graph2
		.SetBkColor(M_COLOR_WHITE)
		.SetFontBold(TRUE)
		.SetFontSize(16)
		.SetText(sLangChange);

	sLangChange.LoadStringA(IDS_STRING570);
	m_label_Graph3
		.SetBkColor(M_COLOR_WHITE)
		.SetFontBold(TRUE)
		.SetFontSize(16)
		.SetText(sLangChange);

	sLangChange.LoadStringA(IDS_STRING1415);
	m_label_Graph5
		.SetBkColor(M_COLOR_WHITE)
		.SetFontBold(TRUE)
		.SetFontSize(16)
		.SetText(sLangChange);

	sLangChange.LoadStringA(IDS_STRING885);
	m_label_Disp_Opt
		.SetBkColor(M_COLOR_WHITE)
		.SetFontBold(TRUE)
		.SetFontSize(16)
		.SetText(sLangChange);

	sLangChange.LoadStringA(IDS_STRING1343);
	m_label_Disp_Defect
		.SetBkColor(M_COLOR_WHITE)
		.SetFontBold(TRUE)
		.SetFontSize(16)
		.SetText(sLangChange);

	CRect rcClient;

	GetDlgItem(IDC_DISP_OPT)->GetClientRect(rcClient);
	m_iSizeX_Client	= rcClient.right - rcClient.left;
	m_iSizeY_Client	= rcClient.bottom - rcClient.top;
#ifdef ON_LINE_VISION
	vision.InitBuffer_AutoDisplay(m_iSizeX_Client, m_iSizeY_Client);

	CWnd* pWnd;
	
	pWnd = GetDlgItem(IDC_DISP_OPT);
	MdispSelectWindow(vision.MilOptDisplayImage, vision.MilOptImage, pWnd->m_hWnd);
	pWnd = GetDlgItem(IDC_DISP_DEFECT);
	MdispSelectWindow(vision.MilDefectDisplayImage, vision.MilDefectImage, pWnd->m_hWnd);

	vision.enableOverlay_AutoDisp();
#endif
}


//! Ŀ���� �������� �����̰� �Ѵ�.
void CAutoDispDlg::MoveCursor_Left_Graph(int iIndex_Plot)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= BUFFER_SIZE_GRAPH_2D )
	{
		return;
	}

	if ( m_apGraphWnd[iIndex_Plot] != NULL )
	{
		if ( m_apGraphWnd[iIndex_Plot]->GetCurrentCursorSize() > 0 )
		{			
			m_apGraphWnd[iIndex_Plot]->MoveCursor(0, false, false, m_aiIndex_Overlap_Cursor[iIndex_Plot]);
			m_apGraphWnd[iIndex_Plot]->Invalidate();
		}
	}
}

//! Ŀ���� ���������� �����̰� �Ѵ�.
void CAutoDispDlg::MoveCursor_Right_Graph(int iIndex_Plot)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= BUFFER_SIZE_GRAPH_2D )
	{
		return;
	}

	if ( m_apGraphWnd[iIndex_Plot] != NULL )
	{
		if ( m_apGraphWnd[iIndex_Plot]->GetCurrentCursorSize() > 0 )
		{			
			m_apGraphWnd[iIndex_Plot]->MoveCursor(0, true, false, m_aiIndex_Overlap_Cursor[iIndex_Plot]);
			m_apGraphWnd[iIndex_Plot]->Invalidate();
		}
	}
}

//! �׷������� Ŀ�� �̺�Ʈ�� �߻��ϸ� �޽����� �´�.
LRESULT CAutoDispDlg::OnCursorEventMsgInPlot(WPARAM wParam, LPARAM lParam)
{
	int iTemp = (int)wParam;
	strGraph2D_Cursor_Event* pCursorInfo = (strGraph2D_Cursor_Event*)lParam;

	if ( pCursorInfo == NULL || m_apGraphWnd == NULL )
	{
		return 0;
	}

	int iPlotIndex    = pCursorInfo->iPlotIndex;	
	int iOverlapIndex = pCursorInfo->iOverlapIndex;
	int iDataIndex    = pCursorInfo->iDataIndex;
	int iCursorIndex  = pCursorInfo->iCursorIndex;

	int iRet = SetCursorValue(iPlotIndex, iOverlapIndex, iDataIndex, iCursorIndex);
	if ( iRet < 0 )
	{
		return 0;
	}

	return 1;
}

//! ���� �׷����� Ŀ������ Edit ��Ʈ�ѿ� ǥ��
//! �׷��� Ŀ���� �ش� �׷����� ���ӵ�����, ����� �׷��� �������� X�� ������ �����ϴ�.
//! ��, �ش� �׷����� Ŀ�� �������� index�� �ش��ϴ� �ٸ� �׷����� �����͸� ǥ���ϸ� �ȴ�. 
int CAutoDispDlg::SetCursorValue(int iIndex_Plot, int iIndex_Overlap, int iDataIndex, int iCursorIndex)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= BUFFER_SIZE_GRAPH_2D )
	{
		return -1;
	}
	if ( iIndex_Overlap < 0 || iIndex_Overlap >= BUFFER_SIZE_GRAPH_OVERLAP )
	{
		return -2;
	}
	if ( iDataIndex < 0 || iDataIndex >= BUFFER_SIZE_PIXEL_GRAPH )
	{
		return -3;
	}
	if ( m_apGraphWnd == NULL )
	{
		return -4;
	}

	bool abRet[2];
	int i = 0;
	double  adDataX[BUFFER_SIZE_GRAPH_OVERLAP], adDataY[BUFFER_SIZE_GRAPH_OVERLAP];
	CString asDataX[BUFFER_SIZE_GRAPH_OVERLAP], asDataY[BUFFER_SIZE_GRAPH_OVERLAP], asData[BUFFER_SIZE_GRAPH_OVERLAP];
	strGraph2D_Y_Axis_Info strAxisYInfo;	//! Y��

	for (i = 0; i < BUFFER_SIZE_GRAPH_OVERLAP; i++ )
	{		
		m_apGraphWnd[iIndex_Plot]->GetResourceYInfo(strAxisYInfo, i);		

		if ( strAxisYInfo.bShowGraph == false )
		{
			abRet[0] = false;
			abRet[1] = false;
		}
		else
		{			
			abRet[0] = m_apGraphWnd[iIndex_Plot]->GetPlotDataX(adDataX[i], iDataIndex, i);
			if ( abRet[0] == true )
			{
				asDataX[i].Format(_T("%.3lf"), adDataX[i]);
			}
			else
			{
				asDataX[i] = _T("-");
			}

			abRet[1] = m_apGraphWnd[iIndex_Plot]->GetPlotDataY(adDataY[i], iDataIndex, i);
			if ( abRet[1] == true )
			{
				asDataY[i].Format(_T("%.3lf"), adDataY[i]);
			}
			else
			{
				asDataY[i] = _T("-");
			}
		}		

		if ( (abRet[0] == true) && (abRet[1] == true) )
		{
			asData[i].Format(_T("%s,%s"), asDataX[i], asDataY[i]);
		}
		else
		{
			asData[i] = _T("-");
		}
	}

	return 1;
}

//! üũ �ڽ��� �̿��Ͽ� �׷����� ���� ���̰ų� �����.
//! �� ��, �׷��� Ŀ�� ���� �۾��� ���� �����Ѵ�. 
//!		1) �׷��� Ŀ���� 5���� �׷��� �� 1���� �׷����� ���ӵȴ�.
//!		2) �׷����� ���� ������ �ʰ� �ϸ�, �ش� �׷����� Ŀ���� ���� ���� ������ �ʰ� �ȴ�.
//!		3) ���� ������ �ʰ� �Ǵ� Ŀ���� �����ϰ�, ���� ���̴� �׷����� ���� ��ġ�� �׷��� Ŀ���� �߰��Ѵ�. 
void CAutoDispDlg::Proc_GraphVisible(int iIndex_Plot, int iIndex_Overlap)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= BUFFER_SIZE_GRAPH_2D )
	{
		return;
	}
	if ( iIndex_Overlap < 0 || iIndex_Overlap >= BUFFER_SIZE_GRAPH_OVERLAP )
	{
		return;
	}

	bool bFlag_Visible_Old = m_abFlag_Visible[iIndex_Plot][iIndex_Overlap];	
	bool bFlag_Visible = false;

	int iCount_Visible = m_aiCount_Visible[iIndex_Plot];

	bFlag_Visible = !bFlag_Visible_Old;

	if ( bFlag_Visible == true )
	{
		iCount_Visible++;
	}
	else
	{
		iCount_Visible--;
	}

	bool bRet = false;

	if ( iCount_Visible <= 0 )
	{
		bRet = false;
	}
	else
	{
		bRet = SetGraphVisible(iIndex_Plot, iIndex_Overlap, bFlag_Visible);		
	}

	if ( bRet == true )
	{
		m_abFlag_Visible[iIndex_Plot][iIndex_Overlap] = bFlag_Visible;
		m_aiCount_Visible[iIndex_Plot] = iCount_Visible;

		ExChangeCursor(iIndex_Plot);
		UpdateCursorValue(iIndex_Plot);
	}
	else
	{
		//! ���� ���� ��Ų��.
		m_abFlag_Visible[iIndex_Plot][iIndex_Overlap] = bFlag_Visible_Old;

		SetGUI_Graph_Visible_Chk();
	}

	if ( iCount_Visible <= 0 )
	{
		sLangChange.LoadStringA(IDS_STRING1414);	//�ּ��� 1���� �׷����� ���� ���̵��� �����ؾ� �մϴ�.
		errMsg2(Task.AutoFlag, sLangChange);
	}
}

//! �ش� �׷����� Ŀ�� ǥ�� ����
int CAutoDispDlg::UpdateCursorValue(int iIndex_Plot)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= BUFFER_SIZE_GRAPH_2D )
	{
		return -1;
	}
	if ( m_apGraphWnd == NULL )
	{
		return -2;
	}

	int iOverlapIndex = 0;
	int iDataIndex = 0;
	m_apGraphWnd[iIndex_Plot]->GetCursorOverlapIndex(0, iOverlapIndex);
	m_apGraphWnd[iIndex_Plot]->GetCursorDataIndex(0, iDataIndex);

	int iRet = SetCursorValue(iIndex_Plot, iOverlapIndex, iDataIndex, 0);

	return 1;
}

//! ���� �Ⱥ��̴� �׷��� Ŀ�� �����ϰ�, ������ ��ġ�� ���� ���̴� �׷����� Ŀ�� �߰�
int CAutoDispDlg::ExChangeCursor(int iIndex_Plot)
{
	int iOverlapIndex = m_aiIndex_Overlap_Cursor[iIndex_Plot];
	if ( iOverlapIndex < 0 || iOverlapIndex >= BUFFER_SIZE_GRAPH_OVERLAP )
	{
		return -1;
	}

	if ( m_abFlag_Visible[iIndex_Plot][iOverlapIndex] == true )
	{
		//! �׷����� ���� ���̹Ƿ�, ������ ����.
		return 1;
	}

	//! ������ �׷��� Ŀ���� ���� ������ �˾Ƴ���.
	int iDataIndex = 0;
	m_apGraphWnd[iIndex_Plot]->GetCursorDataIndex(0, iDataIndex);

	m_apGraphWnd[iIndex_Plot]->DeleteAllCursor(false);

	//! ���� ���̴� �׷��� �߿��� 1���� ����.
	int i = 0;
	int iIndex_Overlap_Add = -1;
	for ( i = 0; i < BUFFER_SIZE_GRAPH_OVERLAP; i++ )
	{
		if ( m_abFlag_Visible[iIndex_Plot][i] == true )
		{
			iIndex_Overlap_Add = i;
			break;
		}
	}

	m_apGraphWnd[iIndex_Plot]->AddCursorInDataIndex(iDataIndex, true, iIndex_Overlap_Add);
	m_apGraphWnd[iIndex_Plot]->SetCursorValueVisible(0, false, false, false);
	m_aiIndex_Overlap_Cursor[iIndex_Plot] = iIndex_Overlap_Add;

	return 1;
}

//! GUI ó�� �Լ�
//! üũ �ڽ��� ���� ������ ǥ��
void CAutoDispDlg::SetGUI_Graph_Visible_Chk()
{
// 	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT1_ROI_1, this, m_abFlag_Visible[0][0]);
// 	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT1_ROI_2, this, m_abFlag_Visible[0][1]);
// 	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT1_ROI_3, this, m_abFlag_Visible[0][2]);
// 	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT1_ROI_4, this, m_abFlag_Visible[0][3]);
// 	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT1_ROI_5, this, m_abFlag_Visible[0][4]);
// 
// 	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT2_ROI_1, this, m_abFlag_Visible[1][0]);
// 	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT2_ROI_2, this, m_abFlag_Visible[1][1]);
// 	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT2_ROI_3, this, m_abFlag_Visible[1][2]);
// 	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT2_ROI_4, this, m_abFlag_Visible[1][3]);
// 	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT2_ROI_5, this, m_abFlag_Visible[1][4]);

	m_abFlag_Visible[0][0] = true;
	m_abFlag_Visible[0][1] = true;
	m_abFlag_Visible[0][2] = true;
	m_abFlag_Visible[0][3] = true;
	m_abFlag_Visible[0][4] = true;

	m_abFlag_Visible[1][0] = true;
	m_abFlag_Visible[1][1] = true;
	m_abFlag_Visible[1][2] = true;
	m_abFlag_Visible[1][3] = true;
	m_abFlag_Visible[1][4] = true;

	m_abFlag_Visible[2][0] = true;
	m_abFlag_Visible[2][1] = true;
	m_abFlag_Visible[2][2] = true;
	m_abFlag_Visible[2][3] = true;
	m_abFlag_Visible[2][4] = true;
}

//! �׷����� ���� ���̰ų� ���� ��
bool CAutoDispDlg::SetGraphVisible(int iIndex_Plot, int iIndex_Overlap, bool bFlag_Visible)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= BUFFER_SIZE_GRAPH_2D )
	{
		return false;
	}
	if ( iIndex_Overlap < 0 || iIndex_Overlap >= BUFFER_SIZE_GRAPH_OVERLAP )
	{
		return false;
	}
	if ( m_apGraphWnd[iIndex_Plot] == NULL )
	{
		return false;
	}

	strGraph2D_Y_Axis_Info strAxisYInfo;	//! Y��

	m_apGraphWnd[iIndex_Plot]->GetResourceYInfo(strAxisYInfo, iIndex_Overlap);

	if ( m_aiDataSize[iIndex_Plot][iIndex_Overlap] <= 0 )
	{
		strAxisYInfo.bShowGraph = false;
	}
	else
	{
		strAxisYInfo.bShowGraph = bFlag_Visible;
	}

	m_apGraphWnd[iIndex_Plot]->SetResourceYInfo(strAxisYInfo, iIndex_Overlap);

	m_apGraphWnd[iIndex_Plot]->m_bChanged = true;
	m_apGraphWnd[iIndex_Plot]->Invalidate();

	return true;
}


//! ���� �ʱ�ȭ
bool CAutoDispDlg::Init_Buffser(int iIndex_Plot, int iIndex_Overlap)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= BUFFER_SIZE_GRAPH_2D )
	{
		return false;
	}
	if ( iIndex_Overlap < 0 || iIndex_Overlap >= BUFFER_SIZE_GRAPH_OVERLAP )
	{
		return false;
	}

	int i = 0;
	for ( i = 0; i < BUFFER_SIZE_PIXEL_GRAPH; i++ )
	{
		m_adDataX[iIndex_Plot][iIndex_Overlap][i] = 0.;
		m_adDataY[iIndex_Plot][iIndex_Overlap][i] = 0.;
	}

	m_aiDataSize[iIndex_Plot][iIndex_Overlap] = 0;

	m_adData_MTF4[iIndex_Plot][iIndex_Overlap] = 0.;
	m_adData_MTF8[iIndex_Plot][iIndex_Overlap] = 0.;
	m_adData_MTF50[iIndex_Plot][iIndex_Overlap] = 0.;

	return true;
}

//! �׷��� ��Ʈ�� ����
bool CAutoDispDlg::MakeGraphWnd()
{
	ReleaseGraphWnd();

	int iIndex_Failed = -1;
	int iRet = 0;
	int i = 0, j = 0;

	for ( i = 0; i < BUFFER_SIZE_GRAPH_2D; i++ )
	{
		m_apGraphWnd[i] = new CGraph2DWnd;
		if ( m_apGraphWnd[i] == NULL )
		{
			iIndex_Failed = i;
			break;
		}
	}
	if ( iIndex_Failed >= 0 )
	{
		return false;
	}

	BOOL bCreateWnd = FALSE;

	for ( i = 0; i < BUFFER_SIZE_GRAPH_2D; i++ )
	{
		bCreateWnd = m_apGraphWnd[i]->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, CRect(0, 0, 0, 0), this, 0x0000, NULL);
		if ( bCreateWnd == FALSE )
		{
			iIndex_Failed = i;
			break;
		}
	}
	if ( iIndex_Failed >= 0 )
	{
		return false;
	}

	strGraph2D_Y_Axis_Info strAxisYInfo;	//! Y��
	strGraph2DResourceInfo strResourceInfo;	//! X��

	for ( i = 0; i < BUFFER_SIZE_GRAPH_2D; i++ )
	{
		//! ���� ���ҽ� ������ ������ ��
		m_apGraphWnd[i]->MakeResourcePtr();

		//! 5���� �׷����� �׸� �� �ְ� �Ѵ�.
		m_apGraphWnd[i]->MakePlotBuffer(BUFFER_SIZE_GRAPH_OVERLAP);

		//! �׷��� ��Ʈ���� ���� �ĺ� index�� ����
		m_apGraphWnd[i]->SetPlotIndex(i);

		//! ���� ���� �׷����� Y���� 1���̴�.
		m_apGraphWnd[i]->m_bMultiPlotOneAxis = true;

		m_apGraphWnd[i]->m_bCursorEventMsg            = true;
		m_apGraphWnd[i]->m_bRegionCursorEventMsg      = false;
		m_apGraphWnd[i]->m_bRegionCursorRMouseDownChk = false;
		m_apGraphWnd[i]->m_bXAutoRangeRegionCursor    = false;
		m_apGraphWnd[i]->m_bYAutoRangeRegionCursor    = false;

		//! �߰� �� Ŀ�� �� ǥ�� ��� ����
		m_apGraphWnd[i]->m_eCursorLineMode = E_GRAPH2D_CUROSR_LINE_X;

		m_apGraphWnd[i]->m_eCursorAllSearchMode      = E_GRAPH2D_CURSOR_ALL_SEARCH_NONE;
		m_apGraphWnd[i]->m_bXValueLineRMouseDownChk  = false;
		m_apGraphWnd[i]->m_bChildCursorRMouseDownChk = false;
		m_apGraphWnd[i]->m_bAutoDrawCompleteMsg      = false;
		m_apGraphWnd[i]->m_bAxisYEventMsg            = false;
		m_apGraphWnd[i]->m_bAuxCursorRMouseDownChk   = false;

		for ( j = 0; j < BUFFER_SIZE_GRAPH_OVERLAP; j++ )
		{
			m_apGraphWnd[i]->GetResourceYInfo(strAxisYInfo, j);
			strAxisYInfo.bShowGraph = false;

			//! �׷��� ����
			switch(j)
			{
			case 0:
				{
					strAxisYInfo.clrData = RGB(0, 0, 255);
				}
				break;
			case 1:
				{
					strAxisYInfo.clrData = RGB(255, 0, 0);
				}
				break;
			case 2:
				{
					strAxisYInfo.clrData = RGB(0, 255, 0);
				}
				break;
			case 3:
				{
					strAxisYInfo.clrData = RGB(63, 0, 153);
				}
				break;
			case 4:
				{
					strAxisYInfo.clrData = RGB(0, 216, 255);
				}
				break;
			}//! switch(j)

			//! �׷������� Y�� Range�� �׷��� �����Ͱ� Ƣ�� �ʴ� �ѿ��� '0 ~ 1'�� �����Ѵ�. 
			strAxisYInfo.strAxisInfo.eAutoRangeReservedMax = E_GRAPH2D_AUTO_RANGE_RESERVED_CONSIDER;
			strAxisYInfo.strAxisInfo.eAutoRangeReservedMin = E_GRAPH2D_AUTO_RANGE_RESERVED_CONSIDER;
			strAxisYInfo.strAxisInfo.eMarginType = E_GRAPH2D_RANGE_MARGIN_NONE;
			strAxisYInfo.strAxisInfo.dbAutoRangeReservedMax = 1.0;
			strAxisYInfo.strAxisInfo.dbAutoRangeReservedMin = 0.;
			strAxisYInfo.iLineWidth = 3;

			strAxisYInfo.strAxisInfo.iGridTextPrecision = 3;
			strAxisYInfo.strAxisInfo.iCursorTextPrecision = 3;
			m_apGraphWnd[i]->SetResourceYInfo(strAxisYInfo, j);

		}//! for ( j = 0; j < BUFFER_SIZE_GRAPH_OVERLAP; j++ )




		////////////////////// ����ȣ ����..
		m_apGraphWnd[i]->GetResourceInfo(strResourceInfo);
		strResourceInfo.iAxisThickness						= 1;
		strResourceInfo.clrEntireBackGround					= RGB(231, 231, 231);	//! �׷��� ���� ����
		strResourceInfo.strAxisInfoX.iCursorTextPrecision	= 0;

		/* LKS */
		//////////////////////////////////////////////////////////////////////////
		if ( i < 2 )
		{
			strResourceInfo.strAxisInfoX.eGridTextType			= E_GRAPH2D_GRID_DOUBLE;	//E_GRAPH2D_GRID_INTEGER;
			strResourceInfo.strAxisInfoX.bIncrementGrid         = false;	//! Modify by LHW (2013/3/26)
			strResourceInfo.strAxisInfoX.dbIncrementGrid        = -5.0;
			strResourceInfo.strAxisInfoX.iGridTextPrecision		= 3;//0;
			strResourceInfo.strAxisInfoX.bDirection_Plus        = false;	//! Added by LHW (2013/3/26)
		}
		else
		{
			strResourceInfo.strAxisInfoX.eGridTextType			= E_GRAPH2D_GRID_DOUBLE;//E_GRAPH2D_GRID_INTEGER;
			strResourceInfo.strAxisInfoX.bIncrementGrid         = false;
			strResourceInfo.strAxisInfoX.dbIncrementGrid        = 5.0;
			strResourceInfo.strAxisInfoX.iGridTextPrecision		= 3;//0;
			strResourceInfo.strAxisInfoX.bDirection_Plus        = true;		//! Added by LHW (2013/3/26)
		}		
		//////////////////////////////////////////////////////////////////////////

		strResourceInfo.strMainTitleInfo.bShow				= true;
		strResourceInfo.strMainTitleInfo.eAlign				= E_GRAPH2D_TITLE_ALIGN_CENTER;		
		m_apGraphWnd[i]->SetResourceInfo(strResourceInfo);

		//! <------------------------------------------------------------
		//! X�� Ư���� ǥ�ü� �߰�, X�࿡ ������
		//! �߰��Ǵ� ������ �����Ѵ�. 
		strValueLineXInfo strXLineInfo;

		strXLineInfo.InitInfo();
		strXLineInfo.bShow = false;
		strXLineInfo.dbValue = 0.;
		strXLineInfo.clrValue = RGB(255, 0, 0);
		strXLineInfo.iLineWidth = 1;
		strXLineInfo.szText.Format(_T("[%d] MTF(N/4)"), j + 1);
		m_apGraphWnd[i]->AddXValueLine(strXLineInfo);

		strXLineInfo.InitInfo();
		strXLineInfo.bShow = false;
		strXLineInfo.dbValue = 0.;
		strXLineInfo.clrValue = RGB(0, 0, 255);
		strXLineInfo.iLineWidth = 1;
		strXLineInfo.szText.Format(_T("[%d] MTF(N/8)"), j + 1);
		m_apGraphWnd[i]->AddXValueLine(strXLineInfo);

		strXLineInfo.InitInfo();
		strXLineInfo.bShow = false;
		strXLineInfo.dbValue = 0.;
		strXLineInfo.clrValue = RGB(255, 128, 0);
		strXLineInfo.iLineWidth = 1;
		strXLineInfo.szText.Format(_T("[%d] MTF50"), j + 1);
		m_apGraphWnd[i]->AddXValueLine(strXLineInfo);
		//! <------------------------------------------------------------

		//! �׷����� X, Y�� �̸��� ����
		m_apGraphWnd[i]->SetCaptionX(_T(""));
		m_apGraphWnd[i]->SetCaptionY(_T("MTF"), _T(""));

		m_apGraphWnd[i]->GetResourceInfo(strResourceInfo);
		strResourceInfo.strMainTitleInfo.szTitle = _T("SFR");
		m_apGraphWnd[i]->SetResourceInfo(strResourceInfo);

		//! �޽����� ���� �ڵ� ����
		m_apGraphWnd[i]->m_hMsgHandle = this->m_hWnd;		
	}

	CRect rcGraph;

	/* Through Graph */
	iRet = GetWindowRect_FromUI(IDC_STATIC_PLOT1, this, rcGraph);	
	ScreenToClient(rcGraph);
	m_apGraphWnd[0]->MoveWindow(rcGraph);
	/* Fine Graph */
	iRet = GetWindowRect_FromUI(IDC_STATIC_PLOT3, this, rcGraph);
	ScreenToClient(rcGraph);
	m_apGraphWnd[1]->MoveWindow(rcGraph);
	/* SFR Graph */
	iRet = GetWindowRect_FromUI(IDC_STATIC_PLOT2, this, rcGraph);
	ScreenToClient(rcGraph);
	m_apGraphWnd[2]->MoveWindow(rcGraph);

	for ( i = 0; i < BUFFER_SIZE_GRAPH_2D; i++ )
	{
		m_apGraphWnd[i]->m_bChanged = true;
	}

	return true;
}

//! �׷��� ��Ʈ�� ����
void CAutoDispDlg::ReleaseGraphWnd()
{
	int i = 0;
	for ( i = 0; i < BUFFER_SIZE_GRAPH_2D; i++ )
	{
		if ( m_apGraphWnd[i] != NULL )
		{
			m_apGraphWnd[i]->DestroyWindow();

			delete m_apGraphWnd[i];
			m_apGraphWnd[i] = NULL;
		}
	}
}

//! �׷����� �����͸� �Է��Ѵ�. 
int CAutoDispDlg::DrawGraph(int iIndex_Plot)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= BUFFER_SIZE_GRAPH_2D)
	{
		return -1;
	}

	int j = 0;
	//	bool bRet = false;
	int iIndex_Line_X_MTF4 = 0;
	int iIndex_Line_X_MTF8 = 0;
	int iIndex_Line_X_MTF50 = 0;

	strGraph2D_Y_Axis_Info strAxisYInfo;	//! Y��
	strValueLineXInfo      strXLineInfo;	//! X��

	//! 1) �׷����� �����͸� �Է��Ѵ�. 
	//! 2) ����, Ŀ�� (Mouse�� ���� ������ �׷��� ������ ǥ�� ���)�� ������ �߰��Ѵ�. 
	//! 3) �׷����� �ٽ� �׸���. 

	if ( m_apGraphWnd[iIndex_Plot] != NULL )
	{
		if ( iIndex_Plot == 0 )
		{
			for ( j = 0; j < BUFFER_SIZE_GRAPH_OVERLAP; j++ )
			{
				//! <-----------------------------------------------------------
				//! �׷����� ���� ���̴� ���θ� �����Ѵ�.
				m_apGraphWnd[iIndex_Plot]->GetResourceYInfo(strAxisYInfo, j);

				m_aiDataSize[iIndex_Plot][j] = Task.m_iCnt_Step_AA_Total;

				if ( m_aiDataSize[iIndex_Plot][j] <= 0 )
				{
					strAxisYInfo.bShowGraph = false;
				}
				else
				{
					strAxisYInfo.bShowGraph = m_abFlag_Visible[iIndex_Plot][j];
				}

				m_apGraphWnd[iIndex_Plot]->SetResourceYInfo(strAxisYInfo, j);

				int iIndex = 0;

				if ( Task.m_bFlag_Decrease_SFR == true )
				{
					iIndex = model.m_iCnt_Check_SFR;
				}

				for(int i_tmp=iIndex; i_tmp<m_aiDataSize[iIndex_Plot][j]; i_tmp++)
				{
#if (____AA_WAY == PCB_TILT_AA)
					m_adDataX[iIndex_Plot][j][i_tmp-iIndex] = Task.SFR.fMotorPos[i_tmp][theApp.MainDlg->TITLE_MOTOR_Z]*-1;
#elif (____AA_WAY == LENS_TILT_AA)
					m_adDataX[iIndex_Plot][j][i_tmp-iIndex] = Task.SFR.fMotorPos[i_tmp][theApp.MainDlg->TITLE_MOTOR_Z]; 
					//�����̴� Z�� �� ��ȣ�� ���� �ٸ��� 
#endif

					m_adDataY[iIndex_Plot][j][i_tmp-iIndex] = Task.SFR.fSfrN4[i_tmp][j];
				}

				//! �׷����� ������ �Է�
				//m_apGraphWnd[i]->SetPlotData(m_adDataX[i], m_adDataY[i], m_aiDataSize[i]);
				m_apGraphWnd[iIndex_Plot]->SetPlotData(m_adDataX[iIndex_Plot][j], m_adDataY[iIndex_Plot][j], m_aiDataSize[iIndex_Plot][j]-iIndex, j);

			}
			//! <-----------------------------------------------------------------------------------------------------------------------------------------
			

			
		}
		else if ( iIndex_Plot == 1)
		{
			for ( j = 0; j < BUFFER_SIZE_GRAPH_OVERLAP; j++ )
			{
				//! <-----------------------------------------------------------
				//! �׷����� ���� ���̴� ���θ� �����Ѵ�.
				m_apGraphWnd[iIndex_Plot]->GetResourceYInfo(strAxisYInfo, j);

				m_aiDataSize[iIndex_Plot][j] = Task.m_iCnt_Step_AA_Total - Task.m_iCnt_Second_AA_Start;

				if ( m_aiDataSize[iIndex_Plot][j] <= 0 )
				{
					strAxisYInfo.bShowGraph = false;
				}
				else
				{
					strAxisYInfo.bShowGraph = m_abFlag_Visible[iIndex_Plot][j];
				}

				m_apGraphWnd[iIndex_Plot]->SetResourceYInfo(strAxisYInfo,j);

				for(int i_tmp=0; i_tmp<m_aiDataSize[iIndex_Plot][j]; i_tmp++)
				{
#if (____AA_WAY == PCB_TILT_AA)
					m_adDataX[iIndex_Plot][j][i_tmp] = Task.SFR.fMotorPos[i_tmp+Task.m_iCnt_Second_AA_Start][theApp.MainDlg->TITLE_MOTOR_Z]*-1;
#elif (____AA_WAY == LENS_TILT_AA)	
					m_adDataX[iIndex_Plot][j][i_tmp] = Task.SFR.fMotorPos[i_tmp+Task.m_iCnt_Second_AA_Start][theApp.MainDlg->TITLE_MOTOR_Z];
#endif

					
					m_adDataY[iIndex_Plot][j][i_tmp] = Task.SFR.fSfrN4[i_tmp+Task.m_iCnt_Second_AA_Start][j];
//////////////////////////////////////////////////////////////////////////////////////////
				}

				m_apGraphWnd[iIndex_Plot]->SetPlotData(m_adDataX[iIndex_Plot][j], m_adDataY[iIndex_Plot][j], m_aiDataSize[iIndex_Plot][j], j);
			}
		}


		m_apGraphWnd[iIndex_Plot]->m_bChanged = true;
		m_apGraphWnd[iIndex_Plot]->Invalidate();
	}

	return 1;
}

void CAutoDispDlg::AddCursor_In_Graph()
{
	int i = 0;
	bool bRet = false;
	for ( i = 0; i < BUFFER_SIZE_GRAPH_2D; i++ )
	{
		if ( m_apGraphWnd[i]== NULL )
		{
			continue;
		}

		//! Ŀ���� ������, �߰��Ѵ�.
		if ( m_apGraphWnd[i]->GetCurrentCursorSize() <= 0 )
		{
			bRet = m_apGraphWnd[i]->AddCursorInDataIndex(0, false);
			m_apGraphWnd[i]->SetCursorValueVisible(0, false, false, false);
		}

		m_apGraphWnd[i]->m_bChanged = true;
		m_apGraphWnd[i]->Invalidate();
	}
}


void CAutoDispDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	ReleaseGraphWnd();
}


void CAutoDispDlg::dispCurTaskOnLabel(int iRtn, CString sLabel)
{
	COLORREF color;

	if ( iRtn > 0 )		color = GRID_COLOR_GREEN;
	else				color = M_COLOR_RED;

	m_label_Cur_Task
		.SetBkColor(color)
		.SetText(sLabel);

	m_label_Cur_Task.Invalidate();
}

BOOL CAutoDispDlg::DestroyWindow()
{
	vision.DeleteBuffer_AutoDisp();

	return CDialogEx::DestroyWindow();
}


void CAutoDispDlg::DispResultImage_Opt()
{
	double dFactorX, dFactorY;
	
	dFactorX	= (double)(m_iSizeX_Client)/(double)(gMIUDevice.nWidth);
	dFactorY	= (double)(m_iSizeY_Client)/(double)(gMIUDevice.nHeight);

	vision.GetBuffer_AutoDisp(0, dFactorX, dFactorY);

	MbufClear(vision.MilOptOverlayImage, vision.TransparentColor_Opt);

	CPoint OptCenterP;

	OptCenterP.x	= (int)(Task.dOpticalPosX * dFactorX + 0.5);
	OptCenterP.y	= (int)(Task.dOpticalPosY * dFactorY + 0.5);

	MgraColor(M_DEFAULT, M_COLOR_RED);
	MgraLine(M_DEFAULT, vision.MilOptOverlayImage, (OptCenterP.x-50), OptCenterP.y, (OptCenterP.x+50), OptCenterP.y);
	MgraLine(M_DEFAULT, vision.MilOptOverlayImage, OptCenterP.x, (OptCenterP.y-50), OptCenterP.x, (OptCenterP.y+50));

	MgraColor(M_DEFAULT, M_COLOR_BLUE);
	MgraLine(M_DEFAULT, vision.MilOptOverlayImage, 0, m_iSizeY_Client/2, m_iSizeX_Client, m_iSizeY_Client/2);
	MgraLine(M_DEFAULT, vision.MilOptOverlayImage, m_iSizeX_Client/2, 0, m_iSizeX_Client/2, m_iSizeY_Client);

	CString sTemp;
	sTemp.Format("X : %.03f mm", Task.dOpticalShiftX);
	MgraColor(M_DEFAULT, M_COLOR_MAGENTA);
	MgraText(M_DEFAULT, vision.MilOptOverlayImage, 10, 10, sTemp.GetBuffer());
	sTemp.Format("Y : %.03f mm", Task.dOpticalShiftY);
	MgraText(M_DEFAULT, vision.MilOptOverlayImage, 10, 30, sTemp.GetBuffer());
}


void CAutoDispDlg::DispResultImage_Defect()
{
	double dFactorX, dFactorY;

	dFactorX	= (double)(m_iSizeX_Client)/(double)(gMIUDevice.nWidth);
	dFactorY	= (double)(m_iSizeY_Client)/(double)(gMIUDevice.nHeight);

	vision.GetBuffer_AutoDisp(1, dFactorX, dFactorY);

	MbufClear(vision.MilDefectOverlayImage, vision.TransparentColor_Defect);

	CRect rcReduceBox;

	MgraColor(M_DEFAULT, M_COLOR_RED);
	for (int i=0 ; i<Task.iCnt_Total_Defect ; i++)
	{
		rcReduceBox.left		= (int)(Task.defect.rect[i].left	* dFactorX + 0.5);
		rcReduceBox.top			= (int)(Task.defect.rect[i].top		* dFactorY + 0.5);
		rcReduceBox.right		= (int)(Task.defect.rect[i].right	* dFactorX + 0.5);
		rcReduceBox.bottom		= (int)(Task.defect.rect[i].bottom	* dFactorY + 0.5);

		if(rcReduceBox.top == rcReduceBox.bottom)
			rcReduceBox.bottom++;

		if(rcReduceBox.left == rcReduceBox.right)
			rcReduceBox.right++;

		MgraRect(M_DEFAULT, vision.MilDefectOverlayImage, rcReduceBox.left, rcReduceBox.top, rcReduceBox.right, rcReduceBox.bottom);
	}
}


void CAutoDispDlg::DrawBarGraph()
{
	CString sTemp;
	int i = 0;
	int j = 0;
	HWND hWnd = GetDlgItem(IDC_STATIC_PLOT4)->m_hWnd;
	CDC pDC;
	HDC hDC = ::GetDC(hWnd);

	pDC.Attach(hDC);

	CRect rcClient;
	GetDlgItem(IDC_STATIC_PLOT4)->GetClientRect(rcClient);

	CBrush Brush;
	CBrush* pOldBrush;
	Brush.CreateSolidBrush(RGB(255,255,255));
	pOldBrush = pDC.SelectObject(&Brush);

	pDC.FillRect(rcClient, &Brush);

	int iSizeX_Client = rcClient.right - rcClient.left;
	int iSizeY_Client = rcClient.bottom - rcClient.top;
	int iOffsetY = 20;

	CPen* pOldPen;
	CPen pen_LineBase;
	CPen pen_LineVertical;
	CPen pen_LineLimitCen;
	CPen pen_LineLimitSide;

	CFont font_LimitVal;
	CFont font_BarName;
	CFont* pOldFont;

	pen_LineBase.CreatePen(PS_SOLID, 1, RGB(0,0,0));
	pen_LineVertical.CreatePen(PS_DOT, 1, RGB(0,0,0));
	pen_LineLimitCen.CreatePen(PS_SOLID, 1, RGB(128,0,0));
	pen_LineLimitSide.CreatePen(PS_SOLID, 1, RGB(128,0,128));


	pOldPen = pDC.SelectObject(&pen_LineBase);

	pDC.MoveTo(5, iSizeY_Client - iOffsetY);
	pDC.LineTo(iSizeX_Client - 5, iSizeY_Client - iOffsetY);

	pOldPen = pDC.SelectObject(&pen_LineVertical);

	pDC.MoveTo(5, 0);
	pDC.LineTo(iSizeX_Client - 5, 0);

	pDC.MoveTo(5, (iSizeY_Client - iOffsetY) / 4 * 1);
	pDC.LineTo(iSizeX_Client - 5, (iSizeY_Client - iOffsetY) / 4 * 1);

	pDC.MoveTo(5, (iSizeY_Client - iOffsetY) / 4 * 2);
	pDC.LineTo(iSizeX_Client - 5, (iSizeY_Client - iOffsetY) / 4 * 2);

	pDC.MoveTo(5, (iSizeY_Client - iOffsetY) / 4 * 3);
	pDC.LineTo(iSizeX_Client - 5, (iSizeY_Client - iOffsetY) / 4 * 3);

	font_LimitVal.CreateFont(10,6,0,0,0,0,0,0,DEFAULT_CHARSET,0,0,0,0,"Airal");
	pOldFont = pDC.SelectObject(&font_LimitVal);
	/*
	pOldPen = pDC.SelectObject(&pen_LineLimitCen);
	pDC.MoveTo(5, (iSizeY_Client-iOffsetY) - (int)(sysData.dLimit_SFR_Cen * (iSizeY_Client-iOffsetY)+0.5));
	pDC.LineTo(iSizeX_Client - 5, (iSizeY_Client-iOffsetY) - (int)(sysData.dLimit_SFR_Cen * (iSizeY_Client-iOffsetY)+0.5));
	sTemp.Format("%.2lf", sysData.dLimit_SFR_Cen);
	pDC.SetTextColor(RGB(128,0,0));
	pDC.TextOut(iSizeX_Client - 30, (iSizeY_Client-iOffsetY) - (int)(sysData.dLimit_SFR_Cen * (iSizeY_Client-iOffsetY)+0.5) - 12, sTemp);

	pOldPen = pDC.SelectObject(&pen_LineLimitSide);
	pDC.MoveTo(5, (iSizeY_Client-iOffsetY) - (int)(sysData.dLimit_SFR_Side * (iSizeY_Client-iOffsetY)+0.5));
	pDC.LineTo(iSizeX_Client - 5, (iSizeY_Client-iOffsetY) - (int)(sysData.dLimit_SFR_Side * (iSizeY_Client-iOffsetY)+0.5));
	sTemp.Format("%.2lf", sysData.dLimit_SFR_Side);
	pDC.SetTextColor(RGB(128,0,128));
	pDC.TextOut(iSizeX_Client - 30, (iSizeY_Client-iOffsetY) - (int)(sysData.dLimit_SFR_Side * (iSizeY_Client-iOffsetY)+0.5) - 12, sTemp);
	*/
	CPen pen_Cen, pen_LeftUpper, pen_RightUpper, pen_LeftLower, pen_RightLower;
	pen_Cen.CreatePen(PS_SOLID, 4, RGB(0,0,255));
	pen_LeftUpper.CreatePen(PS_SOLID, 4, RGB(255,0,0));
	pen_RightUpper.CreatePen(PS_SOLID, 4, RGB(0,255,0));
	pen_LeftLower.CreatePen(PS_SOLID, 4, RGB(63,0,153));
	pen_RightLower.CreatePen(PS_SOLID, 4, RGB(0,216,255));

	font_BarName.CreateFont(12,7,0,0,0,0,0,0,DEFAULT_CHARSET,0,0,0,0,"Arial");
	pOldFont = pDC.SelectObject(&font_BarName);
	pDC.SetTextColor(RGB(0,0,0));
	double MtfSfr[5];
	int mCount = 0;
	int mRoiIndex = 0;
	int mTotalCount = 0;
	double mTempMtf = 0.0;
	
	for (i = 0; i < model.mGlobalChartCount; i++)
	{
		mCount = 0;
		mTempMtf = 0.0;
		if (model.sfrElem.m_AATiltRoiIndex[mRoiIndex] == i)
		{
				
			for (j = 0; j < 4; j++)
			{
				if (model.sfrElem.m_SfrTestUse[i][j] == 1)
				{
					if (sysData.m_iProductComp == 1)
					{
						mTempMtf += MandoInspLog.dMTF_PostUV[mTotalCount];
					}
					else
					{
						if (Task.PCBTask == 65000 || Task.PCBTask == 60200)
						{
							mTempMtf += MandoInspLog.dMTF_PreUV[mTotalCount];
						}
						else
						{
							mTempMtf += MandoInspLog.dMTF_PostUV[mTotalCount];
						}
					}
						
					mCount++;
					mTotalCount++;
				}
			}
			MtfSfr[mRoiIndex] = mTempMtf / mCount;
			mRoiIndex++;
		}
		else
		{
			for (j = 0; j < 4; j++)
			{
				if (model.sfrElem.m_SfrTestUse[i][j] == 1)
				{
					mTotalCount++;
				}
			}
		}
	}

	
	for (int i=0 ; i<MAX_MTF_NO ; i++)
	{
		if ( i == 0 )		pOldPen = pDC.SelectObject(&pen_Cen);
		else if ( i == 1 )	pOldPen = pDC.SelectObject(&pen_LeftUpper);
		else if ( i == 2 )	pOldPen = pDC.SelectObject(&pen_RightUpper);
		else if ( i == 3 )	pOldPen = pDC.SelectObject(&pen_LeftLower);
		else if ( i == 4 )	pOldPen = pDC.SelectObject(&pen_RightLower);

		pDC.MoveTo((iSizeX_Client-5)/6*(i+1), iSizeY_Client - iOffsetY);

//////////////////////////////////////////////////////////////////////////////////////////
		if(sysData.m_iProductComp == 1) 
		{
			//MandoInspLog.dMTF_PostUV

			pDC.LineTo((iSizeX_Client-5)/6*(i+1), (iSizeY_Client - iOffsetY) - (int)(MtfSfr[i] * (iSizeY_Client - iOffsetY)+0.5));
			sTemp.Format("%.3lf", MtfSfr[i]);
			pDC.TextOut((iSizeX_Client-5)/6*(i+1)-15, (iSizeY_Client - iOffsetY) - (int)(MtfSfr[i] * (iSizeY_Client - iOffsetY)+0.5) - 10, sTemp);
		}else
		{
			//pDC.LineTo((iSizeX_Client-5)/6*(i+1), (iSizeY_Client - iOffsetY) - (int)(Task.SFR.fSfrN4[Task.m_iDrawBarStep-1][i] * (iSizeY_Client - iOffsetY)+0.5));
			pDC.LineTo((iSizeX_Client - 5) / 6 * (i + 1), (iSizeY_Client - iOffsetY) - (int)(MtfSfr[i] * (iSizeY_Client - iOffsetY) + 0.5));
			//sTemp.Format("%.3lf", Task.SFR.fSfrN4[Task.m_iDrawBarStep-1][i]);
			sTemp.Format("%.3lf", MtfSfr[i]);
			pDC.TextOut((iSizeX_Client-5)/6*(i+1)-15, (iSizeY_Client - iOffsetY) - (int)(MtfSfr[i] * (iSizeY_Client - iOffsetY)+0.5) - 10, sTemp);
		}
		
//////////////////////////////////////////////////////////////////////////////////////////
	}

	
	
	
	pDC.SetTextColor(RGB(0, 0, 255));
	pDC.TextOut((iSizeX_Client - 5) / 6 - 13, (iSizeY_Client - iOffsetY) + 5, "CEN");
	pDC.SetTextColor(RGB(255, 0, 0));
	if (model.mGlobalChartType == 0)
	{
		//X����
		pDC.TextOut((iSizeX_Client - 5) / 6 * 2 - 10, (iSizeY_Client - iOffsetY) + 5, "TL");
		pDC.SetTextColor(RGB(0, 255, 0));
		pDC.TextOut((iSizeX_Client - 5) / 6 * 3 - 10, (iSizeY_Client - iOffsetY) + 5, "TR");
		pDC.SetTextColor(RGB(63, 0, 153));
		pDC.TextOut((iSizeX_Client - 5) / 6 * 4 - 10, (iSizeY_Client - iOffsetY) + 5, "BL");
		pDC.SetTextColor(RGB(0, 216, 255));
		pDC.TextOut((iSizeX_Client - 5) / 6 * 5 - 10, (iSizeY_Client - iOffsetY) + 5, "BR");
	}
	else
	{
		pDC.TextOut((iSizeX_Client - 5) / 6 * 2 - 10, (iSizeY_Client - iOffsetY) + 5, "T");
		pDC.SetTextColor(RGB(0, 255, 0));
		pDC.TextOut((iSizeX_Client - 5) / 6 * 3 - 10, (iSizeY_Client - iOffsetY) + 5, "B");
		pDC.SetTextColor(RGB(63, 0, 153));
		pDC.TextOut((iSizeX_Client - 5) / 6 * 4 - 10, (iSizeY_Client - iOffsetY) + 5, "L");
		pDC.SetTextColor(RGB(0, 216, 255));
		pDC.TextOut((iSizeX_Client - 5) / 6 * 5 - 10, (iSizeY_Client - iOffsetY) + 5, "R");
	}
	



	pDC.SelectObject(pOldPen);
	pen_LineBase.DeleteObject();
	pen_LineVertical.DeleteObject();
	pen_LineLimitCen.DeleteObject();
	pen_LineLimitSide.DeleteObject();

	pen_Cen.DeleteObject();
	pen_LeftUpper.DeleteObject();
	pen_RightUpper.DeleteObject();
	pen_LeftLower.DeleteObject();
	pen_RightLower.DeleteObject();

	pDC.SelectObject(pOldBrush);
	Brush.DeleteObject();

	pDC.SelectObject(pOldFont);
	font_LimitVal.DeleteObject();
	font_BarName.DeleteObject();

	pDC.Detach();
	::ReleaseDC(hWnd, hDC);
	sTemp.Empty();
}

void CAutoDispDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	DrawBarGraph();
}
