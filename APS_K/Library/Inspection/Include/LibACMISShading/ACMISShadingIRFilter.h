#pragma once

#include "export.h"
#include "ACMISShadingColorShadingDef.h"
#include <iostream>

// DO NOT INCLUDE HEADER FILE!
class CIRFilter;

class ACMISSHADING_API CACMISShadingIRFilter
{
public:
	CACMISShadingIRFilter(void);
	~CACMISShadingIRFilter(void);

	bool Inspect(const BYTE* m_pFrameBuffer, int nWidth, int nHeight, TIRFilterSpec m_stIRFilterSpec,
		EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int blackLevel, double dROIRatio_Width, double dROIRatio_Height);

	const double GetIRFilterResult(void);

	const char* GetLogHeader(void);

	const char* GetLogData(void);

	bool CACMISShadingIRFilter::InSpec(const TIRFilterSpec& stIRFilterSpec);

	const RECT GetInspectionROI(void);

private:
	CIRFilter* m_pIRFilter;
	TIRFilterSpec m_stIRFilterSpec;
	TIRFilterResult m_stIRFilterResult;
};