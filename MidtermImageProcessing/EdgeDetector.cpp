#include "EdgeDetector.h"

EdgeDetector::EdgeDetector()
{
}

EdgeDetector::~EdgeDetector()
{
}

int EdgeDetector::DetectEdge(const Mat& sourceImage, Mat& destinationImage, int kernelWidth, int kernelHeight, int method)
{
	/*
	Nếu là method 3, chỉ cần 1 kernel:
		|1  1 1|
		|1 -8 1|
		|1  1 1|
	Nhân chập rồi dò zero-crossing
	*/
	if (method == 3)
	{
		Convolution convolution;
		vector<int> kernel;
		Mat laplacian;

		kernel.push_back(1); kernel.push_back(1); kernel.push_back(1);

		kernel.push_back(1); kernel.push_back(-8); kernel.push_back(1);

		kernel.push_back(1); kernel.push_back(1); kernel.push_back(1);

		convolution.SetKernel(kernel, 1.0f, kernelWidth, kernelHeight);
		convolution.DoConvolution(sourceImage, laplacian);

		/*
		Tìm điểm ảnh zero-crossing
		Duyệt tất cả điểm ảnh
		Với mỗi điểm ảnh, lần lượt bắt cặp với 3 lân cận của nó: phải, dưới, phải-dưới
			Nếu có bất kì cặp nào thỏa: giá trị của chúng trái dấu và chênh lệnh giá trị của chúng lớn hơn hay bằng threshold
			Thì điểm đang xét thuộc biên cạnh
		Thao tác duyệt tương tự hàm nhân chập (đọc comment hàm nhân chập để hiểu cách duyệt)
		*/
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC1, Scalar(0));
		int rowStep = (int)(destinationImage.step[0]);

		short* pRowLap = (short*)laplacian.data;
		uchar* pRowDst = destinationImage.data;
		float threshold = 150;

		for (int i = 0; i < destinationImage.rows - 1; i++, pRowLap += rowStep, pRowDst += rowStep)
		{
			short* pDataLap = pRowLap;
			uchar* pDataDst = pRowDst;
			for (int j = 0; j < destinationImage.cols - 1; j++, pDataLap++, pDataDst++)
			{
				if (*pDataLap * pDataLap[1] < 0 && abs(*pDataLap - pDataLap[1]) >= threshold)
					*pDataDst = 255;
				if (*pDataLap * pDataLap[rowStep] < 0 && abs(*pDataLap - pDataLap[rowStep]) >= threshold)
					*pDataDst = 255;
				if (*pDataLap * pDataLap[rowStep + 1] < 0 && abs(*pDataLap - pDataLap[rowStep + 1]) >= threshold)
					*pDataDst = 255;
			}
		}

		return 0;
	}

	//Cần 2 kernel cho mỗi method 1 và method 2
	Convolution convolution;
	vector<int> kernelX;
	vector<int> kernelY;
	float division = 1.0f;
	Mat dx, dy;

	//Tạo kernel nhân chập
	if (method == 1)
	{
		/*
					1	|1  0  -1|
			Kx =	- *	|2  0  -2|
					4	|1  0  -1|
		*/
		kernelX.push_back(1); kernelX.push_back(0); kernelX.push_back(-1);

		kernelX.push_back(2); kernelX.push_back(0); kernelX.push_back(-2);

		kernelX.push_back(1); kernelX.push_back(0); kernelX.push_back(-1);

		/*
					1	|-1  -2  -1|
			Ky =	- *	| 0   0   0|
					4	| 1   2   1|
		*/

		kernelY.push_back(-1); kernelY.push_back(-2); kernelY.push_back(-1);

		kernelY.push_back(0); kernelY.push_back(0); kernelY.push_back(0);

		kernelY.push_back(1); kernelY.push_back(2); kernelY.push_back(1);

		division = 4.0f;
	}
	else if (method == 2)
	{
		/*
					1	|1  0  -1|
			Kx =	- *	|1  0  -1|
					3	|1  0  -1|
		*/
		kernelX.push_back(1); kernelX.push_back(0); kernelX.push_back(-1);

		kernelX.push_back(1); kernelX.push_back(0); kernelX.push_back(-1);

		kernelX.push_back(1); kernelX.push_back(0); kernelX.push_back(-1);

		/*
					1	|-1  -1  -1|
			Ky =	- *	| 0   0   0|
					3	| 1   1   1|
		*/

		kernelY.push_back(-1); kernelY.push_back(-1); kernelY.push_back(-1);

		kernelY.push_back(0); kernelY.push_back(0); kernelY.push_back(0);

		kernelY.push_back(1); kernelY.push_back(1); kernelY.push_back(1);

		division = 3.0f;
	}

	convolution.SetKernel(kernelX, division, kernelWidth, kernelHeight);
	convolution.DoConvolution(sourceImage, dx);

	convolution.SetKernel(kernelY, division, kernelWidth, kernelHeight);
	convolution.DoConvolution(sourceImage, dy);

	/*
	Trích chọn điểm ảnh thuộc biên bằng độ lớn của vector gradient:
		e(x, y) = (dx ^ 2 + dy ^ 2) ^ (1/2)
	Những điểm ảnh có giá trị e lớn hơn hay bằng threshold sẽ thuộc biên cạnh
	Tuy nhiên phép lấy căn rất chậm => không cần lấy căn, bình phương threshold lên
	Thao tác duyệt tương tự hàm nhân chập (đọc comment hàm nhân chập để hiểu cách duyệt)
	*/
	destinationImage = Mat(sourceImage.rows, sourceImage.cols, CV_8UC1, Scalar(0));
	int rowStep = (int)(destinationImage.step[0]);

	short* pRowX = (short*)dx.data;
	short* pRowY = (short*)dy.data;
	uchar* pRow = destinationImage.data;
	int threshold = 1024;

	for (int i = 0; i < destinationImage.rows; i++, pRowX += rowStep, pRowY += rowStep, pRow += rowStep)
	{
		short* pDataX = pRowX;
		short* pDataY = pRowY;
		uchar* pData = pRow;
		for (int j = 0; j < destinationImage.cols; j++, pDataX++, pDataY++, pData++)
			if (*pDataX * *pDataX + *pDataY * *pDataY >= threshold)
				*pData = 255;
	}

	return 0;
}