#include "stdAfx.h"
#include "BlobLabeling.h"



CBlobLabeling::CBlobLabeling(void)
{
	m_S_Threshold	 = 0;
	m_L_Threshold	 = 0;
	m_gray_Threshold = 0;
	
	m_Image			= NULL;
	m_gray_Image	= NULL;

	m_pTempBuf  = NULL;
	m_pTempGray = NULL;
		
	m_pbVisited = NULL;
	m_piVisitedX = NULL;
	m_piVisitedY = NULL;

	m_iWidth_Image  = 0;
	m_iHeight_Image = 0;
	m_iWidth_Gray_Image  = 0;
	m_iHeight_Gray_Image = 0;
	
	for (int i = 0; i < _DEF_MAX_BLOBS;i++ )
	{
		m_recBlobs[i].x      = 0;
		m_recBlobs[i].y      = 0;
		m_recBlobs[i].width  = 0;
		m_recBlobs[i].height = 0;
		m_intBlobs[i]        = 0;
		m_Area[i]            = 0;
		m_GD[i]	             = 0.f;
	}
	m_nBlobs	= _DEF_MAX_BLOBS;
}

CBlobLabeling::~CBlobLabeling(void)
{
	FreeMemory();	
}

//! �޸� ���� ����
int CBlobLabeling::AllocMemory(int iImageWidth, int iImageHeight)
{
	if ( iImageWidth <= 0 || iImageHeight <= 0 )
	{
		return -1;
	}

	FreeMemory();

	m_pbVisited = new bool[iImageWidth * iImageHeight];
	m_piVisitedX = new int[iImageWidth * iImageHeight];
	m_piVisitedY = new int[iImageWidth * iImageHeight];
	
	m_pTempBuf  = new short[iImageWidth * iImageHeight];
	m_pTempGray = new unsigned char[iImageWidth * iImageHeight];

	m_Image      = cvCreateImage(cvSize(iImageWidth, iImageHeight),IPL_DEPTH_8U,1);
	m_gray_Image = cvCreateImage(cvSize(iImageWidth, iImageHeight),IPL_DEPTH_8U,1);

	m_iWidth_Image  = iImageWidth;
	m_iHeight_Image = iImageHeight;
	m_iWidth_Gray_Image  = iImageWidth;
	m_iHeight_Gray_Image = iImageHeight;

	if ( m_pbVisited == NULL || m_piVisitedX == NULL || m_piVisitedY == NULL )
	{
		return -2;
	}
	if ( m_pTempBuf == NULL || m_pTempGray == NULL || m_Image == NULL || m_gray_Image == NULL )
	{
		return -3;
	}

	return 1;
}

//! �޸� ���� ����
void CBlobLabeling::FreeMemory()
{
	if( m_Image != NULL )
	{
		if ( m_Image->roi != NULL )
		{
			cvResetImageROI(m_Image);
		}
		cvReleaseImage( &m_Image );
		m_Image = NULL;
	}
	if( m_gray_Image != NULL )
	{
		if ( m_gray_Image->roi != NULL )
		{
			cvResetImageROI(m_gray_Image);
		}
		cvReleaseImage( &m_gray_Image );
		m_gray_Image = NULL;
	}

	if ( m_pTempBuf != NULL )
	{
		delete [] m_pTempBuf;
		m_pTempBuf = NULL;
	}
	if ( m_pTempGray != NULL )
	{
		delete [] m_pTempGray;
		m_pTempGray = NULL;
	}

	m_iWidth_Image  = 0;
	m_iHeight_Image = 0;
	m_iWidth_Gray_Image  = 0;
	m_iHeight_Gray_Image = 0;
		
	if ( m_pbVisited != NULL )
	{
		delete [] m_pbVisited;
		m_pbVisited = NULL;
	}
	if ( m_piVisitedX != NULL )
	{
		delete [] m_piVisitedX;
		m_piVisitedX = NULL;
	}
	if ( m_piVisitedY != NULL )
	{
		delete [] m_piVisitedY;
		m_piVisitedY = NULL;
	}
}

