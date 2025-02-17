#pragma once

#include "../XGraphic/xDataTypes.h"
typedef enum _EPos
{
	POS_CENTER,
	POS_LT,
	POS_RT,
	POS_LB,
	POS_RB
} EPos;

struct TColorRatio
{
	double RG;
	double BG;
	double GrGb;
	double R;
	double G;
	double B;
	TColorRatio(double _RG = 0, double _BG = 0, double _GrGb = 0, double _R = 0, double _G = 0, double _B = 0)
		: RG(_RG), BG(_BG), GrGb(_GrGb), R(_R),  G(_G),  B(_B) {}
};

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
} TColorShadingResult;

typedef struct _TRelativeIlluminationSpec
{
	double dBoxField;
	int	dBoxSize;
	double dSpecRIcornerMin;
	double dSpecRIcornerMax;
	double dSpecRIminMin;
	double dSpecRIminMax;
} TRelativeIlluminationSpec;

typedef struct _TRelativeIlluminationResult
{
	double dRIcornerRAW[4];
	double dRIcenterRAW;
	double dRIcorner;
	double dRImin;
} TRelativeIlluminationResult;

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

typedef struct _TColorReproduction
{
	double L;
	double a;
	double b;
} TColorReproduction;

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
	double dThresholdDelta;
} TColorReproductionSpec;

typedef struct _TIRFilterSpec
{
	int nMinThreshold;
	int nMaxThreshold;
} TIRFilterSpec;

typedef struct _TIRFilterResult
{
	double dBrightAve;
} TIRFilterResult;

typedef struct _TLowLightSpec
{
	int nThreshold_ParticleCount;
	double dThreshold_A;
	double dThreshold_B;
	double dThreshold_C;
	int nFullRectWidth;
	int nFullRectHeight;
	bool bEnableEllipseMiddle;
	bool bEnableEllipseCenter;
} TLowLightSpec;

typedef struct _TLowLightResult
{
	int nParticleCnt;
} TLowLightResult;
