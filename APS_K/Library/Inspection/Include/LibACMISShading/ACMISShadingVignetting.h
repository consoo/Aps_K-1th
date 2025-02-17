#pragma once

#include "export.h"
#include "ACMISShadingColorShadingDef.h"

enum
{
	VIGNETTING_Y_MEAN_RATIO = 0,
	VIGNETTING_DSTRIPE,
	VIGNETTING_ALL
};

// DO NOT INCLUDE HEADER FILE!
class CVignetting;

class ACMISSHADING_API CACMISShadingVignetting
{
public:
	CACMISShadingVignetting(void);
	~CACMISShadingVignetting(void);

	bool Inspect(const BYTE* pYImg, int nImageWidth, int nImageHeight, double dROIRatio, int nVignettingMode = VIGNETTING_Y_MEAN_RATIO);

	RECT* GetInspectionROI(EPos eROIPos, int nVignettingMode = VIGNETTING_Y_MEAN_RATIO);

	double GetVignetting(EPos ePos, int nVignettingMode = VIGNETTING_Y_MEAN_RATIO);

	bool InSpec(EPos ePos, double dVignettingSpec, int nVignettingMode = VIGNETTING_Y_MEAN_RATIO);

	/// @brief Get the header data of CSV log file.
	/// @return log file header string
	const char* GetLogHeader();

	/// @brief Get the data of CSV log file.
	/// @return log file data string
	const char* GetLogData();

private:
	CVignetting* m_pVignetting;
};

