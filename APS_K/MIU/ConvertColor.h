/**************************************************************************************************
*	Project : Eagle Eye Project (Gigabit Ethernet Camera)
*
*
*	
*	File Name : ConvertColor.h 
*	author : hkkim
**************************************************************************************************/
#ifndef __CONVERTCOLOR_H__
#define __CONVERTCOLOR_H__

#include <cv.h>
#include <highgui.h>

/**************************************************************************************************
*   ConvertColor : Display �ϱ� ���� �Է� ������ RGB�� ��ȯ �Ѵ�  
*
*   @param      iPixelFormat, [in] Pixel Format
*   @param      pSrc, [in] ��ȯ�� Source Image (OpenCV�� IplImage*)
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image  
*   @param      pTemp, [in] 10 bit �̻��� ��� ����� destination 
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
void ConvertColor(unsigned int iPixelFormat, IplImage* pSrc, IplImage* pDest, IplImage* pTemp, unsigned int nWidth, unsigned int nHeight);
void CCCR_Interpolation(unsigned short* pData, int nWidth, int nHeight);
void Shift16to12BitMode(unsigned char* pImage, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);
void Shift12to16BitMode(unsigned char* p12bitRaw, unsigned char* p16bitRaw ,int nWidth, int nHeight);

void unpackRaw12(const unsigned char *src, unsigned short *dst, int rows, int cols);

//�߰� 20180305
void DemosaicRCCC(unsigned short *src, unsigned short *dst, int colorOrder, int rows, int cols);
void DemosaicRCCC(unsigned char *src, unsigned char *dst, int colorOrder, int rows, int cols);


#endif	//__CONVERTCOLOR_H__