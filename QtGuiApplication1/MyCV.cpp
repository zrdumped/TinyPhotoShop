#include <MyCV.h>
#include <CVassist.h>
#include <stack>
#include <queue>
#define hist_size 256
#define gray_scale 256
#define MAX_GRAY 255
//pixel per gray value
#define scale 1
#define hist_height 150
#define Pi 3.1415926
using namespace std;




//将rgb转换为灰度图
void MyCvtRGB2GRAY(Mat src, Mat dst) {
	//遍历所有节点
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			Vec4b pt = src.at<Vec4b>(i, j);
			//opencv公式
			int gray = pt[2] * 0.299 + pt[1] * 0.587 + pt[0] * 0.114;
			dst.at<Vec4b>(i, j)[0] = gray;
			dst.at<Vec4b>(i, j)[1] = gray;
			dst.at<Vec4b>(i, j)[2] = gray;

		}
	}
}

//绘制灰度直方图
IplImage* MyCreateHist(Mat src, int mode ) {
	int hist[hist_size];
	for (int i = 0; i < hist_size; i++) {
		hist[i] = 0;
	}
	//遍历所有节点统计频率
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			Vec4b pt = src.at<Vec4b>(i, j);
			int gray = pt[mode];
			hist[gray] += 1;
		}
	}

	//获得最大频率值
	int max_gray = hist[0];
	for (int i = 1; i < hist_size; i++) {
		max_gray = (max_gray < hist[i]) ? hist[i] : max_gray;
	}

	IplImage* hist_image = cvCreateImage(cvSize(hist_size*scale, hist_height), 8, 3);
	cvZero(hist_image);

	//绘制直方图
	for (int i = 0; i<hist_size; i++)
	{
		float bin_val = hist[i]; //像素i的概率  
		int intensity = cvRound(bin_val * hist_height / max_gray);  //要绘制的高度  
		cvRectangle(hist_image,
			cvPoint(i*scale, hist_height - 1),
			cvPoint((i + 1)*scale - 1, hist_height - intensity),
			CV_RGB(255, 255, 255));
	}
	
	return hist_image;
}

//图片二值化
void MyBinary(Mat src, Mat dst, int threshold) {
	//遍历所有节点，判断与阈值的关系
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			Vec4b pt = src.at<Vec4b>(i, j);
			int gray = pt[2];
			if (gray < threshold) {
				dst.at<Vec4b>(i, j)[0] = 0;
				dst.at<Vec4b>(i, j)[1] = 0;
				dst.at<Vec4b>(i, j)[2] = 0;
			}
			else {
				dst.at<Vec4b>(i, j)[0] = 255;
				dst.at<Vec4b>(i, j)[1] = 255;
				dst.at<Vec4b>(i, j)[2] = 255;
			}

		}
	}
}
//双阈值二值化
void MyBinary(Mat src, Mat dst, int threshold, int threshold2) {
	int min;
	int max;
	if (threshold < threshold2) {
		min = threshold;
		max = threshold2;
	}
	else {
		min = threshold2;
		max = threshold;
	}
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			Vec4b pt = src.at<Vec4b>(i, j);
			int gray = pt[2];
			if (gray < min || gray > max) {
				dst.at<Vec4b>(i, j)[0] = 0;
				dst.at<Vec4b>(i, j)[1] = 0;
				dst.at<Vec4b>(i, j)[2] = 0;
			}
			else {
				dst.at<Vec4b>(i, j)[0] = 255;
				dst.at<Vec4b>(i, j)[1] = 255;
				dst.at<Vec4b>(i, j)[2] = 255;
			}

		}
	}
}
//改变通道，使用指定通道的值作为灰度值
void MyModifyRGBChannel(int channel, Mat src, Mat dst) {
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				Vec4b pt = src.at<Vec4b>(i, j);
				int value = pt[channel];
				dst.at<Vec4b>(i, j)[0] = value;
				dst.at<Vec4b>(i, j)[1] = value;
				dst.at<Vec4b>(i, j)[2] = value;
			}
	}
}

//大津算法确定阈值
int MyOtusThreshold(Mat src) {
	int width = src.cols;
	int height = src.rows;
	int pixelCount[gray_scale];
	float pixelPro[gray_scale];
	int i, j, pixelSum = width * height, threshold = 0;
	for (i = 0; i < gray_scale; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	//统计灰度级中每个像素在整幅图像中的个数  
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			Vec4b pt = src.at<Vec4b>(i, j);
			int gray = pt[2];
			pixelCount[gray] += 1;
		}
	}

	//计算每个像素在整幅图像中的比例  
	float maxPro = 0.0;
	int kk = 0;
	for (i = 0; i < gray_scale; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;
		if (pixelPro[i] > maxPro)
		{
			maxPro = pixelPro[i];
			kk = i;
		}
	}

	//遍历灰度级[0,255]  
	//w0 前景占比，w1 背景占比， u0 前景平均灰度， u1背景平均灰度， u 总平均灰度
	//delta（类间方差） = w0 (u0 - u) ^ 2 + w1 (u1- u) ^ 2
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < gray_scale; i++)     // i作为阈值
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for (j = 0; j < gray_scale; j++)
		{
			if (j <= i)   //背景部分  
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else   //前景部分  
			{
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		u = u0tmp + u1tmp;
		deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}

	return threshold;
}

void MyAdd(Mat src, Mat mask, Mat dst, int port, Point2i* startPtSrc, Point2i* startPtMask, Point2i* endPtMask) {
	int height = endPtMask->y - startPtMask->y;
	int width = endPtMask->x - startPtMask->x;
	float p = (float)port / 100.0;
	//dst = src.clone();
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vec4b src_temp = src.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x);
			Vec4b mask_temp = mask.at<Vec4b>(i + startPtMask->y, j + startPtMask->x);
			dst.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x)[0] = src_temp[0] * (1 - p) + mask_temp[0] * p;
			dst.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x)[1] = src_temp[1] * (1 - p) + mask_temp[1] * p;
			dst.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x)[2] = src_temp[2] * (1 - p) + mask_temp[2] * p;
			//std::cout << j << " " << i;
		}
	}
}
void MyAdd(Mat src, Mat mask, Mat dst, Point2i* startPtSrc, Point2i* startPtMask, Point2i* endPtMask) {
	int height = endPtMask->y - startPtMask->y;
	int width = endPtMask->x - startPtMask->x;
	//dst = src.clone();
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vec4b src_temp = src.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x);
			Vec4b mask_temp = mask.at<Vec4b>(i + startPtMask->y, j + startPtMask->x);
			int b = src_temp[0] + mask_temp[0]; if (b > 255) b = 255;
			int g = src_temp[1] + mask_temp[1]; if (g > 255) g = 255;
			int r = src_temp[2] + mask_temp[2]; if (r > 255) r = 255;
			dst.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x)[0] = b;
			dst.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x)[1] = g;
			dst.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x)[2] = r;
			//std::cout << j << " " << i;
		}
	}
}

void MyMinus(Mat src, Mat mask, Mat dst, Point2i* startPtSrc, Point2i* startPtMask, Point2i* endPtMask) {
	int height = endPtMask->y - startPtMask->y;
	int width = endPtMask->x - startPtMask->x;
	//dst = src.clone();
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vec4b src_temp = src.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x);
			Vec4b mask_temp = mask.at<Vec4b>(i + startPtMask->y, j + startPtMask->x);
			int b = src_temp[0] - mask_temp[0]; if (b < 0) b = 0;
			int g = src_temp[1] - mask_temp[1]; if (g < 0) g = 0;
			int r = src_temp[2] - mask_temp[2]; if (r < 0) r = 0;
			dst.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x)[0] = b;
			dst.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x)[1] = g;
			dst.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x)[2] = r;
			//std::cout << j << " " << i;
		}
	}
}
void MyMult(Mat src, Mat mask, Mat dst, int binary, Point2i* startPtSrc, Point2i* startPtMask, Point2i* endPtMask) {
	int height = endPtMask->y - startPtMask->y;
	int width = endPtMask->x - startPtMask->x;
	//dst = src.clone();
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vec4b src_temp = src.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x);
			Vec4b mask_temp = mask.at<Vec4b>(i + startPtMask->y, j + startPtMask->x);
			if (binary) {
				if (mask_temp[0] > 0)mask_temp[0] = 1;
				if (mask_temp[1] > 0)mask_temp[1] = 1;
				if (mask_temp[2] > 0)mask_temp[2] = 1;
			}
			int b = src_temp[0] * mask_temp[0]; if (b > 255) b %= 256;
			int g = src_temp[1] * mask_temp[1]; if (g > 255) g %= 256;
			int r = src_temp[2] * mask_temp[2]; if (r > 255) r %= 256;
			dst.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x)[0] = b;
			dst.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x)[1] = g;
			dst.at<Vec4b>(i + startPtSrc->y, j + startPtSrc->x)[2] = r;
			//std::cout << j << " " << i;
		}
	}
}

Mat MyResize(Mat src, float xRatio, float yRatio, int mode) {
	int rows = static_cast<int>(src.rows * yRatio + 0.5);
	int cols = static_cast<int>(src.cols * xRatio + 0.5);
	Mat dst = Mat(rows, cols, src.type(), Scalar(0, 0, 0, 0));
	if (mode == NEAREAST) {
		for (int i = 0; i < rows; i++) {
			//计算新矩阵中点的纵坐标在原图中的位置
			int row = static_cast<int>((float)i / yRatio + 0.5);
			//四舍五入可能越界
			if (row >= src.rows)
				row--;

			for (int j = 0; j < cols; j++) {
				//计算新矩阵中点的横坐标在原图中的位置
				int col = static_cast<int>((float)j / xRatio + 0.5);
				if (col >= src.cols)
					col--;
				dst.at<Vec4b>(i, j)	= src.at<Vec4b>(row, col);
			}
		}
	}
	else if (mode == BILINEARITY) {
		for (int i = 0; i < rows; i++) {
			//计算新矩阵中点的纵坐标在原图中的位置
			double row = (double)i / yRatio;

			for (int j = 0; j < cols; j++) {
				//计算新矩阵中点的横坐标在原图中的位置
				double col = (double)j / xRatio;
				//计算在原图中目标点四周的像素位置
				int lRow = static_cast<int>(row);
				int rRow = lRow + 1;
				int lCol = static_cast<int>(col);
				int rCol = lCol + 1;
				//计算他与左右两点、上下两点的距离比例
				double u = row - lRow;
				double v = col - lCol;

				//最后一点
				if ((row >= src.rows - 1) && (col >= src.cols - 1)) {
					//std::cout << 1 << std::endl;
					dst.at<Vec4b>(i, j) = src.at<Vec4b>(lRow, lCol);
				}
				//最后一行，利用左右列
				else if (row >= src.rows - 1) {
					//std::cout << 2 << std::endl;
					//lastRow = src.ptr<Vec3b>(lRow);
					//p[j] = v * lastRow[lCol] + (1 - v) * lastRow[rCol];
					dst.at<Vec4b>(i, j) = v * src.at<Vec4b>(lRow, lCol) + (1 - v) * src.at<Vec4b>(lRow, rCol);
					//dst.at<Vec4b>(i, j) = src.at<Vec4b>(lRow, v * lastRow[lCol] + (1 - v) * lastRow[rCol]);
				}
				//最后一列
				else if (col >= src.cols - 1) {
					//std::cout << 3 << std::endl;
					//lastRow = src.ptr<Vec3b>(lRow);
					//nextRow = src.ptr<Vec3b>(nRow);
					//p[j] = u * lastRow[lCol] + (1 - u) * nextRow[lCol];
					dst.at<Vec4b>(i, j) = u * src.at<Vec4b>(lRow, lCol) +(1 - u) * src.at<Vec4b>(rRow, lCol);
				}
				else {
					//std::cout << 4 << std::endl;
					//lastRow = src.ptr<Vec3b>(lRow);
					//nextRow = src.ptr<Vec3b>(nRow);
					Vec4b f1 = v * src.at<Vec4b>(lRow, lCol) + (1 - v) * src.at<Vec4b>(lRow, rCol);
					Vec4b f2 = v * src.at<Vec4b>(rRow, lCol) + (1 - v) * src.at<Vec4b>(rRow, rCol);
					dst.at<Vec4b>(i, j)  = u * f1 + (1 - u) * f2;
				}
			}
		}
	}
	return dst;
}

