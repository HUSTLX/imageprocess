// noiseAddingRemovingView.cpp : implementation of the CnoiseAddingRemovingView class
//
#include"noise.h"
#include<math.h>
void Addinguniformnoise(unsigned char* des, const unsigned char* src, int width, int height, int nChannels,  double SNR )
{	
	 int noise_num=height*width*SNR;
	// TODO: Add your command handler code here
	 srand((unsigned)time(NULL));  //�����������
	 for(int y = 0; y < height; y++)
	    {
		    for(int x = 0; x < width; x++)
		    {
			    for(int n=0;n<nChannels;n++)
			    des[y * width * nChannels + x * nChannels + n] = src[y * width * nChannels + x * nChannels + n] ;
		    }
	    }
	 for(int i=0; i<noise_num; i++) 
	{
        int row = (int)(rand()%height);
        int col = (int)(rand()%width);
        for(int j=0;j<nChannels;j++)
		{
			double p;
			p=randomUniform(-50,50);
		 
		des[row * width* nChannels + col* nChannels+j]+= p;
		if (des[row * width* nChannels + col* nChannels+j]<0)
			des[row * width* nChannels + col* nChannels+j]=0;
		else if (des[row * width* nChannels + col* nChannels+j]>255)
			des[row * width* nChannels + col* nChannels+j]=255;
		else
			des[row * width* nChannels + col* nChannels+j]=des[row * width* nChannels + col* nChannels+j];
		}

	}
					
		
}


double randomUniform(double dMinValue,double dMaxValue)
{

    double pRandomValue = (double)(rand()/(double)RAND_MAX);
    pRandomValue = pRandomValue*(dMaxValue-dMinValue)+dMinValue;
    return pRandomValue;
}


void Addinggaussnoise(unsigned char* des, const unsigned char* src, int width, int height, int nChannels,  double SNR)
{
	 int noise_num=height*width*SNR;
	// TODO: Add your command handler code here
	 srand((unsigned)time(NULL));  //�����������
	 for(int y = 0; y < height; y++)
	    {
		    for(int x = 0; x < width; x++)
		    {
			    for(int n=0;n<nChannels;n++)
			    des[y * width * nChannels + x * nChannels + n] = src[y * width * nChannels + x * nChannels + n] ;
		    }
	    }
	 for(int i=0; i<noise_num; i++) 
	{
        int row = (int)(rand()%height);
        int col = (int)(rand()%width);
        for(int j=0;j<nChannels;j++)
		{
			double p;
			p=gaussrand(0,900);
		  
		des[row * width* nChannels + col* nChannels+j]+= p;
		if (des[row * width* nChannels + col* nChannels+j]<0)
			des[row * width* nChannels + col* nChannels+j]=0;
		else if (des[row * width* nChannels + col* nChannels+j]>255)
		{
			des[row * width* nChannels + col* nChannels+j]=255;
		}
		else
			des[row * width* nChannels + col* nChannels+j]=des[row * width* nChannels + col* nChannels+j];
		}

	}
			
}
//��Marsaglia��Bray��1964������ĳ��ó�������ɸ�˹�ֲ���������еķ���
double gaussrand(int M,int D)
{
    static double V1, V2, S;
    static int phase = 0;
    double X;

    if ( phase == 0 ) {
        do {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;
             
            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while(S >= 1 || S == 0);
         
        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);
         
    phase = 1 - phase;
	X=X*D+M;
    
    return(X);
}


void Addingsaltandpeppernoise(unsigned char* des, const unsigned char* src, int width, int height, int nChannels,  double SNR)
{
	int noise_num=(int)height*width*SNR/2;
	int x,y;
	//double* temp = new double[width * src];
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			for(int n=0;n<nChannels;n++)
			des[y * width * nChannels + x * nChannels + n] = src[y * width * nChannels + x * nChannels + n] ;
		}
	}
	for(int i=0; i<noise_num; i++) 
	{
        int row = (int)(rand()%height);
        int col = (int)(rand()%width);
        for(int j=0;j<nChannels;j++)
		{
		des[row * width* nChannels + col* nChannels+j]= 255;
		}

	}
		for(int i=0; i<noise_num; i++) 
	{
        int row = (int)(rand()%height);
        int col = (int)(rand()%width);
        for(int j=0;j<nChannels;j++)
		{
		des[row * width* nChannels + col* nChannels+j]=0;
		}
	}

}

	

