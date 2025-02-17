// CCDSFRDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CCDSFRDlg.h"
#include "afxdialogex.h"
#include "MarkSetDlg.h"

//! ====================================================================================
//! �׷��� ��Ʈ�� Ŭ������ ���Խ�Ų��. 
#include "Graph2D/Graph2DWnd/Graph2DWnd.h"
//! ====================================================================================
#include "ChartSetDlg.h"
#include "AutoDispDlg.h"
extern CAutoDispDlg* autodispDlg;
extern		CChartSetDlg*			chartSetDlg;
#include "MIU/ConvertColor.h"


extern		bool		bThreadCcmGrabRun;		// CCM ���� ȹ�� Thread ���۽� True




IMPLEMENT_DYNAMIC(CCCDSFRDlg, CDialogEx)

	CCCDSFRDlg::CCCDSFRDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCCDSFRDlg::IDD, pParent)
	, m_bShadeCalibration(FALSE)
{
	int i = 0, j = 0;
	for ( i = 0; i < iBufSize_Graph_2D; i++ )
	{
		m_apGraphWnd[i] = NULL;	

		m_aiIndex_Overlap_Cursor[i] = 0;

		m_aiCount_Visible[i] = iBufSize_Graph_Overlap;

		for ( j = 0; j < iBufSize_Graph_Overlap; j++ )
		{
			Init_Buffser(i, j);

			m_abFlag_Visible[i][j] = true;					
		}		
	}

	m_iSelMoveType = 0;

	m_dTilt_X = m_dTilt_Y = 0;
	m_dShift_X = m_dShift_Y = 0;
    m_pImgBuff = NULL;
}

CCCDSFRDlg::~CCCDSFRDlg()
{
}

void CCCDSFRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Radio(pDX, IDC_RADIO_POS, m_iSelType);
	DDX_Radio(pDX, IDC_RADIO_POS, m_iSelMoveType);
	DDX_Control(pDX, IDC_EDIT_SFR_X, m_SFR_X);
	DDX_Control(pDX, IDC_EDIT_SFR_Y, m_SFR_Y);
	DDX_Control(pDX, IDC_EDIT_SFR_DIC, m_SFR_Dic);
	


	DDX_Control(pDX, IDC_EDIT_UART1, m_Uart_s);
	DDX_Control(pDX, IDC_EDIT_UART2, m_Uart_e);
	DDX_Control(pDX, IDC_COMBO_SFR, Combox_SFRIndex);
}


BEGIN_MESSAGE_MAP(CCCDSFRDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CCCDSFRDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CCCDSFRDlg::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_IMG_SAVE, &CCCDSFRDlg::OnBnClickedButtonImgSave)
	ON_BN_CLICKED(IDC_BUTTON_IMG_LOAD, &CCCDSFRDlg::OnBnClickedButtonImgLoad)
	ON_BN_CLICKED(IDC_RADIO_POS, &CCCDSFRDlg::OnBnClickedRadioPos)
	ON_BN_CLICKED(IDC_RADIO_SIZE, &CCCDSFRDlg::OnBnClickedRadioSize)
	ON_BN_CLICKED(IDC_BUTTON_BOX_UP, &CCCDSFRDlg::OnBnClickedButtonBoxUp)
	ON_BN_CLICKED(IDC_BUTTON_BOX_DOWN, &CCCDSFRDlg::OnBnClickedButtonBoxDown)
	ON_BN_CLICKED(IDC_BUTTON_BOX_LEFT, &CCCDSFRDlg::OnBnClickedButtonBoxLeft)
	ON_BN_CLICKED(IDC_BUTTON_BOX_RIGHT, &CCCDSFRDlg::OnBnClickedButtonBoxRight)
	ON_BN_CLICKED(IDC_BUTTON_BOX_SPEED, &CCCDSFRDlg::OnBnClickedButtonBoxSpeed)
	ON_BN_CLICKED(IDC_BUTTON_REGIST, &CCCDSFRDlg::OnBnClickedButtonRegist)
	ON_BN_CLICKED(IDC_BUTTON_SFR_TEST, &CCCDSFRDlg::OnBnClickedButtonSfrTest)

	ON_MESSAGE(WM_GRAPH2D_CURSOR_EVENT, OnCursorEventMsgInPlot)	//! Added by LHW (2013/2/22)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_AUTO_AA, &CCCDSFRDlg::OnBnClickedButtonAutoAa)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CCCDSFRDlg::OnBnClickedButtonClear)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_REGIST_EDIT, &CCCDSFRDlg::OnBnClickedButtonRegistEdit)
	ON_BN_CLICKED(IDC_PATTERN_REGIST, &CCCDSFRDlg::OnBnClickedPatternRegist)
	ON_BN_CLICKED(IDC_PATTERN_FIND, &CCCDSFRDlg::OnBnClickedPatternFind)
	ON_BN_CLICKED(IDC_PATTERN_POS, &CCCDSFRDlg::OnBnClickedPatternPos)
	ON_BN_CLICKED(IDC_PATTERN_VIEW, &CCCDSFRDlg::OnBnClickedPatternView)
	ON_BN_CLICKED(IDC_BTN_AUTO_Z_CALC, &CCCDSFRDlg::OnBnClickedBtnAutoZCalc)
	ON_BN_CLICKED(IDC_BTN_AUTO_Z_SEARCH, &CCCDSFRDlg::OnBnClickedBtnAutoZSearch)
	ON_BN_CLICKED(IDC_BUTTON_IMG_SAVE2, &CCCDSFRDlg::OnBnClickedButtonImgSave2)
	ON_BN_CLICKED(IDC_BUTTON_CCD_OPEN, &CCCDSFRDlg::OnBnClickedButtonCcdOpen)
	ON_BN_CLICKED(IDC_BUTTON_CCD_STOP, &CCCDSFRDlg::OnBnClickedButtonCcdStop)
	ON_BN_CLICKED(IDC_BUTTON_CCD_PLAY, &CCCDSFRDlg::OnBnClickedButtonCcdPlay)
	ON_BN_CLICKED(IDC_BUTTON_CCD_CLOSE, &CCCDSFRDlg::OnBnClickedButtonCcdClose)
	ON_STN_CLICKED(IDC_EDIT_SFR_X, &CCCDSFRDlg::OnStnClickedEditSfrX)
	ON_STN_CLICKED(IDC_EDIT_SFR_Y, &CCCDSFRDlg::OnStnClickedEditSfrY)
	ON_BN_CLICKED(IDC_BTN_SFRPOS_SAVE, &CCCDSFRDlg::OnBnClickedBtnSfrposSave)

	ON_CBN_CLOSEUP(IDC_COMBO_SFR, &CCCDSFRDlg::OnCbnCloseupComboSfr)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_LOAD, &CCCDSFRDlg::OnBnClickedButtonModelLoad)
	ON_BN_CLICKED(IDC_UART_OPEN, &CCCDSFRDlg::OnBnClickedUartOpen)
	ON_BN_CLICKED(IDC_UART_CLOSE, &CCCDSFRDlg::OnBnClickedUartClose)
	ON_BN_CLICKED(IDC_UART_SEND, &CCCDSFRDlg::OnBnClickedUartSend)
	ON_BN_CLICKED(IDC_UART_SAVE, &CCCDSFRDlg::OnBnClickedUartSave)
	ON_STN_CLICKED(IDC_EDIT_UART1, &CCCDSFRDlg::OnStnClickedEditUart1)
	ON_STN_CLICKED(IDC_EDIT_UART2, &CCCDSFRDlg::OnStnClickedEditUart2)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_LOAD2, &CCCDSFRDlg::OnBnClickedButtonModelLoad2)
    ON_BN_CLICKED(IDC_BUTTON_RAW_LOAD, &CCCDSFRDlg::OnBnClickedButtonRawLoad)
    ON_BN_CLICKED(IDC_BUTTON_6500K_RAW_LOAD, &CCCDSFRDlg::OnBnClickedButton6500kRawLoad)
    ON_BN_CLICKED(IDC_BUTTON_2800K_RAW_LOAD, &CCCDSFRDlg::OnBnClickedButton2800kRawLoad)
    ON_BN_CLICKED(IDC_BUTTON_DARK_RAW_LOAD, &CCCDSFRDlg::OnBnClickedButtonDarkRawLoad)
    ON_BN_CLICKED(IDC_BUTTON_CCD_FOV_MARK_REGIST, &CCCDSFRDlg::OnBnClickedButtonCcdFovMarkRegist)
    ON_BN_CLICKED(IDC_BUTTON_CCD_FOV_MARK_VIEW, &CCCDSFRDlg::OnBnClickedButtonCcdFovMarkView)
    ON_BN_CLICKED(IDC_BUTTON_CCD_SNR_MARK_REGIST, &CCCDSFRDlg::OnBnClickedButtonCcdSnrMarkRegist)
    ON_BN_CLICKED(IDC_BUTTON_CCD_SNR_ROI_VIEW, &CCCDSFRDlg::OnBnClickedButtonCcdSnrRoiView)
	ON_BN_CLICKED(IDC_BUTTON_ZOOX_LINEAR_MODE, &CCCDSFRDlg::OnBnClickedButtonZooxLinearMode)
	ON_BN_CLICKED(IDC_BUTTON_CHART_SECOND_RAW_LOAD, &CCCDSFRDlg::OnBnClickedButtonChartSecondRawLoad)
	ON_BN_CLICKED(IDC_BUTTON_CHART_FIRST_RAW_LOAD, &CCCDSFRDlg::OnBnClickedButtonChartFirstRawLoad)
	ON_STN_CLICKED(IDC_EDIT_SFR_DIC, &CCCDSFRDlg::OnStnClickedEditSfrDic)
	ON_BN_CLICKED(IDC_BUTTON_CHART_SET_VIEW, &CCCDSFRDlg::OnBnClickedButtonChartSetView)
END_MESSAGE_MAP()


// CCCDSFRDlg �޽��� ó�����Դϴ�.


void CCCDSFRDlg::OnBnClickedCancel(){}
void CCCDSFRDlg::OnBnClickedOk(){}


BOOL CCCDSFRDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//	setInterface();
	
	bool bRet = MakeGraphWnd();

	
		
		
		
		
	GetDlgItem(IDC_BUTTON_ZOOX_LINEAR_MODE)->ShowWindow(SW_HIDE);//ZOOX �𵨸� ���

	UartSet();
	ComboxSet();
    SetFovRoi();
    SetSnrRoi();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CCCDSFRDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//! 'Enter' Ű�� Dialog�� ������ �ʵ��� �����Ѵ�. 
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		return TRUE;
	}

	//! 'Esc' Ű�� Dialog�� ������ �ʵ��� �����Ѵ�. 
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
	{		
		return TRUE;
	}

	//! �θ� �����쿡�� �޽����� �����Ѵ�. 
	CAABonderDlg* pParentDlg = NULL;
	pParentDlg = (CAABonderDlg*)(GetParent());
	if ( pParentDlg != NULL )
	{
		return pParentDlg->PreTranslateMessage(pMsg);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CCCDSFRDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	ReleaseGraphWnd();
}

void CCCDSFRDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	BOOL flag = this->IsWindowVisible();

	m_bShowFlag = bShow ? true : false;

	if (bShow)
	{
		m_iStep = 0;

		m_iCurNo_SFR = 0;
        m_nSelectIndexFOV = 0;
        m_nSelectIndexSNR = 0;

		m_iMoveSpeed = 10;
		ComboxValueSet();
		setInterface();

		for (int i=0; i<4; i++)
		{
			m_rcRoiBox[i] = model.sfrElem.m_CircleRoi[i]; 
		}

		#ifdef ON_LINE_VISION
		vision.clearOverlay(CCD);
		drawRectSFR();
		vision.drawOverlay(CCD);
		#endif

		SetGUI_Graph_Visible_Chk();
	}
	else
	{
		CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

		#ifdef ON_LINE_VISION
		vision.clearOverlay(CAM2);
		vision.drawOverlay(CAM2);
		#endif

		pFrame->m_bMeasureDist = FALSE;
		pFrame->UpdateData(false);
	}
}
void CCCDSFRDlg::OnCbnCloseupComboSfr()//�޺��ڽ� ������ ���� ����
{
	ComboxValueSet();
}
void CCCDSFRDlg::ComboxValueSet()
{
	CString sEditData;
	//int size = 20;
	//Combox_SFRIndex.ResetContent();
	////Combox_SFRIndex.Clear();
	//for (int i = 0; i < model.mGlobalSmallChartCount; i++)//for(int i = 0; i<MTF_INSP_CNT; i++)
	//{
	//	sEditData.Format("%d", (i));
	//	Combox_SFRIndex.AddString(sEditData);
	//	Combox_SFRIndex.SetItemHeight(i, size);
	//}

	//Combox_SFRIndex.SetCurSel(0);
	comBox_Index = Combox_SFRIndex.GetCurSel();        //���� ���õ� �ؽ�Ʈ ���� int������ ��ȯ

	sEditData.Format("%d", model.m_MTF_ROI_Pos[comBox_Index].x);
	SetDlgItemText(IDC_EDIT_SFR_X, sEditData);
	sEditData.Format("%d", model.m_MTF_ROI_Pos[comBox_Index].y);
	SetDlgItemText(IDC_EDIT_SFR_Y, sEditData);
	sEditData.Format("%d", model.m_MTF_Direction[comBox_Index]);
	SetDlgItemText(IDC_EDIT_SFR_DIC, sEditData);

	sEditData.Empty();
	UpdateData(false);
}
void CCCDSFRDlg::ComboxSet()
{
	CString str;
	CFont m_font , m_sfrfont;
	int size = 20;
	int fontSize = 40;
	comBox_Index = 0;
	curSfrPosX = model.m_MTF_ROI_Pos[comBox_Index].x;
	curSfrPosY = model.m_MTF_ROI_Pos[comBox_Index].y;
	
	//m_font.CreateFont(fontSize,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET, 0, 0, 0,VARIABLE_PITCH | FF_ROMAN, _T("���� ���"));
	Combox_SFRIndex.SetItemHeight(-1,size);
	//Combox_SFRIndex.SetFont(&m_font);
	for (int i = 0; i<COMMON_MTF_INSP_CNT; i++)//for(int i = 0; i<MTF_INSP_CNT; i++)
	{
		str.Format("%d" , (i));
		Combox_SFRIndex.AddString(str);
		Combox_SFRIndex.SetItemHeight(i,size);
	}
	
	Combox_SFRIndex.SetCurSel(0);
	m_SFR_X.SetBkColor(M_COLOR_WHITE);
	m_SFR_Y.SetBkColor(M_COLOR_WHITE);
	m_SFR_Dic.SetBkColor(M_COLOR_WHITE);
	ComboxValueSet();

}
void CCCDSFRDlg::setInterface()
{
	for (int i = 0; i<model.mGlobalChartCount; i++)//for(int i=0; i<LAST_MARK_CNT; i++)
	{
		m_iOffsetX_SFR[i]	= model.sfrElem.m_iOffsetX[i];
		m_iOffsetY_SFR[i]	= model.sfrElem.m_iOffsetY[i];
		m_iSizeX_SFR[i]		= model.sfrElem.m_iSizeX[i];
		m_iSizeY_SFR[i]		= model.sfrElem.m_iSizeY[i];


	}

    //fov

    //snr
	UpdateData(false);


}


void CCCDSFRDlg::drawROI()
{
	for (int i=0 ; i<4; i++)
		vision.boxlist[CCD].addList(Task.SFR.rcROI[i], PS_DOT, M_COLOR_GREEN);
}