Point2d RotateArdOrigin(Point2d src, double angle) {
	double x = src.x * cos(angle) - src.y * sin(angle);
	double y = src.x * sin(angle) + src.y * cos(angle);
	//std::cout << src.x << " " << src.y << " " << x << " " << y << std::endl;
	return Point2d(x, y);
}

Mat MyRotate(Mat src, int Xstart, int Ystart, int Xend, int Yend, int mode, Point2d* newCenter) {
	//std::cout << newCenter->x << " " << newCenter->y << std::endl;
	Point2d center = Point2d(src.cols / 2, src.rows / 2);

	double endAngle = atan((double)Yend / (double)Xend);
	double startAngle = atan((double)Ystart / (double)Xstart);
	//std::cout << Xstart << " i " << Ystart << " j " << Xend << " x " << Yend << " y " << std::endl;
	//防止180跳变
	if(Xstart > 0 && Xend<0 ) endAngle += pi;
	else if (Xstart < 0 && Xend > 0)endAngle += pi;

	double angle = endAngle - startAngle;

	//std::cout << angle << std::endl;
	const double cosAngle = cos(angle);
	const double sinAngle = sin(angle);
	
	//std::cout << cosAngle << " " << sinAngle << std::endl;

	//原图像四个角的坐标变为以旋转中心的坐标系
	Point2d leftTop(-center.x, center.y); //(0,0)
	Point2d rightTop(src.cols - center.x, center.y); // (width,0)
	Point2d leftBottom(-center.x, -src.rows  +center.y); //(0,height)
	Point2d rightBottom(src.cols - center.x, -src.rows + center.y); // (width,height)
	newCenter->x -= center.x;
	newCenter->y -= center.y;

	//以center为中心旋转后四个角的坐标
	Point2d transLeftTop, transRightTop, transLeftBottom, transRightBottom;
	transLeftTop = RotateArdOrigin(leftTop, angle);
	transRightTop = RotateArdOrigin(rightTop, angle);
	transLeftBottom = RotateArdOrigin(leftBottom, angle);
	transRightBottom = RotateArdOrigin(rightBottom, angle);
	*newCenter = RotateArdOrigin(*newCenter, angle);


	//计算旋转后图像的width，height，四个点相对关系旋转之后可能发生变化
	double left = min({ transLeftTop.x, transRightTop.x, transLeftBottom.x, transRightBottom.x });
	double right = max({ transLeftTop.x, transRightTop.x, transLeftBottom.x, transRightBottom.x });
	double top = max({ transLeftTop.y, transRightTop.y, transLeftBottom.y, transRightBottom.y });
	double down = min({ transLeftTop.y, transRightTop.y, transLeftBottom.y, transRightBottom.y });
	newCenter->x += abs(left);
	newCenter->y += abs(top);

	int width = static_cast<int>(abs(left - right) + 0.5);
	int height = static_cast<int>(abs(top - down) + 0.5);
	//std::cout << left << " " << down << std::endl;
	//std::cout << width << " " << height << std::endl;

	Mat dst = Mat(height, width, src.type(), Scalar(0, 0, 0, 0));

	
	const double num1 = -abs(left) * cosAngle - abs(top) * sinAngle + center.x;
	const double num2 = abs(left) * sinAngle - abs(top) * cosAngle + center.y;
	if (mode == NEAREAST) {
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				//对应新的mat与初始mat的对应位置进行插值
				int x = static_cast<int>(j  * cosAngle + i * sinAngle + num1 + 0.5);
				int y = static_cast<int>(-j * sinAngle + i * cosAngle + num2 + 0.5);

				//int x = static_cast<int>(j  * cosAngle + i * sinAngle + num1 + 0.5);
				//int y = static_cast<int>(-j * sinAngle + i * cosAngle + num2 + 0.5);

				//std::cout << i - abs(left) << " i " << j - abs(top) << " j " << temp.x << " x " << temp.y << " y " << std::endl;
				//int x = static_cast<int>(temp.x + center.x + 0.5);
				//int y = static_cast<int>(temp.y + center.y + 0.5);
				if (x >= 0 && y >= 0 && x < src.cols && y < src.rows) {
					dst.at<Vec4b>(i, j) = src.at<Vec4b>(y, x);
					//std::cout << i << " i " << j << " j " << x << " x " << y << " y " << std::endl;
				}
				else {
					dst.at<Vec4b>(i, j)[3] = 0;
				}

			}
		}

	}
	else if (mode == BILINEARITY) {
		for (int i = 0; i < height; i++) {
			//计算新矩阵中点的纵坐标在原图中的位置

			for (int j = 0; j < width; j++) {
				//计算新矩阵中点的横坐标在原图中的位置
				double col = j  * cosAngle + i * sinAngle + num1;//x
				double row = -j * sinAngle + i * cosAngle + num2;//y
				//计算在原图中目标点四周的像素位置
				int lRow = static_cast<int>(row);
				int rRow = lRow + 1;
				int lCol = static_cast<int>(col);
				int rCol = lCol + 1;
				//计算他与左右两点、上下两点的距离比例
				double u = row - lRow;
				double v = col - lCol;
				if(row <= src.rows - 1 && col <= src.cols - 1 && row >= 0 && col >= 0){
					if ((row == src.rows - 1) && (col == src.cols - 1)) {
						dst.at<Vec4b>(i, j) = src.at<Vec4b>(lRow, lCol);
					}
					//最后一行，利用左右列
					else if (row == src.rows - 1) {
						dst.at<Vec4b>(i, j) = v * src.at<Vec4b>(lRow, lCol) + (1 - v) * src.at<Vec4b>(lRow, rCol);
					}
					//最后一列
					else if (col >= src.cols - 1) {
						dst.at<Vec4b>(i, j) = u * src.at<Vec4b>(lRow, lCol) + (1 - u) * src.at<Vec4b>(rRow, lCol);
					}
					else {
						Vec4b f1 = v * src.at<Vec4b>(lRow, lCol) + (1 - v) * src.at<Vec4b>(lRow, rCol);
						Vec4b f2 = v * src.at<Vec4b>(rRow, lCol) + (1 - v) * src.at<Vec4b>(rRow, rCol);
						dst.at<Vec4b>(i, j) = u * f1 + (1 - u) * f2;
					}
				}
				else {
					dst.at<Vec4b>(i, j)[3] = 0;
				}
			}
		}
	}
	return dst;
}

void MyEnhance(Mat src, Mat dst, int a, int b, int c, int d, int type) {
	if (type == 2  ) {
		c = -1;
		d = -1;
	}
	else if (type == 3) {
		//c = -1;
		d = -1;
	}
	if (a == 0 || b == 0 || c==0 || d == 0) return;
	int temp;
	if (a > b && (type ==0 || type == 1)){
		temp = b;
		b = a;
		a = temp;
	}
	if (c > d && (type == 0 || type == 1)) {
		temp = c;
		c = d;
		d = temp;
	}
	double c0, b0, min = 255, max = 0;	
	float grayMap[256];
	memset(grayMap, 0, 256 * sizeof(float));
	switch (type) {
	case 0:
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				int gray = src.at<Vec4b>(i, j)[0];
				if (gray > b) {
					dst.at<Vec4b>(i, j)[0] = d;
					dst.at<Vec4b>(i, j)[1] = d;
					dst.at<Vec4b>(i, j)[2] = d;
				}
				else if (gray < a) {
					dst.at<Vec4b>(i, j)[0] = c;
					dst.at<Vec4b>(i, j)[1] = c;
					dst.at<Vec4b>(i, j)[2] = c;
				}
				else {
					int newG = (double)(d - c) / (double)(b - a) * (double)(gray - a) + c;
					dst.at<Vec4b>(i, j)[0] = newG;
					dst.at<Vec4b>(i, j)[1] = newG;
					dst.at<Vec4b>(i, j)[2] = newG;
				}
			}
		}
		break;
	case 1:
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				int gray = src.at<Vec4b>(i, j)[0]; 
				if (gray >= b) {
					int newG = (double)(MAX_GRAY - d) / (double)(MAX_GRAY - b) * (double)(gray - b) + d;
					dst.at<Vec4b>(i, j)[0] = newG;
					dst.at<Vec4b>(i, j)[1] = newG;
					dst.at<Vec4b>(i, j)[2] = newG;
				}
				else if (gray < a) {
					int newG = (double)(c) / (double)(a) * (double)(gray );
					dst.at<Vec4b>(i, j)[0] = newG;
					dst.at<Vec4b>(i, j)[1] = newG;
					dst.at<Vec4b>(i, j)[2] = newG;
				}
				else {
					int newG = (double)(d - c) / (double)(b - a) * (double)(gray - a) + c;
					dst.at<Vec4b>(i, j)[0] = newG;
					dst.at<Vec4b>(i, j)[1] = newG;
					dst.at<Vec4b>(i, j)[2] = newG;
				}
			}
		}
		break;
	case 2:
		//std::cout << a << b << c<<std::endl;
		//a * log v+1 (1 + v *r)
		//max = a + log(2) / (b * log(c));
		//min = a + log(1) / (b * log(c));
		//cout << min << " " << max << endl;
		for (int i = 0; i < 256; i++) {
			grayMap[i] = a * (double)log((double)i / 255.0 * (double)b + 1) / ((double)log(b + 1));
		}
		for (int i = 0; i < 256; i++) {
			if (grayMap[i] > max) max = grayMap[i];
			if (grayMap[i] < min) min = grayMap[i];
		}
		for (int i = 0; i < 256; i++) {
			grayMap[i] = (grayMap[i] - min) / (max - min) * 255;
		}

		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				int gray = src.at<Vec4b>(i, j)[0];
				//需要归一化和反归一化
				int newG = grayMap[gray];
				if (newG > 255) newG = 255;
				if (newG < 0) newG = 0;
				//double newG = (double)gray / 255.0;
				//newG = a + log(newG + 1) / (b * log(c));
				//newG = ((newG - min) / (max - min)) * 255;
				//std::cout << gray << " " << newG << std::endl;
				dst.at<Vec4b>(i, j)[0] = newG;
				dst.at<Vec4b>(i, j)[1] = newG;
				dst.at<Vec4b>(i, j)[2] = newG;
			}
		}
		break;
	case 3:
		//S = c * R ^ r
		c0 = (double)c  * 5 / 255.0;
		b0 = (double)b  / 255.0 + 1; 

		std::cout << a << std::endl;

		for (int i = 0; i < 256; i++) {
			//cout << b0 << " " << c0 << endl;
			if(i - a < 0)
				grayMap[i] =0;
			else 
				grayMap[i] = pow(b0, c0 * (i - a)) - 1;
			//cout << grayMap[i] << endl;
		}
		for (int i = 0; i < 256; i++) {
			if (grayMap[i] > max) max = grayMap[i];
			if (grayMap[i] < min) min = grayMap[i];
		}
		for (int i = 0; i < 256; i++) {
			//grayMap[i] = (grayMap[i] - min) / (max - min) * 255;
		}

		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				int gray = src.at<Vec4b>(i, j)[0] ;
				//int newG = pow(b0, c0 * (gray - (double)a/255.0)) - 1;
				int newG = grayMap[gray];
				if (newG > 255) newG = 255;
				if (newG < 0) newG = 0;
				//double newG = (double)gray / 255.0;
				//newG = a + log(newG + 1) / (b * log(c));
				//newG = ((newG - min) / (max - min)) * 255;
				//std::cout << gray << " " << newG << std::endl;
				dst.at<Vec4b>(i, j)[0] = newG;
				dst.at<Vec4b>(i, j)[1] = newG;
				dst.at<Vec4b>(i, j)[2] = newG;
			}
		}
		break;
	default:
		break;
	}
}

