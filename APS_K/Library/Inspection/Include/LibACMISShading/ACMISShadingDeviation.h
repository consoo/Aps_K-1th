#pragma once

#include "export.h"
#include "ACMISShadingDevDef.h"

// DO NOT INCLUDE HEADER FILE!
class CColorShadingDevGeneric;
class CGoldenShadingDev;

class ACMISSHADING_API CACMISColorUniformityOneImg
{
	CColorShadingDevGeneric* m_pImpl;

public:
	CACMISColorUniformityOneImg(void);
	~CACMISColorUniformityOneImg(void);

	bool Inspect(const BYTE* pRaw8bit, int nImageWidth, int nImageHeight, int nBayerOrder, int nGridSizeX, int nGridSizeY);

	int GetGridCount(EShadingDevPos ePos);
	RECT& GetGridRect(EShadingDevPos ePos, int nIndex);
	TGridData& GetGridMean(EShadingDevPos ePos, int nIndex);
	EShadingDevPos GetGridPos(int pos_x, int pos_y, int gridSizeX, int gridSizeY);

	bool GetGridResult(EShadingDevPos ePos, int nIndex);
	TShadingDev& GetDevResult(EShadingDevPos ePos, int nIndex);

	bool InSpec(EShadingDevPos ePos, double dSpecMin, double dSpecMax);
	void GetMinMaxResult(EShadingDevPos ePos, double dMinMaxRG[2]/*0:min, 1:max*/, double dMinMaxBG[2]/*0:min, 1:max*/, double dMinMaxGrGb[2]/*0:min, 1:max*/);
};

class ACMISSHADING_API CACMISGoldenShadingDev
{
	CGoldenShadingDev* m_pImpl;

public:
	CACMISGoldenShadingDev(void);
	~CACMISGoldenShadingDev(void);

	bool Inspect(const BYTE* pRaw8bit, int nImageWidth, int nImageHeight, int nBayerOrder, int nGridSizeX, int nGridSizeY);

	int GetGridCount(EShadingDevPos ePos);
	RECT& GetGridRect(EShadingDevPos ePos, int nIndex);
	TGridData& GetGridMean(EShadingDevPos ePos, int nIndex);
	EShadingDevPos GetGridPos(int pos_x, int pos_y, int gridSizeX, int gridSizeY);

	bool GetGridResult(EShadingDevPos ePos, int nIndex);
	TShadingDev& GetDevResult(EShadingDevPos ePos, int nIndex);

	bool InSpec(EShadingDevPos ePos, double dSpecMin, double dSpecMax);
	void GetMinMaxResult(EShadingDevPos ePos, double dMinMaxRG[2]/*0:min, 1:max*/, double dMinMaxBG[2]/*0:min, 1:max*/, double dMinMaxGrGb[2]/*0:min, 1:max*/);
	void GetResultDevAverage(double* dDevAverage_rg, double* dDevAverage_bg, double* dDevAverage_grgb = NULL);

	bool SetGoldenLSCData(void* stLSCData);
	bool SetGoldenGridDataFromCsv(const char* strGoldenTableInfoPath, int nGridSizeX, int nGridSizeY);
	bool SetGoldenGainTableFromCsv(const char* strGoldenGainTableInfoPath, int nGridSizeX, int nGridSizeY);
};

