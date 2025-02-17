#pragma once
#include "export.h"
#include <LibACMISCommon\ACMISCommon.h>
#include <algorithm>


#define MAX_SFR_LIMIT					(100)

#define WRONG_CHART_ALIGN_SFR_VALUE		(-100)
#define WRONG_EDGE_SLOPE_SFR_VALUE		(-101)
#define WRONG_AVERAGE_SFR_VALUE			(-102)
#define WRONG_ROI_WIDTH_VALUE			(-103)
#define WRONG_ROI_HEIGHT_VALUE			(-104)
#define WRONG_FREQUENCY_VALUE			(-105)
#define WRONG_SFR_VALUE					(-106)
#define FAILED_TO_INITIALIZE_BUFFER		(-107)
#define WRONG_ROI_RANGE_VALUE			(-108)
#define WRONG_PARAMETER_VALUE			(-109)
#define WRONG_EDGE_DIR_SFR_VALUE		(-110)


#define	ROI_LEFT	(0)
#define	ROI_TOP		(1)
#define	ROI_RIGHT	(2)
#define	ROI_BOTTOM	(3)

typedef unsigned int VOIDTYPE;


typedef enum _ESFRAlgorithmType
{
	ESFRAlgorithm_ISO12233,				///< ISO12233 algorithm type
	ESFRAlgorithm_RHOMBUS, 				///< RHOMBUS algorithm type
	ESFRAlgorithm_LGIT_ISO, 			///< Modified ISO12233 algorithm type
	ESFRAlgorithm_VNE,
	ESFRAlgorithm_Mobis,
	ESFRAlgorithm_Tesla_Trinity
} ESFRAlgorithmType;

typedef enum _ESFRAlgorithmMethod
{
	ESFRMethod_Freq2SFR,				///< Calculate SFR value using Spatial Frequency
	ESFRMethod_SFR2Freq					///< Calculate Spatial Frequency using SFR value
} ESFRAlgorithmMethod;

typedef enum _ESFRFrequencyUnit
{
	ESFRFreq_CyclePerPixel,
	ESFRFreq_LinePairPerMilliMeter,
	ESFRFreq_LineWidthPerPictureHeight
} ESFRFrequencyUnit;

typedef enum _EEIAJAlgorithmType
{
	EEIAJAlgorithm_GETLINEMTF,			///< GETLINEMTF algorithm type
	ESFRAlgorithm_GETSOMTHING,			///< GETSOMTHING algorithm type
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

	struct _DRANGE() { min = 0.0; max = 0.0; }
} DRANGE;

typedef enum _ESFRDeltaAlgorithmType
{
	ESFRDelta_Diff,
	ESFRDelta_Ratio
} ESFRDeltaAlgorithmType;

typedef enum _EDistortionAlgorithmType
{
	EDistortion_RealRatio,
	EDistortion_TV,
	EDistortion_SMIA
} EDistortionAlgorithmType;

typedef enum _EFindMarkROIType
{
	EMark_CIRCLE,
	EMark_RECT,
	EMark_SHITOMASHI
} EFindMarkROIType;

typedef struct _TFindROIResult
{
	CxDPoint	LT;
	CxDPoint	RT;
	CxDPoint	LB;
	CxDPoint	RB;
}TFindROIResult;

typedef enum _EMarkROIType
{
	EFixed_ROI,
	ERelative_ROI,
	EAuto_ROI
} EMarkROIType;

#define MAX_RESOLUTION_ROI_COUNT			(80)
#define MAX_FREQUENCY_NUM					(3)//(1)
#define MAX_FIDUCIAL_MARK_COUNT				(4)
#define MAX_SUB_FIDUCIAL_MARK_COUNT			(2)
#define UNDEFINED_RESOLUTION_VALUE			(9999)

enum
{
	EDGEDIR_VERTICAL,
	EDGEDIR_HORIZONTAL
};

typedef enum _EFOVAlgorithmMethod
{
	FOV_METHOD_MODULE_DISTANCE,
	FOV_METHOD_EFL,
	FOV_METHOD_PIXEL_COUNT
} EFOVAlgorithmMethod;

typedef enum _EFiducialMarkType
{
	FIDUCIALMARKTYPE_DOT,		// Fiducial Mark Num = 4
	FIDUCIALMARKTYPE_CROSSDOT,	// Fiducial Mark Num = 4
	FIDUCIALMARKTYPE_GRID,		// Fiducial Mark Num = 9
	FIDUCIALMARKTYPE_LINE,		// Fiducial Mark Num = 2(H) + 2(V)
	FIDUCIALMARKTYPE_SINGLE,	// Fiducial Mark Num = 1
	FIDUCIALMARKTYPE_MAX
} EFiducialMarkType;

