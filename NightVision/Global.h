#ifndef GLOBAL_H
#define GLOBAL_H

#define PI 3.141592653
#define INF 9999999999999999999

#include<math.h>


//���帴���ṹ��
struct Complex
{
	double real;	//ʵ����	
	double imag;	//������
};


//�����ӷ�
Complex complexAdd(const Complex* a, const Complex* b);


//��������
Complex complexSub(const Complex* a, const Complex* b);


//�����˷�
Complex complexMul(const Complex* a, const Complex* b);


//��������ת��
//ע��������һά��ʾ��ת��ǰ�������Ԫ���������䣬���Կ�����ԭλ��ת��
void matrixTranspose(Complex* mat, int width, int height);


/**************************************************************
����ԭ�ͣ�normalization(Complex* img, int width, int height)
�������ܣ��Կ���ͼ����й�һ������
�������������ͼ��img��ͼ��Ĵ�Сwidth,height
�����������һ��ͼ��img
**************************************************************/
void normalization(Complex* img, int width, int height);

void clipNormalization(Complex* img, int width, int height, double cvar);

double getmax(double a, double b);
double getmin(double a, double b);









#endif