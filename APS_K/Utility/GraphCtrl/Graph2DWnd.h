//! 2D ������ǥ �׷��� ��Ʈ���� ��ü Ŭ����
//! Maked by LHW [2008/3/14 ~ ]
//! ���� �۾��� [2013/4/1]

#pragma once

#include "dib256.h"	//! Added by LHW [2009/2/25]

// CGraph2DWnd

//! 1) ȭ���� MM_TEXT ���� ���� �����Ǿ� �־�� ��� ����
//! 2) ��� Remove~(������ index), Delete~(������ index) �Լ��� ����� �Ŀ��� ������ index ���� ��ҵ��� ������ �̵��ϹǷ�, 
//!    ������ index ���� index�� ��� 1�� ���־�� ��
//! 3) ��� �Լ��� index�� 0 based
//! 4) �׷����� �׷��� �����ʹ� �ݵ�� X�� ���� �����Ͱ� ���������� �ǵ��� �Է��� �־�� ��

#include ".\\Graph2D_Resource.h"//! CGraph2D_Resource, strGraph2DResourceInfo �� ���

class CGraph2D_Data;
class CGraph2DWnd : public CWnd
{
	struct strYCaption	//! Y�� ���� ���ڿ��� ����(E_GRAPH2D_XY_CAPTION_POS_UNITE���� ���)
	{
		CString szCaption;
		COLORREF clrCaption;

		void InitInfo()
		{
			szCaption = _T("");
			clrCaption = RGB(0, 0, 0);
		}

		strYCaption& operator=(const strYCaption &info)  //! = ������ ������
		{
			szCaption = info.szCaption;
			clrCaption = info.clrCaption;

			return *this;
		}
	};

	struct strPanPosition	//! Pan ��ɿ��� ���콺�� ������ �������� ��ħ���� ��ǥ
	{
		int iStartX;
		int iStartY;
		int iEndX;
		int iEndY;

		void InitInfo()
		{
			iStartX = iStartY = 0;
			iEndX = iEndY = 0;
		}

		strPanPosition& operator=(const strPanPosition &info)  //! = ������ ������
		{
			iStartX = info.iStartX;
			iStartY = info.iStartY;
			iEndX = info.iEndX;
			iEndY = info.iEndY;

			return *this;
		}
	};

	struct strYRangeMapper	//! �������� �Ǽ��� ���� ȭ�� ���� ������ ������ ��ȯ�� ��
	{		
		int		m_iYPtMin;		//! MM_TEXT ���� ����̹Ƿ� m_dbYDataMax�� ����
		int		m_iYPtMax;		//! MM_TEXT ���� ����̹Ƿ� m_dbYDataMin�� ����
		double	m_dbYDataMin;
		double	m_dbYDataMax;
			
		double  m_dbCoeff;
	
		void CalcCoeff()	//! ��ȯ ��� ���, CalcYDataToPt, CalcPtToYData�Լ� ���� �ݵ�� ȣ��Ǿ�� ��
		{
			m_dbCoeff = ( (double)(m_iYPtMax - m_iYPtMin) ) / (m_dbYDataMax - m_dbYDataMin);
		}
		
		int	CalcYDataToPt(double dbDrawData)	//! ��ȯ ���
		{
			int iYPt = (int)( m_dbCoeff * (m_dbYDataMax - dbDrawData) ) + m_iYPtMin;

			return iYPt;
		}

		double CalcPtToYData(int iYPt)
		{
			double dbYData = m_dbYDataMax - (((double)(iYPt - m_iYPtMin)) / m_dbCoeff);

			return dbYData;
		}
	};

	struct strXRangeMapper	//! �������� �Ǽ��� ���� ȭ�� ���� ������ ������ ��ȯ�� ��
	{		
		int		m_iXPtMin;		//! m_dbXDataMin�� ����
		int		m_iXPtMax;		//! m_dbXDataMax�� ����
		double	m_dbXDataMin;
		double	m_dbXDataMax;
			
		double  m_dbCoeff;
	
		void CalcCoeff()	//! ��ȯ ��� ���, CalcXDataToPt, CalcPtToXData�Լ� ���� �ݵ�� ȣ��Ǿ�� ��
		{
			m_dbCoeff = ( (double)(m_iXPtMax - m_iXPtMin) ) / (m_dbXDataMax - m_dbXDataMin);
		}
		
		int	CalcXDataToPt(double dbDrawData)	//! ��ȯ ���
		{
			int iXPt = (int)( m_dbCoeff * (dbDrawData - m_dbXDataMin) ) + m_iXPtMin;

			return iXPt;
		}

		double CalcPtToXData(int iXPt)
		{
			double dbXData = (((double)(iXPt - m_iXPtMin)) / m_dbCoeff) + m_dbXDataMin;

			return dbXData;
		}
	};

	DECLARE_DYNAMIC(CGraph2DWnd)

public:
	CGraph2DWnd();
	virtual ~CGraph2DWnd();

	bool m_bChanged;			//! ������ ��� true�� ����
								//! �׷����� ũ�Ⱑ ���� ��, strGraph2DResourceInfo ����ü�� ����� ��, �μ⸦ ��ġ�� ����
								//! �׷��� �����͸� �Է��� ��, Ȯ��/��Ҹ� �� ��
	
	bool m_bMultiPlotOneAxis;	//! true�̸� ���� ���� Plot�� �ִ��� Y���� �� ���� ����
								//! true�̸� Y���� Grid �������� SetResourceYInfo�Լ����� 0���� ������ ����
								//! �׷����� ���̴����� ����, �׷��� ������� SetResourceYInfo�Լ����� ���� ����

	HWND m_hMsgHandle;			//! �׷��� ��Ʈ�ѿ��� ������ ��� �޽����� ���� �ڵ鰪
	bool m_bAutoDrawCompleteMsg;//! true�̸� �׷��� ��Ʈ���� ũ�Ⱑ ���ϰų� �߿� ������ ����Ǹ�, 
								//! ����� ȭ�鿡 �׷����� �׸��� �۾��� �Ϸ��Ŀ� �޽����� ����, Added by LHW [2008/7/01]
	bool m_bDrawCompleteMsg;	//! true�̸� ����� ȭ�鿡 �׷����� �׸��� �۾��� �Ϸ��Ŀ� �޽����� ����, Added by LHW [2008/5/31]
	bool m_bCursorEventMsg;		//! true�̸� Ŀ�� �̺�Ʈ �߻� ������ �޽����� ����
	bool m_bZoomEventMsg;		//! true�̸� Zoom �̺�Ʈ �߻� ������ �޽����� ����, Added by LHW [2008/4/19]	
	bool m_bAxisYEventMsg;		//! true�̸� Y�� �κп��� ���콺 ���� Ŭ���� �ϸ� �޽����� ����, Added by LHW [2008/7/31]
	bool m_bTimeAxisXEventMsg;	//! true�̸� X���� �ð� ǥ���̰�, ���� ���� ���̴� �ּ�, �ִ밪�� ������ �� �޽����� ����, Added by LHW [2008/8/18]
	bool m_bRegionCursorEventMsg;	//! true�̸� ���� ǥ�� Ŀ�� �̺�Ʈ �߻� ������ �޽����� ����, Added by LHW [2008/11/28]
	bool m_bAxisXZoom;			//! true�̸� Alt Ű�� ���� ���¿��� ���콺�� X�� ���� Ȯ��, ��� ����, Added by LHW [2009/2/2]
	bool m_bAxisYzoom;			//! true�̸� Alt Ű�� ���� ���¿��� ���콺�� Y�� ���� Ȯ��, ��� ����, Added by LHW [2009/2/2]
	bool m_bAxisXYZoom;			//! true�̸� Alt Ű�� ���� ���¿��� ���콺�� X, Y�� ���� Ȯ��, ��� ����, Added by LHW [2009/2/2]
	bool m_bAxisXPan;			//! true�̸� Shift Ű�� ���� ���¿��� ���콺�� X�� ���� Ȯ��, ��� ����, Added by LHW [2009/2/2]
	bool m_bAxisYPan;			//! true�̸� Shift Ű�� ���� ���¿��� ���콺�� Y�� ���� Ȯ��, ��� ����, Added by LHW [2009/2/2]
	bool m_bAxisXYPan;			//! true�̸� Shift Ű�� ���� ���¿��� ���콺�� X, Y�� ���� Ȯ��, ��� ����, Added by LHW [2009/2/2]
	bool m_bSysmbolOnceDraw;	//! true�̸� Symbol�� �׷��� ���¿����� ���콺 �̺�Ʈ ������ �׸��� �ʵ��� ��, Added by LHW [2009/2/19]
	bool m_bDataPointEventMsg;	//! true�̸� ���� CtrlŰ�� ���� ���¿��� �׷����� ���� Ŭ���� ������ �޽����� ����, Added by LHW [2008/5/23]
	bool m_bDataPointCusor;		//! true�̸� ���� CtrlŰ�� ���� ���¿��� ���콺 Ŀ���� ����, Added by LHW [2008/5/28]
	bool m_bXAutoRangeShowGraph;//! true�̸� ���� ���̵��� ������ �׷����鸸 ����Ͽ� X���� Min, Max ���� ����
	bool m_bYAutoRangeShowGraph;//! true�̸� ���� ���̵��� ������ �׷����鸸 ����Ͽ� Y���� Min, Max ���� ����
	bool m_bYAutoRangeValueLine;//! true�̸� Y�� Ư���� ǥ�ü��� ��� ���̵��� Y���� Min, Max ���� ����
	bool m_bXAutoRangeEquation;	//! true�̸� �Լ������� �׷����� �׷����� ����Ͽ� X���� Min, Max ���� ����, Added by LHW [2008/8/25]
	bool m_bYAutoRangeEquation;	//! true�̸� �Լ������� �׷����� �׷����� ����Ͽ� Y���� Min, Max ���� ����, Added by LHW [2008/8/25]
	bool m_bXAutoRangeRegionCursor;	//! true�̸� ���� ǥ�� Ŀ���� ����Ͽ� X���� Min, Max ���� ����, Added by LHW [2008/12/14]
	bool m_bYAutoRangeRegionCursor;	//! true�̸� ���� ǥ�� Ŀ���� ����Ͽ� Y���� Min, Max ���� ����, Added by LHW [2008/12/14]
	bool m_bXValueLineRMouseDownChk;	//! Added by LHW [2008/4/19] : true�̸� ���콺 ������ Ŭ���� �� ��, 
										//! X�� Ư�� �� ǥ�ü� ��ó���� Ȯ���Ͽ� ���콺 ������ Ŭ�� ���� �޽����� �̺�Ʈ �߻� ����, LineIndex�� �߰��Ͽ� ����
	bool m_bChildCursorRMouseDownChk;	//! Added by LHW [2008/6/13] : true�̸� ���콺 ������ Ŭ���� �� ��, 
										//! �ڽ� Ŀ�� ��ó���� Ȯ���Ͽ� ���콺 ������ Ŭ�� ���� �޽����� �̺�Ʈ �߻� ������ ����
	bool m_bAuxCursorRMouseDownChk;		//! Added by LHW [2008/8/5] : true�̸� ���콺 ������ Ŭ���� �� ��, 
										//! ���� Ŀ�� ��ó���� Ȯ���Ͽ� ���콺 ������ Ŭ�� ���� �޽����� �̺�Ʈ �߻� ������ ����
	bool m_bRegionCursorRMouseDownChk;	//! Added by LHW [2008/11/28] : true�̸� ���콺 ������ Ŭ���� �� ��, 
										//! ���� ǥ�� Ŀ�� ��ó���� Ȯ���Ͽ� ���콺 ������ Ŭ�� ���� �޽����� �̺�Ʈ �߻� ������ ����
	bool m_bSelectedCursorLine;			//! true�̸� ���õ� Ŀ���� ������ �ٸ��� ǥ��, Added by LHW [2008/5/14]
	bool m_bSelectedChildCursorLine;	//! true�̸� ���õ� �ڽ� Ŀ���� ������ �ٸ��� ǥ��, Added by LHW [2008/6/16]
	bool m_bSelectedRegionCursorLine;	//! true�̸� ���õ� ���� ǥ�� Ŀ���� ������ �ٸ��� ǥ��, Added by LHW [2008/11/28]
	int m_iTopOverlapIndex;			//! ���� ���� �׷����� Overlap�Ǿ� ���� ��, �� ���� ���̴� �׷����� index, -1�̸� ������� �׷� ��, Added by LHW [2008/7/24]
	int m_iShowGridY_OverlapIndex;	//! ���� ���� �׷����� Overlap�Ǿ� ���� ��, �� �߿��� Y�� Grid���� ���� �� �׷����� index, -1�̸� Grid���� �׸��� ����, Added by LHW [2008/7/30]
									//! ���� 1���� ���� ��쿡�� ���ο��� 0���� ����
	bool m_bCloseCursorSelect;	//! true�̸� ���콺 �ٿ��� �� ��, ���� ����� Ŀ���� ���õ�, Added by LHW [2008/7/24]
	E_GRAPH2D_CURSOR_ALL_SEARCH_MODE m_eCursorAllSearchMode;//! Overlap�� �׷������� Ŀ���� ã�� ���, Added by LHW [2008/7/24]
	bool m_bXScaleLock;	//! true�̸� X�� Range ó���� ���� ����, Added by LHW [2008/12/27]
	bool m_bYScaleLock;	//! true�̸� Y�� Range ó���� ���� ����, Added by LHW [2008/12/27]
	bool m_bAlwaysInitYPos;		//! true�̸� �׻� ���������� Init_XPos_YAxis �Լ��� ȣ���Ͽ� Y���� X��ǥ�� �ʱ�ȭ, Added by LHW [2009/2/27]
	bool m_bCursorUpDownSearchOperation;	//! true�̸� Ű���� Up, Down Ű�� ���� ��, m_iCursorUpDownSearchMargin ��ŭ�� �����͸� üũ�Ͽ�
											//! ���� ����� X�� ���� ������ Ŀ���� �̵�, Added by LHW [2009/3/1]
	int m_iCursorUpDownSearchMargin;		//! ������ X�� ���� ������ ���ο� Ŀ���� ã�� ���� �˻� ����, Added by LHW [2009/3/1]
	
