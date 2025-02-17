// CCDInspModeDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CCDInspModeDlg.h"
#include "./Library/Inspection/Include/LibACMISSoftISP/SoftISP.h"
//
#include "CcdSfrSpecDlg.h"
#include "CcdDefectSpecDlg.h"
#include "CcdRiOcSpecDlg.h"
#include "CcdBlemishSpecDlg.h"
#include "CcdChartSpecDlg.h"
#include "CcdRISpecDlg.h"
#include "CcdSnrColorSensSpecDlg.h"
//
#include "AutoDispDlg.h"
extern CAutoDispDlg* autodispDlg;
// CCCDInspModeDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCCDInspModeDlg, CDialogEx)

CCCDInspModeDlg::CCCDInspModeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCCDInspModeDlg::IDD, pParent)
{

}

CCCDInspModeDlg::~CCCDInspModeDlg()
{
}

void CCCDInspModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCCDInspModeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_INSP_PIXEL_DEFECT, &CCCDInspModeDlg::OnBnClickedBtnInspPixelDefect)
	ON_BN_CLICKED(IDC_BTN_INSP_STAIN, &CCCDInspModeDlg::OnBnClickedBtnInspStain)
	ON_BN_CLICKED(IDC_BTN_INSP_UNIFORMITY, &CCCDInspModeDlg::OnBnClickedBtnInspUniformity)
	ON_BN_CLICKED(IDC_BTN_INSP_CURRENT, &CCCDInspModeDlg::OnBnClickedBtnInspCurrent)
	ON_BN_CLICKED(IDC_BTN_INSP_TEST_PATTERN, &CCCDInspModeDlg::OnBnClickedBtnInspTestPattern)
	ON_BN_CLICKED(IDC_BTN_INSP_MTF, &CCCDInspModeDlg::OnBnClickedBtnInspMtf)
    ON_BN_CLICKED(IDC_BTN_SPEC_DEFECT, &CCCDInspModeDlg::OnBnClickedBtnSpecDefect)
    ON_BN_CLICKED(IDC_BTN_SPEC_OC, &CCCDInspModeDlg::OnBnClickedBtnSpecOc)
    ON_BN_CLICKED(IDC_BTN_SPEC_BLEMISH, &CCCDInspModeDlg::OnBnClickedBtnSpecBlemish)
    ON_BN_CLICKED(IDC_BTN_SPEC_FOV, &CCCDInspModeDlg::OnBnClickedBtnSpecFov)
    ON_BN_CLICKED(IDC_BUTTON_CCD_RELATIVEI_SPEC, &CCCDInspModeDlg::OnBnClickedButtonCcdRelativeiSpec)
    ON_BN_CLICKED(IDC_BTN_SPEC_SNR, &CCCDInspModeDlg::OnBnClickedBtnSpecSnr)
	ON_BN_CLICKED(IDC_BTN_INSP_COLOR_UNIFORMITY, &CCCDInspModeDlg::OnBnClickedBtnInspColorUniformity)
	ON_BN_CLICKED(IDC_BTN_INSP_RI, &CCCDInspModeDlg::OnBnClickedBtnInspRi)
	ON_BN_CLICKED(IDC_BTN_INSP_SNR, &CCCDInspModeDlg::OnBnClickedBtnInspSnr)
	ON_BN_CLICKED(IDC_BTN_INSP_COLOR_SENSITIVITY, &CCCDInspModeDlg::OnBnClickedBtnInspColorSensitivity)
	ON_BN_CLICKED(IDC_BTN_INSP_FOV_DISTORTION, &CCCDInspModeDlg::OnBnClickedBtnInspFovDistortion)
	ON_BN_CLICKED(IDC_BTN_SPEC_MTF, &CCCDInspModeDlg::OnBnClickedBtnSpecMtf)
    ON_BN_CLICKED(IDC_BTN_INSP_DISTORTION, &CCCDInspModeDlg::OnBnClickedBtnInspDistortion)
	ON_BN_CLICKED(IDC_BTN_INSP_SATURATION, &CCCDInspModeDlg::OnBnClickedBtnInspSaturation)
