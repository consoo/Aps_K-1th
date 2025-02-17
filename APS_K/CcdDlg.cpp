// CcdDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CcdDlg.h"
#include "CCDSFRDlg.h"
#include "CCDInspModeDlg.h"

//! Added by LHW (2013/2/26)
//! Timer ID 
const UINT_PTR nTimer_Frame_Grabber_Diagnostics = 1000;

// CCcdDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCcdDlg, CDialogEx)

	CCcdDlg::CCcdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCcdDlg::IDD, pParent)
{
	//m_pOSChkDlg = NULL;
	m_pSFRDlg   = NULL;
	m_pInspModeDlg = NULL;
	//	m_pCtrlDlg	= NULL;
	m_iMode_View = CCD_VIEW_SFR;
}

CCcdDlg::~CCcdDlg()
{
}

void CCcdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CCD_OSCHECK, m_btnOSChk);
	DDX_Control(pDX, IDC_BUTTON_CCD_SFR, m_btnSFR);
	DDX_Control(pDX, IDC_BUTTON_CCD_DEFECT, m_btnDefect);
	DDX_Control(pDX, IDC_BUTTON_MANDO_INSP, m_btnInspMode);
}


BEGIN_MESSAGE_MAP(CCcdDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCcdDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCcdDlg::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CCD_OSCHECK, &CCcdDlg::OnBnClickedButtonCcdOscheck)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SFR, &CCcdDlg::OnBnClickedButtonCcdSfr)
	ON_BN_CLICKED(IDC_BUTTON_CCD_DEFECT, &CCcdDlg::OnBnClickedButtonCcdDefect)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MANDO_INSP, &CCcdDlg::OnBnClickedButtonMandoInsp)
END_MESSAGE_MAP()


// CCcdDlg �޽��� ó�����Դϴ�.
BOOL CCcdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//! Main ��ư�� ��Ʈ ����
	bool bFlag_Create_Child_Dlg_Font = CreateFont_Child_Dlg_Btn();

	if ( bFlag_Create_Child_Dlg_Font == true )
	{
		m_btnOSChk.SetFont(&m_ftNormal);
		m_btnSFR.SetFont(&m_ftNormal);
		m_btnDefect.SetFont(&m_ftNormal);
	}

	Make_Child_Dialog();

	setInterface();
	setInterface_Child_Dialog();

	SetTimer(nTimer_Frame_Grabber_Diagnostics, 500, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CCcdDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	//! Added by LHW (2013/2/26)
	KillTimer(nTimer_Frame_Grabber_Diagnostics);

	Delete_Child_Dialog();

	//! ��Ʈ ����
	m_ftNormal.DeleteObject();
	m_ftSelected.DeleteObject();
}

void CCcdDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if ( bShow == FALSE )
	{
		return;
	}

	setInterface();
	setInterface_Child_Dialog();
	SetView_Child_Dialog(m_iMode_View);
}

void CCcdDlg::OnBnClickedOk(){}
void CCcdDlg::OnBnClickedCancel(){}

void CCcdDlg::setInterface()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	CRect rcThis, rcBtn, rcManu;
	
	GetWindowRect_FromUI(IDC_LABEL_MENU, pFrame, rcManu);
	GetWindowRect_FromUI(IDC_BUTTON_MANDO_INSP, this, rcBtn);

	rcThis.top = rcManu.top - (rcBtn.bottom - rcBtn.top) - SUB_DLG_OFFSET_Y * 2;
	rcThis.bottom = rcManu.top - 1;
	rcThis.left = pFrame->m_rectCamDispPos2.left;
	rcThis.right = pFrame->m_rectCamDispPos2.right;
	//
	/*WINDOWPLACEMENT wndpl;
	wndpl.rcNormalPosition.left = pFrame->m_rectCamDispPos2.right+1;
	wndpl.rcNormalPosition.right = MAIN_DLG_SIZE_X;
	wndpl.rcNormalPosition.top = rcManu.top - (rcBtn.bottom-rcBtn.top) - SUB_DLG_OFFSET_Y*2;
	wndpl.rcNormalPosition.bottom = rcManu.top -1;*/
	this->MoveWindow(&rcThis);
	//this->MoveWindow(&rcThis);	
}

