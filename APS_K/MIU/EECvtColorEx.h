/******************************************************************************
*	Project : Eagle Eye Project (Gigabit Ethernet Camera)
*
*
*	
*	File Name : EECvtColorEx.h 
*	author : hkkim
******************************************************************************/
#ifndef __EECVT_COLOR_EX_H__
#define __EECVT_COLOR_EX_H__

#ifdef _USRDLL
#define DLLFunction  __declspec(dllexport)
#else
#define DLLFunction  __declspec(dllimport)
#endif


#ifdef _cplusplus
extern "C" {
#endif

/**************************************************************************************************
*   ConvYUYVtoRGB : YUYV ������ RGB�� ��ȯ �Ѵ�  
*
*   @param      pSrc, [in] ��ȯ�� Source Image
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
DLLFunction void __stdcall  ConvYUYVtoRGB(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);


/**************************************************************************************************
*   ConvUYVYtoRGB : YUYV ������ RGB�� ��ȯ �Ѵ�  
*
*   @param      pSrc, [in] ��ȯ�� Source Image
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
DLLFunction void __stdcall  ConvUYVYtoRGB(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);


/**************************************************************************************************
*   ConvYVYUtoRGB : YVYU ������ RGB�� ��ȯ �Ѵ�  
*
*   @param      pSrc, [in] ��ȯ�� Source Image 
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
DLLFunction void __stdcall  ConvYVYUtoRGB(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);


/**************************************************************************************************
*   ConvVYUYtoRGB : VYUY ������ RGB�� ��ȯ �Ѵ�  
*
*   @param      pSrc, [in] ��ȯ�� Source Image 
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
DLLFunction void __stdcall  ConvVYUYtoRGB(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);


/**************************************************************************************************
*   ConvYUV420PLANARtoRGB : YUV420 Planar ������ RGB�� ��ȯ �Ѵ�  
*
*   @param      pSrc, [in] ��ȯ�� Source Image 
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
DLLFunction void __stdcall  ConvYUV420PLANARtoRGB(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);


/**************************************************************************************************
*   ConvYUV420PLANARNV21toRGB : YUV420 Planar NV21 ������ RGB�� ��ȯ �Ѵ�  
*
*   @param      pSrc, [in] ��ȯ�� Source Image 
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
//DLLFunction void __stdcall  ConvYUV420PLANARNV21toRGB(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);

/**************************************************************************************************
*   ConvRGBtoYUV420Planar : RGB ������ YUV420 Planar �� ��ȯ �Ѵ�  
*
*   @param      pSrc, [in] ��ȯ�� Source Image 
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
DLLFunction void __stdcall  ConvRGBtoYUV420Planar(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);


/**************************************************************************************************
*   ConvYUV422PLANARtoRGB : YUV422 Planar ������ RGB�� ��ȯ �Ѵ�  
*
*   @param      pSrc, [in] ��ȯ�� Source Image 
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
DLLFunction void __stdcall  ConvYUV422PLANARtoRGB(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);

/**************************************************************************************************
*   ConvRGBtoYUV422Planar : RGB ������ YUV422 Planar �� ��ȯ �Ѵ�  
*
*   @param      pSrc, [in] ��ȯ�� Source Image 
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
DLLFunction void __stdcall  ConvRGBtoYUV422Planar(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);


/**************************************************************************************************
*   ConvYUYVtoY : YUYV ������ Y�� ��ȯ �Ѵ�  
*
*   @param      pSrc, [in] ��ȯ�� Source Image 
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
DLLFunction void __stdcall  ConvYUYVtoY(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);


/**************************************************************************************************
*   ConvYUYVtoYYU420_NV21 : YUYV ������ YUV420 NV21�� ��ȯ �Ѵ�  
*
*   @param      pSrc, [in] ��ȯ�� Source Image 
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
DLLFunction void __stdcall  ConvYUYVtoYYU420_NV21(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);


/**************************************************************************************************
*   ConvYUYVtoYYU420_NV12 : YUYV ������ YUV420 NV12�� ��ȯ �Ѵ�  
*
*   @param      pSrc, [in] ��ȯ�� Source Image 
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
DLLFunction void __stdcall  ConvYUYVtoYYU420_NV12(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);

/**************************************************************************************************
*   ConvUYVYtoY : YUYV ������ Y�� ��ȯ �Ѵ�  
*
*   @param      pSrc, [in] ��ȯ�� Source Image 
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
DLLFunction void __stdcall  ConvUYVYtoY(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);


/**************************************************************************************************
*   ConvUYVYtoRGB_601 : UYVY ������ RGB�� ��ȯ �Ѵ�  - 601 Format
*
*   @param      pSrc, [in] ��ȯ�� Source Image (OpenCV�� IplImage*)
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
DLLFunction void __stdcall  ConvUYVYtoRGB_601(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);

/**************************************************************************************************
*   ConvUYVYtoRGB_709 : UYVY ������ RGB�� ��ȯ �Ѵ�  - 709 Format
*
*   @param      pSrc, [in] ��ȯ�� Source Image (OpenCV�� IplImage*)
*   @param      pDest, [inout] ��ȯ ����� �Ѱ��� Destination Image   
*   @param      nWidth, [in] Image Width
*	@param		nHeight, [in] Image Height  
*
*   @return     Error Value
**************************************************************************************************/
DLLFunction void __stdcall  ConvUYVYtoRGB_709(unsigned char* pSrc, unsigned char* pDest, unsigned int nWidth, unsigned int nHeight);


#ifdef _cplusplus
}
#endif
#endif		//__EECVT_COLOR_EX_H__