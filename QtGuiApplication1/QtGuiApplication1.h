#pragma once
#include <vector>
#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication1.h"
#include <opencv2/opencv.hpp>
#include "QLabel"
#include "QFileDialog"
#include "QMouseEvent"
#include "QtGui"
#include <qstatusbar.h>
#include <GrayHistogram.h>
#include <AddImageDialog.h>
#include <QPainter>  
#include <QPen>  
#include <MyCV.h>
#include <windows.h>
#include <string.h>

using namespace cv;
using namespace std;
enum MoveLine{ NO, LEFT, RIGHT, UP, DOWN, LEFT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_DOWN, CENTER};
enum COMMAND { COLOR, BINARY, ENHANCE, FILTER, EDGE, HOUGH, BMORPHOLOGY, GMORPHOLOGY, MOVE, CUT, RESIZE, ROTATE, SPARE};
enum ENHANCE_TYPE { LINEAR, SECTION, LOGARITHM, EXPONENTIAL, EQUAL, NONE };
enum FILTER_TYPE {AVERAGE, MEDIAN, GAUSS, PERSONAL, NO_FILTER};
enum EDGE_TYPE {SOBEL, LAPLACE, CANNY};
enum LEVEL_TYPE{LB,LG,LR};
enum HOUGH_TYPE{LINE, CIRCLE};
enum BINARY_MORPHOLOGY_TYPE{ DILATION, EROSION, OPEN, CLOSE, ROUGH, REFINED, DISTANCE, SKELETON, S_RECONST, M_RECONST };
enum GRAY_MORPHOLOGY_TYPE{G_DILATION, G_EROSION, G_OPEN, G_CLOSE, WATERSHED, G_RECONST};





class QtGuiApplication1 : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication1(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtGuiApplication1Class ui;
	AddImageDialog* addWin;

	vector<Mat> CVimg;
	vector<Mat> SrcImg;
	vector<Mat> GrayImg;
	vector<QImage> Qimg;
	QImage QHist;
	vector<QLabel*> ImgLabel;
	vector<QPushButton*> Layers;
	QLabel* HistLabel;
	QLabel* CutLabel;
	QLabel* RotateCenter;
	Point2d* RotateStart;
	Mat ImgBeforeModify;
	int CrtID;
	int x;
	int y;
	bool isGray = false;
	bool hasDouble = false;

	COMMAND cmd = SPARE;
	ENHANCE_TYPE enc = NONE;
	FILTER_TYPE flt = NO_FILTER;
	EDGE_TYPE edg;
	LEVEL_TYPE lt = LR;
	HOUGH_TYPE ht = LINE;
	BINARY_MORPHOLOGY_TYPE bm = DILATION;
	GRAY_MORPHOLOGY_TYPE gm = G_DILATION;
	bool hasMove = false;
	MoveLine lineNum = NO;
	int RelX = 0;
	int RelY = 0;

	bool pressed = false;
	void CalOverlap(QLabel* src, QLabel* mask, Point2i* startPtSrc, Point2i* startPtMask, Point2i* endPtMask);

	QImage ShowImage(Mat src, QLabel* label, QImage::Format format);
	QImage ShowImage(QImage Qimg_temp, QLabel* label, QImage::Format format);
	
private slots:
	void BroseAndOpenImage();
	void ConvertRGBToGray();
	void DrawHistogram(int mode = -1);

	void ChangeHistMode2();
	void ChangeHistMode1();
	void ChangeEnhanceLinear();
	void ChangeEnhanceSection();
	void ChangeEnhanceExp();
	void ChangeEnhanceLog();
	void ChangeMoveMode(bool mode);
	void ChangeCutMode(bool mode);
	void ChangeRotateMode(bool mode);
	void ChangeResizeMode(bool mode);
	void ChangeListMode(int index);
	void ChangeFilterAverage();
	void ChangeFilterMedian();
	void ChangeFilterGauss();
	void ChangeFilterPersonal();
	void ChangeEdgeSobel();
	void ChangeEdgeLaplace();
	void ChangeEdgeCanny();
	void ChangeLevelR();
	void ChangeLevelG();
	void ChangeLevelB();
	void ChangeHoughLine();
	void ChangeHoughCircle();

	void ChangeBMDilation();
	void ChangeBMErosion();
	void ChangeBMOpen();
	void ChangeBMClose();
	void ChangeBMRough();
	void ChangeBMRefined();
	void ChangeBMSkeleton();
	void ChangeBMS_Reconst();
	void ChangeBMM_Rsconst();
	void ChangeBMDistance();

	void ChangeGMDilation();
	void ChangeGMErosion();
	void ChangeGMOpen();
	void ChangeGMClose();
	void ChangeGMReconst();
	void ChangeGMWatershed();

	void ModifyHSV();

	void HistEqual();

	void ConfirmCut();
	void CancleCut();
	void ConfirmHist();
	void CancleHist();
	void CancleFilter();
	void ConfirmFilter();
	void ConfirmLevel();
	void CancleLevel();

	void HistogramInput(int input);
	void Binaryzation();
	void Enhancement();
	void OtusThreshold();
	void DrawFilter();
	void DrawEdge();
	void DrawLevel();

	void ModifyRChannel();
	void ModifyGChannel();
	void ModifyBChannel();
	void ModifyRGBChannel();

	QLabel* CreateLabel(Mat target);
	void CreateLayer(QString fn);
	//void SetPenStatus(bool act);
	//void DrawWithPen();

	void ChangeLayer(int num);
	void ChangeLayer();
	void OpenAddWindow();
	void OpenMinusWindow();
	void OpenMultWindow();
	void SaveImage();



protected:
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
};
