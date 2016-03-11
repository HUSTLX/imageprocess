#ifndef FILTERING_H
#define FILTERING_H

//Filter.h：滤波器生成及空域和频域的滤波

#include"Global.h"
#include"FFT.h"
#include<stdlib.h>

////////////////////////////////////滤波器生成部分/////////////////////////////////////////
/**************************************************************
函数原型：void getGaussMask(Complex** mask, int* width, int* height, int* anchorx = 0, int* anchory = 0, double sigma = 0)
函数功能：获得高斯掩膜(空域）
输入参数：高斯掩膜的指针(函数内部分配空间)mask；掩膜参数的指针width,height；标准差sigma；锚点位置指针anchorx,anchory
输出参数：函数内部为mask分配空间,将mask大小写入width,height，将锚点位置写入anchorx,anchory
**************************************************************/
void getGaussMask(Complex** mask, int* width, int* height, int* anchorx = 0, int* anchory = 0, double sigma = 0);


/**************************************************************
函数原型：void getDefocusMask(Complex** mask, int* width, int* height, int* anchorx = 0, int* anchory= 0, double r = 0)
函数功能：获得散焦掩膜（空域）
输入参数：散焦掩膜的指针（函数内部分配空间）mask；掩膜参数的指针width,height；散焦半径r；锚点位置指针anchorx,anchory
输出参数：函数内部为mask分配空间，将mask大小写入width,height,将锚点位置写入anchorx,anchory
**************************************************************/
void getDefocusMask(Complex** mask, int* width, int* height, int* anchorx = 0, int* anchory = 0, double r = 0);



/**************************************************************
函数原型：void Psf2Otf(Complex* filter, int width, int height, const Complex* mask, int mw, int mh, int ax, int ay)
函数功能：将函数的中心由(anchorx,anchory)转移到(0,0)
输入参数：点扩散函数mask,mw,mh,ax,ay；光学转移函数filter,width,height
输出参数：光学转移函数filter
**************************************************************/
void Psf2Otf(Complex* filter, int width, int height, const Complex* mask, int mw, int mh, int ax, int ay);



/**************************************************************
函数原型：void Psf2Otf(Complex* filter, int width, int height, const Complex* mask, int mw, int mh, int ax, int ay)
函数功能：将函数的中心由(anchorx,anchory)转移到(0,0)
输入参数：点扩散函数mask,mw,mh,ax,ay；光学转移函数filter,width,height
输出参数：光学转移函数filter
**************************************************************/
void Psf2Otf(Complex* filter, int width, int height, const Complex* mask, int mw, int mh, int ax, int ay);




////////////////////////////////////滤波与逆滤波部分/////////////////////////////////////////

/**************************************************************
函数原型：void filtering(Complex* mat, int width, int height, const Complex* filter);
函数功能：滤波：在频域对图像进行滤波，即乘法操作
输入参数：退化函数filter，待滤波图像mat,图像长宽width height
输出参数：滤波完毕的频域图像，存储在mat
**************************************************************/
void filtering(Complex* mat, int width, int height, const Complex* filter);




////////////////////////////////////拓展与反拓展部分/////////////////////////////////////////

/**************************************************************
函数原型：void expand(Complex** eimg, int* ew, int* eh, const Complex* img, int iw, int ih);
函数功能：拓展：在空域上对图像拓展,拓展后长宽均为2的n次方，且大于等于源图像长宽的二倍
输入参数：空域图像img；图像长宽iw,ih；扩展后图像的存储位置eimg；扩展后图像大小ew,eh的指针；算法完成时，eimg被分配空间。
输出参数：拓展后的图像eimg
**************************************************************/
void expand(Complex** eimg, int* ew, int* eh, const Complex* img, int iw, int ih);



/**************************************************************
函数原型：void inverseExpand(Complex* img, int iw, int ih, const Complex* eimg, int ew, int eh);
函数功能：逆拓展：在空域上对拓展的图像进行还原
输入参数：空域图像img；原图像长宽iw,ih；拓展后的图像eimg；扩展后图像大小ew,eh
输出参数：返回原图像
**************************************************************/
void inverseExpand(Complex* img, int iw, int ih, const Complex* eimg, int ew, int eh);


/**************************************************************
函数原型：void convolution(Complex* des, const Complex* src, int iw, int ih, const Complex* mask, int mw, int mh, int ax, int ay)
函数功能：使用掩膜对空域图像进行卷积
输入参数：目标图像des，空域原图像src；图像长宽iw,ih；掩膜mask；掩膜长宽mw,mh；锚点ax,ay
输出参数：卷积后的图像，存储在des
**************************************************************/
void convolution(Complex* des, const Complex* src, int iw, int ih, const Complex* mask, int mw, int mh, int ax, int ay);




#endif