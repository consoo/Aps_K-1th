#pragma once

#include "ACMISImageDef.h"

typedef struct _TLCBSpec
{
	double dCenterThreshold;
	double dEdgeThreshold;
	double dCornerThreshold;
	int nMaxSingleDefectNum;
	int nMinDefectWidthHeight;
	double dCenterMaxR;
	double dCenterMaxGr;
	double dCenterMaxGb;
	double dCenterMaxB;
	double dEdgeMaxR;
	double dEdgeMaxGr;
	double dEdgeMaxGb;
	double dEdgeMaxB;
	double dCornerMaxR;
	double dCornerMaxGr;
	double dCornerMaxGb;
	double dCornerMaxB;
	TCircleSpec tCircleSpec;
	TMultiCircleSpec tMultiCircleSpec;
} TLCBSpec;

typedef struct _TLCBSpecN
{
	double dCenterThreshold;
	double dEdgeThreshold;
	double dCornerThreshold;
	int nMaxSingleDefectNum;
	int nMinDefectWidthHeight;
	double dCenterMaxR;
	double dCenterMaxGr;
	double dCenterMaxGb;
	double dCenterMaxB;
	double dEdgeMaxR;
	double dEdgeMaxGr;
	double dEdgeMaxGb;
	double dEdgeMaxB;
	double dCornerMaxR;
	double dCornerMaxGr;
	double dCornerMaxGb;
	double dCornerMaxB;
	TCircleSpecN tCircleSpec;
	TMultiCircleSpec tMultiCircleSpec;
} TLCBSpecN;

typedef struct _TFDFSpec
{
	double dCenterThreshold;
	double dEdgeThreshold;
	double dCornerThreshold;
	int nMaxSingleDefectNum;
	int nMinDefectWidthHeight;
	int nMedianFilterWidth;
	int nMedianFilterHeight;
	int nWidthScaleRatio;
	int nHeightScaleRatio;
	int nWindowSize;
	int nEdgeSize;
	bool bEnableChannel;
	TCircleSpecN tCircleSpec;
	TMultiCircleSpec tMultiCircleSpec;
}TFDFSpec;


typedef struct _TRU_YmeanSpec
{
	// this algorithm doesn't have corner.
	int nEdgeSize;
	float fCenterThreshold;
	float fEdgeThreshold;
	float fCornerThreshold;
	int nDefectBlockSize;
	int nLscBlockSize;
	int nCalcType;
	double dMaxDefectSize;
	double dPixelSize;
	TCircleSpec tCircleSpec;
	TMultiCircleSpec tMultiCircleSpec;
} TRU_YmeanSpec;

typedef struct _TRU_YmeanSpecN
{
	// this algorithm doesn't have corner.
	int nEdgeSize;
	float fCenterThreshold;
	float fEdgeThreshold;
	float fCornerThreshold;
	int nDefectBlockSize;
	int nLscBlockSize;
	int nCalcType;
	int nMaxRecursiveCount;
	double dMaxDefectSize;
	double dPixelSize;
	TCircleSpecN tCircleSpec;
	TMultiCircleSpec tMultiCircleSpec;
} TRU_YmeanSpecN;

typedef struct _TRUMultiFilterSpec
{
	int		nFilterSize[3];
	double  dZoneSizeRatio[3];

	int		nImageBoundaryThreshold;
	double	dChunkThreshold;
	//double	dBlobThreshold;
	double	dThreshold[3];
	int		nBlobSize[3];
} TRUMultiFilterSpec;

typedef struct _TRUBayerSpec
{
	int		method;

	int		nBlockSizeA;
	int		nBlockSizeB;

	double	nBlockThreshold;	//bins
	int		nBlockCountlimit;
} TRUBayerSpec;

typedef struct _TBlackSpotContrast
{
	int nBlockWidth;
	int nBlockHeight;
	int nClusterSize;
	int nDefectInCluster;
	double dDefectRatio;
	int nMaxSingleDefectNum;
	TCircleSpec tCircleSpec;
	TMultiCircleSpec tMultiCircleSpec;
} TBlackSpotContrast;

typedef struct _TBlackSpotContrastN
{
	int nBlockWidth;
	int nBlockHeight;
	int nClusterSize;
	int nDefectInCluster;
	double dDefectRatio;
	int nMaxSingleDefectNum;
	TCircleSpecN tCircleSpec;
	TMultiCircleSpec tMultiCircleSpec;
} TBlackSpotContrastN;

typedef struct _TBlemishOVTSpec
{
	int nBlockSize;
	int nMetaBlockSize;
	int nCalcType;
	double dDefectRatio;
	TCircleSpecN tCircleSpec;
	TMultiCircleSpec tMultiCircleSpec;
} TBlemishOVTSpec;

