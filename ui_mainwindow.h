/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *wykresy;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_4;
    QComboBox *comboBox;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_11;
    QComboBox *taktowanie_comboBox;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_5;
    QComboBox *wybor_serializacji_comboBox;
    QHBoxLayout *horizontalLayout_23;
    QRadioButton *server_radioButton;
    QRadioButton *klient_radioButton;
    QHBoxLayout *horizontalLayout_21;
    QLabel *ip_label;
    QPlainTextEdit *ip_adres_wyswietl_TextEdit;
    QHBoxLayout *horizontalLayout_22;
    QLabel *label;
    QLCDNumber *lcdNumber;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_sync;
    QLabel *label_sync_title;
    QLabel *label_sync_status;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *start;
    QPushButton *stop;
    QPushButton *reset;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_okno;
    QDoubleSpinBox *okno;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_interwal;
    QDoubleSpinBox *interwal;
    QPushButton *parametryARX;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_7;
    QComboBox *generatorCombo;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_6;
    QDoubleSpinBox *okres;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_9;
    QDoubleSpinBox *amplituda;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_8;
    QDoubleSpinBox *skladowaStala;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_7;
    QDoubleSpinBox *wypelnienie;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_10;
    QDoubleSpinBox *wzmocnienie;
    QHBoxLayout *horizontalLayout;
    QLabel *label_12;
    QDoubleSpinBox *stalaI;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_13;
    QDoubleSpinBox *stalaD;
    QComboBox *rodzajCalkowaniaCombo;
    QHBoxLayout *horizontalLayout_18;
    QPushButton *resetCalki;
    QPushButton *resetRozniczki;
    QHBoxLayout *horizontalLayout_10;
    QCheckBox *ograniczenia;
    QHBoxLayout *horizontalLayout_19;
    QLabel *label_14;
    QDoubleSpinBox *ograniczeniaMin;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_3;
    QDoubleSpinBox *ograniczeniaMax;
    QGroupBox *groupBox_5;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *jsonZapisz;
    QPushButton *jsonWczytaj;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1267, 1098);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        wykresy = new QVBoxLayout();
        wykresy->setObjectName("wykresy");

        horizontalLayout_2->addLayout(wykresy);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(400, 0));
        groupBox->setMaximumSize(QSize(500, 16777215));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName("horizontalLayout_16");
        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");

        horizontalLayout_16->addWidget(label_4);

        comboBox = new QComboBox(groupBox);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");

        horizontalLayout_16->addWidget(comboBox);


        verticalLayout_2->addLayout(horizontalLayout_16);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName("horizontalLayout_20");
        label_11 = new QLabel(groupBox);
        label_11->setObjectName("label_11");

        horizontalLayout_20->addWidget(label_11);

        taktowanie_comboBox = new QComboBox(groupBox);
        taktowanie_comboBox->addItem(QString());
        taktowanie_comboBox->addItem(QString());
        taktowanie_comboBox->setObjectName("taktowanie_comboBox");

        horizontalLayout_20->addWidget(taktowanie_comboBox);


        verticalLayout_2->addLayout(horizontalLayout_20);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName("horizontalLayout_17");
        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");

        horizontalLayout_17->addWidget(label_5);

        wybor_serializacji_comboBox = new QComboBox(groupBox);
        wybor_serializacji_comboBox->addItem(QString());
        wybor_serializacji_comboBox->addItem(QString());
        wybor_serializacji_comboBox->setObjectName("wybor_serializacji_comboBox");

        horizontalLayout_17->addWidget(wybor_serializacji_comboBox);


        verticalLayout_2->addLayout(horizontalLayout_17);


        verticalLayout_3->addLayout(verticalLayout_2);

        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setObjectName("horizontalLayout_23");
        server_radioButton = new QRadioButton(groupBox);
        server_radioButton->setObjectName("server_radioButton");

        horizontalLayout_23->addWidget(server_radioButton);

        klient_radioButton = new QRadioButton(groupBox);
        klient_radioButton->setObjectName("klient_radioButton");

        horizontalLayout_23->addWidget(klient_radioButton);


        verticalLayout_3->addLayout(horizontalLayout_23);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setObjectName("horizontalLayout_21");
        ip_label = new QLabel(groupBox);
        ip_label->setObjectName("ip_label");
        ip_label->setMaximumSize(QSize(16777215, 20));

        horizontalLayout_21->addWidget(ip_label);

        ip_adres_wyswietl_TextEdit = new QPlainTextEdit(groupBox);
        ip_adres_wyswietl_TextEdit->setObjectName("ip_adres_wyswietl_TextEdit");
        ip_adres_wyswietl_TextEdit->setMaximumSize(QSize(16777215, 30));
        ip_adres_wyswietl_TextEdit->setReadOnly(true);

        horizontalLayout_21->addWidget(ip_adres_wyswietl_TextEdit);


        verticalLayout_3->addLayout(horizontalLayout_21);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setObjectName("horizontalLayout_22");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        horizontalLayout_22->addWidget(label);

        lcdNumber = new QLCDNumber(groupBox);
        lcdNumber->setObjectName("lcdNumber");
        lcdNumber->setMaximumSize(QSize(16777215, 20));

        horizontalLayout_22->addWidget(lcdNumber);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        horizontalLayout_22->addWidget(label_2);


        verticalLayout_3->addLayout(horizontalLayout_22);

        horizontalLayout_sync = new QHBoxLayout();
        horizontalLayout_sync->setObjectName("horizontalLayout_sync");
        label_sync_title = new QLabel(groupBox);
        label_sync_title->setObjectName("label_sync_title");

        horizontalLayout_sync->addWidget(label_sync_title);

        label_sync_status = new QLabel(groupBox);
        label_sync_status->setObjectName("label_sync_status");

        horizontalLayout_sync->addWidget(label_sync_status);


        verticalLayout_3->addLayout(horizontalLayout_sync);

        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName("groupBox_4");
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy1);
        groupBox_4->setMaximumSize(QSize(16777215, 150));
        verticalLayout_9 = new QVBoxLayout(groupBox_4);
        verticalLayout_9->setObjectName("verticalLayout_9");
        verticalLayout_9->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        start = new QPushButton(groupBox_4);
        start->setObjectName("start");
        sizePolicy1.setHeightForWidth(start->sizePolicy().hasHeightForWidth());
        start->setSizePolicy(sizePolicy1);
        start->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_4->addWidget(start);

        stop = new QPushButton(groupBox_4);
        stop->setObjectName("stop");
        sizePolicy1.setHeightForWidth(stop->sizePolicy().hasHeightForWidth());
        stop->setSizePolicy(sizePolicy1);
        stop->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_4->addWidget(stop);

        reset = new QPushButton(groupBox_4);
        reset->setObjectName("reset");
        sizePolicy1.setHeightForWidth(reset->sizePolicy().hasHeightForWidth());
        reset->setSizePolicy(sizePolicy1);
        reset->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_4->addWidget(reset);


        verticalLayout_9->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_okno = new QLabel(groupBox_4);
        label_okno->setObjectName("label_okno");
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_okno->sizePolicy().hasHeightForWidth());
        label_okno->setSizePolicy(sizePolicy2);

        horizontalLayout_6->addWidget(label_okno);

        okno = new QDoubleSpinBox(groupBox_4);
        okno->setObjectName("okno");
        sizePolicy1.setHeightForWidth(okno->sizePolicy().hasHeightForWidth());
        okno->setSizePolicy(sizePolicy1);
        okno->setMaximumSize(QSize(16777215, 16777215));
        okno->setCorrectionMode(QAbstractSpinBox::CorrectToPreviousValue);
        okno->setDecimals(0);
        okno->setMinimum(5.000000000000000);
        okno->setMaximum(50.000000000000000);
        okno->setValue(10.000000000000000);

        horizontalLayout_6->addWidget(okno);


        horizontalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label_interwal = new QLabel(groupBox_4);
        label_interwal->setObjectName("label_interwal");
        sizePolicy2.setHeightForWidth(label_interwal->sizePolicy().hasHeightForWidth());
        label_interwal->setSizePolicy(sizePolicy2);

        horizontalLayout_5->addWidget(label_interwal);

        interwal = new QDoubleSpinBox(groupBox_4);
        interwal->setObjectName("interwal");
        sizePolicy1.setHeightForWidth(interwal->sizePolicy().hasHeightForWidth());
        interwal->setSizePolicy(sizePolicy1);
        interwal->setMaximumSize(QSize(16777215, 16777215));
        interwal->setCorrectionMode(QAbstractSpinBox::CorrectToPreviousValue);
        interwal->setDecimals(0);
        interwal->setMinimum(10.000000000000000);
        interwal->setMaximum(1000.000000000000000);
        interwal->setSingleStep(10.000000000000000);
        interwal->setValue(200.000000000000000);

        horizontalLayout_5->addWidget(interwal);


        horizontalLayout_3->addLayout(horizontalLayout_5);


        verticalLayout_9->addLayout(horizontalLayout_3);


        verticalLayout_3->addWidget(groupBox_4);

        parametryARX = new QPushButton(groupBox);
        parametryARX->setObjectName("parametryARX");
        sizePolicy1.setHeightForWidth(parametryARX->sizePolicy().hasHeightForWidth());
        parametryARX->setSizePolicy(sizePolicy1);
        parametryARX->setMinimumSize(QSize(0, 0));
        parametryARX->setMaximumSize(QSize(16777215, 90));

        verticalLayout_3->addWidget(parametryARX);

        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName("groupBox_2");
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        groupBox_2->setMaximumSize(QSize(16777215, 330));
        verticalLayout_7 = new QVBoxLayout(groupBox_2);
        verticalLayout_7->setObjectName("verticalLayout_7");
        generatorCombo = new QComboBox(groupBox_2);
        generatorCombo->setObjectName("generatorCombo");
        sizePolicy1.setHeightForWidth(generatorCombo->sizePolicy().hasHeightForWidth());
        generatorCombo->setSizePolicy(sizePolicy1);
        generatorCombo->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_7->addWidget(generatorCombo);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName("label_6");

        horizontalLayout_9->addWidget(label_6);

        okres = new QDoubleSpinBox(groupBox_2);
        okres->setObjectName("okres");
        sizePolicy1.setHeightForWidth(okres->sizePolicy().hasHeightForWidth());
        okres->setSizePolicy(sizePolicy1);
        okres->setDecimals(2);
        okres->setMinimum(1.000000000000000);
        okres->setMaximum(100.000000000000000);
        okres->setSingleStep(1.000000000000000);
        okres->setValue(10.000000000000000);

        horizontalLayout_9->addWidget(okres);


        verticalLayout_7->addLayout(horizontalLayout_9);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName("horizontalLayout_13");
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName("label_9");

        horizontalLayout_13->addWidget(label_9);

        amplituda = new QDoubleSpinBox(groupBox_2);
        amplituda->setObjectName("amplituda");
        sizePolicy1.setHeightForWidth(amplituda->sizePolicy().hasHeightForWidth());
        amplituda->setSizePolicy(sizePolicy1);
        amplituda->setDecimals(2);
        amplituda->setMinimum(0.000000000000000);
        amplituda->setMaximum(100.000000000000000);
        amplituda->setSingleStep(1.000000000000000);
        amplituda->setValue(1.000000000000000);

        horizontalLayout_13->addWidget(amplituda);


        verticalLayout_7->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName("horizontalLayout_14");
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName("label_8");

        horizontalLayout_14->addWidget(label_8);

        skladowaStala = new QDoubleSpinBox(groupBox_2);
        skladowaStala->setObjectName("skladowaStala");
        sizePolicy1.setHeightForWidth(skladowaStala->sizePolicy().hasHeightForWidth());
        skladowaStala->setSizePolicy(sizePolicy1);
        skladowaStala->setDecimals(2);
        skladowaStala->setMinimum(-100.000000000000000);
        skladowaStala->setMaximum(100.000000000000000);
        skladowaStala->setSingleStep(1.000000000000000);

        horizontalLayout_14->addWidget(skladowaStala);


        verticalLayout_7->addLayout(horizontalLayout_14);

        line_2 = new QFrame(groupBox_2);
        line_2->setObjectName("line_2");
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(line_2->sizePolicy().hasHeightForWidth());
        line_2->setSizePolicy(sizePolicy3);
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_7->addWidget(line_2);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName("horizontalLayout_15");
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName("label_7");

        horizontalLayout_15->addWidget(label_7);

        wypelnienie = new QDoubleSpinBox(groupBox_2);
        wypelnienie->setObjectName("wypelnienie");
        sizePolicy1.setHeightForWidth(wypelnienie->sizePolicy().hasHeightForWidth());
        wypelnienie->setSizePolicy(sizePolicy1);
        wypelnienie->setMaximum(1.000000000000000);
        wypelnienie->setSingleStep(0.100000000000000);
        wypelnienie->setValue(0.500000000000000);

        horizontalLayout_15->addWidget(wypelnienie);


        verticalLayout_7->addLayout(horizontalLayout_15);


        verticalLayout_3->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName("groupBox_3");
        sizePolicy1.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy1);
        groupBox_3->setMaximumSize(QSize(16777215, 690));
        verticalLayout_5 = new QVBoxLayout(groupBox_3);
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName("label_10");

        horizontalLayout_11->addWidget(label_10);

        wzmocnienie = new QDoubleSpinBox(groupBox_3);
        wzmocnienie->setObjectName("wzmocnienie");
        sizePolicy1.setHeightForWidth(wzmocnienie->sizePolicy().hasHeightForWidth());
        wzmocnienie->setSizePolicy(sizePolicy1);
        wzmocnienie->setMaximum(100.000000000000000);
        wzmocnienie->setSingleStep(0.100000000000000);
        wzmocnienie->setValue(0.500000000000000);

        horizontalLayout_11->addWidget(wzmocnienie);


        verticalLayout_5->addLayout(horizontalLayout_11);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName("label_12");

        horizontalLayout->addWidget(label_12);

        stalaI = new QDoubleSpinBox(groupBox_3);
        stalaI->setObjectName("stalaI");
        sizePolicy1.setHeightForWidth(stalaI->sizePolicy().hasHeightForWidth());
        stalaI->setSizePolicy(sizePolicy1);
        stalaI->setMaximum(100.000000000000000);
        stalaI->setValue(5.000000000000000);

        horizontalLayout->addWidget(stalaI);


        verticalLayout_5->addLayout(horizontalLayout);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName("label_13");

        horizontalLayout_7->addWidget(label_13);

        stalaD = new QDoubleSpinBox(groupBox_3);
        stalaD->setObjectName("stalaD");
        sizePolicy1.setHeightForWidth(stalaD->sizePolicy().hasHeightForWidth());
        stalaD->setSizePolicy(sizePolicy1);
        stalaD->setMaximum(100.000000000000000);
        stalaD->setSingleStep(0.100000000000000);
        stalaD->setValue(0.200000000000000);

        horizontalLayout_7->addWidget(stalaD);


        verticalLayout_5->addLayout(horizontalLayout_7);

        rodzajCalkowaniaCombo = new QComboBox(groupBox_3);
        rodzajCalkowaniaCombo->setObjectName("rodzajCalkowaniaCombo");
        sizePolicy1.setHeightForWidth(rodzajCalkowaniaCombo->sizePolicy().hasHeightForWidth());
        rodzajCalkowaniaCombo->setSizePolicy(sizePolicy1);
        rodzajCalkowaniaCombo->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_5->addWidget(rodzajCalkowaniaCombo);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName("horizontalLayout_18");
        resetCalki = new QPushButton(groupBox_3);
        resetCalki->setObjectName("resetCalki");
        sizePolicy1.setHeightForWidth(resetCalki->sizePolicy().hasHeightForWidth());
        resetCalki->setSizePolicy(sizePolicy1);
        resetCalki->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_18->addWidget(resetCalki);

        resetRozniczki = new QPushButton(groupBox_3);
        resetRozniczki->setObjectName("resetRozniczki");

        horizontalLayout_18->addWidget(resetRozniczki);


        verticalLayout_5->addLayout(horizontalLayout_18);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        ograniczenia = new QCheckBox(groupBox_3);
        ograniczenia->setObjectName("ograniczenia");
        sizePolicy3.setHeightForWidth(ograniczenia->sizePolicy().hasHeightForWidth());
        ograniczenia->setSizePolicy(sizePolicy3);
        ograniczenia->setMaximumSize(QSize(16777215, 16777215));
        ograniczenia->setChecked(true);
        ograniczenia->setTristate(false);

        horizontalLayout_10->addWidget(ograniczenia);


        verticalLayout_5->addLayout(horizontalLayout_10);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName("horizontalLayout_19");
        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName("label_14");
        sizePolicy1.setHeightForWidth(label_14->sizePolicy().hasHeightForWidth());
        label_14->setSizePolicy(sizePolicy1);

        horizontalLayout_19->addWidget(label_14);

        ograniczeniaMin = new QDoubleSpinBox(groupBox_3);
        ograniczeniaMin->setObjectName("ograniczeniaMin");
        sizePolicy1.setHeightForWidth(ograniczeniaMin->sizePolicy().hasHeightForWidth());
        ograniczeniaMin->setSizePolicy(sizePolicy1);
        ograniczeniaMin->setDecimals(2);
        ograniczeniaMin->setMinimum(-100.000000000000000);
        ograniczeniaMin->setMaximum(100.000000000000000);
        ograniczeniaMin->setValue(-10.000000000000000);

        horizontalLayout_19->addWidget(ograniczeniaMin);


        verticalLayout_5->addLayout(horizontalLayout_19);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName("label_3");
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);

        horizontalLayout_8->addWidget(label_3);

        ograniczeniaMax = new QDoubleSpinBox(groupBox_3);
        ograniczeniaMax->setObjectName("ograniczeniaMax");
        sizePolicy1.setHeightForWidth(ograniczeniaMax->sizePolicy().hasHeightForWidth());
        ograniczeniaMax->setSizePolicy(sizePolicy1);
        ograniczeniaMax->setDecimals(2);
        ograniczeniaMax->setMinimum(-100.000000000000000);
        ograniczeniaMax->setMaximum(100.000000000000000);
        ograniczeniaMax->setValue(10.000000000000000);

        horizontalLayout_8->addWidget(ograniczeniaMax);


        verticalLayout_5->addLayout(horizontalLayout_8);


        verticalLayout_3->addWidget(groupBox_3);


        verticalLayout->addWidget(groupBox);

        groupBox_5 = new QGroupBox(centralwidget);
        groupBox_5->setObjectName("groupBox_5");
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(groupBox_5->sizePolicy().hasHeightForWidth());
        groupBox_5->setSizePolicy(sizePolicy4);
        groupBox_5->setMaximumSize(QSize(16777215, 90));
        horizontalLayout_12 = new QHBoxLayout(groupBox_5);
        horizontalLayout_12->setObjectName("horizontalLayout_12");
        jsonZapisz = new QPushButton(groupBox_5);
        jsonZapisz->setObjectName("jsonZapisz");
        sizePolicy1.setHeightForWidth(jsonZapisz->sizePolicy().hasHeightForWidth());
        jsonZapisz->setSizePolicy(sizePolicy1);
        jsonZapisz->setMaximumSize(QSize(16777215, 60));

        horizontalLayout_12->addWidget(jsonZapisz);

        jsonWczytaj = new QPushButton(groupBox_5);
        jsonWczytaj->setObjectName("jsonWczytaj");
        sizePolicy1.setHeightForWidth(jsonWczytaj->sizePolicy().hasHeightForWidth());
        jsonWczytaj->setSizePolicy(sizePolicy1);
        jsonWczytaj->setMaximumSize(QSize(16777215, 60));

        horizontalLayout_12->addWidget(jsonWczytaj);


        verticalLayout->addWidget(groupBox_5);


        horizontalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Panel Sterowania ", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Tryb pracy", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Stacjonarny", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Sieciowy", nullptr));

        label_11->setText(QCoreApplication::translate("MainWindow", "Taktowanie", nullptr));
        taktowanie_comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Jednostronne", nullptr));
        taktowanie_comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Obustronne", nullptr));

        label_5->setText(QCoreApplication::translate("MainWindow", "Serializacja", nullptr));
        wybor_serializacji_comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Binarna", nullptr));
        wybor_serializacji_comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Tekstowa", nullptr));

        server_radioButton->setText(QCoreApplication::translate("MainWindow", "Obiekt", nullptr));
        klient_radioButton->setText(QCoreApplication::translate("MainWindow", "Regulator", nullptr));
        ip_label->setText(QCoreApplication::translate("MainWindow", "Adres IP", nullptr));
        ip_adres_wyswietl_TextEdit->setPlainText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "Szybko\305\233\304\207 transmisji", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "pakiety/s", nullptr));
        label_sync_title->setText(QCoreApplication::translate("MainWindow", "Status op\303\263\305\272nienia:", nullptr));
        label_sync_status->setText(QCoreApplication::translate("MainWindow", "Brak danych", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "Kontrola symulacji", nullptr));
        start->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        stop->setText(QCoreApplication::translate("MainWindow", "Stop", nullptr));
        reset->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        label_okno->setText(QCoreApplication::translate("MainWindow", "Okno [s]", nullptr));
        label_interwal->setText(QCoreApplication::translate("MainWindow", "Interwa\305\202 [ms]", nullptr));
        parametryARX->setText(QCoreApplication::translate("MainWindow", "Parametry ARX", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Generatory", nullptr));
        generatorCombo->setPlaceholderText(QCoreApplication::translate("MainWindow", "Wybierz generator", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Okres", nullptr));
        okres->setSuffix(QString());
        label_9->setText(QCoreApplication::translate("MainWindow", "Amplituda", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Sk\305\202adowa sta\305\202a", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Wype\305\202nienie", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Regulator PID", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Wzmocnienie", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Sta\305\202a ca\305\202kowania", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Sta\305\202a r\303\263\305\274niczkowania", nullptr));
        rodzajCalkowaniaCombo->setPlaceholderText(QCoreApplication::translate("MainWindow", "Wybierz tryb ca\305\202kowania", nullptr));
        resetCalki->setText(QCoreApplication::translate("MainWindow", "Reset cz\304\231\305\233ci ca\305\202kuj\304\205cej", nullptr));
        resetRozniczki->setText(QCoreApplication::translate("MainWindow", "Reset cz\304\231\305\233ci r\303\263\305\274niczkuj\304\205cej", nullptr));
        ograniczenia->setText(QCoreApplication::translate("MainWindow", "Ograniczenia", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Min:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Max:", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "JSON", nullptr));
        jsonZapisz->setText(QCoreApplication::translate("MainWindow", "Zapisz", nullptr));
        jsonWczytaj->setText(QCoreApplication::translate("MainWindow", "Wczytaj", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
