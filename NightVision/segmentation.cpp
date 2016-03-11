#include"Backup.h"
#include<math.h>
/**************************************************************
�������ܣ���ͼ��rgb�ռ��ɫ��������ȡ
���������Դͼ��src��Ŀ��ͼ��des��ͼ�����width,height,nChannels��
���������Ŀ��ͼ��
**************************************************************/
void rgb_seg(unsigned char* des, const unsigned char* src, int width, int height, int nChannels)
{	
	printf("%d,%d,%d,",nChannels,width,height);
	unsigned char* ocl = new unsigned char[width * height * nChannels];
	for(int y = 0; y < height; y++)
	    {
		    for(int x = 0; x < width; x++)
		    {	    
			int img_B= src[y * width * nChannels + x * nChannels ] ;  		
			int img_G= src[y * width * nChannels + x * nChannels + 1] ;  		
			int img_R= src[y * width * nChannels + x * nChannels + 2] ;  
			    if((img_R>140)&&(img_G<70)&&(img_B<70))		//�򵥵���ֵ��ȡ
				    for(int n=0;n<nChannels;n++)
			        des[y * width * nChannels + x * nChannels + n] = src[y * width * nChannels + x * nChannels + n] ;
			    else
				   for(int n=0;n<nChannels;n++)
				   des[y * width * nChannels + x * nChannels + n] =255;
			
		    }
	    }
	    ImageDilation(ocl, des,  width, height, nChannels,7);//��7*7��ģ���������
	    ImageErosion(des, ocl, width, height, nChannels,13); //��13*13��ģ����и�ʴ	
	    ImageDilation(ocl, des, width, height, nChannels,13);//��13*13��ģ���������
	    ImageReturn(des,ocl,src, width, height, nChannels,10);//��10*10��ģ�廹ԭ����
}
/**************************************************************
�������ܣ���ͼ��hsi�ռ��ɫ��������ȡ
���������Դͼ��src��Ŀ��ͼ��des��ͼ�����width,height,nChannels��
���������Ŀ��ͼ��
**************************************************************/
void hsi_seg(unsigned char* des, const unsigned char* src, int width, int height, int nChannels)
{	
	printf("%d,%d,%d,",nChannels,width,height);
	unsigned char* ocl = new unsigned char[width * height * nChannels];
	unsigned char* hsi = new unsigned char[width * height * nChannels];
	rgb_hsi(hsi,src, width,  height,  nChannels);//hsi������ȡ
	for(int y = 0; y < height; y++)
	    {
		    for(int x = 0; x < width; x++)
		    {	    
			int img_H= hsi[y * width * nChannels + x * nChannels ] ;  		
			int img_S= hsi[y * width * nChannels + x * nChannels + 1] ;  		
			int img_I= hsi[y * width * nChannels + x * nChannels + 2] ;  
			    if((img_H<104)&&(img_H>102)&&(img_I>40)&&(img_S>60))
				{
					//printf("%d, ",img_S);
				    for(int n=0;n<nChannels;n++)
			        des[y * width * nChannels + x * nChannels + n] = src[y * width * nChannels + x * nChannels + n] ;
				}
			    else
				   for(int n=0;n<nChannels;n++)
				   des[y * width * nChannels + x * nChannels + n] =255;
			
		    }
	    }
		ImageDilation(ocl, des,  width, height, nChannels,3);//����18*18��ģ������
		//memcpy(des, ocl, nChannels*width*height);
	    ImageErosion(des, ocl, width, height, nChannels,5);		
		Imgfill( ocl, des, width,  height, nChannels);
		memcpy(des, ocl, nChannels*width*height);
		ImageErosion(ocl, des, width, height, nChannels,8);	
		memcpy(des, ocl, nChannels*width*height);
        ImageDilation(des, ocl, width, height, nChannels,5);
		memcpy(ocl, des,nChannels*width*height);
		ImageReturn(des,ocl,src, width, height, nChannels,8);
}
/**************************************************************
�������ܣ���ͼ�񴰻�����Ӱ����ֱ��ͼ����
���������Դͼ��src��Ŀ��ͼ��des��ͼ�����width,height,nChannels��
���������Ŀ��ͼ��
**************************************************************/
void win_enhance(unsigned char* des, const unsigned char* src, int width, int height, int nChannels)
{	
	printf("%d,%d,%d,",nChannels,width,height);
	unsigned char* ocl = new unsigned char[width * height * nChannels];
	unsigned char* flag = new unsigned char[width * height * nChannels];
	int r[256] = {0}, g[256] = {0}, b[256] = {0};
	int sum = 0;		//ͼ�������ص���ۺ�
	for(int y = 0; y < height; y++)
	    {
		    for(int x = 0; x < width; x++)
		    {	    
			int B= src[y * width * nChannels + x * nChannels ] ;  		
			int G= src[y * width * nChannels + x * nChannels + 1] ;  		
			int R= src[y * width * nChannels + x * nChannels + 2] ;  
			    if((R+G+B<120)&&(y < height-240)&&(y >250))
				{
					//printf("%d, ",img_S);
				    for(int n=0;n<nChannels;n++)
					{
			        des[y * width * nChannels + x * nChannels + n] = src[y * width * nChannels + x * nChannels + n] ;
					
					}
				}
			    else
				{
				   for(int n=0;n<nChannels;n++)
				   {
				   des[y * width * nChannels + x * nChannels + n] =255;
				   }
				}
			
		    }
	    }
	 ImageErosion(ocl, des, width, height, nChannels,9);	
	 ImageDilation(des, ocl, width, height, nChannels,10);
	 memcpy(ocl, des,nChannels*width*height);
     for(int y=0; y<height; y++)
		for(int x=0; x<width; x++)
		{
			if( des[y * width * nChannels + x * nChannels]!=255)
			{
				sum=sum+1;
				flag[y * width * nChannels + x * nChannels ]= 1;
			    b[des[y * width * nChannels + x * nChannels + 0]]++;
			    g[des[y * width * nChannels + x * nChannels + 1]]++;
			    r[des[y * width * nChannels + x * nChannels + 2]]++;
			}
			else
			{
                flag[y * width * nChannels + x * nChannels ]= 0;
			}
		}
	double val[3] = {0};
	for(int y=0; y<255; y++)
	{
		val[0] += b[y];
		val[1] += g[y];
		val[2] += r[y];
		b[y] = val[0]*255/sum;
		g[y] = val[1]*255/sum;
		r[y] = val[2]*255/sum;
	}
	for(int y=0; y<height; y++)
		for(int x=0; x<width; x++)
		{
			if(flag[y * width * nChannels + x * nChannels]==1)
			{
			    ocl[y * width * nChannels + x * nChannels + 0] = b[des[y * width * nChannels + x * nChannels + 0]] ;
				ocl[y * width * nChannels + x * nChannels + 1] = g[des[y * width * nChannels + x * nChannels + 1]] ;
				ocl[y * width * nChannels + x * nChannels + 2] = r[des[y * width * nChannels + x * nChannels + 2]] ;
			}
		}
		//memcpy(des, ocl, nChannels*width*height);
	for(int y=0; y<height; y++)
		for(int x=0; x<width; x++)
		{
			if(flag[y * width * nChannels + x * nChannels ]==1)
			{
			    for(int n=0;n<nChannels;n++)
			    des[y * width * nChannels + x * nChannels + n] = ocl[y * width * nChannels + x * nChannels + n] ;
			}
			else
			{
			    for(int n=0;n<nChannels;n++)
			    des[y * width * nChannels + x * nChannels + n] = src[y * width * nChannels + x * nChannels + n] ;
			}
		}
}
/**************************************************************
�������ܣ���ͼ�����M*Mģ��ĸ�ʴ
���������Դͼ��src��Ŀ��ͼ��des��ͼ�����width,height,nChannels����ʴ�߳�M
���������Ŀ��ͼ��
**************************************************************/
void ImageErosion(unsigned char* des, const unsigned char* src, int width, int height, int nChannels,int M)
{
	memcpy(des, src, nChannels*width*height);
	int m,p,k=0,q=0;
	for(int y = M+1; y < height-M-1; y++)
	    {
		    for(int x = M+1; x < width-M-1; x++)
                {
                    if((src[y * width * nChannels + x * nChannels]==0))
                    {
						k=k+1;
						for(m=-M;m<=M;m++)
                        {
                        for(p=-M;p<=M;p++)
                            {  
								if((src[(y+m) * width * nChannels + (x+p) * nChannels])!=0)
								{
									for(int n=0;n<nChannels;n++)
									des[y * width * nChannels + x * nChannels + n]=255;           
								}
                             }
                        }
					}
					else
					{
						q=q+1;
					}
             }
       }
	//printf("E%d   %d",k,q);
}

