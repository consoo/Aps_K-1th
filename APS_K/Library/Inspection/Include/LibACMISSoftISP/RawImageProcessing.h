// ImageProcessing.h: interface for the CImageProcessing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSING_H__F885DCCC_610D_4DB9_9780_6888AC47404E__INCLUDED_)
#define AFX_IMAGEPROCESSING_H__F885DCCC_610D_4DB9_9780_6888AC47404E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "export.h"
#include <list>
#include <vector>

#include <LibACMISCommon\ACMISCommon.h>


using namespace std;


namespace ACMISSoftISP
{
	typedef struct _POINTF
	{
		double x, y;
	} POINTD;

	typedef enum _EMEANTYPE
	{
		MEAN_TYPE_ALL,
		MEAN_TYPE_ROW,
		MEAN_TYPE_COLUMN,
		MEAN_TYPE_ALL_BAYER,
		MEAN_TYPE_ROW_BAYER,
		MEAN_TYPE_COLUMN_BAYER,
		MAX_COUNT_MEAN_TYPE
	} EMEANTYPE;

	class ACMISSOFTISP_API CStopWatch
	{
	private:
		LARGE_INTEGER freq, start, end;

	public:
		CStopWatch()
		{
			QueryPerformanceFrequency(&freq);
			start.QuadPart = 0;
			end.QuadPart = 0;
			StartTime();
		};

		inline void StartTime()
		{
			QueryPerformanceCounter(&start);
		};

		inline double CheckTime()
		{
			QueryPerformanceCounter(&end);

			return (double)(end.QuadPart - start.QuadPart) / (freq.QuadPart * 1000); // ms
			//TRACE("%.2f msec\n", elapsed);
		};
	};

	// blob �� ǥ���ϴ� run length encoding data
	class CBlobRunLengthData
	{
	public:
		BOOL visited;
		unsigned short blob_index;	// blob ��ȣ
		unsigned short row;
		unsigned short col_start;		// run length encoding start
		unsigned short col_length;		// run length encoding end
		CBlobRunLengthData() {};
		CBlobRunLengthData(unsigned short blob_index, unsigned short row, unsigned short start, unsigned short length)
		{
			this->blob_index = blob_index;
			this->row = row;
			this->col_start = start;
			this->col_length = length;
			this->visited = FALSE;
		};
	};

	class ACMISSOFTISP_API CRectEx
	{
	public:
		int left, top, right, bottom;

		CRectEx();
		CRectEx(int l, int t, int r, int b);
		int Width();
		int Height();
		void Offset(int x, int y);
		void SetRect(int lleft, int ltop, int lright, int lbottom);
		void SetRectCentered(int centerx, int centery, int width, int height);
		BOOL EvaluateRect(int width, int height);
		void AdjustRect(int width, int height);
		POINT CenterPoint();
		// POINT�� rect �ȿ� ��� ������ �˻��ؼ� TRUE ����
		BOOL PointInRect(int x, int y);
		void InflateRect(int x, int y);
		RECT ToRECT() const;
	};

	// blob �ϳ�, BLOB_RUN_LENGTH_DATA �� ����Ʈ�� ǥ��
	class ACMISSOFTISP_API CBlobRect : public CRectEx
	{
	public:
		int Area;
		float Intensity;
		CBlobRect();

		CBlobRect(int l, int t, int r, int b);
		// �ΰ��� blobrect �� �ϳ��� ��ħ
		CBlobRect operator + (CBlobRect &rect);

		// POINT ������ �Ÿ� ����
		static inline double GetDistance(POINT A, POINT B);

		static inline float GetDistance(float x1, float y1, float x2, float y2);

		// �� Blob Rect ���� �Ÿ�
		static inline double GetOuterDistance(CBlobRect &a, CBlobRect &b);
	};

	class ACMISSOFTISP_API CBlobList
	{
	private:
		vector <CBlobRect> *m_vecBlob;	// to avoid 'needs to have dll-interface' warning, declare pointer of vector...

