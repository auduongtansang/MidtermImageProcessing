#pragma once
#include <opencv2/opencv.hpp>
#include "Convolution.h"
#include <vector>

using namespace std;
using namespace cv;

class EdgeDetector
{
public:
	/*
	Hàm lọc biên cạnh của ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp lọc biên cạnh
		1: Sobel
		2: Prewitt
		3: Laplace
	kernelWidth, kernelHeight: kích thước kernel
	Hàm trả về
		0: nếu lọc thành công
		1: nếu lọc thất bại (không đọc được ảnh input,...)
	*/
	int DetectEdge(const Mat& sourceImage, Mat& destinationImage, int kernelWidth, int kernelHeight, int method);
	EdgeDetector();
	~EdgeDetector();
};