void CCCDSFRDlg::drawRectSFR(int iRectNo /*=999*/)
{
	char szTmp[256];

	for (int i = 0; i < model.mGlobalChartCount; i++)//for (int i=0 ; i < LAST_MARK_CNT ; i++)
	{
		m_rcFixedBox[i].left	= m_iOffsetX_SFR[i]; 
		m_rcFixedBox[i].top		= m_iOffsetY_SFR[i];
		m_rcFixedBox[i].right	= m_rcFixedBox[i].left + m_iSizeX_SFR[i];
		m_rcFixedBox[i].bottom	= m_rcFixedBox[i].top + m_iSizeY_SFR[i];
	
//////////////////////////////////////////////////////////////////////////////////////////

		if (i != 0 && i > model.mGlobalChartCount)//LAST_MARK_CNT)
		{
			continue;
		}
//////////////////////////////////////////////////////////////////////////////////////////

		if (iRectNo == i) 
		{
			vision.boxlist[CCD].addList(m_rcFixedBox[i], PS_SOLID, M_COLOR_MAGENTA);
		}
		else 
		{
			vision.boxlist[CCD].addList(m_rcFixedBox[i], PS_SOLID, M_COLOR_BLUE);
		}
		sprintf_s(szTmp, "%d", i);
		vision.textlist[CCD].addList(m_rcFixedBox[i].left + 10, m_rcFixedBox[i].top + 10, szTmp, M_COLOR_RED, 15, 10, "Arial");

	}


	//������ũ
	for (int i=0; i<4; i++)
	{
		//vision.crosslist[CCD].addList(model.sfrElem.m_CircleP[i], 300, M_COLOR_GREEN);

		if (i == (iRectNo - model.mGlobalChartCount))//if (i == (iRectNo-LAST_MARK_CNT))
		{
			vision.boxlist[CCD].addList(m_rcRoiBox[i], PS_DOT, M_COLOR_MAGENTA);
		}else
		{
			vision.boxlist[CCD].addList(m_rcRoiBox[i], PS_DOT, M_COLOR_GREEN);
		}
		sprintf_s(szTmp, "%d", i);
		vision.textlist[CCD].addList(m_rcRoiBox[i].left + 10, m_rcRoiBox[i].top + 10, szTmp, M_COLOR_MAGENTA, 15, 10, "Arial");
	} 

	//int y1;
	//int y2;
	//int y3;
	//int y4;
	//int y5, y6;
	//int y7, y8;
	//int x1, x2, x3, x4, x5, x6, x7, x8;
	//int xGap1 = 120;
	//int yGap1 = 80;
	//int yGap2 = -200;
	////x = ���μ�
	//x1 = gMIUDevice.nWidth / 2 - ((gMIUDevice.nWidth / 2) / 2) + xGap1;
	//x2 = gMIUDevice.nWidth / 2 + ((gMIUDevice.nWidth / 2) / 2) - xGap1;
	//x3 = 91;//09�ʵ�
	//x4 = 1729;//09�ʵ�
	//x5 = 382.2;//058�ʵ�
	//x6 = 1437.8;//058�ʵ�
	//x7 = 546;//04�ʵ�
	//x8 = 1274;//04�ʵ�
	//		  //y = ���μ�
	//y1 = gMIUDevice.nHeight / 2 - ((gMIUDevice.nHeight / 2) / 2) - yGap1;

	//y2 = 893;//09�ʵ�
	//y3 = 47;//09�ʵ�
	//y4 = 197.4;//58�ʵ�
	//y5 = 742.6;//58�ʵ�
	//y6 = 282;//4�ʵ�
	//y7 = 658;//4�ʵ�



	//vision.linelist[CCD].addList(x1, 0, x1, gMIUDevice.nHeight, PS_SOLID, M_COLOR_YELLOW);
	//vision.linelist[CCD].addList(x2, 0, x2, gMIUDevice.nHeight, PS_SOLID, M_COLOR_YELLOW);
	//
	/*vision.linelist[CCD].addList(x3, 0, x3, gMIUDevice.nHeight, PS_SOLID, M_COLOR_YELLOW);
	vision.linelist[CCD].addList(x4, 0, x4, gMIUDevice.nHeight, PS_SOLID, M_COLOR_YELLOW);
	vision.linelist[CCD].addList(x5, 0, x5, gMIUDevice.nHeight, PS_SOLID, M_COLOR_BLUE);
	vision.linelist[CCD].addList(x6, 0, x6, gMIUDevice.nHeight, PS_SOLID, M_COLOR_BLUE);
	vision.linelist[CCD].addList(x7, 0, x7, gMIUDevice.nHeight, PS_SOLID, M_COLOR_DARK_GREEN);
	vision.linelist[CCD].addList(x8, 0, x8, gMIUDevice.nHeight, PS_SOLID, M_COLOR_DARK_GREEN);*/



	//vision.linelist[CCD].addList(0, y1, 1820, y1, PS_SOLID, M_COLOR_YELLOW);   //norindakkkkk ccd�� ���׸���
	//
	/*vision.linelist[CCD].addList(0, y2, 1820, y2, PS_SOLID, M_COLOR_YELLOW);
	vision.linelist[CCD].addList(0, y3, 1820, y3, PS_SOLID, M_COLOR_YELLOW);
	vision.linelist[CCD].addList(0, y4, 1820, y4, PS_SOLID, M_COLOR_YELLOW);
	vision.linelist[CCD].addList(0, y5, 1820, y5, PS_SOLID, M_COLOR_YELLOW);
	vision.linelist[CCD].addList(0, y6, 1820, y6, PS_SOLID, M_COLOR_DARK_GREEN);
	vision.linelist[CCD].addList(0, y7, 1820, y7, PS_SOLID, M_COLOR_DARK_GREEN);*/
	//vision.linelist[CCD].addList(0, 100, 1820, 100, PS_SOLID, M_COLOR_YELLOW);
	//vision.linelist[CCD].addList(0, 840, 1820, 840, PS_SOLID, M_COLOR_YELLOW);

	vision.drawOverlay(CCD, true); 

//	drawROI();
}


void CCCDSFRDlg::drawRectFOV(int iRectNo /*=999*/)
{
    int nCount;
    int nGapX = 40;
    int nGapY = 40;
    int i;
    int ntxtNum = 0;
    char szTmp[256];
    
    vision.clearOverlay(CCD);
    for (int i = 0; i<MAX_FOV_COUNT; i++)
    {
        if (i > 2 && i < 6)continue;
        vision.boxlist[CCD].addList(m_clRectDrawFov[i], PS_DOT, M_COLOR_MAGENTA);
        sprintf_s(szTmp, "%d", i);
        vision.textlist[CCD].addList(m_clRectDrawFov[i].left + 10, m_clRectDrawFov[i].top + 10, szTmp, M_COLOR_RED, 15, 10, "Arial");
    }

    vision.drawOverlay(CCD);

    //	drawROI();
}


//-----------------------------------------------------------------------------
//
//	FOV ���� ������ ���� �� �̵�
//
//-----------------------------------------------------------------------------
void CCCDSFRDlg::MoveRectFov(int nMoveType, int nType, int nMoveSize)
{
    int nCount;
    int nIndex;

    /*if (m_nSelectIndexFOV < 0)
        return;*/

    nCount = MAX_FOV_COUNT;

    switch (nType)
    {
    case MOVE_UP:
        if (nMoveType == MOVE_POS)
        {
            nIndex = m_nSelectIndexFOV;
			m_clRectDrawFov[nIndex].top -= nMoveSize;
			m_clRectDrawFov[nIndex].bottom -= nMoveSize;
        }
        else
        {
            nIndex = m_nSelectIndexFOV;
			m_clRectDrawFov[nIndex].bottom -= nMoveSize;

        }
        break;
    case MOVE_DOWN:
        if (nMoveType == MOVE_POS)
        {
            nIndex = m_nSelectIndexFOV;
			m_clRectDrawFov[nIndex].top += nMoveSize;
			m_clRectDrawFov[nIndex].bottom += nMoveSize;
        }
        else
        {
            nIndex = m_nSelectIndexFOV;
			m_clRectDrawFov[nIndex].bottom += nMoveSize;
        }
        break;
    case MOVE_LEFT:
        if (nMoveType == MOVE_POS)
        {
            nIndex = m_nSelectIndexFOV;
			m_clRectDrawFov[nIndex].left -= nMoveSize;
			m_clRectDrawFov[nIndex].right -= nMoveSize;
        }
        else
        {
            nIndex = m_nSelectIndexFOV;
			m_clRectDrawFov[nIndex].right -= nMoveSize;
        }
        break;
    case MOVE_RIGHT:
        if (nMoveType == MOVE_POS)
        {
            nIndex = m_nSelectIndexFOV;
			m_clRectDrawFov[nIndex].left += nMoveSize;
			m_clRectDrawFov[nIndex].right += nMoveSize;
        }
        else
        {
            nIndex = m_nSelectIndexFOV;
			m_clRectDrawFov[nIndex].right += nMoveSize;
        }
        break;
    }

    drawRectFOV(m_nSelectIndexFOV);
}
void CCCDSFRDlg::MoveRectSNR(int nMoveType, int nType, int nMoveSize)
{
    int nCount;
    int nIndex;

	CString strLog = "";
    nCount = MAX_SNR_COUNT;

    switch (nType)
    {
    case MOVE_UP:
        if (nMoveType == MOVE_POS)
        {
            nIndex = m_nSelectIndexSNR;
			m_clRectDrawSnr[nIndex].top -= nMoveSize;
			m_clRectDrawSnr[nIndex].bottom -= nMoveSize;
        }
        else
        {
            nIndex = m_nSelectIndexSNR;
			m_clRectDrawSnr[nIndex].bottom -= nMoveSize;

        }
        break;
    case MOVE_DOWN:
        if (nMoveType == MOVE_POS)
        {
            nIndex = m_nSelectIndexSNR;
			m_clRectDrawSnr[nIndex].top += nMoveSize;
			m_clRectDrawSnr[nIndex].bottom += nMoveSize;
        }
        else
        {
            nIndex = m_nSelectIndexSNR;
			m_clRectDrawSnr[nIndex].bottom += nMoveSize;
        }
        break;
    case MOVE_LEFT:
        if (nMoveType == MOVE_POS)
        {
            nIndex = m_nSelectIndexSNR;
			m_clRectDrawSnr[nIndex].left -= nMoveSize;
			m_clRectDrawSnr[nIndex].right -= nMoveSize;
        }
        else
        {
            nIndex = m_nSelectIndexSNR;
			m_clRectDrawSnr[nIndex].right -= nMoveSize;
        }
        break;
    case MOVE_RIGHT:
        if (nMoveType == MOVE_POS)
        {
            nIndex = m_nSelectIndexSNR;
			m_clRectDrawSnr[nIndex].left += nMoveSize;
			m_clRectDrawSnr[nIndex].right += nMoveSize;
        }
        else
        {
            nIndex = m_nSelectIndexSNR;
			m_clRectDrawSnr[nIndex].right += nMoveSize;
        }
        break;
    }
	int sizeX = m_clRectDrawSnr[nIndex].right - m_clRectDrawSnr[nIndex].left;
	int sizeY = m_clRectDrawSnr[nIndex].bottom - m_clRectDrawSnr[nIndex].top;
	strLog.Format("x: %lu, y: %lu, sizeX: %d, sizeY: %d", m_clRectDrawSnr[nIndex].left, m_clRectDrawSnr[nIndex].top, sizeX, sizeY);
	theApp.MainDlg->putListLog(strLog);

    drawRectSNR(m_nSelectIndexSNR);
}


void CCCDSFRDlg::drawRectSNR(int iRectNo /*=999*/)
{
    int nCount;
    int nGapX = 40;
    int nGapY = 40;
    int i;
    int ntxtNum = 0;
    char szTmp[256];

    for (int i = 0; i<MAX_SNR_COUNT; i++)
    {
        //vision.crosslist[CCD].addList(model.sfrElem.m_clPtSnr[i], 300, M_COLOR_GREEN);
        vision.boxlist[CCD].addList(m_clRectDrawSnr[i], PS_DOT, M_COLOR_MAGENTA); //snr ������ �׸��� �κ�

        sprintf_s(szTmp, "%d", i);
        vision.textlist[CCD].addList(m_clRectDrawSnr[i].left + 10, m_clRectDrawSnr[i].top + 10, szTmp, M_COLOR_RED, 15, 10, "Arial");
    }

    vision.drawOverlay(CCD, true);
}

void CCCDSFRDlg::OnBnClickedButtonImgSave()
{
	int liveMode = gMIUDevice.CurrentState;

	vision.saveImageonExplorer(3);

	gMIUDevice.CurrentState = liveMode;
}


void CCCDSFRDlg::OnBnClickedButtonImgLoad()
{

	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1463);
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	if(Task.AutoFlag == 2)
	{
		sLangChange.LoadStringA(IDS_STRING1468);
		delayMsg(sLangChange, 2000, M_COLOR_RED);// �Ͻ� ���� �� ��� �Ұ�
		return;
	}


	vision.loadImageonExplorer(CCD);
	MIU.MilCopyToCv(vision.MilProcImageChild+3);

	//OnBnClickedButtonSfrTest();
}


void CCCDSFRDlg::OnBnClickedRadioPos()
{
	UpdateData(true);
}


void CCCDSFRDlg::OnBnClickedRadioSize()
{
	UpdateData(true);
}


void CCCDSFRDlg::OnBnClickedButtonBoxUp()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
    vision.clearOverlay(CCD);
    if (vision.m_FovSetMode == true)
    {
        if (m_iSelMoveType == 0)
        {
            MoveRectFov(MOVE_POS, MOVE_UP, m_iMoveSpeed);
        }
        else
        {
            MoveRectFov(MOVE_SIZE, MOVE_UP, m_iMoveSpeed);
        }
       
    }
    else if (vision.m_SnrSetMode == true)
    {
        if (m_iSelMoveType == 0)
        {
            MoveRectSNR(MOVE_POS, MOVE_UP, m_iMoveSpeed);
        }
        else
        {
            MoveRectSNR(MOVE_SIZE, MOVE_UP, m_iMoveSpeed);
        }

    }
    else
    {

        /* �̵� */
        if (m_iSelMoveType == 0)
        {
            m_iOffsetY_SFR[m_iCurNo_SFR] -= m_iMoveSpeed;
        }
        /* ũ�� */
        else
        {
            m_iSizeY_SFR[m_iCurNo_SFR] -= m_iMoveSpeed;
        }
        drawRectSFR(m_iCurNo_SFR);
    }
	

	
	
	vision.drawOverlay(CCD);

	pFrame->m_rBox = m_rcFixedBox[m_iCurNo_SFR];
}


void CCCDSFRDlg::OnBnClickedButtonBoxDown()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
    vision.clearOverlay(CCD);
    if (vision.m_FovSetMode == true)
    {
        if (m_iSelMoveType == 0)
        {
            MoveRectFov(MOVE_POS, MOVE_DOWN, m_iMoveSpeed);
        }
        else
        {
            MoveRectFov(MOVE_SIZE, MOVE_DOWN, m_iMoveSpeed);
        }
    }
    else if (vision.m_SnrSetMode == true)
    {
        if (m_iSelMoveType == 0)
        {
            MoveRectSNR(MOVE_POS, MOVE_DOWN, m_iMoveSpeed);
        }
        else
        {
            MoveRectSNR(MOVE_SIZE, MOVE_DOWN, m_iMoveSpeed);
        }

    }
    else
    {
        if (m_iSelMoveType == 0)
        {
            m_iOffsetY_SFR[m_iCurNo_SFR] += m_iMoveSpeed;
        }
        else
        {
            m_iSizeY_SFR[m_iCurNo_SFR] += m_iMoveSpeed;
        }
        drawRectSFR(m_iCurNo_SFR);
    }

	vision.drawOverlay(CCD);

	pFrame->m_rBox = m_rcFixedBox[m_iCurNo_SFR];
}


