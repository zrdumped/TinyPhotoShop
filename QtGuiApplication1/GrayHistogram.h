#pragma once

#include <QWidget>
#include "ui_GrayHistogram.h"

class GrayHistogram : public QWidget
{
	Q_OBJECT

public:
	GrayHistogram(QWidget *parent = Q_NULLPTR);
	~GrayHistogram();

private:
	Ui::GrayHistogram ui;
};
