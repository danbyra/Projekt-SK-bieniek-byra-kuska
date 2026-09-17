#include "ust_arx.h"
#include "ui_ust_arx.h"
#include <QRegularExpression>
#include <QStringList>


UST_ARX::UST_ARX(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UST_ARX)
{
    ui->setupUi(this);
}

UST_ARX::~UST_ARX()
{
    delete ui;
}



std::vector<double> UST_ARX::getWspolczynnikiA() const {
    std::vector<double> vec;
    // Dzielimy wpisany tekst używając przecinków lub spacji jako separatorów
    QStringList lista = ui->lineEdit_A->text().split(QRegularExpression("[,\\s]+"), Qt::SkipEmptyParts);
    for (const QString& s : lista) {
        bool ok;
        double val = s.toDouble(&ok);
        if (ok) vec.push_back(val);
    }
    return vec;
}

std::vector<double> UST_ARX::getWspolczynnikiB() const {
    std::vector<double> vec;
    QStringList lista = ui->LineEdit_B->text().split(QRegularExpression("[,\\s]+"), Qt::SkipEmptyParts);
    for (const QString& s : lista) {
        bool ok;
        double val = s.toDouble(&ok);
        if (ok) vec.push_back(val);
    }
    return vec;
}

int UST_ARX::getOpoznienie() const {
    return ui->opoznienie->value();
}

double UST_ARX::getSzum() const {
    return ui->szum->value();
}


void UST_ARX::setWspolczynnikiA(const std::vector<double>& A) {
    QStringList strList;
    for (double v : A) {
        strList << QString::number(v);
    }
    ui->lineEdit_A->setText(strList.join(", "));
}

void UST_ARX::setWspolczynnikiB(const std::vector<double>& B) {
    QStringList strList;
    for (double v : B) {
        strList << QString::number(v);
    }
    ui->LineEdit_B->setText(strList.join(", "));
}


void UST_ARX::setOpoznienie(double opoznienie) {
    ui->opoznienie->setValue(opoznienie);
}

void UST_ARX::setSzum(double szum) {
    ui->szum->setValue(szum);
}


void UST_ARX::on_zastosuj_clicked() {
    accept();
}

void UST_ARX::on_anuluj_clicked() {
    reject();
}
void UST_ARX::setOgraniczeniaSterowania(bool wlaczone, double min, double max) {
    ui->sterowana->setChecked(wlaczone);
    ui->sterMin->setValue(min);
    ui->sterMax->setValue(max);
}

void UST_ARX::setOgraniczeniaRegulacji(bool wlaczone, double min, double max) {
    ui->regulowana->setChecked(wlaczone);
    ui->regMin->setValue(min);
    ui->regMax->setValue(max);
}

void UST_ARX::zablokujEdycje() {
    ui->lineEdit_A->setReadOnly(true);
    ui->LineEdit_B->setReadOnly(true);
    ui->opoznienie->setReadOnly(true);
    ui->szum->setReadOnly(true);
    ui->sterowana->setEnabled(false);
    ui->sterMin->setReadOnly(true);
    ui->sterMax->setReadOnly(true);
    ui->regulowana->setEnabled(false);
    ui->regMin->setReadOnly(true);
    ui->regMax->setReadOnly(true);
    ui->zastosuj->setEnabled(false);
    ui->domyslne->setEnabled(false);
}

bool UST_ARX::getSterowanieWlaczone() const {
    return ui->sterowana->isChecked();
}
double UST_ARX::getMinSterowania() const {
    return ui->sterMin->value();
}
double UST_ARX::getMaxSterowania() const {
    return ui->sterMax->value();
}

bool UST_ARX::getRegulacjaWlaczona() const {
    return ui->regulowana->isChecked();
}
double UST_ARX::getMinRegulowania() const {
    return ui->regMin->value();
}
double UST_ARX::getMaxRegulowania() const {
    return ui->regMax->value();
}

void UST_ARX::on_domyslne_clicked() {
    // Wpisanie wartości jako domyślne stringi
    ui->lineEdit_A->setText("-0.4, 0, 0");
    ui->LineEdit_B->setText("0.6, 0, 0");

    ui->opoznienie->setValue(1);
    ui->szum->setValue(0);

    ui->sterowana->setCheckState(Qt::Checked);
    ui->sterMin->setValue(-10);
    ui->sterMax->setValue(10);

    ui->regulowana->setCheckState(Qt::Checked);
    ui->regMin->setValue(-10);
    ui->regMax->setValue(10);
}
