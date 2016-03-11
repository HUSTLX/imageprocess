#include "cv.h"
#include "highgui.h"
#define SRC 0
#define DST 1
//直方图均衡化
#include"Backup.h"
#define HDIM 256
#define SRC 0
#define DST 1
//直方图均衡
void histogram(unsigned char* des,  const unsigned char* src, int width, int height, int nChannels)
{
	CvHistogram *hist = 0;
	int n[] = {HDIM,HDIM,HDIM};
	int r[256] = {0}, g[256] = {0}, b[256] = {0};
	int sum = width * height;		//图像中像素点的综合
	int x,y;

	//分别统计R、G、B的灰度分布，类似于向直方图不同的bin积累信息
	for(y=0; y<height; y++)
		for(x=0; x<width; x++)
		{
			if(src[y * width * nChannels + x * nChannels + 0]!=255)
			{
			b[src[y * width * nChannels + x * nChannels + 0]]++;
			g[src[y * width * nChannels + x * nChannels + 1]]++;
			r[src[y * width * nChannels + x * nChannels + 2]]++;
			}
		}

	//构建直方图的累计分布方程，用于对直方图进行均衡化
	double val[3] = {0};
	for(y=0; y<HDIM; y++)
	{
		val[0] += b[y];
		val[1] += g[y];
		val[2] += r[y];
		b[y] = val[0]*255/sum;
		g[y] = val[1]*255/sum;
		r[y] = val[2]*255/sum;
	}
	//归一化直方图
	for(y=0; y<height; y++)
		for(x=0; x<width; x++)
		{
			    des[y * width * nChannels + x * nChannels + 0] = b[src[y * width * nChannels + x * nChannels + 0]] ;
				des[y * width * nChannels + x * nChannels + 1] = g[src[y * width * nChannels + x * nChannels + 1]] ;
				des[y * width * nChannels + x * nChannels + 2] = r[src[y * width * nChannels + x * nChannels + 2]] ;
		}
}
/*******************************************
自适应直方图均衡
*****************************************************/
void AHE(unsigned char* des,  const unsigned char* src, int width, int height, int nChannels,int M)
{
    double totalnum=(2*M+1)*(2*M+1);
    double prob[3][256];
    int mapping[3][256];
    for(int i=0;i<height;i++)
	{
        for(int j=0;j<width;j++)
		{
            memset(prob,0,sizeof(prob));
            memset(mapping,0,sizeof(mapping));
            //get adaptive val for (j,i)
            for(int u=i-M;u<=i+M;u++)
			{
                for(int v=j-M;v<=j+M;v++)
				{
                    int wi=v;
                    int hi=u;
                    //mirror at boarder
                    if (hi < 0) hi = -hi;
                    else if (hi >= height) hi = 2 * (height - 1) - hi;
                    if (wi < 0) wi = -wi;
                    else if (wi >= width) wi = 2 * (width - 1) - wi;
                    int curind=(hi*width+wi)*nChannels;

                    prob[0][src[curind+2]]+=1;
                    prob[1][src[curind+1]]+=1;
                    prob[2][src[curind+0]]+=1;

                }

            }
            //get ahe val:
            double probsum[3];
            memset(probsum,0,sizeof(probsum));

            for(int k=0;k<256;k++)
			{
                prob[0][k]/=totalnum;
                prob[1][k]/=totalnum;
                prob[2][k]/=totalnum;

                probsum[0]+=prob[0][k];
                probsum[1]+=prob[1][k];
                probsum[2]+=prob[2][k];

                mapping[0][k]=(int)(probsum[0]*255.0);
                mapping[1][k]=(int)(probsum[1]*255.0);
                mapping[2][k]=(int)(probsum[2]*255.0);

            }
            int curIJ=(i*width+j)*nChannels;

            des[curIJ+2]=mapping[0][src[curIJ+2]];
            des[curIJ+1]=mapping[1][src[curIJ+1]];
            des[curIJ]=mapping[2][src[curIJ]];
        }
    }
}

