#include "AddImageDialog.h"

AddImageDialog::AddImageDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

}

AddImageDialog::~AddImageDialog()
{
}

void AddImageDialog::PrePareDialogAdd(vector<QPushButton*> Layers, int CrtID) {
	ui.checkBox->setEnabled(true);
	ui.checkBox_2->setEnabled(false);
	layers = Layers;
	size = Layers.size();
	if (size < 2)
	{
		QMessageBox::critical(NULL, "Wrong", "No Enough Images To Add", QMessageBox::Ok, QMessageBox::Ok);
		if (QMessageBox::Ok) {
			reject();
		}
	}
	for (int i = 0; i <size; i++) {
		if (i == CrtID) continue;
		else
			ui.comboBox->addItem(Layers[i]->text());
	}
	ui.SrcFileName->setText(Layers[CrtID]->text());
}

void AddImageDialog::PrePareDialogMinus(vector<QPushButton*> Layers, int CrtID) {
	ui.checkBox->setEnabled(false);
	ui.checkBox_2->setEnabled(false);
	layers = Layers;
	size = Layers.size();
	if (size < 2)
	{
		QMessageBox::critical(NULL, "Wrong", "No Enough Images To Minus", QMessageBox::Ok, QMessageBox::Ok);
		if (QMessageBox::Ok) {
			reject();
		}
	}
	for (int i = 0; i <size; i++) {
		if (i == CrtID) continue;
		else
			ui.comboBox->addItem(Layers[i]->text());
	}
	ui.SrcFileName->setText(Layers[CrtID]->text());
}

void AddImageDialog::PrePareDialogMult(vector<QPushButton*> Layers, int CrtID) {
	ui.checkBox->setEnabled(false);
	ui.checkBox_2->setEnabled(true);
	layers = Layers;
	size = Layers.size();
	if (size < 2)
	{
		QMessageBox::critical(NULL, "Wrong", "No Enough Images To Mult", QMessageBox::Ok, QMessageBox::Ok);
		if (QMessageBox::Ok) {
			reject();
		}
	}
	for (int i = 0; i <size; i++) {
		if (i == CrtID) continue;
		else
			ui.comboBox->addItem(Layers[i]->text());
	}
	ui.SrcFileName->setText(Layers[CrtID]->text());
}

int AddImageDialog::AddImagePort() {
	if (ui.horizontalSlider->isEnabled())
		return ui.horizontalSlider->value();
	else
		return -1;
}

int AddImageDialog::TargetImage() {
	QString result = ui.comboBox->currentText();
	for (int i = 0; i < size; i++) {
		if (layers[i]->text() == result) return i;
	}
}

void AddImageDialog::ChangeAddMode(bool mode) {
	ui.horizontalSlider->setEnabled(mode);
	ui.spinBox->setEnabled(mode);
}

bool AddImageDialog::BinaryMult() {
	return ui.checkBox_2->isChecked();
}
