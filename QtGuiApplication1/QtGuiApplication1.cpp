#include "QtGuiApplication1.h"

QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//addWin.setupUi(this);
	showMaximized();
	statusBar()->setSizeGripEnabled(false);//去掉状态栏右下角的三角  
	CrtID = -1;
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
}

void QtGuiApplication1::ChangeLayer(int num) {
	for (int i = 0; i < Layers.size(); i++) {
		if (num == i) Layers[i]->setChecked(true);
		else
		Layers[i]->setChecked(false);
	}
}

void QtGuiApplication1::ChangeLayer() {
	QPushButton*btn = qobject_cast<QPushButton*>(sender());
	for (int i = 0; i < Layers.size(); i++) {
		if (Layers[i]->text() == btn->text()) {
			Layers[i]->setChecked(true);
			CrtID = i;
			ImgLabel[i]->raise();
		}
		else
			Layers[i]->setChecked(false);
	}
}

QLabel* QtGuiApplication1::CreateLabel(Mat target) {
	QLabel* label_temp = new QLabel(ui.canvas);
	//将label移动到窗口的正中间
	label_temp->move(QPoint((ui.canvas->width() - target.cols)/2, (ui.canvas->height() - target.rows) / 2));

	return label_temp;
}

void QtGuiApplication1::CreateLayer(QString fn) {
	QPushButton* btn_temp = new QPushButton(ui.tab_8);
	for (int i = 0; i < Layers.size(); i++) {
		if (fn == Layers[i]->text()) fn += "_copy";
	}
	btn_temp->setText(fn);
	//btn_temp->setGeometry(QRect(50, 50, 300, 300));
	btn_temp->setCheckable(true);
	btn_temp->setChecked(true);
	btn_temp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	btn_temp->resize(273, 25);
	btn_temp->move(0, CrtID * 25);
	//btn_temp->setEnabled(false);
	btn_temp->setFont(QFont("Microsoft YaHei", 8, 75));
	btn_temp->show();
	connect(btn_temp, SIGNAL(clicked()), this, SLOT(ChangeLayer()));
	Layers.push_back(btn_temp);
	//ChangeLayer();
}

//显示图片到指定的label
QImage QtGuiApplication1::ShowImage(Mat src, QLabel* label, QImage::Format format = QImage::Format_ARGB32) {
	QImage Qimg_temp = QImage((const unsigned char*)(src.data),
		src.cols, src.rows, format);
	label->setPixmap(QPixmap::fromImage(Qimg_temp));
	label->resize(label->pixmap()->size());
	label->show();
	return Qimg_temp;
}
QImage QtGuiApplication1::ShowImage(QImage Qimg_temp, QLabel* label, QImage::Format format = QImage::Format_ARGB32) {
	label->setPixmap(QPixmap::fromImage(Qimg_temp));
	label->resize(label->pixmap()->size());
	label->show();
	return Qimg_temp;
}

//浏览文件并打开图片
void QtGuiApplication1::BroseAndOpenImage() {
	isGray = false;
	QString fileName = QFileDialog::getOpenFileName(this, NULL, NULL, "*.bmp *.jpeg *.png *.jpeg 2000 *.gif *.jpg");
	if (fileName == NULL) return;
	CrtID = CVimg.size();
	CreateLayer(fileName);
	ChangeLayer(CrtID);
	CVimg.push_back(imread(fileName.toStdString()));//读取图像  
	cvtColor(CVimg[CrtID], CVimg[CrtID], CV_RGB2RGBA);//图像在QT显示前，必须转化成QImage格式，将RGB格式转化成RGBA
	//记录初始图片
	SrcImg.push_back(CVimg[CrtID].clone());
	//记录灰度图片
	Mat gray_temp = CVimg[CrtID].clone();
	MyCvtRGB2GRAY(CVimg[CrtID], gray_temp);
	GrayImg.push_back(gray_temp);


	//创建图片所在的label并显示
	QLabel* label_temp = CreateLabel(CVimg[CrtID]);
	//setCentralWidget(label_temp);
	//label_temp->setGeometry(rect().x + 240, rect().y + 241);
	ImgLabel.push_back(label_temp);
	Qimg.push_back(ShowImage(CVimg[CrtID], ImgLabel[CrtID]));

	//gurantee the mouse move event cab be triggered without clicking the screen
	ui.canvas->setMouseTracking(true);
	this->centralWidget()->setMouseTracking(true);
	this->setMouseTracking(true);
	ImgLabel[CrtID]->setMouseTracking(true);

	ui.toolBox->setCurrentIndex(0);
	
	//在有图的时候修改按钮状态
	ui.Hitogram_tab->setEnabled(true);
	ui.radioButton_2->setEnabled(true);
	ui.radioButton_R->setEnabled(true);
	ui.radioButton_G->setEnabled(true);
	ui.radioButton_B->setEnabled(true);
	DrawHistogram();

}



