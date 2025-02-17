

//PSH150112

#ifdef  _M_X64
	#include "C:/Program Files (x86)/EzSoftware UC/AXL(Library)/C, C++/AXL.h"
	#include "C:/Program Files (x86)/EzSoftware UC/AXL(Library)/C, C++/AXM.h"
	#include "C:/Program Files (x86)/EzSoftware UC/AXL(Library)/C, C++/AXDev.h"
	#include "C:/Program Files (x86)/EzSoftware UC/AXL(Library)/C, C++/AXHS.h"
	#include "C:/Program Files (x86)/EzSoftware UC/AXL(Library)/C, C++/AXD.h"
	#pragma comment(lib, "C:/Program Files (x86)/EzSoftware UC/AXL(Library)/Library/64Bit/AxL.lib")

#else
	#include "C:\\Program Files\\EzSoftware UC\\AXL(Library)\\C, C++\\AXL.h"
	#include "C:\\Program Files\\EzSoftware UC\\AXL(Library)\\C, C++\\AXM.h"
	#include "C:\\Program Files\\EzSoftware UC\\AXL(Library)\\C, C++\\AXDev.h"
	#include "C:\\Program Files\\EzSoftware UC\\AXL(Library)\\C, C++\\AXHS.h"
	#include "C:\\Program Files\\EzSoftware UC\\AXL(Library)\\C, C++\\AXD.h"
	//#pragma comment(lib, "C:/Program Files/EzSoftware UC/AXL(Library)/Library/32Bit/AxL.lib")
	#pragma comment(lib, "C:\\Program Files\\EzSoftware UC\\AXL(Library)\\Library\\32Bit\\AxL.lib")
	//#pragma comment(lib, "C:/Program Files (x86)/EzSoftware RM/AXL(Library)/Library/32Bit/AxL.lib")
#endif


#define BOARD_CNT	1

#define ABS			1	
#define REL			0	

#define SERVO		0		
#define STEPING		1


//#define ORG_INITIAL_SETTING		//! �ʱ� ���� Setting ���� ���� �ӵ� ����.
#define USE_LED_CTRL_BACKDU		//! ��ο�ƼĮ ���� ����� ���.(���� �������� �����Ʈ�ѷ� ���)

#define MOTOR_ACC_TYPE_SEC	
#ifdef	MOTOR_ACC_TYPE_SEC	
	#define ACC_CAL		1.0
#else
	#define ACC_CAL		9.8 * 1000
#endif

enum	MOTOR_NUM							{Motor_PCB_X,		Motor_PCB_Y,		Motor_PCB_Z,	Motor_PCB_TH,			Motor_PCB_Yt,	Motor_PCB_Xt,
											Motor_Lens_X,		Motor_Lens_Y,		Motor_Lens_Yt,  Motor_Lens_Xt, Motor_Lens_Z, MAX_MOTOR_NO};

static int  MOTOR_SPEED_MAX[MAX_MOTOR_NO] = { 500,				1000,				500,				1000,				500,		500,				
500,				1000,				500,			500,				500  };

static int  MOTOR_TYPE[MAX_MOTOR_NO] = {
SERVO,				SERVO,				STEPING,				STEPING,			STEPING,		STEPING,
SERVO,				SERVO,				STEPING,			STEPING,			STEPING };

static long	MOTOR_HOME_DIR[MAX_MOTOR_NO] = { DIR_CW,			DIR_CCW,			DIR_CCW,				DIR_CW,				DIR_CW,     DIR_CCW,			
DIR_CW,				DIR_CCW,				DIR_CW,				DIR_CW,				DIR_CW };

static long MOTOR_HOME_SIG[MAX_MOTOR_NO] = { 
										HomeSensor,		HomeSensor,			NegEndLimit,			HomeSensor,			HomeSensor,		HomeSensor,			
											HomeSensor,			HomeSensor,			HomeSensor,			HomeSensor,			PosEndLimit };
static char MotorName[MAX_MOTOR_NO][50]	= {	"PCB X[0]",		"PCB Y[1]",		"PCB Z[2]",	"PCB ��[3]",		"PCB Ty[4]", 		"PCB Tx[5]",
"Lens X[6]",		"Lens Y[7]",			"Lens Ty[8]"	,	"Lens Tx[9]",	"Lens Z[10]"};








//! DIO Module�� ����, Added by LHW (2013/4/23)
#define MAX_MODULE_CNT 2 
enum	DIO_REF		{ DIO_OFF = 0,		DIO_ON};
enum	GRIP_IO		{ GRIP_DOWN = 0,	GRIP_UP};					//! Tray GRIP �ϰ�(0), ���(1)
enum	SENSOR_IO	{ SENSOR_OFF = 0,	SENSOR_ON};					//! ��ȣ OFF(0), ON(1)
enum	CYLINDER_IO	{ BACKWARD = 0,		FORWARD};					//! �Ǹ��� ����(0), ����(1)
enum	UPDOWN_IO	{ PICK_DOWN = 0,	PICK_UP};					//! �Ǹ��� �ϰ�(0), ���(1)
enum	VACUM_MODE  { VACCUM_OFF=0, VACCUM_ON, VACCUM_BLOW, VACCUM_BLOW_ON };
enum	TURN_IN		{ TURN_0 = 0,		TURN_90, TURN_180};			//ȸ�� 0��(0), 90��(1), 180��(2)
enum	SWITCH_TYPE	{ IN_SWITCH = 0,	OUT_SWITCH};				//����ġ ���ԿϷ�(0), ����Ϸ�(1)
enum	BUZZER		{ BUZ_ALLOFF=0, BUZ_1, BUZ_2, BUZ_3, BUZ_4};	//���� �˶�(��Ī �ٲܰ�)
enum	TOWER_LAMP	{ LAMP_ALL_OFF=0, LAMP_R, LAMP_Y, LAMP_G};		//Tower Lamp ����
enum	MODE_SWITCH	{ SWITCH_ALL_OFF=0, SWITCH_START, SWITCH_STOP, SWITCH_ALARM};	// Main Mode Switch 

#define		MOTOR_JOG_LOW			0.1				//JOG_SPEED_LOW
#define		MOTOR_JOG_MID			0.5				//JOG_SPEED_MID
#define		MOTOR_JOG_HIGH			1.0				//JOG_SPEED_HIGH

#define SleepTimeDelay 5
#define ORG_PROC_TIME	40000


