// LensEdgeDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LensEdgeDlg.h"
#include "afxdialogex.h"

#include "MarkSetDlg.h"


// CLensEdgeDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CLensEdgeDlg, CDialogEx)

CLensEdgeDlg::CLensEdgeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLensEdgeDlg::IDD, pParent)
{
	m_iSelMoveType	= 0;
	m_iSelMotor		= Motor_PCB_TH;
}

CLensEdgeDlg::~CLensEdgeDlg()
{
}

void CLensEdgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_EDGE_POS, m_iSelMoveType);
	DDX_Control(pDX, IDC_LABEL_EDGE_DISP5, m_labelDisp5);
	DDX_Control(pDX, IDC_LABEL_EDGE_DISP6, m_labelDisp6);
	DDX_Control(pDX, IDC_BUTTON_EDGE_MOTOR_ALIGN, m_btnAlignPos);
	DDX_Control(pDX, IDC_BUTTON_Z_MOTOR_ALIGN, m_btnAlignZPos);

	//
	DDX_Control(pDX, IDC_BUTTON_EDGE_MOTOR_PCBALIGN, m_btnPcbAlignPos);
	DDX_Control(pDX, IDC_BUTTON_Z_MOTOR_PCBALIGN, m_btnPcbAlignZPos);
	//
	DDX_Control(pDX, IDC_BTN_CAM_LASER_FOR, m_btnLaserCamFor);
	DDX_Control(pDX, IDC_BTN_CAM_LASER_BACK, m_btnLaserCamBack);
	//
	DDX_Control(pDX, IDC_BUTTON_EDGE_MOTOR_PCBLOAD, m_btnPcbLoadPos);
	DDX_Control(pDX, IDC_BUTTON_EDGE_Z_MOTOR_PCBLOAD, m_btnPcbLoadZPos);
	DDX_Control(pDX, IDC_BUTTON_EDGE_MOTOR_COVERLOAD, m_btnCoverLoadPos);
	DDX_Control(pDX, IDC_BUTTON_EDGE_Z_MOTOR_COVERLOAD, m_btnCoverLoadZPos);
	DDX_Control(pDX, IDC_BUTTON_EDGE_MOTOR_COVERUNLOAD, m_btnCoverUnLoadPos);
	DDX_Control(pDX, IDC_BUTTON_EDGE_Z_MOTOR_COVERUNLOAD, m_btnCoverUnLoadZPos);
	
	DDX_Control(pDX, IDC_BUTTON_EDGE_MOTOR_GIVEUP, m_btnGiveUpPos);
	DDX_Control(pDX, IDC_BUTTON_EDGE_MOTOR_SUPPLY, m_btnSupplyPos);
	DDX_Control(pDX, IDC_BUTTON_EDGE_PCB_COVER_OFF, m_btnCoverOff);
	DDX_Control(pDX, IDC_BUTTON_EDGE_PCB_COVER_ON, m_btnCoverOn);
	DDX_Control(pDX, IDC_BUTTON_EDGE_VACCUM_OFF, m_btnVaccumOff);
	DDX_Control(pDX, IDC_BUTTON_EDGE_VACCUM_ON, m_btnVaccumOn);
	DDX_Control(pDX, IDC_BUTTON_EDGE_MOTOR_WAIT, m_btnWaitPos);
	DDX_Control(pDX, IDC_BUTTON_EDGE_GOTO_LENSALIGN, m_btnPCB);
	DDX_Control(pDX, IDC_BUTTON_EDGE_GOTO_PCBALIGN, m_btnLens);
	DDX_Control(pDX, IDC_BUTTON_EDGE_MOTOR_LOADING_ALIGN, m_btnLoadingAlign);
	DDX_Control(pDX, IDC_BUTTON_EDGE_GOTO_LENSEDGE_ALIGN, m_btnLensEdge);
	DDX_Control(pDX, IDC_PCB_EDGE_ALIGN_RESULT, m_labelAlignResult);
	DDX_Control(pDX, IDC_BUTTON_EDGE_BOX_LEFT, m_btn_BoxLeft);
	DDX_Control(pDX, IDC_BUTTON_EDGE_BOX_UP, m_btn_BoxUP);
	DDX_Control(pDX, IDC_BUTTON_EDGE_BOX_RIGHT, m_btn_BoxRight);
	DDX_Control(pDX, IDC_BUTTON_EDGE_BOX_DOWN, m_btn_BoxDown);
	DDX_Control(pDX, IDC_BUTTON_EDGE_MOTOR_LOADING, m_btnLoading);

	DDX_Control(pDX, IDC_BUTTON_Z_MOTOR_UP2, m_btn_ZWaitePos);
	DDX_Control(pDX, IDC_BUTTON_Z_MOTOR_DOWN, m_btn_ZLoading);
	DDX_Control(pDX, IDC_BUTTON_Z_MOTOR_DOWN_BONDIng, m_btn_ZBonding);
	DDX_Control(pDX, IDC_BUTTON_EDGE_EPOXY_UV_ON, m_btn_UVOn);
	DDX_Control(pDX, IDC_BUTTON_EDGE_EPOXY_UV_OFF, m_btn_UVOff);
	DDX_Control(pDX, IDC_BUTTON_SMINI_UV_ON, m_btn_PcbUVOn);
	DDX_Control(pDX, IDC_BUTTON_SMINI_UV_OFF, m_btn_PcbUVOff);

	DDX_Control(pDX, IDC_BUTTON_LASER_CYL_UP_LENSTAB, m_btn_Laser_CylUp_LensTab);
	DDX_Control(pDX, IDC_BUTTON_LASER_CYL_DOWN_LENSTAB, m_btn_Laser_CylDown_LensTab);
	
	DDX_Control(pDX, IDC_BUTTON_MOTOR_CONTI_POS, m_btn_ContiDelay);
	DDX_Control(pDX, IDC_BUTTON_Z_MOTOR_CONTI_POS, m_btn_ZContiDelay);

	DDX_Control(pDX, IDC_STC_CONTI_DOWN_OFFSET, m_Conti_Down_Offset);
}


BEGIN_MESSAGE_MAP(CLensEdgeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLensEdgeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLensEdgeDlg::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_EDGE_LIVEMODE, &CLensEdgeDlg::OnBnClickedButtonLivemode)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_REGIST, &CLensEdgeDlg::OnBnClickedButtonRegist)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_FIND, &CLensEdgeDlg::OnBnClickedButtonFind)
	ON_BN_CLICKED(IDC_RADIO_EDGE_POS, &CLensEdgeDlg::OnBnClickedRadioPos)
	ON_BN_CLICKED(IDC_RADIO_EDGE_SIZE, &CLensEdgeDlg::OnBnClickedRadioSize)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_BOX_UP, &CLensEdgeDlg::OnBnClickedButtonBoxUp)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_BOX_DOWN, &CLensEdgeDlg::OnBnClickedButtonBoxDown)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_BOX_LEFT, &CLensEdgeDlg::OnBnClickedButtonBoxLeft)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_BOX_RIGHT, &CLensEdgeDlg::OnBnClickedButtonBoxRight)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_BOX_SPEED, &CLensEdgeDlg::OnBnClickedButtonBoxSpeed)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_MOTOR_SUPPLY, &CLensEdgeDlg::OnBnClickedButtonMotorSupply)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_MOTOR_ALIGN, &CLensEdgeDlg::OnBnClickedButtonMotorAlign)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_MOTOR_GIVEUP, &CLensEdgeDlg::OnBnClickedButtonMotorGiveUp)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_MARKSET, &CLensEdgeDlg::OnBnClickedButtonMarkset)
//	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_EDGE_IMG_LOAD, &CLensEdgeDlg::OnBnClickedButtonImgLoad)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_IMG_SAVE, &CLensEdgeDlg::OnBnClickedButtonImgSave)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_EDGE_VACCUM_ON, &CLensEdgeDlg::OnBnClickedButtonVaccumOn)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_VACCUM_OFF, &CLensEdgeDlg::OnBnClickedButtonVaccumOff)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_EDGE_MOTOR_WAIT, &CLensEdgeDlg::OnBnClickedButtonMotorWait)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_ALIGN_TEST2, &CLensEdgeDlg::OnBnClickedButtonAlignTest2)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_Menu_COPY, &CLensEdgeDlg::OnMenuCopy)
	ON_COMMAND(ID_Menu_DELETE, &CLensEdgeDlg::OnMenuDelete)
	ON_COMMAND(ID_Menu_PASTE, &CLensEdgeDlg::OnMenuPaste)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_PCB_COVER_ON, &CLensEdgeDlg::OnBnClickedButtonPcbCoverOn)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_PCB_COVER_OFF, &CLensEdgeDlg::OnBnClickedButtonPcbCoverOff)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_GOTO_PCBALIGN, &CLensEdgeDlg::OnBnClickedButtonGotoPcbalign)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_EDGE_GOTO_LENSEDGE_ALIGN, &CLensEdgeDlg::OnBnClickedButtonGotoLensalign)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_GOTO_LENSALIGN, &CLensEdgeDlg::OnBnClickedButtonEdgeGotoLensalign)
	ON_BN_CLICKED(IDC_BUTTON_Z_MOTOR_UP2, &CLensEdgeDlg::OnBnClickedButtonZMotorUp2)
	ON_BN_CLICKED(IDC_BUTTON_Z_MOTOR_DOWN, &CLensEdgeDlg::OnBnClickedButtonZMotorDown)
	ON_BN_CLICKED(IDC_BUTTON_Z_MOTOR_DOWN_BONDIng, &CLensEdgeDlg::OnBnClickedButtonZMotorDownBonding)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_MOTOR_LOADING_ALIGN, &CLensEdgeDlg::OnBnClickedButtonEdgeMotorLoadingAlign)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_EPOXY_UV_ON, &CLensEdgeDlg::OnBnClickedButtonEdgeEpoxyUvOn)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_EPOXY_UV_OFF, &CLensEdgeDlg::OnBnClickedButtonEdgeEpoxyUvOff)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_MOTOR_LOADING, &CLensEdgeDlg::OnBnClickedButtonEdgeMotorLoading)
	ON_BN_CLICKED(IDC_BUTTON_Z_MOTOR_ALIGN, &CLensEdgeDlg::OnBnClickedButtonZMotorAlign)
	ON_BN_CLICKED(IDC_BTN_CAM_LASER_FOR, &CLensEdgeDlg::OnBnClickedBtnCamLaserFor)
	ON_BN_CLICKED(IDC_BTN_CAM_LASER_BACK, &CLensEdgeDlg::OnBnClickedBtnCamLaserBack)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_MOTOR_PCBALIGN, &CLensEdgeDlg::OnBnClickedButtonEdgeMotorPcbalign)
	ON_BN_CLICKED(IDC_BUTTON_Z_MOTOR_PCBALIGN, &CLensEdgeDlg::OnBnClickedButtonZMotorPcbalign)
	ON_BN_CLICKED(IDC_BUTTON_Z_MOTOR_CONTI_POS, &CLensEdgeDlg::OnBnClickedButtonZMotorContiPos)
	ON_STN_CLICKED(IDC_STC_CONTI_DOWN_OFFSET, &CLensEdgeDlg::OnStnClickedStcContiDownOffset)
	ON_BN_CLICKED(IDC_BUTTON_MOTOR_CONTI_POS, &CLensEdgeDlg::OnBnClickedButtonMotorContiPos)
	ON_BN_CLICKED(IDC_BUTTON_LENS_LASER_POS1, &CLensEdgeDlg::OnBnClickedButtonLensLaserPos1)
	ON_BN_CLICKED(IDC_BUTTON_LASER_AUTO_INSP, &CLensEdgeDlg::OnBnClickedButtonLaserAutoInsp)
	ON_BN_CLICKED(IDC_BUTTON_LENS_LASER_POS2, &CLensEdgeDlg::OnBnClickedButtonLensLaserPos2)
	ON_BN_CLICKED(IDC_BUTTON_LENS_LASER_POS3, &CLensEdgeDlg::OnBnClickedButtonLensLaserPos3)
	ON_BN_CLICKED(IDC_BUTTON_LENS_LASER_POS4, &CLensEdgeDlg::OnBnClickedButtonLensLaserPos4)
	ON_BN_CLICKED(IDC_BUTTON_LASER_MANUAL_INSP, &CLensEdgeDlg::OnBnClickedButtonLaserManualInsp)
	ON_BN_CLICKED(IDC_BUTTON_SMINI_UV_ON, &CLensEdgeDlg::OnBnClickedButtonSminiUvOn)
	ON_BN_CLICKED(IDC_BUTTON_SMINI_UV_OFF, &CLensEdgeDlg::OnBnClickedButtonSminiUvOff)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_MOTOR_PCBLOAD, &CLensEdgeDlg::OnBnClickedButtonEdgeMotorPcbload)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_Z_MOTOR_PCBLOAD, &CLensEdgeDlg::OnBnClickedButtonEdgeZMotorPcbload)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_MOTOR_COVERLOAD, &CLensEdgeDlg::OnBnClickedButtonEdgeMotorCoverload)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_Z_MOTOR_COVERLOAD, &CLensEdgeDlg::OnBnClickedButtonEdgeZMotorCoverload)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_MOTOR_COVERUNLOAD, &CLensEdgeDlg::OnBnClickedButtonEdgeMotorCoverunload)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_Z_MOTOR_COVERUNLOAD, &CLensEdgeDlg::OnBnClickedButtonEdgeZMotorCoverunload)
	ON_BN_CLICKED(IDC_BUTTON_LASER_CYL_UP_LENSTAB, &CLensEdgeDlg::OnBnClickedButtonLaserCylUpLenstab)
	ON_BN_CLICKED(IDC_BUTTON_LASER_CYL_DOWN_LENSTAB, &CLensEdgeDlg::OnBnClickedButtonLaserCylDownLenstab)
