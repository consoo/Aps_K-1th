//! 2D ������ǥ �׷��� ��Ʈ���� ����, �� �β�, Range, Y�� Ư���� ǥ�ü�, Ȯ�� Record ���� ���� ���� ������ �����ϴ� Ŭ����
//! Maked by LHW (2008/3/14 ~ )
//! ���� �۾��� [2009/6/4]
//! Modify by LHW [2008/6/12] : operator ������ ������ �κ��� ���� �ϰ� ����

#pragma once

#include <afxtempl.h>			//! CArray, CTypedPtrArray ���
#include ".\\Graph2D_Struct.h"

struct strZoomRecord
{
	bool bAutoRange;
	double dbMin;
	double dbMax;
	
	void InitInfo()
	{
		bAutoRange = true;
		dbMin = 0.;
		dbMax = 0.;		
	}

	strZoomRecord& operator=(const strZoomRecord &info)  //! = ������ ������
	{
		bAutoRange = info.bAutoRange;
		dbMin = info.dbMin;
		dbMax = info.dbMax;		

		return *this;
	}
};

//! Modify by LHW [2008/5/14] : m_penCursorLineSelected �߰�
//! Modify by LHW [2008/6/30] : m_eGridTextType, m_eCursorTextType, m_iGridTextPrecision, m_iCursorTextPrecision �߰�
//! Modify by LHW [2008/7/02] : m_dbAutoRangeMin, m_dbAutoRangeMax �߰�
//! Modify by LHW [2009/5/30] : X�� �ּ�, �ִ밪�� Step ������ �߰�
class CGraph2D_Resource
{
public:
	CGraph2D_Resource(void);
	~CGraph2D_Resource(void);

	strGraph2DResourceInfo m_strResourceInfo;

	E_GRAPH2D_GRID_TEXT_TYPE m_eGridTextType;	//! Grid ǥ�� ������ ǥ�� ����(������, �Ǽ���, ������)
	E_GRAPH2D_GRID_TEXT_TYPE m_eCursorTextType;	//! Ŀ�� ǥ�� ������ ǥ�� ����(������, �Ǽ���, ������)
	int m_iGridTextPrecision;					//! Grid�� �Ҽ��� �ڸ���
	int m_iCursorTextPrecision;					//! Ŀ������ �Ҽ��� �ڸ���
	double m_dbAutoRangeMin;					//! Auto Range�� ���� �ּҰ�
	double m_dbAutoRangeMax;					//! Auto Range�� ���� �ִ밪

	CBrush		m_brEntireBackGround;
	CBrush		m_brGraphBackGround;
	CBrush		m_brCursorPoint;
	CBrush		m_brCursorTextBack;
	CBrush		m_brZoomArea;

	CPen		m_penAxis;
	CPen		m_penGrid;	
	CPen		m_penCursorLine;
	CPen		m_penCursorLineSelected;
	CPen		m_penCursorTextBorder;

	CFont		m_ftGridText;
	CFont		m_ftCursorText;
	CFont		m_ftLegend;
	CFont		m_ftMainTitle;
	CFont		m_ftSubTitle;

	CArray<strZoomRecord, strZoomRecord&> m_arrZoomRecord;		//! Ȯ�� ����� ����� ������ Ȯ�� ���� ������ ����

	double m_dbPanOffset;	//! Pan ��ɿ��� X�� ���������� Offset��

	CArray<double, double> m_arrMinBound;	//! X�� �ּҰ��� Step ������
	CArray<double, double> m_arrMaxBound;	//! X�� �ִ밪�� Step ������
	
	void SetResource();
	void ReleaseResource();
	void AddZoomRecord();
};

