#pragma once

#include "include.h"

namespace Ui {
class UST_ARX;
}

class UST_ARX : public QDialog {
        Q_OBJECT

    private:
        Ui::UST_ARX *ui;

    public:
        explicit UST_ARX(QWidget *parent = nullptr);
        ~UST_ARX();

        void setWspolczynnikiA(const std::vector<double>& A);
        void setWspolczynnikiB(const std::vector<double>& B);
        void setOpoznienie(double opoznienie);
        void setSzum(double szum);

        void setOgraniczeniaSterowania(bool wlaczone, double min, double max);
        void setOgraniczeniaRegulacji(bool wlaczone, double min, double max);
        void zablokujEdycje();

        bool getSterowanieWlaczone() const;
        double getMinSterowania() const;
        double getMaxSterowania() const;

        bool getRegulacjaWlaczona() const;
        double getMinRegulowania() const;
        double getMaxRegulowania() const;


        std::vector<double> getWspolczynnikiA() const;
        std::vector<double> getWspolczynnikiB() const;
        int getOpoznienie() const;
        double getSzum() const;

    private slots:
        void on_zastosuj_clicked();
        void on_anuluj_clicked();
        void on_domyslne_clicked();
};
