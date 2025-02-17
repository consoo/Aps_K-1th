#pragma once

#include "export.h"
#include "ACMISShadingColorShadingDef.h"
#include "../LibACMISCommon/ACMISCommon.h"

// DO NOT INCLUDE HEADER FILE!
class CRelativeUniformity;

class ACMISSHADING_API CACMISShadingRelativeUniformity
{
public:
	CACMISShadingRelativeUniformity(void);
	~CACMISShadingRelativeUniformity(void);

	bool Inspect(const BYTE* pRaw, int nImageWidth, int nImageHeight, TRelativeUniformitySpec &_Spec, EDATAFORMAT dataFormat, EOUTMODE outMode, ESENSORTYPE sensorType, int nBlackLevel);

	const TRelativeUniformityResult& GetRelativeUniformityResult(void);

	const char* GetLogHeader(void);
	const char* GetLogData(void);

	bool InSpec(const TRelativeUniformitySpec& stRelativeUniformitySpec);

private:
	CRelativeUniformity* m_pRelativeUniformity;
};
