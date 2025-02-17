#pragma once

#include "export.h"
#include "ACMISShadingColorShadingDef.h"

// DO NOT INCLUDE HEADER FILE!
class CRelativeIllumination;

class ACMISSHADING_API CACMISShadingRelativeIllumination
{
public:
	CACMISShadingRelativeIllumination(void);
	~CACMISShadingRelativeIllumination(void);

	bool Inspect(const BYTE* pRaw10bit, int nImageWidth, int nImageHeight, int nBayerOrder, double boxField, int boxSize);

	const TRelativeIlluminationResult& GetRelativeIlluminationResult(void);

	const char* GetLogHeader(void);

	const char* GetLogData(void);

	bool InSpec(const TRelativeIlluminationSpec& stRelativeIlluminationSpec);

	inline RECT* GetInspectionROI(EPos ePos);

private:
	CRelativeIllumination* m_pRelativeIllumination;
};