inline int GetFiducialMarkNum(EFiducialMarkType eType)
{
	int nCount = 4;

	switch (eType)
	{
		case FIDUCIALMARKTYPE_DOT:
		case FIDUCIALMARKTYPE_CROSSDOT:
		case FIDUCIALMARKTYPE_LINE:
			nCount = 4;
			break;
		case FIDUCIALMARKTYPE_GRID:
			nCount = 9;
			break;
		case FIDUCIALMARKTYPE_SINGLE:
			nCount = 1;
			break;
		default:
			nCount = 4;
	}

	return nCount;
}

typedef struct _TFiducialMarkInfo
{
	char* pszChartType; ///< DOT, CROSSDOT, ...
	double dDistanceXFromCenter; ///< pixel or relative position of main fiducial mark
	double dDistanceYFromCenter; ///< pixel or relative position of main fiducial mark
	int nROIBoxSize;
	int nMaxROIBoxSize;
	double dRadius;
	double dRealGapX; ///< cm, between horizontal bases
	double dRealGapY;  ///< cm between vertical bases
	int nFiducialMarkNum;
	int nFiducialMarkType;
	double dModuleChartDistance; ///< distance between module and chart
	int nDistortionAlrotithmType;	///< algorithm for Distortion : RealRatio and TV Distortion.
} TFiducialMarkInfo;

typedef struct _TFiducialMarkInfoN
{
	char* pszChartType; ///< DOT, CROSSDOT, ...
	double dDistanceXFromCenter; ///< pixel or relative position of main fiducial mark
	double dDistanceYFromCenter; ///< pixel or relative position of main fiducial mark
	int nROIBoxSize;
	int nMaxROIBoxSize;
	double dRadius;
	double dRealGapX; ///< cm, between horizontal bases
	double dRealGapY;  ///< cm between vertical bases
	double dRealSubGapX; ///< cm, between horizontal bases
	double dRealSubGapY;  ///< cm between vertical bases
	int nFiducialMarkNum;
	int nFiducialMarkType;
	double dModuleChartDistance; ///< distance between module and chart
	int nDistortionAlrotithmType;	///< algorithm for Distortion : RealRatio and TV Distortion.
	double dBinaryThreshold; /// threshold for Binary of image
	double dMaxDeviation; /// maximum deviation from ROI center
	double dPixelSize; /// Pixel size of Image Sensor
	double dEFL; /// Effective Focal Length of Lens
} TFiducialMarkInfoN;

typedef struct _TFiducialMarkSpec
{
	int nUsedFixedROI; ///< for SFR Value, 0: Relative ROI, 1:Fixed ROI(read from csv file), 2:Auto ROI
	int nUseMachine;   ///< True if the Machine finds the Fiducial Mark

	// Fiducial Mark Info
	TFiducialMarkInfoN stFiducialMark;
	POINT *ptRefFiducialMark;

	// Spec
	double dXTiltSpecMin;
	double dXTiltSpecMax;
	double dYTiltSpecMin;
	double dYTiltSpecMax;
	double dHFOVSpecMIn;
	double dHFOVSpecMax;
	double dVFOVSpecMIn;
	double dVFOVSpecMax;
	double dDFOVSpecMIn;
	double dDFOVSpecMax;
	double dRotationSpecMin;
	double dRotationSpecMax;
	double dDistortionSpecMin;
	double dDistortionSpecMax;

	// Offset
	double dTiltXOffset;
	double dTiltYOffset;
	double dHFOVOffset;
	double dVFOVOffset;
	double dDFOVOffset;
	double dRotateOffset;
	double dDistortionOffset;
} TFiducialMarkSpec;

typedef struct _TFiducialMarkType
{
	POINT ptFiducialMarkPoint;
	int nFiducialMarkShape; // Shape of Fiducial Mark : BlackCircle(0), WhiteCircle(1), CrossPoint(2), WhiteCircle in Black(3)
} TFiducialMarkType;