	bool m_bDrawBackGroundImage;	//! true�̸� ��� �̹����� �׸�, Added by LHW [2009/2/25]
	CDIBitmap	m_bmpBackground;	//! ��� �̹���, Added by LHW [2009/2/25]
	
	E_GRAPH2D_MOVE_CURSOR_TYPE IsCurrentMoveCursorType();	//! ���������� ���콺, Ű����� ������ Ŀ���� ���� ��ȯ, Added by LHW [2008/6/16]
    
	E_GRAPH2D_MOUSE_OPERATION m_eMouseOperationMode;	//! ���� ���콺�� �����ϴ� ����� ����, �⺻������ Ŀ�� ������ �����Ǿ� ����
	E_GRAPH2D_CURSOR_SEARCH_MODE m_eCursorSearchMode;	//! ���콺�� Ŀ������ ã�� ���, �⺻������ E_GRAPH2D_CURSOR_SEARCH_X_RANGE ����
	E_GRAPH2D_CURSOR_LINE_MODE m_eCursorLineMode;		//! ���� �߰��Ǵ� Ŀ���� Ŀ�� �� ǥ�� ���

	//! Plot�� ���� ���� �Լ�
	void SetPlotIndex(int iPlotIndex);	//! Plot index ����, �ϳ��� ȭ�鿡 ���� ���� �׷����� ������ Plot index�� ���� ����
	int GetPlotIndex() const;			//! Plot index ��ȯ

	bool MakePlotBuffer(int iNewSize);	//! Overlap�Ǵ� �׷��� ���� ������ŭ ���� ����, �� �Լ��� ȣ������ �ʾƵ� �⺻������ 1���� ���� ����
										//! �� �Լ��� ȣ���ϸ� ���� ���۴� �����ϰ� ���� ����
	bool AddPlotBuffer();				//! ���� ���� �ڿ� ���ο� ���۸� �߰�, Added by LHW [2008/4/11]
	int GetPlotBufferSize() const;		//! Overlap�Ǵ� �׷��� ���� �׸��� ������ ������ ��ȯ
	bool RemoveOnePlotBuffer(int iOverlapIndex);	//! �׷��� �ϳ��� ����, iOverlapIndex�� Overlap�Ǵ� �׷����� index (0 based)
	void RemoveAllPlotBuffer();						//! �׷��� ��ü ����

	bool SetResourcePtr(CGraph2D_Resource *pResource);	//! �ܺο��� ���ҽ� ��ü�� ���� (���� ���� �׷����� ���ÿ� �׸� �� ���)
														//! OS���� �Ҵ����ִ� ���ҽ��� �翡�� �Ѱ谡 �����Ƿ�
														//! �� �Լ��� ����ϸ� MakeResourcePtr �Լ��� ������� ����
	bool MakeResourcePtr();				//! ���� CGraph2D_Resource ���ҽ� ��ü�� ����, �׷��� ���� �۰ų� ������ ���� �ؾ� �� ��쿡 ���
	bool ReleaseResourcePtr();			//! ���� ������ CGraph2D_Resource ���ҽ� ��ü�� ����, MakeResourcePtr�Լ��� ¦���� ���

	//! ���콺 Ŀ�� ����
	void SetPanCursor(HCURSOR hNewPanCursor1, HCURSOR hNewPanCursor2);	//! Pan ���ۿ��� ����ϴ� Ŀ�� �ڵ鰪�� ����
																		//! hNewPanCursor1 : Pan ���ۿ��� Mouse Down ���¿����� ���콺 Ŀ��
																		//! hNewPanCursor2 : Pan ���ۿ��� ���� ���¿����� ���콺 Ŀ��
	void SetDataPointCursor(HCURSOR hNewDataPointCursor);		//! Data-Point Event ���ۿ��� ����ϴ� Ŀ�� �ڵ鰪�� ����, Added by LHW [2008/5/28]
	void SetZoomXYCursor(HCURSOR hNewCursor);					//! Zoom Area ���ۿ��� ����ϴ� Ŀ�� �ڵ鰪�� ����, Added by LHW [2009/2/2]
	void SetZoomXCursor(HCURSOR hNewCursor);					//! Zoom X ���ۿ��� ����ϴ� Ŀ�� �ڵ鰪�� ����, Added by LHW [2009/2/2]
	void SetZoomYCursor(HCURSOR hNewCursor);					//! Zoom Y ���ۿ��� ����ϴ� Ŀ�� �ڵ鰪�� ����, Added by LHW [2009/2/2]
	void SetPanXCursor(HCURSOR hNewCursor);						//! X�� ���� Pan ���ۿ��� ����ϴ� Ŀ�� �ڵ鰪�� ����, Added by LHW [2009/2/2]
	void SetPanYCursor(HCURSOR hNewCursor);						//! Y�� ���� Pan ���ۿ��� ����ϴ� Ŀ�� �ڵ鰪�� ����, Added by LHW [2009/2/2]

	//! �׷��� ���� ���ڿ� ���� �Լ�, MakeResourcePtr �Ǵ� SetResourcePtr �Լ� ȣ���Ŀ� ����ؾ� ��
	//! iOverlapIndex�� Overlap�Ǵ� �׷����� index (0 based)
	bool SetCaptionX(CString szInput);							//! X�� ���� ���ڿ� �Է�
	bool SetCaptionY(CString szInput1, CString szInput2, int iOverlapIndex = 0);	//! Y�� ���� ���ڿ� �Է�, szInput2 �߰�, Modify by LHW [2008/7/24]
	bool SetCursorUnitX(CString szInput);						//! Ŀ�� ����â�� ǥ�õ� X�� ���� ���ڿ� �Է�
	bool SetCursorUnitY(CString szInput, int iOverlapIndex = 0);//! Ŀ�� ����â�� ǥ�õ� Y�� ���� ���ڿ� �Է�

	//! ����, Range, Ŀ���� �Ҽ��� �ڸ��� ���� �Լ�, "Graph2D_Struct.h" ���Ͽ� ����ü ����ΰ� ����
	//! MakeResourcePtr �Ǵ� SetResourcePtr �Լ� ȣ���Ŀ� ����ؾ� ��
	bool SetResourceInfo(strGraph2DResourceInfo strInfo);
	bool GetResourceInfo(strGraph2DResourceInfo &strInfo);
	//! Y�� Grid, Ŀ���� Y �� ǥ��, �׷����� �� ���� ����, iOverlapIndex�� Overlap�Ǵ� �׷����� index (0 based), MakePlotBuffer �Լ� ȣ�� �Ŀ� ���
	bool SetResourceYInfo(strGraph2D_Y_Axis_Info strInfo, int iOverlapIndex = 0);
	bool GetResourceYInfo(strGraph2D_Y_Axis_Info &strInfo, int iOverlapIndex = 0);

	//! Y���� X��ǥ�� Grid�� ���ڿ��� �ʺ� Ŀ���� ���� Ŀ�� �� �� ũ��� ����, �ִ�ġ�� ���� ȭ�� �ʺ��� ����, ȭ�� ũ�Ⱑ �ٲ�� Y���� X��ǥ�� �ʱ�ȭ, Added by LHW [2008/10/25]
	bool Init_XPos_YAxis(int iOverlapIndex);	//! Y���� X��ǥ�� �ʱ�ȭ
	bool Get_XPos_YAxis(int iOverlapIndex, int &iLeftXPtofYAxis, int &iRightXPtofYAxis);//! Y���� X��ǥ�� ��ȯ
	bool Set_XPos_YAxis(int iOverlapIndex, int iLeftXPtofYAxis, int iRightXPtofYAxis);	//! Y���� X��ǥ�� ������ ����, ������ Grid�� ���ڿ��� �ʺ� Ŀ���� �ڵ����� ����

	//! ������ �Է�,���� �Լ�, Overlap�Ǵ� �������� index(iOverlapIndex)�� 0 based
	//! SetPlotData �Լ��� ������ �����͸� �����ϰ� ��� �����͸� �߰��ϴ� �Լ�, ���������� ClearPlotData, DecideMinMax ȣ���
	//! AddPlotData �Լ��� �����͸� �ϳ��� �Է��� �� ���, ������ �����Ϳ� ��� �߰��� ���� ����
	//!		��, AddPlotData �Լ��� ���� �����͸� �Է��� ���, �� �Լ��� ����ϱ� ���� ClearPlotData�� ȣ���ؾ� ��
	//!		    AddPlotData �Լ��� �����͸� ��� �Է��� �Ŀ� �ݵ�� DecideMinMax�� ȣ���ؾ� ��
	bool SetPlotData(double *pdbInY, int iInputSize, int iOverlapIndex = 0);//! ������ �Է�, X�� �����ʹ� 0 based�� index���� �Էµ�
	bool SetPlotData(double dbStartX, double dbIncrementX, double *pdbInY, int iInputSize, int iOverlapIndex = 0);	
																		//! ������ �Է�, X�� �������δ� ���۰��� �������� �Է�
	bool SetPlotData(double *pdbInX, double *pdbInY, int iInputSize, int iOverlapIndex = 0);	
																		//! ������ �Է�, X, Y�� �������� ���� ������ �Է�
	bool SetPlotData(COleDateTime timeStartX, int iIncrementSecondX, double *pdbInY, int iInputSize, int iOverlapIndex = 0);
											//! ������ �Է�, X�� �������� ���� �ð��� �� ������ �������� �Է�, Added by LHW [2008/4/2]
	bool AddPlotData(double dbInX, double dbInY, int iOverlapIndex = 0);//! �����͸� �ϳ��� �Է�
	bool AddPlotData(COleDateTime timeInX, double dbInY, int iOverlapIndex = 0);//! �����͸� �ϳ��� �Է�, X �����ʹ� Time ������, Added by LHW [2008/4/2]
	bool ClearPlotData(int iOverlapIndex = 0);	//! ���� ������ ���� �Լ�
	bool DecideMinMax(int iOverlapIndex = 0);	//! ���� �Էµ� X, Y �������� �ִ�, �ּҰ��� ã�Ƴ��� �ش� ������ ����

