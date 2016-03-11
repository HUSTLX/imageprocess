#include"FFT.h"

//二进制倒序排列
void leide(Complex* arr, int n)
{
	//雷德算法
	//在正常序列中，后面的数比前面的数序号大1；在FFT中，后面的数的序号是上面一个数的序号的最高位+1并向低位进位的结果
	//如在n=8时，倒序为:000 100 010 110 001 101 011 111
	//注意到100比000最高位大1；010是100最高位+1，并向第二位（低位）进位的结果
	

	/*若已知某数的倒序数j，求下一个倒序数，先判断j的最高位是否为0，与k=n/2比较即可；若j<k，说明最高位为0，
	将最高位变为1即可(j+n/2)；若j>=k，则说明最高位为1，将其变为0(j-n/2)。再判断次高位，若次高位为0，则将其变1（j+n/4)，
	若次高位为1，则将其变0(j-n/4)，继续判断下一位.......
	即从高到低依次判断是否为1，为1变0，判断下一位；为0加1，结束。
	注意：必须倒序数小于顺序数才能换，否则会重复交换,交换一次其实排好了两个数*/

	int i,j,k;	//i:当前位置 j:当前位置需要交换到的位置
	Complex temp;
	j = 0;
	for(i = 0; i < n; i++)
	{
		if(i < j)
		{
			temp.real = arr[j].real;
			temp.imag = arr[j].imag;

			arr[j].real = arr[i].real;
			arr[j].imag = arr[i].imag;

			arr[i].real = temp.real;
			arr[i].imag = temp.imag;
		}

		//由j寻找下一个数交换的位置
		k = n / 2;
		while( ( k <= j ) && (k > 0 ) )
		{
			//只要j >= k，说明当前位为1，需要进位
			j = j - k; //当前位赋0
			k = k / 2; //判断下一位
		}
		j = j + k;	//当前为赋1
	}
}


//一维傅里叶变换
void FFT1D(Complex* arr, int n)
{
	//1D FFT算法
	//1.判断n是否为2的k次方
	 if( !( n > 0 && ((n & ((~n) + 1)) == n)) )
		 return;

	 //2.FFT运算
	 int i = 0, j = 0, k = 0;
	 int t = n; //临时变量
	 int m = 0;	//蝶形运算级数
	 int l = 0;	//当前级数下的奇数项(or偶数项)的大小K

	 //Complex temp;	
	 Complex sum, dif;
	 //Complex W;			//变换系数W

	 double cosx;
	 double sinx;

	 //雷德算法倒序
	 leide(arr, n);


	 //计算蝶形运算的级数
	 for(m = 1; (t = t / 2) != 1; m++)
		 ;

	 //2.1每级蝶形运算，一直分解到只有一个值的傅里叶变换（就是其本身）,产生当前级的傅里叶变换值
	 for(i = 1; i <= m; i++)
	 {
		 l = 1<<(i-1); //计算当前级下的奇数项（or偶数项)周期数，即每组项个数的一半，即K，

		 //2.2每组蝶形运算，由于将傅里叶运算分为奇数项和偶数项。
		 //每级分为n/l组，每组有2l个值，在计算时，每组的奇数项的傅里叶变换和偶数项的傅里叶变换已知。
		 for(j = 0; j < n; j += 2*l )
		 {
			//j指向每组项的首位
			//2.3每个蝶形运算
			 for(k = 0; k < l; k++)
			 {
				 //k即傅里叶变换的取值，即u，此时变换系数为W(k)(2l)
				 cosx = cos(PI * k / l);
				 sinx = -sin(PI * k / l);

				 sum.real = (arr[j+k].real + arr[j+k+l].real * cosx + arr[j+k+l].imag * sinx) ; //除2保证反变换能够还原到原图像
				 sum.imag = (arr[j+k].imag + arr[j+k+l].imag * cosx - arr[j+k+l].real * sinx) ;

				 dif.real = (arr[j+k].real - arr[j+k+l].real * cosx - arr[j+k+l].imag * sinx) ;
				 dif.imag = (arr[j+k].imag - arr[j+k+l].imag * cosx + arr[j+k+l].real * sinx) ;

				 arr[j+k] = sum;
				 arr[j+k+l] = dif;
			 }
		 }
	 }

}


//二维傅里叶变换
void FFT2D(Complex* des, const Complex* img, int width, int height)
{
	//1.判断width和height是否为2的k次方
	 if( !( width > 0 && ((width & ((~width) + 1)) == width)) )
		 return;

	 if( !( height > 0 && ((height & ((~height) + 1)) == height)) )
		 return;


	 //2.将img备份到des并乘-1^(x+y)，保证转换后的频域图在中心
	 int i,j;
	 for(i = 0; i < height; i++)
	 {
		 for(j = 0; j < width; j++)
		 {
			 des[i * width + j].real = img[i * width + j].real * pow( (double)-1, i+j);
			 des[i * width + j].imag = 0;
		 }
	 }

	 //3.对每行进行傅里叶变换
	 for(i = 0; i < height; i++)
	 {
		 //定位每行的位置
		 Complex* arr = &des[i * width];
		 //一维FFT变换
		 FFT1D(arr, width);
	 }

	 //4.对每列进行傅里叶变换
	 //转置矩阵
	 matrixTranspose(des, width, height);

	 //转置后变为height * width
	 for(j = 0; j < width; j++)
	 {
		 //定位每列位置
		 Complex* arr = &des[j * height];
		 //一维FFT转换
		 FFT1D(arr, height);
	 }

	 //5.再次转置回归
	 matrixTranspose(des, height, width);
}


