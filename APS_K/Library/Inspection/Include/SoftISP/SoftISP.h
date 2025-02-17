#pragma once

#include "export.h"

#ifndef max
#define max(a, b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b)            (((a) < (b)) ? (a) : (b))
#endif

namespace AtomSoftISP
{
	typedef enum _EOUTMODE
	{
		OUTMODE_YUV = 0,
		OUTMODE_BAYER_BGGR,
		OUTMODE_BAYER_RGGB,
		OUTMODE_BAYER_GBRG,
		OUTMODE_BAYER_GRBG,
		OUTMODE_BAYER_BLACKWHITE
	} EOUTMODE;

	typedef enum _EYUVOutMode
	{
		YUV422OutMode_YCbYCr = 1,
		YUV422OutMode_YCrYCb = 2,
		YUV422OutMode_CbYCrY = 3,
		YUV422OutMode_CrYCbY = 4
	} EYUVOutMode;

	typedef enum _EDATAFORMAT
	{
		DATAFORMAT_YUV,
		DATAFORMAT_BAYER_8BIT,
		DATAFORMAT_BAYER_10BIT,
		DATAFORMAT_BAYER_PARALLEL_10BIT,
		DATAFORMAT_BAYER_12BIT,
		DATAFORMAT_BAYER_PARALLEL_12BIT,
		DATAFORMAT_BAYER_RCCC_12BIT
	} EDATAFORMAT;

	ATOMSOFTISP_API void xPacked10bitBuffer(short* p10BitBuffer, const BYTE* pOrgBuffer, int nWidth, int nHeight, int nBlackLevel);
	ATOMSOFTISP_API void xPacked12bitBuffer(short* p12BitBuffer, const BYTE* pOrgBuffer, int nWidth, int nHeight, int nBlackLevel); // CSI-2 Standard
	ATOMSOFTISP_API void xPacked2byteBuffer(short* p10BitBuffer, const BYTE* pOrgBuffer, int nWidth, int nHeight, int nBlackLevel);
	ATOMSOFTISP_API void xMake10to8Buffer(BYTE* p8BitBuffer, const short* p10bitBuffer, int nWidth, int nHeight);
	ATOMSOFTISP_API void xMake12to8Buffer(BYTE* p8BitBuffer, const short* p12bitBuffer, int nWidth, int nHeight);

	// Y = 0.2990*R + 0.5870*G + 0.1140*B;
	ATOMSOFTISP_API void xBMPtoY(BYTE* pY, const BYTE* pBMP, int nWidth, int nHeight);
	ATOMSOFTISP_API void xBMPtoY(SHORT* pY, const SHORT* pBMP, int nWidth, int nHeight);

	ATOMSOFTISP_API void xInterpolationRCCC(BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EOUTMODE eFormat = OUTMODE_BAYER_GRBG);
	ATOMSOFTISP_API void xInterpolation2ByteRCCC(BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight, EOUTMODE eFormat = OUTMODE_BAYER_GRBG);
	ATOMSOFTISP_API void xInterpolation3x3bilinear(EOUTMODE eFormat, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight);
	ATOMSOFTISP_API void xInterpolationWithOpenCV(EOUTMODE eFormat, BYTE *pRaw, BYTE *pBMP, int nWidth, int nHeight);

	ATOMSOFTISP_API bool xMeanBayerData(const BYTE* pRaw, int nWidth, int nHeight, RECT& rtROI, double* pdMeanData);
	ATOMSOFTISP_API bool xMean2ByteBayerData(const SHORT* pRaw, int nWidth, int nHeight, RECT& rtROI, double* pdMeanData);

	// YCbCr 422 to RGB
	ATOMSOFTISP_API void xYCbCrSD422toRGB(EYUVOutMode eOutMode, const BYTE *pYuvBuffer, BYTE *pBmpBuffer, int size_x, int size_y);

	// Y Table
	ATOMSOFTISP_API void xMakeYLookupTable();
	ATOMSOFTISP_API BYTE xGetY(BYTE r, BYTE g, BYTE b);

	// Lens Shading Correction
	ATOMSOFTISP_API void xCreateLensShadingCorrectionGainTable(int nImageWidth, int nImageHeight, int nScaleLevel, double dGain);
	ATOMSOFTISP_API void xLensShadingCorrection(BYTE* pRaw, int nImageWidth, int nImageHeight, int nScaleLevel);
};



