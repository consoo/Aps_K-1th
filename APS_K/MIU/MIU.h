#pragma once

//! LPMC-500 Frame Grabber ���ۿ� Ŭ����
//! 1���� LPMC-500 Frame Grabber�� ������ �� �ֵ��� �Ѵ�.
//! Modified by LHW (2013/2/25)
//! ���� �۾��� : 2013/5/22, by LHW

#include "../stdafx.h"
#include "mil.h"
#include <cv.h>
#include <highgui.h>
#include "PixelFormat.h"
//#include "LPMC500DLLEx.h"
#include "BoardControl.h"
//
//#define RGGB	1
//#define GRBG	2
//#define GBRG	3
//#define BGGR	4
//
//#define RCCC	5
//#define CRCC	6
//#define CCRC	7
//#define CCCR	8

typedef enum IMAGE_FILE_TYPE_t {
	IMAGE_TYPE_RAW = 0,
	IMAGE_TYPE_BMP,
	IMAGE_TYPE_JPG,
	IMAGE_TYPE_AVI_BMP,
	IMAGE_TYPE_AVI_SELECT
} IMAGE_FILE_TYPE;

//! Sony, IMX135-0APH5-L, 13M, CMOS, Sensor���� ����ϴ� ���� ����ü
//! Sony, IMX179QQH5-C, 8M, CMOS, Sensor������ ������ �ּҰ��� ������ ���� Ȯ�� (2013/4/23)
//! Exposure Time ���� ���� ����ü

//! Sony, IMX135-0APH5-L, 13M, CMOS, Sensor���� ����ϴ� ���� ����ü
//! Sony, IMX179QQH5-C, 8M, CMOS, Sensor������ ������ �ּҰ��� ������ ���� Ȯ�� (2013/4/23)
//! Exposure Time ���� ���� ����ü
typedef struct _Info_IMX135 {
	//unsigned short iMainClock;

	char bFlag_Valid;	//! ��ȿ�� �������̸� '1', �׷��� ������ '0'

	unsigned short iPrePLLClk_OP_Div;	//! Register Address : 0x0305
	unsigned short iPLL_OP_MPY;			//! Register Address : 0x030C, 0x030D
	unsigned short iVTSYCK_Div;			//! Register Address : 0x0303
	unsigned short iVTPXCK_Div;			//! Register Address : 0x0301

	unsigned short iFrame_Length;		//! Register Address : 0x0340, 0x0341
	unsigned short iLine_Length;		//! Register Address : 0x0342, 0x0343

	unsigned short iIntegrationTime_Coarse;	//! Register Address : 0x0202, 0x0203
	unsigned short iIntegrationTime_Fine;	//! Register Address : 0x0200, 0x0201 (Read Only)

	//! ���� us
	int iExposureTime_Min;		//! Exposure Time�� ������ �ּҰ�
	int iExposureTime_Max;		//! Exposure Time�� ������ �ִ밪
	int iExposureTime_Current;	//! Exposure Time�� ���簪
} Info_IMX135, *PInfo_IMX135, *LPInfo_IMX135;

typedef struct _Info_IMX247 {
	//unsigned short iMainClock;

	char bFlag_Valid;	//! ��ȿ�� �������̸� '1', �׷��� ������ '0'

	unsigned short iPrePLLClk_OP_Div;	//! Register Address : 0x0305
	unsigned short iPLL_OP_MPY;			//! Register Address : 0x030C, 0x030D
	unsigned short iVTSYCK_Div;			//! Register Address : 0x0303
	unsigned short iVTPXCK_Div;			//! Register Address : 0x0301

	unsigned short iFrame_Length;		//! Register Address : 0x0340, 0x0341
	unsigned short iLine_Length;		//! Register Address : 0x0342, 0x0343

	unsigned short iIntegrationTime_Coarse;	//! Register Address : 0x0202, 0x0203
	unsigned short iIntegrationTime_Fine;	//! Register Address : 0x0200, 0x0201 (Read Only)

	//! ���� us
	int iExposureTime_Min;		//! Exposure Time�� ������ �ּҰ�
	int iExposureTime_Max;		//! Exposure Time�� ������ �ִ밪
	int iExposureTime_Current;	//! Exposure Time�� ���簪
} Info_IMX247, *PInfo_IMX247, *LPInfo_IMX247;

