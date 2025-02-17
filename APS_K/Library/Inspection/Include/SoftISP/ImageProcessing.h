#pragma once

#include "export.h"
#include "CDemosaic.h"

namespace AtomSoftISP
{
	typedef struct _COMPLEX
	{
		double re;
		double im;
	} COMPLEX;

#define HIGH_CLIP(x, limit) (((x) > (limit)) ? (limit) : (x))
#define LOW_CLIP(x, limit)  (((x) < (limit)) ? (limit) : (x))
#define CLIP(x, l, h) HIGH_CLIP(LOW_CLIP(x, l), h)

#ifndef ABS
#define ABS(x) ((x < 0) ? (-(x)) : (x))
#endif

#define MAX_LSC_CHANNEL 4

	struct _TCirclePosSpec
	{
		int nPosOffsetX;
		int nPosOffsetY;
		double dRadiusRatioX;
		double dRadiusRatioY;
	};
	ATOMSOFTISP_API double xRotationX(double x, double y, double angle);
	ATOMSOFTISP_API double xRotationY(double x, double y, double angle);
	ATOMSOFTISP_API void xFFT(COMPLEX *f, int logN, int numpoints, int dir);
	ATOMSOFTISP_API void xDFT(double *x, int N, double *amplitude);
	ATOMSOFTISP_API void xGetBestFit(double *x, double *y, int n, double* slope, double* offset);
	ATOMSOFTISP_API double xGetDistance(double x1, double x2, double y1, double y2);
	ATOMSOFTISP_API double xMean(BYTE* pBMP, int nWidth, int nHeight, RECT& rtROI, int nChannel, int nSelChannel); // Channel B=0, G=1, R=2
	ATOMSOFTISP_API int xCalcImageRadius2Byte(int&ocx, int&ocy, int &radx, int &rady, const short* img, int w, int h, bool bYImage, _TCirclePosSpec *Spec);
	ATOMSOFTISP_API int xCalcImageRadius(int&ocx, int&ocy, int &radx, int &rady, const BYTE* img, int w, int h, bool bYImage, _TCirclePosSpec *Spec);
};
