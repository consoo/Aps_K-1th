#pragma once
#include "export.h"
#include <vector>
#include <memory>
#include <algorithm>
#include "../XGraphic/xDataTypes.h"

#define MAX_SFR_LIMIT					(100)

#define WRONG_CHART_ALIGN_SFR_VALUE		(-100)
#define WRONG_EDGE_SLOPE_SFR_VALUE		(-101)
#define WRONG_AVERAGE_SFR_VALUE			(-102)
#define WRONG_ROI_WIDTH_VALUE			(-103)
#define WRONG_ROI_HEIGHT_VALUE			(-104)
#define WRONG_FREQUENCY_VALUE			(-105)


#define	ROI_LEFT	(0)
#define	ROI_TOP		(1)
#define	ROI_RIGHT	(2)
#define	ROI_BOTTOM	(3)


typedef enum _ESFRAlgorithmType
{
	ESFRAlgorithm_ISO12233,				///< ISO12233 algorithm type
	ESFRAlgorithm_RHOMBUS, 				///< RHOMBUS algorithm type
	ESFRAlgorithm_LGIT_ISO 				///< Modified ISO12233 algorithm type
} ESFRAlgorithmType;

typedef enum _EEIAJAlgorithmType
{
	EEIAJAlgorithm_GETLINEMTF, ///< GETLINEMTF algorithm type
	ESFRAlgorithm_GETSOMTHING, ///< GETSOMTHING algorithm type
} EEIAJAlgorithmType;

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

typedef enum _EDistortionAlgorithmType
{
	EDistortion_RealRatio,
	EDistortion_TV
} EDistortionAlgorithmType;

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

	int nAlgorithmType;	///< algorithm info like SFR_ISO12233, SFR_ROHMBUS...
	int nDeltaAlgorithmType; ///< algorithm for calculation differece value among corner resolution values
	int nMTFavgLineNum; ///< Number of Average MTF Value for EIAJ ROI's
	int nDistortionAlrotithmType;	///< algorithm for Distortion : RealRatio and TV Distortion.

	POINT ptDisplayXY; ///< Result display position
	int nFontSize; ///< Result display font size
	int nDisplayGap; ///< Result display line gap
	int nDisplayGapFromROI; ///< Result display result gap from SFR ROI

	_TChartSpec() : strChartDetailInfoPath(NULL), strChartDetailDeltaSpecInfoPath(NULL), nDeltaAlgorithmType(0), nAlgorithmType(0)
	{
		memset(&stMainFiducialMark, 0, sizeof(TFiducialMarkInfo));
		memset(&stSubFiducialMark, 0, sizeof(TFiducialMarkInfo));
	}
} TChartSpec;

// EIAJ CSV
//number	name	signX	signY	direction	threshold	relativeX	relativeY	ROI width	ROI height
// average_flag	average_index0	average_index1	average_index2	average_index3
typedef struct _TEIAJROIInfo
{
	int number;
	char name[64];
	int signX;
	int signY;
	int nRoiDirection;
	int nThresholdVlaue;
	double relativeX;
	double relativeY;
	int ROIwidth;
	int ROIheight;
	int averageCount;
	int averageIndex[MAX_RESOLUTION_ROI_COUNT];
	int nMTFspec;
	int nSharpspec;
	int nMaxResolutionValue;
	int nMinResolutionValue;
	int nValidMTFguaranteeMargin;
} TEIAJROIInfo;

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

