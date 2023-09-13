#ifndef SSIM_PSNR_H
#define SSIM_PSNR_H
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

double calculateSSIM(const unsigned char* image1, const unsigned char* image2, int width, int height);

double calculatePSNR(const unsigned char* image1, const unsigned char* image2, int width, int height, double maxVal);

void RGBToGRAY(const unsigned char* imageRGB, unsigned char* imageGRAY, int width, int height);

void GRAYToRGB(const unsigned char* imageGRAY, unsigned char* imageRGB, int width, int height);

#ifdef __cplusplus
}
#endif  //__cplusplus
#endif  // SSIM_PSNR_H