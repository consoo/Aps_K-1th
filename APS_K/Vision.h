#pragma once


#include "stdafx.h"
#ifdef  _M_X64
	LONG64 MFTYPE ProcessingFunction(LONG64 HookType, MIL_ID HookId, void MPTYPE *HookDataPtr);
#else 
	long MFTYPE ProcessingFunction(long HookType, MIL_ID HookId, void MPTYPE *HookDataPtr);
#endif


typedef struct
{
	int		liveMode;
	MIL_ID  MilGrabImage;
	MIL_ID  MilSmallImage;
} HookDataStruct;


#define		MAX_BUFFER_SIZE			10		// MdigProcess ���� ��� �� ���� ���� ����..


#define MAX_PIXEL_CNT	50000
#define MAX_CROSS_CNT	10000	
#define MAX_BOX_CNT		20000
#define MAX_LINE_CNT	10000
#define MAX_TEXT_CNT	10000

//! MbufBayer �Լ����� ����� �Է� ������ ������ ����
//! Added by LHW (2013/3/13)
const int iBufSize_Bayer = 3;

class CElemPixel
{
public:
	int iCnt;
	CPoint p[MAX_PIXEL_CNT];
	COLORREF c[MAX_PIXEL_CNT];


	CElemPixel(){iCnt=0;}
	~CElemPixel(){}

	void addList(int iPosX, int iPosY, COLORREF color);
	void addList(CPoint Pos, COLORREF color);
	void draw(HDC OverlayDC, double dZoomFactorX, double dZoomFactorY);
};

class CElemCross
{
public:
	int iCnt;
	int iSize[MAX_CROSS_CNT];
	CPoint p[MAX_CROSS_CNT];
	COLORREF c[MAX_CROSS_CNT];


	CElemCross(){iCnt=0;}
	~CElemCross(){}

	void addList(int iPosX, int iPosY, int w, COLORREF color);
	void addList(CPoint Pos, int w, COLORREF color);
	void draw(HDC OverlayDC, double dZoomFactorX, double dZoomFactorY);
};

class CElemBox
{
public:
	int iCnt;
	int iStyle[MAX_BOX_CNT];
	CRect r[MAX_BOX_CNT];
	COLORREF c[MAX_BOX_CNT];

	CElemBox(){iCnt=0;}
	~CElemBox(){}

	void addList(int iLeft, int iTop, int iRight, int iBottom, int iLineStyle, COLORREF color);
	void addList(CRect rBox, int iLineStyle, COLORREF c);	
	void draw(HDC OverlayDC, double dZoomFactorX, double dZoomFactorY, bool largeFlag);
};
class CElemEllipse
{
public:
	int iCnt;
	int iStyle[MAX_BOX_CNT];
	CPoint cp1[MAX_BOX_CNT];
	CPoint cp2[MAX_BOX_CNT];
	COLORREF c[MAX_BOX_CNT];

	CElemEllipse(){iCnt=0;}
	~CElemEllipse(){}

	void addList(CPoint pos1,CPoint pos2, int iLineStyle, COLORREF c);	
	void draw(HDC OverlayDC, double dZoomFactorX, double dZoomFactorY);
};
class CElemLine
{
public:
	int iCnt;
	int iStyle[MAX_LINE_CNT];
	CPoint sp[MAX_LINE_CNT];
	CPoint ep[MAX_LINE_CNT];
	COLORREF c[MAX_LINE_CNT];

	CElemLine(){iCnt=0;}
	~CElemLine(){}

	void addList(int iSx, int iSy, int iEx, int iEy, int iLineStyle, COLORREF color);
	void draw(HDC OverlayDC, double dZoomFactorX, double dZoomFactorY);
};

class CElemText
{
public:
	int iCnt;
	CPoint p[MAX_TEXT_CNT];
	CPoint s[MAX_TEXT_CNT];
	CString str[MAX_TEXT_CNT];
	CString FontName[MAX_TEXT_CNT];
	COLORREF c[MAX_TEXT_CNT];

	CElemText(){iCnt=0;}
	~CElemText(){}

	void addList(int iPosX, int iPosY, CString Str, COLORREF color, int iSizeX, int iSizeY, CString FontStr);
	void draw(HDC OverlayDC, double dZoomFactorX, double dZoomFactorY);
};

