#pragma once

#include <LibACMISCommon\ACMISCommon.h>

//////////////////////////////////////////////////////////////////////////
// Signal to Noise Ratio

typedef enum _EFPNMethod
{
	EFPN_Common,
	EFPN_ONSEMI,
	EFPN_EMVA1288
} EFPNMethod;

typedef enum _EPatternNoiseType
{
	EFPN_DSNU,
	EFPN_PRNU,
	EFPN_DSNU_ONSEMI,
	EFPN_PRNU_ONSEMI,
	EFPN_DSNU_EMVA1288,
	EFPN_PRNU_EMVA1288
} EPatternNoiseType;

typedef enum _SNR_BW_IMAGE_INDEX
{
	SNR_BW_IMAGE_WHITE,
	SNR_BW_IMAGE_BLACK,
	SNR_BW_IMAGE_GRAY
} SNR_BW_IMAGE_INDEX;

typedef struct _TSNR
{
	double dSNRResult;
	double dDRResult;
	long double dAverage;
	long double dVariance;
	RECT rtROI;
	int nIndex;
} TSNRResult;

typedef struct _TSNRNormSpec
{
	int nROIWidth;
	int nROIHeight;
	int nLscBlockSize;
	int nMaxROICount;
	POINT *ptROICenter;
	double dSNRThreshold;
} TSNRNormSpec;
#define TSNROECFSpec TSNRNormSpec

typedef struct _TSNRNormSpecN
{
	TROIData tROI;
	double dSNRThreshold;
} TSNRNormSpecN;

typedef struct _TSNRNormResult
{
	double dSNRResult;
	double dAverage;
	double dVariance;
	RECT rtROI;
	int nIndex;
} TSNRNormResult;

typedef struct _TPatternNoiseSpec
{
	int nROIWidth;
	int nROIHeight;
	int nPatternType;
	int nLscBlockSize;
	int nMaxROICount;
	double dROIXPosRatio;
	double dROIYPosRatio;
	double dSNRThreshold;
} TPatternNoiseSpec;

typedef struct _TPatternNoiseSpecN
{
	int nPatternType;
	TROIData tROI;
	double dSNRThreshold;
} TPatternNoiseSpecN;

typedef struct _TPatternNoiseResult
{
	double dDSNU;
	double dPRNU;
	double dFPNData[6];
	RECT rtROI;
	int nIndex;
} TPatternNoiseResult;

typedef enum _EFPNDATAINDEX
{
	FPNDATA_DSNU_AVERAGE,
	FPNDATA_DSNU_VARIANCE,
	FPNDATA_PRNU_AVERAGE,
	FPNDATA_PRNU_VARIANCE,
	FPNDATA_PRNU_VAR_TOT = FPNDATA_PRNU_VARIANCE,
	FPNDATA_PRNU_VAR_ROW,
	FPNDATA_PRNU_VAR_COL,
	FPNDATA_PRNU_DARK_AVG = FPNDATA_PRNU_AVERAGE,
	FPNDATA_PRNU_DARK_VAR,
	FPNDATA_PRNU_SAT50_AGV,
	FPNDATA_PRNU_SAT50_VAR
} EFPNDATAINDEX;

typedef struct _TSNRBWSpec
{
	int nROIWidth;
	int nROIHeight;
	int nLscBlockSize;
	int nMaxROICount;
	POINT *ptROICenter;
	double dSNRThreshold;
	double dDRThreshold;
} TSNRBWSpec;

typedef struct _TSNRBWSpecN
{
	TROIData tROI;
	double dSNRThreshold;
	double dDRThreshold;
} TSNRBWSpecN;

typedef struct _TSNRBWResult
{
	double dWhiteAverage;
	double dBlackAverage;
	double dGrayVariance;
	double dSNRResult;
	double dDRResult;
	RECT rtROI;
	int nIndex;
} TSNRBWResult;