void MyHistEqual(Mat src, Mat dst) {
	int width = src.cols;
	int height = src.rows;
	int pixelCount[gray_scale];
	float pixelPro[gray_scale];
	float Sk[gray_scale];
	int i, j, pixelSum = width * height, threshold = 0;
	for (i = 0; i < gray_scale; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	//统计灰度级中每个像素在整幅图像中的个数  
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			Vec4b pt = src.at<Vec4b>(i, j);
			int gray = pt[2];
			pixelCount[gray] += 1;
		}
	}

	//计算每个像素在整幅图像中的比例  
	for (i = 0; i < gray_scale; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;
		if (i == 0) Sk[i] = pixelPro[i];
		else Sk[i] = pixelPro[i] + Sk[i - 1];
	}
	//灰度值取整
	for (i = 0; i < gray_scale; i++)
	{
		Sk[i] = static_cast<int>((Sk[i] * MAX_GRAY) + 0.5);
	}
	//遍历所有节点
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			Vec4b pt = src.at<Vec4b>(i, j);
			//opencv公式
			int gray = Sk[pt[0]];
			dst.at<Vec4b>(i, j)[0] = gray;
			dst.at<Vec4b>(i, j)[1] = gray;
			dst.at<Vec4b>(i, j)[2] = gray;

		}
	}
}
//src与dst调用时需要保证不一样
void MyFilterAverage(Mat src, Mat dst, int size) {
	if (size == 3) {
		for (int i = 1; i < src.rows - 1; i++) {
			for (int j = 1; j < src.cols - 1; j++) {
				int gray = (src.at<Vec4b>(i, j)[0] + src.at<Vec4b>(i + 1, j)[0] + src.at<Vec4b>(i, j + 1)[0] + src.at<Vec4b>(i + 1, j + 1)[0]
					+ src.at<Vec4b>(i - 1, j)[0] + src.at<Vec4b>(i - 1, j - 1)[0] + src.at<Vec4b>(i - 1, j + 1)[0]
					+ src.at<Vec4b>(i, j - 1)[0] + src.at<Vec4b>(i + 1, j - 1)[0])/9;
				dst.at<Vec4b>(i, j)[0] = gray;
				dst.at<Vec4b>(i, j)[1] = gray;
				dst.at<Vec4b>(i, j)[2] = gray;
			}
		}
	}
	else {
		for (int i = 2; i < src.rows - 2; i++) {
			for (int j = 2; j < src.cols - 2; j++) {
				int gray = (src.at<Vec4b>(i - 2, j - 2)[0] + src.at<Vec4b>(i - 2, j - 1)[0] + src.at<Vec4b>(i - 2, j)[0] + src.at<Vec4b>(i - 2, j + 1)[0] + src.at<Vec4b>(i - 2, j + 2)[0] + 
							src.at<Vec4b>(i - 1, j - 2)[0] + src.at<Vec4b>(i - 1, j - 1)[0] + src.at<Vec4b>(i - 1, j)[0] + src.at<Vec4b>(i - 1, j + 1)[0] + src.at<Vec4b>(i - 1, j + 2)[0] + 
							src.at<Vec4b>(i, j - 2)[0] + src.at<Vec4b>(i, j - 1)[0] + src.at<Vec4b>(i, j)[0] + src.at<Vec4b>(i, j + 1)[0] + src.at<Vec4b>(i, j + 2)[0] +
							src.at<Vec4b>(i + 1, j - 2)[0] + src.at<Vec4b>(i + 1, j - 1)[0] + src.at<Vec4b>(i + 1, j)[0] + src.at<Vec4b>(i + 1, j + 1)[0] + src.at<Vec4b>(i + 1, j + 2)[0] +
							src.at<Vec4b>(i + 2, j - 2)[0] + src.at<Vec4b>(i + 2, j - 1)[0] + src.at<Vec4b>(i + 2, j)[0] + src.at<Vec4b>(i + 2, j + 1)[0] + src.at<Vec4b>(i + 2, j + 2)[0]) / 25;
				dst.at<Vec4b>(i, j)[0] = gray;
				dst.at<Vec4b>(i, j)[1] = gray;
				dst.at<Vec4b>(i, j)[2] = gray;
			}
		}
	}
}
//src与dst调用时需要保证不一样
void MyFilterMedian(Mat src, Mat dst, int size) {
	if (size == 3) {
		for (int i = 1; i < src.rows - 1; i++) {
			for (int j = 1; j < src.cols - 1; j++) {
				std::vector<int> arr = { src.at<Vec4b>(i, j)[0] , src.at<Vec4b>(i + 1, j)[0] , src.at<Vec4b>(i, j + 1)[0] , src.at<Vec4b>(i + 1, j + 1)[0]
					, src.at<Vec4b>(i - 1, j)[0] , src.at<Vec4b>(i - 1, j - 1)[0] , src.at<Vec4b>(i - 1, j + 1)[0]
					, src.at<Vec4b>(i, j - 1)[0] , src.at<Vec4b>(i + 1, j - 1)[0] };
				std::vector<int>::iterator start = arr.begin(), end = arr.end();
				std::nth_element(start, start + 4, end);
				int gray = arr[4];
				dst.at<Vec4b>(i, j)[0] = gray;
				dst.at<Vec4b>(i, j)[1] = gray;
				dst.at<Vec4b>(i, j)[2] = gray;
			}
		}
	}
	else {
		for (int i = 2; i < src.rows - 2; i++) {
			for (int j = 2; j < src.cols - 2; j++) {
				std::vector<int> arr = { src.at<Vec4b>(i - 2, j - 2)[0] , src.at<Vec4b>(i - 2, j - 1)[0] , src.at<Vec4b>(i - 2, j)[0] , src.at<Vec4b>(i - 2, j + 1)[0] , src.at<Vec4b>(i - 2, j + 2)[0] ,
					src.at<Vec4b>(i - 1, j - 2)[0] , src.at<Vec4b>(i - 1, j - 1)[0] , src.at<Vec4b>(i - 1, j)[0] , src.at<Vec4b>(i - 1, j + 1)[0] , src.at<Vec4b>(i - 1, j + 2)[0] ,
					src.at<Vec4b>(i, j - 2)[0] , src.at<Vec4b>(i, j - 1)[0] , src.at<Vec4b>(i, j)[0] , src.at<Vec4b>(i, j + 1)[0] , src.at<Vec4b>(i, j + 2)[0] ,
					src.at<Vec4b>(i + 1, j - 2)[0] , src.at<Vec4b>(i + 1, j - 1)[0] , src.at<Vec4b>(i + 1, j)[0] , src.at<Vec4b>(i + 1, j + 1)[0] , src.at<Vec4b>(i + 1, j + 2)[0] ,
					src.at<Vec4b>(i + 2, j - 2)[0] , src.at<Vec4b>(i + 2, j - 1)[0] , src.at<Vec4b>(i + 2, j)[0] , src.at<Vec4b>(i + 2, j + 1)[0] , src.at<Vec4b>(i + 2, j + 2)[0] };
				std::vector<int>::iterator start = arr.begin(), end = arr.end();
				std::nth_element(start, start + 12, end);
				int gray = arr[12];
				dst.at<Vec4b>(i, j)[0] = gray;
				dst.at<Vec4b>(i, j)[1] = gray;
				dst.at<Vec4b>(i, j)[2] = gray;
			}
		}
	}
	dst = dst.clone();
}
//src与dst调用时需要保证不一样
void MyFilterGauss(Mat src, Mat dst, int size, int sigma) {
	if (size == 3) {
		double a = exp(-0 /(2.0 * (double)sigma * (double)sigma)) / (2.0 * pi * (double)sigma* (double)sigma);
		double b = exp(-1.0 / (2.0 * (double)sigma * (double)sigma)) / (2.0 * pi * (double)sigma* (double)sigma);
		double c = exp(-2.0 / (2.0 * (double)sigma * (double)sigma)) / (2.0 * pi * (double)sigma* (double)sigma);
		double total = a + b * 4.0 + c * 4.0;
		//std::cout << a << " " << b << " " << c << std::endl;
		for (int i = 1; i < src.rows - 1; i++) {
			for (int j = 1; j < src.cols - 1; j++) {
				double gray = (src.at<Vec4b>(i, j)[0] * a + src.at<Vec4b>(i + 1, j)[0] * b + src.at<Vec4b>(i, j + 1)[0] * b + src.at<Vec4b>(i + 1, j + 1)[0] * c
					+ src.at<Vec4b>(i - 1, j)[0] * b + src.at<Vec4b>(i - 1, j - 1)[0] * c+ src.at<Vec4b>(i - 1, j + 1)[0] * c
					+ src.at<Vec4b>(i, j - 1)[0] * b + src.at<Vec4b>(i + 1, j - 1)[0] * c)/total ;
				//if(i == 200 && j == 300)
				//std::cout << gray << std::endl;
				dst.at<Vec4b>(i, j)[0] = gray;
				dst.at<Vec4b>(i, j)[1] = gray;
				dst.at<Vec4b>(i, j)[2] = gray;
			}
		}
	}
	else {
		double a = exp(-0 / (2.0 * (double)sigma * (double)sigma)) / (2.0 * pi * (double)sigma);
		double b = exp(-1.0 / (2.0 * (double)sigma * (double)sigma)) / (2.0 * pi * (double)sigma);
		double c = exp(-2.0 / (2.0 * (double)sigma * (double)sigma)) / (2.0 * pi * (double)sigma);
		double d = exp(-4.0 / (2.0 * (double)sigma * (double)sigma)) / (2.0 * pi * (double)sigma);
		double e = exp(-5.0 / (2.0 * (double)sigma * (double)sigma)) / (2.0 * pi * (double)sigma);
		double f = exp(-8.0 / (2.0 * (double)sigma * (double)sigma)) / (2.0 * pi * (double)sigma);
		double total = a + b * 4.0 + c * 4.0 + d * 4.0 + e * 8.0 + f * 4.0;
		for (int i = 2; i < src.rows - 2; i++) {
			for (int j = 2; j < src.cols - 2; j++) {
				int gray = (src.at<Vec4b>(i - 2, j - 2)[0] * f + src.at<Vec4b>(i - 2, j - 1)[0] * e + src.at<Vec4b>(i - 2, j)[0] * d + src.at<Vec4b>(i - 2, j + 1)[0] * e + src.at<Vec4b>(i - 2, j + 2)[0] * f +
					src.at<Vec4b>(i - 1, j - 2)[0] * e + src.at<Vec4b>(i - 1, j - 1)[0] * c + src.at<Vec4b>(i - 1, j)[0] * b + src.at<Vec4b>(i - 1, j + 1)[0] * c + src.at<Vec4b>(i - 1, j + 2)[0] * e +
					src.at<Vec4b>(i, j - 2)[0] * d + src.at<Vec4b>(i, j - 1)[0] * b + src.at<Vec4b>(i, j)[0] * a + src.at<Vec4b>(i, j + 1)[0] * b + src.at<Vec4b>(i, j + 2)[0] * d +
					src.at<Vec4b>(i + 1, j - 2)[0] * e + src.at<Vec4b>(i + 1, j - 1)[0] * c + src.at<Vec4b>(i + 1, j)[0] * b + src.at<Vec4b>(i + 1, j + 1)[0] * c + src.at<Vec4b>(i + 1, j + 2)[0] * e +
					src.at<Vec4b>(i + 2, j - 2)[0] * f + src.at<Vec4b>(i + 2, j - 1)[0] * e + src.at<Vec4b>(i + 2, j)[0] * d + src.at<Vec4b>(i + 2, j + 1)[0] * e + src.at<Vec4b>(i + 2, j + 2)[0] * f) / total;
				dst.at<Vec4b>(i, j)[0] = gray;
				dst.at<Vec4b>(i, j)[1] = gray;
				dst.at<Vec4b>(i, j)[2] = gray;
			}
		}
	}
	return;
}
//src与dst调用时需要保证不一样
void MyFilterPersonal(Mat src, Mat dst, int size, int para[]) {
	int sum = 0;
	if (size == 3) {
		for (int i = 0; i < 9; i++) {
			sum += para[i];
		}
		if (sum == 0) return;
		for (int i = 1; i < src.rows - 1; i++) {
			for (int j = 1; j < src.cols - 1; j++) {
				int gray = (src.at<Vec4b>(i, j)[0] * para[4] + src.at<Vec4b>(i + 1, j)[0] * para[7] + src.at<Vec4b>(i, j + 1)[0] * para[5] + src.at<Vec4b>(i + 1, j + 1)[0] * para[8]
					+ src.at<Vec4b>(i - 1, j)[0] * para[1] + src.at<Vec4b>(i - 1, j - 1)[0] * para[0] + src.at<Vec4b>(i - 1, j + 1)[0] * para[2]
					+ src.at<Vec4b>(i, j - 1)[0] * para[3] + src.at<Vec4b>(i + 1, j - 1)[0] * para[6]) / sum;
				dst.at<Vec4b>(i, j)[0] = gray;
				dst.at<Vec4b>(i, j)[1] = gray;
				dst.at<Vec4b>(i, j)[2] = gray;
			}
		}
	}
	else {
		//std::cout << "here" << std::endl;
		for (int i = 0; i < 25; i++) {
			sum += para[i];
		}
		if (sum == 0) return;
		//std::cout << sum << std::endl;
		for (int i = 2; i < src.rows - 2; i++) {
			for (int j = 2; j < src.cols - 2; j++) {
				int gray = (src.at<Vec4b>(i - 2, j - 2)[0] * para[0] + src.at<Vec4b>(i - 2, j - 1)[0] * para[1] + src.at<Vec4b>(i - 2, j)[0] * para[2] + src.at<Vec4b>(i - 2, j + 1)[0] * para[3] + src.at<Vec4b>(i - 2, j + 2)[0] * para[4] +
					src.at<Vec4b>(i - 1, j - 2)[0] * para[5] + src.at<Vec4b>(i - 1, j - 1)[0] * para[6] + src.at<Vec4b>(i - 1, j)[0] * para[7] + src.at<Vec4b>(i - 1, j + 1)[0] * para[8] + src.at<Vec4b>(i - 1, j + 2)[0] * para[9] +
					src.at<Vec4b>(i, j - 2)[0] * para[10] + src.at<Vec4b>(i, j - 1)[0] * para[11] + src.at<Vec4b>(i, j)[0] * para[12] + src.at<Vec4b>(i, j + 1)[0] * para[13] + src.at<Vec4b>(i, j + 2)[0] * para[14] +
					src.at<Vec4b>(i + 1, j - 2)[0] * para[15] + src.at<Vec4b>(i + 1, j - 1)[0] * para[16] + src.at<Vec4b>(i + 1, j)[0] * para[17] + src.at<Vec4b>(i + 1, j + 1)[0] * para[18] + src.at<Vec4b>(i + 1, j + 2)[0] * para[19] +
					src.at<Vec4b>(i + 2, j - 2)[0] * para[20] + src.at<Vec4b>(i + 2, j - 1)[0] * para[21] + src.at<Vec4b>(i + 2, j)[0] * para[22] + src.at<Vec4b>(i + 2, j + 1)[0] * para[23] + src.at<Vec4b>(i + 2, j + 2)[0] * para[24]) / sum;
				dst.at<Vec4b>(i, j)[0] = gray;
				dst.at<Vec4b>(i, j)[1] = gray;
				dst.at<Vec4b>(i, j)[2] = gray;
			}
		}
	}
}