class CVision
{
public:
	int m_iDispMode;									/* 0:���� ����, 1:���� �˻� ����ȭ ����, 2:LensShade ����..*/
	int m_iThValue;

	MIL_ID		MilApplication;							/* MilApplition id*/
	MIL_ID		MilSystem;								/* MilSystem id */
	MIL_ID		MilDigitizer;							/* MilDigitizer id */

	CCriticalSection m_csGrab;
	CCriticalSection m_acsGrabLock[2];					//! MilGrabImage ��ȣ��, Added by LHW (2013/2/26)
	CCriticalSection m_acsAlignGrabLock;				//! MilGrabImage ��ȣ��, Added by LHW (2013/2/26)

	MIL_ID			 MilGrabImage[2];					/* ȹ�� ���� Copy �� */	
	MIL_ID			 MilGrabImageChild[Cam_Cnt*2];
	MIL_ID			 MilGrabBufferList[MAX_BUFFER_SIZE];	/* ���� ȹ�� �� ���� */
	HookDataStruct	 UserHookData;
	SYSTEMTIME		 m_aTimeGrab[2];					//! ���� ������� ���� �ð�, Added by LHW (2013/2/26)


	MIL_ID		MilProcImage[2];						/* ���� ó���� ���� */
	MIL_ID		MilProcImageChild[Cam_Cnt*2];			/* ���� ó���� ���� ���ϵ� */
	SYSTEMTIME	m_aTimeGrab_Proc[2];					//! ���� ������� ���� �ð�, Added by LHW (2013/2/26)

	CCriticalSection m_csProcImageCV;					//! Process�� Open CV �̹��� ���� ��ȣ��
	SYSTEMTIME		 m_TimeProcImageCV;					//! Process�� Open CV �̹��� ���� ��� �ð�

	//! Added by LHW (2013/3/12)
	SYSTEMTIME  m_aTimeBayer [iBufSize_Bayer];			//! ���� ������� ���� �ð�
	MIL_ID      MilBayerImage[iBufSize_Bayer];			//! CCD Sensor���� ����� Bayer ���� �̹��� ����, Added by LHW (2013/3/12)
	MIL_ID		MilWBCoefficients;						//! Array for the white balance coefficients, Added by LHW (2013/3/12)
	int			m_iIndex_Cvt_Clr_Working;				//! Color ��ȯ ���� ������ index, 0 based
	int			m_iIndex_Cvt_Clr_Ready;					//! Color ��ȯ �غ� �Ϸ�� ������ index, 0 based
	int			m_iIndex_Cvt_Clr_Used;					//! Color ��ȯ �Ϸ�� ������ index, 0 based
	bool		m_bFlag_Use_WB_Coeff;					//! true�̸�, MilWBCoefficients ����Ѵ�. 


	MIL_ID		MilOptImage;							/* AutoRun �� ���� ���� Display�� */
	MIL_ID		MilOptDisplayImage;
	MIL_ID		MilOptOverlayImage;
#ifdef  _M_X64
	LONG64		TransparentColor_Opt;
#else 
	long		TransparentColor_Opt;
#endif
	
	MIL_ID		MilSfrTargetImage;
	MIL_ID		MilDefectImage;							/* AutoRun �� �̹� �˻� Display�� */
	MIL_ID		MilDefectDisplayImage;
	MIL_ID		MilDefectOverlayImage;
#ifdef  _M_X64
	LONG64		TransparentColor_Defect;
#else 
	long		TransparentColor_Defect;
#endif
	
	MIL_ID      MilSaveDisplayDefectImage;				//! �̹� �˻� ��� �̹��� (ȭ�鿡 Display �Ǵ� ���� �̹��� ũ��� ����), Added by LHW (2013/4/1)

	//! �̹� �˻翡�� ��� Added by LHW (2013/3/6)

	MIL_ID		MilSmallImage[2];						/* ���÷��̿� ���� */
	MIL_ID		MilSmallImageChild[Cam_Cnt*2];			/* ���÷��̿� ���� ���ϵ� */

	MIL_ID		MilDisplay[MAX_CAM_NO];					/* Mildisplay id */

	MIL_ID		MilOverlayImage[MAX_CAM_NO];			/* MilOverlayImage id */
#ifdef  _M_X64
	LONG64		TransparentColor[MAX_CAM_NO];			/* Transparentcolor id */
#else 
	long		TransparentColor[MAX_CAM_NO];			/* Transparentcolor id */
#endif
	

