// PerlinNoiseMap.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <random>
#include <iomanip>
#include <array>

using namespace std;

// длина карты
const unsigned int length = 10;
// ширина карты
const unsigned int weight = 10;
// массив значений которые мы будем генерировать
float xValue[length][weight];
// скорее всего будет передаваться структура с параметрами

// ширина и высота квадрата
int w = 2;
int h = 3;

// вектор E 
	int EVector[4][2] = { {0, 1},
	{0, -1},
	{1, 0},
	{-1, 0} };


void randX()
{
	random_device rd;   // non-deterministic generator
	mt19937 gen(rd());  // to seed mersenne twister.
	int min = 1;
	int max = 4;
	uniform_int_distribution<> dist(min, max); // distribute results between 1 and 6 inclusive.
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < weight; j++)
		{
			xValue[i][j] = dist(gen);
			std::cout << setw(4) << xValue[i][j];
		}
		std::cout << std::endl;
	}
}

// f(t)
double functionT(float t) {
	return t * t * t * ((6 * t - 15) * t + 10);
}


float* GetEGradientVector(int nowPoint)
{
	switch (nowPoint)
	{
	case 1:  return new float[2] {  1, 0 };
	case 2:  return new float[2] { -1, 0 };
	case 3:  return new float[2] {  0, 1 };
	default: return new float[2] {  0, -1 };
	};
}
// скалярное произведение векторов a*b = ax*bx + ay*by
float getScalarProductOfVectors(float a[], float b[]) {
	return a[0] * b[0] + a[1] * b[1];
}

float getHeight(float a, float b, float alpha) {
	return a + (b - a) * alpha;
}
// imlement perlin noise algorithm
float noise(float nowPoint, float x, float y) {
	// index now point 
	int xIndex = floor(x / w);
	int yIndex = floor(y / h);
	// x', y'
	float xloc = x - xIndex * w;
	float yloc = y - yIndex * h;
	float alpfa = functionT(xloc / w);
	float betta = functionT(yloc / h);
	// вектора к точке из углов квадрата
	float b10[2] = { xloc, yloc - h };
	float b11[2] = { xloc - w ,yloc - h };
	float b00[2] = { xloc ,yloc };
	float b01[2] = { xloc - w ,yloc };

	//GetEGradientVector(nowPoint, )
	float* e00 = GetEGradientVector(xValue[yIndex][xIndex]+1);
	float* e10 = GetEGradientVector(xValue[yIndex][xIndex + 1] + 1);
	float* e01 = GetEGradientVector(xValue[yIndex + 1][xIndex] + 1);
	float* e11 = GetEGradientVector(xValue[yIndex + 1][xIndex + 1] + 1);

	// поиск высоты 
	float c00 = getScalarProductOfVectors(e00, b00);
	float c01 = getScalarProductOfVectors(e01, b01);
	float c10 = getScalarProductOfVectors(e10, b10);
	float c11 = getScalarProductOfVectors(e11, b11);

	float cx1 = getHeight(c00, c01, alpfa);
	float cx2 = getHeight(c10, c11, alpfa);
	float result = getHeight(cx1, cx2, alpfa);
	return result;
}

int main()
{
	std::cout << "Hello World!\n";
	randX();
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < weight; j++) {
			xValue[i][j] = noise(xValue[i][j], i, j);
			std::cout << setw(10) << xValue[i][j];
		}
		std::cout << std::endl;
	}
}
