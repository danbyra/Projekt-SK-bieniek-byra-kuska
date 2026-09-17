/********************************************************************************
** Form generated from reading UI file 'ust_arx.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UST_ARX_H
#define UI_UST_ARX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UST_ARX
{
public:
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_8;
    QLineEdit *lineEdit_A;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_9;
    QLineEdit *LineEdit_B;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QDoubleSpinBox *opoznienie;
    QLabel *label_5;
    QDoubleSpinBox *szum;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QCheckBox *sterowana;
    QLabel *label_2;
    QDoubleSpinBox *sterMax;
    QDoubleSpinBox *sterMin;
    QLabel *label;
    QCheckBox *regulowana;
    QDoubleSpinBox *regMin;
    QDoubleSpinBox *regMax;
    QHBoxLayout *horizontalLayout;
    QPushButton *zastosuj;
    QPushButton *anuluj;
    QPushButton *domyslne;

    void setupUi(QDialog *UST_ARX)
    {
        if (UST_ARX->objectName().isEmpty())
            UST_ARX->setObjectName("UST_ARX");
        UST_ARX->setWindowModality(Qt::WindowModal);
        UST_ARX->resize(473, 403);
        UST_ARX->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_4 = new QVBoxLayout(UST_ARX);
        verticalLayout_4->setObjectName("verticalLayout_4");
        groupBox = new QGroupBox(UST_ARX);
        groupBox->setObjectName("groupBox");
        horizontalLayout_8 = new QHBoxLayout(groupBox);
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalLayout_8->setContentsMargins(6, 0, 6, 6);
        lineEdit_A = new QLineEdit(groupBox);
        lineEdit_A->setObjectName("lineEdit_A");

        horizontalLayout_8->addWidget(lineEdit_A);


        verticalLayout_4->addWidget(groupBox);

        groupBox_3 = new QGroupBox(UST_ARX);
        groupBox_3->setObjectName("groupBox_3");
        horizontalLayout_9 = new QHBoxLayout(groupBox_3);
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        horizontalLayout_9->setContentsMargins(6, 0, 6, 6);
        LineEdit_B = new QLineEdit(groupBox_3);
        LineEdit_B->setObjectName("LineEdit_B");

        horizontalLayout_9->addWidget(LineEdit_B);


        verticalLayout_4->addWidget(groupBox_3);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label_3 = new QLabel(UST_ARX);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        opoznienie = new QDoubleSpinBox(UST_ARX);
        opoznienie->setObjectName("opoznienie");
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(opoznienie->sizePolicy().hasHeightForWidth());
        opoznienie->setSizePolicy(sizePolicy);
        opoznienie->setDecimals(0);
        opoznienie->setMinimum(1.000000000000000);

        gridLayout->addWidget(opoznienie, 0, 1, 1, 1);

        label_5 = new QLabel(UST_ARX);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 0, 0, 1, 1);

        szum = new QDoubleSpinBox(UST_ARX);
        szum->setObjectName("szum");
        sizePolicy.setHeightForWidth(szum->sizePolicy().hasHeightForWidth());
        szum->setSizePolicy(sizePolicy);
        szum->setMinimum(0.000000000000000);
        szum->setMaximum(10.000000000000000);
        szum->setSingleStep(0.010000000000000);

        gridLayout->addWidget(szum, 1, 1, 1, 1);


        verticalLayout_4->addLayout(gridLayout);

        groupBox_2 = new QGroupBox(UST_ARX);
        groupBox_2->setObjectName("groupBox_2");
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(6, 0, 6, 6);
        sterowana = new QCheckBox(groupBox_2);
        sterowana->setObjectName("sterowana");
        sizePolicy.setHeightForWidth(sterowana->sizePolicy().hasHeightForWidth());
        sterowana->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(sterowana, 1, 0, 1, 1);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName("label_2");

        gridLayout_2->addWidget(label_2, 0, 2, 1, 1);

        sterMax = new QDoubleSpinBox(groupBox_2);
        sterMax->setObjectName("sterMax");
        sizePolicy.setHeightForWidth(sterMax->sizePolicy().hasHeightForWidth());
        sterMax->setSizePolicy(sizePolicy);
        sterMax->setMinimum(-100.000000000000000);
        sterMax->setMaximum(100.000000000000000);
        sterMax->setValue(10.000000000000000);

        gridLayout_2->addWidget(sterMax, 1, 2, 1, 1);

        sterMin = new QDoubleSpinBox(groupBox_2);
        sterMin->setObjectName("sterMin");
        sizePolicy.setHeightForWidth(sterMin->sizePolicy().hasHeightForWidth());
        sterMin->setSizePolicy(sizePolicy);
        sterMin->setMinimum(-100.000000000000000);
        sterMin->setMaximum(100.000000000000000);
        sterMin->setValue(-10.000000000000000);

        gridLayout_2->addWidget(sterMin, 1, 1, 1, 1);

        label = new QLabel(groupBox_2);
        label->setObjectName("label");

        gridLayout_2->addWidget(label, 0, 1, 1, 1);

        regulowana = new QCheckBox(groupBox_2);
        regulowana->setObjectName("regulowana");
        sizePolicy.setHeightForWidth(regulowana->sizePolicy().hasHeightForWidth());
        regulowana->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(regulowana, 2, 0, 1, 1);

        regMin = new QDoubleSpinBox(groupBox_2);
        regMin->setObjectName("regMin");
        sizePolicy.setHeightForWidth(regMin->sizePolicy().hasHeightForWidth());
        regMin->setSizePolicy(sizePolicy);
        regMin->setMinimum(-100.000000000000000);
        regMin->setMaximum(100.000000000000000);
        regMin->setValue(-10.000000000000000);

        gridLayout_2->addWidget(regMin, 2, 1, 1, 1);

        regMax = new QDoubleSpinBox(groupBox_2);
        regMax->setObjectName("regMax");
        sizePolicy.setHeightForWidth(regMax->sizePolicy().hasHeightForWidth());
        regMax->setSizePolicy(sizePolicy);
        regMax->setMinimum(-100.000000000000000);
        regMax->setMaximum(100.000000000000000);
        regMax->setValue(10.000000000000000);

        gridLayout_2->addWidget(regMax, 2, 2, 1, 1);


        verticalLayout_4->addWidget(groupBox_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        zastosuj = new QPushButton(UST_ARX);
        zastosuj->setObjectName("zastosuj");
        sizePolicy.setHeightForWidth(zastosuj->sizePolicy().hasHeightForWidth());
        zastosuj->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(zastosuj);

        anuluj = new QPushButton(UST_ARX);
        anuluj->setObjectName("anuluj");
        sizePolicy.setHeightForWidth(anuluj->sizePolicy().hasHeightForWidth());
        anuluj->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(anuluj);

        domyslne = new QPushButton(UST_ARX);
        domyslne->setObjectName("domyslne");
        sizePolicy.setHeightForWidth(domyslne->sizePolicy().hasHeightForWidth());
        domyslne->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(domyslne);


        verticalLayout_4->addLayout(horizontalLayout);


        retranslateUi(UST_ARX);

        QMetaObject::connectSlotsByName(UST_ARX);
    } // setupUi

    void retranslateUi(QDialog *UST_ARX)
    {
        UST_ARX->setWindowTitle(QCoreApplication::translate("UST_ARX", "Ustawienia ARX", nullptr));
        groupBox->setTitle(QCoreApplication::translate("UST_ARX", "Wielomian A", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("UST_ARX", "Wielomian B", nullptr));
        label_3->setText(QCoreApplication::translate("UST_ARX", "Szum", nullptr));
        label_5->setText(QCoreApplication::translate("UST_ARX", "Op\303\263\305\272nienie", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("UST_ARX", "Ograniczenia", nullptr));
        sterowana->setText(QCoreApplication::translate("UST_ARX", "Wart. sterowana", nullptr));
        label_2->setText(QCoreApplication::translate("UST_ARX", "Max", nullptr));
        label->setText(QCoreApplication::translate("UST_ARX", "Min", nullptr));
        regulowana->setText(QCoreApplication::translate("UST_ARX", "Wart. regulowana", nullptr));
        zastosuj->setText(QCoreApplication::translate("UST_ARX", "Zastosuj", nullptr));
        anuluj->setText(QCoreApplication::translate("UST_ARX", "Anuluj", nullptr));
        domyslne->setText(QCoreApplication::translate("UST_ARX", "Przywr\303\263\304\207 domy\305\233lne", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UST_ARX: public Ui_UST_ARX {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UST_ARX_H
