#include "QtGuiApplication1.h"

//鼠标移动事件
void QtGuiApplication1::mouseMoveEvent(QMouseEvent* event) {
	//获得相对于图片的坐标
	x = event->x() - ImgLabel[CrtID]->x() - ui.canvas->x();// -xoffset;
	y = event->y() - ImgLabel[CrtID]->y() - ui.canvas->y() - ui.menuBar->height() - ui.toolBar->height();// -yoffset;
																										 //超出范围不显示信息
	if (x < 0 || x > Qimg[CrtID].width() || y < 0 || y > Qimg[CrtID].height()) {
		statusBar()->showMessage("");
	}
	//显示位置和颜色信息或灰度信息
	else {
		QRgb rgb = Qimg[CrtID].pixel(x, y);
		if (!isGray)
			statusBar()->showMessage(QString("Pos: (%1, %2); RGB: (%3, %4, %5)").arg
			(QString::number(x), QString::number(y)
				, QString::number(qBlue(rgb)), QString::number(qGreen(rgb)), QString::number(qRed(rgb))));
		else
			statusBar()->showMessage(QString("Pos: (%1, %2); GRAY: %3").arg
			(QString::number(x), QString::number(y)
				, QString::number(qRed(rgb))));
	}
	//如果鼠标按下的话
	//为了保证在不按下的时候也能出发鼠标移动时间所以修改了参数，是否按下只能人工判断
	if (pressed) {
		//如果是移动状态
		if (cmd == MOVE && hasMove) {
			//位移图片
			int x_tmp = event->x() - RelX - ui.canvas->x();
			int y_tmp = event->y() - RelY - ui.canvas->y() - ui.menuBar->height() - ui.toolBar->height();
			ImgLabel[CrtID]->move(QPoint(x_tmp, y_tmp));
		}
		else if (cmd == CUT) {
			//cout << lineNum << endl;
			int left = CutLabel->x();
			//int right = CutLabel->x() + CutWidStart;
			int right = CutLabel->x() + CutLabel->width();
			int up = CutLabel->y();
			int down = CutLabel->y() + CutLabel->height();
			RelX = event->x() - ui.canvas->x();// -xoffset;
			RelY = event->y() - ui.canvas->y() - ui.menuBar->height() - ui.toolBar->height();// -yoffset;
			switch (lineNum) {
			case NO:case CENTER:return;
			case LEFT_UP:
				if (RelX < ImgLabel[CrtID]->x()) RelX = ImgLabel[CrtID]->x();
				if (RelX > right) RelX = right;
				if (RelY < ImgLabel[CrtID]->y()) RelY = ImgLabel[CrtID]->y();
				if (RelY > down) RelY = down;
				CutLabel->resize(CutLabel->width() + (left - RelX) / 2, CutLabel->height() + (up - RelY));
				CutLabel->move(CutLabel->x() - (left - RelX) / 2, CutLabel->y() - (up - RelY));
				break;
			case LEFT_DOWN:
				if (RelX < ImgLabel[CrtID]->x()) RelX = ImgLabel[CrtID]->x();
				if (RelX > right) RelX = right;
				if (RelY > ImgLabel[CrtID]->y() + ImgLabel[CrtID]->height()) RelY = ImgLabel[CrtID]->y() + ImgLabel[CrtID]->height();
				if (RelY < up) RelY = up;
				CutLabel->resize(CutLabel->width() + (left - RelX) / 2, CutLabel->height() + (RelY - down));
				CutLabel->move(CutLabel->x() - (left - RelX) / 2, CutLabel->y());
				break;
			case RIGHT_UP:
				if (RelX > ImgLabel[CrtID]->x() + ImgLabel[CrtID]->width()) RelX = ImgLabel[CrtID]->x() + ImgLabel[CrtID]->width();
				if (RelX < left) RelX = left;
				if (RelY < ImgLabel[CrtID]->y()) RelY = ImgLabel[CrtID]->y();
				if (RelY > down) RelY = down;
				CutLabel->resize(CutLabel->width() + (RelX - right), CutLabel->height() + (up - RelY));
				CutLabel->move(CutLabel->x(), CutLabel->y() - (up - RelY));
				break;
			case RIGHT_DOWN:
				if (RelX > ImgLabel[CrtID]->x() + ImgLabel[CrtID]->width()) RelX = ImgLabel[CrtID]->x() + ImgLabel[CrtID]->width();
				if (RelX < left) RelX = left;
				if (RelY > ImgLabel[CrtID]->y() + ImgLabel[CrtID]->height()) RelY = ImgLabel[CrtID]->y() + ImgLabel[CrtID]->height();
				if (RelY < up) RelY = up;
				CutLabel->resize(CutLabel->width() + (RelX - right), CutLabel->height() + (RelY - down));
				break;
			case LEFT:
				if (RelX < ImgLabel[CrtID]->x()) RelX = ImgLabel[CrtID]->x();
				if (RelX > right) RelX = right;
				CutLabel->resize(CutLabel->width() + (left - RelX) / 2, CutLabel->height());
				CutLabel->move(CutLabel->x() - (left - RelX) / 2, CutLabel->y());
				break;
			case RIGHT:
				if (RelX > ImgLabel[CrtID]->x() + ImgLabel[CrtID]->width()) RelX = ImgLabel[CrtID]->x() + ImgLabel[CrtID]->width();
				if (RelX < left) RelX = left;
				CutLabel->resize(CutLabel->width() + (RelX - right), CutLabel->height());
				break;
			case UP:
				if (RelY < ImgLabel[CrtID]->y()) RelY = ImgLabel[CrtID]->y();
				if (RelY > down) RelY = down;
				CutLabel->resize(CutLabel->width(), CutLabel->height() + (up - RelY));
				CutLabel->move(CutLabel->x(), CutLabel->y() - (up - RelY));
				break;
			case DOWN:
				if (RelY > ImgLabel[CrtID]->y() + ImgLabel[CrtID]->height()) RelY = ImgLabel[CrtID]->y() + ImgLabel[CrtID]->height();
				if (RelY < up) RelY = up;
				CutLabel->resize(CutLabel->width(), CutLabel->height() + (RelY - down));
				break;
			}
		}
		else if (cmd == RESIZE) {
			int left = CutLabel->x();
			int right = CutLabel->x() + CutLabel->width();
			int up = CutLabel->y();
			int down = CutLabel->y() + CutLabel->height();
			RelX = event->x() - ui.canvas->x();// -xoffset;
			RelY = event->y() - ui.canvas->y() - ui.menuBar->height() - ui.toolBar->height();// -yoffset;
			float xRatio, yRatio;
			switch (lineNum) {
			case NO:case CENTER:return;
			case LEFT_UP:
				if (RelX > right) RelX = right;
				if (RelY > down) RelY = down;
				CutLabel->resize(CutLabel->width() + (left - RelX) / 2, CutLabel->height() + (up - RelY));
				CutLabel->move(CutLabel->x() - (left - RelX) / 2, CutLabel->y() - (up - RelY));
				break;
			case LEFT_DOWN:
				if (RelX > right) RelX = right;
				if (RelY < up) RelY = up;
				CutLabel->resize(CutLabel->width() + (left - RelX) / 2, CutLabel->height() + (RelY - down));
				CutLabel->move(CutLabel->x() - (left - RelX) / 2, CutLabel->y());
				break;
			case RIGHT_UP:
				if (RelX < left) RelX = left;
				if (RelY > down) RelY = down;
				CutLabel->resize(CutLabel->width() + (RelX - right), CutLabel->height() + (up - RelY));
				CutLabel->move(CutLabel->x(), CutLabel->y() - (up - RelY));
				break;
			case RIGHT_DOWN:
				if (RelX < left) RelX = left;
				if (RelY < up) RelY = up;
				CutLabel->resize(CutLabel->width() + (RelX - right), CutLabel->height() + (RelY - down));
				break;
			case LEFT:
				if (RelX > right) RelX = right;
				CutLabel->resize(CutLabel->width() + (left - RelX) / 2, CutLabel->height());
				CutLabel->move(CutLabel->x() - (left - RelX) / 2, CutLabel->y());
				break;
			case RIGHT:
				if (RelX < left) RelX = left;
				CutLabel->resize(CutLabel->width() + (RelX - right), CutLabel->height());
				break;
			case UP:
				if (RelY > down) RelY = down;
				CutLabel->resize(CutLabel->width(), CutLabel->height() + (up - RelY));
				CutLabel->move(CutLabel->x(), CutLabel->y() - (up - RelY));
				break;
			case DOWN:
				if (RelY < up) RelY = up;
				CutLabel->resize(CutLabel->width(), CutLabel->height() + (RelY - down));
				break;
			}

			xRatio = (float)CutLabel->width() / (float)ImgBeforeModify.cols;
			yRatio = (float)CutLabel->height() / (float)ImgBeforeModify.rows;
			if (ui.radioButton->isChecked())
				CVimg[CrtID] = MyResize(ImgBeforeModify, xRatio, yRatio, NEAREAST);
			else if (ui.radioButton_3->isChecked())
				CVimg[CrtID] = MyResize(ImgBeforeModify, xRatio, yRatio, BILINEARITY);
			Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID], QImage::Format_ARGB32);
			ImgLabel[CrtID]->move(CutLabel->x(), CutLabel->y());

		}
		else if (cmd == ROTATE) {
			if (lineNum == CENTER) {
				//移动旋转中心
				RelX = event->x() - ui.canvas->x();// -xoffset;
				RelY = event->y() - ui.canvas->y() - ui.menuBar->height() - ui.toolBar->height();// -yoffset;
				int left = CutLabel->x();
				int right = CutLabel->x() + CutLabel->width();
				int up = CutLabel->y();
				int down = CutLabel->y() + CutLabel->height();
				if (RelX < left) RelX = CutLabel->x();
				else if (RelX > right) RelX = right;
				if (RelY < up) RelY = CutLabel->y();
				else if (RelY > down) RelY = down;

				RotateCenter->move(RelX, RelY);
			}
			else {
				RelX = event->x() - RotateCenter->x() - ui.canvas->x();// -xoffset;
				RelY = event->y() - RotateCenter->y() - ui.canvas->y() - ui.menuBar->height() - ui.toolBar->height();// -yoffset;
																													 //始终按照中心旋转，然后平移到旋转中心相应的位置
																													 //cout << RelX << " " << RelY << endl;
				Point2d* newCenter = new Point2d(RotateCenter->x() - CutLabel->x(), RotateCenter->y() - CutLabel->y());
				if (ui.radioButton->isChecked())
					CVimg[CrtID] = MyRotate(ImgBeforeModify, RotateStart->x, RotateStart->y, RelX, RelY, NEAREAST, newCenter);
				else if (ui.radioButton_3->isChecked())
					CVimg[CrtID] = MyRotate(ImgBeforeModify, RotateStart->x, RotateStart->y, RelX, RelY, BILINEARITY, newCenter);
				Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID], QImage::Format_ARGB32);
				//cout << newCenter->x << " " << newCenter->y << endl;
				//cout << ImgLabel[CrtID]->x() << endl;
				//cout << RotateCenter->x() << endl;
				//cout << ImgLabel[CrtID]->width() / 2 << endl ;
				//int newX = ImgLabel[CrtID]->x() - (CVimg[CrtID].cols / 2 - RotateCenter->x());
				//int newY = ImgLabel[CrtID]->y() - (CVimg[CrtID].rows / 2 - RotateCenter->y());
				int newX = RotateCenter->x() - newCenter->x;
				int newY = RotateCenter->y() - newCenter->y;
				//int newX = RotateCenter->x() - CutLabel->x() - newCenter->x + ImgLabel[CrtID]->x();
				//int newY = RotateCenter->y() - CutLabel->y() - newCenter->y + ImgLabel[CrtID]->y();
				//cout << ImgLabel[CrtID]->x() << endl;
				ImgLabel[CrtID]->move(newX, newY);
				//cout << ImgLabel[CrtID]->x() << endl << endl;
			}
		}
	}
}

