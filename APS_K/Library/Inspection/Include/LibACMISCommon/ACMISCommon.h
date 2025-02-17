#pragma once

#include <XGraphic\xDataTypes.h>
#include <vector>
#include <memory>
#include <iomanip>
#include <ctime>
#include <chrono>

#define DEBUG_FILE_SAVE_PATH	"C:\\Users\\User\\Desktop"

typedef enum _EOUTMODE
{
	OUTMODE_BAYER_BGGR = 1, // Bayer
	OUTMODE_BAYER_RGGB,
	OUTMODE_BAYER_GBRG,
	OUTMODE_BAYER_GRBG,
	OUTMODE_BAYER_BLACKWHITE,
	OUTMODE_YUV422_YCbYCr = 1, // YUV422
	OUTMODE_YUV422_YCrYCb = 2,
	OUTMODE_YUV422_CbYCrY = 3,
	OUTMODE_YUV422_CrYCbY = 4,
	OUTMODE_RGB_BGR = 1, // RGB
	OUTMODE_RGB_RGB = 2,
	OUTMODE_BAYER_BNGR = 1, // RGB-IR Uniform pattern
	OUTMODE_BAYER_RGNB,
	OUTMODE_BAYER_NBRG,
	OUTMODE_BAYER_GRBN,
	OUTMODE_BAYER_BGGN = 1,	// RGB-IR Pattern 2
	OUTMODE_BAYER_RGGN,
	OUTMODE_BAYER_GBNG,
	OUTMODE_BAYER_GRNG,
	OUTMODE_BAYER_NGGR,
	OUTMODE_BAYER_NGGB,
	OUTMODE_BAYER_GNRG,
	OUTMODE_BAYER_GNBG,
} EOUTMODE;

typedef enum _EYCONVERTMETHOD
{
	Y_CONVERTMETHOD_ITU_R_601,
	Y_CONVERTMETHOD_ITU_R_709
} EYCONVERTMETHOD;
#define ITU_R_709_Y		0x80
#define OrgOutMode(x)	((EOUTMODE)(x & ~ITU_R_709_Y))

typedef enum _EDATAFORMAT
{
	DATAFORMAT_YUV,
	DATAFORMAT_BAYER_8BIT,
	DATAFORMAT_BAYER_10BIT,
	DATAFORMAT_BAYER_PARALLEL_10BIT,
	DATAFORMAT_BAYER_12BIT,
	DATAFORMAT_BAYER_PARALLEL_12BIT,
	DATAFORMAT_BAYER_16BIT,
	DATAFORMAT_BAYER_16BIT_BE,
	DATAFORMAT_BAYER_16BIT_PWL_HDR,
	DATAFORMAT_BAYER_16BIT_PWL_DECOMP,
	DATAFORMAT_BAYER_24BIT,
	DATAFORMAT_BAYER_PARALLEL_10BIT_BE,
	DATAFORMAT_BAYER_PARALLEL_12BIT_BE,
	DATAFORMAT_RGB = 100
} EDATAFORMAT;

typedef enum _ESENSORTYPE
{
	SENSORTYPE_RGGB = 0,
	SENSORTYPE_RCCC,
	SENSORTYPE_RCCB,
	SENSORTYPE_CCCC,
	SENSORTYPE_RBCC,
	SENSORTYPE_OTHER,
	SENSORTYPE_RGBIR2,
	MAX_COUNT_SENSORTYPE
} ESENSORTYPE;

#define SENSORTYPE_RGGB_VNG		8
#define SENSORTYPE_RGGB_G_ONLY	9