	//! Added by LHW [2008/5/23] : ������ �Է�,���� �Լ�, Custom Data �Է� ��� �߰�, �� �̿ܿ��� ���� ������ �Է�,���� �Լ���� ����
	//! Custom Data�� 0�̻��� �������� �ԷµǾ� ���� ��쿡�� �̺�Ʈ�� �簢 ���� ����
	//! Overlap�Ǵ� �������� index(iOverlapIndex)�� 0 based
	//! Custom Data�� ������ X, Y �Է°��� ������ �����ؾ� ��, Custom Data �Է� ���۸� NULL�� �ϸ� -1�� �Է�
	bool SetPlotData(double *pdbInY, int *piCustomData, int iInputSize, int iOverlapIndex = 0);//! ������ �Է�, X�� �����ʹ� 0 based�� index���� �Էµ�
	bool SetPlotData(double dbStartX, double dbIncrementX, double *pdbInY, int *piCustomData, int iInputSize, int iOverlapIndex = 0);	
																		//! ������ �Է�, X�� �������δ� ���۰��� �������� �Է�
	bool SetPlotData(double *pdbInX, double *pdbInY, int *piCustomData, int iInputSize, int iOverlapIndex = 0);	
																		//! ������ �Է�, X, Y�� �������� ���� ������ �Է�
	bool SetPlotData(COleDateTime timeStartX, int iIncrementSecondX, double *pdbInY, int *piCustomData, int iInputSize, int iOverlapIndex = 0);
											//! ������ �Է�, X�� �������� ���� �ð��� �� ������ �������� �Է�
	bool AddPlotData(int iCustomData, double dbInX, double dbInY, int iOverlapIndex = 0);//! �����͸� �ϳ��� �Է�
	bool AddPlotData(int iCustomData, COleDateTime timeInX, double dbInY, int iOverlapIndex = 0);//! �����͸� �ϳ��� �Է�, X �����ʹ� Time ������

	//! Added by LHW [2013/2/22]
	bool GetPlotDataX(double &dbValueX, int iDataIndex, int iOverlapIndex = 0);
	bool GetPlotDataY(double &dbValueY, int iDataIndex, int iOverlapIndex = 0);

	//! �ܺο��� CDC��ü�� �簢 ������ �־� Plot�� �׷��ִ� �Լ�, �μ� ��ɿ� �ʿ�
	//! �μ⸦ �� ��, �ݵ�� MM_TEXT ���� ��� �����̾�� ��
	bool Display(CDC *pDC, const CRect crDraw, bool bPrintFlag);		//! ���������� �׷����� �׸��� �Լ�
																		//! crDraw�� �׷����� ���� ��ġ, ũ�⸦ ���� ��
																		//! ȣ������ �ʾƵ� ���������� OnPaint()���� ȣ���
																		//! �μ��� ���� bPrintFlag�� true�� �����Ͽ� ���� ȣ���ؾ� ��

	//! Added by LHW [2008/4/19]
	//! ���� �������� Peak ���� ã�� �Լ�, Data index�� ��� 0 based, iOverlapIndex�� Overlap�Ǵ� �׷����� index (0 based)
	//! iStartDataIndex : ���� index, iEndDataIndex : ��ħ index, iPeakDataIndex�� ã�Ƴ� Peak ���� ������ index
	bool GetLocalPeakDataIndex(int iStartDataIndex, int iEndDataIndex, int &iPeakDataIndex, int iOverlapIndex = 0);

	//! Ŀ�� ���� �Լ�, Ŀ�� index, Data index�� ��� 0 based, iOverlapIndex�� Overlap�Ǵ� �׷����� index (0 based)
	//! bRecvEvent : Ŀ�� �̺�Ʈ�� �޾� ó���� ���� {{�ݵ��}} true�� �ؾ� ��, �� �̿ܿ��� false�� ����
	int GetCurrentCursorSize();							//! Ŀ�� ���� ��ȯ
	bool IsCurrentCursorSelect(int &iSelectCursorIndex);//! ���� ���õ� Ŀ���� index�� iSelectCursorIndex�� ��ȯ
														//! ���� ���õ� Ŀ���� ������ iSelectCursorIndex�� -1�� ��ȯ, ���� Ŀ���� ������ false�� ��ȯ														
	bool IsCursorSelected(int &iSelectCursorIndex);		//! ���� ���õ� Ŀ���� index�� iSelectCursorIndex�� ��ȯ, ���� Ŀ���� ������ false�� ��ȯ
														//! IsCurrentCursorSelect�ʹ� �޸� ���� ���õ� Ŀ���� ������ ���������� ���õ� Ŀ���� index���� ��ȯ

	bool GetCursorOverlapIndex(int iCursorIndex, int &iOverlapIndex);	//! �־��� Ŀ�� index�� Ŀ���� �׷��� index ��ȯ
	bool SetCursorOverlapIndex(int iCursorIndex, int iOverlapIndex);	//! �־��� Ŀ�� index�� Ŀ���� �׷��� index ���� ����, Added by LHW [2008/7/29]
	bool GetCursorDataIndex(int iCursorIndex, int &iDataIndex);			//! �־��� Ŀ�� index�� Ŀ���� Data index ��ȯ

	bool ShowToggleCursorInfoInSelected(bool bRecvEvent);				//! ���õ� Ŀ�� ����â�� ���̰� �ϰų� ������ �ʰ� ��
	bool ShowToggleCursorInfo(int iCursorIndex, bool bRecvEvent);		//! �־��� Ŀ�� index�� Ŀ�� ����â�� ���̰� �ϰų� ������ �ʰ� ��
	bool ShowCursorInfoInSelected(bool bShow, bool bRecvEvent);			//! ���õ� Ŀ�� ����â�� ���̰� �ϰų� ������ �ʰ� ��
	bool ShowCursorInfo(int iCursorIndex, bool bShow, bool bRecvEvent);	//! �־��� Ŀ�� index�� Ŀ�� ����â�� ���̰� �ϰų� ������ �ʰ� ��
	bool IsShowCursorInfoInSelected();				//! ���� ���õ� Ŀ�� ����â�� ���̴� �����̸� true�� ��ȯ
	bool IsShowCursorInfo(int iCursorIndex);		//! �־��� Ŀ�� index�� Ŀ�� ����â�� ���̴� �����̸� true�� ��ȯ
	
	//! Modify by LHW [2008/7/24] : AddCursorInMouseClick, AddCursorInPosition�� iOverlapIndex �߰�
	//!								iOverlapIndex�� -1�̸� ��� �׷������� Ŀ�� �˻�, 0�̻��̸� �ش� �׷��������� Ŀ�� �߰�
	bool AddCursorInMouseClick(bool bRecvEvent, int iOverlapIndex = -1);					//! ���� ���콺�� Ŭ���� ������ Ŀ���� ���� ����
	bool AddCursorInPosition(int iXPos, int iYPos, bool bRecvEvent, int iOverlapIndex = -1);//! �־��� ��ǥ�� ������ Ŀ���� ���� ����
	bool AddCursorInDataIndex(int iDataIndex, bool bRecvEvent, int iOverlapIndex = 0);		//! Ư�� Data index�� Ŀ���� ���� ����

	bool DeleteCursorInSelected(bool bRecvEvent);					//! ���õ� Ŀ���� ����
	bool DeleteCursor(int iCursorIndex, bool bRecvEvent);			//! �־��� Ŀ�� index�� Ŀ���� ����, Modify by LHW [2008/4/19]
	bool DeleteAllCursor(bool bRecvEvent);							//! Ŀ�� ��� ����, Modify by LHW [2008/4/19]
	bool DeleteAllCursor(int iOverlapIndex, bool bRecvEvent);		//! iOverlapIndex�� �׷����� ����� Ŀ�� ��� ����, Added by LHW [2008/7/29]
	bool ReAdjustAllCursor(bool bSearchDirectionFlag);				//! Ŀ������ ���� ���� iOverlapIndex�� ������, Added by LHW [2008/7/29]
																	//! bSearchDirectionFlag�� true�̸� ���ο� iOverlapIndex�� �տ��� ���� ã�ƺ�
	
	bool MoveCursor(int iCursorIndex, bool bMoveRight, bool bRecvEvent, int iOverlapIndex = 0);	//! �־��� Ŀ�� index�� Ŀ�� �̵�, bMoveRight�� true�̸� ������ �̵�, false�̸� ���� �̵�
	bool MoveCursor(int iCursorIndex, int iDataIndex, bool bRecvEvent, int iOverlapIndex = 0);	//! �־��� Ŀ�� index�� Ŀ���� �־��� Data index�� �̵�
	
	bool GetCursorLineMode(int iCursorIndex, E_GRAPH2D_CURSOR_LINE_MODE &eLineMode);				//! �־��� Ŀ�� index�� Ŀ�� �� ǥ�� ��� ��ȯ
	bool SetCursorLineMode(int iCursorIndex, E_GRAPH2D_CURSOR_LINE_MODE eLineMode, bool bRecvEvent);//! �־��� Ŀ�� index�� Ŀ�� �� ǥ�� ��� ����

	bool GetCursorMaxHold(int iCursorIndex, bool &bMaxHold);				//! �־��� Ŀ�� index�� Ŀ���� �ݵ�� �ִ밪�� ��ġ�ϵ��� �����Ǿ� ������ bMaxHold�� true�� ��ȯ
	bool SetCursorMaxHold(int iCursorIndex, bool bMaxHold, bool bRecvEvent);//! bMaxHold�� true�̸� �־��� Ŀ�� index�� Ŀ���� �ݵ�� �ִ밪�� ��ġ�ϵ��� ����

	bool GetCursorValue(int iCursorIndex, int &iDataIndex, double &dbValueX, double &dbValueY);	//! �־��� Ŀ�� index�� Ŀ���� X, Y, ������ index ���� ��ȯ

	//! �־��� ������ index, X���� ������, iOverlapIndex�� �׷����� Y���� ��ȯ
	//! bEqualInterval�� true�̸� �����Ͱ� X�������� ������ ������ �ǹ�
	//! iDataIndex�� -1�̰ų� �ش� �׷��� �������� iDataIndex�� �ش��ϴ� X���� �Էµ� X���� �ٸ���,
	//! X���� �ش��ϴ� Y���� ã�ƺ���, �׷��� ������ ���������� ����Ͽ� ��ȯ, Added by LHW [2008/7/24]
	bool GetDataYValue(int iDataIndex, double dbValueX, int iOverlapIndex, bool bEqualInterval, double &dbValueY);																									

	//! Added by LHW [2008/7/01]
	bool GetCursorValueString(int iCursorIndex, CString &szValueX, CString &szValueY);	//! �־��� Ŀ�� index�� Ŀ���� X, Y ������ ���ڿ� ��ȯ
	bool GetCursorRealPrecision(int iCursorIndex, int &iPrecisionX, int &iPrecisionY);	//! �־��� Ŀ�� index�� Ŀ���� �Ҽ��� �ڸ��� ��ȯ

	//! Added by LHW [2008/4/4]
	bool GetCursorSearchMargin(int iCursorIndex, int &iMargin);					//! �־��� Ŀ�� index�� Ŀ���� ���� ����� �����͸� ã�� ��, ã�� ������ ���� �� ��ȯ																						
	bool SetCursorSearchMargin(int iCursorIndex, int iMargin, bool bRecvEvent);	//! ã�� ������ ���� ��(�߰��� �˻��ؾ� �� ������ ����) ����, ���� ���� m_eCursorSearchMode�� E_GRAPH2D_CURSOR_SEARCH_X_RANGE ���� ���
	
