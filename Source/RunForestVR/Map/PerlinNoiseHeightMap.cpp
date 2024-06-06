#include "PerlinNoiseHeightMap.h"
#include <random>
#include <array>

double PerlinNoiseHeightMap::functionT(float t)
{
	return t * t * t * ((6 * t - 15) * t + 10);
}

float PerlinNoiseHeightMap::getHeight(float a, float b, float alpha)
{
	return a + (b - a) * alpha;
}


float PerlinNoiseHeightMap::getScalarProductOfVectors(float a[], float b[])
{
	return a[0] * b[0] + a[1] * b[1];
}

float* PerlinNoiseHeightMap::GetEGradientVector(int nowPoint)
{
	switch (nowPoint)
	{
	case 0:  return new float[]{  1, 0 };
	case 1:  return new float[]{ -1, 0 };
	case 2:  return new float[]{  0, 1 };
	default: return new float[]{  0,-1 };
	};
}
float PerlinNoiseHeightMap::noise(float x, float y, TArray<float> XValue, int N)
{
	// index now point 
	int xIndex = floor(x / w);
	int yIndex = floor(y / h);
	// x', y'
	// x*n + y = index TArray<Float>
	float xloc = x - xIndex * w;
	float yloc = y - yIndex * h;
	float alpfa = functionT(xloc / w);
	float betta = functionT(yloc / h);
	// вектора к точке из углов квадрата координаты
	float b00[2] = { xloc ,yloc };
	float b01[2] = { xloc - w ,yloc };
	float b10[2] = { xloc, yloc - h };
	float b11[2] = { xloc - w ,yloc - h };


	//GetEGradientVector(nowPoint, ) /// xValue is null!
	float* e00 = GetEGradientVector(XValue[xIndex * N + yIndex]+1); // left up
	float* e01 = GetEGradientVector(XValue[(xIndex + 1) * N + yIndex] + 1); //left down
	float* e10 = GetEGradientVector(XValue[xIndex  * N + yIndex + 1] + 1); //down rigth
	float* e11 = GetEGradientVector(XValue[(xIndex + 1) * N + yIndex + 1 ] + 1); //up

	// поиск высоты 
	float c00 = getScalarProductOfVectors(e00, b00);
	float c01 = getScalarProductOfVectors(e01, b01);
	float c10 = getScalarProductOfVectors(e10, b10);
	float c11 = getScalarProductOfVectors(e11, b11);

	float cx1 = getHeight(c00, c01, alpfa);
	float cx2 = getHeight(c10, c11, alpfa);
	float result = getHeight(cx1, cx2, betta);
	return result;
}



