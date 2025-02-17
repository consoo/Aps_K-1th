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


using namespace std;


namespace AtomSoftISP
{
	typedef struct _POINTF
	{
		double x, y;
	} POINTD;

	// blob 을 표현하는 run length encoding data
	class CBlobRunLengthData
	{
	public:
		BOOL visited;
		unsigned short blob_index;	// blob 번호
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

	class ATOMSOFTISP_API CRectEx
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
		// POINT가 rect 안에 드는 것인지 검사해서 TRUE 리턴
		BOOL PointInRect(int x, int y);
		void InflateRect(int x, int y);
		RECT ToRECT() const;
	};

	// blob 하나, BLOB_RUN_LENGTH_DATA 의 리스트로 표현
	class ATOMSOFTISP_API CBlobRect : public CRectEx
	{
	public:
		int Area;
		float Intensity;
		CBlobRect();

		CBlobRect(int l, int t, int r, int b);
		// 두개의 blobrect 를 하나로 합침
		CBlobRect operator + (CBlobRect &rect);

		// POINT 끼리의 거리 연산
		static inline double GetDistance(POINT A, POINT B);
		
		static inline float GetDistance(float x1,float y1,float x2,float y2);

		// 두 Blob Rect 간의 거리
		static inline double GetOuterDistance(CBlobRect &a, CBlobRect &b);
	};

	class ATOMSOFTISP_API CBlobList
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

		// width, height 범위로 Blob 필터링
		void SelectBlobWidthHeight(int width_low, int width_high, int height_low, int height_high, CBlobList *);
		void SelectBlobPosition(CRectEx rect, CBlobList *vecBlob);
	};

	ATOMSOFTISP_API void Threshold(BYTE *srcpixel, int width, int height, CBlobList *bloblist, int min_thr, int max_thr, int MinArea, int BoundaryMarginX, int BoundaryMarginY);
	ATOMSOFTISP_API float Intensity(BYTE *pixel, int width, int height, CRectEx roi);
	ATOMSOFTISP_API void MeanImage(BYTE *image, BYTE*, int width, int height, int WindowSize);
	ATOMSOFTISP_API void MeanImage2Byte(SHORT *image, SHORT*, int width, int height, int WindowSize);
	ATOMSOFTISP_API void MeanImageFloat(BYTE *image, float *dest, int width, int height, int WindowSize);
	ATOMSOFTISP_API void MergeAdjacentBlob(CBlobList &BlobList, CBlobList *BlobListMergedResult, int BlockCriteria, int distance);
	
	ATOMSOFTISP_API void LensShadingCorrection(BYTE *pixel, BYTE *DestPixel, int MeanBlockSize, int width, int height);
	ATOMSOFTISP_API float IntensityFiltered(BYTE *pixel, int width, int height, CRectEx roi, float filter);
	ATOMSOFTISP_API void IntensityRGB(BYTE *pixel, int width, int height, CRectEx roi, float *R, float *G, float *B);
};

#endif // !defined(AFX_IMAGEPROCESSING_H__F885DCCC_610D_4DB9_9780_6888AC47404E__INCLUDED_)
