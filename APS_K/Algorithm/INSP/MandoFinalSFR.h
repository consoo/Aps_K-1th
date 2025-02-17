#pragma once

#include "algo_base.h"

//#include "../Insp/LibAtomResolution/Include/AtomResolutionSFR.h"
#include "../Insp/LibAtomSoftISP/Include/SoftISP.h"

#define FINAL_SPEC_FILE_ERROR	-2
#define FINAL_IMAGE_ERROR		-1
#define FINAL_SFR_ERROR			 1
#define FINAL_DISTORTION_ERROR	 2
#define FINAL_OC_ERROR			 4

#define SAFE_DELETE(ptr) if (ptr != NULL) delete [] ptr;

class CMandoFinalSFR
{
public:
	CMandoFinalSFR(void);
	~CMandoFinalSFR(void);

	void SetUnit(int nUnit){ m_nUnit = nUnit; }

public:
	//int func_check_MandoSFR_Final(IplImage* img, CString ResultPath, bool bMTFRelsultReg = true, bool bAutoMode = false, bool bUvTiming = true);		//20150116 ���¿���� ���� ����
	int func_check_MandoSFR_Final(IplImage* img, unsigned char* imgBuf12, CString ResultPath = _T(""), bool bMTFRelsultReg = true, bool bAutoMode = false, bool bUvTiming = true);		
	int func_check_MandoSFR_Final_Quad(IplImage* img, unsigned char* imgBuf12, CString ResultPath = _T(""), bool bMTFRelsultReg = true, bool bAutoMode = false, bool bUvTiming = true);		
	int GetDistortionMarkPoint(IplImage* img, vector <POINTFLOAT>& pos, CMapData spec_);
	
	int LoadBMPfile(char *filename,unsigned char *data,int *width,int *height);
	BOOL SaveImageToBmp(char *path, unsigned char* bmp_buffer, __int16 width, __int16 height);
	void MarkRectangleOnBMPimage(unsigned char *data,int w,int h,int cx,int cy,int side_length_x,int side_length_y,int R,int G,int B);
	void ImageFlip(unsigned char* source,unsigned char* target,int width,int height);

	CString SetDir_Check(CString sPath);


	unsigned char* m_imgBuf8;
	void RawToBmpQuater(unsigned char *pRaw8, unsigned char *pBMP, int image_width, int image_height,int ch);
	void PRI_12BitToRaw8 (unsigned char *p12Bit, unsigned char *pRawBit8, int w, int h);

	void unpackRaw12(const unsigned char *src, unsigned short *dst, int rows, int cols);
	void Raw12to8(unsigned short *pBuffer, unsigned char *pBuf, int rows, int cols);
	void RawToBmpQuater2(unsigned char *pRaw8, unsigned char *pBMP, int image_width, int image_height, int ch);

private:
	int m_nUnit;
};
