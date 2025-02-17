#include "../stdafx.h"

#include <Windows.h>
#include <math.h>

class CDemosaic
{
	public:
		CDemosaic();
		~CDemosaic();
	public:
		unsigned short *bBuf;
		unsigned short *gBuf;
		unsigned short *rBuf;

		int rows, cols;
		int colorOrder;

		void interpolateLuminance(const unsigned short *src);
		void interpolateChrominance(const unsigned short *src);

	

		void Bayer2BGR(const unsigned short *pBayer, unsigned short *pBGR, int image_height, int image_width, int bayerPattern);

public:
	/**************************************************************************************************
	*	Project : Eagle Eye Project (Gigabit Ethernet Camera)
	*
	*
	*	
	*	File Name : ConvertColor.h 
	*	author : hkkim
	
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
	void Shift12BitMode(unsigned char* pImage, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);
	void Shift10BitMode(unsigned char* pImage, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);
	void ConvertColor(unsigned int iPixelFormat, IplImage* pSrc, IplImage* pDest, IplImage* pTemp, unsigned int nWidth, unsigned int nHeight);
	void CCCR_Interpolation(unsigned short* pData, int nWidth, int nHeight);
	void Shift16to12BitMode(unsigned char* pImage, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);
	void Shift12to16BitMode(unsigned char* p12bitRaw, unsigned char* p16bitRaw ,int nWidth, int nHeight);

	void unpackRaw12(const unsigned char *src, unsigned short *dst, int rows, int cols);

	//�߰� 20180305
	void DemosaicRCCC(unsigned short *src, unsigned short *dst, int colorOrder, int rows, int cols);
	void DemosaicRCCC(unsigned char *src, unsigned char *dst, int colorOrder, int rows, int cols);

	void Raw12_to_Raw8(const unsigned short *src, unsigned char *dst, int rows, int cols);
};

