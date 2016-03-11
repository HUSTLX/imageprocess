#include"Filtering.h"

////////////////////////////////////�˲������ɲ���/////////////////////////////////////////
//��ø�˹��Ĥ
void getGaussMask(Complex** mask, int* width, int* height, int* anchorx, int* anchory, double sigma)
{
	//���ڸ�˹ģ������3*sigma֮������ض����Բ����ǣ��������ĵ㡣�����Ĥ��СΪ(6*sigma+1)*(6*sigma+1),���ĵ�λ��3*sigma��
	//��sigma��Ϊ��ʱ,��3*sigma����ȡ֤��֤�Գ���, ���ĵ�λ��ceil(3*sigma)
	int M = (int)( 2 * ceil(3 * sigma) + 1 );	//��
	int N = (int)( 2 * ceil(3 * sigma) + 1 );	//��
	int x,y;

	//1.����ռ�
	*mask = new Complex[ M * N ];

	Complex* kernal = *mask;

	double sum = 0;
	//2.�������
	for( x = 0; x < M; x++ )
	{
		for( y = 0; y < N; y++ )
		{
			//���㵽(r,r)�ľ���
			kernal[ y * M + x ].real = exp(  ((double)(-((x-ceil(3*sigma))*(x-ceil(3*sigma)) + (y-ceil(3*sigma))*(y-ceil(3*sigma))))) / (2*sigma*sigma)   ) / (2 * PI * sigma * sigma);
			kernal[ y * M + x ].imag = 0;
			sum += kernal[ y * M + x].real;
		}
	}

	//�����غ�
	for(x = 0; x < M * N; x++)
		kernal[x].real = kernal[x].real / sum;

	//3.���س���
	if( width != NULL )
		*width = M;
	if( height != NULL )
		*height = N;

	//4.����ê��
	if( anchorx != NULL )
		*anchorx = (int)ceil(3*sigma);
	if( anchory != NULL )
		*anchory = (int)ceil(3*sigma);
}




//���ɢ����Ĥ
void getDefocusMask(Complex** mask, int* width, int* height, int* anchorx, int* anchory, double r)
{
	//��rΪɢ���뾶����ĳ��Ϊԭ�㣬��ô�����ԭ��ľ���Ϊ��
	//r,...,2,1,0,1,2,...,r���򳤿�Ϊ2 * r + 1�����ĵ�Ϊ(r,r)
	//��r��Ϊ��ʱ���뾶Ӧ��Ϊfloor(r),���ĵ�Ϊ(floor(r), floor(r))
	int radius = (int)floor(r);
	int M = 2 * radius + 1;	//��
	int N = 2 * radius + 1;	//��
	int x,y;

	
	
	//1.����ռ�
	*mask = new Complex[ M * N ];
	Complex* kernal = *mask;


	double sum = 0;
	//2.�������
	for( x = 0; x < M; x++ )
	{
		for( y = 0; y < N; y++ )
		{
			//���㵽(r,r)�ľ���
			if( (x-radius)*(x-radius) + (y-radius)*(y-radius) <= r * r )
				kernal[ y * M + x ].real = 1 / PI / r / r;
			else
				kernal[ y * M + x ].real = 0;
			kernal[ y * M + x ].imag = 0;
			sum += kernal[ y * M + x].real;
		}
	}

	//�����غ�
	for(x = 0; x < M * N; x++)
		kernal[x].real = kernal[x].real / sum;


	//3.���س���
	if(width != NULL)
		*width = M;
	if(height != NULL)
		*height = N;

	//����ê��λ��
	if(anchorx != NULL)
		*anchorx = radius;
	if(anchory != NULL)
		*anchory = radius;

}


//psf2otf
void Psf2Otf(Complex* filter, int width, int height, const Complex* mask, int mw, int mh, int ax, int ay)
{
	//����(ax,ay)Ϊ���ĵ�psfӳ�䵽(0,0)Ϊ���ĵ�otf,��������������
	//x' = ((x-ax)<0)? (x-ax+width):(x-ax)
	//y' = ((y-ay)<0)? (y-ay+height):(y-ay)
	
	int x,y;
	int fx,fy;

	//��ʼ��
	for(x = 0; x < width * height; x++)
	{
		filter[x].real = 0;
		filter[x].imag = 0;
	}
	
	if(mw > width && mh > height)
	{
		//��Ĥ����λ��(ax, ay)
		//Ҫ�����width * height�ľ����
		//(ax-width/2, ay-height/2)~(ax+width/2, ay+height/2)
		double sum = 0;
		for(x = ax - width/2; x < ax + width/2; x++)
		{
			for(y = ay - height/2; y < ay + height/2; y++)
			{
				fx = ((x-ax)<0)? (x-ax+width):(x-ax);
				fy = ((y-ay)<0)? (y-ay+height):(y-ay);
				filter[fy * width + fx].real = mask[y * mw + x].real;
				
				sum += filter[fy * width + fx].real;
			}
		}
		for(x = 0; x < width * height; x++)
			filter[x].real = filter[x].real / sum;
	}
	else
	{
		for(x = 0; x < mw; x++)
		{
			for(y = 0; y < mh; y++)
			{
				fx = ((x-ax)<0)? (x-ax+width):(x-ax);
				fy = ((y-ay)<0)? (y-ay+height):(y-ay);
				filter[fy * width + fx].real = mask[y * mw + x].real;
			}
		}
		
	}
	
}