//! �ڽ� Dialog�� ��ġ, ũ�⸦ �����Ѵ�. 
//! [���� ����] setInterface �Լ� ȣ�� �Ŀ� ����� �־�� �Ѵ�. 
void CCcdDlg::setInterface_Child_Dialog()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	CRect rcThis, rcBtn, rcManu;

	GetWindowRect(rcThis);
	GetWindowRect_FromUI(IDC_LABEL_MENU, pFrame, rcManu);
	GetWindowRect_FromUI(IDC_BUTTON_CCD_SFR, this, rcBtn);

	CRect rcChildDlg;
	rcChildDlg.left   = rcThis.left;
	rcChildDlg.right  = rcThis.right;

	rcChildDlg.top    = pFrame->m_rectCamDispPos2.top;
	rcChildDlg.bottom = rcManu.top - (rcBtn.bottom-rcBtn.top) - SUB_DLG_OFFSET_Y*2-1-100;	

	/*if ( m_pOSChkDlg != NULL )
	{
		CRect rcTemp;
		rcTemp.left = pFrame->m_rectCamDispPos1.left;
		rcTemp.right = pFrame->m_rectCamDispPos2.right;
		rcTemp.top = pFrame->m_rectCamDispPos1.top - SUB_DLG_OFFSET_Y;
		rcTemp.bottom = MAIN_DLG_SIZE_Y-5 - 95;

		m_pOSChkDlg->MoveWindow(&rcTemp);
	}*/
	if ( m_pSFRDlg != NULL)
	{
		m_pSFRDlg->MoveWindow(&rcChildDlg);
	}

	if ( m_pInspModeDlg != NULL )
	{
		m_pInspModeDlg->MoveWindow(&rcChildDlg);
	}
}

//! �ڽ� Dialog ����
int CCcdDlg::Make_Child_Dialog()
{
	Delete_Child_Dialog();

	BOOL bCreate = FALSE;

	////////////////////////////////////////////////////////////
	/*m_pOSChkDlg  = new CCCDOSChkDlg;
	if ( m_pOSChkDlg == NULL )
	{
		return -9;
	}
	bCreate = m_pOSChkDlg->Create(IDD_DIALOG_CCD_OSCHECK);
	if ( bCreate == FALSE )
	{
		return -10;
	}
	m_pOSChkDlg->ShowWindow(SW_HIDE);*/

	////////////////////////////////////////////////////////////
	m_pSFRDlg  = new CCCDSFRDlg;
	if ( m_pSFRDlg == NULL )
	{
		return -11;
	}
	bCreate = m_pSFRDlg->Create(IDD_DIALOG_CCD_SFR);
	if ( bCreate == FALSE )
	{
		return -12;
	}
	m_pSFRDlg->ShowWindow(SW_HIDE);

	////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	m_pInspModeDlg = new CCCDInspModeDlg;
	if( m_pInspModeDlg == NULL )
	{
		return -13;
	}
	bCreate = m_pInspModeDlg->Create(IDD_DIALOG_MANDO_INSP);
	if ( bCreate == FALSE )
	{
		return -14;
	}
	m_pInspModeDlg->ShowWindow(SW_HIDE);

	return 1;
}

//! �ڽ� Dialog ����
void CCcdDlg::Delete_Child_Dialog()
{
	/*if ( m_pOSChkDlg != NULL )
	{
		m_pOSChkDlg->DestroyWindow();
		delete m_pOSChkDlg;
		m_pOSChkDlg = NULL;
	}*/
	if ( m_pSFRDlg != NULL )
	{
		m_pSFRDlg->DestroyWindow();
		delete m_pSFRDlg;
		m_pSFRDlg = NULL;
	}
	if( m_pInspModeDlg != NULL )
	{
		m_pInspModeDlg->DestroyWindow();
		delete m_pInspModeDlg;
		m_pInspModeDlg = NULL;
	}
}

//! �� Child Dialog�� ���� ���̱�/�����, Child Dialog�� �����ֱ� ���� ��ư�� ����/���� ���¸� �׷� �ش�. 

