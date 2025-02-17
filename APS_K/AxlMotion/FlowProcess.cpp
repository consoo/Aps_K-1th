#include "stdafx.h"
#include "FlowProcess.h"

#include "../AA Bonder.h"
#include "../AA BonderDlg.h"
#include "../DataHandler.h"
#include "../DioControl.h"
#include "../vision.h"
#include "../MIU/Miu.h"
#include "../ForceAlignDlg.h"

#include "../PcbDlg.h"

extern	CDioControl		Dio;
extern	CMIU			MIU;
extern	MIU_DEVICE		gMIUDevice;
extern	CForceAlignDlg* g_pFoceDlg;

extern	CPcbDlg*		pcbDlg;

extern	CVision			vision;
extern  MandoInspLog	MandoInspLog;

extern	CTask			Task;
extern	CSystemData		sysData;
extern	CModelList		modelList;
extern	CModelData		model;
extern	CWork			work;

extern	bool			bThreadCcmGrab;
extern	bool			bThreadCcmGrabRun;		// CCM ���� ȹ�� Thread ���۽� True

extern double			g_dFindCenterX[3];
extern double			g_dFindCenterY[3];
extern double			g_dFindAngle[3];


// CFlowProcess
IMPLEMENT_DYNAMIC(CFlowProcess, CWnd)

#define TIME_MOTOR_MOVE			20000	//���� �̵� �˶� �ð�
#define TIME_DIO_SENSOR			5000	//DI/O ���� Ȯ�� �ð�
#define TIME_DIO_VACCUM			3000	//���� Ȯ�� �ð�
#define ENCORDER_GAP			0.006	//���� �̵� Ȯ�ν� Encorder ��� ����(��)

CFlowProcess::CFlowProcess()
{
	dProcessTime = 0.0;
}

CFlowProcess::~CFlowProcess()
{
}


BEGIN_MESSAGE_MAP(CFlowProcess, CWnd)
END_MESSAGE_MAP()