//显示灰度图
void QtGuiApplication1::ConvertRGBToGray() {
	//MyCvtRGB2GRAY(SrcImg[CrtID], CVimg[CrtID]);
	Qimg[CrtID] = ShowImage(GrayImg[CrtID], ImgLabel[CrtID]);
}

//绘制灰度直方图
void QtGuiApplication1::DrawHistogram(int mode ) {
	if (mode == -1) {
		IplImage* hist = MyCreateHist(GrayImg[CrtID]);
		cvCvtColor(hist, hist, CV_BGR2RGB);
		//QHist = QImage(imgData, img->width, img->height, QImage::Format_RGB888);
		uchar *imgData = (uchar *)hist->imageData;
		QHist = QImage(imgData, hist->width, hist->height, QImage::Format_RGB888);
		HistLabel = ui.Hist;
		ShowImage(QHist, HistLabel, QImage::Format_RGB888);
	}
	else {
		IplImage* hist = MyCreateHist(SrcImg[CrtID], mode);
		cvCvtColor(hist, hist, CV_BGR2RGB);
		//QHist = QImage(imgData, img->width, img->height, QImage::Format_RGB888);
		uchar *imgData = (uchar *)hist->imageData;
		QHist = QImage(imgData, hist->width, hist->height, QImage::Format_RGB888);
		HistLabel = ui.Hist_2;
		ShowImage(QHist, HistLabel, QImage::Format_RGB888);
	}
}




void QtGuiApplication1::HistogramInput(int input) {
	//cout << input << endl;
	switch (cmd) {
	case BINARY:
		Binaryzation();
		break;
	case ENHANCE:
		Enhancement();
		break;
	default:
		break;
	}
}



void QtGuiApplication1::Enhancement() {
	int a, b, c, d;
	switch (enc) {
	case LINEAR: case SECTION: case LOGARITHM:case EXPONENTIAL:
		a= ui.horizontalSlider->value();
		b= ui.horizontalSlider_2->value();
		c = ui.horizontalSlider_3->value();
		d = ui.horizontalSlider_4->value();
		MyEnhance(GrayImg[CrtID], CVimg[CrtID], a, b, c, d, enc);
		break;
	default:return;
	}
	//cout << "3" << endl;
	Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
}

//二值化，根据阈值数目传参调用
void QtGuiApplication1::Binaryzation() {
	int threshold = ui.horizontalSlider->value();
	if (hasDouble) {
		int threshold2 = ui.horizontalSlider_2->value();
		if (threshold2 == threshold) threshold2 = ui.horizontalSlider->value();
		MyBinary(GrayImg[CrtID], CVimg[CrtID], threshold, threshold2);
	}
	else
		MyBinary(GrayImg[CrtID], CVimg[CrtID], threshold);
	Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
}



//使用大津算法计算阈值并绘制二值图
void QtGuiApplication1::OtusThreshold() {
	ui.Hitogram_tab->show();
	ChangeHistMode1();
	int threshold = MyOtusThreshold(GrayImg[CrtID]);
	MyBinary(GrayImg[CrtID], CVimg[CrtID], threshold);
	Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
	ui.horizontalSlider->setValue(threshold);
}

//修改颜色通道，如果是rgb则显示原图
void QtGuiApplication1::ModifyRGBChannel() {
	Qimg[CrtID] = ShowImage(GrayImg[CrtID], ImgLabel[CrtID]);
}

//只打开RGB一个通道并显示
void QtGuiApplication1::ModifyRChannel() {
	MyModifyRGBChannel(R, SrcImg[CrtID], CVimg[CrtID]);
	Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
}
void QtGuiApplication1::ModifyGChannel() {
	MyModifyRGBChannel(G, SrcImg[CrtID], CVimg[CrtID]);
	Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
}
void QtGuiApplication1::ModifyBChannel() {
	//MyModifyRGBChannel(B, SrcImg, CVimg);
	MyModifyRGBChannel(B, SrcImg[CrtID], CVimg[CrtID]);
	Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
}