typedef struct _TUnfocusedBlemish
{
	int nSingleDefectCount;
	int nMeanBlockSize;
	double dMaxBLValue[5]; // A1 ~ A5
	int nAllowedCount[5]; // A1 ~ A5
	int nRefAreaWidth; // A1,2,3,5
	int nGapAreaWidth; // A1,2,3,5
	int nRefAreaWidthEdge; // A4
	int nGapAreaWidthEdge; // A4
	int nCornerAreaSize; // A3
} TUnfocusedBlemish;

typedef struct _TFocusedBlemish
{
	int nSingleDefectCount;
	int nMeanBlockSize;
	double dMaxBLValue[5]; // A1 ~ A5
	int nAllowedCount[5]; // A1 ~ A5
	int nClusterSize;
	int nDefectInCluster;
	int nCornerAreaSize; // A3
	int nMaxSingleDefectNum;
} TFocusedBlemish;

typedef struct _TBadPixelTable
{
//	int nBadPixelBrightSpec;
	int nMaxBadPixelBlobSpec;
	int nBlockSizeX;
	int nBlockSizeY;
	double dConstantMultipeBrightSpec;
	double dConstantPlusBrightSpec;
} TBadPixelTable;

typedef struct _TLCBPosSpec
{
	RECT nPosOffset;
} TLCBPosSpec;

template<typename T, typename DataType=BYTE, typename AREA=EImageRegion>
class IBlemishMethod
{
public:
	IBlemishMethod() {}
	virtual ~IBlemishMethod() {}

	typedef std::vector<TDefectResult> vDefectResult;

	virtual int Inspect(const DataType* pBuffer, int nImageWidth, int nImageHeight, T& pSpec) = 0;
	virtual int GetSingleDefectCount() const = 0;
	virtual int GetDefectCount(AREA pos) const = 0;
	virtual const TDefectResult* GetDefectResult(AREA pos, int nIndex) const = 0;
	virtual const TDefectResult* GetMaxDefectResult(AREA pos) const = 0;
	virtual int GetDefectBlobCount() = 0;
	virtual const RECT* GetDefectBlobRect(int nIndex) = 0;
	virtual const char* GetLogHeader() = 0;
	virtual const char* GetLogData() = 0;

protected:
	std::string m_strLogHeader;
	std::string m_strLogData;
};

template<typename T, typename DataType=BYTE, typename AREA=EImageRegion>
class CACMISImage
{
protected:
#if (_MSC_VER >= 1700)	// Visual C++ 11
	std::shared_ptr<IBlemishMethod<T, DataType, AREA>> m_pMethod;
	CACMISImage() {}
	CACMISImage(const CACMISImage&) {}
#else
	IBlemishMethod<T, DataType, AREA>* m_pMethod;
	CACMISImage() : m_pMethod(NULL) {}
	CACMISImage(const CACMISImage&) { delete m_pMethod; }
#endif

public:
	virtual ~CACMISImage() {}

	inline int Inspect(const DataType* pBuffer, int nWidth, int nHeight, T& _Spec)
	{
		return m_pMethod->Inspect(pBuffer, nWidth, nHeight, _Spec);
	}
	inline int GetSingleDefectCount() const
	{
		return m_pMethod->GetSingleDefectCount();
	}
	inline int GetDefectCount(AREA pos) const
	{
		return m_pMethod->GetDefectCount(pos);
	}
	inline const TDefectResult* GetDefectResult(AREA pos, int nIndex) const
	{
		return m_pMethod->GetDefectResult(pos, nIndex);
	}
	inline const TDefectResult* GetMaxDefectResult(AREA pos) const
	{
		return m_pMethod->GetMaxDefectResult(pos);
	}
	inline int GetDefectBlobCount() const
	{
		return m_pMethod->GetDefectBlobCount();
	}
	inline const RECT* GetDefectBlobRect(int nIndex) const
	{
		return m_pMethod->GetDefectBlobRect(nIndex);
	}
	inline const char* GetLogHeader() const
	{
		return m_pMethod->GetLogHeader();
	}
	inline const char* GetLogData() const
	{
		return m_pMethod->GetLogData();
	}
};

//////////////////////////////////////////////////////////////////////////
// Blemish의 입력영상을 센서 입력 영상 그대로 사용하기 위해 Class 내 DataFormat 추가
template<typename T, typename DataType = BYTE, typename AREA = EImageRegion, typename DataFormat = EDATAFORMAT, typename OutMode = EOUTMODE, typename SensorType = ESENSORTYPE, typename DemosaicMethod = EDEMOSAICMETHOD>
class IBlemishMethodCommon
{
public:
	IBlemishMethodCommon() : m_nImageWidth(0), m_nImageHeight(0), m_bUsing8BitOnly(false), m_bPartialDemosaic(false), m_Mask(NULL), m_bUsingMask(false) { memset(&m_tDataSpec, 0, sizeof(TDATASPEC)); memset(&m_stSpec, 0, sizeof(T)); memset(&m_stCircleData, 0, sizeof(TCircleData)); memset(&m_tBorderLine, 0, sizeof(RECT)); memset(&m_tInspectRegionOffset, 0, sizeof(TInspectRegionOffset)); }
	virtual ~IBlemishMethodCommon() {}

