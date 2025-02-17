#pragma once

//! �Ϻ� HAMAMATSU ���� UV Spot Light Source ����� LIGHTNINGCURE LC8 (L9566/L9588)����� RS-232C ��� ����� Ŭ����
//!  (1.LC8 Instruction Manual_7.pdf ������ 32, 33 ������, 4.LC8_cnt Instruction Manual(English)_1.pdf ���� ����)
//!  (�Ŵ����� ���뿡 ���� Command ���ڿ� ����/�ؼ� �۾��� �ô´�.)
//! Made by LHW (2013/4/29)
//! ������ �۾� ��¥ : 2013/5/7

//! [���� ����]
//!   UV Lamp�� RS-232C Command�� ����ġ ���� ���, 'ERR' ���ڿ��� PC�� ������.

//! RS-232C ��� Ŭ������ �����´�. (�θ� Ŭ����)
//! �θ� Ŭ������ �Լ�, public ��� ������ �ٷ� ��� ����
#include "../../Library/RS232C/Handler_RS232C.h"

//! ��񿡼��� STX, ETX ���̴�. (�Ŵ����� 'Delimiter' ���ڸ� �̿��Ѵ�.)
#define UV_STX	0x0D
#define UV_ETX	0x0D

const int iBufSize_Cure_Program = 9;
const int iBufSize_Step_In_Cure = 7;

//! Memory Cure Program ���� ���� ����ü
struct sreSetupInfo_UV_Program
{
	//int iNo_Program;			//! Program number (1 ~ 9)
	int iModel_Shutter;			//! Shutter mode (1 : mode 1, 2 : mode 2)

	int iUnit_Output_Intensity; //! Intensity Mode (Unit) (1 : %, 2 : W)
								
	double adIntensity[iBufSize_Step_In_Cure];		//! Output Intensity
	double adTime     [iBufSize_Step_In_Cure];		//! Shutter Time, ���� sec

	sreSetupInfo_UV_Program()
	{
		InitInfo();
	}

	void InitInfo()
	{
		//iNo_Program = 0;
		iModel_Shutter = 1;
		iUnit_Output_Intensity = 1;

		int i = 0;
		for ( i = 0; i < iBufSize_Step_In_Cure; i++ )
		{
			adIntensity[i] = 0.;
			adTime[i] = 0.;
		}
	}

	//! �̻��� ������ ������ ��ȯ, �̻��� ������ '1'�� ��ȯ
	int IsValid()
	{
		if ( (iModel_Shutter != 1) && (iModel_Shutter != 2) )
		{
			return -1;
		}
		if ( (iUnit_Output_Intensity != 1) && (iUnit_Output_Intensity != 2) )
		{
			return -2;
		}

		int iFailed = -1;
		int i = 0;
		for ( i = 0; i < iBufSize_Step_In_Cure; i++ )
		{
			if ( adIntensity[i] < 0. )
			{
				iFailed = i;
			}
			if ( adTime[i] < 0. )
			{
				iFailed = i;
			}
		}
		if ( iFailed >= 0 )
		{
			return (-100 - iFailed);
		}

		return 1;
	}
	
	sreSetupInfo_UV_Program& operator=(const sreSetupInfo_UV_Program &input)	//! ������ ������
	{
		//iNo_Program            = input.iNo_Program;
		iModel_Shutter         = input.iModel_Shutter;
		iUnit_Output_Intensity = input.iUnit_Output_Intensity;

		int i = 0;
		for ( i = 0; i < iBufSize_Step_In_Cure; i++ )
		{
			adIntensity[i] = input.adIntensity[i];
			adTime[i]      = input.adTime[i];
		}

		return *this;
	}
};

const int iBufSize_INP_Bit = 16;

class CUVCommand : public CHandler_RS232C
{
public:
	CUVCommand(void);
	~CUVCommand(void);

	void ReadyDevice();

private:
	int m_iNo_Machine;	//! UV Lamp ��ġ�� ���� ��ȣ, 0 based

	bool m_bFlag_Ready;	//! UV Lamp�� �⺻���� ��� �غ� �������� true�� ����
	bool m_bFlag_Stable;//! UV Lamp�� ����ȭ�Ǹ� true�� ����
	
	TCHAR m_acRecvStorage[16384];
	int m_iRecvSize;
	void OutputRecvData(void *pProcPtr, TCHAR *pcBuff, int iDataSize, int iCnt_Recv);

	int SendData_UV_Controller(BYTE *pbySend, int iSendSize);	

	int	Check_Ready_UV();

protected:
	//! �θ� Ŭ������ ���� �Լ����� ��ü���� ����� �����Ѵ�. 

	//! RS-232C ����� ����� �Ŀ� �� ó�� 1�� �� �����ؾ� �� ����� �����ϴ�. 
	//! ���� ó�� ��ü �Լ�
	virtual void ProcessReceiveData(void *pProcPtr, BYTE *pbyBuff, int iSize, bool bFlag_Active, int iCnt_Recv);
	//! ���� ó�� �� ��ó�� �Լ�
	virtual void PostProcRecvData();

public:
	int m_iMode_Control;	//! UV Lamp ��ġ�� ���� ���� ���
							//! 0 : Front Panel ���, 1 : PC������ RS-232C ��� ����

	int m_iMode_Program;	//! UV Lamp ��ġ�� ���� Program Mode
							//! 1 : 1 Step. 7 : 7 Step

	double m_dOutputIntensity;		//! UV Lamp ��ġ�� Output Intensity

	int m_iUnit_Output_Intensity;	//! UV Lamp ��ġ�� Output Intensity�� ����
									//! 1 : %, 2 : W