//src与dst调用时需要保证不一样
void MyConvolution(Mat src, Mat dst, int para[]) {
	int sum;
	for (int i = 0; i < 9; i++) {
		sum += para[i];
	}
	for (int i = 1; i < src.rows - 1; i++) {
		for (int j = 1; j < src.cols - 1; j++) {
			int gray = (src.at<Vec4b>(i, j)[0] * para[4] + src.at<Vec4b>(i + 1, j)[0] * para[7] + src.at<Vec4b>(i, j + 1)[0] * para[5] + src.at<Vec4b>(i + 1, j + 1)[0] * para[8]
				+ src.at<Vec4b>(i - 1, j)[0] * para[1] + src.at<Vec4b>(i - 1, j - 1)[0] * para[0] + src.at<Vec4b>(i - 1, j + 1)[0] * para[2]
				+ src.at<Vec4b>(i, j - 1)[0] * para[3] + src.at<Vec4b>(i + 1, j - 1)[0] * para[6]);
			dst.at<int>(i, j) = gray;
		}
	}
}

void MyEdgeSobel(Mat src, Mat dst, int threshold) {
	int paraX[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	int paraY[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };

	Mat GX = Mat(src.rows, src.cols, CV_32SC4, Scalar(0, 0, 0, 0));
	Mat GY = Mat(src.rows, src.cols, CV_32SC4, Scalar(0, 0, 0, 0));
	MyConvolution(src, GX, paraX);
	MyConvolution(src, GY, paraY);

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			//Vec4b pt = src.at<Vec4b>(i, j);
			int res = sqrt(GX.at<int>(i, j) * GX.at<int>(i, j) + GY.at<int>(i, j) * GY.at<int>(i, j));
			//近似：
			//int res = abs(GX.at<Vec4b>(i, j)[0]) +  abs(GY.at<Vec4b>(i, j)[0]);
			if (res > 255) res = 255;
			/*if (res > threshold) {
				dst.at<Vec4b>(i, j)[0] = 255;
				dst.at<Vec4b>(i, j)[1] = 255;
				dst.at<Vec4b>(i, j)[2] = 255;
			}
			else {
				dst.at<Vec4b>(i, j)[0] = 0;
				dst.at<Vec4b>(i, j)[1] = 0;
				dst.at<Vec4b>(i, j)[2] = 0;
			}*/
			//direction.at<double>(i, j) = atan2(GY.at<double>(i, j), GX.at<double>(i, j));
			dst.at<Vec4b>(i, j)[0] = res;
			dst.at<Vec4b>(i, j)[1] = res;
			dst.at<Vec4b>(i, j)[2] = res;
		}
	}
}
void MyEdgeSobel(Mat src, Mat dst, Mat GX, Mat GY) {
	int paraX[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	int paraY[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };

	MyConvolution(src, GX, paraX);
	MyConvolution(src, GY, paraY);

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			Vec4b pt = src.at<Vec4b>(i, j);
			int res = sqrt(GX.at<int>(i, j) * GX.at<int>(i, j) + GY.at<int>(i, j) * GY.at<int>(i, j));
			if (res > 255) res = 255;
			dst.at<Vec4b>(i, j)[0] = res;
			dst.at<Vec4b>(i, j)[1] = res;
			dst.at<Vec4b>(i, j)[2] = res;
		}
	}
}

void MyEdgeSobel(Mat src, Mat dst, Mat GX, Mat GY, Mat dist) {
	float acc_dx = 0, acc_dy = 0;         //accumulators
	float k1[] = { -1,-2,-1,0,0,0,1,2,1 }; //{-2,-4,-2,0,0,0,2,4,2};//{-1,-2,-1,0,0,0,1,2,1};    //sobel kernal dx
	float k2[] = { -1,0,1,-2,0,2,-1,0,1 };//{-2,0,2,-4,0,4,-2,0,2};//{-1,0,1,-2,0,2,-1,0,1};    //sobel kernal dy

	for (int i = 0; i<src.rows; i++) {
		for (int j = 0; j<src.cols; j++) {
			acc_dx = acc_dy = 0;

			//apply kernel/mask
			for (int nn = -1; nn<2; nn++) {
				for (int mm = -1; mm < 2; mm++) {
					if (i + nn > 0 && i + nn < src.rows && j + mm > 0 && j + mm < src.cols) {
						acc_dx += (float)src.at<Vec4b>(i + nn, j + mm)[0] * k1[((mm + 1) * 3) + nn + 1];
						acc_dy += (float)src.at<Vec4b>(i + nn, j + mm)[0] * k2[((mm + 1) * 3) + nn + 1];
					}
				}
			}
			//write final values
			GX.at<float>(i, j) = acc_dx;
			GY.at<float>(i, j) = acc_dy;
			//src.at<float>(i, j) = (sqrtf(acc_dy*acc_dy + acc_dx*acc_dx)) > 100 ? 255 : 0;
			dist.at<float>(i, j) = atan2f(acc_dy, acc_dx);
			dst.at<Vec4b>(i, j)[0] = (sqrtf(acc_dy*acc_dy + acc_dx*acc_dx)) > 100 ? 255 : 0;
			dst.at<Vec4b>(i, j)[1] = (sqrtf(acc_dy*acc_dy + acc_dx*acc_dx)) > 100 ? 255 : 0;
			dst.at<Vec4b>(i, j)[2] = (sqrtf(acc_dy*acc_dy + acc_dx*acc_dx)) > 100 ? 255 : 0;
			// printf("dist : %f \n", dist.at<float>(i,j) / 3.14159265f * 180 );
		}
	}
}

void MyEdgeLaplace(Mat src, Mat dst) {
	int para[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };

	Mat mask = Mat(src.rows, src.cols, CV_32SC4, Scalar(0, 0, 0, 0));
	MyConvolution(src, mask, para);

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			int res = mask.at<int>(i, j);
			if (res > 255) res = 255;
			if (res < 0) res = 0;
			dst.at<Vec4b>(i, j)[0] = res;
			dst.at<Vec4b>(i, j)[1] = res;
			dst.at<Vec4b>(i, j)[2] = res;
		}
	}
}