//����ð������Գ���ΪN������a���д�С��������
void orderN(double* a, int N)
{
	double temp=0; //��¼�м���ʱ����
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N-i-1;j++)
		{
			if(a[j]>a[j+1])
			{
				temp=a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
		}
	}
}

//Alpha trimmed mean filter
void Alphatrimmedmeanfilter(unsigned char* des, const unsigned char* src, int width, int height, int nChannels)
{
	// TODO: Add your command handler code here
	
			//processing
			int len=9;         //����ģ���СΪ3*3
			double* or=new double[len];     //��ģ����ǿ�ȼ�¼�����д�����
			int d=4;     //����dֵ
			//��uint8��ת��Ϊdouble��
			//filtering
				for(int x=2;x<width-2;x++)
					for(int y=2;y<height-2;y++)
						for(int n=0;n<nChannels;n++)
					    {
						or[0]=src[(y-1)*width*nChannels+(x-1)*nChannels + n];
						or[1]=src[(y-1)*width*nChannels+x*nChannels + n];
						or[2]=src[(y-1)*width*nChannels+(x+1)*nChannels + n];
						or[3]=src[y*width*nChannels+(x-1)*nChannels + n];
						or[4]=src[y*width*nChannels+x*nChannels + n];
						or[5]=src[y*width*nChannels+(x+1)*nChannels + n];
						or[6]=src[(y+1)*width*nChannels+(x-1)*nChannels + n];
						or[7]=src[(y+1)*width*nChannels+x*nChannels + n];
						or[8]=src[(y+1)*width*nChannels+(x+1)*nChannels + n];
					
					    orderN(or,9);           //��С��������
					    double sum=0;         //��¼ʣ��ǿ��֮��
					    for(int k=d/2;k<9-d/2;k++)
					    {
						   sum=sum+or[k];
					    }
					    sum=sum/(9-d);

					
					    des[y*width*nChannels+x*nChannels + n]=int(sum);
					   }
					
}


