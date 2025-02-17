//! 2D ������ǥ �׷��� ��Ʈ���� �ܺο��� �����ϴ� �� �ʿ��� ������, ����ü, �޽��� ����
//! Maked by LHW (2008/3/14 ~ )
//! ���� �۾��� [2013/3/26]

//! Modify by LHW [2008/6/12] : operator ������ ������ �κ��� ���� �ϰ� ����

#pragma once

#define WM_GRAPH2D_DRAW_COMPLETE	WM_USER + 1232	//! ����� ȭ�鿡 �׷��� �׸��� �۾��� �Ϸ��ϸ� �޽��� �߻�
													//! WPARAM : NULL, LPARAM : NULL

#define WM_GRAPH2D_AXIS_Y_EVENT		WM_USER + 1238	//! Y�� �κп��� ���� Ŭ���� �ϸ� �߻��ϴ� �޽���, Added by LHW [2008/7/31]
													//! WPARAM : NULL, LPARAM : NULL

#define WM_GRAPH2D_PAN_XY_EVENT		WM_USER + 1242	//! XY �� ������ Pan ����� �����ϰ� ���콺�� ���� �� �߻��ϴ� �޽���, Added by LHW [2009/3/1]
													//! WPARAM : NULL, LPARAM : NULL

#define WM_GRAPH2D_PAN_X_EVENT		WM_USER + 1243	//! X �� ������ Pan ����� �����ϰ� ���콺�� ���� �� �߻��ϴ� �޽���, Added by LHW [2009/3/1]
													//! WPARAM : NULL, LPARAM : NULL

#define WM_GRAPH2D_PAN_Y_EVENT		WM_USER + 1244	//! Y �� ������ Pan ����� �����ϰ� ���콺�� ���� �� �߻��ϴ� �޽���, Added by LHW [2009/3/1]
													//! WPARAM : NULL, LPARAM : NULL

#define WM_GRAPH2D_LMOUSE_DOWN		WM_USER + 1233	//! ���콺 ���� Ŭ���� �ϸ� �޽��� �߻�
													//! WPARAM : NULL
													//! LPARAM : ����ü strGraph2D_MouseDown_Info, ���콺 Ŭ����ġ ����
#define WM_GRAPH2D_RMOUSE_DOWN		WM_USER + 1234	//! ���콺 ������ Ŭ���� �ϸ� �޽��� �߻�
													//! WPARAM : NULL
													//! LPARAM : ����ü strGraph2D_MouseDown_Info, ���콺 Ŭ����ġ ����

//! Modify by LHW [2008/4/19] : X�� Ư���� ǥ�ü� ���� �߰�
//! Modify by LHW [2008/6/13] : �ڽ� Ŀ�� ���� �߰�
//! Modify by LHW [2008/8/5] : bAuxCursorEvent �߰�
//! Modify by LHW [2008/11/28] : bRegionCursorEvent �߰�
struct strGraph2D_MouseDown_Info	//! �޽����� ���޵Ǵ� ����ü
{
	int iPlotIndex;		//! ������ Plot index
	int iMouseDownX;	//! ���콺 Ŭ����ġ X ��ǥ
	int iMouseDownY;	//! ���콺 Ŭ����ġ Y ��ǥ

	bool bXValueLineEvent;	//! X�� Ư���� ǥ�ü����� ���콺 �̺�Ʈ�� �߻��ϸ� true
	int iXValueLineIndex;	//! X�� Ư���� ǥ�ü��� index

	bool bChildCursorEvent;	//! �ڽ� Ŀ������ ���콺 �̺�Ʈ�� �߻��ϸ� true
	bool bAuxCursorEvent;	//! ���� Ŀ������ ���콺 �̺�Ʈ�� �߻��ϸ� true
	bool bRegionCursorEvent;//! ���� ǥ�� Ŀ������ ���콺 �̺�Ʈ�� �߻��ϸ� true

	void InitInfo()
	{
		iPlotIndex = -1;
		iMouseDownX = 0;
		iMouseDownY = 0;

		bXValueLineEvent = false;
		iXValueLineIndex = -1;

		bChildCursorEvent = false;
		bAuxCursorEvent = false;
		bRegionCursorEvent = false;
	}

	strGraph2D_MouseDown_Info& operator=(const strGraph2D_MouseDown_Info &input)
	{
		iPlotIndex = input.iPlotIndex;
		iMouseDownX = input.iMouseDownX;
		iMouseDownY = input.iMouseDownY;
		bXValueLineEvent = input.bXValueLineEvent;
		iXValueLineIndex = input.iXValueLineIndex;
		bChildCursorEvent = input.bChildCursorEvent;
		bAuxCursorEvent = input.bAuxCursorEvent;
		bRegionCursorEvent = input.bRegionCursorEvent;

		return *this;
	}
};

//! Added by LHW [2008/6/16] : ���콺, Ű����� ������ �� �ִ� Ŀ�� ����
typedef enum _E_GRAPH2D_MOVE_CURSOR_TYPE
{
	E_GRAPH2D_MOVE_MAIN_CURSOR_TYPE = 0, 
	E_GRAPH2D_MOVE_CHILD_CURSOR_TYPE = 1, 
}E_GRAPH2D_MOVE_CURSOR_TYPE;

#define WM_GRAPH2D_CURSOR_EVENT WM_USER + 1235	//! Ŀ�� ���� �̺�Ʈ�� �߻��� ������ �޽��� �߻�
												//! WPARAM : NULL
												//! LPARAM : ����ü strGraph2D_Cursor_Event

//! Modify by LHW [2008/8/20] : E_GRAPH2D_CURSOR_SEARCH_X_VISUAL �߰�
typedef enum _E_GRAPH2D_CURSOR_SEARCH_MODE	//! Ŀ���� ã�� ���
{
	E_GRAPH2D_CURSOR_SEARCH_X_RANGE = 0,	//! X�� �������� ������ ������ ���Ͽ� �� �ȿ��� Ŀ������ ã��. X�� ���, X-Y �����Ͱ� 1:1 Ȥ�� 1:0 ���� �׷����� ����
	E_GRAPH2D_CURSOR_SEARCH_X_VISUAL = 1,	//! X�� �������� ���� ���̴� �������� Ŀ������ ã��. X�� ��� �ƴ�, X-Y �����Ͱ� 1:1 Ȥ�� 1:0 ���� �׷����� ����
	E_GRAPH2D_CURSOR_SEARCH_ALL = 2,		//! ��� �����͸� �˻��Ͽ� Ŀ������ ã��. X�� ��� �ƴ�, X-Y �����Ͱ� 1:�� ���� �׷����� ����
}E_GRAPH2D_CURSOR_SEARCH_MODE;

typedef enum _E_GRAPH2D_CURSOR_ALL_SEARCH_MODE	//! Overlap�� �׷������� Ŀ���� ã�� ���, Added by LHW [2008/7/24]
{
	E_GRAPH2D_CURSOR_ALL_SEARCH_SELECT = 0,		//! ���콺�� Ŀ�� �߽����� Ŭ���� ���� ���� ��� �׷������� Ŀ������ ã��
	E_GRAPH2D_CURSOR_ALL_SEARCH_NONE = 1,		//! ���� �׷��������� Ŀ������ ã��
	E_GRAPH2D_CURSOR_ALL_SEARCH_ALWAYS = 2,		//! ������ ��� �׷������� Ŀ������ ã��
}E_GRAPH2D_CURSOR_ALL_SEARCH_MODE;

typedef enum _E_GRAPH2D_CURSOR_OPERATION_MODE	//! Ŀ�� ���� ����
{
	E_GRAPH2D_CURSOR_ADD = 0,				//! Ŀ�� �߰�
	E_GRAPH2D_CURSOR_MOVE = 1,				//! Ŀ�� �̵�
	E_GRAPH2D_CURSOR_SHOW_INFO = 2,			//! Ŀ�� ����â �����ֱ�/�����
	E_GRAPH2D_CURSOR_LINE = 3,				//! Ŀ�� �� ǥ�� ��� ����
	E_GRAPH2D_CURSOR_MAX_HOLD = 4,			//! Ŀ���� Max-Hold ���� ����
	E_GRAPH2D_CURSOR_SERACH_MARGIN = 5,		//! Ŀ���� X�� ���� �˻� �� ���� �� ����, Modify by LHW [2008/4/4]
	E_GRAPH2D_CURSOR_VALUE_VISIBLE = 6,		//! Ŀ���� X,Y �� ǥ�� ���� ���� ����, Modify by LHW [2008/4/4]
	E_GRAPH2D_CURSOR_POINT_VISIBLE = 7,		//! Ŀ���� ��Ȯ�� ��ġ�� �簢�������� ǥ���ϴ����� ���� ���� ����, Modify by LHW [2008/4/18]
	E_GRAPH2D_CURSOR_XY_STRING_VISIBLE = 8, //! Ŀ���� 'X : ', 'Y : ' ���ڿ� ǥ�� ���� ����, Modify by LHW [2008/8/01]
	E_GRAPH2D_CURSOR_DELETE = 9,			//! Ŀ�� ����
	E_GRAPH2D_CURSOR_DELETE_ALL = 10,		//! Ŀ�� ��� ����
}E_GRAPH2D_CURSOR_OPERATION_MODE;

typedef enum _E_GRAPH2D_CURSOR_LINE_MODE	//! Ŀ�� �� ǥ�� ���
{
	E_GRAPH2D_CURSOR_LINE_XY = 0,			//! Ŀ�� ���� X-Y�������� ��� ǥ��
	E_GRAPH2D_CUROSR_LINE_X = 1,			//! Ŀ�� ���� X�������θ� ��� ǥ��
}E_GRAPH2D_CURSOR_LINE_MODE;

struct strGraph2D_Cursor_Event	//! Ŀ���� ���� ���� ���� ����ü
{
	int iPlotIndex;			//! ������ Plot index
	bool bMouseOperation;	//! Mouse �������� �߻��� Ŀ�� �̺�Ʈ�̸� true�� ����

	E_GRAPH2D_CURSOR_OPERATION_MODE eOperationMode;	//! Ŀ�� ���� ����
	E_GRAPH2D_CURSOR_LINE_MODE		eLineMode;		//! Ŀ�� �� ǥ�� ���

	int iCursorIndex;		//! Ŀ���� index, 0 based, eOperationMode�� E_GRAPH2D_CURSOR_DELETE_ALL�� ���� -1
	int iOverlapIndex;		//! Ŀ���� ����Ű�� Overlap�Ǵ� �׷����� index, 0 based
	int iDataIndex;			//! Ŀ���� ������ index, 0 based

	double dbXData;			//! Ŀ���� X��
	double dbYData;			//! Ŀ���� Y��
	int iXPos;				//! Ŀ���� X ��ġ
	int iYPos;				//! Ŀ���� Y ��ġ

	bool bMaxHold;			//! true�̸� Ŀ���� �ݵ�� �ִ밪�� ��ġ
	int iSearchMargin;		//! Ŀ���� X�� ���� �˻� �� ���� ��, Modify by LHW [2008/4/4]

	strGraph2D_Cursor_Event()
	{
		InitInfo();
	}

	void InitInfo()
	{
		iPlotIndex = -1;
		bMouseOperation = true;
		eOperationMode = E_GRAPH2D_CURSOR_MOVE;
		eLineMode = E_GRAPH2D_CURSOR_LINE_XY;
		iCursorIndex = -1;
		iOverlapIndex = -1;
		iDataIndex = -1;
		dbXData = 0.;
		dbYData = 0.;
		iXPos = 0;
		iYPos = 0;
		bMaxHold = false;
		iSearchMargin = 0;
	}

	strGraph2D_Cursor_Event& operator=(const strGraph2D_Cursor_Event &input)
	{
		iPlotIndex = input.iPlotIndex;
		bMouseOperation = input.bMouseOperation;
		eOperationMode = input.eOperationMode;
		eLineMode = input.eLineMode;
		iCursorIndex = input.iCursorIndex;
		iOverlapIndex = input.iOverlapIndex;
		iDataIndex = input.iDataIndex;
		dbXData = input.dbXData;
		dbYData = input.dbYData;
		iXPos = input.iXPos;
		iYPos = input.iYPos;
		bMaxHold = input.bMaxHold;
		iSearchMargin = input.iSearchMargin;

		return *this;
	}
};