	//! Added by LHW [2008/4/4]
	bool GetCursorValueVisible(int iCursorIndex, bool &bVisibleX, bool &bVisibleY);					//! �־��� Ŀ�� index�� Ŀ���� X,Y ���ڿ��� ���� ���̰ų� ������ ���¸� ��ȯ
	bool SetCursorValueVisible(int iCursorIndex, bool bVisibleX, bool bVisibleY, bool bRecvEvent);	//! �־��� Ŀ�� index�� Ŀ���� X,Y ���ڿ��� ���� ���̰ų� ������ ���¸� ����

	//! Added by LHW [2008/8/01]
	bool GetCursorXYStringVisible(int iCursorIndex, bool &bVisibleX, bool &bVisibleY);					//! �־��� Ŀ�� index�� Ŀ���� 'X : ', "Y : ' ���ڿ��� ���� ���̰ų� ������ ���¸� ��ȯ
	bool SetCursorXYStringVisible(int iCursorIndex, bool bVisibleX, bool bVisibleY, bool bRecvEvent);	//! �־��� Ŀ�� index�� Ŀ���� 'X : ', "Y : ' ���ڿ��� ���� ���̰ų� ������ ���¸� ����

	//! Added by LHW [2008/4/18]
	bool GetCursorPointVisible(int iCursorIndex, bool &bVisible);					//! �־��� Ŀ�� index�� Ŀ���� �簢�� ���� ǥ�÷� Ŀ�� ��ġ�� ǥ���ϴ����� ���� ��ȯ
	bool SetCursorPointVisible(int iCursorIndex, bool bVisible, bool bRecvEvent);	//! �־��� Ŀ�� index�� Ŀ���� �簢�� ���� ǥ�÷� Ŀ�� ��ġ�� ǥ���ϴ����� ���� ����

	//! Added by LHW [2008/4/19] : ���� Ŀ�� ���� index�� �����ؾ� Ŀ���� ������ ��, ����� ���� Ŀ���� ���� ������, �ʱⰪ���� -1�� �ԷµǾ� ����
	//! Modify by LHW [2008/6/12] : �Լ��� ����
	bool GetCursorAuxCursorGroupIndex(int iCursorIndex, int &iAuxCursorGroupElementIndex);	//! �־��� Ŀ�� index�� Ŀ���� ���� ����ҿ� ���� Ŀ�� �׷��� ���� index�� ��ȯ
	bool SetCursorAuxCursorGroupIndex(int iCursorIndex, int iAuxCursorGroupElementIndex);	//! �־��� Ŀ�� index�� Ŀ���� ���� ����ҿ� ���� Ŀ�� �׷��� ���� index�� ����

	//! Added by LHW [2008/4/19] : �׷����� ������ ������ Ŀ���� ���� ������ �ʰ� �����Ϸ��� true�� �����ؾ� ��
	bool GetCursorGraphVisibleCheck(int iCursorIndex, bool &bVisibleChk);
	bool SetCursorGraphVisibleCheck(int iCursorIndex, bool bVisibleChk);

	//! Added by LHW [2009/5/30]
	bool Add_YMinBound(int iOverlapIndex, double dbInput);	//! Y�� ���� �ּҰ��� ������ Step ���� �߰�
	bool Add_YMaxBound(int iOverlapIndex, double dbInput);	//! Y�� ���� �ִ밪�� ������ Step ���� �߰�
	bool RemoveAll_YMinBound(int iOverlapIndex);			//! Y�� ���� �ּҰ��� ������ Step �� ��� ����
	bool RemoveAll_YMaxBound(int iOverlapIndex);			//! Y�� ���� �ִ밪�� ������ Step �� ��� ����
	bool Add_XMinBound(double dbInput);						//! X�� ���� �ּҰ��� ������ Step ���� �߰�
	bool Add_XMaxBound(double dbInput);						//! X�� ���� �ִ밪�� ������ Step ���� �߰�
	bool RemoveAll_XMinBound();								//! X�� ���� �ּҰ��� ������ Step �� ��� ����
	bool RemoveAll_XMaxBound();								//! X�� ���� �ִ밪�� ������ Step �� ��� ����

	//! Added by LHW [2008/4/15]
	//! Ű������ Ű�� ������ Ŀ�� �̵� ó��, �ٸ� ������� Ŀ���� �̵���Ű�� ���ؼ��� ���� �����ؾ� ��
	//! true�� ��ȯ�Ǹ� invalidate�� ȣ���ؾ� ��
	//! ���� ȭ��ǥ : Ŀ���� ������ �� �� �ڷ� �̵�, ������ ȭ��ǥ : Ŀ���� ������ �� �� ������ �̵�
	//! ���� ȭ��ǥ, �Ʒ��� ȭ��ǥ : Overlap�� �׷��� ������ �̵�
	//! Page UP Ű : Ŀ���� iMoveSize��ŭ �ڷ� �̵�, Page Down Ű : Ŀ���� iMoveSize��ŭ ������ �̵�
	bool CursorOperationWithKeyBoard(E_GRAPH2D_KEY_TYPE eKeyType, int iMoveSize = 100);

	//! ���� Ŀ�� ���� �Լ� : Added by LHW [2008/3/31]
	//! Added by LHW [2008/5/31] : GetAuxCursorValue �߰�
	//! Modify by LHW [2008/6/3] : GetAuxCursorValue�� bValid �߰�
	//! ���� Ŀ�� ���� �Լ�, iAuxCursorGroupIndex, iAuxCursorIndex�� ���� Ŀ���� �����ϴ� index, 0 based
	//! SetAuxCursorInfo, GetAuxCursorInfo, SetAuxCursorGroupInfo, GetAuxCursorGroupInfo �Լ��� AddAuxCursor �Լ� ȣ�� �� ��� ����
	int GetCurrentAuxCursorGroupSize();						//! ���� Ŀ�� �׷��� ���� ��ȯ
	int GetCurrentAuxCursorSize(int iAuxCursorGroupIndex);	//! iAuxCursorGroupIndex�� �ش��ϴ� ���� Ŀ�� �׷��� ���� Ŀ�� ���� ��ȯ	
	int AddAuxCursor(strGraph2D_Aux_Cursor strAuxCursorInfo, int iAuxCursorGroupIndex = -1);//! ���ο� ���� Ŀ�� �߰��ϰ� ���� Ŀ���� �Ҽӵ� ���� Ŀ�� �׷� index�� ��ȯ
																							//! iAuxCursorGroupIndex�� 0 �̻��� �����̸� �ش� index�� ���� Ŀ�� �׷쿡 �߰�
																							//! iAuxCursorGroupIndex�� -1�̸� ���� Ŀ�� �׷��� ���� �����ϰ� ���� Ŀ�� �߰�
	bool SetAuxCursorInfo(int iAuxCursorGroupIndex, int iAuxCursorIndex, strGraph2D_Aux_Cursor strAuxCursorInfo);	//! ���� Ŀ���� ���� ����
	bool GetAuxCursorInfo(int iAuxCursorGroupIndex, int iAuxCursorIndex, strGraph2D_Aux_Cursor &strAuxCursorInfo);	//! ���� Ŀ���� ���� ��ȯ
	bool SetAuxCursorGroupInfo(int iAuxCursorGroupIndex, strGraph2D_Aux_CursorGroup strAuxCursorGroupInfo);	//! ���� Ŀ�� �׷��� ���� ����
	bool GetAuxCursorGroupInfo(int iAuxCursorGroupIndex, strGraph2D_Aux_CursorGroup &strAuxCursorGroupInfo);//! ���� Ŀ�� �׷��� ���� ��ȯ	
	bool ChangeAuxCursorGroupOverlapIndex(int iAuxCursorGroupIndex, int iOverlapIndex);	//! ���� Ŀ�� �׷� ��ü�� Overlap index ����, Added by LHW [2008/7/27]
	//! �־��� ���� Ŀ�� index�� Ŀ���� X, Y, ������ index ���� ��ȯ
	bool GetAuxCursorValue(int iAuxCursorGroupIndex, int iAuxCursorIndex, bool &bValid, int &iDataIndex, double &dbValueX, double &dbValueY);
	bool MoveAuxCursor(int iAuxCursorGroupIndex, int iAuxCursorIndex, int iDataIndex, int iOverlapIndex = 0);	//! ���� Ŀ���� ���ο� ��ġ�� ����, Added by LHW [2008/8/5]
	bool IsCurrentAuxCursorSelect(int &iSelectAuxCursorGroupIndex, int &iSelectAuxCursorIndex);	//! Added by LHW [2008/8/5], ���� ���õ� ���� Ŀ���� index�� ��ȯ
																								//! ���� ���õ� ���� Ŀ���� ������ ���ڷ� -1�� ��ȯ
																								//! ���� ���� Ŀ���� ������ false�� ��ȯ
	bool IsAuxCursorSelected(int &iSelectAuxCursorGroupIndex, int &iSelectAuxCursorIndex);	//! Added by LHW [2008/8/5], ���� ���õ� Ŀ���� index�� ��ȯ
																							//! ���� ���� Ŀ���� ������ false�� ��ȯ
																							//! IsCurrentAuxCursorSelect�ʹ� �޸� ���� ���õ� Ŀ���� ������ ���������� ���õ� Ŀ���� index���� ��ȯ
	bool RemoveOneAuxCursor(int iAuxCursorGroupIndex, int iAuxCursorIndex);	//! Ư�� ���� Ŀ�� ����
	bool RemoveOneAuxCursorGroup(int iAuxCursorGroupIndex);					//! Ư�� ���� Ŀ�� �׷� ����
	void RemoveAllAuxCursor();												//! ���� Ŀ�� ��� ����

	//! �Ʒ��� MakeFrom~ �Լ����� X�� �����Ͱ� ����̰� , X-Y �����Ͱ� 1:1 ���� �� ���� ������ ��

	//! Added by LHW [2008/4/10]
	//! iOverlapIndex�� Overlap�Ǵ� �׷����� index (0 based)
	//! �Էµ� Y�� �����Ϳ��� ū ������� iAuxCursorSize �� ��ŭ ���� Ŀ���� �߰��ϰ�, iAuxCursorGroupIndex�� ��ȯ
	bool MakeFromMaxValuesToAuxCursor(int iAuxCursorSize, int &iAuxCursorGroupIndex, int iOverlapIndex = 0);
	//! �־��� Threshold ������ ū Y�� �����͵��� ���� Ŀ���� �߰��ϰ�, iAuxCursorGroupIndex�� ��ȯ
	//! Modify by LHW [2008/9/1] : iMaxAddSize �߰�, Threshold ������ ū Y�� �������� ���� Ŀ�� �ִ� ������ ����
	//!												-1�̸� Threshold���� ū ��� ���� ���� Ŀ���� �߰�
	bool MakeFromThresholdToAuxCursor(double dbThreshold, int iMaxAddSize, int &iAuxCursorGroupIndex, int iOverlapIndex = 0);

	//! Added by LHW [2008/4/28]
	//! iOverlapIndex�� Overlap�Ǵ� �׷����� index (0 based)
	//! �Էµ� Y�� �������� Peak������ ū ������� iAuxCursorSize �� ��ŭ ���� Ŀ���� �߰��ϰ�, iAuxCursorGroupIndex�� ��ȯ
	bool MakeFromMaxPeakValuesToAuxCursor(int iAuxCursorSize, int &iAuxCursorGroupIndex, int iOverlapIndex = 0);

