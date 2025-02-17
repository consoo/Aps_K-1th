#pragma once

#include "ACMISEtcDef.h"

typedef enum _EPos
{
	POS_CENTER,
	POS_LT,
	POS_RT,
	POS_LB,
	POS_RB
} EPos;

typedef enum _EROIPOSITION
{
	eHorizontal = 0,
	eVertical,
	eDiagonalA,
	eDiagonalB,
	eCenter
} EROIPosition;

typedef enum _ERICommonType
{
	RICommon_Coordinate,
	RICommon_Block_Uniform,
	RICommon_Block_NonUniform,
	RICommon_Ratio
}ERICommonType;

enum
{
	VIGNETTING_Y_MEAN_RATIO = 0,
	VIGNETTING_DSTRIPE,
	VIGNETTING_ALL
};


typedef enum _EColorSpaceType
{
	ColorSpace_Bayer,
	ColorSpace_RGB
} EColorSpaceType;

typedef enum _EMaxDiffType
{
	MaxDiff_Total,
	MaXDiff_Each
} EMaxDiffType;

typedef struct _TColorSensitivitySpec
{
	bool bAdjustFactor;
	double dRGFactor;
	double dBGFactor;
	double dGrGbFactor;
	double dSpecROISizeRatio;

	double dSpecDevMaxRG;
	double dSpecDevMinRG;
	double dSpecDevMaxBG;
	double dSpecDevMinBG;
	double dSpecDevMaxGrGb;
	double dSpecDevMinGrGb;
} TColorSensitivitySpec;

typedef struct _TColorSensitivitySpecN
{
	int nAdjustType;	// 0: plus, 1: multiply
	int nTypicalValueType; // ETypicalValueType : Mean or Median
	TROIData tROI;

	double dSpecDevMaxRG;
	double dSpecDevMinRG;
	double dSpecDevMaxBG;
	double dSpecDevMinBG;
	double dSpecDevMaxGrGb;
	double dSpecDevMinGrGb;
} TColorSensitivitySpecN;

typedef struct _TColorSensitivityResult
{
	RECT rtROI;
	TColorRatio tColorRatio;
	TColorRatio tAdjustColorRatio;
} TColorSensitivityResult;

typedef struct _TColorShadingSpec
{
	double dSpecROISizeRatio;

	double dSpecDevMaxRG;
	double dSpecDevMinRG;
	double dSpecDevMaxBG;
	double dSpecDevMinBG;
	double dSpecDevMaxGrGb;
	double dSpecDevMinGrGb;

	double dSpecOffsetMaxRG;
	double dSpecOffsetMinRG;
	double dSpecOffsetMaxBG;
	double dSpecOffsetMinBG;
	double dSpecOffsetMaxGrGb;
	double dSpecOffsetMinGrGb;

	double dSpecDiffRG;
	double dSpecDiffBG;
	double dSpecDiffGrGb;
} TColorShadingSpec;

typedef struct _TColorShadingSpecN
{
	int nTypicalValueType; // ETypicalValueType : Mean or Median
	TROIData tROI;

	double dSpecDevMaxRG;
	double dSpecDevMinRG;
	double dSpecDevMaxBG;
	double dSpecDevMinBG;
	double dSpecDevMaxGrGb;
	double dSpecDevMinGrGb;

	double dSpecOffsetMaxRG;
	double dSpecOffsetMinRG;
	double dSpecOffsetMaxBG;
	double dSpecOffsetMinBG;
	double dSpecOffsetMaxGrGb;
	double dSpecOffsetMinGrGb;

	double dSpecDiffRG;
	double dSpecDiffBG;
	double dSpecDiffGrGb;

	double dSpecRIMinR;
	double dSpecRIMinGr;
	double dSpecRIMinGb;
	double dSpecRIMinB;

	double dSpecRIMaxRG;
	double dSpecRIMinRG;
	double dSpecRIMaxBG;
	double dSpecRIMinBG;
	double dSpecRIMaxGrGb;
	double dSpecRIMinGrGb;
} TColorShadingSpecN;

