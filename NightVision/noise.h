#include<time.h>
#include<iostream>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#include"Global.h"
#include"Filtering.h"
#include"FFT.h"
#include<math.h>
#include<cv.h>
#include<highgui.h>
#include"Backup.h"
void Addinguniformnoise(unsigned char* des, const unsigned char* src, int width, int height, int nChannels,  double SNR );
double randomUniform(double dMinValue,double dMaxValue);
void Addingsaltandpeppernoise(unsigned char* des, const unsigned char* src, int width, int height, int nChannels,  double SNR);
void Addinggaussnoise(unsigned char* des, const unsigned char* src, int width, int height, int nChannels,  double SNR);
double gaussrand(int M,int D);
void Alphatrimmedmeanfilter(unsigned char* des, const unsigned char* src, int width, int height, int nChannels);
void orderN(double* a, int N);
void Adaptivemedianfilter(unsigned char* des, const unsigned char* src, int width, int height, int nChannels);