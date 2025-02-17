#pragma once

#include "export.h"
#include "CDemosaic.h"
#include "SoftISP.h"


typedef struct _THarrisCornerPoint
{
	POINT ptCorner;
	double dValue;
	int nPatchIndex;
} THarrisCornerPoint;

typedef struct _TPatchInfo
{
	RECT rtROI;
	POINT ptCenter;
	POINT ptStartVertex;
	double dThMin;
	double dThMax;
	int nPatchShape;
	int nPatchColor;
	int nMinDistance;
	bool bSeparate;
} TPatchInfo;

typedef struct _TPatchSize
{
	int nArea;
	int nLeft;
	int nTop;
	int nWidth;
	int nHeight;
} TPatchSize;

typedef enum _EPatchShape
{
	PatchShape_Square,
	PatchShape_Clamp,
	PatchShape_PacMan,
	PatchShape_Fan
} EPatchShape;

#define MAX_CORNER_OF_PATCH		6

namespace ACMISSoftISP
{
	typedef struct _COMPLEX
	{
		double re;
		double im;
	} COMPLEX;

#define HIGH_CLIP(x, limit) (((x) > (limit)) ? (limit) : (x))
#define LOW_CLIP(x, limit)  (((x) < (limit)) ? (limit) : (x))
#define CLIP(x, l, h) HIGH_CLIP(LOW_CLIP(x, l), h)

#define CHECK_HIGH_LIMIT(x, limit) (((x) > (limit)) ? (false) : (true))
#define CHECK_LOW_LIMIT(x, limit)  (((x) < (limit)) ? (false) : (true))
#define CHECK_LIMIT(x, l, h) (CHECK_HIGH_LIMIT(x, h) && CHECK_LOW_LIMIT(x, l))

#ifndef ABS
#define ABS(x) ((x < 0) ? (-(x)) : (x))
#endif

#define	ROUND2INTEGER(X) (((X) > 0) ? ((int)((X) + 0.5)) : ((int)((X) - 0.5)))

#define MAX_POLYFIT_N 		9
#define LSC_SHIFT_FACTOR 	8
#define LSC_FACTOR 			256
#define LS_DATA_ROI_SIZE 	50
#define LSC_IMAGE_MAX_X		5500
#define LSC_IMAGE_MAX_Y		3190
#define MAX_LSC_INDEX 		(LSC_IMAGE_MAX_X * LSC_IMAGE_MAX_Y / LS_DATA_ROI_SIZE / LS_DATA_ROI_SIZE + 50)
#define ROUND2EVEN(x) 		((int)((((float)(x) / 2.0) + 0.5) * 2.0))
#define MAX_LSC_CHANNEL 	4

//#define USE_ELLIPSE_ANGLE
#ifdef USE_ELLIPSE_ANGLE
#define	CIRCLE_POSX(OCX, OCY, PosX, PosY, Angle)  ((double)(OCX - PosX) * cos(Angle * M_PI / 180.0) + (double)(OCY - PosY) * sin(Angle * M_PI / 180.0))
#define	CIRCLE_POSY(OCX, OCY, PosX, PosY, Angle)  (-(double)(OCX - PosX) * sin(Angle * M_PI / 180.0) + (double)(OCY - PosY) * cos(Angle * M_PI / 180.0))
#else
#define	CIRCLE_POSX(OCX, OCY, PosX, PosY, Angle)  ((double)(OCX - PosX))
#define	CIRCLE_POSY(OCX, OCY, PosX, PosY, Angle)  ((double)(OCY - PosY))
#endif

//#define POSITION(x, roi)	(((x + 1) * roi - roi / 2) * 2 - 1)
#define POSITION_CENTER(x, bsize, size0, ch)	((x == 0 ? size0 / 2 : ((x - 1) * bsize + size0 + bsize / 2)) * ch)
#define POSITION_START(x, bsize, size0, ch)		((x == 0 ? 0 : ((x - 1) * bsize + size0)) * ch)
#define POSITION_END(x, bsize, size0, ch)		((x == 0 ? size0 : (x * bsize + size0)) * ch - 1)

	struct sLensShadingCorrectionData
	{
		double ocx;
		double ocy;
		int maxIndex;
		double maxData[MAX_LSC_CHANNEL];
		double data[MAX_LSC_CHANNEL][MAX_LSC_INDEX * 3];
		//	double error[MAX_LSC_CHANNEL][MAX_LSC_INDEX * 3];
		//	double fitCurve[MAX_LSC_CHANNEL][MAX_LSC_INDEX * 3];
		//	double rsquare[MAX_LSC_CHANNEL];
		double coefficientLSCcurve[MAX_LSC_CHANNEL][MAX_POLYFIT_N + 1];
		int x[MAX_LSC_INDEX * 3];
		int y[MAX_LSC_INDEX * 3];
		double distance[MAX_LSC_INDEX * 3];
		double tableLSCcurve[MAX_LSC_CHANNEL][LSC_IMAGE_MAX_X * 2];
		int tableLSCgain[MAX_LSC_CHANNEL][LSC_IMAGE_MAX_X * 2];
		int tableLSCgainlength;
	};


