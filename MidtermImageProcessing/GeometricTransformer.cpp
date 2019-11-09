#pragma once
#include "GeometricTransformer.h"

#define MIN -10000
#define MAX 10000

int GeometricTransformer::Transform(const Mat & beforeImage, Mat & afterImage, AffineTransform * transformer, PixelInterpolate * interpolator)
{
	if (beforeImage.rows == 0 || beforeImage.cols == 0)
		return 0;
	if (transformer == NULL)
		return 0;
	//lấy ma trận của affine thuận
	Mat* affine_matrix = new Mat(transformer->get_AffineTransform());
	// biến đổi ma trận vừa lấy thành ma trận ngịch đảo
	Mat* inverse_affine_matrix = new Mat(affine_matrix->inv());
	// gán lại ma trận nghịch đảo vào lớp biến đổi affine
	AffineTransform* inverse_transformer = new AffineTransform(*inverse_affine_matrix);

	//các dữ liệu cần thiết từ ảnh gốc
	uchar* pSrc = beforeImage.data;
	int nChannels = beforeImage.channels();
	int srcwidthStep = beforeImage.cols*nChannels;
	int src_rows = beforeImage.rows;
	int src_cols = beforeImage.cols;


	int x, y; // bien chay tung pixel
	float tx, ty; // bien su dung phep bien doi affine thuan
	float ix, iy; // bien su dung phep bien doi affine nguoc
	int tmp_x, tmp_y;


	//các dữ liệu cần thiết từ ảnh đích
	uchar* pDest = afterImage.data;
	int des_rows = afterImage.rows;
	int des_cols = afterImage.cols;
	int deswidthStep = afterImage.cols*nChannels;


	//nội suy
	uchar* interpolate_color;
	if (nChannels == 3)
		interpolate_color = new uchar[3];
	else
		interpolate_color = new uchar;
	uchar* black_color = NULL;
	if (nChannels == 3) {
		black_color = new uchar[3];
		*(black_color + 0) = 0;
		*(black_color + 1) = 0;
		*(black_color + 2) = 0;
	}
	else {
		black_color = new uchar;
		*black_color = 0;
	}
	for (x = 0; x < des_rows; x++) {
		for (y = 0; y < des_cols; y++) {
			tx = x;
			ty = y;
			// tìm ngược lại vị trí theo affine nghịch
			inverse_transformer->TransformPoint(tx, ty);

			if (floor(tx) == src_rows - 1 || floor(ty) == src_cols - 1)
				continue;
			if (0 <= tx && tx < src_rows && 0 <= ty && ty < src_cols) {// chỉ quan tâm đến điểm ảnh tồn tại
				interpolate_color = interpolator->Interpolate(tx, ty, pSrc, srcwidthStep, nChannels);
				if (nChannels == 3) {
					*(pDest + x*deswidthStep + y*nChannels + 0) = *(interpolate_color + 0); // B
					*(pDest + x*deswidthStep + y*nChannels + 1) = *(interpolate_color + 1); // G
					*(pDest + x*deswidthStep + y*nChannels + 2) = *(interpolate_color + 2); // R
				}
				else {
					*(pDest + x*deswidthStep + y*nChannels) = *interpolate_color;
				}
			}
			else { // nếu điểm ảnh không tồn tại thì gán màu đen
				if (nChannels == 3) {
					*(pDest + x*deswidthStep + y*nChannels + 0) = *(black_color + 0); // B
					*(pDest + x*deswidthStep + y*nChannels + 1) = *(black_color + 1); // G
					*(pDest + x*deswidthStep + y*nChannels + 2) = *(black_color + 2); // R
				}
				else {
					*(pDest + x*deswidthStep + y*nChannels) = *black_color;
				}
			}
		}
	}

	return 1;
}