void CCCDSFRDlg::OnBnClickedButtonBoxLeft()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
    vision.clearOverlay(CCD);
    if (vision.m_FovSetMode == true)
    {
        if (m_iSelMoveType == 0)
        {
            MoveRectFov(MOVE_POS, MOVE_LEFT, m_iMoveSpeed);
        }
        else
        {
            MoveRectFov(MOVE_SIZE, MOVE_LEFT, m_iMoveSpeed);
        }
    }
    else if (vision.m_SnrSetMode == true)
    {
        if (m_iSelMoveType == 0)
        {
            MoveRectSNR(MOVE_POS, MOVE_LEFT, m_iMoveSpeed);
        }
        else
        {
            MoveRectSNR(MOVE_SIZE, MOVE_LEFT, m_iMoveSpeed);
        }

    }
    else
    {
        /* �̵� */
        if (m_iSelMoveType == 0)
        {
            m_iOffsetX_SFR[m_iCurNo_SFR] -= m_iMoveSpeed;
        }
        /* ũ�� */
        else
        {
            if (m_iSizeX_SFR[m_iCurNo_SFR] > m_iMoveSpeed)
                m_iSizeX_SFR[m_iCurNo_SFR] -= m_iMoveSpeed;
            else
                m_iSizeX_SFR[m_iCurNo_SFR] = 1;
        }
        drawRectSFR(m_iCurNo_SFR);
    }
	

	
	
	vision.drawOverlay(CCD);

	pFrame->m_rBox = m_rcFixedBox[m_iCurNo_SFR];
}


void CCCDSFRDlg::OnBnClickedButtonBoxRight()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
    vision.clearOverlay(CCD);
    if (vision.m_FovSetMode == true)
    {
        if (m_iSelMoveType == 0)
        {
            MoveRectFov(MOVE_POS, MOVE_RIGHT, m_iMoveSpeed);
        }
        else
        {
            MoveRectFov(MOVE_SIZE, MOVE_RIGHT, m_iMoveSpeed);
        }
    }
    else if (vision.m_SnrSetMode == true)
    {
        if (m_iSelMoveType == 0)
        {
            MoveRectSNR(MOVE_POS, MOVE_RIGHT, m_iMoveSpeed);
        }
        else
        {
            MoveRectSNR(MOVE_SIZE, MOVE_RIGHT, m_iMoveSpeed);
        }

    }
    else
    {
        if (m_iSelMoveType == 0)
        {
            m_iOffsetX_SFR[m_iCurNo_SFR] += m_iMoveSpeed; //Move
        }
        else
        {
            m_iSizeX_SFR[m_iCurNo_SFR] += m_iMoveSpeed; //Size
        }
        drawRectSFR(m_iCurNo_SFR);
    }
    //
	
	
	vision.drawOverlay(CCD);
    //
	pFrame->m_rBox = m_rcFixedBox[m_iCurNo_SFR];
}


void CCCDSFRDlg::OnBnClickedButtonBoxSpeed()
{
	CString strTemp;
	GetDlgItem(IDC_BUTTON_BOX_SPEED)->GetWindowText(strTemp);

	if (strTemp=="50")
	{
		m_iMoveSpeed = 1;
		GetDlgItem(IDC_BUTTON_BOX_SPEED)->SetWindowText("1");
	}
    else if (strTemp == "10")
    {
        m_iMoveSpeed = 50;
        GetDlgItem(IDC_BUTTON_BOX_SPEED)->SetWindowText("50");
    }
	else
	{
		m_iMoveSpeed = 10;
		GetDlgItem(IDC_BUTTON_BOX_SPEED)->SetWindowText("10");
	}
}


int CCCDSFRDlg::checkNoSFR(CPoint point)
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	CRect rcTemp;
	CPoint p;

	point.x -= pFrame->m_rectCamDispPos1.left;
	point.y -= pFrame->m_rectCamDispPos1.top;

	p.x = (int)(point.x * (double)gMIUDevice.nWidth/SMALL_CCD_SIZE_X + 0.5);
	p.y = (int)(point.y * (double)gMIUDevice.nHeight/SMALL_CCD_SIZE_Y + 0.5);

	for (int i = 0; i<model.mGlobalChartCount; i++)//for (int i=0 ; i<LAST_MARK_CNT ; i++)
	{
		rcTemp.left		= m_iOffsetX_SFR[i];
		rcTemp.top		= m_iOffsetY_SFR[i];
		rcTemp.right	= rcTemp.left + m_iSizeX_SFR[i];
		rcTemp.bottom	= rcTemp.top + m_iSizeY_SFR[i];

		if (PtInRect(rcTemp, p))
		{
			m_iCurNo_SFR = i;
			return i;
		}
	}

	for (int i=0 ; i<4 ; i++)
	{
		if (PtInRect(m_rcRoiBox[i], p))
		{
			m_iCurNo_SFR = i + model.mGlobalChartCount;//m_iCurNo_SFR = i+LAST_MARK_CNT;
			return i + model.mGlobalChartCount;// LAST_MARK_CNT;
		}
	}

	return -1;
}

//-----------------------------------------------------------------------------
//
//	���õ� SNR ROI ���� �ε��� ��ȯ
//
//-----------------------------------------------------------------------------
int CCCDSFRDlg::GetSelectedSNRNo(CPoint point)
{
    CRect clRect;
    CPoint clPtPos;
    int nSelectNo;
    int i;
    CPoint p;
    //clPtPos.x = (int)((point.x * ((double)gMIUDevice.nWidth / (double)SMALL_CAM_SIZE_X)) + 0.5);
    //clPtPos.y = (int)((point.y * ((double)gMIUDevice.nHeight / (double)SMALL_CAM_SIZE_Y)) + 0.5);
    point.x -= theApp.MainDlg->m_rectCamDispPos1.left;
    point.y -= theApp.MainDlg->m_rectCamDispPos1.top;

    clPtPos.x = (int)(point.x * (double)gMIUDevice.nWidth / SMALL_CCD_SIZE_X + 0.5);
    clPtPos.y = (int)(point.y * (double)gMIUDevice.nHeight / SMALL_CCD_SIZE_Y + 0.5);
    //
    for (i = 0; i < MAX_SNR_COUNT; i++)
    {
        if (PtInRect(m_clRectDrawSnr[i], clPtPos) == TRUE)
        {
            nSelectNo = i;
            return nSelectNo;
        }
    }
    return -1;
}
//-----------------------------------------------------------------------------
//
//	���õ� FOV ROI ���� �ε��� ��ȯ
//
//-----------------------------------------------------------------------------
int CCCDSFRDlg::GetSelectedFOVNo(CPoint point)
{
    CRect clRect;
    CPoint clPtPos;
    int nSelectNo;
    int i;
    point.x -= theApp.MainDlg->m_rectCamDispPos1.left;
    point.y -= theApp.MainDlg->m_rectCamDispPos1.top;
    clPtPos.x = (int)((point.x * ((double)gMIUDevice.nWidth / (double)SMALL_CCD_SIZE_X)) + 0.5);
    clPtPos.y = (int)((point.y * ((double)gMIUDevice.nHeight / (double)SMALL_CCD_SIZE_Y)) + 0.5);
    //
    for (i = 0; i < MAX_FOV_COUNT; i++)
    {
        
        if (PtInRect(m_clRectDrawFov[i], clPtPos) == TRUE)
        {
            nSelectNo = i;
            return nSelectNo;
        }
    }

    return -1;
}

void CCCDSFRDlg::OnBnClickedButtonSfrTest()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	CDPoint findP[5];
	SYSTEMTIME sysTime;

	
	vision.clearOverlay(CCD);
	::GetLocalTime(&sysTime);

	double ep = myTimer(true);

	m_iStep = 0;

	int j=0;
	//int retryCnt = 1;// model.strInfo_CCM.m_iCnt_Average_Thru_Focus;//


	Task.m_iHour	= sysTime.wHour;
	Task.m_iMin		= sysTime.wMinute;
	Task.m_iSec		= sysTime.wSecond;

	
	//==================================================================

	MandoInspLog.func_InitData();
	Task.initSFRElem();

	if (Task.AutoFlag == 2)
	{
		autodispDlg->DrawGraph(0);
		autodispDlg->DrawGraph(1);
	}


	Task.m_iCnt_Step_AA_Total = 5;
	theApp.MainDlg->_getMTF(SFR_MANUAL);

	Task.bInsCenter = true;
	theApp.MainDlg->_calcImageAlignment();
	Task.bInsCenter = false;

	CString sTemp = "";
	sTemp.Format("SFR Test Demosaic:%d" , gMIUDevice.dTDATASPEC_n.eDemosaicMethod);	// , m_iStep);
	theApp.MainDlg->putListLog(sTemp);
	sTemp.Empty();

	DrawGraph(0);
	
	dispDataOnGrid();

	for (int i=0 ; i < 5 ; i++)
	{
		UpdateCursorValue(i);
	}
	if ( m_apGraphWnd[0] != NULL )
	{
		if ( m_apGraphWnd[0]->GetCurrentCursorSize() <= 0 )
		{			
			m_apGraphWnd[0]->AddCursorInDataIndex(0, false, m_aiIndex_Overlap_Cursor[0]);
			m_apGraphWnd[0]->SetCursorValueVisible(0, false, false, false);
		}
	}
}



//! Ŀ���� �������� �����̰� �Ѵ�.
void CCCDSFRDlg::MoveCursor_Left_Graph(int iIndex_Plot)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= iBufSize_Graph_2D )
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
void CCCDSFRDlg::MoveCursor_Right_Graph(int iIndex_Plot)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= iBufSize_Graph_2D )
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
LRESULT CCCDSFRDlg::OnCursorEventMsgInPlot(WPARAM wParam, LPARAM lParam)
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
int CCCDSFRDlg::SetCursorValue(int iIndex_Plot, int iIndex_Overlap, int iDataIndex, int iCursorIndex)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= iBufSize_Graph_2D )
	{
		return -1;
	}
	if ( iIndex_Overlap < 0 || iIndex_Overlap >= iBufSize_Graph_Overlap )
	{
		return -2;
	}
	if ( iDataIndex < 0 || iDataIndex >= iBufSize_Pixel_Graph )
	{
		return -3;
	}
	if ( m_apGraphWnd == NULL )
	{
		return -4;
	}

	bool abRet[2];
	int i = 0;
	double  adDataX[iBufSize_Graph_Overlap], adDataY[iBufSize_Graph_Overlap];
	CString asDataX[iBufSize_Graph_Overlap], asDataY[iBufSize_Graph_Overlap], asData[iBufSize_Graph_Overlap];
	strGraph2D_Y_Axis_Info strAxisYInfo;	//! Y��

	for (i = 0; i < iBufSize_Graph_Overlap; i++ )
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

	switch(iIndex_Plot)
	{
	case 0:
		{
// 			SetString_to_UI(IDC_EDIT_PLOT1_1, this, asData[0]);
// 			SetString_to_UI(IDC_EDIT_PLOT1_2, this, asData[1]);
// 			SetString_to_UI(IDC_EDIT_PLOT1_3, this, asData[2]);
// 			SetString_to_UI(IDC_EDIT_PLOT1_4, this, asData[3]);
// 			SetString_to_UI(IDC_EDIT_PLOT1_5, this, asData[4]);
		}
		break;
	case 1:
		{
// 			SetString_to_UI(IDC_EDIT_PLOT2_1, this, asData[0]);
// 			SetString_to_UI(IDC_EDIT_PLOT2_2, this, asData[1]);
// 			SetString_to_UI(IDC_EDIT_PLOT2_3, this, asData[2]);
// 			SetString_to_UI(IDC_EDIT_PLOT2_4, this, asData[3]);
// 			SetString_to_UI(IDC_EDIT_PLOT2_5, this, asData[4]);
		}
		break;
	}//! switch(iPlotIndex)

	return 1;
}

//! üũ �ڽ��� �̿��Ͽ� �׷����� ���� ���̰ų� �����.
//! �� ��, �׷��� Ŀ�� ���� �۾��� ���� �����Ѵ�. 
//!		1) �׷��� Ŀ���� 5���� �׷��� �� 1���� �׷����� ���ӵȴ�.
//!		2) �׷����� ���� ������ �ʰ� �ϸ�, �ش� �׷����� Ŀ���� ���� ���� ������ �ʰ� �ȴ�.
//!		3) ���� ������ �ʰ� �Ǵ� Ŀ���� �����ϰ�, ���� ���̴� �׷����� ���� ��ġ�� �׷��� Ŀ���� �߰��Ѵ�. 
void CCCDSFRDlg::Proc_GraphVisible(int iIndex_Plot, int iIndex_Overlap)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= iBufSize_Graph_2D )
	{
		return;
	}
	if ( iIndex_Overlap < 0 || iIndex_Overlap >= iBufSize_Graph_Overlap )
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
int CCCDSFRDlg::UpdateCursorValue(int iIndex_Plot)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= iBufSize_Graph_2D )
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
int CCCDSFRDlg::ExChangeCursor(int iIndex_Plot)
{
	int iOverlapIndex = m_aiIndex_Overlap_Cursor[iIndex_Plot];
	if ( iOverlapIndex < 0 || iOverlapIndex >= iBufSize_Graph_Overlap )
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
	for ( i = 0; i < iBufSize_Graph_Overlap; i++ )
	{
		if ( m_abFlag_Visible[iIndex_Plot][i] == true )
		{
			iIndex_Overlap_Add = i;
			break;
		}
	}

	m_apGraphWnd[iIndex_Plot]->AddCursorInDataIndex(iDataIndex, true, iIndex_Overlap_Add);
	m_apGraphWnd[0]->SetCursorValueVisible(0, false, false, false);
	m_aiIndex_Overlap_Cursor[iIndex_Plot] = iIndex_Overlap_Add;

	return 1;
}

//! GUI ó�� �Լ�
//! üũ �ڽ��� ���� ������ ǥ��
void CCCDSFRDlg::SetGUI_Graph_Visible_Chk()
{
	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT1_ROI_1, this, m_abFlag_Visible[0][0]);
	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT1_ROI_2, this, m_abFlag_Visible[0][1]);
	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT1_ROI_3, this, m_abFlag_Visible[0][2]);
	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT1_ROI_4, this, m_abFlag_Visible[0][3]);
	SetButtonCheckState_InCtrl(IDC_CHECK_PLOT1_ROI_5, this, m_abFlag_Visible[0][4]);
}