//! Modify by LHW [2008/4/10] : m_brData �߰�
//! Modify by LHW [2008/4/11] : m_iElementIndex �߰�
//! Modify by LHW [2008/5/23] : m_penEvent �߰�
//! Modify by LHW [2008/6/30] : m_eGridTextType, m_eCursorTextType, m_iGridTextPrecision, m_iCursorTextPrecision �߰�
//! Modify by LHW [2008/7/02] : m_dbAutoRangeMin, m_dbAutoRangeMax �߰�
//! Modify by LHW [2008/10/25] : m_iLeftXPtofYAxis, m_iRightXPtofYAxis �߰�
//! Modify by LHW [2009/2/3] : m_brData�� m_brSymbol�� ����, m_penSymbol, m_brEvent �߰�
//! Modify by LHW [2009/2/14] : m_dbTempRangeMax, m_dbTempRangeMin �߰�
//! Modify by LHW [2009/5/30] : Y�� �ּ�, �ִ밪�� Step ������ �߰�
class CGraph2D_ResourceY
{
public:
	CGraph2D_ResourceY(void);
	~CGraph2D_ResourceY(void);

	int m_iElementIndex;	//! Y�� ���ҽ� ������� ���� index

	strGraph2D_Y_Axis_Info m_strResourceYinfo;

	E_GRAPH2D_GRID_TEXT_TYPE m_eGridTextType;	//! Grid ǥ�� ������ ǥ�� ����(������, �Ǽ���, ������)
	E_GRAPH2D_GRID_TEXT_TYPE m_eCursorTextType;	//! Ŀ�� ǥ�� ������ ǥ�� ����(������, �Ǽ���, ������)
	int m_iGridTextPrecision;					//! Grid�� �Ҽ��� �ڸ���
	int m_iCursorTextPrecision;					//! Ŀ������ �Ҽ��� �ڸ���
	double m_dbAutoRangeMin;					//! Auto Range�� ���� �ּҰ�
	double m_dbAutoRangeMax;					//! Auto Range�� ���� �ִ밪
	int m_iLeftXPtofYAxis;						//! ���ʿ� Y���� ������ X ��ǥ
	int m_iRightXPtofYAxis;						//! �����ʿ� Y���� ������ X ��ǥ

	double m_dbTempRangeMax;					//! Range �ִ밪�� �ӽ� ����
	double m_dbTempRangeMin;					//! Range �ּҰ��� �ӽ� ����

	CPen		m_penYAxis;	//! Y�༱�� �׸��� ��, Multi-Axis�� �� �ʿ�
	CPen		m_penData;	//! ���� �׷����� �׸��� ��
	CPen		m_penSymbol;//! ���� �׷��� ������ ���� �׸��� ��
	CPen		m_penEvent;	//! Data-Point Event ������ �׸� �� ����ϴ� ��
	CBrush		m_brSymbol;	//! ���� �׷��� ������ ���� �׸��� �귯��
	CBrush		m_brEvent;	//! Data-Point Event ������ �׸� �� ����ϴ� �귯��
	int			m_iDrawXPt;	//! Y�༱�� �׸� X ��ǥ

	CArray<strZoomRecord, strZoomRecord&> m_arrZoomRecord;		//! Ȯ�� ����� ����� ������ Ȯ�� ���� ������ ����

	double		m_dbPanOffset;	//! Pan ��ɿ��� Y�� ���������� Offset��

	CArray<double, double> m_arrMinBound;	//! Y�� �ּҰ��� Step ������
	CArray<double, double> m_arrMaxBound;	//! Y�� �ִ밪�� Step ������
	
	void SetResource();
	void ReleaseResource();
	void AddZoomRecord();
};

//! X�� Ư�� �� ǥ�ü��� Ŭ����, Added by LHW [2008/4/19]
class CGraph2D_ValueLineX
{
public:
	CGraph2D_ValueLineX(void);
	~CGraph2D_ValueLineX(void);

	int m_iElementIndex;	//! X�� Ư�� �� ǥ�ü��� ���� index

	strValueLineXInfo m_strXLineInfo;	//! X�� Ư�� �� ǥ�ü��� ���� ����

	CPen		m_penXLine;
	CRect		m_crSelect;	//! ���콺 �̺�Ʈ ����

	int			m_iFontHeight;
	CFont		m_ftValueLineX;
    
	void SetResource();
	void ReleaseResource();
};

//! Y�� Ư�� �� ǥ�ü��� Ŭ����, Added by LHW [2008/3/29]
//! Modify by LHW [2008/4/11] : m_iElementIndex �߰�
//! Modify by LHW [2009/2/3] : m_iFontHeight, m_ftValueLineY �߰�
class CGraph2D_ValueLineY
{
public:
	CGraph2D_ValueLineY(void);
	~CGraph2D_ValueLineY(void);