END_MESSAGE_MAP()


// CLensEdgeDlg �޽��� ó�����Դϴ�.
BOOL CLensEdgeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	setInterface();
	//DigLedputData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CLensEdgeDlg::OnBnClickedOk(){}
void CLensEdgeDlg::OnBnClickedCancel(){}


void CLensEdgeDlg::setInterface()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	WINDOWPLACEMENT wndpl;
	this->GetWindowPlacement(&wndpl);

	//wndpl.rcNormalPosition = pFrame->m_rectCamDispPos2;
	//wndpl.rcNormalPosition.top -= SUB_DLG_OFFSET_Y-10;
	//wndpl.rcNormalPosition.bottom = MAIN_DLG_SIZE_Y-5 - 100;
	//
	/*wndpl.rcNormalPosition.left = pFrame->m_rectCamDispPos2.right+1;
	wndpl.rcNormalPosition.right = MAIN_DLG_SIZE_X;
	wndpl.rcNormalPosition.top = pFrame->m_rectCamDispPos1.top - SUB_DLG_OFFSET_Y+10;
	wndpl.rcNormalPosition.bottom = MAIN_DLG_SIZE_Y - 85;*/

	wndpl.rcNormalPosition = pFrame->m_rectCamDispPos2;
	wndpl.rcNormalPosition.right = pFrame->m_rectCamDispPos2.right + 100;
	wndpl.rcNormalPosition.left = pFrame->m_rectCamDispPos2.left - 10;
	wndpl.rcNormalPosition.top -= SUB_DLG_OFFSET_Y - 10;
	wndpl.rcNormalPosition.bottom = MAIN_DLG_SIZE_Y - 5 - 90;
	this->MoveWindow(&wndpl.rcNormalPosition);
#ifdef ON_LINE_VISION
	GetDlgItem(IDC_MARK_EDGE_DISP1)->GetWindowPlacement(&wndpl);
	m_iMarkDispSizeX = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
	m_iMarkDispSizeY = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;

	CWnd* pWnd;
	pWnd = (CWnd*)GetDlgItem(IDC_MARK_EDGE_DISP1);
	MdispSelectWindow(vision.MilMarkDisplay[LENS_Align_MARK][0], vision.MilMarkSmallImage[LENS_Align_MARK][0], pWnd->m_hWnd);
	pWnd = (CWnd*)GetDlgItem(IDC_MARK_EDGE_DISP2);
	MdispSelectWindow(vision.MilMarkDisplay[LENS_Align_MARK][1], vision.MilMarkSmallImage[LENS_Align_MARK][1], pWnd->m_hWnd);

	ctrlLiveButton();

	m_iMoveSpeed = 10;
	m_rMarkBox.left = 10;
	m_rMarkBox.top = 10;
	m_rMarkBox.right = CAM_SIZE_X - 10;
	m_rMarkBox.bottom = CAM_SIZE_Y - 10;	
	

	m_labelAlignResult.SetBkColor(M_COLOR_WHITE);

	/* m_iMarkNo �ʱ�ȭ */
	clickedMarkDisp(0);
#endif
	m_btnLensEdge.m_iStateBtn  =1;
	m_btnLensEdge.Invalidate();


	GetDlgItem(IDC_STC_CONTI_DOWN_OFFSET)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_Z_MOTOR_CONTI_POS)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_BUTTON_EDGE_MOTOR_PCBLOAD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_EDGE_Z_MOTOR_PCBLOAD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_EDGE_MOTOR_COVERLOAD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_EDGE_Z_MOTOR_COVERLOAD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_EDGE_MOTOR_COVERUNLOAD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_EDGE_Z_MOTOR_COVERUNLOAD)->ShowWindow(SW_HIDE);

#if  (____AA_WAY == LENS_TILT_AA)
	GetDlgItem(IDC_BTN_CAM_LASER_FOR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_CAM_LASER_BACK)->ShowWindow(SW_HIDE);
#else
	//GetDlgItem(IDC_BUTTON_Z_MOTOR_PCBALIGN)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_BUTTON_EDGE_MOTOR_PCBALIGN)->ShowWindow(SW_HIDE);
#endif
	


}

void CLensEdgeDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		if(Task.AutoFlag == 0)
			LightControlthird.ctrlLedVolume(LIGHT_PCB, model.m_iLedValue[LEDDATA_LENS]);

		/* Grid */
		initMarkGrid();

		ctrlLiveButton();

		dispMarkImage();
		dispData();
		//DigLedputData();
#ifdef ON_LINE_VISION
		vision.clearOverlay(CAM1);
		vision.drawOverlay(CAM1);
#endif
		pushErrCnt = coverErrCnt = vaccumErrCnt = 0;

		timerRunFlag = false;
		m_bCopyFlag = false;
		m_iCopyMarkNo = -1;

		SetTimer(PCB_TIMER, 100, NULL);
	}
	else
	{
		KillTimer(PCB_TIMER);

		CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
#ifdef ON_LINE_VISION
		vision.clearOverlay(CAM1);
		vision.drawOverlay(CAM1);
#endif
		pFrame->m_bMeasureDist = FALSE;
		pFrame->UpdateData(false);

		pFrame->m_rBox.left = 0;
		pFrame->m_rBox.top = 0;
		pFrame->m_rBox.right = 0;
		pFrame->m_rBox.bottom = 0;
	}
}


void CLensEdgeDlg::dispData()
{
	/*CString tmpStr;

	tmpStr.Format("%d", model.m_iLimitRate[LENS_Align_MARK][0]);
	m_gridMark.SetTextMatrix(1, 1, tmpStr);

	tmpStr.Format("%d", model.m_iLimitRate[LENS_Align_MARK][1]);
	m_gridMark.SetTextMatrix(1, 2, tmpStr);*/

}


void CLensEdgeDlg::getData()
{
	/*CString strTemp;
	strTemp = m_gridMark.GetTextMatrix(1, 1);
	model.m_iLimitRate[LENS_Align_MARK][0] = atoi(strTemp);

	strTemp = m_gridMark.GetTextMatrix(1, 2);
	model.m_iLimitRate[LENS_Align_MARK][1] = atoi(strTemp);*/
}


void CLensEdgeDlg::initMarkGrid()
{
	//CString strTemp;

	//m_gridMark.SetRows(2);
	//m_gridMark.SetCols(3);

	//m_gridMark.SetColAlignment(0, 4);
	//m_gridMark.SetColAlignment(1, 4);
	//m_gridMark.SetColAlignment(2, 4);

	//m_gridMark.SetColWidth(0,1200);//1450
	//m_gridMark.SetColWidth(1,950);//2350
	//m_gridMark.SetColWidth(2,950);//2350

	//sLangChange.LoadStringA(IDS_STRING761);	//Mark #1
	//m_gridMark.SetTextMatrix(0, 1, sLangChange);
	//sLangChange.LoadStringA(IDS_STRING762);	//Mark #2
	//m_gridMark.SetTextMatrix(0, 2, sLangChange);

	//sLangChange.LoadStringA(IDS_STRING767);	//Matching(%)
	//m_gridMark.SetTextMatrix(1, 0, sLangChange);

	//m_gridMark.SetRowHeight(1, 510);
}


void CLensEdgeDlg::ctrlLiveButton()
{
	if (vision.getLiveMode())
	{
		sLangChange.LoadStringA(IDS_STRING1464);	//������
		GetDlgItem(IDC_BUTTON_EDGE_LIVEMODE)->SetWindowText(sLangChange);
	}
	else
	{
		sLangChange.LoadStringA(IDS_STRING1465);	//��������
		GetDlgItem(IDC_BUTTON_EDGE_LIVEMODE)->SetWindowText(sLangChange);
	}
}

void CLensEdgeDlg::OnBnClickedButtonLivemode()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1463);
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	if(vision.getLiveMode())
		vision.setLiveMode(0);
	else
		vision.setLiveMode(1);

	ctrlLiveButton();
}

void CLensEdgeDlg::getMarkSize()
{
	m_iMarkSizeX = m_rMarkBox.right - m_rMarkBox.left;
	m_iMarkSizeY = m_rMarkBox.bottom - m_rMarkBox.top; 
}

void CLensEdgeDlg::OnBnClickedButtonRegist()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1463);
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}
	int CamNu = 0;
#if (____AA_WAY == PCB_TILT_AA)
	CamNu = CAM1; 
#elif (____AA_WAY == LENS_TILT_AA)
	CamNu = CAM1;
#endif

	if (vision.getLiveMode())
	{
		vision.getSnapImage(CamNu);
	}

	getMarkSize();

	bool bRtn = vision.markRegist(CamNu, LENS_Align_MARK, m_iMarkNo, m_rMarkBox);

	if (!bRtn)
	{
		sLangChange.LoadStringA(IDS_STRING1466);	//��ũ��� ����.
		::errMsg2(Task.AutoFlag, sLangChange);
		return;
	}

	CMarkSetDlg dlg(CamNu, LENS_Align_MARK, m_iMarkNo, m_iMarkSizeX, m_iMarkSizeY, true);

	double dZoomX = (double)MARK_SIZE_X/(double)m_iMarkSizeX;
	double dZoomY = (double)MARK_SIZE_Y/(double)m_iMarkSizeY;

	if (dlg.DoModal()==IDOK)
	{
#ifdef USE_GEOMETRIC
		MmodDraw(M_DEFAULT, vision.ModModel[LENS_Align_MARK][m_iMarkNo], vision.MilMarkImage[LENS_Align_MARK][m_iMarkNo], M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT);

		MmodInquire(vision.ModModel[LENS_Align_MARK][m_iMarkNo], M_DEFAULT, M_REFERENCE_X+M_TYPE_DOUBLE, &model.m_MarkCenter[LENS_Align_MARK][m_iMarkNo].x);
		MmodInquire(vision.ModModel[LENS_Align_MARK][m_iMarkNo], M_DEFAULT, M_REFERENCE_Y+M_TYPE_DOUBLE, &model.m_MarkCenter[LENS_Align_MARK][m_iMarkNo].y);
		MmodInquire(vision.ModModel[LENS_Align_MARK][m_iMarkNo], M_DEFAULT, M_REFERENCE_X+M_TYPE_DOUBLE, &model.m_MarkOrigin[LENS_Align_MARK][m_iMarkNo].x);
		MmodInquire(vision.ModModel[LENS_Align_MARK][m_iMarkNo], M_DEFAULT, M_REFERENCE_Y+M_TYPE_DOUBLE, &model.m_MarkOrigin[LENS_Align_MARK][m_iMarkNo].y);
#else
		MpatDraw(M_DEFAULT, vision.PatModel[LENS_Align_MARK][m_iMarkNo], vision.MilMarkImage[LENS_Align_MARK][m_iMarkNo], M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT);

		MpatInquire(vision.PatModel[LENS_Align_MARK][m_iMarkNo], M_CENTER_X, &model.m_MarkCenter[LENS_Align_MARK][m_iMarkNo].x);
		MpatInquire(vision.PatModel[LENS_Align_MARK][m_iMarkNo], M_CENTER_Y, &model.m_MarkCenter[LENS_Align_MARK][m_iMarkNo].y);
		MpatInquire(vision.PatModel[LENS_Align_MARK][m_iMarkNo], M_ORIGINAL_X, &model.m_MarkOrigin[LENS_Align_MARK][m_iMarkNo].x);
		MpatInquire(vision.PatModel[LENS_Align_MARK][m_iMarkNo], M_ORIGINAL_Y, &model.m_MarkOrigin[LENS_Align_MARK][m_iMarkNo].y);
#endif

		MimResize(vision.MilMarkImage[LENS_Align_MARK][m_iMarkNo], vision.MilMarkSmallImage[LENS_Align_MARK][m_iMarkNo], dZoomX, dZoomY, M_DEFAULT);

		model.m_MarkSize[LENS_Align_MARK][m_iMarkNo].x = m_iMarkSizeX;
		model.m_MarkSize[LENS_Align_MARK][m_iMarkNo].y = m_iMarkSizeY;

		vision.geometricMarkPreProc(CamNu, LENS_Align_MARK, m_iMarkNo);

		model.Save();
	}
}