//! �׷����� ���� ���̰ų� ���� ��
bool CCCDSFRDlg::SetGraphVisible(int iIndex_Plot, int iIndex_Overlap, bool bFlag_Visible)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= iBufSize_Graph_2D )
	{
		return false;
	}
	if ( iIndex_Overlap < 0 || iIndex_Overlap >= iBufSize_Graph_Overlap )
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
bool CCCDSFRDlg::Init_Buffser(int iIndex_Plot, int iIndex_Overlap)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= iBufSize_Graph_2D )
	{
		return false;
	}
	if ( iIndex_Overlap < 0 || iIndex_Overlap >= iBufSize_Graph_Overlap )
	{
		return false;
	}

	int i = 0;
	for ( i = 0; i < iBufSize_Pixel_Graph; i++ )
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
bool CCCDSFRDlg::MakeGraphWnd()
{
	ReleaseGraphWnd();

	int iIndex_Failed = -1;
	int iRet = 0;
	int i = 0, j = 0;

	for ( i = 0; i < iBufSize_Graph_2D; i++ )
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

	for ( i = 0; i < iBufSize_Graph_2D; i++ )
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

	for ( i = 0; i < iBufSize_Graph_2D; i++ )
	{
		//! ���� ���ҽ� ������ ������ ��
		m_apGraphWnd[i]->MakeResourcePtr();

		//! 5���� �׷����� �׸� �� �ְ� �Ѵ�.
		m_apGraphWnd[i]->MakePlotBuffer(iBufSize_Graph_Overlap);

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

		for ( j = 0; j < iBufSize_Graph_Overlap; j++ )
		{
			m_apGraphWnd[i]->GetResourceYInfo(strAxisYInfo, j);
			strAxisYInfo.bShowGraph = false;

			//! �׷��� ����
			switch(j)
			{
			case 0:
				{
					strAxisYInfo.clrData = RGB(0, 128, 0);
				}
				break;
			case 1:
				{
					strAxisYInfo.clrData = RGB(64, 128, 0);
				}
				break;
			case 2:
				{
					strAxisYInfo.clrData = RGB(0, 128, 64);
				}
				break;
			case 3:
				{
					strAxisYInfo.clrData = RGB(64, 64, 0);
				}
				break;
			case 4:
				{
					strAxisYInfo.clrData = RGB(128, 0, 64);
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

		}//! for ( j = 0; j < iBufSize_Graph_Overlap; j++ )




		////////////////////// ����ȣ ����..
		m_apGraphWnd[i]->GetResourceInfo(strResourceInfo);
		strResourceInfo.iAxisThickness						= 1;
		strResourceInfo.clrEntireBackGround					= RGB(231, 231, 231);	//! �׷��� ���� ����
		strResourceInfo.strAxisInfoX.iCursorTextPrecision	= 0;		
		strResourceInfo.strAxisInfoX.eGridTextType			= E_GRAPH2D_GRID_DOUBLE;//E_GRAPH2D_GRID_INTEGER;
		strResourceInfo.strAxisInfoX.iGridTextPrecision		= 3;//0;
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
		m_apGraphWnd[i]->SetCaptionX(_T("cycles/pixel"));
		m_apGraphWnd[i]->SetCaptionY(_T("MTF"), _T(""));

		m_apGraphWnd[i]->GetResourceInfo(strResourceInfo);
		strResourceInfo.strMainTitleInfo.szTitle = _T("SFR");
		m_apGraphWnd[i]->SetResourceInfo(strResourceInfo);

		//! �޽����� ���� �ڵ� ����
		m_apGraphWnd[i]->m_hMsgHandle = this->m_hWnd;		
	}

	return true;
}

//! �׷��� ��Ʈ�� ����
void CCCDSFRDlg::ReleaseGraphWnd()
{
	int i = 0;
	for ( i = 0; i < iBufSize_Graph_2D; i++ )
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
int CCCDSFRDlg::DrawGraph(int iIndex_Plot)
{
	if ( iIndex_Plot < 0 || iIndex_Plot >= iBufSize_Graph_2D )
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
		if(m_i_graph_disp_mode==1)
		{
			for ( j = 0; j < iBufSize_Graph_Overlap; j++ )
			{
				//! <-----------------------------------------------------------
				//! �׷����� ���� ���̴� ���θ� �����Ѵ�.
				m_apGraphWnd[iIndex_Plot]->GetResourceYInfo(strAxisYInfo, j);

				m_aiDataSize[iIndex_Plot][j] = Task.m_iCnt_Step_AA;

				if ( m_aiDataSize[iIndex_Plot][j] <= 0 )
				{
					strAxisYInfo.bShowGraph = false;
				}
				else
				{
					strAxisYInfo.bShowGraph = m_abFlag_Visible[iIndex_Plot][j];
				}

				m_apGraphWnd[iIndex_Plot]->SetResourceYInfo(strAxisYInfo, j);

				for(int i_tmp=0; i_tmp<m_aiDataSize[iIndex_Plot][j]; i_tmp++)
				{
					m_adDataX[iIndex_Plot][j][i_tmp] = Task.SFR.fMotorPos[i_tmp][2];
					m_adDataY[iIndex_Plot][j][i_tmp] = Task.SFR.fSfrN4[i_tmp][j];
				}

				//! �׷����� ������ �Է�
				//m_apGraphWnd[i]->SetPlotData(m_adDataX[i], m_adDataY[i], m_aiDataSize[i]);
				m_apGraphWnd[iIndex_Plot]->SetPlotData(m_adDataX[iIndex_Plot][j], m_adDataY[iIndex_Plot][j], m_aiDataSize[iIndex_Plot][j], j);



				//! <-----------------------------------------------------------------------------------------------------------------------------------------
				//! X�� Ư���� ǥ�ü� ����

				//! [�߰� ����] X�� Ư���� ǥ�ü� ������ �Էµ� ������� ���ο��� CArray~ ������� ����ǹǷ�, �Էµ� �������� �̿��Ͽ� �ٽ� �ҷ��� �� �ִ�.
				iIndex_Line_X_MTF4  = (j * 3) + iIndex_MTF4;
				iIndex_Line_X_MTF8  = (j * 3) + iIndex_MTF8;
				iIndex_Line_X_MTF50 = (j * 3) + iIndex_MTF50;

				m_apGraphWnd[iIndex_Plot]->GetXValueLineInfo(iIndex_Line_X_MTF4, strXLineInfo);

				if ( m_aiDataSize[iIndex_Plot][j] <= 0 )
				{
					strXLineInfo.bShow = false;
				}
				else
				{
					strXLineInfo.bShow = m_abFlag_Visible[iIndex_Plot][j];
				}

				strXLineInfo.dbValue = 0.125;

				m_apGraphWnd[iIndex_Plot]->SetXValueLineInfo(iIndex_Line_X_MTF4, strXLineInfo);
			}
			//! <-----------------------------------------------------------------------------------------------------------------------------------------
		}
		else if(m_i_graph_disp_mode==2)
		{
			for ( j = 0; j < iBufSize_Graph_Overlap; j++ )
			{
				//! <-----------------------------------------------------------
				//! �׷����� ���� ���̴� ���θ� �����Ѵ�.
				m_apGraphWnd[iIndex_Plot]->GetResourceYInfo(strAxisYInfo, j);

				m_aiDataSize[iIndex_Plot][j] = Task.m_iCnt_Step_AA;

				if ( m_aiDataSize[iIndex_Plot][j] <= 0 )
				{
					strAxisYInfo.bShowGraph = false;
				}
				else
				{
					strAxisYInfo.bShowGraph = m_abFlag_Visible[iIndex_Plot][j];
				}

				m_apGraphWnd[iIndex_Plot]->SetResourceYInfo(strAxisYInfo, j);

				for(int i_tmp=0; i_tmp<m_aiDataSize[iIndex_Plot][j]; i_tmp++)
				{
					m_adDataX[iIndex_Plot][j][i_tmp] = Task.SFR.fMotorPos[i_tmp][2];
					m_adDataY[iIndex_Plot][j][i_tmp] = Task.SFR.fSfrN4[i_tmp][j];
				}

				//! �׷����� ������ �Է�
				//m_apGraphWnd[i]->SetPlotData(m_adDataX[i], m_adDataY[i], m_aiDataSize[i]);
				m_apGraphWnd[iIndex_Plot]->SetPlotData(m_adDataX[iIndex_Plot][j], m_adDataY[iIndex_Plot][j], m_aiDataSize[iIndex_Plot][j], j);



				//! <-----------------------------------------------------------------------------------------------------------------------------------------
				//! X�� Ư���� ǥ�ü� ����

				//! [�߰� ����] X�� Ư���� ǥ�ü� ������ �Էµ� ������� ���ο��� CArray~ ������� ����ǹǷ�, �Էµ� �������� �̿��Ͽ� �ٽ� �ҷ��� �� �ִ�.
				iIndex_Line_X_MTF4  = (j * 3) + iIndex_MTF4;
				iIndex_Line_X_MTF8  = (j * 3) + iIndex_MTF8;
				iIndex_Line_X_MTF50 = (j * 3) + iIndex_MTF50;

				m_apGraphWnd[iIndex_Plot]->GetXValueLineInfo(iIndex_Line_X_MTF4, strXLineInfo);

				if ( m_aiDataSize[iIndex_Plot][j] <= 0 )
				{
					strXLineInfo.bShow = false;
				}
				else
				{
					strXLineInfo.bShow = m_abFlag_Visible[iIndex_Plot][j];
				}


				strXLineInfo.dbValue = 0.125;

				m_apGraphWnd[iIndex_Plot]->SetXValueLineInfo(iIndex_Line_X_MTF4, strXLineInfo);
			}
			//! <-----------------------------------------------------------------------------------------------------------------------------------------
		}
		else 
		{
			for ( j = 0; j < iBufSize_Graph_Overlap; j++ )
			{
				//! <-----------------------------------------------------------
				//! �׷����� ���� ���̴� ���θ� �����Ѵ�.
				m_apGraphWnd[iIndex_Plot]->GetResourceYInfo(strAxisYInfo, j);

				m_aiDataSize[iIndex_Plot][j] = Task.SFR.iDataCnt[j];

				if ( m_aiDataSize[iIndex_Plot][j] <= 0 )
				{
					strAxisYInfo.bShowGraph = false;
				}
				else
				{
					strAxisYInfo.bShowGraph = m_abFlag_Visible[iIndex_Plot][j];
				}

				m_apGraphWnd[iIndex_Plot]->SetResourceYInfo(strAxisYInfo, j);

				for(int i_tmp=0; i_tmp<Task.SFR.iDataCnt[j]; i_tmp++)
				{
					m_adDataY[iIndex_Plot][j][Task.SFR.iDataCnt[j]-i_tmp-1] = m_adDataY[iIndex_Plot][j][Task.SFR.iDataCnt[j]+i_tmp] = Task.SFR.fSFR_DataY[j][i_tmp];
					m_adDataX[iIndex_Plot][j][Task.SFR.iDataCnt[j]-i_tmp-1] = -Task.SFR.fSFR_DataX[j][i_tmp];
					m_adDataX[iIndex_Plot][j][Task.SFR.iDataCnt[j]+i_tmp] = Task.SFR.fSFR_DataX[j][i_tmp];
				}

				//! �׷����� ������ �Է�
				//m_apGraphWnd[i]->SetPlotData(m_adDataX[i], m_adDataY[i], m_aiDataSize[i]);
				m_apGraphWnd[iIndex_Plot]->SetPlotData(m_adDataX[iIndex_Plot][j], m_adDataY[iIndex_Plot][j], Task.SFR.iDataCnt[j]*2, j);



				//! <-----------------------------------------------------------------------------------------------------------------------------------------
				//! X�� Ư���� ǥ�ü� ����

				//! [�߰� ����] X�� Ư���� ǥ�ü� ������ �Էµ� ������� ���ο��� CArray~ ������� ����ǹǷ�, �Էµ� �������� �̿��Ͽ� �ٽ� �ҷ��� �� �ִ�.
				iIndex_Line_X_MTF4  = (j * 3) + iIndex_MTF4;
				iIndex_Line_X_MTF8  = (j * 3) + iIndex_MTF8;
				iIndex_Line_X_MTF50 = (j * 3) + iIndex_MTF50;

				m_apGraphWnd[iIndex_Plot]->GetXValueLineInfo(iIndex_Line_X_MTF4, strXLineInfo);

				if ( m_aiDataSize[iIndex_Plot][j] <= 0 )
				{
					strXLineInfo.bShow = false;
				}
				else
				{
					strXLineInfo.bShow = m_abFlag_Visible[iIndex_Plot][j];
				}

				strXLineInfo.dbValue = 0.125;

				m_apGraphWnd[iIndex_Plot]->SetXValueLineInfo(iIndex_Line_X_MTF4, strXLineInfo);
				//! <-----------------------------------------------------------------------------------------------------------------------------------------
			}//! for ( j = 0; j < iBufSize_Graph_Overlap; j++ )
		}

		//! Ŀ���� ������, �߰��Ѵ�.
		//if ( m_apGraphWnd[i]->GetCurrentCursorSize() <= 0 )
		//{
		//	bRet = m_apGraphWnd[i]->AddCursorInDataIndex(0, false);
		//  m_apGraphWnd[i]->SetCursorValueVisible(0, false, false, false);
		//}

		m_apGraphWnd[iIndex_Plot]->m_bChanged = true;
		m_apGraphWnd[iIndex_Plot]->Invalidate();
	}

	return 1;
}




void CCCDSFRDlg::dispDataOnGrid()
{
}


BOOL CCCDSFRDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if ( m_bShowFlag == true )
	{

	}
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CCCDSFRDlg::OnBnClickedButtonAutoAa()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1463);
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	if(Task.AutoFlag == 2)
	{
		sLangChange.LoadStringA(IDS_STRING1468);
		delayMsg(sLangChange, 2000, M_COLOR_RED);// �Ͻ� ���� �� ��� �Ұ�
		return;
	}


	if(Task.AutoFlag ==2) return;


	double pcbX = motor.GetEncoderPos(Motor_PCB_X);
	double pcbY = motor.GetEncoderPos(Motor_PCB_Y);
	double lensX = motor.GetEncoderPos(Motor_Lens_X);
	double lensY = motor.GetEncoderPos(Motor_Lens_Y);

	double distPcbX = fabs(pcbX - model.axis[Motor_PCB_X].pos[Bonding_Pos]);
	double distPcbY = fabs(pcbY - model.axis[Motor_PCB_Y].pos[Bonding_Pos]);
	double distLensX = fabs(lensX - model.axis[Motor_Lens_X].pos[Bonding_Pos]);
	double distLensY = fabs(lensY - model.axis[Motor_Lens_Y].pos[Bonding_Pos]);

	if(distPcbX>3)
	{
		sLangChange.LoadStringA(IDS_STRING971);	//PCB X ���Ͱ� ���� ��ġ�� ��� �ֽ��ϴ�.
		errMsg2(Task.AutoFlag, sLangChange);
		return;
	}

	if(distPcbY>3)
	{
		sLangChange.LoadStringA(IDS_STRING974);	//PCB Y ���Ͱ� ���� ��ġ�� ��� �ֽ��ϴ�.
		errMsg2(Task.AutoFlag, sLangChange);
		return;
	}

	if(distLensX>3)
	{
		sLangChange.LoadStringA(IDS_STRING698);	//Lens X ���Ͱ� ���� ��ġ�� ��� �ֽ��ϴ�.
		errMsg2(Task.AutoFlag, sLangChange);
		return;
	}

	if(distLensY>3)
	{
		sLangChange.LoadStringA(IDS_STRING700);	//Lens Y ���Ͱ� ���� ��ġ�� ��� �ֽ��ϴ�.
		errMsg2(Task.AutoFlag, sLangChange);
		return;
	}

	motor.PCB_Z_Motor_Move(Bonding_Pos);
	Sleep(1000);

	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	Task.AutoFlag = 1;
	pFrame->AutoRunView(Task.AutoFlag);

	pFrame->OnBnClickedButtonCcdAlign();
}


void CCCDSFRDlg::OnBnClickedButtonClear()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}


	int pitch;
	int width;
	int Height;

#ifdef	USE_MIL_BAYER_CONVERSION
	pitch	= MbufInquire(vision.MilProcImageChild[4], M_PITCH, NULL);
	width	= MbufInquire(vision.MilProcImageChild[4], M_SIZE_X, NULL);
	Height	= MbufInquire(vision.MilProcImageChild[4], M_SIZE_Y, NULL);
#else
	pitch = gMIUDevice.imageItp->width;
	Height = gMIUDevice.imageItp->height;
#endif

	
	vision.MiuBufferUpdate();
	Task.getROI();

	int tt = 1;
	vision.clearOverlay(CCD);
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	double SfrResetX[COMMON_LAST_MARK_CNT] = { 0.1, };
	double SfrResetY[COMMON_LAST_MARK_CNT] = { 0.1, };
