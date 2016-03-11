#ifndef HOMOMORPHICFILTERING_H
#define HOMOMORPHICFILTERING_H
#include"noise.h"
#include"Global.h"
#include"Filtering.h"
#include"FFT.h"
#include<math.h>

/**************************************************************
����ԭ�ͣ�homomorphicFilteringInterface(unsigned char* des, const unsigned char*src, int width, int height, int nChannels, double cutoff, int n, double strengthen);
�������ܣ���ͼ��src����̬ͬ�˲�
���������Դͼ��src��Ŀ��ͼ��des��ͼ�����width,height,nChannels����ֹƵ��cutoff���˲�������n
���������Ŀ��ͼ��
ע���������Ƕ�ͨ������BGR��˳�������������
**************************************************************/
void homomorphicFilteringInterface(unsigned char* des, const unsigned char*src, int width, int height, int nChannels, double cutoff, int n, double strengthen);


/**************************************************************
����ԭ�ͣ�getButterworthFilter(Complex* mask, int width, int height, double cutoff, int n);
�������ܣ������(width/2, height/2)Ϊ���ĵİ�����˹�˲���
����������˲���ָ��mask��ͼ��Ĵ�Сwidth,height����ֹƵ��cutoff���˲�������n
������������ɵ��˲���mask
ע�����cvarԽ��ͼ��̬��ΧԽС��cvarԽС��ͼ��̬��ΧԽ��
**************************************************************/
void getButterworthFilter(Complex* mask, int width, int height, double cutoff, int n);


/**************************************************************
����ԭ�ͣ�void homomorphicFiltering(unsigned char* des, const unsigned char* src, int height, int width, double cutoff, int n, double cvar);
�������ܣ���Դͼ��src����̬ͬ�˲�
���������Դͼ��src��Ŀ��ͼ��des��ͼ�����width,height������������cutoff(��ֹƵ��),n(����),cvar(ͼ����ǿϵ��)
���������������ͼ��des
**************************************************************/
void homomorphicFiltering(unsigned char* des, const unsigned char* src, int height, int width, double cutoff, int n, double cvar);

#endif