typedef struct _TFiducialMarkSpecN
{
	int nUsedFixedROI; ///< for SFR Value, 0: Relative ROI, 1:Fixed ROI(read from csv file), 2:Auto ROI
	int nUseMachine;   ///< True if the Machine finds the Fiducial Mark

	// Fiducial Mark Info
	TFiducialMarkInfoN stFiducialMark;
	TFiducialMarkType *ptRefFiducialMark;

	// Spec
	double dXTiltSpecMin;
	double dXTiltSpecMax;
	double dYTiltSpecMin;
	double dYTiltSpecMax;
	double dHFOVSpecMIn;
	double dHFOVSpecMax;
	double dVFOVSpecMIn;
	double dVFOVSpecMax;
	double dDFOVSpecMIn;
	double dDFOVSpecMax;
	double dRotationSpecMin;
	double dRotationSpecMax;
	double dDistortionSpecMin;
	double dDistortionSpecMax;

	// Offset
	double dTiltXOffset;
	double dTiltYOffset;
	double dHFOVOffset;
	double dVFOVOffset;
	double dDFOVOffset;
	double dRotateOffset;
	double dDistortionOffset;
} TFiducialMarkSpecN;

typedef struct _TChartSpec
{
	int dbufferType;			 ///< buffer type define

	// align info
	TFiducialMarkInfoN stMainFiducialMark;
	TFiducialMarkInfoN stSubFiducialMark;

	// spec.
	DRANGE  rSFRSpec;
	DRANGE	rXTiltSpec; ///< chart x tilt spec(min, max)
	DRANGE	rYTiltSpec; ///< chart y tilt spec(min, max)
	DRANGE	rDFOVSpec; ///< Diagonal FOV spec(min, max)
	DRANGE	rHFOVSpec; ///< Horizontal FOV spec(min, max)
	DRANGE	rVFOVSpec; ///< Vertical FOV spec(min, max)
	DRANGE	rRotationSpec; ///< chart rotation spec(min, max)
	DRANGE	rDistortionSpec; ///< chart Distortion spec(min,max)

	// Offset
	double dHFOVOffset;
	double dVFOVOffset;
	double dDFOVOffset;
	double dRotateOffset;
	double dTiltXOffset;
	double dTiltYOffset;
	double dDistortionOffset;

	/// frequency(Nyquest) lists..
	double	dCriteria[MAX_FREQUENCY_NUM];

	/// Pixel Size
	double dPixelSize; ///< Unit: um

	/// SFR Method
	int nAlgorithmMethod; ///< algorithm method such as Caculate SFR using spatial frequency, Caculate spatial frequecy using SFR value

	/// Frequency Unit
	int nFreqUnit; ///< SFR spatial frequency unit such as C/P, lp/mm, LW/PH

	/// ROI detail info - full path name(csv file)
	char* strDetailInfoPath;
	char* strDetailDeltaSpecInfoPath;

	int nAlgorithmType;	///< algorithm info like SFR_ISO12233, SFR_ROHMBUS...
	int nDeltaAlgorithmType; ///< algorithm for calculation differece value among corner resolution values
	int nMTFavgLineNum; ///< Number of Average MTF Value for EIAJ ROI's

	POINT ptDisplayXY; ///< Result display position
	int nFontSize; ///< Result display font size
	int nDisplayGap; ///< Result display line gap
	int nDisplayGapFromROI; ///< Result display result gap from SFR ROI

	int nUsedFixedROI; ///< for SFR Value, 0: Relative ROI, 1:Fixed ROI(read from csv file), 2:Auto ROI
	int nUseMachine;   // /< True if the Machine finds the Fiducial Mark
	int nROIWidth;		///< Only Use Auto ROI
	int nROIHeight;		///< Only Use Auto ROI
	int nDisplayROI;	///< Only Use Auto ROI

	int nCompensationMC; // mechanical OC corretion
	int nCompensationRotate; // Rotation correction

	int nSearchMaxPoxStep;
	int nUseFineSearch;
	int nSearchExclusionSize;

	double dGamma;

	char* strFiducialMarkInfoPath;

	// FOV Algorithm
	int nFOVAlgorithmMethod; // EFOVAlgorithmMethod

	_TChartSpec() : strDetailInfoPath(NULL), strDetailDeltaSpecInfoPath(NULL), nDeltaAlgorithmType(0), nAlgorithmType(0), strFiducialMarkInfoPath(NULL)
	{
		memset(&stMainFiducialMark, 0, sizeof(TFiducialMarkInfoN));
		memset(&stSubFiducialMark, 0, sizeof(TFiducialMarkInfoN));
	}
} TChartSpec;