END_MESSAGE_MAP()


// CCCDInspModeDlg �޽��� ó�����Դϴ�.

bool CCCDInspModeDlg::func_Check_MIU_Mode()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
#ifdef NORINDA_MODE
	return true;
#endif
	if(Task.AutoFlag == 1)
	{
		sLangChange.LoadStringA(IDS_STRING295);
		pFrame->putListLog(sLangChange);
		return false;
	}
	if(Task.AutoFlag == 2)
	{
		sLangChange.LoadStringA(IDS_STRING294);
		pFrame->putListLog(sLangChange);
		return false;
	}

	if ( gMIUDevice.bMIUOpen != 1 )
	{
		sLangChange.LoadStringA(IDS_STRING293);
		pFrame->putListLog(sLangChange);
		return false;
	}

	/*if( gMIUDevice.bMIUInit != 1 )
	{
		sLangChange.LoadStringA(IDS_STRING292);
		pFrame->putListLog(sLangChange);
		return false;
	}*/

	if( gMIUDevice.CurrentState != 4)
	{
		sLangChange.LoadStringA(IDS_STRING290);
		pFrame->putListLog(sLangChange);
		return false;
	}

	return true;
}

void CCCDInspModeDlg::OnBnClickedBtnInspPixelDefect()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	if( !func_Check_MIU_Mode() )	return;

}
//-----------------------------------------------------------------------------
//
//	��ư : STAIN �˻�
//
//-----------------------------------------------------------------------------

void CCCDInspModeDlg::OnBnClickedBtnInspStain()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	//if( !func_Check_MIU_Mode() )	return;
	
	if (Task.AutoFlag == MODE_AUTO)
	{
		pFrame->putListLog(_T("[�����˻�] �ڵ� ���� �� ��� �Ұ�"));
		return;
	}

	if (Task.AutoFlag == MODE_PAUSE)
	{
		pFrame->putListLog(_T("[�����˻�] �Ͻ� ���� �� ��� �Ұ�"));
		return;
	}

	MandoInspLog.func_InitData();	//-- Log �ʱ�ȭ

	vision.clearOverlay(CCD);
	vision.drawOverlay(CCD);
	pFrame->putListLog("=====================================stain �����˻� ����");

	
	if (g_clPriInsp.func_Insp_Stain(MIU.m_pFrameRawBuffer, false) == false)
	{
		pFrame->putListLog(_T("[�����˻�] STAIN �˻� ����"));
		return;
	}
	vision.drawOverlay(CCD);
	pFrame->putListLog(_T("[�����˻�] STAIN �˻� �Ϸ�"));
	
}


void CCCDInspModeDlg::OnBnClickedBtnInspUniformity()
{

}
//-----------------------------------------------------------------------------
//
//	��ư : Current
//
//-----------------------------------------------------------------------------
void CCCDInspModeDlg::OnBnClickedBtnInspCurrent()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	//if( !func_Check_MIU_Mode() )	return;

	if (Task.AutoFlag == MODE_AUTO)
	{
		pFrame->putListLog(_T("[�����˻�] �ڵ� ���� �� ��� �Ұ�"));
		return;
	}

	if (Task.AutoFlag == MODE_PAUSE)
	{
		pFrame->putListLog(_T("[�����˻�] �Ͻ� ���� �� ��� �Ұ�"));
		return;
	}
	g_clPriInsp.func_insp_CDP800_Current();
	//if( !pFrame->func_Insp_CurrentMeasure(true, false) )
	//{
	//	sLangChange.LoadStringA(IDS_STRING296);
	//	pFrame->putListLog(sLangChange);
	//	return;
	//}
 //   MandoInspLog.func_InitData();	//-- Log �ʱ�ȭ
	//vision.clearOverlay(CCD);

	//if (g_clPriInsp.func_Insp_CurrentMeasure(true, false) == false)
	//{
	//	pFrame->putListLog(_T("[�����˻�] Current ���� ����"));
	//	return;
	//}
	//vision.drawOverlay(CCD);
	pFrame->putListLog(_T("[�����˻�] Current ���� �Ϸ�"));

}