void CLensEdgeDlg::OnBnClickedButtonFind()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1463);
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}
	int CamNu = 0;
#if (____AA_WAY == PCB_TILT_AA)
	CamNu = CAM1;
#elif (____AA_WAY == LENS_TILT_AA)
	CamNu = CAM1;
#endif

	int iMarkNo=0;
	CString sTemp="";

	vision.clearOverlay(CamNu);
	 
	if(vision.getLiveMode())
	{
		vision.getSnapImage(CamNu);
	}

	double ep1=0.0;
	MappTimer(M_TIMER_RESET, &ep1);
	iMarkNo = vision.findMark(CamNu, LENS_Align_MARK);
	double ep2=0.0;
	MappTimer(M_TIMER_READ, &ep2);

	sTemp.Format(" %.3lf sec", ep2 - ep1);
	vision.textlist[CamNu].addList((CAM_SIZE_X-140), (CAM_SIZE_Y-60), sTemp, M_COLOR_GREEN, 24, 10, "Arial");

	vision.drawOverlay(CamNu);

	if (iMarkNo!=-1)
	{
#ifdef USE_GEOMETRIC
		MgraColor(M_DEFAULT, M_COLOR_GREEN);
		MmodDraw(M_DEFAULT, vision.ModResult[LENS_Align_MARK][iMarkNo], vision.MilOverlayImage[CamNu], M_DRAW_BOX+M_DRAW_POSITION+M_DRAW_EDGES+M_DRAW_AXIS, M_DEFAULT, M_DEFAULT);
#endif
	}

	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	pFrame->m_rBox.left = 0;
	pFrame->m_rBox.top = 0;
	pFrame->m_rBox.right = 0;
	pFrame->m_rBox.bottom = 0;
}


void CLensEdgeDlg::OnBnClickedRadioPos()
{
	UpdateData(true);
}


void CLensEdgeDlg::OnBnClickedRadioSize()
{
	UpdateData(true);
}


void CLensEdgeDlg::OnBnClickedButtonBoxUp()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	if (m_iSelMoveType==0)
	{
		m_rMarkBox.top -= m_iMoveSpeed;
		m_rMarkBox.bottom -= m_iMoveSpeed;
	}
	/* ũ�� */
	else
	{
		m_rMarkBox.bottom -= m_iMoveSpeed;
	}

	if (m_rMarkBox.top<1)
	{
		m_rMarkBox.top = 1;
		m_rMarkBox.bottom = 1 + m_rcFixedBox.Height();
	}

	vision.clearOverlay(CAM1);
	vision.boxlist[CAM1].addList(m_rMarkBox, PS_DOT, M_COLOR_GREEN);
	vision.drawOverlay(CAM1);

	pFrame->m_rBox = m_rcFixedBox = m_rMarkBox;
}


void CLensEdgeDlg::OnBnClickedButtonBoxDown()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	/* �̵� */
	if (m_iSelMoveType==0)
	{
		m_rMarkBox.top += m_iMoveSpeed;
		m_rMarkBox.bottom += m_iMoveSpeed;
	}
	/* ũ�� */
	else
	{
		m_rMarkBox.bottom += m_iMoveSpeed;
	}

	if (m_rMarkBox.bottom>CAM_SIZE_Y-1)
	{
		m_rMarkBox.bottom	= CAM_SIZE_Y - 1;
		m_rMarkBox.top		= m_rMarkBox.bottom - m_rcFixedBox.Height();
	}

	vision.clearOverlay(CAM1);
	vision.boxlist[CAM1].addList(m_rMarkBox, PS_DOT, M_COLOR_GREEN);
	vision.drawOverlay(CAM1);

	pFrame->m_rBox = m_rcFixedBox = m_rMarkBox;
}


void CLensEdgeDlg::OnBnClickedButtonBoxLeft()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	/* �̵� */
	if (m_iSelMoveType==0)
	{
		m_rMarkBox.left -= m_iMoveSpeed;
		m_rMarkBox.right -= m_iMoveSpeed;
	}
	/* ũ�� */
	else
	{
		m_rMarkBox.right -= m_iMoveSpeed;
	}

	if (m_rMarkBox.left<1)
	{
		m_rMarkBox.left		= 1;
		m_rMarkBox.right	= 1 + m_rcFixedBox.Width();
	}

	vision.clearOverlay(CAM1);
	vision.boxlist[CAM1].addList(m_rMarkBox, PS_DOT, M_COLOR_GREEN);
	vision.drawOverlay(CAM1);

	pFrame->m_rBox = m_rcFixedBox = m_rMarkBox;
}


void CLensEdgeDlg::OnBnClickedButtonBoxRight()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	/* �̵� */
	if (m_iSelMoveType==0)
	{
		m_rMarkBox.left += m_iMoveSpeed;
		m_rMarkBox.right += m_iMoveSpeed;
	}
	/* ũ�� */
	else
	{
		m_rMarkBox.right += m_iMoveSpeed;
	}

	if (m_rMarkBox.right>CAM_SIZE_X-1)
	{
		m_rMarkBox.right	= CAM_SIZE_X - 1;
		m_rMarkBox.left		= m_rMarkBox.right - m_rcFixedBox.Width();
	}

	vision.clearOverlay(CAM1);
	vision.boxlist[CAM1
	
	].addList(m_rMarkBox, PS_DOT, M_COLOR_GREEN);
	vision.drawOverlay(CAM1);

	pFrame->m_rBox = m_rcFixedBox = m_rMarkBox;
}


void CLensEdgeDlg::OnBnClickedButtonBoxSpeed()
{
	CString strTemp;
	GetDlgItem(IDC_BUTTON_EDGE_BOX_SPEED)->GetWindowText(strTemp);

	if (strTemp=="10")
	{
		m_iMoveSpeed = 1;
		GetDlgItem(IDC_BUTTON_EDGE_BOX_SPEED)->SetWindowText("1");
	}
	else
	{
		m_iMoveSpeed = 10;
		GetDlgItem(IDC_BUTTON_EDGE_BOX_SPEED)->SetWindowText("10");
	}
}


void CLensEdgeDlg::dispMarkImage()
{
	double dZoomX, dZoomY;
#ifdef ON_LINE_VISION
	for (int i=0 ; i<2 ; i++)
	{
		MbufClear(vision.MilMarkImage[LENS_Align_MARK][i], 0);
		MbufClear(vision.MilMarkSmallImage[LENS_Align_MARK][i], 0);


		if (model.m_MarkSize[LENS_Align_MARK][i].x<=0 ||model.m_MarkSize[LENS_Align_MARK][i].y<=0)
			continue;

		dZoomX = (double)MARK_SIZE_X/(double)model.m_MarkSize[LENS_Align_MARK][i].x;
		dZoomY = (double)MARK_SIZE_Y/(double)model.m_MarkSize[LENS_Align_MARK][i].y;

#ifdef USE_GEOMETRIC
		if (vision.ModModel[LENS_Align_MARK][i]==M_NULL)	continue;

		MmodDraw(M_DEFAULT, vision.ModModel[LENS_Align_MARK][i], vision.MilMarkImage[LENS_Align_MARK][i], M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT);
#else
		if (vision.PatModel[LENS_Align_MARK][i]==M_NULL)	continue;

		MpatDraw(M_DEFAULT, vision.PatModel[LENS_Align_MARK][i], vision.MilMarkImage[LENS_Align_MARK][i], M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT);
#endif

		MimResize(vision.MilMarkImage[LENS_Align_MARK][i], vision.MilMarkSmallImage[LENS_Align_MARK][i], dZoomX, dZoomY, M_DEFAULT);
	}
#endif
}

void CLensEdgeDlg::OnBnClickedButtonMarkset()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1463);
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	int iMarkSizeX = model.m_MarkSize[LENS_Align_MARK][m_iMarkNo].x;
	int iMarkSizeY = model.m_MarkSize[LENS_Align_MARK][m_iMarkNo].y;

#ifdef USE_GEOMETRIC
	MmodDraw(M_DEFAULT, vision.ModModel[LENS_Align_MARK][m_iMarkNo], vision.MilTempImage, M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT);
#else
	MpatDraw(M_DEFAULT, vision.PatModel[LENS_Align_MARK][m_iMarkNo], vision.MilTempImage, M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT);
#endif

	CMarkSetDlg dlg(CAM1, LENS_Align_MARK, m_iMarkNo, iMarkSizeX, iMarkSizeY, false);

	double dZoomX = (double)MARK_SIZE_X/(double)iMarkSizeX;
	double dZoomY = (double)MARK_SIZE_Y/(double)iMarkSizeY;

	if (dlg.DoModal()==IDOK)
	{
#ifdef USE_GEOMETRIC
		MmodInquire(vision.ModModel[LENS_Align_MARK][m_iMarkNo], M_DEFAULT, M_REFERENCE_X+M_TYPE_DOUBLE, &model.m_MarkCenter[LENS_Align_MARK][m_iMarkNo].x);
		MmodInquire(vision.ModModel[LENS_Align_MARK][m_iMarkNo], M_DEFAULT, M_REFERENCE_Y+M_TYPE_DOUBLE, &model.m_MarkCenter[LENS_Align_MARK][m_iMarkNo].y);
		MmodInquire(vision.ModModel[LENS_Align_MARK][m_iMarkNo], M_DEFAULT, M_REFERENCE_X+M_TYPE_DOUBLE, &model.m_MarkOrigin[LENS_Align_MARK][m_iMarkNo].x);
		MmodInquire(vision.ModModel[LENS_Align_MARK][m_iMarkNo], M_DEFAULT, M_REFERENCE_Y+M_TYPE_DOUBLE, &model.m_MarkOrigin[LENS_Align_MARK][m_iMarkNo].y);
#else								   
		MpatInquire(vision.PatModel[LENS_Align_MARK][m_iMarkNo], M_CENTER_X, &model.m_MarkCenter[LENS_Align_MARK][m_iMarkNo].x);
		MpatInquire(vision.PatModel[LENS_Align_MARK][m_iMarkNo], M_CENTER_Y, &model.m_MarkCenter[LENS_Align_MARK][m_iMarkNo].y);
		MpatInquire(vision.PatModel[LENS_Align_MARK][m_iMarkNo], M_ORIGINAL_X, &model.m_MarkOrigin[LENS_Align_MARK][m_iMarkNo].x);
		MpatInquire(vision.PatModel[LENS_Align_MARK][m_iMarkNo], M_ORIGINAL_Y, &model.m_MarkOrigin[LENS_Align_MARK][m_iMarkNo].y);
#endif
		model.m_MarkSize[LENS_Align_MARK][m_iMarkNo].x = iMarkSizeX;
		model.m_MarkSize[LENS_Align_MARK][m_iMarkNo].y = iMarkSizeY;

		model.Save();
	}
}



void CLensEdgeDlg::OnBnClickedButtonImgLoad()
{
	GetDlgItem(IDC_BTN_CAM_LASER_FOR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_CAM_LASER_BACK)->ShowWindow(SW_SHOW);
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1463);
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	vision.loadImageonExplorer(CAM1);

	ctrlLiveButton();

	OnBnClickedButtonFind();
}


void CLensEdgeDlg::OnBnClickedButtonImgSave()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1463);
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}

	int liveMode = vision.getLiveMode();

	vision.saveImageonExplorer(CAM1);

	vision.setLiveMode(liveMode);
}


