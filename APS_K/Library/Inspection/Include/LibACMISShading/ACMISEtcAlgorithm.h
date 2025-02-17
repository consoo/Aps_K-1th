#pragma once

#include "export.h"
#include "ACMISEtcDef.h"

// DO NOT INCLUDE HEADER FILE!

// Color Reproduction
class ACMISETC_API CACMISColorReproduction : public CACMISEtcCommon<TColorReproductionSpecN, TColorReproductionResult>
{
public:
	CACMISColorReproduction(void);
	~CACMISColorReproduction(void);

	bool Inspect(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TColorReproductionSpec& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool Inspect(TBufferInfo& tBufferInfo, TColorReproductionSpec& _Spec);
	bool Inspect(TFileInfo& tFileInfo, TColorReproductionSpec& _Spec);
	bool Inspect(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TColorReproductionSpecN& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool Inspect(TBufferInfo& tBufferInfo, TColorReproductionSpecN& _Spec);
	bool Inspect(TFileInfo& tFileInfo, TColorReproductionSpecN& _Spec);
	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TColorReproductionSpecN& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TColorReproductionSpecN& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TColorReproductionSpecN& _Spec);
	bool InSpec(TColorReproductionSpecN& _Spec);
	bool InSpec(TColorReproductionSpec& _Spec);
	bool InSpec(int nIndex, TColorReproductionSpecN& _Spec);

private:
	bool ConvertSpec(TColorReproductionSpec _Spec, TColorReproductionSpecN& _NewSpec);
};

// Color Intensity
class ACMISETC_API CACMISColorIntensity : public CACMISEtcCommon<TColorIntensitySpec, TColorIntensityResult>
{
public:
	CACMISColorIntensity(void);
	~CACMISColorIntensity(void);

	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TColorIntensitySpec& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TColorIntensitySpec& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TColorIntensitySpec& _Spec);
	bool InSpec(TColorIntensitySpec& _Spec);
	bool InSpec(int nIndex, TColorIntensitySpec& _Spec);
};
#define CACMISShadingColorIntensity CACMISColorIntensity

// IR Filter
class ACMISETC_API CACMISIRFilter : public CACMISEtcCommon<TIRFilterSpecN, TIRFilterResult>
{
public:
	CACMISIRFilter(void);
	~CACMISIRFilter(void);

	bool Inspect(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TIRFilterSpec& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool Inspect(TBufferInfo& tBufferInfo, TIRFilterSpec& _Spec);
	bool Inspect(TFileInfo& tFileInfo, TIRFilterSpec& _Spec);
	bool Inspect(const BYTE* pBuffer, int nImageWidth, int nImageHeight, TIRFilterSpecN& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool Inspect(TBufferInfo& tBufferInfo, TIRFilterSpecN& _Spec);
	bool Inspect(TFileInfo& tFileInfo, TIRFilterSpecN& _Spec);
	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TIRFilterSpecN& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TIRFilterSpecN& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TIRFilterSpecN& _Spec);
	const TIRFilterResult* GetInspectionResult() const;
	const RECT* GetInspectionROI() const;
	bool InSpec(TIRFilterSpecN& _Spec);
	bool InSpec(TIRFilterSpec& _Spec);
	bool InSpec(int nIndex, TIRFilterSpecN& _Spec);

private:
	bool ConvertSpec(TIRFilterSpec _Spec, TIRFilterSpecN& _NewSpec);
};
#define CACMISShadingIRFilter CACMISIRFilter

// Flare
class ACMISETC_API CACMISFlare : public CACMISEtcCommon<TFlareSpec, TFlareResult>
{
public:
	CACMISFlare(void);
	~CACMISFlare(void);

	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TFlareSpec& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TFlareSpec& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TFlareSpec& _Spec);
	bool InSpec(TFlareSpec& _Spec);
	bool InSpec(int nIndex, TFlareSpec& _Spec);
};
#define CACMISShadingFlare CACMISFlare

// View Mode for Distance error after correction
class ACMISETC_API CACMISViewMode : public CACMISEtcCommon<TViewModeSpec, TViewModeResult>
{
public:
	CACMISViewMode(void);
	~CACMISViewMode(void);

	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TViewModeSpec& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TViewModeSpec& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TViewModeSpec& _Spec);
	const TViewModeResult* GetInspectionResult() const;
};
#define CACMISShadingViewMode CACMISViewMode

// Saturation
class ACMISETC_API CACMISSaturation : public CACMISEtcCommon<TSaturationSpec, TSaturationResult>
{
public:
	CACMISSaturation(void);
	~CACMISSaturation(void);

	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TSaturationSpec& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TSaturationSpec& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TSaturationSpec& _Spec);
	bool InSpec(TSaturationSpec& _Spec);
	bool InSpec(int nIndex, TSaturationSpec& _Spec);
};

// StrayLight
class ACMISETC_API CACMISStrayLight : public CACMISEtcCommon<TStrayLightSpec, TStrayLightResult>
{
public:
	CACMISStrayLight(void);
	~CACMISStrayLight(void);

	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TStrayLightSpec& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TStrayLightSpec& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TStrayLightSpec& _Spec);

	bool InSpec(TStrayLightSpec& _Spec);
	bool InSpec(int nIndex, TStrayLightSpec& _Spec);
};