#define WM_GRAPH2D_REGION_CURSOR_EVENT	WM_USER + 1241	//! ���� ǥ�� Ŀ�� ���� �̺�Ʈ�� �߻��� ������ �޽��� �߻�, Added by LHW [2008/11/28]
														//! WPARAM : NULL
														//! LPARAM : ����ü strGraph2D_Region_Cursor_Event

struct strGraph2D_Region_Cursor_Event	//! ���� ǥ�� Ŀ���� ���� ���� ���� ����ü, Added by LHW [2008/11/28]
{
	int iPlotIndex;		//! ������ Plot index
	int iCursorIndex;	//! Ŀ���� index

	double dbMinDataX;	//! Ŀ���� X �ּҰ�
	double dbMaxDataX;	//! Ŀ���� X �ִ밪
	double dbMinDataY;	//! Ŀ���� Y �ּҰ�
	double dbMaxDataY;	//! Ŀ���� Y �ִ밪

	int iMinPosX;		//! Ŀ���� X �ּ� ��ġ
	int iMaxPosX;		//! Ŀ���� X �ִ� ��ġ	
	int iMinPosY;		//! Ŀ���� Y �ּ� ��ġ
	int iMaxPosY;		//! Ŀ���� Y �ִ� ��ġ

	strGraph2D_Region_Cursor_Event()
	{
		InitInfo();
	}

	void InitInfo()
	{
		iPlotIndex = -1;
		iCursorIndex = -1;
		dbMinDataX = 0.;
		dbMaxDataX = 0.;
		dbMinDataY = 0.;
		dbMaxDataY = 0.;
		iMinPosX = 0;
		iMaxPosX = 0;
		iMinPosY = 0;
		iMaxPosY = 0;
	}

	strGraph2D_Region_Cursor_Event& operator=(const strGraph2D_Region_Cursor_Event &input)
	{
		iPlotIndex = input.iPlotIndex;
		iCursorIndex = input.iCursorIndex;
		dbMinDataX = input.dbMinDataX;
		dbMaxDataX = input.dbMaxDataX;
		dbMinDataY = input.dbMinDataY;
		dbMaxDataY = input.dbMaxDataY;
		iMinPosX = input.iMinPosX;
		iMaxPosX = input.iMaxPosX;
		iMinPosY = input.iMinPosY;
		iMaxPosY = input.iMaxPosY;

		return *this;
	}
};

//! Added by LHW [2008/11/28] : ���� ǥ�� Ŀ�� �߰�, �׷����ʹ� ���������� �����ϴ� Ŀ��
//! Modify by LHW [2008/11/30] : iOverlapIndex, bShowGraphChk �߰�
struct strGraph2D_Region_Cursor
{
	bool bShow;			//! true�̸� Ŀ���� ���̰� ��
	bool bShowText;		//! true�̸� Ŀ�� ǥ�� ���ڿ��� ���̰� ��
	bool bShowGraphChk;	//! true�̸� �Ʒ� iOverlapIndex�� �׷����� ���� ������ ������ ���� ������ �ʰ�, �׷����� ���� ���̸� ���� ����

	int iOverlapIndex;			//! ���� ǥ�� Ŀ���� ����Ű�� Overlap�Ǵ� �׷����� index, 0 based

	bool bCursorFontHeight;		//! true�̸� Ŀ�� ǥ�� ���ڿ� ���� ũ�⸦ �ڵ� ����
	int	iCursorFontHeight;		//! Ŀ�� ǥ�� ���ڿ� ���� ũ��, ���������� ����
	CString	szCursorFontName;	//! Ŀ�� ǥ�� ���ڿ��� ��Ʈ ����

	COLORREF clrCursorLine;			//! Ŀ�� ���� ����
	COLORREF clrSelectCursorLine;	//! ���õ� Ŀ�� ���� ����
	COLORREF clrCursorText;			//! Ŀ�� ǥ�� ���ڿ��� ����

	CString szCursorText;		//! Ŀ�� ǥ�� ���ڿ�

	int iCursorLineThickness;	//! Ŀ�� ���� �β�

	strGraph2D_Region_Cursor()
	{
		InitInfo();
	}

	void InitInfo()
	{
		bShow = true;
		bShowText = true;
		bShowGraphChk = true;
		iOverlapIndex = -1;
		bCursorFontHeight = true;
		iCursorFontHeight = -10;
		szCursorFontName = _T("MS Sans Serif");
		clrCursorLine = RGB(255, 128, 0);
		clrSelectCursorLine = RGB(255, 0, 0);
		clrCursorText = RGB(0, 0, 0);
		szCursorText = _T("");
		iCursorLineThickness = 1;
	}

	strGraph2D_Region_Cursor& operator=(const strGraph2D_Region_Cursor &input)
	{
		bShow = input.bShow;
		bShowText = input.bShowText;
		bShowGraphChk = input.bShowGraphChk;
		iOverlapIndex = input.iOverlapIndex;
		bCursorFontHeight = input.bCursorFontHeight;
		iCursorFontHeight = input.iCursorFontHeight;
		szCursorFontName = input.szCursorFontName;
		clrCursorLine = input.clrCursorLine;
		clrSelectCursorLine = input.clrSelectCursorLine;
		clrCursorText = input.clrCursorText;
		szCursorText = input.szCursorText;
		iCursorLineThickness = input.iCursorLineThickness;
		
		return *this;
	}
};

//! Added by LHW [2008/6/12] : �ڽ� Ŀ���� ã�� ���
//! Modify by LHW [2008/8/20] : E_GRAPH2D_CHILD_CURSOR_SEARCH_X_VISUAL �߰�
typedef enum _E_GRAPH2D_CHILD_CURSOR_SEARCH_MODE
{
	E_GRAPH2D_CHILD_CURSOR_SEARCH_X_RANGE = 0,	//! X�� �������� ������ ������ ���Ͽ� �� �ȿ��� �ڽ� Ŀ������ ã��. X�� ���, X-Y �����Ͱ� 1:1 Ȥ�� 1:0 ���� �׷����� ����
	E_GRAPH2D_CHILD_CURSOR_SEARCH_X_VISUAL = 1,	//! X�� �������� ���� ���̴� �������� �ڽ� Ŀ������ ã��. X�� ��� �ƴ�, X-Y �����Ͱ� 1:1 Ȥ�� 1:0 ���� �׷����� ����
	E_GRAPH2D_CHILD_CURSOR_SEARCH_ALL = 2,		//! ��� �����͸� �˻��Ͽ� �ڽ� Ŀ������ ã��. X�� ��� �ƴ�, X-Y �����Ͱ� 1:�� ���� �׷����� ����
}E_GRAPH2D_CHILD_CURSOR_SEARCH_MODE;

typedef enum _E_GRAPH2D_CHILD_CURSOR_LINE_MODE	//! �ڽ� Ŀ�� �� ǥ�� ���, Added by LHW [2008/6/12]
{
	E_GRAPH2D_CHILD_CURSOR_LINE_XY = 0,			//! �ڽ� Ŀ�� ���� X-Y�������� ��� ǥ��
	E_GRAPH2D_CHILD_CUROSR_LINE_X = 1,			//! �ڽ� Ŀ�� ���� X�������θ� ��� ǥ��
}E_GRAPH2D_CHILD_CURSOR_LINE_MODE;

//! Added by LHW [2008/6/12] : Ŀ���� ���ӵǾ� �����ϴ� �ڽ� Ŀ�� �߰�, ���� Ŀ���ʹ� �޸� ���콺 �̺�Ʈ�� �޾� ���� ����
//! Modify by LHW [2008/6/14] : bCursorOverlapAllSearch �߰�
//! Modify by LHW [2008/6/15] : bFollowCursor �߰�
//! Modify by LHW [2008/6/16] : clrSelectCursorLine �߰�
//! Modify by LHW [2008/8/11] : bShowStringX, bShowStringY �߰�
//! Modify by LHW [2009/6/4] : ���콺�� ������ ���� Ŀ�� ������ ������ ���̱� ���� iSearchMargin�� �ʱⰪ�� 100���� ����
struct strGraph2D_Child_Cursor
{
	E_GRAPH2D_CHILD_CURSOR_SEARCH_MODE	eSearchMode;	//! �ڽ� Ŀ���� ã�� ��� ����
	E_GRAPH2D_CHILD_CURSOR_LINE_MODE	eLineMode;		//! �ڽ� Ŀ�� �� ǥ�� ���

	int iOverlapIndex;			//! �ڽ� Ŀ���� ����Ű�� Overlap�Ǵ� �׷����� index, 0 based
	int iCursorElementIndex;	//! �θ� Ŀ���� ���� index, �ڽ� Ŀ�� index�� �ƴ�, 0 based

	bool bShow;						//! true�̸� �ڽ� Ŀ���� ���̰� ��
	bool bCursorOverlapAllSearch;	//! true�̸� �ڽ� Ŀ���� ���� ���ϱ� ���� ������ Overlap�� ��� �׷������� �۾� ����
	bool bFollowCursor;				//! true�̸� �θ� Ŀ���� ������ ��� ���� ������

	int iSearchMargin;			//! �ڽ� Ŀ���� ã�� ����� X�� �������� ������ ������ ���Ͽ� �� �ȿ��� �ڽ� Ŀ������ ã�� ��� ������ ���� ��
								//! �� ���� Ŭ���� �� ���� �����͸� �˻�
	
	int	iCursorLineThickness;		//! �ڽ� Ŀ�� ���� �β�
	int	iCursorTextBorderThickness;	//! �ڽ� Ŀ�� ����â�� �׵θ� ���� �β�

	bool bCursorFontHeight;		//! true�̸� �ڽ� Ŀ�� ǥ�� ���� ���� ũ�⸦ �ڵ� ����
	int	iCursorFontHeight;		//! �ڽ� Ŀ�� ǥ�� ���� ���� ũ��, ���������� ����
	CString	szCursorFontName;	//! �ڽ� Ŀ�� ǥ�� ������ ��Ʈ ����

	COLORREF clrCursorLine;			//! �ڽ� Ŀ�� ���� ����
	COLORREF clrSelectCursorLine;	//! ���õ� �ڽ� Ŀ�� ���� ����
	COLORREF clrCursorPoint;		//! �ڽ� Ŀ�� ���� ����
	COLORREF clrCursorText;			//! �ڽ� Ŀ���� ������ ����
	COLORREF clrCursorTextBorder;	//! �ڽ� Ŀ�� ����â�� �׵θ� ���� ����
	COLORREF clrCursorTextBack;		//! �ڽ� Ŀ�� ����â�� ������

	bool bShowGraphChk;				//! true�̸� �Ʒ� iOverlapIndex�� �׷����� ���� ������ ������ ���� ������ �ʰ�, �׷����� ���� ���̸� ���� ����
	bool bShowInfo;					//! �ڽ� Ŀ���� ����â ��� ����
	bool bShowValueX;				//! �ڽ� Ŀ���� X�� ��� ����
	bool bShowValueY;				//! �ڽ� Ŀ���� Y�� ��� ����
	bool bShowValueDeltaX;			//! �ڽ� Ŀ���� �θ� Ŀ�� ������ X ���̰� ��� ����
	bool bShowValueDeltaY;			//! �ڽ� Ŀ���� �θ� Ŀ�� ������ Y ���̰� ��� ����
	bool bShowPoint;				//! �ڽ� Ŀ���� ǥ�� �簢�� ��� ����
	bool bShowStringX;				//! Ŀ������ 'X : ' ���ڿ� ǥ��
	bool bShowStringY;				//! Ŀ������ 'Y : ' ���ڿ� ǥ��

	strGraph2D_Child_Cursor()
	{
		InitInfo();
	}