	//! �ڽ� Ŀ�� ����, Added by LHW [2008/6/13]
	//! Added by LHW [2008/6/15] : MoveChildCursor �߰�
	int GetCurrentChildCursorSize();								//! �ڽ� Ŀ�� ���� ��ȯ
	bool IsCurrentChildCursorSelect(int &iSelectChildCursorIndex);	//! ���� ���õ� �ڽ� Ŀ���� index�� iSelectCursorIndex�� ��ȯ
																	//! ���� ���õ� �ڽ� Ŀ���� ������ iSelectCursorIndex�� -1�� ��ȯ
																	//! ���� �ڽ� Ŀ���� ������ false�� ��ȯ
	bool IsChildCursorSelected(int &iSelectChildCursorIndex);		//! ���� ���õ� Ŀ���� index�� iSelectChildCursorIndex�� ��ȯ, Added by LHW [2008/6/14]
																	//! ���� Ŀ���� ������ false�� ��ȯ
																	//! IsCurrentChildCursorSelect�ʹ� �޸� ���� ���õ� Ŀ���� ������ ���������� ���õ� Ŀ���� index���� ��ȯ
	bool IsExistChildCursor(int iElementCursorIndex);				//! �־��� iElementCursorIndex�� Ŀ���� ����� �ڽ� Ŀ���� ������ true�� ��ȯ, Added by LHW [2008/6/16]
	int AddChildCursor(strGraph2D_Child_Cursor strChildCursor);	//! �ڽ� Ŀ�� �߰�
	bool SetChildCursorInfo(int iChildCursorIndex, strGraph2D_Child_Cursor strChildCursor);	//! �ڽ� Ŀ���� ���� ����
	bool GetChildCursorInfo(int iChildCursorIndex, strGraph2D_Child_Cursor &strChildCursor);//! �ڽ� Ŀ���� ���� ��ȯ
	bool MoveChildCursor(int iChildCursorIndex, int iDataIndex, int iOverlapIndex = 0);	//! �־��� iChildCursorIndex�� �ڽ� Ŀ���� �־��� Data index�� �̵�	
	//! �־��� �ڽ� Ŀ�� index�� Ŀ���� X, Y, ������ index ���� ��ȯ
	bool GetChildCursorValue(int iChildCursorIndex, bool &bValid, int &iDataIndex, double &dbValueX, double &dbValueY, double &dbDeltaX, double &dbDeltaY);
	bool RemoveOneChildCursor(int iChildCursorIndex);	//! Ư�� �ڽ� Ŀ�� ����
	void RemoveChildCursor(int iElementCursorIndex);	//! �־��� iElementCursorIndex�� Ŀ���� ����� ��� �ڽ� Ŀ�� ����
	void RemoveAllChildCursor();						//! �ڽ� Ŀ�� ��� ����

	//! ���� ǥ�� Ŀ�� ����, Added by LHW [2008/11/28]
	int GetCurrentRegionCursorSize();
	bool IsCurrentRegionCursorSelect(int &iSelectRegionCursorIndex);
	bool IsRegionCursorSelected(int &iSelectRegionCursorIndex);
	int AddRegionCursor(strGraph2D_Region_Cursor strRegionCursor, bool bChkOverlapIndex = true);	//! Modify by LHW [2008/12/16] : bChkOverlapIndex �߰�
	bool SetRegionCursorInfo(int iRegionCursorIndex, strGraph2D_Region_Cursor strRegionCursor);
	bool GetRegionCursorInfo(int iRegionCursorIndex, strGraph2D_Region_Cursor &strRegionCursor);
	bool MoveRegionCursor(int iRegionCursorIndex, double dbInputMinX, double dbInputMaxX, double dbInputMinY, double dbInputMaxY);
	bool GetRegionCursorValue(int iRegionCursorIndex, double &dbOutputMinX, double &dbOutputMaxX, double &dbOutputMinY, double &dbOutputMaxY);
	bool RemoveOneRegionCursor(int iRegionCursorIndex);
	void RemoveAllRegionCursor();
	
	//! Ȯ��/��� ���� �Լ�
	//! Modify by LHW [2008/4/19] : bRecvEvent �߰�, Zoom �̺�Ʈ�� �޾� ó���� ���� {{�ݵ��}} true�� �ؾ� ��, �� �̿ܿ��� false�� ����
	bool ZoomArea(CRect crZoomArea, bool bRecvEvent, E_GRAPH2D_ZOOM_DIRECTION eMode = E_GRAPH2D_ZOOM_DIRECTION_XY);	//! �־��� �簢 ������ŭ �׷����� Ȯ��
	bool ZoomX(double dbMin, double dbMax, bool bRecvEvent);//! Added by LHW [2009/2/14]
	bool ZoomY(bool bRecvEvent);							//! Added by LHW [2009/2/14]
	bool ZoomOut(bool bRecvEvent = false);			//! ���� Ȯ�� ���·� ����
	bool IsZoomIn();								//! ���콺�� �����Ͽ� Ȯ��� �����̸� true�� ��ȯ
	//! bDirectionX�� true�̸� X�� �������� Auto Range ����, bDirectionY�� true�̸� Y�� �������� Auto Range ����
	bool ZoomFirstBack(bool bDirectionX, bool bDirectionY, bool bRecvEvent = false);//! Ȯ�� ����� �ϱ� ���� �� ó�� ���·� ����, Modify by LHW [2008/4/17]	
	bool ZoomFit(bool bDirectionX, bool bDirectionY, bool bRecvEvent = false);		//! ��ü �׷����� ��� Auto Range�� ����, Modify by LHW [2008/4/17]

	//! Pan ���� �Լ�	
	bool IsPanOperated();							//! ���� Pan ����� ����Ͽ� �׷����� ���� �̵��� �����̸� true�� ��ȯ

	//! X�� Ư���� ǥ�ü� ���� �Լ�, iLineIndex�� ���� �����ϴ� index, 0 based, Added by LHW [2008/4/19]
	int GetXValueLineSize();												//! X�� Ư���� ǥ�ü��� ���� ��ȯ
	bool SetXValueLineInfo(int iLineIndex, strValueLineXInfo strXLineInfo);	//! X�� Ư���� ǥ�ü� ���� ����
	bool GetXValueLineInfo(int iLineIndex, strValueLineXInfo &strXLineInfo);//! X�� Ư���� ǥ�ü� ���� ��ȯ
	int AddXValueLine(strValueLineXInfo strXLineInfo);						//! ���ο� X�� Ư���� ǥ�ü� �߰�, Line index�� ��ȯ
	bool RemoveOneXValueLine(int iLineIndex);								//! Ư�� Line index�� X�� Ư���� ǥ�ü� ����
	void RemoveAllXValueLine();												//! ��� X�� Ư���� ǥ�ü� ����

	//! Y�� Ư���� ǥ�ü� ���� �Լ�, iLineIndex�� ���� �����ϴ� index, 0 based, Added by LHW [2008/3/29]
	int GetYValueLineSize();												//! Y�� Ư���� ǥ�ü��� ���� ��ȯ
	bool SetYValueLineInfo(int iLineIndex, strValueLineYInfo strYLineInfo);	//! Y�� Ư���� ǥ�ü� ���� ����
	bool GetYValueLineInfo(int iLineIndex, strValueLineYInfo &strYLineInfo);//! Y�� Ư���� ǥ�ü� ���� ��ȯ
	int AddYValueLine(strValueLineYInfo strYLineInfo);						//! ���ο� Y�� Ư���� ǥ�ü� �߰�, Line index�� ��ȯ
	bool RemoveOneYValueLine(int iLineIndex);								//! Ư�� Line index�� Y�� Ư���� ǥ�ü� ����
	void RemoveAllYValueLine();												//! ��� Y�� Ư���� ǥ�ü� ����

	//! �Լ������� �׷����� �׷��� ���� �Լ�, Added by LHW [2008/8/23]
	int GetGraphEquationSize();															//! �Լ������� �׷����� �׷��� ���� ��ȯ
	bool SetGraphEquationInfo(int iGraphIndex, strGraph2DEquationInfo strEquationInfo);	//! �Լ������� �׷����� �׷��� ���� ����
	bool GetGraphEquationInfo(int iGraphIndex, strGraph2DEquationInfo &strEquationInfo);//! �Լ������� �׷����� �׷��� ���� ��ȯ																						
	int AddGraphEquation(strGraph2DEquationInfo strEquationInfo);						//! �Լ������� �׷����� �׷��� �߰�, Graph index�� ��ȯ
	bool RemoveOneGraphEquation(int iGraphIndex);										//! Ư�� Graph index�� �Լ������� �׷����� �׷��� ����
	void RemoveAllGraphEquation();														//! ��� �Լ������� �׷����� �׷��� ����
	
	//! Plot�� ���� ���� ������ ��ȯ�ϴ� �Լ�
	//! iOverlapIndex�� Overlap�Ǵ� �׷����� index (0 based)
	//! Modify by LHW [2008/4/2] : bShow�� true�̸� ���� ���̵��� ������ �׷����� ����Ͽ� �ش� ���� ������ ��ȯ
	//! Modify by LHW [2008/8/25] : bEquation�� true�̸� �Լ������� �׷����� �׷����� ����Ͽ� �ش� ���� ������ ��ȯ�ϵ��� ����
	int GetGraphWidth();		//! �׷����� ���� ��, pixel ������ ��ȯ
	int GetGraphHeight();		//! �׷����� ���� ����, pixel ������ ��ȯ
	int GetRealDataSize(bool bShow = false);		//! �׷��� �������� ���� �ִ밪�� ��ȯ
	int GetRealDataSize(int iOverlapIndex, bool bShow = false);	//! �ش� �׷����� ������ ������ ��ȯ, ���ڰ��� �߸��Ǿ����� ������ ��ȯ, Added by LHW [2008/8/02]
	int GetRealVisualDataSize(bool bShow = false);	//! ���� ���̴� �׷��� �������� ���� �ִ밪�� ��ȯ
	int GetRealVisualDataSize(int iOverlapIndex, bool bShow = false);	//! �ش� �׷����� ���� ���̴� ������ ������ ��ȯ, ���ڰ��� �߸��Ǿ����� ������ ��ȯ, Added by LHW [2008/11/18]
	bool GetVisualDataIndex(int iOverlapIndex, int &iStartDataIndex, int &iEndDataIndex);	//! �ش� �׷����� ���� ���̴� ������ index�� ��ȯ, Added by LHW [2008/11/18]
	//! Modify by LHW [2008/12/14] : ���� ǥ�� Ŀ�� ����ϴ� ��� �߰�
	double GetRealMinX(bool bEquation, bool bRegionCursor, bool bShow = false);	//! �׷��� ������ ���� m_arrPlotData�� �Էµ� �����͵��� X �ּҰ�, ���ۿ� �����Ͱ� ������ 0�� ��ȯ
	double GetRealMaxX(bool bEquation, bool bRegionCursor, bool bShow = false);	//! �׷��� ������ ���� m_arrPlotData�� �Էµ� �����͵��� X �ִ밪, ���ۿ� �����Ͱ� ������ 0�� ��ȯ
	double GetRealMinY(bool bEquation, bool bYValueLine, bool bRegionCursor, bool bShow = false);	//! �׷��� ������ ���� m_arrPlotData�� �Էµ� �����͵��� Y �ּҰ�, ���ۿ� �����Ͱ� ������ 0�� ��ȯ
																				//! bEquation�� true�̸� �Լ������� �׷����� �׷����� ��� ���̵��� Y�� �ּҰ��� ��ȯ
																				//! bYValueLine�� true�̸� Y�� Ư���� ǥ�ü��� ��� ���̵��� Y �ּҰ��� ��ȯ
																				//! bRegionCursor�� true�̸� ���� ǥ�� Ŀ���� ��� ���̵��� Y �ּҰ��� ��ȯ
	double GetRealMaxY(bool bEquation, bool bYValueLine, bool bRegionCursor, bool bShow = false);	//! �׷��� ������ ���� m_arrPlotData�� �Էµ� �����͵��� Y �ִ밪, ���ۿ� �����Ͱ� ������ 0�� ��ȯ
																				//! bEquation�� true�̸� �Լ������� �׷����� �׷����� ��� ���̵��� Y�� �ִ밪�� ��ȯ
																				//! bYValueLine�� true�̸� Y�� Ư���� ǥ�ü��� ��� ���̵��� Y �ִ밪�� ��ȯ
																				//! bRegionCursor�� true�̸� ���� ǥ�� Ŀ���� ��� ���̵��� Y �ִ밪�� ��ȯ
	double GetRealMinY(int iOverlapIndex);		//! �׷��� ������ ���� m_arrPlotData�� �Էµ� �����͵��� Y �ּҰ�, Added by LHW [2008/7/28]
	double GetRealMaxY(int iOverlapIndex);		//! �׷��� ������ ���� m_arrPlotData�� �Էµ� �����͵��� Y �ִ밪, Added by LHW [2008/7/28]
	double GetVisualMinX();						//! ���� ���̴� �׷����� X�� �ּҰ�, Added by LHW [2008/4/18]
	double GetVisualMaxX();						//! ���� ���̴� �׷����� X�� �ִ밪, Added by LHW [2008/4/18]
	bool GetVisualMinY(double &dbMinY, int iOverlapIndex = 0);	//! ���� ���̴� �׷����� Y�� �ּҰ�, Added by LHW [2008/4/18]
	bool GetVisualMaxY(double &dbMaxY, int iOverlapIndex = 0);	//! ���� ���̴� �׷����� Y�� �ִ밪, Added by LHW [2008/4/18]
	bool GetAutoRangeMinX(double &dbMinX);							//! Auto Range�� ���� X�� �ּҰ� ��ȯ, Margin���� ����� ��, Added by LHW [2008/7/02]
	bool GetAutoRangeMaxX(double &dbMaxX);							//! Auto Range�� ���� X�� �ִ밪 ��ȯ, Margin���� ����� ��, Added by LHW [2008/7/02]
	bool GetAutoRangeMinY(double &dbMinY, int iOverlapIndex = 0);	//! Auto Range�� ���� Y�� �ּҰ� ��ȯ, Margin���� ����� ��, Added by LHW [2008/7/02]
	bool GetAutoRangeMaxY(double &dbMaxY, int iOverlapIndex = 0);	//! Auto Range�� ���� Y�� �ִ밪 ��ȯ, Margin���� ����� ��, Added by LHW [2008/7/02]