////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < COMMON_LAST_MARK_CNT; i++)//for(int i = 0;i < LAST_MARK_CNT; i++)
	{
		//m_iOffsetX_SFR[i]	= model.m_FieldBox[i].left ;
		//m_iOffsetY_SFR[i]	= model.m_FieldBox[i].top ;
		m_iSizeX_SFR[i] = width / 11;// model.m_FieldBox[i].right - model.m_FieldBox[i].left;
		m_iSizeY_SFR[i] = Height / 7;// 	= model.m_FieldBox[i].bottom - model.m_FieldBox[i].top ;
	}
	m_iOffsetX_SFR[0] = (int)(width / 2.15);
	m_iOffsetY_SFR[0] = (int)(Height / 2.31);

	m_iOffsetX_SFR[1] = (int)(width / 4.25);
	m_iOffsetY_SFR[1] = (int)(Height / 4.82);

	m_iOffsetX_SFR[2] = (int)(width / 1.38);
	m_iOffsetY_SFR[2] = (int)(Height / 4.82);

	m_iOffsetX_SFR[3] = (int)(width / 4.25);
	m_iOffsetY_SFR[3] = (int)(Height / 1.53);

	m_iOffsetX_SFR[4] = (int)(width / 1.38);
	m_iOffsetY_SFR[4] = (int)(Height / 1.53);

	m_iOffsetX_SFR[5] = (int)(width / 19.2);
	m_iOffsetY_SFR[5] = (int)(Height / 20.6);

	m_iOffsetX_SFR[6] = (int)(width / 1.15);
	m_iOffsetY_SFR[6] = (int)(Height / 19.0);

	m_iOffsetX_SFR[7] = (int)(width / 19.2);
	m_iOffsetY_SFR[7] = (int)(Height / 1.25);

	m_iOffsetX_SFR[8] = (int)(width / 1.15);
	m_iOffsetY_SFR[8] = (int)(Height / 1.25);


	

	

	m_iOffsetX_SFR[9] = (int)(width / 1.15);
	m_iOffsetY_SFR[9] = (int)(Height / 1.27);

	m_iOffsetX_SFR[10] = (int)(width / 20.9);
	m_iOffsetY_SFR[10] = (int)(Height / 1.28);

	 	m_iOffsetX_SFR[11] = (int)(width / 1.36);
	 	m_iOffsetY_SFR[11] = (int)(Height/ 1.50);
	 
	 	m_iOffsetX_SFR[12] = (int)(width / 2.19);
	 	m_iOffsetY_SFR[12] = (int)(Height/ 1.23);
	 
	 	m_iOffsetX_SFR[13] = (int)(width / 5.61);
	 	m_iOffsetY_SFR[13] = (int)(Height/ 1.51);
	 
	 	m_iOffsetX_SFR[14] = (int)(width / 11.1);
	 	m_iOffsetY_SFR[14] = (int)(Height/ 2.41);

		m_iOffsetX_SFR[15] = (int)(width / 19.2);
		m_iOffsetY_SFR[15] = (int)(Height/ 20.6);

		m_iOffsetX_SFR[16] = (int)(width / 1.15);
		m_iOffsetY_SFR[16] = (int)(Height/ 19.0);

		m_iOffsetX_SFR[17] = (int)(width / 1.15);
		m_iOffsetY_SFR[17] = (int)(Height/ 1.27);

		m_iOffsetX_SFR[18] = (int)(width / 20.9);
		m_iOffsetY_SFR[18] = (int)(Height/ 1.28);
//////////////////////////////////////////////////////////////////////////// ���� ��...
    m_rcRoiBox[0].left = (int)(gMIUDevice.nWidth*0.37);
    m_rcRoiBox[0].top = (int)(gMIUDevice.nHeight*0.15);
	m_rcRoiBox[0].right = m_rcRoiBox[0].left + (width / 11) ;// (int)(gMIUDevice.nWidth*0.34);
	m_rcRoiBox[0].bottom = m_rcRoiBox[0].top + (Height / 7);// (int)(gMIUDevice.nHeight*0.18);
	 
    m_rcRoiBox[1].left = (int)(gMIUDevice.nWidth*0.54);
    m_rcRoiBox[1].top = (int)(gMIUDevice.nHeight*0.15);
    m_rcRoiBox[1].right = m_rcRoiBox[1].left + (width / 11);//(int)(gMIUDevice.nWidth*0.73);
    m_rcRoiBox[1].bottom = m_rcRoiBox[1].top + (Height / 7);// (int)(gMIUDevice.nHeight*0.28);

    m_rcRoiBox[2].left = (int)(gMIUDevice.nWidth*0.37);
    m_rcRoiBox[2].top = (int)(gMIUDevice.nHeight*0.72);
    m_rcRoiBox[2].right = m_rcRoiBox[2].left + (width / 11);//(int)(gMIUDevice.nWidth*0.41);
    m_rcRoiBox[2].bottom = m_rcRoiBox[2].top + (Height / 7);// (int)(gMIUDevice.nHeight*0.97);

    m_rcRoiBox[3].left = (int)(gMIUDevice.nWidth*0.54);
    m_rcRoiBox[3].top = (int)(gMIUDevice.nHeight*0.72);
    m_rcRoiBox[3].right = m_rcRoiBox[3].left + (width / 11);//(int)(gMIUDevice.nWidth*0.73);
    m_rcRoiBox[3].bottom = m_rcRoiBox[3].top + (Height / 7);// (int)(gMIUDevice.nHeight*0.97);
    //
	/*m_clRectDrawFov[0].left = (int)(gMIUDevice.nWidth*0.25);
	m_clRectDrawFov[0].top = (int)(gMIUDevice.nHeight*0.03);
	m_clRectDrawFov[0].right = (int)(gMIUDevice.nWidth*0.41);
	m_clRectDrawFov[0].bottom = (int)(gMIUDevice.nHeight*0.28);

    m_clRectDrawFov[1].left = (int)(gMIUDevice.nWidth*0.59);
    m_clRectDrawFov[1].top = (int)(gMIUDevice.nHeight*0.03);
    m_clRectDrawFov[1].right = (int)(gMIUDevice.nWidth*0.73);
    m_clRectDrawFov[1].bottom = (int)(gMIUDevice.nHeight*0.28);

	m_clRectDrawFov[2].left = (int)(gMIUDevice.nWidth*0.25);
	m_clRectDrawFov[2].top = (int)(gMIUDevice.nHeight*0.72);
	m_clRectDrawFov[2].right = (int)(gMIUDevice.nWidth*0.41);
	m_clRectDrawFov[2].bottom = (int)(gMIUDevice.nHeight*0.97);

	m_clRectDrawFov[3].left = (int)(gMIUDevice.nWidth*0.59);
	m_clRectDrawFov[3].top = (int)(gMIUDevice.nHeight*0.72);
	m_clRectDrawFov[3].right = (int)(gMIUDevice.nWidth*0.73);
	m_clRectDrawFov[3].bottom = (int)(gMIUDevice.nHeight*0.97);*/

	pFrame->m_rBox = m_rcFixedBox[0];

	drawRectSFR(999);
	vision.drawOverlay(CCD, true);
}

void CCCDSFRDlg::OnBnClickedButtonRegist()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	bool passFlag = password.m_bFlag;
	passFlag = true;
	//KYH �ӽ� �ּ� ó��
	//if(passFlag == false)
	//{
	//	CString strTemp;
	//	CKeyPadDlg keyDlg;

	//	keyDlg.m_strKeyPad = "Password�� �Է��ϼ���.";

	//	if (keyDlg.DoModal()==IDOK)
	//	{
	//		strTemp = keyDlg.GetstrKeypad();

	//		passFlag = strTemp.CompareNoCase(password.m_szPassword) ? false : true;
	//	}
	//	else
	//	{
	//		return;
	//	}
	//}


	if(passFlag)
	{
		password.m_bFlag = true;
	}
	else
	{
		sLangChange.LoadStringA(IDS_STRING892);
		errMsg2(Task.AutoFlag, sLangChange);	//"Password�� �� �� �Է��߽��ϴ�."
		return;
	}

	sLangChange.LoadStringA(IDS_STRING1068);	//SFR ��ũ ��ġ�� ���� �Ͻðڽ��ϱ�?
	if(!askMsg(sLangChange))
		return;

	CDPoint findP;
	int pitch, width, Height;

	char szTmp[256];

#ifdef	USE_MIL_BAYER_CONVERSION
	int sizeX = MbufInquire(vision.MilProcImageChild[4], M_SIZE_X, NULL);
	Height = MbufInquire(vision.MilProcImageChild[4], M_SIZE_Y, NULL);
	pitch = MbufInquire(vision.MilProcImageChild[4], M_PITCH, NULL);
#else
	pitch = gMIUDevice.imageItp->width;
	Height = gMIUDevice.imageItp->height;
#endif

	vision.clearOverlay(3);
	//vision.MiuBufferUpdate();
	vision.MilBufferUpdate();
	Task.getROI();

	pitch	= MbufInquire(vision.MilProcImageChild[4], M_PITCH, NULL);
	width	= MbufInquire(vision.MilProcImageChild[4], M_SIZE_X, NULL);
	Height	= MbufInquire(vision.MilProcImageChild[4], M_SIZE_Y, NULL);

	/*bool bRtn = Task._findCirclePos(vision.MilImageBuffer[4], pitch, width, Height, Task.SFR.rcROI);

	if ( !bRtn )
	{
		return;
	}*/

	for (int i=0 ; i<4 ; i++)
	{
		model.sfrElem.m_CircleP[i].x	= Task.m_CircleP[i].x;
		model.sfrElem.m_CircleP[i].y	= Task.m_CircleP[i].y;
	}

	for (int i = 0; i<model.mGlobalChartCount; i++)//for (int i=0 ; i<LAST_MARK_CNT ; i++)
	{
		model.sfrElem.m_iOffsetX[i]	= m_rcFixedBox[i].left;
		model.sfrElem.m_iOffsetY[i]	= m_rcFixedBox[i].top;

		model.sfrElem.m_iSizeX[i]		= m_rcFixedBox[i].right - m_rcFixedBox[i].left;
		model.sfrElem.m_iSizeY[i]		= m_rcFixedBox[i].bottom - m_rcFixedBox[i].top;

		vision.boxlist[CCD].addList(model.sfrElem.m_iOffsetX[i], model.sfrElem.m_iOffsetY[i], model.sfrElem.m_iOffsetX[i]+model.sfrElem.m_iSizeX[i], model.sfrElem.m_iOffsetY[i]+model.sfrElem.m_iSizeY[i], 50, M_COLOR_BLUE);

		sprintf_s(szTmp, "%d", i);
		vision.textlist[3].addList(model.sfrElem.m_iOffsetX[i] +10, model.sfrElem.m_iOffsetY[i] + 10, szTmp, M_COLOR_RED, 15, 10, "Arial");
	}

	model.Save();
	model.Load();

	for(int i = 0; i < MARK_CNT; i++)
	{
		int iCh = 1;
		if(i == PCB_Chip_MARK)	iCh = 0;

		for(int j = 0; j < 2; j++){
			vision.geometricMarkPreProc(iCh, i, j);
		}
	}

	vision.drawOverlay(CCD);
}

void CCCDSFRDlg::OnBnClickedButtonRegistEdit()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	bool passFlag = password.m_bFlag;
	passFlag = true;

	if(passFlag)
	{
		password.m_bFlag = true;
	}
	else
	{
		sLangChange.LoadStringA(IDS_STRING892);
		errMsg2(Task.AutoFlag, sLangChange);	//"Password�� �� �� �Է��߽��ϴ�."
		return;
	}

	sLangChange.LoadStringA(IDS_STRING1066);	//SFR �˻� ������ ���� �Ͻðڽ��ϱ�?
	if(!askMsg(sLangChange))
		return;

	CDPoint findP;
	int pitch, Height;

	char szTmp[256];

#ifdef	USE_MIL_BAYER_CONVERSION
	int sizeX = MbufInquire(vision.MilProcImageChild[4], M_SIZE_X, NULL);
	Height = MbufInquire(vision.MilProcImageChild[4], M_SIZE_Y, NULL);
	pitch = MbufInquire(vision.MilProcImageChild[4], M_PITCH, NULL);
#else
	pitch = gMIUDevice.imageItp->width;
	Height = gMIUDevice.imageItp->height;
#endif

	vision.clearOverlay(3);

	int iTemp=0;
	for (int i = 0; i<model.mGlobalChartCount; i++)//for (int i=0 ; i<LAST_MARK_CNT ; i++)
	{
		/*if(i!=0 && i>LAST_MARK_CNT)  
			continue;*/

		
		if(m_rcFixedBox[i].right < m_rcFixedBox[i].left)
		{
			iTemp=m_rcFixedBox[i].right;
			m_rcFixedBox[i].right=m_rcFixedBox[i].left;
			m_rcFixedBox[i].left=iTemp;
		}

		if(m_rcFixedBox[i].bottom < m_rcFixedBox[i].top)
		{
			iTemp=m_rcFixedBox[i].bottom;
			m_rcFixedBox[i].bottom=m_rcFixedBox[i].top;
			m_rcFixedBox[i].top=iTemp;
		}

		//�˻翵�� �ٲٸ� m_rcFixedBox �� ���Ѵ�.
		//���� 7����

		model.sfrElem.m_iOffsetX[i]	= m_rcFixedBox[i].left;//box x��ǥ
		model.sfrElem.m_iOffsetY[i]	= m_rcFixedBox[i].top;//box y��ǥ

		model.sfrElem.m_iSizeX[i]	= abs( m_rcFixedBox[i].right - m_rcFixedBox[i].left);//box width
		model.sfrElem.m_iSizeY[i]	= abs(m_rcFixedBox[i].bottom - m_rcFixedBox[i].top);//box height

		vision.boxlist[CCD].addList(model.sfrElem.m_iOffsetX[i], model.sfrElem.m_iOffsetY[i], model.sfrElem.m_iOffsetX[i]+model.sfrElem.m_iSizeX[i], model.sfrElem.m_iOffsetY[i]+model.sfrElem.m_iSizeY[i], PS_SOLID, M_COLOR_BLUE);

		sprintf_s(szTmp, "%d", i);
		vision.textlist[3].addList(model.sfrElem.m_iOffsetX[i] + 10, model.sfrElem.m_iOffsetY[i] + 10, szTmp, M_COLOR_RED, 15, 10, "Arial");
		
		if(i==0 && false)//i==0  center�� ����
		{
			MpatFree(vision.ChartCenterPatternImage);
			MpatFree(vision.MilTempImage);
			MbufClear(vision.ChartCenterPatternImage, 0);
			MbufClear(vision.MilTempImage, 0);

			vision.ChartCenterPatternImage = M_NULL;
			MbufAlloc2d(vision.MilSystem,model.sfrElem.m_iSizeX[i], model.sfrElem.m_iSizeY[i], M_DEF_IMAGE_TYPE, M_IMAGE+M_DISP, &vision.MilTempImage);
			MpatAllocModel(vision.MilSystem, vision.MilGrabImageChild[3], m_rcFixedBox[i].left, m_rcFixedBox[i].top, model.sfrElem.m_iSizeX[i], model.sfrElem.m_iSizeY[i], M_NORMALIZED, &vision.ChartCenterPatternImage);
			MpatDraw(M_DEFAULT, vision.ChartCenterPatternImage, vision.MilTempImage, M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT);

			MpatSetCenter(vision.ChartCenterPatternImage, model.sfrElem.m_iSizeX[i]/2, model.sfrElem.m_iSizeY[i]/2);

			//����
			{
				CString strPatDir;
				//strPatDir.Format("%s\\Model\\%s\\CHART_Mark_%d-Mask.pat", DATA_DIR, model.mCurModelName,  i);
				strPatDir.Format("%s\\%s\\Data\\Mark\\CHART_Mark_%d-Mask.pat", AA_MODEL_LOAD_DIR, model.mCurModelName, i);
				//
				MpatSave(strPatDir.GetBuffer(999), vision.ChartCenterPatternImage);			
				
				//strPatDir.Format("%s\\Model\\%s\\CHART_Mark_%d-Mask.bmp", DATA_DIR, model.mCurModelName,  i);
				strPatDir.Format("%s\\%s\\Data\\Mark\\CHART_Mark_%d-Mask.bmp", AA_MODEL_LOAD_DIR, model.mCurModelName, i);
				MbufSave(strPatDir.GetBuffer(999), vision.MilTempImage);
				strPatDir.Empty();
			}
		}

		
	}

	for (int i=0 ; i<4 ; i++)
	{
		model.sfrElem.m_CircleRoi[i]	= m_rcRoiBox[i];

		vision.boxlist[CCD].addList(model.sfrElem.m_CircleRoi[i], PS_DOT, M_COLOR_GREEN);

		sprintf_s(szTmp, "%d", i);
		vision.textlist[3].addList(model.sfrElem.m_CircleRoi[i].left + 50, model.sfrElem.m_CircleRoi[i].top + 50, szTmp, M_COLOR_GREEN, 15, 10, "Arial");
	}

	model.Save();
	model.Load();

	vision.drawOverlay(CCD);
}


