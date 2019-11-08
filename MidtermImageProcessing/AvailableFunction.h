#pragma once
#include "opencv2/opencv.hpp"

using namespace cv;

#define hist_size 256

/*
Hàm chuẩn hóa histogram để vẽ đồ thị, tính chiều cao của ảnh dựa trên giá trị lớn nhất của đồ thị
*/
void hist_normalization(Mat &hist)
{
	 
	if (hist.rows == 1) //ảnh xám
	{
		unsigned short max = hist.at<unsigned short>(0, 0);

		for (int i = 0; i < hist.cols; i++)
			max = max < hist.at<unsigned short>(0, i) ? hist.at<unsigned short>(0, i) : max;
		
		for (int i = 0; i < hist.cols; i++)
			hist.at<unsigned short>(0, i) = hist.at<unsigned short>(0, i) * 300 / max;
	}
	else if (hist.rows == 3)
	{
		unsigned short max = hist.at<unsigned short>(0, 0);

		for (int i = 0; i < hist.rows; i++)
			for (int j = 0; j < hist.cols; j++)
				max = max < hist.at<unsigned short>(i, j) ? hist.at<unsigned short>(i, j) : max;

		for (int i = 0; i < hist.rows; i++)
			for (int j = 0; j < hist.cols; j++)
				hist.at<unsigned short>(i, j) = hist.at<unsigned short>(i, j) * 300 / max;
	}
}
//Tính riêng thuộc tính V trong HSV để cân bằng ảnh màu
void calc_HistV_HSV(Mat src, int histV[hist_size])
{
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
			histV[src.at<cv::Vec3b>(i, j)[2]] += 1;
}