void search(int xt, int yt, Mat mask) {
	std::stack<std::vector<int>> s;
	std::queue<Vec2b> q;
	std::vector<int> a = { xt, yt };
	s.push(a);
	int x = 0, y = 0;
	std::vector<int> tmp;
	while (!s.empty()) {
		tmp = s.top();
		s.pop();
		x = tmp[0];
		y = tmp[1];
		if (x >= 1) {
			if (mask.at<int>(x - 1, y) == 1) {
				mask.at<int>(x - 1, y) = 2;
				// q.push(Vec2b(x - 1, y));
				std::vector<int> b = { x-1, y };
				s.push(b);
			}
			if (y >= 1) {
				if (mask.at<int>(x-1, y - 1) == 1) {
					mask.at<int>(x-1, y - 1) = 2;
					//q.push(Vec2b(x , y-1));
					std::vector<int> b = { x-1, y - 1 };
					s.push(b);
				}
			}
			if (y < mask.cols - 1) {
				if (mask.at<int>(x-1, y + 1) == 1) {
					mask.at<int>(x-1, y + 1) = 2;
					//q.push(Vec2b(x , y+1));
					std::vector<int> b = { x-1  , y + 1 };
					s.push(b);
				}
			}
		}
		if (y >= 1) {
			if (mask.at<int>(x, y - 1) == 1) {
				mask.at<int>(x, y - 1) = 2;
				//q.push(Vec2b(x , y-1));
				std::vector<int> b = { x, y-1 };
				s.push(b);
			}
		}
		if (x < mask.rows - 1) {
			if (mask.at<int>(x + 1, y) == 1) {
				mask.at<int>(x + 1, y) = 2;
				//q.push(Vec2b(x + 1, y));
				std::vector<int> b = { x + 1, y };
				s.push(b);
			}
			if (y >= 1) {
				if (mask.at<int>(x+1, y - 1) == 1) {
					mask.at<int>(x+1, y - 1) = 2;
					//q.push(Vec2b(x , y-1));
					std::vector<int> b = { x+1, y - 1 };
					s.push(b);
				}
			}
			if (y < mask.cols - 1) {
				if (mask.at<int>(x+1, y + 1) == 1) {
					mask.at<int>(x+1, y + 1) = 2;
					//q.push(Vec2b(x , y+1));
					std::vector<int> b = { x+1  , y + 1 };
					s.push(b);
				}
			}
		}
		if (y < mask.cols - 1) {
			if (mask.at<int>(x, y + 1) == 1) {
				mask.at<int>(x, y + 1) = 2;
				//q.push(Vec2b(x , y+1));
				std::vector<int> b = { x  , y+1 };
				s.push(b);
			}
		}
		
	}
	
	/*if (x >= 1) {
		if (mask.at<int>(x-1, y) == 1) {
			mask.at<int>(x - 1, y) = 2;
			search(x - 1, y, mask);
		}
	}
	if (y >= 1) {
		if (mask.at<int>(x , y-1) == 1) {
			mask.at<int>(x , y-1) = 2;
			search(x, y-1, mask);
		}
	}
	if (x < mask.rows-1) {
		if (mask.at<int>(x + 1, y) == 1) {
			mask.at<int>(x + 1, y) = 2;
			search(x + 1, y, mask);
		}
	}
	if (y < mask.cols-1) {
		if (mask.at<int>(x, y + 1) == 1) {
			mask.at<int>(x, y + 1) = 2;
			search(x, y + 1, mask);
		}
	}*/
	return;
}

void MyEdgeCanny(Mat src, Mat dst,int high, int low, int sigma) {
	Mat tmp = Mat(src.rows, src.cols, src.type(), Scalar(0, 0, 0, 0));
	MyFilterGauss(src, tmp, 3, sigma);

	Mat GX = Mat(src.rows, src.cols, CV_32SC4, Scalar(0, 0, 0, 0));
	Mat GY = Mat(src.rows, src.cols, CV_32SC4, Scalar(0, 0, 0, 0));
	MyEdgeSobel(tmp, tmp, GX, GY);
	Mat temp = tmp.clone();
	Mat mask = Mat(src.rows, src.cols, CV_32SC4, Scalar(0, 0, 0, 0));
	std::vector<int> rootxs;
	std::vector<int> rootys;
	for (int i = 1; i < dst.rows - 1; i++) {
		for (int j = 1; j < dst.cols - 1; j++) {
			double angle = atan2(GY.at<int>(i, j), GX.at<int>(i, j));
			if (angle <= 0) angle += pi;
			//std::cout << angle << std::endl;
			int gray = tmp.at<Vec4b>(i, j)[0];
			int dir;
			//竖着为0， 二四象限为1， 横为2， 一三象限为3
			if (angle > pi  * 3.0 / 8.0 && angle < pi * 5.0 / 8.0) dir = 0;
			else if (angle >= pi * 5.0 / 8.0 && angle < pi  * 7.0 / 8.0) dir = 1;
			else if (angle >= pi / 8.0 && angle < pi * 3.0 / 8.0) dir = 3;
			else dir = 2;
			switch (dir) {
			case 0:
				if (tmp.at<Vec4b>(i - 1, j)[0] < gray && tmp.at<Vec4b>(i + 1, j)[0] < gray){
					temp.at<Vec4b>(i, j)[0] = tmp.at<Vec4b>(i, j)[0];
					temp.at<Vec4b>(i, j)[1] = tmp.at<Vec4b>(i, j)[1];
					temp.at<Vec4b>(i, j)[2] = tmp.at<Vec4b>(i, j)[2];
				}
				else {
					temp.at<Vec4b>(i, j)[0] = 0;
					temp.at<Vec4b>(i, j)[1] = 0;
					temp.at<Vec4b>(i, j)[2] = 0;
				}
				break;
			case 1:
				if (tmp.at<Vec4b>(i - 1, j - 1)[0] < gray && tmp.at<Vec4b>(i + 1, j + 1)[0] < gray)
				{
					temp.at<Vec4b>(i, j)[0] = tmp.at<Vec4b>(i, j)[0];
					temp.at<Vec4b>(i, j)[1] = tmp.at<Vec4b>(i, j)[1];
					temp.at<Vec4b>(i, j)[2] = tmp.at<Vec4b>(i, j)[2];
				}
				else {
					temp.at<Vec4b>(i, j)[0] = 0;
					temp.at<Vec4b>(i, j)[1] = 0;
					temp.at<Vec4b>(i, j)[2] = 0;
				}
				break;
			case 2:
				if (tmp.at<Vec4b>(i, j + 1)[0] < gray && tmp.at<Vec4b>(i, j - 1)[0] < gray)
				{
					temp.at<Vec4b>(i, j)[0] = tmp.at<Vec4b>(i, j)[0];
					temp.at<Vec4b>(i, j)[1] = tmp.at<Vec4b>(i, j)[1];
					temp.at<Vec4b>(i, j)[2] = tmp.at<Vec4b>(i, j)[2];
				}
				else {
					temp.at<Vec4b>(i, j)[0] = 0;
					temp.at<Vec4b>(i, j)[1] = 0;
					temp.at<Vec4b>(i, j)[2] = 0;
				}
				break;
			case 3:
				if (tmp.at<Vec4b>(i - 1, j + 1)[0] < gray && tmp.at<Vec4b>(i + 1, j - 1)[0] < gray)
				{
					temp.at<Vec4b>(i, j)[0] = tmp.at<Vec4b>(i, j)[0];
					temp.at<Vec4b>(i, j)[1] = tmp.at<Vec4b>(i, j)[1];
					temp.at<Vec4b>(i, j)[2] = tmp.at<Vec4b>(i, j)[2];
				}
				else {
					temp.at<Vec4b>(i, j)[0] = 0;
					temp.at<Vec4b>(i, j)[1] = 0;
					temp.at<Vec4b>(i, j)[2] = 0;
				}
				break;
			default:
				break;
			}
			//0 no 1 weak 2 strong
			if (temp.at<Vec4b>(i, j)[0] > high) {
				mask.at<int>(i, j) = 2;
				rootxs.push_back(i);
				rootys.push_back(j);
			}
			else if (temp.at<Vec4b>(i, j)[0] < low)mask.at<int>(i, j) = 0;
			else mask.at<int>(i, j) = 1;
		}
		
	}

/*	for (int i = 1; i < dst.rows - 1; i++) {
		for (int j = 1; j < dst.cols - 1; j++) {
				int gray = temp.at<Vec4b>(i, j)[0];
				dst.at<Vec4b>(i, j)[0] = gray;
				dst.at<Vec4b>(i, j)[1] = gray;
				dst.at<Vec4b>(i, j)[2] = gray;
			
		}
	}
	return;*/

	
	for (int i = 0; i < rootxs.size(); i++) {
	search(rootxs[i], rootys[i], mask);
	}
	for (int i = 1; i < dst.rows - 1; i++) {
		for (int j = 1; j < dst.cols - 1; j++) {
			if (mask.at<int>(i, j) == 2) {
				int gray = temp.at<Vec4b>(i, j)[0];
				dst.at<Vec4b>(i, j)[0] = gray;
				dst.at<Vec4b>(i, j)[1] = gray;
				dst.at<Vec4b>(i, j)[2] = gray;
			}
			
			else {
				dst.at<Vec4b>(i, j)[0] = 0;
				dst.at<Vec4b>(i, j)[1] = 0;
				dst.at<Vec4b>(i, j)[2] = 0;
			}
		}
	}
	
	

}

void MyAdjustHSV(Mat src, Mat dst, int dh, int ds, int dv) {
	float b, g, r;
	float h, s, v;
	int hi;
	float f, p, q, t;
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			b = (float)src.at<Vec4b>(i, j)[0] / 255.0;
			g = (float)src.at<Vec4b>(i, j)[1] / 255.0;
			r = (float)src.at<Vec4b>(i, j)[2] / 255.0;

			v = MyMax(r, g, b);

			if (v == 0) s = 0;
			else s = (v - MyMin(r, g, b)) / v;

			if (v == MyMin(r, b, g)) h = 0;
			else if (v == r) h = 60.0 * (g - b) / (v - MyMin(r, g, b));
			else if (v == g) h = 120 + 60.0 * (b - r) / (v - MyMin(r, g, b));
			else if (v == b) h = 240 + 60.0 * (r - g) / (v - MyMin(r, g, b));
			if (h < 0) h += 360;
			else if (h >= 360) h -= 360;


			h += dh;
			if (h < 0) h += 360;
			else if (h >= 360) h -= 360;

			v += dv / 100.0;
			if (v < 0) v = 0;
			else if (v > 1) v = 1;

			s += ds / 100.0;
			if (s < 0) s = 0;
			else if (s > 1) s = 1;

			hi = h / 60;
			//cout << r << " r " << g << " g " << b << " b " << h << " h " << s << " s " << v << " v " << hi << " hi " << endl;
			f = h / 60.0 - hi;
			p = v * (1.0 - s) * 255;
			q = v * (1.0 - f * s) * 255;
			t = v * (1.0 - (1.0 - f) * s) * 255;
			v *= 255;
			switch (hi) {
			case 0:
				r = v; g = t; b = p; break;
			case 1:
				r = q; g = v; b = p; break;
			case 2:
				r = p; g = v; b = t; break;
			case 3:
				r = p; g = q; b = v; break;
			case 4:
				r = t; g = p; b = v; break;
			case 5:
				r = v; g = p; b = q; break;
			default:
				break;
			}

			dst.at<Vec4b>(i, j)[0] = b;
			dst.at<Vec4b>(i, j)[1] = g;
			dst.at<Vec4b>(i, j)[2] = r;
		}
	}
}

void MyLevel(Mat src, Mat dst, int in_start, int in_end, float port, int out_start, int out_end, int mode) {
	int map[256];
	memset(map, 0, 256 * sizeof(int));
	for (int i = 0; i < 256; i++) {
		if (i <= in_start) map[i] = out_start;
		else if (i >= in_end) map[i] = out_end;
		else {
			float port_tmp = (float)(i - in_start) / (float)(in_end - in_start) * port;
			map[i] = out_end * port_tmp + out_start * (1 - port_tmp);
		}
	}
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			int gray = src.at<Vec4b>(i, j)[mode];
			dst.at<Vec4b>(i, j)[mode] = map[gray];
		}
	}
}