void CCCDSFRDlg::SetAlignData_Tilt(double dx, double dy)
{
	m_dTilt_X = dx;
	m_dTilt_Y = dy;
	UpdateAlignData();
}

void CCCDSFRDlg::SetAlignData_Shift(double dx, double dy)
{
	m_dShift_X = dx;
	m_dShift_Y = dy;
	UpdateAlignData();
}

void CCCDSFRDlg::UpdateAlignData()
{
}


void CCCDSFRDlg::OnBnClickedPatternRegist()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}
		
	CDPoint findP;
	int pitch = 0;
	int Height = 0;
	

#ifdef	USE_MIL_BAYER_CONVERSION
	int sizeX = MbufInquire(vision.MilProcImageChild[4], M_SIZE_X, NULL);
	Height = MbufInquire(vision.MilProcImageChild[4], M_SIZE_Y, NULL);
	pitch = MbufInquire(vision.MilProcImageChild[4], M_PITCH, NULL);
#else
	pitch = gMIUDevice.imageItp->width;
	Height = gMIUDevice.imageItp->height;
#endif

	vision.clearOverlay(3);

	CString strPatDir;
	int iTemp = 0;
	for (int i = 0; i < model.mGlobalChartCount; i++)//for (int i = 0 ; i < LAST_MARK_CNT; i++)
	{
		if(m_rcFixedBox[i].right < m_rcFixedBox[i].left)
		{
			iTemp = m_rcFixedBox[i].right;
			m_rcFixedBox[i].right=m_rcFixedBox[i].left;
			m_rcFixedBox[i].left = iTemp;
		}
		if(m_rcFixedBox[i].bottom < m_rcFixedBox[i].top)
		{
			iTemp = m_rcFixedBox[i].bottom;
			m_rcFixedBox[i].bottom = m_rcFixedBox[i].top;
			m_rcFixedBox[i].top=iTemp;
		}
		model.m_FieldPattern_Pos[i].x	= m_rcFixedBox[i].left;
		model.m_FieldPattern_Pos[i].y	= m_rcFixedBox[i].top;
		model.m_FieldPattern_Size[i].x	= abs(m_rcFixedBox[i].right - m_rcFixedBox[i].left);
		model.m_FieldPattern_Size[i].y	= abs(m_rcFixedBox[i].bottom - m_rcFixedBox[i].top);

		vision.boxlist[CCD].addList(model.m_FieldPattern_Pos[i].x, model.m_FieldPattern_Pos[i].y, model.m_FieldPattern_Pos[i].x+model.m_FieldPattern_Size[i].x, model.m_FieldPattern_Pos[i].y+model.m_FieldPattern_Size[i].y, PS_SOLID, M_COLOR_BLUE);

#if 1
		MpatFree(vision.FieldPatternImage[i]);
		MbufClear(vision.FieldPatternImage[i], 0);
		MpatFree(vision.MilTempImage);
		MbufClear(vision.MilTempImage, 0);

		MbufAlloc2d(vision.MilSystem, model.m_FieldPattern_Size[i].x, model.m_FieldPattern_Size[i].y, M_DEF_IMAGE_TYPE, M_IMAGE + M_DISP, &vision.MilTempImage);
		MpatAllocModel(vision.MilSystem, vision.MilGrabImageChild[3], model.m_FieldPattern_Pos[i].x, model.m_FieldPattern_Pos[i].y, model.m_FieldPattern_Size[i].x, model.m_FieldPattern_Size[i].y, M_NORMALIZED, &vision.FieldPatternImage[i]);
		MpatDraw(M_DEFAULT, vision.FieldPatternImage[i], vision.MilTempImage, M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT);
		MpatSetCenter(vision.FieldPatternImage[i], model.m_FieldPattern_Size[i].x / 2, model.m_FieldPattern_Size[i].y / 2);

		//strPatDir.Format("%s\\%s\\Data\\Mark\\CHART_Mark_%d-Mask.pat", AA_MODEL_LOAD_DIR, model.mCurModelName, i);
		//strPatDir.Format("%s\\Model\\%s\\CHART_Mark_%d-Mask.pat", DATA_DIR, model.mCurModelName, i);
		strPatDir.Format("%s\\%s\\Data\\Mark\\CHART_Mark_%d-Mask.pat", AA_MODEL_LOAD_DIR, model.mCurModelName, i);
		MpatSave(strPatDir.GetBuffer(999), vision.FieldPatternImage[i]);	//���ϵ��

		strPatDir.Format("%s\\%s\\Data\\Mark\\CHART_Mark_%d-Mask.bmp", AA_MODEL_LOAD_DIR, model.mCurModelName, i);
		//strPatDir.Format("%s\\Model\\%s\\CHART_Mark_%d-Mask.bmp", DATA_DIR, model.mCurModelName, i);
		MbufSave(strPatDir.GetBuffer(999), vision.MilTempImage);
#else
		MpatFree(vision.ChartCenterPatternImage);
		MpatFree(vision.MilTempImage);
		MbufClear(vision.ChartCenterPatternImage, 0);
		MbufClear(vision.MilTempImage, 0);

		vision.ChartCenterPatternImage = M_NULL;
		//
		//
		MbufAlloc2d(vision.MilSystem,model.m_FieldPattern_Size[i].x, model.m_FieldPattern_Size[i].y, M_DEF_IMAGE_TYPE, M_IMAGE+M_DISP, &vision.MilTempImage);
		MpatAllocModel(vision.MilSystem, vision.MilGrabImageChild[3], model.m_FieldPattern_Pos[i].x, model.m_FieldPattern_Pos[i].y, model.m_FieldPattern_Size[i].x, model.m_FieldPattern_Size[i].y, M_NORMALIZED, &vision.ChartCenterPatternImage);
		MpatDraw(M_DEFAULT, vision.ChartCenterPatternImage, vision.MilTempImage, M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT);

		MpatSetCenter(vision.ChartCenterPatternImage, model.m_FieldPattern_Size[i].x/2, model.m_FieldPattern_Size[i].y/2);

		//����

		
		strPatDir.Format("%s\\Model\\%s\\CHART_Mark_%d-Mask.pat", DATA_DIR, model.mCurModelName, i);
		MpatSave(strPatDir.GetBuffer(999), vision.ChartCenterPatternImage);	//���ϵ��
		MpatRestore(vision.MilSystem, strPatDir.GetBuffer(999), &vision.FieldPatternImage[i]);


		strPatDir.Format("%s\\Model\\%s\\CHART_Mark_%d-Mask.bmp", DATA_DIR, model.mCurModelName, i);
		MbufSave(strPatDir.GetBuffer(999), vision.MilTempImage);
		//MpatRestore(vision.MilSystem, strPatDir.GetBuffer(999), &vision.FieldPatternImage[i]);		//221107
#endif
		 
		

	}
	//strPatDir.Format("%s\\Model\\%s\\CHART_Mark_%d-Mask.pat", DATA_DIR, model.mCurModelName, 0);
	//MpatRestore(vision.MilSystem, strPatDir.GetBuffer(999), &vision.ChartCenterPatternImage);

	model.Save();
	vision.drawOverlay(CCD);

	strPatDir.Empty();
}


void CCCDSFRDlg::OnBnClickedPatternFind()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}
	double dCenterPosX = 0.0;
	double dCenterPosY = 0.0;
	double dAngle = 0.0;
	double dRate = 0.0;
	

	vision.clearOverlay(CCD);

	CString strPatDir;

	for (int i = 0; i < model.mGlobalChartCount; i++)//for(int i = 0;i < LAST_MARK_CNT; i++)
	{
		strPatDir.Format("%s\\%s\\Data\\Mark\\CHART_Mark_%d-Mask.pat", AA_MODEL_LOAD_DIR, model.mCurModelName,  i);
		MpatRestore(vision.MilSystem, strPatDir.GetBuffer(999), &vision.FieldPatternImage[i]);
		 
		if(FieldPatternMatching(i,  dCenterPosX,  dCenterPosY,  dAngle,  dRate))
		{
			vision.boxlist[CCD].addList(dCenterPosX-model.m_FieldPattern_Size[i].x/2,dCenterPosY-model.m_FieldPattern_Size[i].y/2,dCenterPosX+model.m_FieldPattern_Size[i].x/2,dCenterPosY+model.m_FieldPattern_Size[i].y/2, PS_SOLID, M_COLOR_RED);
		}
	}
	strPatDir.Empty();
	vision.drawOverlay(CCD, true);
}


void CCCDSFRDlg::OnBnClickedPatternPos()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	CDPoint findP;
	int pitch, Height;

	//char szTmp[256];

#ifdef	USE_MIL_BAYER_CONVERSION
	int sizeX = MbufInquire(vision.MilProcImageChild[4], M_SIZE_X, NULL);
	Height = MbufInquire(vision.MilProcImageChild[4], M_SIZE_Y, NULL);
	pitch = MbufInquire(vision.MilProcImageChild[4], M_PITCH, NULL);
#else
	pitch = gMIUDevice.imageItp->width;
	Height = gMIUDevice.imageItp->height;
#endif

	vision.clearOverlay(CCD);

	int iTemp=0;
	for (int i = 0; i<model.mGlobalChartCount; i++)//for (int i=0 ; i<LAST_MARK_CNT ; i++)
	{
		m_rcFixedBox[i].left	= model.m_FieldPattern_Pos[i].x;
		m_rcFixedBox[i].top		= model.m_FieldPattern_Pos[i].y;
		m_rcFixedBox[i].right	= m_rcFixedBox[i].left + model.m_FieldPattern_Size[i].x;
		m_rcFixedBox[i].bottom	= m_rcFixedBox[i].top  + model.m_FieldPattern_Size[i].y;

		model.sfrElem.m_iOffsetX[i]	= m_rcFixedBox[i].left;
		model.sfrElem.m_iOffsetY[i]	= m_rcFixedBox[i].top;

		model.sfrElem.m_iSizeX[i]		= abs( m_rcFixedBox[i].right - m_rcFixedBox[i].left);
		model.sfrElem.m_iSizeY[i]		= abs(m_rcFixedBox[i].bottom - m_rcFixedBox[i].top);

		vision.boxlist[CCD].addList(model.sfrElem.m_iOffsetX[i], model.sfrElem.m_iOffsetY[i], model.sfrElem.m_iOffsetX[i]+model.sfrElem.m_iSizeX[i], model.sfrElem.m_iOffsetY[i]+model.sfrElem.m_iSizeY[i], PS_SOLID, M_COLOR_BLUE);

	}

	vision.drawOverlay(CCD);
}


void CCCDSFRDlg::OnBnClickedPatternView()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	CString strPatDir;
	//for (int i=0 ; i<LAST_MARK_CNT ; i++)
	int iMarkSizeX = model.m_FieldPattern_Size[m_iCurNo_SFR].x;
	int iMarkSizeY = model.m_FieldPattern_Size[m_iCurNo_SFR].y;

	int cnt = m_iCurNo_SFR;

	strPatDir.Format("%s\\%s\\Data\\Mark\\CHART_Mark_%d-Mask.pat", AA_MODEL_LOAD_DIR, model.mCurModelName, m_iCurNo_SFR);
	MpatRestore(vision.MilSystem, strPatDir.GetBuffer(999), &vision.FieldPatternImage[m_iCurNo_SFR]);		//���� view

	MpatDraw(M_DEFAULT, vision.FieldPatternImage[m_iCurNo_SFR], vision.MilTempImage, M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT);

	CMarkSetDlg dlg(CCD, m_iCurNo_SFR, 0, iMarkSizeX, iMarkSizeY, false);

	double dZoomX = (double)MARK_SIZE_X/(double)iMarkSizeX;
	double dZoomY = (double)MARK_SIZE_Y/(double)iMarkSizeY;

	if (dlg.DoModal()==IDOK)
	{
		MpatInquire(vision.FieldPatternImage[m_iCurNo_SFR], M_CENTER_X, &model.m_FieldPatternCenter[m_iCurNo_SFR].x);
		MpatInquire(vision.FieldPatternImage[m_iCurNo_SFR], M_CENTER_Y, &model.m_FieldPatternCenter[m_iCurNo_SFR].y);
		MpatInquire(vision.FieldPatternImage[m_iCurNo_SFR], M_ORIGINAL_X, &model.m_FieldPatternOrigin[m_iCurNo_SFR].x);
		MpatInquire(vision.FieldPatternImage[m_iCurNo_SFR], M_ORIGINAL_Y, &model.m_FieldPatternOrigin[m_iCurNo_SFR].y);

		model.m_FieldPattern_Size[m_iCurNo_SFR].x = iMarkSizeX;
		model.m_FieldPattern_Size[m_iCurNo_SFR].y = iMarkSizeY;

		model.Save();
	}
	strPatDir.Empty();
}


void CCCDSFRDlg::OnBnClickedDistortionPos()
{ 


}

void CCCDSFRDlg::OnBnClickedBtnAutoZCalc()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	int pitch = MbufInquire(vision.MilProcImageChild[4], M_PITCH, NULL);
	int width = MbufInquire(vision.MilProcImageChild[4], M_SIZE_X, NULL);
	int Height = MbufInquire(vision.MilProcImageChild[4], M_SIZE_Y, NULL);


	vision.clearOverlay(CCD);
	//vision.MiuBufferUpdate();
	vision.MilBufferUpdate();
	Task.getROI();

	bool bRtn = Task._findCirclePos(vision.MilImageBuffer[4], pitch, width, Height, Task.SFR.rcROI);
	if (bRtn == false)	return;
	vision.drawOverlay(CCD);


	if ( askMsg("PCB Z ��ġ�� ���� �Ͻðڽ��ϱ�?") != IDOK )return;

	for (int i = 0; i<4; i++)
	{
		model.m_Fiducial_Mark[i] = Task.m_CircleP[i];
	}

	model.Save();
}


