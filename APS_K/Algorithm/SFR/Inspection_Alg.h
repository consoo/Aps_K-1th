#pragma once
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "BlobLabeling.h"


//! Modified by LHW, 2013/3/4, �޸� ����/���� �Լ� �߰�, Spot �˻� �߰�
//! Modified by LHW, 2013/3/13, GetAverageValue �Լ� �߰�
//! Modified by LHW, 2013/3/17, SFR �Լ� ����
//! Modified by LHW, 2013/3/30, Edge �˻� �߰�
//! Modified by LHW, 2013/4/18, �̹� �˻� �����Ͽ� �޸� ������ �ּ�ȭ (OpenCV ���ۿ����� ROI ����)
//! Modified by LHW, 2013/4/23, LG �̳��ؿ��� ������ SFR ���� DLL ���

#define		MAX_SFR_DATA_CNT		4000

//! LG �̳��ؿ��� ������ DLL���� SFR ���� �Լ��� ����
typedef int(*SFRalgorithm)(
	int *pROI
	, int widthROI
	, int heightROI
	, int directionROI
	, double *SFR, double *cyclePerPixel
	, double cyclePerPixel1, double *valueSFR1
	, double cyclePerPixel2, double *valueSFR2
	, int optionSFR
	);

class CInspection_Alg
{
public:
	CInspection_Alg(void);
	~CInspection_Alg(void);

public:
	//! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//! �̹� �˻�
	//! ã�Ƴ� �̹��� �� ����
	struct results
	{
		int num[_DEF_MAX_BLOBS * 4];
		int area[_DEF_MAX_BLOBS * 4];
		float gd[_DEF_MAX_BLOBS * 4];
		CvPoint	TopLeft[_DEF_MAX_BLOBS * 4];
		CvPoint	BottomRight[_DEF_MAX_BLOBS * 4];
	};
	//static struct results Blob_results;	
	//static int            Blob_results_Num;

	struct results Blob_results;
	int            Blob_results_Num;

	//! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	CString	sLogMessage;

public:
	//! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//! �̹� �˻�
	int AllocMemory(int iImageWidth, int iImageHeight);
	int AllocMemory_Spot(int iImageWidth, int iImageHeight);
	int AllocMemory_Scale(int iImageWidth, int iImageHeight, int iScale);
	void FreeMemory();

	int Inspection_Scale(IplImage* Gray_Img,
		int Scale,
		int Filter_S_Size, int Filter_L_Size,
		float m_Threshold,
		int Defect_Small_Size, int Defect_Large_Size,
		float m_GD,
		int m_Boundary, int m_Edge,
		int m_MaxDefectsCount);

	int Inspection_Spot(IplImage* Gray_Img,
		float m_Threshold,
		int Defect_Small_Size, int Defect_Large_Size,
		float m_GD,
		int m_Boundary,
		int m_MaxDefectsCount);

	int Inspection_edge(IplImage* Gray_Img,
		int Scale,
		int Filter_S_Size, int Filter_L_Size,
		float m_Threshold,
		int Defect_Small_Size, int Defect_Large_Size,
		float m_GD,
		int m_Boundary, int m_Edge,
		int m_MaxDefectsCount);

	//! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//! ���� �˻�
	bool getOpticalAxis(IplImage* src_img, int scale, CRect roi, double &offsetX, double &offsetY, int &iAvr);

	//! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//! SFR ����
	bool getSFR(IplImage* src_img, int fi_iWidth, int fi_iHight, float fi_fCcdSize, CRect fi_RectRoi, int& iDataCnt, double fo_DataX[MAX_SFR_DATA_CNT], double fo_DataY[MAX_SFR_DATA_CNT], double dGain = 1.0);
	bool getSFR2(unsigned char* r_buf, unsigned char* g_buf, unsigned char* b_buf, int fi_iWidth, int fi_iHight, float fi_fCcdSize, CRect fi_RectRoi, int& iDataCnt, double fo_DataX[MAX_SFR_DATA_CNT], double fo_DataY[MAX_SFR_DATA_CNT], double dGain = 1.0);
	double GetValue_SFR_N_4() { return m_dSFR_N_4; }
	double GetValue_SFR_N_8() { return m_dSFR_N_8; }
	double GetValue_SFR_N_4_X() { return m_dSFR_N_4_X; }
	double GetValue_SFR_N_8_X() { return m_dSFR_N_8_X; }

	HINSTANCE	 m_hDLL_SFR;					//! SFR DLL �ڵ�
	SFRalgorithm m_pSFRalgo;					//! �Լ� ������ (DLL�� SFR ��ü ���� �Լ�)
	bool Ready_SFR_DLL(CString fi_sFilePath);	//! DLL �غ�
	void Release_SFR_DLL();						//! DLL ����

												//! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
												//! 2�� Fitting
	bool calcPeakPos(int cnt, int* pos, double &val_A, double &val_B, double &val_C);
	bool _calcDoublePeakPos(int cnt, double posX[5], double posY[5], double &val_A, double &val_B, double &val_C);

	//! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//! Added by LHW (2013/3/13)
	//! �ش� Color �̹������� �� ���� ��հ��� �˾Ƴ���. 
	bool GetAverageValue(IplImage* src_img, CRect fi_RectRoi, int fi_iSkipX, int fi_iSkipY, double &fo_dAvgRed, double &fo_dAvgGreen, double &fo_dAvgBlue);

private:
	//! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//! �̹� �˻�
	CBlobLabeling m_blob;

	IplImage* m_bw;
	IplImage* m_bw_t;
	IplImage* m_Morph_Img;
	IplImage* m_Sub_Img;
	IplImage* m_Scale_Img;
	IplImage* m_filtered_Img;
	CvMat*    m_Mat_kernel;

	//! ���� �̹��� ������ ũ��
	int m_iReal_Width_Image;
	int m_iReal_Height_Image;

	//! ���� �̹����� ũ�� (���ذ��� �ʿ��ؼ� ���� �ӽ� �����Ѵ�.)
	int m_iWidth_Org_Image;
	int m_iHeight_Org_Image;

	void Make_Kernel(int S_KERNEL_LENGTH, int L_KERNEL_LENGTH, CvMat* mat_kernel);
	void Tri_Threshold(IplImage* src, float m_threshold, IplImage* bw, int scale, int boundary);
	void Edge_Except(IplImage* bw, int scale, int margin);
	int  BLOB_Analysis(IplImage* bw, IplImage* gray, int SMALL_SIZE, int LARGE_SIZE, int scale, float gray_threshold, int m_MaxDefectsCount);
	int  Spot_BLOB_Analysis(IplImage* bw, IplImage* gray, int SMALL_SIZE, int LARGE_SIZE, float gray_threshold, int m_Boundary, int m_MaxDefectsCount);

	//! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//! SFR ����
	double m_dSFR_N_4;	//! SFR N/4��, 0.125
	double m_dSFR_N_8;	//! SFR N/8��, 0.0625
	double m_dSFR_N_4_X;
	double m_dSFR_N_8_X;

	void doGraySharpening(IplImage* img);
	int Linear_Interpolation(double* xdata, double* ydata, double* L_xdata, double* L_ydata);
};