/******************************************************************************
*
*   MIU Device ����ü 
*
*******************************************************************************/
typedef struct strMIUDevice
{
	unsigned char		bMIUOpen ;			//! LPMC-500 Frame Grabber�� ����Ǿ� ������ '1', �ƴϸ� '0'
	unsigned char       bMIUInit;			//! LPMC-500 Frame Grabber�� �ʱ�ȭ �Ǿ� ������ '1', �׷��� ������ '0'
	unsigned char       bReadyVCM;			//! VCM Driver ��� �غ� �Ϸ�Ǹ� '1', �׷��� ������ '0'
	unsigned char		CurrentState;		//	-1:���� ȹ�� Thread ���� ����.
											//	0: NULL, 
											//	1: �ʱ�ȭ �Ϸ� (Frame Grabber�� CCD Camera Sensor ��� �ʱ�ȭ �Ϸ��ؾ� '1'�� �ȴ�.), 
											//	2: ī�޶�� ������ ���α׷����� �ƹ��͵� ����. Multi Thread �Լ����� �Ϻη� ���� ����� ���� �ʰ� �ִ� ����
											//	3: ���� ���� ȹ��
											//	4: ������ Display
	
	unsigned char       bBayerFormat;		//! Bayer Color Format�̸� '1', �׷��� ������ '0'

	SYSTEMTIME			TimeGrab;			//! ���� ������� ���� �ð�

	Info_IMX135			IMX135;				//! Ư�� CCD Sensor ���� ���� ����ü

	Info_IMX247			IMX247;				//! Ư�� CCD Sensor ���� ���� ����ü

	float				fCcdSize;
	
	//MIU_INITIALValue	InitialValue;
	unsigned short		iSensorID;			// Sensor Product ID
	unsigned short		iSensorType;		// Sensor Resolution
	unsigned int		iPixelFormat;		// Sensor Out Format
	unsigned short		nMaxWidth;			// Max Width
	unsigned short		nMaxHeight;			// Max Height   
	int					nWidth;				// Sensor Width//unsigned short		nWidth;				// Sensor Width
	int					nHeight;			// Sensor Height    
	unsigned short		nBitPerPixel;		// Bit Per Pixel
	unsigned int		nFrameImageSize;	// 1 Frame Image size  
	unsigned int		nFrameRawSize;		// 1 Frame RAW size (Only Data Size)

	int					nDataFormat;
	int					nOutMode;
	int					nSensorType;

	IplImage*			imageItp;			// Display 
	char				szWindowName[256];
	
	unsigned char		fResizeFlag;
	unsigned short		nDisplayWidth;  // Display Width
	unsigned short		nDisplayHeight; // Display Height

	unsigned char		iImageSave;     // Image ���� ���� flag 0:���� ����, 1: Still Capture, 2:������ ���� 
	unsigned char		fSaveNewFrame;  // ������ Image�� �ִ��� �˷��ִ� flag

	unsigned short		*pBuffer;


	TDATASPEC dTDATASPEC_n;
} MIU_DEVICE, *PMIU_DEVICE;



//=================================================================================================
// Structure of sensor's information
//=================================================================================================
typedef struct _SENSOR_INFORMATION
{
	char			szResolution[256];		//  Resolution 
	int				nOutputWidth;           //  Image size Width
	int				nOutputHeight;          //  Image size Height
	unsigned int	iPixelFormat;           //  Pixel Format
}SENSOR_INFORMATION;


//=================================================================================================
// Structure of Sensor List
//=================================================================================================
typedef struct _IMAGESENSOR_LIST
{
	char				szSensorName[256];	// Sensor Name
	int					nInfoCount;         // Info Count
	SENSOR_INFORMATION* pInfo;	            // Sensor Information Pointer
}IMAGESENSOR_LIST;

//! Display FrameRate ������ ����ü
struct strDiagInfo_Display
{
	int     iCnt_Display;
	double	dGrabStart;	
};

//! ��� �ð� ������ Ŭ����
const int iBufSize_Measure_Time_Elapsed = 100;
class CMeasureTimeElapsed
{
public:
	CMeasureTimeElapsed();
	~CMeasureTimeElapsed();

private:
	bool				m_bFlag_Valid_Freq;
	LARGE_INTEGER		m_liFreq;
	LARGE_INTEGER		m_liStartCounter;
	LARGE_INTEGER		m_aliCounter[iBufSize_Measure_Time_Elapsed];
	
public:
	double				m_adTime[iBufSize_Measure_Time_Elapsed];

	inline bool IsSuccess_Start_Time() { return m_bFlag_Valid_Freq; }

