#include "opencv2/opencv.hpp"
#include "AvailableFunction.h"
#include "ColorTransformer.h"
#include "EdgeDetector.h"
#include "Converter.h"
#include "Blur.h"
#include "GeometricTransformer.h"
#include <string>
#include <iostream>
using namespace cv; 
using namespace std;

void getInfomation(int n_ARGUMENT, char **ARGUMENTS, 
	char * &IN_PATH, char * &TASK, char *&INTER_METHOD, vector<string> &OTHER_ARGV);

int main(int argc, char **argv)
{
	/*	These following variables is used for loading Image InputPath, Task-to-do, Interpolation methods
		For more description, please check the Lab01,02,03 instruction
	*/
	char * input_path = NULL, * task = NULL, * interpolate = NULL;
	vector<string> other_argv;
	
	/*Parsing Arguments here*/
	getInfomation(argc, argv, input_path, task, interpolate, other_argv);

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
	Mat image = imread(input_path, IMREAD_COLOR);
	Mat destinationImage;

	ColorTransformer colorTransformer;
	Converter converter;
	EdgeDetector edgeDetector;
	Blur blur;
	PixelInterpolate* type_interpolation;
	GeometricTransformer* geotransform;
	/*
		Detect Interpolation methods to create appropriate Class
	*/
	if (interpolate != NULL)
	{
		if (strcmp(interpolate, "--bl") == 0)
			type_interpolation = new BilinearInterpolate;
		else
			type_interpolation = new NearestNeighborInterpolate;
	}
	else
		type_interpolation = NULL;
	/*
		To do task required
	*/

	// Các task của Lab01
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
	// Các task của Lab03
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
	// Các task của Lab02
	else if (strcmp(argv[1], "--zoom") == 0)
	{
		float sx = stof(other_argv[0]);
		float sy = stof(other_argv[1]);
		imshow("Source (Original)", image);
		geotransform = new GeometricTransformer;
		if (geotransform->Scale(image, destinationImage, sx, sy, type_interpolation) == 1)
			imshow("Image after Zooming", destinationImage);
	}
	else if (strcmp(argv[1], "--resize") == 0)
	{
		int height, width;
		height = stoi(other_argv[0]);
		width = stoi(other_argv[1]);
		imshow("Source (Original)", image);
		geotransform = new GeometricTransformer;
		if (geotransform->Resize(image, destinationImage, width, height, type_interpolation) == 1)
			imshow("Image after Zooming", destinationImage);
	}
	else if (strcmp(argv[1], "--rotate") == 0)
	{
		float angle = stof(other_argv[0]);
		imshow("Source (Original)", image);
		geotransform = new GeometricTransformer;
		if (geotransform->RotateKeepImage(image, destinationImage, angle, type_interpolation) == 1)
			imshow("Image after Rotate", destinationImage);
	}
	else if (strcmp(argv[1], "--rotateN") == 0)
	{
		float angle = stof(other_argv[0]);
		imshow("Source (Original)", image);
		geotransform = new GeometricTransformer;
		if (geotransform->RotateUnkeepImage(image, destinationImage, angle, type_interpolation) == 1)
			imshow("Image after UnkeepRotate", destinationImage);
	}
	else if (strcmp(argv[1], "--flip") == 0)
	{
		bool axis = (other_argv[0] == "Ox") ? 1 : 0;
		imshow("Source (Original)", image);
		geotransform = new GeometricTransformer;
		if (geotransform->Flip(image, destinationImage, axis, type_interpolation) == 1)
			imshow("Image after UnkeepRotate", destinationImage);
	}
	waitKey(0);
	
	/*getInfomation(argc, argv, input_path, task, interpolate, other_argv);
	cout << task << ' ' << input_path << ' ' <<interpolate << ' ';
	for (auto item : other_argv)
		cout << item << ' ';*/
	//return EXIT_SUCCESS;

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

void getInfomation(int n_ARGUMENT, char **ARGUMENTS, char *&IN_PATH, char *&TASK, char *&INTER_METHOD, vector<string> &OTHER_ARGV)
{
	int start_other; // This variable is used for checking the starting index of other_argv
	TASK = new char[strlen(ARGUMENTS[1])];
	strcpy(TASK, ARGUMENTS[1]);
	if (strcmp(ARGUMENTS[2], "--bl") == 0 || strcmp(ARGUMENTS[2], "--nn") == 0)
	{
		INTER_METHOD = new char[strlen(ARGUMENTS[2])];
		strcpy(INTER_METHOD, ARGUMENTS[2]);
		IN_PATH = new char[strlen(ARGUMENTS[3])];
		strcpy(IN_PATH, ARGUMENTS[3]);
		start_other = 4;
	}
	else
	{	
		INTER_METHOD = NULL;
		IN_PATH = new char[strlen(ARGUMENTS[2])];
		strcpy(IN_PATH, ARGUMENTS[2]);
		start_other = 3;
	}
	for (unsigned int index=start_other; index < n_ARGUMENT; index++)
		OTHER_ARGV.push_back(ARGUMENTS[index]);
}