	void InitInfo()
	{
		eSearchMode = E_GRAPH2D_CHILD_CURSOR_SEARCH_X_RANGE;
		eLineMode = E_GRAPH2D_CHILD_CURSOR_LINE_XY;
		iOverlapIndex = -1;
		iCursorElementIndex = -1;
		bShow = true;
		bCursorOverlapAllSearch = false;
		bFollowCursor = true;
		iSearchMargin = 100;
		iCursorLineThickness = 1;
		iCursorTextBorderThickness = 1;
		bCursorFontHeight = true;
		iCursorFontHeight = -10;
		szCursorFontName = _T("MS Sans Serif");
		clrCursorLine = RGB(0, 255, 0);
		clrSelectCursorLine = RGB(0, 128, 0);
		clrCursorPoint = RGB(255, 0, 0);
		clrCursorText = RGB(0, 0, 0);
		clrCursorTextBorder = RGB(0, 0, 0);
		clrCursorTextBack = RGB(255, 255, 128);
		bShowGraphChk = true;
		bShowInfo = true;
		bShowValueX = true;
		bShowValueY = true;
		bShowValueDeltaX = true;
		bShowValueDeltaY = true;
		bShowPoint = true;
		bShowStringX = false;
		bShowStringY = false;
	}

	strGraph2D_Child_Cursor& operator=(const strGraph2D_Child_Cursor &input)
	{
		eSearchMode = input.eSearchMode;
		eLineMode = input.eLineMode;
		iOverlapIndex = input.iOverlapIndex;
		iCursorElementIndex = input.iCursorElementIndex;
		bShow = input.bShow;
		bCursorOverlapAllSearch = input.bCursorOverlapAllSearch;
		bFollowCursor = input.bFollowCursor;
		iSearchMargin = input.iSearchMargin;
		iCursorLineThickness = input.iCursorLineThickness;
		iCursorTextBorderThickness = input.iCursorTextBorderThickness;
		bCursorFontHeight = input.bCursorFontHeight;
		iCursorFontHeight = input.iCursorFontHeight;
		szCursorFontName = input.szCursorFontName;
		clrCursorLine = input.clrCursorLine;
		clrSelectCursorLine = input.clrSelectCursorLine;
		clrCursorPoint = input.clrCursorPoint;
		clrCursorText = input.clrCursorText;
		clrCursorTextBorder = input.clrCursorTextBorder;
		clrCursorTextBack = input.clrCursorTextBack;
		bShowGraphChk = input.bShowGraphChk;
		bShowInfo = input.bShowInfo;
		bShowValueX = input.bShowValueX;
		bShowValueY = input.bShowValueY;
		bShowValueDeltaX = input.bShowValueDeltaX;
		bShowValueDeltaY = input.bShowValueDeltaY;
		bShowPoint = input.bShowPoint;
		bShowStringX = input.bShowStringX;
		bShowStringY = input.bShowStringY;

		return *this;
	}
};

//! Added by LHW [2008/4/19]
#define WM_GRAPH2D_ZOOM_EVENT WM_USER + 1236	//! Ŀ�� ���� �̺�Ʈ�� �߻��� ������ �޽��� �߻�
												//! WPARAM : NULL
												//! LPARAM : ����ü strGraph2D_Zoom_Event

//! Added by LHW [2008/4/19]
typedef enum _E_GRAPH2D_ZOOM_OPERATION_MODE	//! Zoom ���� ����
{
	E_GRAPH2D_ZOOM_IN = 0,		//! Ȯ��
	E_GRAPH2D_ZOOM_OUT = 1,		//! ���� ���·� ���ư���
	E_GRAPH2D_ZOOM_FIT = 2,		//! �������
}E_GRAPH2D_ZOOM_OPERATION_MODE;

//! Added by LHW [2008/4/19]
struct strGraph2D_Zoom_Event	//! Ȯ��/��� ���� ���� ���� ����ü
{
	int iPlotIndex;				//! ������ Plot index
	E_GRAPH2D_ZOOM_OPERATION_MODE eZoomMode;	//! Zoom ���� ����

	strGraph2D_Zoom_Event()
	{
		InitInfo();
	}

	void InitInfo()
	{
		iPlotIndex = -1;
		eZoomMode = E_GRAPH2D_ZOOM_IN;
	}

	strGraph2D_Zoom_Event& operator=(const strGraph2D_Zoom_Event &input)
	{
		iPlotIndex = input.iPlotIndex;
		eZoomMode = input.eZoomMode;

		return *this;
	}
};

//! Added by LHW [2008/5/23]
#define WM_GRAPH2D_DATA_POINT_EVENT	WM_USER + 1237 //! �׷����� ������ ������ �̺�Ʈ�� �߻����� ���� �޽���
												   //! WPARAM : NULL
  												   //! LPARAM : ����ü strGraph2D_DataPoint_Event

//! Added by LHW [2008/5/23]
struct strGraph2D_DataPoint_Event	//! �׷����� ������ ������ �������� ���� ����ü
{
	int iOverlapIndex;	//! �׷��� index, 0 based
	int iDataIndex;		//! ������ index, 0 based
	double dbXData;		//! X��
	double dbYData;		//! Y��
	int iCustomData;	//! ����ڰ� �߰� �Է��� ������, �ʱⰪ -1

	strGraph2D_DataPoint_Event()
	{
		InitInfo();
	}

	void InitInfo()
	{
		iOverlapIndex = -1;
		iDataIndex = -1;
		dbXData = 0.;
		dbYData = 0.;
		iCustomData = -1;
	}

	strGraph2D_DataPoint_Event& operator=(const strGraph2D_DataPoint_Event &input)
	{
		iOverlapIndex = input.iOverlapIndex;
		iDataIndex = input.iDataIndex;
		dbXData = input.dbXData;
		dbYData = input.dbYData;
		iCustomData = input.iCustomData;

		return *this;
	}
};

typedef enum _E_GRAPH2D_KEY_TYPE	//! Ű���忡�� ������ Ű�� ����, Added by LHW [2008/4/15]
{
	E_GRAPH2D_KEY_UP_ARROW = 0,		//! ���� ȭ��ǥ
	E_GRAPH2D_KEY_DOWN_ARROW = 1,	//! �Ʒ��� ȭ��ǥ
	E_GRAPH2D_KEY_LEFT_ARROW = 2,	//! ���� ȭ��ǥ
	E_GRAPH2D_KEY_RIGHT_ARROW = 3,	//! ������ ȭ��ǥ
	E_GRAPH2D_KEY_PAGE_UP = 4,		//! 'Page UP' Ű
	E_GRAPH2D_KEY_PAGE_DOWN = 5,	//! 'Page Down' Ű
}E_GRAPH2D_KEY_TYPE;

//! Added by LHW [2008/3/31]
//! Modify by LHW [2008/4/1] : E_GRAPH2D_AUX_CURSOR_FIXED_INDEX �߰�
typedef enum _E_GRAPH2D_AUX_CURSOR_TYPE	//! ���� Ŀ�� ����� ����
{
	E_GRAPH2D_AUX_CURSOR_FIXED = 0,			//! Ư�� X���� ���� ����
	E_GRAPH2D_AUX_CURSOR_VARIABLE = 1,		//! Ŀ���� �����ӿ� ���� Ŀ���� �߽����� ��ȭ�Ǵ� X���� ���� ǥ��
	E_GRAPH2D_AUX_CURSOR_FIXED_INDEX = 2,	//! Ư�� ������ index�� ���� ����
}E_GRAPH2D_AUX_CURSOR_TYPE;

//! Added by LHW [2008/4/1] : ���� Ŀ���� ã�� ���
//! Modify by LHW [2008/8/20] : E_GRAPH2D_AUX_CURSOR_SEARCH_X_VISUAL �߰�
typedef enum _E_GRAPH2D_AUX_CURSOR_SEARCH_MODE
{
	E_GRAPH2D_AUX_CURSOR_SEARCH_X_RANGE = 0,	//! X�� �������� ������ ������ ���Ͽ� �� �ȿ��� ���� Ŀ������ ã��. X�� ���, X-Y �����Ͱ� 1:1 Ȥ�� 1:0 ���� �׷����� ����
	E_GRAPH2D_AUX_CURSOR_SEARCH_X_VISUAL = 1,	//! X�� �������� ���� ���̴� ���������� ���� Ŀ������ ã��
												//! ���� Ŀ���� Ű����, ���콺�� ������ ���. X�� ��� �ƴ�, X-Y �����Ͱ� 1:1 Ȥ�� 1:0 ���� �׷����� ����												
	E_GRAPH2D_AUX_CURSOR_SEARCH_ALL = 2,		//! ��� �����͸� �˻��Ͽ� ���� Ŀ������ ã��. X�� ��� �ƴ�, X-Y �����Ͱ� 1:�� ���� �׷����� ����
}E_GRAPH2D_AUX_CURSOR_SEARCH_MODE;

//! Added by LHW [2008/4/19]
//! Modify by LHW [2008/4/21] : E_GRAPH2D_AUX_CURSOR_DASH_LINE �߰�
//! Modify by LHW [2008/8/04] : E_GRAPH2D_AUX_CURSOR_SOLID_LINE �߰�
typedef enum _E_GRAPH2D_AUX_CURSOR_DRAW_MODE	//! ���� Ŀ�� �׸��� ���
{
	E_GRAPH2D_AUX_CURSOR_CIRCLE = 0,			//! ������ ���� Ŀ�� ǥ��
	E_GRAPH2D_AUX_CURSOR_SOLID_LINE = 1,		//! �Ǽ����� ���� Ŀ�� ǥ��
	E_GRAPH2D_AUX_CURSOR_DOT_LINE = 2,			//! ������ ���� �������� ���� Ŀ�� ǥ��
	E_GRAPH2D_AUX_CURSOR_DASH_LINE = 3,			//! ������ �� �������� ���� Ŀ�� ǥ��
}E_GRAPH2D_AUX_CURSOR_DRAW_MODE;

typedef enum _E_GRAPH2D_AUX_CURSOR_BOUNDARY_MODE	//! ���� Ŀ�� ��� ����, Added by LHW [2008/8/19]
{
	E_GRAPH2D_AUX_CURSOR_BOUNDARY_NONE = 0,			//! ��� ���� ����
	E_GRAPH2D_AUX_CURSOR_BOUNDARY_CURSOR_LEFT = 1,	//! Main Ŀ���� ���ʿ��� ��ġ
	E_GRAPH2D_AUX_CURSOR_BOUNDARY_CURSOR_RIGHT = 2, //! Main Ŀ���� �����ʿ��� ��ġ
}E_GRAPH2D_AUX_CURSOR_BOUNDARY_MODE;

#define WM_GRAPH2D_AUX_CURSOR_EVENT	WM_USER + 1239	//! ���� Ŀ���� ���콺, Ű����� �����ϸ� �޽��� �߻�, Added by LHW [2008/8/5]
													//! ���� Ŀ���� ���ο� ��ġ ������ �޽����� �޾� ���� ���־�� ��

struct strGraph2D_Aux_Cursor_Event	//! ���� Ŀ�� �̺�Ʈ ����ü, Added by LHW [2008/8/5]
{
	int iPlotIndex;				//! �׷��� index

	int iAuxCursorGroupElementIndex;//! �̺�Ʈ�� �߻��� ���� Ŀ�� �׷� Element index
	int iAuxCursorElementIndex;		//! �̺�Ʈ�� �߻��� ���� Ŀ�� Element index

	int iAuxCursorGroupIndex;	//! �̺�Ʈ�� �߻��� ���� Ŀ�� �׷� index
	int iAuxCursorIndex;		//! �̺�Ʈ�� �߻��� ���� Ŀ�� index

	int iDataIndex;				//! �̺�Ʈ�� �߻��� ���� Ŀ���� ������ index
	double dbXData;				//! �̺�Ʈ�� �߻��� ���� Ŀ���� X��
	double dbYData;				//! �̺�Ʈ�� �߻��� ���� Ŀ���� Y��

	strGraph2D_Aux_Cursor_Event()
	{
		InitInfo();
	}

	void InitInfo()
	{
		iPlotIndex = -1;
		iAuxCursorGroupElementIndex = -1;
		iAuxCursorElementIndex = -1;
		iAuxCursorGroupIndex = -1;
		iAuxCursorIndex = -1;

		iDataIndex = -1;
		dbXData = 0.;
		dbYData = 0.;
	}

	strGraph2D_Aux_Cursor_Event& operator=(const strGraph2D_Aux_Cursor_Event &input)
	{
		iPlotIndex = input.iPlotIndex;
		iAuxCursorGroupElementIndex = input.iAuxCursorGroupElementIndex;
		iAuxCursorElementIndex = input.iAuxCursorElementIndex;
		iAuxCursorGroupIndex = input.iAuxCursorGroupIndex;
		iAuxCursorIndex = input.iAuxCursorIndex;

		iDataIndex = input.iDataIndex;
		dbXData = input.dbXData;
		dbYData = input.dbYData;
	}
};

