#pragma once

#include "../StdAfx.h"
//#include "AxlDefine.h"


//! AjinExTek Motion Controller Ŭ����
//! Created, 2013/4/10
//! Recently Modified, 2013/4/19

//! 1] ���������� Motion Controller�� ��ġ ��� �����Ѵ�. ������, AxmMoveVel ó�� ������ �ӵ��� �����̴� ����� �����Ѵ�. 
//! 2] RTEX ��ǰ (PCI-R1604)������ Interrupt, Software Limit�� �������� �ʴ´�. 
//! 3] ���� �������� �� �� �ִ� ����� ����. 
//! 4] PCI-R1604 A4N, A5N�� ��� ������� �Է´��� �������� �ʴ´�.
//!     (��� ��⿡ �ִ� ������� �Է´� ���� ��ȣ�� �ְ� �Ǹ� ��ȣ�� ������ ESTOP�� �ڵ����� ���� ��� ��ǿ� ���ؼ� ����)

//! [���� ����] AA Bonder Vision�� Homing���� �ⱸ ����
//!		1) ��� ������ ���� ��Ų �Ŀ� Homing�� �����ؾ� �Ѵ�. 
//!		2) Lens Z�� Motor�� �������� �ö� �Ŀ� �ٸ� Motor���� Homing�ؾ� �Ѵ�.
//!		3) PCB Y�� Motor�� (-) Limit ���� ������ �� ���Ŀ�, PCB X, Theta �� Motor�� Homing�ؾ� �Ѵ�. 
//!		4) PCB Theta�� Motor�� Homing�ϱ� ����, PCB Cover ����� �Ǹ����� ��½��Ѿ� �Ѵ�. 

//! ������ MMC Motion Controller�� �Լ����� �ִ��� ���� �Ͽ�, �ҽ� �ڵ��� �������� ���δ�. 


//! �Ʒ� ����ü�� Ư�� ��ġ (Limit ������ Home ����)���� �ڵ����� ���ߵ��� �Ͽ� ���͸� ������ �� ����Ѵ�.
struct strMove_Input_Signal_Search
{
	double dVel;
	double dAccel;

	//! ���� ��ȣ ���� (�Ʒ� 4���� ���� �� 1���� true�̾�� �Ѵ�)
	bool bEndLimit_Positive;
	bool bEndLimit_Negative;
	bool bHomeSensor;
	bool bPhase_Encoder_Z;		//! Encoder�� Z�� ��ȣ

	//! Edge ���� (�Ʒ� 2���� ���� �� 1���� true�̾�� �Ѵ�)
	bool bEdge_Down;
	bool bEdge_Up;

	//! ���� ��� (�Ʒ� 2���� ���� �� 1���� true�̾�� �Ѵ�)
	bool bEmergency;
	bool bSlowDown;

	strMove_Input_Signal_Search()
	{
		InitInfo();
	}

	void InitInfo()
	{
		dVel = 0.;
		dAccel = 0.;

		bEndLimit_Positive = false;
		bEndLimit_Negative = false;
		bHomeSensor = false;
		bPhase_Encoder_Z = false;

		bEdge_Down = false;
		bEdge_Up = false;

		bEmergency = false;
		bSlowDown = false;
	}

	strMove_Input_Signal_Search& operator=(const strMove_Input_Signal_Search &input)	//! '=' ������ overloading
	{
		dVel   = input.dVel;
		dAccel = input.dAccel;

		bEndLimit_Positive = input.bEndLimit_Positive;
		bEndLimit_Negative = input.bEndLimit_Negative;
		bHomeSensor        = input.bHomeSensor;
		bPhase_Encoder_Z   = input.bPhase_Encoder_Z;

		bEdge_Down = input.bEdge_Down;
		bEdge_Up   = input.bEdge_Up;

		bEmergency = input.bEmergency;
		bSlowDown  = input.bSlowDown;

		return *this;
	}
};//! struct strMove_Input_Signal_Search


class CAxlMotorSet
{
public:
	CAxlMotorSet(void);
	~CAxlMotorSet(void);

private:
	//! �ش� ���� ����� ���Ͽ� Motion Board�� �������� ���Ϸ� ���� ����
	CString m_sMotionSettingFilePath;



public:
	CString sMsg;
	bool m_bOrgFlag[MAX_MOTOR_NO];
	char m_szLogMsg[256];

	CCriticalSection m_criHomeProc;

	bool bAxdConnect;

	bool Axl_Init();
	int  Axl_Axisconfig();
	int  Axl_DIO_Config();
	bool Axl_DIO_Reset();

	void Axl_Close();


	bool InDIO(int moduleNo, int &inVal);
	bool OutDIO(int moduleNo, int outVal);
	bool OutDIO(int moduleNo, int chNo, int no);

	bool GetOutputIO(int moduleNo, int &outVal);