	void   Init_Time();
	bool   Start_Time();
	double Measure_Time(int iIndex_Time);
};


//! <-------------------------------------------------------------------------------------------------------------------
//! 50�� ������ ���۸� �����Ϸ���, �޸� �Ҵ� �߿� ���α׷��� �ٿ�� ������
//! ó������ 6�� ������ ���۸� ����ϰ�, �ʿ��ϸ� 1�� �� �÷��� ��
const int iBufSize_CCM_Temp = 4;
//! <-------------------------------------------------------------------------------------------------------------------

//! ���� ��� ��� �ð� ����� ������ ũ��
const int iBufSize_CCM_Time = 20;

//! ��� �̹����� Averaging ó���� ���� ������ ����
const int iBufSize_BW_Avg = 1;


class CMIU
{
public:
	CMIU();
	~CMIU();

public:
	bool				m_bFlag_Color_Covert_Ext;	//! Laon People�翡�� ������ Color ��ȯ �Լ��� ������� ���� �� true�� ����
	bool				m_bFlag_Mil_Convert;		//! Mil ���� ��ȯ ����� �����ϸ� true�� ����
	bool				m_bFlag_Averaging;			//! Add_Sum_To_Buffer, Get_Average_From_Buffer �Լ��� ����Ϸ��� true�� ����
	
	//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int                 m_iDeviceCount;		//! ����� LPMC-500 Frame Grabber�� ����

	IMAGESENSOR_LIST*   m_pSID;				//! 'ImageSensor.ini' ���Ͽ��� �о�� CCD Sensor�� ��� ����
	int                 m_iSensorListCnt;	//! 'ImageSensor.ini' ���Ͽ��� �о�� CCD Sensor�� ����
		 
	BOOL                m_ubBurst;			//! CcmInit �Լ����� ���, CCD Sensor���� Registry�� �Ѳ����� �Է°��� ���� �� ���� �� true�� ����

	unsigned char		m_cInputCurrent;	//! Open/Short �˻翡���� �Է� ������ (Range 0 ~ 200, �ʱⰪ 150)

	IplImage*	        m_acvChildImage[3];	//! �ӽ� ����, Open CV <----> Mil ���� ��ȯ�� �ʿ��ϴ�. 

	CString             sLogMessage;		//! ���� ���ڿ�

	float				m_fRate_Display;	//! Display�� Frame Rate ��°�

	CCriticalSection	m_csGrabLock;		//! ���� ����� �����ǰ� Stop�ϰ� �ϱ� ���� �߰�, ���� ��� ���� ���� ����

	//! <-------------------------------------------------------------------------------------------------------------------
	//! Multi Thread �Լ��� �̿��Ͽ�, Display Frame/sec�� �ִ��� �ø��� ���� Queue�� ���ۿ� ���� ������ ���� index
	//! m_aTemp�� index, 0 based
	int                 m_iIndex_Grab_Working;		//! ���� ����� �����̰ų�, ���� ��� ���� ������ index, 0 based
	int                 m_iIndex_Grab_Ready;		//! ���� ����� �Ϸ�ǰ�, Color ��ȯ �غ� �Ϸ�� ������ index, 0 based
	int					m_iIndex_Grab_Used;			//!	Color ��ȯ�� �Ϸ�� ������ index, 0 based

	int					m_iIndex_Cvt_Clr_Working;	//! Color ��ȯ ���� ������ index, 0 based
	int					m_iIndex_Cvt_Clr_Used;		//! Color ��ȯ �Ϸ�� ������ index, 0 based

	int					m_iIndex_Cvt_Mil_Working;	//! Open CV --> Mil ���� ��ȯ ���� ������ index, 0 based
	int					m_iIndex_Cvt_Mil_Used;		//! Open CV --> Mil ���� ��ȯ �Ϸ�� ������ index, 0 based

	int					m_iIndex_Display_Working;	//! ȭ�鿡 ǥ�õ� �����̰ų�, ȭ�鿡 ǥ�� ���� ������ index, 0 based

	MIU_DEVICE			m_aTemp[iBufSize_CCM_Temp];	//! �̹��� ������ ��ü, ����, Color ��ȯ�� ���۸� ��� ������ �ִ�.
	
	//! Thread�� ���е� ���� ��� ���� ������ ��� �ð�
	double				m_adTime_Grab[iBufSize_CCM_Time]; //! ���� msec	
	//! <-------------------------------------------------------------------------------------------------------------------

