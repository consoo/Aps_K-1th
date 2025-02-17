#pragma once

#include "export.h"
#include "ACMISResolutionDef.h"


class ACMISRESOLUTION_API CACMISFiducialMark
{
	CACMISFiducialMark(const CACMISFiducialMark& c);
	CACMISFiducialMark& operator=(const CACMISFiducialMark& c);
	bool ConvertSpec(TFiducialMarkSpec _Spec, TFiducialMarkSpecN &_SpecN);

public:
	CACMISFiducialMark(void);
	~CACMISFiducialMark(void);

	bool Inspect(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TFiducialMarkSpec& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool Inspect(TBufferInfo& tBufferInfo, TFiducialMarkSpec& _Spec);
	bool Inspect(TFileInfo& tFileInfo, TFiducialMarkSpec& _Spec);
	bool Inspect(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TFiducialMarkSpecN& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool Inspect(TBufferInfo& tBufferInfo, TFiducialMarkSpecN& _Spec);
	bool Inspect(TFileInfo& tFileInfo, TFiducialMarkSpecN& _Spec);
	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TFiducialMarkSpecN& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TFiducialMarkSpecN& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TFiducialMarkSpecN& _Spec);

	/// @brief Get the header data of CSV log file.
	/// @return log file header string
	const char* GetLogHeader();

	/// @brief Get the data of CSV log file.
	/// @return log file data string
	const char* GetLogData();

	/// @brief Get the version of Resolution algorithm.
	/// @return version data string
	const char* GetVersion();

	/// Chart Process
	bool SetFiducialMarkSpec(TFiducialMarkInfo* p, RECT *ptRect, int nImageWidth, int nImageHeight);
	bool SetFiducialMarkSpec(TFiducialMarkInfoN* p, RECT *ptRect, int nImageWidth, int nImageHeight);
	bool SetFiducialMarkSpec(TFiducialMarkInfoN* p, POINT *ptMark, int nImageWidth, int nImageHeight);
	bool SetFiducialMarkSpec(TFiducialMarkInfoN* p, TROICPoint *ptMark, int nImageWidth, int nImageHeight);
	bool SetFiducialMarkSpec(TFiducialMarkInfoN* p, TROIField *ptMark, int nImageWidth, int nImageHeight);
	bool SetFiducialMarkSpec(TFiducialMarkInfoN* p, TROISPoint *ptMark, int nImageWidth, int nImageHeight);

	bool ScanFiducialMark(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TFiducialMarkSpec& _Spec, TDATASPEC& tDataSpec, bool bUsing8BitOnly = false, bool bPartialDemosaic = false);
	bool ScanFiducialMark(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TFiducialMarkSpecN& _Spec, TDATASPEC& tDataSpec, bool bUsing8BitOnly = false, bool bPartialDemosaic = false);

	int GetMaxFiducialMarkCount() const;
	const CxDRect& GetFiducialMarkROI(int nIndex) const;
	const CxDPoint& GetFiducialMarkPoint(int nIndex) const;
	const CxDRect& GetFiducialMarkRadius(int nIndex) const;
	const TFindROIResult* GetSFRAutoROI(int nIndex) const;
	int GetSFRAutoROICount() const;

	const CxDPoint& GetRefChartLength() const;

	bool CalcDFOV(int nImageWidth, int nImageHeight);
	bool CalcTiltAndRotation(int nImageWidth, int nImageHeight);
	bool CalcRotation();
	bool CalcDistortion();
	bool CalcTilt(int nImageWidth, int nImageHeight);

	double GetDFOV() const;
	double GetHFOV() const;
	double GetVFOV() const;
	double GetRotation() const;
	double GetDistortion() const;

	const CxDPoint& GetTilt() const;
	const CxDPoint& GetRealCenter() const; // it's NOT an Image Center. Chart center
	const CxDPoint& GetCmPerPixel() const;

	/// Check Spec
	bool InSpecDFOV(double dValue = UNDEFINED_RESOLUTION_VALUE);
	bool InSpecHFOV(double dValue = UNDEFINED_RESOLUTION_VALUE);
	bool InSpecVFOV(double dValue = UNDEFINED_RESOLUTION_VALUE);
	bool InSpecTiltX(double dValue = UNDEFINED_RESOLUTION_VALUE);
	bool InSpecTiltY(double dValue = UNDEFINED_RESOLUTION_VALUE);
	bool InSpecRotation(double dValue = UNDEFINED_RESOLUTION_VALUE);
	bool InSpecDistortion(double dValue = UNDEFINED_RESOLUTION_VALUE);

	const double& GetSpecDFOVMin() const;
	const double& GetSpecDFOVMax() const;
	const double& GetSpecHFOVMin() const;
	const double& GetSpecHFOVMax() const;
	const double& GetSpecVFOVMin() const;
	const double& GetSpecVFOVMax() const;
	const double& GetSpecTiltXMin() const;
	const double& GetSpecTiltXMax() const;
	const double& GetSpecTiltYMin() const;
	const double& GetSpecTiltYMax() const;
	const double& GetSpecRotationMin() const;
	const double& GetSpecRotationMax() const;
	const double& GetSpecDistortionMin() const;
	const double& GetSpecDistortionMax() const;

	/// forced Setting data
	void SetFiducialMarkPoint(CxDPoint* ptData, int nMaxIndex);
	void SetDFOV(double dDFOV);
	void SetTilt(double dTiltX, double dTiltY);
	void SetRotation(double dRotation);

	inline void SetInspectPosOffset(int nStartOffsetX, int nStartOffsetY, int nEndOffsetX, int nEndOffsetY) const;
	inline void SetInspectPosOffset(TInspectRegionOffset tInspectRegionOffset) const;
	inline long long GetElapsedTime() const;
	inline double GetSaturationResult(int nIndex = 0) const;

private:
	std::chrono::system_clock::time_point m_startTime;
	std::chrono::system_clock::time_point m_endTime;
	std::chrono::milliseconds m_elapsedTime;
	std::chrono::milliseconds m_startTimems;
	std::chrono::milliseconds m_endTimems;

	std::string m_strLogHeader;
	std::string m_strLogData;
};