//-----------------------------------------------------------------------------
//
//	��ư : PIXEL DEFECT
//
//-----------------------------------------------------------------------------
void CCCDInspModeDlg::OnBnClickedBtnInspTestPattern()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	//bool bDefect=pFrame->BrightDefect_Insp(false);// (10����)

	if (Task.AutoFlag == MODE_AUTO)
	{
		pFrame->putListLog(_T("[�����˻�] �ڵ� ���� �� ��� �Ұ�"));
		return;
	}

	if (Task.AutoFlag == MODE_PAUSE)
	{
		pFrame->putListLog(_T("[�����˻�] �Ͻ� ���� �� ��� �Ұ�"));
		return;
	}

	MandoInspLog.func_InitData();	//-- Log �ʱ�ȭ
	vision.clearOverlay(CCD);

	 
	
	if (g_clPriInsp.func_Insp_Defect(MIU.vDefectMidBuffer_6500K, MIU.vDefectLowBuffer, false) == false)
	{
		pFrame->putListLog(_T("[�����˻�] Defect �˻� ����"));
	    return;
    }
	vision.drawOverlay(CCD);
	pFrame->putListLog(_T("[�����˻�] Defect �˻� �Ϸ�"));
}
//-----------------------------------------------------------------------------
//
//	��ư : MTF �˻�
//
//-----------------------------------------------------------------------------

void CCCDInspModeDlg::OnBnClickedBtnInspMtf()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	//if( !func_Check_MIU_Mode() )	return;

	if (Task.AutoFlag == MODE_AUTO)
	{
		pFrame->putListLog(_T("[�����˻�] �ڵ� ���� �� ��� �Ұ�"));
		return;
	}

	if (Task.AutoFlag == MODE_PAUSE)
	{
		pFrame->putListLog(_T("[�����˻�] �Ͻ� ���� �� ��� �Ұ�"));
		return;
	}
	MandoInspLog.func_InitData();	//-- Log �ʱ�ȭ

	pFrame->func_MTF(MIU.m_pFrameRawBuffer ,false);//�����˻�

	pFrame->putListLog("[�����˻�] MTF ���� �Ϸ�"); 
	MandoInspLog.func_LogSave_UVAfter();
	MIU.iImgUpdateFlag = 0;
}



BOOL CCCDInspModeDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.


	m_clSfrSpecDlg.Create(IDD_DIALOG_CCD_SFR_SPEC, this);
	m_clSfrSpecDlg.ShowWindow(SW_HIDE);

    m_clDefectSpecDlg.Create(IDD_DIALOG_CCD_DEFECT_SPEC, this);
    m_clDefectSpecDlg.ShowWindow(SW_HIDE);

    m_clRiOcSpecDlg.Create(IDD_DIALOG_CCD_RI_OC_SPEC, this);
    m_clRiOcSpecDlg.ShowWindow(SW_HIDE);

    m_clBlemishSpecDlg.Create(IDD_DIALOG_CCD_BLEMISH_SPEC, this);
    m_clBlemishSpecDlg.ShowWindow(SW_HIDE);

    m_clChartSpecDlg.Create(IDD_DIALOG_CCD_CHART_SPEC, this);
    m_clChartSpecDlg.ShowWindow(SW_HIDE);

    m_clRiSpecDlg.Create(IDD_DIALOG_CCD_RI_SPEC, this);
    m_clRiSpecDlg.ShowWindow(SW_HIDE);

    m_clSnrColorSensSpecDlg.Create(IDD_DIALOG_CCD_SNR_COLORSENS_SPEC, this);
    m_clSnrColorSensSpecDlg.ShowWindow(SW_HIDE);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//-----------------------------------------------------------------------------
//
//	��ư : DEFECT/UNIFORM
//
//-----------------------------------------------------------------------------
void CCCDInspModeDlg::OnBnClickedBtnSpecDefect()
{
	//m_clDefectSpecDlg.SetUnit(m_nUnit);
    m_clDefectSpecDlg.ShowWindow(SW_SHOW);
}
//-----------------------------------------------------------------------------
//
//	��ư : OC
//
//-----------------------------------------------------------------------------