	//unsigned char* Image[Cam_Cnt*2];
	unsigned char* MilImageBuffer[Cam_Cnt * 2];



	MIL_ID			MilSmallShadeImage;					/* Lens Shade ���� �� */	
	MIL_ID			MilShadeImage;						/* Lens Shade ���� �� */	
	unsigned char*	ImageShade;



	MIL_ID		MilMarkImage[3][2];
	MIL_ID		MilMarkSmallImage[3][2];
	MIL_ID		MilMarkDisplay[3][2];

	MIL_ID		MilTempImage;

	MIL_ID		PatModel[3][2];						/* ���ϸ�Ī ��ũ �� */
	MIL_ID		PatResult[3][2];					/* ���ϸ�Ī ��� */
	MIL_ID		ModModel[3][2];						/* ������Ʈ�� ��ũ �� */
	MIL_ID		ModResult[3][2];					/* ������Ʈ�� ��� */
	MIL_ID		ChartCenterPatternImage;			/* �߽��� Ȯ�ο� ���ϸ�Ī ��ũ �� */
	MIL_ID		ChartCenterPatternResult;			/* �߽��� Ȯ�ο� ���ϸ�Ī ��� */

	MIL_ID		FieldPatternImage[COMMON_LAST_MARK_CNT];// LAST_MARK_CNT];			/* �ʵ� ROI�� ���ϸ�Ī ��ũ �� */
	MIL_ID		FieldPatternResult[COMMON_LAST_MARK_CNT];// LAST_MARK_CNT];			/* �ʵ� ROI�� ���ϸ�Ī ��� */

	MIL_ID		MilMask;
	HANDLE		hMaskBuf;
	unsigned char* ucMaskBuf;

	MIL_ID		MilMarkSetImage;
	MIL_ID		MilMarkSetDisplay;
	MIL_ID		MilMarkSetOverlayImage;
#ifdef  _M_X64
	LONG64		MarkSetTransparentColor;
#else 
	long		MarkSetTransparentColor;
#endif
	

	CElemPixel  pixellist[MAX_CAM_NO];
	CElemCross	crosslist[MAX_CAM_NO];
	CElemBox	boxlist[MAX_CAM_NO];
	CElemLine	linelist[MAX_CAM_NO];
	CElemText	textlist[MAX_CAM_NO];
	CElemEllipse ellipselist[MAX_CAM_NO];

	double		m_dReduceFactorX, m_dReduceFactorY;		// ī�޶� ����� �ػ󵵰� �� ���� Ʋ���Ƿ�, ��갪�� ������ ������ ����..
	

	bool initVB();										/* Vision Alloc		  */
	void closeVB();										/* Vision Close		  */
	bool initDisplay();									/* Display Alloc	  */
	void initMarkDisplay(int iSizeX, int iSizeY);		/* Mark Display Alloc */
	void InitMarkSetDisplay(int iSizeX, int iSizeY);	/* MarkSet Alloc	  */
	void enableOverlay();								/* �������� ����		  */
	void enableMarkSetOverlay();						/* MarkSet Overlay    */

	void InitBuffer_AutoDisplay(int iSizeX, int iSizeY);		/* AutoRun Display�� */
	void enableOverlay_AutoDisp();								/* AutoRun Display�� */
	void DeleteBuffer_AutoDisp();								/* AutoRun Display�� */
	void GetBuffer_AutoDisp(int iMode, 
							double dFactorX, 
							double dFactorY);					/* AutoRun �� �̹��� ��� */

	void StartGrab();
	void StopGrab();
	
	void CloseMarkSetDisplay();

	void clearOverlay(int iCamNo=999);		/* �������� Ŭ���� */
 	void drawOverlay(int iCamNo=999, bool largeFlag=false, bool BaseFlag=true);		/* �������� �׸��� */
 	void drawBase(int iCamNo=999);			/* ���̽� �׸��� */
	void drawResult(int iCamNo, int iMarkNo, CDPoint Center, double dRate, double dAngle, int iResult);
	void drawResolResult(int iCamNo, int iSizeX, int iSizeY, CDPoint Center, int iResult);
	//! <---------------------------------------
	//! Added by LHW (2013/2/5)
	bool ChangeColor_Box(int iCamNo, COLORREF c);
	void ChangeColor_Box_All(COLORREF c);
	//! <---------------------------------------