typedef struct _TColorShadingResult
{
	double dDevMaxRG;
	double dDevMinRG;
	double dDevMaxBG;
	double dDevMinBG;
	double dDevMaxGrGb;
	double dDevMinGrGb;

	double dOffsetRG;
	double dOffsetBG;
	double dOffsetGrGb;

	double dDiffRG;
	double dDiffBG;
	double dDiffGrGb;

	double dRIMinR;
	double dRIMinGr;
	double dRIMinGb;
	double dRIMinB;

	double dRIMaxRG;
	double dRIMinRG;
	double dRIMaxBG;
	double dRIMinBG;
	double dRIMaxGrGb;
	double dRIMinGrGb;

	TColorRatio *pColorRatio;
} TColorShadingResult;

typedef struct _TColorChannelThreshold
{
	double dCH_R_Max;
	double dCH_GR_Max;
	double dCH_GB_Max;
	double dCH_B_Max;
	double dCH_R_Min;
	double dCH_GR_Min;
	double dCH_GB_Min;
	double dCH_B_Min;
} TColorChannelThreshold;

typedef struct _TColorShadingSpec_CommonN
{
	int nNormalizeIndex;
	int nTypicalValueType; // ETypicalValueType : Mean or Median
	TROIData tHorROI;
	TROIData tVerROI;
	TROIData tDiaAROI;
	TROIData tDiaBROI;
	double *dHorThreshold;
	double *dVerThreshold;
	double *dDiaThreshold;
	double dHorThdMin[4];
	double dHorThdMax[4];
	double dVerThdMin[4];
	double dVerThdMax[4];
	double dDiaThdMin[4];
	double dDiaThdMax[4];
} TColorShadingSpec_CommonN;

typedef struct _TColorShadingResult_Common
{
	RECT rtROI;
	int nROIPosition;
	int nIndex;
	double dResult[4];
	double dNormalResult[4];
	bool bPass;
} TColorShadingResult_Common;

typedef struct _TRelativeIlluminationSpec
{
	double dBoxField;
	int	nBoxSize;
	double dSpecRIcornerMin;
	double dSpecRIcornerMax;
	double dSpecRIminMin;
	double dSpecRIminMax;
	double dCenterIntensity;
} TRelativeIlluminationSpec;

typedef struct _TRelativeIlluminationSpecN
{
	TROIData tROI;
	double dSpecRIcornerMin;
	double dSpecRIcornerMax;
	double dSpecRIminMin;
	double dSpecRIminMax;
	double dCenterIntensity;
	int nSpecRINormalizeIndex;
} TRelativeIlluminationSpecN;

typedef struct _TRelativeIlluminationSpecX
{
	TROIData tROI;
	int nSpecPixelCntInBlock;
	int nEnableChannel;
	double *dSpecCH0Threshold;
	double *dSpecCH1Threshold;
	double *dSpecCH2Threshold;
	double *dSpecCH3Threshold;
	double dSpecCenterIntensity[4];
} TRelativeIlluminationSpecX;

typedef struct _TRelativeIlluminationResult
{
	double dRIcornerRAW[4];
	double dRIROIMin[5];
	double dRIROIMax[5];
	double dRIcenterRAW;
	double dRIcorner;
	double dRImin;
} TRelativeIlluminationResult;

typedef struct _TRelativeIlluminationResultN
{
	double *dRIcornerRAW;
	double *dRIcornerRatio;
	double *dRIROIMin;
	double *dRIROIMax;
	double dRIcenterRAW;
	double dRIcorner;
	double dRImin;
} TRelativeIlluminationResultN;

typedef struct _TRelativeIlluminationResultX
{
	RECT rtROI;
	int nROIPosition;
	int nIndex;
	double dRIRawResult[4];
	double dRINormalResult[4];
	bool bPass;
} TRelativeIlluminationResultX;

