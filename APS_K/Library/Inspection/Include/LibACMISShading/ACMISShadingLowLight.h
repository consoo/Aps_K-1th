#pragma once

#include "export.h"
#include "ACMISShadingColorShadingDef.h"
#include <iostream>

// DO NOT INCLUDE HEADER FILE!
class CLowLight;

class ACMISSHADING_API CACMISShadingLowLight
{
public:
	CACMISShadingLowLight(void);
	~CACMISShadingLowLight(void);

	bool Inspect(const BYTE* m_pFrameBuffer, int nWidth, int nHeight, TLowLightSpec m_stLowLightSpec,
		EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int blackLevel);

	const int GetLowLightResult(void);

	const char* GetLogHeader(void);

	const char* GetLogData(void);

	bool InSpec(const TLowLightSpec& stLowLightSpec);

	const RECT GetInspectionROI(int i);

	POINT* GetParticlePoint();

	RECT* GetParticleZone();

private:
	CLowLight* m_pLowLight;
	TLowLightSpec m_stLowLightSpec;
	TLowLightResult m_stLowLightResult;
};