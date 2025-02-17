#pragma once

#include "export.h"
#include "ACMISOpticalCenterDef.h"


class ACMISIMAGE_API CACMISOpticalCenterPolynomialFit : public CACMISOpticalCenter<TOpticalCenterPolyFit>
{
public:
	CACMISOpticalCenterPolynomialFit();
	~CACMISOpticalCenterPolynomialFit();

	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TOpticalCenterPolyFit& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TOpticalCenterPolyFit& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TOpticalCenterPolyFit& _Spec);
};

class ACMISIMAGE_API CACMISOpticalCenterCentroid : public CACMISOpticalCenter<TOpticalCenter>
{
public:
	CACMISOpticalCenterCentroid();
	~CACMISOpticalCenterCentroid();

	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TOpticalCenter& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TOpticalCenter& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TOpticalCenter& _Spec);
};

class ACMISIMAGE_API CACMISOpticalCenterCentroidG : public CACMISOpticalCenter<TOpticalCenterG>
{
public:
	CACMISOpticalCenterCentroidG();
	~CACMISOpticalCenterCentroidG();

	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TOpticalCenterG& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TOpticalCenterG& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TOpticalCenterG& _Spec);
};

class ACMISIMAGE_API CACMISOpticalCenterCentroidRGB : public CACMISOpticalCenter<TOpticalCenterRGB>
{
public:
	CACMISOpticalCenterCentroidRGB();
	~CACMISOpticalCenterCentroidRGB();

	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TOpticalCenterRGB& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TOpticalCenterRGB& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TOpticalCenterRGB& _Spec);
};
#define CACMISOpticalCenterCentroidRgb CACMISOpticalCenterCentroidRGB

class ACMISIMAGE_API CACMISOpticalCenterCentroidY : public CACMISOpticalCenter<TOpticalCenterY>
{
public:
	CACMISOpticalCenterCentroidY();
	~CACMISOpticalCenterCentroidY();

	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TOpticalCenterY& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TOpticalCenterY& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TOpticalCenterY& _Spec);
};

class ACMISIMAGE_API CACMISOpticalCenterDistortionDev : public CACMISOpticalCenter<TOpticalCenterDistortionDev>
{
public:
	CACMISOpticalCenterDistortionDev();
	~CACMISOpticalCenterDistortionDev();

	//const POINT* GetInspectionROI(int index) const;
};

class ACMISIMAGE_API CACMISOpticalCenterBrightest : public CACMISOpticalCenter<TOpticalCenterBrightest>
{
public:
	CACMISOpticalCenterBrightest();
	~CACMISOpticalCenterBrightest();
	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TOpticalCenterBrightest& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TOpticalCenterBrightest& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TOpticalCenterBrightest& _Spec);
};

class ACMISIMAGE_API CACMISOpticalCenterCentroidCircle : public CACMISOpticalCenter<TOpticalCenterCentroidCircle>
{
public:
	CACMISOpticalCenterCentroidCircle();
	~CACMISOpticalCenterCentroidCircle();
	bool InspectM(const BYTE** pBuffer, int nImageWidth, int nImageHeight, TOpticalCenterCentroidCircle& _Spec, EDATAFORMAT nDataFormat, EOUTMODE nOutMode, ESENSORTYPE nSensorType, int nBlackLevel, bool bUsing8BitOnly = false, int nImageCount = 1, bool bPartialDemosaic = false, EDEMOSAICMETHOD nDemosaicMethod = DEMOSAICMETHOD_GRADIENT);
	bool InspectM(TBufferInfoM& tBufferInfo, TOpticalCenterCentroidCircle& _Spec);
	bool InspectM(TFileInfoM& tFileInfo, TOpticalCenterCentroidCircle& _Spec);
};