void QtGuiApplication1::OpenAddWindow() {
	//新建一个窗口对象，其中的this指定了新的窗口的父对象是MainWindow，在销毁Mainwindow时也会销毁子对象，即该窗口对象
	addWin = new AddImageDialog(this);
	//Modal属性决定了show()应该将弹出的dialog设置为模态还是非模态
	//默认情况下该属性为false并且show（）弹出的窗口是非模态的
	//把这个属性设置为true就详单与设置QWidget::windowModality为Qt::ApplicationModal
	//执行exec（）忽略这个Modal这个属性，并且把弹出的dialog设置为模态的
	addWin->setModal(false);    //此处ture为模态，false为非模态
	addWin->setWindowTitle("Add Image");
	addWin->PrePareDialogAdd(Layers, CrtID);
	if (addWin->exec() == QDialog::Accepted) {
		int targetNum = addWin->TargetImage();
		int port = addWin->AddImagePort();
		//确定重叠范围，运行相应操作
		Point2i* startPtSrc = new Point2i(), *startPtMask = new Point2i(), *endPtMask = new Point2i();
		CalOverlap(ImgLabel[CrtID], ImgLabel[targetNum], startPtSrc, startPtMask, endPtMask);
		if(port == -1)
			MyAdd(CVimg[CrtID], CVimg[targetNum], CVimg[CrtID], startPtSrc, startPtMask, endPtMask);
		else
			MyAdd(CVimg[CrtID], CVimg[targetNum], CVimg[CrtID], port, startPtSrc, startPtMask, endPtMask);
		Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
	}
}

void QtGuiApplication1::OpenMinusWindow() {
	addWin = new AddImageDialog(this);
	addWin->setWindowTitle("Minus Image");
	addWin->PrePareDialogMinus(Layers, CrtID);
	if (addWin->exec() == QDialog::Accepted) {
		int targetNum = addWin->TargetImage();
		Point2i* startPtSrc = new Point2i(), *startPtMask = new Point2i(), *endPtMask = new Point2i();
		CalOverlap(ImgLabel[CrtID], ImgLabel[targetNum], startPtSrc, startPtMask, endPtMask);
		MyMinus(CVimg[CrtID], CVimg[targetNum], CVimg[CrtID], startPtSrc, startPtMask, endPtMask);
		Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
	}
}

void QtGuiApplication1::OpenMultWindow() {
	addWin = new AddImageDialog(this);
	addWin->setWindowTitle("Mult Image");
	addWin->PrePareDialogMult(Layers, CrtID);
	if (addWin->exec() == QDialog::Accepted) {
		int targetNum = addWin->TargetImage();
		bool binary = addWin->BinaryMult();
		Point2i* startPtSrc = new Point2i(), *startPtMask = new Point2i(), *endPtMask = new Point2i();
		CalOverlap(ImgLabel[CrtID], ImgLabel[targetNum], startPtSrc, startPtMask, endPtMask);
		MyMult(CVimg[CrtID], CVimg[targetNum], CVimg[CrtID], binary, startPtSrc, startPtMask, endPtMask);
		Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
	}
}

void QtGuiApplication1::CalOverlap(QLabel* src, QLabel* mask, Point2i* startPtSrc, Point2i* startPtMask, Point2i* endPtMask) {
	//相对于src坐标
	int x1 = -1, y1 = -1;
	//相对于mask坐标
	int x11 = -1, y11 = -1, x22 = -1, y22 = -1;
	int left = src->x(), right = src->x() + src->width(), up = src->y(), down = src->y() + src->height();
	//遍历mask所有点
	for (int i = mask->x(); i < mask->x() + mask->width(); i++) {
		for (int j = mask->y(); j <  mask->y() + mask->height(); j++) {
			//如果在范围内
			if (i >= left && i <= right && j >= up && j <= down) {
				//没有最小值
				if (x1 == -1) {
					x1 = i - left; y1 = j - up;
					x11 = i - mask->x(); y11 = j - mask->y();
				}
				//更新最大值
				if (i > x22) {
					x22 = i;
				}
				if (j > y22) {
					y22 = j;
				}

			}
		}
	}
	//最大值转化为相对坐标
	//if(x22 == right)
	//cout << right << " " << x22 << endl;
	//x22 -= mask->x() - 1; y22 -= mask->y() - 1;

	x22 -= (x22 == right - 1) ? mask->x() - 1 : mask->x();
	y22 -= (y22 == down - 1) ? mask->y() - 1 : mask->y();
	//赋值
	*startPtSrc =   Point2i(x1, y1);
	*startPtMask =   Point2i(x11, y11);
	*endPtMask =   Point2i(x22, y22);
	return;
}