/**************************************************************
�������ܣ���ͼ�����M*Mģ�������
���������Դͼ��src��Ŀ��ͼ��des��ͼ�����width,height,nChannels�����ͱ߳�M
���������Ŀ��ͼ��
**************************************************************/
void ImageDilation(unsigned char* des, const unsigned char* src, int width, int height,int nChannels,int M)
{
	    int m,p,k=0,q=0;
		memcpy(des, src, nChannels*width*height);
	for(int y = M+1; y < height-M-1; y++)
	    {
		    for(int x = M+1; x < width-M-1; x++)
                {
                    if((src[y * width * nChannels + x * nChannels ]==0))
                    {
						k=k+1;
						for(m=-M;m<=M;m++)
                        {
                        for(p=-M;p<=M;p++)
                            {  
							  for(int n=0;n<nChannels;n++)
                              des[(y+m) * width * nChannels + (x+p) * nChannels + n]=src[y * width * nChannels + x * nChannels + n];           
                             }
                        }
					}
					else
					{
						q=q+1;
					}
             }
       }
	//printf("D%d  %d",k,q);
}
/**************************************************************
�������ܣ���ͼ�����M*Mģ���ԭͼ��ԭ
���������Դͼ��src��Ŀ��ͼ��des��ͼ�����width,height,nChannels����ԭ�߳�M
���������Ŀ��ͼ��
**************************************************************/
void ImageReturn(unsigned char* des, const unsigned char* ocl, const unsigned char* src, int width, int height, int nChannels,int M)
{
	//memcpy(des, ocl, nChannels*width*height);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			for(int n = 0; n < nChannels; n++)
			{
				des[y * width * nChannels + x * nChannels + n ] = 255;
			}
			
		}
	}
	int m,p,k=0,q=0;
	for(int y = M+1; y < height-M-1; y++)
	    {
		    for(int x = M+1; x < width-M-1; x++)
                {
                    if((ocl[y * width * nChannels + x * nChannels + 2]!=255))
                    {
						k=k+1;
						for(m=-M;m<=M;m++)
                        {
                        for(p=-M;p<=M;p++)
                            {  
							int B= src[(y+m) * width * nChannels + (x+p) * nChannels ] ;  		
			                int G= src[(y+m) * width * nChannels + (x+p) * nChannels + 1] ;  		
			                int R= src[(y+m) * width * nChannels + (x+p) * nChannels + 2] ;  
			                 if(R>90)	
							 {
							  for(int n=0;n<nChannels;n++)
                              des[(y+m) * width * nChannels + (x+p) * nChannels + n]=src[(y+m) * width * nChannels + (x+p) * nChannels + n];    
							 //����ԭ������Դͼ��������
							 }
                             }
                        }
					}
					else
					{
						q=q+1;
					}
             }
       }
	printf("D%d  %d",k,q);
}
/**************************************************************
�������ܣ���ͼ�������RGB�ռ䵽HSI�ռ��ת��
���������Դͼ��src��Ŀ��ͼ��des��ͼ�����width,height,nChannels��
���������Ŀ��ͼ��
**************************************************************/
void rgb_hsi(unsigned char* des, const unsigned char* src, int width, int height, int nChannels)
{
	int k=0;

    for(int y=0; y<height; y++)
		{
			for (int x=0; x<width; x++)
			{				
			double B= src[y * width * nChannels + x * nChannels ] ;  		
			double G= src[y * width * nChannels + x * nChannels + 1] ;  		
			double R= src[y * width * nChannels + x * nChannels + 2] ;  
			double H,S,I=0;//Hɫ����S���Ͷȣ����ȣ���Iǿ��
			double mx,mi;
			mx=max(max(R,G),B);
			mi=min(min(R,G),B);	
			if (mx==mi)     //���RGB���
			{
				k=k+1;
				H=0;    //H����Ϊ0
				S=0;    //S����Ϊ0
				I=mi;
			}
			else 
			{
				if (B<=G) 
				{
				H=acos((0.5*((R-B)+(R-G)))/(sqrt(1.0*((R-G)*(R-G)+(R-B)*(G-B)))));
				}
				else 
				{
				H=360-acos((0.5*((R-B)+(R-G)))/(sqrt(1.0*((R-G)*(R-G)+(R-B)*(G-B)))));
				}
				S=(3*mi)/(R+B+G);
				S=1-S;
				I=(R+B+G)/3;
			}
			des[y * width * nChannels + x * nChannels + 0]= int(H);   
			des[y * width * nChannels + x * nChannels + 1]= int (S*255);    
			des[y * width * nChannels + x * nChannels + 2]=int(I);
			}
	}
	printf("k %d",k);
}
/**************************************************************
�������ܣ���ͼ�������HSI�ռ䵽RGB�ռ��ת��
���������Դͼ��src��Ŀ��ͼ��des��ͼ�����width,height,nChannels��
���������Ŀ��ͼ��
**************************************************************/
void hsi_rgb(unsigned char* des, const unsigned char* src, int width, int height, int nChannels)
{
	int k=0;

    for(int y=0; y<height; y++)
		{
			for (int x=0; x<width; x++)
			{				
			double H= src[y * width * nChannels + x * nChannels ] ;  	
				//printf("H%d",H);
			double S= src[y * width * nChannels + x * nChannels + 1]/255 ;  
			//printf("S%d",S);
			double I= src[y * width * nChannels + x * nChannels + 2] ;  
			//printf("I%d",I);
			double R,G,B;//Hɫ����S���Ͷȣ����ȣ���Iǿ��
			if((H>=0)&&(H<120))
			{
			    B=I*(1-S);
			    R=I*(1+S*cos(H)/cos(60-H));
			    G=3*I-(R+B);
			}
			else if((H>=120)&&(H<240))
			{
			    H=H-120;
			    R=I*(1-S);
			    G=I*(1+S*cos(H)/cos(60-H));
			    B=3*I-(R+G);
			}
			else
			{
				H=H-240;
			    G=I*(1-S);
			    B=I*(1+S*cos(H)/cos(60-H));
			    R=3*I-(B+G);
			}
			des[y * width * nChannels + x * nChannels + 0]= int(B);   
			//printf("b%d",B);
			des[y * width * nChannels + x * nChannels + 1]= int(G);   
			//printf("G%d",G);
			des[y * width * nChannels + x * nChannels + 2]= int(R);
			//printf("R%d",R);
			 for(int n=0;n<nChannels;n++)
			 {
             int val=des[y * width * nChannels + x * nChannels + n];
			 if(val<0)
				 des[y * width * nChannels + x * nChannels + n]=0;
			 else if(val>255)
				 des[y * width * nChannels + x * nChannels + n]=255;
			 else
				 des[y * width * nChannels + x * nChannels + n]=des[y * width * nChannels + x * nChannels + n];
			 }
			}
	}
	printf("k %d",k);
}
void Imgfill(unsigned char* des, const unsigned char* src, int width, int height, int nChannels)
{
	memcpy(des, src, nChannels*width*height);
	// TODO: �ڴ���������������
	//��¼����е�����λ��
	int left_X=0;
	//��¼����е�����λ��
    int right_X=0;
	//��¼����е�����λ��
	int top_Y=0;
	//��¼����е�����λ��
	int bottom_Y=0;
	    //�������
	for(int y=0;y<height;y++)
	{
		for(int x=0;x<width-61;x++)
		{	
			int sl=src[y * width * nChannels + x * nChannels + 2];
			if(sl!=255)
			{
				left_X=x;//�ҵ�����ߵİ�ɫ��
				for(int k=60;k>0;k--)
				{
					int sr=src[y * width * nChannels + (x+k) * nChannels + 2];
					if(sr!=255)
					{
						right_X=x+k;
						break; //����Ѱ�����ұߵ��ѭ��
					}
				}
				//��left_X��right_X֮��ĵ�����Ϊ��ɫ
				for(int x=left_X;x<right_X-1;x++)
				{
					for(int n=0;n<nChannels;n++)
                        des[y * width * nChannels + x * nChannels + n]=0;  
				}
				//x=right_X;
				//printf("L%d ",left_X);
			}
		}
	}

	//�������
	for(int x=0;x<width;x++)
	{
		for(int y=0;y<height-61;y++)
		{	
			int st=src[y * width * nChannels + x * nChannels + 2];
			if(255!=st)
			{
				top_Y=y;//y�����ϱߵķǰ�ɫ��
				for(int m=10;m>0;m--)
				{
					int sb=src[(y+m) * width * nChannels + x * nChannels + 2];
					if(sb!=255)
					{
						bottom_Y=m+y;
						break; //����Ѱ�����±ߵ��ѭ��
					}
				}
				//��top_Y��bottom_Y֮��ĵ�����Ϊ��ɫ
				for(int y=top_Y;y<bottom_Y-1;y++)
				{
					for(int n=0;n<nChannels;n++)
                        des[y * width * nChannels + x * nChannels + n]=0;  
				}
			}	
		}
	}
}