//! Added by LHW [2008/3/31]
//! Modify by LHW [2008/4/1] : bValid, iOverlapIndex, eSearchMode, iDataIndex �߰�
//! Modify by LHW [2008/4/11] : ���� Ŀ�� ����ü���� ����ڰ� ���� �Է��ϸ� �ȵǴ� �κ��� �и�
//! Modify by LHW [2008/8/4] : eDrawMode, bMouseEvent, iCustomData, dbCustomData �߰�
//! Modify by LHW [2008/8/5] : iSearchMargin, bIndependency �߰�
//! Modify by LHW [2008/8/19] : eBoundaryMode �߰�
//! Modify by LHW [2008/8/20] : bCursorOverlapAllSearch, eMouseSearchMode �߰�
//! Modify by LHW [2009/6/4] : ���콺�� ������ ���� Ŀ�� ������ ������ ���̱� ���� iSearchMargin�� �ʱⰪ�� 100���� ����
struct strGraph2D_Aux_Cursor	//! ���� Ŀ�� ����ü
{
	E_GRAPH2D_AUX_CURSOR_TYPE			eCursorType;		//! ���� Ŀ���� ��� ���� ����
	E_GRAPH2D_AUX_CURSOR_SEARCH_MODE	eSearchMode;		//! ���� Ŀ���� ã�� ��� ����
	E_GRAPH2D_AUX_CURSOR_SEARCH_MODE	eMouseSearchMode;	//! Ű���峪 ���콺�� ���� Ŀ���� ã�� ��� ����
	E_GRAPH2D_AUX_CURSOR_DRAW_MODE		eDrawMode;			//! ���� Ŀ�� �׸��� ���
	E_GRAPH2D_AUX_CURSOR_BOUNDARY_MODE	eBoundaryMode;		//! ���� Ŀ���� ��� ����

	bool bCursorOverlapAllSearch;	//! true�̸� ���� Ŀ���� ���� ���ϱ� ���� ������ Overlap�� ��� �׷������� �۾� ����

	bool bMouseEvent;			//! true�̸� ���콺 �̺�Ʈ�� �޾� ó���� �� ����, true�̸� ���� Ŀ������ Ŀ�� ����� ����
	int iSearchMargin;			//! ���콺�� �����͸� ã�� ���� ���� ���� ��

	bool bIndependency;			//! ���� ����, true�̸� ���콺, Ű���� �����ӿ� ���� ���������� ������
	
	int iOverlapIndex;			//! ���� Ŀ���� ����Ű�� Overlap�Ǵ� �׷����� index, 0 based
	int iCursorIndex;			//! Ŀ�� index, ���� Ŀ�� index�� �ƴ�, 0 based
								//! eCursorType�� E_GRAPH2D_AUX_CURSOR_VARIABLE�� �� ������ �Ǵ� Ŀ�� index
	int iDataIndex;				//! ������ index, eCursorType�� E_GRAPH2D_AUX_CURSOR_FIXED_INDEX�� �� ���, 0 based

	double dbXData;				//! X��, eCursorType�� E_GRAPH2D_AUX_CURSOR_FIXED_INDEX�� ���� �����ϸ� ���� �Է��ؾ� ��
								//! eCursorType�� E_GRAPH2D_AUX_CURSOR_FIXED�� ���� ���밪
								//! eCursorType�� E_GRAPH2D_AUX_CURSOR_VARIABLE�� ���� ���� Ŀ�� ���� ���� ��밪

	CString szCursorText;		//! ���� Ŀ�� ����, ���� Ŀ�� ���� ǥ��

	int iCustomData;			//! ����� ������
	double dbCustomData;		//! ����� ������

	strGraph2D_Aux_Cursor()
	{
		InitInfo();
	}

	void InitInfo()
	{		
		eCursorType = E_GRAPH2D_AUX_CURSOR_FIXED;
		eSearchMode = E_GRAPH2D_AUX_CURSOR_SEARCH_X_RANGE;
		eMouseSearchMode = E_GRAPH2D_AUX_CURSOR_SEARCH_X_RANGE;
		eDrawMode = E_GRAPH2D_AUX_CURSOR_CIRCLE;
		eBoundaryMode = E_GRAPH2D_AUX_CURSOR_BOUNDARY_NONE;
		bCursorOverlapAllSearch = false;
		bMouseEvent = false;
		iSearchMargin = 100;
		bIndependency = false;
		iOverlapIndex = 0;
		iCursorIndex = 0;
		iDataIndex = 0;
		dbXData = 0.;				
		szCursorText = _T("");
		iCustomData = 0;
		dbCustomData = 0.;
	}

	strGraph2D_Aux_Cursor& operator=(const strGraph2D_Aux_Cursor &input)
	{		
		eCursorType = input.eCursorType;
		eSearchMode = input.eSearchMode;
		eMouseSearchMode = input.eMouseSearchMode;
		eDrawMode = input.eDrawMode;
		eBoundaryMode = input.eBoundaryMode;
		bCursorOverlapAllSearch = input.bCursorOverlapAllSearch;
		bMouseEvent = input.bMouseEvent;
		iSearchMargin = input.iSearchMargin;
		bIndependency = input.bIndependency;
		iOverlapIndex = input.iOverlapIndex;
		iCursorIndex = input.iCursorIndex;
		iDataIndex = input.iDataIndex;
		dbXData = input.dbXData;				
		szCursorText = input.szCursorText;
		iCustomData = input.iCustomData;
		dbCustomData = input.dbCustomData;

		return *this;
	}
};

//! Added by LHW [2008/3/31]
//! Modify by LHW [2008/4/1] : bShow �߰�
//! Modify by LHW [2008/4/10] : bFontVertical �߰�
//! Modify by LHW [2008/4/19] : eDrawMode �߰�
//! Modify by LHW [2008/5/9] : bFontBold �߰�
//! Modify by LHW [2008/8/4] : eDrawMode ����, iPenThickness�� iSolidPenThickness�� ��Ī ����, iCustomData, dbCustomData �߰�
struct strGraph2D_Aux_CursorGroup	//! ���� Ŀ�� �׷��� ���� ������ ���� ����ü
{
	bool		bShow;				//! true�̸� ���� Ŀ������ ���̰� ��
	
	bool		bFontHeight;		//! true�̸� ���� ���� ũ�⸦ �ڵ� ����
	int			iFontHeight;		//! ���� ���� ũ��, ���������� ����
	bool		bFontVertical;		//! true�̸� ���ڸ� �������� ��ġ��Ŵ
	bool		bFontBold;			//! true�̸� ���ڸ� ���� ǥ��
	CString		szFontName;			//! ������ ��Ʈ ����

	bool		bRadius;			//! true�̸� ���� Ŀ���� �ݰ��� �ڵ� ����
	int			iRadius;			//! ���� Ŀ���� �ݰ� (���� Ŀ���� �������� ǥ��)

	int			iSolidPenThickness;	//! ���� Ŀ�� �Ǽ��� �β�

	COLORREF	clrAuxCursor;		//! ���� Ŀ���� ����
	COLORREF	clrAuxCursorText;	//! ���� Ŀ�� ������ ����

	int iCustomData;			//! ����� ������
	double dbCustomData;		//! ����� ������

	strGraph2D_Aux_CursorGroup()
	{
		InitInfo();
	}

	void InitInfo()
	{
		bShow = true;		
		bFontHeight = true;
		iFontHeight = 10;
		bFontVertical = false;
		bFontBold = false;
		szFontName = _T("MS Sans Serif");
		bRadius = true;
		iRadius = 4;
		iSolidPenThickness = 1;
		clrAuxCursor = RGB(0, 0, 0);
		clrAuxCursorText = RGB(0, 0, 0);
		iCustomData = 0;
		dbCustomData = 0.;
	}

	strGraph2D_Aux_CursorGroup& operator=(const strGraph2D_Aux_CursorGroup &input)
	{
		bShow = input.bShow;		
		bFontHeight = input.bFontHeight;
		iFontHeight = input.iFontHeight;
		bFontVertical = input.bFontVertical;
		bFontBold = input.bFontBold;
		szFontName = input.szFontName;
		bRadius = input.bRadius;
		iRadius = input.iRadius;
		iSolidPenThickness = input.iSolidPenThickness;
		clrAuxCursor = input.clrAuxCursor;
		clrAuxCursorText = input.clrAuxCursorText;
		iCustomData = input.iCustomData;
		dbCustomData = input.dbCustomData;
		
		return *this;
	}
};

typedef enum _E_GRAPH2D_ZOOM_DIRECTION	//! Ȯ��Ǵ� ����
{
	E_GRAPH2D_ZOOM_DIRECTION_XY = 0,	//! X-Y�� ��� Ȯ��
	E_GRAPH2D_ZOOM_DIRECTION_X = 1,		//! X�ุ Ȯ��
	E_GRAPH2D_ZOOM_DIRECTION_Y = 2,		//! Y�ุ Ȯ��
}E_GRAPH2D_ZOOM_DIRECTION;

//! Modify by LHW [2008/10/25] : E_GRAPH2D_ZOOM_Y �߰�
//! Modify by LHW [2009/2/2] : E_GRAPH2D_PAN_X, E_GRAPH2D_PAN_Y �߰�
typedef enum _E_GRAPH2D_MOUSE_OPERATION	//! Mouse�� �̿��� ������ ����
{
	E_GRAPH2D_CURSOR_XY = 0,	//! X-Y�� �������� �����ϴ� Ŀ��
	E_GRAPH2D_ZOOM_XY = 1,		//! X-Y�� �������� Ȯ���� ���� ����
	E_GRAPH2D_ZOOM_X = 2,		//! X�� �������� Ȯ���� ���� ����
	E_GRAPH2D_ZOOM_Y = 3,		//! Y�� �������� Ȯ���� ���� ����
	E_GRAPH2D_PAN_XY = 4,		//! X-Y�� �������� �����ϴ� �׷����� ���� �̵�
	E_GRAPH2D_PAN_X = 5,		//! X�� �������� �����ϴ� �׷����� ���� �̵�
	E_GRAPH2D_PAN_Y = 6,		//! Y�� �������� �����ϴ� �׷����� ���� �̵�
}E_GRAPH2D_MOUSE_OPERATION;

typedef enum _E_GRAPH2D_GRID_TEXT_TYPE	//! Grid �� ǥ�� ���
{
	E_GRAPH2D_GRID_INTEGER = 0,	//! ������ ǥ��
	E_GRAPH2D_GRID_DOUBLE = 1,	//! �Ǽ��� ǥ��
	E_GRAPH2D_GRID_EXP = 2,		//! ������ ǥ��
}E_GRAPH2D_GRID_TEXT_TYPE;

//! Added by LHW [2008/4/2]
//! Modify by LHW [2008/4/9] : E_GRAPH2D_DATA_VISUAL_MILLI_SECOND �߰�
typedef enum _E_GRAPH2D_DATA_VISUAL_TYPE	//! �Էµ� �������� ������ ó�� ��Ŀ� ���� �����͸� ǥ���ϴ� ����� ����
{
	E_GRAPH2D_DATA_VISUAL_LINEAR = 0,		//! �Էµ� �����͸� �״�� �̿�
	E_GRAPH2D_DATA_VISUAL_CALCULATION = 1,	//! �Էµ� �����Ϳ� 1�� �Լ� ���� �����Ͽ� ������ �����͸� ����� ��
	E_GRAPH2D_DATA_VISUAL_TIME = 2,			//! �Էµ� �����Ͱ� Time �������� ��
	E_GRAPH2D_DATA_VISUAL_SECOND = 3,		//! �Էµ� �����Ͱ� �� ������ ������ ��
	E_GRAPH2D_DATA_VISUAL_MILLI_SECOND = 4,	//! �Էµ� �����Ͱ� Milli �� ������ ������ ��	
}E_GRAPH2D_DATA_VISUAL_TYPE;