	//! �Ʒ� ������ �ݿ��Ϸ��� �ݵ�� m_bChanged�� true�� �ϰ� �ٽ� �׷��־�� ��
	bool m_bAutoRangeY_UsingRangeX;			//! true�̸� m_dbMinX_AutoRangeY, m_dbMaxX_AutoRangeY ������ X���� �ش��ϴ� Y���� ������, Auto Range ����, Added by LHW [2009/3/5]
	double m_dbMinX_AutoRangeY;				//! Y�� Auto Range�� ���� X�� �ּҰ�, Added by LHW [2009/3/5]
	double m_dbMaxX_AutoRangeY;				//! Y�� Auto Range�� ���� X�� �ִ밪, Added by LHW [2009/3/5]
	bool GetMinY_UsingRangeX(double &dbMinY, int iOverlapIndex = 0);//! m_dbMinX_AutoRangeY, m_dbMaxX_AutoRangeY ������ Y�� �ּҰ� ��ȯ, Added by LHW [2009/3/5]
	bool GetMaxY_UsingRangeX(double &dbMaxY, int iOverlapIndex = 0);//! m_dbMinX_AutoRangeY, m_dbMaxX_AutoRangeY ������ Y�� �ִ밪 ��ȯ, Added by LHW [2009/3/5]

	//! �ش� �׷����� �ִ밪�� index�� ��ȯ, ������ �߻��ϸ� -1�� ��ȯ
	int GetDataIndexofMaxValue(int iOverlapIndex);												//! ������ ��ü ���������� �ִ밪, Added by LHW [2008/4/15]
	int GetDataIndexofMaxValue(int iOverlapIndex, int iStartDataIndex, int iEndDataIndex);		//! �־��� Data Index���̿����� �ִ밪, Added by LHW [2009/2/27]
	int GetDataIndexofMaxValue(int iOverlapIndex, double dbStartValue, double dbEndValue);		//! �־��� X�� �� ������ �ִ밪, Added by LHW [2009/2/27]
	int GetDataIndexofMaxValue(int iOverlapIndex, COleDateTime timeStart, COleDateTime timeEnd);//! �־��� X�� �� ������ �ִ밪, Added by LHW [2009/2/27]

	bool IsAutoRange(bool bDirectionX, bool bDirectionY, bool bShow = false);	//! �ϳ��� Manual Range�� �����Ǿ� ������ false�� ��ȯ
																				//! bDirectionX�� true�̸� X�������� Ȯ��
																				//! bDirectionY�� true�̸� Y�������� Ȯ��
	bool IsShowGraph();			//! �ϳ��� �׷����� ���̸� true�� ��ȯ
	bool IsScreenDrawing();		//! ���� WM_PAINT �޽����� �޾� OnPaint()���� �׷����� �׸��� ���̸� true�� ��ȯ

	//! ���� index�� index ������ ��ȯ ����, ���� index�� ���� �ߺ����� �ʴ� ���� index, �����ڷ� ��� ����, Added by LHW [2008/4/11]
	//! Added by LHW [2008/6/13] : GetChildCursorIndexFromElementIndex, GetChildCursorElementIndexFromIndex �߰�
	//! Added by LHW [2008/8/23] : GetGraphEquationIndexFromElementIndex, GetGraphEquationElementIndexFromIndex �߰�
	//! Added by LHW [2008/11/28] : GetRegionCursorIndexFromElementIndex, GetRegionCursorElementIndexFromIndex �߰�
	//! �Էµ� ���� index, iElementIndex�� ���� ���� index�� ��ȯ
	bool GetCursorIndexFromElementIndex(int iElementIndex, int &iIndex);
	bool GetAuxCursorGroupIndexFromElementIndex(int iElementIndex, int &iIndex);
	bool GetChildCursorIndexFromElementIndex(int iElementIndex, int &iIndex);
	bool GetResourceYIndexFromElementIndex(int iElementIndex, int &iIndex);
	bool GetDataBufferIndexFromElementIndex(int iElementIndex, int &iIndex);
	bool GetXValueLineIndexFromElementIndex(int iElementIndex, int &iIndex);
	bool GetYValueLineIndexFromElementIndex(int iElementIndex, int &iIndex);
	bool GetGraphEquationIndexFromElementIndex(int iElementIndex, int &iIndex);
	bool GetRegionCursorIndexFromElementIndex(int iElementIndex, int &iIndex);
	//! ���� index�� ���� ���� index, iElementIndex�� ��ȯ
	bool GetCursorElementIndexFromIndex(int iIndex, int &iElementIndex);
	bool GetAuxCursorGroupElementIndexFromIndex(int iIndex, int &iElementIndex);
	bool GetChildCursorElementIndexFromIndex(int iIndex, int &iElementIndex);
	bool GetResourceYElementIndexFromIndex(int iIndex, int &iElementIndex);
	bool GetDataBufferElementIndexFromIndex(int iIndex, int &iElementIndex);
	bool GetXValueLineElementIndexFromIndex(int iIndex, int &iElementIndex);
	bool GetYValueLineElementIndexFromIndex(int iIndex, int &iElementIndex);
	bool GetGraphEquationElementIndexFromIndex(int iIndex, int &iElementIndex);
	bool GetRegionCursorElementIndexFromIndex(int iIndex, int &iElementIndex);

	//! Added by LHW [2008/4/3]
	//! Modify by LHW [2008/7/02] : protected���� public���� ����
	//! �־��� �����Ͱ��� ���� ������ ���˿� ���� Grid ������ ǥ�� ����, Ŀ���� ǥ�� ���ڷ� ��ȯ
	//! bDirectionX : X�� ���̸� true, bSecondRow : Time�̰� 2�ٷ� ǥ���� �� 2��° ���̸� true, bCursorText : Ŀ�� ���̸� true
	//! Overlap�Ǵ� �������� index(iOverlapIndex)�� 0 based�̰� Y�� ���� ���� ���
	CString GetVisualDataText(double dbData, bool bDirectionX, bool bSecondRow, bool bCursorText, int iOverlapIndex = 0);

	CString GetDeltaTimeText(double dbDeltaTime);	//! Added by LHW [2008/8/11], �ð� ���� ���ڿ��� ��ȯ
	
	
protected:
	int m_iPlotIndex;					//! �׷��� ��ȣ

	CGraph2D_Resource *m_pPlotResource;	//! ���ҽ� ��ü
	bool m_bSelfCreateResourcePtr;		//! ���� ���ҽ� ��ü�� �����ϸ� true

	strXRangeMapper	m_XMapper;													//! �����͸� ȭ�� ���� X ��ǥ�� ���
	CArray<strYRangeMapper, strYRangeMapper&> m_arrYMapper;						//! �����͸� ȭ�� ���� Y ��ǥ�� ���
	CTypedPtrArray<CPtrArray, CGraph2D_ResourceY*> m_arrResourceY;				//! Y�� ������ ���ҽ� ������ �����
	CTypedPtrArray<CPtrArray, CGraph2D_Data*> m_arrPlotData;					//! �׷��� ������ �����
	CTypedPtrArray<CPtrArray, CGraph2D_ValueLineX*> m_arrValueLineX;			//! X�� Ư�� �� ǥ�ü� ���� �����, Added by LHW [2008/4/19]
	CTypedPtrArray<CPtrArray, CGraph2D_ValueLineY*> m_arrValueLineY;			//! Y�� Ư�� �� ǥ�ü� ���� �����, Added by LHW [2008/3/29]
	CArray<strGraph2DCursorInfo, strGraph2DCursorInfo&> m_arrCursor;			//! Ŀ�� ���� ����ü
	CTypedPtrArray<CPtrArray, CGraph2D_Aux_CursorGroup*> m_arrAuxCursorGroup;	//! ���� Ŀ�� ���� �����, Added by LHW [2008/3/31]
	CTypedPtrArray<CPtrArray, CGraph2D_Child_Cursor*> m_arrChildCursor;			//! �ڽ� Ŀ�� ���� �����, Added by LHW [2008/6/13]
	CTypedPtrArray<CPtrArray, CGraph2D_Equation*> m_arrGraphEquation;			//! �Լ������� �׷����� �׷����� ���� �����, Added by LHW [2008/8/22]
	CTypedPtrArray<CPtrArray, CGraph2D_Region_Cursor*> m_arrRegionCursor;		//! ���� ǥ�� Ŀ�� ���� �����, Added by LHW [2008/11/28]
	
	bool IsValidCursorIndex(int iCursorIndex);						//! iCursorIndex�� Ŀ���� ��ȿ�ϸ� true�� ��ȯ, Added by LHW [2008/6/15]
	bool IsValidOverlapIndex(int iOverlapIndex);					//! iOverlapIndex�� �׷����� ���� ��� �����ϸ� true�� ��ȯ
	bool IsValidXValueLineIndex(int iLineIndex);					//! iLineIndex�� X�� Ư���� ǥ�ü��� ��ȿ�ϸ� true�� ��ȯ
	bool IsValidYValueLineIndex(int iLineIndex);					//! iLineIndex�� Y�� Ư���� ǥ�ü��� ��ȿ�ϸ� true�� ��ȯ
	bool IsValidChildCursorIndex(int iChildCursorIndex);			//! iChildCursorIndex�� ��ȿ�ϸ� true�� ��ȯ, Added by LHW [2008/6/14]
	bool IsValidGraphEquationIndex(int iGraphIndex);				//! iGraphIndex�� ��ȿ�ϸ� true�� ��ȯ, Added by LHW [2008/8/23]
	bool IsValidRegionCursorIndex(int iRegionCursorIndex);			//! iRegionCursorIndex�� ��ȿ�ϸ� true�� ��ȯ, Added by LHW [2008/11/28]

	E_GRAPH2D_MOVE_CURSOR_TYPE m_eSelectCursorType;					//! ���� ���콺, Ű����� ���������� ������ Ŀ���� ����, Added by LHW [2008/6/16]
	