bool CBlobLabeling::SetParam(IplImage* image, IplImage* gray, int S_Threshold,int L_Threshold, float Gray_Threshold, int maxBlob)
{	
	//! ã�Ƴ� �̹� (Label Blob)�� ��� �ʱ�ȭ
	for (int i = 0; i < _DEF_MAX_BLOBS;i++ )
	{
		m_recBlobs[i].x      = 0;
		m_recBlobs[i].y      = 0;
		m_recBlobs[i].width  = 0;
		m_recBlobs[i].height = 0;
		m_intBlobs[i]        = 0;
		m_Area[i]            = 0;
		m_GD[i]	             = 0.f;
	}
	m_nBlobs	= _DEF_MAX_BLOBS;

	//! �Է¹޴� 2���� �̹��� ������ ũ�Ⱑ ���ٴ� ���� Ȯ�ι޴´�.
	if ( image->width != gray->width || image->height != gray->height )
	{
		return false;
	}
	if ( image->width <= 0 || image->height <= 0 )
	{
		return false;
	}

	bool bFlag_Memory = false;
	int  iAllocWidth  = m_iWidth_Image;
	int  iAllocHeight = m_iHeight_Image;
	
	//! Ȥ�ó� ���۰� �����Ǿ� ���� ������, �Է¹��� �̹��� ������ ũ�⸸ŭ �����Ѵ�. 
	if ( m_pbVisited == NULL || m_piVisitedX == NULL || m_piVisitedY == NULL )
	{
		bFlag_Memory = true;
		iAllocWidth  = image->width;
		iAllocHeight = image->height;
	}
	if ( m_pTempBuf == NULL || m_pTempGray == NULL || m_Image == NULL || m_gray_Image == NULL )
	{
		bFlag_Memory = true;
		iAllocWidth  = image->width;
		iAllocHeight = image->height;
	}
	if ( m_iWidth_Image <= 0 || m_iHeight_Image <= 0 )
	{
		bFlag_Memory = true;
		iAllocWidth  = image->width;
		iAllocHeight = image->height;
	}

	/*if ( image->width != m_iWidth_Image )
	{
		bFlag_Memory = true;
	}
	if ( image->height != m_iHeight_Image )
	{
		bFlag_Memory = true;
	}*/

	//! �Էµ� �̹����� ���� ���ۺ��� Ŭ ��쿡�� �ٽ� �����Ѵ�. 
	if ( image->width > m_iWidth_Image )
	{
		bFlag_Memory = true;
		iAllocWidth  = image->width;
	}
	if ( image->height > m_iHeight_Image )
	{
		bFlag_Memory = true;
		iAllocHeight = image->height;
	}

	if ( bFlag_Memory == true )
	{
		FreeMemory();

		int iRet = AllocMemory(iAllocWidth, iAllocHeight);
		if ( iRet < 0 )
		{
			return false;
		}
	}

	//! ���� ROI ����
	if ( m_Image->roi != NULL )
	{
		cvResetImageROI(m_Image);
	}
	if ( m_gray_Image->roi != NULL )
	{
		cvResetImageROI(m_gray_Image);
	}
	//! ROI ����
	if ( image->roi != NULL )
	{
		cvSetImageROI(m_Image, cvRect(0, 0, image->roi->width, image->roi->height));
	}
	if ( gray->roi != NULL )
	{
		cvSetImageROI(m_gray_Image, cvRect(0, 0, gray->roi->width, gray->roi->height));
	}

	//! �̹��� ���� ����
	cvCopyImage(image, m_Image);
	cvCopyImage(gray,  m_gray_Image);
			
	//! ������ �Է¹޴´�.
	m_S_Threshold	  = S_Threshold;
	m_L_Threshold	  = L_Threshold;
	m_gray_Threshold  = Gray_Threshold;
	m_MaxDefectsCount = maxBlob;

	return true;
}