//! Added by LHW [2008/4/3]
typedef enum _E_GRAPH2D_DATA_CALC_TYPE		//! �Էµ� �����Ϳ� 1�� �Լ� ���� ������ ���� ����
{
	//! y : ���� ��� ��, x : ���� ������, a : ���, b : Offset ��
	E_GRAPH2D_DATA_CALC_MULTIPLY = 0,		//! y = a * x + b
	E_GRAPH2D_DATA_CALC_DIVIDE = 1,			//! y = x / a + b
}E_GRAPH2D_DATA_CALC_TYPE;

//! Modify by LHW [2008/6/30] : E_GRAPH2D_RANGE_MARGIN_MAX_PERCENT �߰�
typedef enum _E_GRAPH2D_RANGE_MARGIN_TYPE	//! Auto Range�� ���� ���� �� ���� ����, �ּҰ� �Ʒ�, �ִ밪 ���� ���� ���� ���
{
	E_GRAPH2D_RANGE_MARGIN_NONE = 0,		//! ���� ���� ������� ����
	E_GRAPH2D_RANGE_MARGIN_PERCENT = 1,		//! strGraph2D_Axis_Info�� (dbManualMax - dbManualMin)�� %������ ����
	E_GRAPH2D_RANGE_MARGIN_MAX_PERCENT = 2,	//! strGraph2D_Axis_Info�� dbManualMax, dbManualMin �� ���밪���� ���Ͽ� �ִ밪�� ���� ��, �� ���� %���� ����
	E_GRAPH2D_RANGE_MARGIN_VALUE = 3,		//! ���� ���� ���� ������ ����	
}E_GRAPH2D_GRID_MARGIN_TYPE;

//! Added by LHW [2008/4/17] : Auto Range���� ����� ���� �����ϴ� ���
//! Modify by LHW [2009/5/30] : E_GRAPH2D_AUTO_RANGE_RESERVED_STEP �߰�
typedef enum _E_GRAPH2D_AUTO_RANGE_RESERVED_TYPE
{
	E_GRAPH2D_AUTO_RANGE_RESERVED_NONE = 0,		//! ���� ���� ����
	E_GRAPH2D_AUTO_RANGE_RESERVED_FORCED = 1,	//! ������ ���ప���� ����
	E_GRAPH2D_AUTO_RANGE_RESERVED_CONSIDER = 2,	//! ���������� �۰ų� Ŭ ��쿡�� ����
	E_GRAPH2D_AUTO_RANGE_RESERVED_STEP = 3,		//! ������ Step���� ������ ���� ����, �ּҰ��� ��쿡�� Step�� �� ���� ��, �ִ밪�� ��쿡�� Step�� �� ū ���� ����
}E_GRAPH2D_AUTO_RANGE_RESERVED_TYPE;

//! Modify by LHW [2008/4/2] : GetManualMinTime, SetManualMinTime, GetManualMaxTime, SetManualMaxTime, eGridType �߰�
//! Modify by LHW [2008/4/4] : bAutoRangeReservedMin, dbAutoRangeReservedMin, bAutoRangeReservedMax, dbAutoRangeReservedMax �߰�
//! Modify by LHW [2008/4/16] : bKilo �߰�
//! Modify by LHW [2008/4/17] : bAutoRangeReservedMin�� eAutoRangeReservedMin��, bAutoRangeReservedMax�� eAutoRangeReservedMax�� ����
//! Modify by LHW [2008/6/30] : bAutoGridTextPrecision, bAutoCursorTextPrecision �߰�
//! Modify by LHW [2008/7/1] : bSymmetry �߰�
//! Modify by LHW [2008/7/24] : szCaption�� �����ϰ�, szCaption1, szCaption2 �߰�
//! Modify by LHW [2009/2/4] : bMaxOperation, bAutoRangeGridSize, iAutoRangeGridSize �߰�
//! Modify by LHW [2009/3/1] : bGridCalc, bCursorCalc �߰�
//! Modify by LHW [2009/5/30] : bMarginIncludeMin, bMarginIncludeMax �߰�
//! Modify by LHW [2013/3/26] : bDirection_Plus �߰�
struct strGraph2D_Axis_Info	//! ��, Grid, Ŀ�� ���� ���� ����ü
{
	bool bAutoRange;		//! true�̸� Auto Range
	double dbManualMin;		//! ���� ���̴� �ּҰ�
	double dbManualMax;		//! ���� ���̴� �ִ밪

	bool bMaxOperation;		//! true�̸� bAutoRange�� true�� ��, �ִ밪�� ... 0.01, 0.25, 0.5, 1, 2.5, 5, 10, 25, 50, 100, 250, 500, ... �� �� �� �ϳ��� �������� ��
	bool bMinOperation;		//! true�̸� bAutoRange�� true�� ��, �ִ밪�� ... -0.01, -0.25, -0.5, -1, -2.5, -5, -10, -25, -50, -100, -250, -500, ... �� �� �� �ϳ��� �������� ��

	bool bDirection_Plus;	//! true�̸� �ּҰ����� �ִ밪���� �����ϴ� �������� �׷����� �׷�����.
							//!   - X�࿡�� true�̸�, ���ʿ��� ���������� �����ϴ� �������� �׷����� �׸���. 
							//!   - Y�࿡�� true�̸�, �Ʒ��ʿ��� �������� �����ϴ� �������� �׷����� �׸���. 

	bool bSymmetry;			//! true�̸� bAutoRange�� true�� ��, �ִ�, �ּҰ� �� ���밪�� ū ������ ��Ī�ǰ� ����
	E_GRAPH2D_AUTO_RANGE_RESERVED_TYPE eAutoRangeReservedMin;
	double dbAutoRangeReservedMin;	//! ���� �ּҰ�, dbManualMin���� ����
	E_GRAPH2D_AUTO_RANGE_RESERVED_TYPE eAutoRangeReservedMax;
	double dbAutoRangeReservedMax;	//! ���� �ִ밪, dbManualMax���� ����

	E_GRAPH2D_GRID_MARGIN_TYPE eMarginType;	//! �ּҰ� �Ʒ�, �ִ밪 ���� ���� ���� ����ϴ� ��� ����
											//! bAutoRange�� true�̰�,
											//! eAutoRangeReservedMin, eAutoRangeReservedMax�� ���� E_GRAPH2D_AUTO_RANGE_RESERVED_NONE �� ���� ���
	double dbMargin;		//! eMarginType�� E_GRAPH2D_RANGE_MARGIN_PERCENT�̸� '(dbManualMax - dbManualMin)'�� %��
							//! eMarginType�� E_GRAPH2D_RANGE_MARGIN_MAX_PERCENT�̸� 'max(fabs(dbManualMax), fabs(dbManualMin))'�� %��
							//! eMarginType�� E_GRAPH2D_RANGE_MARGIN_VALUE�̸� ���� ��

	bool bMarginZero;		//! true�̸� (dbManualMax - dbManualMin)�� 0�� ���, ���� ���� ���
	double dbMarginZero;	//! ���� ��, %���� �ƴ� ���밪

	bool bMarginIncludeMin;	//! eAutoRangeReservedMin�� E_GRAPH2D_AUTO_RANGE_RESERVED_STEP�� ��, true�̸� Margin�� ���
	bool bMarginIncludeMax;	//! eAutoRangeReservedMax�� E_GRAPH2D_AUTO_RANGE_RESERVED_STEP�� ��, true�̸� Margin�� ���

	bool bShowGrid;				//! false�̸� �� ������ ������ �ʰ� ��
	bool bShowMin;				//! false�̸� �� �ּҰ��� ������ �ʰ� ��
	bool bShowMax;				//! false�̸� �� �ִ밪�� ������ �ʰ� ��
	bool bIncrementGrid;		//! true�̸� �� �ּҰ����� dbIncrementGrid��ŭ ������Ű�� Grid ����
	double dbIncrementGrid;		//! �� Grid ������
	int iGridSize;				//! bIncrementGrid�� false�̸� ������ ������ŭ �ִ��� ������� Grid ����
	bool bAutoRangeGridSize;	//! true�̸� bAutoRange�� true�� ��, iAutoRangeGridSize�� iGridSize�� ����
	int iAutoRangeGridSize;		//! bAutoRange�� true�̸� ����ϴ� �� Grid�� ����
	bool bAutoGridTextPrecision;//! true�̸� Grid���� ǥ�� ���ڰ� �Ǽ��� ǥ��(E_GRAPH2D_GRID_DOUBLE)�� ���� �Ҽ��� �ڸ��� �ڵ� ����
	int iGridTextPrecision;		//! Grid���� ǥ�� ���ڰ� �Ǽ��� ǥ���� ���� �Ҽ��� �ڸ��� (0 �̻��� ������)
								//! Grid���� ǥ�� ���ڰ� ������ ǥ���� ���� �����κ��� �Ҽ��� �ڸ��� (0 �̻��� ������)
	E_GRAPH2D_GRID_TEXT_TYPE eGridTextType;		//! Grid ǥ�� ������ ǥ�� ����(������, �Ǽ���, ������)
	E_GRAPH2D_DATA_VISUAL_TYPE eGridVisualType;	//! Grid�� ����(����, Time ��)
	bool bKilo;				//! true�̸� �Էµ� �����Ͱ� 1000���� ũ�� 1000���� ������ �ְ� 'k'�� ����
							//! eGridVisualType�� E_GRAPH2D_DATA_VISUAL_LINEAR, E_GRAPH2D_DATA_VISUAL_CALCULATION�϶��� ���

	E_GRAPH2D_DATA_CALC_TYPE eDataCalcType;		//! �����Ϳ� 1�� �Լ� ���� ������ �� � ������ �� ������ ����
												//! eGridVisualType�� E_GRAPH2D_DATA_VISUAL_CALCULATION �� ���� ���
	double dbCoeff;								//! 1�� �Լ����� ��� ��
	double dbYOffset;							//! 1�� �Լ����� Y Offset ��
	bool bGridCalc;								//! true�̸� Grid ���� ����
												//! eGridVisualType�� E_GRAPH2D_DATA_VISUAL_CALCULATION �� ���� ���
	bool bCursorCalc;							//! true�̸� Cursor ���� ����
												//! eGridVisualType�� E_GRAPH2D_DATA_VISUAL_CALCULATION �� ���� ���

	bool bTimeGrid_CoupleRow;					//! Grid ���ڿ��� 2�� �� �� true
												//! eGridVisualType�� E_GRAPH2D_DATA_VISUAL_TIME �� ���� ���
												//! false�̸� szGridTimeFormat1 �� ���
	CString szGridTimeFormat1;					//! Grid �ð� ���� ���ڿ�, ��) %Y/%m/%d %H:%M:%S
	CString szGridTimeformat2;					//! Grid �ð� ���� ���ڿ�, ��) %Y/%m/%d %H:%M:%S
    
	bool bAutoCursorTextPrecision;				//! true�̸� Ŀ�� ����â���� ǥ�� ���ڰ� �Ǽ��� ǥ��(E_GRAPH2D_GRID_DOUBLE)�� ���� �Ҽ��� �ڸ��� �ڵ� ����
	int iCursorTextPrecision;					//! Ŀ�� ����â�� ǥ�õǴ� ������ �Ҽ��� �ڸ��� (0 �̻��� ������)
												//! Ŀ�� ����â�� ���ڴ� �Ҽ��� �ڸ����� �����ϸ�, 
												//! eGridTextType, eGridVisualType ���� Grid ǥ�� ������ ���� ����
	CString szCursorTimeFormat;					//! Ŀ�� �ð� ���� ���ڿ�, ��) %Y/%m/%d %H:%M:%S

	CString szCaption1;			//! �� �̸� (�׷��� ������ �� �κп� ǥ�õǴ� ���ڿ� [X : ~ , Y : ~])
								//!			E_GRAPH2D_XY_CAPTION_POS_UNITE, E_GRAPH2D_XY_CAPTION_POS_SEPARATE (X, Y�� �Ʒ� �κ�)���� ���
	CString szCaption2;			//! �� �̸� (�׷��� ������ �� �κп� ǥ�õǴ� ���ڿ� [X : ~ , Y : ~])
								//!			E_GRAPH2D_XY_CAPTION_POS_SEPARATE������ ���(Y�� �� �κ�)
	CString szCursorUnit;		//! Ŀ�� ����â�� ǥ�õ� ����

	strGraph2D_Axis_Info()
	{
		InitInfo();
	}

