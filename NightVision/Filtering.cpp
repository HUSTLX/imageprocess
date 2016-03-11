#include"Filtering.h"

////////////////////////////////////滤波器生成部分/////////////////////////////////////////
//获得高斯掩膜
void getGaussMask(Complex** mask, int* width, int* height, int* anchorx, int* anchory, double sigma)
{
	//对于高斯模糊，在3*sigma之外的像素都可以不考虑，加上中心点。因此掩膜大小为(6*sigma+1)*(6*sigma+1),中心点位于3*sigma处
	//当sigma不为整时,对3*sigma向上取证保证对称性, 中心点位于ceil(3*sigma)
	int M = (int)( 2 * ceil(3 * sigma) + 1 );	//宽
	int N = (int)( 2 * ceil(3 * sigma) + 1 );	//高
	int x,y;

	//1.分配空间
	*mask = new Complex[ M * N ];

	Complex* kernal = *mask;

	double sum = 0;
	//2.填充卷积核
	for( x = 0; x < M; x++ )
	{
		for( y = 0; y < N; y++ )
		{
			//计算到(r,r)的距离
			kernal[ y * M + x ].real = exp(  ((double)(-((x-ceil(3*sigma))*(x-ceil(3*sigma)) + (y-ceil(3*sigma))*(y-ceil(3*sigma))))) / (2*sigma*sigma)   ) / (2 * PI * sigma * sigma);
			kernal[ y * M + x ].imag = 0;
			sum += kernal[ y * M + x].real;
		}
	}

	//能量守恒
	for(x = 0; x < M * N; x++)
		kernal[x].real = kernal[x].real / sum;

	//3.返回长宽
	if( width != NULL )
		*width = M;
	if( height != NULL )
		*height = N;

	//4.返回锚点
	if( anchorx != NULL )
		*anchorx = (int)ceil(3*sigma);
	if( anchory != NULL )
		*anchory = (int)ceil(3*sigma);
}




//获得散焦掩膜
void getDefocusMask(Complex** mask, int* width, int* height, int* anchorx, int* anchory, double r)
{
	//以r为散焦半径，设某点为原点，那么各点距原点的距离为：
	//r,...,2,1,0,1,2,...,r，则长宽都为2 * r + 1，中心点为(r,r)
	//当r不为整时，半径应该为floor(r),中心点为(floor(r), floor(r))
	int radius = (int)floor(r);
	int M = 2 * radius + 1;	//宽
	int N = 2 * radius + 1;	//高
	int x,y;

	
	
	//1.分配空间
	*mask = new Complex[ M * N ];
	Complex* kernal = *mask;


	double sum = 0;
	//2.填充卷积核
	for( x = 0; x < M; x++ )
	{
		for( y = 0; y < N; y++ )
		{
			//计算到(r,r)的距离
			if( (x-radius)*(x-radius) + (y-radius)*(y-radius) <= r * r )
				kernal[ y * M + x ].real = 1 / PI / r / r;
			else
				kernal[ y * M + x ].real = 0;
			kernal[ y * M + x ].imag = 0;
			sum += kernal[ y * M + x].real;
		}
	}

	//能量守恒
	for(x = 0; x < M * N; x++)
		kernal[x].real = kernal[x].real / sum;


	//3.返回长宽
	if(width != NULL)
		*width = M;
	if(height != NULL)
		*height = N;

	//返回锚点位置
	if(anchorx != NULL)
		*anchorx = radius;
	if(anchory != NULL)
		*anchory = radius;

}


//psf2otf
void Psf2Otf(Complex* filter, int width, int height, const Complex* mask, int mw, int mh, int ax, int ay)
{
	//将以(ax,ay)为中心的psf映射到(0,0)为中心的otf,并进行周期延拓
	//x' = ((x-ax)<0)? (x-ax+width):(x-ax)
	//y' = ((y-ay)<0)? (y-ay+height):(y-ay)
	
	int x,y;
	int fx,fy;

	//初始化
	for(x = 0; x < width * height; x++)
	{
		filter[x].real = 0;
		filter[x].imag = 0;
	}
	
	if(mw > width && mh > height)
	{
		//掩膜中心位于(ax, ay)
		//要想填充width * height的卷积核
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




////////////////////////////////////滤波与逆滤波部分/////////////////////////////////////////
//滤波
void filtering(Complex* mat, int width, int height, const Complex* filter)
{
	int i,j;
	double treal, timag;
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			//复数乘法
			treal = filter[i * width + j].real * mat[i * width + j].real - filter[i * width + j].imag * mat[i * width + j].imag;
			timag = filter[i * width + j].real * mat[i * width + j].imag + filter[i * width + j].imag * mat[i * width + j].real;

			mat[i * width + j].real = treal;
			mat[i * width + j].imag = timag;
		}
	}
}

//逆滤波
void inverseFiltering(Complex* mat, int width, int height,const Complex* filter)
{
	int i,j;
	double treal, timag;
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			//复数除法
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




////////////////////////////////////拓展与反拓展部分/////////////////////////////////////////

//图像拓展
void expand(Complex** eimg, int* ew, int* eh, const Complex* img, int iw, int ih)
{
	//1.计算扩展图像的大小，注意图像必须是方形的
	int WM = (int)ceil( log((double)iw ) / log(2.0) );
	int HM = (int)ceil( log((double)ih ) / log(2.0) );

	WM = (int)pow(2.0, WM);
	HM = (int)pow(2.0, HM);

	//2.拓展图像
	*eimg = new Complex[WM * HM];
	Complex* ex = *eimg;

	int x,y;
	int r,c;

		//2.直接补0即可
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

//拓展图像还原
void inverseExpand(Complex* img, int iw, int ih, const Complex* eimg, int ew, int eh)
{
	
	//扩展图像必须比源图像大
	if( ew < iw || eh < ih )
		return;

	int x,y;

	//扩展图像的左上角f(0~iw-1,0~ih-1)为原图像，剩余补0
	for(x = 0; x < iw ; x++)
	{
		for( y = 0; y < ih; y++)
		{
			img[ y * iw + x ].real = eimg[ y * ew + x ].real;
			img[ y * iw + x ].imag = 0;
		}
	}
	
}








////////////////////////////////////空域卷积部分////////////////////////////////////////

//卷积操作
void convolution(Complex* des, const Complex* src, int iw, int ih, const Complex* mask, int mw, int mh, int ax, int ay)
{
	//将mask在src上滑行，得到的结果存储在des中
	int x, y, m, n, p, q;
	double sum ;

	for(x = 0; x < iw; x++)
	{
		for(y = 0; y < ih; y++)
		{
			//计算src(x,y)与mask的卷积
			des[y * iw + x].real = 0;	//初始化
			des[y * iw + x].imag = 0;
			sum = 0;

			//对mask上每一点(m,n)
			for(m = 0; m < mw; m++)
			{
				for(n = 0; n < mh; n++)
				{
					//设mask当前点对应img上点的坐标为p,q。那么(m,n)(ax,ay)的相对关系与(p,q)（x,y)相同
					//m-ax=p-x; n-ay=q-y

					//当前位置下(m,n)的对应点
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



