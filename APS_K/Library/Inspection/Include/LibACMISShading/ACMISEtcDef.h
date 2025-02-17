#pragma once

#include <LibACMISCommon\ACMISCommon.h>

struct DSIZE
{
	double	width;
	double	height;
};

typedef struct _DBox2D
{
	DPOINT center;
	DSIZE size;
	double angle;
	double saturation;
} DBox2D;

typedef struct _TColorIntensitySpec
{
	int nAdjustType;	// 0: plus, 1: multiply
	int nTypicalValueType; // ETypicalValueType : Mean or Median
	TROIData tROI;

	TColorRatio* ptRefColorRGB;
	double *dColorDiffTh;
	double *dDistanceTh;
} TColorIntensitySpec;

typedef struct _TColorIntensityResult
{
	int nIndex;
	RECT rtROI;
	double dDistance;
	double dColorDiff[3];
	TColorRatio tColorRatio;
	TColorRatio tAdjustColorRatio;
} TColorIntensityResult;

typedef struct _TColorReproduction
{
	double L;
	double a;
	double b;
} TColorReproduction, TColorLab;

typedef struct _TColorReproductionResult
{
	int nIndex;
	double dDelta;
	TColorReproduction tColorLab;
	RECT rtROI;
} TColorReproductionResult;

typedef struct _TColorReproductionSpec
{
	int nROIWidth;
	int nROIHeight;
	int nMaxROICount;
	POINT *ptROICenter;
	TColorReproduction *ptRefColorLab;
	double *pdThresholdDelta;
} TColorReproductionSpec;

typedef struct _TColorReproductionSpecN
{
	TROIData tROI;
	TColorReproduction *ptRefColorLab;
	double *pdThresholdDelta;
} TColorReproductionSpecN;

typedef struct _TIRFilterSpec
{
	double dMinThreshold;
	double dMaxThreshold;
	double dROIVarThreshold;
	RECT rtROI;
} TIRFilterSpec;

typedef struct _TIRFilterSpecN
{
	double dMinThreshold;
	double dMaxThreshold;
	double dROIVarThreshold;
	TROIData tROI;
} TIRFilterSpecN;

typedef struct _TIRFilterResult
{
	RECT rtROI;
	double dFullBrightAvg;
	double dROIBrightAvg;
	double dROIBrightVar;
	double dROIBrightDev;
} TIRFilterResult;

typedef struct _TFlareSpec
{
	TROIData tROI;
	EFlareTarget eFlareTarget;
	double dGamma;
	double dBorderWidth;
	double dThresholdFlare;

	// Circle Mark
	double dRadius;
	double dBinaryThreshold; /// threshold for Binary of image
} TFlareSpec;

typedef struct _TFlareResult
{
	DBox2D BoxBlack;
	DBox2D BoxTarget;
	DBox2D BoxWhite[4];
	double dFlare;
	double dWhiteY;
	double dBlackY;
} TFlareResult;

typedef struct _TViewModeSpec
{
	TROIData tROI;
	POINT *ptRefVertex;
	double dThreshold;
} TViewModeSpec;

typedef struct _TViewModeResult
{
	int nSqureCount;
	int nVertexCount;
	RECT *rtSqure;
	POINT *ptVertex;
	POINT *ptConvertedVertex;
	double dMaxDistance;
	double *dHomographyMatrix;
} TViewModeResult;

typedef struct _TSaturationSpec
{
	TROIData tROI;
	double *dSpecBrightMin;
	double *dSpecBrightMax;
	double *dBinaryThreshold;
} TSaturationSpec;

typedef struct _TSaturationResult
{
	RECT rtROI;
	POINT ptBrightMin;
	POINT ptBrightMax;
	double dROIBrightMin;
	double dROIBrightMax;
	double dROIBrightAvg;
	int nROIBrightMaxBin;
	int nROIBrightMaxBinPixelCnt;
} TSaturationResult;

typedef struct _TStrayLightSpec
{
	TROIData tROI;
	int *nScanSize;
	int *nExcludeSize;
	double *dThreshold;
	bool bSmoothing;
	int nSmoothWindowSize;
} TStrayLightSpec;

typedef struct _TStrayLightResult
{
	POINT ptCenter;
	int nDiaMax;
	double dStrayLightThreshold;
	double dROIAvgLight;
	double dROIMaxLight;
	double dROIMinLight;
	double dMeanY[360];
} TStrayLightResult;


template<typename T, typename TResult, typename DataType = BYTE, typename DataFormat = EDATAFORMAT, typename OutMode = EOUTMODE, typename SensorType = ESENSORTYPE, typename DemosaicMethod = EDEMOSAICMETHOD>
class IEtcCommon
{
public:
	IEtcCommon() : m_nImageWidth(0), m_nImageHeight(0), m_bUsing8BitOnly(false), m_bPartialDemosaic(false) { memset(&m_tDataSpec, 0, sizeof(TDATASPEC)); memset(&m_stSpec, 0, sizeof(T)); memset(&m_tInspectRegionOffset, 0, sizeof(TInspectRegionOffset)); }
	virtual ~IEtcCommon() {}

	virtual bool Inspect(const DataType* pBuffer, int nImageWidth, int nImageHeight, T& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT) = 0;
	virtual bool Inspect(TBufferInfo& tBufferInfo, T& _Spec) = 0;
	virtual bool Inspect(TFileInfo& tFileInfo, T& _Spec) = 0;
	virtual int GetInspectionRegionCount() = 0;
	virtual const TResult* GetInspectionResult(int nIndex = 0) = 0;
	virtual bool InSpec(const T& _Spec) = 0;
	virtual const RECT* GetInspectionROI(int nIndex = 0) = 0;
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
class CACMISEtcCommon
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
	std::shared_ptr<IEtcCommon<T, TResult, DataType, DataFormat, OutMode, SensorType, DemosaicMethod>> m_pMethod;
	CACMISEtcCommon() {}
	CACMISEtcCommon(const CACMISEtcCommon&) {}
#else
	IEtcCommon<T, TResult, DataType, DataFormat, OutMode, SensorType, DemosaicMethod>* m_pMethod;
	CACMISEtcCommon() : m_pMethod(NULL) {}
	CACMISEtcCommon(const CACMISEtcCommon&) { delete m_pMethod; }
#endif

public:
	virtual ~CACMISEtcCommon() {}

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
	inline const TResult* GetInspectionResult(int nIndex = 0)
	{
		return m_pMethod->GetInspectionResult(nIndex);
	}
	inline bool InSpec(const T& _Spec)
	{
		return m_pMethod->InSpec(_Spec);
	}
	inline const RECT* GetInspectionROI(int nIndex = 0)
	{
		return m_pMethod->GetInspectionROI(nIndex);
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