void CBlobLabeling::DoLabeling()
{
	m_nBlobs = Labeling(m_Image, m_gray_Image, m_S_Threshold, m_L_Threshold, m_gray_Threshold);

	//! ROI ����
	if ( m_Image->roi != NULL )
	{
		cvResetImageROI(m_Image);
	}
	if ( m_gray_Image->roi != NULL )
	{
		cvResetImageROI(m_gray_Image);
	}
}

int CBlobLabeling::Labeling(IplImage* image, IplImage* gray, int S_Threshold, int L_Threshold, float Gray_Threshold)
{
	if( image->nChannels != 1 ) 	return 0;

	if ( m_pbVisited == NULL || m_piVisitedX == NULL || m_piVisitedY == NULL )
	{
		return 0;
	}
	if ( m_pTempBuf == NULL || m_pTempGray == NULL || m_Image == NULL || m_gray_Image == NULL )
	{
		return 0;
	}

	int nNumber = 0;
	
	int nWidth	= image->width;
	int nHeight = image->height;
	if ( image->roi != NULL )
	{
		nWidth = image->roi->width;
		nHeight = image->roi->height;
	}

	int gWidth	= gray->width;
	int gHeight = gray->height;
	if ( gray->roi != NULL )
	{
		gWidth = gray->roi->width;
		gHeight = gray->roi->height;
	}

	int i,j;

	//! ���� �̹��� ������ ���۸� �غ��Ѵ�. 
	for(j=0;j<nHeight;j++)
	{
		for(i=0;i<nWidth ;i++)
		{
			m_pTempBuf[j*nWidth+i] = (short)(uchar)image->imageData[j*image->widthStep+i];
		}
	}
	for(j=0;j<gHeight;j++)
	{
		for(i=0;i<gWidth ;i++)
		{
			m_pTempGray[j*nWidth+i] = (unsigned char)gray->imageData[j*gray->widthStep+i];
		}
	}	

	// ���̺��� ���� ����Ʈ �ʱ�ȭ
	InitvPoint(nWidth, nHeight);

	// ���̺�
	nNumber = _Labeling(m_pTempBuf, m_pTempGray, nWidth, nHeight, S_Threshold, L_Threshold, Gray_Threshold);
		
	if( nNumber != 0 )	DetectLabelingRegion(m_pTempBuf, nWidth, nHeight);

	//for(j=0;j<nHeight;j++)
	//for(i=0;i<nWidth ;i++)
	//	image->imageData[j*image->widthStep+i] = m_pTempBuf[j*nWidth+i];
	
	return nNumber;
}

// m_vPoint �ʱ�ȭ �Լ�
void CBlobLabeling::InitvPoint(int nWidth, int nHeight)
{
	if ( m_pbVisited == NULL || m_piVisitedX == NULL || m_piVisitedY == NULL )
	{
		return;
	}
	
	int nX, nY;

	for(nY = 0; nY < nHeight; nY++)
	{
		for(nX = 0; nX < nWidth; nX++)
		{
			m_pbVisited [nY * nWidth + nX] = false;
			m_piVisitedX[nY * nWidth + nX] = nX;
			m_piVisitedY[nY * nWidth + nX] = nY;			
		}
	}
}

