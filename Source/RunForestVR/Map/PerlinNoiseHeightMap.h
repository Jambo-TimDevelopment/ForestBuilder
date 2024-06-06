#pragma once

#include <random>
#include <iomanip>
#include "UObject/ReflectedTypeAccessors.h"
#include "PerlinNoiseHeightMap.h"

class PerlinNoiseHeightMap
{
public:
	// длина карты
	long Length = 0;
	// ширина карты
	int weight = 100;
	// массив значений которые мы будем генерировать
	// скорее всего будет передаваться структура с параметрами
	// вектор векторов / переписать на одномерный массив анрила TArray (C) Semen
	//TArray<float> XValue;

	// ширина и высота квадрата
	int w = 1;
	int h = 1;

	// вектор E 
	int E[4][2] = { {0, 1},
		{0, -1},
		{1, 0},
		{-1, 0} };
	float noise( float x, float y, TArray<float> XValue, int N);
private:
	double functionT(float t);
	float* GetEGradientVector(int nowPoint);
	float getScalarProductOfVectors(float a[], float b[]);
	float getHeight(float a, float b, float alpha);
};
