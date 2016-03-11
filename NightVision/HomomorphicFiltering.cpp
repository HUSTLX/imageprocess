#include"HomomorphicFiltering.h"
#include"BackUp.h"


void homomorphicFilteringInterface(unsigned char* des, const unsigned char*src, int width, int height, int nChannels, double cutoff, int n, double strengthen)
{
	if(nChannels == 1)
	{
		//单通道
		homomorphicFiltering(des, src, height, width, cutoff, n, strengthen);
	}
	else if(nChannels == 3)
	{
		//多通道
		//转换为YIQ通道，注意Y为[0,255]，I为[-152,152]，Q为[-134,134]
		unsigned char* Y = new unsigned char[width * height];
		int* I = new int[width * height];
		int* Q = new int[width * height];

		int B,G,R;
		int x,y;
		for(y = 0; y < height; y++)
		{
			for(x = 0; x < width; x++)
			{
				B = src[y * width * nChannels + x * nChannels + 0];
				G = src[y * width * nChannels + x * nChannels + 1];
				R = src[y * width * nChannels + x * nChannels + 2];
				Y[y * width + x] = (unsigned char)(0.299 * R + 0.587 * G + 0.114 * B);
				I[y * width + x] = (int)(0.596 * R - 0.275 * G - 0.321 * B);
				Q[y * width + x] = (int)(0.212 * R - 0.523 * G + 0.311 * B);
			}
		}

		//对亮度空间单独进行同态映射
		homomorphicFiltering(Y, Y, height, width ,cutoff, n, strengthen);

		//映射回RGB空间
		for(y = 0; y < height; y++)
		{
			for(x = 0; x < width; x++)
			{
				des[y * width * nChannels + x * nChannels + 0] = (unsigned char)getmax(getmin((Y[y * width + x] - 1.104 * I[y * width + x] + 1.701 * Q[y * width + x]),255),0);
				des[y * width * nChannels + x * nChannels + 1] = (unsigned char)getmax(getmin((Y[y * width + x] - 0.273 * I[y * width + x] - 0.647 * Q[y * width + x]),255),0);
				des[y * width * nChannels + x * nChannels + 2] = (unsigned char)getmax(getmin((Y[y * width + x] + 0.956 * I[y * width + x] + 0.621 * Q[y * width + x]),255),0);
			}
		}

		delete[] Y;
		delete[] I;
		delete[] Q;
		
	}
}

void getButterworthFilter(Complex* mask, int width, int height, double cutoff, int n)
{
	int x,y;
	//设置中心点
	int ax = width / 2;
	int ay = height / 2;

	double alphaL = 0.0999;
	double alphaH = 1.01;
	double D;
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			D = sqrt((double)(y - height/2) * (y - height/2) + (x - width/2) * (x - width/2));
			mask[y * width + x].real = 1 / ( 1 + pow(cutoff/D, 2*n));
			//mask[y * width + x].real = (alphaH-alphaL) / ( 1 + pow(cutoff/D, 2*n)) + alphaL;
			//mask[y * width + x].real = 1 - mask[y * width + x].real;
			mask[y * width + x].imag = 0;
		}
	}

}

void homomorphicFiltering(unsigned char* des, const unsigned char* src, int height, int width, double cutoff, int n, double cvar)
{
	int x,y;

	int ewidth,eheight;
	Complex* temp = new Complex[width * height];	//图像的复数形式
	Complex* eimg1;		//扩展后图像（适应FFT的大小要求）
	Complex* eimg2;		//扩展后图像
	Complex* filter;	//滤波器

	//1.转换图像形式,并取log
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			temp[y * width + x].real = log((double)src[y * width + x] + 1);
			temp[y * width + x].imag = 0;
		}
	}

	//2.扩展源图像
	expand(&eimg1, &ewidth, &eheight, temp, width, height);
	eimg2 = new Complex[ewidth * eheight];
	filter = new Complex[ewidth * eheight];


	//3.获得滤波器
	getButterworthFilter(filter, ewidth, eheight, cutoff, n);

	//4.FFT
	FFT2D(eimg2, eimg1, ewidth, eheight);

	//5.滤波
	filtering(eimg2, ewidth, eheight, filter);

	//6.IFFT
	IFFT2D(eimg1, eimg2, ewidth, eheight);

	//7.还原并取exp
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			temp[y * width + x].real = exp(eimg1[y * ewidth + x].real);
		}
	}

	//8.归一化
	clipNormalization(temp, width, height, cvar);


	//9.转换为图像形式
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			des[y * width + x] = (unsigned char)temp[y * width + x].real;
		}
	}
}