void Adaptivemedianfilter(unsigned char* des, const unsigned char* src, int width, int height, int nChannels)
{
	// TODO: Add your command handler code here
			int s=11;     //���������ο��С
			int k=0;
			k=(s-1)/2;
			//filtering
			double medtemp=0;
			double maxtemp=0;
			double mintemp=0;
			double pxytemp=0;             //�����˲������е���ʱ����

			for(int i=2;i<width-2;i++)
				for(int j=2;j<height-2;j++)            //���ɨ��
				    for(int n=0;n<nChannels;n++)
					{
			            int len=7;
			            while(len<=s)
			            {
				            double* or=new double[len*len];         //��̬����һά���д��
						    int len2=0;
						    len2=(len-1)/2;
						    for(int x=0;x<len;x++)            //�����ο��е����ݶ���or��������
							    for(int y=0;y<len;y++)
							    {
								    or[x*len+y]=src[(i-len2+x)*nChannels+(j-len2+y)*nChannels*width+n];
							    }
						    orderN(or,len*len);
						    medtemp=or[(len*len-1)/2];
						    maxtemp=or[len*len-1];
						    mintemp=or[0];
						    pxytemp=src[i*nChannels+j*nChannels*width+n];
						    if((maxtemp>medtemp)&(mintemp<medtemp))
						    {
							if((maxtemp>pxytemp)&(mintemp<pxytemp))
							{
								des[i*nChannels+j*nChannels*width+n]=int(pxytemp);
							}
							else
							{
								des[i*nChannels+j*nChannels*width+n]=int(medtemp);
							}
							break;
						}
						else
						{
							len=len+2;
						}
						delete []or;
					}
					if(len>s)
					{
						des[i*nChannels+j*nChannels*width+n]=int(medtemp);
					}
				}
				
}
//����˵��:
//LPSTR lpDIBBits:ָ��ԴDIBͼ��ָ��
//LONG  width:Դͼ���ȣ���������
//LONG  height:Դͼ��߶ȣ���������
//����˵��:
//�ú���������DIBͼ�����ά���˲���ԭ������
/*
bool Wienerfilter (unsigned char* des, const unsigned char* src, int width, int height, int nChannels)
{


	unsigned char pixel;
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	//������FFT������
	double *fftSrc,*fftKernel,*fftNoise;
	double a,b,c,d,e,f,multi;
	//��άFFT�ĳ��ȺͿ��
	unsigned long nn[3];
	//ͼ���һ������
	double MaxNum;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = width * nChannels;
	
	double dPower = log((double)lLineBytes)/log(2.0);
	if(dPower != (int) dPower)
	{
		return false;
	}
	dPower = log((double)height)/log(2.0);
	if(dPower != (int) dPower)
	{
		return false;
	}
	
	fftSrc = new double [height*lLineBytes*2+1];
	fftKernel = new double [height*lLineBytes*2+1];
	fftNoise = new double [height*lLineBytes*2+1];
	nn[1] = height;
	nn[2] = lLineBytes;
	for ( int j = 0;j < height ;j++)
	{
		for(int i = 0;i < lLineBytes ;i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			src = (char *)lpDIBBits + lLineBytes * j + i;
			
			pixel = (unsigned char)*src;
			
			fftSrc[(2*lLineBytes)*j + 2*i + 1] = (double)pixel;
			fftSrc[(2*lLineBytes)*j + 2*i + 2] = 0.0;
			
			if(i < 5 && j == 0)
			{
				fftKernel[(2*lLineBytes)*j + 2*i + 1] = 1/5.0;
			}
			else
			{
				fftKernel[(2*lLineBytes)*j + 2*i + 1] = 0.0;
			}
			fftKernel[(2*lLineBytes)*j + 2*i + 2] = 0.0;
			if ( i + j == ((int)((i+j)/8))*8)
			{
				fftNoise [(2*lLineBytes)*j + 2*i + 1]= -16.0;
			}
			else
			{
				fftNoise [(2*lLineBytes)*j + 2*i + 1]= 0.0;
			}
			fftNoise[(2*lLineBytes)*j + 2*i + 2] = 0.0;
		}
	}
	
	srand((unsigned)time(NULL));
	//��Դͼ�����FFT
	fourn(fftSrc,nn,2,1);
	//�Ծ����ͼ�����FFT
	fourn(fftKernel,nn,2,1);
	//������ͼ�����FFT
	fourn(fftNoise,nn,2,1);
	
	for (i = 1;i <height*lLineBytes*2;i+=2)
	{
		a = fftSrc[i];
		b = fftSrc[i+1];
		c = fftKernel[i];
		d = fftKernel[i+1];
		e = fftNoise[i];
		f = fftNoise[i+1];
		multi = (a*a + b*b)/(a*a + b*b - e*e - f*f);
		if (c*c + d*d > 1e-3)
		{
			fftSrc[i] = ( a*c + b*d ) / ( c*c + d*d ) / multi;
			fftSrc[i+1] = ( b*c - a*d ) / ( c*c + d*d )/multi;
		}
	}
	
	//�Խ��ͼ����з�FFT
	fourn(fftSrc,nn,2,-1);
	
	//ȷ����һ������
	MaxNum = 0.0;
	for (j = 0;j < height ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			fftSrc[(2*lLineBytes)*j + 2*i + 1] = 
				sqrt(fftSrc[(2*lLineBytes)*j + 2*i + 1] * fftSrc[(2*lLineBytes)*j + 2*i + 1]\
				+fftSrc[(2*lLineBytes)*j + 2*i + 2] * fftSrc[(2*lLineBytes)*j + 2*i + 2]);
			if( MaxNum < fftSrc[(2*lLineBytes)*j + 2*i + 1])
				MaxNum = fftSrc[(2*lLineBytes)*j + 2*i + 1];
		}
	}
	
	//ת��Ϊͼ��
	for (j = 0;j < height ;j++)
	{
		for(i = 0;i < lLineBytes ;i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			src = (char *)lpDIBBits + lLineBytes * j + i;
			
			*src = (unsigned char) (fftSrc[(2*lLineBytes)*j + 2*i + 1]*255.0/MaxNum );
		}
	}
	
	delete fftSrc;
	delete fftKernel;
	delete fftNoise;
	// ����
	return true;
}
*/




					