#pragma once

#include "export.h"
#include <LibACMISShading\ColorReproduction.h>

// DO NOT INCLUDE HEADER FILE!
class CColorReproduction;

class ACMISSHADING_API CACMISColorReproduction
{
public:
	CACMISColorReproduction(void);
	~CACMISColorReproduction(void);

	bool Inspect(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TColorReproductionSpec& pSpec, EDATAFORMAT dataFormat, EOUTMODE outMode, ESENSORTYPE sensorType, int nBlackLevel);

	int GetColorReproductionRegionCount();

	const TColorReproductionResult* GetColorReproductionResult(int nIndex);

	const char* GetLogHeader(void);

	const char* GetLogData(void);

	const char* GetVersion(void);

	bool InSpec(const TColorReproductionSpec& stColorReproductionSpec);

	inline RECT* GetColorReproductionROI(int nIndex);

private:
	CColorReproduction* m_pColorReproduction;
};
