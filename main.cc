#include "browse.h"
#define USE_SHELL_OPEN
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define TJE_IMPLEMENTATION
#include "tiny_jpeg.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "timing.h"
#include <stdint.h>
#include <assert.h>
#include "ssim-psnr.h"

char saveFile[1024];

// 读取图片
unsigned char *loadImage(const char *filename, int *Width, int *Height, int *Channels) {
    return (stbi_load(filename, Width, Height, Channels, 0));
}

// 保存图片
void saveImage(const char *filename, int Width, int Height, int Channels, unsigned char *Output) {
    memcpy(saveFile + strlen(saveFile), filename, strlen(filename));
    *(saveFile + strlen(saveFile) + 1) = 0;

    if (!tje_encode_to_file(saveFile, Width, Height, Channels, true, Output)) {
        fprintf(stderr, "save JPEG fail.\n");
        return;
    }
#ifdef USE_SHELL_OPEN
    browse(saveFile);
#endif
}

void splitpath(const char *path, char *drv, char *dir, char *name, char *ext) {
    const char *end;
    const char *p;
    const char *s;
    if (path[0] && path[1] == ':') {
        if (drv) {
            *drv++ = *path++;
            *drv++ = *path++;
            *drv = '\0';
        }
    } else if (drv)
        *drv = '\0';
    for (end = path; *end && *end != ':';) end++;
    for (p = end; p > path && *--p != '\\' && *p != '/';)
        if (*p == '.') {
            end = p;
            break;
        }
    if (ext)
        for (s = end; (*ext = *s++);) ext++;
    for (p = end; p > path;)
        if (*--p == '\\' || *p == '/') {
            p++;
            break;
        }
    if (name) {
        for (s = p; s < end;) *name++ = *s++;
        *name = '\0';
    }
    if (dir) {
        for (s = path; s < p;) *dir++ = *s++;
        *dir = '\0';
    }
}

void getCurrentFilePath(const char *filePath, char *saveFile) {
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    splitpath(filePath, drive, dir, fname, ext);
    size_t n = strlen(filePath);
    memcpy(saveFile, filePath, n);
    char *cur_saveFile = saveFile + (n - strlen(ext));
    cur_saveFile[0] = '_';
    cur_saveFile[1] = 0;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        return (0);
    }
    char *szfile1 = argv[1];
    char *szfile2 = argv[2];
    int width1, width2, height1, height2, channels1, channels2;
    width1 = width2 = height1 = height2 = channels1 = channels2 = 0;
    unsigned char *inputImage1 = loadImage(szfile1, &width1, &height1, &channels1);
    unsigned char *inputImage2 = loadImage(szfile2, &width2, &height2, &channels2);
    if (width1 != width2 || height1 != height2 || channels1 != channels2 || channels1 != 3) {
        printf("Different image formats, Supports 3-channel images only\n");
        return -1;
    }
    unsigned char *grayImage1 = (unsigned char *)malloc(width1 * height1 * sizeof(unsigned char));
    unsigned char *grayImage2 = (unsigned char *)malloc(width2 * height2 * sizeof(unsigned char));
    RGBToGRAY(inputImage1, grayImage1, width1, height1);
    RGBToGRAY(inputImage2, grayImage2, width2, height2);
    double SSIM = calculateSSIM(grayImage1, grayImage2, width1, height1);
    double PSNR = calculatePSNR(grayImage1, grayImage2, width1, height1, 255.0);
    delete[] inputImage1;
    delete[] inputImage2;
    delete[] grayImage1;
    delete[] grayImage2;
    printf("SSIM is: %f\nPSNR is: %f\n", SSIM, PSNR);
    printf("fulfillment");
}