void QtGuiApplication1::CancleCut() {
	ui.actionconfirmCut->setEnabled(false);
	ui.actioncancleCut->setEnabled(false);
	CutLabel->hide();
	if (cmd == CUT) {
		ui.actioncut->setChecked(false);
	}
	else if(cmd == RESIZE){
		ui.actionresize->setChecked(false);
	}
	else if (cmd == ROTATE) {
		ui.actionrotate->setChecked(false);
		RotateCenter->hide();
	}
	cmd = SPARE;
	CVimg[CrtID] = ImgBeforeModify;
	Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);

	Mat gray_temp = CVimg[CrtID].clone();
	MyCvtRGB2GRAY(CVimg[CrtID], gray_temp);
	GrayImg[CrtID] = gray_temp;
}

void QtGuiApplication1::ConfirmCut() {
	if (cmd == CUT) {
		ui.actioncut->setChecked(false);

		Point2i* startPtSrc = new Point2i(), *startPtMask = new Point2i(), *endPtMask = new Point2i();
		CalOverlap(CutLabel, ImgLabel[CrtID], startPtSrc, startPtMask, endPtMask);
		Mat CutResult = Mat(CutLabel->height(), CutLabel->width(), CVimg[CrtID].type(), Scalar(0, 0, 0, 255));
		MyAdd(CutResult, CVimg[CrtID], CutResult, startPtSrc, startPtMask, endPtMask);
		CVimg[CrtID] = CutResult;

		Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
		ImgLabel[CrtID]->move(CutLabel->x(), CutLabel->y());
	}
	else if (cmd == RESIZE) {
		ui.actionresize->setChecked(false);
	}
	else if (cmd == ROTATE) {
		ui.actionrotate->setChecked(false);

		RotateCenter->hide();
	}
	cmd = SPARE;
	Mat gray_temp = CVimg[CrtID].clone();
	MyCvtRGB2GRAY(CVimg[CrtID], gray_temp);
	GrayImg[CrtID] = gray_temp;
	

	CutLabel->hide();
	ui.actionconfirmCut->setEnabled(false);
	ui.actioncancleCut->setEnabled(false);
}

void QtGuiApplication1::ConfirmHist() {
	GrayImg[CrtID] = CVimg[CrtID].clone();
	DrawHistogram();
}
void QtGuiApplication1::CancleHist() {
	CVimg[CrtID] = GrayImg[CrtID].clone();
	Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
	DrawHistogram();
}


void QtGuiApplication1::HistEqual() {
	MyHistEqual(GrayImg[CrtID], CVimg[CrtID]);
	Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
	Mat temp = GrayImg[CrtID].clone();
	GrayImg[CrtID] = CVimg[CrtID].clone();
	DrawHistogram();
	GrayImg[CrtID] = temp.clone();
}

