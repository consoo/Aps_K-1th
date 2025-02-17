#pragma once

#include "Label/Label.h"
//! ===============================================
//! Added by LHW (2013/2/22)

//! �׷��� ��Ʈ�� Ŭ���� ����
class CGraph2DWnd;

//! �׷��� ��Ʈ���� ����
// const int iBufSize_Graph_2D = 2;
// 
// //! 1���� �׷��� ��Ʈ�ѿ� �׷����� �׷����� ����
// const int iBufSize_Graph_Overlap = 5;
// 
// //! 1���� �׷����� �̷�� �׷��� ������ ������ ũ��
// const int iBufSize_Pixel_Graph = 8192;

//! X�� �������� ���� index
// const int iIndex_Max_Center		= 0;
// const int iIndex_Max_UpperLeft	= 1;
// const int iIndex_Max_UpperRight = 2;
// const int iIndex_Max_LowerLeft	= 3;
// const int iIndex_Max_LowerRight = 4;

// const int iIndex_MTF4  = 0;	//! MTF(N/4)
// const int iIndex_MTF8  = 1;	//! MTF(N/8)
// const int iIndex_MTF50 = 2;	//! MTF50

#define BUFFER_SIZE_GRAPH_2D		3
#define BUFFER_SIZE_GRAPH_OVERLAP	5
#define BUFFER_SIZE_PIXEL_GRAPH		8192

#define INDEX_MTF4	0
#define INDEX_MTF8	1
#define INDEX_MTF50	2

// CAutoDispDlg ��ȭ �����Դϴ�.

class CAutoDispDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoDispDlg)

public:
	CAutoDispDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAutoDispDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_AUTODISP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

protected:
	void	setInterface();

	//! �׷��� ��Ʈ�� �迭
	CGraph2DWnd *m_apGraphWnd[2];	
	bool MakeGraphWnd();
	void ReleaseGraphWnd();

	//! �׷��� Ŀ���� ������ �ִ� �׷����� index, 0 based
	int m_aiIndex_Overlap_Cursor[2];

	//! ���α׷� �ٿ� ������, �׷��� ��Ʈ�� ���� ���� ����
	CCriticalSection m_acsGraph[2];	

	//! �׷����� ���� ���̰ų� ���� ��
	void Proc_GraphVisible(int iIndex_Plot, int iIndex_Overlap);
	bool SetGraphVisible(int iIndex_Plot, int iIndex_Overlap, bool bFlag_Visible);
	int UpdateCursorValue(int iIndex_Plot);
	//! ���� �Ⱥ��̴� �׷��� Ŀ�� �����ϰ�, ������ ��ġ�� ���� ���̴� �׷����� Ŀ�� �߰�
	int ExChangeCursor(int iIndex_Plot);

	//! Ŀ���� �����̰� �� ��
	void MoveCursor_Left_Graph(int iIndex_Plot);
	void MoveCursor_Right_Graph(int iIndex_Plot);

	//! �׷����� Ŀ�� �̺�Ʈ�� ���� ��
	LRESULT OnCursorEventMsgInPlot(WPARAM wParam, LPARAM lParam);	

	//! GUI ó�� �Լ�
	void SetGUI_Graph_Visible_Chk();

	//! �ӽ� ������ ����
	int RangedRandDemo( int range_min, int range_max, int n );
	void SetTempData(int iIndex_Plot);

	//! �׷����� �׸���, �׷��� Ŀ������ ǥ���Ѵ�.
	int SetCursorValue(int iIndex_Plot, int iIndex_Overlap, int iDataIndex, int iCursorIndex = 0);

	

public:
	//! =========================================================================================================
	//! �׷��� �Է� ������ ����	
	double m_adDataX        [BUFFER_SIZE_GRAPH_2D][BUFFER_SIZE_GRAPH_OVERLAP][BUFFER_SIZE_PIXEL_GRAPH];
	double m_adDataY        [BUFFER_SIZE_GRAPH_2D][BUFFER_SIZE_GRAPH_OVERLAP][BUFFER_SIZE_PIXEL_GRAPH];
	int    m_aiDataSize     [BUFFER_SIZE_GRAPH_2D][BUFFER_SIZE_GRAPH_OVERLAP];

	//! �׷��� ���� Ư�� ������ X�࿡ �������� ǥ���Ѵ�.
	double m_adData_MTF4    [BUFFER_SIZE_GRAPH_2D][BUFFER_SIZE_GRAPH_OVERLAP];	//! MTF(N/4) X�� ��
	double m_adData_MTF8    [BUFFER_SIZE_GRAPH_2D][BUFFER_SIZE_GRAPH_OVERLAP];	//! MTF(N/8) X�� ��
	double m_adData_MTF50   [BUFFER_SIZE_GRAPH_2D][BUFFER_SIZE_GRAPH_OVERLAP];	//! MTF50	 X�� ��

	bool   m_abFlag_Visible [BUFFER_SIZE_GRAPH_2D][BUFFER_SIZE_GRAPH_OVERLAP];	//! true�̸� �׷����� ���� ���̰� �Ѵ�.
	int    m_aiCount_Visible[BUFFER_SIZE_GRAPH_2D];							//! ���� ���̴� �׷����� ����

	CLabel	m_label_Cur_Task;
	CLabel	m_label_Graph1;
	CLabel	m_label_Graph2;
	CLabel	m_label_Graph3;
	CLabel  m_label_Graph5;
	CLabel	m_label_Disp_Opt;
	CLabel	m_label_Disp_Defect;

	int		m_iCur_Index_Through;

	//! ���� �ʱ�ȭ
	bool Init_Buffser(int iIndex_Plot, int iIndex_Overlap);

	int DrawGraph(int iIndex_Plot);				/* iMode 0 : ������� 1 : �ڵ���� */

	void	dispCurTaskOnLabel(int iRtn, CString sLabel);

	//! �׷����� Ŀ���� �߰��Ѵ�. 
	void AddCursor_In_Graph();

	//! =========================================================================================================

	void	DispResultImage_Opt();
	void	DispResultImage_Defect();
	void	DrawBarGraph();

	int		m_iSizeX_Client;
	int		m_iSizeY_Client;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL DestroyWindow();
	afx_msg void OnPaint();
};
