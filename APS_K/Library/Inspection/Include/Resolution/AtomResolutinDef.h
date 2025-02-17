#pragma once
#include "export.h"
#include <vector>
#include <memory>
#include "../XGraphic/xDataTypes.h"

#define MAX_SFR_LIMIT					(100)

#define WRONG_CHART_ALIGN_SFR_VALUE		(-100)
#define WRONG_EDGE_SLOPE_SFR_VALUE		(-101)
#define WRONG_AVERAGE_SFR_VALUE			(-102)


typedef enum _ESFRAlgorithmType
{
	ESFRAlgorithm_ISO12233,				///< ISO12233 algorithm type
} ESFRAlgorithmType;

typedef enum _EResolutionClass
{
	EResolutionClass_A_PASS,			///< All PASS
	EResolutionClass_B_PASS,			///< Conditional PASS
	EResolutionClass_RESOLUTION_FAIL,	///< FAIL
	EResolutionClass_DELTA_FAIL			///< Delta fail
} EResolutionClass;

typedef enum _EAlignType
{
	EAlign_DOT,
	EAlign_CROSSHAIR
}EAlignType;

typedef enum _EChartAlignStatus
{
	EChartAlign_NORMAL,
	EChartAlign_WRONG
} EChartAlignStatus;

typedef struct _DRANGE
{
	double min;
	double max;

	struct _DRANGE() { min = 0; max = 0; }
} DRANGE;

typedef enum _ESFRDeltaAlgorithmType
{
	ESFRDelta_Diff,
	ESFRDelta_Ratio
} ESFRDeltaAlgorithmType;

#define MAX_RESOLUTION_ROI_COUNT			(80)
#define MAX_FREQUENCY_NUM					(1)
#define MAX_FIDUCIAL_MARK_COUNT				(4)
#define MAX_SUB_FIDUCIAL_MARK_COUNT			(2)
#define UNDEFINED_RESOLUTION_VALUE			(9999)

enum {
	EDGEDIR_VERTICAL,
	EDGEDIR_HORIZONTAL
};

typedef struct _TFiducialMarkInfo
{
	char* pszChartType; ///< DOT, CROSSDOT, ...
	double dDistanceXFromCenter; ///< pixel or relative position of main fiducial mark
	double dDistanceYFromCenter; ///< pixel or relative position of main fiducial mark
	int nROIBoxSize;
	double dRadius;
	double dRealGapX; ///< cm, between horizontal bases
	double dRealGapY;  ///< cm between vertical bases
} TFiducialMarkInfo;

typedef struct _TChartSpec
{
	int dbufferType;			 ///< buffer type define
	double dModuleChartDistance; ///< distance between module and chart

	// align info
	TFiducialMarkInfo stMainFiducialMark;
	TFiducialMarkInfo stSubFiducialMark;

	// spec.
	DRANGE	rXTiltSpec; ///< chart x tilt spec(min, max)
	DRANGE	rYTiltSpec; ///< chart y tilt spec(min, max)
	DRANGE	rDFOVSpec; ///< DFOV spec(min, max)
	DRANGE	rRotationSpec; ///< chart rotation spec(min, max)
	DRANGE	rDistortionSpec; ///< chart Distortion spec(min,max)

	/// frequency(Nyquest) lists..
	double	dFrequency[MAX_FREQUENCY_NUM];

	/// chart detail info - full path name(csv file)
	char* strChartDetailInfoPath;
	char* strChartDetailDeltaSpecInfoPath;

	int nAlgorithmType;	///< algorithm info like SFR_ISO12233, ...
	int nDeltaAlgorithmType; ///< algorithm for calculation differece value among corner resolution values

	POINT ptDisplayXY; ///< Result display position
	double nFontSize; ///< Result display font size
	int nDisplayGap; ///< Result display line gap
	int nDisplayGapFromROI; ///< Result display result gap from SFR ROI

	_TChartSpec() : strChartDetailInfoPath(NULL), strChartDetailDeltaSpecInfoPath(NULL), nDeltaAlgorithmType(0), nAlgorithmType(0)
	{
		memset(&stMainFiducialMark, 0, sizeof(TFiducialMarkInfo));
		memset(&stSubFiducialMark, 0, sizeof(TFiducialMarkInfo));
	}
} TChartSpec;

// SFR CSV
//number	name	signX	signY	direction	threshold	relativeX	relativeY	ROI width	ROI height
// average_flag	average_index0	average_index1	average_index2	average_index3
typedef struct _TSFRROIInfo
{
	int number;
	char name[64];
	int signX;
	int signY;
	int direction;
	int threshold;
	double relativeX;
	double relativeY;
	int ROIwidth;
	int ROIheight;
	int averageCount;
	int averageIndex[MAX_RESOLUTION_ROI_COUNT];
} TSFRROIInfo;