	//! ���� UV Lamp ��ġ�� ������ Cure Program ���� ���� ����ü
	sreSetupInfo_UV_Program m_astrCureInfo[iBufSize_Cure_Program];

	//! UV Lamp ��ġ�� ������ Cure Program ���� ���� ����ü
	sreSetupInfo_UV_Program m_astrTemp_CureInfo[iBufSize_Cure_Program];

	//! "INP" Command�� ���� ���信�� �� Bit�� ó�� ���� Flag ����
	//! "INP" Command�� ���ŵ� ������ ���� �۾��� �ݺ��Ǵ� ���� ���� ���� �߰��ϴ�.
	int m_aiBuf_INP_Bit[iBufSize_INP_Bit];

public:
	//! UV ��ġ�� ����
	//! [���� ����] �ݵ��, SetReceiveProcPtr �Լ��� ���� ó���� �� Dialog�� GUI Ŭ������ �����ϰ�, �� �Լ��� ȣ���Ѵ�. 
	bool Connect_Device(int sPort);
	//! ���� ����
	void Close_Device();

	//! ���ڿ� ���� ����
	int SendData_UV_Controller(CString sSend);

	//! UV Lamp�� �⺻���� ���� �غ� ��ġ�� true�� ��ȯ
	bool IsReady();
	//! UV Lamp�� ����ȭ�Ǹ� true�� ��ȯ
	bool IsStable_UV_Lamp();

	//! �Ʒ� �Լ����� �������� ���ؼ�, '4.LC8_cnt Instruction Manual(English)_1.pdf' ������ ���� �ٶ�

	void Set_Flag_UV_Lamp_Stable();		//! UV Lamp�� ����ȭ�Ǿ��� ���� ȣ���Ѵ�. 
										//! [���� ����] �� �Լ��� ���� ȣ���ϸ� �ȵȴ�. ���� ���ڿ� ó�������� ȣ���Ѵ�. 

	//! �Ʒ��� �Լ����� 'Ask~' �Լ��� UV ��ġ�� ���� ���� ������ ��û�ϴ� �Լ���
	//! 'CAABonderDlg::Parse_From_UV_Lamp' �Լ����� ���� ���ڿ� ó���� �Ѵ�. 	

	//! 'CNT' ����
	int Set_Control_Mode_Front_Panel();	//! Front Panel ����� ����
	int Set_Control_Mode_RS232C();		//! RS-232C ��� ����� ����
	int Ask_Control_Mode();				//! ���� ���� ����� ���´�. 

	//! 'PROG' ����
	int Set_Program_Mode_1_Step();		//! AT~, INT~ ���� �̿�
	int Set_Program_Mode_7_Steps();		//! Memory cure program ��� ���
	int Ask_Program_Mode();				//! ���� �������� ���´�. (PROG? Command)

	//! Shutter�� Diapgragm�� ����� ������ �Ѵ�. ������, Shutter�� ũ�� �����̴� �뵵�̰�, Diapgragm�� �̼� �������̴�.
	int UV_Shutter_PowerSet(int value);
	//! ���� ����
	int UV_Lamp_On();
	int UV_Lamp_Off();
	int UV_Shutter_Open();
	int UV_Shutter_Close();

	//! AT~' ���� (1) (1 Step������ ��� ����)
	int Set_Shutter_AutoTime_1_Step(double fi_dTime);
	int Ask_Shutter_AutoTime_1_Step(double *fo_pdTime);

	//! AT~' ���� (2) (1 Step������ ��� ����)
	int Start_Shutter_Auto_Operation_1_Step();
	int Ask_RemainTime_Shutter_Auto_Operation_1_Step(double *fo_pdTime);

	//! 'INT' ���� (1) (1 Step, 7 Step ��� ��� ����)
	int Set_Diaphragm_Stop();
	int Set_Diaphragm_Up();
	int Set_Diaphragm_Down();
	int Set_Diaphragm_Up_1_Point();
	int Set_Diaphragm_Down_1_Point();

	//! 'INT' ���� (2) (1 Step������ ��� ����)
	int Set_Output_Intensity_1_Step(double fi_dInput);

	//! 'INT' ���� (3) (1 Step, 7 Step ��� ��� ����)
	bool m_bFlag_Request_INP;	//! �ߺ� ó�� ������ ��� ����, ���� ó�� �Ŀ� Ask_Output_Intensity �Լ� ȣ�� 
	int Ask_Output_Intensity();

	//! 'LIF, LAMP~' ����
	int Reset_UV_Lamp_Operation_Time();
	int Ask_UV_Lamp_Operation_time();

	//! 'CURE~' ���� (7 Step������ ��� ����)
	//! Memory cure program ����� �Լ�
	//! '4.LC8_cnt Instruction Manual(English)_1.pdf' ������ 'Main Window' ȭ�� ������ �о��,
	//! �Ʒ� �Լ��� �뵵�� ���� �� �� �ִ�.
	int Set_Cure_Program_Info_7_Steps(int fi_iNoProgram, sreSetupInfo_UV_Program *fi_pstrInput);
	int Ask_Cure_Program_Info_7_Steps(int fi_iNoProgram);
	int Start_Cure_Program_7_Steps(int fi_iNoProgram);
	int Stop_Cure_Program_7_Steps();
	int Ask_Cure_Program_Progress_7_Steps();
	
	//! 'INP' ���� Flag ���� �ʱ�ȭ
	void Init_Buffer_Input_Pin();
	//! 'INP' Command�� UV ��ġ�� ������ ������ �پ��� ���� ������ �˾Ƴ���. 
	int  Ask_Input_Pin();

	CString GetExplanationNG(CString sNumberNG);
};