int GeometricTransformer::RotateKeepImage(const Mat & srcImage, Mat & dstImage, float angle, PixelInterpolate * interpolator)
{
	//Không thể quay ảnh
	if (interpolator == NULL)
		return 0;
	if (srcImage.rows <= 0 || srcImage.cols <= 0)
		return 0;

	//lấy các thông tin cần thiết từ ảnh gốc
	uchar* pSrc = srcImage.data;
	int src_rows = srcImage.rows;
	int src_cols = srcImage.cols;
	int nChannel = srcImage.channels();

	//giảm độ về khoảng từ 0 đến 360 và đổi từ độ sang radian
	while (abs(angle) - 360 > 0) angle -= 360;
	angle = angle / (180.0)*3.14;

	// thiết lập affine quay
	float rotateArray[] = { cos(angle), -sin(angle), 0, sin(angle), cos(angle), 0, 0, 0, 1 };
	Mat rotate = Mat(3, 3, CV_32FC1, rotateArray).clone();
	AffineTransform* rotate_affine = new AffineTransform(rotate);

	/*
	* tính toán độ to của ảnh sau khi quay
	* dựa vào các điểm ở góc của ảnh
	*/ 
	// góc dưới trái
	float x1 = src_rows - 1;
	float y1 = 0;

	// góc dưới phải
	float x2 = src_rows - 1;
	float y2 = src_cols - 1;

	// góc trên phải
	float x3 = 0;
	float y3 = src_cols - 1;

	// góc trên trái
	float x4 = 0;
	float y4 = 0;
	
	// thực hiện phép quay đối với các điểm trên để tính tọa độ ảnh mới
	rotate_affine->TransformPoint(x1, y1);
	rotate_affine->TransformPoint(x2, y2);
	rotate_affine->TransformPoint(x3, y3);

	float x_max = x1, y_max = y1, x_min = x1, y_min = y1; // tìm kiếm kích thước lớn nhất có thể có của ảnh
	if (x_max < x2)
		x_max = x2;
	if (y_max < y2)
		y_max = y2;

	if (x_max < x3)
		x_max = x3;
	if (y_max < y3)
		y_max = y3;

	if (x_max < x4)
		x_max = x4;
	if (y_max < y4)
		y_max = y4;

	if (x_min > x2)
		x_min = x2;
	if (y_min > y2)
		y_min = y2;

	if (x_min > x3)
		x_min = x3;
	if (y_min > y3)
		y_min = y3;

	if (x_min > x4)
		x_min = x4;
	if (y_min > y4)
		y_min = y4;


	float dx, dy; // vecto tịnh tiến
	Mat translate, backward_translate, scale;
	//xét trường hợp các góc quay để thiết lập affine tịnh tiến cho phù hợp
	//TH1: y_min = 0, x_min <= 0
	if (x_min < 0 && y_min == 0) {
		x_min = floor(x_min);
		dx = -x_min;
		dy = 0;
		float translateArray[] = { 1, 0, dx, 0, 1, dy, 0, 0, 1 };
		translate = Mat(3, 3, CV_32FC1, translateArray).clone();
	}
	//TH2: x_min < 0, y_min < 0
	else if (x_min < 0 && y_min < 0) {
		x_min = floor(x_min);
		y_min = floor(y_min);
		dx = -x_min;
		dy = -y_min;
		float translateArray[] = { 1, 0, dx, 0, 1, dy, 0, 0, 1 };
		translate = Mat(3, 3, CV_32FC1, translateArray).clone();
	}
	//TH3: x_min >= 0, y_min <= 0
	else if (x_min >= 0 && y_min <= 0) {
		std::cout << x_min << "     " << y_min << std::endl;
		x_min = floor(x_min);
		y_min = floor(y_min);
		dx = 0;
		dy = -y_min;
		float translateArray[] = { 1, 0, dx, 0, 1, dy, 0, 0, 1 };
		translate = Mat(3, 3, CV_32FC1, translateArray).clone();
	}

	//
	x_max = dx + ceil(x_max);
	y_max = dy + ceil(y_max);

	////thiết lập affine giảm kích thước ảnh
	//float sx = src_rows*1.0 / x_max;
	//float sy = src_cols*1.0 / y_max;
	//float scaleArray[] = { sx, 0, 0, 0, sy, 0, 0, 0, 1 };
	//scale = Mat(3, 3, CV_32FC1, scaleArray).clone();

	// ma trận affine tổng hợp theo thứ tự thực hiện: (nếu muốn giữ nguyên kích thước ảnh(giảm kích thước ảnh -->)) tịnh tiến ảnh --> quay ảnh
	Mat res = translate*rotate;
	AffineTransform* res_affine = new AffineTransform(res);

	// tạo ma trận ảnh mới
	if (nChannel == 3)
		dstImage = Mat(x_max, y_max, CV_8UC3);
	else
		dstImage = Mat(x_max, y_max, CV_8UC1);

	// thực hiện biến đổi affine thuận và nghịch, nếu không biến đổi thành công thì chekc  = 0
	int check = Transform(srcImage, dstImage, res_affine, interpolator);

	if (check == 0) {
		return 0;
	}
	return 1;
}