	int	 findMark(int iCamNo, int iMarkType, bool flag = false);
	int	 findComAlignMark(int iCamNo, int iMarkType, bool flag = false);
	//����ǰ��� Align
	
	int	 patternMatching(int iSearchMode, int iCh, int iMarkType, int iMarkNo, CDPoint& Center, double& dRate, double& dAngle);			/* ���� ��Ī */
	int	 geometricModelFinder(int iSearchMode, int iCh, int iMarkType, int iMarkNo, CDPoint& Center, double& dRate, double& dAngle);	/* ������Ʈ�� */
	bool markRegist(int iCamNo, int iMarkType, int iMarkNo, CRect rBox);																/* ��ũ ��� */
	void geometricMarkPreProc(int iCamNo, int iMarkType, int iMarkNo);

    CRect m_clRectFov[MAX_FOV_COUNT];
    CRect m_clRectSnr[MAX_SNR_COUNT];

    
	void loadImage(int iCamNo, CString DirStr);				/* �����̹��� �ε� */
	void saveImage(int iCamNo, CString DirStr);				/* �̹��� ���Ϸ� ���� */
	void loadImageonExplorer(int iCamNo);					/* Ž���� �󿡼� �̹��� �ε� */
	void saveImageonExplorer(int iCamNo);					/* Ž���� �󿡼� �̹��� ���� */
	void saveRealImage(int iCamNo, bool bResult=false);		/* ���� ���� ���� */
	void saveCaptureImage(bool bResult=false);				/* ĸó �̹��� ����*/

	void setLiveMode(int flag);
	int  getLiveMode();

	void getSnapImage(int camNo);

	void MiuBufferFree();
	void MiuCCdBufferReset();
	bool MiuBufferAlloc(int fi_Width, int fi_Height);
	void MiuBufferUpdate();		//! Added by LHW (2013/2/26)
	void MiuBufferIndexReady();	//! Added by LHW (2013/3/13)

	void MilBufferUpdate();		//! MilGrab -->MilProcImage

	CRect rcFindROI[MAX_CAM_NO][MAX_MARK];


    bool m_FovSetMode;          //Fov ���� ���̰�
    bool m_SnrSetMode;          //Snr ���� ���̰�

	CVision()
	{
        m_FovSetMode = false;          //Fov ���� ���̰�
        m_SnrSetMode = false;          //Snr ���� ���̰�


		m_iDispMode = 0;
		m_iThValue = 150;

		int i = 0;

		for(i=0; i<MAX_CAM_NO; i++)
		{
			pixellist[i].iCnt = crosslist[i].iCnt = boxlist[i].iCnt = linelist[i].iCnt = textlist[i].iCnt = 0;

			MilDisplay[i] = M_NULL;
			MilOverlayImage[i] = M_NULL;
		}

		MilApplication = M_NULL;
		MilSystem = M_NULL;
		MilDigitizer = M_NULL;

		for ( i = 0; i < iBufSize_Bayer; i++ )
		{
			MilBayerImage[i] = M_NULL;
		}		

		m_bFlag_Use_WB_Coeff = false;


		for ( i = 0; i < 2; i++ )
		{
			MilGrabImage[i] = M_NULL;
			MilProcImage[i] = M_NULL;
			MilSmallImage[i] = M_NULL;
		}

		for ( i = 0; i < (Cam_Cnt*2); i++ )
		{
			MilGrabImageChild[i] = M_NULL;
			MilProcImageChild[i] = M_NULL;
			MilSmallImageChild[i] = M_NULL;
		}

		for ( i = 0; i < MAX_BUFFER_SIZE; i++ )
		{
			MilGrabBufferList[i] = M_NULL;
		}

		MilTempImage = M_NULL;

		MilOptImage = M_NULL;
		MilOptDisplayImage = M_NULL;
		MilOptOverlayImage = M_NULL;

		MilDefectImage = M_NULL;
		MilDefectDisplayImage = M_NULL;
		MilDefectOverlayImage = M_NULL;
		MilSaveDisplayDefectImage = M_NULL;

		MiuBufferIndexReady();
	}
};