	public:
		CBlobList();
		~CBlobList();

		CBlobList(const CBlobList& c)
		{
			m_vecBlob = new vector<CBlobRect>(c.m_vecBlob->size());
			std::copy(c.m_vecBlob->begin(), c.m_vecBlob->end(), m_vecBlob->begin());
		}

		CBlobList& operator=(const CBlobList& c)
		{
			m_vecBlob->resize(c.m_vecBlob->size());
			std::copy(c.m_vecBlob->begin(), c.m_vecBlob->end(), m_vecBlob->begin());
			return (*this);
		}

		void Clear();
		void Add(CBlobRect obj);
		int Size();
		CBlobRect Get(int i);
		void Remove(int index);
		CBlobList operator + (CBlobList &bloblist);

		// width, height ������ Blob ���͸�
		void SelectBlobWidthHeight(int width_low, int width_high, int height_low, int height_high, CBlobList *);
		void SelectBlobPosition(CRectEx rect, CBlobList *vecBlob);
	};

	class CRawImageProccessor
	{
	private:
		enum COLORCHANNEL
		{
			chR = 0,
			chGr,
			chGb,
			chB
		};

		enum BAYERORDER
		{
			bayerOrderRGGB = 0,
			bayerOrderBGGR
		};

		// raw image ũ�� ��ŭ�� bayer type array �� ���� �ϴ� table
		// 0 : R, 1 : G, 2 : B, 3 : c
		BYTE *BayerTypeTable;
		int *IntegralImageSum[4];
		int *IntegralImageCount[4];
		int width, height;

		void NormalizeLSCTable(int blockCountX, int blockCountY);
		void SaveTextLSCRawTrainingModel(int blockCountX, int blockCountY);

	public:
		CRawImageProccessor();
		virtual ~CRawImageProccessor();

		BYTE *ColorPlane[4];

		BYTE * GetBayerTypeTable() { return BayerTypeTable; }

		// 0 : RGBC, 1 : BGGR
		void GenerateBayerTypeTable(int width, int height, BAYERORDER bayerOrder);
		void CalculateIntegralImage(BYTE *SrcImage, int width, int height);
		void DestroyBayerTypeTable();
		void CalculateMeanBlock(CRectEx rect, float mean[4]);

		// Integral Sum �� Count �� �ش� Block �� ����� ���� (width, height �� �̹� CalculateIntegralImage ���� �̹� ���� �س���)
		void CalculateMeanBlock(int x0, int y0, int x1, int y1, float mean[4]);

		void CalculateMeanBlockDirect(BYTE *raw, int width, int height, int x0, int y0, int x1, int y1, float mean[4]);
		void SplitColorPlane(BYTE *pixel, int width, int height);
	};

	ACMISSOFTISP_API void Threshold(BYTE *srcpixel, int width, int height, CBlobList *bloblist, int min_thr, int max_thr, int MinArea, int BoundaryMarginX, int BoundaryMarginY);
	ACMISSOFTISP_API float Intensity(BYTE *pixel, int width, int height, CRectEx roi);
	ACMISSOFTISP_API void MeanImage(BYTE *image, BYTE *dest, int width, int height, int WindowSize, int startX = 0, int startY = 0, int endX = 0, int endY = 0);
	ACMISSOFTISP_API void MeanImage2Byte(unsigned short *image, unsigned short *dest, int width, int height, int WindowSize, int startX = 0, int startY = 0, int endX = 0, int endY = 0);
	ACMISSOFTISP_API void MeanImage4Byte(int *image, int *dest, int width, int height, int WindowSize, int startX = 0, int startY = 0, int endX = 0, int endY = 0);
	ACMISSOFTISP_API void MeanImageFloat(BYTE *image, float *dest, int width, int height, int WindowSize, int startX = 0, int startY = 0, int endX = 0, int endY = 0);
	ACMISSOFTISP_API void MeanImageFloat2Byte(unsigned short *image, float *dest, int width, int height, int WindowSize, int startX = 0, int startY = 0, int endX = 0, int endY = 0);
	ACMISSOFTISP_API void MeanImageFloat4Byte(int *image, float *dest, int width, int height, int WindowSize, int startX = 0, int startY = 0, int endX = 0, int endY = 0);
	ACMISSOFTISP_API void MeanImageDouble(BYTE *image, double *dest, int width, int height, int WindowSize, int startX = 0, int startY = 0, int endX = 0, int endY = 0);
	ACMISSOFTISP_API void MeanImageDouble2Byte(unsigned short *image, double *dest, int width, int height, int WindowSize, int startX = 0, int startY = 0, int endX = 0, int endY = 0);
	ACMISSOFTISP_API void MeanImageDouble4Byte(int *image, double *dest, int width, int height, int WindowSize, int startX = 0, int startY = 0, int endX = 0, int endY = 0);

