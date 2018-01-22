/********************************************************************************
** Form generated from reading UI file 'GrayHistogram.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAYHISTOGRAM_H
#define UI_GRAYHISTOGRAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GrayHistogram
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QLabel *HintInfo;
    QSpinBox *spinBox;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QLabel *label;
    QSpacerItem *verticalSpacer;
    QSlider *horizontalSlider;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer_3;
    QLabel *SrcFileName;

    void setupUi(QWidget *GrayHistogram)
    {
        if (GrayHistogram->objectName().isEmpty())
            GrayHistogram->setObjectName(QStringLiteral("GrayHistogram"));
        GrayHistogram->resize(400, 300);
        gridLayout = new QGridLayout(GrayHistogram);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton = new QPushButton(GrayHistogram);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 6, 1, 1, 1);

        HintInfo = new QLabel(GrayHistogram);
        HintInfo->setObjectName(QStringLiteral("HintInfo"));

        gridLayout->addWidget(HintInfo, 1, 1, 1, 1);

        spinBox = new QSpinBox(GrayHistogram);
        spinBox->setObjectName(QStringLiteral("spinBox"));

        gridLayout->addWidget(spinBox, 4, 4, 1, 1);

        comboBox = new QComboBox(GrayHistogram);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        gridLayout->addWidget(comboBox, 3, 2, 1, 3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 6, 2, 1, 1);

        label_2 = new QLabel(GrayHistogram);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 4, 1, 1, 1);

        label = new QLabel(GrayHistogram);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 3, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 5, 1, 1, 4);

        horizontalSlider = new QSlider(GrayHistogram);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setMaximum(100);
        horizontalSlider->setSingleStep(1);
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 4, 2, 1, 2);

        pushButton_2 = new QPushButton(GrayHistogram);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(90, 0));

        gridLayout->addWidget(pushButton_2, 6, 3, 1, 2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 0, 1, 1, 4);

        SrcFileName = new QLabel(GrayHistogram);
        SrcFileName->setObjectName(QStringLiteral("SrcFileName"));
        SrcFileName->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(SrcFileName, 1, 2, 1, 3);


        retranslateUi(GrayHistogram);
        QObject::connect(horizontalSlider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
        QObject::connect(spinBox, SIGNAL(valueChanged(int)), horizontalSlider, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(GrayHistogram);
    } // setupUi

    void retranslateUi(QWidget *GrayHistogram)
    {
        GrayHistogram->setWindowTitle(QApplication::translate("GrayHistogram", "GrayHistogram", Q_NULLPTR));
        pushButton->setText(QApplication::translate("GrayHistogram", "\347\241\256\345\256\232", Q_NULLPTR));
        HintInfo->setText(QApplication::translate("GrayHistogram", "\345\216\237\345\247\213\345\233\276\345\203\217", Q_NULLPTR));
        label_2->setText(QApplication::translate("GrayHistogram", "\350\276\205\345\212\251\345\233\276\345\203\217\346\257\224\344\276\213\357\274\210%\357\274\211", Q_NULLPTR));
        label->setText(QApplication::translate("GrayHistogram", "\350\276\205\345\212\251\345\233\276\345\203\217", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("GrayHistogram", "\345\217\226\346\266\210", Q_NULLPTR));
        SrcFileName->setText(QApplication::translate("GrayHistogram", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GrayHistogram: public Ui_GrayHistogram {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAYHISTOGRAM_H