typedef struct _TDarkCurrentSpec
{
	TROIData tROI;
	double dDarkCurrentThreshold;
	double dExposureTimeInterval;
	double dStartExposureTime;
} TDarkCurrentSpec;

typedef struct _TDarkCurrentResult
{
	double dDarkCurrent;
	double dDarkSignalBasis;
	RECT rtROI;
	int nIndex;
} TDarkCurrentResult;

#define TSNRVNESpec TSNRNormSpecN
#define TSNRVNEResult TSNRNormResult

typedef struct _TDarkNoiseSpec
{
	TROIData tROI;
	double dK;
	int nAlternativeValue;
	int nEnableChannel;
	int nBitDepth;
	int nPedestal;
	int nConvKerenelSize;
	double *pConvKernel;
	double dFPNPThreshold[4];
	double dColumnFPNPThreshold[4];
	double dRowFPNPThreshold[4];
	double dMaxColumnFPNPThreshold[4];
	double dMaxRowFPNPThreshold[4];
	double dColumnFPNVThreshold[4];
	double dRowFPNVThreshold[4];
	double dTemporalNoisePThreshold[4];
	double dColumnNoisePThreshold[4];
	double dRowNoisePThreshold[4];
	double dTThreshold[4];
	double dColumnNoiseVThreshold[4];
	double dRowNoiseVThreshold[4];
	double dBlackLevelThreshold[4];
} TDarkNoiseSpec;

typedef struct _TDarkNoiseResult
{
	double dFPNP[4];
	double dColumnFPNP[4];
	double dRowFPNP[4];
	double dMaxColumnFPNP[4];
	double dMaxRowFPNP[4];
	double dColumnFPNV[4];
	double dRowFPNV[4];
	double dTemporalNoiseP[4];
	double dColumnNoiseP[4];
	double dRowNoiseP[4];
	double dT[4];
	double dColumnNoiseV[4];
	double dRowNoiseV[4];
	double dBlackLevel[4];
	RECT rtROI;
	int nIndex;
} TDarkNoiseResult;


template<typename T, typename TResult, typename DataType = BYTE, typename DataFormat = EDATAFORMAT, typename OutMode = EOUTMODE, typename SensorType = ESENSORTYPE, typename DemosaicMethod = EDEMOSAICMETHOD>
class ISignalNoiseRatio
{
public:
	ISignalNoiseRatio() : m_nImageWidth(0), m_nImageHeight(0), m_bUsing8BitOnly(false), m_bPartialDemosaic(false) { memset(&m_tDataSpec, 0, sizeof(TDATASPEC)); memset(&m_stSpec, 0, sizeof(T)); memset(&m_tInspectRegionOffset, 0, sizeof(TInspectRegionOffset));}
	virtual ~ISignalNoiseRatio() {}

	typedef std::vector<TSNRResult> vSNRResult;

	virtual bool Inspect() = 0;
	virtual bool Inspect(const DataType* pBuffer, int nImageWidth, int nImageHeight, T& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT) = 0;
	virtual bool Inspect(TBufferInfo& tBuferInfo, T& _Spec) = 0;
	virtual bool Inspect(TFileInfo& tFileInfo, T& _Spec) = 0;
	virtual int GetSNRRegionCount() = 0;
	virtual const TSNRResult* GetSNRResult(int nIndex) = 0;
	virtual const TSNRResult* GetMinSNRResult() = 0;
	virtual int GetInspectionRegionCount() = 0;
	virtual const TResult* GetInspectionResult(int nIndex) = 0;
	virtual const char* GetLogHeader() = 0;
	virtual const char* GetLogData() = 0;
	virtual const char* GetVersion() = 0;
	virtual bool SetImageSpec(int nImageWidth, int nImageHeight, int nImageCount, T& pSpecc, EDATAFORMAT dataFormat, EOUTMODE outMode, ESENSORTYPE sensorType, int nBlackLevel, bool bUsing8BitOnly = false, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT) = 0;
	virtual bool SetImageBuffer(const DataType* pBuffer, int nImageWidth, int nImageHeight, int nImageIndex) = 0;
	virtual void SetInspectPosOffset(int nStartOffsetX, int nStartOffsetY, int nEndOffsetX, int nEndOffsetY) = 0;
	virtual void SetInspectPosOffset(TInspectRegionOffset tInspectRegionOffset) = 0;
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
	vSNRResult m_vSNRResult;
	std::vector<TResult> m_vResult;
	TInspectRegionOffset m_tInspectRegionOffset;
	std::vector<double> m_vSaturationResult;

