#pragma once

#include <LibACMISCommon\ACMISCommon.h>

//////////////////////////////////////////////////////////////////////////
// Optical Center

typedef struct _TOpticalCenter
{
	double dOpticalCenterSpecX;
	double dOpticalCenterSpecY;
	double dPixelPitch;
	double dThresholdRatio;
	int nOCBlockSize;
	int nEdgeTopMargin;
} TOpticalCenter, TOpticalCenterG, TOpticalCenterRGB, TOpticalCenterY;
#define TOpticalCenterRgb TOpticalCenterRGB

typedef struct _TOpticalCenterPolyFit
{
	double dOpticalCenterSpecX;
	double dOpticalCenterSpecY;
	double dPixelPitch;
	int nStep;
} TOpticalCenterPolyFit;

typedef struct _TOpticalCenterCombi
{
	double dOpticalCenterSpecX;
	double dOpticalCenterSpecY;
	double dPixelPitch;
	double dInspectField;
	int nStep;
} TOpticalCenterCombi;

typedef struct _TOpticalCenterDistortionDev
{
	double dOpticalCenterSpecX;
	double dOpticalCenterSpecY;
	int nMinCircleWidth;
	int nMinCircleHeight;
	double dMinCircularity;
	double dBinThreshold;
	double dMaxROIDelta;
	int nROICnt;
	POINT *ptROICenter;
}TOpticalCenterDistortionDev;

typedef enum _ESmoothMethod
{
	ESmoothMethod_1DConvolution,
	ESmoothMethod_GaussianFilter
} ESmoothMethod;

typedef struct _TOpticalCenterBrightest
{
	double dOpticalCenterSpecX;
	double dOpticalCenterSpecY;
	double dPixelPitch;
	double dThresholdRatio;
	double dSigma;
	int nFilterDim;
	int nMinWidthRange;
	int nMinHeightRange;
	int nSmoothWindowSize;
	int nSmoothMethod;
}TOpticalCenterBrightest;

typedef struct _TOpticalCenterCentroidCircle
{
	TROIData tROI;
	double dOpticalCenterSpecX;
	double dOpticalCenterSpecY;
	double dPixelPitch;
	double dThresholdRatio;
	double dThresholdMargin;
	int bEnableChannel;
}TOpticalCenterCentroidCircle;

template<typename T, typename DataType = BYTE, typename DataFormat = EDATAFORMAT, typename OutMode = EOUTMODE, typename SensorType = ESENSORTYPE, typename DemosaicMethod = EDEMOSAICMETHOD>
class IOpticalCenterMethod
{
public:
	IOpticalCenterMethod() : m_nImageWidth(0), m_nImageHeight(0), m_bUsing8BitOnly(false), m_bPartialDemosaic(false) { memset(&m_tDataSpec, 0, sizeof(TDATASPEC)); memset(&m_stSpec, 0, sizeof(T)); memset(&m_tInspectRegionOffset, 0, sizeof(TInspectRegionOffset)); }
	virtual ~IOpticalCenterMethod() {}

	virtual bool Inspect(const DataType* pBuffer, int nImageWidth, int nImageHeight, T& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT) = 0;
	virtual bool Inspect(TBufferInfo& tBufferInfo, T& _Spec) = 0;
	virtual bool Inspect(TFileInfo& tFileInfo, T& _Spec) = 0;
	virtual const DPOINT* GetOpticalCenterResult() = 0;
	virtual const DPOINT* GetSubOpticalCenterResult(int nIndex) = 0;
	virtual int GetSubPointCount() = 0;
	virtual const POINT* GetSubPoint(int nIndex) = 0;
	virtual DPOINT GetDeCenterResult() = 0;
	virtual bool InSpec(double dSpecX, double dSpecY) = 0;
	virtual const char* GetLogHeader() = 0;
	virtual const char* GetLogData() = 0;
	virtual const char* GetVersion() = 0;
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
	DPOINT m_ptOpticalCenter;
	POINT m_ptImgCenter;
	TDATASPEC m_tDataSpec;
	T m_stSpec;
	TInspectRegionOffset m_tInspectRegionOffset;
	std::vector<double> m_vSaturationResult;
};

template<typename T, typename DataType = BYTE, typename DataFormat = EDATAFORMAT, typename OutMode = EOUTMODE, typename SensorType = ESENSORTYPE, typename DemosaicMethod = EDEMOSAICMETHOD>
class CACMISOpticalCenter
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
	std::shared_ptr<IOpticalCenterMethod<T, DataType, DataFormat, OutMode, SensorType, DemosaicMethod>> m_pMethod;
	CACMISOpticalCenter() {}
	CACMISOpticalCenter(const CACMISOpticalCenter&) {}
#else
	IOpticalCenterMethod<T, DataType, DataFormat, OutMode, SensorType, DemosaicMethod>* m_pMethod;
	CACMISOpticalCenter() : m_pMethod(NULL) {}
	CACMISOpticalCenter(const CACMISOpticalCenter&) { delete m_pMethod; }
#endif

public:
	virtual ~CACMISOpticalCenter() {}

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
	inline const DPOINT* GetOpticalCenterResult() const
	{
		return m_pMethod->GetOpticalCenterResult();
	}
	inline const DPOINT* GetSubOpticalCenterResult(int nIndex) const
	{
		return m_pMethod->GetSubOpticalCenterResult(nIndex);
	}
	inline bool InSpec(double dSpecX, double dSpecY) const
	{
		return m_pMethod->InSpec(dSpecX, dSpecY);
	}
	inline DPOINT GetDeCenterResult() const
	{
		return m_pMethod->GetDeCenterResult();
	}
	inline int GetSubPointCount() const
	{
		return m_pMethod->GetSubPointCount();
	}
	inline const POINT* GetSubPoint(int nIndex) const
	{
		return m_pMethod->GetSubPoint(nIndex);
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