void CCCDInspModeDlg::OnBnClickedBtnSpecOc()
{
	//m_clRiOcSpecDlg.SetUnit(m_nUnit);
    m_clRiOcSpecDlg.ShowWindow(SW_SHOW);
}

//-----------------------------------------------------------------------------
//
//	��ư : BLEMISH
//
//-----------------------------------------------------------------------------
void CCCDInspModeDlg::OnBnClickedBtnSpecBlemish()
{
	//m_clBlemishSpecDlg.SetUnit(m_nUnit);
    m_clBlemishSpecDlg.ShowWindow(SW_SHOW);
}
//-----------------------------------------------------------------------------
//
//	��ư : FOR/DISTORTION
//
//-----------------------------------------------------------------------------

void CCCDInspModeDlg::OnBnClickedBtnSpecFov()
{
	//m_clChartSpecDlg.SetUnit(m_nUnit);
    m_clChartSpecDlg.ShowWindow(SW_SHOW);
}
//-----------------------------------------------------------------------------
//
//	��ư : RI
//
//-----------------------------------------------------------------------------

void CCCDInspModeDlg::OnBnClickedButtonCcdRelativeiSpec()
{
	//m_clRiSpecDlg.SetUnit(m_nUnit);
	m_clRiSpecDlg.ShowWindow(SW_SHOW);
}
//-----------------------------------------------------------------------------
//
//	��ư : SNR/COLOR SENS
//
//-----------------------------------------------------------------------------

void CCCDInspModeDlg::OnBnClickedBtnSpecSnr()
{
	//m_clSnrColorSensSpecDlg.SetUnit(m_nUnit);
	m_clSnrColorSensSpecDlg.ShowWindow(SW_SHOW);
}
//-----------------------------------------------------------------------------
//
//	��ư : UNIFORMITY
//
//-----------------------------------------------------------------------------

void CCCDInspModeDlg::OnBnClickedBtnInspColorUniformity()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	if (!func_Check_MIU_Mode())	return;

	if (Task.AutoFlag == MODE_AUTO)
	{
		pFrame->putListLog(_T("[�����˻�] �ڵ� ���� �� ��� �Ұ�"));
		return;
	}

	if (Task.AutoFlag == MODE_PAUSE)
	{
		pFrame->putListLog(_T("[�����˻�] �Ͻ� ���� �� ��� �Ұ�"));
		return;
	}

	MandoInspLog.func_InitData();	//-- Log �ʱ�ȭ

	vision.clearOverlay(CCD);

	if (g_clPriInsp.func_Insp_Uniformity(MIU.vDefectMidBuffer_6500K, false) == false)
	{
		pFrame->putListLog(_T("[�����˻�] COLOR UNIFORMITY �˻� ����"));
		return;
	}
	vision.drawOverlay(CCD);
	pFrame->putListLog(_T("[�����˻�] COLOR UNIFORMITY �˻� �Ϸ�"));
}
//-----------------------------------------------------------------------------
//
//	��ư : RI
//
//-----------------------------------------------------------------------------

void CCCDInspModeDlg::OnBnClickedBtnInspRi()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	//if (!func_Check_MIU_Mode())	return;

	if (Task.AutoFlag == MODE_AUTO)
	{
		pFrame->putListLog(_T("[�����˻�] �ڵ� ���� �� ��� �Ұ�"));
		return;
	}

	if (Task.AutoFlag == MODE_PAUSE)
	{
		pFrame->putListLog(_T("[�����˻�] �Ͻ� ���� �� ��� �Ұ�"));
		return;
	}

	//MandoInspLog.func_InitData();	//-- Log �ʱ�ȭ

	vision.clearOverlay(CCD);

	if (g_clPriInsp.func_Insp_Illumination(MIU.m_pFrameRawBuffer, false) == false)
	{
		pFrame->putListLog(_T("[�����˻�] Relative Illumination �˻� ����"));
		return;
	}
	vision.drawOverlay(CCD);
	pFrame->putListLog(_T("[�����˻�] Relative Illumination �˻� �Ϸ�"));
} 

