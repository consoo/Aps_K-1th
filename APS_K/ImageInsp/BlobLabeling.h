////////////////////////////////////////////////////
//
//  Ŭ���� : CBlobLabeling
//
//                    by ��ƾ(http://martinblog.net)

//! Modified by LHW, 2013/3/4,  �޸� ����/���� �Լ� �߰�
//! Modified by LHW, 2013/4/17, �޸� ���� �κ� ���� (���� ���� ����)

#pragma once

#include <cv.h>
#include <highgui.h>

typedef struct 
{
	BOOL	bVisitedFlag;
	CvPoint ptReturnPoint;
} Visited;

class  CBlobLabeling
{
public:
	CBlobLabeling(void);
public:
	~CBlobLabeling(void);

private:
	IplImage*	m_Image;				// ���̺��� ���� �̹���
	IplImage*	m_gray_Image;			// Gray �̹���

	int         m_iWidth_Image;
	int			m_iHeight_Image;
	int			m_iWidth_Gray_Image;
	int			m_iHeight_Gray_Image;

	short*			m_pTempBuf;			//! Memory ������ ����, int�� �ƴ� short�� ���۷� ����
	unsigned char*  m_pTempGray;

	int			m_S_Threshold;			// ���̺� ������Ȧ�� ��
	int			m_L_Threshold;			// ���̺� ������Ȧ�� ��
	float		m_gray_Threshold;		// Gray ������Ȧ�� ��
	
	//! ���̺��� �湮����
	//! Ư�� PC������ '�޸� ����' ���� ������ ���� (2013/3/4)
	bool		*m_pbVisited;
	int			*m_piVisitedX;
	int			*m_piVisitedY;
	int			m_MaxDefectsCount;		// �ִ� �ҷ� ��
	
public:
	//! ã�Ƴ� �̹� (Label Blob)�� ���
	int			m_nBlobs;					// ���̺��� ����	
	CvRect		m_recBlobs[_DEF_MAX_BLOBS];	// �� ���̺� ���� 
	int			m_intBlobs[_DEF_MAX_BLOBS];	// �� ���̺� �ε���, ���̺� �� �ε��� ����
	int			m_Area    [_DEF_MAX_BLOBS];	// �� ����� ����ũ��
	float		m_GD      [_DEF_MAX_BLOBS];	// �� ����� GD
	
public:
	//! �޸� ���� ����
	int         AllocMemory(int iImageWidth, int iImageHeight);

	//! �޸� ���� ����
	void        FreeMemory();

	// ���̺� �̹��� ����
	bool		SetParam(IplImage* image, IplImage* gray, int S_Threshold, int L_Threshold, float m_gray_Threshold, int maxBlob);

	// ���̺�(����)
	void		DoLabeling();

private:
	// ���̺�(����)
	int		 Labeling(IplImage* image, IplImage* gray, int S_Threshold, int L_Threshold, float m_gray_Threshold);

	// ����Ʈ �ʱ�ȭ
	void	 InitvPoint(int nWidth, int nHeight);

	// ���̺� ��� ���
	void	 DetectLabelingRegion(short *DataBuf, int nWidth, int nHeight);

	// ���̺�(���� �˰���)
	int		_Labeling(short *DataBuf, unsigned char *GrayBuf, int nWidth, int nHeight, int S_Threshold, int L_Threshold, float Gray_Threshold);
	
	// _Labling ���� ��� �Լ�
	int		__NRFIndNeighbor(short *DataBuf, int nWidth, int nHeight, int nPosX, int nPosY, int *StartX, int *StartY, int *EndX, int *EndY );
	int		__Area(short *DataBuf, int StartX, int StartY, int EndX, int EndY, int nWidth, int nLevel);
	float	__GrayMean(short *DataBuf, unsigned char *GrayBuf, int StartX, int StartY, int EndX, int EndY, int nWidth, int nHeight, int nLevel);	
};