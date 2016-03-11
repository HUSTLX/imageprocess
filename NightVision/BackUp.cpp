#include"Backup.h"

void deblurMatToImg(IplImage* img, Complex* mat, int img_type)
{
	if( img_type == DEBLUR_SPA_DOMAIN)
	{
		//Ҫ�������ͼ��
		//����������ԭΪͼ��,Ҫ��mat��С������width * height
		//��һ��
		normalization(mat, img->width, img->height);
		int y,x;
		for(y = 0; y < img->height; y++)
		{
			unsigned char* ptr = (unsigned char*)img->imageData + y * img->widthStep;
			for(x = 0; x < img->width; x++)
			{
				ptr[x] = (unsigned char)mat[y * img->width + x].real ;
			}
		}
	}

	if( img_type == DEBLUR_FRE_DOMAIN )
	{
		double* temp = new double[img->width * img->height];
		//Ҫ���Ƶ��ͼ��
		int y,x;
		
		double max = sqrt( mat[0].real *  mat[0].real + mat[0].imag * mat[0].imag );
		double min = sqrt( mat[0].real *  mat[0].real + mat[0].imag * mat[0].imag  );
		for(x = 0; x < img->width * img->height; x++)
		{
			temp[x] = sqrt( mat[x].real *  mat[x].real + mat[x].imag * mat[x].imag ) ;
			if( temp[x] > max )
				max = temp[x];
			if( temp[x] < min )
				min = temp[x];
		}

		//ȡlog������һ������
		for(y = 0; y < img->height; y++)
		{
			unsigned char* ptr = (unsigned char*)img->imageData + y * img->widthStep;
			for(x = 0; x < img->width; x++)
			{
				ptr[x] = (unsigned char)( (log(temp[y * img->width + x] + 1) - log(min + 1)) / (log(max + 1) - log(min + 1)) * 255  ); 
			}
		}
		
		delete[] temp;
	}

}



void deblurImgToMat(IplImage* img, Complex* mat)
{
	//��ͼ��ת��Ϊ��������,Ҫ��mat��С������width * height
	int y,x;
	for(y = 0; y < img->height; y++)
	{
		unsigned char* ptr = (unsigned char*)img->imageData + y * img->widthStep;
		for(x = 0; x < img->width; x++)
		{
			mat[y * img->width + x].real = ptr[x];
			mat[y * img->width + x].imag = 0;
		}
	}

}
void img_data(IplImage* src,unsigned char* img,int width, int height, int nChannels)
{
	for(int y = 0; y < src->height; y++)
	{
		unsigned char* ptr = (unsigned char*)(src->imageData + y * src->widthStep);
		for(int x = 0; x < src->width; x++)
		{
			for(int n = 0; n < nChannels; n++)
			{
				img[y * width * nChannels + x * nChannels + n ] = ptr[nChannels * x + n];
			}
			
		}
	}
}
void data_img(IplImage* dst,unsigned char* src,int width, int height, int nChannels)
{
    for(int y = 0; y < dst->height; y++)
	{
		unsigned char* ptr = (unsigned char*)(dst->imageData + y * dst->widthStep);
		for(int x = 0; x < dst->width; x++)
		{
			for(int n = 0; n < nChannels; n++)
			{
				ptr[nChannels * x + n] = src[y * width * nChannels + x * nChannels + n ];
			}
		}
	}
	
	/*delete[] des;
	cvDestroyWindow("show");
	cvReleaseImage(&dst);*/
}