	typedef std::vector<TDefectResult> vDefectResult;

	virtual bool Inspect(const DataType* pBuffer, int nImageWidth, int nImageHeight, T& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT) = 0;
	virtual bool Inspect(TBufferInfo& tBufferInfo, T& _Spec) = 0;
	virtual bool Inspect(TFileInfo& tFileInfo, T& _Spec) = 0;
	virtual int GetSingleDefectCount() const = 0;
	virtual int GetDefectCount(AREA pos = EIMAGEREGION_CENTER) const = 0;
	virtual const TDefectResult* GetDefectResult(AREA pos, int nIndex) const = 0;
	virtual const TDefectResult* GetMaxDefectResult(AREA pos = EIMAGEREGION_CENTER) const = 0;
	virtual int GetDefectBlobCount() = 0;
	virtual const RECT* GetDefectBlobRect(int nIndex) = 0;
	virtual const TBlobResult* GetBlobDefectResult(int nIndex) = 0;
	virtual const char* GetLogHeader() = 0;
	virtual const char* GetLogData() = 0;
	virtual const char* GetVersion() = 0;
	virtual const TCircleData GetCircleData() = 0;
	virtual void SetCircleData(TCircleSpecN& Spec, TCircleData& tCircleData) = 0;
	virtual void SetMask(bool bUsingMask, BYTE* pMask) = 0;
	virtual void SetInspectPosOffset(int nStartOffsetX, int nStartOffsetY, int nEndOffsetX, int nEndOffsetY) = 0;
	virtual void SetInspectPosOffset(TInspectRegionOffset tInspectRegionOffset) = 0;
	virtual void Init() = 0;
	virtual const RECT* GetBorderLine() = 0;
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
	TCircleData m_stCircleData;
	BYTE* m_Mask;
	bool m_bUsingMask;
	TInspectRegionOffset m_tInspectRegionOffset;
	RECT m_tBorderLine;
	std::vector<double> m_vSaturationResult;
};

template<typename T, typename DataType = BYTE, typename AREA = EImageRegion, typename DataFormat = EDATAFORMAT, typename OutMode = EOUTMODE, typename SensorType = ESENSORTYPE, typename DemosaicMethod = EDEMOSAICMETHOD>
class CACMISImageCommon
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
	std::shared_ptr<IBlemishMethodCommon<T, DataType, AREA, DataFormat, OutMode, SensorType, DemosaicMethod>> m_pMethod;
	CACMISImageCommon() {}
	CACMISImageCommon(const CACMISImageCommon&) {}
#else
	IBlemishMethodCommon<T, DataType, AREA, DataFormat, OutMode, SensorType, DemosaicMethod>* m_pMethod;
	CACMISImageCommon() : m_pMethod(NULL) {}
	CACMISImageCommon(const CACMISImageCommon&) { delete m_pMethod; }
#endif

public:
	virtual ~CACMISImageCommon() {}

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
	inline int GetSingleDefectCount() const
	{
		return m_pMethod->GetSingleDefectCount();
	}
	inline int GetDefectCount(AREA pos = EIMAGEREGION_CENTER) const
	{
		return m_pMethod->GetDefectCount(pos);
	}
	inline const TDefectResult* GetDefectResult(AREA pos, int nIndex) const
	{
		return m_pMethod->GetDefectResult(pos, nIndex);
	}
	inline const TDefectResult* GetMaxDefectResult(AREA pos = EIMAGEREGION_CENTER) const
	{
		return m_pMethod->GetMaxDefectResult(pos);
	}
	inline int GetDefectBlobCount() const
	{
		return m_pMethod->GetDefectBlobCount();
	}
	inline const RECT* GetDefectBlobRect(int nIndex) const
	{
		return m_pMethod->GetDefectBlobRect(nIndex);
	}
	inline const TBlobResult* GetBlobDefectResult(int nIndex) const
	{
		return m_pMethod->GetBlobDefectResult(nIndex);
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
	inline const TCircleData GetCircleData() const
	{
		return m_pMethod->GetCircleData();
	}
	inline void SetCircleData(TCircleSpecN& Spec, TCircleData& tCircleData) const
	{
		return m_pMethod->SetCircleData(Spec, tCircleData);
	}
	inline void SetMask(bool bUsingMask, BYTE* pMask) const
	{
		return m_pMethod->SetMask(bUsingMask, pMask);
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
	inline const RECT* GetBorderLine() const
	{
		return m_pMethod->GetBorderLine();
	}
	inline long long GetElapsedTime() const
	{
		return m_elapsedTime.count();
	}
	inline double GetSaturationResult(int nIndex) const
	{
		return m_pMethod->GetSaturationResult(nIndex);
	}
	virtual bool MakeHistEQImage(const BYTE *pBuffer, BYTE *pBMP, int nWidth, int nHeight, TDATASPEC& tDataSpec, int nHistEQMethod) { return true; }
};