	BOOL m_ChkBurst;

	
public:	
	//! 'ImageSensor.ini' ���Ͽ��� CCD Sensor�� ��� ������ �о�´�.
	bool SensorListInitialize();
	void setInterface();
	void ImageBufferSet();
	//! PC�� USB 3.0���� ����� LPMC-Frame Grabber�� ã�ƺ���.
	int  Search_Device();

	//! Model�� ����� ������ ȣ���� �־�� �Ѵ�. 
	//! ����, LPMC-800 Frame Grabber�� �̹� Open�Ǿ� �ִٸ�, Model ���� �Ŀ� �� �Լ��� �ݵ�� ȣ���� �־�� �Ѵ�. 
	bool SelectSensor();

	void CMIU::GetImagePixelFormat(char* szPixelFormat, unsigned int* nPixelFormat);

	//! ���� ���õ� LPM-500 Frame Grabber�� ��ġ index�� ��ȯ�Ѵ�. 0 based
	inline int GetIndexDevice() { return m_iIndexDevice; }

	//! Display Frame Rate ������ �Լ�
	//! [���� ����] �ݵ��, Display ���Ŀ� ȣ���Ѵ�. (Display�� ���� ���� ��쿡�� ���� ��� ���� ���� �ܰ迡�� ȣ���Ѵ�.)
	void Add_Display_Count();
	//! Display Frame Rate �ʱ�ȭ
	void Init_Display_Count();

	//! Thread�� ���е� ���� ��� ���� ������ ��� �ð� ���� ���� �Լ�
	void   Init_Grab_Time();
	bool   Init_Grab_Time(int fi_iIndex_Time_Grab, bool fi_bFlag_Init_Temp = false);
	bool   Start_Grab_Time(int fi_iIndex_Time_Grab);
	double Measure_Grab_Time(int fi_iIndex_Time_Grab);

	//! <---------------------------------------------------------------------------------------------------------------------------
	//! LPMC-500 Frame Grabber�� �����Ѵ�. 
	bool Open();
	//! Open/Short Test
	bool OSTest(int f_iOsCheckFlag[OS_PIN_COUNT], float	f_foOsUpperResult[OS_PIN_COUNT], float f_foOsLowerResult[OS_PIN_COUNT]);

	//! LPMC-500 Frame Grabber�� CCD Sensor�� ��� �ʱ�ȭ �Ѵ�. 2������ ��� �ʱ�ȭ �ؾ� ���� ����� �� �� �ִ�.
	bool CcmInit();
	bool CCM_Init();

	//! LPMC-500 Frame Grabber�� �ʱ�ȭ
	//! [���� ����] OIS (VCM) ���� ����� ����ϱ� ���ؼ��� Frame Grabber�� �ݵ�� �ʱ�ȭ�ؾ� �Ѵ�. 
	bool MIU_Init();
	//! CCD Camera Sensor�� �ʱ�ȭ�� �����Ѵ�.
	bool CCD_Init();
	//! ���� ����
	bool cvBufferAlloc();

	//! ���� ��� ����
	bool Start();

	//! gMIUDevice.imageItp ���� ���� ���� �Լ�
	bool cvCopyToMil(MIL_ID* f_pDestBuf);
	bool MilCopyToCv(MIL_ID* f_pDestBuf);

	//! ���� ����� ��ģ��.
	bool Stop(bool fi_bFlag_PowerOff = false);

	//! ���� ����
	void cvBufferFree();

	//! LPMC-500 Frame Grabber���� ������ ���´�. 
	bool Close();
	void SwitchHDRToLinearMode();
	void SwitchLinearToHDRMode();
	void ISPmodeCheck();
	//! <---------------------------------------------------------------------------------------------------------------------------

	//! Averaging ó���� �Լ�
	int Add_Sum_To_Buffer(IplImage* fi_cvInput, bool fi_bFlag_Start, int fi_iIndex_Avg = 0);
	int Get_Average_From_Buffer(IplImage* fo_cvOutput, int fi_iIndex_Avg = 0);

	//! Color ��ȯ�� ���
	void Shift10BitMode(unsigned char* pImage, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);
	void Shift12BitMode(unsigned char* pImage, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);
	void Shift12to16BitMode(unsigned char* p12bitRaw, unsigned char* p16bitRaw ,int nWidth, int nHeight);
	void CopyBuffer(unsigned char* pImage, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);
	void CCCR_Interpolation(unsigned short* pData, int nWidth, int nHeight);

