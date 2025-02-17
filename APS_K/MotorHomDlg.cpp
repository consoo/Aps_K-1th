// MotorHomDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AA Bonder.h"
#include "MotorHomDlg.h"
#include "afxdialogex.h"
#include "MotorSet.h"
#include "DioControl.h"


extern		CMotorSet	motor;
extern bool g_bMovingflag;
extern	CDioControl		Dio;

// CMotorHomDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMotorHomDlg, CDialogEx)

	CMotorHomDlg::CMotorHomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMotorHomDlg::IDD, pParent)
{

}

CMotorHomDlg::~CMotorHomDlg()
{
}

void CMotorHomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_MSFLEXGRID_MOTOR, m_gridMotor);
}


BEGIN_MESSAGE_MAP(CMotorHomDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SERVO_ON_ALL, &CMotorHomDlg::OnBnClickedButtonServoOnAll)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CMotorHomDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_SERVO_OFF_ALL, &CMotorHomDlg::OnBnClickedButtonServoOffAll)
	ON_BN_CLICKED(IDC_BUTTON_HOME, &CMotorHomDlg::OnBnClickedButtonHome)
	ON_BN_CLICKED(IDC_BUTTON_HOME_STOP, &CMotorHomDlg::OnBnClickedButtonHomeStop)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CMotorHomDlg::OnBnClickedButtonReset)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MOTION_INIT, &CMotorHomDlg::OnBnClickedButtonMotionInit)
END_MESSAGE_MAP()


// CMotorHomDlg �޽��� ó�����Դϴ�.


void CMotorHomDlg::OnBnClickedButtonReset()
{
	/* �̵����̸� Return */
	if(g_bMovingflag)
		return;
	g_bMovingflag =true;

	char dispStr[100];
	sprintf_s(dispStr, "���� Drive�� Alarm�� ���� �մϴ�.");
	delayMsg(dispStr, 5000, M_COLOR_BLUE);

	if(!motor.AmpFaultResetAll())
	{
		errMsg("Amp Fault�� ���������� Reset���� �ʾҽ��ϴ�");

		delayMsg(dispStr, -1, M_COLOR_GREEN);
	}
	else
	{
		sprintf_s(dispStr, "���� Drive�� Alarm ����.");
		delayMsg(dispStr, 1500, M_COLOR_GREEN);
	}

	g_bMovingflag =false;
}


void CMotorHomDlg::OnBnClickedButtonExit()
{
	/* �̵����̸� Return */
	if(g_bMovingflag)
		return;
	g_bMovingflag =true;
	KillTimer(HOME_TIMER);

	g_bMovingflag =false;

	CDialogEx::OnOK();
}


BOOL CMotorHomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	setInterface();

	timerRunFlag = false;
	SetTimer(HOME_TIMER, 200, NULL);

	return TRUE;
}

void CMotorHomDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == HOME_TIMER)
	{
		if (!timerRunFlag)
		{
			timerRunFlag = true;

			dispMotorStatus();

			timerRunFlag = false;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CMotorHomDlg::dispMotorStatus()
{
	CString str;
	int		status;
	double	motorPos;

	m_gridMotor.SetRedraw(FALSE);

	for(int i=0; i<MAX_MOTOR_NO; i++)
	{
		status	= motor.GetAxisStatus(i);
		motorPos	= motor.GetCurrentPos(i);


		str.Format("%.03f", motorPos);
		m_gridMotor.SetTextMatrix(i+1, 1, str);										// ���� ��ǥ..


		m_gridMotor.SetRow(i+1);

		m_gridMotor.SetCol(2);														// ���� ����
		if(motor.m_bOrgFlag[i])						m_gridMotor.SetCellBackColor(GRID_COLOR_GREEN);
		else										m_gridMotor.SetCellBackColor(GRID_COLOR_RED);

		//		m_gridMotor.SetCol(3);														// Servo On
		//		if(status[i] & ST_INPOSITION_STATUS)		m_gridMotor.SetCellBackColor(GRID_COLOR_GREEN);
		//		else										m_gridMotor.SetCellBackColor(0);

		m_gridMotor.SetCol(3);														// In-Position
		if(motor.GetInposition(i))					m_gridMotor.SetCellBackColor(GRID_COLOR_GREEN);
		else										m_gridMotor.SetCellBackColor(0);


		m_gridMotor.SetCol(4);														// (-) ����
		if(status & ST_NEG_LIMIT)					m_gridMotor.SetCellBackColor(GRID_COLOR_RED);
		else										m_gridMotor.SetCellBackColor(0);

		m_gridMotor.SetCol(5);														// HOM ����
		if(status & ST_HOME_SWITCH)					m_gridMotor.SetCellBackColor(GRID_COLOR_GREEN);
		else										m_gridMotor.SetCellBackColor(0);

		m_gridMotor.SetCol(6);														// (+) ����
		if(status & ST_POS_LIMIT)					m_gridMotor.SetCellBackColor(GRID_COLOR_RED);
		else										m_gridMotor.SetCellBackColor(0);

		m_gridMotor.SetCol(7);														// Servo Alarm
		if(status & ST_AMP_FAULT)
			m_gridMotor.SetCellBackColor(GRID_COLOR_RED);
		else
			m_gridMotor.SetCellBackColor(0);
	}

	m_gridMotor.SetRedraw(TRUE); 
	m_gridMotor.Refresh();
}

void CMotorHomDlg::setInterface()
{
	CString tmpStr;

	m_gridMotor.SetRedraw(false); 

	m_gridMotor.SetRows(MAX_MOTOR_NO+1);
	m_gridMotor.SetCols(8);

	for(int i=0;i<MAX_MOTOR_NO+1;i++)
	{
		m_gridMotor.SetRowHeight(i, 450);
	}

	for(int i=0;i<8;i++)
		m_gridMotor.SetColAlignment(i, 4);

	int width = 870;
	m_gridMotor.SetColWidth(0,1600);
	m_gridMotor.SetColWidth(1,1600);
	m_gridMotor.SetColWidth(2,width);
	m_gridMotor.SetColWidth(3,width);
	m_gridMotor.SetColWidth(4,width);
	m_gridMotor.SetColWidth(5,width);
	m_gridMotor.SetColWidth(6,width);
	m_gridMotor.SetColWidth(7,width);

	m_gridMotor.SetTextMatrix(0, 0, "���͸�");
	m_gridMotor.SetTextMatrix(0, 1, "���� ��ġ");
	m_gridMotor.SetTextMatrix(0, 2, "���� Ȯ��");
	m_gridMotor.SetTextMatrix(0, 3, "In-Pos");
	m_gridMotor.SetTextMatrix(0, 4, "Limit (-)");
	m_gridMotor.SetTextMatrix(0, 5, "HOME");
	m_gridMotor.SetTextMatrix(0, 6, "Limit (+)");
	m_gridMotor.SetTextMatrix(0, 7, "Alram");

	for(int i=0; i<MAX_MOTOR_NO; i++)
	{
		m_gridMotor.SetTextMatrix(i+1, 0, MotorName[i]);
	}


	m_gridMotor.SetRedraw(TRUE); 
	m_gridMotor.Refresh();
}


void CMotorHomDlg::OnBnClickedButtonMotionInit()
{
	char dispStr[100];
	sprintf_s(dispStr, "��� ���带 �ʱ�ȭ �մϴ�.");
	delayMsg(dispStr, 5000, M_COLOR_BLUE);


	motor.MMC_Close();

	if(!motor.MMC_Init(dispStr))
	{
		errMsg(dispStr);

		char logStr[1000];
		sprintf_s(logStr, "���� �ʱ�ȭ�� ���� �Ͽ����ϴ�. 2");
		errMsg(logStr);

	}
	else
	{
		sprintf_s(dispStr, "��� ���带 �ʱ�ȭ ����.");
		delayMsg(dispStr, 1000, M_COLOR_GREEN);
	}
}

void CMotorHomDlg::OnBnClickedButtonServoOnAll()
{
	/* �̵����̸� Return */
	if(g_bMovingflag)
		return;
	g_bMovingflag =true;

	char dispStr[100];
	sprintf_s(dispStr, "���͸� ��ü Servo ON ���ϴ�.");
	delayMsg(dispStr, 5000, M_COLOR_BLUE);

	if(!motor.AmpEnableAll())
	{
		errMsg("Servo On ���� �̻�");
	}
	else
	{
		sprintf_s(dispStr, "���͸� ��ü Servo ON.");
		delayMsg(dispStr, 1500, M_COLOR_GREEN);
	}

	g_bMovingflag =false;
}

void CMotorHomDlg::OnBnClickedButtonServoOffAll()
{
	/* �̵����̸� Return */
	if(g_bMovingflag)
		return;
	g_bMovingflag =true;

	char dispStr[100];
	sprintf_s(dispStr, "���͸� ��ü Servo OFF.");
	delayMsg(dispStr, 5000, M_COLOR_BLUE);

	if(!motor.AmpDisableAll())
	{
		errMsg("Servo Off ���� �̻�");
	}
	else
	{
		sprintf_s(dispStr, "���͸� ��ü Servo OFF.");
		delayMsg(dispStr, 1500, M_COLOR_GREEN);
	}

	g_bMovingflag =false;
}


void CMotorHomDlg::OnBnClickedButtonHome()
{
	/* �̵����̸� Return */
	if(g_bMovingflag)
		return;
	g_bMovingflag =true;

	if (askMsg("��ü ���� ���� ������ ���� �Ͻðڽ��ϱ�?"))
	{
		Dio.setLedLight(1);
		delayMsg("��ü ���͸� ���� ���� �մϴ�.", 500000, M_COLOR_DARK_GREEN);
		motor.HomeProcessAll();
		delayMsg("��ü ���� ���� ���͸� �Ϸ�.", 50000, M_COLOR_GREEN);
	}

	g_bMovingflag =false;
}


void CMotorHomDlg::OnBnClickedButtonHomeStop()
{
	/* �̵����̸� Return */
	if(g_bMovingflag)
		return;
	g_bMovingflag =true;

	char dispStr[100];
	sprintf_s(dispStr, "���� ���� ��.");
	delayMsg(dispStr, 5000, M_COLOR_BLUE);

	if(!motor.StopAxisAll())
	{
		errMsg("���� ���� ���� �ð� �ʰ� [1sec]");
	}
	else
	{
		sprintf_s(dispStr, "���� ����.");
		delayMsg(dispStr, 1500, M_COLOR_GREEN);
	}

	g_bMovingflag =false;
}
