//ֱ��ͼ���⻯
#include "hist.h"
#define HDIM 256
#define SRC 0
#define DST 1
//int argc, char** argv
int main(int argc, char** argv)
{
	IplImage *src = 0, *dst = 0;
	CvHistogram *hist = 0;
	int n[] = {HDIM,HDIM,HDIM};
	int r[256] = {0}, g[256] = {0}, b[256] = {0};

	if(argc!=2 || (src = cvLoadImage(argv[1],3))== NULL)	return -1;

	cvNamedWindow("source",1);
	cvNamedWindow("result",1);

	int width = src->width;			
	int height = src->height;
	int sum = width * height;		//ͼ�������ص���ۺ�
	int i,j;

	//�ֱ�ͳ��R��G��B�ĻҶȷֲ�����������ֱ��ͼ��ͬ��bin������Ϣ
	for(i=0; i<height; i++)
		for(j=0; j<width; j++)
		{
			b[((uchar*)(src->imageData+i*src->width))[j*src->nChannels+0]]++;
			g[((uchar*)(src->imageData+i*src->width))[j*src->nChannels+1]]++;
			r[((uchar*)(src->imageData+i*src->width))[j*src->nChannels+2]]++;
		}

	//����ֱ��ͼ���ۼƷֲ����̣����ڶ�ֱ��ͼ���о��⻯
	double val[3] = {0};
	for(i=0; i<HDIM; i++)
	{
		val[0] += b[i];
		val[1] += g[i];
		val[2] += r[i];
		b[i] = val[0]*255/sum;
		g[i] = val[1]*255/sum;
		r[i] = val[2]*255/sum;
	}

	dst = cvCreateImage(cvSize(width,height),8,3);
	//��һ��ֱ��ͼ
	for(i=0; i<height; i++)
		for(j=0; j<width; j++)
		{
			((uchar*)(dst->imageData+i*dst->widthStep))[j*dst->nChannels+0]=b[((uchar*)(src->imageData+i*src->widthStep))[j*src->nChannels+0]];
			((uchar*)(dst->imageData+i*dst->widthStep))[j*dst->nChannels+1]=g[((uchar*)(src->imageData+i*src->widthStep))[j*src->nChannels+1]];
			((uchar*)(dst->imageData+i*dst->widthStep))[j*dst->nChannels+2]=r[((uchar*)(src->imageData+i*src->widthStep))[j*src->nChannels+2]];
		}

	imhist(src,SRC);
	imhist(dst,DST);
	cvShowImage("source",src);
	cvShowImage("result",dst);
	cvSaveImage("out.jpg",dst);
	cvWaitKey(0);

	cvDestroyWindow("source");
	cvDestroyWindow("result");
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	cvReleaseHist(&hist);

	return 0;
}