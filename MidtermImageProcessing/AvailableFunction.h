#pragma once
#include "opencv2/opencv.hpp"

using namespace cv;

#define hist_size 256

/*
Hàm chuẩn hóa histogram để vẽ đồ thị, tính chiều cao của ảnh dựa trên giá trị lớn nhất của đồ thị
Nên chia n để giảm độ cao của đồ thị, có thể chỉnh sửa n để đồ thị đẹp hơn.
*/
unsigned short hist_normalization(Mat &hist)
{
	 
	if (hist.rows == 1) //ảnh xám
	{
		unsigned short max = hist.at<unsigned short>(0, 0) / 15; 
		for (int i = 0; i < hist.cols; i++)
		{
			//n = 15
			hist.at<unsigned short>(0, i) /= 15;
			if (max < hist.at<unsigned short>(0, i))
				max = hist.at<unsigned short>(0, i);
		}
		return max + 1;
	}
	else if (hist.rows == 3)
	{
		unsigned short max = hist.at<unsigned short>(0, 0) / 6;
		for (int i = 0; i < hist.rows; i++)
			for (int j = 0; j < hist.cols; j++)
			{
				//n = 15
				hist.at<unsigned short>(i, j) /= 15;
				if (max < hist.at<unsigned short>(i, j))
					max = hist.at<unsigned short>(i, j);
			}
		return max + 1;
	}
	return 0;
}
//Tính riêng thuộc tính V trong HSV để cân bằng ảnh màu
void calc_HistV_HSV(Mat src, int histV[hist_size])
{
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
			histV[src.at<cv::Vec3b>(i, j)[2]] += 1;
}