	void InitInfo()
	{
		bAutoRange = true;
		dbManualMin = 0.;
		dbManualMax = 0.;

		bMaxOperation = false;
		bMinOperation = false;

		bDirection_Plus = true;

		bSymmetry = false;
		eAutoRangeReservedMin = E_GRAPH2D_AUTO_RANGE_RESERVED_NONE;
		dbAutoRangeReservedMin = 0.;
		eAutoRangeReservedMax = E_GRAPH2D_AUTO_RANGE_RESERVED_NONE;
		dbAutoRangeReservedMax = 0.;

		eMarginType = E_GRAPH2D_RANGE_MARGIN_NONE;
		dbMargin = 0.;

		bMarginZero = true;
		dbMarginZero = 1.;

		bMarginIncludeMin = true;
		bMarginIncludeMax = true;

		bShowGrid = true;
		bShowMin = true;
		bShowMax = true;
		bIncrementGrid = false;
		dbIncrementGrid = 10.;
		iGridSize = 4;
		bAutoRangeGridSize = false;
		iAutoRangeGridSize = 5;
		bAutoGridTextPrecision = true;
		iGridTextPrecision = 1;
		eGridTextType = E_GRAPH2D_GRID_DOUBLE;
		eGridVisualType = E_GRAPH2D_DATA_VISUAL_LINEAR;
		bKilo = false;

		eDataCalcType = E_GRAPH2D_DATA_CALC_MULTIPLY;
		dbCoeff = 1.;
		dbYOffset = 0.;
		bGridCalc = true;
		bCursorCalc = true;

		bTimeGrid_CoupleRow = false;
		szGridTimeFormat1 = _T("%Y/%m/%d %H:%M:%S");
		szGridTimeformat2 = _T("%Y/%m/%d %H:%M:%S");

		bAutoCursorTextPrecision = true;
		iCursorTextPrecision = 1;
		szCursorTimeFormat = _T("%Y/%m/%d %H:%M:%S");

		szCaption1 = _T("");
		szCaption2 = _T("");
		szCursorUnit = _T("");
	}

	COleDateTime GetManualMinTime()	//! X���� Time�� ���� �� �Լ��� ����Ͽ� �ּҰ��� ��ȯ
	{
		COleDateTime timeMin;
		timeMin.m_dt = dbManualMin;
		return timeMin;
	}

	void SetManualMinTime(COleDateTime timeMin)	//! X���� Time�� ���� �� �Լ��� ����Ͽ� �ּҰ��� ����
	{
		dbManualMin = (double)(timeMin.m_dt);
	}

	COleDateTime GetManualMaxTime()	//! X���� Time�� ���� �� �Լ��� ����Ͽ� �ִ밪�� ��ȯ
	{
		COleDateTime timeMax;
		timeMax.m_dt = dbManualMax;
		return timeMax;
	}

	void SetManualMaxTime(COleDateTime timeMax)	//! X���� Time�� ���� �� �Լ��� ����Ͽ� �ִ밪�� ����
	{
		dbManualMax = (double)(timeMax.m_dt);
	}

	strGraph2D_Axis_Info& operator=(const strGraph2D_Axis_Info &input)
	{
		bAutoRange = input.bAutoRange;		
		dbManualMin = input.dbManualMin;
		dbManualMax = input.dbManualMax;

		bMaxOperation = input.bMaxOperation;
		bMinOperation = input.bMinOperation;

		bDirection_Plus = input.bDirection_Plus;

		bSymmetry = input.bSymmetry;
		eAutoRangeReservedMin = input.eAutoRangeReservedMin;
		dbAutoRangeReservedMin = input.dbAutoRangeReservedMin;
		eAutoRangeReservedMax = input.eAutoRangeReservedMax;
		dbAutoRangeReservedMax = input.dbAutoRangeReservedMax;

		eMarginType = input.eMarginType;
		dbMargin = input.dbMargin;

		bMarginZero = input.bMarginZero;
		dbMarginZero = input.dbMarginZero;

		bMarginIncludeMin = input.bMarginIncludeMin;
		bMarginIncludeMax = input.bMarginIncludeMax;
		
		bShowGrid = input.bShowGrid;
		bShowMin = input.bShowMin;
		bShowMax = input.bShowMax;
		bIncrementGrid = input.bIncrementGrid;
		dbIncrementGrid = input.dbIncrementGrid;
		iGridSize = input.iGridSize;
		bAutoRangeGridSize = input.bAutoRangeGridSize;
		iAutoRangeGridSize = input.iAutoRangeGridSize;
		bAutoGridTextPrecision = input.bAutoGridTextPrecision;
		iGridTextPrecision = input.iGridTextPrecision;
		eGridTextType = input.eGridTextType;
		eGridVisualType = input.eGridVisualType;
		bKilo = input.bKilo;

		eDataCalcType = input.eDataCalcType;
		dbCoeff = input.dbCoeff;
		dbYOffset = input.dbYOffset;
		bGridCalc = input.bGridCalc;
		bCursorCalc = input.bCursorCalc;

		bTimeGrid_CoupleRow = input.bTimeGrid_CoupleRow;
		szGridTimeFormat1 = input.szGridTimeFormat1;
		szGridTimeformat2 = input.szGridTimeformat2;

		bAutoCursorTextPrecision = input.bAutoCursorTextPrecision;
		iCursorTextPrecision = input.iCursorTextPrecision;
		szCursorTimeFormat = input.szCursorTimeFormat;

		szCaption1 = input.szCaption1;
		szCaption2 = input.szCaption2;
		szCursorUnit = input.szCursorUnit;

		return *this;
	}
};

//! Modify by LHW [2008/5/23] : E_GRAPH2D_DATA_DRAW_TYPE ���� ������ ���� ��Ī ����
typedef enum _E_GRAPH2D_DRAW_RANGE_MODE	//! �׷��� ���� �׸��� ����� ����
{
	E_GRAPH2D_VISIBLE_RANGE_DRAW = 0,	//! ���� ���̴� �κ��� �׷����� �׸�
										//! X�� �������� �����Կ� ���� Y �����Ͱ� �����ϴ� ��쿡�� ��ȿ�� ����� ������
										//! 1) X-Y �׷����� �׸� ��, 2) �׷����� Overlap�� ��쿡�� Ȯ���� �� ��� ������ ������ ����
	E_GRAPH2D_ALL_DRAW = 1,				//! ��� �����͸� �׸�
}E_GRAPH2D_DRAW_RANGE_MODE;

//! Added by LHW [2008/5/23]
//! Modify by LHW [2009/2/3] : E_GRAPH2D_DATA_DRAW_RECTANGLE�� E_GRAPH2D_DATA_DRAW_SYMBOL�� ����, E_GRAPH2D_DATA_DRAW_LINE_SYMBOL �߰�
typedef enum _E_GRAPH2D_DATA_DRAW_TYPE	//! �׷��� ǥ�� ����� ����
{
	E_GRAPH2D_DATA_DRAW_LINE = 0,		//! ��
	E_GRAPH2D_DATA_DRAW_SYMBOL = 1,		//! �ɺ�
	E_GRAPH2D_DATA_DRAW_LINE_SYMBOL = 2,//! �� + �ɺ�
}E_GRAPH2D_DATA_DRAW_TYPE;

//! Added by LHW [2009/2/3]
//! Modify by LHW [2009/2/16] : ���� ����
//! Modify by LHW [2009/3/6] : E_GRAPH2D_DRAW_CROSS, E_GRAPH2D_DRAW_DIAMOND, E_GRAPH2D_DRAW_REVERSE_TRIANGLE �߰�
typedef enum _E_GRAPH2D_DRAW_SYMBOL_TYPE	//! �׷����� �����͸� ǥ���ϴ� �ɺ��� ����
{
	E_GRAPH2D_DRAW_RECTANGLE = 0,
	E_GRAPH2D_DRAW_CIRCLE = 1, 
	E_GRAPH2D_DRAW_TRIANGLE = 2, 	
	E_GRAPH2D_DRAW_CROSS = 3, 
	E_GRAPH2D_DRAW_DIAMOND = 4, 
	E_GRAPH2D_DRAW_REVERSE_TRIANGLE = 5, 
	E_GRAPH2D_DRAW_FILL_RECT = 6, 
}E_GRAPH2D_DRAW_SYMBOL_TYPE;

//! Added by LHW [2008/7/24]
typedef enum _E_GRAPH2D_LINE_TYPE		//! �׷��� ���� ����
{
	E_GRAPH2D_LINE_TYPE_SOLID = 0,		//! �Ǽ�
	E_GRAPH2D_LINE_TYPE_DOT = 1,		//! ������ ������ ����
	E_GRAPH2D_LINE_TYPE_DASH = 2,		//! ������ �뼺�� ����
	E_GRAPH2D_LINE_TYPE_DASH_DOT = 3,	//! 1���⼱
	E_GRAPH2D_LINE_TYPE_DASH_DOTDOT = 4,//! 2���⼱
}E_GRAPH2D_LINE_TYPE;

//! Modify by LHW [2008/5/23] : eDrawRangeMode ����, eDataDrawType, bShowEventRect, iEventRectSize �߰�
//! Modify by LHW [2008/5/28] : iEventLineWidth �߰�
//! Modify by LHW [2008/7/24] : eLineType �߰�
//! Modify by LHW [2009/2/3] : iSymbolLineWidth, iSymbolSize, eSymbolType, eEventSymbolType �߰�
//! Modify by LHW [2009/2/4] : �ɺ��� ũ�� ���� �ʱⰪ ����
struct strGraph2D_Y_Axis_Info			//! Y���� ���� ����ü
{
	strGraph2D_Axis_Info strAxisInfo;	//! Grid ���� ����

	bool		bShowGraph;				//! false�̸� Grid, Min, Max, �׷��� ��� ������ �ʰ� ��
	bool		bAxisLeft;				//! true�̸� Y���� ���ʿ� �׸��� false�̸� �����ʿ� �׸�	
	COLORREF	clrData;				//! �׷��� ���� ����
	int			iLineWidth;				//! �׷��� ���� �β�
	
	COLORREF	clrSymbol;				//! �׷����� �� �����͸� ǥ���ϴ� �ɺ��� ����
	int			iSymbolLineWidth;		//! �׷����� �� �����͸� ǥ���ϴ� �ɺ� ���� �β�
	int			iSymbolSize;			//! �׷����� �� �����͸� ǥ���ϴ� �ɺ��� ũ��
										//! �簢���� ���� �� ���� ���̰� (2 * iLineWidth) + 1 �ȼ��� ��, 0 �̸� 1 �ȼ��� ǥ��
	
	E_GRAPH2D_DRAW_RANGE_MODE eDrawRangeMode;	//! �׷��� ���� �׸��� ����� ������ ����
	E_GRAPH2D_DATA_DRAW_TYPE eDataDrawType;		//! �׷����� ǥ�� ����� ����
	E_GRAPH2D_LINE_TYPE eLineType;				//! �׷��� ���� ����, eDataDrawType�� E_GRAPH2D_DATA_DRAW_LINE, E_GRAPH2D_DATA_DRAW_LINE_SYMBOL�� ���� ��ȿ
	E_GRAPH2D_DRAW_SYMBOL_TYPE eSymbolType;		//! �׷����� �� �����͸� ǥ���ϴ� �ɺ��� ����

	//! CGraph2DWnd�� m_bDataPointEventMsg�� true�� ���� ���
	bool		bShowEventRect;					//! true�̸� Data-Point Event ������ ���� ���̰� ��, ���ΰ� ������ �簢������ ǥ��
	int			iEventLineWidth;				//! Data-Point Event ���� ���� �β�
	int			iEventRectSize;					//! Data-Point Event ������ ũ��, �� ���� ���̰� (2 * iEventRectSize) + 1 �ȼ��� �簢������ ����
	COLORREF	clrEventRect;					//! Data-Point Event �ɺ��� ����
	E_GRAPH2D_DRAW_SYMBOL_TYPE eEventSymbolType;//! Data-Point ������ ��Ÿ���� �ɺ��� ����, ũ��� iEventRectSize�� ���

	strGraph2D_Y_Axis_Info()
	{
		InitInfo();
	}

