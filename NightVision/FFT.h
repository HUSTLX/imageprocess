#ifndef FFT_H
#define FFT_H

#include"Global.h"

/**************************************************************
函数原型：void leide(Complex* arr , int n)
函数功能：对序列进行倒序排列，采用雷德算法
输入参数：输入复数序列指针arr，序列大小n
输出参数：倒序排列完毕的数组，存储在原位置
**************************************************************/
void leide(Complex* arr, int n);



/**************************************************************
函数原型：void FFT1D(Complex* arr , int n)
函数功能：对1-D序列进行傅里叶变换
输入参数：输入复数序列指针arr，序列大小n，要求n必须为2的k次方
输出参数：输出序列的频域形式，存储在原位置
**************************************************************/
void FFT1D(Complex* arr, int n);




/**************************************************************
函数原型：void FFT2D(Complex* des, const Complex* img, int width, int height)
函数功能：对2-D矩阵进行傅里叶变换
输入参数：频域图像存储位置des；源图像img；图像大小height,width,要求height,width必须为2的k次方,des和img可以相同
输出参数：输出序列的频域形式，存储在des
注意事项：des和img可以相同
**************************************************************/
void FFT2D(Complex* des, const Complex* img, int width, int height);


/**************************************************************
函数原型：void IFFT1D(Complex* arr , int n)
函数功能：对1-D序列进行傅里叶反变换
输入参数：输入复数序列指针arr，序列大小n，要求n必须为2的k次方
输出参数：输出序列的时域形式，存储在原位置
**************************************************************/
void IFFT1D(Complex* arr, int n);




/**************************************************************
函数原型：void IFFT2D(Complex* des, const Complex* mat, int width, int height)
函数功能：对2-D矩阵进行傅里叶反变换
输入参数：空域图像存储位置des；频域图像指针mat，大小height,width,要求height,width必须为2的k次方，其中des和mat可以相同
输出参数：输出图像的空域形式，储存在des中
注意事项：des和mat可以相同
**************************************************************/
void IFFT2D(Complex* des, const Complex* mat, int width, int height);








#endif