	int m_iElementIndex;	//! Y�� Ư�� �� ǥ�ü��� ���� index

	strValueLineYInfo m_strYLineInfo;	//! Y�� Ư�� �� ǥ�ü��� ���� ����

	CPen		m_penYLine;

	int			m_iFontHeight;
	CFont		m_ftValueLineY;

	void SetResource();
	void ReleaseResource();
};

//! Added by LHW [2008/4/11] : ���� Ŀ�� ����ü���� ����ڰ� ���� �Է��ϸ� �ȵǴ� �κ��� �и�
//! Modify by LHW [2008/5/31] : iRealDataIndex, dbXData, dbYData �߰�
//! Modify by LHW [2008/8/04] : crSelectX, crSelectY �߰�
struct strGraph2D_Aux_Cursor_Extend
{
	int iElementIndex;	//! ���� Ŀ�� ����ü�� ���� index
	bool bValid;		//! true�̸� ���� Ŀ���� ������ ��ȿ

	int iRealDataIndex;	//! ���� ������ index, �ƴϸ� ���� ����� �������� index
	double dbXData;		//! X ��
	double dbYData;		//! Y ��

	int iXPos;			//! X ��ġ
	int iYPos;			//! Y ��ġ

	strGraph2D_Aux_Cursor strAuxCursorUser;	//! ����ڰ� �Է��� �� �ִ� ���� Ŀ�� ����ü

	CRect crSelectX;	//! Ŀ�� ���� ����
	CRect crSelectY;	//! Ŀ�� ���� ����

	void InitInfo()
	{
		iElementIndex = -1;
		bValid = true;
		iXPos = 0;
		iYPos = 0;
		iRealDataIndex = -1;
		dbXData = 0.;
		dbYData = 0.;
		strAuxCursorUser.InitInfo();
		crSelectX.SetRectEmpty();
		crSelectY.SetRectEmpty();
	}

	strGraph2D_Aux_Cursor_Extend& operator=(const strGraph2D_Aux_Cursor_Extend &input)
	{
		iElementIndex = input.iElementIndex;
		bValid = input.bValid;
		iXPos = input.iXPos;
		iYPos = input.iYPos;
		iRealDataIndex = input.iRealDataIndex;
		dbXData = input.dbXData;
		dbYData = input.dbYData;
		strAuxCursorUser = input.strAuxCursorUser;
		crSelectX = input.crSelectX;
		crSelectY = input.crSelectY;

		return *this;
	}
};

//! ���� Ŀ�� �׷��� Ŭ����, Added by LHW [2008/3/31]
//! Modify by LHW [2008/4/11] : strGraph2D_Aux_Cursor_Extend�� m_arrCursorInfo�� �����ϵ��� ����
//! Modify by LHW [2008/4/11] : m_iElementIndex, m_iAuxCursorElementCnt, IsExistAuxCursorElementIndex, GetAuxCursorIndexFromElementIndex �߰�
//! Modify by LHW [2008/4/19] : SetResource���� m_penAuxCursor ���� �κ� ����
//! Modify by LHW [2008/8/04] : ���� Ŀ���� Pen �κ��� ����
class CGraph2D_Aux_CursorGroup
{
public:
	CGraph2D_Aux_CursorGroup(void);
	~CGraph2D_Aux_CursorGroup(void);

	int m_iElementIndex;		//! ���� Ŀ�� �׷��� ���� index
	int m_iAuxCursorElementCnt;	//! ���� ���� Ŀ�� index�� ���ϱ� ���� ����

	CArray<strGraph2D_Aux_Cursor_Extend, strGraph2D_Aux_Cursor_Extend&> m_arrCursorInfo;	//! �׷쿡 ���� ���� Ŀ���� ���� ����

	strGraph2D_Aux_CursorGroup	m_strGroupInfo;	//! ���� Ŀ���� ����, �� �β� ���� ����

	CPen						m_penAuxCursorSolid;
	CPen						m_penAuxCursorDot;
	CPen						m_penAuxCursorDash;
	//CPen						m_penAuxCursor;
	CFont						m_ftAuxCursorText;

