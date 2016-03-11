#include"FFT.h"

//�����Ƶ�������
void leide(Complex* arr, int n)
{
	//�׵��㷨
	//�����������У����������ǰ�������Ŵ�1����FFT�У�������������������һ��������ŵ����λ+1�����λ��λ�Ľ��
	//����n=8ʱ������Ϊ:000 100 010 110 001 101 011 111
	//ע�⵽100��000���λ��1��010��100���λ+1������ڶ�λ����λ����λ�Ľ��
	

	/*����֪ĳ���ĵ�����j������һ�������������ж�j�����λ�Ƿ�Ϊ0����k=n/2�Ƚϼ��ɣ���j<k��˵�����λΪ0��
	�����λ��Ϊ1����(j+n/2)����j>=k����˵�����λΪ1�������Ϊ0(j-n/2)�����жϴθ�λ�����θ�λΪ0�������1��j+n/4)��
	���θ�λΪ1�������0(j-n/4)�������ж���һλ.......
	���Ӹߵ��������ж��Ƿ�Ϊ1��Ϊ1��0���ж���һλ��Ϊ0��1��������
	ע�⣺���뵹����С��˳�������ܻ���������ظ�����,����һ����ʵ�ź���������*/

	int i,j,k;	//i:��ǰλ�� j:��ǰλ����Ҫ��������λ��
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

		//��jѰ����һ����������λ��
		k = n / 2;
		while( ( k <= j ) && (k > 0 ) )
		{
			//ֻҪj >= k��˵����ǰλΪ1����Ҫ��λ
			j = j - k; //��ǰλ��0
			k = k / 2; //�ж���һλ
		}
		j = j + k;	//��ǰΪ��1
	}
}


//һά����Ҷ�任
void FFT1D(Complex* arr, int n)
{
	//1D FFT�㷨
	//1.�ж�n�Ƿ�Ϊ2��k�η�
	 if( !( n > 0 && ((n & ((~n) + 1)) == n)) )
		 return;

	 //2.FFT����
	 int i = 0, j = 0, k = 0;
	 int t = n; //��ʱ����
	 int m = 0;	//�������㼶��
	 int l = 0;	//��ǰ�����µ�������(orż����)�Ĵ�СK

	 //Complex temp;	
	 Complex sum, dif;
	 //Complex W;			//�任ϵ��W

	 double cosx;
	 double sinx;

	 //�׵��㷨����
	 leide(arr, n);


	 //�����������ļ���
	 for(m = 1; (t = t / 2) != 1; m++)
		 ;

	 //2.1ÿ���������㣬һֱ�ֽ⵽ֻ��һ��ֵ�ĸ���Ҷ�任�������䱾��,������ǰ���ĸ���Ҷ�任ֵ
	 for(i = 1; i <= m; i++)
	 {
		 l = 1<<(i-1); //���㵱ǰ���µ������orż����)����������ÿ���������һ�룬��K��

		 //2.2ÿ��������㣬���ڽ�����Ҷ�����Ϊ�������ż���
		 //ÿ����Ϊn/l�飬ÿ����2l��ֵ���ڼ���ʱ��ÿ���������ĸ���Ҷ�任��ż����ĸ���Ҷ�任��֪��
		 for(j = 0; j < n; j += 2*l )
		 {
			//jָ��ÿ�������λ
			//2.3ÿ����������
			 for(k = 0; k < l; k++)
			 {
				 //k������Ҷ�任��ȡֵ����u����ʱ�任ϵ��ΪW(k)(2l)
				 cosx = cos(PI * k / l);
				 sinx = -sin(PI * k / l);

				 sum.real = (arr[j+k].real + arr[j+k+l].real * cosx + arr[j+k+l].imag * sinx) ; //��2��֤���任�ܹ���ԭ��ԭͼ��
				 sum.imag = (arr[j+k].imag + arr[j+k+l].imag * cosx - arr[j+k+l].real * sinx) ;

				 dif.real = (arr[j+k].real - arr[j+k+l].real * cosx - arr[j+k+l].imag * sinx) ;
				 dif.imag = (arr[j+k].imag - arr[j+k+l].imag * cosx + arr[j+k+l].real * sinx) ;

				 arr[j+k] = sum;
				 arr[j+k+l] = dif;
			 }
		 }
	 }

}