void CLensEdgeDlg::clickedMarkDisp(int iMarkNo)
{
	if (iMarkNo==0)
	{
		m_labelDisp5.SetBkColor(M_COLOR_LIGHT_BLUE);
		m_labelDisp6.SetBkColor(M_COLOR_LIGHT_GRAY);

		m_iMarkNo = 0;
	}
	else
	{
		m_labelDisp5.SetBkColor(M_COLOR_LIGHT_GRAY);
		m_labelDisp6.SetBkColor(M_COLOR_LIGHT_BLUE);

		m_iMarkNo = 1;
	}

	m_labelDisp5.Invalidate();
	GetDlgItem(IDC_MARK_EDGE_DISP1)->Invalidate();
	m_labelDisp6.Invalidate();
	GetDlgItem(IDC_MARK_EDGE_DISP2)->Invalidate();
}

void CLensEdgeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	WINDOWPLACEMENT wndpl1, wndpl2;
	CRect rTemp1, rTemp2;

	m_labelDisp5.GetWindowPlacement(&wndpl1);
	m_labelDisp6.GetWindowPlacement(&wndpl2);

	rTemp1 = wndpl1.rcNormalPosition;
	rTemp2 = wndpl2.rcNormalPosition;

	if (PtInRect(rTemp1, point))
		clickedMarkDisp(0);
	if (PtInRect(rTemp2, point))
		clickedMarkDisp(1);

	CDialogEx::OnLButtonDown(nFlags, point);
}



void CLensEdgeDlg::OnBnClickedButtonVaccumOn()
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

	//KYH DI/O �߰� �ʿ�
	//Dio.PCBvaccumOn(VACCUM_ON, false);
}


void CLensEdgeDlg::OnBnClickedButtonVaccumOff()
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

	//KYH DI/O �߰� �ʿ�
	//Dio.PCBvaccumOn(VACCUM_OFF, false);
}


BEGIN_EVENTSINK_MAP(CLensEdgeDlg, CDialogEx)

END_EVENTSINK_MAP()




void CLensEdgeDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==PCB_TIMER)
	{
		if (!timerRunFlag)
		{
			timerRunFlag = true;

			changeColorMotorPosBtn();

			// IO ��ư �߰�..
			changeColorLaserCamBtn();
			changeColorGripBtn();
			changeColorUVBtn();
			changeColorLaserCylinderLensTabBtn();
			timerRunFlag = false;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CLensEdgeDlg::OnBnClickedButtonAlignTest2()
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

	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	int CamNu = 0;
	int MotoX = 0;
	int MotoY = 0;
	int  iRtn = 0;
	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;
#if (____AA_WAY == PCB_TILT_AA)
	CamNu = CAM1;
	MotoX = Motor_Lens_X;
	MotoY = Motor_Lens_Y;
	iRtn = pFrame->procCamAlign(CamNu, LENS_Align_MARK, true, offsetX, offsetY,offsetTh);
	offsetX *= 1;
	offsetY *= 1;
#elif (____AA_WAY == LENS_TILT_AA)
	CamNu = CAM1; 
	MotoX = Motor_PCB_X;
	MotoY = Motor_PCB_Y;
	iRtn = pFrame->procCamAlign(CamNu, LENS_Align_MARK, true,  offsetX, offsetY,offsetTh);
#endif

	int iRtnVal=0;
	if (iRtn == 0)
	{
		iRtnVal = pFrame->AlignLimitCheck(CamNu,  offsetX, offsetY,0);

		if(iRtnVal==0)
		{
			sLangChange.LoadStringA(IDS_STRING1263);	//������ �̵� ������ Limit �ʰ� �߽��ϴ�.
			errMsg2(Task.AutoFlag, sLangChange);
		}
		else
		{
			short axis[3]; 
			double pos[3];

			axis[0] = MotoX;
			axis[1] = MotoY;
			//axis[2] = Motor_PCB_TH;

			pos[0]	= motor.GetEncoderPos(MotoX)	- offsetX;
			pos[1]	= motor.GetEncoderPos(MotoY)	- offsetY;
			//pos[2]	= motor.GetCommandPos(Motor_PCB_TH);
			motor.goMotorPos(2, axis, pos);
			sLangChange.LoadStringA(IDS_STRING1261);	//���� �̵� �Ϸ�.
			delayMsg(sLangChange, 3000, M_COLOR_GREEN);
			Sleep(10);

			pos[0]	= (pos[0] - model.axis[MotoX].pos[Lens_Pos]);
			pos[1]	= (pos[1] - model.axis[MotoY].pos[Lens_Pos]);
			//pos[2]	= (pos[2] - model.axis[Motor_Lens_TH].pos[Lens_Pos]);

			Task.d_Align_offset_x[LENS_Align_MARK] = pos[0];
			Task.d_Align_offset_y[LENS_Align_MARK] = pos[1];
			Task.d_Align_offset_th[LENS_Align_MARK] = 0;//+= offsetTh;	//Lens�� ������ Theta�� ����

			CString sTemp;
			sLangChange.LoadStringA(IDS_STRING1482);	//������ : %.3lf %.3lf %.3lf
			sTemp.Format(sLangChange, 
				Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
			m_labelAlignResult.SetText(sTemp);
			m_labelAlignResult.Invalidate();

			double etime = myTimer(true);
			while(1)
			{
				if(motor.IsStopAxis(MotoX)  && motor.IsStopAxis(MotoY))
				{
					Sleep(20);
					break;
				}
				if((myTimer(true) - etime) > 3000)
				{
					sLangChange.LoadStringA(IDS_STRING1260);	//���� ��ġ �̵� ����
					delayMsg(sLangChange, 3000, M_COLOR_RED);
					break;
				}
			}
		}
	}
	else
	{
		sLangChange.LoadStringA(IDS_STRING1215);	//��ũ �ν� ����.
		errMsg2(Task.AutoFlag, sLangChange);
	}

	CString sLog="";
	sLog.Format("Lens Align ������ X:%.03f, Y:%.03f, ��:%.04f", offsetX, offsetY, 0.000);
	pFrame->putListLog(sLog);

	pFrame->m_rBox.left = 0;
	pFrame->m_rBox.top = 0;
	pFrame->m_rBox.right = 0;
	pFrame->m_rBox.bottom = 0;
}


void CLensEdgeDlg::OnBnClickedButtonMotorWait()
{
	if( !func_TaskMovingCheck() )	return;

	
	//if( !func_MovingInterlock() )	return;
	g_bMovingflag = true;

	CString sTmpLog="";
	if(!motor.LENS_Z_Motor_Move(Wait_Pos))
	{
		sLangChange.Format("LENS Z axis Wait_Pos Move Failed");
		delayMsg(sLangChange, 1000, M_COLOR_RED);
	}
	if(!motor.Lens_Tilt_Motor_Move(Wait_Pos))
	{
		sLangChange.LoadStringA(IDS_STRING648);	//Lens %s �̵� ����
		sTmpLog.Format(sLangChange, MotorPosName[Wait_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag =false;
		return;
	}

	if(motor.Lens_Motor_MoveXY(Wait_Pos))  
	{
		
		sLangChange.LoadStringA(IDS_STRING649);	//Lens %s �̵� �Ϸ�
		sTmpLog.Format(sLangChange, MotorPosName[Wait_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_GREEN);
	}

	g_bMovingflag =false;
}


void CLensEdgeDlg::OnBnClickedButtonMotorSupply()
{
	CString sTmpLog = "";
	if( !func_TaskMovingCheck() )	return;
	if (!func_MovingInterlock())	return;

	

	g_bMovingflag =true;
	if(!motor.Lens_Tilt_Motor_Move(Loading_Pos))
	{
		sLangChange.LoadStringA(IDS_STRING648);	//Lens %s �̵� ����
		sTmpLog.Format(sLangChange, MotorPosName[Loading_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag =false;
		return;
	}

	if(!motor.Lens_Motor_MoveXY(Loading_Pos))
	{
		sTmpLog.Format("Lens %s �̵� ����", MotorPosName[Loading_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag =false;
		return;
	}else{
		sTmpLog.Format("Lens %s �̵� �Ϸ�", MotorPosName[Loading_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_GREEN);
	}
	g_bMovingflag =false;
}

void CLensEdgeDlg::OnBnClickedButtonMotorAlign()
{
	if( !func_TaskMovingCheck() )	return;
	if( !func_MovingInterlock() )	return;
	g_bMovingflag =true;
#if (____AA_WAY == PCB_TILT_AA)
	//PCB Z�� Ȯ����
	if(motor.Lens_Motor_MoveXY(Lens_Pos)) 
	{
		CString sTmpLog;
		sTmpLog.Format("Lens Align ��ġ �̵�");
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_GREEN);
	}
#elif (____AA_WAY == LENS_TILT_AA)
	if (!motor.PCB_Z_Motor_Move(Wait_Pos))
	{
		sLangChange.Format("PCB Z axis Wait_Pos Move Failed");
		delayMsg(sLangChange, 1000, M_COLOR_RED);
	}
	if (motor.Pcb_Motor_Move(Lens_Pos))
	{
		sTempLang.Format("PCB�� %s �̵� �Ϸ�", MotorPosName[Lens_Pos]);
		delayMsg(sTempLang.GetBuffer(99), 3000, M_COLOR_GREEN);
	}
	//delayMsg("���� ƿƮ�� �̻�� ��ġ�Դϴ�.", 2000, M_COLOR_RED);
	//return;
#endif

	g_bMovingflag =false;
}

void CLensEdgeDlg::OnBnClickedButtonMotorGiveUp()
{
	CString sTmpLog="";
	if( !func_TaskMovingCheck() )	return;
	if( !func_MovingInterlock() )	return;

	g_bMovingflag =true;
	//if(!motor.Lens_Tilt_Motor_Move(Bonding_Pos))
	//{
	//	sLangChange.LoadStringA(IDS_STRING648);	//Lens %s �̵� ����
	//	sTmpLog.Format(sLangChange, MotorPosName[Bonding_Pos]); 
	//	delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
	//	g_bMovingflag =false;
	//	return;
	//}
#if 0	
	double lensCurrentPosY = 0.0;
	lensCurrentPosY = (float)motor.GetEncoderPos(Motor_Lens_Y);
	if (lensCurrentPosY < (float)model.axis[Motor_Lens_Y].pos[Bonding_Pos])
	{
		sTmpLog.Format("Lens XY ��� ��ġ �̵��� ���� �ٶ��ϴ�.");
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag = false;
		return;
	}
#endif
	//pcb �� ������ġ���� üũ�ؼ� lens z�� ȸ���ؼ� ������ġ �̵�
	/*double pcbPosX = (float)model.axis[Motor_PCB_X].pos[Bonding_Pos] - (float)motor.GetEncoderPos(Motor_PCB_X);
	double pcbPosY = (float)model.axis[Motor_PCB_Y].pos[Bonding_Pos] - (float)motor.GetEncoderPos(Motor_PCB_Y);
	if (fabs(pcbPosX) < 2.1 && fabs(pcbPosY) < 2.1)
	{
		if (askMsg("pcb x,y ������ġ�Դϴ�. \nLens Z�� ȸ���� �̵��Ͻðڽ��ϱ�?") == IDOK)
		{
			if (!motor.LENS_Z_Motor_Move(Wait_Pos , true))
			{
				sTmpLog.Format("Lens Z ȸ����ġ �̵� ����");
				delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
				g_bMovingflag = false;
				return;
			}
		}
	}*/

	if(!motor.Lens_Motor_MoveXY(Bonding_Pos))
	{
		sTmpLog.Format("Lens %s �̵� ����", MotorPosName[Bonding_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag =false;
		return;
	}else
	{
		sTmpLog.Format("Lens %s �̵� �Ϸ�", MotorPosName[Bonding_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_GREEN);
	}
	g_bMovingflag =false;
}



void CLensEdgeDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CRect rcTemp1, rcTemp2;
	WINDOWPLACEMENT wndpl1, wndpl2;
	GetDlgItem(IDC_MARK_EDGE_DISP1)->GetWindowPlacement(&wndpl1);
	GetDlgItem(IDC_MARK_EDGE_DISP2)->GetWindowPlacement(&wndpl2);
	rcTemp1 = wndpl1.rcNormalPosition;
	rcTemp2 = wndpl2.rcNormalPosition;

	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	CPoint p = point;
	p.x -= pFrame->m_rectCamDispPos2.left;
	p.y -= pFrame->m_rectCamDispPos2.top + SUB_DLG_OFFSET_Y;

	if (PtInRect(rcTemp1, p))
		clickedMarkDisp(0);
	else if (PtInRect(rcTemp2, p))
		clickedMarkDisp(1);
	else
		return;

	CMenu* pMenu = NULL;
	CMenu* pSubMenu= NULL;

	pMenu = new CMenu();
	pMenu->LoadMenu(IDR_MENU1);
	pSubMenu = pMenu->GetSubMenu(0);

	if (m_bCopyFlag)	pSubMenu->EnableMenuItem(ID_Menu_PASTE, MF_ENABLED);
	else				pSubMenu->EnableMenuItem(ID_Menu_PASTE, MF_GRAYED);

	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CLensEdgeDlg::OnMenuCopy()
{
	m_bCopyFlag = true;

	m_iCopyMarkNo = m_iMarkNo;
}

void CLensEdgeDlg::OnMenuDelete()
{
	model.m_MarkCenter[LENS_Align_MARK][m_iMarkNo].x	=	0.0f;
	model.m_MarkCenter[LENS_Align_MARK][m_iMarkNo].y	=	0.0f;
	model.m_MarkSize[LENS_Align_MARK][m_iMarkNo].x		=	0;
	model.m_MarkSize[LENS_Align_MARK][m_iMarkNo].y		=	0;
	model.m_iLimitRate[LENS_Align_MARK][m_iMarkNo]		=	70;

	CString sDelFile;

#ifdef USE_GEOMETRIC
	MmodFree(vision.ModModel[LENS_Align_MARK][m_iMarkNo]);
	vision.ModModel[LENS_Align_MARK][m_iMarkNo] = M_NULL;

	sDelFile.Format("%s\\%s\\Data\\Mark\\LENS_Align_MARK_%d.mod", AA_MODEL_LOAD_DIR, model.mCurModelName, m_iMarkNo);
#else
	MpatFree(vision.PatModel[LENS_Align_MARK][m_iMarkNo]);
	vision.PatModel[LENS_Align_MARK][m_iMarkNo] = M_NULL;

	sDelFile.Format("%s\\Model\\%s\\LENS_Align_MARK_%d.pat", DATA_DIR, model.name, m_iMarkNo);
#endif

	::DeleteFile(sDelFile);

	model.Save();
	//	model.Load();

	dispMarkImage();
}


void CLensEdgeDlg::OnMenuPaste()
{
	m_bCopyFlag =false;

#ifdef USE_GEOMETRIC
	if (vision.ModModel[LENS_Align_MARK][m_iCopyMarkNo]==NULL)
		return;
#else
	if (vision.PatModel[LENS_Align_MARK][m_iCopyMarkNo]==NULL)
		return;
#endif

	model.m_MarkCenter[LENS_Align_MARK][m_iMarkNo].x	=	model.m_MarkCenter[LENS_Align_MARK][m_iCopyMarkNo].x;
	model.m_MarkCenter[LENS_Align_MARK][m_iMarkNo].y	=	model.m_MarkCenter[LENS_Align_MARK][m_iCopyMarkNo].y;
	model.m_MarkSize[LENS_Align_MARK][m_iMarkNo]		=	model.m_MarkSize[LENS_Align_MARK][m_iCopyMarkNo];
	model.m_iLimitRate[LENS_Align_MARK][m_iMarkNo]	=	model.m_iLimitRate[LENS_Align_MARK][m_iCopyMarkNo];

	CString sScr, sDes;
#ifdef USE_GEOMETRIC
	sScr.Format("%s\\%s\\Data\\Mark\\LENS_Align_MARK_%d.mod", AA_MODEL_LOAD_DIR, model.mCurModelName, m_iCopyMarkNo);
	sDes.Format("%s\\%s\\Data\\Mark\\LENS_Align_MARK_%d.mod", AA_MODEL_LOAD_DIR, model.mCurModelName, m_iMarkNo);
#else
	sScr.Format("%s\\%s\\Data\\Mark\\LENS_Align_MARK_%d.pat", AA_MODEL_LOAD_DIR, model.name, m_iCopyMarkNo);
	sDes.Format("%s\\%s\\Data\\Mark\\LENS_Align_MARK_%d.pat", AA_MODEL_LOAD_DIR, model.name, m_iMarkNo);
#endif

	::CopyFile(sScr, sDes, FALSE);

	model.loadMark(CAM1, LENS_Align_MARK, m_iMarkNo);
	model.Save();

	vision.geometricMarkPreProc(CAM1, LENS_Align_MARK, m_iMarkNo);

	dispMarkImage();
}


void CLensEdgeDlg::changeColorVaccumBtn()
{
	//int moduleNo = 0;
	//int channelNo = 1;

	//bool bOutVaccumFlag, bOutBlowFlag, bInVaccumOnFlag;			// �� �� OFF �� ����, Vaccum�� ��� Ż��

	//bInVaccumOnFlag = Dio.PCBvaccumOnCheck(true, false);


	//bOutVaccumFlag = (curOutDioByte[moduleNo][channelNo] & DIO_OUT_PCB_VACUUM) ? true : false;
	//bOutBlowFlag = (curOutDioByte[moduleNo][channelNo] & DIO_OUT_PCB_VACUUM_OFF) ? true : false;


	//if(!bOutVaccumFlag && !bOutBlowFlag)
	//{
	//	if(bInVaccumOnFlag)
	//	{
	//		vaccumErrCnt = 0;

	//		m_btnVaccumOn.m_iStateBtn	= 1;
	//		m_btnVaccumOff.m_iStateBtn	= 0;
	//	}
	//	else
	//	{
	//		if(vaccumErrCnt<5)
	//			m_btnVaccumOn.m_iStateBtn	= 0;
	//		else 
	//			m_btnVaccumOn.m_iStateBtn	= 2;
	//	
	//		if(vaccumErrCnt >=10)
	//			vaccumErrCnt = 0;

	//		vaccumErrCnt++;
	//		m_btnVaccumOff.m_iStateBtn	= 0;
	//	}
	//}


	//else if (bInVaccumOnFlag)
	//{
	//	m_btnVaccumOn.m_iStateBtn	= 1;
	//	m_btnVaccumOff.m_iStateBtn	= 0;
	//}
	//else
	//{
	//	m_btnVaccumOn.m_iStateBtn	= 0;
	//	m_btnVaccumOff.m_iStateBtn	= 1;
	//}

	//m_btnVaccumOn.Invalidate();
	//m_btnVaccumOff.Invalidate();
}

bool CLensEdgeDlg::checkCurMotorPos(int iPos)
{
	double dCurPos;
	double dMotorPos;

	for (int i=Motor_Lens_X ; i<=Motor_Lens_Y; i++)
	{
		dCurPos		= motor.GetEncoderPos(i);	//motor.GetCommandPos(i);
		dMotorPos	= model.axis[i].pos[iPos];

		if (fabs(dMotorPos-dCurPos) > 0.01)
			return false;
	}

	return true;
}
bool CLensEdgeDlg::checkCurMotorPCBPosZ(int iPos)
{
	double dCurPos;
	double dMotorPos;

	dCurPos		= motor.GetCommandPos(Motor_PCB_Z);
	dMotorPos	= model.axis[Motor_PCB_Z].pos[iPos];
	if (fabs(dMotorPos-dCurPos) > 0.01)
		return false;

	return true;
}
bool CLensEdgeDlg::checkCurMotorPCBPos(int iPos)
{
	double dCurPos;
	double dMotorPos;

	for (int i=Motor_PCB_X ; i<=Motor_PCB_Y; i++)
	{
		dCurPos		= motor.GetEncoderPos(i);	//motor.GetCommandPos(i);
		dMotorPos	= model.axis[i].pos[iPos];

		if (fabs(dMotorPos-dCurPos) > 0.01)
			return false;
	}

	return true;
}

bool CLensEdgeDlg::checkCurMotorPosZ(int iPos)
{	//Z�� ���� ��ġ Ȯ��
	double dCurPos;
	double dMotorPos;

	dCurPos		= motor.GetCommandPos(Motor_Lens_Z);
	dMotorPos	= model.axis[Motor_Lens_Z].pos[iPos];
	if (fabs(dMotorPos-dCurPos) > 0.01)
		return false;

	return true;
}

void CLensEdgeDlg::changeColorMotorPosBtn()
{
	int iIndex = - 1, iIndex2 = -1;
	int pcbIndex = -1;
	if (checkCurMotorPos(Wait_Pos))						iIndex = 0;
	else if (checkCurMotorPos(Loading_Pos))				iIndex = 1;
	else if (checkCurMotorPos(Bonding_Pos))				iIndex = 2;
	else if (checkCurMotorPos(Lens_Pos))				iIndex = 3;
	else if (checkCurMotorPos(ContiTailDelay_Pos))		iIndex = 4;
	else if (checkCurMotorPos(Pcb_Loading_Pos))			iIndex = 5;
	else if (checkCurMotorPos(Cover_Loading_Pos))		iIndex = 6;
	else if (checkCurMotorPos(Cover_UnLoading_Pos))		iIndex = 7;
	else if (checkCurMotorPos(Holder_Uv_Pos))			iIndex = 8;

	//
	if (checkCurMotorPCBPos(Lens_Pos))		pcbIndex = 1;

	m_btnWaitPos.m_iStateBtn = 0;
	m_btnSupplyPos.m_iStateBtn = 0;
	m_btnAlignPos.m_iStateBtn = 0;
	m_btnGiveUpPos.m_iStateBtn = 0;
	m_btnLoading.m_iStateBtn = 0;
	m_btnAlignPos.m_iStateBtn = 0;
	m_btnPcbAlignPos.m_iStateBtn = 0;
	m_btn_ContiDelay.m_iStateBtn = 0;
	m_btnPcbLoadPos.m_iStateBtn = 0;
	m_btnCoverLoadPos.m_iStateBtn = 0;
	m_btnCoverUnLoadPos.m_iStateBtn = 0;


	if (iIndex==0)					m_btnWaitPos.m_iStateBtn = 3;
	else if (iIndex==1)				m_btnSupplyPos.m_iStateBtn = 3;
	else if (iIndex==2)				m_btnGiveUpPos.m_iStateBtn = 3;
	else if (iIndex == 3)			m_btnAlignPos.m_iStateBtn = 3;
	else if (iIndex == 4)			m_btn_ContiDelay.m_iStateBtn = 3;
	else if (iIndex == 5)			m_btnPcbLoadPos.m_iStateBtn = 3;
	else if (iIndex == 6)			m_btnCoverLoadPos.m_iStateBtn = 3;
	else if (iIndex == 8)			m_btnCoverUnLoadPos.m_iStateBtn = 3;

	if(pcbIndex==1)			m_btnPcbAlignPos.m_iStateBtn = 3;

	m_btnWaitPos.Invalidate();
	m_btnSupplyPos.Invalidate();
	m_btnAlignPos.Invalidate();
	m_btnGiveUpPos.Invalidate();
	m_btnLoading.Invalidate();
	m_btn_ContiDelay.Invalidate();
	m_btnPcbLoadPos.Invalidate();
	m_btnCoverLoadPos.Invalidate();
	m_btnCoverUnLoadPos.Invalidate();

	m_btnPcbAlignPos.Invalidate();
	//-- z�� ǥ��

	iIndex = -1;
	pcbIndex = -1;
	if ( checkCurMotorPosZ(Wait_Pos) )					iIndex = 0;
	else if( checkCurMotorPosZ(Loading_Pos) )			iIndex = 1;
	else if( checkCurMotorPosZ(Bonding_Pos ) )			iIndex = 2;
#if (____AA_WAY == PCB_TILT_AA)
	else if (checkCurMotorPosZ(Lens_Pos))				iIndex = 3;
#endif
	else if (checkCurMotorPosZ(ContiTailDelay_Pos))		iIndex = 4;
	else if (checkCurMotorPosZ(Pcb_Loading_Pos))		iIndex = 5;
	else if (checkCurMotorPosZ(Cover_Loading_Pos))		iIndex = 6;
	else if (checkCurMotorPosZ(Cover_UnLoading_Pos))		iIndex = 7;
	else if (checkCurMotorPosZ(Holder_Uv_Pos))		iIndex = 8;
	
	if( checkCurMotorPCBPosZ(Lens_Pos ) )				pcbIndex = 0;

	m_btn_ZWaitePos.m_iStateBtn = 0;
	m_btn_ZLoading.m_iStateBtn = 0; 
	m_btn_ZBonding.m_iStateBtn = 0;
	m_btnAlignZPos.m_iStateBtn = 0;
	m_btn_ZContiDelay.m_iStateBtn = 0;
	m_btnPcbAlignZPos.m_iStateBtn = 0;
	m_btnPcbLoadZPos.m_iStateBtn = 0;
	m_btnCoverLoadZPos.m_iStateBtn = 0;
	m_btnCoverUnLoadZPos.m_iStateBtn = 0;

	if (iIndex==0)				m_btn_ZWaitePos.m_iStateBtn = 3;
	else if (iIndex==1)			m_btn_ZLoading.m_iStateBtn = 3;
	else if (iIndex==2)			m_btn_ZBonding.m_iStateBtn = 3;
	else if (iIndex == 3)		m_btnAlignZPos.m_iStateBtn = 3;
	else if (iIndex == 4)		m_btn_ZContiDelay.m_iStateBtn = 3;
	else if (iIndex == 5)		m_btnPcbLoadZPos.m_iStateBtn = 3;
	else if (iIndex == 6)		m_btnCoverLoadZPos.m_iStateBtn = 3;
	else if (iIndex == 8)		m_btnCoverUnLoadZPos.m_iStateBtn = 3;
	
	if (pcbIndex==0)		m_btnPcbAlignZPos.m_iStateBtn = 3;

	m_btn_ZWaitePos.Invalidate();
	m_btn_ZLoading.Invalidate();
	m_btn_ZBonding.Invalidate();
	m_btnAlignZPos.Invalidate();
	m_btn_ZContiDelay.Invalidate();
	m_btnPcbLoadZPos.Invalidate();
	m_btnCoverLoadZPos.Invalidate();
	m_btnCoverUnLoadZPos.Invalidate();

	m_btnPcbAlignZPos.Invalidate();

}
void CLensEdgeDlg::changeColorLaserCamBtn()
{
	bool flag1 = false;	
	bool flag2 = false;	
	flag1 = Dio.CamLaserSlinderCheck(true, false);
	flag2 = Dio.CamLaserSlinderCheck(false, false);

	if (flag1)
	{ 
		m_btnLaserCamFor.m_iStateBtn	= 1;
	}
	else
	{
		m_btnLaserCamFor.m_iStateBtn	= 0;
	}
	if (flag2)
	{ 
		m_btnLaserCamBack.m_iStateBtn	= 1;
	}
	else
	{
		m_btnLaserCamBack.m_iStateBtn	= 0;
	}

	m_btnLaserCamFor.Invalidate();
	m_btnLaserCamBack.Invalidate();

}

void CLensEdgeDlg::changeColorGripBtn()
{
	bool flag1 = false;	
	bool flag2 = false;
	flag1 = Dio.LensMotorGripCheck(true, false);
	flag2 = Dio.LensMotorGripCheck(false, false);

	if (flag1)
	{ 
		m_btnCoverOn.m_iStateBtn	= 1;
	}
	else
	{
		m_btnCoverOn.m_iStateBtn	= 0;
	}
	if (flag2)
	{ 
		m_btnCoverOff.m_iStateBtn	= 1;
	}
	else
	{
		m_btnCoverOff.m_iStateBtn	= 0;
	}

	m_btnCoverOn.Invalidate();
	m_btnCoverOff.Invalidate();

}

void CLensEdgeDlg::changeColorLaserCylinderLensTabBtn()
{
	bool flag = false;
	bool flag2 = false;
	flag = Dio.LaserCylinderCheck(true, false);

	if (flag)
	{
		m_btn_Laser_CylUp_LensTab.m_iStateBtn = 1;
		m_btn_Laser_CylDown_LensTab.m_iStateBtn = 0;
	}
	else
	{
		m_btn_Laser_CylUp_LensTab.m_iStateBtn = 0;
		flag2 = Dio.LaserCylinderCheck(false, false);
		if (flag2)
		{
			m_btn_Laser_CylUp_LensTab.m_iStateBtn = 0;
			m_btn_Laser_CylDown_LensTab.m_iStateBtn = 1;
		}
		else
		{
			m_btn_Laser_CylDown_LensTab.m_iStateBtn = 0;
		}

	}

	m_btn_Laser_CylUp_LensTab.Invalidate();
	m_btn_Laser_CylDown_LensTab.Invalidate();
}

void CLensEdgeDlg::changeColorUVBtn()
{

	/*bool bInUVFlag;			
	bInUVFlag = Dio.UVCheck(true, false);

	if (bInUVFlag)
	{
		m_btn_UVOn.m_iStateBtn	= 1;
		m_btn_UVOff.m_iStateBtn	= 0;
	}
	else
	{
		m_btn_UVOn.m_iStateBtn	= 0;
		m_btn_UVOff.m_iStateBtn	= 1;
	}

	m_btn_UVOn.Invalidate();
	m_btn_UVOff.Invalidate();
*/
}

void CLensEdgeDlg::OnBnClickedButtonPcbCoverOn()
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

	/* �̵����̸� Return */
	if(g_bMovingflag)
		return;
	g_bMovingflag =true;

	//KYH DI/O �߰� �ʿ�
	Dio.LensMotorGrip(true, false);

	g_bMovingflag =false;
}


void CLensEdgeDlg::OnBnClickedButtonPcbCoverOff()
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

	/* �̵����̸� Return */
	if(g_bMovingflag)
		return;
	g_bMovingflag =true;

	Dio.LensMotorGrip(false, false);
	Task.m_bOKLensPass = -1;//���� �ѱ� �Ϸ� Lamp ���
	g_bMovingflag =false;
}


void CLensEdgeDlg::DigLedputData()
{
	/*CAABonderDlg* pFrame = (CAABonderDlg*)(AfxGetApp()->m_pMainWnd);

	pFrame->SetDigReference(LENS_Align_MARK);

	UpdateData(false);*/
}


void CLensEdgeDlg::OnBnClickedButtonGotoLensalign()
{

}
void CLensEdgeDlg::OnBnClickedButtonGotoPcbalign()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	pFrame->m_bIsLensMode = 0;
	pFrame->ctrlSubDlg(LENS_DLG);

	//DigLedputData();
}


void CLensEdgeDlg::OnBnClickedButtonEdgeGotoLensalign()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	pFrame->m_bIsLensMode = 1;
	pFrame->ctrlSubDlg(PCB_DLG);

	//DigLedputData();
}


void CLensEdgeDlg::OnBnClickedButtonZMotorUp2()
{
	if( !func_TaskMovingCheck() )	return;

	g_bMovingflag =true;
	CString sTmpLog = _T("");
	/*double PcbPosX = (float)model.axis[Motor_PCB_X].pos[Bonding_Pos] - (float)motor.GetEncoderPos(Motor_PCB_X);
	double PcbPosY = (float)model.axis[Motor_PCB_Y].pos[Bonding_Pos] - (float)motor.GetEncoderPos(Motor_PCB_Y);
	double PcbPosZ = (float)model.axis[Motor_PCB_Z].pos[Wait_Pos] - (float)motor.GetCommandPos(Motor_PCB_Z);
	if (fabs(PcbPosX) < 2.1 && fabs(PcbPosY) < 2.1)
	{
		if (fabs(PcbPosZ) >= 1.0)
		{

			sTmpLog.Format("Pcb Z �����ġ �̵��� �����Ͻʽÿ�!");
			delayMsg(sTmpLog, 5000, M_COLOR_RED);
			g_bMovingflag = false;
			return;
		}
	}*/
	if(!motor.LENS_Z_Motor_Move(Wait_Pos))
	{
		sLangChange.Format("LENS Z axis Wait_Pos Move Failed");
		delayMsg(sLangChange, 1000, M_COLOR_RED);
	}
	g_bMovingflag =false;
}


void CLensEdgeDlg::OnBnClickedButtonZMotorDown()
{ 
	if( !func_TaskMovingCheck() )	return;

	if (!checkCurMotorPos(Loading_Pos))
	{
		delayMsg("Do not Loading_Pos", 2000, M_COLOR_RED);
		return;
	}

	
	g_bMovingflag =true;
	if( !motor.Lens_Tilt_Motor_Move(Loading_Pos) )
	{
		delayMsg("Lens Tx,Ty Loading_Pos �̵� ����.", 1000, M_COLOR_RED);
		g_bMovingflag =false;
		return;
	}
	if(!motor.LENS_Z_Motor_Move(Loading_Pos))
	{
		sLangChange.Format("LENS Z axis Loading_Pos Move Failed");
		delayMsg(sLangChange, 1000, M_COLOR_RED);
	}

	g_bMovingflag =false;
}


void CLensEdgeDlg::OnBnClickedButtonZMotorDownBonding()
{
	if( !func_TaskMovingCheck() )	return;

	
	if ( !checkCurMotorPos(Bonding_Pos) )
	{
		delayMsg("Do not Bonding_Pos", 2000, M_COLOR_RED);
		return;
	}
	g_bMovingflag =true;
	if(!motor.LENS_Z_Motor_Move(Bonding_Pos))
	{
		sLangChange.Format("LENS Z axis Bonding_Pos Move Failed");
		delayMsg(sLangChange, 1000, M_COLOR_RED);
	}

	g_bMovingflag =false;
}


void CLensEdgeDlg::OnBnClickedButtonEdgeMotorLoadingAlign()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	if( !func_TaskMovingCheck() )	return;

	g_bMovingflag =true;

	if( !func_MovingInterlock() )	return;

	//if(!motor.LENS_Z_Motor_Move(Wait_Pos))
	//{
	//	g_bMovingflag =false;
	//	return;
	//}
	//if(!motor.PCB_Z_Motor_Move(Wait_Pos))
	//{
	//	g_bMovingflag =false;
	//	return;
	//}

	double offsetX, offsetY, offsetTh;
	int iRtnVal;
	int  iRtn = 0;//pFrame->procCamAlign(CAM1, LENS_Align_MARK, false, offsetX, offsetY, offsetTh);
	
	CString sMsg;
	sLangChange.LoadStringA(IDS_STRING969);	//PCB Unit ���� ��[X:%.04lf, Y:%.04lf, T:%.04lf]���� �ѱ� ��ġ �̵� �Ͻðڽ��ϱ�?
	sMsg.Format(sLangChange, 
		Task.d_Align_offset_x[LENS_Align_MARK], 
		Task.d_Align_offset_y[LENS_Align_MARK], 
		Task.d_Align_offset_th[LENS_Align_MARK] );

	if ( askMsg(sMsg) != IDOK)
	{
		g_bMovingflag =false;
		return;
	}
	offsetX = Task.d_Align_offset_x[LENS_Align_MARK];
	offsetY = Task.d_Align_offset_y[LENS_Align_MARK];
	offsetTh = Task.d_Align_offset_th[LENS_Align_MARK];


	iRtnVal = pFrame->AlignLimitCheck(CAM1, offsetX, offsetY, offsetTh);
	//double pos[3];
	if(iRtnVal==0)
	{
		sLangChange.LoadStringA(IDS_STRING1263);	//������ �̵� ������ Limit �ʰ� �߽��ϴ�.
			errMsg2(Task.AutoFlag, sLangChange);
	}
	else
	{
		//pos[0]	= (motor.GetCommandPos(Motor_PCB_X) - model.axis[Motor_PCB_X].pos[Align_Pos])	- offsetX;
		//pos[1]	= (motor.GetCommandPos(Motor_PCB_Y) - model.axis[Motor_PCB_Y].pos[Align_Pos])	- offsetY;
		//pos[2]	= (motor.GetCommandPos(Motor_PCB_TH) - model.axis[Motor_PCB_TH].pos[Align_Pos])	+ offsetTh;

		CString sTemp;
		sLangChange.LoadStringA(IDS_STRING1482);	//������ : %.3lf %.3lf %.3lf
		sTemp.Format(sLangChange, offsetX, offsetY, offsetTh);
		m_labelAlignResult.SetText(sTemp);
		m_labelAlignResult.Invalidate();

		if( !motor.Pcb_Motor_Move(Loading_Pos, offsetX, offsetY, offsetTh) )
		{
			CString sTmpLog;
			sLangChange.LoadStringA(IDS_STRING1016);	//PCB�� Lens %s �̵� �Ϸ�
			sTmpLog.Format(sLangChange, MotorPosName[Loading_Pos]);
			delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_GREEN);
		}
	}


	pFrame->m_rBox.left = 0;
	pFrame->m_rBox.top = 0;
	pFrame->m_rBox.right = 0;
	pFrame->m_rBox.bottom = 0;

	g_bMovingflag =false;
}


void CLensEdgeDlg::OnBnClickedButtonEdgeEpoxyUvOn()
{
	UVCommand.UV_Shutter_Open();

	if (sysData.nUvIndex == 1)
	{
		Sleep(100);
		UVCommand2.UV_Shutter_Open();
	}
}

void CLensEdgeDlg::OnBnClickedButtonEdgeEpoxyUvOff()
{
	UVCommand.UV_Shutter_Close();
	if (sysData.nUvIndex == 1)
	{
		Sleep(100);
		UVCommand2.UV_Shutter_Close();
	}
}


void CLensEdgeDlg::OnBnClickedButtonEdgeMotorLoading()
{
	CString sTmpLog;

	if( !func_TaskMovingCheck() )	return;

	g_bMovingflag =true;

	if( !func_MovingInterlock() )	return;

	if(!motor.LENS_Z_Motor_Move(Wait_Pos))
	{
		g_bMovingflag =false;
		return;
	}
	if(!motor.PCB_Z_Motor_Move(Wait_Pos))
	{
		g_bMovingflag =false;
		return;
	}

	if( !motor.Pcb_Tilt_Motor_Move(Loading_Pos))
	{
		sLangChange.LoadStringA(IDS_STRING1017);	//PCB�� Tx, Ty�� ��� ��ġ �̵� ����
		sTmpLog.Format(sLangChange);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag =false;
		return;
	}

	if(	!motor.Pcb_Motor_Move(Loading_Pos))
	{
		sLangChange.LoadStringA(IDS_STRING1481);	//PCB�� %s �̵� ����
		sTmpLog.Format(sLangChange, MotorPosName[Loading_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag =false;
		return;
	}

	sLangChange.LoadStringA(IDS_STRING1015);	//PCB�� %s �̵� �Ϸ�
	sTmpLog.Format(sLangChange, MotorPosName[Loading_Pos]);
	delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_GREEN);

	g_bMovingflag =false;
}


bool CLensEdgeDlg::func_TaskMovingCheck()
{
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1463);
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return false;
	}

	if(Task.AutoFlag == 2)
	{
		sLangChange.LoadStringA(IDS_STRING1468);
		delayMsg(sLangChange, 2000, M_COLOR_RED);// �Ͻ� ���� �� ��� �Ұ�
		return false;
	}

	if(g_bMovingflag)
	{
		sLangChange.LoadStringA(IDS_STRING1228);
		delayMsg(sLangChange, 2000, M_COLOR_RED);
		return false;
	}
	CString sLog="";

//#if (____AA_WAY == PCB_TILT_AA)
//	Dio.CamLaserSlinderMove(false, false);
//	if(!Dio.CamLaserSlinderCheck(false, false))
//	{
//		if(!askMsg("Laser/Cam �Ǹ��� ���� �����Դϴ�. \n��� �����Ͻðڽ��ϱ�?") == IDOK)
//		{
//			return false;
//		}
//	}
//#endif

	return true;
}

bool CLensEdgeDlg::func_MovingInterlock()
{
	if( !checkCurMotorPosZ(Wait_Pos))
	{
		delayMsg("LENS Wait_Pos Z��ġ�� �ƴմϴ�", 3000, M_COLOR_RED);
		g_bMovingflag = false;
		return false;
	}
	return true;
}

bool CLensEdgeDlg::checkInterlockLoadingZ()
{//-- LensZ�� �ѱ���ġ �̵��� ���Ͷ� Check - PCB Y �������� ������ġ ���� ���� ���� ��� ���� �߻�.

	double MotorPos;
	MotorPos = motor.GetEncoderPos(Motor_PCB_Y);
	
	if(fabs(MotorPos) > model.axis[Motor_PCB_Y].pos[Bonding_Pos] + 5)
	{//���� ��ġ�� ������ġ+5mm ���� ���� ����� �� ��ġ�� ���� ��� ���� �߻�
		delayMsg("PCB Y Postion Error.. Move PCB_Y.", 1000, M_COLOR_RED);
		return false;
	}
	return true;
}

void CLensEdgeDlg::OnBnClickedButtonZMotorAlign()
{
#if (____AA_WAY == PCB_TILT_AA)

	if( !func_TaskMovingCheck() )	return;
	
	if ( !checkCurMotorPos(Lens_Pos) )
	{
		delayMsg("Do not Lens_Pos", 2000, M_COLOR_RED);
		return;
	}
	g_bMovingflag =true;
	if(motor.LENS_Z_Motor_Move(Lens_Pos))
	{
		CString sTmpLog="";
		sTmpLog.Format("Lens Align Z��ġ �̵�");
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_GREEN);
	}
	g_bMovingflag =false;
#elif (____AA_WAY == LENS_TILT_AA)
	if (!checkCurMotorPCBPos(Lens_Pos))
	{
		delayMsg("Do not Lens_Pos", 2000, M_COLOR_RED);
		return;
	}
	g_bMovingflag = true;

	if (!motor.PCB_Z_Motor_Move(Lens_Pos))
	{
		sLangChange.Format("PCB Z axis Lens_Pos Move Failed");
		delayMsg(sLangChange, 1000, M_COLOR_RED);
	}

	g_bMovingflag = false;
	/*delayMsg("���� ƿƮ�� �̻�� ��ġ�Դϴ�.", 2000, M_COLOR_RED);
	return;*/
#endif
}


void CLensEdgeDlg::OnBnClickedBtnCamLaserFor()
{
	if( !func_TaskMovingCheck() )	return;
	/*if ( !checkCurMotorPos(Wait_Pos) )
	{
		delayMsg("Do not Wait_Pos", 2000, M_COLOR_RED);
		return;
	}*/
	/* �̵����̸� Return */
	if(g_bMovingflag)
		return;
	g_bMovingflag =true;
	Dio.CamLaserSlinderMove(true, false);
	g_bMovingflag =false;
}


void CLensEdgeDlg::OnBnClickedBtnCamLaserBack()
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

	/* �̵����̸� Return */
	if(g_bMovingflag)
		return;
	g_bMovingflag =true;
	Dio.CamLaserSlinderMove(false, false);

	g_bMovingflag =false;
}


void CLensEdgeDlg::OnBnClickedButtonEdgeMotorPcbalign()
{
	if( !func_TaskMovingCheck() )	return;
	if( !func_MovingInterlock())
	{
		return;
	}
	g_bMovingflag =true;
	if(!motor.PCB_Z_Motor_Move(Wait_Pos))
	{
		sLangChange.Format("PCB Z axis Wait_Pos Move Failed");
		delayMsg(sLangChange, 1000, M_COLOR_RED);
	}
	if(	motor.Pcb_Motor_Move(Lens_Pos))
	{
		sTempLang.Format("PCB�� %s �̵� �Ϸ�", MotorPosName[Lens_Pos]);
		delayMsg(sTempLang.GetBuffer(99), 3000, M_COLOR_GREEN);
	}

	

	g_bMovingflag =false;
}


void CLensEdgeDlg::OnBnClickedButtonZMotorPcbalign()
{
	if( !func_TaskMovingCheck() )	return;


	if ( !checkCurMotorPCBPos(Lens_Pos) )
	{
		delayMsg("Do not Lens_Pos", 2000, M_COLOR_RED);
		return;
	}
	g_bMovingflag =true;

	if(!motor.PCB_Z_Motor_Move(Lens_Pos))
	{
		sLangChange.Format("PCB Z axis Lens_Pos Move Failed");
		delayMsg(sLangChange, 1000, M_COLOR_RED);
	}

	g_bMovingflag =false;
}


void CLensEdgeDlg::OnBnClickedButtonZMotorContiPos()
{
	if (!func_TaskMovingCheck())	return;



	if (!checkCurMotorPos(ContiTailDelay_Pos))
	{
		delayMsg("Do not ContiTailDelay_Pos", 2000, M_COLOR_RED);
		return;
	}
	g_bMovingflag = true;
	if (!motor.Lens_Tilt_Motor_Move(ContiTailDelay_Pos))
	{
		delayMsg("Lens Tx,Ty ContiTailDelay_Pos �̵� ����.", 1000, M_COLOR_RED);
		g_bMovingflag = false;
		return;
	}
	if (!motor.LENS_Z_Motor_Move(ContiTailDelay_Pos))
	{
		sLangChange.Format("LENS Z axis ContiTailDelay_Pos Move Failed");
		delayMsg(sLangChange, 1000, M_COLOR_RED);
	}

	g_bMovingflag = false;
}


void CLensEdgeDlg::OnStnClickedStcContiDownOffset()
{
	if (!func_TaskMovingCheck())	return;
	CString celData = "";
	GetDlgItemText(IDC_STC_CONTI_DOWN_OFFSET, celData);
	CGetNumDlg dlg(10, (LPTSTR)(LPCTSTR)celData);
	if (dlg.DoModal() == IDOK)
	{
		float f_num = fabs((float)atof(dlg.GetstrNum()));

		if (f_num > 10.0)			f_num = 10.0;

		celData.Format("%.01f", (f_num));
		m_Conti_Down_Offset.SetText(celData);
		model.m_ContiDownOffset = (double)f_num;
		model.Save();
	}
}


void CLensEdgeDlg::OnBnClickedButtonMotorContiPos()
{
	CString sTmpLog = "";
	if (!func_TaskMovingCheck())	return;

	if (!func_MovingInterlock())	return;

	g_bMovingflag = true;
	if (!motor.Lens_Tilt_Motor_Move(ContiTailDelay_Pos))
	{
		sLangChange.LoadStringA(IDS_STRING648);	//Lens %s �̵� ����
		sTmpLog.Format(sLangChange, MotorPosName[ContiTailDelay_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag = false;
		return;
	}

	if (!motor.Lens_Motor_MoveXY(ContiTailDelay_Pos))
	{
		sTmpLog.Format("Lens %s �̵� ����", MotorPosName[ContiTailDelay_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag = false;
		return;
	}
	else {
		sTmpLog.Format("Lens %s �̵� �Ϸ�", MotorPosName[ContiTailDelay_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_GREEN);
	}
	g_bMovingflag = false;
}


void CLensEdgeDlg::OnBnClickedButtonLensLaserPos1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!func_TaskMovingCheck())	return;

#if (____AA_WAY == LENS_TILT_AA)
	return;
#endif
	if (!askMsg("LensMotor ������ ��ġ �̵��Ͻðڽ��ϱ�?"))
	{
		return;
	}
	/* �̵����̸� Return */
	g_bMovingflag = true;
	CString sTmpLog = "";


	if(!motor.LENS_Z_Motor_Move(Wait_Pos))
	{
		g_bMovingflag =false;
		return;
	}
	
	if( motor.Lens_Motor_Move_Laser(0, 0, 0, 0) )
	{
		if(motor.LENS_Z_Motor_Move(Laser_Lens_Pos))
		{

			delayMsg("Lens�� Laser ���� ���� 1����ġ �̵� �Ϸ�", 3000, M_COLOR_GREEN);
		}

	}else
	{
		delayMsg("Lens�� Laser ���� ��ġ Ȯ�� �ٶ�", 2000, M_COLOR_RED);
	}

	g_bMovingflag = false;
}


void CLensEdgeDlg::OnBnClickedButtonLaserAutoInsp()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
#if (____AA_WAY == LENS_TILT_AA)
	return;
#endif
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CDPoint LaserPos[4];
	CString sLog = _T("");
	int iLaserDelay = model.strInfo_Cam[0].m_iDelayTime;	//Laser ������ Delay
	double dTx = 0.0;
	double dTy = 0.0;

	if (!func_TaskMovingCheck())	return;

	if (!askMsg("LensMotor ������ �ڵ������Ͻðڽ��ϱ�?"))
	{
		return;
	}
	/* �̵����̸� Return */
	g_bMovingflag = true;
	if (!motor.LENS_Z_Motor_Move(Wait_Pos))
	{
		g_bMovingflag = false;
		return;
	}


	Sleep(500);

	Task.oldLaserTx = 0.1;
	Task.oldLaserTy = 0.1;
	for (int iNo = 0; iNo < 4; iNo++)
	{
		motor.Lens_Motor_Move_Laser(iNo, 0, 0, 0);
		Sleep(10);
		motor.LENS_Z_Motor_Move(Laser_Lens_Pos);

		Task.PCBTaskTime = myTimer(true);
		while (1)
		{
			if ((myTimer(true) - Task.PCBTaskTime) > iLaserDelay && (motor.IsStopAxis(Motor_Lens_X) && motor.IsStopAxis(Motor_Lens_Y) && motor.IsStopAxis(Motor_Lens_Z)))
			{
				break;
			}
			else if ((myTimer(true) - Task.PCBTaskTime) > iLaserDelay * 2)
			{
				sLangChange.LoadStringA(IDS_STRING1257);	//���� ���� ��ġ �̵� �Ϸ� �ð� �ʰ�....
				delayMsg(sLangChange, 2000, M_COLOR_RED);
				g_bMovingflag = false;
				return;
			}

			checkMessage();
			Sleep(5);
			checkMessage();
		}
		Sleep(400);
		//Keyence.func_LT9030_Scan(Task.m_Laser_Point[iNo]);
		Keyence.func_CL3000_Scan(Task.m_Laser_Point[iNo]);
		LaserPos[iNo].x = motor.GetEncoderPos(Motor_Lens_X);
		LaserPos[iNo].y = motor.GetEncoderPos(Motor_Lens_Y);

		sLog.Format("Laser ������ (%d) : %.04f", iNo + 1, Task.m_Laser_Point[iNo]);
		pFrame->putListLog(sLog);

		Sleep(5);
	}

	g_bMovingflag = false;
	pFrame->_calcLaserTilt(LaserPos, Task.m_Laser_Point, dTx, dTy);

	sLog.Format("Laser Tilting ������ X:%.04f, Y:%.04f", dTx, dTy);
	pFrame->putListLog(sLog);
	motor.Lens_Move_Tilt(dTx, dTy);
}


void CLensEdgeDlg::OnBnClickedButtonLensLaserPos2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!func_TaskMovingCheck())	return;
#if (____AA_WAY == LENS_TILT_AA)
	return;
#endif
	if (!askMsg("LensMotor ������ ��ġ �̵��Ͻðڽ��ϱ�?"))
	{
		return;
	}
	/* �̵����̸� Return */
	g_bMovingflag = true;
	CString sTmpLog = "";
	

	if (!motor.LENS_Z_Motor_Move(Wait_Pos))
	{
		g_bMovingflag = false;
		return;
	}

	if (motor.Lens_Motor_Move_Laser(1, 0, 0, 0))
	{
		if (motor.LENS_Z_Motor_Move(Laser_Lens_Pos))
		{

			delayMsg("Lens�� Laser ���� ���� 2����ġ �̵� �Ϸ�", 3000, M_COLOR_GREEN);
		}

	}
	else
	{
		delayMsg("Lens�� Laser ���� ��ġ Ȯ�� �ٶ�", 2000, M_COLOR_RED);
	}

	g_bMovingflag = false;
}


void CLensEdgeDlg::OnBnClickedButtonLensLaserPos3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!func_TaskMovingCheck())	return;
#if (____AA_WAY == LENS_TILT_AA)
	return;
#endif
	if (!askMsg("LensMotor ������ ��ġ �̵��Ͻðڽ��ϱ�?"))
	{
		return;
	}
	/* �̵����̸� Return */
	g_bMovingflag = true;
	CString sTmpLog = "";

	
	if (!motor.LENS_Z_Motor_Move(Wait_Pos))
	{
		g_bMovingflag = false;
		return;
	}

	if (motor.Lens_Motor_Move_Laser(2, 0, 0, 0))
	{
		if (motor.LENS_Z_Motor_Move(Laser_Lens_Pos))
		{

			delayMsg("Lens�� Laser ���� ���� 3����ġ �̵� �Ϸ�", 3000, M_COLOR_GREEN);
		}

	}
	else
	{
		delayMsg("Lens�� Laser ���� ��ġ Ȯ�� �ٶ�", 2000, M_COLOR_RED);
	}

	g_bMovingflag = false;
}


void CLensEdgeDlg::OnBnClickedButtonLensLaserPos4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!func_TaskMovingCheck())	return;
#if (____AA_WAY == LENS_TILT_AA)
	return;