	void SetResource();
	void ReleaseResource();
	int GetCursorInfoSize();					//! m_arrCursorInfo�� ������ ��ȯ

	int GetAuxCursorPossibleElementIndex();					//! ��� ������ ���� index ��ȯ, ������ ������ -1�� ��ȯ
	bool IsExistAuxCursorElementIndex(int iElementIndex);	//! �Էµ� iElementIndex�� ���� strGraph2D_Aux_Cursor_Extend�� ������ true�� ��ȯ	
	bool GetAuxCursorIndexFromElementIndex(int iElementIndex, int &iIndex);	//! �Էµ� ���� index, iElementIndex�� ���� ���� index�� ��ȯ
	bool GetAuxCursorElementIndexFromIndex(int iIndex, int &iElementIndex);	//! ���� index�� ���� ���� index, iElementIndex�� ��ȯ
};

//! �ڽ� Ŀ���� Ŭ����, Added by LHW [2008/6/12]
//! Modify by LHW [2008/6/13] : m_dbDeltaX, m_dbDeltaY, m_iDataIndex �߰�
//! Modify by LHW [2008/6/14] : m_bDataValid �߰�
//! Modify by LHW [2008/6/15] : m_iCursorDataIndex, m_bCursorMoved �߰�
//! Modify by LHW [2008/6/16] : m_penSelectCursorLine �߰�
class CGraph2D_Child_Cursor
{
public:
	CGraph2D_Child_Cursor(void);
	~CGraph2D_Child_Cursor(void);

	int m_iElementIndex;	//! �ڽ� Ŀ�� �׷��� ���� index
	int m_iDataIndex;		//! �ڽ� Ŀ���� ������ index
	int m_iCursorDataIndex;	//! �θ� Ŀ���� ������ index

	bool m_bCursorMoved;	//! �θ� Ŀ���� �����̸� true
	bool m_bDataValid;		//! �������� ��ȿ�� ����

	double m_dbXData;		//! �ڽ� Ŀ���� X ������
	double m_dbYData;		//! �ڽ� Ŀ���� Y ������

	double m_dbDeltaX;		//! �ڽ� Ŀ���� Ŀ���� X ������ ���̰�
	double m_dbDeltaY;		//! �ڽ� Ŀ���� Ŀ���� Y ������ ���̰�

	int m_iXPos;			//! �ڽ� Ŀ���� X ��ġ
	int m_iYPos;			//! �ڽ� Ŀ���� Y ��ġ

	CRect m_crSelectX;		//! �ڽ� Ŀ�� ���� ����
	CRect m_crSelectY;		//! �ڽ� Ŀ�� ���� ����
	CRect m_crInfo;			//! �ڽ� Ŀ���� ����â ��� ����
	CRect m_crData;			//! m_crInfo�� Y ��ǥ�� ������ ���� m_DataRect �� ����, �μ� ��ɶ����� �ʿ�

	strGraph2D_Child_Cursor m_strCursorInfo;	//! �ڽ� Ŀ���� ����, �� �β� ���� ����

	CBrush		m_brCursorPoint;
	CBrush		m_brCursorTextBack;

	CPen		m_penCursorLine;
	CPen		m_penSelectCursorLine;
	CPen		m_penCursorTextBorder;

	CFont		m_ftCursorText;

	void SetResource();
	void ReleaseResource();
};

#define REGION_CURSOR_SELECT_SIZE	4	//! ���� ǥ�� Ŀ�� ���� ����
#define REGION_CURSOR_LEFT_VERT		0	//! ���� ǥ�� Ŀ�� ������ ���� ���� ��
#define REGION_CURSOR_UP_HORZ		1	//! ���� ǥ�� Ŀ�� ������ ���� ���� ��
#define REGION_CURSOR_DOWN_HORZ		2	//! ���� ǥ�� Ŀ�� ������ �Ʒ��� ���� ��
#define REGION_CURSOR_RIGHT_VERT	3	//! ���� ǥ�� Ŀ�� ������ ������ ���� ��
#define REGION_CURSOR_CENTER		4	//! ���� ǥ�� Ŀ�� ������ �ѷ����� �簢 ����, Added by LHW [2008/12/1]