typedef enum _EDEMOSAICMETHOD
{
	DEMOSAICMETHOD_NONE = 0,
	DEMOSAICMETHOD_GRADIENT,
	DEMOSAICMETHOD_BL33, // Bilinear3x3
	DEMOSAICMETHOD_OPENCV_VNG,
	DEMOSAICMETHOD_VNG,  // Variable Number of Gradient
	DEMOSAICMETHOD_GR_ONLY,
	DEMOSAICMETHOD_GB_ONLY,
	DEMOSAICMETHOD_GBTF, // Gradient based Threshold Free
	DEMOSAICMETHOD_BLLC, // Bilinear Laplacian Corrected
	DEMOSAICMETHOD_RESIDUAL, // Residual Interpolation
	DEMOSAICMETHOD_BAYER_Y, // Y = (R + Gr + Gb + B) / 4
	DEMOSAICMETHOD_GRADIENT_RESIDUAL = 101, // G(Gradient) + R/B/IR(RI)
	DEMOSAICMETHOD_GBTF_RESIDUAL, // G(GBTF) + R/B/IR(RI)
	DEMOSAICMETHOD_RESIDUAL_RESIDUAL, // G(RI) + R/B/IR(RI)
	DEMOSAICMETHOD_BL33_RESIDUAL, // G(BL33) + R/B/IR(RI)
	DEMOSAICMETHOD_IR_ONLY,
	DEMOSAICMETHOD_GRADIENT_G_ONLY, // G(Gradient)
	DEMOSAICMETHOD_GBTF_G_ONLY, // G(GBTF)
	DEMOSAICMETHOD_RESIDUAL_G_ONLY, // G(RI)
	DEMOSAICMETHOD_BL33_G_ONLY, // G(BL33)
	DEMOSAICMETHOD_GRADIENT_IR_ONLY, // IR(Gradient)
	DEMOSAICMETHOD_GBTF_IR_ONLY, // IR(GBTF)
	DEMOSAICMETHOD_RESIDUAL_IR_ONLY, // IR(RI)
	DEMOSAICMETHOD_BL33_IR_ONLY, // IR(BL33)
	MAX_COUNT_DEMOSAICMETHOD
} EDEMOSAICMETHOD;

typedef enum _EImageIndex
{
	/* ONSEMI or OVT Sensor */
	EImageIndex_Bright = 0,
	EImageIndex_Dark,
	EMax_ImageIndex_Count,
	/* Sony Sensor */
	EImageIndex_Highlight_SP1L = 0,
	EImageIndex_Highlight_SP2L,
	EImageIndex_Dark_SP1L,
	EImageIndex_Dark_SP2H,
	EImageIndex_Dark_SP2L,
	EImageIndex_Saturated_SP1H,
	EImageIndex_Saturated_SP1L,
	EImageIndex_Saturated_SP2H,
	EImageIndex_Saturated_SP2L,
	EMax_Sony_ImageIndex_Count
} EImageIndex;

typedef struct _TDATASPEC
{
	EOUTMODE eOutMode;
	EDATAFORMAT eDataFormat;
	ESENSORTYPE eSensorType;
	EDEMOSAICMETHOD eDemosaicMethod;
	int nBlackLevel;
} TDATASPEC;

typedef struct _TBufferInfo
{
	BYTE *pBuffer;
	int nImageWidth;
	int nImageHeight;
	bool bUsing8BitOnly;
	bool bPartialDemosaic;
	TDATASPEC tDataSpec;
} TBufferInfo;

typedef struct _TBufferInfoM
{
	BYTE **pBuffer;
	int nBufferCount;
	int nImageWidth;
	int nImageHeight;
	bool bUsing8BitOnly;
	bool bPartialDemosaic;
	TDATASPEC tDataSpec;
} TBufferInfoM;

typedef struct _TFileInfo
{
	char *strFileName;
	bool bIsRaw;
	int nImageWidth;
	int nImageHeight;
	bool bUsing8BitOnly;
	bool bPartialDemosaic;
	TDATASPEC tDataSpec;
} TFileInfo;

typedef struct _TFileInfoM
{
	char **strFileName;
	bool bIsRaw;
	int nFileCount;
	int nImageWidth;
	int nImageHeight;
	bool bUsing8BitOnly;
	bool bPartialDemosaic;
	TDATASPEC tDataSpec;
} TFileInfoM;

struct TColorRatio
{
	double RG;
	double BG;
	double GrGb;
	double R;
	double G;
	double B;
	double Gr;
	double Gb;
	TColorRatio(double _RG = 0, double _BG = 0, double _GrGb = 0, double _R = 0, double _G = 0, double _B = 0)
		: RG(_RG), BG(_BG), GrGb(_GrGb), R(_R), G(_G), B(_B), Gr(_G), Gb(_G)
	{
	}
};