#endif
	if (!askMsg("LensMotor ������ ��ġ �̵��Ͻðڽ��ϱ�?"))
	{
		return;
	}
	/* �̵����̸� Return */
	g_bMovingflag = true;
	CString sTmpLog = "";

	
	if (!motor.LENS_Z_Motor_Move(Wait_Pos))
	{
		g_bMovingflag = false;
		return;
	}

	if (motor.Lens_Motor_Move_Laser(3, 0, 0, 0))
	{
		if (motor.LENS_Z_Motor_Move(Laser_Lens_Pos))
		{

			delayMsg("Lens�� Laser ���� ���� 4����ġ �̵� �Ϸ�", 3000, M_COLOR_GREEN);
		}

	}
	else
	{
		delayMsg("Lens�� Laser ���� ��ġ Ȯ�� �ٶ�", 2000, M_COLOR_RED);
	}

	g_bMovingflag = false;
}


void CLensEdgeDlg::OnBnClickedButtonLaserManualInsp()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	double dVel = 0.0;
	Keyence.func_CL3000_Scan(dVel);

	//Keyence.func_LT9030_Scan(dVel);

	CString sLog = _T("");
	sLog.Format("Laser ���� ���� ��(mm) : %lf", dVel);
	pFrame->putListLog(sLog);
}


