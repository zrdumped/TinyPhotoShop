#pragma once

#include <QDialog>
#include <vector>

#include <qmessagebox.h>
#include "ui_AddImageDialog.h"
using namespace std;

class AddImageDialog : public QDialog
{
	Q_OBJECT

public:
	AddImageDialog(QWidget *parent = Q_NULLPTR);
	~AddImageDialog();
	void PrePareDialogAdd(vector<QPushButton*> Layers, int CrtID);
	void PrePareDialogMinus(vector<QPushButton*> Layers, int CrtID);
	void PrePareDialogMult(vector<QPushButton*> Layers, int CrtID);
	int TargetImage();
	int AddImagePort();
	bool BinaryMult();

private slots:
	void ChangeAddMode(bool mode);

private:
	Ui::AddImageDialog ui;
	int size;
	vector<QPushButton*> layers;
};