//一维傅里叶反变换
void IFFT1D(Complex* arr, int n)
{	//1D IFFT算法
	//1.判断n是否为2的k次方
	 if( !( n > 0 && ((n & ((~n) + 1)) == n)) )
		 return;

	  //雷德算法倒序
	 leide(arr, n);

	 //2.IFFT运算
	 int i = 0, j = 0, k = 0;
	 int t = n; //临时变量
	 int m = 0;	//蝶形运算级数
	 int l = 0;	//当前级数下的奇数项(or偶数项)的大小K
	 //Complex temp;	
	 Complex sum, dif;
	 //Complex W;			//变换系数W

	 double cosx, sinx;

	 //计算蝶形运算的级数
	 for(m = 1; (t = t / 2) != 1; m++)
		 ;

	 //2.1每级蝶形运算，一直分解到只有一个值的傅里叶变换（就是其本身）,产生当前级的傅里叶变换值
	 for(i = 1; i <= m; i++)
	 {
		 l = 1<<(i-1); //计算当前级下的奇数项（or偶数项)周期数，即每组项个数的一半，即K，

		 //2.2每组蝶形运算，由于将傅里叶运算分为奇数项和偶数项。
		 //每级分为n/l组，每组有2l个值，在计算时，每组的奇数项的傅里叶变换和偶数项的傅里叶变换已知。
		 for(j = 0; j < n; j += 2*l )
		 {
			//j指向每组项的首位
			//2.3每个蝶形运算
			 for(k = 0; k < l; k++)
			 {
				 //k即傅里叶变换的取值，即u，此时变换系数为W(k)(2l)

				 /*
				 W.real = cos(PI * k / l);
				 W.imag = sin(PI * k / l); //////////////唯一与FFT不同之处//////////////////

				 temp = ComplexMul(&arr[j+k+l],&W); //因为是奇数项所以要加l
				 sum = ComplexAdd(&arr[j+k], &temp);
				 dif = ComplexSub(&arr[j+k], &temp);
				 
				 arr[j+k] = sum;
				 arr[j+k+l] = dif;
				 */

				 
				 cosx = cos(PI * k / l);
				 sinx = sin(PI * k / l); //////////////唯一与FFT不同之处//////////////////

				 sum.real = (arr[j+k].real + arr[j+k+l].real * cosx + arr[j+k+l].imag * sinx) / 2;
				 sum.imag = (arr[j+k].imag + arr[j+k+l].imag * cosx - arr[j+k+l].real * sinx) / 2;

				 dif.real = (arr[j+k].real - arr[j+k+l].real * cosx - arr[j+k+l].imag * sinx) / 2;
				 dif.imag = (arr[j+k].imag - arr[j+k+l].imag * cosx + arr[j+k+l].real * sinx) / 2;

				 arr[j+k] = sum;
				 arr[j+k+l] = dif;
				 
			 }
		 }
	 }
}

//二维傅里叶反变换
void IFFT2D(Complex* des, const Complex* mat, int width, int height)
{
	//1.判断width和height是否为2的k次方
	 if( !( width > 0 && ((width & ((~width) + 1)) == width)) )
		 return;

	 if( !( height > 0 && ((height & ((~height) + 1)) == height)) )
		 return;


	 //2.对每行进行傅里叶变换
	 int i,j;

	 if( mat != des )
	 {
		for(i = 0; i < height; i++)
		{
			 for(j = 0; j < width; j++)
			{
				 des[i * width + j].real = mat[i * width + j].real;
				 des[i * width + j].imag = mat[i * width + j].imag;
			}
		}
	 }

	 for(i = 0; i < height; i++)
	 {
		 //定位每行的位置
		 Complex* arr = &des[i * width];
		 //一维IFFT变换
		 IFFT1D(arr, width);
	 }

	 //3.对每列进行傅里叶变换
	 //转置矩阵
	 matrixTranspose(des, width, height);
	 for(j = 0; j < width; j++)
	 {
		 //定位每列位置
		 Complex* arr = &des[j * height];
		 //一维IFFT转换
		 IFFT1D(arr, height);
	 }

	 //4.再次转置回归
	 matrixTranspose(des, height, width);

	 //5.乘-1^(x+y)，还原
	 for(i = 0; i < height; i++)
	 {
		 for(j = 0; j < width; j++)
		 {
			des[i * width + j].real = des[i * width + j].real * pow( (double)-1, i+j);
		 }
	 }
}





