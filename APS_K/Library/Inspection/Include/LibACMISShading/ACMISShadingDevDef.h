#pragma once
#include<vector>

typedef enum _EShadingDevPos
{
	SHADINGDEV_CENTER = 0,
	SHADINGDEV_EDGE,
	SHADINGDEV_CORNER
} EShadingDevPos;

// color uniformityOneImg spec.
typedef struct _TColorUniformityOneImgSpec
{
	int nGridSizeX;
	int nGridSizeY;
	double dCenterMin;
	double dCenterMax;
	double dEdgeMin;
	double dEdgeMax;
	double dCornerMin;
	double dCornerMax;
} TColorUniformityOneImgSpec;

// Golden shading deviation spec.
typedef struct _TGoldenShadingDevSpec
{
	int nGridSizeX;
	int nGridSizeY;
	double dMinDevCenter;
	double dMaxDevCenter;
	double dMinDevEdge;
	double dMaxDevEdge;
	double dMinDevCorner;
	double dMaxDevCorner;
	char* strGoldenLSCOffFilePath;
	char* strGoldenLSCOnFilePath;
	char* strGoldenGainFilePath;
} TGoldenShadingDevSpec;

// color uniformity or deviation with golden
// [colorUniformity] calculates normalization with center level
// ex., dev_rg(i) = r(i) / g(i), g(i) = (gr(i) + gb(i)) /2
// [goldenShadingDev] calculates shading deviation with golden shading
// ex., dev_rg(i) = (dev_rg(i) - dev_rg(i)_golden) / dev_rg(i)_golden
typedef struct _TShadingDev
{
	double dev_rg;
	double dev_bg;
	double dev_grgb;
	bool bRstFlag;	// 0:false 1:pass
} TShadingDev;

// average level of grid
typedef struct _TGridData
{
	double r_avg;
	double gr_avg;
	double gb_avg;
	double b_avg;
} TGridData;

typedef struct _TColorRIThreshold
{
	//double dHorMax;
	//double dVerMax;
	//double dDiaMax;
	//double dHorMin;
	//double dVerMin;
	//double dDiaMin;
	double dCH_R_Max;
	double dCH_GR_Max;
	double dCH_GB_Max;
	double dCH_B_Max;
	double dCH_R_Min;
	double dCH_GR_Min;
	double dCH_GB_Min;
	double dCH_B_Min;
} TColorRIThreshold;

