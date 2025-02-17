#pragma once

#include "export.h"
#include "ACMISShadingColorShadingDef.h"

class CColorSensitivity;

class ACMISSHADING_API CACMISShadingColorSensitivity
{
public:
	CACMISShadingColorSensitivity(void);
	~CACMISShadingColorSensitivity(void);

	bool SetFactorValue(bool bEnable, double rg_factor, double bg_factor, double grgb_factor);

	bool Inspect(const BYTE* pRaw8bit, int nImageWidth, int nImageHeight, int nBayerOrder, double dShadingRatio);

	const TColorRatio& GetShadingResult(void);

	const char* GetLogHeader(void);

	const char* GetLogData(void);

	bool InSpec(const TColorShadingSpec& stColorShadingSpec);

	inline RECT* GetInspectionROI(); 

private:
	CColorSensitivity* m_pColorSensitivity;
};