void CCCDSFRDlg::OnBnClickedBtnAutoZSearch()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString sTemp="";
	CString logStr = "";

	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	double dAutoZ = AutoZSearchCalc();

	double dCurZPos = motor.GetCommandPos(theApp.MainDlg->TITLE_MOTOR_Z);
	double dMovePos = motor.GetCommandPos(theApp.MainDlg->TITLE_MOTOR_Z) - dAutoZ;

	logStr.Format("PCB Auto Z��ġ ������ġ:(%.3lf), ������:(%.3lf) , ������:(%.3lf)",dCurZPos, dMovePos, dAutoZ);
	pFrame->putListLog(logStr);


	return;
	//----------------------------------------------------
	//----------------------------------------------------
	motor.MoveAxis(theApp.MainDlg->TITLE_MOTOR_Z, ABS, dMovePos, sysData.fMotorSpeed[theApp.MainDlg->TITLE_MOTOR_Z], sysData.fMotorAccTime[theApp.MainDlg->TITLE_MOTOR_Z]);
	double dCurPos;
	double ep = myTimer(true);
	while (1)
	{

		if (myTimer(true)-ep< 5000)
		{
			if ( motor.IsStopAxis(theApp.MainDlg->TITLE_MOTOR_Z))// && motor.GetInposition(theApp.MainDlg->TITLE_MOTOR_Z) )
			{
				dCurPos = motor.GetCommandPos(theApp.MainDlg->TITLE_MOTOR_Z);

				if (fabs(dCurPos-dMovePos)<=0.003)
				{
					logStr.Format("PCB Auto Z��ġ �̵� �Ϸ�");
					pFrame->putListLog(sLangChange);
					break;
				}
			}
		}
	}
}


void CCCDSFRDlg::OnBnClickedButtonImgSave2()
{
	char tmpStr[1000];
	CString FileName = _T("");
	CFile           savefile;
	CFileException  e;
	sprintf_s(tmpStr, "");
	CFileDialog* fileSaveDialog = NULL;
	fileSaveDialog = new CFileDialog(FALSE, "*.raw", _T(tmpStr),OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, "Image Files(*.raw)|*.raw|");
	if (fileSaveDialog)
	{
		if (fileSaveDialog->DoModal() == IDOK)
		{
			FileName = fileSaveDialog->GetPathName();
			if (!savefile.Open(FileName, CFile::modeCreate | CFile::modeWrite, &e))
			{
				AfxMessageBox("File open fail:" + e.m_cause);
			}
			savefile.Write(MIU.m_pFrameRawBuffer, gMIUDevice.nFrameImageSize);
			savefile.Close();
		}

		delete fileSaveDialog;
	}
	
}


void CCCDSFRDlg::OnBnClickedButtonCcdOpen()
{
	bool rtn = MIU.Open();
	if(rtn)
	{
		theApp.MainDlg->putListLog("MIU Open ����.");
	}else
	{
		theApp.MainDlg->putListLog("	MIU Open ����.");

	}
}


void CCCDSFRDlg::OnBnClickedButtonCcdStop()
{
	theApp.MainDlg->putListLog("		MIU Stop ����.");
	MIU.Stop();
	theApp.MainDlg->putListLog("MIU Stop �Ϸ�.");
}


void CCCDSFRDlg::OnBnClickedButtonCcdPlay()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	if(MIU.Start())
	{
		if (!bThreadCcmGrabRun)
		{
			pFrame->CcmThreadStart();
		}
		Sleep(100);
		gMIUDevice.CurrentState = 4;
		pFrame->putListLog("MIU Start ����.");
	}else
	{
		pFrame->putListLog("MIU Start ����.");
	}
}


void CCCDSFRDlg::OnBnClickedButtonCcdClose()
{
	theApp.MainDlg->putListLog("MIU Close ����.");
	if(MIU.Close())
	{
		theApp.MainDlg->putListLog("MIU Close �Ϸ�.");
	}else
	{
		theApp.MainDlg->putListLog("MIU Close ����.");
	}
	

	/*MIU.m_pBoard->CloseBoard();

	if (MIU.m_pFrameRawBuffer != NULL)
		delete MIU.m_pFrameRawBuffer;
	MIU.m_pFrameRawBuffer = NULL;

	if (MIU.m_pFrameBMPBuffer != NULL)
		delete MIU.m_pFrameBMPBuffer;
	MIU.m_pFrameBMPBuffer = NULL;*/
}


void CCCDSFRDlg::OnStnClickedEditSfrX()
{
	CString celData = "";
	GetDlgItem(IDC_EDIT_SFR_X)->GetWindowText(celData);
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);

	if(dlg.DoModal() == IDOK)
	{
		int _num = atoi(dlg.GetstrNum());

		if(_num < -1000)			_num = -1000;
		if(_num > 1000)				_num = 1000;

		celData.Format("%d", (_num));
		m_SFR_X.SetText(celData);
		curSfrPosX = _num;
	}
	GetDlgItemText(IDC_EDIT_SFR_X, celData);
	model.m_MTF_ROI_Pos[comBox_Index].x	= atoi(celData);
	model.Save();
	model.Load();
	celData.Format("SFR ROI X [%d]���� �Ϸ�" , model.m_MTF_ROI_Pos[comBox_Index].x);
	theApp.MainDlg->putListLog(celData);
}

void CCCDSFRDlg::OnStnClickedEditSfrY()
{
	CString celData = "";
	GetDlgItem(IDC_EDIT_SFR_Y)->GetWindowText(celData);
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);
	if(dlg.DoModal() == IDOK)
	{
		int _num = atoi(dlg.GetstrNum());

		if(_num < -1000)			_num = -1000;
		if(_num > 1000)				_num = 1000;

		celData.Format("%d", _num);
		m_SFR_Y.SetText(celData);
		curSfrPosY = _num;
	}
	GetDlgItemText(IDC_EDIT_SFR_Y, celData);
	model.m_MTF_ROI_Pos[comBox_Index].y	= atoi(celData);
	//
	model.Save();
	model.Load();
	celData.Format("SFR ROI Y [%d]���� �Ϸ�" , model.m_MTF_ROI_Pos[comBox_Index].y);
	theApp.MainDlg->putListLog(celData);
}


void CCCDSFRDlg::OnBnClickedBtnSfrposSave()
{
	CString sEditData;
	
	
	GetDlgItemText(IDC_EDIT_SFR_X, sEditData);
	model.m_MTF_ROI_Pos[comBox_Index].x	= atoi(sEditData);
	GetDlgItemText(IDC_EDIT_SFR_Y, sEditData);
	model.m_MTF_ROI_Pos[comBox_Index].y	= atoi(sEditData);

	GetDlgItemText(IDC_EDIT_SFR_DIC, sEditData);
	model.m_MTF_Direction[comBox_Index] = atoi(sEditData);
	//
	model.Save();
	model.Load();
	theApp.MainDlg->putListLog("modelData ���� �Ϸ�");
}


void CCCDSFRDlg::OnBnClickedButtonModelLoad()
{
	model.Load();
}


void CCCDSFRDlg::OnBnClickedUartOpen()
{

}


void CCCDSFRDlg::OnBnClickedUartClose()
{

}


void CCCDSFRDlg::OnBnClickedUartSend()
{
	theApp.MainDlg->uart_ThreadFn();
}


void CCCDSFRDlg::OnBnClickedUartSave()
{
	CString sEditData;

	GetDlgItemText(IDC_EDIT_UART1, sEditData);
	model.m_Uart_Range1 = atoi(sEditData);
	GetDlgItemText(IDC_EDIT_UART2, sEditData);
	model.m_Uart_Range2 = atoi(sEditData);
	//
	model.Save();
	model.Load();
	theApp.MainDlg->putListLog("modelData ���� �Ϸ�");
}

void CCCDSFRDlg::UartSet()
{
	CString sEditData;
	m_Uart_s.SetBkColor(M_COLOR_WHITE);
	m_Uart_e.SetBkColor(M_COLOR_WHITE);

	sEditData.Format("%d", model.m_Uart_Range1);
	SetDlgItemText(IDC_EDIT_UART1, sEditData);
	sEditData.Format("%d", model.m_Uart_Range2);
	SetDlgItemText(IDC_EDIT_UART2, sEditData);
	UpdateData(false);



	GetDlgItem(IDC_UART_OPEN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_UARTCOM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STR_UART1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STR_UART2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_UART1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_UART2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_UART_SAVE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_UART_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_UART_SEND)->ShowWindow(SW_HIDE);
	

}

void CCCDSFRDlg::OnStnClickedEditUart1()
{
	CString celData;
	GetDlgItem(IDC_EDIT_UART1)->GetWindowText(celData);
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);

	if (dlg.DoModal() == IDOK)
	{
		int _num = atoi(dlg.GetstrNum());

		if (_num < 0)			_num = 0;
		if (_num > 255)			_num = 255;

		celData.Format("%d", (_num));
		m_Uart_s.SetText(celData);
	}
}


void CCCDSFRDlg::OnStnClickedEditUart2()
{
	CString celData;
	GetDlgItem(IDC_EDIT_UART2)->GetWindowText(celData);
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);

	if (dlg.DoModal() == IDOK)
	{
		int _num = atoi(dlg.GetstrNum());

		if (_num < 0)			_num = 0;
		if (_num > 255)			_num = 255;

		celData.Format("%d", (_num));
		m_Uart_e.SetText(celData);
	}
}


void CCCDSFRDlg::OnBnClickedButtonModelLoad2()
{
	MIU.INI_LOAD();
}


void CCCDSFRDlg::OnBnClickedButtonRawLoad()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CFileDialog* pDlg = new CFileDialog(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY, _T("���� ���� ����(*.raw) | *.raw||"));
    if (pDlg != NULL)
    {
        if (pDlg->DoModal() == IDOK)
        {
            //FILE* fp;

            //unsigned char* pBuff = (unsigned char*)malloc(g_clLaonGrabberWrapper[m_nUnit].m_stMIUDevice.nFrameImageSize/*g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 2*/);
            //memset(pBuff, 0x00, g_clLaonGrabberWrapper[m_nUnit].m_stMIUDevice.nFrameImageSize/*(g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 2)*/);
            
            if (m_pImgBuff != NULL)
            {
                delete m_pImgBuff;
                m_pImgBuff = NULL;
            }

            //unsigned char* pBuff = NULL;
            CFile				ReadFile;
            CFileException		e;
            int					nFileLength;

            if (!ReadFile.Open(pDlg->GetPathName(), CFile::modeRead, &e))
            {
                e.ReportError(MB_OK);
                return;
            }
            nFileLength = ReadFile.GetLength();
            m_pImgBuff = new unsigned char[nFileLength];
            ReadFile.Read(m_pImgBuff, nFileLength);
            ReadFile.Close();

            //
            /// 
            //
            CCARawImageLoad((byte*)m_pImgBuff, MID_6500K_RAW);
        }
        delete pDlg;
    }
}

void CCCDSFRDlg::Raw_ImageSave(BYTE* rawImg, int index)
{
    SYSTEMTIME time;
    ::GetLocalTime(&time);

    CFileFind finder;
    BOOL IsFind;
    CString FolderName;
    CString rawName;
    //stainImg = Raw �̹���
    FolderName.Format("%s\\%s\\%04d%02d\\Image", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth);

    IsFind = finder.FindFile(FolderName);
    if (!IsFind)
    {
        FolderName.Format("%s", BASE_DIR);
        IsFind = finder.FindFile(FolderName);
        if (!IsFind)
        {
            CreateDirectory(FolderName, NULL);
        }

        FolderName.Format("%s", BASE_LOG_DIR);
        IsFind = finder.FindFile(FolderName);
        if (!IsFind)
        {
            CreateDirectory(FolderName, NULL);
        }

        FolderName.Format("%s\\%s", BASE_LOG_DIR, model.mCurModelName);
		IsFind = finder.FindFile(FolderName);
		if (!IsFind)
		{
			CreateDirectory(FolderName, NULL);
		}

        FolderName.Format("%s\\%s\\%04d%02d", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth);
        CreateDirectory(FolderName, NULL);
    }
    //
    CString imgName;
    if (index == MID_6500K_RAW)
    {
        imgName.Format("raw_6500k");
    }
    else if (index == MID_2800K_RAW)
    {
        imgName.Format("raw_2800k");
    }
    else if (index == LOW_LEVEL_RAW) {
        imgName.Format("raw_Dark");
    }
    SYSTEMTIME SysTime;
    ::GetLocalTime(&SysTime);

    finder.Close();

    CString		fileName;
    rawName.Format("%s\\%s_%s_%04d%02d%02d%02d%02d%02d.raw", FolderName, Task.ChipID, imgName, SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond);


    CFile savefile;
    CFileException  e;

    if (!savefile.Open(rawName, CFile::modeCreate | CFile::modeWrite, &e))
    {
        AfxMessageBox("File open fail:" + e.m_cause);
    }

    savefile.Write(rawImg, gMIUDevice.nFrameImageSize);//savefile.Write(gMIUDevice.image->imageData , gMIUDevice.nFrameImageSize);
    savefile.Close();


    CCARawImageLoad(MIU.m_pFrameRawBuffer, index, true);
}
void CCCDSFRDlg::CCARawImageLoad(BYTE* LoadImg, int index , bool autoRun)
{
    double dZoomX = 0.0;
    double dZoomY = 0.0;
    CString imgName;
    CString imgpath = "";
    dZoomX = ((double)SMALL_CCD_SIZE_X / (double)gMIUDevice.nWidth);
    dZoomY = ((double)SMALL_CCD_SIZE_Y / (double)gMIUDevice.nHeight);
	//gMIUDevice.dTDATASPEC_n;
    TDATASPEC tspec;
	tspec.eOutMode = gMIUDevice.dTDATASPEC_n.eOutMode;;
    tspec.eDataFormat = gMIUDevice.dTDATASPEC_n.eDataFormat;;
	tspec.eSensorType =  gMIUDevice.dTDATASPEC_n.eSensorType;;
	tspec.eDemosaicMethod = gMIUDevice.dTDATASPEC_n.eDemosaicMethod;
	
	int rSize = MIU.m_pBoard->GetFrameRawSize();
    memcpy(MIU.m_pFrameRawBuffer, LoadImg, rSize);

    if (index == MID_6500K_RAW)
    {
        imgName.Format("img6500k");
        memcpy(MIU.vDefectMidBuffer_6500K, LoadImg, MIU.m_pBoard->GetFrameRawSize());
    }
	else if (index == MID_2800K_RAW)
	{
		imgName.Format("img2800k");
		memcpy(MIU.vDefectMidBuffer_2800K, LoadImg, MIU.m_pBoard->GetFrameRawSize());
	}
	else if (index == CHART_RAW)
	{
		imgName.Format("ChartFirst");
		memcpy(MIU.vChartBuffet, LoadImg, MIU.m_pBoard->GetFrameRawSize());
	}
	else if (index == CHART_SECOND_RAW)
	{
		imgName.Format("ChartSecond");
		memcpy(MIU.vChart_Second_Buffet, LoadImg, MIU.m_pBoard->GetFrameRawSize());
	}
	
    else {
        imgName.Format("imgDark");
        memcpy(MIU.vDefectLowBuffer, LoadImg, MIU.m_pBoard->GetFrameRawSize());
    }

    /*
    BYTE* vDefectMidBuffer_6500K;	//�߰���� �̹��� (dark/Defect/Blemish/RI/Color Uniformity �����)
    BYTE* vDefectLowBuffer;			//���� ����(Hot Defect �����)
    BYTE* vDefectMidBuffer_2800K;	//�߰���� (Color Sensitivity �����)
    */
    if (autoRun == true)
    {
        return;
    }
   // imgpath.Format("%s.bmp", imgName);

   ACMISSoftISP::xMakeBMP(MIU.m_pFrameRawBuffer, (byte*)MIU.m_pFrameBMPBuffer,gMIUDevice.nWidth, gMIUDevice.nHeight, tspec);

    gMIUDevice.imageItp->imageData = (char*)MIU.m_pFrameBMPBuffer;
   // cvSaveImage(imgpath, gMIUDevice.imageItp);

    cvSplit(gMIUDevice.imageItp, MIU.pImageBuf[0], MIU.pImageBuf[1], MIU.pImageBuf[2], NULL);

    MbufPut(vision.MilGrabImageChild[3], MIU.pImageBuf[2]->imageData);
    MbufPut(vision.MilGrabImageChild[4], MIU.pImageBuf[1]->imageData);
    MbufPut(vision.MilGrabImageChild[5], MIU.pImageBuf[0]->imageData);
    //
    MimResize(vision.MilGrabImage[1], vision.MilSmallImage[1], dZoomX, dZoomY, M_DEFAULT);
	imgName.Empty();
	imgpath.Empty();
}