//! ���� ǥ�� Ŀ��, Added by LHW [2008/11/28]
//! Modify by LHW [2008/11/30] : IsPtInSelect ����
//! Modify by LHW [2008/12/1] : m_crSelectRegion, m_ptMouseSave �߰�
class CGraph2D_Region_Cursor
{
public:
	CGraph2D_Region_Cursor(void);
	~CGraph2D_Region_Cursor(void);

	int m_iElementIndex;	//! ���� ǥ�� Ŀ���� ���� index

	strGraph2D_Region_Cursor m_strCursorInfo;

	CPen		m_penCursorLine;
	CPen		m_penSelectCursorLine;

	CFont		m_ftCursorText;

	double		m_dbMinDataX;	//! Ŀ���� X �ּ� ������
	double		m_dbMaxDataX;	//! Ŀ���� X �ִ� ������
	double		m_dbMinDataY;	//! Ŀ���� Y �ּ� ������
	double		m_dbMaxDataY;	//! Ŀ���� Y �ִ� ������

	int			m_iMinPosX;		//! Ŀ���� X �ּ� ��ġ
	int			m_iMaxPosX;		//! Ŀ���� X �ִ� ��ġ
	int			m_iMinPosY;		//! Ŀ���� Y �ּ� ��ġ
	int			m_iMaxPosY;		//! Ŀ���� Y �ִ� ��ġ

	CRect		m_crSelectRegion;	//! Ŀ�� ������ �ѷ����� ����
	CPoint		m_ptMouseSave;		//! ���콺 �̺�Ʈ�� �߻��� ��ǥ ����, m_crSelectRegion�����Ͽ� �ʿ�
	
	CRect		m_crSelectLine[REGION_CURSOR_SELECT_SIZE];		//! Ŀ�� �� ���� ����
	bool IsPtInSelect(CPoint ptInput, int &iCursorLineIndex);	//! Ŀ�� �� ���� �����ȿ� ������ true�� ��ȯ
																//! true�� ��ȯ�� ��� ���õ� Ŀ�� ���� index�� ��ȯ
	
	void SetResource();
	void ReleaseResource();
};

//! �Լ������� �׷����� �׷����� Ŭ����, Added by LHW [2008/8/22]
//! Modify by LHW [2008/8/25] : Y�� �ּ�, �ִ밪 �߰�
class CGraph2D_Equation
{
public:
	CGraph2D_Equation(void);
	~CGraph2D_Equation(void);

	int m_iElementIndex;	//! �Լ������� �׷����� �׷����� ���� index

	strGraph2DEquationInfo m_strEquationInfo;

	double m_dbDataMinY;	//! Y�� �ּҰ�
	double m_dbDataMaxY;	//! Y�� �ִ밪

	CPen m_penLine;

	void SetResource();
	void ReleaseResource();
	void ReleaseStructureBuf();
};

//! Modify by LHW [2008/4/4] : iSearchMargin �߰�
//! Modify by LHW [2008/4/11] : iElementIndex �߰�
//! Modify by LHW [2008/4/18] : bShowPoint �߰�
//! Modify by LHW [2008/4/19] : bShowGraphChk �߰�
//! Modify by LHW [2008/6/12] : iAuxCursorIndex -> iAuxCursorGroupElementIndex
//! Modify by LHW [2008/7/01] : szXData, szYData �߰�
//! Modify by LHW [2008/8/01] : bShowStringX, bShowStringY �߰�
//! Modify by LHW [2009/6/4] : ���콺�� ������ ���� Ŀ�� ������ ������ ���̱� ���� iSearchMargin�� �ʱⰪ�� 100���� ����
struct strGraph2DCursorInfo
{
	int iElementIndex;		//! Ŀ���� ���� index

	int iDataIndex;			//! Ŀ���� ��ġ�ϴ� �������� Index
	int iSelectOverlapIndex;//! Ŀ���� ��ġ�ϴ� �׷����� index

	double dbXData;		//! Ŀ���� X ������
	double dbYData;		//! Ŀ���� Y ������

	CString szXData;	//! Ŀ���� X ������ ���ڿ�
	CString szYData;	//! Ŀ���� Y ������ ���ڿ�

