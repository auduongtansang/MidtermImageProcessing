#pragma once
#include "opencv2/opencv.hpp"
#include "Converter.h"

using namespace cv;

class ColorTransformer
{
public:
	/*
	Hàm nhận vào một ảnh, thay đổi độ sáng của ảnh này và lưu kết quả vào ảnh mới
	Tham số:
		sourceImage	: ảnh ban đầu
		destinationImage: ảnh kết quả
		b: giá trị số nguyên dùng để thay đổi độ sáng của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b)
	{
		if (sourceImage.empty())
			return 0;

		Mat src = sourceImage.clone();

		for (int i = 0; i < src.rows; i++)
			for (int j = 0; j < src.cols; j++)
			{
				//Xét kênh màu blue
				if (src.at<cv::Vec3b>(i, j)[0] + b > 255)
					//Nếu giá trị màu vượt quá 255 thì gán bằng 255
					src.at<cv::Vec3b>(i, j)[0] = 255;
				else if(src.at<cv::Vec3b>(i, j)[0] + b < 0)
					//Nếu giá trị màu nhỏ hơn 0 thì gán bằng 0
					src.at<cv::Vec3b>(i, j)[0] = 0;
				else
					src.at<cv::Vec3b>(i, j)[0] = (int)(src.at<cv::Vec3b>(i, j)[0] + b);

				//Xét kênh màu green
				if (src.at<cv::Vec3b>(i, j)[1] + b > 255)
					//Nếu giá trị màu vượt quá 255 thì gán bằng 255
					src.at<cv::Vec3b>(i, j)[1] = 255;
				else if (src.at<cv::Vec3b>(i, j)[1] + b < 0)
					//Nếu giá trị màu nhỏ hơn 0 thì gán bằng 0
					src.at<cv::Vec3b>(i, j)[1] = 0;
				else
					src.at<cv::Vec3b>(i, j)[1] = (int)(src.at<cv::Vec3b>(i, j)[1] + b);

				//Xét kênh màu red
				if (src.at<cv::Vec3b>(i, j)[2] + b > 255)
					//Nếu giá trị màu vượt quá 255 thì gán bằng 255
					src.at<cv::Vec3b>(i, j)[2] = 255;
				else if (src.at<cv::Vec3b>(i, j)[2] + b < 0)
					//Nếu giá trị màu nhỏ hơn 0 thì gán bằng 0
					src.at<cv::Vec3b>(i, j)[2] = 0;
				else
					src.at<cv::Vec3b>(i, j)[2] = (int)(src.at<cv::Vec3b>(i, j)[2] + b);
			}

		destinationImage = src;
		return 1;
	}

	/*
	Hàm nhận vào một ảnh, thay đổi độ tương phản của ảnh này và lưu kết quả vào ảnh mới
	Tham số:
		sourceImage: ảnh ban đầu
		destinationImage: ảnh kết quả
		c: giá trị số thực dùng để thay đổi độ tương phản của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c)
	{
		if (sourceImage.empty())
			return 0;

		Mat src = sourceImage.clone();

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				//Xét kênh màu blue
				if (src.at<cv::Vec3b>(i,j)[0] * c > 255)
					//Nếu giá trị màu vượt quá 255 thì gán bằng 255
					src.at<cv::Vec3b>(i, j)[0] = 255; 
				else
					src.at<cv::Vec3b>(i, j)[0] = (int)(src.at<cv::Vec3b>(i, j)[0] * c);

				 //Xét kênh màu green
				if (src.at<cv::Vec3b>(i, j)[1] * c > 255)
					//Nếu giá trị màu vượt quá 255 thì gán bằng 255
					src.at<cv::Vec3b>(i, j)[1] = 255;
				else
					src.at<cv::Vec3b>(i, j)[1] = (int)(src.at<cv::Vec3b>(i, j)[1] * c);

				//Xét kênh màu red
				if (src.at<cv::Vec3b>(i, j)[2] * c > 255)
					//Nếu giá trị màu vượt quá 255 thì gán bằng 255
					src.at<cv::Vec3b>(i, j)[2] = 255;
				else
					src.at<cv::Vec3b>(i, j)[2] = (int)(src.at<cv::Vec3b>(i, j)[2] * c);
			}
		}

		destinationImage = src;
		return 1;
	}


	/*
	Hàm tính lược đồ màu tổng quát cho ảnh bất kỳ
	Tham số:
		sourceImage: ảnh ban đầu
		histMatrix: ma trận histogram (nChannels x 256), mỗi dòng là 1 mảng 256 phần tử histogram của từng kênh màu
	Hàm trả về:
		1: Nếu thành công thì trả về matrix kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tính được histogram hoặc ảnh input không tồn tại
	*/
	int CalcHistogram(const Mat& sourceImage, Mat& histMatrix) 
	{
		if (sourceImage.empty())
			return 0;

		if (sourceImage.channels() == 1)
		{
			Mat histMat = Mat(1, 256, CV_16UC1, Scalar(0));
			for (int i = 0; i < sourceImage.rows; i++)
				for (int j = 0; j < sourceImage.cols; j++)
					histMat.at<unsigned short>(0, (int)sourceImage.at<uchar>(i, j)) += 1;

			histMatrix = histMat;
			return 1;
		}
		
		if (sourceImage.channels() == 3)
		{
			Mat histMat = Mat(3, 256, CV_16UC1,Scalar(0));
			for (int i = 0; i < sourceImage.rows; i++)
				for (int j = 0; j < sourceImage.cols; j++)
				{
					histMat.at<unsigned short>(0, (int)sourceImage.at<Vec3b>(i, j)[0]) += 1;
					histMat.at<unsigned short>(1, (int)sourceImage.at<Vec3b>(i, j)[1]) += 1;
					histMat.at<unsigned short>(2, (int)sourceImage.at<Vec3b>(i, j)[2]) += 1;
				}

			histMatrix = histMat;
			return 1;
		}
		return 0;
	}


