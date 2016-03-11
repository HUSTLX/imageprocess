#ifndef HOMOMORPHICFILTERING_H
#define HOMOMORPHICFILTERING_H
#include"noise.h"
#include"Global.h"
#include"Filtering.h"
#include"FFT.h"
#include<math.h>

/**************************************************************
函数原型：homomorphicFilteringInterface(unsigned char* des, const unsigned char*src, int width, int height, int nChannels, double cutoff, int n, double strengthen);
函数功能：对图像src进行同态滤波
输入参数：源图像src；目标图像des；图像参数width,height,nChannels；截止频率cutoff；滤波器阶数n
输出参数：目标图像
注意事项：如果是多通道按照BGR的顺序进行依次排列
**************************************************************/
void homomorphicFilteringInterface(unsigned char* des, const unsigned char*src, int width, int height, int nChannels, double cutoff, int n, double strengthen);


/**************************************************************
函数原型：getButterworthFilter(Complex* mask, int width, int height, double cutoff, int n);
函数功能：获得以(width/2, height/2)为中心的巴特沃斯滤波器
输入参数：滤波器指针mask；图像的大小width,height；截止频率cutoff；滤波器阶数n
输出参数：生成的滤波器mask
注意事项：cvar越大，图像动态范围越小；cvar越小，图像动态范围越大
**************************************************************/
void getButterworthFilter(Complex* mask, int width, int height, double cutoff, int n);


/**************************************************************
函数原型：void homomorphicFiltering(unsigned char* des, const unsigned char* src, int height, int width, double cutoff, int n, double cvar);
函数功能：对源图像src进行同态滤波
输入参数：源图像src；目标图像des；图像参数width,height；调整参数：cutoff(截止频率),n(阶数),cvar(图像增强系数)
输出参数：处理后的图像des
**************************************************************/
void homomorphicFiltering(unsigned char* des, const unsigned char* src, int height, int width, double cutoff, int n, double cvar);

#endif