	int m_iSelectCursorIndex;										//! ���� ���õ� Ŀ���� Index
	int m_iSelectedCursorIndex;										//! ���������� ���õ� Ŀ���� index

	bool m_bCursorSelected;			//! Ŀ���� ���õǾ� �����̴� ���̸� true
	bool m_bCursorOverlap;			//! true�̸� Overlap�� �׷��� ����� ������ �˻�, false�̸� ���� ���õ� �׷��� �����Ϳ����� �˻�
									//! ����, Ŀ���� ������ ���� Ŭ���ϸ� true�� �����ǰ� �Ǿ� ����
	bool m_bCursorInfoSelected;		//! Ŀ�� ����â�� ���õǾ� �����̴� ���̸� true
	CPoint m_ptSave;				//! ���콺�� ������ Ŭ���� ������ ����, AddCursorInMouseClick �Լ����� ���
	int m_iCursorMargin;			//! Ŀ�� ���� ó���� �ʿ��� ���� pixel ��

	strGraph2D_MouseDown_Info m_strMouseDownInfo;				//! ���콺 Ŭ���� �������� ����
	strGraph2D_Cursor_Event m_strCursorEventInfo;				//! Ŀ�� ���� �̺�Ʈ ���� ����
	strGraph2D_DataPoint_Event m_strDataPointEventInfo;			//! �׷����� ������ �߻��� �̺�Ʈ ���� ����, Added by LHW [2008/5/23]
	strGraph2D_Aux_Cursor_Event m_strAuxCursorEventInfo;		//! ���� Ŀ�� ���� �̺�Ʈ ���� ����, Added by LHW [2008/8/5]
	strGraph2D_Region_Cursor_Event m_strRegionCursorEventInfo;	//! ���� ǥ�� Ŀ�� ���� �̺�Ʈ ���� ����, Added by LHW [2008/11/28]

	int m_bMouseMoveProcessing;	//! ���콺�� �������� Ŀ�� ���� �۾��� �ϴ� ���� true�� ����, ���ϸ� ���̱� ���� ���, Added by LHW [2008/7/24]
	
	bool CalcCursorPos(int iXPos, int iYPos, bool bAddCursorFlag, strGraph2DCursorInfo* pstrCursorInfo);	//! Ŀ�� ��ġ ���
								//! Ŀ�� �߰��� ���� bAddCursorFlag�� true�̾�� ��, Modify by LHW [2008/4/17]
	bool SetCursorInfo(int iCursorDataIndex, strGraph2DCursorInfo* pstrCursorInfo, int iOverlapIndex = 0);	//! �־��� Data index�� �ش��ϴ� ������ Ŀ�� ����ü�� ����
	void ReCalcCursorPos();		//! MouseMove �� Ŀ�� ��ġ �ٽ� ���, �׷��� ũ�� ��ȭ�� ���� Ŀ���� ��ġ �ٽ� ���
	bool SendCursorEventMsg(int iCursorIndex, bool bMouseOperation, E_GRAPH2D_CURSOR_OPERATION_MODE eOperationMode, strGraph2DCursorInfo* pstrCursorInfo);	//! Ŀ�� �̺�Ʈ �޽��� �߼�
								//! iCursorIndex : Ŀ�� index, bMouseOperation : ���콺�� �߻��� �̺�Ʈ�̸� true
								//! eOperationMode : Ŀ���� ���� ����

	//! �ڽ� Ŀ���� ��ġ, ������ ���� ó��, Added by LHW [2008/6/13]
	int m_iSelectChildCursorIndex;	//! ���� ���õ� �ڽ� Ŀ���� Index
	int m_iSelectedChildCursorIndex;//! ���������� ���õ� �ڽ� Ŀ���� index, Added by LHW [2008/6/14]
	bool m_bChildCursorSelected;	//! �ڽ� Ŀ���� ���õǾ� �����̴� ���̸� true
	bool m_bChildCursorOverlap;		//! true�̸� Overlap�� �׷��� ����� ������ �˻�, false�̸� ���� ���õ� �׷��� �����Ϳ����� �˻�
									//!  ����, �ڽ� Ŀ���� ������ ���� Ŭ���ϸ� true�� �����ǰ� �Ǿ� ����
	bool m_bChildCursorInfoSelected;//! �ڽ� Ŀ�� ����â�� ���õǾ� �����̴� ���̸� true
	int m_iChildCursorMargin;		//! �ڽ� Ŀ�� ���� ó���� �ʿ��� ���� pixel ��
	bool CalcChildCursorPos(int iXPos, int iYPos, bool bAddCursorFlag, int iChildCursorIndex);	//! �ڽ� Ŀ�� ��ġ ���
																								//! Ŀ�� �߰��� ���� bAddCursorFlag�� true�̾�� ��
	//! �־��� Data index�� �ش��ϴ� ������ ����
	//! Modify by LHW [2008/6/16] : bAddFlag �߰�, �ڽ� Ŀ���� ���� �߰��� ���� bAddFlag�� true�� ����
	bool SetChildCursorInfo(int iChildCursorDataIndex, int iChildCursorIndex, bool bAddFlag, int iOverlapIndex = 0);
	void ReCalcChildCursorPos();//! MouseMove �� �ڽ� Ŀ�� ��ġ �ٽ� ���, �׷��� ũ�� ��ȭ�� ���� �ڽ� Ŀ���� ��ġ �ٽ� ���

	//! Ŀ���� �ڽ� Ŀ���� ����, Added by LHW [2008/6/15]
	bool SetFlag_If_CursorMoved(int iCursorIndex);	//! iCursorIndex�� Ŀ���� �������� ���
	//bool UpdateCursorDataIndex_Childcursor();		//! Ŀ���� ������ index�� �ڽ� Ŀ���� �ݿ�

	//! ���� ǥ�� Ŀ���� ��ġ, ������ ���� ó��, Added by LHW [2008/11/28]
	//! Modify by LHW [2008/11/30] : iRegionCursorLineIndex, m_iSelectRegionCursorLineIndex, m_iRegionCursorMargin �߰�
	//! Modify by LHW [2008/12/1] : SendRegionCursorEventMsg �߰�
	int m_iSelectRegionCursorIndex;		//! ���� ���õ� ���� ǥ�� Ŀ���� Index
	int m_iSelectedRegionCursorIndex;	//! ���������� ���õ� ���� ǥ�� Ŀ���� index
	int m_iSelectRegionCursorLineIndex;	//! ���� ���õ� ���� ǥ�� Ŀ���� Ŀ�� ���� Index
	int m_iRegionCursorMargin;			//! ���� ǥ�� Ŀ�� ���� ó���� �ʿ��� ���� pixel ��
	bool m_bRegionCursorSelected;		//! ���� ǥ�� Ŀ���� ���õǾ� �����̴� ���̸� true
	bool CalcRegionCursorPos(int iXPos, int iYPos, int iRegionCursorIndex, int iRegionCursorLineIndex);
	bool CalcRegionCursor();
	bool SendRegionCursorEventMsg(int iRegionCursorIndex);

	//! ���� Ŀ���� ��ġ, ������ ���� ó��
	bool CalcAuxCursorPos(int iXPos, int iYPos, int iAuxCursorGroupIndex, int iAuxCursorIndex);	//! ���� Ŀ���� ��ġ ���, Added by LHW [2008/8/5]
	bool CalcAuxCursor();		//! ���� Ŀ���� X���� ���� Y���� X, Y ��ġ�� ��� ����, Added by LHW [2008/4/1]
	//! Added by LHW [2008/8/4]
    bool m_bAuxCursorSelected;	//! ���� Ŀ���� ���õǾ� �����̴� ���̸� true
	int m_iSelectAuxCursorGroupIndex;	//! ���� ���õ� ���� Ŀ�� �׷��� index
	int m_iSelectedAuxCursorGroupIndex;	//! ���������� ���õ� ���� Ŀ�� �׷��� index
	int m_iSelectAuxCursorIndex;		//! ���� ���õ� ���� Ŀ���� index
	int m_iSelectedAuxCursorIndex;		//! ���������� ���õ� ���� Ŀ���� index

	bool m_bLeftMouseDown;		//! true�̸� ���� ���콺 �ٿ� ����, Added by LHW [2008/5/28]
	bool m_bRightMouseDown;		//! true�̸� ������ ���콺 �ٿ� ����, Added by LHW [2009/2/2]
	bool m_bMouseMoved;			//! true�̸� OnMouseMove�� ȣ��� ����, Added by LHW [2009/2/2]

	bool m_bZoomMouseDown;		//! true�̸� Zoom�� ���� �簢 ���� ���� ��
	bool m_bPanMouseDown;		//! true�̸� Pan ���� ���� ��
	bool m_bAxisEventFront;		//! true�̸� ���콺 �̺�Ʈ�� �߻��� ��ġ�� ���� �������� �ǹ�, Added by LHW [2009/2/2]

	strGraph2D_Zoom_Event m_strZoomEventInfo;	//! Zoom ���� �̺�Ʈ ���� ����
	bool SendZoomEventMsg(E_GRAPH2D_ZOOM_OPERATION_MODE eZoomMode);	//! Zoom �̺�Ʈ �޽��� �߼�, Added by LHW [2008/4/19]

	bool SetDataPointRect();		//! �׷����� �� ���� �̺�Ʈ �簢 ������ ����

	int m_iPanOffsetX;				//! Pan ��ɿ��� X�� ���������� Offset��
	int m_iPanOffsetY;				//! Pan ��ɿ��� Y�� ���������� Offset��
	strPanPosition m_strPanPosition;//! Pan ��ɿ��� ���콺�� ������ �������� ��ħ���� ��ǥ
	bool ApplyPanPosition();		//! m_strPanPosition�� ������ Range Offset������ ����

	double GetCalcData(double dbData, bool bDirectionX, bool bCursorText, int iOverlapIndex = 0);//! ����Ǿ� ���� �������� �����͸� ��ȯ, Added by LHW [2008/8/02]

	double GetFloor(double dbInput, int iResultMode);	//! �Ҽ��� �ݿø� �Լ�
														//! [iResultMode] 0 �̻��� ���� ���� 
														//! (��. 1 : �Ҽ��� 2° �ڸ����� �ݿø�, 2 : �Ҽ��� 3° �ڸ����� �ݿø�)
	
	//! �� (iX1, iY1), �� (iX2, iY2)�� �������� ����Ǿ� ���� ��, �� ������ �� iInputX�� Y��ǥ�� ����Ͽ� ��ȯ, Added by LHW [2008/4/1]
	int CalcLinearInterpolate(int iX1, int iY1, int iX2, int iY2, int iInputX);

	double CalcPolynomialEquation(double *pdbCoeff, int iCoeffSize, double dbInput);	//! �־��� ����� ���� ���׽��� �Է°��� ���� ��°��� ��ȯ, Added by LHW [2008/8/23]

	double DecideMinMaxOperation(double dbInput);	//! �Էµ� �ּ�, �ִ밪�� ������ ��Ģ�� ���� ���ο� �ּ�, �ִ밪�� ���Ͽ� ��ȯ, Added by LHW [2009/2/4]
													//! �ִ밪�� �Է°��� ����� ��, �ּҰ��� �Է°��� ������ ���� ��� ����
													//! �׷����� ������ ���� ���� �ϱ� ���� ���

	//! Added by LHW [2009/3/1]
	//! �־��� Data Index�� �ش��ϴ� X�� �����Ϳ� ���� �����ϴ� Data Index�� �˾Ƴ�, iMargin�� 0�̸� ��� ������ �˻�
	bool SearchEqualValueX(int iOverlapIndex1, int iDataIndex1, int iOverlapIndex2, int &iDataIndex2, int iMargin = 0);

	//! Added by LHW [2009/5/30]
	//! X�� ���� Step ���� ������ �Է°��� �״�� ��ȯ
	double GetCompareXMinBound(double dbInput);	//! �־��� ���� X�� ���� Step ������ ���Ͽ� �ּҰ� ��ȯ
	double GetCompareXMaxBound(double dbInput);	//! �־��� ���� X�� ���� Step ������ ���Ͽ� �ִ밪 ��ȯ

