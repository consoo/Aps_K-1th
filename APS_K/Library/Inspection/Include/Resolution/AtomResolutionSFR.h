#pragma once

#include "export.h"
#include "AtomResolutinDef.h"
#include <memory>
#include <vector>

// DO NOT INCLUDE HEADER FILES!!
class CAlgorithmSFR;
class CSFRChart;

class ATOMRESOLUTION_API CAtomResolutionSFR
{
public:
	CAtomResolutionSFR(void);
	~CAtomResolutionSFR(void);

	///@brief This function sets imageBuffer & imageInfo for inspection.
	///       it must be called before inspection.
	bool SetImageBuffer(ResolutionImageBufferManager& mgr);
	
	///@brief This function set the config for calculating SFR
	void SetAlgorithmConfig(int nMaxROIWidth, int nMaxROIHeight, ESFRAlgorithmType eType, bool bUseFFT);

	///@brief This function calculates SFR.
	///@param [in] rtROI Calculating area
	///@param [in] nEdgeDir An Edge direction. if an edge is vertical, this value is 0. if it's vertical edge, this value is 1.
	///@param [in] pFrequency Nyquest frequency lists which you want to get. ex) N/4=0.125, N/8=0.0625
	///@param [in] nFrequencyNum Frequency lists number
	///@param [out] pdSFRResult The calculated SFR result buffer. It must be inputed allocated memory buffer as nFrequencyNum.
	///@param [in] nIndex ROI Index
	///@return success or fail
	bool CalcSFR(RECT& rtROI, int nEdgeDir, double* pFrequencyLists, int nFrequencyNum, double* pdSFRResult);

	/// @brief Get the header data of CSV log file.
	/// @return log file header string
	const char* GetLogHeader();

	/// @brief Get the data of CSV log file.
	/// @return log file data string
	const char* GetLogData();

	/// Chart Process
	bool SetChartSpec(TChartSpec* p,  int nImageWidth, int nImageHeight);

	bool ScanMainFiducialMark();
	bool ScanSubFiducialMark();
	
	int GetMaxMainFiducialMarkCount() const;
	const CxDRect& GetMainFiducialMarkROI(int nIndex) const;
	const CxDPoint& GetMainFiducialMarkPoint(int nIndex) const;
	const CxDRect& GetMainFiducialMarkRadius(int nIndex) const;

	int GetMaxSubFiducialMarkCount() const;
	const CxDRect& GetSubFiducialMarkROI(int nIndex) const;
	const CxDPoint& GetSubFiducialMarkPoint(int nIndex) const;
	const CxDRect& GetSubFiducialMarkRadius(int nIndex) const;

	const CxDPoint& GetRefChartLength() const;

	bool CalcDFOV();
	bool CalcTiltAndRotation();
	bool CalcDistortion();

	double GetDFOV() const;
	double GetRotation() const;
	const CxDPoint& GetTilt() const;
	const CxDPoint& GetRealCenter() const; // it's NOT an Image Center. Chart center
	const CxDPoint& GetCmPerPixel() const;
	double GetDistortion() const;

	void InitResolution();
	bool CalcResolution();
	int GetMaxResolutionCount() const;

	double GetFinalSFR(int nFieldNo, int nFrequencyIndex = 0);
	EResolutionClass GetResolutionClass() const;
	const TSFRROIResult* GetSFRROIResult(int nFieldNo) const;

	/// Check Spec
	bool InSpecDFOV(double dValue = UNDEFINED_RESOLUTION_VALUE);
	bool InSpecTiltX(double dValue = UNDEFINED_RESOLUTION_VALUE);
	bool InSpecTiltY(double dValue = UNDEFINED_RESOLUTION_VALUE);
	bool InSpecRotation(double dValue = UNDEFINED_RESOLUTION_VALUE);
	bool InSpecDistortion(double dValue = UNDEFINED_RESOLUTION_VALUE);
	bool InSpecResolution(int nFieldNo, int nFrequencyIndex = 0, double dValue = UNDEFINED_RESOLUTION_VALUE);

	const DRANGE& GetSpecDFOV() const;
	const DRANGE& GetSpecTiltX() const;
	const DRANGE& GetSpecTiltY() const;
	const DRANGE& GetSpecRotation() const;
	double GetSpecResolution(int nFieldNo, int nFrequencyIndex=0) const;
	const TSFRROIInfo& GetSFRROIInfo(int nFieldNo) const;

	/// forced Setting data
	void SetFiducialMarkPoint(CxDPoint* ptData, int nMaxIndex);
	void SetDFOV(double dDFOV);
	void SetTilt(double dTiltX, double dTiltY);
	void SetRotation(double dRotation);
	void SetResolutionAll(double* pSFR, int nMaxFieldNum, EResolutionClass eClassResult);

private:
	CAlgorithmSFR* m_pAlgorithm;
	CSFRChart* m_pChart;
	ResolutionBufferPtr<unsigned short> m_BufferPtr;
	std::vector<unsigned short> m_vRawBuffer;
};