void MyHoughLine(Mat src, Mat dst, int high, int low, int sigma, int MinNum){
	Mat tmp = Mat(src.rows, src.cols, src.type(), Scalar(0, 0, 0, 0));
	MyEdgeCanny(src, tmp, high, low, sigma);
	MyBinary(tmp, tmp, 1);

	int width = tmp.cols;
	int height = tmp.rows;
	int iRMax = (int)sqrt(width*width + height*height) + 1;
	int iThMax = 500;// theta0;
	int *pArray;
	pArray = new int[2 * iRMax*iThMax];
	memset(pArray, 0, sizeof(int) * 2 * iRMax*iThMax);
	float fRate = (float)(Pi / 180);
	vector<Vec2f> lines;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			if (tmp.at<Vec4b>(y, x)[0] == 255)
			{
				for (int iTh = 0; iTh < iThMax; iTh += 1)
				{
					int iR = (int)(x*cos(iTh*fRate) + y*sin(iTh*fRate));
					//if (iR < 0)
					//    continue;
					iR = iR + iRMax;
					pArray[2 * iRMax*iTh + iR]++;
				}
			}
		}
	for (int theta = 0; theta < 500; theta++)
	{
		for (int r = 0; r < iRMax * 2; r++)
		{
			int thetaL = max(0, theta - 1);
			int thetaR = min(359, theta + 1);
			int rL = max(0, r - 1);
			int rR = min(iRMax * 2 - 1, r + 1);
			int tmp = pArray[theta*iRMax * 2 + r];
			//if ((tmp>MinNum) 
			//    && tmp>pArray[thetaL*iRMax * 2 + rL] && tmp>pArray[thetaL*iRMax * 2 + r] 
			//    && (tmp>pArray[thetaL*iRMax * 2 + rR])&& (tmp > pArray[theta*iRMax * 2 + rL]) 
			//    && (tmp > pArray[theta*iRMax * 2 + rR])&& (tmp > pArray[thetaR*iRMax * 2 + rL]) 
			//    && (tmp > pArray[thetaR*iRMax * 2 + r]) && (tmp > pArray[thetaR*iRMax * 2 + rR])
			if ((tmp>MinNum)
				&& tmp>pArray[thetaL*iRMax * 2 + r]
				&& (tmp >= pArray[theta*iRMax * 2 + rR]) && (tmp > pArray[theta*iRMax * 2 + rL])
				&& (tmp >= pArray[thetaR*iRMax * 2 + r]))
			{
				Vec2f t = Vec2f(float(r - iRMax), float(theta));
				lines.push_back(t);
			}
		}
	}

	vector<Vec2f>::const_iterator it = lines.begin();
	cout << lines.size() << endl;
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta*fRate), b = sin(theta*fRate);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));

		line(dst, pt1, pt2, Scalar(255, 100, 5, 255), 2, CV_AA);
		//line(dstImage,pt1,pt2,Scalar(55,100,195),1,LINE_AA);
	}

}

void MyHoughCircle(Mat src, Mat dst, int high, int low, int sigma, int MinNum, int MinRadius, int MaxRadius, int  MinDist, int mode) {
	//Mat tmp1 = Mat(src.rows, src.cols, src.type(), Scalar(0, 0, 0, 0));
	//MyFilterGauss(src, tmp1, 3, sigma);
	cout << MinNum << " " << MinRadius << " " << MaxRadius << " " << MinDist << endl;
	Mat GX = Mat(src.rows, src.cols, CV_32SC1, Scalar(0));
	Mat GY = Mat(src.rows, src.cols, CV_32SC1, Scalar(0));
	//MyEdgeSobel(tmp1, tmp1, GX, GY);

	Mat tmp = Mat(src.rows, src.cols, src.type(), Scalar(0,0,0,0));
	Mat dist = Mat(src.rows, src.cols, CV_32FC1, Scalar(0));
	MyEdgeSobel(src, tmp, GX, GY, dist);
	if (mode == 1) {
		MyEdgeCanny(src, tmp, high, low, sigma);
	}

	int radiusRange = MaxRadius - MinRadius;
	int height = tmp.rows;
	int width = tmp.cols;
	int depth = radiusRange;
	cout << height << " " << width << endl;
	
	double ***H;

	Mat h_acc = Mat(tmp.rows, tmp.cols, CV_32FC1,Scalar(0));

	// Allocate memory
	H = new double**[height];
	for (int i = 0; i < height; ++i) {
		H[i] = new double*[width];

		for (int j = 0; j < width; ++j) {
			H[i][j] = new double[depth];
			for (int k = 0; k < depth; ++k)
				H[i][j][k] = 0;
		}

	}
	//memset(H, 0, height * width * depth * sizeof(double));

	for (int y = 0; y<tmp.rows; y++)
	{
		for (int x = 0; x<tmp.cols; x++)
		{
			//cout << tmp.at<Vec4b>(y, x)[0] << endl;
			// printf("data point : %f\n", img_data.at<float>(y,x));
			if (tmp.at<Vec4b>(y, x)[0] > 0)  //threshold image  
			{
				for (int r = MinRadius; r<radiusRange; r++)
				{
					//cout << dist.at<float>(y, x) << endl;
					int x0 = round(x + r * cos(dist.at<float>(y,x)));
					int x1 = round(x - r * cos(dist.at<float>(y,x)));
					int y0 = round(y + r * sin(dist.at<float>(y,x)));
					int y1 = round(y - r * sin(dist.at<float>(y,x)));


					inc_if_inside(H, x0, y0, height, width, r);
					// inc_if_inside(H,x0,y1,HEIGHT, WIDTH, r);
					// inc_if_inside(H,x1,y0,HEIGHT, WIDTH, r);
					inc_if_inside(H, x1, y1, height, width, r);
				}
			}
		}
	}
	//create 2D image by summing values of the radius dimension
	for (int y0 = 0; y0 < height; y0++) {
		for (int x0 = 0; x0 < width; x0++) {
			for (int r = MinRadius; r < radiusRange; r++) {
				h_acc.at<float>(y0, x0) += H[y0][x0][r];// > 1 ? 255 : 0;
														// printf("h : %d", H[y0][x0][r]);
				//cout << H[y0][x0][r] << " qwe" << endl;
			}
			//cout << h_acc.at<float>(y0, x0) << endl;
		}
	}

	vector<Point3f> bestCircles;
	//compute optimal circles
	for (int y0 = 0; y0 < height; y0++) {
		for (int x0 = 0; x0 < width; x0++) {
			for (int r = MinRadius; r < radiusRange; r++) {
				if (H[y0][x0][r] > MinNum) {
					Point3f circle(x0, y0, r);
					int i;
					for (i = 0; i < bestCircles.size(); i++) {
						int xCoord = bestCircles[i].x;
						int yCoord = bestCircles[i].y;
						int radius = bestCircles[i].z;
						if (abs(xCoord - x0) < MinDist && abs(yCoord - y0) < MinDist) {
							if (H[y0][x0][r] > H[yCoord][xCoord][radius]) {
								bestCircles.erase(bestCircles.begin() + i);
								bestCircles.insert(bestCircles.begin(), circle);
							}
							break;
						}
					}
					if (i == bestCircles.size()) {
						bestCircles.insert(bestCircles.begin(), circle);
					}
				}
			}
		}
	}

	cout << bestCircles.size() << " aa" << endl;
	for (int i = 0; i < bestCircles.size(); i++) {
		int lineThickness = 4;
		int lineType = 10;
		int shift = 0;
		int xCoord = bestCircles[i].x;
		int yCoord = bestCircles[i].y;
		int radius = bestCircles[i].z;
		Point2f center(xCoord, yCoord);
		circle(dst, center, radius - 1, Scalar(255, 0, 0, 255), lineThickness, lineType, shift);
	}
}

void MyBinaryMorphologyDilation(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL) {
	Mat tmp = Mat(src.rows, src.cols, CV_32SC1, Scalar(0));

	for (int i = SE_ROW; i < src.rows - (4 - SE_ROW); i++) {
		for (int j = SE_COL; j < src.cols - (4 - SE_COL); j++) {
			if (src.at<Vec4b>(i, j)[0] > 0) {
				for (int a = 0; a < 5; a++) {
					for (int b = 0; b < 5; b++) {
						if (para[a * 5 + b] == 1) tmp.at<int>(i - SE_ROW + a, j - SE_COL + b) = 1;
					}
				}
			}
		}
	}
	for (int i = 0; i < tmp.rows; i++) {
		for (int j = 0; j < tmp.cols; j++) {
			if (tmp.at<int>(i, j) == 1) {
				dst.at<Vec4b>(i, j)[0] = 255;
				dst.at<Vec4b>(i, j)[1] = 255;
				dst.at<Vec4b>(i, j)[2] = 255;
			}
			else {
				dst.at<Vec4b>(i, j)[0] = 0;
				dst.at<Vec4b>(i, j)[1] = 0;
				dst.at<Vec4b>(i, j)[2] = 0;
			}
		}
	}
}

void MyBinaryMorphologyErosion(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL) {
	Mat tmp = Mat(src.rows, src.cols, CV_32SC1, Scalar(0));
	bool chosen;
	for (int i = SE_ROW; i < src.rows - (4 - SE_ROW); i++) {
		for (int j = SE_COL; j < src.cols - (4 - SE_COL); j++) {
			chosen = true;
			for (int a = 0; a < 5; a++) {
				for (int b = 0; b < 5; b++) {
					if (para[a * 5 + b] == 1) {
						if (src.at<Vec4b>(i - SE_ROW + a, j - SE_COL + b)[0] != 255) chosen = false;
					}
					else if (para[a * 5 + b] == 0) {
						if (src.at<Vec4b>(i - SE_ROW + a, j - SE_COL + b)[0] != 0) chosen = false;
					}
				}
			}
			if (chosen) tmp.at<int>(i, j) = 1;
			else tmp.at<int>(i, j) = 0;
		}
	}
	for (int i = 0; i < tmp.rows; i++) {
		for (int j = 0; j < tmp.cols; j++) {
			if (tmp.at<int>(i, j) == 1) {
				dst.at<Vec4b>(i, j)[0] = 255;
				dst.at<Vec4b>(i, j)[1] = 255;
				dst.at<Vec4b>(i, j)[2] = 255;
			}
			else {
				dst.at<Vec4b>(i, j)[0] = 0;
				dst.at<Vec4b>(i, j)[1] = 0;
				dst.at<Vec4b>(i, j)[2] = 0;
			}
		}
	}
}

void MyBinaryMorphologyOpen(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL) {
	Mat tmp = src.clone();
	cout << "open" << endl;
	//Mat tmp = Mat(src.rows, src.cols, src.type(), Scalar(0, 0, 0, 0));
	MyBinaryMorphologyErosion(src, tmp, size, para, SE_ROW, SE_COL);
	MyBinaryMorphologyDilation(tmp, dst, size, para, SE_ROW, SE_COL);
}

void MyBinaryMorphologyClose(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL) {
	Mat tmp = src.clone();
	cout << "close" << endl;
	//Mat tmp = Mat(src.rows, src.cols, src.type(), Scalar(0, 0, 0, 0));
	MyBinaryMorphologyDilation(src, tmp, size, para, SE_ROW, SE_COL);
	MyBinaryMorphologyErosion(tmp, dst, size, para, SE_ROW, SE_COL);
}