void  QtGuiApplication1::DrawFilter() {
	int size;
	if (ui.radioButton_5->isChecked() || !ui.radioButton_4->isEnabled()) size = 5;
	else size = 3;
	if (cmd == FILTER) {
		switch (flt) {
		case AVERAGE:
			MyFilterAverage(GrayImg[CrtID], CVimg[CrtID], size);
			Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
			break;
		case MEDIAN:
			MyFilterMedian(GrayImg[CrtID], CVimg[CrtID], size);
			Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
			break;
		case GAUSS:
			MyFilterGauss(GrayImg[CrtID], CVimg[CrtID], size, ui.horizontalSlider_5->value());
			Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
			break;
		case PERSONAL:
			if (size == 3) {
				int para[9];
				para[0] = ui.spinBox_11->value();
				para[1] = ui.spinBox_12->value();
				para[2] = ui.spinBox_13->value();
				para[3] = ui.spinBox_16->value();
				para[4] = ui.spinBox_17->value();
				para[5] = ui.spinBox_18->value();
				para[6] = ui.spinBox_21->value();
				para[7] = ui.spinBox_22->value();
				para[8] = ui.spinBox_23->value();
				MyFilterPersonal(GrayImg[CrtID], CVimg[CrtID], size, para);
			}
			else {
				int para[25];
				para[0] = ui.spinBox_5->value();
				para[1] = ui.spinBox_6->value();
				para[2] = ui.spinBox_7->value();
				para[3] = ui.spinBox_8->value();
				para[4] = ui.spinBox_9->value();
				para[5] = ui.spinBox_10->value();
				para[6] = ui.spinBox_11->value();
				para[7] = ui.spinBox_12->value();
				para[8] = ui.spinBox_13->value();
				para[9] = ui.spinBox_14->value();
				para[10] = ui.spinBox_15->value();
				para[11] = ui.spinBox_16->value();
				para[12] = ui.spinBox_17->value();
				para[13] = ui.spinBox_18->value();
				para[14] = ui.spinBox_19->value();
				para[15] = ui.spinBox_20->value();
				para[16] = ui.spinBox_21->value();
				para[17] = ui.spinBox_22->value();
				para[18] = ui.spinBox_23->value();
				para[19] = ui.spinBox_24->value();
				para[20] = ui.spinBox_25->value();
				para[21] = ui.spinBox_26->value();
				para[22] = ui.spinBox_27->value();
				para[23] = ui.spinBox_28->value();
				para[24] = ui.spinBox_29->value();
				MyFilterPersonal(GrayImg[CrtID], CVimg[CrtID], size, para);
			}
			Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
			break;
		default:
			return;
		}
	}
	else if (cmd == BMORPHOLOGY) {
		int SE_ROW;
		int SE_COL;
		int para[25];
		para[0] = ui.spinBox_5->value();
		para[1] = ui.spinBox_6->value();
		para[2] = ui.spinBox_7->value();
		para[3] = ui.spinBox_8->value();
		para[4] = ui.spinBox_9->value();
		para[5] = ui.spinBox_10->value();
		para[6] = ui.spinBox_11->value();
		para[7] = ui.spinBox_12->value();
		para[8] = ui.spinBox_13->value();
		para[9] = ui.spinBox_14->value();
		para[10] = ui.spinBox_15->value();
		para[11] = ui.spinBox_16->value();
		para[12] = ui.spinBox_17->value();
		para[13] = ui.spinBox_18->value();
		para[14] = ui.spinBox_19->value();
		para[15] = ui.spinBox_20->value();
		para[16] = ui.spinBox_21->value();
		para[17] = ui.spinBox_22->value();
		para[18] = ui.spinBox_23->value();
		para[19] = ui.spinBox_24->value();
		para[20] = ui.spinBox_25->value();
		para[21] = ui.spinBox_26->value();
		para[22] = ui.spinBox_27->value();
		para[23] = ui.spinBox_28->value();
		para[24] = ui.spinBox_29->value();
		SE_ROW = ui.comboBox->currentIndex();
		SE_COL = ui.comboBox_2->currentIndex();
		switch (bm) {
		case DILATION:
			MyBinaryMorphologyDilation(GrayImg[CrtID], CVimg[CrtID], size, para, SE_ROW, SE_COL);
			break;
		case EROSION:
			MyBinaryMorphologyErosion(GrayImg[CrtID], CVimg[CrtID], size, para, SE_ROW, SE_COL);
			break;
		case OPEN:
			MyBinaryMorphologyOpen(GrayImg[CrtID], CVimg[CrtID], size, para, SE_ROW, SE_COL);
			break;
		case CLOSE:
			MyBinaryMorphologyClose(GrayImg[CrtID], CVimg[CrtID], size, para, SE_ROW, SE_COL);
			break;
		case ROUGH:
			MyBinaryMorphologyRough(GrayImg[CrtID], CVimg[CrtID], size, para, SE_ROW, SE_COL);
			break;
		case REFINED:
			MyBinaryMorphologyRefined(GrayImg[CrtID], CVimg[CrtID], size, para, SE_ROW, SE_COL);
			break;
		case DISTANCE:
			MyBinaryMorphologyDistance(GrayImg[CrtID], CVimg[CrtID], size, para, SE_ROW, SE_COL);
			break;
		case SKELETON:
			MyBinaryMorphologySkeleton(GrayImg[CrtID], CVimg[CrtID], size, para, SE_ROW, SE_COL);
			break;
		case S_RECONST:
			MyBinaryMorphologySkeletonReconstruction(GrayImg[CrtID], CVimg[CrtID], size, para, SE_ROW, SE_COL);
			break;
		case M_RECONST:
			//必须在开操作后进行
			MyBinaryMorphologyReconstrustion(GrayImg[CrtID], CVimg[CrtID], CVimg[CrtID], size, para, SE_ROW, SE_COL);
			break;
		default:break;
		}
		Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
	}
	else if (cmd == GMORPHOLOGY) {
		int length = ui.spinBox_40->value();
		int width = ui.spinBox_41->value();
		int Rlength = ui.spinBox_42->value();
		int Rwidth = ui.spinBox_43->value();
		int distance = ui.spinBox_44->value();
		switch (gm) {
		case G_DILATION:
			MyGrayMorphologyDilation(GrayImg[CrtID], CVimg[CrtID], length, width);
			break;
		case G_EROSION:
			MyGrayMorphologyErosion(GrayImg[CrtID], CVimg[CrtID], length, width);
			break;
		case G_OPEN:
			MyGrayMorphologyOpen(GrayImg[CrtID], CVimg[CrtID], length, width);
			break;
		case G_CLOSE:
			MyGrayMorphologyClose(GrayImg[CrtID], CVimg[CrtID], length, width);
			break;
		case WATERSHED:
			MyGrayMorphologyWatershed(GrayImg[CrtID], CVimg[CrtID], distance);
			break;
		case G_RECONST:
			if(ui.radioButton_11->isChecked())
				MyGrayMorphologyReconstrustion(GrayImg[CrtID], CVimg[CrtID], length, width, Rlength, Rwidth, 0);
			else 
				MyGrayMorphologyReconstrustion(GrayImg[CrtID], CVimg[CrtID], length, width, Rlength, Rwidth, 1);
			break;
		default:break;
		}
		Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
	}
}