//-----------------------------------------------------------------------------
//
//	��ư : SMR
//
//-----------------------------------------------------------------------------
void CCCDInspModeDlg::OnBnClickedBtnInspSnr()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	//if (!func_Check_MIU_Mode())	return;

	if (Task.AutoFlag == MODE_AUTO)
	{
		pFrame->putListLog(_T("[�����˻�] �ڵ� ���� �� ��� �Ұ�"));
		return;
	}

	if (Task.AutoFlag == MODE_PAUSE)
	{
		pFrame->putListLog(_T("[�����˻�] �Ͻ� ���� �� ��� �Ұ�"));
		return;
	}

	//MandoInspLog.func_InitData();	//-- Log �ʱ�ȭ

	vision.clearOverlay(CCD);
	if (g_clPriInsp.func_Insp_Snr(MIU.m_pFrameRawBuffer, false) == false)
	{
		pFrame->putListLog(_T("[�����˻�] SNR �˻� ����"));
		return;
	} 

	vision.drawOverlay(CCD);
	pFrame->putListLog(_T("[�����˻�] SNR �˻� �Ϸ�"));
}

//-----------------------------------------------------------------------------
//
//	��ư : COLOR SENSITIVITY
//
//-----------------------------------------------------------------------------
void CCCDInspModeDlg::OnBnClickedBtnInspColorSensitivity()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	if (!func_Check_MIU_Mode())	return;

	if (Task.AutoFlag == MODE_AUTO)
	{
		pFrame->putListLog(_T("[�����˻�] �ڵ� ���� �� ��� �Ұ�"));
		return;
	}

	if (Task.AutoFlag == MODE_PAUSE)
	{
		pFrame->putListLog(_T("[�����˻�] �Ͻ� ���� �� ��� �Ұ�"));
		return;
	}

	MandoInspLog.func_InitData();	//-- Log �ʱ�ȭ

	vision.clearOverlay(CCD);
    pFrame->putListLog(_T("[�����˻�] Color Sensitivity MID_6500K_RAW TEST..."));
    if (g_clPriInsp.func_Insp_ColorSensitivity(MIU.vDefectMidBuffer_6500K, MID_6500K_RAW, false) == false)
    {
        pFrame->putListLog(_T("[�����˻�] Color Sensitivity MID_6500K_RAW �˻� ����"));
    }
    pFrame->putListLog(_T("[�����˻�] Color Sensitivity MID_2800K_RAW TEST..."));
    if (g_clPriInsp.func_Insp_ColorSensitivity(MIU.vDefectMidBuffer_2800K, MID_2800K_RAW, false) == false)
    {
        pFrame->putListLog(_T("[�����˻�] Color Sensitivity MID_2800K_RAW �˻� ����"));
    }
	vision.drawOverlay(CCD);
	pFrame->putListLog(_T("[�����˻�] Color Sensitivity �˻� �Ϸ�"));
}
//-----------------------------------------------------------------------------
//
//	��ư : FOV / DISTORTION
//
//-----------------------------------------------------------------------------

void CCCDInspModeDlg::OnBnClickedBtnInspFovDistortion()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

	if (!func_Check_MIU_Mode())	return;

	if (Task.AutoFlag == MODE_AUTO)
	{
		pFrame->putListLog(_T("[�����˻�] �ڵ� ���� �� ��� �Ұ�"));
		return;
	}

	if (Task.AutoFlag == MODE_PAUSE)
	{
		pFrame->putListLog(_T("[�����˻�] �Ͻ� ���� �� ��� �Ұ�"));
		return;
	}

	MandoInspLog.func_InitData();	//-- Log �ʱ�ȭ

	vision.clearOverlay(CCD);

	/*int nPitch, nSizeX, nSizeY;
	TCHAR szPos[SIZE_OF_100BYTE];
	nPitch = (int)MbufInquire(g_clVision.m_MilCcdProcChild[m_nUnit][1], M_PITCH, M_NULL);
	nSizeX = (int)MbufInquire(g_clVision.m_MilCcdProcChild[m_nUnit][1], M_SIZE_X, M_NULL);
	nSizeY = (int)MbufInquire(g_clVision.m_MilCcdProcChild[m_nUnit][1], M_SIZE_Y, M_NULL);
	if (g_FindFovPos(m_nUnit, g_clVision.m_pImgBuff[m_nUnit][1], nPitch, nSizeX, nSizeY, g_clModelData[m_nUnit].m_clSfrInfo.m_clRectFov) == false)
	{
		return;
	}*/

    int _type = 0;
