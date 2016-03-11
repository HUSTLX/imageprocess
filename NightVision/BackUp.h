#ifndef BACKUP_H
#define BACKUP_H
#include"hist.h"
#include"FFT.h"
#include"Filter.h"
#include"Global.h"
#include<cv.h>
#include<highgui.h>
#include<math.h>
#include<stdio.h>
#include"segmentation.h"
#include"HomomorphicFiltering.h"
#include"EdgeDetect.h"
#define DEBLUR_FRE_DOMAIN 1
#define DEBLUR_SPA_DOMAIN 2

void deblurMatToImg(IplImage* img, Complex* mat, int img_type);
void deblurImgToMat(IplImage* img, Complex* mat);
void img_data(IplImage* src,unsigned char* img,int width, int height, int nChannels);
void data_img(IplImage* dst,unsigned char* src,int width, int height, int nChannels);
/*
	//œ‘ æ∆µ”ÚÕº
	IplImage* fimg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	deblurMatToImg(fimg, h, DEBLUR_FRE_DOMAIN);
	cvShowImage("show", fimg);
	cvWaitKey(0);


	//œ‘ æø’”Ú
	IplImage* simg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	deblurMatToImg(simg, h, DEBLUR_SPA_DOMAIN);
	cvShowImage("show", simg);
	cvWaitKey(0);


	IplImage* simg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	DeblurComplex* ttt = new DeblurComplex[width * height];
	for(int z = 0; z < width * height; z++)
		ttt[z].real = w[0][z];
	deblurMatToImg(simg, ttt, DEBLUR_SPA_DOMAIN);
	cvShowImage("show", simg);
	cvWaitKey(0);
*/

#endif