void  QtGuiApplication1::CancleFilter() {
	CVimg[CrtID] = GrayImg[CrtID].clone();
	Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
}

void QtGuiApplication1::ConfirmFilter() {
	GrayImg[CrtID] = CVimg[CrtID].clone();
	DrawHistogram();
}

void QtGuiApplication1::DrawEdge() {
	cout << cmd << endl;
	if (cmd == EDGE) {
		if (edg == SOBEL) {
			MyEdgeSobel(GrayImg[CrtID], CVimg[CrtID], ui.horizontalSlider_6->value());
			Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
			/*Mat ax, ay;
			Mat axx, ayy;
			Sobel(GrayImg[CrtID], ax, CV_16S, 1, 0, -1);
			Sobel(GrayImg[CrtID], ay, CV_16S, 0, 1, -1);
			convertScaleAbs(ax, axx);      //将CV_16S转为CV_8U
			convertScaleAbs(ay, ayy);
			addWeighted(axx, 0.5, ayy, 0.5, 0, GrayImg[CrtID]);     //将两图相加
			imshow("效果图", GrayImg[CrtID]);*/
		}
		else if (edg == LAPLACE) {
			MyEdgeLaplace(GrayImg[CrtID], CVimg[CrtID]);
			Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
		}
		else if (edg == CANNY) {
			int low = ui.spinBox_31->value();
			int high = ui.spinBox_30->value();
			int sigma = ui.horizontalSlider_6->value();
			MyEdgeCanny(GrayImg[CrtID], CVimg[CrtID], high, low, sigma);
			Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
		}
	}
	else if (cmd == HOUGH) {
		cout << ht << endl;
		if (ht == LINE) {
			cout << "st" << endl;
			int low = ui.spinBox_31->value();
			int high = ui.spinBox_30->value();
			int sigma = ui.horizontalSlider_6->value();
			int MinNum = ui.spinBox_36->value();
			MyHoughLine(GrayImg[CrtID], CVimg[CrtID], high, low, sigma, MinNum);
			cout << "ed" << endl;
		}
		else if (ht == CIRCLE) {
			cout << "st" << endl;
			int low = ui.spinBox_31->value();
			int high = ui.spinBox_30->value();
			int sigma = ui.horizontalSlider_6->value();
			int MinNum = ui.spinBox_36->value();
			int MaxRadius = ui.spinBox_37->value();
			int MinRadius = ui.spinBox_38->value();
			int MinDist = ui.spinBox_39->value();
			//canny
			if(ui.radioButton_8->isChecked())
				MyHoughCircle(GrayImg[CrtID], CVimg[CrtID], high, low, sigma, MinNum, MinRadius, MaxRadius, MinDist, 1);
			//sobel
			else
				MyHoughCircle(GrayImg[CrtID], CVimg[CrtID], high, low, sigma, MinNum, MinRadius, MaxRadius, MinDist, 0);
			cout << "ed" << endl;
		}
		Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
	}
}