void histogramHSI(unsigned char* des,  const unsigned char* src, int depth,int width, int height, int nChannels)
{
	CvHistogram *hist = 0;
	int n[] = {HDIM,HDIM,HDIM};
	int h[256] = {0}, s[256] = {0}, i[256] = {0};
	int sum = width * height;		//图像中像素点的综合
	int x,y;
	unsigned char* ocl = new unsigned char[width * height * nChannels];
	rgb_hsi( des,src, width, height,  nChannels);
	//分别统计R、G、B的灰度分布，类似于向直方图不同的bin积累信息
	for(y=0; y<height; y++)
		for(x=0; x<width; x++)
		{
			h[des[y * width * nChannels + x * nChannels + 0]]++;
			s[des[y * width * nChannels + x * nChannels + 1]]++;
			i[des[y * width * nChannels + x * nChannels + 2]]++;
		}

	//构建直方图的累计分布方程，用于对直方图进行均衡化
	double val[3] = {0};
	for(y=0; y<HDIM; y++)
	{
		val[0] += h[y];
		val[1] += s[y];
		val[2] += i[y];
		h[y] = val[0]*255/sum;
		s[y] = val[1]*255/sum;
		i[y] = val[2]*255/sum;
	}
	//归一化直方图
	for(y=0; y<height; y++)
		for(x=0; x<width; x++)
		{
			    ocl[y * width * nChannels + x * nChannels + 0] = h[des[y * width * nChannels + x * nChannels + 0]] ;
				ocl[y * width * nChannels + x * nChannels + 1] = s[des[y * width * nChannels + x * nChannels + 1]] ;
				ocl[y * width * nChannels + x * nChannels + 2] = i[des[y * width * nChannels + x * nChannels + 2]] ;
		}
		hsi_rgb(des,ocl,width, height, nChannels);
}
void imsinglehist(IplImage *src, char *winName,CvScalar color)
{
	IplImage* imh;//显示的直方图
	//计算灰度0~255的分布
	int Arr[256]={0};//使用无符号长整型才能保存那么多的像素信息
	int r,c,i;
	for(r=0; r<src->height; r++)
		for(c=0; c<src->width; c++)
			Arr[int(cvGetReal2D(src,r,c))]++;
	//归一化直方图序列Arr
	int Maxpix=0;
	for(i=0; i<256; i++)
		if(Maxpix<=Arr[i])
			Maxpix=Arr[i];
	for(i=0; i<256; i++)
	{
		Arr[i]=(int)(((float)Arr[i]/(float)Maxpix)*500);//显示高度为30~530，故最大显示到500比例
	}
	//下面通过生成一个图像来显示图像
	imh=cvCreateImage(cvSize(600, 600),IPL_DEPTH_8U,3);//为了显示彩色，必须是3通道的，即rgb，而大小为600x600为默认规则
	cvSet(imh, cvScalarAll(255));//赋值255，白色			
	CvFont font;
	cvInitFont( &font, CV_FONT_HERSHEY_PLAIN, 1.0f, 1.0f, 0, 1, CV_AA );
	char text[10];
	for(i=0; i<256; i++)
	{
		for(r=530; r>=530-Arr[i]; r--);
			cvLine(imh,cvPoint(2*(i+20),530),cvPoint(2*(i+20),530-Arr[i]),color,1);
	}
	cvLine(imh,cvPoint(40,530),cvPoint(40,30),color,1);//横轴
	cvLine(imh,cvPoint(40,530),cvPoint(550,530),color,1);//纵轴
	//纵轴刻度
	for(i=0; i<=10; i++)
	{
		cvLine(imh,cvPoint(36,530-i*50),cvPoint(40,530-i*50),color,1);//标刻
		if(i>0&&i<10)
		{
			cvPutText(imh,"0.",cvPoint(5,535-i*50),&font,color);//刻字
			itoa(i,text,10);
			cvPutText(imh,text,cvPoint(18,535-i*50),&font,color);//刻字
			continue;
		}
		if(i==10)
		{
			cvPutText(imh,"1",cvPoint(10,535-i*50),&font,color);//刻字
		}
	}
	//横轴刻度
	for(i=0; i<=10; i++)
	{
		cvLine(imh,cvPoint(40+i*50,534),cvPoint(40+i*50,530),color,1);//纵轴
		itoa(i*25,text,10);
		cvPutText(imh,text,cvPoint(31+i*50,550),&font,color);//刻字
	}
	cvNamedWindow(winName);
	cvShowImage(winName,imh);
	cvReleaseImage(&imh);
}

bool imhist(IplImage *src,bool flag)
{
	//本函数显示rgb图或灰度图的直方图
	if(src->nChannels==1&&src->depth==IPL_DEPTH_8U)//单通道灰度图
	{
		imsinglehist(src,"灰度图的直方图",cvScalar(111,111,111));
		return true;
	}
	else if(src->nChannels==3&&src->depth==IPL_DEPTH_8U && flag==SRC )//标准rgb图
	{
		IplImage *srcr=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
		IplImage *srcg=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
		IplImage *srcb=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
		cvCvtPixToPlane(src,srcr,srcg,srcb,NULL);
		imsinglehist(srcr,"原图像——红色通道直方图",cvScalar(0,0,255));
		imsinglehist(srcg,"原图像——绿色通道直方图",cvScalar(0,255,0));
		imsinglehist(srcb,"原图像——蓝色通道直方图",cvScalar(255,0,0));
		cvReleaseImage(&srcr);
		cvReleaseImage(&srcg);
		cvReleaseImage(&srcb);
		return true;
	}

	else if(src->nChannels==3&&src->depth==IPL_DEPTH_8U && flag==DST )//标准rgb图
	{
		IplImage *srcr=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
		IplImage *srcg=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
		IplImage *srcb=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
		cvCvtPixToPlane(src,srcr,srcg,srcb,NULL);
		imsinglehist(srcr,"目的图像——红色通道直方图",cvScalar(0,0,255));
		imsinglehist(srcg,"目的图像——绿色通道直方图",cvScalar(0,255,0));
		imsinglehist(srcb,"目的图像——蓝色通道直方图",cvScalar(255,0,0));
		cvReleaseImage(&srcr);
		cvReleaseImage(&srcg);
		cvReleaseImage(&srcb);
		return true;
	}

	else
	{
		return false;
	}
	
}