typedef struct _TRelativeUniformitySpec
{
	int	nROISize;
	int nUseOverlap;
	double dSpecMax;
} TRelativeUniformitySpec;

typedef struct _TRelativeUniformityResult
{
	double dResultMax;
	int nMaxBlockX;
	int nMaxBlockY;
	int nRefBlockX; // 기준점
	int nRefBlockY; // 기준점
} TRelativeUniformityResult;

typedef struct _TShadingUniformitySpec
{
	int nROIWidth;
	int nROIHeight;
	int nMaxROICount;
	int nNormalizeIndex;
	POINT *ptHorROI;
	POINT *ptVerROI;
	POINT *ptDiaAROI;
	POINT *ptDiaBROI;
	double *dHorThreshold;
	double *dVerThreshold;
	double *dDiaThreshold;
	double dHorThdMin;
	double dHorThdMax;
	double dVerThdMin;
	double dVerThdMax;
	double dDiaThdMin;
	double dDiaThdMax;
} TShadingUniformitySpec;

typedef struct _TShadingUniformitySpecN
{
	int nNormalizeIndex;
	TROIData tHorROI;
	TROIData tVerROI;
	TROIData tDiaAROI;
	TROIData tDiaBROI;
	double *dHorThreshold;
	double *dVerThreshold;
	double *dDiaThreshold;
	double dHorThdMin;
	double dHorThdMax;
	double dVerThdMin;
	double dVerThdMax;
	double dDiaThdMin;
	double dDiaThdMax;
} TShadingUniformitySpecN;

typedef struct _TShadingUniformityResult
{
	RECT rtROI;
	int nROIPosition;
	int nIndex;
	double dResult;
	double dNormalResult;
	bool bPass;
} TShadingUniformityResult;

typedef struct _TRICommon_Coordinate
{
	TROIData tHorROI;
	TROIData tVerROI;
	TROIData tDiaAROI;
	TROIData tDiaBROI;
}TRICommon_Coordinate;

typedef struct _TRICommon_Block
{
	int *ptHorROI;
	int *ptVerROI;
	int *ptDiaAROI;
	int *ptDiaBROI;
}TRICommon_Block;

typedef struct _TRelativeIlluminationSpec_Common
{
	int ntype;
	int nBlockWidth;
	int nBlockHeight;
	int nMaxROICount;
	int nNormalizeIndex;
	int *ptHorROI;
	int *ptVerROI;
	int *ptDiaAROI;
	int *ptDiaBROI;
	double *dHorThreshold;
	double *dVerThreshold;
	double *dDiaThreshold;
	double *dHorOffset;
	double *dVerOffset;
	double *dDiaAOffset;
	double *dDiaBOffset;
	double dHorThdMin;
	double dHorThdMax;
	double dVerThdMin;
	double dVerThdMax;
	double dDiaThdMin;
	double dDiaThdMax;
} TRelativeIlluminationSpec_Common;

typedef struct _TRelativeIlluminationResult_Common
{
	RECT rtROI;
	int nROIPosition;
	int nIndex;
	double dResult;
	double dNormalResult;
	bool bPass;
} TRelativeIlluminationResult_Common;

typedef struct _TLowLightSpec
{
	int nThreshold_ParticleCount;
	double dThreshold_Center;
	double dThreshold_Middle;
	double dThreshold_Full;
	int nFullRectWidth;
	int nFullRectHeight;
	bool bEnableEllipseMiddle;
	bool bEnableEllipseCenter;
} TLowLightSpec;

typedef struct _TLowLightResult
{
	int nParticleCnt;
	POINT *ptParticlePnt;
	RECT *rtParticleRect;
	RECT rtROIRect[3];
} TLowLightResult;