	bool AmpEnable(int iAxis);				//! ���� ���� Servo-On�� �ǽ�
	bool AmpEnableAll();						
	bool AmpDisable(int iAxis);				//! ���� ���� Servo-Off�� �ǽ�
	bool AmpDisableAll();
	bool GetAmpEnable(int iAxis);			//! ���� ���� Servo-On ��ȣ�� ��� ���¸� ��ȯ(T=ON, F=OFF ����)
	bool GetAmpEnableAll();					//! AAB Unit�� Servo-Off ���°� ���� ��� false
	
	bool AmpFaultReset(int iAxis);			//! �˶� Reset�� Servo-On �ǽ�
	bool AmpFaultResetAll();
	int	 AmpFaultCheck();					//! �˶� ��ȣ Ȯ��

	bool GetAmpFault( int iAxis);			//! ���� ���� �˶� ���� ��ȯ
	
	bool GetInposition(int iAxis);			//! ���� ���� Motor�� ��ǥ ������ �����ߴ��� Ȯ��
	bool GetInpositionPos(int iAxis, double dPos);	//! ���� ���� Motor�� ��ǥ ������ �����ߴ��� Ȯ��( Encord������ Ȯ��)
	bool GetHomeSensor(int iAxis);			//! ���� ���� Home ���� ��ȣ�� �Է� Ȯ��
	bool GetNegaSensor(int iAxis);			//! ���� ���� (-)����Ʈ ���� ��ȣ�� �Է� Ȯ��
	bool GetPosiSensor(int iAxis);			//! ���� ���� (+)����Ʈ ���� ��ȣ�� �Է� Ȯ��

	bool ActCRC(int axis);

	bool IsStopAxis(int axis);				//! ���� ���� ���� ��ȯ(T=����, F=������)
	bool StopAxis(int axis);
	bool StopAxisAll();

	double GetEncoderPos(int iAxis);		//! Linear ���� ��ġ ��ǥ ��ȯ
	double GetEncoderPos_Disp(int iAxis);	//! Linear ���� ��ġ��ǥ���� ���� ���� ��ǥ���� ���� ��ǥ ��ȯ
	double GetCommandPos(int iAxis);		//! Steping ���� ��ġ ��ǥ ��ȯ
	double GetCommandPos_Disp(int iAxis);	//! Steping ���� ��ġ��ǥ���� ���� ���� ��ǥ���� ���� ��ǥ ��ȯ	
	double CalcAccFromAccTime(double dVel, double dTime);						//! ���ӵ� = �ӵ�/(���� �ð�)���� ���
	BOOL JogMove(int iAxis, double dVel);										//! ������ �ӵ��� �����Ѵ�.
	BOOL JogStop(int iAxis);													//! ������ ���� ������ ���������մϴ�.
	bool MoveAxis(int axis, bool AbSFlag, double dPos, double dVel, float iAcc);						//! ���� ���� ���� ���� �Ǵ� ��� �������� �̵��Ѵ�.
	bool MoveAxisMulti(int iAxisCnt, short iAxis[MAX_MOTOR_NO], double pos[MAX_MOTOR_NO], int iSpeed);	//! ���� ���� �࿡ ���ؼ� ������ ��ġ���� ������ �Ÿ���ŭ �̵��� ���ÿ� �����Ѵ�

	bool MoveAxis_To_Sensor(int axis, strMove_Input_Signal_Search *pstrInput);



	

	bool goMotorPos(int no, short axis[MAX_MOTOR_NO], double pos[MAX_MOTOR_NO], bool bAutoRun = true);

	bool goMotorZPos(int no, short axis[MAX_MOTOR_NO], double pos[MAX_MOTOR_NO], double dVel, bool bAutoRun = true);

	bool Move_From_GUI(int axis, int iID_GUI, CWnd* pParentWnd, bool bPlus = true);
		
	bool GetResult_Home_Search(int axis, CString &sResult);	//! Home �˻� �۾� ���� �� ��� ���ڿ��� �˾Ƴ���. 
	int IsCompleted_Home_Search(int axis);					//! Home�˻� ���� Ȯ��(0=Fail, 1=Success, 2=Searching)
	int  GetRate_Home_Search(int axis);						//! Home �˻� �۾� ���� ���� ���� ��ȯ, (���� : %) 

	void Stop_Home_Search(int iAxis);						//! Home �˻� �߿� ������ ���� ��Ŵ

	int  Start_Home_Search(int iAxis);						//! Home �˻� �۾� ����	
	DWORD SetHomeSearchSpeed(int iAxis);					//! ���� �˻��� ���Ǵ� �ӵ� ����	



	//! �Ʒ��� �Լ����� ���� ��񿡼� �ٽ� �����ؾ� ��
	int  HomeCheck();

	//! ���� ���	
	bool HomeProcess(int axis);		//! ���� ���� ����
	bool HomeProcessAll();			//! ��ü ���� ����.

	int HomeSubReqProcess(int iAxis, int iStep, double &checkTime);	//! ���� ���� ����.

	int HomeSubProcess(int axis, int iStep, double &iTime);		//! ���� ���� ����. - ������ ����.
	