	std::vector< std::vector<BYTE> > m_vRawBuffer;
};

template<typename T, typename TResult, typename DataType = BYTE, typename DataFormat = EDATAFORMAT, typename OutMode = EOUTMODE, typename SensorType = ESENSORTYPE, typename DemosaicMethod = EDEMOSAICMETHOD>
class CACMISSignalNoiseRatio
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
	std::shared_ptr<ISignalNoiseRatio<T, TResult, DataType, DataFormat, OutMode, SensorType, DemosaicMethod>> m_pMethod;
	CACMISSignalNoiseRatio() {}
	CACMISSignalNoiseRatio(const CACMISSignalNoiseRatio&) {}
#else
	ISignalNoiseRatio<T, TResult, DataType, DataFormat, OutMode, SensorType, DemosaicMethod>* m_pMethod;
	CACMISSignalNoiseRatio() : m_pMethod(NULL) {}
	CACMISSignalNoiseRatio(const CACMISSignalNoiseRatio&) { delete m_pMethod; }
#endif

public:
	virtual ~CACMISSignalNoiseRatio() {}

	inline bool Inspect()
	{
		m_startTime = std::chrono::system_clock::now();

		bool bRet = m_pMethod->Inspect();

		m_endTime = std::chrono::system_clock::now();
		CalcElpasedTime(m_startTime, m_endTime, m_elapsedTime, m_startTimems, m_endTimems);

		return bRet;
	}
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
	inline int GetSNRRegionCount() const
	{
		return m_pMethod->GetSNRRegionCount();
	}
	inline const TSNRResult* GetSNRResult(int nIndex) const
	{
		return m_pMethod->GetSNRResult(nIndex);
	}
	inline const TSNRResult* GetMinSNRResult() const
	{
		return m_pMethod->GetMinSNRResult();
	}
	inline int GetInspectionRegionCount() const
	{
		return m_pMethod->GetInspectionRegionCount();
	}
	inline const TResult* GetInspectionResult(int nIndex) const
	{
		return m_pMethod->GetInspectionResult(nIndex);
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
	inline const char* GetVersion() const
	{
		return m_pMethod->GetVersion();
	}
	inline bool SetImageSpec(int nImageWidth, int nImageHeight, int nImageCount, T& _Spec, EDATAFORMAT dataFormat, EOUTMODE outMode, ESENSORTYPE sensorType, int nBlackLevel, bool bUsing8BitOnly = false, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT) const
	{
		return m_pMethod->SetImageSpec(nImageWidth, nImageHeight, nImageCount, _Spec, dataFormat, outMode, sensorType, nBlackLevel, bUsing8BitOnly, bPartialDemosaic, nDemosaicMethod);
	}
	inline bool SetImageBuffer(const DataType* pBuffer, int nImageWidth, int nImageHeight, int nImageIndex)
	{
		return m_pMethod->SetImageBuffer(pBuffer, nImageWidth, nImageHeight, nImageIndex);
	}
	inline void SetInspectPosOffset(int nStartOffsetX, int nStartOffsetY, int nEndOffsetX, int nEndOffsetY) const
	{
		return m_pMethod->SetInspectPosOffset(nStartOffsetX, nStartOffsetY, nEndOffsetX, nEndOffsetY);
	}
	inline void SetInspectPosOffset(TInspectRegionOffset tInspectRegionOffset) const
	{
		return m_pMethod->SetInspectPosOffset(tInspectRegionOffset);
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

