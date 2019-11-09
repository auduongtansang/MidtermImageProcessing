#pragma once
#include "GeometricTransformer.h"

PixelInterpolate::PixelInterpolate()
{

}

PixelInterpolate::~PixelInterpolate()
{
}
/***************************************/
uchar* BilinearInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels)
{
	int small_x = floor(tx);
	int small_y = floor(ty);


	float a = tx - 1.0*small_x;
	float b = ty - 1.0*small_y;

	/* Tính nội suy theo công thức
	g(tx,ty) = (1-a)*(1-b)*f(small_x,small_y) + a*(1-b)*f(small_x + 1, small_y) + b*(1 - a)*f(small_x, small_y + 1) + a*b*f(small_x + 1, small_y + 1)
	*/
	uchar* tmp = NULL;
	
	if (nChannels == 3) {
		tmp = new uchar[3];
		uchar cal = 0;
		for (int i = 0; i < 3; i++) {
			cal = 0;
			// *tmp = f(small_x, small_y)
			cal = *(pSrc + small_x*srcWidthStep + small_y*nChannels + i);
			// *tmp = (1 - a)*(1 - b)*f(small_x, small_y)
			cal = 1.0*(cal)*(1.0 - a)*(1.0 - b);

			// *tmp = *tmp + a*(1-b)*f(small_x + 1, small_y)
			cal += (*(pSrc + (small_x + 1)*srcWidthStep + small_y*nChannels + i))*1.0*a*(1.0 - b);

			// *tmp = *tmp + b*(1 - a)*f(small_x, small_y + 1)
			cal += (*(pSrc + small_x*srcWidthStep + (small_y + 1)*nChannels + i))*1.0*b*(1.0 - a);

			// *tmp = *tmp + a*b*f(small_x + 1, small_y + 1)
			cal += (*(pSrc + (small_x + 1)*srcWidthStep + (small_y + 1)*nChannels + i))*a*b;
			*(tmp + i) = cal;
		}
	}
	else {
		tmp = new uchar;
		uchar cal = 0;

		// *tmp = f(small_x, small_y)
		cal = *(pSrc + small_x*srcWidthStep + small_y*nChannels);
		// *tmp = (1 - a)*(1 - b)*f(small_x, small_y)
		cal = 1.0*(cal)*(1.0 - a)*(1.0 - b);

		// *tmp = *tmp + a*(1-b)*f(small_x + 1, small_y)
		cal += (*(pSrc + (small_x + 1)*srcWidthStep + small_y*nChannels))*1.0*a*(1.0 - b);

		// *tmp = *tmp + b*(1 - a)*f(small_x, small_y + 1)
		cal += (*(pSrc + small_x*srcWidthStep + (small_y + 1)*nChannels))*1.0*b*(1.0 - a);

		// *tmp = *tmp + a*b*f(small_x + 1, small_y + 1)
		cal += (*(pSrc + (small_x + 1)*srcWidthStep + (small_y + 1)*nChannels))*1.0*a*b;
		*tmp = cal;
	}

	return tmp;
}

BilinearInterpolate::BilinearInterpolate()
{
}

BilinearInterpolate::~BilinearInterpolate()
{
}

/*****************************************/
uchar* NearestNeighborInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels)
{
	int closest_x = round(tx);
	int closest_y = round(ty);

	return (pSrc + closest_x*srcWidthStep + closest_y*nChannels);
}

NearestNeighborInterpolate::NearestNeighborInterpolate()
{
}

NearestNeighborInterpolate::~NearestNeighborInterpolate()
{
}
