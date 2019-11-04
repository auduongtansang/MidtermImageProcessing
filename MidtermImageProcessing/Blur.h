#pragma once
#include <opencv2/opencv.hpp>
#include "Convolution.h"
#include <vector>

using namespace cv;
using namespace std;

#define PI 3.14159265358979323846f

class Blur
{
public:
	/*
	Hàm lọc nhiễu ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp lọc nhiễu
		0: trung bình
		1: trung vị
		2: Gaussian
	Hàm trả về:
		0: nếu lọc nhiễu thành công
		1: nếu lọc nhiễu thất bại (không đọc được ảnh input,...)
	*/
	int BlurImage(const Mat& sourceImage, Mat& destinationImage, int kernelWidth, int kernelHeight, int method);
	Blur();
	~Blur();
};