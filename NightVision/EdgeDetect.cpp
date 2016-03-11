#include"EdgeDetect.h"
void rgbToGray(unsigned char* gray, const unsigned char* src, int width, int height, int nChannels)
{
	if (nChannels == 1)
	{
		memcpy(gray, src, nChannels*width*height);
	}
	else
    {
		for(int y = 0; y < height; y++)
	    {
		    for(int x = 0; x < width; x++)
		    {	    
			double B= src[y * width * nChannels + x * nChannels ] ;  		
			double G= src[y * width * nChannels + x * nChannels + 1] ;  		
			double R= src[y * width * nChannels + x * nChannels + 2] ;  
		    gray[y * width + x]=R*0.299 + G*0.587 + B*0.114; //ת��Ϊ�Ҷ�ͼ
			
			}
		}
    }
	//cvShowImage("GRAY",gray);
	//printf("%d",gray->nChannels);
}
void thresh (unsigned char* des,int width,int height)
{
	int histo[256];
	int i,j,t;
	
/* Compute a grey level histogram */
	for (i=0; i<256; i++) 
		histo[i] = 0;
	for (int y=1; y<height-1; y++)
		for (int x=1; x<width-1; x++) 
	  {
	    histo[des[y * width + x]]++;
	  }
	
/* Threshold at the middle of the occupied levels */
	i = 255; 
	while (histo[i] == 0) 
		i--;
	j = 0;
	while (histo[j] == 0) 
		j++;
	t = (i+j)/2;

/* Apply the threshold */
	for (int y=0; y<height; y++)
		for (int x=0; x<width; x++) 
	    if (des[y * width + x] >= t) 
			des[y * width + x] = 0;
	    else 
			des[y * width + x] = 255;
}

/*	Apply a Sobel edge mask to the image X	*/