// EIAJ CSV
/* Old */ //number	name	signX	signY	direction  TxtPosition	threshold	relativeX	relativeY	ROI width	ROI height
/* New */ //number	name	Left	Top		Right		Bottom		direction  TxtPosition	threshold	relativeX	relativeY	ROI width	ROI height
// average_flag	average_index0	average_index1	average_index2	average_index3
typedef struct _TEIAJROIInfo
{
	int number;
	//char name[64];
	//int signX;
	//int signY;
	//int nLeft;
	//int nTop;
	//int nRight;
	//int nBottom;
	//int ROIwidth;
	//int ROIheight;
	TROIDataCommon tROIData;
	int nRoiDirection;
	int nTxtPosition;
	int nThresholdVlaue;
	double relativeX;
	double relativeY;
	double offset;
	int averageCount;
	int averageIndex[MAX_RESOLUTION_ROI_COUNT];
	int nMTFspec;
	int nSharpspec;
	int nMaxResolutionValue;
	int nMinResolutionValue;
	int nValidMTFguaranteeMargin;
} TEIAJROIInfo;

// SFR CSV
//number	name	signX	signY	direction  TxtPosition	threshold	relativeX	relativeY	ROI width	ROI height
// average_flag	average_index0	average_index1	average_index2	average_index3
typedef struct _TSFRROIInfo
{
	int number;
	//char name[64];
	//int signX;
	//int signY;
	//int ROIwidth;
	//int ROIheight;
	TROIDataCommon tROIData;
	int direction;
	int txtPosition;
	double threshold;
	double relativeX;
	double relativeY;
	double freq[MAX_FREQUENCY_NUM]; // for Freq2SFR method
	double sfr; // for SFR2Freq method
	int freqUnit;
	double offset;
	bool bUseCosineCorrection;
	int nPolyFitOrder;
	int averageCount;
	int averageIndex[MAX_RESOLUTION_ROI_COUNT];
	int nDiffIndex;
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
	double dEdgeAngle;
	double dSaturation;
	EChartAlignStatus eChartStatus; ///< Chart status. if this value is WRONG_CHART, FinalResult is WRONG value.

	void Init()
	{
		dEdgeAngle = 0.0;
		dSaturation = 0.0;
		eChartStatus = EChartAlign_NORMAL;
		for(int i = 0; i < MAX_FREQUENCY_NUM; i++)
			dFinalResult[i] = 0.0;
	}
} TSFRROIResult;

typedef struct _TSFRConfig
{
	int nMaxROIWidth;
	int nMaxROIHeight;
	double dMaxEdgeAngle;
	double dPixelSize;
	ESFRAlgorithmType eAlgorithmType;
	ESFRAlgorithmMethod eAlgorithmMethod;
	ESFRFrequencyUnit eFrequencyUnit;
} TSFRConfig;

typedef struct _TSFRSpec
{
	ESFRDeltaAlgorithmType eSFRDeltaAlgorithmType;
	TSFRConfig tSFRConfig;
	TROIData tROI;
	int *dEdgeDir;
	double *dFrequency;
	double *dSFR;
	double *dThreshold;
	double dGamma;
	TDeltaSFRspec *tDelataSpec;
} TSFRSpec;

typedef struct _TEIAJConfig
{
	EEIAJAlgorithmType eAlgorithmType;
	int nMTFavgLineNum;
} TEIAJConfig;

typedef struct _TEIAJROISpec
{
	int		nMTFspec;
	float	fDeltaSpec;
	int		nSharpSpec;

	int		nMinResolutionValue;
	int		nMaxResolutionValue;
	int		nValidMTFguaranteeMargin;

	_TEIAJROISpec() : nMTFspec(0), fDeltaSpec(0.0f), nSharpSpec(0), nMinResolutionValue(0), nMaxResolutionValue(0), nValidMTFguaranteeMargin(0)
	{
	}
} TEIAJROISpec;

typedef struct _TEIAJROIResult
{
	POINT	ptCenter;
	RECT	rtROI;
	BOOL	bPass;
	int		nThresholdAxis;
	int		nResolutionValue;
	double	dOffset;

	TEIAJROISpec tROISpec;

	int		nRoiDirection;

	double	dAvgMTF;

	int		nResolutionAxis;
	EChartAlignStatus eChartStatus;

	void Init()
	{
		eChartStatus = EChartAlign_NORMAL;
		ptCenter.x = 0;
		ptCenter.y = 0;
		rtROI.left = 0;
		rtROI.right = 0;
		rtROI.top = 0;
		rtROI.bottom = 0;
		tROISpec.nMTFspec = 50;
		tROISpec.fDeltaSpec = 0.0f;
		tROISpec.nSharpSpec = 50;
		tROISpec.nMinResolutionValue = 0;
		tROISpec.nMaxResolutionValue = 0;
		tROISpec.nValidMTFguaranteeMargin = 0;
	}

	_TEIAJROIResult() : bPass(FALSE), nThresholdAxis(0), nResolutionValue(0),
		nRoiDirection(0), dAvgMTF(0.0), nResolutionAxis(0), eChartStatus(EChartAlign_NORMAL)
	{
		Init();
	}
} TEIAJROIResult;

