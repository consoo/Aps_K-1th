#include "StdAfx.h"
#include "Inspection_Alg.h"
#include "SFR.h"

CSFR	SFR;

//extern CVision		vision;

//struct CInspection_Alg::results CInspection_Alg::Blob_results;
//int CInspection_Alg::Blob_results_Num;

CInspection_Alg::CInspection_Alg(void)
{
	m_bw = NULL;
	m_bw_t = NULL;
	m_Morph_Img = NULL;
	m_Sub_Img = NULL;
	m_Scale_Img = NULL;
	m_filtered_Img = NULL;

	m_iReal_Width_Image = 0;
	m_iReal_Height_Image = 0;

	m_iWidth_Org_Image = 0;
	m_iHeight_Org_Image = 0;

	int i = 0;

	for (i = 0; i < _DEF_MAX_BLOBS * 4; i++) {
		Blob_results.num[i] = 0;
		Blob_results.area[i] = 0;
		Blob_results.gd[i] = 0;
		Blob_results.TopLeft[i].x = 0;
		Blob_results.TopLeft[i].y = 0;
		Blob_results.BottomRight[i].x = 0;
		Blob_results.BottomRight[i].y = 0;
	}

	m_dSFR_N_4 = 0.;
	m_dSFR_N_8 = 0.;
	m_dSFR_N_4_X = 0.;
	m_dSFR_N_8_X = 0.;

	m_hDLL_SFR = NULL;
	m_pSFRalgo = NULL;
}


CInspection_Alg::~CInspection_Alg(void)
{
	Release_SFR_DLL();
	FreeMemory();
}

int CInspection_Alg::AllocMemory(int iImageWidth, int iImageHeight)
{
	if (iImageWidth <= 0 || iImageHeight <= 0)
	{
		return -1;
	}

	FreeMemory();

	int iRet = m_blob.AllocMemory(iImageWidth, iImageHeight);
	if (iRet < 0)
	{
		return (-10 + iRet);
	}

	//! ���� �̹����� ũ��
	m_iWidth_Org_Image = iImageWidth;
	m_iHeight_Org_Image = iImageHeight;

	m_bw = cvCreateImage(cvSize(iImageWidth, iImageHeight), IPL_DEPTH_8U, 1);
	m_bw_t = cvCreateImage(cvSize(iImageWidth, iImageHeight), IPL_DEPTH_8U, 1);

	m_Morph_Img = cvCreateImage(cvSize(iImageWidth, iImageHeight), IPL_DEPTH_8U, 1);
	m_Sub_Img = cvCreateImage(cvSize(iImageWidth, iImageHeight), IPL_DEPTH_16S, 1);

	m_Scale_Img = cvCreateImage(cvSize(iImageWidth, iImageHeight), IPL_DEPTH_8U, 1);
	m_filtered_Img = cvCreateImage(cvSize(iImageWidth, iImageHeight), IPL_DEPTH_8S, 1);

	//! ������ �̹��� ������ ũ��
	m_iReal_Width_Image = iImageWidth;
	m_iReal_Height_Image = iImageHeight;

	if (m_bw == NULL || m_bw_t == NULL ||
		m_Morph_Img == NULL || m_Sub_Img == NULL ||
		m_Scale_Img == NULL || m_filtered_Img == NULL)
	{
		return -2;
	}

	return 1;
}

int CInspection_Alg::AllocMemory_Spot(int iImageWidth, int iImageHeight)
{
	if (iImageWidth <= 0 || iImageHeight <= 0)
	{
		return -1;
	}

	FreeMemory();

	int iRet = m_blob.AllocMemory(iImageWidth, iImageHeight);
	if (iRet < 0)
	{
		return (-10 + iRet);
	}

	//! ���� �̹����� ũ��
	m_iWidth_Org_Image = iImageWidth;
	m_iHeight_Org_Image = iImageHeight;

	m_bw = cvCreateImage(cvSize(iImageWidth, iImageHeight), IPL_DEPTH_8U, 1);
	m_bw_t = cvCreateImage(cvSize(iImageWidth, iImageHeight), IPL_DEPTH_8U, 1);

	m_Morph_Img = cvCreateImage(cvSize(iImageWidth, iImageHeight), IPL_DEPTH_8U, 1);
	m_Sub_Img = cvCreateImage(cvSize(iImageWidth, iImageHeight), IPL_DEPTH_16S, 1);

	//! ������ �̹��� ������ ũ��
	m_iReal_Width_Image = iImageWidth;
	m_iReal_Height_Image = iImageHeight;

	if (m_bw == NULL || m_bw_t == NULL || m_Morph_Img == NULL || m_Sub_Img == NULL)
	{
		return -2;
	}

	return 1;
}

int CInspection_Alg::AllocMemory_Scale(int iImageWidth, int iImageHeight, int iScale)
{
	if (iImageWidth <= 0 || iImageHeight <= 0 || iScale <= 0)
	{
		return -1;
	}

	// ���� Scale%�� ��� �Ǵ� Ȯ��
	int iTempWidth = iImageWidth  * iScale / 100;
	int iTempHeight = iImageHeight * iScale / 100;

	if (iTempWidth <= 0 || iTempHeight <= 0)
	{
		return -2;
	}

	FreeMemory();

	int iRet = m_blob.AllocMemory(iTempWidth, iTempHeight);
	if (iRet < 0)
	{
		return (-10 + iRet);
	}

	//! ���� �̹����� ũ��
	m_iWidth_Org_Image = iImageWidth;
	m_iHeight_Org_Image = iImageHeight;

	m_Scale_Img = cvCreateImage(cvSize(iTempWidth, iTempHeight), IPL_DEPTH_8U, 1);
	m_filtered_Img = cvCreateImage(cvSize(iTempWidth, iTempHeight), IPL_DEPTH_8S, 1);
	m_bw = cvCreateImage(cvSize(iTempWidth, iTempHeight), IPL_DEPTH_8U, 1);

	//! ������ �̹��� ������ ũ��
	m_iReal_Width_Image = iTempWidth;
	m_iReal_Height_Image = iTempHeight;

	if (m_bw == NULL || m_Scale_Img == NULL || m_filtered_Img == NULL)
	{
		return -2;
	}

	return 1;
}

void CInspection_Alg::FreeMemory()
{
	if (m_bw != NULL)
	{
		if (m_bw->roi != NULL)
		{
			cvResetImageROI(m_bw);
		}
		cvReleaseImage(&m_bw);
		m_bw = NULL;
	}
	if (m_bw_t != NULL)
	{
		if (m_bw_t->roi != NULL)
		{
			cvResetImageROI(m_bw_t);
		}
		cvReleaseImage(&m_bw_t);
		m_bw_t = NULL;
	}
	if (m_Morph_Img != NULL)
	{
		if (m_Morph_Img->roi != NULL)
		{
			cvResetImageROI(m_Morph_Img);
		}
		cvReleaseImage(&m_Morph_Img);
		m_Morph_Img = NULL;
	}
	if (m_Sub_Img != NULL)
	{
		if (m_Sub_Img->roi != NULL)
		{
			cvResetImageROI(m_Sub_Img);
		}
		cvReleaseImage(&m_Sub_Img);
		m_Sub_Img = NULL;
	}
	if (m_Scale_Img != NULL)
	{
		if (m_Scale_Img->roi != NULL)
		{
			cvResetImageROI(m_Scale_Img);
		}
		cvReleaseImage(&m_Scale_Img);
		m_Scale_Img = NULL;
	}
	if (m_filtered_Img != NULL)
	{
		if (m_filtered_Img->roi != NULL)
		{
			cvResetImageROI(m_filtered_Img);
		}
		cvReleaseImage(&m_filtered_Img);
		m_filtered_Img = NULL;
	}

	m_iReal_Width_Image = 0;
	m_iReal_Height_Image = 0;

	m_blob.FreeMemory();
}