	//! Added by LHW [2009/5/30]
	//! �־��� iOverlapIndex�� Y�� ���� Step ���� ������ �Է°��� �״�� ��ȯ
	double GetCompareYMinBound(int iOverlapIndex, double dbInput);	//! �־��� ���� Y�� ���� Step ������ ���Ͽ� �ּҰ� ��ȯ
	double GetCompareYMaxBound(int iOverlapIndex, double dbInput);	//! �־��� ���� Y�� ���� Step ������ ���Ͽ� �ִ밪 ��ȯ

	//! Added by LHW [2008/4/4]
	//! �־��� iOverlapIndex�� Y�� Ư���� ǥ�ü��� ������ �Է°��� �״�� ��ȯ
	double GetMinCompareYValueLine(int iOverlapIndex, double dbInput);	//! �־��� ���� Y�� Ư���� ǥ�ü��� ������ ���Ͽ� �ּҰ� ��ȯ
	double GetMaxCompareYValueLine(int iOverlapIndex, double dbInput);	//! �־��� ���� Y�� Ư���� ǥ�ü��� ������ ���Ͽ� �ִ밪 ��ȯ

	//! Added by LHW [2008/8/25]
	//! �־��� iOverlapIndex�� ����� �Լ������� �׷����� �׷����� ������ �Է°��� �״�� ��ȯ
	double GetMinCompareYEquation(int iOverlapIndex, double dbInput);	//! �־��� ���� �Լ������� �׷����� �׷����� ������ ���Ͽ� Y�� �ּҰ� ��ȯ
	double GetMaxCompareYEquation(int iOverlapIndex, double dbInput);	//! �־��� ���� �Լ������� �׷����� �׷����� ������ ���Ͽ� Y�� �ִ밪 ��ȯ

	//! Added by LHW [2008/12/14]
	//! �־��� iOverlapIndex�� ���� ǥ�� Ŀ���� ������ �Է°��� �״�� ��ȯ
	double GetMinCompareYRegionCursor(int iOverlapIndex, double dbInput);	//! �־��� ���� ���� ǥ�� Ŀ���� ������ ���Ͽ� �ּҰ� ��ȯ
	double GetMaxCompareYRegionCursor(int iOverlapIndex, double dbInput);	//! �־��� ���� ���� ǥ�� Ŀ���� ������ ���Ͽ� �ִ밪 ��ȯ

	bool SetGraphEquationMinMaxY();	//! �Լ������� �׷����� �׷����� Y�� �ּ�, �ִ밪�� ã�Ƴ��� ����, Added by LHW [2008/8/25]

	//! ���� index ������ ���� ����, �Լ�, Added by LHW [2008/4/11]
	//! Added by LHW [2008/4/19] : GetXValueLinePossibleElementIndex, IsExistXValueLineElementIndex �߰�
	//! Added by LHW [2008/6/13] : GetChildCursorPossibleElementIndex, IsExistChildCursorElementIndex �߰�
	//! Added by LHW [2008/8/23] : GetGraphEquationPossibleElementIndex, IsExistGraphEquationElementIndex �߰�
	//! Added by LHW [2008/11/28] : GetRegionCursorPossibleElementIndex, IsExistRegionCursorElementIndex �߰�
	//! ��� ������ ���� index ��ȯ, ������ ������ -1�� ��ȯ
	int GetCursorPossibleElementIndex();		
	int GetAuxCursorGroupPossibleElementIndex();
	int GetChildCursorPossibleElementIndex();	
	int GetResourceYPossibleElementIndex();		
	int GetDataBufferPossibleElementIndex();	
	int GetXValueLinePossibleElementIndex();	
	int GetYValueLinePossibleElementIndex();
	int GetGraphEquationPossibleElementIndex();
	int GetRegionCursorPossibleElementIndex();
	//! �Էµ� ����index�� ���� ��Ұ� ������ true�� ��ȯ
	bool IsExistCursorElementIndex(int iElementIndex);			
	bool IsExistAuxCursorGroupElementIndex(int iElementIndex);	
	bool IsExistChildCursorElementIndex(int iElementIndex);
	bool IsExistResourceYElementIndex(int iElementIndex);		
	bool IsExistDataBufferElementIndex(int iElementIndex);		
	bool IsExistXValueLineElementIndex(int iElementIndex);		
	bool IsExistYValueLineElementIndex(int iElementIndex);
	bool IsExistGraphEquationElementIndex(int iElementIndex);
	bool IsExistRegionCursorElementIndex(int iElementIndex);

	//! Added by LHW [2008/5/28]
	bool m_bDataPointSearchProcessing;		//! Data-Point Event ������ ���콺�� �� �ִ��� Ȯ�� �۾� ���̸� true�� ����, Added by LHW [2008/9/19]
	bool m_bDataPointSearchComplete;		//! Data-Point Event ������ ���콺�� �� ������ �ش� ������ ����ü�� ���������� true�� ����
	bool SearchDataPoint(CPoint ptInput);	//! Data-Point Event ������ ���콺�� �� ������ true�� ��ȯ
	
	bool m_bDrawing;		//! �׷����� �׸��� �����̸� true
	bool m_bCalcCursor;		//! Ŀ�� ���� ó�� ���̸� true
	bool m_bCalcChildCursor;//! �ڽ� Ŀ�� ���� ó�� ���̸� true, Added by LHW [2008/6/13]
	bool m_bCalcAuxCursor;	//! ���� Ŀ�� ���� ó�� ���̸� true, Added by LHW [2008/8/4]

	bool m_bTemporaryMouseOperation;	//! true�̸� m_eMouseOperationMode�� E_GRAPH2D_CURSOR_XY�� ��, �ӽ÷� Ȯ��, ���, Pan �۾��� ����, Added by LHW [2009/2/2]
	
	//! ���콺 Ŀ���� �ڵ鰪
	HCURSOR	m_hCursor_Horz;
	HCURSOR	m_hCursor_Vert;
	HCURSOR m_hCursor_Move;
	HCURSOR m_hCursor_Pan_DOWN;
	HCURSOR m_hCursor_Pan_UP;
	HCURSOR m_hCursor_DataPoint_Event;	//! Data-Point Event���� ���, Added by LHW [2008/5/28]
	HCURSOR m_hCursor_Zoom_X;			//! Added by LHW [2009/2/2]
	HCURSOR m_hCursor_Zoom_Y;			//! Added by LHW [2009/2/2]
	HCURSOR m_hCursor_Zoom_XY;			//! Added by LHW [2009/2/2]
	HCURSOR m_hCursor_Pan_X;			//! Added by LHW [2009/2/2]
	HCURSOR m_hCursor_Pan_Y;			//! Added by LHW [2009/2/2]

	int m_iRealGraphHeight;	//! ���� �׷����� �׷����� �κ��� ����

	int m_iGridMarginX, m_iGridMarginY;	//! Grid�� ���� ���ݰ�, Added by LHW [2007/7/24]
	int m_iAxisCaptionMarginX;			//! ���� ���ڿ� ǥ�ø� ���� ���� ���ݰ�, Added by LHW [2008/11/28]

	CPoint m_ptMouseDownPos;//! ���콺 ���� Ŭ�� �� ������ ����, Added by LHW [2009/1/6]

	bool m_bCreateDC;		//! �޸� DC �ʱ�ȭ�ϸ� true
	CDC  m_DrawDC;			//! �޸� DC
	CBitmap  m_DrawBitmap;
	CBitmap *m_pOldDrawBitmap;
	CRect m_CurRect;			//! �׷��� ��Ʈ�� ��ü�� �簢 ����
	CRect m_MainTitleRect;		//! �� ������ �簢 ����
	CRect m_SubTitleRect;		//! ���� ������ �簢 ����
	CRect m_AxisCaptionRect;	//! ���� ���ڿ� ǥ�ø� ���� �簢 ����, Added by LHW [2008/8/01]	
	CRect m_crAxisX_Event;		//! X�� ǥ�� �簢 ����, Added by LHW [2009/2/2]
	CRect m_crAxisY_Left_Event;	//! ������ Y�� ǥ�� �簢 ����, Added by LHW [2008/7/31], ���� �� ����, Modify by LHW [2009/2/2]
	CRect m_crAxisY_Right_Event;//! �������� Y�� ǥ�� �簢 ����, Added by LHW [2008/7/31], ���� �� ����, Modify by LHW [2009/2/2]
	CRect m_DataRect;			//! ���� �׷����� �׸��� ������ �簢 ���� (���� ����)
	CRect m_graphRect;			//! ���� �׷����� �׸��� ������ �簢 ���� (���� �������� ����)
	CRect m_RealGraphRect;		//! �׷����� ���� �簢 ����
	CRect m_crZoomArea;			//! Ȯ�� ������ �簢 ����
	
	CRgn m_DrawRgn;				//! ���� �׷����� �׸��� ����
	
	//! ����� ���ҽ� ��ü
	CPen* m_penOld;
	CFont* m_pOldFont;
	CBrush* m_pOldBrush;
	COLORREF m_clrBackup;

	bool ReadyDraw(CDC *pDC);		//! �׷����� �׸��� ���� �ʿ��� �۾�(�簢 ���� ũ�� ����, X,Y�� ������ Mapping ���� ���� ��)�� ����

	bool GetStringMaxWidth_AxisY(CDC *pDC, int iOverlapIndex, double dbMinValue, double dbMaxValue, int &iMaxWidth);	//! Y�� ���� ���ڿ��� �ִ� �ʺ� ��ȯ, Added by LHW [2008/10/25]
	
	bool DrawInit(CDC *pDC);												//! �޸� DC �ʱ�ȭ
	bool DrawBackground(CDC *pDC);											//! ����
	bool DrawAxis(CDC *pDC);												//! X-Y ��, ����
	bool DrawAxisY(CDC *pDC, int iOverlapIndex, bool bOnlyAxis, bool bLeft);//! Y��, Modify by LHW [2008/4/4]
																			//! bOnlyAxis�� true�̸� Y���� �Ѱ��� �ִ� ���
																			//! bLeft�� true�̸� ���ʿ� ���� �׷���
	bool DrawInfo(CDC *pDC);												//! �׷��� ������ ǥ��
	bool DrawData(CDC *pDC);												//! �׷���
	bool DrawGraph(CDC *pDC, int iOverlapIndex);							//! ������ �׷����� �׸�, Added by LHW [2008/7/24]
	bool DrawValueLineX(CDC *pDC);											//! X�� Ư���� ǥ�� ��, Added by LHW [2008/4/19]
	bool DrawValueLineY(CDC *pDC);											//! Y�� Ư���� ǥ�� ��, Added by LHW [2008/3/29]
	bool DrawEquation(CDC *pDC, int iOverlapIndex);							//! �Լ������� �׷����� �׷����� �׸�, Added by LHW [2008/8/23]
																			//! iOverlapIndex�� 0�̻��� ����, �ش� iOverlapIndex�� ����� �׷����� �׸� 																			
	bool DrawAuxCursor(CDC *pDC);											//! ���� Ŀ��, Added by LHW [2008/3/31]
	bool DrawCursor(CDC *pDC);												//! Ŀ��
	bool DrawChildCursor(CDC *pDC);											//! �ڽ� Ŀ��, Added by LHW [2008/6/13]
	bool DrawRegionCursor(CDC *pDC);										//! ���� ǥ�� Ŀ��, Added by LHW [2008/11/28]
	bool DrawZoomArea(CDC *pDC);											//! ���õ� Ȯ�� ������ ǥ��

	//! �� ������ ��ġ�� ǥ���ϴ� �ɺ��� �׸��� �Լ�, Added by LHW [2009/2/3]
	bool DrawSymbol(CDC *pDC, int iXPt, int iYPt, E_GRAPH2D_DRAW_SYMBOL_TYPE eType, int iSymbolSize, CBrush *pBrush = NULL);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


