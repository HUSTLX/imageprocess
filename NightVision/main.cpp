#include"Backup.h"
#include<cv.h>
#include<highgui.h>
using namespace std;
using namespace cv;
int main()
{
	IplImage* src = cvLoadImage("dm5.bmp");
	IplImage* gray = cvCreateImage(cvGetSize(src), src->depth, 1);
	IplImage *color_dst = cvCloneImage(src);cvSetZero(color_dst);  
    CvMemStorage *storage = cvCreateMemStorage();  
    CvSeq *lines = 0; 
	cvCvtColor(src,gray,CV_BGR2GRAY);
	int width = src->width;
	int height = src->height;
	IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, gray->nChannels);
	IplImage* scr = cvCreateImage(cvGetSize(gray), gray->depth, gray->nChannels);
	cvSmooth(gray, gray, CV_MEDIAN, 3, 0, 0, 0); //��ֵ�˲�������С��������
	cvSmooth(gray, gray, CV_GAUSSIAN, 9, gray->nChannels);
	cvCvtColor(src,scr,CV_BGR2GRAY);
	//cvAdaptiveThreshold( gray, gray, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,3,5);
	cvThreshold( gray, gray, 190, 255, CV_THRESH_BINARY);
	int nChannels =gray->nChannels;
	cvNamedWindow("origin",0);
    cvResizeWindow("origin",int(width/2),int(height/2));
	cvShowImage("origin", src);
	unsigned char* img = new unsigned char[width * height ];
	unsigned char* des = new unsigned char[width * height ];
	unsigned char* gra = new unsigned char[width * height];
	unsigned char* grt = new unsigned char[width * height];
	img_data(gray, gra,width,height, nChannels);
	img_data(scr, img,width,height,nChannels);
	AHE(des, img, width, height,nChannels,10);
	Projection( grt,gra,width,height);
	img_extract(des,grt,width,height,1);
	//kirsch(des,gra, width,height);
	data_img( scr, des, width,  height, nChannels);
	cvNamedWindow("����",0);
	cvResizeWindow("����",int(width/2),int(height/2));
	cvShowImage("����", scr);
	cvThreshold(scr, scr, 100, 255, CV_THRESH_BINARY);
	img_data(scr, img,width,height,nChannels);
	Adaptivemedianfilter(des, img, width, height,  nChannels);
	ImageDilation( img, des, width, height,nChannels,1);
	ImageErosion( des,img,width, height,nChannels,1);
	location(img, des, width, height);
	data_img( scr, img, width,  height, nChannels);
	cvNamedWindow("����",0);
    cvResizeWindow("����",int(width/2),int(height/2));
	cvSaveImage("123.bmp",scr); 
	cvShowImage("����", scr);
	data_img( gray,des, width,  height, nChannels);
	cvNamedWindow("erzhi",0);
	cvResizeWindow("erzhi",int(width/2),int(height/2));
	cvShowImage("erzhi", gray);
	cvWaitKey(0);
}