//! Modify by LHW (2013/3/29)
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inspection spot function
// Input : IplImage* src_img ��� �Է� ���� (���� �̹��� ũ���� ����)
//         float m_Threshold : ��Ȱȭ �� �̹������� �ҷ��ĺ� ������ ������� ĳ�� �Ӱ谪. 0�̻��� float���� �Է�, 0~255���� �Է°���
//         int Defect_Small_Size, int Defect_Large_Size : �ҷ��ĺ��� ������ labeling�Ͽ� �� ���� ũ��(ȭ�Ҽ�)�� Defect_Small_Size~Defect_Large_Size�� ����.
//         float m_GD : �ҷ��ĺ��� ������ labeling�Ͽ� �� ���� ���ȭ�Ұ��� ����Ͽ� ȭ�ֺ��� ȭ�Ұ� ��հ��� ���̰� m_GD �̻��̸� ����.
//         int m_Boundary : m_Boundary�� ������ �����ڸ� �˻���� ������ �ȼ���
//		   int m_MaxDefectsCount : �ҷ� ��������.
// Output : ����� �ҷ���
//          
//          Ŭ������ �Ʒ� ����ü�� ��� ����
//			Blob_results.TopLeft[i] : ����� �ҷ��� MBR �»���ǥ
//			Blob_results.BottomRight[i] : ����� �ҷ��� MBR ������ǥ
//			Blob_results.num[i] : ����� �ҷ��� ��ȣ
//			Blob_results.area[i]  : ����� �ҷ��� ȭ�Ұ���
//			Blob_results.gd[i]  : ����� �ҷ��� ������ ���̰�
//
// By CDJung
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CInspection_Alg::Inspection_Spot(IplImage* Gray_Img,
	float m_Threshold,
	int Defect_Small_Size, int Defect_Large_Size,
	float m_GD,
	int m_Boundary,
	int m_MaxDefectsCount)
{
	//! �Է� �Ű� ���� Ȯ��
	if (Gray_Img == NULL)
	{
		return 0;
	}

	//! [���� ����] Boundary ���� '1'���� ������ �ȵȴ�.
	if (m_Boundary < 1)
	{
		m_Boundary = 1;
	}

	//! ���� ũ�� Ȯ��
	bool bFlag_Memory = false;
	int  iAllocWidth = m_iReal_Width_Image;
	int  iAllocHeight = m_iReal_Height_Image;

	if (m_bw == NULL || m_bw_t == NULL || m_Morph_Img == NULL || m_Sub_Img == NULL)
	{
		bFlag_Memory = true;
		iAllocWidth = Gray_Img->width;
		iAllocHeight = Gray_Img->height;
	}

	//if ( Gray_Img->width != m_iReal_Width_Image || Gray_Img->height != m_iReal_Height_Image )
	//{
	//	bFlag_Memory = true;
	//}

	if (Gray_Img->width > m_iReal_Width_Image)
	{
		bFlag_Memory = true;
		iAllocWidth = Gray_Img->width;
	}
	if (Gray_Img->height > m_iReal_Height_Image)
	{
		bFlag_Memory = true;
		iAllocHeight = Gray_Img->height;
	}

	if (bFlag_Memory == true)
	{
		int iRet = AllocMemory(iAllocWidth, iAllocHeight);
		if (iRet < 0)
		{
			return 0;
		}
	}

	//! ROI (Region of Interest) ����
	cvResetImageROI(Gray_Img);
	cvResetImageROI(m_Morph_Img);
	cvResetImageROI(m_Sub_Img);
	cvResetImageROI(m_bw_t);
	cvResetImageROI(m_bw);

	//! ROI (Region of Interest) ����
	//cvSetImageROI(Gray_Img,    cvRect(0, 0, Gray_Img->width, Gray_Img->height));
	//cvSetImageROI(m_Morph_Img, cvRect(0, 0, Gray_Img->width, Gray_Img->height));
	//cvSetImageROI(m_Sub_Img,   cvRect(0, 0, Gray_Img->width, Gray_Img->height));
	//cvSetImageROI(m_bw_t,      cvRect(0, 0, Gray_Img->width, Gray_Img->height));
	//cvSetImageROI(m_bw,        cvRect(0, 0, Gray_Img->width, Gray_Img->height));

	//! Filtering
	int morph_num = 3;

	cvDilate(Gray_Img, m_Morph_Img, NULL, morph_num);
	cvErode(m_Morph_Img, m_Morph_Img, NULL, morph_num * 2);

	cvSub(Gray_Img, m_Morph_Img, m_Sub_Img);
	cvAddS(m_Sub_Img, cvScalarAll(128), Gray_Img);

	//! ��ġȭ
	cvThreshold(Gray_Img, m_bw_t, 128 + m_Threshold, 255, CV_THRESH_BINARY);
	cvThreshold(Gray_Img, m_bw, 128 - m_Threshold, 255, CV_THRESH_BINARY_INV);

	cvAdd(m_bw, m_bw_t, m_bw);

	//! �ҷ� �ĺ����� BLOB �ؼ����� ���� �ҷ� ����
	int total_num = 0;
	total_num = Spot_BLOB_Analysis(m_bw, Gray_Img, Defect_Small_Size, Defect_Large_Size, m_GD, m_Boundary, m_MaxDefectsCount);

	Blob_results_Num = total_num;

	return total_num;
}

