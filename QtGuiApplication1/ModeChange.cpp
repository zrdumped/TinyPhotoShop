#include "QtGuiApplication1.h"
//是否是双阈值二值化
void QtGuiApplication1::ChangeHistMode2() {
	hasDouble = true;
	ui.spinBox_2->setEnabled(true);
	ui.horizontalSlider_2->setEnabled(true);
}
void QtGuiApplication1::ChangeHistMode1() {
	hasDouble = false;
	ui.spinBox_2->setEnabled(false);
	ui.horizontalSlider_2->setEnabled(false);
}


void QtGuiApplication1::ChangeListMode(int index) {
	//ui.Hitogram_tab->show();
	cmd = COMMAND(index);
	switch (cmd) {
	case BINARY:
		//ShowImage(QHist, HistLabel, QImage::Format_RGB888);
		ui.tabWidget->setCurrentIndex(1);
		ui.label_2->setText("BINARIZATION");
		ui.label->setText("");
		ui.Hitogram_tab->setEnabled(true);
		ui.spinBox->setEnabled(true);
		ui.horizontalSlider->setEnabled(true);
		ui.spinBox_2->setEnabled(false);
		ui.horizontalSlider_2->setEnabled(false);
		ui.spinBox_3->setEnabled(false);
		ui.horizontalSlider_3->setEnabled(false);
		ui.spinBox_4->setEnabled(false);
		ui.horizontalSlider_4->setEnabled(false);
		break;
	case ENHANCE:
		//ShowImage(QHist, HistLabel, QImage::Format_RGB888);
		ui.tabWidget->setCurrentIndex(1);
		ui.label_2->setText("ENHANCEMENT FROM a - b");
		ui.label->setText("TO c - d");
		ui.Hitogram_tab->setEnabled(true);
		ui.spinBox_2->setEnabled(true);
		ui.horizontalSlider_2->setEnabled(true);
		ui.spinBox_3->setEnabled(true);
		ui.horizontalSlider_3->setEnabled(true);
		ui.spinBox_4->setEnabled(true);
		ui.horizontalSlider_4->setEnabled(true);
		break;
	case FILTER:
		ui.tabWidget->setCurrentIndex(2);
		break;
		//ui.Hitogram_tab->setEnabled(false);
		//ui.tab_2->show();
	case EDGE:
		ui.tabWidget->setCurrentIndex(3);
		break;
		//ui.Hitogram_tab->setEnabled(false);
		//ui.tab_3->show();
	case HOUGH:
		ui.radioButton_4->setEnabled(true);
		ui.tabWidget->setCurrentIndex(3);
		break;
	case BMORPHOLOGY:
		ui.radioButton_4->setEnabled(false);
		ui.tabWidget->setCurrentIndex(2);
		break;
	case GMORPHOLOGY:
		ui.tabWidget->setCurrentIndex(2);
		break;
	default:
		break;
	}
}

//修改move的状态
void QtGuiApplication1::ChangeMoveMode(bool mode) {
	cmd = MOVE;
}


void QtGuiApplication1::ChangeEnhanceLinear() {
	enc = LINEAR;
}

void QtGuiApplication1::ChangeEnhanceSection() {
	enc = SECTION;
}

//此时d没有用处
void QtGuiApplication1::ChangeEnhanceLog() {
	enc = LOGARITHM;
}
void QtGuiApplication1::ChangeEnhanceExp() {
	enc = EXPONENTIAL;
}

void QtGuiApplication1::ChangeCutMode(bool mode) {
	cmd = CUT;
	ui.actionconfirmCut->setEnabled(true);
	ui.actioncancleCut->setEnabled(true);
	ui.actioncut->setChecked(true);

	CutLabel = new QLabel(ui.canvas);
	CutLabel->setGeometry(ImgLabel[CrtID]->geometry());
	CutLabel->setStyleSheet("QLabel{border-width : 2px;border-color : black;border-style:solid;}");
	CutLabel->show();

	ImgBeforeModify = CVimg[CrtID];
}