// Size�� nWidth�̰� nHeight�� DataBuf���� 
// nThreshold���� ���� ������ ������ �������� blob���� ȹ��
int CBlobLabeling::_Labeling(short *DataBuf, unsigned char *GrayBuf, int nWidth, int nHeight, int S_Threshold, int L_Threshold, float Gray_Threshold)
{
	int Index = 0, num = 0;
	int nX, nY, k, l;
	int StartX , StartY, EndX , EndY;
	int area = 0;
	float diff_gray = 0;
	
	// Find connected components
	for(nY = 0; nY < nHeight; nY++)
	{
		for(nX = 0; nX < nWidth; nX++)
		{
			if(DataBuf[nY * nWidth + nX] == 255)		// Is this a new component?, 255 == Object
			{
				num++;

				//! Label ����, 1 based
				DataBuf[nY * nWidth + nX] = num;
				
				//! �̹��� ����� ��θ� �����Ͽ�, �簢 ������ �����Ѵ�. 
				StartX = nX;
				StartY = nY;
				EndX = nX;
				EndY= nY;

				__NRFIndNeighbor(DataBuf, nWidth, nHeight, nX, nY, &StartX, &StartY, &EndX, &EndY);
				
				//! �簢 ���������� Ư¡���� ���Ѵ�. 
				area = __Area(DataBuf, StartX, StartY, EndX, EndY, nWidth, num);
				diff_gray = __GrayMean(DataBuf, GrayBuf,StartX, StartY, EndX, EndY, nWidth,nHeight, num);

				if(area < S_Threshold || area > L_Threshold || diff_gray < Gray_Threshold)
				{
					//! �̹��� ���� ������ ����� ����������, Ư¡���� ������ ���� ���̹Ƿ� Label ����

		 			for(k = StartY; k <= EndY; k++)
					{
						for(l = StartX; l <= EndX; l++)
						{
							if(DataBuf[k * nWidth + l] == num)
								DataBuf[k * nWidth + l] = 0;
						}
					}
					--num;
					if(num >= _DEF_MAX_BLOBS) {
						return  num;}
				}
				else if (area >= S_Threshold && area <= L_Threshold && diff_gray >= Gray_Threshold){
					//! �̹��� �����Ǿ�, ������ Label�� �״�� �д�.
					
					m_Area[num-1] = area;
					m_GD[num-1] = diff_gray;
					if (num >= m_MaxDefectsCount)
					{
						return num;
					}
				}
			}
		}
	}
	
	return num;	
}

// Blob labeling�ؼ� ����� ����� rec�� �� 
void CBlobLabeling::DetectLabelingRegion(short *DataBuf, int nWidth, int nHeight)
{
	int nX, nY;
	int nLabelIndex;

	BOOL bFirstFlag[_DEF_MAX_BLOBS + 1] = {FALSE,};
	
	for(nY = 1; nY < nHeight - 1; nY++)
	{
		for(nX = 1; nX < nWidth - 1; nX++)
		{
			nLabelIndex = DataBuf[nY * nWidth + nX];

			if(nLabelIndex != 0)	// Is this a new component?, 255 == Object
			{
				if(bFirstFlag[nLabelIndex] == FALSE)
				{
					m_recBlobs[nLabelIndex-1].x			= nX;
					m_recBlobs[nLabelIndex-1].y			= nY;
					m_recBlobs[nLabelIndex-1].width		= 0;
					m_recBlobs[nLabelIndex-1].height	= 0;
				
					bFirstFlag[nLabelIndex] = TRUE;
				}
				else
				{
					int left	= m_recBlobs[nLabelIndex-1].x;
					int right	= left + m_recBlobs[nLabelIndex-1].width;
					int top		= m_recBlobs[nLabelIndex-1].y;
					int bottom	= top + m_recBlobs[nLabelIndex-1].height;

					if( left   >= nX )	left	= nX;
					if( right  <= nX )	right	= nX;
					if( top    >= nY )	top		= nY;
					if( bottom <= nY )	bottom	= nY;

					m_recBlobs[nLabelIndex-1].x			= left;
					m_recBlobs[nLabelIndex-1].y			= top;
					m_recBlobs[nLabelIndex-1].width		= right - left;
					if (bottom - top == 0){
						m_recBlobs[nLabelIndex-1].height = 1;}
					else{
						m_recBlobs[nLabelIndex-1].height	= bottom - top;
					}
					m_intBlobs[nLabelIndex-1]			= nLabelIndex;
				}
			}//! if(nLabelIndex != 0)		
		}//! for(nX = 1; nX < nWidth - 1; nX++)
	}//! for(nY = 1; nY < nHeight - 1; nY++)	
}