	ACMISSOFTISP_API double SumBlock(const BYTE *pBuffer, int nImageWidth, int nImageHeight, RECT rtROI);
	ACMISSOFTISP_API double SumBlock(const unsigned short* pBuffer, int nImageWidth, int nImageHeight, RECT rtROI);
	ACMISSOFTISP_API double SumBlock(const int* pBuffer, int nImageWidth, int nImageHeight, RECT rtROI);
	ACMISSOFTISP_API double SumBlock(const BYTE* pBuffer, int nWidth, int nHeight, int nBlockWidth, int nBlockHeight, int nStartX, int nStartY, int nStartOffsetX = 0, int nStartOffsetY = 0, int nEndOffsetX = 0, int nEndOffsetY = 0);
	ACMISSOFTISP_API double SumBlock(const unsigned short* pBuffer, int nWidth, int nHeight, int nBlockWidth, int nBlockHeight, int nStartX, int nStartY, int nStartOffsetX = 0, int nStartOffsetY = 0, int nEndOffsetX = 0, int nEndOffsetY = 0);
	ACMISSOFTISP_API double SumBlock(const int* pBuffer, int nWidth, int nHeight, int nBlockWidth, int nBlockHeight, int nStartX, int nStartY, int nStartOffsetX = 0, int nStartOffsetY = 0, int nEndOffsetX = 0, int nEndOffsetY = 0);

	ACMISSOFTISP_API void MergeAdjacentBlob(CBlobList &BlobList, CBlobList *BlobListMergedResult, int BlockCriteria, int distance, int MaxRecursiveCount = 1000);
	ACMISSOFTISP_API float CalculateRegionIntensity(BYTE *pixel, int width, int height, CRectEx roi);

	ACMISSOFTISP_API void LensShadingCorrection(BYTE *pixel, BYTE *DestPixel, int MeanBlockSize, int width, int height, int nStartOffsetX = 0, int nStartOffsetY = 0, int nEndOffsetX = 0, int nEndOffsetY = 0);
	ACMISSOFTISP_API void LensShadingCorrection(unsigned short *pixel, unsigned short *DestPixel, int MeanBlockSize, int width, int height, int nStartOffsetX = 0, int nStartOffsetY = 0, int nEndOffsetX = 0, int nEndOffsetY = 0);
	ACMISSOFTISP_API void LensShadingCorrection(int *pixel, int *DestPixel, int MeanBlockSize, int width, int height, EDATAFORMAT eDataFormat, int nStartOffsetX = 0, int nStartOffsetY = 0, int nEndOffsetX = 0, int nEndOffsetY = 0);
	ACMISSOFTISP_API float IntensityFiltered(BYTE *pixel, int width, int height, CRectEx roi, float filter);
	ACMISSOFTISP_API float IntensityMax(float *pixel, int width, int height, RECT roi);
	ACMISSOFTISP_API bool IntensityMax(float *pixel, int width, int height, RECT roi, float &maxValue, int &maxX, int &maxY);
	ACMISSOFTISP_API void IntensityRGB(BYTE *pixel, int width, int height, CRectEx roi, float *R, float *G, float *B);

