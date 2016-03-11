#include "cv.h"
#include "highgui.h"
#include"Backup.h"
void histogram(unsigned char* des,  const unsigned char* src,int width, int height, int nChannels);
void AHE(unsigned char* des,  const unsigned char* src, int width, int height, int nChannels,int M);
void histogramHSI(unsigned char* des,  const unsigned char* src, int depth,int width, int height, int nChannels);
bool imhist(IplImage *src,bool flag);
void imsinglehist(IplImage *src, char *winName,CvScalar color);