void MyHitOrMiss(Mat src, int mask[], Mat dst, int SE_ROW, int SE_COL) {
	int mask_c[25];
	for (int i = 0; i < 25; i++) {
		if (mask[i] == 1) mask_c[i] = 0;
		else if (mask[i] == 0) mask_c[i] = 1;
		else mask_c[i] = mask[i];

		//cout << mask[i] << " " << mask_c[i] << endl;
	}
	Mat src_c = Mat(src.rows, src.cols, src.type(), Scalar(0, 0, 0, 0));
	MatComplement(src, src_c);
	//imwrite("src_c.jpg", src_c);

	Mat tmp1 = Mat(src.rows, src.cols, src.type(), Scalar(0, 0, 0, 0));
	//Mat tmp1 = src.clone();
	MyBinaryMorphologyErosion(src, tmp1, 5, mask, SE_ROW, SE_COL);

	Mat tmp2 = Mat(src.rows, src.cols, src.type(), Scalar(0, 0, 0, 0));
	//Mat tmp2 = src.clone();
	//Mat src_c = src.clone();
	MyBinaryMorphologyErosion(src_c, tmp2, 5, mask_c, SE_ROW, SE_COL);

/*	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (tmp1.at<Vec4b>(i, j)[0] != tmp2.at<Vec4b>(i, j)[0]) cout << "diff" << endl;
		}
	}
	cout << "end" << endl;*/

	MatAnd(tmp1, tmp2, dst);
}

void MyBinaryMorphologyRough(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL) {
	//cout << "Rough" << endl;
	//Mat tmp = Mat(src.rows, src.cols, src.type(), Scalar(0, 0, 0, 0));
	Mat tmp = src.clone();
	for (int i = 0; i < 8; i++) {
		Mat tmp1 = tmp.clone();
		MyHitOrMiss(tmp, para, tmp1, SE_ROW, SE_COL);
		//MatComplement(tmp1, tmp1);
		//MatAnd(tmp, tmp1, tmp);
		MatOr(tmp, tmp1, tmp);

		int tmp_mask[25];
		tmp_mask[0] = para[10];
		tmp_mask[1] = para[5];
		tmp_mask[2] = para[0];
		tmp_mask[3] = para[1];
		tmp_mask[4] = para[2];
		tmp_mask[5] = para[15];
		tmp_mask[6] = para[11];
		tmp_mask[7] = para[6];
		tmp_mask[8] = para[7];
		tmp_mask[9] = para[3];
		tmp_mask[10] = para[20];
		tmp_mask[11] = para[16];
		tmp_mask[12] = para[12];
		tmp_mask[13] = para[8];
		tmp_mask[14] = para[4];
		tmp_mask[15] = para[21];
		tmp_mask[16] = para[17];
		tmp_mask[17] = para[18];
		tmp_mask[18] = para[13];
		tmp_mask[19] = para[9];
		tmp_mask[20] = para[22];
		tmp_mask[21] = para[23];
		tmp_mask[22] = para[24];
		tmp_mask[23] = para[19];
		tmp_mask[24] = para[14];
		for (int i = 0; i < 25; i++) {
			para[i] = tmp_mask[i];
		}
	}


	for (int i = 0; i < tmp.rows; i++) {
		for (int j = 0; j < tmp.cols; j++) {
			int a;
			if (tmp.at<Vec4b>(i, j)[0] == 255) a = 255;
			else  a = 0;

			dst.at<Vec4b>(i, j)[0] = a;
			dst.at<Vec4b>(i, j)[1] = a;
			dst.at<Vec4b>(i, j)[2] = a;
		}
	}
}

void MyBinaryMorphologyRefined(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL) {
	Mat tmp = src.clone();
	for (int i = 0; i < 8; i++) {
		Mat tmp1 = tmp.clone();
		MyHitOrMiss(tmp, para, tmp1, SE_ROW, SE_COL);
		MatComplement(tmp1, tmp1);
		MatAnd(tmp, tmp1, tmp);
		//MatOr(tmp, tmp1, tmp);

		int tmp_mask[25];
		tmp_mask[0] = para[10];
		tmp_mask[1] = para[5];
		tmp_mask[2] = para[0];
		tmp_mask[3] = para[1];
		tmp_mask[4] = para[2];
		tmp_mask[5] = para[15];
		tmp_mask[6] = para[11];
		tmp_mask[7] = para[6];
		tmp_mask[8] = para[7];
		tmp_mask[9] = para[3];
		tmp_mask[10] = para[20];
		tmp_mask[11] = para[16];
		tmp_mask[12] = para[12];
		tmp_mask[13] = para[8];
		tmp_mask[14] = para[4];
		tmp_mask[15] = para[21];
		tmp_mask[16] = para[17];
		tmp_mask[17] = para[18];
		tmp_mask[18] = para[13];
		tmp_mask[19] = para[9];
		tmp_mask[20] = para[22];
		tmp_mask[21] = para[23];
		tmp_mask[22] = para[24];
		tmp_mask[23] = para[19];
		tmp_mask[24] = para[14];
		for (int i = 0; i < 25; i++) {
			para[i] = tmp_mask[i];
		}
	}


	for (int i = 0; i < tmp.rows; i++) {
		for (int j = 0; j < tmp.cols; j++) {
			int a;
			if (tmp.at<Vec4b>(i, j)[0] == 255) a = 255;
			else  a = 0;

			dst.at<Vec4b>(i, j)[0] = a;
			dst.at<Vec4b>(i, j)[1] = a;
			dst.at<Vec4b>(i, j)[2] = a;
		}
	}
}

void MyBinaryMorphologyDistance(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL) {
	//cout << "Distance" << endl;
	int time = 0;
	Mat res = Mat(src.rows, src.cols, CV_32SC1, Scalar(0));
	Mat tmp = src.clone();
	//cout << SE_ROW<<" "<<SE_COL << endl;
	while (time < 255) {
		bool hasOne = false;
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				if (tmp.at<Vec4b>(i, j)[0] == 255) {
					res.at<int>(i, j) += 1;
					hasOne = true;
				}
				//cout << tmp.at<Vec4b>(i, j)[0] << endl;
			}
		}
		if (!hasOne) break;
		MyBinaryMorphologyErosion(tmp, tmp, 5, para, SE_ROW, SE_COL);
		time += 1;
		cout << time << endl;
	}

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			dst.at<Vec4b>(i, j)[0] = (float)res.at<int>(i, j) / time * 255.0;
			dst.at<Vec4b>(i, j)[1] = (float)res.at<int>(i, j) / time * 255.0;
			dst.at<Vec4b>(i, j)[2] = (float)res.at<int>(i, j) / time * 255.0;
		}
	}
}

void MyBinaryMorphologySkeleton(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL){
	int time = 0;
	Mat res = Mat(src.rows, src.cols, CV_32SC1, Scalar(0));
	Mat tmp = src.clone();
	//cout << SE_ROW<<" "<<SE_COL << endl;
	while (time < 255) {
		bool hasOne = false;
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				if (tmp.at<Vec4b>(i, j)[0] == 255) {
					res.at<int>(i, j) += 1;
					hasOne = true;
				}
				//cout << tmp.at<Vec4b>(i, j)[0] << endl;
			}
		}
		if (!hasOne) break;
		MyBinaryMorphologyErosion(tmp, tmp, 5, para, SE_ROW, SE_COL);
		time += 1;
		cout << time << endl;
	}

	Mat skeleton = Mat(src.rows, src.cols, CV_32SC1, Scalar(0));

	for (int i = 1; i < src.rows -1; i++) {
		for (int j = 1; j < src.cols - 1; j++) {
			int value = res.at<int>(i, j);
			if (((value >= res.at<int>(i - 1, j - 1) && para[6] != 0) || para[6] == 0) &&
				((value >= res.at<int>(i - 1, j) && para[7] != 0) || para[7] == 0) &&
				((value >= res.at<int>(i - 1, j + 1) && para[8] != 0) || para[8] == 0) &&
				((value >= res.at<int>(i, j - 1) && para[11] != 0) || para[11] == 0) &&
				((value >= res.at<int>(i, j + 1) && para[13] != 0) || para[13] == 0) &&
				((value >= res.at<int>(i + 1, j - 1) && para[16] != 0) || para[16] == 0) &&
				((value >= res.at<int>(i + 1, j) && para[17] != 0) || para[17] == 0) &&
				((value >= res.at<int>(i + 1, j + 1) && para[18] != 0) || para[18] == 0)) {
				skeleton.at<int>(i, j) = 1;
			}
			else {
				skeleton.at<int>(i, j) = 0;
			}
		}
	}

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (skeleton.at<int>(i, j) == 1) {
				dst.at<Vec4b>(i, j)[0] = 255;
				dst.at<Vec4b>(i, j)[1] = 255;
				dst.at<Vec4b>(i, j)[2] = 255;
			}
			else {
				dst.at<Vec4b>(i, j)[0] = 0;
				dst.at<Vec4b>(i, j)[1] = 0;
				dst.at<Vec4b>(i, j)[2] = 0;
			}
		}
	}
}

void MyBinaryMorphologySkeletonReconstruction(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL) {
	int time = 0;
	Mat res = Mat(src.rows, src.cols, CV_32SC1, Scalar(0));
	Mat tmp = src.clone();
	//cout << SE_ROW<<" "<<SE_COL << endl;
	while (time < 255) {
		bool hasOne = false;
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				if (tmp.at<Vec4b>(i, j)[0] == 255) {
					res.at<int>(i, j) += 1;
					hasOne = true;
				}
				//cout << tmp.at<Vec4b>(i, j)[0] << endl;
			}
		}
		if (!hasOne) break;
		MyBinaryMorphologyErosion(tmp, tmp, 5, para, SE_ROW, SE_COL);
		time += 1;
		cout << time << endl;
	}

	Mat skeleton = Mat(src.rows, src.cols, CV_32SC1, Scalar(0));

	for (int i = 1; i < src.rows - 1; i++) {
		for (int j = 1; j < src.cols - 1; j++) {
			int value = res.at<int>(i, j);
			if ( (value >= res.at<int>(i - 1, j - 1) && para[6] != 0)  &&
				(value >= res.at<int>(i - 1, j) && para[7] != 0)  &&
				(value >= res.at<int>(i - 1, j + 1) && para[8] != 0) &&
				(value >= res.at<int>(i , j - 1) && para[11] != 0) &&
				(value >= res.at<int>(i , j + 1) && para[13] != 0) &&
				(value >= res.at<int>(i + 1, j - 1) && para[16] != 0)  &&
				(value >= res.at<int>(i + 1, j ) && para[17] != 0)  &&
				(value >= res.at<int>(i + 1, j + 1) && para[18] != 0)  ) {
				skeleton.at<int>(i, j) = 1;
			}
			else {
				skeleton.at<int>(i, j) = 0;
			}
		}
	}



	/*for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (skeleton.at<int>(i, j) == 1) {
				int value = res.at<int>(i, j);
				int start_j = (j - value + 1 < 0) ? 0 : j - value + 1;
				int start_i = (i - value + 1 < 0) ? 0 : i - value + 1;
				int end_j = (j + value - 1 > src.cols - 1) ? src.cols - 1 : j + value - 1;
				int end_i = (i + value - 1 > src.rows - 1) ? src.rows - 1 : i + value - 1;
				for (int k = start_i; k <= end_i; k++) {
					for (int l = start_j; l <= end_j; l++) {
						reconst.at<int>(k, l) = 1;
					}
				}
			}
		}
	}*/
	int max = 0;
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (skeleton.at<int>(i, j) == 1) {
				if (res.at<int>(i, j) > max) max = res.at<int>(i, j);
			}
		}
	}

	Mat reconst = Mat(src.rows, src.cols, src.type(), Scalar(0, 0, 0, 0));
	for (int k = 1; k <= max; k++) {
		Mat tmp_r = Mat(src.rows, src.cols, src.type(), Scalar(0, 0, 0, 0));
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				if (skeleton.at<int>(i, j) == 1 && res.at<int>(i, j) == k) {
					tmp_r.at<Vec4b>(i, j)[0] = 255;
					tmp_r.at<Vec4b>(i, j)[1] = 255;
					tmp_r.at<Vec4b>(i, j)[2] = 255;
				}
			}
		}
		for (int l = 0; l < k; l++) {
			MyBinaryMorphologyDilation(tmp_r, tmp_r, 5, para, SE_ROW, SE_COL);
		}
		MatOr(reconst, tmp_r, reconst);
	}
	

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (reconst.at<Vec4b>(i, j)[0] == 255) {
				dst.at<Vec4b>(i, j)[0] = 255;
				dst.at<Vec4b>(i, j)[1] = 255;
				dst.at<Vec4b>(i, j)[2] = 255;
			}
			else {
				dst.at<Vec4b>(i, j)[0] = 0;
				dst.at<Vec4b>(i, j)[1] = 0;
				dst.at<Vec4b>(i, j)[2] = 0;
			}
		}
	}
}