	ACMISSOFTISP_API int CalcAvgSdv(BYTE *pBuffer, int nImageWidth, int nImageHeight, RECT &rtROI, long double *dAvg, long double *dVar, long double *dSdv, EMEANTYPE eMeanType);
	ACMISSOFTISP_API int CalcAvgSdv(short *pBuffer, int nImageWidth, int nImageHeight, RECT &rtROI, long double *dAvg, long double *dVar, long double *dSdv, EMEANTYPE eMeanType);
	ACMISSOFTISP_API int CalcAvgSdv(unsigned short *pBuffer, int nImageWidth, int nImageHeight, RECT &rtROI, long double *dAvg, long double *dVar, long double *dSdv, EMEANTYPE eMeanType);
	ACMISSOFTISP_API int CalcAvgSdv(int *pBuffer, int nImageWidth, int nImageHeight, RECT &rtROI, long double *dAvg, long double *dVar, long double *dSdv, EMEANTYPE eMeanType);
	ACMISSOFTISP_API int CalcAvgSdv(float *pBuffer, int nImageWidth, int nImageHeight, RECT &rtROI, long double *dAvg, long double *dVar, long double *dSdv, EMEANTYPE eMeanType);
	ACMISSOFTISP_API int CalcAvgSdv(double *pBuffer, int nImageWidth, int nImageHeight, RECT &rtROI, long double *dAvg, long double *dVar, long double *dSdv, EMEANTYPE eMeanType);
	ACMISSOFTISP_API int CalcAvgSdv(void *pBuffer, int nImageWidth, int nImageHeight, RECT &rtROI, long double *dAvg, long double *dVar, long double *dSdv, EMEANTYPE eMeanType, int Variable_Type);
	ACMISSOFTISP_API int LoadImageFile(TFileInfo& tFileInfo, BYTE **pBuffer);
	ACMISSOFTISP_API bool AverageImage(BYTE **pBuffer, int nWidth, int nHeight, TDATASPEC tDataSpec, int nImageCount, BYTE *pDstBuffer);
	ACMISSOFTISP_API bool GetIRImage(const BYTE **pSrcBuffer, int nImageWidth, int nImageHeight, TDATASPEC tDataSpec, BYTE *pDstBuffer);
	ACMISSOFTISP_API void MakeGammaLUT(EDATAFORMAT eDataFormat, double m_dGamma, std::vector<unsigned short> &m_vGamma);
	ACMISSOFTISP_API void MakeGammaLUT(EDATAFORMAT eDataFormat, double m_dGamma, std::vector<int> &m_vGamma);

	ACMISSOFTISP_API bool xMakeCompPWLLUT(int nSizeLut, int nSizeInOut, long * pLutX, long * pLutY);
	ACMISSOFTISP_API long* xGetCompPWLLUTData();
	ACMISSOFTISP_API long xGetCompPWLLUTDataSize();
	ACMISSOFTISP_API void xClearCompPWLLUTData();
	ACMISSOFTISP_API bool xMakeDecompPWLLUT(int nSizeLut, int nSizeInOut, long * pLutX, long * pLutY);
	ACMISSOFTISP_API long* xGetDecompPWLLUTData();
	ACMISSOFTISP_API long xGetDecompPWLLUTDataSize();
	ACMISSOFTISP_API void xClearDecompPWLLUTData();

	ACMISSOFTISP_API void BayerSplit(void *Bayer, int bayerPattern, void *Dst);
	ACMISSOFTISP_API void AdjustBayerPattern(int bayerPattern, RECT rtROI, int &AdjustedBayerPattern);
};

#endif // !defined(AFX_IMAGEPROCESSING_H__F885DCCC_610D_4DB9_9780_6888AC47404E__INCLUDED_)
