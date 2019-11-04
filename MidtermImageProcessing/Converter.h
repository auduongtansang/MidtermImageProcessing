#pragma once
#include "opencv2/opencv.hpp"

using namespace cv;

class Converter
{
	/*
	Hàm chuyển đổi không gian màu của ảnh từ RGB sang GrayScale
		sourceImage: ảnh input
		destinationImage: ảnh output
	Hàm trả về
		0: nếu chuyển thành công
		1: nếu chuyển thất bại (không đọc được ảnh input,...)
	Y  =  0.2126 × R   +   0.7152 × G   +   0.0722 × B
	*/
	int RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage)
	{
		//Lay dong va cot
		int cols = sourceImage.cols;
		int rows = sourceImage.rows;

		//Tao anh output
		Mat image(rows, cols, CV_8UC1);
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				uchar red, green, blue; //uint8_t		type: 1 byte unsigned		range: 0 to 255
				blue = sourceImage.data[i * cols * 3 + j * 3 + 0]; // B
				green = sourceImage.data[i * cols * 3 + j * 3 + 1]; // G
				red = sourceImage.data[i * cols * 3 + j * 3 + 2]; // R

				image.data[i * cols + j] = (uchar)(0.2126 * red + 0.7152 * green + 0.0722 * blue);
			}
		}

		destinationImage = image;
		return 1;
	}

	/*
	Hàm chuyển đổi không gian màu của ảnh từ GrayScale sang RGB
		sourceImage: ảnh input
		destinationImage: ảnh output
	Hàm trả về
		0: nếu chuyển thành công
		1: nếu chuyển thất bại (không đọc được ảnh input,...)
	B = G = R = Gray Value
	*/
	int GrayScale2RGB(const Mat& sourceImage, Mat& destinationImage)
	{
		int cols = sourceImage.cols;
		int rows = sourceImage.rows;

		Mat image(rows, cols, CV_8UC3, Scalar(0));
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				uchar gray;
				gray = sourceImage.data[i * cols + j];

				image.data[i * cols * 3 + j * 3 + 0] = gray; // B
				image.data[i * cols * 3 + j * 3 + 1] = gray; // G
				image.data[i * cols * 3 + j * 3 + 2] = gray; // R
			}
		}
		destinationImage = image;
		return 1;
	}


	/*
	Hàm chuyển đổi không gian màu của ảnh từ RGB sang HSV
		sourceImage: ảnh input
		destinationImage: ảnh output
	Hàm trả về
		0: nếu chuyển thành công
		1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int RGB2HSV(const Mat& sourceImage, Mat& destinationImage)
	{
		if (sourceImage.empty())
			return 0;

		//Số dòng và cột
		int nl = sourceImage.rows;
		int nc = sourceImage.cols;

		destinationImage = sourceImage.clone();
		for (int i = 0; i < nl; i++)
		{
			//Lấy địa chỉ của dòng thứ i
			const uchar* data = sourceImage.ptr<uchar>(i);
			uchar* data2 = destinationImage.ptr<uchar>(i);

			for (int j = 0; j < nc; j++)
			{
				float R, G, B, H, S, V;
				R = data[j * 3 + 2] / 255.0f;
				G = data[j * 3 + 1] / 255.0f;
				B = data[j * 3 + 0] / 255.0f;

				V = MAX(R, MAX(G, B));
				if (V != 0)
					S = (V - MIN(R, MIN(G, B))) / V;
				else
					S = 0;

				if (V == R)
					H = 60 * (G - B) / (V - MIN(R, MIN(G, B)));
				else if (V == G)
					H = 120 + 60 * (B - R) / (V - MIN(R, MIN(G, B)));
				else
					H = 240 + 60 * (R - G) / (V - MIN(R, MIN(G, B)));

				if (H < 0)
					H += 360;

				data2[j * 3 + 0] = (uchar)(H / 2);
				data2[j * 3 + 1] = (uchar)(255 * S);
				data2[j * 3 + 2] = (uchar)(255 * V);
			}
		}
		return 1;
	}



	/*
	Hàm chuyển đổi không gian màu của ảnh từ HSV sang RGB
		sourceImage: ảnh input
		destinationImage: ảnh output
	Hàm trả về
		0: nếu chuyển thành công
		1: nếu chuyển thất bại (không đọc được ảnh input,...)
	*/
	int HSV2RGB(const Mat& sourceImage, Mat& destinationImage)
	{
		if (sourceImage.empty())
			return 0;

		//Số dòng và cột
		int nl = sourceImage.rows;
		int nc = sourceImage.cols;

		destinationImage = sourceImage.clone();
		for (int i = 0; i < nl; i++)
		{
			//Lấy địa chỉ của dòng thứ i
			const uchar* data = sourceImage.ptr<uchar>(i);
			uchar* data2 = destinationImage.ptr<uchar>(i);

			for (int j = 0; j < nc; j++)
			{
				float R, G, B, H, S, V, C, X, m;
				H = data[j * 3 + 0] * 2.0f;
				S = data[j * 3 + 1] / 255.0f;
				V = data[j * 3 + 2] / 255.0f;

				C = V * S; //Chroma: sắc độ
				X = C * (1 - abs((int)(H / 60) % 2 - 1)); //Giá trị trung gian để tính RGB
				m = V - C; //Thành phần RGB mà có giá trị min

				if (H < 60)
				{
					R = C;
					G = X;
					B = 0;
				}
				else if (H < 120)
				{
					R = X;
					G = C;
					B = 0;
				}
				else if (H < 180)
				{
					R = 0;
					G = C;
					B = X;
				}
				else if (H < 240)
				{
					R = 0;
					G = X;
					B = C;
				}
				else if (H < 300)
				{
					R = X;
					G = 0;
					B = C;
				}
				else
				{
					R = C;
					G = 0;
					B = X;
				}

				data2[j * 3 + 2] = (uchar)((R + m) * 255);
				data2[j * 3 + 1] = (uchar)((G + m) * 255);
				data2[j * 3 + 0] = (uchar)((B + m) * 255);
			}
		}
		return 1;
	}



public:
	/*
	Hàm chuyển đổi không gian màu của ảnh
	sourceImage: ảnh input
	destinationImage: ảnh output cùng kích thước, cùng loại với ảnh input
	type: loại chuyển đổi
		0, 1: chuyển từ RGB sang GrayScale và ngược lại
		2, 3: chuyển từ RGB sang HSV và ngược lại
	Hàm trả về
		0: nếu chuyển thành công
		1: nếu chuyển thất bại (không đọc được ảnh input hay type không chính xác,...)
	*/
	int Convert(const Mat& sourceImage, Mat& destinationImage, int type)
	{
		if (sourceImage.empty())
			return 1;

		if (type == 0)
			return RGB2GrayScale(sourceImage, destinationImage);
		else if (type == 1)
			return GrayScale2RGB(sourceImage, destinationImage);
		else if (type == 2)
			return RGB2HSV(sourceImage, destinationImage);
		else if (type == 3)
			return HSV2RGB(sourceImage, destinationImage);

		return 0;
	}

	Converter()
	{
	}
	~Converter() 
	{
	}
};