void CCCDSFRDlg::OnBnClickedButton6500kRawLoad()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CFileDialog* pDlg = new CFileDialog(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY, _T("���� ���� ����(*.raw) | *.raw||"));
    if (pDlg != NULL)
    {
        if (pDlg->DoModal() == IDOK)
        {
            //FILE* fp;

            //unsigned char* pBuff = (unsigned char*)malloc(g_clLaonGrabberWrapper[m_nUnit].m_stMIUDevice.nFrameImageSize/*g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 2*/);
            //memset(pBuff, 0x00, g_clLaonGrabberWrapper[m_nUnit].m_stMIUDevice.nFrameImageSize/*(g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 2)*/);

            if (m_pImgBuff != NULL)
            {
                delete m_pImgBuff;
                m_pImgBuff = NULL;
            }

            //unsigned char* pBuff = NULL;
            CFile				ReadFile;
            CFileException		e;
            int					nFileLength;

            if (!ReadFile.Open(pDlg->GetPathName(), CFile::modeRead, &e))
            {
                e.ReportError(MB_OK);
                return;
            }
            nFileLength = ReadFile.GetLength();
            m_pImgBuff = new unsigned char[nFileLength];
            ReadFile.Read(m_pImgBuff, nFileLength);
            ReadFile.Close();

            //
            /// 
            //
            CCARawImageLoad((byte*)m_pImgBuff, MID_6500K_RAW);
        }
        delete pDlg;
    }
}


void CCCDSFRDlg::OnBnClickedButton2800kRawLoad()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CFileDialog* pDlg = new CFileDialog(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY, _T("���� ���� ����(*.raw) | *.raw||"));
    if (pDlg != NULL)
    {
        if (pDlg->DoModal() == IDOK)
        {
            //FILE* fp;

            //unsigned char* pBuff = (unsigned char*)malloc(g_clLaonGrabberWrapper[m_nUnit].m_stMIUDevice.nFrameImageSize/*g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 2*/);
            //memset(pBuff, 0x00, g_clLaonGrabberWrapper[m_nUnit].m_stMIUDevice.nFrameImageSize/*(g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 2)*/);

            if (m_pImgBuff != NULL)
            {
                delete m_pImgBuff;
                m_pImgBuff = NULL;
            }

            //unsigned char* pBuff = NULL;
            CFile				ReadFile;
            CFileException		e;
            int					nFileLength;

            if (!ReadFile.Open(pDlg->GetPathName(), CFile::modeRead, &e))
            {
                e.ReportError(MB_OK);
                return;
            }
            nFileLength = ReadFile.GetLength();
            m_pImgBuff = new unsigned char[nFileLength];
            ReadFile.Read(m_pImgBuff, nFileLength);
            ReadFile.Close();

            //
            /// 
            //
            CCARawImageLoad((byte*)m_pImgBuff, MID_2800K_RAW);
        }
        delete pDlg;
    }
}


void CCCDSFRDlg::OnBnClickedButtonDarkRawLoad()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CFileDialog* pDlg = new CFileDialog(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY, _T("���� ���� ����(*.raw) | *.raw||"));
    if (pDlg != NULL)
    {
        if (pDlg->DoModal() == IDOK)
        {
            //FILE* fp;

            //unsigned char* pBuff = (unsigned char*)malloc(g_clLaonGrabberWrapper[m_nUnit].m_stMIUDevice.nFrameImageSize/*g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 2*/);
            //memset(pBuff, 0x00, g_clLaonGrabberWrapper[m_nUnit].m_stMIUDevice.nFrameImageSize/*(g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 2)*/);

            if (m_pImgBuff != NULL)
            {
                delete m_pImgBuff;
                m_pImgBuff = NULL;
            }

            //unsigned char* pBuff = NULL;
            CFile				ReadFile;
            CFileException		e;
            int					nFileLength;

            if (!ReadFile.Open(pDlg->GetPathName(), CFile::modeRead, &e))
            {
                e.ReportError(MB_OK);
                return;
            }
            nFileLength = ReadFile.GetLength();
            m_pImgBuff = new unsigned char[nFileLength];
            ReadFile.Read(m_pImgBuff, nFileLength);
            ReadFile.Close();

            //
            /// 
            //
            CCARawImageLoad((byte*)m_pImgBuff, LOW_LEVEL_RAW);
        }
        delete pDlg;
    }
}


void CCCDSFRDlg::OnBnClickedButtonCcdFovMarkRegist()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    if (Task.AutoFlag == 1)
    {
        sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
        delayMsg(sLangChange, 1000, M_COLOR_RED);
        return;
    }

    CString sTemp;
    sTemp.Format("FOV ��ũ ��ġ�� �����Ͻðڽ��ϱ� ? ");
    if (!askMsg(sTemp))return;

    RegistFovMark();
    vision.drawOverlay(CCD);
     
} 


void CCCDSFRDlg::OnBnClickedButtonCcdFovMarkView()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    if (Task.AutoFlag == 1)
    {
        sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
        delayMsg(sLangChange, 1000, M_COLOR_RED);
        return;
    }

    vision.m_FovSetMode = !vision.m_FovSetMode;
    //
    //
    //
    vision.m_SnrSetMode = false;
    vision.clearOverlay(CCD);
    if (vision.m_FovSetMode == true)
    {
        //g_pCarAABonderDlg->m_clVisionStaticCcd[m_nUnit].DrawRectFov(999);
        drawRectFOV(999);
    }
    else
    {
        drawRectSFR(999);
    }
}


void CCCDSFRDlg::OnBnClickedButtonCcdSnrMarkRegist()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    if (Task.AutoFlag == 1)
    {
        sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
        delayMsg(sLangChange, 1000, M_COLOR_RED);
        return;
    }

    CString sTemp;
    sTemp.Format("SNR ��ũ ��ġ�� �����Ͻðڽ��ϱ� ? ");
    if (!askMsg(sTemp))return;

    RegistSnrMark();

    theApp.MainDlg->putListLog("[SAVE] SNR ��ũ ��ġ ����");
}
//-----------------------------------------------------------------------------
//
//	�� �������� fov ������ ���ο� ����
//
//-----------------------------------------------------------------------------
void CCCDSFRDlg::SetFovRoi()
{
    int nCount;
    int i;
    for (i = 0; i < MAX_FOV_COUNT; i++)
    {
		m_clRectDrawFov[i] = model.sfrElem.m_clRectFov[i];
    }
}

void CCCDSFRDlg::SetSnrRoi()
{
    int nCount;
    int i;
    for (i = 0; i < MAX_SNR_COUNT; i++)
    {
		m_clRectDrawSnr[i] = model.sfrElem.m_clRectSnr[i];
    }
}
//-----------------------------------------------------------------------------
//
//	FOV DISTORTION ROTATION ��ũ ����
//
//-----------------------------------------------------------------------------
void CCCDSFRDlg::RegistFovMark()
{
    vision.clearOverlay(CCD);
    vision.MilBufferUpdate();	//ù�忡�� ������ Image�� ����� ���� �߻����� �ʱ� �ѹ� ���� ������

    int nPitch, nSizeX, nSizeY;
    int i;
    nPitch = (int)MbufInquire(vision.MilProcImageChild[4], M_PITCH, M_NULL);
    nSizeX = (int)MbufInquire(vision.MilProcImageChild[4], M_SIZE_X, M_NULL);
    nSizeY = (int)MbufInquire(vision.MilProcImageChild[4], M_SIZE_Y, M_NULL);

    //if (Task._findFovPos(vision.Image[4], nPitch, nSizeX, nSizeY, m_clRectFov) == false)
    //{
    //    // g_ShowMsgPopup(_T("ERROR"), _T("FiducialMark Find Fail!"), RGB_COLOR_RED);
    //    theApp.MainDlg->putListLog("FiducialMark Find Fail!");
    //    return;
    //}

    for (i = 0; i < MAX_FOV_COUNT; i++)
    {
        model.sfrElem.m_clPtFov[i] = Task.m_clPtFov[i];
        model.sfrElem.m_clRectFov[i] = m_clRectDrawFov[i];
    }

    // g_clModelData[m_nCamNo].Save(g_clSysData.m_szModelName);
    //model.save();
    model.RoiSave();
    model.RoiLoad();

    int markNumber = MAX_FOV_COUNT;         //������ 4�� , ������ 6��
    for (i = 0; i < markNumber; i++)
    {
        if (i > 2 && i < 6)continue;//��� 3�� ����
                                   
        vision.boxlist[CCD].addList(m_clRectDrawFov[i], PS_SOLID, M_COLOR_BLUE);
    }
    theApp.MainDlg->putListLog("[SAVE] FOV ��ũ ��ġ ����");
    vision.drawOverlay(CCD);
}
void CCCDSFRDlg::RegistSnrMark()
{
    int nPitch, nSizeX, nSizeY;
    int i;
    nPitch = (int)MbufInquire(vision.MilProcImageChild[4], M_PITCH, M_NULL);
    nSizeX = (int)MbufInquire(vision.MilProcImageChild[4], M_SIZE_X, M_NULL);
    nSizeY = (int)MbufInquire(vision.MilProcImageChild[4], M_SIZE_Y, M_NULL);

    for (i = 0; i < MAX_SNR_COUNT; i++)
    {
        model.sfrElem.m_clPtSnr[i] = Task.m_clPtSnr[i];
        model.sfrElem.m_clRectSnr[i] = m_clRectDrawSnr[i];

    }

    model.RoiSave();
    model.RoiLoad();

    vision.clearOverlay(CCD);

    char szTmp[256];
    for (i = 0; i < MAX_SNR_COUNT; i++)
    {
        vision.boxlist[CCD].addList(m_clRectDrawSnr[i], PS_SOLID, M_COLOR_BLUE);
        sprintf_s(szTmp, "%d", i);
        vision.textlist[CCD].addList(m_clRectDrawSnr[i].left + 10, m_clRectDrawSnr[i].top + 10, szTmp, M_COLOR_RED, 15, 10, "Arial");

    }
    theApp.MainDlg->putListLog("[SAVE] SNR ��ũ ��ġ ����");
    vision.drawOverlay(CCD);
}

void CCCDSFRDlg::OnBnClickedButtonCcdSnrRoiView()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    if (Task.AutoFlag == 1)
    {
        sLangChange.LoadStringA(IDS_STRING1368);	//�ڵ� ���� �� �Դϴ�.
        delayMsg(sLangChange, 1000, M_COLOR_RED);
        return;
    }
    vision.m_SnrSetMode = !vision.m_SnrSetMode;
    vision.m_FovSetMode = false;
    vision.clearOverlay(CCD);
    if (vision.m_SnrSetMode == true)
    {
        drawRectSNR(999);
    }
    else
    {
        drawRectSFR(999);
    }
    
}


void CCCDSFRDlg::OnBnClickedButtonZooxLinearMode()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//MIU.ISPmodeCheck();
	//MIU.SwitchLinearToHDRMode();

	//MIU.SwitchLinearToHDRMode();
	//MIU.SwitchHDRToLinearMode();

}


void CCCDSFRDlg::OnBnClickedButtonChartSecondRawLoad()
{
	// TODO: Add your control notification handler code here
	CFileDialog* pDlg = new CFileDialog(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY, _T("���� ���� ����(*.raw) | *.raw||"));
	if (pDlg != NULL)
	{
		if (pDlg->DoModal() == IDOK)
		{
			//FILE* fp;

			//unsigned char* pBuff = (unsigned char*)malloc(g_clLaonGrabberWrapper[m_nUnit].m_stMIUDevice.nFrameImageSize/*g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 2*/);
			//memset(pBuff, 0x00, g_clLaonGrabberWrapper[m_nUnit].m_stMIUDevice.nFrameImageSize/*(g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 2)*/);

			if (m_pImgBuff != NULL)
			{
				delete m_pImgBuff;
				m_pImgBuff = NULL;
			}

			//unsigned char* pBuff = NULL;
			CFile				ReadFile;
			CFileException		e;
			int					nFileLength;

			if (!ReadFile.Open(pDlg->GetPathName(), CFile::modeRead, &e))
			{
				e.ReportError(MB_OK);
				return;
			}
			nFileLength = ReadFile.GetLength();
			m_pImgBuff = new unsigned char[nFileLength];
			ReadFile.Read(m_pImgBuff, nFileLength);
			ReadFile.Close();

			//
			/// 
			//
			CCARawImageLoad((byte*)m_pImgBuff, CHART_SECOND_RAW);
		}
		delete pDlg;
	}
}


void CCCDSFRDlg::OnBnClickedButtonChartFirstRawLoad()
{
	// TODO: Add your control notification handler code here
	CFileDialog* pDlg = new CFileDialog(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY, _T("���� ���� ����(*.raw) | *.raw||"));
	if (pDlg != NULL)
	{
		if (pDlg->DoModal() == IDOK)
		{
			//FILE* fp;

			//unsigned char* pBuff = (unsigned char*)malloc(g_clLaonGrabberWrapper[m_nUnit].m_stMIUDevice.nFrameImageSize/*g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 2*/);
			//memset(pBuff, 0x00, g_clLaonGrabberWrapper[m_nUnit].m_stMIUDevice.nFrameImageSize/*(g_clModelData[m_nUnit].m_nWidth * g_clModelData[m_nUnit].m_nHeight * 2)*/);

			if (m_pImgBuff != NULL)
			{
				delete m_pImgBuff;
				m_pImgBuff = NULL;
			}

			//unsigned char* pBuff = NULL;
			CFile				ReadFile;
			CFileException		e;
			int					nFileLength;

			if (!ReadFile.Open(pDlg->GetPathName(), CFile::modeRead, &e))
			{
				e.ReportError(MB_OK);
				return;
			}
			nFileLength = ReadFile.GetLength();
			m_pImgBuff = new unsigned char[nFileLength];
			ReadFile.Read(m_pImgBuff, nFileLength);
			ReadFile.Close();

			//
			/// 
			//
			CCARawImageLoad((byte*)m_pImgBuff, CHART_RAW);
		}
		delete pDlg;
	}
}


void CCCDSFRDlg::OnStnClickedEditSfrDic()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString celData = "";
	GetDlgItem(IDC_EDIT_SFR_DIC)->GetWindowText(celData);
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);
	if (dlg.DoModal() == IDOK)
	{
		int _num = atoi(dlg.GetstrNum());

		if (_num < 1)			_num = 0;
		if (_num >= 1)			_num = 1;

		celData.Format("%d", _num);
		m_SFR_Dic.SetText(celData);
		curSfrPosY = _num;
	}
	GetDlgItemText(IDC_EDIT_SFR_DIC, celData);
	model.m_MTF_Direction[comBox_Index] = atoi(celData);
	//
	model.Save();
	model.Load();
	celData.Format("SFR ROI DIRECTION [%d]���� �Ϸ�", model.m_MTF_Direction[comBox_Index]);
	theApp.MainDlg->putListLog(celData);
}


void CCCDSFRDlg::OnBnClickedButtonChartSetView()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	chartSetDlg->ShowWindow(SW_SHOW);
}
