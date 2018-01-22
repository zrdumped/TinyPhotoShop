#pragma once
#include <opencv2/opencv.hpp>
#include <math.h>  
#define B 0
#define G 1
#define R 2
#define BILINEARITY 0
#define NEAREAST 1
#define pi 3.1415926
using namespace cv;

void MyCvtRGB2GRAY(Mat src, Mat dst);

IplImage* MyCreateHist(Mat src, int mode = R);

int MyOtusThreshold(Mat src);

void MyBinary(Mat src, Mat dst, int threshold);
void MyBinary(Mat src, Mat dst, int threshold, int threshold2);

void MyModifyRGBChannel(int channel, Mat src, Mat dst); //B-0, G-1, R-2

void MyAdd(Mat src, Mat mask, Mat dst, int port, Point2i* startPtSrc, Point2i* startPtMask, Point2i* endPtMask);
void MyAdd(Mat src, Mat mask, Mat dst, Point2i* startPtSrc, Point2i* startPtMask, Point2i* endPtMask);
void MyMinus(Mat src, Mat mask, Mat dst, Point2i* startPtSrc, Point2i* startPtMask, Point2i* endPtMask);
void MyMult(Mat src, Mat mask, Mat dst, int binary, Point2i* startPtSrc, Point2i* startPtMask, Point2i* endPtMask);

Mat MyResize(Mat src, float xRatio, float yRatio, int mode);
Mat MyRotate(Mat src, int Xstart, int Ystart, int Xend, int Yend, int mode, Point2d* newCenter);
Point2d RotateArdOrigin(Point2d src, double angle);

void MyEnhance(Mat src, Mat dst, int a, int b, int c, int d, int type);
void MyHistEqual(Mat src, Mat dst);


void MyFilterAverage(Mat src, Mat dst, int size);
void MyFilterMedian(Mat src, Mat dst, int size);
void MyFilterGauss(Mat src, Mat dst, int size, int sigma);
void MyFilterPersonal(Mat src, Mat dst, int size, int para[]);

void MyConvolution(Mat src, Mat dst,int para[]);
void MyEdgeSobel(Mat src, Mat dst, int threshold);
void MyEdgeSobel(Mat src, Mat dst, Mat GX, Mat GY);
void MyEdgeSobel(Mat src, Mat dst, Mat GX, Mat GY, Mat dist);
void MyEdgeLaplace(Mat src, Mat dst);
void MyEdgeCanny(Mat src, Mat dst, int high, int low, int sigma);

void MyAdjustHSV(Mat src, Mat dst, int dh, int ds, int dv);

void MyLevel(Mat src, Mat dst, int in_start, int in_end, float port, int out_start, int out_end, int mode);

void MyHoughLine(Mat src, Mat dst, int high, int low, int sigma, int MinNum);
void MyHoughCircle(Mat src, Mat dst, int high, int low, int sigma, int MinNum, int MinRadius, int MaxRadius, int  MinDist, int mode);

void MyBinaryMorphologyDilation(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL);
void MyBinaryMorphologyErosion(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL);
void MyBinaryMorphologyOpen(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL);
void MyBinaryMorphologyClose(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL);
void MyHitOrMiss(Mat src, int mask[], Mat dst, int SE_ROW, int SE_COL);
void MyBinaryMorphologyRough(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL);
void MyBinaryMorphologyRefined(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL);
void MyBinaryMorphologyDistance(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL);
void MyBinaryMorphologySkeleton(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL);
void MyBinaryMorphologySkeletonReconstruction(Mat src, Mat dst, int size, int para[], int SE_ROW, int SE_COL);
void MyBinaryMorphologyReconstrustion(Mat src, Mat before_open, Mat dst, int size, int para[], int SE_ROW, int SE_COL);

void MyGrayMorphologyDilation(Mat src, Mat dst, int length, int width);
void MyGrayMorphologyErosion(Mat src, Mat dst, int length, int width);
void MyGrayMorphologyOpen(Mat src, Mat dst, int length, int width);
void MyGrayMorphologyClose(Mat src, Mat dst, int length, int width);
void MyGrayMorphologyReconstrustion(Mat src, Mat dst, int length, int width, int Rlength, int Rwidth, int mode);
void MyGrayMorphologyWatershed(Mat src, Mat dst, int distance);
