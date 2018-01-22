/********************************************************************************
** Form generated from reading UI file 'AddImageDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDIMAGEDIALOG_H
#define UI_ADDIMAGEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_AddImageDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *HintInfo;
    QSpacerItem *verticalSpacer_3;
    QLabel *label;
    QLabel *SrcFileName;
    QComboBox *comboBox;
    QSlider *horizontalSlider;
    QSpinBox *spinBox;
    QPushButton *pushButton;
    QLabel *label_2;
    QSpacerItem *verticalSpacer;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *AddImageDialog)
    {
        if (AddImageDialog->objectName().isEmpty())
            AddImageDialog->setObjectName(QStringLiteral("AddImageDialog"));
        AddImageDialog->resize(400, 300);
        gridLayout = new QGridLayout(AddImageDialog);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        HintInfo = new QLabel(AddImageDialog);
        HintInfo->setObjectName(QStringLiteral("HintInfo"));

        gridLayout->addWidget(HintInfo, 1, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 85, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 0, 0, 1, 1);

        label = new QLabel(AddImageDialog);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 2, 0, 1, 1);

        SrcFileName = new QLabel(AddImageDialog);
        SrcFileName->setObjectName(QStringLiteral("SrcFileName"));
        SrcFileName->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(SrcFileName, 1, 1, 1, 3);

        comboBox = new QComboBox(AddImageDialog);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        gridLayout->addWidget(comboBox, 2, 1, 1, 3);

        horizontalSlider = new QSlider(AddImageDialog);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setEnabled(false);
        horizontalSlider->setMaximum(100);
        horizontalSlider->setSingleStep(1);
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 3, 1, 1, 2);

        spinBox = new QSpinBox(AddImageDialog);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setEnabled(false);
        spinBox->setMaximum(100);

        gridLayout->addWidget(spinBox, 3, 3, 1, 1);

        pushButton = new QPushButton(AddImageDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 6, 0, 1, 1);

        label_2 = new QLabel(AddImageDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 84, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 5, 0, 1, 1);

        checkBox = new QCheckBox(AddImageDialog);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout->addWidget(checkBox, 4, 3, 1, 1);

        checkBox_2 = new QCheckBox(AddImageDialog);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        gridLayout->addWidget(checkBox_2, 4, 2, 1, 1);

        pushButton_2 = new QPushButton(AddImageDialog);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(90, 0));

        gridLayout->addWidget(pushButton_2, 6, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(175, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 6, 1, 1, 2);


        retranslateUi(AddImageDialog);
        QObject::connect(horizontalSlider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
        QObject::connect(spinBox, SIGNAL(valueChanged(int)), horizontalSlider, SLOT(setValue(int)));
        QObject::connect(pushButton_2, SIGNAL(clicked()), AddImageDialog, SLOT(reject()));
        QObject::connect(pushButton, SIGNAL(clicked()), AddImageDialog, SLOT(accept()));
        QObject::connect(checkBox, SIGNAL(clicked(bool)), AddImageDialog, SLOT(ChangeAddMode(bool)));

        QMetaObject::connectSlotsByName(AddImageDialog);
    } // setupUi

    void retranslateUi(QDialog *AddImageDialog)
    {
        AddImageDialog->setWindowTitle(QApplication::translate("AddImageDialog", "AddImageDialog", Q_NULLPTR));
        HintInfo->setText(QApplication::translate("AddImageDialog", "\345\216\237\345\247\213\345\233\276\345\203\217", Q_NULLPTR));
        label->setText(QApplication::translate("AddImageDialog", "\350\276\205\345\212\251\345\233\276\345\203\217", Q_NULLPTR));
        SrcFileName->setText(QApplication::translate("AddImageDialog", "TextLabel", Q_NULLPTR));
        pushButton->setText(QApplication::translate("AddImageDialog", "\347\241\256\345\256\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("AddImageDialog", "\350\276\205\345\212\251\345\233\276\345\203\217\346\257\224\344\276\213\357\274\210%\357\274\211", Q_NULLPTR));
        checkBox->setText(QApplication::translate("AddImageDialog", "\346\214\211\346\257\224\344\276\213\347\233\270\345\212\240", Q_NULLPTR));
        checkBox_2->setText(QApplication::translate("AddImageDialog", "\344\272\214\345\200\274\345\233\276\347\233\270\344\271\230", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("AddImageDialog", "\345\217\226\346\266\210", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AddImageDialog: public Ui_AddImageDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDIMAGEDIALOG_H