typedef struct _TVignettingSpec
{
	double dYmeanSpec;
	double dYmeanROISizeRatio;
	double dDStripeSpec;
	double dDStripeStrideRatio;
	int nMode;
} TVignettingSpec;

typedef struct _TVignettingData
{
	RECT rtVignettingROI[5]; // center(index 0) + 4 corner
	double dROIRatioX[5];
	double dROIRatioY[5];
	double dVignetting[4]; // 4 corner vignetting
} TVignettingData;

typedef struct _TColorUniformitySpecN
{
	int	nGridSizeX;
	int nGridSizeY;
	int nTypicalValueType; // ETypicalValueType : Mean or Median
	int nColorSpaceType;   // EColorSpaceType : Bayer or RGB
	int nUseOverlap;
	int nMaxDiffType;      // EMaxDiffType : Total diff or Each max diff
	double dSpecMaxDiffRG;
	double dSpecMaxDiffBG;
	double dSpecMaxDiffRB;
	double dSpecMaxDiffGrGb;
	double dSpecMinDiffRG;
	double dSpecMinDiffBG;
	double dSpecMinDiffRB;
	double dSpecMinDiffGrGb;
} TColorUniformitySpecN;

typedef struct _TColorUniformitySpec
{
	int	nGridSizeX;
	int nGridSizeY;
	int nTypicalValueType; // ETypicalValueType : Mean or Median
	int nColorSpaceType;   // EColorSpaceType : Bayer or RGB
	int nUseOverlap;
	int nMaxDiffType;      // EMaxDiffType : Total diff or Each max diff
	double dSpecMaxDiffRG;
	double dSpecMaxDiffBG;
	double dSpecMaxDiffRB;
} TColorUniformitySpec;

typedef struct _TColorUniformityResultN
{
	double dRG;
	double dRB;
	double dBG;
	double dGrGb;
	double dMaxTotalDiffRG;
	double dMaxTotalDiffRB;
	double dMaxTotalDiffBG;
	double dMaxTotalDiffGrGb;
	double dMaxEachDiffRG;
	double dMaxEachDiffRB;
	double dMaxEachDiffBG;
	double dMaxEachDiffGrGb;
	double dMinTotalDiffRG;
	double dMinTotalDiffRB;
	double dMinTotalDiffBG;
	double dMinTotalDiffGrGb;
	double dMinEachDiffRG;
	double dMinEachDiffRB;
	double dMinEachDiffBG;
	double dMinEachDiffGrGb;
	int nMaxBlockX;
	int nMaxBlockY;
	int nMaxRefBlockX; // 기준점
	int nMaxRefBlockY; // 기준점
	int nMinBlockX;
	int nMinBlockY;
	int nMinRefBlockX; // 기준점
	int nMinRefBlockY; // 기준점
} TColorUniformityResultN;

typedef struct _TColorUniformityResult
{
	double dRG;
	double dRB;
	double dBG;
	double dMaxDiffRG;
	double dMaxDiffRB;
	double dMaxDiffBG;
	int nMaxBlockX;
	int nMaxBlockY;
	int nRefBlockX; // 기준점
	int nRefBlockY; // 기준점
} TColorUniformityResult;

typedef struct _TLensShadingSpec
{
	int	nGridSizeX;
	int nGridSizeY;
} TLensShadingSpec;

typedef struct _TLensShadingResult
{
	double dR;
	double dG;
	double dB;
} TLensShadingResult;


template<typename T, typename TResult, typename DataType = BYTE, typename DataFormat = EDATAFORMAT, typename OutMode = EOUTMODE, typename SensorType = ESENSORTYPE, typename DemosaicMethod = EDEMOSAICMETHOD>
class IShadingCommon
{
public:
	IShadingCommon() : m_nImageWidth(0), m_nImageHeight(0), m_bUsing8BitOnly(false), m_bPartialDemosaic(false) { memset(&m_tDataSpec, 0, sizeof(TDATASPEC)); memset(&m_stSpec, 0, sizeof(T)); memset(&m_tInspectRegionOffset, 0, sizeof(TInspectRegionOffset)); }
	virtual ~IShadingCommon() {}