typedef struct _TEIAJSpec
{
	TEIAJConfig tConfig;
	TEIAJROISpec *tROISpec;
	TROIData tROI;
	int *nDirection;
	int *nThreshold;
} TEIAJSpec;

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

	_TSFRResolutionResult() : nDeltaResult(0), dMaxDeltaSFR(0.0), nMaxDeltaSFRgrade(0) { Init(); }
} TSFRResolutionResult;

typedef struct _TEIAJResolutionResult
{
	EResolutionClass eClass; ///< Class Level
	/// status string. PASS is '1', FAIL is '0'. dash(-) means separator of field ex) "1111-0111"
	char strStatus[128];
	int nMaxROINum;
	std::vector <TEIAJROIResult> pROIResult;

	void Init()
	{
		eClass = EResolutionClass_RESOLUTION_FAIL;
			*(strStatus) = '\0';
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
	virtual const TDATASPEC& GetDataSpec() const = 0;
	virtual void SetDataSpec(EDATAFORMAT eDataFormat, EOUTMODE eOutMode, ESENSORTYPE eSensorType, int nBlackLevel) {};
};

template<typename T> class ResolutionBufferPtr : public IResolutionBuffer
{
	const T* m_pImageBuffer;
	int m_nWidth;
	int m_nHeight;
	int m_nWBytes;
	TDATASPEC m_tDataSpec;

public:
	ResolutionBufferPtr() : m_pImageBuffer(nullptr), m_nWBytes(0), m_nWidth(0), m_nHeight(0)
	{
		InitDataSpec();
	}
	ResolutionBufferPtr(const T* pImageBuffer, int nWidth, int nHeight, int nWidthByte)
		: m_pImageBuffer(pImageBuffer), m_nWidth(nWidth), m_nHeight(nHeight), m_nWBytes(nWidthByte)
	{
		InitDataSpec();
	}

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

	void InitDataSpec()
	{
		SetDataSpec(DATAFORMAT_YUV, OUTMODE_YUV422_YCbYCr, (ESENSORTYPE)OUTMODE_YUV422_YCbYCr, 0);
	}

	void SetDataSpec(EDATAFORMAT eDataFormat, EOUTMODE eOutMode, ESENSORTYPE eSensorType, int nBlackLevel)
	{
		m_tDataSpec.eDataFormat = eDataFormat;
		m_tDataSpec.eOutMode = eOutMode;
		m_tDataSpec.eSensorType = eSensorType;
		m_tDataSpec.nBlackLevel = nBlackLevel;
	}

	void* GetBuffer() const { return (void*)m_pImageBuffer; }
	int GetWBytes() const { return m_nWBytes; }
	int GetWidth() const { return m_nWidth; }
	int GetHeight() const { return m_nHeight; }

	int GetLengthBytes() const { return sizeof(T); }
	const type_info& GetBufferTypeid() const { return typeid(T); }
	const TDATASPEC& GetDataSpec() const { return m_tDataSpec; }
};

class ACMISRESOLUTION_API ResolutionImageBufferManager
{
	std::shared_ptr<IResolutionBuffer> m_Ptr;

public:
	template<typename T> ResolutionImageBufferManager(const T* pImageBuffer, int nWidth, int nHeight, int nWidthByte)
		: m_Ptr(new ResolutionBufferPtr<T>(pImageBuffer, nWidth, nHeight, nWidthByte)) {}

	inline void* GetBuffer() const { return m_Ptr->GetBuffer(); }
	inline const type_info& GetBufferTypeid() { return m_Ptr->GetBufferTypeid(); }
	inline int GetLengthBytes() { return m_Ptr->GetLengthBytes(); }
	inline int GetWidth() const { return m_Ptr->GetWidth(); }
	inline int GetHeight() const { return m_Ptr->GetHeight(); }
	inline int GetWBytes() const { return m_Ptr->GetWBytes(); }
	inline const TDATASPEC& GetDataSpec() const { return m_Ptr->GetDataSpec(); }
	inline void SetDataSpec(EDATAFORMAT eDataFormat, EOUTMODE eOutMode, ESENSORTYPE eSensorType, int nBlackLevel) { m_Ptr->SetDataSpec(eDataFormat, eOutMode, eSensorType, nBlackLevel); }
};