typedef struct _TEIAJROIResult
{
	POINT	ptCenter;
	RECT	rtROI;
	BOOL	bPass;
	int		nThresholdAxis;
	int		nResolutionValue;

	int		nMTFspec;
	float	fDeltaSpec;
	int		nSharpSpec;

	int		nMinResolutionValue;
	int		nMaxResolutionValue;
	int		nRoiDirection;

	double	dAvgMTF;

	int		nResolutionAxis;
	int		nValidMTFguaranteeMargin;
	EChartAlignStatus eChartStatus;

	void Init()
	{
		eChartStatus = EChartAlign_NORMAL;
		ptCenter.x =0, ptCenter.y = 0;
		rtROI.left = 0; rtROI.right = 0; rtROI.top = 0; rtROI.bottom = 0; nMTFspec = 50; nSharpSpec = 50;
	}

	_TEIAJROIResult() : bPass(FALSE), nThresholdAxis(0), nResolutionValue(0), nMTFspec(0), fDeltaSpec(0), nSharpSpec(0),
		nMinResolutionValue(0), nMaxResolutionValue(0), nRoiDirection(0), dAvgMTF(0), nResolutionAxis(0), nValidMTFguaranteeMargin(0), eChartStatus(EChartAlign_NORMAL)
	{
		Init();
	}

} TEIAJROIResult;

typedef struct _TSFRResolutionResult
{
	EResolutionClass eClass; ///< Class Level
	/// status string. PASS is '1', FAIL is '0'. dash(-) means separator of field ex) "1111-0111"
	char strStatus[MAX_FREQUENCY_NUM][128];
	std::vector<TSFRROIResult> vROIResult;
	int nDeltaResult;
	double dMaxDeltaSFR;
	int nMaxDeltaSFRgrade;

	void Init()
	{
		eClass = EResolutionClass_RESOLUTION_FAIL;
		for(int i = 0; i < MAX_FREQUENCY_NUM; i++)
			*(strStatus[i]) = '\0';
		std::for_each(vROIResult.begin(), vROIResult.end(), [](TSFRROIResult& n) { n.Init(); });
	}

	_TSFRResolutionResult() : nDeltaResult(0), dMaxDeltaSFR(0), nMaxDeltaSFRgrade(0) { Init(); }
} TSFRResolutionResult;

typedef struct _TEIAJResolutionResult
{
	EResolutionClass eClass; ///< Class Level
	/// status string. PASS is '1', FAIL is '0'. dash(-) means separator of field ex) "1111-0111"
	char strStatus[MAX_FREQUENCY_NUM][128];
	int nMaxROINum;
	std::vector <TEIAJROIResult> pROIResult;

	void Init()
	{
		eClass = EResolutionClass_RESOLUTION_FAIL;
		for(int i = 0; i < MAX_FREQUENCY_NUM; i++)
			*(strStatus[i]) = '\0';
		nMaxROINum = static_cast<int>(pROIResult.size());
		std::for_each(pROIResult.begin(), pROIResult.end(), [](TEIAJROIResult& n) { n.Init(); } );
	}
} TEIAJResolutionResult;

class IResolutionBuffer
{
public:
	IResolutionBuffer(){};
	virtual ~IResolutionBuffer() {};
	virtual void* GetBuffer(void) const = 0;
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
	ResolutionBufferPtr() : m_pImageBuffer(nullptr), m_nWBytes(0), m_nWidth(0), m_nHeight(0) {}
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

	void* GetBuffer() const { return (void*)m_pImageBuffer; }
	int GetWBytes() const { return m_nWBytes; }
	int GetWidth() const { return m_nWidth; }
	int GetHeight() const { return m_nHeight; }

	int GetLengthBytes() const { return sizeof(T); }
	const type_info& GetBufferTypeid() const { return typeid(T); }
};

class ACMISRESOLUTION_API ResolutionImageBufferManager
{
	std::shared_ptr<IResolutionBuffer> m_Ptr;

public:
	template<typename T> ResolutionImageBufferManager(const T* pImageBuffer, int nWidth, int nHeight, int nWidthByte)
		: m_Ptr(new ResolutionBufferPtr<T>(pImageBuffer,nWidth,nHeight,nWidthByte) )   {}
	
	inline void* GetBuffer() const { return m_Ptr->GetBuffer(); }
	inline const type_info& GetBufferTypeid() { return m_Ptr->GetBufferTypeid(); }
	inline int GetLengthBytes() { return m_Ptr->GetLengthBytes(); }
	inline int GetWidth() const { return m_Ptr->GetWidth(); }
	inline int GetHeight() const { return m_Ptr->GetHeight(); }
	inline int GetWBytes() const { return m_Ptr->GetWBytes(); }
};