int GeometricTransformer::RotateUnkeepImage(const Mat & srcImage, Mat & dstImage, float angle, PixelInterpolate * interpolator)
{
	//Không thể quay ảnh
	if (interpolator == NULL)
		return 0;
	if (srcImage.rows <= 0 || srcImage.cols <= 0)
		return 0;

	//lấy các thông tin cần thiết từ ảnh gốc
	uchar* pSrc = srcImage.data;
	int src_rows = srcImage.rows;
	int src_cols = srcImage.cols;
	int nChannel = srcImage.channels();

	//giảm độ về khoảng từ 0 đến 360 và đổi từ độ sang radian
	while (abs(angle) - 360 > 0) angle -= 360;
	angle = angle / (180.0)*3.14;

	// thiết lập affine quay
	float rotateArray[] = { cos(angle), -sin(angle), 0, sin(angle), cos(angle), 0, 0, 0, 1 };
	Mat rotate = Mat(3, 3, CV_32FC1, rotateArray).clone();
	AffineTransform* rotate_affine = new AffineTransform(rotate);

	Mat translate, backward_translate, scale;

	// thiết lập affine tịnh tiến tâm
	float dx, dy; // vecto tịnh tiến
	dx = src_rows/2;
	dy = src_cols/2;
	float translateArray[] = { 1, 0, dx, 0, 1, dy, 0, 0, 1 };
	translate = Mat(3, 3, CV_32FC1, translateArray).clone();

	// thiết lập affine tính tiến ngược tâm
	float idx = -dx;
	float idy = -dy;
	float backward_translateArray[] = { 1, 0, idx, 0, 1, idy, 0, 0, 1 };
	backward_translate = Mat(3, 3, CV_32FC1, backward_translateArray).clone();

	// ma trận affine tổng hợp theo thứ tự thực hiện: tịnh tiến tâm ảnh --> quay ảnh --> tinh tiến ngược ảnh
	Mat res = translate*rotate*backward_translate;
	AffineTransform* res_affine = new AffineTransform(res);

	// tạo ma trận ảnh mới
	if (nChannel == 3)
		dstImage = Mat(src_rows, src_cols, CV_8UC3);
	else
		dstImage = Mat(src_rows, src_cols, CV_8UC1);

	// thực hiện biến đổi affine thuận và nghịch, nếu không biến đổi thành công thì chekc  = 0
	int check = Transform(srcImage, dstImage, res_affine, interpolator);

	if (check == 0) {
		return 0;
	}
	return 1;
}

int GeometricTransformer::Scale(const Mat & srcImage, Mat & dstImage, float sx, float sy, PixelInterpolate * interpolator)
{
	//Không thể zoom ảnh
	if (interpolator == NULL)
		return 0;
	if (srcImage.rows <= 0 || srcImage.cols <= 0)
		return 0;
	if (sx <= 0 || sy <= 0)
		return 0;

	//lấy các thông tin cần thiết từ ảnh gốc
	uchar* pSrc = srcImage.data;
	int src_rows = srcImage.rows;
	int src_cols = srcImage.cols;
	int nChannel = srcImage.channels();

	// thiết lập affine thay đổi kích thước
	float scaleArray[] = { sx, 0, 0, 0, sy, 0, 0, 0, 1 };
	Mat scale = Mat(3, 3, CV_32FC1, scaleArray).clone();
	AffineTransform* scale_affine = new AffineTransform(scale);

	if (nChannel == 3)
		dstImage = Mat(src_rows, src_cols, CV_8UC3);
	else
		dstImage = Mat(src_rows, src_cols, CV_8UC1);

	// thực hiện biến đổi affine thuận và nghịch, nếu không biến đổi thành công thì chekc  = 0
	int check = Transform(srcImage, dstImage, scale_affine, interpolator);

	if (check == 0) {
		return 0;
	}
	return 1;
}