	int iXPos;			//! Ŀ���� X ��ġ
	int iYPos;			//! Ŀ���� Y ��ġ

	CRect crSelectX;	//! Ŀ�� ���� ����
	CRect crSelectY;	//! Ŀ�� ���� ����
	CRect crInfo;		//! Ŀ���� ����â ��� ����
	CRect crData;		//! crInfo�� Y ��ǥ�� ������ ���� m_DataRect �� ����, �μ� ��ɶ����� �ʿ�

	bool bShowGraphChk;	//! true�̸� �Ʒ� iSelectOverlapIndex�� �׷����� ���� ������ ������ ���� ������ �ʰ�, 
						//! �׷����� ���� ���̸� ���� ����
	bool bShowInfo;		//! Ŀ���� ����â ��� ����
	bool bShowValueX;	//! Ŀ���� X�� ��� ����
	bool bShowValueY;	//! Ŀ���� Y�� ��� ����
	bool bShowPoint;	//! Ŀ���� ǥ�� �簢�� ��� ����
	bool bShowStringX;	//! Ŀ������ 'X : ' ���ڿ� ǥ��
	bool bShowStringY;	//! Ŀ������ 'Y : ' ���ڿ� ǥ��

	E_GRAPH2D_CURSOR_LINE_MODE eLineMode;	//! Ŀ�� �� ǥ�� ���
	bool bMaxHold;							//! true�̸� �ݵ�� ���� ���� ���̴� �׷����� �ִ밪�� Ŀ���� ��ġ��Ŵ
	int iSearchMargin;						//! Ŀ���� ã�� ����� X�� �������� ������ ������ ���Ͽ� �� �ȿ��� Ŀ������ ã�� ��� ������ ���� ��
											//! �� ���� Ŭ���� �� ���� �����͸� �˻�

	int iAuxCursorGroupElementIndex;		//! �����ϴ� ���� Ŀ�� �׷��� ���� index

	strGraph2DCursorInfo()
	{
		InitInfo();
	}

	void InitInfo()
	{
		iElementIndex = -1;
		iDataIndex = -1;
		iSelectOverlapIndex = -1;
		dbXData = 0.;
		dbYData = 0.;
		szXData = _T("");
		szYData = _T("");
		iXPos = -1;
		iYPos = -1;		
		crSelectX.SetRectEmpty();
		crSelectY.SetRectEmpty();
		crInfo.SetRectEmpty();
		crData.SetRectEmpty();
		bShowGraphChk = true;
		bShowInfo = true;
		bShowValueX = true;
		bShowValueY = true;
		bShowPoint = true;
		bShowStringX = false;
		bShowStringY = false;
		eLineMode = E_GRAPH2D_CURSOR_LINE_XY;
		bMaxHold = false;
		iSearchMargin = 100;
		iAuxCursorGroupElementIndex = -1;	//! -1�� ���� Ŀ���� ���ٴ� �ǹ̷� ���ǹǷ� �ʱⰪ�� �ٲٸ� �ȵ�
	}

	strGraph2DCursorInfo& operator=(const strGraph2DCursorInfo &info)  //! = ������ ������
	{
		iElementIndex = info.iElementIndex;
		iDataIndex = info.iDataIndex;
		iSelectOverlapIndex = info.iSelectOverlapIndex;
		dbXData = info.dbXData;
		dbYData = info.dbYData;
		szXData = info.szXData;
		szYData = info.szYData;
		iXPos = info.iXPos;
		iYPos = info.iYPos;		
		crSelectX = info.crSelectX;
		crSelectY = info.crSelectY;
		crInfo = info.crInfo;
		crData = info.crData;
		bShowGraphChk = info.bShowGraphChk;
		bShowInfo = info.bShowInfo;
		bShowValueX = info.bShowValueX;
		bShowValueY = info.bShowValueY;
		bShowPoint = info.bShowPoint;
		bShowStringX = info.bShowStringX;
		bShowStringY = info.bShowStringY;
		eLineMode = info.eLineMode;
		bMaxHold = info.bMaxHold;
		iSearchMargin = info.iSearchMargin;
		iAuxCursorGroupElementIndex = info.iAuxCursorGroupElementIndex;
		
		return *this;
	}
};