	void InitInfo()
	{
		strAxisInfo.InitInfo();
		strAxisInfo.iGridTextPrecision = 3;
		strAxisInfo.iCursorTextPrecision = 3;
		bShowGraph = true;
		bAxisLeft = true;		
		clrData = RGB(0, 64, 0);
		iLineWidth = 1;
		clrSymbol = RGB(0, 0, 64);
		iSymbolLineWidth = 1;
		iSymbolSize = 3;
		eDrawRangeMode = E_GRAPH2D_VISIBLE_RANGE_DRAW;
		eDataDrawType = E_GRAPH2D_DATA_DRAW_LINE;
		eLineType = E_GRAPH2D_LINE_TYPE_SOLID;
		eSymbolType = E_GRAPH2D_DRAW_RECTANGLE;
		bShowEventRect = false;
		iEventLineWidth = 1;
		iEventRectSize = 3;
		clrEventRect = RGB(0, 0, 64);
		eEventSymbolType = E_GRAPH2D_DRAW_RECTANGLE;
	}

	strGraph2D_Y_Axis_Info& operator=(const strGraph2D_Y_Axis_Info &input)
	{
		strAxisInfo = input.strAxisInfo;
		bShowGraph = input.bShowGraph;
		bAxisLeft = input.bAxisLeft;	
		clrData = input.clrData;
		iLineWidth = input.iLineWidth;
		clrSymbol = input.clrSymbol;
		iSymbolLineWidth = input.iSymbolLineWidth;
		iSymbolSize = input.iSymbolSize;
		eDrawRangeMode = input.eDrawRangeMode;
		eDataDrawType = input.eDataDrawType;
		eLineType = input.eLineType;
		eSymbolType = input.eSymbolType;
		bShowEventRect = input.bShowEventRect;
		iEventLineWidth = input.iEventLineWidth;
		iEventRectSize = input.iEventRectSize;
		clrEventRect = input.clrEventRect;
		eEventSymbolType = input.eEventSymbolType;

		return *this;
	}
};

//! Modify by LHW [2008/8/21] : bShowChk �߰�
//! Modify by LHW [2009/2/3] : bShowText, szText, bTextPosExt �߰�
struct strValueLineYInfo		//! Y�� Ư���� ���� ����ü
{
	bool		bShow;			//! ���� ���̸� true
	bool		bGraphShowChk;	//! true�̸� �Ʒ� iOverlapIndex�� �׷����� ���� ������ ������ ���� ������ �ʰ�, 
								//! �׷����� ���� ���̸� ���� ����
	bool		bShowChk;		//! true�̸� bShow�� false�� ���, Y�� Auto Range�� Y�� Ư������ �ݿ����� ����
	int			iOverlapIndex;	//! �׷��� index, � �׷������� ����� ������ ����, 0 based
								//! ���� ���� �׷����� �ְ�, ���� ���϶��� �ش� �׷��� index ����
								//! �ʱⰪ�� 0
	double		dbValue;		//! Y�� Ư����
	COLORREF	clrValue;		//! �� ����
	int			iLineWidth;		//! �� �β�
	bool		bShowText;		//! true�̸� szText�� ���̰� ��, �� Y�� ���� ���� ���������� ���ڿ��� ������
	CString		szText;			//! ǥ�� ���ڿ�
	bool		bTextPosExt;	//! true�̸� �׷��� ������ �ٱ� �κп� ���ڿ��� ��ġ��Ű��, false�̸� �׷��� ������ ������ �κп� ��ġ

	strValueLineYInfo()
	{
		InitInfo();
	}

	void InitInfo()
	{
		bShow = true;
		bGraphShowChk = true;
		bShowChk = true;
		iOverlapIndex = 0;
		dbValue = 0.;
		clrValue = RGB(0, 0, 0);
		iLineWidth = 1;
		bShowText = false;
		szText = _T("");
		bTextPosExt = false;
	}

	strValueLineYInfo& operator=(const strValueLineYInfo &input)
	{
		bShow = input.bShow;
		bGraphShowChk = input.bGraphShowChk;
		bShowChk = input.bShowChk;
		iOverlapIndex = input.iOverlapIndex;
		dbValue = input.dbValue;
		clrValue = input.clrValue;
		iLineWidth = input.iLineWidth;
		bShowText = input.bShowText;
		szText = input.szText;
		bTextPosExt = input.bTextPosExt;
		
		return *this;
	}
};

struct strValueLineXInfo		//! X�� Ư���� ���� ����ü
{
	bool		bShow;			//! ���� ���̸� true

	double		dbValue;		//! X�� Ư����
	COLORREF	clrValue;		//! �� ����
	int			iLineWidth;		//! �� �β�

	CString		szText;			//! ǥ�� ���ڿ�

	strValueLineXInfo()
	{
		InitInfo();
	}

	void InitInfo()
	{
		bShow = true;

		dbValue = 0.;
		clrValue = RGB(0, 0, 0);
		iLineWidth = 1;
		szText = _T("");
	}

	strValueLineXInfo& operator=(const strValueLineXInfo &input)
	{
		bShow = input.bShow;
		dbValue = input.dbValue;
		clrValue = input.clrValue;
		iLineWidth = input.iLineWidth;
		szText = input.szText;

		return *this;
	}
};

#define WM_GRAPH2D_DECIDE_TIME_X_AXIS WM_USER + 1240	//! X���� �ð��� ǥ���� ��, ���� ���̴� X���� �ּ�, �ִ밪�� ������ �� �߻��ϴ� �޽���, Added by LHW [2008/8/18]
														//! WPARAM : NULL, LPARAM : NULL

typedef enum _E_GRAPH2D_TITLE_ALIGN		//! ���� ���� ����
{
	E_GRAPH2D_TITLE_ALIGN_LEFT = 0,		//! ���� ����
	E_GRAPH2D_TITLE_ALIGN_CENTER = 1,	//! �߾� ����
	E_GRAPH2D_TITLE_ALIGN_RIGHT = 2,	//! ������ ����
}E_GRAPH2D_TITLE_ALIGN;

//! Modify by LHW [2009/2/26] : ��Ʈ ����, ���� ũ�� �߰�
struct strGraph2DTitleInfo					//! ���� ���� ����ü
{
	bool					bShow;				//! true�̸� ������ ���̰�, false�̸� ������ ����
	bool					bBold;				//! true�̸� ���ڸ� ���� ó��
	E_GRAPH2D_TITLE_ALIGN	eAlign;				//! ���� ���� ����
	double					dbLeftMargin;		//! ���� �����ڸ����� �Ÿ�, ���� : ��Ʈ�� ��ü ���� ���� %
												//! E_GRAPH2D_TITLE_ALIGN_LEFT, E_GRAPH2D_TITLE_ALIGN_CENTER������ ���
	double					dbRightMargin;		//! ������ �����ڸ����� �Ÿ�, ���� : ��Ʈ�� ��ü ���� ���� %
												//! E_GRAPH2D_TITLE_ALIGN_RIGHT������ ���
	COLORREF				clrTitle;			//! ���� ����
	CString					szTitle;			//! ���� ���ڿ�
	bool					bTitleFontHeight;	//! ���� ǥ�� ���� ���� ũ�⸦ �ڵ� ����
	int						iTitleFontHeight;	//! ���� ǥ�� ���� ���� ũ��, ���������� ����
	CString					szTitleFontName;	//! ���� ǥ�� ������ ��Ʈ ����

	strGraph2DTitleInfo()
	{
		InitInfo();
	}

	void InitInfo()
	{
		bShow = false;
		bBold = false;
		eAlign = E_GRAPH2D_TITLE_ALIGN_CENTER;
		dbLeftMargin = 0.;
		dbRightMargin = 0.;
		clrTitle = RGB(0, 0, 0);
		szTitle = _T("");
		bTitleFontHeight = true;
		iTitleFontHeight = -10;
		szTitleFontName = _T("MS Sans Serif");
	}

	strGraph2DTitleInfo& operator=(const strGraph2DTitleInfo &input)
	{
		bShow = input.bShow;
		bBold = input.bBold;
		eAlign = input.eAlign;
		dbLeftMargin = input.dbLeftMargin;
		dbRightMargin = input.dbRightMargin;
		clrTitle = input.clrTitle;
		szTitle = input.szTitle;
		bTitleFontHeight = input.bTitleFontHeight;
		iTitleFontHeight = input.iTitleFontHeight;
		szTitleFontName = input.szTitleFontName;

		return *this;
	}
};

//! Added by LHW [2008/7/24] : X-Y ǥ�� ���ڿ��� ǥ�� ���, strGraph2D_Axis_Info�� szCaption1�� ǥ��
//! Modify by LHW [2008/8/1] : E_GRAPH2D_XY_CAPTION_POS_UNITE_Y �߰�
//! Modify by LHW [2008/10/24] : E_GRAPH2D_XY_CAPTION_POS_ONLY_XY �߰�
//! 2012/12/20 : E_GRAPH2D_XY_CAPTION_POS_ONLY_XY �߰�
typedef enum _E_GRAPH2D_XY_CAPTION_POSITION
{
	E_GRAPH2D_XY_CAPTION_POS_NONE = 0,		//! ǥ������ ����
	E_GRAPH2D_XY_CAPTION_POS_ONLY_XY = 1,	//! X�� �����ʿ� 'X'���� ǥ��, Y�� ���ʿ� 'Y'���� ǥ�ø� ǥ��
	E_GRAPH2D_XY_CAPTION_POS_UNITE = 2,		//! �� �ٷ� �����Ͽ� �� �ٷ��� ���� Ȥ�� �����ʿ� ��ġ, X�� �����ʿ� 'X'���� ǥ��, Y�� ���ʿ� 'Y'���� ǥ��
	E_GRAPH2D_XY_CAPTION_POS_SEPARATE = 3,	//! �� ���� ��, �Ʒ��� ǥ��
	E_GRAPH2D_XY_CAPTION_POS_UNITE_Y = 4,	//! X���� E_GRAPH2D_XY_CAPTION_POS_SEPARATE�� �����ϰ� ǥ��, Y���� E_GRAPH2D_XY_CAPTION_POS_UNITE�� �����ϰ� ǥ��
	E_GRAPH2D_XY_CAPTION_POS_ONE_Y = 5,		//! X���� E_GRAPH2D_XY_CAPTION_POS_SEPARATE�� �����ϰ� ǥ��, Y���� strGraph2DResourceInfo�� szCaptionY ǥ��
	E_GRAPH2D_XY_CAPTION_POS_ONLY_X = 6,	//! X���� E_GRAPH2D_XY_CAPTION_POS_SEPARATE�� �����ϰ� ǥ��, Y���� ǥ������ ����
}E_GRAPH2D_XY_CAPTION_POSITION;

//! Added by LHW [2008/5/14] : clrCursorSelectedLine �߰�
//! Modify by LHW [2008/7/24] : eXYCaptionPosition �߰�
//! Modify by LHW [2008/8/01] : bYCaptionLeftPos �߰�
//! Modify by LHW [2008/12/27] : szCaptionY �߰�
//! Modify by LHW [2009/2/26] : ������ ��Ʈ ����, ���� ũ�� ����
struct strGraph2DResourceInfo				//! ����, X�� ���� ����ü
{
	COLORREF	clrEntireBackGround;		//! �׷��� ��Ʈ�� ��ü ������
	COLORREF	clrGraphBackGround;			//! ���� �׷��� �κ� ������
	COLORREF	clrAxis;					//! �༱�� ����
	COLORREF	clrGrid;					//! Grid ������ ����
	COLORREF	clrGridText;				//! Grid ���� ������ ����
	COLORREF	clrCursorLine;				//! Ŀ�� ���� ����
	COLORREF	clrCursorSelectedLine;		//! ���õ� Ŀ�� ���� ����
	COLORREF	clrCursorText;				//! Ŀ���� ������ ����
	COLORREF	clrCursorTextBorder;		//! Ŀ�� ����â�� �׵θ� ���� ����
	COLORREF	clrCursorPoint;				//! Ŀ�� ���� ����
	COLORREF	clrCursorTextBack;			//! Ŀ�� ����â�� ������
	COLORREF	clrLegend;					//! X,Y �� ���� ������ ����
	COLORREF	clrZoomArea;				//! Ȯ�� ���� ����

