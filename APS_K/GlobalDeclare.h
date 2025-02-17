#pragma once
#ifndef ____GLOBAL_DEFINE_H__
#define ____GLOBAL_DEFINE_H__
//=================================================================================================================
//=================================================================================================================
#include "AA Bonder.h"
#include "AA BonderDlg.h"


#include "./Library/Utility/ximg/ximage.h"	//2017101201


#include "./Library/Inspection/Include/LibACMISCommon/ACMISCommon.h"
#include "./Library/Inspection/Include/LibACMISSoftISP/SoftISP.h"
#include "./Library/Inspection/Include/LibACMISImage/ACMISImageDef.h"
//
#include "./Library/Inspection/Include/LibACMISImage/ACMISImageBlemish.h"
#include "./Library/Inspection/Include/LibACMISImage/ACMISImageDarkTest.h"
#include "./Library/Inspection/Include/LibACMISResolution/ACMISResolutionSFR.h"
//
#include "./Library/Inspection/Include/LibACMISShading/ACMISShadingDef.h"
#include "./Library/Inspection/Include/LibACMISShading/ACMISShadingAlgorithm.h"
#include "./Library/Inspection/Include/LibACMISShading/ACMISShadingDeviation.h"
//
#include "./Library/Inspection/Include/LibACMISImage/ACMISSignalNoiseRatio.h"
#include "./Library/Inspection/Include/LibACMISResolution/ACMISFiducialMark.h"

#include "./Library/Inspection/Include/LibACMISSoftISP/RawImageProcessing.h"

#include "./Library/Inspection/Include/LibACMISImage/ACMISSignalNoiseRatio.h"

#include <math.h>
#include <float.h>
#include <io.h>
#include "mil.h"
#include "keyence\KeyenceSensor.h"
#include "IrvLed\IrvLedControl.h"
#include "barcode\BarcodeRead.h"
#include "EZIni.h"
#include "Vision.h"
#include "DataHandler.h"
#include "ADOAccess/ADOClass.h"
#include "Path.h"
#include "Device/Light/LightControl.h"
#include "DioControl.h"
#include "DataHandler.h"
#include "CeepromData.h"
#include "GetNumDlg.h"
#include "KeyPadDlg.h"
#include "MIU/Miu.h"
#include "Device/UV/UVCommand.h"
#include "DelayMsgDlg.h"
#include "AlarmDialog.h"
#include "AskMsgDlg.h"
#include "AskMsgDlg2.h"
#include "InformDlg.h"
#include "visa.h"
//! <--------------------------------------------------------------------------
//! 전역 Utility 함수를 사용하기 위해 필요하다. 
#include "Library/Utility/UtilityFunc.h"	//! (Added by LHW, 2013/2/5)
#include "Library/Utility/TimeChecker.h"	//! (Added by LHW, 2013/3/4)
#include "Algorithm\INSP\PRIFunc_Insp.h"
//! <--------------------------------------------------------------------------
extern  CAxlMotorSet	motor;
extern CADOClass		g_ADOData;
extern CBarcodeRead		barcode;
//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern	CDioControl		Dio;
extern	CModelData		model;
extern	CPasswordData	password;
extern MIU_DEVICE					gMIUDevice;
extern char							gDeviceCount;
extern char							gDeviceIndex;
extern CMIU       MIU;
extern CMandoSfrSpec		MandoSfrSpec;
extern  CDelayMsgDlg*	delayDlg;
extern CAlarmDialog*	alarmDlg;
extern CTask	Task;
extern CWork			work;
extern CSystemData	sysData;
extern CVision vision;
extern CMandoInspLog		MandoInspLog;
extern CModelList		modelList;
extern CList_RS232C		SerialPortList;
extern CLightControl LightControl;
extern CLightControl LightControlSecond;
extern CLightControl LightControlthird;
extern bool	g_bReady_UV;
extern CIrvLedControl		IrvAlignLed;
extern CMESCommunication	MESCommunication;
extern CeepromData	eepromData;
extern double	dSFR_N_4_PositionX;
extern double	dSFR_N_8_PositionX;
extern CPRIFunc_Insp g_clPriInsp;
//! DIO 현재 상태 임시 저장 버퍼
extern int curInDio[5];
extern int curOutDioByte[5][4];
extern double	g_dFindCenterX[3];
extern double	g_dFindCenterY[3];
extern double	g_dFindAngle[3];
extern CKeyenceSensor		Keyence;
extern		int			g_iCCDCamView;	//! Added by LHW [2013/3/27)
extern bool g_bMovingflag;
extern	bool JogPlusflag;
extern	bool JogMinusflag;
extern bool		g_AlarmFlag;
extern int			g_AlarmCnt;
extern CUVCommand UVCommand;
extern CUVCommand UVCommand2;
extern LARGE_INTEGER	m_liStartCounter;
extern LARGE_INTEGER	m_liFreq;
extern CDPoint		LaserPos[4];
extern double		LaserValue[4];
extern CPoint cpFiducialPos[4];
extern int iLaser_Pos;
extern CRect crFiducialRoi[4];
const double w = 0.002118902;
//
double	myTimer(bool flag);
void delayMsg(CString msg, int delay = 3000, COLORREF color = M_COLOR_GREEN);
void delayMsg(bool showFlag);
bool askMsg(CString MSG);
int  askMsg2(CString MSG);
void errMsg2(int Autoflag, CString MSG);

void LogSave(CString logStr);
void saveNgList(CString ngMessage);
void SocketDataSave(int flag, CString logStr);
void MiuLogSave(CString strLog);
void AlignLogSave(CString strLog);
void AlignResultSave(CString strLog);

void	DoEvents();

bool g_SaveLGITLog(int nUnit, CString name, CString title, CString data);
bool g_FinalInspLog();
void Jpg_ImageSave(IplImage* jpgImg, int index);

bool PcbOsTest();

void deletedir(CString FolderName);


double getAngle(double x1, double y1, double x2, double y2);
double getRadianFromAng(double Angle);
double getAngFromRadian(double Angle);

bool getMotorPos(int axis, int index, double &pos, bool flagDisp);
bool saveSfrLog(int iMode = 0);
void saveInspImage(int iMode, int iStep);
void saveDefectLog();
void DeletePath(CString strPath);
void GetDateAndTime(int &nYear, int &nMonth, int &nDay, int &nHour, int &nMinute, int &nSecond);

void CheckHandle();

bool ChartPatternMatching(int iField);
bool ChartCenterPatternMatching();
bool FieldPatternMatching(int iFieldNo, double &dCenterPosX, double &dCenterPosY, double &dAngle, double &dRate);

bool SetJogSpeed(int iID_GUI, CWnd* pParentWnd, double *pOutSpeedGain);


bool getDistortionCenter(CPoint& dc);
double calcChiSquareCol(cv::Size boardSize, cv::vector<cv::Point2f> pointBuf);
double calcChiSquareRow(cv::Size boardSize, cv::vector<cv::Point2f> pointBuf);

double AutoZSearchCalc();
CString CovertToHex(CString data, int type);
CString CovertToChar(CString data, int type);




//=================================================================================================================
//=================================================================================================================
#endif