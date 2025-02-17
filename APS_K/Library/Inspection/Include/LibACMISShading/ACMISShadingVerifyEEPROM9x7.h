#pragma once

#include "export.h"

//bins
// DO NOT INCLUDE BLEMISH HEADER FILE
//#define LSC_DATA_NUM		884	//bytes
//#define LSC_ROW_NUM			13
//#define LSC_COL_NUM			17
//#define LSC_CHANNEL_DATA	221 // (=LSC_ROW_NUM*LSC_COL_NUM)
#define LSC_DATA_NUM		252	//bytes
#define LSC_ROW_NUM			7
#define LSC_COL_NUM			9
#define LSC_CHANNEL_DATA	63 // (=LSC_ROW_NUM*LSC_COL_NUM)
#define BAYER_CHANNEL_NUM	4	// R/Gr/Gb/B

typedef enum _EReturnMessage
{
	eRetMsgSpecFail=-1,	// spec fail
	eRetMsgJudgeFail=0,	// FAIL
	eRetMsgJudgeOK=1,	// PASS
} ERetMsg;

typedef struct _TOutputYSensitivity
{
	int nMaxLvl[BAYER_CHANNEL_NUM];		// R/GR/GB/B
	int nMaxLvlCnt[BAYER_CHANNEL_NUM];
} TOutputYSensitivity;

typedef struct _TOutputNonLinearity
{
	int nMaxNonLinearityCnt[BAYER_CHANNEL_NUM];  // R/GR/GB/B
} TOutputNonLinearity;

typedef struct _TOutputCurvature
{
	int nMaxCurvatureCnt[BAYER_CHANNEL_NUM];  // R/GR/GB/B
} TOutputCurvature;

typedef struct _TOutputCurvatureEx
{
	int nMaxCurvatureDepth[BAYER_CHANNEL_NUM][2];  // R/GR/GB/B, center, edge
	int nMaxCurvatureCnt[BAYER_CHANNEL_NUM][2];  // R/GR/GB/B, center, edge	
} TOutputCurvatureEx;

typedef struct _TOutputSymmetric
{
	int nMaxLvlCenter[BAYER_CHANNEL_NUM]; // R/GR/GB/B
	int nMinLvlCorner[BAYER_CHANNEL_NUM];
	double dShadingMin[BAYER_CHANNEL_NUM];
	double dCornerDeltaMin[BAYER_CHANNEL_NUM];
} TOutputSymmetric;

typedef struct _TOutputBrightImage
{
	int nMaxLvl[BAYER_CHANNEL_NUM]; // R/GR/GB/B
} TOutputBrightImage;

typedef struct _TOutputDarkImage
{
	int nMinLvl[BAYER_CHANNEL_NUM]; // R/GR/GB/B
} TOutputDarkImage;

typedef struct _TOutputOpticalCenter	// Optical center
{
	int nOpticalCenter[BAYER_CHANNEL_NUM]; // R/GR/GB/B
} TOutputOpticalCenter;

typedef struct _TOutputFitCurveDeviation
{
	double dDevLvlMax;		// R/GR/GB/B
	double dDevLvlSum;

	double dDitance[LSC_CHANNEL_DATA*2];
	double dOriginLevel[LSC_CHANNEL_DATA*2];
	double dFitLevel[LSC_CHANNEL_DATA*2];
} TOutputFitCurveDeviation;

class ATOMSHADING_API CAtomShadingVerifyEEPROM
{
public:
	CAtomShadingVerifyEEPROM(void);
	~CAtomShadingVerifyEEPROM(void);

	ERetMsg YSensitivity( const unsigned short* pEepromBuf, int nDataNum, int nSpecMaxLevel, int nSpecMaxLvlCnt, TOutputYSensitivity* pstOutput=NULL);
	ERetMsg NonLinearity( const unsigned short* pEepromBuf, int nDataNum, int nSpecMaxCnt, TOutputNonLinearity* pstOutput=NULL );
	ERetMsg Curvature( const unsigned short* pEepromBuf, int nDataNum, int nSpecLvlDiff, int nSpecSkipChkZeroDiff, TOutputCurvature* pstOutput=NULL);
	ERetMsg Curvature_Center(const unsigned short* pEepromBuf, int nDataNum, int nSpecLvlDiff, int nSpecEdgeNum, int nSpecMaxCnt, TOutputCurvature* pstOutput=NULL);
	ERetMsg Curvature_Edge(const unsigned short* pEepromBuf, int nDataNum, int nSpecLvlDiff, int nSpecEdgeNum, int nSpecMaxCnt, TOutputCurvature* pstOutput=NULL);
	ERetMsg Symmetric( const unsigned short* pEepromBuf, int nDataNum, int nSpecCenterLimitLevel, double dSpecShadingMin, double dSpecCornerDeltaMin, TOutputSymmetric* pstOutput=NULL);
	ERetMsg DarkImage( const unsigned short* pEepromBuf, int nDataNum, int nSpecLvlMinRR, int nSpecLvlMinGR, int nSpecLvlMinGB, int nSpecLvlMinBB, TOutputDarkImage* pstOutput=NULL);
	ERetMsg BrightImage( const unsigned short* pEepromBuf, int nDataNum, int nSpecLvlMaxRR, int nSpecLvlMaxGR, int nSpecLvlMaxGB, int nSpecLvlMaxBB, TOutputBrightImage* pstOutput=NULL);
	ERetMsg OpticalCenter( const unsigned short* pEepromBuf, int nDataNum, int nSpecDecenterOffset, TOutputOpticalCenter* pstOutput=NULL);
	ERetMsg FitCurve( const unsigned short* pEepromBuf, const int nMaxRow, const int nMaxCol, double dSpecLvlDevMax, double dSpecLvlDevSum, double* pResultDistance, double* pResultOriginCurve, double* pResultFitCurve ,double dColumnRatio=0);
	ERetMsg FitCurveDeviation( const unsigned short* pEepromBuf, int nDataNum, double dSpecLvlDevMax, double dSpecLvlDevSum, TOutputFitCurveDeviation* pstOutput ,double dColumnRatio=0);

	ERetMsg CurvatureEx( const unsigned short* pEepromBuf, const char* pMap, int nDataNum, int nSpecCurvatureDepth, int nSpecCurvatureCnt, TOutputCurvatureEx* pstOutput);
};