void sobel (unsigned char* des, const unsigned char* src, int width, int height)
{
	for (int y=0; y<height; y++)
		for (int x=0; x<width; x++) 
			des[y * width + x]=255;

/* Now compute the convolution, scaling */
	for (int y=1; y<height-1; y++)
		for (int x=1; x<width-1; x++) 
		{
	    double n = (src[(y+1)*width+x-1]+2*src[(y+1)*width+x]+src[(y+1)*width+x+1]) -
	           (src[(y-1)*width+x-1]+2*src[(y-1)*width+x]+src[(y-1)*width+x+1]);
	    double m = (src[(y-1)*width+x+1]+2*src[y*width+x+1]+src[(y+1)*width+x+1])-
	           (src[(y-1)*width+x-1]+2*src[y*width+x-1]+src[(y+1)*width+x-1]);
	    double k = (int)( sqrt( (double)(n*n + m*m) )/4.0 );
	    des[y * width + x] = k;
		}
		thresh (des, width,height);
}
void roberts(unsigned char* des, const unsigned char* src, int width, int height)
{
	for (int y=0; y<height; y++)
		for (int x=0; x<width; x++) 
			des[y * width + x]=255;

/* Now compute the convolution, scaling */
	for (int y=1; y<height-1; y++)
		for (int x=1; x<width-1; x++) 
		{
	    double n = src[y*width+x] - src[(y+1)*width+x+1];
	    double m = src[(y+1)*width+x] - src[y*width+x+1];
	    double k = abs(m)+abs(n);
	    des[y * width + x] = k;
		}
	thresh (des, width,height);
}
void kirsch(unsigned char* des, const unsigned char* src, int width, int height) 
{
	// TODO: Add your command handler code here
                //��ʾ��ֵ
	long int i,j,Ns;
	static int nWeight[8][3][3];//��һ����̬�������鸳��ֵ��ģ��
		
	double dGrad[8];

	int nTmp[3][3],xx,yy;//ÿ���ص������ֵ

	nWeight[0][0][0] = -1 ;   
	nWeight[0][0][1] =  0 ;   
	nWeight[0][0][2] =  1 ;   
	nWeight[0][1][0] = -2 ;   
	nWeight[0][1][1] =  0 ;   
	nWeight[0][1][2] =  2 ;   
	nWeight[0][2][0] = -1 ;   
	nWeight[0][2][1] =  0 ;   
	nWeight[0][2][2] =  1 ;   
		
	nWeight[1][0][0] = -1 ;   
	nWeight[1][0][1] = -2 ;   
	nWeight[1][0][2] = -1 ;   
	nWeight[1][1][0] =  0 ;   
	nWeight[1][1][1] =  0 ;   
	nWeight[1][1][2] =  0 ;   
	nWeight[1][2][0] =  1 ;   
	nWeight[1][2][1] =  2 ;   
	nWeight[1][2][2] =  1 ;//��������??? �Ѹĳ�8������ģ���ֵ
	
    nWeight[2][0][0] =  0 ;   
	nWeight[2][0][1] = -1 ;   
	nWeight[2][0][2] = -2 ;   
	nWeight[2][1][0] =  1 ;   
	nWeight[2][1][1] =  0 ;   
	nWeight[2][1][2] = -1 ;   
	nWeight[2][2][0] =  2 ;   
	nWeight[2][2][1] =  1 ;   
	nWeight[2][2][2] =  0 ;

    nWeight[3][0][0] =  1 ;   
	nWeight[3][0][1] =  0 ;   
	nWeight[3][0][2] = -1 ;   
	nWeight[3][1][0] =  2 ;   
	nWeight[3][1][1] =  0 ;   
	nWeight[3][1][2] = -2 ;   
	nWeight[3][2][0] =  1 ;   
	nWeight[3][2][1] =  0 ;   
	nWeight[3][2][2] = -1 ;

    nWeight[4][0][0] =  2 ;   
	nWeight[4][0][1] =  1 ;   
	nWeight[4][0][2] =  0 ;   
	nWeight[4][1][0] =  1 ;   
	nWeight[4][1][1] =  0 ;   
	nWeight[4][1][2] = -1 ;   
	nWeight[4][2][0] =  0 ;   
	nWeight[4][2][1] = -1 ;   
	nWeight[4][2][2] = -2 ;

	nWeight[5][0][0] =  1 ;   
	nWeight[5][0][1] =  2 ;   
	nWeight[5][0][2] =  1 ;   
	nWeight[5][1][0] =  0 ;   
	nWeight[5][1][1] =  0 ;   
	nWeight[5][1][2] =  0 ;   
	nWeight[5][2][0] = -1 ;   
	nWeight[5][2][1] = -2 ;   
	nWeight[5][2][2] = -1 ;

    nWeight[6][0][0] =  0 ;   
	nWeight[6][0][1] =  1 ;   
	nWeight[6][0][2] =  2 ;   
	nWeight[6][1][0] = -1 ;   
	nWeight[6][1][1] =  0 ;   
	nWeight[6][1][2] =  1 ;   
	nWeight[6][2][0] = -2 ;   
	nWeight[6][2][1] = -1 ;   
	nWeight[6][2][2] =  0 ;

    nWeight[7][0][0] = -2 ;   
	nWeight[7][0][1] = -1 ;   
	nWeight[7][0][2] =  0 ;   
	nWeight[7][1][0] = -1 ;   
	nWeight[7][1][1] =  0 ;   
	nWeight[7][1][2] =  1 ;   
	nWeight[7][2][0] =  0 ;   
	nWeight[7][2][1] = -1 ;   
	nWeight[7][2][2] =  2 ;


//ע�⣺ÿ�е��ֽ���������4��������!!!�Ȳ�����
	Ns=height*width;
    unsigned char* kk = new unsigned char[width * height];                     //��ʼ�任  initiion 
	for(i=0; i<height ; i++ )
	//if(i==0)//tt change at 05.05.16
		for(j=0 ; j<width ; j++ )
		{
			des[i*width + j]=0;//*(pdGrad+y*nWidth+x)
		}

    for(i=1; i<height-1 ; i++ )
	{
			for(j=1 ; j<width-1 ; j++ )
			{ 
				dGrad[0] = 0 ;
				dGrad[1] = 0 ;
                dGrad[2] = 0 ;
				dGrad[3] = 0 ;
				dGrad[4] = 0 ;
				dGrad[5] = 0 ;
				dGrad[6] = 0 ;
				dGrad[7] = 0 ;
				
				// sobel������Ҫ�ĸ�������ֵ
					// ģ���һ��
				nTmp[0][0] = src[(i-1)*width + j - 1 ]; 
				nTmp[0][1] = src[(i-1)*width + j     ] ; 
				nTmp[0][2] = src[(i-1)*width + j + 1 ] ; 
				
				// ģ��ڶ���
				nTmp[1][0] = src[i*width + j - 1 ] ; 
				nTmp[1][1] = src[i*width + j     ] ; 
				nTmp[1][2] = src[i*width + j + 1 ] ; 
				
				// ģ�������
				nTmp[2][0] = src[(i+1)*width + j - 1 ] ; 
				nTmp[2][1] = src[(i+1)*width + j     ] ; 
				nTmp[2][2] = src[(i+1)*width + j + 1 ] ; 
				
				// �����ݶ�
				for(yy=0; yy<3; yy++)
					for(xx=0; xx<3; xx++)
					{
						dGrad[0] += nTmp[yy][xx] * nWeight[0][yy][xx] ;
						dGrad[1] += nTmp[yy][xx] * nWeight[1][yy][xx] ;
						dGrad[2] += nTmp[yy][xx] * nWeight[2][yy][xx] ;
						dGrad[3] += nTmp[yy][xx] * nWeight[3][yy][xx] ;
						dGrad[4] += nTmp[yy][xx] * nWeight[4][yy][xx] ;
						dGrad[5] += nTmp[yy][xx] * nWeight[5][yy][xx] ;
						dGrad[6] += nTmp[yy][xx] * nWeight[6][yy][xx] ;
						dGrad[7] += nTmp[yy][xx] * nWeight[7][yy][xx] ;
					}
				for (xx=1;xx<8;xx++)
				{
                    if (dGrad[xx]>dGrad[0])
                       dGrad[0]=dGrad[xx];
				}
                des[i*width + j]=dGrad[0];// �ݶ�ֵд��src[i]					
			} 
	}

	//�趨��ֵ
  int th[5120],newth[5120],shuN,newN,flagyuzhi;//winframe=32,ii,jj,initpos;
  double thk,kmin,mvalue[8];
  shuN=0;
  thk=0.5;
  for (i=0;i<Ns;i++)//ÿ���ÿ����
  {
      if ((i>=width) && (i<(Ns-width)))//���ǷǱ߽�㣬�򡭡�
	  {
         if ((i%width!=0) && ((i+1)%width!=0))
		 {
			//ÿ�����任��������kirs(c)h����
            mvalue[0]=fabs(double(des[i+1]+des[i+width+1]+des[i+width]+\
				      des[i+width-1]+des[i-1]-des[i-width-1]-\
					  des[i-width]-des[i-width+1]));
            mvalue[1]=fabs(double(des[i+width+1]+des[i+width]+\
				      des[i+width-1]+des[i-1]+des[i-width-1]-\
					  des[i-width]-des[i-width+1]-des[i+1]));
            mvalue[2]=fabs(double(des[i+width]+des[i+width-1]+des[i-1]+\
				      des[i-width-1]+des[i-width]-\
					  des[i-width+1]-des[i+1]-des[i+width+1]));
            mvalue[3]=fabs(double(des[i+width-1]+des[i-1]+\
				      des[i-width-1]+des[i-width]+\
					  des[i-width+1]-des[i+1]-des[i+width+1]-\
					  des[i+width]));
            mvalue[4]=fabs(double(des[i-1]+des[i-width-1]+\
				      des[i-width]+des[i-width+1]+des[i+1]-\
					  des[i+width+1]-des[i+width]-\
					  des[i+width-1]));
            mvalue[5]=fabs(double(des[i-width-1]+des[i-width]+\
					  des[i-width+1]+des[i+1]+des[i+width+1]-\
					  des[i+width]-des[i+width-1]-des[i-1]));
            mvalue[6]=fabs(double(des[i-width]+des[i-width+1]+des[i+1]+\
					  des[i+width+1]+des[i+width]-\
					  des[i+width-1]-des[i-1]-des[i-width-1]));
            mvalue[7]=fabs(double(des[i-width+1]+des[i+1]+des[i+width+1]+\
					  des[i+width]+des[i+width-1]-\
					  des[i-1]-des[i-width-1]-des[i-width]));
            for (j=1;j<8;j++)  //�Ƚϵó�����,mvalue[0]Ϊ���
			{
               if (mvalue[0]<mvalue[j])
				   mvalue[0]=mvalue[j];
			}
            kk[i]=max(1,mvalue[0]/15);
			if (shuN==0)
			   kmin=kk[i];
			if (kk[i]>thk)
			{
               th[shuN]=i;
			   kmin=min(kmin,kk[i]);
			   shuN++;
			   if (shuN>=5*height)//������5*H���㣬������ȷ��
			   {
				  //AfxMessageBox("lll");
                  thk=kmin;
				  newN=0;
				  for (j=0;j<shuN;j++)
				  {
                      if (kk[th[j]]>thk)
					  {
                         if (newN==0)
						    kmin=kk[th[j]];
						 newth[newN]=th[j];
						 kmin=min(kmin,kk[th[j]]);
						 newN++;
					  }
					  //else des[th[j]]=0;
				  }
                  for (j=0;j<5120;j++)
				  {
                      th[j]=newth[j]; 
				  }
				  shuN=newN;
			   }//����ȷ����
			}
            //�Ǳ߽��ÿ��任����
         }
	  }

  }//һ�����

  for (i=0;i<Ns;i++)//ÿ���ÿ����
  {
	  if (des[i]<thk)
	     des[i]=0;	
  }
  	thresh (des, width,height);
//�˵���������   
}
// ���������   
// ����:   
// 1. pBinary: �����ֵͼ�񣬵�ͨ����λ��IPL_DEPTH_8U��  
// 2. dAreaThre: �����ֵ�������������С�ڵ���dAreaThreʱ��������䡣   
void FillInternalContours(IplImage *pBinary, double dAreaThre)   
{   
    double dConArea;   
    CvSeq *pContour = NULL;   
    CvSeq *pConInner = NULL;   
    CvMemStorage *pStorage = NULL;   
    // ִ������   
    if (pBinary)   
    {   
        // ������������   
        pStorage = cvCreateMemStorage(0);   
        cvFindContours(pBinary, pStorage, &pContour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);   
        // �����������   
        cvDrawContours(pBinary, pContour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 2, CV_FILLED, 8, cvPoint(0, 0));  
        // ������ѭ��   
        for (; pContour != NULL; pContour = pContour->h_next)   
        {   
            // ������ѭ��   
            for (pConInner = pContour->v_next; pConInner != NULL; pConInner = pConInner->h_next)   
            {   
                // ���������   
                dConArea = fabs(cvContourArea(pConInner, CV_WHOLE_SEQ));   
                if (dConArea <= dAreaThre)   
                {   
                    cvDrawContours(pBinary, pConInner, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 0, CV_FILLED, 8, cvPoint(0, 0));  
                }   
            }   
        }   
        cvReleaseMemStorage(&pStorage);   
        pStorage = NULL;   
    }   
}   
	/*cvCvtColor(gray,color_dst,CV_GRAY2RGB); 

#if 0
        lines = cvHoughLines2( gray, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 80, 0, 0 );//��׼����任����������Ϊ0������line_storage���ڴ�ռ䣬���Է���һ��CvSeq���нṹ��ָ��

        for( int i = 0; i < lines->total; i++ )
        {
            float* line = (float*)cvGetSeqElem(lines,i);//��GetSeqElem�õ�ֱ��
            float rho = line[0];
            float theta = line[1];//����SHT��MSHT(��׼�任)����line[0],line[1]��rho(��������ص�λ�ľ��뾫��)��theta(���Ȳ����ĽǶȾ���)
            CvPoint pt1, pt2;
            double a = cos(theta), b = sin(theta);
            if( fabs(a) < 0.001 )
            {
                pt1.x = pt2.x = cvRound(rho);
                pt1.y = 0;
                pt2.y = color_dst->height;
            }
            else if( fabs(b) < 0.001 )
            {
                pt1.y = pt2.y = cvRound(rho);
                pt1.x = 0;
                pt2.x = color_dst->width;
            }
            else
            {
                pt1.x = 0;
                pt1.y = cvRound(rho/b);
                pt2.x = cvRound(rho/a);
                pt2.y = 0;
            }
            cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 3, 8 );
        }
#else
        lines = cvHoughLines2( gray, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180,80, 70, 30 );
        for(int i = 0; i < lines->total; i++ )
        {
            CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
			
			if(((abs(line[1].x-line[0].x)<10)||(abs(line[1].y-line[0].y)<10))&&(line[1].x!=0)&&(line[1].y!=0)&&(line[0].x!=0)&&(line[0].y!=0))
				{
					cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, 8 );
					printf("0-%d %d 1-%d %d,",line[0].x,line[0].y,line[1].x,line[1].y);
				}
			else 
			{
				/*double theta=abs((line[1].y-line[0].y)/(line[1].x-line[0].x));
				
				if((theta<0.000001)||(theta>1000))
				{
					printf("%lf,",theta);
					cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, 8 );
				}
			}
        }
#endif*/
//cvCvtColor(gray,color_dst,CV_GRAY2RGB);
	//CvConnectedComp comp;
	//cvFloodFill( gray, cvPoint(width/2-20,height/2-20),cvScalar(255,0,0),cvScalar(0),cvScalar(0),&comp,8,NULL);