//
//int CFlowProcess::RunProc_ProductLoading(int iUseStep)
//{//! �۾��� ��ǰ ���� �ڵ� Step( 10000 ~ 11000 )
//	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
//
//	int iRtnFunction = iUseStep;
//	CString logStr = "", sBarCode="";
//	bool bChk = true;
//
//	switch( iUseStep )
//	{
//	case 10000://! DoorĿư Lift Open.
//		if( Dio.DoorLift(true, true) )	iRtnFunction = 10050;
//		else 
//		{
//			logStr.Format("Door Open ����[%d]", iUseStep);
//			errMsg2(Task.AutoFlag, logStr);
//			iRtnFunction = -10000;
//		}
//		break;
//
//	case 10050://! Lens�� Lift �Ǹ��� Check
//		if( Dio.LensLiftCheck(true))	iRtnFunction = 10080;
//		else
//		{
//			logStr.Format("Door Open Sensor ���� ����[%d]", iUseStep);
//			errMsg2(Task.AutoFlag, logStr);
//			iRtnFunction = -10000;
//		}
//		break;
//
//	case 10080://! Dark �˻� �Ǹ��� �ϰ��� ��� ���
//		if( !Dio.PCBDarkCheck(true, false) )
//		{
//			if( !Dio.PCBDark(true, true) )
//			{
//				logStr.Format("Dark�˻� �Ǹ��� ��� ����.[%d]", iUseStep);
//				errMsg2(Task.AutoFlag, logStr);
//				iRtnFunction = -10080;
//				break;
//			}
//		}
//		iRtnFunction = 10100;
//		break;
//
//	case 10100://! Chip ���� Draw
//		pFrame->func_ChipID_Draw();
//		iRtnFunction = 10150;
//		break;
//
//	case 10150://! Start P/B Button Push ���.
//		if( Dio.StartPBOnCheck(true, false) )	
//		{
//			bool bflag = true;
//
//			bflag = pFrame->EpoxyTimeCheck(pFrame->bEpoxyTimeChk);
//			if(!bflag)
//			{
//				iRtnFunction = -10150;
//				break;
//			}
//
//			Task.func_TactTimeInit();
//			pFrame->dispGrid();
//			Task.m_timeChecker.Measure_Time(1);	//Start ��ư Ŭ�� �ð�
//
//			pFrame->func_ChipID_Draw();
//			iRtnFunction = 10200;
//		}
//		break;
//
//	case 10200://! PCB ���� ���� Ȯ��
//		if(Dio.PCBPushSocketCheck(true, false) == false)
//		{
//			logStr.Format("PCB�� Ŀ�� ���� Open ���� Start ��ư ���� Error [%d]", iUseStep);
//			errMsg2(Task.AutoFlag, logStr);
//			iRtnFunction = -10200;
//			break;
//		}
//		iRtnFunction = 10300;
//		break;
//
//	case 10300://! Lens Grip ����
//		Task.PcbOnStage = 100;
//		if( Dio.LensGrip(true,false) )
//		{
//			Task.PCBTaskTime = myTimer(true);
//			iRtnFunction = 10310;
//		}
//		else
//		{
//			logStr.Format("Lens Grip ���� ��� ����[%d]", iUseStep);
//			errMsg2(Task.AutoFlag, logStr);
//			iRtnFunction = -10300;
//		}		
//		break;
//
//	case 10310://! Lens Grip ���� Ȯ��
//		if(Dio.LensGripCheck(true))		iRtnFunction = 10400;
//		else if((myTimer(true) - Task.PCBTaskTime) > IO_DELAY_TIME)
//		{
//			logStr.Format("Lens Grip ���� ���� ��ȣ Ȯ�� �ð� �ʰ�[%d]", iUseStep);
//			errMsg2(Task.AutoFlag, logStr);
//			iRtnFunction = -10300;
//		}
//		break;
//
//	case 10400:	//! ���ڵ� ���� Check
//		MandoInspLog.func_InitData();	//-- Log �ʱ�ȭ
//		sBarCode.Format("%s", Task.ChipID);
//
//		if( sBarCode == "EMPTY" )	//-> �ʱ⿡ ������ EMPTY�� �ٲ� ����.
//		{//���ڵ������� ���� ��� Ȯ�� Message
//			logStr.Format("���ڵ� ������ �����ϴ�. \n [ %s ]������ ���� �Ͻðڽ��ϱ�?", Task.ChipID);
//			if ( askMsg(logStr) == IDOK)
//			{
//				sBarCode.Format("%s", Task.ChipID);
//			}
//			else
//			{
//				iRtnFunction = -10400;
//				break;
//			}
//		}
//		
//		vision.clearOverlay(CCD);
//		//vision.drawOverlay(CCD);
//		iRtnFunction = 10450;
//		break;
//
//	case 10450:	//! Area ���� ���� Check
//		if( Dio.LightCurtainOnCheck(false, false) )
//		{
//			if( Dio.DoorLift(false, false) )
//			{
//				Sleep(100);
//				iRtnFunction = 10460;
//			} 
//			else
//			{
//				logStr.Format("Door Close ����[%d]", iUseStep);
//				errMsg2(Task.AutoFlag, logStr);
//				iRtnFunction = -10450;
//			}
//		}
//		break;
//
//	case 10460:	//Light Ŀư ���� ���� ���� Ȯ��
//		if( Dio.DoorLiftOnCheck(false, false) )
//		{//Door Lift�� Close�Ǹ� ���� Step
//
//			sysData.Load();
//
//			Task.PcbOnStage = 100;
//			Task.m_b_AA_Retry_Flag = false;
//
//			logStr.Format("%d", Task.m_iPcbPickupNo);
//			pFrame->m_labelPickupNoPcb.SetText(logStr);
//			pFrame->m_labelPickupNoPcb.Invalidate();
//	 
//			MandoInspLog.sBarcodeID.Format("%s", Task.ChipID);	//���� ������Camera �˻� Log ����
//
//			iRtnFunction = 11000;
//		}
//		if( Dio.LightCurtainOnCheck(true, false) )
//		{//Lift Close���� ���� ���� ���� �� ��� Open
//			if( Dio.DoorLift(true, false) )
//			{
//				iRtnFunction = 10450;
//			}
//			else
//			{
//				logStr.Format("Door Open ����[%d]", iUseStep);
//				errMsg2(Task.AutoFlag, logStr);
//				iRtnFunction = -10460;
//			}
//		}
//		break;
//
//
//	default:
//		logStr.Format("PCB ��ǰ ���� Thread Step ��ȣ ������ . [%d]", iUseStep);
//		errMsg2(Task.AutoFlag, logStr);
//		iRtnFunction = -1;
//		break;
//	}
//
//	return iRtnFunction;
//}