bool CCcdDlg::SetVisible_Child_Dialog(int iMode_View, bool bFlag_Visible)
{
	bool bRet = true;

	switch(iMode_View)
	{
	case CCD_VIEW_OSCHECK:
		{
			/*if ( bFlag_Visible == true )
			{
				m_pOSChkDlg->ShowWindow(SW_SHOW);
				m_btnOSChk.m_iStateBtn = 1;
			}
			else
			{
				m_pOSChkDlg->ShowWindow(SW_HIDE);
				m_btnOSChk.m_iStateBtn = 0;
			}

			m_btnOSChk.Invalidate();*/
		}
		break;
	case CCD_VIEW_SFR:
		{
			if ( bFlag_Visible == true )
			{
				m_pSFRDlg->ShowWindow(SW_SHOW);
				m_btnSFR.m_iStateBtn = 1;
			}
			else
			{
				m_pSFRDlg->ShowWindow(SW_HIDE);
				m_btnSFR.m_iStateBtn = 0;
			}

			m_btnSFR.Invalidate();
		}
		break;
	case CCD_VIEW_DEFECT:
		{

		}
		break;
	case CCD_VIEW_INSP_MODE:
		{
			if ( bFlag_Visible == true )
			{
				m_pInspModeDlg->ShowWindow(SW_SHOW);
				m_btnInspMode.m_iStateBtn = 1;
			}
			else
			{
				m_pInspModeDlg->ShowWindow(SW_HIDE);
				m_btnInspMode.m_iStateBtn = 0;
			}

			m_btnInspMode.Invalidate();
		}
		break;
	default:
		{
			bRet = false;
		}
		break;
	}

	return bRet;
}

//! ��� Child Dialog�� ���� ������ �ʰ� �Ѵ�. 
void CCcdDlg::Hide_All_Child_Dialog()
{
	const int iSize_View = 5;
	int i = 0;

	for ( i = 0; i < iSize_View; i++ )
	{
		SetVisible_Child_Dialog(i, false);
	}
}

//! ���� ���̴� �ڽ� Dialog�� ��ü �۾�
void CCcdDlg::SetView_Child_Dialog(int iMode_View)
{
	const int iSize_View = 5;
	int i = 0;

	//! ���õ��� ���� Child Dialog�� �����. 
	for ( i = 0; i < iSize_View; i++ )
	{
		if ( iMode_View != i )
		{
			SetVisible_Child_Dialog(i, false);
		}
	}

	//! ��û���� Child Dialog�� ���� ���̰� �ϸ� true�� ����
	bool bFlag_Mode = SetVisible_Child_Dialog(iMode_View, true);	

	if ( bFlag_Mode == true )
	{
		m_iMode_View = iMode_View;
	}
}

//! [���] ��Ʈ�� �����Ѵ�. 
//! [��ȯ��] �۾� �� ������ �߻��ϸ� false�� ��ȯ�Ѵ�.
bool CCcdDlg::CreateFont_Child_Dlg_Btn()
{
	CDC *pDC = GetDC();

	if ( pDC == NULL )
	{
		return false;
	}

	int i = 0;
	int iFailed = -1;
	const int iBufSize_Font = 2;
	const int iIndex_Normal = 0;
	const int iIndex_Selected = 1;

	BOOL abRet[iBufSize_Font];
	LOGFONT alfTemp[iBufSize_Font];

	for ( i = 0; i < iBufSize_Font; i++ )
	{
		::ZeroMemory(&(alfTemp[i]), sizeof(alfTemp[i]));
	}

	//! ��Ʈ ����

	//! �ش� ��ư�� ���� ������ ���� ��Ʈ
	alfTemp[iIndex_Normal].lfHeight = -MulDiv(9, ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 72);
	alfTemp[iIndex_Normal].lfWeight = FW_MEDIUM;
	_tcscpy_s(alfTemp[iIndex_Normal].lfFaceName, _T("Arial"));

	//! �ش� ��ư�� ���õ� ������ ���� ��Ʈ
	alfTemp[iIndex_Selected].lfHeight = -MulDiv(10, ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 72);
	alfTemp[iIndex_Selected].lfWeight = FW_EXTRABOLD;
	_tcscpy_s(alfTemp[iIndex_Selected].lfFaceName, _T("Arial"));

	//! ��Ʈ ����
	for ( i = 0; i < iBufSize_Font; i++ )
	{
		switch(i)
		{
		case iIndex_Normal:
			{
				abRet[i] = m_ftNormal.CreateFontIndirect( &(alfTemp[i]) );
			}
			break;
		case iIndex_Selected:
			{
				abRet[i] = m_ftSelected.CreateFontIndirect( &(alfTemp[i]) );
			}
			break;
		}
	}//! for ( i = 0; i < iBufSize_Font; i++ )

	ReleaseDC(pDC);

	for ( i = 0; i < iBufSize_Font; i++ )
	{
		if ( abRet[i] == FALSE )
		{
			iFailed = i;
			break;
		}
	}
	if ( iFailed >= 0 )
	{
		return false;
	}

	return true;
}

