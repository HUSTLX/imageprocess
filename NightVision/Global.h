#ifndef GLOBAL_H
#define GLOBAL_H

#define PI 3.141592653
#define INF 9999999999999999999

#include<math.h>


//定义复数结构体
struct Complex
{
	double real;	//实数部	
	double imag;	//虚数部
};


//复数加法
Complex complexAdd(const Complex* a, const Complex* b);


//复数减法
Complex complexSub(const Complex* a, const Complex* b);


//复数乘法
Complex complexMul(const Complex* a, const Complex* b);


//复数矩阵转置
//注意由于是一维表示，转置前后矩阵中元素总数不变，所以可以在原位置转置
void matrixTranspose(Complex* mat, int width, int height);


/**************************************************************
函数原型：normalization(Complex* img, int width, int height)
函数功能：对空域图像进行归一化操作
输入参数：空域图像img，图像的大小width,height
输出参数：归一化图像img
**************************************************************/
void normalization(Complex* img, int width, int height);

void clipNormalization(Complex* img, int width, int height, double cvar);

double getmax(double a, double b);
double getmin(double a, double b);









#endif