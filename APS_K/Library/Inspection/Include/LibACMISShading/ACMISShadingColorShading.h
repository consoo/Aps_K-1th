#pragma once

#include "export.h"
#include "ACMISShadingColorShadingDef.h"

// DO NOT INCLUDE HEADER FILE!
class CColorShading;

class ACMISSHADING_API CACMISShadingColorShading
{
public:
	CACMISShadingColorShading(void);
	~CACMISShadingColorShading(void);

	bool Inspect(const BYTE* pRaw8bit, int nImageWidth, int nImageHeight, int nBayerOrder, double dShadingRatio);

	const TColorShadingResult& GetShadingResult(void);

	const TColorRatio& GetColorRatio(int nIndex);

	const char* GetLogHeader(void);

	const char* GetLogData(void);

	bool InSpec(const TColorShadingSpec& stColorShadingSpec);

	inline RECT* GetInspectionROI(EPos ePos);

private:
	CColorShading* m_pColorShading;
};