//��ά����Ҷ�任
void FFT2D(Complex* des, const Complex* img, int width, int height)
{
	//1.�ж�width��height�Ƿ�Ϊ2��k�η�
	 if( !( width > 0 && ((width & ((~width) + 1)) == width)) )
		 return;

	 if( !( height > 0 && ((height & ((~height) + 1)) == height)) )
		 return;


	 //2.��img���ݵ�des����-1^(x+y)����֤ת�����Ƶ��ͼ������
	 int i,j;
	 for(i = 0; i < height; i++)
	 {
		 for(j = 0; j < width; j++)
		 {
			 des[i * width + j].real = img[i * width + j].real * pow( (double)-1, i+j);
			 des[i * width + j].imag = 0;
		 }
	 }

	 //3.��ÿ�н��и���Ҷ�任
	 for(i = 0; i < height; i++)
	 {
		 //��λÿ�е�λ��
		 Complex* arr = &des[i * width];
		 //һάFFT�任
		 FFT1D(arr, width);
	 }

	 //4.��ÿ�н��и���Ҷ�任
	 //ת�þ���
	 matrixTranspose(des, width, height);

	 //ת�ú��Ϊheight * width
	 for(j = 0; j < width; j++)
	 {
		 //��λÿ��λ��
		 Complex* arr = &des[j * height];
		 //һάFFTת��
		 FFT1D(arr, height);
	 }

	 //5.�ٴ�ת�ûع�
	 matrixTranspose(des, height, width);
}


//һά����Ҷ���任
void IFFT1D(Complex* arr, int n)
{	//1D IFFT�㷨
	//1.�ж�n�Ƿ�Ϊ2��k�η�
	 if( !( n > 0 && ((n & ((~n) + 1)) == n)) )
		 return;

	  //�׵��㷨����
	 leide(arr, n);

	 //2.IFFT����
	 int i = 0, j = 0, k = 0;
	 int t = n; //��ʱ����
	 int m = 0;	//�������㼶��
	 int l = 0;	//��ǰ�����µ�������(orż����)�Ĵ�СK
	 //Complex temp;	
	 Complex sum, dif;
	 //Complex W;			//�任ϵ��W

	 double cosx, sinx;

	 //�����������ļ���
	 for(m = 1; (t = t / 2) != 1; m++)
		 ;

	 //2.1ÿ���������㣬һֱ�ֽ⵽ֻ��һ��ֵ�ĸ���Ҷ�任�������䱾��,������ǰ���ĸ���Ҷ�任ֵ
	 for(i = 1; i <= m; i++)
	 {
		 l = 1<<(i-1); //���㵱ǰ���µ������orż����)����������ÿ���������һ�룬��K��

		 //2.2ÿ��������㣬���ڽ�����Ҷ�����Ϊ�������ż���
		 //ÿ����Ϊn/l�飬ÿ����2l��ֵ���ڼ���ʱ��ÿ���������ĸ���Ҷ�任��ż����ĸ���Ҷ�任��֪��
		 for(j = 0; j < n; j += 2*l )
		 {
			//jָ��ÿ�������λ
			//2.3ÿ����������
			 for(k = 0; k < l; k++)
			 {
				 //k������Ҷ�任��ȡֵ����u����ʱ�任ϵ��ΪW(k)(2l)

				 /*
				 W.real = cos(PI * k / l);
				 W.imag = sin(PI * k / l); //////////////Ψһ��FFT��֮ͬ��//////////////////

				 temp = ComplexMul(&arr[j+k+l],&W); //��Ϊ������������Ҫ��l
				 sum = ComplexAdd(&arr[j+k], &temp);
				 dif = ComplexSub(&arr[j+k], &temp);
				 
				 arr[j+k] = sum;
				 arr[j+k+l] = dif;
				 */

				 
				 cosx = cos(PI * k / l);
				 sinx = sin(PI * k / l); //////////////Ψһ��FFT��֮ͬ��//////////////////

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

//��ά����Ҷ���任
void IFFT2D(Complex* des, const Complex* mat, int width, int height)
{
	//1.�ж�width��height�Ƿ�Ϊ2��k�η�
	 if( !( width > 0 && ((width & ((~width) + 1)) == width)) )
		 return;

	 if( !( height > 0 && ((height & ((~height) + 1)) == height)) )
		 return;


	 //2.��ÿ�н��и���Ҷ�任
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
		 //��λÿ�е�λ��
		 Complex* arr = &des[i * width];
		 //һάIFFT�任
		 IFFT1D(arr, width);
	 }

	 //3.��ÿ�н��и���Ҷ�任
	 //ת�þ���
	 matrixTranspose(des, width, height);
	 for(j = 0; j < width; j++)
	 {
		 //��λÿ��λ��
		 Complex* arr = &des[j * height];
		 //һάIFFTת��
		 IFFT1D(arr, height);
	 }

	 //4.�ٴ�ת�ûع�
	 matrixTranspose(des, height, width);

	 //5.��-1^(x+y)����ԭ
	 for(i = 0; i < height; i++)
	 {
		 for(j = 0; j < width; j++)
		 {
			des[i * width + j].real = des[i * width + j].real * pow( (double)-1, i+j);
		 }
	 }
}