void QtGuiApplication1::ModifyHSV() {
	int H = ui.horizontalSlider_9->value();
	int S = ui.horizontalSlider_8->value();
	int V = ui.horizontalSlider_7->value();
	cout << H << " " << S << " " << V << endl;
	MyAdjustHSV(SrcImg[CrtID], CVimg[CrtID], H, S, V);
	
	Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
}

void QtGuiApplication1::DrawLevel() {
	int in_start = ui.spinBox_32->value();
	int in_end = ui.spinBox_33->value();
	float port = ui.doubleSpinBox->value();
	int out_start = ui.spinBox_34->value();
	int out_end = ui.spinBox_35->value();
	MyLevel(SrcImg[CrtID], CVimg[CrtID], in_start, in_end, port, out_start, out_end, lt);
	Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
}

void QtGuiApplication1::ConfirmLevel() {
	SrcImg[CrtID] = CVimg[CrtID].clone();
	DrawHistogram(lt);
}
void QtGuiApplication1::CancleLevel() {
	CVimg[CrtID] = SrcImg[CrtID].clone();
	Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
}

void QtGuiApplication1::SaveImage() {
	QString fileName = QFileDialog::getSaveFileName(this, NULL, NULL, "*.bmp *.jpeg *.png *.jpeg 2000 *.gif *.jpg");
	if (fileName == NULL) return;
	imwrite(fileName.toStdString(), CVimg[CrtID]);
}
/*
void QtGuiApplication1::disableButton() {
	ui.pushButton->setEnabled(false);
	ui.pushButton_3->setEnabled(false);
	ui.pushButton_4->setEnabled(false);
	ui.pushButton_5->setEnabled(false);
	ui.pushButton_6->setEnabled(false);
	ui.pushButton_7->setEnabled(false);
	ui.pushButton_8->setEnabled(false);
	ui.pushButton_9->setEnabled(false);
	ui.pushButton_10->setEnabled(false);
	ui.pushButton_12->setEnabled(false);
	ui.pushButton_13->setEnabled(false);
	ui.pushButton_14->setEnabled(false);
	ui.pushButton_19->setEnabled(false);
	ui.pushButton_20->setEnabled(false);
	ui.pushButton_21->setEnabled(false);
	ui.pushButton_27->setEnabled(false);
	ui.pushButton_26->setEnabled(false);
	ui.pushButton_28->setEnabled(false);
	ui.pushButton_29->setEnabled(false);
	ui.pushButton_30->setEnabled(false);
	ui.pushButton_31->setEnabled(false);
	ui.pushButton_32->setEnabled(false);
	ui.pushButton_33->setEnabled(false);
	ui.pushButton_34->setEnabled(false);
	ui.pushButton_35->setEnabled(false);
	ui.pushButton_36->setEnabled(false);
	ui.pushButton_37->setEnabled(false);
	ui.pushButton_38->setEnabled(false);
	ui.pushButton_39->setEnabled(false);
	ui.pushButton_40->setEnabled(false);
	ui.pushButton_41->setEnabled(false);
	ui.pushButton_42->setEnabled(false);
	ui.pushButton_43->setEnabled(false);


	ui.horizontalSlider_7->setEnabled(false);
	ui.horizontalSlider_8->setEnabled(false);
	ui.horizontalSlider_9->setEnabled(false);

	ui.radioButton_6->setEnabled(false);
	ui.radioButton_7->setEnabled(false);
	ui.radioButton_9->setEnabled(false);
}*/