#ifndef FILTERING_H
#define FILTERING_H

//Filter.h���˲������ɼ������Ƶ����˲�

#include"Global.h"
#include"FFT.h"
#include<stdlib.h>

////////////////////////////////////�˲������ɲ���/////////////////////////////////////////
/**************************************************************
����ԭ�ͣ�void getGaussMask(Complex** mask, int* width, int* height, int* anchorx = 0, int* anchory = 0, double sigma = 0)
�������ܣ���ø�˹��Ĥ(����
�����������˹��Ĥ��ָ��(�����ڲ�����ռ�)mask����Ĥ������ָ��width,height����׼��sigma��ê��λ��ָ��anchorx,anchory
��������������ڲ�Ϊmask����ռ�,��mask��Сд��width,height����ê��λ��д��anchorx,anchory
**************************************************************/
void getGaussMask(Complex** mask, int* width, int* height, int* anchorx = 0, int* anchory = 0, double sigma = 0);


/**************************************************************
����ԭ�ͣ�void getDefocusMask(Complex** mask, int* width, int* height, int* anchorx = 0, int* anchory= 0, double r = 0)
�������ܣ����ɢ����Ĥ������
���������ɢ����Ĥ��ָ�루�����ڲ�����ռ䣩mask����Ĥ������ָ��width,height��ɢ���뾶r��ê��λ��ָ��anchorx,anchory
��������������ڲ�Ϊmask����ռ䣬��mask��Сд��width,height,��ê��λ��д��anchorx,anchory
**************************************************************/
void getDefocusMask(Complex** mask, int* width, int* height, int* anchorx = 0, int* anchory = 0, double r = 0);



/**************************************************************
����ԭ�ͣ�void Psf2Otf(Complex* filter, int width, int height, const Complex* mask, int mw, int mh, int ax, int ay)
�������ܣ���������������(anchorx,anchory)ת�Ƶ�(0,0)
�������������ɢ����mask,mw,mh,ax,ay����ѧת�ƺ���filter,width,height
�����������ѧת�ƺ���filter
**************************************************************/
void Psf2Otf(Complex* filter, int width, int height, const Complex* mask, int mw, int mh, int ax, int ay);



/**************************************************************
����ԭ�ͣ�void Psf2Otf(Complex* filter, int width, int height, const Complex* mask, int mw, int mh, int ax, int ay)
�������ܣ���������������(anchorx,anchory)ת�Ƶ�(0,0)
�������������ɢ����mask,mw,mh,ax,ay����ѧת�ƺ���filter,width,height
�����������ѧת�ƺ���filter
**************************************************************/
void Psf2Otf(Complex* filter, int width, int height, const Complex* mask, int mw, int mh, int ax, int ay);




////////////////////////////////////�˲������˲�����/////////////////////////////////////////

/**************************************************************
����ԭ�ͣ�void filtering(Complex* mat, int width, int height, const Complex* filter);
�������ܣ��˲�����Ƶ���ͼ������˲������˷�����
����������˻�����filter�����˲�ͼ��mat,ͼ�񳤿�width height
����������˲���ϵ�Ƶ��ͼ�񣬴洢��mat
**************************************************************/
void filtering(Complex* mat, int width, int height, const Complex* filter);




////////////////////////////////////��չ�뷴��չ����/////////////////////////////////////////

/**************************************************************
����ԭ�ͣ�void expand(Complex** eimg, int* ew, int* eh, const Complex* img, int iw, int ih);
�������ܣ���չ���ڿ����϶�ͼ����չ,��չ�󳤿��Ϊ2��n�η����Ҵ��ڵ���Դͼ�񳤿�Ķ���
�������������ͼ��img��ͼ�񳤿�iw,ih����չ��ͼ��Ĵ洢λ��eimg����չ��ͼ���Сew,eh��ָ�룻�㷨���ʱ��eimg������ռ䡣
�����������չ���ͼ��eimg
**************************************************************/
void expand(Complex** eimg, int* ew, int* eh, const Complex* img, int iw, int ih);



/**************************************************************
����ԭ�ͣ�void inverseExpand(Complex* img, int iw, int ih, const Complex* eimg, int ew, int eh);
�������ܣ�����չ���ڿ����϶���չ��ͼ����л�ԭ
�������������ͼ��img��ԭͼ�񳤿�iw,ih����չ���ͼ��eimg����չ��ͼ���Сew,eh
�������������ԭͼ��
**************************************************************/
void inverseExpand(Complex* img, int iw, int ih, const Complex* eimg, int ew, int eh);


/**************************************************************
����ԭ�ͣ�void convolution(Complex* des, const Complex* src, int iw, int ih, const Complex* mask, int mw, int mh, int ax, int ay)
�������ܣ�ʹ����Ĥ�Կ���ͼ����о��
���������Ŀ��ͼ��des������ԭͼ��src��ͼ�񳤿�iw,ih����Ĥmask����Ĥ����mw,mh��ê��ax,ay
���������������ͼ�񣬴洢��des
**************************************************************/
void convolution(Complex* des, const Complex* src, int iw, int ih, const Complex* mask, int mw, int mh, int ax, int ay);




#endif