	virtual bool Inspect(const DataType* pBuffer, int nImageWidth, int nImageHeight, T& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT) = 0;
	virtual bool Inspect(TBufferInfo& tBufferInfo, T& _Spec) = 0;
	virtual bool Inspect(TFileInfo& tFileInfo, T& _Spec) = 0;
	virtual int GetInspectionRegionCount() = 0;
	virtual const TResult* GetInspectionResult(int nIndex = 0, EROIPosition pos = eHorizontal) = 0;
	virtual bool InSpec(const T& _Spec) = 0;
	virtual const RECT* GetInspectionROI(int nIndex = 0, EROIPosition pos = eHorizontal) = 0;
	virtual const char* GetLogHeader() = 0;
	virtual const char* GetLogData() = 0;
	virtual const char* GetVersion() = 0;
	virtual void SetInspectPosOffset(int nStartOffsetX, int nStartOffsetY, int nEndOffsetX, int nEndOffsetY) = 0;
	virtual void SetInspectPosOffset(TInspectRegionOffset tInspectRegionOffset) = 0;
	virtual void Init() = 0;
	virtual double GetSaturationResult(int nIndex = 0) = 0;

protected:
	int m_nImageWidth;
	int m_nImageHeight;
	bool m_bUsing8BitOnly;
	bool m_bPartialDemosaic;
	std::string m_strLogHeader;
	std::string m_strLogData;
	std::string m_strVersion;
	TDATASPEC m_tDataSpec;
	T m_stSpec;
	TInspectRegionOffset m_tInspectRegionOffset;
	std::vector<double> m_vSaturationResult;
};

template<typename T, typename TResult, typename DataType = BYTE, typename DataFormat = EDATAFORMAT, typename OutMode = EOUTMODE, typename SensorType = ESENSORTYPE, typename DemosaicMethod = EDEMOSAICMETHOD>
class CACMISShadingCommon
{
protected:
	std::chrono::system_clock::time_point m_startTime;
	std::chrono::system_clock::time_point m_endTime;
	std::chrono::milliseconds m_elapsedTime;
	std::chrono::milliseconds m_startTimems;
	std::chrono::milliseconds m_endTimems;

	std::string m_strLogHeader;
	std::string m_strLogData;

protected:
#if (_MSC_VER >= 1700)	// Visual C++ 11
	std::shared_ptr<IShadingCommon<T, TResult, DataType, DataFormat, OutMode, SensorType, DemosaicMethod>> m_pMethod;
	CACMISShadingCommon() {}
	CACMISShadingCommon(const CACMISShadingCommon&) {}
#else
	IShadingCommon<T, TResult, DataType, DataFormat, OutMode, SensorType, DemosaicMethod>* m_pMethod;
	CACMISShadingCommon() : m_pMethod(NULL) {}
	CACMISShadingCommon(const CACMISShadingCommon&) { delete m_pMethod; }
#endif

public:
	virtual ~CACMISShadingCommon() {}