typedef enum _EROIType
{
	ROIType_RECT,
	ROIType_POINT,
	ROIType_FIELD,
	ROIType_SPOINT,
	ROIType_Common,
	ROIType_CPOINT = ROIType_POINT
} EROIType;

typedef struct _TROIPoint
{
	POINT ptCenter;
	LONG nROIWidth;
	LONG nROIHeight;
} TROIPoint, TROICPoint;

typedef struct _TROISPoint
{
	POINT ptStart;
	LONG nROIWidth;
	LONG nROIHeight;
} TROISPoint;

typedef struct _TROIField
{
	float fHorField;
	float fVerField;
	LONG nROIWidth;
	LONG nROIHeight;
} TROIField;

typedef struct _TROITemp
{
	EROIType eROIType;
	float fData[4];
}TROIDataCommon;

typedef struct _TROIData
{
	double *dOffset;
	EROIType eROIType;
	int ROICount;
	void *pROIData;
} TROIData;

typedef struct _TInspectRegionOffset
{
	int nStartOffsetX;
	int nStartOffsetY;
	int nEndOffsetX;
	int nEndOffsetY;
} TInspectRegionOffset;

typedef enum _EFlareTarget
{
	FlareTarget_Dot,
	FlareTarget_Window
} EFlareTarget;

typedef struct _TCircleData
{
	double dOcx;
	double dOcy;
	double dRadx;
	double dRady;
	double dAngle;
} TCircleData;

typedef struct _TCircleSpecN
{
	bool bEnableCircle;
	int nPosOffsetX;
	int nPosOffsetY;
	double dRadiusRatioX;
	double dRadiusRatioY;
	double dThresholdRatio;
	double dROIRange;
	int nUsedFixedCircle;
} TCircleSpecN;

typedef enum _ETypicalValueType
{
	TypicalValue_Mean,
	TypicalValue_Media
} ETypicalValueType;

typedef enum _EFiducialMarkShape
{
	FiducialMarkShape_BlackCircle,
	FiducialMarkShape_WhiteCircle,
	FiducialMarkShape_CrossPoint,
	FiducialMarkShape_WhiteCircleInBlack,
	FiducialMarkShape_Patch
} EFiducialMarkShape;

extern void Convert2RECT(TROICPoint tCenterPoint, RECT& tROIRect);
extern void Convert2RECT(TROIField tField, int nImageWidth, int nImageHeight, RECT& tROIRect);
extern void Convert2RECT(TROISPoint tStartPoint, RECT& tROIRect);

extern void Convert2TROICommon(TROICPoint tCenterPoint, TROIDataCommon& tROIData);
extern void Convert2TROICommon(TROIField tField, TROIDataCommon& tROIData);
extern void Convert2TROICommon(TROISPoint tStartPoint, TROIDataCommon& tROIData);
extern void Convert2TROICommon(RECT tRect, TROIDataCommon& tROIData);

extern void Convert2CPoint(TROIDataCommon tROIData, TROICPoint &tCenterPoint);
extern void Convert2Field(TROIDataCommon tROIData, TROIField &tField);
extern void Convert2SPoint(TROIDataCommon tROIData, TROISPoint &tStartPoint);

extern void Convert2RECT(TROIDataCommon tROIData, int nWidth, int nHeight, RECT &tRect);

extern double GetPixelSize(EDATAFORMAT eDataFormat);

extern void CalcElpasedTime(std::chrono::system_clock::time_point& m_startTime, std::chrono::system_clock::time_point& m_endTime, std::chrono::milliseconds& m_elapsedTime, std::chrono::milliseconds& m_startTimems, std::chrono::milliseconds& m_endTimems);

extern void CheckDemosaicMethod(ESENSORTYPE &nSensorType, EDEMOSAICMETHOD &nDemosaicMethod, EOUTMODE nOutMode = OUTMODE_BAYER_RGGB);

typedef enum _VAR_TYPE
{
	VAR_TYPE_BYTE = 0,
	VAR_TYPE_SHORT,
	VAR_TYPE_USHORT,
	VAR_TYPE_INT,
	VAR_TYPE_FLOAT,
	VAR_TYPE_DOUBLE
} VAR_TYPE;


