
#pragma once

#include <LibACMISCommon\ACMISCommon.h>
#include <opencv2/opencv.hpp>

#define RESIDUAL_MARGIN				22	// 11(G?) + 11(X?)

class CDemosaicResidual
{
private:
	int m_nImageWidth, m_nImageHeight;
	int m_nColorOrder;
	double m_dSigma;

	void GetGreenMask(int image_width, int image_height, int bayerPattern, cv::Mat &maskGr, cv::Mat &maskGb);
	cv::Mat Residual_G_Interpolation(cv::Mat &Src, std::vector<cv::Mat> &bgr, std::vector<cv::Mat> &mask, int bayerPattern, double sigma);
	void Residual_Interpolation(cv::Mat &Bayer, cv::Mat &BMP, RECT* pROI, int nROICount, int bayerPattern = 0, int nMaxValue = 0, double sigma = 1.0); // Partial Image Demosaic

public:
	CDemosaicResidual();
	~CDemosaicResidual();

	cv::Mat Residual_X_Interpolation(cv::Mat &X, cv::Mat &G, cv::Mat &maskX);
	void G_Interpolate(const int *pBayer, int *pBGR, int image_height, int image_width, int bayerPattern, EDATAFORMAT eDataFormat, double sigma = 1.0);
	void G_Interpolate(const unsigned short *pBayer, unsigned short *pBGR, int image_height, int image_width, int bayerPattern, EDATAFORMAT eDataFormat, double sigma = 1.0);
	void G_Interpolate(const BYTE *pBayer, BYTE *pBGR, int image_height, int image_width, int bayerPattern, EDATAFORMAT eDataFormat, double sigma = 1.0);
	void G_Interpolate(const int *pBayer, int *pBGR, int image_height, int image_width, RECT *pROI, int nROICount, int bayerPattern, EDATAFORMAT eDataFormat, double sigma = 1.0);
	void G_Interpolate(const unsigned short *pBayer, unsigned short *pBGR, int image_height, int image_width, RECT *pROI, int nROICount, int bayerPattern, EDATAFORMAT eDataFormat, double sigma = 1.0);
	void G_Interpolate(const BYTE *pBayer, BYTE *pBGR, int image_height, int image_width, RECT *pROI, int nROICount, int bayerPattern, EDATAFORMAT eDataFormat, double sigma = 1.0);
	void RB_Interpolate(const int *pBayer, int *pBGR, int image_height, int image_width, int bayerPattern, EDATAFORMAT eDataFormat);
	void RB_Interpolate(const unsigned short *pBayer, unsigned short *pBGR, int image_height, int image_width, int bayerPattern, EDATAFORMAT eDataFormat);
	void RB_Interpolate(const BYTE *pBayer, BYTE *pBGR, int image_height, int image_width, int bayerPattern, EDATAFORMAT eDataFormat);
	void RB_Interpolate(const int *pBayer, int *pBGR, int image_height, int image_width, RECT *pROI, int nROICount, int bayerPattern, EDATAFORMAT eDataFormat);
	void RB_Interpolate(const unsigned short *pBayer, unsigned short *pBGR, int image_height, int image_width, RECT *pROI, int nROICount, int bayerPattern, EDATAFORMAT eDataFormat);
	void RB_Interpolate(const BYTE *pBayer, BYTE *pBGR, int image_height, int image_width, RECT *pROI, int nROICount, int bayerPattern, EDATAFORMAT eDataFormat);

	void Bayer2BGR(const int *pBayer, int *pBGR, int image_height, int image_width, int bayerPattern, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT, double sigma = 1.0);
	void Bayer2BGR(const unsigned short *pBayer, unsigned short *pBGR, int image_height, int image_width, int bayerPattern, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT, double sigma = 1.0);
	void Bayer2BGR(const BYTE *pBayer, BYTE *pBGR, int image_height, int image_width, int bayerPattern, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT, double sigma = 1.0);
	void Bayer2BGR(const int *pBayer, int *pBGR, int image_height, int image_width, RECT *pROI, int nROICount, int bayerPattern, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT, double sigma = 1.0);
	void Bayer2BGR(const unsigned short *pBayer, unsigned short *pBGR, int image_height, int image_width, RECT *pROI, int nROICount, int bayerPattern, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT, double sigma = 1.0);
	void Bayer2BGR(const BYTE *pBayer, BYTE *pBGR, int image_height, int image_width, RECT *pROI, int nROICount, int bayerPattern, EDATAFORMAT eDataFormat = DATAFORMAT_BAYER_PARALLEL_12BIT, double sigma = 1.0);
};