	bool		bGridFontHeight;			//! true�̸� Grid ǥ�� ���� ���� ũ�⸦ �ڵ� ����
	bool		bCursorFontHeight;			//! true�̸� Cursor ǥ�� ���� ���� ũ�⸦ �ڵ� ����
	bool		bLegendFontHeight;			//! true�̸� X-Y ���� ���ڿ� ǥ�� ���� ���� ũ�⸦ �ڵ� ����
	
	int			iGridFontHeight;			//! Grid ǥ�� ���� ���� ũ��, ���������� ����
	int			iCursorFontHeight;			//! Cursor ǥ�� ���� ���� ũ��, ���������� ����
	int			iLegendFontHeight;			//! X-Y ���� ���ڿ� ǥ�� ���� ���� ũ��, ���������� ����
	
	CString		szGridFontName;				//! Grid ǥ�� ������ ��Ʈ ����
	CString		szCursorFontName;			//! Cursor ǥ�� ������ ��Ʈ ����
	CString		szLegendFontName;			//! X-Y ���� ���ڿ� ǥ�� ������ ��Ʈ ����
	
	int			iAxisThickness;				//! �� ���� �β�
	int			iGridThickness;				//! Grid ���� �β�
	int			iCursorLineThickness;		//! Ŀ�� ���� �β�
	int			iCursorTextBorderThickness;	//! Ŀ�� ����â�� �׵θ� ���� �β�

	strGraph2D_Axis_Info strAxisInfoX;		//! X�� ����
	CString		szCaptionY;					//! Y�� �̸�, E_GRAPH2D_XY_CAPTION_POS_ONE_Y������ ���

	bool		bAutoDivisionLength;		//! true�̸� iDivisionLength���� �ڵ� ����
	int			iDivisionLength;			//! Grid���� ���� ǥ���ϴ� �κ��� ���� ����

	strGraph2DTitleInfo strMainTitleInfo;	//! �� ���� ����
	strGraph2DTitleInfo strSubTitleInfo;	//! ���� ���� ����
	
	E_GRAPH2D_XY_CAPTION_POSITION eXYCaptionPosition;//! X-Y ǥ�� ���ڿ� ǥ�� ���
	bool		bYCaptionLeftPos;			//! eXYCaptionPosition�� E_GRAPH2D_XY_CAPTION_POS_UNITE, E_GRAPH2D_XY_CAPTION_POS_UNITE_Y�� ��,
											//! true�̸� Y�� ���ڿ��� ���ʿ� ��ġ

	strGraph2DResourceInfo()
	{
		InitInfo();
	}

	void InitInfo()
	{
		clrEntireBackGround = RGB(231, 231, 231);
		clrGraphBackGround = RGB(255, 255, 255);
		clrAxis = RGB(0, 0, 0);
		clrGrid = RGB(0, 0, 0);		
		clrGridText = RGB(0, 0, 0);
		clrCursorLine = RGB(255, 0, 255);
		clrCursorSelectedLine = RGB(255, 0, 0);
		clrCursorPoint = RGB(255, 0, 0);
		clrCursorText = RGB(0, 0, 0);
		clrCursorTextBorder = RGB(0, 0, 0);
		clrCursorTextBack = RGB(255, 255, 128);
		clrLegend = RGB(0, 0, 0);
		clrZoomArea = RGB(255, 128, 128);

		bGridFontHeight = true;
		bCursorFontHeight = true;
		bLegendFontHeight = true;
		
		iGridFontHeight = -10;
		iCursorFontHeight = -10;
		iLegendFontHeight = -10;
		
		szGridFontName = _T("MS Sans Serif");
		szCursorFontName = _T("MS Sans Serif");
		szLegendFontName = _T("MS Sans Serif");
		
		iAxisThickness = 1;
		iGridThickness = 1;
		iCursorLineThickness = 1;
		iCursorTextBorderThickness = 1;

		strAxisInfoX.InitInfo();
		strAxisInfoX.iGridTextPrecision = 1;
		strAxisInfoX.iCursorTextPrecision = 1;

		szCaptionY = _T("");

		bAutoDivisionLength = true;
		iDivisionLength = 4;

		strMainTitleInfo.InitInfo();
		strSubTitleInfo.InitInfo();
		
		eXYCaptionPosition = E_GRAPH2D_XY_CAPTION_POS_NONE;
		bYCaptionLeftPos = true;
	}

	strGraph2DResourceInfo& operator=(const strGraph2DResourceInfo &input)
	{
		clrEntireBackGround = input.clrEntireBackGround;
		clrGraphBackGround = input.clrGraphBackGround;
		clrAxis = input.clrAxis;
		clrGrid = input.clrGrid;		
		clrGridText = input.clrGridText;
		clrCursorLine = input.clrCursorLine;
		clrCursorSelectedLine = input.clrCursorSelectedLine;
		clrCursorPoint = input.clrCursorPoint;
		clrCursorText = input.clrCursorText;
		clrCursorTextBorder = input.clrCursorTextBorder;
		clrCursorTextBack = input.clrCursorTextBack;
		clrLegend = input.clrLegend;
		clrZoomArea = input.clrZoomArea;

		bGridFontHeight = input.bGridFontHeight;
		bCursorFontHeight = input.bCursorFontHeight;
		bLegendFontHeight = input.bLegendFontHeight;
		
		iGridFontHeight = input.iGridFontHeight;
		iCursorFontHeight = input.iCursorFontHeight;
		iLegendFontHeight = input.iLegendFontHeight;
		
		szGridFontName = input.szGridFontName;
		szCursorFontName = input.szCursorFontName;
		szLegendFontName = input.szLegendFontName;
		
		iAxisThickness = input.iAxisThickness;
		iGridThickness = input.iGridThickness;
		iCursorLineThickness = input.iCursorLineThickness;
		iCursorTextBorderThickness = input.iCursorTextBorderThickness;

		strAxisInfoX = input.strAxisInfoX;

		szCaptionY = input.szCaptionY;

		bAutoDivisionLength = input.bAutoDivisionLength;
		iDivisionLength = input.iDivisionLength;

		strMainTitleInfo = input.strMainTitleInfo;
		strSubTitleInfo = input.strSubTitleInfo;
			
		eXYCaptionPosition = input.eXYCaptionPosition;
		bYCaptionLeftPos = input.bYCaptionLeftPos;

		return *this;
	}
};

//! Added by LHW [2008/8/22] : �Լ������� �׷����� �׸� ��� �ʿ��� ������ ����ü �߰�
//! Modify by LHW [2008/8/23] : bShowGraphChk, iOverlapIndex �߰�
//! Modify by LHW [2008/8/26] : ���� ������, bShowChk �߰�
//! Modify by LHW [2008/9/1] : dbInputOffsetX, dbOutputOffsetY, bUseExp �߰�
struct strGraph2DEquationInfo
{
	//! iCoeffSize�� 1�̸� Y�� ������ ���� (y = a),
	//! iCoeffSize�� 2�̸� 1�� �׷��� (y = ax + b), 
	//! iCoeffSize�� 3�̸� 2�� �׷��� (y = ax^2 + bx + c) ������ �׷����� �׷���
	//! a, b, c ���� ��� �Լ��� �����
	
	int iCoeffSize;			//! �Լ��� ����� ����
	double *pdbCoeff;		//! �Լ��� ���

	double dbStartRangeX;	//! �Լ������� �׸��� �׷����� X�� ���� ����
	double dbEndRangeX;		//! �Լ������� �׸��� �׷����� X�� ��ħ ����

	double dbInputOffsetX;	//! X �������� Offset ��
	double dbOutputOffsetY;	//! Y ������� Offset ��

	int iOverlapIndex;		//! �� �׷����� ����� Main �׷����� index
	int iCustomData;		//! ����� ������
	double dbCustomData;	//! ����� ������

	bool bUseExp;			//! ���� �Լ�(�Լ��� ����) ���, �Լ��� ����� ������ �ݵ�� 2���̾�� ��

	E_GRAPH2D_LINE_TYPE eLineType;	//! �׷��� �� ����
	COLORREF clrLine;				//! �׷��� �� ����
	int iLineWidth;					//! �׷��� ���� ����, �Ǽ��� ��쿡�� 2�̻��� ���� ���� ����
	bool bShow;						//! true�̸� �׷����� ���̰� ��
	bool bShowChk;					//! true�̸� bShow�� false�̸� Auto Range ���� �������� ���ܽ�Ŵ
	bool bShowGraphChk;				//! true�̸� bShow�� true�̴���, ¦�� �Ǵ� �׷����� ������ ������ ������ �ʰ� ��

	strGraph2DEquationInfo()
	{
		InitInfo();
	}

	~strGraph2DEquationInfo()
	{
		ReleaseCoeffBuf();
	}

	void InitInfo()
	{
		iCoeffSize = 0;
		pdbCoeff = NULL;
		dbStartRangeX = 0.;
		dbEndRangeX = 0.;
		dbInputOffsetX = 0.;
		dbOutputOffsetY = 0.;
		iOverlapIndex = -1;
		iCustomData = 0;
		dbCustomData = 0.;
		bUseExp = false;
		eLineType = E_GRAPH2D_LINE_TYPE_SOLID;
		clrLine = RGB(0, 0, 255);
		iLineWidth = 1;
		bShow = true;
		bShowChk = true;
		bShowGraphChk = true;
	}

	bool MakeCoeffBuf(int iNewSize)
	{
		if ( iNewSize <= 0 )
		{
			return false;
		}

		ReleaseCoeffBuf();

		pdbCoeff = new double[iNewSize + 1];
		if ( pdbCoeff == NULL )
		{
			return false;
		}

		for ( int i = 0; i < (iNewSize + 1); i++ )
		{
			pdbCoeff[i] = 0.;
		}

		iCoeffSize = iNewSize;

		return true;
	}

	void ReleaseCoeffBuf()
	{
		if ( pdbCoeff != NULL )
		{
			delete [] pdbCoeff;
			pdbCoeff = NULL;
		}
		iCoeffSize = 0;
	}

	strGraph2DEquationInfo(const strGraph2DEquationInfo &input)	//! ���� ������
	{
		InitInfo();

		if ( input.iCoeffSize > 0 && input.pdbCoeff != NULL )
		{
			MakeCoeffBuf(input.iCoeffSize);
		}

		if ( iCoeffSize > 0 && pdbCoeff != NULL )
		{
			for ( int i = 0; i < iCoeffSize; i++ )
			{
				pdbCoeff[i] = input.pdbCoeff[i];
			}
		}

		dbStartRangeX = input.dbStartRangeX;
		dbEndRangeX = input.dbEndRangeX;
		dbInputOffsetX = input.dbInputOffsetX;
		dbOutputOffsetY = input.dbOutputOffsetY;

		iOverlapIndex = input.iOverlapIndex;
		iCustomData = input.iCustomData;
		dbCustomData = input.dbCustomData;

		bUseExp = input.bUseExp;

		eLineType = input.eLineType;
		clrLine = input.clrLine;
		iLineWidth = input.iLineWidth;
		bShow = input.bShow;
		bShowChk = input.bShowChk;
		bShowGraphChk = input.bShowGraphChk;
	}

	strGraph2DEquationInfo& operator=(const strGraph2DEquationInfo &input)
	{
		if ( (iCoeffSize != input.iCoeffSize) || pdbCoeff == NULL )
		{
			if ( input.iCoeffSize > 0 )
			{
				if ( MakeCoeffBuf(input.iCoeffSize) == false )
				{
					return *this;
				}				
			}
			else
			{
				ReleaseCoeffBuf();
			}
		}

		if ( iCoeffSize > 0 && pdbCoeff != NULL )
		{
			for ( int i = 0; i < iCoeffSize; i++ )
			{
				pdbCoeff[i] = input.pdbCoeff[i];
			}
		}

		dbStartRangeX = input.dbStartRangeX;
		dbEndRangeX = input.dbEndRangeX;
		dbInputOffsetX = input.dbInputOffsetX;
		dbOutputOffsetY = input.dbOutputOffsetY;

		iOverlapIndex = input.iOverlapIndex;
		iCustomData = input.iCustomData;
		dbCustomData = input.dbCustomData;

		bUseExp = input.bUseExp;

		eLineType = input.eLineType;
		clrLine = input.clrLine;
		iLineWidth = input.iLineWidth;
		bShow = input.bShow;
		bShowChk = input.bShowChk;
		bShowGraphChk = input.bShowGraphChk;

		return *this;
	}
};