//int CFlowProcess::RunProc_PCBOutsideAlign(int iUseStep)
//{//! PCB �ܺ� Align Step(11000 ~ 14999)					//!! MIU_Initialize -> PCB Holder Align ��ġ
//	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
//
//	int iRtnFunction = iUseStep;
//	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;		//PCB ���� ȹ�� Delay
//	int iAlignRetry = model.strInfo_Cam[1].m_iRetryCnt;
//	int iRtn = 0;
//	CString logStr = "";
//	bool bChk = true;
//	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;
//
//	switch( iUseStep )
//	{
//	case 11000://! MIU ���� Initialize
//		Task.PCBTaskTime = myTimer(true);
//		if (sysData.m_FreeRun == 0)
//		{
//			pFrame->m_csLock_Miu.Lock();
//			MIU.Close();
//	
//			if( !pFrame->MIUCheck_process() || gMIUDevice.CurrentState != 4)
//			{
//				logStr.Format("CCD ��� ���� �ʱ�ȭ ����.[%d]\n ��ǰ ���� ���� �� ��ǰ �ҷ� Ȯ�� �ϼ���.", iUseStep);
//				errMsg2(Task.AutoFlag, logStr);
//				iRtnFunction = -11000;
//				break;
//			}
//			pFrame->m_csLock_Miu.Unlock();	
//		}
//
//		iRtnFunction = 12000;
//		break;
//
//	case 12000://! ���ڵ� DB Data ����
//		if( !g_ADOData.func_AATaskToRecordID(modelList.curModelName, Task.ChipID) )
//		{
//			logStr.Format("DataBase ���� ����.[%d]\n MS Office�� �ݾ��ּ���.", iUseStep);
//			errMsg2(Task.AutoFlag,logStr);
//			iRtnFunction = -12000;
//			break;
//		}
//		
//		if((sysData.m_iDispensePass == 1) || (Task.m_bOkDispense == 1) )	iRtnFunction = 16350;	//-- ���� Pass Mode �̰ų�, ���� �Ϸ� �� ���	//Holder Align -> Clean
//		else																iRtnFunction = 13000;
//		break;
//
//	case 13000: //PCB Holder Align ��ġ
//		Task.m_timeChecker.Measure_Time(2);	//���� ���� T/T Check
//		pFrame->SetDigReference(PCB_Holder_MARK);
//		LightControl.ctrlLedVolume(LIGHT_OUTSIDE, model.m_iLedValue[LEDDATA_OUTSIDE]);		// Align�� ���� ON
//
//		Task.m_iRetry_Opt = 0;
//		Dio.BackLightOn(false, false);
//
//		pFrame->m_iCurCamNo = 0;
//		pFrame->setCamDisplay(0, 1);
//
//		if (motor.Pcb_Motor_Move(Holder_Pos))
//		{
//			logStr.Format("PCB Holder Align �̵� [%d]", iUseStep);
//			pFrame->putListLog(logStr);
//			iRtnFunction = 13500;
//		} else {
//			logStr.Format("PCB Align �̵� ����[%d]", iUseStep);
//			pFrame->putListLog(logStr);
//			errMsg2(Task.AutoFlag, logStr);
//			iRtnFunction = -13000;
//		}
//
//		Task.PCBTaskTime = myTimer(true);
//		Task.d_Align_offset_x[PCB_Holder_MARK] = 0;
//		Task.d_Align_offset_y[PCB_Holder_MARK] = 0;
//		Task.d_Align_offset_th[PCB_Holder_MARK] = 0;
//		break;
//
//	case 13500:
//		checkMessage();
//		if((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
//		{
//			if(gMIUDevice.CurrentState != 4 || bThreadCcmGrab == false ||
//				 !PRIInspection.func_Insp_CurrentMeasure(false) )
//			{
//				logStr.Format("CCD �ǽð� ���� ȹ�� ����.[%d]\n ��ǰ ���� ���� �� ��ǰ �ҷ� Ȯ�� �ϼ���.", iUseStep);
//				errMsg2(Task.AutoFlag, logStr);
//				iRtnFunction = -13500;
//				break;
//			}
//
//			iRtnFunction = 14000;
//		}
//		break;
//
//	case 14000:	// 14000 : ���� ȹ�� �� ������ ���
//		if(sysData.m_FreeRun == 1)
//		{
//			for(int i=0; i<100; i++)
//			{
//				Sleep(10);
//				checkMessage();
//			}
//
//			iRtnFunction = 16000;
//			Task.d_Align_offset_x[PCB_Holder_MARK] = 0;
//			Task.d_Align_offset_y[PCB_Holder_MARK] = 0;
//			Task.d_Align_offset_th[PCB_Holder_MARK] = 0;
//			break;
//		}
//
//		if (Task.m_iRetry_Opt>iAlignRetry)
//		{
//			pFrame->alignImageBackup(CAM2);
//
//			logStr.Format("PCB Align ��˻� %d ȸ ����[%d]", Task.m_iRetry_Opt, iUseStep);
//			errMsg2(Task.AutoFlag,logStr);
//
//			Task.m_iRetry_Opt =0;
//			iRtnFunction = -14000;
//
//			//�ӽ� ��ũ ���â
//			pFrame->setCamDisplay(0, 1);
//			pFrame->ctrlSubDlg(MAIN_DLG);
//			pFrame->changeMainBtnColor(MAIN_DLG);
//
//			g_pFoceDlg->SetCamNo(CAM2, PCB_Holder_MARK);
//			g_pFoceDlg->SetMarkPosition( (int)g_dFindCenterX[PCB_Holder_MARK], (int)g_dFindCenterY[PCB_Holder_MARK]);
//			g_pFoceDlg->ShowWindow(SW_SHOW);
//			break;
//		}
//
//		offsetX = offsetY = offsetTh = 0.0;	
//		iRtn = pFrame->procCamAlign(CAM2, PCB_Holder_MARK, false, offsetX, offsetY, offsetTh);
//		saveInspImage(PCB_IMAGE_SAVE, Task.m_iRetry_Opt);
//
//		Task.m_iRetry_Opt++;
//
//		if (iRtn == 0)
//		{
//			Task.d_Align_offset_x[PCB_Holder_MARK] -= offsetX;
//			Task.d_Align_offset_y[PCB_Holder_MARK] -= offsetY;
//			Task.d_Align_offset_th[PCB_Holder_MARK] += offsetTh;
//
//			logStr.Format("		PCB Holder Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]);
//			pcbDlg->m_labelAlignResult.SetText(logStr);
//			pcbDlg->m_labelAlignResult.Invalidate();
//
//			pFrame->putListLog(logStr);
//
//			int iRtnVal = pFrame->AlignLimitCheck(CAM1, offsetX, offsetY, offsetTh); // PCB����� �����̴� �κ��̶� Limit Check �Ҷ� CAM1��
//
//			if(iRtnVal==1)
//			{
//				iRtnFunction = 14100;				// ���� �̵�
//			}
//			else if(iRtnVal==2)
//			{
//				logStr.Format("		PCB Align [%d] �Ϸ� [%d]", Task.m_iRetry_Opt, iUseStep);
//				pFrame->putListLog(logStr);
//
//				pFrame->InstantMarkDelete(PCB_Holder_MARK);			//�ӽ� ��� ��ũ ����
//
//				logStr.Format("		PCB Holder Align Complete :  %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]);
//				AlignResultSave(logStr);	
//				Task.m_iRetry_Opt = 0;
//				iRtnFunction = 14200;				// �Ϸ�
//			}
//			else
//			{
//				logStr.Format("PCB Align [%d] ���� ���� �ʰ�[%d]", Task.m_iRetry_Opt, iUseStep);
//				errMsg2(Task.AutoFlag,logStr);
//				AlignLogSave(logStr);
//				pFrame->InstantMarkDelete(PCB_Holder_MARK);
//				iRtnFunction = -14000;				// NG
//			}
//		}
//		else
//		{
//			logStr.Format("PCB Align [%d] ��ũ �ν� ����[%d]", Task.m_iRetry_Opt, iUseStep);
//			pFrame->putListLog(logStr);
//
//			iRtnFunction = 14000;				//  ��˻� 
//		}
//		break;
//	
//	case 14100://!! ����� �̵�- Retry
//		if(motor.Pcb_Holder_Align_Move(Holder_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]))
//		{
//			Task.PCBTaskTime = myTimer(true);
//			iRtnFunction = 13500;
//		}
//		else
//		{
//			logStr.Format("PCB Align ���� ��ġ �̵� �Ϸ� ����[%d]", iUseStep);
//			errMsg2(Task.AutoFlag,logStr);
//			iRtnFunction = -14100;
//		}	
//		break;
//
//	case 14200:	//TestPattern Check
//		iRtn = PRIInspection.func_Insp_TestPattern(true);
//
//		logStr.Format("[�˻�] TestPattern �˻� : %d(EA)..[%d]", iRtn, iUseStep);
//		pFrame->putListLog(logStr);
//
//		iRtnFunction = 15000;
//		break;
//
//
//	default:
//		logStr.Format("PCB ��ǰ ���� Thread Step ��ȣ ������ . [%d]", iUseStep);
//		errMsg2(Task.AutoFlag, logStr);
//		iRtnFunction = -1;
//		break;
//	}
//
//	return iRtnFunction;
//}