	/*
	Hàm cân bằng lược đồ màu tổng quát cho ảnh bất kỳ
	Tham số:
		sourceImage: ảnh ban đầu
		destinationImage: ảnh kết quả
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int HistogramEqualization(const Mat& sourceImage, Mat& destinationImage)
	{
		if (sourceImage.empty())
			return 0;

		if (sourceImage.channels() == 1) //Ảnh xám
		{
			//Khởi tạo ảnh kết quả des
			Mat hist, des = Mat(sourceImage.rows, sourceImage.cols, CV_8UC1, Scalar(0));
			CalcHistogram(sourceImage, hist);

			int t[hist_size];
			memset(t, 0, sizeof(t));

			//Tính lược đồ xám tích lũy, lưu vào t
			t[0] = hist.at<unsigned short>(0, 0);
			for (int i = 1; i < hist_size; i++)
				t[i] = t[i - 1] + hist.at<unsigned short>(0, i);

			//Chuẩn hóa t về đoạn [0, 255]
			for (int i = 0; i < hist_size; i++)
			{
				float tmp = (hist_size - 1) / (float)(sourceImage.cols * sourceImage.rows);
				t[i] = (int)round(tmp * t[i]);
			}

			//Biến đổi ảnh
			for (int i = 0; i < des.rows; i++)
				for (int j = 0; j < des.cols; j++)
					des.at<uchar>(i, j) = t[sourceImage.at<uchar>(i, j)];

			destinationImage = des;
			return 1;
		}
		else if (sourceImage.channels() == 3) //Ảnh màu
		{
			Mat des(sourceImage.rows, sourceImage.cols, CV_8UC3,Scalar(0));
			Mat hsv(sourceImage.rows, sourceImage.cols, CV_8UC3, Scalar(0));
			Mat hsv_des(sourceImage.rows, sourceImage.cols, CV_8UC3,Scalar(0)); //Ảnh HSV sau khi cân bằng V

			int histV[hist_size];
			memset(histV, 0, sizeof(histV));
			int t[hist_size];
			memset(t, 0, sizeof(t));

			Converter v;
			v.Convert(sourceImage, hsv, 2); //HẾT LỖI RỒI !!!
			//Tính histogram của V trong ảnh HSV
			calc_HistV_HSV(hsv, histV);

			//Tính lược đồ xám tích lũy của V, lưu vào t
			t[0] = histV[0];
			for (int i = 1; i < hist_size; i++)
				t[i] = t[i - 1] + histV[i];

			//Chuẩn hóa t về đoạn [0, 255]
			for (int i = 0; i < hist_size; i++)
			{
				float tmp = (hist_size - 1) / (float)(sourceImage.cols * sourceImage.rows);
				t[i] = (int)round(tmp * t[i]);
			}

			//Biến đổi ảnh
			hsv_des = hsv.clone(); //Chép 2 thuộc tính H và S vào ảnh HSV cân bằng
			for (int i = 0; i < hsv_des.rows; i++)
				for (int j = 0; j < hsv_des.cols; j++)
					hsv_des.at<Vec3b>(i, j)[2] = t[hsv.at<cv::Vec3b>(i, j)[2]];

			v.Convert(hsv_des, des, 3);
			destinationImage = des;
			return 1;
		}
		return 0;
	}

	/*
	Hàm vẽ lược đồ màu tổng quát cho ảnh bất kỳ
	Tham số:
		histMatrix: ma trận histogram đã tính được
		histImage: ảnh histogram được vẽ
	Hàm trả về:
		1: Nếu thành công vẽ được histogram
		0: Nếu không vẽ được histogram
	*/
	int DrawHistogram(Mat& histMatrix, Mat& histImage)
	{
		if (histMatrix.empty())
			return 0;

		hist_normalization(histMatrix);

		if (histMatrix.rows == 1) //Lược đồ của ảnh xám
		{
			Mat des_Hist_1 = Mat(300, 256, CV_8UC1, Scalar(0));
			for (int j = 0; j < des_Hist_1.cols; j++)
				line(des_Hist_1, Point(j, des_Hist_1.rows - 1 - histMatrix.at<unsigned short>(0, j)), Point(j, des_Hist_1.rows - 1), Scalar(255), 2);

			histImage = des_Hist_1;
			return 1;
		}
		if (histMatrix.rows == 3)
		{
			Mat des_Hist_3 = Mat(300, 256 * 3, CV_8UC3, Scalar(0));
			for (int i = 0; i < histMatrix.rows; i++)
				for (int j = 0; j < histMatrix.cols; j++)
				{
					line(des_Hist_3, Point(j, des_Hist_3.rows - 1 - histMatrix.at<unsigned short>(0, j)), Point(j, des_Hist_3.rows - 1), Scalar(255, 0, 0), 2);
					line(des_Hist_3, Point(256 + j, des_Hist_3.rows - 1 - histMatrix.at<unsigned short>(1, j)), Point(256 + j, des_Hist_3.rows - 1), Scalar(0, 255, 0), 2);
					line(des_Hist_3, Point(512 + j, des_Hist_3.rows - 1 - histMatrix.at<unsigned short>(2, j)), Point(512 + j, des_Hist_3.rows - 1), Scalar(0, 0, 255), 2);
				}

			histImage = des_Hist_3;
			return 1;
		}
		return 0;
	}

	/*
	Hàm so sánh hai ảnh
	Tham số:
		image1: ảnh thứ nhất
		image2: ảnh thứ hai
	Hàm trả về:
		độ đo sự tương đồng giữa hai ảnh
	*/
	float CompareImage(const Mat& image1, Mat& image2)
	{
		float res = 0;
		Mat Hist1, Hist2;
		CalcHistogram(image1, Hist1);
		CalcHistogram(image2, Hist2);

		for (int i = 0; i < Hist1.cols; i++)
			res += (Hist1.at<unsigned short>(0,i) - Hist2.at<unsigned short>(0,i))*(Hist1.at<unsigned short>(0, i) - Hist2.at<unsigned short>(0, i));

		return sqrt(res);
 	}

	ColorTransformer()
	{
	}

	~ColorTransformer()
	{
	}
};