int GeometricTransformer::Resize(const Mat & srcImage, Mat & dstImage, int newWidth, int newHeight, PixelInterpolate * interpolator)
{
	//Không thể thay đổi kích thước ảnh
	if (interpolator == NULL)
		return 0;
	if (srcImage.rows <= 0 || srcImage.cols <= 0)
		return 0;
	if (newWidth <= 0 || newHeight <= 0)
		return 0;

	//lấy các thông tin cần thiết từ ảnh gốc
	uchar* pSrc = srcImage.data;
	int src_rows = srcImage.rows;
	int src_cols = srcImage.cols;
	int nChannel = srcImage.channels();

	// thiết lập affine thay đổi kích thước
	float sx = (1.0*newHeight) / (1.0*src_rows);
	float sy = (1.0*newWidth) / (1.0*src_cols);
	float scaleArray[] = { sx, 0, 0, 0, sy, 0, 0, 0, 1 };
	Mat scale = Mat(3, 3, CV_32FC1, scaleArray).clone();
	AffineTransform* scale_affine = new AffineTransform(scale);

	if (nChannel == 3)
		dstImage = Mat(ceil(src_rows*sx), ceil(src_cols*sy), CV_8UC3);
	else
		dstImage = Mat(ceil(src_rows*sx), ceil(src_cols*sy), CV_8UC1);

	// thực hiện biến đổi affine thuận và nghịch, nếu không biến đổi thành công thì chekc  = 0
	int check = Transform(srcImage, dstImage, scale_affine, interpolator);

	if (check == 0) {
		return 0;
	}
	return 1;
}

int GeometricTransformer::Flip(const Mat & srcImage, Mat & dstImage, bool direction, PixelInterpolate * interpolator)
{
	//Không thể lật ảnh
	if (interpolator == NULL)
		return 0;
	if (direction < 0 || direction > 1)
		return 0;
	if (srcImage.rows <= 0 || srcImage.cols <= 0)
		return 0;

	//lấy các thông tin cần thiết từ ảnh gốc
	uchar* pSrc = srcImage.data;
	int src_rows = srcImage.rows;
	int src_cols = srcImage.cols;
	int nChannel = srcImage.channels();

	Mat backward_translate, posToneg;

	float dx, dy; // vecto tịnh tiến
	dx = src_rows;
	dy = src_cols;


	// thiết lập affine tính tiến ngược trục Ox hoặc Oy
	float idx = -dx;
	float idy = -dy;
	float backward_translateArray_Ox[] = { 1, 0, -idx, 0, 1, 0, 0, 0, 1 };
	float backward_translateArray_Oy[] = { 1, 0, 0, 0, 1, -idy, 0, 0, 1 };
	if (direction == 1) {
		backward_translate = Mat(3, 3, CV_32FC1, backward_translateArray_Ox).clone();
	}
	else {
		backward_translate = Mat(3, 3, CV_32FC1, backward_translateArray_Oy).clone();
	}

	// thiết lập affine biến âm thành dương và dương thành âm
	float posToneg_Ox[] = { -1, 0, 0, 0, 1, 0, 0, 0, 1 };
	float posToneg_Oy[] = { 1, 0, 0, 0, -1, 0, 0, 0, 1 };
	if (direction == 1) {
		posToneg = Mat(3, 3, CV_32FC1, posToneg_Ox).clone();
	}
	else {
		posToneg = Mat(3, 3, CV_32FC1, posToneg_Oy).clone();
	}

	// ma trận affine tổng hợp theo thứ tự thực hiện: tịnh tiến tâm ảnh --> quay ảnh --> tinh tiến ngược ảnh
	Mat res = backward_translate*posToneg;
	AffineTransform* res_affine = new AffineTransform(res);

	// tạo ma trận ảnh mới
	if (nChannel == 3)
		dstImage = Mat(src_rows, src_cols, CV_8UC3);
	else
		dstImage = Mat(src_rows, src_cols, CV_8UC1);

	// thực hiện biến đổi affine thuận và nghịch, nếu không biến đổi thành công thì chekc  = 0
	int check = Transform(srcImage, dstImage, res_affine, interpolator);

	if (check == 0) {
		return 0;
	}
	return 1;
}

GeometricTransformer::GeometricTransformer()
{
}

GeometricTransformer::~GeometricTransformer()
{
}