#if   (____MODEL_NAME == ZOOX_WIDE_D || ____MODEL_NAME == GCM2 || ____MODEL_NAME == FCM30_W_PCB)
    //�����϶��� ������ũ
    //������ũ ã��
    _type = DOT_TYPE;
#elif (____MODEL_NAME == M_COB_8M|| ____MODEL_NAME ==  M_TRINITY_WIDE ||____MODEL_NAME == M_GEN2 ||____MODEL_NAME == M_TRINITY )
    _type = GRID_TYPE;
#endif

	if (g_clPriInsp.func_Insp_Fov_Distortion_Rotate(MIU.m_pFrameRawBuffer, _type, false) == false)
	{
		pFrame->putListLog(_T("[�����˻�] Fov �˻� ����"));
		vision.drawOverlay(CCD);
		return;
	}
	vision.drawOverlay(CCD);
	pFrame->putListLog(_T("[�����˻�] Fov �˻� �Ϸ�"));
}

//-----------------------------------------------------------------------------
//
//	��ư : MTP SPEC
//
//-----------------------------------------------------------------------------
void CCCDInspModeDlg::OnBnClickedBtnSpecMtf()
{
	//m_clSfrSpecDlg.SetUnit(m_nUnit);
	m_clSfrSpecDlg.ShowWindow(SW_SHOW);
}


void CCCDInspModeDlg::OnBnClickedBtnInspDistortion()
{
    CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

    if (!func_Check_MIU_Mode())	return;

    if (Task.AutoFlag == MODE_AUTO)
    {
        pFrame->putListLog(_T("[�����˻�] �ڵ� ���� �� ��� �Ұ�"));
        return;
    }

    if (Task.AutoFlag == MODE_PAUSE)
    {
        pFrame->putListLog(_T("[�����˻�] �Ͻ� ���� �� ��� �Ұ�"));
        return;
    }

    MandoInspLog.func_InitData();	//-- Log �ʱ�ȭ

    vision.clearOverlay(CCD);

    if (g_clPriInsp.func_Insp_Fov_Distortion_Rotate(MIU.m_pFrameRawBuffer, GRID_TYPE, false) == false)
    {
        pFrame->putListLog(_T("[�����˻�] Distortion �˻� ����"));
		vision.drawOverlay(CCD);
        return;
    }
    vision.drawOverlay(CCD);
    pFrame->putListLog(_T("[�����˻�] Distortion �˻� �Ϸ�"));
}


void CCCDInspModeDlg::OnBnClickedBtnInspSaturation()
{
	// TODO: Add your control notification handler code here
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	//bool bDefect=pFrame->BrightDefect_Insp(false);// (10����)

	if (Task.AutoFlag == MODE_AUTO)
	{
		pFrame->putListLog(_T("[�����˻�] �ڵ� ���� �� ��� �Ұ�"));
		return;
	}

	if (Task.AutoFlag == MODE_PAUSE)
	{
		pFrame->putListLog(_T("[�����˻�] �Ͻ� ���� �� ��� �Ұ�"));
		return;
	}

	if (g_clPriInsp.func_Insp_Saturation(MIU.m_pFrameRawBuffer, false) == false)
	{
		pFrame->putListLog(_T("[�����˻�] Saturation �˻� ����"));
		return;
	}
	vision.drawOverlay(CCD);
	pFrame->putListLog(_T("[�����˻�] Saturation �˻� �Ϸ�"));
}