	//! ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//! CCD Sensor�� ����
	//! IMX135-0APH5-L CMOS, Sony, 2013/3/13
	//! ���� Gain ����
	bool SetRegister_WhiteBalance_Red_IMX135  (double fi_dGain);
	bool SetRegister_WhiteBalance_Green_IMX135(double fi_dGain);
	bool SetRegister_WhiteBalance_Blue_IMX135 (double fi_dGain);
	bool SetRegister_WhiteBalance_Red_IMX135  (CString fi_sGain);
	bool SetRegister_WhiteBalance_Green_IMX135(CString fi_sGain);
	bool SetRegister_WhiteBalance_Blue_IMX135 (CString fi_sGain);
	//! Exposure Time ���� (Added by LHW, 2013/3/14)
	bool GetCurrentInfo_Exposure_From_CCD_IMX135();
	bool Set_ExposureTime_To_CCD_IMX135(int fi_iExposureTime);	//! fi_iExposureTime : Exposure �ð�, ���� usec
	//! ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool CMIU::SetValue_VCM_Direct(BYTE fi_Byte1, BYTE fi_Byte2);

	//! ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//! OIS
	//! ��� ����̹� : DW9714 - 10bit Resolution VCM driver IC with I2C interface (DONGWOON ANATECH), Ver 1.93
	//! Lens ��� ���� �Լ�
	//! 2013/5/22 �߰� (Added by LHW)
	//! 
	//! Laon People�翡 �����غ� �亯
	//!  1) VCM Control �����Ͽ� ���ο� IIC �Լ����� ���ο� FPGA Version�� ����� LPMC-500CV BoardVersion������ ��밡�� 
	//!	 2) ���� Sensor�� IIC ����ϴ� Pin�� �ٸ��� GPIO Pin�� �̿��Ͽ� OIS Board�� IIC ���
	//!	 3) DLL�� Version�� 1.0.0.4
	//!	 4) VCM�� ���� �ΰ��� MIUInitialize()�Լ��� ȣ���ؾ� ������ �ΰ� �ǵ��� �Ǿ� ������, 
	//!		MIUOpenDevice()�Լ��� MIUInitialize()�Լ� ȣ��ڿ� ��� ���� (Ŭ���� ���� Open, MIU_Init �Լ� ȣ�� �� ��� ����)
	//!  5) ���� ����Ŀ� ��� ����, �߰��� OIS IIC �Լ� ȣ��ڿ� Sensor�� �ٽ� ������ �ʿ�� ����
	//!  6) FPGA�� DLL�Լ��� VCM���� IIC ����� �Ҽ� �ֵ��ϸ� ���� �Ǿ� ������ ���ο��� ���� 'Slew Rate Control' ���� ó���� ���� ����
	//!  7) Data input bit (D0 ~ D9)�̿��� bit ����� ���� 'MIUWriteOIS' �Լ��� �Է��ؾ� ��

	//! VCM Diver ��� �غ�
	bool ReadyVCM();
	//! mA ������ ���� �Է¹޾� ����
	bool MoveVCM_Current(int current);

	//! �Ʒ��� �Լ����� �ݵ�� �ʿ��� ���� ����Ѵ�. 

	//! code������ �Է¹޾� ����
	bool MoveVCM_Code(int code);
	//! �Էµ� code���� �Էµ� BYTE���� bit�� �����Ͽ� ��ȯ�Ѵ�.
	void Apply_Data_Code_VCM(BYTE fi_Byte1, BYTE fi_Byte2, BYTE &fo_Byte1, BYTE &fo_Byte2, int fi_code);
	//! �Էµ� BYTE���� bit���� code���� �˾Ƴ���.
	void Get_Data_Code_VCM(BYTE fi_Byte1, BYTE fi_Byte2, int &fo_code);
	//! �Էµ� BYTE���� ���� �����Ѵ�. 
	//! ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
private:
	char m_acIndexDevice[4];	//! PC�� USB 3.0�� ����� LPMC-500 Frame Grabber�� ���� index��
	int  m_iIndexDevice;		//! ����� LPMC-500 Frame Grabber�� ���� index, 0 ~ 3

	int	 m_iSensor;				//! ���� ���õ� CCD Sensor�� index, 0 based, m_pSID�� 1��° index
	int	 m_iSensorType;			//! ���� ���õ� CCD Sensor�� index, 0 based, m_pSID�� 2���� index

	//! Display Frame Rate ������ ����ü
	strDiagInfo_Display m_strDisplayDiag;
	CMeasureTimeElapsed m_aMeasureTimeForDisplayFrameRate;