/**************************************************************************************
��������Projection
��  �ܣ�ͶӰ���ҳ���������
��  �룺 Ŀ��ͼ��des,  ԭͼ�� src,ͼ���width, ��height
����ֵ��no
**************************************************************************************/
void Projection(unsigned char* des,  const unsigned char* src,int width, int height)
{
	int* h_sum = new int[height];
	int* w_sum = new int[width];
	int up=0;
	int below=height;
	int left=0;
	int right=width;
	for(int y=0;y<height;y++)
	{
		for(int x=0;x<width;x++)
		{
			des[y*width+x]=255;
		}
	}
	for(int y=0;y<height;y++)
	{
		h_sum[y]=0;
		for(int x=0;x<width;x++)
		{
			//printf("src %d",src[y*width+x]);
			h_sum[y]=h_sum[y]+src[y*width+x];
		}
		//printf("%d��%d ",y,h_sum[y]);
	}
	for(int y=height/2;y<height;y++)
	{

		if((h_sum[y]-h_sum[height/2])>255*60)
		{
			below=y;
			break;
		}
	}
	for(int y=height/2;y>0;y--)
	{
		if((h_sum[y]-h_sum[height/2])>255*60)
		{
			up=y;
			break;
		}
	}
	for(int x=0;x<width;x++)
	{
		w_sum[x]=0;
		for(int y=up;y<below;y++)
		{
			w_sum[x]=w_sum[x]+src[y*width+x];
		}
		//printf("%d��%d ",x,w_sum[x]);
	}
	int max_r=0;
	int max_l=0;
	for(int x=width/2+100;x<width;x++)
	{
		if(w_sum[x]>max_r)
		{
			right=x;
			max_r=w_sum[x];
		}
	}
	for(int x=width/2-100;x>0;x--)
	{
		if(w_sum[x]>max_l)
		{
			left=x;
			max_l=w_sum[x];
		}
	}
	for(int y=up;y<below;y++)
	{
		for(int x=left;x<right;x++)
		{
			des[y*width+x]=0;
		}
	}
	printf("up%d below%d left%d right%d",up, below,left, right);

}
void img_extract(unsigned char* des,  const unsigned char* src,int width, int height, int nChannels)
{
	for (int y=0;y<height;y++)
		for(int x=0;x<width;x++)
			if(src[y*width+x]!=0)
			{
				for(int n = 0; n < nChannels; n++)
				{
					des[y * width * nChannels + x * nChannels + n ] = 255;
				}
			}
}
void digital(unsigned char* des,  const unsigned char* src,int width, int height,int n)
{
	for (int y=1;y<height/n;y++)
	{
		for(int x=1;x<width/n;x++)
		{
			int temp=0;
			for(int i=-n/2;i<n/2+1;i++)
			{
				for(int j=-n/2;j<n/2+1;j++)
				{
					temp=temp+src[(n*y+i)*width+n*x+j];
				}
			}
			if(temp<255*18)
			{
				for(int i=-n/2;i<n/2+1;i++)
				{
					for(int j=-n/2;j<n/2+1;j++)
					{
					des[(n*y+i)*width+n*x+j]=0;
					}
				}
			}
			else
			{
				for(int i=-n/2;i<n/2+1;i++)
				{
					for(int j=-n/2;j<n/2+1;j++)
					{
					des[(n*y+i)*width+n*x+j]=255;
					}
				}
			}
		}
	}
}
void gray_hist(unsigned char* des,  const unsigned char* src,int width, int height)
{
	unsigned char* ocl = new unsigned char[width * height];
	unsigned char* flag = new unsigned char[width * height];
	memcpy(des, src,width*height);
	int gray[256] = {0};
	int sum = 0;	
	for(int y=0; y<height; y++)
		for(int x=0; x<width; x++)
		{
			if( src[y * width + x]!=255)
			{
				sum=sum+1;
				flag[y * width + x]= 1;
			    gray[src[y * width+ x]]++;
			}
			else
			{
                flag[y * width + x]= 0;
			}
		}
	double val[1] = {0};
	for(int y=0; y<255; y++)
	{
		val[0] +=gray[y];
		gray[y] = val[0]*255/sum;
	}
	for(int y=0; y<height; y++)
		for(int x=0; x<width; x++)
		{
			if(flag[y * width + x]==1)
			{
				des[y * width + x] = gray[src[y * width + x]] ;
			}
		}
		//memcpy(des, ocl, nChannels*width*height);
	/*for(int y=0; y<height; y++)
		for(int x=0; x<width; x++)
		{
			if(flag[y * width + x]==1)
			{
			    des[y * width + x ] = ocl[y * width + x] ;
			}
			else
			{
			    des[y * width+ x ] = src[y * width + x] ;
			}
		}*/
}
/**************************************************************************************
��������location
��  �ܣ�ͶӰ���ҳ�����
��  �룺Ŀ��ͼ��des,  ԭͼ�� src,ͼ���width, ��height
����ֵ��no
**************************************************************************************/
void location(unsigned char* des,  const unsigned char* src,int width, int height)
{
	int* h_sum = new int[height];
	int* w_sum = new int[width];
	int up=0;
	int below=height;
	int left=0;
	int right=width;
	for(int y=0;y<height;y++)
	{
		for(int x=0;x<width;x++)
		{
			des[y*width+x]=255;
		}
	}
	for(int y=0;y<height;y++)
	{
		h_sum[y]=0;
		for(int x=0;x<width;x++)
		{
			//printf("src %d",src[y*width+x]);
			h_sum[y]=h_sum[y]+src[y*width+x];
		}
		//printf("%d��%d ",y,h_sum[y]);
	}
	int h_mid=(h_sum[height/2]+h_sum[height/2-10]+h_sum[height/2-20]+h_sum[height/2-30]+h_sum[height/2-40]);
	h_mid=h_mid/5;
	for(int y=height/2;y<height;y++)
	{

		if((h_sum[y]-h_mid)>255*35)
		{
			below=y;
			break;
		}
	}
	for(int y=height/2;y>0;y--)
	{
		if((h_sum[y]-h_mid)>255*37)
		{
			up=y;
			break;
		}
	}
	for(int x=0;x<width;x++)
	{
		w_sum[x]=0;
		for(int y=up;y<below;y++)
		{
			w_sum[x]=w_sum[x]+src[y*width+x];
		}
		//printf("%d��%d ",x,w_sum[x]);
	}
	int right_start=width-10;
	for(int x=width-10;x>width/2;x--)
	{
		if(w_sum[x]!=(below-up)*255)
		{
			right_start=x;
			break;
		}
	}
	for(int x=right_start-45;x>width/2;x--)
	{
		if(w_sum[x]<255*(below-up-40))
		{
			right=x;
			break;
		}
	}
	int left_start=10;
	for(int x=10;x<width;x++)
	{
		if(w_sum[x]!=(below-up)*255)
		{
			left_start=x;
			break;
		}
	}
	for(int x=left_start+100;x<width;x++)
	{
			if(w_sum[x]<255*(below-up-20))
			{
				left=x;
				break;
			}
	}

	for(int y=up;y<below;y++)
	{
		for(int x=left-5;x<right+5;x++)
		{
			des[y*width+x]=src[y*width+x];
		}
	}
	printf("up%d below%d left%d right%d left_start%d h_mid%d height/2%d width%d",up, below,left, right,left_start,h_mid,height/2,width);

}