//
//
//int CFlowProcess::RunProc_EpoxyResing(int iUseStep)
//{//! Epoxy ���� �� ���� �˻� Step(15000 ~
//	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
//
//	int iRtnFunction = iUseStep;
//	int iCamDelay = model.strInfo_Cam[1].m_iDelayTime;		//PCB ���� ȹ�� Delay
//	int iRtn = 0;
//	CString logStr = "";
//	bool bChk = true;
//	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;
//
//	switch( iUseStep )
//	{
//	case 15000:
//		if( motor.moveEpoxyCleanPos(work.Cleancnt, true) )
//		{
//			logStr.Format("PCB Clean ��ġ �̵� �Ϸ�[%d]", iUseStep);
//  			pFrame->putListLog(logStr);
//  			iRtnFunction = 15100;
//			work.Cleancnt++;
//			work.Save();
//		}
//		else
//		{
//			logStr.Format("PCB Clean ��ġ �̵� ���� ����[%d]", iUseStep);
//			errMsg2(Task.AutoFlag,logStr);
//			iRtnFunction = -15000;
//		}
//		break;
//
//	case 15100: // ���潺�� Clean Z�� �ϰ�.
//		if(motor.PCB_Z_Motor_Move(Clean_Pos))	iRtnFunction = 15700;
//		else
//		{
//			logStr.Format("PCB Z Clean ��ġ �̵� �Ϸ� ����[%d]", iUseStep);
//			errMsg2(Task.AutoFlag,logStr);
//			iRtnFunction = -15600;
//		}
//		break;
//
//	case 15700: //Dispenser -Z�� ���
//		if(motor.PCB_Z_Motor_Move(Wait_Pos))
//		{
//			if( pFrame->func_MIU_ConnectLiveCheck() == false)		//MIU ���� Check
//			{
//				iRtnFunction = -15700;
//				break;
//			}
//  			iRtnFunction = 16000;
//		}
//		else
//		{
//			logStr.Format("PCB Z Wait ��ġ �̵� �Ϸ� ����[%d]", iUseStep);
//			errMsg2(Task.AutoFlag,logStr);
//			iRtnFunction = -15700;
//		}
//		break;
//
//	case 16000:  //���� ��ġ �̵�
//		LightControl.ctrlLedVolume(LIGHT_OUTSIDE, 0);		// Align ���� OFF
//
//		if(motor.Pcb_Motor_Move(Dispense_Pos, Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]))
//		{
//			logStr.Format("PCB Dispense ���� ��ġ �̵� �Ϸ�-������ X:%.03lf,Y:%.03lf,T:%.03lf", Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]);
//  			pFrame->putListLog(logStr);
//  			iRtnFunction = 16100;
//		}
//		else
//  		{
//  			logStr.Format("PCB Dispense ��ġ �̵� ����[%d]", iUseStep);
//			errMsg2(Task.AutoFlag,logStr);
// 			iRtnFunction = -16000;
//  		}
//		break;
//
//	case 16100: //Dispenser-z �� �ϰ�
//		if(motor.PCB_Z_Motor_Move(Dispense_Pos))		iRtnFunction = 16200;
//		else
//		{
//			logStr.Format("PCB Z Dispense ��ġ �̵� ����[%d]", iUseStep);
//			errMsg2(Task.AutoFlag,logStr);
// 			iRtnFunction = -16100;
//		}
//		break;
//
//	case 16200: // ���潺 ����
//		if(motor.func_Epoxy_CircleDraw())
//		{
//			Task.m_bOkDispense = 1;
//			logStr.Format("PCB Dispense �Ϸ�[%d]", iUseStep);
//  			pFrame->putListLog(logStr);
//  			iRtnFunction = 16300;
//		}
//		else
//		{
//			logStr.Format("PCB Dispense ����[%d]", iUseStep);
//			errMsg2(Task.AutoFlag,logStr);
// 			iRtnFunction = -16200;
//		}
//		break;
//
//	case 16300:
//		if(motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y))
//		{
//			if(motor.PCB_Z_Motor_Move(Wait_Pos))
//			{
//  				iRtnFunction = 16400;
//			}
//			else
//			{
//				logStr.Format("PCB Z ��� ��ġ �̵� ����[%d]", iUseStep);
//  				pFrame->putListLog(logStr);
//  				iRtnFunction = -16300;
//			}
//		}
//		break;
//
//	case 16400://! ���� �˻� ��� ���� �Ǵ�
//		if( sysData.m_iDispenseInspPass == 0 )	iRtnFunction = 16410;
//		else									iRtnFunction = 17000;
//		break;
//
//	case 16410: // �����˻� �̵�
//		// LHC - �����˻� ��ġ�� �ִµ� holder Align ��ġ�� ���� ������ �����ΰ���..? ��������ġ���� �˻縦 �ؾߵǴϱ�..�׷��ǰ���?
//		//if(motor.Pcb_Motor_Move(Insp_Pos,Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]))
//		LightControl.ctrlLedVolume(LIGHT_OUTSIDE, model.m_iLedValue[LEDDATA_EPOXY_INSP]); // LHC - �����˻綧 ����� ���� �ҷ�����
//
//		if(motor.Pcb_Motor_Move(Holder_Pos,
//			Task.d_Align_offset_x[PCB_Holder_MARK], Task.d_Align_offset_y[PCB_Holder_MARK], Task.d_Align_offset_th[PCB_Holder_MARK]))
//		{
//			Task.PCBTaskTime = myTimer(true);
//			logStr.Format("PCB �����˻� ��ġ �̵� �Ϸ�[%d]", iUseStep);
//  			pFrame->putListLog(logStr);
//  			iRtnFunction = 16450;
//		}
//		else
//		{
//			logStr.Format("PCB �����˻� ��ġ �̵� ����[%d]", iUseStep);
//			errMsg2(Task.AutoFlag,logStr);
// 			iRtnFunction = -16400;
//		}
//		break;
//
//	case 16450:// �̵���, ���� �ð� ����
//		if( (myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)) )
//		{
//			iRtnFunction = 16500;
//		}
//		break;
//
//	case 16500: //�����˻�                                �߰��ؾ���
//		bChk = pFrame->_EpoxyFinddispense(0);
//		if(!bChk)
//		{
//			pFrame->setCamDisplay(0, 1);  //LHC - �����˻� ���� �� �ٸ�ȭ���� �������� ���� �� �ֱ� ������ ����Ȯ���ϱ����� ȭ����ȯ.
//			pFrame->putListLog("Epoxy ���� �˻� ����");
//			CString putLog;
//			putLog.Format("Epoxy Table ��ǥ (X:%.03f, Y:%.03f, ��:%.03f)", motor.GetEncoderPos(Motor_PCB_X), motor.GetEncoderPos(Motor_PCB_Y), motor.GetEncoderPos(Motor_PCB_TH));
//			pFrame->putListLog(putLog);
//
//			sprintf_s(motor.m_szLogMsg, "Epoxy ���� �˻� ����. \n�ڵ� ���� ���� �Ͻðڽ��ϱ�?");
//
//			Dio.setAlarm(ALARM_ON);
//			if (askMsg(motor.m_szLogMsg))
//			{
//				Task.PCBTaskTime = myTimer(true);
//
//				Dio.setAlarm(ALARM_OFF);
//			}
//			else
//			{
//				Dio.setAlarm(ALARM_OFF);
//
//				logStr.Format("Epoxy ���� �˻� �Ϸ� ����[%d]", iUseStep);
//				iRtnFunction = -16500;
//				break;
//			}
//		}
//		else
//		{
//			Task.m_timeChecker.Measure_Time(3);	//���� �Ϸ� �ð�
//			Task.m_dTime_Epoxy = Task.m_timeChecker.m_adTime[3] - Task.m_timeChecker.m_adTime[2];
//			pFrame->dispGrid();
//
//			logStr.Format("PCB �����˻� �Ϸ�[%d]", iUseStep);
//  			pFrame->putListLog(logStr);
//		}
//
//		Task.d_Align_offset_x[LENS_Align_MARK] = 0;
//		Task.d_Align_offset_y[LENS_Align_MARK] = 0;
//		Task.d_Align_offset_th[LENS_Align_MARK] = 0;
//
//		iRtnFunction = 16600;
//		break;
//
//		default:
//		logStr.Format("PCB Dispenser Thread Step ��ȣ ������ . [%d]", iUseStep);
//		errMsg2(Task.AutoFlag, logStr);
//		iRtnFunction = -1;
//		break;
//	}
//
//	return iRtnFunction;
//}

