#pragma once

#include "msflexgrid.h"
#include "Label/Label.h"
#include "Library/AutoRepeat/AutoRepeatButton.h"

// CResinDlg ��ȭ �����Դϴ�.

class CResinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResinDlg)

public:
	CResinDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CResinDlg();

	int m_iImageIndex;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_RESIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	bool	timerRunFlag;

	void	setInterface();
	void	dispAreaPos();
	void	dispData();
	void	ctrlLiveButton();
	void	initResinGrid();
	void	initMotorGrid();
	void	changeMotorNo(int motorNo);
	void	dispMotorStatus();
	void	CheckJogMotorSpeed();

	void	changeColorDischargeBtn();			/* ���� ��ư ���� �ٲٱ� */
	int		checkSyringeMotorPos(int iIndex);	/* Syring ���� ��ġ Ȯ�� */
	void	changeColorSyringeBtn();			/* Syring ��ư ���� �ٲٱ� */
	void	changeColorMotorPosBtn();			/* ���� ��ġ Ȯ�� */
	bool	checkCurMotorPos(int iPos);			/* ���� ��ġ Ȯ�� */
	void	checkResinpara();

	int		dischargeEpoxy();				/* Epoxy �� ��� */
	int		findEpoxyPoint();				/* Epoxy ��ġ ã�� */
	int		calcOffset_Epoxy();				/* Epoxy ���� ��ġ offset ��� */
	void	putEpoxyCalcData();				/* Epoxy ��ġ ���� ������ġ�� offset �Ÿ� �ֱ� */
	void	getEpoxyCalcData();				/* Epoxy ��ġ ���� ������ġ�� offset �Ÿ� ��� */
	void    showEpoxyCalcControl();			/* Epoxy ��ġ ���� ��Ʈ�ѷ��� ���̱�/����� */
	void	drawArea_EpoxyCalc();			/* Epoxy ��ġ ��� �� ã�� ���� �׸��� */

	int		m_iBase_X;
	int		m_iBase_Y;
	double	m_dOffset_X;
	double	m_dOffset_Y;
	CRect	m_rcArea_EpoxyCalc;

	MIL_ID  MilOriginImage;
	MIL_ID	MilOriginImageChild[3];
	MIL_ID	MilBinaryImage;
	MIL_ID  MilBinaryImageChild[3];
	MIL_ID	MilBlobResult;
	MIL_ID	MilBlobFeatureList;

	long	m_iTotalBlobs;

	double	m_dCogX;
	double	m_dCogY;

	bool	m_bSelArea[4];

	CRect	m_rArea[4];
	CRect	m_rcFixedBox;

	CLabel	m_labelMotorName;

	int		m_iSelMotor;
	int		m_iMoveSpeed;

	CMSFlexGrid	m_gridMotor;
	CMSFlexGrid m_gridResin;
	double  m_fMotorJogSpeed;
	bool OLD_JogPlusflag;
	bool OLD_JogMinusflag;

	int DrowBox;

public:
	int m_iCamNo;
	BOOL m_bCalc_Epoxy_Pos;
	int m_iSelType_EpoxyCalc;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CSliderCtrl m_sliderLed;
	afx_msg void OnBnClickedButtonParaSave();
	afx_msg void OnBnClickedButtonBoxUp();
	afx_msg void OnBnClickedButtonBoxDown();
	afx_msg void OnBnClickedButtonBoxLeft();
	afx_msg void OnBnClickedButtonBoxRight();
	afx_msg void OnBnClickedButtonBoxSpeed();
	afx_msg void OnBnClickedRadioPos();
	afx_msg void OnBnClickedRadioSize();
	int m_iSelArea;
	afx_msg void OnBnClickedRadioSelarea1();
	afx_msg void OnBnClickedRadioSelarea2();
	afx_msg void OnBnClickedRadioSelarea3();
	afx_msg void OnBnClickedRadioSelarea4();
	int m_iSelMoveType;
	afx_msg void OnBnClickedButtonLivemode();
	afx_msg void OnBnClickedButtonImgLoad();
	afx_msg void OnBnClickedButtonImgSave();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonMotorSave();
	DECLARE_EVENTSINK_MAP()
	void DblClickMsflexgridMotor();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonResinInsp();
	afx_msg void OnBnClickedButtonMotorAlign();
	afx_msg void OnBnClickedButtonMotorEmission();
	afx_msg void OnBnClickedButtonMotorSpeed();
	afx_msg void OnBnClickedButtonSyringeOn();
	afx_msg void OnBnClickedButtonSyringeOff();
	afx_msg void OnBnClickedButtonDischargeOn();
	afx_msg void OnBnClickedButtonDischargeOff();
	afx_msg void OnBnClickedButtonPlusX();
	afx_msg void OnBnClickedButtonMinusX();
	afx_msg void OnBnClickedButtonResinDraw();
	void DblClickMsflexgridResin();
	CAutoRepeatButton m_JogMinus;
	CAutoRepeatButton m_JogPlus;
	CExButton m_btnAlignPos;
	CExButton m_btnEmissionPos;
	CExButton m_btnInspPos;
	CExButton m_btnSyringeOff;
	CExButton m_btnSyringeOn;
	CExButton m_btnDischargeOff;
	CExButton m_btnDischargeOn;
	CExButton m_btnEpoxyCalPos;
	CExButton m_btnEpoxyCalInspPos;
	afx_msg void OnBnClickedRadioPcbAlign();
	afx_msg void OnBnClickedRadioEpoxy();
//	int m_bResinPara;
	BOOL m_bResinPara;
	afx_msg void OnBnClickedCheckResinDraw();
//	CExButton m_btnResinInsptect;
	afx_msg void OnBnClickedButtonResinInspect();
	afx_msg void OnBnClickedButtonEpoxyCal();
	afx_msg void OnBnClickedButtonEpoxyCalInsp();
	afx_msg void OnBnClickedCheckResinInspDisp();
	afx_msg void OnBnClickedButtonEpoxyCalc2();
	afx_msg void OnBnClickedCheckEpoxyCalc();
	BOOL m_bResinInspDIsp;
	afx_msg void OnBnClickedRadioSelTypeEpoxyCalc();
	afx_msg void OnBnClickedRadioSelTypeEpoxyCalc2();
	afx_msg void OnBnClickedButtonFindEpoxyPoint();
};
