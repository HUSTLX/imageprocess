#include"Global.h"


//复数加法
Complex complexAdd(const Complex* a, const Complex* b)
{
	Complex result;
	result.real = a -> real + b -> real;
	result.imag = a -> imag + b -> imag;
	return result;
}

//复数减法
Complex complexSub(const Complex* a, const Complex* b)
{
	Complex result;
	result.real = a -> real - b -> real;
	result.imag = a -> imag - b -> imag;
	return result;
}

//复数乘法
Complex complexMul(const Complex* a, const Complex* b)
{
	Complex result;
	result.real = (a->real * b->real) - (a->imag * b->imag);
	result.imag = (a->real * b->imag) + (a->imag * b->real);
	return result;
}

//复数矩阵转置
void matrixTranspose(Complex* mat, int width, int height)
{
	int x,y;
	Complex* temp = new Complex[width * height];
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			temp[x * height + y].real = mat[y * width + x].real;
			temp[x * height + y].imag = mat[y * width + x].imag;
		}
	}

	for(x = 0; x < width * height; x++)
	{
		mat[x].real = temp[x].real;
		mat[x].imag = temp[x].imag;
	}

	delete[] temp;

}

//图像归一化操作
void normalization(Complex* mat, int width, int height)
{
	double max = mat[0].real;
	double min = mat[0].real;
	int i;

	//取最大最小
	for( i = 0; i < width * height; i++)
	{
		if( mat[i].real > max )
			max = mat[i].real;
		if( mat[i].real < min )
			min = mat[i].real;
	}

	//归一化
	for( i = 0; i < width * height; i++)
		mat[i].real = (mat[i].real - min) / ( max - min ) * 255;
}

void clipNormalization(Complex* img, int width, int height, double cvar)
{
	double mean, var, maxi, mini, range;
	int x,y;
	mean = 0.0;
	var = 0.0;
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			mean += img[y * width + x].real;
			var += img[y * width + x].real * img[y * width + x].real;
		}
	}

	mean /= width * height;
	var /= width * height;
	var = var - (mean) * (mean);	//var^2 = E(x^2)-E(x)^2
	var = sqrt(var);

	mini = mean - cvar * var;
	maxi = mean + cvar * var;
	range = (maxi-mini)==0? 1: (maxi-mini);

	for(x = 0; x < width * height; x++)
	{
		img[x].real = getmax(getmin((img[x].real - mini) / range * 255, 255),0);
	}

}

double getmax(double a, double b)
{
	return (a>b)? a: b;
}

double getmin(double a, double b)
{
	return (a<b)? a: b;
}