void QtGuiApplication1::mousePressEvent(QMouseEvent* event) {
	//鼠标按下时状态为true
	pressed = true;
	//如果在移动状态
	if (cmd == MOVE) {
		hasMove = true;
		//计算按下位置相对图片位置
		RelX = event->x() - ImgLabel[CrtID]->x() - ui.canvas->x();// -xoffset;
		RelY = event->y() - ImgLabel[CrtID]->y() - ui.canvas->y() - ui.menuBar->height() - ui.toolBar->height();// -yoffset;
																												//如果在范围内则继续
		if (RelX <= Qimg[CrtID].width() && RelX >= 0 && RelY < Qimg[CrtID].height() && RelY > 0) return;
		//否则遍历所有图片，寻找鼠标所在的图片
		for (int i = 0; i < Qimg.size(); i++) {
			if (i == CrtID) continue;
			RelX = event->x() - ImgLabel[i]->x() - ui.canvas->x();// -xoffset;
			RelY = event->y() - ImgLabel[i]->y() - ui.canvas->y() - ui.menuBar->height() - ui.toolBar->height();// -yoffset;
																												//如果有则切换为当前图片，重绘直方图，并退出
			if (RelX <= Qimg[i].width() && RelX >= 0 && RelY < Qimg[i].height() && RelY > 0) {
				CrtID = i;
				ChangeLayer(CrtID);
				DrawHistogram();
				ImgLabel[CrtID]->raise();
				return;
			}
		}
		//若没有选中任何图片则不移动
		hasMove = false;
		return;
	}
	else if (cmd == CUT || cmd == RESIZE) {
		//计算按下位置相对裁剪框位置
		RelX = event->x() - CutLabel->x() - ui.canvas->x();// -xoffset;
		RelY = event->y() - CutLabel->y() - ui.canvas->y() - ui.menuBar->height() - ui.toolBar->height();// -yoffset;
																										 //cout << RelX << " " << RelY << endl;
																										 //左侧线
		if (abs(RelX) < 5 && abs(RelY) < 5) lineNum = LEFT_UP;
		else if (abs(RelX) < 5 && abs(RelY - CutLabel->height()) < 5) lineNum = LEFT_DOWN;
		else if (abs(RelX - CutLabel->width()) < 5 && abs(RelY) < 5) lineNum = RIGHT_UP;
		else if (abs(RelX - CutLabel->width()) < 5 && abs(RelY - CutLabel->height()) < 5) lineNum = RIGHT_DOWN;
		else if (abs(RelX) < 5 && RelY > 0 && RelY < CutLabel->height()) lineNum = LEFT;
		else if (abs(RelX - CutLabel->width()) < 5 && RelY > 0 && RelY < CutLabel->height()) lineNum = RIGHT;
		else if (abs(RelY) < 5 && RelX > 0 && RelX < CutLabel->width()) lineNum = UP;
		else if (abs(RelY - CutLabel->height()) < 5 && RelX > 0 && RelX < CutLabel->width()) lineNum = DOWN;
		else lineNum = NO;
		//cout << lineNum << endl;
	}
	else if (cmd == ROTATE) {
		//计算按下位置相对旋转中心位置
		RelX = event->x() - RotateCenter->x() - ui.canvas->x();// -xoffset;
		RelY = event->y() - RotateCenter->y() - ui.canvas->y() - ui.menuBar->height() - ui.toolBar->height();// -yoffset;
		if (abs(RelX) < 5 && abs(RelY) < 5)lineNum = CENTER;
		else {
			RotateStart = new Point2d(RelX, RelY);
			lineNum = NO;
		}

		/*CVimg[CrtID] = MyRotate(ImgBeforeModify, 200, 0, 200, 200, BILINEARITY, Point2d(RotateCenter->x() - ImgLabel[CrtID]->x(), RotateCenter->y() - ImgLabel[CrtID]->y()));
		Qimg[CrtID] = ShowImage(CVimg[CrtID], ImgLabel[CrtID]);
		ImgLabel[CrtID]->move(CutLabel->x(), CutLabel->y());*/
	}
}

//鼠标松开的时候状态为false
void QtGuiApplication1::mouseReleaseEvent(QMouseEvent* event) {
	pressed = false;
}