void CCcdDlg::OnBnClickedButtonCcdOscheck()
{
	SetView_Child_Dialog(CCD_VIEW_OSCHECK);
}


void CCcdDlg::OnBnClickedButtonCcdSfr()
{
	SetView_Child_Dialog(CCD_VIEW_SFR);
}


void CCcdDlg::OnBnClickedButtonCcdDefect()
{
	SetView_Child_Dialog(CCD_VIEW_DEFECT);
}


//! Added by LHW (2013/2/26)
void CCcdDlg::OnTimer(UINT_PTR nIDEvent)
{
	//! ȭ���� ���� ������ ������, �Ʒ��� ������ �ʿ䰡 ����
	if ( this->IsWindowVisible() == FALSE )
	{
		return;
	}
	if(nIDEvent == nTimer_Frame_Grabber_Diagnostics)
	{
//		CString sFPS_FrameGrabber = _T("0");
//
//		//! PC�� ����� LPMC-500 Frame Grabber�� ���� ��
//		if ( MIU.m_iDeviceCount > 0 )
//		{	
//			int iIndex_Device = MIU.GetIndexDevice();
//
//			//! ����� LPMC-500 Frame Grabber�� index�� ��ȿ�� ��
//			if ( iIndex_Device >= 0 && iIndex_Device < MIU.m_iDeviceCount )
//			{
//				//! LPMC-500 Frame Grabber�� Open�Ǿ� �ְ�, ������ Play ���� ��
//				if ( gMIUDevice.bMIUOpen != 0 && gMIUDevice.CurrentState == 4 )
//				{
//					/*unsigned int nFrameCount;
//					float        fFrameRate;							
//					float        fMbps;
//					float        fSensorFrameRate;*/
//
//					//MIUGetDiagnostics(iIndex_Device ,&nFrameCount, &fFrameRate, &fMbps);
//					//MIUGetSensorFrameRate(iIndex_Device, &fSensorFrameRate);
//					 
//					//if(fSensorFrameRate>0)sFPS_FrameGrabber.Format(_T("%.2f"), fSensorFrameRate);
////					if(MIU.m_fRate_Display>0)sFPS_FrameGrabber.Format(_T("%.2f"), MIU.m_fRate_Display);
////					if(MIU.m_FrameRate>0)sFPS_FrameGrabber.Format(_T("%.2f"), MIU.m_FrameRate);
//				}
//			}
//		} 
		//SetString_to_UI(IDC_EDIT_FG_FPS, this, sFPS_FrameGrabber);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CCcdDlg::SetAlignData_Tilt(double dx, double dy)
{
	m_pSFRDlg->SetAlignData_Tilt(dx, dy);
}

void CCcdDlg::SetAlignData_Shift(double dx, double dy)
{
	m_pSFRDlg->SetAlignData_Shift(dx, dy);
}

void CCcdDlg::OnBnClickedButtonMandoInsp()
{
	SetView_Child_Dialog(CCD_VIEW_INSP_MODE);
}


void CCcdDlg::DisplayFrameRate(int iFrameRate)
{
	CString sFPS_FrameGrabber = _T("-");
	sFPS_FrameGrabber.Format(_T("%d"), iFrameRate);
	SetString_to_UI(IDC_EDIT_FG_FPS, this, sFPS_FrameGrabber);
}