	ACMISSOFTISP_API int xGaussianNoiseCheck(BYTE* pBMP, int nWidth, int nHeight, float fThreshold, int nChannel);
	ACMISSOFTISP_API bool xGetGaussian(int nWidth, int nHeight, double dSigma, double** dKernel);
	ACMISSOFTISP_API BOOL xMedian(double* pData, int nWidth, int nHeight, int nFilterWidth, int nFilterHeight);
	ACMISSOFTISP_API void xHistogramforBMP(BYTE* pBMP, int nWidth, int nHeight, long* pHistogram, int nChannel);
	ACMISSOFTISP_API void xHistogramEqualize(BYTE* pBMP, long* pHistogram, long lNumofPx, int nChannel);
	ACMISSOFTISP_API double xRotationX(double x, double y, double angle);
	ACMISSOFTISP_API double xRotationY(double x, double y, double angle);
	ACMISSOFTISP_API void xFFT(COMPLEX *f, int logN, int numpoints, int dir);
	ACMISSOFTISP_API void xDFT(double *x, int N, double *amplitude);
	ACMISSOFTISP_API void xGetBestFit(double *x, double *y, int n, double* slope, double* offset);
	ACMISSOFTISP_API double xRsquare(double* x, double*y, int n, double slope, double offset);
	ACMISSOFTISP_API double xGetDistance(double x1, double x2, double y1, double y2);
	ACMISSOFTISP_API double xMean(BYTE* pBMP, int nWidth, int nHeight, RECT& rtROI, int nChannel, int nSelChannel); // Channel B=0, G=1, R=2
	ACMISSOFTISP_API double xMean(unsigned short* pBMP, int nWidth, int nHeight, RECT& rtROI, int nChannel, int nSelChannel);

    ACMISSOFTISP_API int PolynomialFit(const BYTE *pixel, int count);
	ACMISSOFTISP_API int PolynomialFit(const unsigned short *pixel, int count);
	ACMISSOFTISP_API int PolynomialFit(const int *pixel, int count);
	ACMISSOFTISP_API bool PolynomialFit(const double *x, const double *y, const int count, const int order, double *coeffs);
	ACMISSOFTISP_API bool PolynomialFit(const std::vector<double> &x, const std::vector<double> &y, const int order, std::vector<double> &coeffs);
	ACMISSOFTISP_API void PolyFit(double *x, double *y, int num, double *coff, int polyN);
	ACMISSOFTISP_API void GenerateLSCtableWithPolyFitCurve(BYTE *pRaw, int width, int height, struct sLensShadingCorrectionData *dataLSC);
	ACMISSOFTISP_API void LensShadingCorrectionWithPolyFit(BYTE *pRawLSCoff, BYTE *pRawLSCon, int width, int height, struct sLensShadingCorrectionData *dataLSC);

	ACMISSOFTISP_API int xCalcImageRadius4Byte(int&ocx, int&ocy, int &radx, int &rady, const int* img, int w, int h, TCircleSpecN *Spec, double &angle, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT); // using Y Image Only
	ACMISSOFTISP_API int xCalcImageRadius2Byte(int&ocx, int&ocy, int &radx, int &rady, const unsigned short* img, int w, int h, TCircleSpecN *Spec, double &angle); // using Y Image Only
	ACMISSOFTISP_API int xCalcImageRadius(int&ocx, int&ocy, int &radx, int &rady, const BYTE* img, int w, int h, TCircleSpecN *Spec, double &angle); // using Y Image Only

	ACMISSOFTISP_API int xCalcCenterCircleObject4Byte(int&ocx, int&ocy, const int* img, int w, int h, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT); // using Y Image Only
	ACMISSOFTISP_API int xCalcCenterCircleObject2Byte(int&ocx, int&ocy, const unsigned short* img, int w, int h); // using Y Image Only
	ACMISSOFTISP_API int xCalcCenterCircleObject(int&ocx, int&ocy, const BYTE* img, int w, int h); // using Y Image Only