void CLensEdgeDlg::OnBnClickedButtonSminiUvOn()
{
	UVCommand2.UV_Shutter_Open();
	Sleep(model.UV_Time);
	UVCommand2.UV_Shutter_Close();
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CLensEdgeDlg::OnBnClickedButtonSminiUvOff()
{
	UVCommand2.UV_Shutter_Close();
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CLensEdgeDlg::OnBnClickedButtonEdgeMotorPcbload()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sTmpLog = "";
	if (!func_TaskMovingCheck())	return;

	if (!func_MovingInterlock())	return;

	g_bMovingflag = true;
	if (!motor.Lens_Tilt_Motor_Move(Pcb_Loading_Pos))
	{
		sLangChange.LoadStringA(IDS_STRING648);	//Lens %s �̵� ����
		sTmpLog.Format(sLangChange, MotorPosName[Pcb_Loading_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag = false;
		return;
	}

	if (!motor.Lens_Motor_MoveXY(Pcb_Loading_Pos))
	{
		sTmpLog.Format("Lens %s �̵� ����", MotorPosName[Pcb_Loading_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag = false;
		return;
	}
	else {
		sTmpLog.Format("Lens %s �̵� �Ϸ�", MotorPosName[Pcb_Loading_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_GREEN);
	}
	g_bMovingflag = false;
}


void CLensEdgeDlg::OnBnClickedButtonEdgeZMotorPcbload()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!func_TaskMovingCheck())	return;
	if (!checkCurMotorPos(Pcb_Loading_Pos))
	{
		delayMsg("Do not Pcb_Loading_Pos", 2000, M_COLOR_RED);
		return;
	}
	g_bMovingflag = true;
	if (!motor.Lens_Tilt_Motor_Move(Pcb_Loading_Pos))
	{
		delayMsg("Lens Tx,Ty Pcb_Loading_Pos �̵� ����.", 1000, M_COLOR_RED);
		g_bMovingflag = false;
		return;
	}
	if (!motor.LENS_Z_Motor_Move(Pcb_Loading_Pos))
	{
		sLangChange.Format("LENS Z axis Pcb_Loading_Pos Move Failed");
		delayMsg(sLangChange, 1000, M_COLOR_RED);
	}

	g_bMovingflag = false;
}


void CLensEdgeDlg::OnBnClickedButtonEdgeMotorCoverload()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sTmpLog = "";
	if (!func_TaskMovingCheck())	return;

	if (!func_MovingInterlock())	return;

	g_bMovingflag = true;
	if (!motor.Lens_Tilt_Motor_Move(Cover_Loading_Pos))
	{
		sLangChange.LoadStringA(IDS_STRING648);	//Lens %s �̵� ����
		sTmpLog.Format(sLangChange, MotorPosName[Cover_Loading_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag = false;
		return;
	}

	if (!motor.Lens_Motor_MoveXY(Cover_Loading_Pos))
	{
		sTmpLog.Format("Lens %s �̵� ����", MotorPosName[Cover_Loading_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag = false;
		return;
	}
	else {
		sTmpLog.Format("Lens %s �̵� �Ϸ�", MotorPosName[Cover_Loading_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_GREEN);
	}
	g_bMovingflag = false;
}


void CLensEdgeDlg::OnBnClickedButtonEdgeZMotorCoverload()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!func_TaskMovingCheck())	return;
	if (!checkCurMotorPos(Cover_Loading_Pos))
	{
		delayMsg("Do not Cover_Loading_Pos", 2000, M_COLOR_RED);
		return;
	}
	g_bMovingflag = true;
	if (!motor.Lens_Tilt_Motor_Move(Cover_Loading_Pos))
	{
		delayMsg("Lens Tx,Ty Cover_Loading_Pos �̵� ����.", 1000, M_COLOR_RED);
		g_bMovingflag = false;
		return;
	}
	if (!motor.LENS_Z_Motor_Move(Cover_Loading_Pos))
	{
		sLangChange.Format("LENS Z axis Cover_Loading_Pos Move Failed");
		delayMsg(sLangChange, 1000, M_COLOR_RED);
	}

	g_bMovingflag = false;
}


void CLensEdgeDlg::OnBnClickedButtonEdgeMotorCoverunload()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sTmpLog = "";
	if (!func_TaskMovingCheck())	return;

	if (!func_MovingInterlock())	return;

	g_bMovingflag = true;
	if (!motor.Lens_Tilt_Motor_Move(Holder_Uv_Pos))
	{
		sLangChange.LoadStringA(IDS_STRING648);	//Lens %s �̵� ����
		sTmpLog.Format(sLangChange, MotorPosName[Holder_Uv_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag = false;
		return;
	}

	if (!motor.Lens_Motor_MoveXY(Holder_Uv_Pos))
	{
		sTmpLog.Format("Lens %s �̵� ����", MotorPosName[Holder_Uv_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_RED);
		g_bMovingflag = false;
		return;
	}
	else {
		sTmpLog.Format("Lens %s �̵� �Ϸ�", MotorPosName[Holder_Uv_Pos]);
		delayMsg(sTmpLog.GetBuffer(99), 3000, M_COLOR_GREEN);
	}
	g_bMovingflag = false;
}


void CLensEdgeDlg::OnBnClickedButtonEdgeZMotorCoverunload()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!func_TaskMovingCheck())	return;
	if (!checkCurMotorPos(Holder_Uv_Pos))
	{
		delayMsg("Do not Cover_UnLoading_Pos", 2000, M_COLOR_RED);
		return;
	}
	g_bMovingflag = true;
	if (!motor.Lens_Tilt_Motor_Move(Holder_Uv_Pos))
	{
		delayMsg("Lens Tx,Ty Cover_UnLoading_Pos �̵� ����.", 1000, M_COLOR_RED);
		g_bMovingflag = false;
		return;
	}
	if (!motor.LENS_Z_Motor_Move(Holder_Uv_Pos))
	{
		sLangChange.Format("LENS Z axis Cover_UnLoading_Pos Move Failed");
		delayMsg(sLangChange, 1000, M_COLOR_RED);
	}

	g_bMovingflag = false;
}


void CLensEdgeDlg::OnBnClickedButtonLaserCylUpLenstab()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1463);
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}
	if (g_bMovingflag)
	{
		sLangChange.LoadStringA(IDS_STRING1228);	//���� ���� �� ��� �Ұ�..
		delayMsg(sLangChange, 2000, M_COLOR_RED);
		return;
	}
	g_bMovingflag = true;
	Dio.LaserCylinderUPDOWN(true, false);
	g_bMovingflag = false;
}


void CLensEdgeDlg::OnBnClickedButtonLaserCylDownLenstab()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING1463);
		delayMsg(sLangChange, 1000, M_COLOR_RED);
		return;
	}
	if (g_bMovingflag)
	{
		sLangChange.LoadStringA(IDS_STRING1228);	//���� ���� �� ��� �Ұ�..
		delayMsg(sLangChange, 2000, M_COLOR_RED);
		return;
	}
	g_bMovingflag = true;
	Dio.LaserCylinderUPDOWN(false, false);
	g_bMovingflag = false;
}