	bool Lens_Motor_Conti_Down(int MotorAxis, bool waitFlag);//




	bool Lens_Motor_Move(int index, double offsetX=0, double offsetY=0);
	bool Lens_Motor_MoveXY(int index, double offsetX=0, double offsetY=0);
	bool Lens_Motor_Align_Move(double offsetX=0, double offsetY=0, double offsetTh=0);			/* Align Retry�� ���� �߰� */
	bool Lens_Tilt_Motor_Move(int index, double offsetX=0.0, double offsetY=0.0);
	bool Lens_Motor_Move_Laser(int Pos, double offsetX, double offsetY, double offsetTh);		// Lens Tilt�� ���� �߰�

	bool Pcb_Motor_Move(int index, double offsetX = 0, double offsetY = 0, double offsetTh = 0);

	bool Dispense_XY_Motor_Move(int index, double dVel, double offsetX = 0, double offsetY = 0, double offsetTh = 0);
	bool DispenseGoMotorPos(int no, short axis[MAX_MOTOR_NO], double pos[MAX_MOTOR_NO], double dVel);
	bool Dispense_Z_Motor_Move(int index, double dVel);

	
	bool Pcb_Motor_Align_Move(double offsetX=0, double offsetY=0, double offsetTh=0);			/* Align Retry�� ���� �߰� */

	bool Pcb_Holder_Align_Move(int index, double offsetX=0, double offsetY=0, double offsetTh=0);	//! Align �̵��ø� ��� �Ұ�
	bool Pcb_Tilt_Align_Move(double offsetX=0, double offsetY=0);
	bool Pcb_Tilt_Motor_Move(int index, double offsetX=0.0, double offsetY=0.0);
	
	bool Pcb_Tilt_Trinity_Move(double offsetX = 0, double offsetY = 0);

	bool Pcb_Motor_Move_Laser(int Pos, double offsetX=0, double offsetY=0, double offsetTh=0);
	bool Pcb_Move_Tilt(double offsetX=0, double offsetY=0);
	bool Lens_Move_Tilt(double offsetX=0, double offsetY=0);
	
	bool moveEpoxyCleanPos(int iIndex, bool waitFlag);

	bool LENS_Z_Motor_Move(int index , bool evasionMode = false);//evasion = ȸ�Ǹ��
	bool LENS_Z_Motor_Move_Do(int index);
	bool LENS_Z_Motor_Move_Check(int index);
	bool PCB_Z_Motor_Move(int index);
	bool PCB_Z_V_Motor_Move(int index,double dVel);

	bool checkLensMotorPos(int index);
	bool checkPcbMotorPos(int index);

	bool Pcb_Motor_CircleMove(double dAngle, bool bWait = true, int Pos = 0);	//��ȣ ���� ����(PCB X/Y��)
	bool Pcb_Motor_Twister_Move(double dAngle, bool bWait = true, int Pos = 0);	//��ȣ ���� ����(PCB X/Y��)
	bool func_Epoxy_CircleDraw(bool CONTI = false);	//Dispenser ���� ���� �ϸ� ��ȣ ���� ����  , conti�� �������׸����� true
	bool func_Epoxy_Draw();	//Dispenser ���� ���� �ϸ� ��ũ ���� ����
	bool func_Epoxy_Point_Draw();
	bool func_Epoxy_Rect_Circle_Draw();		//sony �� ������ ���� 


	bool func_New_Epoxy_Rect_Arck_Draw();


	bool func_Trinity_Epoxy_Rect_Draw();		//�޸� �ܰ� ��
	bool func_Trinity_Epoxy_Circle_Draw();		//�޸� ���� 3����Ʈ

	bool func_Epoxy_Right_StraightDraw();	//20150603 ����I/O�� 2���� Left, Right ����
	bool func_Epoxy_Left_StraightDraw();	//20150603 ����I/O�� 2���� Left, Right ����
	bool Straight_Dispense_MoveAxis(int axis, bool AbSFlag, double dPos, double dVel, float iAcc); //20150603 ���� ������ �� Y�� �����̴� �Լ�

	bool Continue_Line_Arck_Dispense(); 	//���� + ��ũ  ���� ������� ����

	bool moveSensorContactPos(bool waitFlag);
	bool checkOriginStatus(int iCnt, short* axis);		/* ���� ���� ���� Ȯ�� */
	bool moveWaitPos_Z(bool waitFlag);

	bool movePointDischargePos(bool waitFlag);		
	bool movePointDischarge2Pos(bool waitFlag);		
	bool moveDispensePos_Z(int iPcbNo, bool waitFlag);
	bool movePointDischargePos_Z(bool waitFlag);	
	bool moveDispense2Pos_Z(int iPcbNo, bool waitFlag);
	bool movePointDischarge2Pos_Z(bool waitFlag);



	//M_TRINITY �� ���� ����
	bool Trinityt_Dispense_MoveAxis();

};//! class CAxlMotorSet