typedef struct _TDeltaSFRspec
{
	int nNumber;
	double dLowLimit;
	double dHighLimit;
	bool bEnablePassFail;
	int nGrade;
	int nDeltaROIcount;
	int nDeltaROIIndex[MAX_RESOLUTION_ROI_COUNT];
	bool bPassFailResult;
	double dDeltaSFR;
} TDeltaSFRspec;

typedef struct _TSFRROIResult
{
	CxDPoint ptCenter; ///< Found edge center in ROI
	CxDRect rtROI; ///< ROI of calculating Resolution
	double dFinalResult[MAX_FREQUENCY_NUM]; ///< the final result lists per frequency
	EChartAlignStatus eChartStatus; ///< Chart status. if this value is WRONG_CHART, FinalResult is WRONG value.

	void Init()
	{
		eChartStatus = EChartAlign_NORMAL;
		for(int i = 0; i < MAX_FREQUENCY_NUM; i++)
			dFinalResult[i] = 0.0f;
	}
} TSFRROIResult;

typedef struct _TSFRResolutionResult
{
	EResolutionClass eClass; ///< Class Level
	/// status string. PASS is '1', FAIL is '0'. dash(-) means separator of field ex) "1111-0111"
	char strStatus[MAX_FREQUENCY_NUM][128];
	TSFRROIResult* pROIResult;
	int nMaxROINum;
	int nDeltaResult;
	double dMaxDeltaSFR;
	int nMaxDeltaSFRgrade;

	void Init()
	{
		eClass = EResolutionClass_RESOLUTION_FAIL;
		for(int i = 0; i < MAX_FREQUENCY_NUM; i++)
			*(strStatus[i]) = '\0';
	}

	_TSFRResolutionResult() : pROIResult(NULL) {}
} TSFRResolutionResult;

class IResolutionBuffer
{
public:
	IResolutionBuffer(){};
	virtual ~IResolutionBuffer() {};
	virtual void* GetBuffer(void) = 0;
	virtual int GetLengthBytes() const = 0;
	virtual int GetWBytes() const = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual const type_info& GetBufferTypeid() const = 0;
};

template<typename T> class ResolutionBufferPtr : public IResolutionBuffer
{
	const T* m_pImageBuffer;
	int m_nWidth;
	int m_nHeight;
	int m_nWBytes;
public:
	ResolutionBufferPtr() : m_nWBytes(0), m_nWidth(0), m_nHeight() {}
	ResolutionBufferPtr(const T* pImageBuffer, int nWidth, int nHeight, int nWidthByte)
		: m_pImageBuffer(pImageBuffer), m_nWidth(nWidth), m_nHeight(nHeight), m_nWBytes(nWidthByte) {}

	virtual ~ResolutionBufferPtr() { }

	T &operator[](size_t i) const
	{
		return m_vImageBuffer[i];
	}

	void Set(const T* pImageBuffer, int nWidth, int nHeight, int nWidthByte)
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nWBytes = nWidthByte;
		m_pImageBuffer = pImageBuffer;
	}

	void* GetBuffer() { return (void*)m_pImageBuffer; }
	int GetLengthBytes() const { return sizeof(T); }
	int GetWBytes() const { return m_nWBytes; }
	int GetWidth() const { return m_nWidth; }
	int GetHeight() const { return m_nHeight; }
	const type_info& GetBufferTypeid() const { return typeid(T); }
};

class ATOMRESOLUTION_API ResolutionImageBufferManager
{
	std::shared_ptr<IResolutionBuffer> m_Ptr;

public:
	template<typename T> ResolutionImageBufferManager(const T* pImageBuffer, int nWidth, int nHeight, int nWidthByte)
		: m_Ptr(new ResolutionBufferPtr<T>(pImageBuffer,nWidth,nHeight,nWidthByte) )   {}
	
	 inline void* GetBuffer() { return m_Ptr->GetBuffer(); }
	 inline const type_info& GetBufferTypeid() { return m_Ptr->GetBufferTypeid(); }
	 inline int GetLengthBytes() { return m_Ptr->GetLengthBytes(); }
	 inline int GetWidth() { return m_Ptr->GetWidth(); }
	 inline int GetHeight() { return m_Ptr->GetHeight(); }
	 inline int GetWBytes() const { return m_Ptr->GetWBytes(); }
};