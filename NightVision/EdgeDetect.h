#include"Backup.h"
#include<cv.h>
#include<highgui.h>
#include <math.h>
void rgbToGray(unsigned char* des, const unsigned char* src, int width, int height, int nChannels);
void thresh (unsigned char* des,int width,int height);
void sobel (unsigned char* des, const unsigned char* src, int width, int height);
void roberts(unsigned char* des, const unsigned char* src, int width, int height);
void kirsch(unsigned char* des, const unsigned char* src, int width, int height);
void Projection(unsigned char* des,  const unsigned char* src,int width, int height);
void img_extract(unsigned char* des,  const unsigned char* src,int width, int height, int nChannels);
void digital(unsigned char* des,  const unsigned char* src,int width, int height,int n);
void gray_hist(unsigned char* des,  const unsigned char* src,int width, int height);
void location(unsigned char* des,  const unsigned char* src,int width, int height);
