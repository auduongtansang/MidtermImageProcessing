#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Convolution
{
	vector<int> _kernel;  //Lưu giá trị của ma trận kernel dùng để tích chập
	float _division;  //Nhân tử chung của kernel: 1/_division
	int _kernelWidth;  //Chiều rộng (ngang) kernel
	int _kernelHeight;  //Chiều cao (dọc) kernel
public:
	//Trả về kernel
	vector<int> GetKernel();
	//Set kernel, chiều rộng và chiều cao phải là số lẻ
	void SetKernel(vector<int> kernel, float division, int kernelWidth, int kernelHeight);

	/*
	Hàm tính convolution của một ảnh xám với kernel được xác định trước
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về:
		0: nếu tính thành công
		1: nếu tính thất bại (không đọc được ảnh input,...)
	*/
	int DoConvolution(const Mat& sourceImage, Mat& destinationImage);
	Convolution();
	~Convolution();
};