	ACMISSOFTISP_API void GetMeanFromYdouble(const BYTE *pRaw, int dataWidth, int dataHeight, int leftROI, int topROI, int rightROI, int bottomROI, double *mean);
	ACMISSOFTISP_API void xCalcFixedCircleCenter4Byte(int&ocx, int&ocy, const int* img, int w, int h, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API void xCalcFixedCircleCenter2Byte(int&ocx, int&ocy, const unsigned short* img, int w, int h);
	ACMISSOFTISP_API void xCalcFixedCircleCenter(int&ocx, int&ocy, const BYTE* img, int w, int h);

	ACMISSOFTISP_API int xCalcCircleData4Byte(const int* img, int w, int h, TCircleSpecN *Spec, int &m_nOcx, int &m_nOcy, int &m_nRadx, int &m_nRady, double &m_dAngle, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API int xCalcCircleData2Byte(const unsigned short* img, int w, int h, TCircleSpecN *Spec, int &m_nOcx, int &m_nOcy, int &m_nRadx, int &m_nRady, double &m_dAngle);
	ACMISSOFTISP_API int xCalcCircleData(const BYTE* img, int w, int h, TCircleSpecN *Spec, int &m_nOcx, int &m_nOcy, int &m_nRadx, int &m_nRady, double &m_dAngle);
	ACMISSOFTISP_API int xCalcCircleData4Byte(const int* img, int w, int h, TCircleSpecN *Spec, TCircleData& tCircleData, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT);
	ACMISSOFTISP_API int xCalcCircleData2Byte(const unsigned short* img, int w, int h, TCircleSpecN *Spec, TCircleData& tCircleData);
	ACMISSOFTISP_API int xCalcCircleData(const BYTE* img, int w, int h, TCircleSpecN *Spec, TCircleData& tCircleData);
	ACMISSOFTISP_API int xGetCircleMask(int w, int h, TCircleSpecN& Spec, TCircleData& tCircleData, BYTE* pMask);

	ACMISSOFTISP_API void SeparateCFA(int* pBuffer, int nImageWidth, int nImageHeight, int* pChannelLT, int* pChannelRT, int* pChannelLB, int* pChannelRB);
	ACMISSOFTISP_API void SeparateCFA(unsigned short* pBuffer, int nImageWidth, int nImageHeight, unsigned short* pChannelLT, unsigned short* pChannelRT, unsigned short* pChannelLB, unsigned short* pChannelRB);
	ACMISSOFTISP_API void SeparateCFA(BYTE* pBuffer, int nImageWidth, int nImageHeight, BYTE* pChannelLT, BYTE* pChannelRT, BYTE* pChannelLB, BYTE* pChannelRB);
	ACMISSOFTISP_API void MergeCFA(double* pChannelLT, double* pChannelRT, double* pChannelLB, double* pChannelRB, int nImageWidth, int nImageHeight, double* pBuffer);

	ACMISSOFTISP_API bool DetectSqureVertexs(const BYTE *pBuffer, int nImageWidth, int nImageHeight, RECT* inputROI, int num, POINT* outputPos);
	ACMISSOFTISP_API bool DetectPatchVertexs(const BYTE *pBuffer, int nImageWidth, int nImageHeight, TPatchInfo* tPatchInfo, int num, THarrisCornerPoint* outputPos);
	ACMISSOFTISP_API bool DetectPatchVertexsOpenCV(const BYTE *pBuffer, int nImageWidth, int nImageHeight, void *params, std::vector<TPatchInfo> vPatchInfo, THarrisCornerPoint* outputPos);
	ACMISSOFTISP_API bool DetectPatchVertexsOpenCV(const BYTE *pBuffer, int nImageWidth, int nImageHeight, double dThreshold, TPatchInfo tPatchInfo, int nPatchIndex, THarrisCornerPoint* outputPos);
	ACMISSOFTISP_API bool DetectCrossCenter(const BYTE *pBuffer, int nImageWidth, int nImageHeight, RECT* inputROI, int num, POINT* outputPos);
	ACMISSOFTISP_API bool GetPatchSize(const BYTE *pBuffer, int nImageWidth, int nImageHeight, double dThreshold, TPatchInfo tPatchInfo, TPatchSize& tMaxPatchSize);

	ACMISSOFTISP_API void HarrisCorner(const BYTE* img, int w, int h, RECT rtROI, double th, std::vector<THarrisCornerPoint>& vCorners);
	ACMISSOFTISP_API void HarrisCorner(const unsigned short* img, int w, int h, RECT rtROI, double dThreshold, std::vector<THarrisCornerPoint>& vCorners);
	ACMISSOFTISP_API void HarrisCornerOpenCV(const BYTE* img, int w, int h, RECT rtROI, double dThreshold, std::vector<THarrisCornerPoint>& vCorners);
	ACMISSOFTISP_API void HarrisCornerOpenCV(const unsigned short* img, int w, int h, RECT rtROI, double dThreshold, std::vector<THarrisCornerPoint>& vCorners);

	ACMISSOFTISP_API bool xSaveImage(char* strFileName, void *srcImage);
	ACMISSOFTISP_API bool xSaveImage(char* strFileName, BYTE *pBuffer, int nImageWidth, int nImageHeight, int nType);
	ACMISSOFTISP_API bool xSaveRawImage(char* strFileName, BYTE *pBuffer, int nImageSize);

	ACMISSOFTISP_API bool xSaturation(const BYTE* pBuffer, EDATAFORMAT eDataFormat, int nImageWidth, int nImageHeight, RECT rtROI, double dBinaryThreshold, double& dBrightAvg);
	ACMISSOFTISP_API bool xSaturation(const unsigned short* pBuffer, EDATAFORMAT eDataFormat, int nImageWidth, int nImageHeight, RECT rtROI, double dBinaryThreshold, double& dBrightAvg);
	ACMISSOFTISP_API bool xSaturation(const int* pBuffer, EDATAFORMAT eDataFormat, int nImageWidth, int nImageHeight, RECT rtROI, double dBinaryThreshold, double& dBrightAvg);
};