// Blob Labeling�� ���� ���ϴ� function
// 2000�� ����ó����ȸ�� �Ǹ� �� ����
int CBlobLabeling::__NRFIndNeighbor(short *DataBuf, int nWidth, int nHeight, int nPosX, int nPosY, int *StartX, int *StartY, int *EndX, int *EndY )
{
	CvPoint CurrentPoint;
	
	CurrentPoint.x = nPosX;
	CurrentPoint.y = nPosY;

	m_pbVisited [CurrentPoint.y * nWidth +  CurrentPoint.x] = true;
	m_piVisitedX[CurrentPoint.y * nWidth +  CurrentPoint.x] = nPosX;
	m_piVisitedY[CurrentPoint.y * nWidth +  CurrentPoint.x] = nPosY;
				
	while(1)
	{
		if( (CurrentPoint.x != 0) && (DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x - 1] == 255) )   // -X ����
		{			
			if( m_pbVisited[CurrentPoint.y * nWidth +  CurrentPoint.x - 1] == false )
			{
				DataBuf     [CurrentPoint.y * nWidth +  CurrentPoint.x - 1]	= DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_pbVisited [CurrentPoint.y * nWidth +  CurrentPoint.x - 1]	= true;
				m_piVisitedX[CurrentPoint.y * nWidth +  CurrentPoint.x - 1]	= CurrentPoint.x;
				m_piVisitedY[CurrentPoint.y * nWidth +  CurrentPoint.x - 1]	= CurrentPoint.y;
				
				CurrentPoint.x--;
				
				if(CurrentPoint.x <= 0)
					CurrentPoint.x = 0;

				if(*StartX >= CurrentPoint.x)
					*StartX = CurrentPoint.x;

				continue;
			}
		}

		if( (CurrentPoint.x != nWidth - 1) && (DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x + 1] == 255) )   // +X ����
		{			
			if( m_pbVisited[CurrentPoint.y * nWidth +  CurrentPoint.x + 1] == false )
			{
				DataBuf     [CurrentPoint.y * nWidth +  CurrentPoint.x + 1]	= DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_pbVisited [CurrentPoint.y * nWidth +  CurrentPoint.x + 1]	= true;
				m_piVisitedX[CurrentPoint.y * nWidth +  CurrentPoint.x + 1]	= CurrentPoint.x;
				m_piVisitedY[CurrentPoint.y * nWidth +  CurrentPoint.x + 1]	= CurrentPoint.y;
				
				CurrentPoint.x++;

				if(CurrentPoint.x >= nWidth - 1)
					CurrentPoint.x = nWidth - 1;
				
				if(*EndX <= CurrentPoint.x)
					*EndX = CurrentPoint.x;

				continue;
			}
		}

		if( (CurrentPoint.y != 0) && (DataBuf[(CurrentPoint.y - 1) * nWidth + CurrentPoint.x] == 255) )   // -Y ����
		{			
			if( m_pbVisited[(CurrentPoint.y - 1) * nWidth +  CurrentPoint.x] == false )
			{
				DataBuf     [(CurrentPoint.y - 1) * nWidth +  CurrentPoint.x] = DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_pbVisited [(CurrentPoint.y - 1) * nWidth +  CurrentPoint.x] = true;
				m_piVisitedX[(CurrentPoint.y - 1) * nWidth +  CurrentPoint.x] = CurrentPoint.x;
				m_piVisitedY[(CurrentPoint.y - 1) * nWidth +  CurrentPoint.x] = CurrentPoint.y;
				
				CurrentPoint.y--;

				if(CurrentPoint.y <= 0)
					CurrentPoint.y = 0;

				if(*StartY >= CurrentPoint.y)
					*StartY = CurrentPoint.y;

				continue;
			}
		}
	
		if( (CurrentPoint.y != nHeight - 1) && (DataBuf[(CurrentPoint.y + 1) * nWidth + CurrentPoint.x] == 255) )   // +Y ����
		{			
			if( m_pbVisited[(CurrentPoint.y + 1) * nWidth +  CurrentPoint.x] == false )
			{
				DataBuf     [(CurrentPoint.y + 1) * nWidth +  CurrentPoint.x] = DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_pbVisited [(CurrentPoint.y + 1) * nWidth +  CurrentPoint.x] = true;
				m_piVisitedX[(CurrentPoint.y + 1) * nWidth +  CurrentPoint.x] = CurrentPoint.x;
				m_piVisitedY[(CurrentPoint.y + 1) * nWidth +  CurrentPoint.x] = CurrentPoint.y;
				
				CurrentPoint.y++;

				if(CurrentPoint.y >= nHeight - 1)
					CurrentPoint.y = nHeight - 1;

				if(*EndY <= CurrentPoint.y)
					*EndY = CurrentPoint.y;

				continue;
			}
		}

		if(	(CurrentPoint.x == m_piVisitedX[CurrentPoint.y * nWidth + CurrentPoint.x]) && 
			(CurrentPoint.y == m_piVisitedY[CurrentPoint.y * nWidth + CurrentPoint.x]) )
		{
			break;
		}
		else
		{
			CurrentPoint.x = m_piVisitedX[CurrentPoint.y * nWidth + CurrentPoint.x];
			CurrentPoint.y = m_piVisitedY[CurrentPoint.y * nWidth + CurrentPoint.x];
		}		
	}

	return 0;
}