////////////////////////////////////�˲������˲�����/////////////////////////////////////////
//�˲�
void filtering(Complex* mat, int width, int height, const Complex* filter)
{
	int i,j;
	double treal, timag;
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			//�����˷�
			treal = filter[i * width + j].real * mat[i * width + j].real - filter[i * width + j].imag * mat[i * width + j].imag;
			timag = filter[i * width + j].real * mat[i * width + j].imag + filter[i * width + j].imag * mat[i * width + j].real;

			mat[i * width + j].real = treal;
			mat[i * width + j].imag = timag;
		}
	}
}

//���˲�
void inverseFiltering(Complex* mat, int width, int height,const Complex* filter)
{
	int i,j;
	double treal, timag;
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			//��������
			if((filter[i * width + j].imag * filter[i * width + j].imag + filter[i * width + j].real * filter[i * width + j].real ) == 0)
				treal = timag = 0;
			else
			{
				treal = (filter[i * width + j].real * mat[i * width + j].real + filter[i * width + j].imag * mat[i * width + j].imag) / (filter[i * width + j].imag * filter[i * width + j].imag + filter[i * width + j].real * filter[i * width + j].real );
				timag = (mat[i * width + j].imag * filter[i * width + j].real - mat[i * width + j].real * filter[i * width + j].imag) / (filter[i * width + j].imag * filter[i * width + j].imag + filter[i * width + j].real * filter[i * width + j].real );
			}

			mat[i * width + j].real = treal;
			mat[i * width + j].imag = timag;
		}
	}
}




////////////////////////////////////��չ�뷴��չ����/////////////////////////////////////////

//ͼ����չ
void expand(Complex** eimg, int* ew, int* eh, const Complex* img, int iw, int ih)
{
	//1.������չͼ��Ĵ�С��ע��ͼ������Ƿ��ε�
	int WM = (int)ceil( log((double)iw ) / log(2.0) );
	int HM = (int)ceil( log((double)ih ) / log(2.0) );

	WM = (int)pow(2.0, WM);
	HM = (int)pow(2.0, HM);

	//2.��չͼ��
	*eimg = new Complex[WM * HM];
	Complex* ex = *eimg;

	int x,y;
	int r,c;

		//2.ֱ�Ӳ�0����
	for(x = 0; x < WM; x++)
	{
		for(y = 0; y < HM; y++)
		{
			if(x < 0)
				r = x + iw;
			else if(x >= iw)
				r = x - iw;
			else 
				r = x;

			if(y < 0)
				c = y + ih;
			else if(y >= ih)
				c = y - ih;
			else
				c = y;

			ex[y * WM + x].real = img[c * iw + r].real;
			ex[y * WM + x].imag = 0;
		}
	}

	*ew = WM;
	*eh = HM;

}

//��չͼ��ԭ
void inverseExpand(Complex* img, int iw, int ih, const Complex* eimg, int ew, int eh)
{
	
	//��չͼ������Դͼ���
	if( ew < iw || eh < ih )
		return;

	int x,y;

	//��չͼ������Ͻ�f(0~iw-1,0~ih-1)Ϊԭͼ��ʣ�ಹ0
	for(x = 0; x < iw ; x++)
	{
		for( y = 0; y < ih; y++)
		{
			img[ y * iw + x ].real = eimg[ y * ew + x ].real;
			img[ y * iw + x ].imag = 0;
		}
	}
	
}








////////////////////////////////////����������////////////////////////////////////////

//�������
void convolution(Complex* des, const Complex* src, int iw, int ih, const Complex* mask, int mw, int mh, int ax, int ay)
{
	//��mask��src�ϻ��У��õ��Ľ���洢��des��
	int x, y, m, n, p, q;
	double sum ;

	for(x = 0; x < iw; x++)
	{
		for(y = 0; y < ih; y++)
		{
			//����src(x,y)��mask�ľ��
			des[y * iw + x].real = 0;	//��ʼ��
			des[y * iw + x].imag = 0;
			sum = 0;

			//��mask��ÿһ��(m,n)
			for(m = 0; m < mw; m++)
			{
				for(n = 0; n < mh; n++)
				{
					//��mask��ǰ���Ӧimg�ϵ������Ϊp,q����ô(m,n)(ax,ay)����Թ�ϵ��(p,q)��x,y)��ͬ
					//m-ax=p-x; n-ay=q-y

					//��ǰλ����(m,n)�Ķ�Ӧ��
					p = m - ax + x;    
					q = n - ay + y;
					
					if( p >= 0 && q >= 0 && p < iw && q < ih)
					{
						des[ y * iw + x ].real += src[ q * iw + p ].real * mask[ n * mw + m ].real ;
						sum += mask[ n * mw + m ].real;
					}
				}
			}

			des[y * iw + x].real = des[y * iw + x].real / sum;
		}
	}
}