//
//int	CFlowProcess::RunProc_LensOutSideAlign(int iUseStep)
//{//! Lens �ܺ� Align Step	( 16600 ~ 
//	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
//
//	int iRtnFunction = iUseStep;
//	int iRtn = 0;
//	CString logStr = "";
//	bool bChk = true;
//	double offsetX=0.0, offsetY=0.0, offsetTh=0.0;
//
//	switch( iUseStep )
//	{
//	case 16600:
//		if( Task.m_bOKLensPass == 1 )
//		{//LENS �ѱ� �Ϸ� �� ���
//			iRtnFunction = 16800;	
//			break;
//		}
//
//		Task.m_iRetry_Opt = 0;
//		pFrame->SetDigReference(LENS_Align_MARK);
//		LightControl.ctrlLedVolume(LIGHT_OUTSIDE, model.m_iLedValue[LEDDATA_OUTSIDE]);		// Align�� ���� ON
//
//		if(motor.Pcb_Motor_Move(Lens_Pos))
//		{
//			Task.PCBTaskTime = myTimer(true);
//			logStr.Format("Lens Align ��ġ �̵� �Ϸ�[%d]", iUseStep);
//  			pFrame->putListLog(logStr);
//  			iRtnFunction = 16650;
//		}
//		else
//		{
//			logStr.Format("Lens Align ��ġ �̵� ����[%d]", iUseStep);
//			errMsg2(Task.AutoFlag,logStr);
// 			iRtnFunction = -16600;
//		}
//		break;
//
//	case 16650:
//		checkMessage();
//		if((myTimer(true) - Task.PCBTaskTime) > iCamDelay && (motor.IsStopAxis(Motor_PCB_X) && motor.IsStopAxis(Motor_PCB_Y)))
//		{//!! Lens UnGrip Check
//			if( Dio.LensMotorGripCheck(true, false) )
//			{
//				logStr.Format("Lens Gripper Grip ����..�浹 ���� �߻�..\n Lens�� Grip ���� �� �����ϼ���.", iStep);
//				iRtnFunction = -16650;
//				break;
//			}
//
//			Task.PCBTaskTime = myTimer(true);
//			iRtnFunction = 16700;
//		}
//		break;
//
//	case 16700: // lens Align
//		if (Task.m_iRetry_Opt>iAlignRetry)
//		{
//			alignImageBackup(CAM2);
//
//			logStr.Format("Lens Align ��˻� %d ȸ ����[%d]", Task.m_iRetry_Opt, iStep);
//			errMsg2(Task.AutoFlag,logStr);
//
//			Task.m_iRetry_Opt =0;
//			iRtnFunction = -16700;
//
//			//�ӽ� ��ũ ���â
//			setCamDisplay(0, 1);
//			ctrlSubDlg(MAIN_DLG);
//			changeMainBtnColor(MAIN_DLG);
//
//			g_pFoceDlg->SetCamNo(CAM2, LENS_Align_MARK);
//			g_pFoceDlg->SetMarkPosition((int)g_dFindCenterX[LENS_Align_MARK], (int)g_dFindCenterY[LENS_Align_MARK]);
//			g_pFoceDlg->ShowWindow(SW_SHOW);
//			break;
//		}
//			
//		offsetX = offsetY = offsetTh = 0.0;
//		LightControl.ctrlLedVolume(UP_LIGHT, model.m_iLedValue[UP_LENS_LIGHT]);		// Align�� ���� ON
//		
//		iRtn = procCamAlign(CAM2, LENS_Align_MARK, false, offsetX, offsetY, offsetTh);
//
//		saveInspImage(LENS_IMAGE_SAVE, Task.m_iRetry_Opt);
//
//		Task.m_iRetry_Opt++;
//
//		if (iRtn == 0)
//		{
//			Task.d_Align_offset_x[LENS_Align_MARK] -= offsetX;
//			Task.d_Align_offset_y[LENS_Align_MARK] -= offsetY;
//			Task.d_Align_offset_th[LENS_Align_MARK] = 0;//+= offsetTh;	//Lens�� ������ Theta�� ����
//
//			logStr.Format("		Lens Align : %.3lf %.3lf %.3lf", Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
//			pcbDlg->m_labelAlignResult.SetText(logStr);
//			pcbDlg->m_labelAlignResult.Invalidate();
//
//			putListLog(logStr);
//
//			int iRtnVal = AlignLimitCheck(CAM1, offsetX, offsetY, 0);
//
//			if(iRtnVal==1)
//			{
//				iRtnFunction = 16750;				// ���� �̵�
//			}
//			else if(iRtnVal==2 )
//			{
//				logStr.Format("		Lens Align [%d] �Ϸ� [%d]", Task.m_iRetry_Opt, iStep);
//				putListLog(logStr);
//
//				InstantMarkDelete(LENS_Align_MARK);			//�ӽ� ��� ��ũ ����
//
//				logStr.Format("		Lens Align Complete : %d, %.03f, %.03f, %.03f", Task.m_iPcbPickupNo, Task.d_Align_offset_x[LENS_Align_MARK], Task.d_Align_offset_y[LENS_Align_MARK], Task.d_Align_offset_th[LENS_Align_MARK]);
//				AlignResultSave(logStr);	
//				Task.m_iRetry_Opt = 0;
//				iRtnFunction = 16800;				// �Ϸ�
//			}
//			else
//			{
//				logStr.Format("Lens Align [%d] ���� ���� �ʰ�[%d]", Task.m_iRetry_Opt, iStep);
//				errMsg2(Task.AutoFlag,logStr);
//				AlignLogSave(logStr);
//				InstantMarkDelete(LENS_Align_MARK);
//				iRtnFunction = -16700;				// NG
//			}
//		}
//		else
//		{
//			logStr.Format("Lens Align [%d] ��ũ �ν� ����[%d]", Task.m_iRetry_Opt, iStep);
//			putListLog(logStr);
//
//			iRtnFunction = 16700;					//  ��˻� 
//		}
//		break;
//
//
//
//
//	default:
//		logStr.Format("Lens �ܺ� Align Thread Step ��ȣ ������ . [%d]", iUseStep);
//		errMsg2(Task.AutoFlag, logStr);
//		iRtnFunction = -1;
//		break;
//	}
//
//	return iRtnFunction;
//}