//! Added by LHW (2013/3/30)
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inspection edge function
// Input : IplImage* src_img ��� �Է� ���� (���� �̹��� ũ���� ����)
//         int Scale : ������ ũ��(%), 1~100�� �Է� ��)50�̸� ������ 50%�� �����
//         int Filter_S_Size,int Filter_L_Size : ���� ��Ȱȭ ���� ũ�� Filter_S_Size�� 1 �Ǵ� 3, Filter_L_Size�� Filter_S_Size���� ū Ȧ��(15,17,19,21 ��õ)
//         ū�̹� �˻�� 3,21    �����̹� �˻�� 1,15   1,17   1,19 ��õ
//         float m_Threshold : ��Ȱȭ �� �̹������� �ҷ��ĺ� ������ ������� ĳ�� �Ӱ谪. 0�̻��� float���� �Է�, 0~255���� �Է°���
//         int Defect_Small_Size, int Defect_Large_Size : �ҷ��ĺ��� ������ labeling�Ͽ� �� ���� ũ��(ȭ�Ҽ�)�� Defect_Small_Size~Defect_Large_Size�� ����.
//         float m_GD : �ҷ��ĺ��� ������ labeling�Ͽ� �� ���� ���ȭ�Ұ��� ����Ͽ� ȭ�ֺ��� ȭ�Ұ� ��հ��� ���̰� m_GD �̻��̸� ����.
//         int m_Boundary, int m_Edge : m_Boundary�� ������ �����ڸ� �˻���� ������ �ȼ���, m_Edge�� �𼭸� �˻���� ������ �ȼ���
//		   int m_MaxDefectsCount : �ҷ� ��������.
// Output : ����� �ҷ���
//          
//          Ŭ������ �Ʒ� ����ü�� ��� ����
//			Blob_results.TopLeft[i] : ����� �ҷ��� MBR �»���ǥ
//			Blob_results.BottomRight[i] : ����� �ҷ��� MBR ������ǥ
//			Blob_results.num[i] : ����� �ҷ��� ��ȣ
//			Blob_results.area[i]  : ����� �ҷ��� ȭ�Ұ���
//			Blob_results.gd[i]  : ����� �ҷ��� ������ ���̰�
//
// By CDJung
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CInspection_Alg::Inspection_edge(IplImage* Gray_Img,
	int Scale,
	int Filter_S_Size, int Filter_L_Size,
	float m_Threshold,
	int Defect_Small_Size, int Defect_Large_Size,
	float m_GD,
	int m_Boundary, int m_Edge,
	int m_MaxDefectsCount)
{
	return 1;

	//! �Է� �Ű� ���� Ȯ��
	if (Gray_Img == NULL)
	{
		return 0;
	}

	//! [���� ����] Boundary ���� '1'���� ������ �ȵȴ�.
	if (m_Boundary < 1)
	{
		m_Boundary = 1;
	}

	Filter_S_Size = 3;
	Filter_L_Size = 5;

	//! ���� ũ�� Ȯ��
	bool bFlag_Memory = false;
	int  iAllocWidth = m_iReal_Width_Image;
	int  iAllocHeight = m_iReal_Height_Image;

	// ���� Scale%�� ��� �Ǵ� Ȯ��
	int iTempWidth = Gray_Img->width  * Scale / 100;
	int iTempHeight = Gray_Img->height * Scale / 100;

	if (m_bw == NULL || m_Scale_Img == NULL || m_filtered_Img == NULL)
	{
		bFlag_Memory = true;
		iAllocWidth = iTempWidth;
		iAllocHeight = iTempHeight;
	}

	//if ( iTempWidth != m_iReal_Width_Image || iTempHeight != m_iReal_Height_Image )
	//{
	//	bFlag_Memory = true;
	//}

	if (iTempWidth > m_iReal_Width_Image)
	{
		bFlag_Memory = true;
		iAllocWidth = iTempWidth;
	}
	if (iTempHeight > m_iReal_Height_Image)
	{
		bFlag_Memory = true;
		iAllocHeight = iTempHeight;
	}

	if (bFlag_Memory == true)
	{
		int iRet = AllocMemory(iAllocWidth, iAllocHeight);
		if (iRet < 0)
		{
			return 0;
		}
	}

	//! ROI (Region of Interest) ����
	cvResetImageROI(Gray_Img);
	cvSetImageROI(m_Scale_Img, cvRect(0, 0, iTempWidth, iTempHeight));
	cvSetImageROI(m_filtered_Img, cvRect(0, 0, iTempWidth, iTempHeight));
	cvSetImageROI(m_bw, cvRect(0, 0, iTempWidth, iTempHeight));

	//! �Էµ� �̹����� ���
	cvResize(Gray_Img, m_Scale_Img, CV_INTER_NN);

	//! ���͸�	
	CvMat* mat_kernel = cvCreateMat(Filter_L_Size, Filter_L_Size, CV_32FC1);


	Make_Kernel(Filter_S_Size, Filter_L_Size, mat_kernel);


	cvFilter2D(m_Scale_Img, m_filtered_Img, mat_kernel, cvPoint(-1, -1));		// Filter_L_Size = 3, Filter_S_Size = 1���� �ٿ� �Ǵ� ��� �߻�.

																				//! ĳ�� ���� ����
	cvAddS(m_filtered_Img, cvScalarAll(128), m_Scale_Img);
	cvSmooth(m_Scale_Img, m_Scale_Img, CV_GAUSSIAN, 9, 9, 1, 1);
	cvCanny(m_Scale_Img, m_Scale_Img, m_Threshold, m_Threshold * 3, 3);

	//! ���Ἲ Ȯ��
	cvSetZero(m_bw);
	cvAdd(m_Scale_Img, m_bw, m_bw);
	cvDilate(m_bw, m_bw, NULL, 5);
	cvErode(m_bw, m_bw, NULL, 5);

	//! �����ڸ� ���� �˻� ����
	Edge_Except(m_bw, Scale, m_Edge);

	//! �ҷ� �ĺ����� BLOB �ؼ����� ���� �ҷ� ����
	int total_num = 0;
	total_num = BLOB_Analysis(m_bw, m_Scale_Img, Defect_Small_Size, Defect_Large_Size, Scale, m_GD, m_MaxDefectsCount);

	//! �޸� ����	
	cvReleaseMat(&mat_kernel);

	Blob_results_Num = total_num;

	//! ROI (Region of Interest) ����
	cvResetImageROI(m_Scale_Img);
	cvResetImageROI(m_filtered_Img);
	cvResetImageROI(m_bw);

	return total_num;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inspection scale function
// Input : IplImage* src_img �÷� �Է� ���� (���� �̹��� ũ���� ����)
//         int Scale : ������ ũ��(%), 1~100�� �Է� ��)20�̸� ������ 20% (1/5) �� ����� 
//         int Filter_S_Size,int Filter_L_Size : ���� ��Ȱȭ ���� ũ�� Filter_S_Size�� 1 �Ǵ� 3, Filter_L_Size�� Filter_S_Size���� ū Ȧ��(15,17,19,21 ��õ)
//         ū�̹� �˻�� 3,21    �����̹� �˻�� 1,15   1,17   1,19 ��õ
//         float m_Threshold : ��Ȱȭ �� �̹������� �ҷ��ĺ� ������ ������� �Ӱ谪. 0�̻��� float���� �Է�, 0~255���� �Է°���
//         int Defect_Small_Size, int Defect_Large_Size : �ҷ��ĺ��� ������ labeling�Ͽ� �� ���� ũ��(ȭ�Ҽ�)�� Defect_Small_Size~Defect_Large_Size�� ����.
//         float m_GD : �ҷ��ĺ��� ������ labeling�Ͽ� �� ���� ���ȭ�Ұ��� ����Ͽ� ȭ�ֺ��� ȭ�Ұ� ��հ��� ���̰� m_GD �̻��̸� ����.
//         int m_Boundary, int m_Edge : m_Boundary�� ������ �����ڸ� �˻���� ������ �ȼ���, m_Edge�� �𼭸� �˻���� ������ �ȼ���
// Output : ����� �ҷ���
//          
//          Ŭ������ �Ʒ� ����ü�� ��� ����
//			Blob_results.TopLeft[i] : ����� �ҷ��� MBR �»���ǥ
//			Blob_results.BottomRight[i] : ����� �ҷ��� MBR ������ǥ
//			Blob_results.num[i] : ����� �ҷ��� ��ȣ
//			Blob_results.area[i]  : ����� �ҷ��� ȭ�Ұ���
//			Blob_results.gd[i]  : ����� �ҷ��� ������ ���̰�
//
// By CDJung
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CInspection_Alg::Inspection_Scale(IplImage* Gray_Img,
	int Scale,
	int Filter_S_Size, int Filter_L_Size,
	float m_Threshold,
	int Defect_Small_Size, int Defect_Large_Size,
	float m_GD,
	int m_Boundary, int m_Edge,
	int m_MaxDefectsCount)
{
	//! �Է� �Ű� ���� Ȯ��
	if (Gray_Img == NULL)
	{
		return 0;
	}

	//! ���� ũ�� Ȯ��
	bool bFlag_Memory = false;
	int  iAllocWidth = m_iReal_Width_Image;
	int  iAllocHeight = m_iReal_Height_Image;

	// ���� Scale%�� ��� �Ǵ� Ȯ��
	int iTempWidth = Gray_Img->width  * Scale / 100;
	int iTempHeight = Gray_Img->height * Scale / 100;

	if (m_bw == NULL || m_Scale_Img == NULL || m_filtered_Img == NULL)
	{
		bFlag_Memory = true;
		iAllocWidth = iTempWidth;
		iAllocHeight = iTempHeight;
	}

	//if ( iTempWidth != m_iReal_Width_Image || iTempHeight != m_iReal_Height_Image )
	//{
	//	bFlag_Memory = true;
	//}

	if (iTempWidth > m_iReal_Width_Image)
	{
		bFlag_Memory = true;
		iAllocWidth = iTempWidth;
	}
	if (iTempHeight > m_iReal_Height_Image)
	{
		bFlag_Memory = true;
		iAllocHeight = iTempHeight;
	}

	if (bFlag_Memory == true)
	{
		int iRet = AllocMemory(iAllocWidth, iAllocHeight);
		if (iRet < 0)
		{
			return 0;
		}
	}

	//! ROI (Region of Interest) ����
	cvResetImageROI(Gray_Img);
	cvSetImageROI(m_Scale_Img, cvRect(0, 0, iTempWidth, iTempHeight));
	cvSetImageROI(m_filtered_Img, cvRect(0, 0, iTempWidth, iTempHeight));
	cvSetImageROI(m_bw, cvRect(0, 0, iTempWidth, iTempHeight));


	//! �Էµ� �̹����� ���
	cvResize(Gray_Img, m_Scale_Img, CV_INTER_NN);

	//! Filtering
	if (Scale <= 80 && Scale >= 20)
	{
		cvSmooth(m_Scale_Img, m_Scale_Img, CV_GAUSSIAN, 3, 3, 1, 1);
	}

	//! ���͸�	
	CvMat* mat_kernel = cvCreateMat(Filter_L_Size, Filter_L_Size, CV_32FC1);
	Make_Kernel(Filter_S_Size, Filter_L_Size, mat_kernel);
	cvFilter2D(m_Scale_Img, m_filtered_Img, mat_kernel, cvPoint(-1, -1));

	//! �Ӱ�ȭ
	cvSetZero(m_bw);
	Tri_Threshold(m_filtered_Img, m_Threshold, m_bw, Scale, m_Boundary);


	/*
	cvShowImage("Kernel", mat_kernel);
	cvShowImage("Scale_Img", m_Scale_Img);
	cvShowImage("filtered_Img", m_filtered_Img);
	cvShowImage("Threshold", m_bw);

	*/

	//! �����ڸ� ���� �˻� ����
	Edge_Except(m_bw, Scale, m_Edge);

	//! �ҷ� �ĺ����� BLOB �ؼ����� ���� �ҷ� ����
	int total_num = 0;
	total_num = BLOB_Analysis(m_bw, m_Scale_Img, Defect_Small_Size, Defect_Large_Size, Scale, m_GD, m_MaxDefectsCount);

	//! �޸� ����	
	cvReleaseMat(&mat_kernel);

	Blob_results_Num = total_num;

	//! ROI (Region of Interest) ����
	cvResetImageROI(m_Scale_Img);
	cvResetImageROI(m_filtered_Img);
	cvResetImageROI(m_bw);

	return total_num;
}

//! Filter�� �����. 
void CInspection_Alg::Make_Kernel(int small_Kernel, int large_Kernel, CvMat* mat_kernel)
{
	cvZero(mat_kernel);

	for (int i = 0; i<large_Kernel; i++)
	{
		for (int j = 0; j<large_Kernel; j++)
		{
			if (abs((large_Kernel - 1) / 2 - i) <= (small_Kernel - 1) / 2 &&
				abs((large_Kernel - 1) / 2 - j) <= (small_Kernel - 1) / 2)
			{
				cvmSet(mat_kernel, i, j, 10.0f / (float)(small_Kernel*small_Kernel));
			}
			else
			{
				cvmSet(mat_kernel, i, j, -10.0f / (float)(large_Kernel*large_Kernel - small_Kernel*small_Kernel));
			}
		}
	}
}

//! Filtering�� �̹������� �̹��� �ִ� ������ ����Ǵ� pixel���� ã�Ƴ���. 
void CInspection_Alg::Tri_Threshold(IplImage* src, float m_threshold, IplImage* bw, int scale, int boundary)
{
	int m_boundary = (int)((float)boundary*(float)scale / 100);
	if (m_boundary == 0)
	{
		m_boundary = 1;
	}

	int iWidth = 0;
	int iHeight = 0;
	if (src->roi != NULL)
	{
		iWidth = src->roi->width - m_boundary;
		iHeight = src->roi->height - m_boundary;
	}
	else
	{
		iWidth = src->width - m_boundary;
		iHeight = src->height - m_boundary;
	}

	for (int i = m_boundary; i<iHeight; i++)
	{
		for (int j = m_boundary; j<iWidth; j++)
		{
			if (src->imageData[i*src->widthStep + j] <= -m_threshold * 10)
			{
				bw->imageData[i*src->widthStep + j] = (unsigned char)255;	//! [���� ����] '255'�� �״�� �μ���. �������� ���ʽÿ�. by LHW
			}
		}
	}
}

//! �̹����� 4���� �𼭸��� �����Ѵ�.
void CInspection_Alg::Edge_Except(IplImage* bw, int scale, int margin)
{
	int m_margin = (int)((float)margin*(float)scale / 100);
	if (m_margin == 0)
	{
		m_margin = 1;
	}

	int iWidth = 0;
	int iHeight = 0;
	if (bw->roi != NULL)
	{
		iWidth = bw->roi->width;
		iHeight = bw->roi->height;
	}
	else
	{
		iWidth = bw->width;
		iHeight = bw->height;
	}

	for (int i = 0; i<m_margin; i++)
	{
		for (int j = 0; j<m_margin; j++)
		{
			if (sqrtf((float)i*(float)i + (float)j*(float)j)<m_margin)
			{
				bw->imageData[i*bw->widthStep + j] = 0;
			}
		}
	}
	for (int i = 0; i<m_margin; i++)
	{
		for (int j = iWidth - m_margin; j<iWidth; j++)
		{
			if (sqrtf((float)i*(float)i + (float)(j - iWidth)*(float)(j - iWidth))<m_margin)
			{
				bw->imageData[i*bw->widthStep + j] = 0;
			}
		}
	}
	for (int i = iHeight - m_margin; i<iHeight; i++)
	{
		for (int j = 0; j<m_margin; j++)
		{
			if (sqrtf((float)(i - iHeight)*(float)(i - iHeight) + (float)j*(float)j)<m_margin)
			{
				bw->imageData[i*bw->widthStep + j] = 0;
			}
		}
	}
	for (int i = iHeight - m_margin; i<iHeight; i++)
	{
		for (int j = iWidth - m_margin; j<iWidth; j++)
		{
			if (sqrtf((float)(i - iHeight)*(float)(i - iHeight) + (float)(j - iWidth)*(float)(j - iWidth))<m_margin)
			{
				bw->imageData[i*bw->widthStep + j] = 0;
			}
		}
	}
}

int CInspection_Alg::BLOB_Analysis(IplImage* bw, IplImage* gray, int SMALL_SIZE, int LARGE_SIZE, int scale, float gray_threshold, int m_MaxDefectsCount)
{
	m_blob.SetParam(bw, gray, SMALL_SIZE, LARGE_SIZE, gray_threshold, m_MaxDefectsCount);	// ���̺� �� �̹����� �ּ� �ȼ��� ���� ����
	m_blob.DoLabeling();

	CvPoint	pt1;
	CvPoint	pt2;


	// �� �ĺ����� �˻�
	if (m_blob.m_nBlobs > 0) {
		for (int i = 0; i < m_blob.m_nBlobs; i++)
		{
			pt1 = cvPoint((m_blob.m_recBlobs[i].x) * 100 / scale,
				(m_blob.m_recBlobs[i].y) * 100 / scale);
			pt2 = cvPoint(pt1.x + m_blob.m_recBlobs[i].width * 100 / scale,
				pt1.y + m_blob.m_recBlobs[i].height * 100 / scale);

			Blob_results.TopLeft[i] = pt1;
			Blob_results.BottomRight[i] = pt2;
			Blob_results.num[i] = i;
			Blob_results.area[i] = m_blob.m_Area[i];
			Blob_results.gd[i] = m_blob.m_GD[i];
		}
	}

	return m_blob.m_nBlobs;
}


int CInspection_Alg::Spot_BLOB_Analysis(IplImage* bw, IplImage* gray, int SMALL_SIZE, int LARGE_SIZE, float gray_threshold, int m_Boundary, int m_MaxDefectsCount)
{
	for (int i = 0; i<m_Boundary; i++)
	{
		for (int j = 0; j<bw->width; j++)
		{
			bw->imageData[i*bw->widthStep + j] = 0;
		}
	}
	for (int i = bw->height - m_Boundary; i<bw->height; i++)
	{
		for (int j = 0; j<bw->width; j++)
		{
			bw->imageData[i*bw->widthStep + j] = 0;
		}
	}
	for (int i = 0; i<bw->height; i++)
	{
		for (int j = 0; j<m_Boundary; j++)
		{
			bw->imageData[i*bw->widthStep + j] = 0;
		}
	}
	for (int i = 0; i<bw->height; i++)
	{
		for (int j = bw->width - m_Boundary; j<bw->width; j++)
		{
			bw->imageData[i*bw->widthStep + j] = 0;
		}
	}

	m_blob.SetParam(bw, gray, SMALL_SIZE, LARGE_SIZE, gray_threshold, m_MaxDefectsCount);	// ���̺� �� �̹����� �ּ� �ȼ��� ���� ����
	m_blob.DoLabeling();

	CvPoint	pt1;
	CvPoint	pt2;

	// �� �ĺ����� �˻�
	if (m_blob.m_nBlobs > 0) {
		for (int i = 0; i < m_blob.m_nBlobs; i++)
		{
			pt1 = cvPoint(m_blob.m_recBlobs[i].x, m_blob.m_recBlobs[i].y);
			pt2 = cvPoint(pt1.x + m_blob.m_recBlobs[i].width, pt1.y + m_blob.m_recBlobs[i].height);

			Blob_results.TopLeft[i] = pt1;
			Blob_results.BottomRight[i] = pt2;
			Blob_results.num[i] = i;
			Blob_results.area[i] = m_blob.m_Area[i];
			Blob_results.gd[i] = m_blob.m_GD[i];
		}
	}

	return m_blob.m_nBlobs;
}




bool CInspection_Alg::getOpticalAxis(IplImage* src_img, int fi_scale, CRect roi, double &fo_dOffsetX, double &fo_dOffsetY, int &iAvr)
{
	int width = src_img->width;
	int height = src_img->height;

	fo_dOffsetX = width * 0.5;
	fo_dOffsetY = height * 0.5;



	int sx = roi.left;
	int ex = roi.right;
	int sy = roi.top;
	int ey = roi.bottom;

	if (sx<0)			sx = 0;
	if (sy<0)			sy = 0;
	if (ex>width)		ex = width - 1;
	if (ey>height)		ey = height - 1;

	if ((ex <= sx) || (ey <= sy))
	{
		sLogMessage.Format(_T("getOpticalAxis �˻� ���� Error (%d, %d, %d, %d)"), roi.left, roi.top, roi.right, roi.bottom);
		return false;
	}

	int* pHist_x;
	int* pHist_y;

	pHist_x = new int[width];
	memset(pHist_x, 0, sizeof(int)*width);
	pHist_y = new int[height];
	memset(pHist_y, 0, sizeof(int)*height);


	IplImage* Gray_Img;

	Gray_Img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
	cvCvtColor(src_img, Gray_Img, CV_BGR2GRAY);

	int x, y, pos, sum, curGap;

	curGap = fi_scale * width;

	for (x = sx; x<ex; x += fi_scale)
	{
		sum = 0;
		pos = sy*width + x;
		for (y = sy; y<ey; y += fi_scale, pos += curGap)
		{
			sum += Gray_Img->imageData[pos];
		}

		pHist_x[x] = sum;
	}

	int cnt = 0;
	int TotalSum = 0;
	curGap = fi_scale;
	for (y = sy; y<ey; y += fi_scale)
	{
		sum = 0;
		pos = y*width + sx;
		for (x = sx; x<ex; x += fi_scale, pos += curGap)
		{
			cnt++;
			sum += Gray_Img->imageData[pos];
		}

		pHist_y[y] = sum;
		TotalSum += sum;
	}

	iAvr = (int)(TotalSum / cnt);
	cvReleaseImage(&Gray_Img);

	double Ax, Bx, Cx;
	double Ay, By, Cy;

	if (!calcPeakPos(width, pHist_x, Ax, Bx, Cx))
	{
		sLogMessage = "calcPeakPos ���� ���� [X].";
	}
	else if (!calcPeakPos(height, pHist_y, Ay, By, Cy))
	{
		sLogMessage = "calcPeakPos ���� ���� [Y].";
	}
	else
	{
		fo_dOffsetX = -Bx / 2 / Ax;
		fo_dOffsetY = -By / 2 / Ay;
	}

	delete[] pHist_x;
	pHist_x = NULL;

	delete[] pHist_y;
	pHist_y = NULL;

	return true;
}

//! Added by LHW (2013/3/13)
//! �ش� Color �̹������� �� ���� ��հ��� �˾Ƴ���. 
bool CInspection_Alg::GetAverageValue(IplImage* src_img, CRect fi_RectRoi, int fi_iSkipX, int fi_iSkipY, double &fo_dAvgRed, double &fo_dAvgGreen, double &fo_dAvgBlue)
{
	int iSizeX = fi_RectRoi.right - fi_RectRoi.left;
	int iSizeY = fi_RectRoi.bottom - fi_RectRoi.top;

	if (iSizeX <= 0 || iSizeY <= 0)
	{
		return false;
	}

	if (fi_RectRoi.left<0 || fi_RectRoi.top<0)
	{
		sLogMessage.Format(_T("�˻� ������ �������Դϴ�. L:%d, T:%d"), fi_RectRoi.left, fi_RectRoi.top);
		return false;
	}

	if (fi_iSkipX <= 0)
	{
		fi_iSkipX = 1;
	}
	if (fi_iSkipX >= (fi_RectRoi.Width() / 2))
	{
		fi_iSkipX = fi_RectRoi.Width() / 2;
	}
	if (fi_iSkipY <= 0)
	{
		fi_iSkipY = 1;
	}
	if (fi_iSkipY >= (fi_RectRoi.Height() / 2))
	{
		fi_iSkipY = fi_RectRoi.Height() / 2;
	}

	CvScalar v_pixel;

	double dSumRed = 0., dSumGreen = 0., dSumBlue = 0.;
	int iCnt = 0;

	for (int y = 0; y<iSizeY; y += fi_iSkipY)
	{
		for (int x = 0; x<iSizeX; x += fi_iSkipX)
		{
			v_pixel = cvGet2D(src_img, fi_RectRoi.top + y, fi_RectRoi.left + x);

			//	openCV������ BGR ������ Color �迭..?
			dSumRed += v_pixel.val[0];
			dSumGreen += v_pixel.val[1];
			dSumBlue += v_pixel.val[2];

			iCnt++;
		}
	}

	if (iCnt <= 0)
	{
		return false;
	}

	fo_dAvgRed = dSumRed / ((double)iCnt);
	fo_dAvgGreen = dSumGreen / ((double)iCnt);
	fo_dAvgBlue = dSumBlue / ((double)iCnt);

	return true;
}

//! SFR ���� ��ü �Լ�
//! [�Է°�] 
//!		src_img : ���� �̹��� ��ü
//!		fi_iWidth, fi_iHight : ���� �̹����� ũ��
//!		fi_fCcdSize : CCD Sensor�� ũ��, ������� ����
//!		fi_RectRoi : SFR ROI �簢 ������ ��ġ, ũ��
//! [��ȯ��]
//!     iDataCnt : SFR ���� ����� ����
//!		fo_DataX, fo_DataY : SFR ���� ��� ������
bool CInspection_Alg::getSFR(IplImage* src_img, int fi_iWidth, int fi_iHight, float fi_fCcdSize, CRect fi_RectRoi, int& iDataCnt, double fo_DataX[MAX_SFR_DATA_CNT], double fo_DataY[MAX_SFR_DATA_CNT], double dGain)
{
	int    *m_Image_Buffer_Int = NULL;
	double *m_Image_Buffer_Double = NULL;
	double *m_MTF_Buffer;
	double *m_LPMM_Buffer;

	int iSizeX = fi_RectRoi.right - fi_RectRoi.left;
	int iSizeY = fi_RectRoi.bottom - fi_RectRoi.top;

	//! 2 : ���� ���, 100 : LG �̳����� DLL ���
	int iFlag_X = 100;

	if (m_pSFRalgo == NULL)
	{
		//! DLL Library Loading�� �������� ���� ���� ��� ���
		iFlag_X = 2;
	}

	if (iFlag_X == 100)
	{
		//! LG �̳��� SFR DLL������ ROI�� ũ��� Ȧ���̾�� �Ѵ�. 
		if ((iSizeX % 2) == 0)
			iSizeX--;

		if ((iSizeY % 2) == 0)
			iSizeY--;
	}
	else
	{
		//! SFR ROI�� ũ��� ¦���̾�� �Ѵ�.
		if ((iSizeX % 2) != 0)
			iSizeX--;

		if ((iSizeY % 2) != 0)
			iSizeY--;
	}

	if (iSizeX<10 || iSizeY<10)
	{
		sLogMessage.Format(_T("�˻� ������ ũ�Ⱑ �������Դϴ�. X:%d, Y:%d"), iSizeX, iSizeY);
		return false;
	}

	if (fi_RectRoi.left<0 || fi_RectRoi.top<0)
	{
		sLogMessage.Format(_T("�˻� ������ �������Դϴ�. L:%d, T:%d"), fi_RectRoi.left, fi_RectRoi.top);
		return false;
	}

	CvScalar v_pixel;

	iDataCnt = 0;
	m_dSFR_N_4 = 0.;
	m_dSFR_N_8 = 0.;
	m_dSFR_N_4_X = 0.;
	m_dSFR_N_8_X = 0.;

	// SFR �̹��� ���� ���� �� �ʱ�ȭ
	if (iFlag_X == 100)
	{
		m_Image_Buffer_Int = new int[iSizeX*iSizeY];
		memset(m_Image_Buffer_Int, 0, sizeof(int)*iSizeX*iSizeY);
	}
	else
	{
		m_Image_Buffer_Double = new double[iSizeX*iSizeY];
		memset(m_Image_Buffer_Double, 0, sizeof(double)*iSizeX*iSizeY);
	}

	// Lp/mm���� �� m_pFreqBuffer ���� �� �ʱ�ȭ
	// MTF ���� �� m_pSfrBuffer �̹��� ���� ���� �� �ʱ�ȭ
	if (iSizeX < iSizeY) // �������� SFR�� ����� �̹����� ���� < ����
	{
		m_MTF_Buffer = new double[(int)(iSizeX)];
		memset(m_MTF_Buffer, 0, sizeof(double)*(int)(iSizeX));

		m_LPMM_Buffer = new double[(int)(iSizeX)];
		memset(m_LPMM_Buffer, 0, sizeof(double)*(int)(iSizeX));
	}
	else // ���� >= ���� �ϰ��� �˰��򿡼� ���η� �ٲٹǷ� �Ʒ��� ���� ����
	{
		m_MTF_Buffer = new double[(int)(iSizeY)];
		memset(m_MTF_Buffer, 0, sizeof(double)*(int)(iSizeY));

		m_LPMM_Buffer = new double[(int)(iSizeY)];
		memset(m_LPMM_Buffer, 0, sizeof(double)*(int)(iSizeY));
	}

	bool bFlag_Pre_Process = false;
	bool bFlag_Noise_Reduction = false;
	bool bFlag_Sharpening_Filter = false;
	bool bFlag_Gamma = false;

	//! 'm_Image_Buffer' �Է� ���۸� �غ��Ѵ�.
	//! ��ó���� �� ���� �ִ�.
	if (bFlag_Pre_Process == true)
	{
		IplImage* Img = cvCreateImage(cvSize(iSizeX, iSizeY), IPL_DEPTH_8U, 3);

		for (int j = 0; j<iSizeY; j++) {
			for (int i = 0; i<iSizeX; i++) {
				v_pixel = cvGet2D(src_img, fi_RectRoi.top + j, fi_RectRoi.left + i);
				cvSet2D(Img, j, i, v_pixel);
			}
		}

		//cvSaveImage(_T("E:\\Test\\SFR.bmp"), Img);	

		if (bFlag_Pre_Process == true)
		{
			// Noise Reduction ��ó�� �ǽ�.
			cvSmooth(Img, Img, CV_BLUR, 3, 3);
		}

		if (bFlag_Sharpening_Filter == true)
		{
			// Sharpening ��ó�� �ǽ�.
			IplImage *red = cvCreateImage(cvGetSize(Img), IPL_DEPTH_8U, 1);
			IplImage *green = cvCreateImage(cvGetSize(Img), IPL_DEPTH_8U, 1);
			IplImage *blue = cvCreateImage(cvGetSize(Img), IPL_DEPTH_8U, 1);
			cvCvtPixToPlane(Img, blue, green, red, NULL);

			//! Sharpening
			doGraySharpening(blue);
			doGraySharpening(green);
			doGraySharpening(red);

			//! Sharpening ��ó�� �ǽ�
			cvCvtPlaneToPix(blue, green, red, NULL, Img);
			cvReleaseImage(&red);
			cvReleaseImage(&green);
			cvReleaseImage(&blue);
		}

		//cvSaveImage(_T("E:\\Test\\SFR2.bmp"), Img);

		if (bFlag_Gamma == true)
		{
			// SFR �˰����� �ֵ���(Luminance)���� ���, Gamma ���� ���� �ǽ�.
			// Luminance = 0.299 R + 0.587 G + 0.114 B
			double gamma = 0.5;

			if (iFlag_X == 100)
			{
				for (int m = 0; m<iSizeY; m++) {
					for (int n = 0; n<iSizeX; n++) {
						v_pixel = cvGet2D(Img, m, n);
						m_Image_Buffer_Int[m*iSizeX + n] = (int)(255 * pow((0.114*(double)v_pixel.val[0] + 0.587*(double)v_pixel.val[1] + 0.299*(double)v_pixel.val[2]) / 255.0, gamma));
					}
				}
			}
			else
			{
				for (int m = 0; m<iSizeY; m++) {
					for (int n = 0; n<iSizeX; n++) {
						v_pixel = cvGet2D(Img, m, n);
						m_Image_Buffer_Double[m*iSizeX + n] = 255 * pow((0.114*(double)v_pixel.val[0] + 0.587*(double)v_pixel.val[1] + 0.299*(double)v_pixel.val[2]) / 255.0, gamma);
					}
				}
			}

		}
		else
		{
			// SFR �˰����� �ֵ���(Luminance)���� ���
			// Luminance = 0.299 R + 0.587 G + 0.114 B

			if (iFlag_X == 100)
			{
				for (int m = 0; m<iSizeY; m++) {
					for (int n = 0; n<iSizeX; n++) {
						v_pixel = cvGet2D(Img, m, n);
						m_Image_Buffer_Int[m*iSizeX + n] = (int)(0.114*(double)v_pixel.val[0] + 0.587*(double)v_pixel.val[1] + 0.299*(double)v_pixel.val[2]);
					}
				}
			}
			else
			{
				for (int m = 0; m<iSizeY; m++) {
					for (int n = 0; n<iSizeX; n++) {
						v_pixel = cvGet2D(Img, m, n);
						m_Image_Buffer_Double[m*iSizeX + n] = 0.114*(double)v_pixel.val[0] + 0.587*(double)v_pixel.val[1] + 0.299*(double)v_pixel.val[2];
					}
				}
			}

		}

		cvReleaseImage(&Img);
	}
	else
	{
		// SFR �˰����� �ֵ���(Luminance)���� ���
		// Luminance = 0.299 R + 0.587 G + 0.114 B

		if (iFlag_X == 100)
		{
			for (int m = 0; m<iSizeY; m++) {
				for (int n = 0; n<iSizeX; n++) {
					v_pixel = cvGet2D(src_img, fi_RectRoi.top + m, fi_RectRoi.left + n);
					m_Image_Buffer_Int[m*iSizeX + n] = (int)(0.114*(double)v_pixel.val[0] + 0.587*(double)v_pixel.val[1] + 0.299*(double)v_pixel.val[2]);
				}
			}
		}
		else
		{
			for (int m = 0; m<iSizeY; m++) {
				for (int n = 0; n<iSizeX; n++) {
					v_pixel = cvGet2D(src_img, fi_RectRoi.top + m, fi_RectRoi.left + n);
					m_Image_Buffer_Double[m*iSizeX + n] = 0.114*(double)v_pixel.val[0] + 0.587*(double)v_pixel.val[1] + 0.299*(double)v_pixel.val[2];
				}
			}
		}
	}

	int length = 0;

	if (iFlag_X == 100 && m_pSFRalgo != NULL)
	{
		m_dSFR_N_4_X = SFR_N_4_POSITION_X;
		m_dSFR_N_8_X = SFR_N_8_POSITION_X;


		//! [���� ����] 
		//!		1) �Է� ���۴� �ݵ�� int�� �����̾�� �Ѵ�. 
		//!		2) ��� Y �������� ������ '%'�̹Ƿ�, 0 ~ 100�� ������ ������. 
		m_pSFRalgo(m_Image_Buffer_Int, iSizeX, iSizeY, 0, fo_DataY, fo_DataX, SFR_N_4_POSITION_X, &m_dSFR_N_4, SFR_N_8_POSITION_X, &m_dSFR_N_8, 0);
		//pSFRalgo((int*)pImage_Buffer, iSizeX, iSizeY, 0, fo_DataY, fo_DataX, SFR_N_4_PositionX, &m_dSFR_N_4, SFR_N_8_PositionX, &m_dSFR_N_8, 0);

		length = iSizeX - 1;

		//! ��� Y �������� ������ '0 ~ 1'������ �Ҽ��� ���ڷ� �����Ѵ�. 
		//! SFR ��� �����͸� �̿��� ó�� ������ ���߱� ���ؼ��̴�.

		for (int i = 0; i < length; i++)
		{
			if (i >= MAX_SFR_DATA_CNT)
			{
				break;
			}

			fo_DataY[i] /= 100.;
		}

		m_dSFR_N_4 /= 100.;
		m_dSFR_N_8 /= 100.;

		//FreeLibrary(hDll);
	}
	else
	{
		// SFR �˰��� ����
		// �������� 1. m_LPMM_Buffer: X�� ������� �� ���� (��� ����)
		//         2. m_MTF_Buffer: MTF���� �� ���� (��� ����)
		//         3. m_Image_Buffer: Luminance �̹��� ���� (�Է� ����)
		//         4. m_Image_Width: �̹��� ���α���
		//		   5. m_Image_Height: �̹��� ���α���

		int iRet = SFR.SFR_Processing(m_LPMM_Buffer, m_MTF_Buffer, m_Image_Buffer_Double, iSizeX, iSizeY, fi_fCcdSize, iFlag_X, dGain);
		if (iRet == 1)
		{
			// �޸� ����
			if (m_Image_Buffer_Double != NULL)
			{
				delete[] m_Image_Buffer_Double;
			}
			if (m_MTF_Buffer != NULL)
			{
				delete[] m_MTF_Buffer;
			}
			if (m_LPMM_Buffer != NULL)
			{
				delete[] m_LPMM_Buffer;
			}

			m_Image_Buffer_Double = NULL;
			m_MTF_Buffer = NULL;
			m_LPMM_Buffer = NULL;

			return false;
		}

		switch (iFlag_X)
		{
		case 0:
		{
			length = Linear_Interpolation(m_LPMM_Buffer, m_MTF_Buffer, fo_DataX, fo_DataY);
		}
		break;
		case 1:
		{
			length = iSizeX - 1;
		}
		break;
		case 2:
		{
			length = iSizeX / 2 - 1;
		}
		break;
		}//! switch(iFlag_X)
	}

	iDataCnt = length;

	if (iDataCnt > MAX_SFR_DATA_CNT)
	{
		TRACE("SFR ��� Data ���� �ʰ� %d\n\n", iDataCnt);
	}

	switch (iFlag_X)
	{
	case 0:
	{
		int index = (int)(iDataCnt / 4 + 0.5);
		m_dSFR_N_4 = fo_DataY[index];
		m_dSFR_N_4_X = (double)(iDataCnt / 4);

		index = (int)(iDataCnt / 8 + 0.5);
		m_dSFR_N_8 = fo_DataY[index];
		m_dSFR_N_8_X = (double)(iDataCnt / 8);
	}
	break;
	case 1:
	case 2:
	{
		if (length > 0)
		{
			double start = 0;
			double end = 0;

			int iFlag_Cnt = 0;

			double dSFR_N_4 = SFR_N_4_POSITION_X;
			double dSFR_N_8 = SFR_N_8_POSITION_X;

			m_dSFR_N_4_X = SFR_N_4_POSITION_X;
			m_dSFR_N_8_X = SFR_N_8_POSITION_X;

			for (int i = 0; i < (length - 1); i++)
			{
				if (i >= (MAX_SFR_DATA_CNT - 2))
				{
					//! Output ������ ������ ũ�⸦ �Ѵ��� Ȯ��
					break;
				}

				start = m_LPMM_Buffer[i];
				end = m_LPMM_Buffer[i + 1];

				//! SFR N/8���� ã�Ƴ���.
				if ((start <= dSFR_N_8 && end >= dSFR_N_8) && (iFlag_Cnt == 0))
				{
					if (fabs(start - dSFR_N_8) < 0.01)
					{
						m_dSFR_N_8 = m_MTF_Buffer[i];
						iFlag_Cnt++;
					}
					else if (fabs(end - dSFR_N_8) < 0.01)
					{
						m_dSFR_N_8 = m_MTF_Buffer[i];
						iFlag_Cnt++;
					}
					else
					{
						m_dSFR_N_8 = ((m_MTF_Buffer[i + 1] - m_MTF_Buffer[i]) / (m_LPMM_Buffer[i + 1] - m_LPMM_Buffer[i]))*(dSFR_N_8 - m_LPMM_Buffer[i]) + m_MTF_Buffer[i];
						iFlag_Cnt++;
					}
				}

				//! SFR N/4���� ã�Ƴ���.
				if ((start <= dSFR_N_4 && end >= dSFR_N_4) && (iFlag_Cnt == 1))
				{
					if (fabs(start - dSFR_N_4) < 0.01)
					{
						m_dSFR_N_4 = m_MTF_Buffer[i];
						iFlag_Cnt++;
					}
					else if (fabs(end - dSFR_N_4) < 0.01)
					{
						m_dSFR_N_4 = m_MTF_Buffer[i];
						iFlag_Cnt++;
					}
					else
					{
						m_dSFR_N_4 = ((m_MTF_Buffer[i + 1] - m_MTF_Buffer[i]) / (m_LPMM_Buffer[i + 1] - m_LPMM_Buffer[i]))*(dSFR_N_4 - m_LPMM_Buffer[i]) + m_MTF_Buffer[i];
						iFlag_Cnt++;
					}
				}

				//! <--------------------------------------------
				//! ��� �����͸� Output ���ۿ� �Է��Ѵ�.
				fo_DataX[i] = m_LPMM_Buffer[i];
				fo_DataY[i] = m_MTF_Buffer[i];

				if (i == (length - 2))
				{
					fo_DataX[i + 1] = m_LPMM_Buffer[i + 1];
					fo_DataY[i + 1] = m_MTF_Buffer[i + 1];
				}
				//! <--------------------------------------------

			}//! for ( int i = 0; i < (length - 1); i++ )
		}//! if ( length > 0 )
	}
	break;
	}//! switch(iFlag_X)

	 // �޸� ����
	if (m_Image_Buffer_Int != NULL)
	{
		delete[] m_Image_Buffer_Int;
	}
	if (m_Image_Buffer_Double != NULL)
	{
		delete[] m_Image_Buffer_Double;
	}
	if (m_MTF_Buffer != NULL)
	{
		delete[] m_MTF_Buffer;
	}
	if (m_LPMM_Buffer != NULL)
	{
		delete[] m_LPMM_Buffer;
	}

	m_Image_Buffer_Int = NULL;
	m_Image_Buffer_Double = NULL;
	m_MTF_Buffer = NULL;
	m_LPMM_Buffer = NULL;

	if (length <= 0)
	{
		return false;
	}

	return true;
}

bool CInspection_Alg::getSFR2(unsigned char* r_buf, unsigned char* g_buf, unsigned char* b_buf, int fi_iWidth, int fi_iHight, float fi_fCcdSize, CRect fi_RectRoi, int& iDataCnt, double fo_DataX[MAX_SFR_DATA_CNT], double fo_DataY[MAX_SFR_DATA_CNT], double dGain)
{
	int iSizeX = fi_RectRoi.right - fi_RectRoi.left;
	int iSizeY = fi_RectRoi.bottom - fi_RectRoi.top;

	if (iSizeX<10 || iSizeY<10)
	{
		sLogMessage.Format(_T("�˻� ������ ũ�Ⱑ �������Դϴ�. X:%d, Y:%d"), iSizeX, iSizeY);
		return false;
	}

	if (fi_RectRoi.left<5 || fi_RectRoi.top<5)
	{
		sLogMessage.Format(_T("�˻� ������ �������Դϴ�. L:%d, T:%d"), fi_RectRoi.left, fi_RectRoi.top);
		return false;
	}

	if (fi_RectRoi.right >= (fi_iWidth - 5) || fi_RectRoi.bottom >= (fi_iHight - 5))
	{
		sLogMessage.Format(_T("�˻� ������ �������Դϴ�. R:%d, B:%d"), fi_RectRoi.right, fi_RectRoi.bottom);
		return false;
	}


	int    *m_Image_Buffer_Int = NULL;
	double *m_Image_Buffer_Double = NULL;
	double *m_MTF_Buffer = NULL;
	double *m_LPMM_Buffer = NULL;


	//! 2 : ���� ���, 100 : LG �̳����� DLL ���
	int iFlag_X = 100;

	if (m_pSFRalgo == NULL)
	{
		//! DLL Library Loading�� �������� ���� ���� ��� ���
		iFlag_X = 2;
	}

	if (iFlag_X == 100)
	{
		//! LG �̳��� SFR DLL������ ROI�� ũ��� Ȧ���̾�� �Ѵ�. 
		if ((iSizeX % 2) == 0)
			iSizeX--;

		if ((iSizeY % 2) == 0)
			iSizeY--;
	}
	else
	{
		//! SFR ROI�� ũ��� ¦���̾�� �Ѵ�.
		if ((iSizeX % 2) != 0)
			iSizeX--;

		if ((iSizeY % 2) != 0)
			iSizeY--;
	}



	iDataCnt = 0;
	m_dSFR_N_4 = 0.;
	m_dSFR_N_8 = 0.;
	m_dSFR_N_4_X = 0.;
	m_dSFR_N_8_X = 0.;


	// SFR �̹��� ���� ���� �� �ʱ�ȭ
	if (iFlag_X == 100)
	{
		m_Image_Buffer_Int = new int[iSizeX*iSizeY];
		memset(m_Image_Buffer_Int, 0, sizeof(int)*iSizeX*iSizeY);
	}
	else
	{
		m_Image_Buffer_Double = new double[iSizeX*iSizeY];
		memset(m_Image_Buffer_Double, 0, sizeof(double)*iSizeX*iSizeY);
	}

	// Lp/mm���� �� m_pFreqBuffer ���� �� �ʱ�ȭ
	// MTF ���� �� m_pSfrBuffer �̹��� ���� ���� �� �ʱ�ȭ
	if (iSizeX < iSizeY) // �������� SFR�� ����� �̹����� ���� < ����
	{
		m_MTF_Buffer = new double[(int)(iSizeX)];
		memset(m_MTF_Buffer, 0, sizeof(double)*(int)(iSizeX));

		m_LPMM_Buffer = new double[(int)(iSizeX)];
		memset(m_LPMM_Buffer, 0, sizeof(double)*(int)(iSizeX));
	}
	else // ���� >= ���� �ϰ��� �˰��򿡼� ���η� �ٲٹǷ� �Ʒ��� ���� ����
	{
		m_MTF_Buffer = new double[(int)(iSizeY)];
		memset(m_MTF_Buffer, 0, sizeof(double)*(int)(iSizeY));

		m_LPMM_Buffer = new double[(int)(iSizeY)];
		memset(m_LPMM_Buffer, 0, sizeof(double)*(int)(iSizeY));
	}

	int pos;


	// SFR �˰����� �ֵ���(Luminance)���� ���
	// Luminance = 0.299 R + 0.587 G + 0.114 B

	if (iFlag_X == 100)
	{
		if (iSizeX>iSizeY)
		{
			for (int m = 0; m<iSizeY; m++)
			{
				pos = (m + fi_RectRoi.top)*fi_iWidth + fi_RectRoi.left;

				for (int n = 0; n<iSizeX; n++)
				{
					m_Image_Buffer_Int[n*iSizeY + m] = (int)(0.299*r_buf[pos] + 0.587*g_buf[pos] + 0.114*b_buf[pos]);
					pos++;
				}
			}
		}
		else
		{
			for (int m = 0; m<iSizeY; m++)
			{
				pos = (m + fi_RectRoi.top)*fi_iWidth + fi_RectRoi.left;

				for (int n = 0; n<iSizeX; n++)
				{
					m_Image_Buffer_Int[m*iSizeX + n] = (int)(0.299*r_buf[pos] + 0.587*g_buf[pos] + 0.114*b_buf[pos]);
					pos++;
				}
			}
		}

	}
	else
	{
		for (int m = 0; m<iSizeY; m++)
		{
			pos = (m + fi_RectRoi.top)*fi_iWidth + fi_RectRoi.left;

			for (int n = 0; n<iSizeX; n++)
			{
				m_Image_Buffer_Double[m*iSizeX + n] = 0.299*r_buf[pos] + 0.587*g_buf[pos] + 0.114*b_buf[pos];
				pos++;
			}
		}
	}

	int length = 0;

	if (iFlag_X == 100 && m_pSFRalgo != NULL)
	{
		m_dSFR_N_4_X = SFR_N_4_POSITION_X;
		m_dSFR_N_8_X = SFR_N_8_POSITION_X;

		//! [���� ����] 
		//!		1) �Է� ���۴� �ݵ�� int�� �����̾�� �Ѵ�. 
		//!		2) ��� Y �������� ������ '%'�̹Ƿ�, 0 ~ 100�� ������ ������. 
		if (iSizeX>iSizeY)
		{
			m_pSFRalgo(m_Image_Buffer_Int, iSizeY, iSizeX, 0, fo_DataY, fo_DataX, SFR_N_4_POSITION_X, &m_dSFR_N_4, SFR_N_8_POSITION_X, &m_dSFR_N_8, 0);
		}
		else
			m_pSFRalgo(m_Image_Buffer_Int, iSizeX, iSizeY, 0, fo_DataY, fo_DataX, SFR_N_4_POSITION_X, &m_dSFR_N_4, SFR_N_8_POSITION_X, &m_dSFR_N_8, 0);

		//pSFRalgo((int*)pImage_Buffer, iSizeX, iSizeY, 0, fo_DataY, fo_DataX, SFR_N_4_PositionX, &m_dSFR_N_4, SFR_N_8_PositionX, &m_dSFR_N_8, 0);

		length = iSizeX - 1;

		//! ��� Y �������� ������ '0 ~ 1'������ �Ҽ��� ���ڷ� �����Ѵ�. 
		//! SFR ��� �����͸� �̿��� ó�� ������ ���߱� ���ؼ��̴�.

		for (int i = 0; i < length; i++)
		{
			if (i >= MAX_SFR_DATA_CNT)
			{
				break;
			}

			fo_DataY[i] /= 100.;
		}

		m_dSFR_N_4 /= 100.;
		m_dSFR_N_8 /= 100.;

		//FreeLibrary(hDll);
	}
	else
	{
		// SFR �˰��� ����
		// �������� 1. m_LPMM_Buffer: X�� ������� �� ���� (��� ����)
		//         2. m_MTF_Buffer: MTF���� �� ���� (��� ����)
		//         3. m_Image_Buffer: Luminance �̹��� ���� (�Է� ����)
		//         4. m_Image_Width: �̹��� ���α���
		//		   5. m_Image_Height: �̹��� ���α���

		int iRet = SFR.SFR_Processing(m_LPMM_Buffer, m_MTF_Buffer, m_Image_Buffer_Double, iSizeX, iSizeY, fi_fCcdSize, iFlag_X, dGain);
		if (iRet == 1)
		{
			// �޸� ����
			if (m_Image_Buffer_Double != NULL)
			{
				delete[] m_Image_Buffer_Double;
			}
			if (m_MTF_Buffer != NULL)
			{
				delete[] m_MTF_Buffer;
			}
			if (m_LPMM_Buffer != NULL)
			{
				delete[] m_LPMM_Buffer;
			}

			m_Image_Buffer_Double = NULL;
			m_MTF_Buffer = NULL;
			m_LPMM_Buffer = NULL;

			return false;
		}

		switch (iFlag_X)
		{
		case 0:
		{
			length = Linear_Interpolation(m_LPMM_Buffer, m_MTF_Buffer, fo_DataX, fo_DataY);
		}
		break;
		case 1:
		{
			length = iSizeX - 1;
		}
		break;
		case 2:
		{
			length = iSizeX / 2 - 1;
		}
		break;
		}//! switch(iFlag_X)
	}

	iDataCnt = length;

	if (iDataCnt > MAX_SFR_DATA_CNT)
	{
		TRACE("SFR ��� Data ���� �ʰ� %d\n\n", iDataCnt);
	}

	switch (iFlag_X)
	{
	case 0:
	{
		int index = (int)(iDataCnt / 4 + 0.5);
		m_dSFR_N_4 = fo_DataY[index];
		m_dSFR_N_4_X = (double)(iDataCnt / 4);

		index = (int)(iDataCnt / 8 + 0.5);
		m_dSFR_N_8 = fo_DataY[index];
		m_dSFR_N_8_X = (double)(iDataCnt / 8);
	}
	break;
	case 1:
	case 2:
	{
		if (length > 0)
		{
			double start = 0;
			double end = 0;

			int iFlag_Cnt = 0;

			double dSFR_N_4 = SFR_N_4_POSITION_X;
			double dSFR_N_8 = SFR_N_8_POSITION_X;

			m_dSFR_N_4_X = SFR_N_4_POSITION_X;
			m_dSFR_N_8_X = SFR_N_8_POSITION_X;

			for (int i = 0; i < (length - 1); i++)
			{
				if (i >= (MAX_SFR_DATA_CNT - 2))
				{
					//! Output ������ ������ ũ�⸦ �Ѵ��� Ȯ��
					break;
				}

				start = m_LPMM_Buffer[i];
				end = m_LPMM_Buffer[i + 1];

				//! SFR N/8���� ã�Ƴ���.
				if ((start <= dSFR_N_8 && end >= dSFR_N_8) && (iFlag_Cnt == 0))
				{
					if (fabs(start - dSFR_N_8) < 0.01)
					{
						m_dSFR_N_8 = m_MTF_Buffer[i];
						iFlag_Cnt++;
					}
					else if (fabs(end - dSFR_N_8) < 0.01)
					{
						m_dSFR_N_8 = m_MTF_Buffer[i];
						iFlag_Cnt++;
					}
					else
					{
						m_dSFR_N_8 = ((m_MTF_Buffer[i + 1] - m_MTF_Buffer[i]) / (m_LPMM_Buffer[i + 1] - m_LPMM_Buffer[i]))*(dSFR_N_8 - m_LPMM_Buffer[i]) + m_MTF_Buffer[i];
						iFlag_Cnt++;
					}
				}

				//! SFR N/4���� ã�Ƴ���.
				if ((start <= dSFR_N_4 && end >= dSFR_N_4) && (iFlag_Cnt == 1))
				{
					if (fabs(start - dSFR_N_4) < 0.01)
					{
						m_dSFR_N_4 = m_MTF_Buffer[i];
						iFlag_Cnt++;
					}
					else if (fabs(end - dSFR_N_4) < 0.01)
					{
						m_dSFR_N_4 = m_MTF_Buffer[i];
						iFlag_Cnt++;
					}
					else
					{
						m_dSFR_N_4 = ((m_MTF_Buffer[i + 1] - m_MTF_Buffer[i]) / (m_LPMM_Buffer[i + 1] - m_LPMM_Buffer[i]))*(dSFR_N_4 - m_LPMM_Buffer[i]) + m_MTF_Buffer[i];
						iFlag_Cnt++;
					}
				}

				//! <--------------------------------------------
				//! ��� �����͸� Output ���ۿ� �Է��Ѵ�.
				fo_DataX[i] = m_LPMM_Buffer[i];
				fo_DataY[i] = m_MTF_Buffer[i];

				if (i == (length - 2))
				{
					fo_DataX[i + 1] = m_LPMM_Buffer[i + 1];
					fo_DataY[i + 1] = m_MTF_Buffer[i + 1];
				}
				//! <--------------------------------------------

			}//! for ( int i = 0; i < (length - 1); i++ )
		}//! if ( length > 0 )
	}
	break;
	}//! switch(iFlag_X)

	 // �޸� ����
	if (m_Image_Buffer_Int != NULL)
	{
		delete[] m_Image_Buffer_Int;
	}
	if (m_Image_Buffer_Double != NULL)
	{
		delete[] m_Image_Buffer_Double;
	}
	if (m_MTF_Buffer != NULL)
	{
		delete[] m_MTF_Buffer;
	}
	if (m_LPMM_Buffer != NULL)
	{
		delete[] m_LPMM_Buffer;
	}

	m_Image_Buffer_Int = NULL;
	m_Image_Buffer_Double = NULL;
	m_MTF_Buffer = NULL;
	m_LPMM_Buffer = NULL;

	if (length <= 0)
	{
		return false;
	}

	return true;
}

//! SFR ���꿡�� X���� lp/mm (Line Pair/mm)�� ��, X���� �����͸� 1�������� ���߾��ִ� ������ �Ѵ�.
int CInspection_Alg::Linear_Interpolation(double* xdata, double* ydata, double* L_xdata, double* L_ydata)
{
	int i = 0;
	int num = 0;
	int step = 0;
	int start = 0;
	int end = 0;

	while (1)
	{
		if (ydata[num]< 0.000001)
		{
			break;
		}
		else
		{
			num++;
		}
	}

	start = (int)xdata[step];
	end = (int)xdata[step + 1];

	for (i = 0; i<(int)xdata[num - 1] + 1; i++)
	{
		if (end <= i)
		{
			step++;
			start = (int)xdata[step];
			end = (int)xdata[step + 1];
		}
		if (start <= i && end > i)
		{
			L_xdata[i] = (double)i;
			L_ydata[i] = ((ydata[step + 1] - ydata[step]) / (xdata[step + 1] - xdata[step]))*(L_xdata[i] - xdata[step]) + ydata[step];
		}
	}
	return (int)(xdata[num - 1]);
}

//! DLL �غ�
bool CInspection_Alg::Ready_SFR_DLL(CString fi_sFilePath)
{
	CFileFind clFinder;

	if(clFinder.FindFile(fi_sFilePath) == FALSE)
	{
		return false;
	}

	Release_SFR_DLL();

	m_hDLL_SFR = LoadLibrary(fi_sFilePath);
	if (m_hDLL_SFR == NULL)
	{
		return false;
	}

	m_pSFRalgo = (SFRalgorithm)GetProcAddress(m_hDLL_SFR, "SFRalgorithm");
	if (m_pSFRalgo == NULL)
	{
		return false;
	}

	return true;
}

//! DLL ����
void CInspection_Alg::Release_SFR_DLL()
{
	if (m_hDLL_SFR != NULL)
	{
		FreeLibrary(m_hDLL_SFR);
		m_hDLL_SFR = NULL;
		m_pSFRalgo = NULL;
	}
}

void CInspection_Alg::doGraySharpening(IplImage* img)
{
	int w = img->width;
	int h = img->height;

	IplImage* gray2 = cvCreateImage(cvGetSize(img), 32, 1);

	cvConvertScale(img, gray2, 1.0);

	IplImage* lapl = cvCreateImage(cvGetSize(img), 32, 1);


	CvMat* ker = cvCreateMat(5, 5, CV_32FC1);

	// Imatest Alg ����.
	double ksharp = 0.3;
	cvSet(ker, cvScalarAll(0.0));
	cvSet2D(ker, 2, 0, cvScalarAll(-0.5*ksharp / (1 - ksharp)));
	cvSet2D(ker, 2, 4, cvScalarAll(-0.5*ksharp / (1 - ksharp)));
	cvSet2D(ker, 2, 2, cvScalarAll(1 / (1 - ksharp)));


	cvFilter2D(gray2, lapl, ker);

	cvReleaseMat(&ker);

	double maxv = 0.0;

	double minv = DBL_MAX;

	cvMinMaxLoc(lapl, &minv, &maxv);

	for (int i = 0; i<w*h; i++)
	{
		double lap_val = cvGet1D(lapl, i).val[0];
		int v = (int)((255.0*lap_val / maxv) + 0.5);
		cvSet1D(img, i, cvScalarAll(v));
	}
	maxv = 0.0;

	cvMinMaxLoc(img, &minv, &maxv);

	for (int i = 0; i<w*h; i++)
	{
		double val = cvGet1D(img, i).val[0];
		int v = (int)((255.0*val / maxv) + 0.5);
		cvSet1D(img, i, cvScalarAll(v));
	}

	cvReleaseImage(&gray2);
	cvReleaseImage(&lapl);
}

//! 2�� Fitting
bool CInspection_Alg::calcPeakPos(int cnt, int* pos, double &val_A, double &val_B, double &val_C)
{
	int count = 0;
	double XXXXi = 0.0, XXXi = 0.0, XXi = 0.0, Xi = 0.0, XXYi = 0.0, XYi = 0.0, Yi = 0.0, D;

	for (int i = 0; i<cnt; i++)
	{
		if (pos[i] != 0)
		{
			XXXXi += (double)i*i*i*i;
			XXXi += (double)i*i*i;
			XXi += (double)i*i;
			Xi += (double)i;
			XXYi += (double)i*i*(double)pos[i];
			XYi += (double)i*(double)pos[i];
			Yi += (double)pos[i];
			count++;
		}
	}

	D = (XXXXi*XXi*count) + 2 * (XXXi*Xi*XXi) - ((XXi*XXi*XXi) + (XXXXi*Xi*Xi) + (XXXi*XXXi*count));

	if (D == 0)
		return false;

	val_A = (((count*XXi - Xi*Xi)*XXYi) - ((count*XXXi - Xi*XXi)*XYi) + ((XXXi*Xi - XXi*XXi)*Yi)) / D;
	val_B = (-((count*XXXi - Xi*XXi)*XXYi) + ((count*XXXXi - XXi*XXi)*XYi) - ((XXXXi*Xi - XXXi*XXi)*Yi)) / D;
	val_C = (((XXXi*Xi - XXi*XXi)*XXYi) - ((XXXXi*Xi - XXi*XXXi)*XYi) + ((XXXXi*XXi - XXXi*XXXi)*Yi)) / D;

	return true;
}

//! 2�� Fitting
bool CInspection_Alg::_calcDoublePeakPos(int cnt, double posX[5], double posY[5], double &val_A, double &val_B, double &val_C)
{
	double valY, gapY, rateY, pos;

	if (cnt>5)
	{
		cnt = 5;
	}


	double XXXXi = 0.0, XXXi = 0.0, XXi = 0.0, Xi = 0.0, XXYi = 0.0, XYi = 0.0, Yi = 0.0, D;

	for (int i = 0; i<cnt; i++)
	{
		XXXXi += posX[i] * posX[i] * posX[i] * posX[i];
		XXXi += posX[i] * posX[i] * posX[i];
		XXi += posX[i] * posX[i];
		Xi += posX[i];
		XXYi += posX[i] * posX[i] * posY[i];
		XYi += posX[i] * posY[i];
		Yi += posY[i];
	}

	D = (XXXXi*XXi*cnt) + 2 * (XXXi*Xi*XXi) - ((XXi*XXi*XXi) + (XXXXi*Xi*Xi) + (XXXi*XXXi*cnt));


	//	if(D == 0)
	if (1)
	{
		int maxIndexY = -1;
		double maxVal = -9999;

		for (int i = 0; i<cnt; i++)
		{
			if (maxVal < posY[i])
			{
				maxVal = posY[i];
				maxIndexY = i;
			}
		}

		if ((maxIndexY < 0) || (maxIndexY >= cnt))
			return false;


		// double d_max_Z_pos = -d_calc_B / (2*d_calc_A);

		if (maxIndexY == 0)
		{
			val_A = -0.5;
			val_B = posY[maxIndexY];
		}
		else if (maxIndexY == cnt - 1)
		{
			val_A = -0.5;
			val_B = posY[maxIndexY];
		}
		else
		{
			val_A = -0.5;

			if (posY[maxIndexY - 1] > posY[maxIndexY + 1])
			{
				valY = posY[maxIndexY] - posY[maxIndexY - 1];
				gapY = posY[maxIndexY] - posY[maxIndexY + 1];

				rateY = valY / gapY;
				pos = posX[maxIndexY] + (posX[maxIndexY + 1] - posX[maxIndexY]) * rateY;

				val_B = (posX[maxIndexY - 1] + pos) / 2;
			}
			else
			{
				valY = posY[maxIndexY] - posY[maxIndexY + 1];
				gapY = posY[maxIndexY] - posY[maxIndexY - 1];

				rateY = valY / gapY;
				pos = posX[maxIndexY] + (posX[maxIndexY - 1] - posX[maxIndexY]) * rateY;

				val_B = (posX[maxIndexY + 1] + pos) / 2;
			}
		}

		return true;
	}
	else
	{
		val_A = (((cnt*XXi - Xi*Xi)*XXYi) - ((cnt*XXXi - Xi*XXi)*XYi) + ((XXXi*Xi - XXi*XXi)*Yi)) / D;
		val_B = (-((cnt*XXXi - Xi*XXi)*XXYi) + ((cnt*XXXXi - XXi*XXi)*XYi) - ((XXXXi*Xi - XXXi*XXi)*Yi)) / D;
		val_C = (((XXXi*Xi - XXi*XXi)*XXYi) - ((XXXXi*Xi - XXi*XXXi)*XYi) + ((XXXXi*XXi - XXXi*XXXi)*Yi)) / D;
	}

	return true;
}