//! Modified by LHW (2013/4/19) : Memory �浹 ������ ���� (StartX, StartY�� '0'�̸� ���α׷� �ٿ��)
// ������ ���� blob�� Į�� ���� ������ ũ�⸦ ȹ��
int CBlobLabeling::__Area(short *DataBuf, int StartX, int StartY, int EndX, int EndY, int nWidth, int nLevel)
{
	int nArea = 0;
	int nX, nY;

	for (nY = StartY; nY < EndY; nY++)
	{
		for (nX = StartX; nX < EndX; nX++)
		{
			if (DataBuf[nY * nWidth + nX] == nLevel)
			{
				++nArea;
			}
		}
	}
	
	return nArea;
}

// ������ ���� blob�� Į�� ���� ������ Gray���� ����� ȹ��
float CBlobLabeling::__GrayMean(short *DataBuf, unsigned char *GrayBuf, int StartX, int StartY, int EndX, int EndY, int nWidth, int nHeight, int nLevel)
{
	int nArea = 0;
	int outArea = 0;
	float gMean = 0;
	float outMean = 0;
	float diff = 0;
	int nX, nY;
	int bandwidth = 25;
	
	if (StartY - bandwidth >= 0)
		{StartY = StartY - bandwidth;}
	if (EndY + bandwidth < nHeight)
		{EndY = EndY + bandwidth;}
	else
	{
		EndY = nHeight;
	}

	if (StartX - bandwidth >= 0)
		{StartX = StartX - bandwidth;}
	if (EndX + bandwidth < nWidth)
		{EndX = EndX + bandwidth;}
	else
	{
		EndX = nWidth;
	}

	for (nY = StartY; nY < EndY; nY++){
		for (nX = StartX; nX < EndX; nX++){
			if (DataBuf[nY * nWidth + nX] == nLevel){
				++nArea;
				gMean += GrayBuf[nY * nWidth + nX];}
			else if(DataBuf[nY * nWidth + nX] == 0){
				++outArea;
				outMean += GrayBuf[nY * nWidth + nX];}
		}
	}

	if (nArea*outArea > 0){
		diff = abs(gMean/nArea - outMean/outArea);
	} else
	{
		diff = gMean;
	}
	return diff;
}