	//! Thread�� ���е� ���� ��� ���� ������ ��� �ð�
	CMeasureTimeElapsed m_aMeasureTime[iBufSize_CCM_Time];



private:
	//! m_pSID�� �޸� ����
	void SensorList_Free();

	//! LPMC-500 Frame Grabber�� CCD Sensor������ Interface pin�� ��ȣ�� �ٽ� �����.
	int  OsTest_PinMapping(unsigned char ucSensorMode, float* TestResult);

	//! Multi Thread ���� ��� ���� index �ʱ�ȭ
	void Init_Index_Grab();
	void Start_Index_Grab();
// PRI Grabber Porting
public:
	bool bFlag_RawImageSave;
	int iImgUpdateFlag;
	HANDLE			m_hImgViewThread ;
	DWORD			m_dwImgViewThreadID;
	IplImage	*pImageBuf[3];



	int				preIdx ;
	int	m_PreIdx ;
	float m_FrameRate;
	void ReadRawImg(CString strInFileName,int nSizeX,int nSizeY,int nChannel,IplImage *outImage);
	void ReadRawImg2(CString strInFileName,int nSizeX,int nSizeY,int nChannel,IplImage *outImage);
	void SaveSfrImage(unsigned char* ucImage, int pitch, int sizeX, int sizeY, int index, CRect rcRoi);
	int GetCurrent ( float *pPwrCur );
	int Grab_StopImage ();
	int StopLive ();
	int TestPattern_CHK();
	int ImageShow();
    void RawImageSave(CString str);


	bool m_bFlagImgReady;
	bool m_bFlagImgUsed;	// ���÷��� �����尡 ���ų� �����̹��� memset �϶�
	bool m_bThreadUsed;	
	bool b_BufCopyflag;
	void RawToBmp2(unsigned char *pRaw8,unsigned char *pBMP,int image_width,int image_height,int bayer_format);
	//void RawToBmp2(unsigned short *pRaw8,unsigned char *pBMP,int image_width,int image_height,int bayer_format);
	int SaveRawImage();

	int iUseClic;
	void PRI_12BitToRaw8 (unsigned char *p12Bit, unsigned char *pRawBit8 );

	void SetUartHead();
	
	unsigned char *pBmpBuffer;
	unsigned char *pImageData;
	unsigned char *pImageTemp;
	unsigned char *pImageTemp1;
	unsigned char *pImageTemp2;
	
	
	//-- 20140907 Test Pattern ����(PRI �輼�� ������)
/*
TestPattern Check() �Լ� ���� ����
1. Image Grab ���¿��� ���� �Ǿ�� �մϴ�.
2. ������ Test Pattern Mode�� �ٲ� �� �� �ֽ��ϴ�.
3. �������� �̹����� ���� Ȥ�� ������ ���� �����մϴ�.
4. �׽�Ʈ ���� ��, �Լ� �������� ������ Image Mode�� �ٲ��� �� �ֽ��ϴ�.
*/
	//bool m_bImageRcving;

	BITMAPINFO* pBmpInfo;
	bool m_bFlagTestPattern_test;
	int  m_iPatternTest_ErrCnt;
	BOOL m_Burst; 
	void func_Set_InspImageCopy(int nType = 0);	//�˻���۷� Image ����
	CCriticalSection m_acsInspImage;	//! �˻���۷� Image ��ȣ��
	void YuvToBmp(unsigned char *pRaw8,unsigned char *pBMP,int image_width,int image_height,int YUV_format);	// YUV to RGB

	int CMIU::Process_iniFile(char iDeviceIndex, CString strIniFile);

	float	fGrapStatTickCount;		// fps����



public:
	CString selectedIniFile ;
	//
	char m_cBoardIndex;
	IBoardControl*	m_pBoard;				// for dynamic loading
	HMODULE	 m_hBoardLibrary;
	CString m_strIniFileName;

	BYTE* m_pFrameRawBuffer;
	BYTE* m_pFrameBMPBuffer;
	BYTE* vChartBuffet;
	BYTE* vChart_Second_Buffet;
	BYTE* vDefectMidBuffer_6500K;	//�߰���� �̹��� (dark/Defect/Blemish/RI/Color Uniformity �����)
	BYTE* vDefectLowBuffer;			//���� ����(Hot Defect �����)
	BYTE* vDefectMidBuffer_2800K;	//�߰���� (Color Sensitivity �����)

	void disConnectedInit();
	void INI_LOAD();

};