void MyBinaryMorphologyReconstrustion(Mat src, Mat before_open, Mat dst, int size, int para[], int SE_ROW, int SE_COL) {
	Mat tmp = before_open.clone();
	Mat tmp2 = before_open.clone();
	do {
		tmp2 = tmp.clone();
		MyBinaryMorphologyDilation(tmp2, tmp, size, para, SE_ROW, SE_COL);
		MatAnd(src, tmp, tmp);
	} while (!MatEqual(tmp, tmp2));

	for (int i = 0; i<src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			dst.at<Vec4b>(i, j)[0] = tmp.at<Vec4b>(i, j)[0];
			dst.at<Vec4b>(i, j)[1] = tmp.at<Vec4b>(i, j)[0];
			dst.at<Vec4b>(i, j)[2] = tmp.at<Vec4b>(i, j)[0];
		}
	}
}

void MyGrayMorphologyDilation(Mat src, Mat dst, int length, int width) {
	int left, right, up, down;
	left = length / 2;
	if (length % 2 == 0)
		right = length / 2 - 1;
	else
		right = length / 2;
	up = width / 2;
	if (width % 2 == 0)
		down = width / 2 - 1;
	else
		down = width / 2;


	for (int i = 0; i < src.rows ; i++) {
		for (int j = 0; j < src.cols ; j++) {
			int max = 0;
			for (int k = -up; k <= down; k++) {
				for (int l = -left; l <= right; l++){
					int x = i + k, y = j + l;
					if (x < 0) x = 0;
					else if (x > src.rows - 1) x = src.rows - 1;
					if (y < 0) y = 0;
					else if (y > src.cols - 1) y = src.cols - 1;
					if(src.at<Vec4b>(x, y)[0] > max) max = src.at<Vec4b>(x, y)[0];
				}
			}
			dst.at<Vec4b>(i, j)[0] = max;
			dst.at<Vec4b>(i, j)[1] = max;
			dst.at<Vec4b>(i, j)[2] = max;
		}
	}
}
void MyGrayMorphologyErosion(Mat src, Mat dst, int length, int width) {
	int left, right, up, down;
	left = length / 2;
	if (length % 2 == 0)
		right = length / 2 - 1;
	else
		right = length / 2;
	up = width / 2;
	if (width % 2 == 0)
		down = width / 2 - 1;
	else
		down = width / 2;


	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			int min = 255;
			for (int k = -up; k <= down; k++) {
				for (int l = -left; l <= right; l++) {
					int x = i + k, y = j + l;
					if (x < 0) x = 0;
					else if (x > src.rows - 1) x = src.rows - 1;
					if (y < 0) y = 0;
					else if (y > src.cols - 1) y = src.cols - 1;
					if(src.at<Vec4b>(x, y)[0] < min) min = src.at<Vec4b>(x, y)[0];
				}
			}
			dst.at<Vec4b>(i, j)[0] = min;
			dst.at<Vec4b>(i, j)[1] = min;
			dst.at<Vec4b>(i, j)[2] = min;
		}
	}
}
void MyGrayMorphologyOpen(Mat src, Mat dst, int length, int width) {
	Mat tmp = src.clone();
	MyGrayMorphologyErosion(src, tmp, length, width);
	MyGrayMorphologyDilation(tmp, dst, length, width);
}
void MyGrayMorphologyClose(Mat src, Mat dst, int length, int width) {
	Mat tmp = src.clone();
	MyGrayMorphologyDilation(src, tmp, length, width);
	MyGrayMorphologyErosion(tmp, dst, length, width);
}

void MyGrayMorphologyReconstrustion(Mat src, Mat dst, int length, int width, int Rlength, int Rwidth, int mode) {
	//mode == 0 : open; mode == 1 : close
	//src 开的结果
	Mat result = src.clone();
	if (mode == 0)
		MyGrayMorphologyOpen(src, result, length, width);
	else
		MyGrayMorphologyClose(src, result, length, width);

	Mat tmp2 = Mat(src.rows, src.cols, CV_8UC4, Scalar(0, 0, 0, 1));
	do {
		tmp2 = result.clone();
		if(mode == 0)
			MyGrayMorphologyDilation(tmp2, result, Rlength, Rwidth);
		else
			MyGrayMorphologyErosion(tmp2, result, Rlength, Rwidth);
		for (int i = 0; i<src.rows; i++) {
			for (int j = 0; j<src.cols; j++) {
				int src_gray = src.at<Vec4b>(i, j)[0];
				int tmp_gray = result.at<Vec4b>(i, j)[0];
				if ((src_gray >= tmp_gray && mode == 0) || (src_gray <= tmp_gray && mode == 1)) {
					result.at<Vec4b>(i, j)[0] = tmp_gray;
					result.at<Vec4b>(i, j)[1] = tmp_gray;
					result.at<Vec4b>(i, j)[2] = tmp_gray;
				}
				else {
					result.at<Vec4b>(i, j)[0] = src_gray;
					result.at<Vec4b>(i, j)[1] = src_gray;
					result.at<Vec4b>(i, j)[2] = src_gray;
				}
			}
		}
	} while (!MatEqual(result, tmp2));
	for (int i = 0; i<src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			dst.at<Vec4b>(i, j)[0] = result.at<Vec4b>(i, j)[0];
			dst.at<Vec4b>(i, j)[1] = result.at<Vec4b>(i, j)[0];
			dst.at<Vec4b>(i, j)[2] = result.at<Vec4b>(i, j)[0];
		}
	}
}
void MyGrayMorphologyWatershed(Mat src, Mat dst, int distance) {
	map<int, vector<int>> grey2row;
	map<int, vector<int>> grey2col;
	Mat tmp = src.clone();
	int greyLevels[256], grey, size, x, y, counter, rootnum = 1;
	for (int i = 0; i<src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			tmp.at<Vec4b>(i, j)[0] /= distance;
		}
	}
	vector<int>greys;
	Mat mask = Mat(src.rows, src.cols, CV_32SC4, Scalar(0, 0, 0, 1));
	for (int i = 0; i<256; i++) {
		greyLevels[i] = 0;
	}
	int il, ih, jl, jh;
	queue<int>xqueue;
	queue<int>yqueue;
	for (int i = 0; i<src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			grey = tmp.at<Vec4b>(i, j)[0];
			greyLevels[grey]++;
			grey2row[grey].push_back(i);
			grey2col[grey].push_back(j);
		}
	}
	for (int i = 0; i<256; i++) {
		if (greyLevels[i]>0) {
			greys.push_back(i);
		}
	}
	size = grey2row[greys[0]].size();
	vector<int>is = grey2row[greys[0]];
	vector<int>js = grey2col[greys[0]];
	int i, j;
	for (int k = 0; k<1; k++) {
		xqueue.push(is[k]);
		yqueue.push(js[k]);
		while (!xqueue.empty()) {
			i = xqueue.front();
			j = yqueue.front();
			xqueue.pop();
			yqueue.pop();
			il = max(0, i - 1);
			ih = std::min(src.rows - 1, i + 1);
			jl = max(0, j - 1);
			jh = std::min(src.cols - 1, j + 1);
			if (mask.at<Vec4i>(i, j)[0] == 0) {
				if (mask.at<Vec4i>(i, j)[1] == 0) {
					mask.at<Vec4i>(i, j)[1] = rootnum;
					rootnum++;
				}
				mask.at<Vec4i>(i, j)[0] = 1;
				for (int m = il; m <= ih; m++) {
					for (int n = jl; n <= jh; n++) {
						if (tmp.at<Vec4b>(m, n)[0] == greys[0] && mask.at<Vec4i>(m, n)[1] == 0) {
							mask.at<Vec4i>(m, n)[1] = mask.at<Vec4i>(i, j)[1];
							xqueue.push(m);
							yqueue.push(n);
						}
						else if (mask.at<Vec4i>(m, n)[1]>0 && mask.at<Vec4i>(m, n)[1] != mask.at<Vec4i>(i, j)[1]) {
							mask.at<Vec4i>(m, n)[1] = -1;
							mask.at<Vec4i>(m, n)[0] = 2;
						}
						else if (mask.at<Vec4i>(m, n)[1] == 0) {
							mask.at<Vec4i>(m, n)[1] = mask.at<Vec4i>(i, j)[1];
						}
					}
				}
			}
		}
	}
		for (int k = 1; k<greys.size(); k++) {
			is = grey2row[greys[k]];
			js = grey2col[greys[k]];

			for (int p = 0; p<is.size(); p++) {
				i = is[p];
				j = js[p];
				if (mask.at<Vec4i>(i, j)[0] == 0) {
					xqueue.push(i);
					yqueue.push(j);
					while (!xqueue.empty()) {
						i = xqueue.front();
						j = yqueue.front();
						xqueue.pop();
						yqueue.pop();
						il = max(0, i - 1);
						ih = std::min(src.rows - 1, i + 1);
						jl = max(0, j - 1);
						jh = std::min(src.cols - 1, j + 1);
						if (mask.at<Vec4i>(i, j)[0] == 0) {
							if (mask.at<Vec4i>(i, j)[1] == 0) {
								mask.at<Vec4i>(i, j)[1] = rootnum;
								rootnum++;
							}
							mask.at<Vec4i>(i, j)[0] = 1;
							for (int m = il; m <= ih; m++) {
								for (int n = jl; n <= jh; n++) {
									if (tmp.at<Vec4b>(m, n)[0] == greys[k] && mask.at<Vec4i>(m, n)[1] == 0) {
										mask.at<Vec4i>(m, n)[1] = mask.at<Vec4i>(i, j)[1];
										xqueue.push(m);
										yqueue.push(n);
									}
									else if (mask.at<Vec4i>(m, n)[1]>0 && mask.at<Vec4i>(m, n)[1] != mask.at<Vec4i>(i, j)[1]) {
										mask.at<Vec4i>(m, n)[1] = -1;
										mask.at<Vec4i>(m, n)[0] = 2;
									}
									else if (mask.at<Vec4i>(m, n)[1] == 0) {
										mask.at<Vec4i>(m, n)[1] = mask.at<Vec4i>(i, j)[1];
									}
								}
							}
						}
					}
				}
			}
		}
	for (int i = 0; i<src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (mask.at<Vec4i>(i, j)[0] == 2) {
				dst.at<Vec4b>(i, j)[0] = 255;
				dst.at<Vec4b>(i, j)[1] = 0;
				dst.at<Vec4b>(i, j)[2] = 0;
			}
			else {
				dst.at<Vec4b>(i, j)[0] = src.at<Vec4b>(i, j)[0];
				dst.at<Vec4b>(i, j)[1] = src.at<Vec4b>(i, j)[0];
				dst.at<Vec4b>(i, j)[2] = src.at<Vec4b>(i, j)[0];
			}
		}
	}
}