void QtGuiApplication1::ChangeRotateMode(bool mode) {
	cmd = ROTATE;
	ui.actionconfirmCut->setEnabled(true);
	ui.actioncancleCut->setEnabled(true);
	ui.actionrotate->setChecked(true);


	CutLabel = new QLabel(ui.canvas);
	CutLabel->setGeometry(ImgLabel[CrtID]->geometry());
	CutLabel->setStyleSheet("QLabel{border-width : 2px;border-color : black;border-style:solid;}");
	CutLabel->show();

	RotateCenter = new QLabel(ui.canvas);
	RotateCenter->setGeometry(ImgLabel[CrtID]->x() + ImgLabel[CrtID]->width() / 2, ImgLabel[CrtID]->y() + ImgLabel[CrtID]->height() / 2, 5, 5);
	RotateCenter->setStyleSheet("QLabel{border-width : 2px;border-color : black;border-style:solid;background-color:black;}");
	RotateCenter->show();



	ImgBeforeModify = CVimg[CrtID];
}

void QtGuiApplication1::ChangeResizeMode(bool mode) {
	cmd = RESIZE;
	ui.actionconfirmCut->setEnabled(true);
	ui.actioncancleCut->setEnabled(true);
	ui.actionresize->setChecked(true);

	CutLabel = new QLabel(ui.canvas);
	CutLabel->setGeometry(ImgLabel[CrtID]->geometry());
	CutLabel->setStyleSheet("QLabel{border-width : 2px;border-color : black;border-style:solid;}");
	CutLabel->show();

	ImgBeforeModify = CVimg[CrtID];
}


void QtGuiApplication1::ChangeFilterAverage() {
	flt = AVERAGE;
}
void QtGuiApplication1::ChangeFilterMedian() {
	flt = MEDIAN;
}
void QtGuiApplication1::ChangeFilterGauss() {
	flt = GAUSS;
}
void QtGuiApplication1::ChangeFilterPersonal() {
	flt = PERSONAL;
}
void QtGuiApplication1::ChangeEdgeSobel() {
	edg = SOBEL;
}
void QtGuiApplication1::ChangeEdgeLaplace() {
	edg = LAPLACE;
}
void QtGuiApplication1::ChangeEdgeCanny() {
	edg = CANNY;
}

void QtGuiApplication1::ChangeLevelR() {
	lt = LR;
	DrawHistogram(2);
}
void QtGuiApplication1::ChangeLevelG() {
	lt = LG;
	DrawHistogram(1);
}
void QtGuiApplication1::ChangeLevelB() {
	lt = LB;
	DrawHistogram(0);
}

void QtGuiApplication1::ChangeHoughLine() {
	ht = LINE;
}
void QtGuiApplication1::ChangeHoughCircle() {
	ht = CIRCLE;
	//MyHoughCircle(GrayImg[CrtID], CVimg[CrtID], 100, 50, 1, 100, 1, 500, 50);

}

void QtGuiApplication1::ChangeBMDilation() {
	bm = DILATION;
}
void QtGuiApplication1::ChangeBMErosion() {
	bm = EROSION;
}
void QtGuiApplication1::ChangeBMOpen() {
	bm = OPEN;
}
void QtGuiApplication1::ChangeBMClose() {
	bm = CLOSE;
}
void QtGuiApplication1::ChangeBMRough() {
	bm = ROUGH;
}
void QtGuiApplication1::ChangeBMRefined() {
	bm = REFINED;
}
void QtGuiApplication1::ChangeBMSkeleton() {
	bm = SKELETON;
}
void QtGuiApplication1::ChangeBMS_Reconst() {
	bm = S_RECONST;
}
void QtGuiApplication1::ChangeBMM_Rsconst() {
	bm = M_RECONST;
}
void QtGuiApplication1::ChangeBMDistance() {
	bm = DISTANCE;
}
void  QtGuiApplication1::ChangeGMDilation() {
	gm = G_DILATION;
}
void  QtGuiApplication1::ChangeGMErosion() {
	gm = G_EROSION;
}
void  QtGuiApplication1::ChangeGMOpen() {
	gm = G_OPEN;
}
void  QtGuiApplication1::ChangeGMClose() {
	gm = G_CLOSE;
}
void  QtGuiApplication1::ChangeGMReconst() {
	gm = G_RECONST;
}
void  QtGuiApplication1::ChangeGMWatershed() {
	gm = WATERSHED;
}