	inline bool Inspect(const DataType* pBuffer, int nImageWidth, int nImageHeight, T& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT)
	{
		m_startTime = std::chrono::system_clock::now();

		bool bRet = m_pMethod->Inspect(pBuffer, nImageWidth, nImageHeight, _Spec, nDataFormat, nOutMode, nSensorType, nBlackLevel, bUsing8BitOnly, bPartialDemosaic, nDemosaicMethod);

		m_endTime = std::chrono::system_clock::now();
		CalcElpasedTime(m_startTime, m_endTime, m_elapsedTime, m_startTimems, m_endTimems);

		return bRet;
	}
	inline bool Inspect(TBufferInfo& tBufferInfo, T& _Spec)
	{
		m_startTime = std::chrono::system_clock::now();

		bool bRet = m_pMethod->Inspect(tBufferInfo, _Spec);

		m_endTime = std::chrono::system_clock::now();
		CalcElpasedTime(m_startTime, m_endTime, m_elapsedTime, m_startTimems, m_endTimems);

		return bRet;
	}
	inline bool Inspect(TFileInfo& tFileInfo, T& _Spec)
	{
		m_startTime = std::chrono::system_clock::now();

		bool bRet = m_pMethod->Inspect(tFileInfo, _Spec);

		m_endTime = std::chrono::system_clock::now();
		CalcElpasedTime(m_startTime, m_endTime, m_elapsedTime, m_startTimems, m_endTimems);

		return bRet;
	}
	inline int GetInspectionRegionCount()
	{
		return m_pMethod->GetInspectionRegionCount();
	}
	inline const TResult* GetInspectionResult(int nIndex = 0, EROIPosition pos = eHorizontal)
	{
		return m_pMethod->GetInspectionResult(nIndex, pos);
	}
	inline bool InSpec(const T& _Spec)
	{
		return m_pMethod->InSpec(_Spec);
	}
	inline const RECT* GetInspectionROI(int nIndex = 0, EROIPosition pos = eHorizontal)
	{
		return m_pMethod->GetInspectionROI(nIndex, pos);
	}
	inline const char* GetLogHeader()
	{
		m_strLogHeader = "StarTime,EndTime,ElapsedTime,";
		m_strLogHeader += m_pMethod->GetLogHeader();
		return m_strLogHeader.c_str();
	}
	inline const char* GetLogData()
	{
		char temp[512];

		memset(temp, 0, sizeof(temp));

		std::time_t startTime_c = std::chrono::system_clock::to_time_t(m_startTime);
		struct tm* tmStartTime = std::localtime(&startTime_c);
		sprintf_s(temp, sizeof(temp), "\'%04d-%02d-%02d %02d:%02d:%02d.%03d,", tmStartTime->tm_year + 1900, tmStartTime->tm_mon + 1, tmStartTime->tm_mday, tmStartTime->tm_hour, tmStartTime->tm_min, tmStartTime->tm_sec, (int)m_startTimems.count());
		m_strLogData = temp;

		std::time_t endTime_c = std::chrono::system_clock::to_time_t(m_endTime);
		struct tm* tmEndTime = std::localtime(&endTime_c);
		sprintf_s(temp, sizeof(temp), "\'%04d-%02d-%02d %02d:%02d:%02d.%03d,", tmEndTime->tm_year + 1900, tmEndTime->tm_mon + 1, tmEndTime->tm_mday, tmEndTime->tm_hour, tmEndTime->tm_min, tmEndTime->tm_sec, (int)m_endTimems.count());
		m_strLogData += temp;

		sprintf_s(temp, sizeof(temp), "%lld,", m_elapsedTime.count());
		m_strLogData += temp;
		m_strLogData += m_pMethod->GetLogData();

		return m_strLogData.c_str();
	}
	inline const char* GetVersion()
	{
		return m_pMethod->GetVersion();
	}
	inline void SetInspectPosOffset(int nStartOffsetX, int nStartOffsetY, int nEndOffsetX, int nEndOffsetY) const
	{
		return m_pMethod->SetInspectPosOffset(nStartOffsetX, nStartOffsetY, nEndOffsetX, nEndOffsetY);
	}
	inline void SetInspectPosOffset(TInspectRegionOffset tInspectRegionOffset) const
	{
		return m_pMethod->SetInspectPosOffset(tInspectRegionOffset);
	}
	inline void Init() const
	{
		return m_pMethod->Init();
	}
	inline long long GetElapsedTime() const
	{
		return m_elapsedTime.count();
	}
	inline double GetSaturationResult(int nIndex) const
	{
		return m_pMethod->GetSaturationResult(nIndex);
	}
};
