#include <iostream>
#include <cmath>
#include <vector>
#include <omp.h>

#include "ssim-psnr.h"

using namespace std;

// 定义常数
const double C1 = 0.01 * 255 * 0.01 * 255;
const double C2 = 0.03 * 255 * 0.03 * 255;

// 计算均值
double calculateMean(const unsigned char* data, int width, int height) {
    double sum = 0.0;
    for (int i = 0; i < width * height; i++) {
        sum += data[i];
    }
    return sum / (width * height);
}

// 计算方差
double calculateVariance(const unsigned char* data, double mean, int width, int height) {
    double sum = 0.0;
    for (int i = 0; i < width * height; i++) {
        double diff = data[i] - mean;
        sum += diff * diff;
    }
    return sum / (width * height - 1);
}

// 计算协方差
double calculateCovariance(const unsigned char* data1, const unsigned char* data2, double mean1, double mean2,
                           int width, int height) {
    double sum = 0.0;
    for (int i = 0; i < width * height; i++) {
        double diff1 = data1[i] - mean1;
        double diff2 = data2[i] - mean2;
        sum += diff1 * diff2;
    }
    return sum / (width * height - 1);
}

// 计算SSIM
double calculateSSIM(const unsigned char* image1, const unsigned char* image2, int width, int height) {
    double mean1 = calculateMean(image1, width, height);
    double mean2 = calculateMean(image2, width, height);

    double variance1 = calculateVariance(image1, mean1, width, height);
    double variance2 = calculateVariance(image2, mean2, width, height);

    double covariance = calculateCovariance(image1, image2, mean1, mean2, width, height);

    double numerator = (2.0 * mean1 * mean2 + C1) * (2.0 * covariance + C2);
    double denominator = (mean1 * mean1 + mean2 * mean2 + C1) * (variance1 + variance2 + C2);

    return numerator / denominator;
}

// 计算均方误差（MSE）
double calculateMSE(const unsigned char* image1, const unsigned char* image2, int width, int height) {
    double sumSquaredDiff = 0.0;
    for (size_t i = 0; i < width * height; i++) {
        double diff = static_cast<double>(image1[i]) - static_cast<double>(image2[i]);
        sumSquaredDiff += diff * diff;
    }

    return sumSquaredDiff / static_cast<double>(width * height);
}

// 计算PSNR
double calculatePSNR(const unsigned char* image1, const unsigned char* image2, int width, int height, double maxVal) {
    double mse = calculateMSE(image1, image2, width, height);
    if (mse < 0.0) {
        return -1.0;  // 错误的MSE值
    }

    if (mse == 0.0) {
        return 100.0;  // 两幅图像完全相同，PSNR为无穷大
    }

    return 10.0 * log10((maxVal * maxVal) / mse);
}

void RGBToGRAY(const unsigned char* imageRGB, unsigned char* imageGRAY, int width, int height) {
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (size_t i = 0; i < width * height; i++) {
        imageGRAY[i] = 0.299 * imageRGB[i * 3 + 0] + 0.587 * imageRGB[i * 3 + 1] + 0.114 * imageRGB[i * 3 + 2];
    }
}

void GRAYToRGB(const unsigned char* imageGRAY, unsigned char* imageRGB, int width, int height) {
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (size_t i = 0; i < width * height; i++) {
        unsigned char grayValue = imageGRAY[i];
        imageRGB[i * 3 + 0] = grayValue;  // R
        imageRGB[i * 3 + 1] = grayValue;  // G
        imageRGB[i * 3 + 2] = grayValue;  // B
    }
}