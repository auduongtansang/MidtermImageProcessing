#include "opencv2/opencv.hpp"
#include "AvailableFunction.h"
#include "ColorTransformer.h"
#include "EdgeDetector.h"
#include "Converter.h"
#include "Blur.h"
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	if (argc < 2)
		return EXIT_FAILURE;
	if ((strcmp(argv[1], "--bright") == 0 || strcmp(argv[1], "--contrast") == 0) && (argc < 4))
		return EXIT_FAILURE;
	if ((strcmp(argv[1], "--mean") == 0 || strcmp(argv[1], "--median") == 0 || strcmp(argv[1], "--gauss") == 0) && (argc < 5))
		return EXIT_FAILURE;

	/*
	Đọc ảnh màu RGB
	Ở những yêu cầu có đầu vào là ảnh xám, hoặc ảnh HSV thì dùng hàm chuyển
	*/
	Mat image = imread(argv[2], IMREAD_COLOR);
	Mat destinationImage;

	ColorTransformer colorTransformer;
	Converter converter;
	EdgeDetector edgeDetector;
	Blur blur;

	if (strcmp(argv[1], "--contrast") == 0)
	{
		imshow("Source (RGB)", image);
		float f = stof(argv[3]);

		if (colorTransformer.ChangeContrast(image, destinationImage, f) == 1)
			imshow("Contrast changed image", destinationImage);
	}
	else if (strcmp(argv[1], "--bright") == 0)
	{
		imshow("Source (RGB)", image);
		short b = (short)(stoi(argv[3]));

		if (colorTransformer.ChangeBrighness(image, destinationImage, b) == 1)
			imshow("Bright changed image", destinationImage);
	}
	else if (strcmp(argv[1], "--hist") == 0)
	{
		Mat histMatrix;

		//Ảnh màu
		imshow("Source (RGB)", image);
		colorTransformer.CalcHistogram(image, histMatrix);
		cout << "Intensity: blue green red" << endl;
		for (int i = 0; i < histMatrix.cols; i++)
			cout << i << ": " << histMatrix.at<unsigned short>(0, i) << "_" << histMatrix.at<unsigned short>(1, i) << "_" << histMatrix.at<unsigned short>(2, i) << endl;
		
		//Ảnh xám
		converter.Convert(image, image, 0);
		imshow("Source (GRAY)", image);
		colorTransformer.CalcHistogram(image, histMatrix);
		cout << "\nIntensity: graylevel" << endl;
		for (int i = 0; i < histMatrix.cols; i++)
			cout << i << ": " << histMatrix.at<unsigned short>(0, i) << endl;
	}
	else if (strcmp(argv[1], "--drawhist") == 0)
	{
		Mat hist_src, histImage;

		//Ảnh màu
		imshow("Source (RGB)", image);
		colorTransformer.CalcHistogram(image, hist_src);
		if (colorTransformer.DrawHistogram(hist_src, histImage) == 1)
			imshow("Color histogram", histImage);

		//Ảnh xám
		converter.Convert(image, image, 0);
		imshow("Source (GRAY)", image);
		colorTransformer.CalcHistogram(image, hist_src);
		if (colorTransformer.DrawHistogram(hist_src, histImage) == 1)
			imshow("Gray histogram", histImage);
	}
	else if (strcmp(argv[1], "--equalhist") == 0)
	{
		Mat desitationImage;

		//Ảnh màu
		imshow("Source (RGB)", image);
		if (colorTransformer.HistogramEqualization(image, desitationImage) == 1)
		{
			imshow("Color equalized image", desitationImage);
			//Vẽ đồ thị cân bằng
			Mat equalized_colorImage_Hist, des_equalized_colorImage_Hist;
			colorTransformer.CalcHistogram(desitationImage, equalized_colorImage_Hist);
			colorTransformer.DrawHistogram(equalized_colorImage_Hist, des_equalized_colorImage_Hist);
			imshow("Color equalized histogram", des_equalized_colorImage_Hist);
		}

		//Ảnh xám
		converter.Convert(image, image, 0);
		imshow("Source (GRAY)", image);
		if (colorTransformer.HistogramEqualization(image, desitationImage) == 1)
		{
			imshow("Gray equalized image", desitationImage);
			//Vẽ đồ thị cân bằng 
			Mat equalized_grayImage_Hist, des_equalized_grayImage_Hist;
			colorTransformer.CalcHistogram(desitationImage, equalized_grayImage_Hist);
			colorTransformer.DrawHistogram(equalized_grayImage_Hist, des_equalized_grayImage_Hist);
			imshow("Gray equalized histogram", des_equalized_grayImage_Hist);
		}
	}
	else if (strcmp(argv[1], "--compare") == 0)
	{
		Mat image1 = imread(argv[2], IMREAD_GRAYSCALE);
		Mat image2 = imread(argv[3], IMREAD_GRAYSCALE);
		imshow("Image1", image1);
		imshow("Image2", image2);
		cout << "Euclidean distance: " << colorTransformer.CompareImage(image1, image2) << endl;
	}
	else if (strcmp(argv[1], "--rgb2gray") == 0)
	{
		imshow("Source (RGB)", image);
		if (converter.Convert(image,destinationImage, 0) == 1)
			imshow("Destination (GRAY)", destinationImage);
	}
	else if (strcmp(argv[1], "--gray2rgb") == 0)
	{
		converter.Convert(image, image, 0);
		imshow("Source (GRAY)", image);
		if (converter.Convert(image, destinationImage, 1) == 1)
			imshow("Destination (RGB)", destinationImage);
	}
	else if (strcmp(argv[1], "--rgb2hsv") == 0)
	{
		imshow("Source (RGB)", image);
		if (converter.Convert(image, destinationImage, 2) == 1)
			imshow("Destination (HSV)", destinationImage);
	}
	else if (strcmp(argv[1], "--hsv2rgb") == 0)
	{
		converter.Convert(image, image, 2);
		imshow("Source (HSV)", image);
		if (converter.Convert(image, destinationImage, 3) == 1)
			imshow("Destination (RGB)", destinationImage);
	}
	else if (strcmp(argv[1], "--mean") == 0)
	{
		converter.Convert(image, image, 0);
		imshow("Source (GRAY)", image);
		if (blur.BlurImage(image, destinationImage, stoi(argv[3]), stoi(argv[4]), 0) == 0)
			imshow("Mean blured image", destinationImage);
	}
	else if (strcmp(argv[1], "--median") == 0)
	{
		converter.Convert(image, image, 0);
		imshow("Source (GRAY)", image);
		if (blur.BlurImage(image, destinationImage, stoi(argv[3]), stoi(argv[4]), 1) == 0)
			imshow("Median blured image", destinationImage);
	}
	else if (strcmp(argv[1], "--gauss") == 0)
	{
		converter.Convert(image, image, 0);
		imshow("Source (GRAY)", image);
		if (blur.BlurImage(image, destinationImage, stoi(argv[3]), stoi(argv[4]), 2) == 0)
			imshow("Gaussian blured image", destinationImage);
	}
	else if (strcmp(argv[1], "--sobel") == 0)
	{
		converter.Convert(image, image, 0);
		imshow("Source (GRAY)", image);
		if (edgeDetector.DetectEdge(image, destinationImage, 3, 3, 1) == 0)
			imshow("Sobel edge detected image", destinationImage);
	}
	else if (strcmp(argv[1], "--prewitt") == 0)
	{
		converter.Convert(image, image, 0);
		imshow("Source (GRAY)", image);
		if (edgeDetector.DetectEdge(image, destinationImage, 3, 3, 2) == 0)
			imshow("Prewitt edge detected image", destinationImage);
	}
	else if (strcmp(argv[1], "--laplace") == 0)
	{
		converter.Convert(image, image, 0);
		imshow("Source (GRAY)", image);
		if (edgeDetector.DetectEdge(image, destinationImage, 3, 3, 3) == 0)
			imshow("Laplacian edge detected image", destinationImage);
	}
	
	waitKey(0);
	return EXIT_SUCCESS;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

//Y = 0.2126 × R + 0.7152 × G + 0.0722 × B