#pragma once

#include "algo_base.h"
//#include "../../Library/Inspection/Include/LibACMISCommon/ACMISCommon.h"
//#include "../../Library/Inspection/Include/LibACMISSoftISP/SoftISP.h"
//#include "../../Library/Inspection/Include/LibACMISImage/ACMISImageDef.h"
////
//#include "../../Library/Inspection/Include/LibACMISImage/ACMISImageBlemish.h"
//#include "../../Library/Inspection/Include/LibACMISImage/ACMISImageDarkTest.h"
//#include "../../Library/Inspection/Include/LibACMISResolution/ACMISResolutionSFR.h"
////
//#include "../../Library/Inspection/Include/LibACMISShading/ACMISShadingDef.h"
//#include "../../Library/Inspection/Include/LibACMISShading/ACMISShadingAlgorithm.h"
//#include "../../Library/Inspection/Include/LibACMISShading/ACMISShadingDeviation.h"
////
//#include "../../Library/Inspection/Include/LibACMISImage/ACMISSignalNoiseRatio.h"
//#include "../../Library/Inspection/Include/LibACMISResolution/ACMISFiducialMark.h"
//
//#include "../../Library/Inspection/Include/LibACMISSoftISP/RawImageProcessing.h"
//
//#include "../../Library/Inspection/Include/LibACMISImage/ACMISSignalNoiseRatio.h"



typedef struct __TStainSpec
{
    TBlackSpotContrastN stSpecBlackSpot;
    TLCBSpecN stSpecLCB;
    TRU_YmeanSpecN stSpecYmean;
} TStainSpec;


class CPRIFunc_Insp
{
public:
	CPRIFunc_Insp(void);
	~CPRIFunc_Insp(void);
	
	void SetUnit(int nUnit){ m_nUnit = nUnit;}

	bool func_Insp_OpenStain(IplImage* img, bool bAutoMode = false);

	bool func_insp_CDP800_Current();
	bool func_Insp_CurrentMeasure(bool bLogDraw=true, bool bAutoMode = false);			//전류 측정 검사
	int  func_Insp_TestPattern(bool bAutoMode = false);				//Test Pattern 검사
	CString SetDir_Check(CString sPath);

	bool func_Insp_OpenStainLGIT(unsigned char* pImgBuff, bool bAutoMode = false);
	int InspectStain(TDATASPEC &tDataSpec, TStainSpec &tStainSpec, unsigned char *pBuffer, int nWidth, int nHeight, bool bEnableBlackSpot = true, bool bEnableLCB = true, bool bEnableRYUmean = true, bool bAutoMode = false);
	int InspectBlackSpotContrast(const BYTE* pBuffer, int nWidth, int nHeight, TBlackSpotContrast& _Spec, EDATAFORMAT dataFormat, EOUTMODE outMode, ESENSORTYPE sensorType, int nBlackLevel, IplImage *cvImgBlackSpot);
	//int InspectStainRUYMean(const BYTE* pBuffer, int nWidth, int nHeight, TRU_YmeanSpec& _Spec, EDATAFORMAT dataFormat, EOUTMODE outMode, ESENSORTYPE sensorType, int nBlackLevel, IplImage *cvImgYmean);
	int func_EEprom_Write(bool bAutoMode);
    //
    //190730 추가
	//
    //color uniformity
    bool func_Insp_Stain(BYTE* img, bool bAutoMode = false, bool bUse8BitOnly = false);

	//Blemish
    int InspectStain(BYTE* stainImg, bool bAutoMode);

	int BlackSpotInsp(BYTE* img, int nWidth, int nHeight, TDATASPEC& tDataSpec);// , IplImage* bmpImg);
    int LCBInsp(BYTE* img, int nWidth, int nHeight, TDATASPEC& tDataSpec);//, IplImage* bmpImg);
    int Blemish_YmeanInsp(BYTE* img, int nWidth, int nHeight, TDATASPEC& tDataSpec);//, IplImage* bmpImg);

	//oc
	bool func_Insp_Fov_Distortion_Rotate(BYTE* img, int _TYPE, bool bAutoMode = false);
    
	//distortion
	bool func_Insp_Distortion(BYTE* img, int nWidth, int nHeight, TDefectCluster& _Spec, EDATAFORMAT dataFormat, EOUTMODE outMode, ESENSORTYPE sensorType, int nBlackLevel, IplImage *cvImgDefect, bool bAutoMode = false);
    //Fov , Distortion , Rotate
    bool func_Insp_OC_Distortion(BYTE* img, int nWidth, int nHeight, TSNRBWSpecN& _Spec, EDATAFORMAT dataFormat, EOUTMODE outMode, ESENSORTYPE sensorType, int nBlackLevel, IplImage *cvImgDefect, bool bAutoMode = false);


	//R/C,B/C,Cr/Cb
	bool func_Insp_ColorSensitivity(BYTE* stainImg, int index, bool bAutoMode);

	//Relative Illumination (RI)
	bool func_Insp_Illumination(BYTE* img, bool bAutoMode = false, bool bUse8BitOnly = false);
	int Inspect_RelativeIllumination(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TRelativeIlluminationSpecN& _Spec, TDATASPEC& tDataSpec, IplImage *cvImgRI, bool bUse8BitOnly = false);
	int Inspect_RelativeIlluminationX(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TRelativeIlluminationSpecX& _Spec, TDATASPEC& tDataSpec, IplImage *cvImgRI, bool bUse8BitOnly = false);
	int Inspect_RelativeUniformity(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TRelativeUniformitySpec& _Spec, TDATASPEC& tDataSpec, IplImage *cvImgRU, bool bUse8BitOnly = false);
	int Inspect_ColorSensitivity(const BYTE* pBuffer, bool bUse8BitOnly = false);
	
	//SNR
	bool func_Insp_Snr_Vne(BYTE* ChartRaw, BYTE* ChartSecondRaw, bool bAutoMode);

	bool func_Insp_Snr(BYTE* img,bool bAutoMode = false);
	void CPRIFunc_Insp::DisplaySNRGraphics(IplImage* _cvImgBuf, const TSNRResult* _SNRResult, int _nImageWidth, CvScalar color);

	//dark , white , hot
	bool func_Insp_Defect(BYTE* midImage, BYTE* lowImage, bool bAutoMode = false);

	bool func_Insp_Saturation(BYTE* ChartRawImage, bool bAutoMode = false);

	//color uniformity
	bool func_Insp_Uniformity(BYTE* img,  bool bAutoMode = false, bool bUse8BitOnly = false);
	//Dark
	bool func_Insp_Dark(IplImage* img, bool bAutoMode = false);
	
	
	//
public:
	IplImage* imagePattern;			// Display // test by PRI


	int  m_iPatternTest_ErrCnt;

#ifndef _M_X64
	CAlgoPixelDefectInspection* m_pAlgoPixelDefectInspection;
	CAlgoStainInspection* m_pAlgoStainInspection;
	CAlgoUniformityInspection* m_pAlgoUniformityInspection;
#endif

private:
	int m_nUnit;
};
