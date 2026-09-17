#pragma once

#include "include.h"
#include <QRandomGenerator>
#include "ModelARX.h"
#include "RegulatorPID.h"
#include "GenSyg.h"
#include "ProstyUAR.h"

// using namespace std;
using TypSyg = GenSyg::TypSyg;

class Centrala : public QObject {
    Q_OBJECT

private:
    GenSyg m_generatorSygnalu;
    ProstyUAR m_ProstyUAR;

    double m_ostatnie_w = 0.0;
    double m_ostatnie_y = 0.0;

    bool m_uzyjCustomWykres = false;
    double m_custom_u = 0.0;
    double m_custom_e = 0.0;
    double m_custom_uP = 0.0;
    double m_custom_uI = 0.0;
    double m_custom_uD = 0.0;

    QTimer* m_timer;
    double m_czas = 0.0;
    double m_okno = 10;

public:
    explicit Centrala();
    ~Centrala();



    void resetSymulacji();
    void wykonajKrokSymulacji();
    void zaktualizujWykres();

    // Emituje sygnał do odświeżenia wykresów bez inkrementowania czasu->dla sieciowego
    void emitujWykres();

    // Metody do trybów sieciowych
    void wykonajKrokKlienta();
    void wykonajKrokSerwera();
    void odbierzY(double noweY);
    void odbierzU(double noweU);
    void odbierzW(double noweW);
    // Ustawia m_ostatnie_w do wyświetlania na wykresie
    void setWyswietlanaW(double w) { m_ostatnie_w = w; }
    void ustawCustomDaneWykresu(double w, double y, double u, double e, double uP, double uI, double uD) {
        m_ostatnie_w = w;
        m_ostatnie_y = y;
        m_custom_u = u;
        m_custom_e = e;
        m_custom_uP = uP;
        m_custom_uI = uI;
        m_custom_uD = uD;
        m_uzyjCustomWykres = true;
    }
    void czyscCustomDaneWykresu() {
        m_uzyjCustomWykres = false;
    }

    //do wykresu dane
    double getAktualnaWartoscZadana() const; //w_i
    double getAktualnaWartoscRegulowana() const; //y_i
    double getAktualnyUchyb() const; //e_i
    double getAktualneSterowanie() const; //u_i
    double getSkladowaP(); //P
    double getSkladowaI(); //I
    double getSkladowaD(); //D

    //generator
    void setTypGenerator(TypSyg typ);
    void setOkresGenerator(double Trz);
    void setAmplitudaGenerator(double A);
    void setSkladowaStalaGenerator(double S);
    void setWypelnienieGenerator(double p);
    void setTaktowanie(int Tt);
    TypSyg getGenerator_Typ() const;
    double getGenerator_Okres() const;
    double getGenerator_Amplituda() const;
    double getGenerator_SkladowaStala() const;
    double getGenerator_Wypelnienie() const;

    int getTaktowanie() const;

    //przerwanie polaczenia sieciowego -> obliczenia w tle
    ProstyUAR& getProstyUAR(){return m_ProstyUAR;}
    GenSyg& getGenerator() { return m_generatorSygnalu; }

    /*Roboczo wykorzystałem poprzednia logikę zwiazaną z typem regulatora*/
    void SetRegulator(ProstyUAR::TypRegulatora typ);
    ProstyUAR::TypRegulatora GetRegulator(){return m_ProstyUAR.GetRegulator();}
    //PID
    double getPID_K();
    double getPID_Ti();
    double getPID_Td();
    RegulatorPID::SposobLiczeniaCalki getPID_TrybCalki() const;
    bool getPID_Ograniczenia() const;
    double getPID_U_Min() const;
    double getPID_U_Max() const;
    //regulator PID
    void setPID_K(double K);
    void setPID_Ti(double Ti);
    void setPID_Td(double Td);
    void setPID_TrybCalki(RegulatorPID::SposobLiczeniaCalki tryb);
    void resetPID_Calka();
    void resetPID_Rozniczka();
    void setPID_Nasycenie(bool wlacznik, double min, double max);
    void setPID_Ograniczenia(bool wlacz);
    void setPID_ZakresOgraniczen(double min, double max);

    //!Bezuderzeniowe przejście
    void dostosujCalkeDoU(double u_cel, double e) { m_ProstyUAR.dostosujCalkeDoU(u_cel, e); }
    void resetPID() { m_ProstyUAR.resetPID(); }

    void skopiujStan(const Centrala& inna);


    //modelarx
    void zastosujParametryARX(vector<double> A, vector<double> B, int opoznienie, double szum, bool wlaczSter, double minS, double maxS, bool wlaczReg, double minR, double maxR);
    // ModelARX* getModelARX(); //wskaznik do odczytu

    bool isSterowanieWlaczone() { return m_ProstyUAR.get_ARX()->isSterowanieWlaczone(); }
    bool isRegulacjaWlaczona() { return m_ProstyUAR.get_ARX()->isRegulacjaWlaczona(); }

    double getMinSterowania() { return m_ProstyUAR.get_ARX()->getMinSterowania(); }
    double getMaxSterowania() { return m_ProstyUAR.get_ARX()->getMaxSterowania(); }
    double getMinRegulowania() { return m_ProstyUAR.get_ARX()->getMinRegulowania(); }
    double getMaxRegulowania() { return m_ProstyUAR.get_ARX()->getMaxRegulowania(); }
    const vector<double>& getWspolczynnikiA() { return m_ProstyUAR.get_ARX()->getWspolczynnikiA(); }
    const vector<double>& getWspolczynnikiB() { return m_ProstyUAR.get_ARX()->getWspolczynnikiB(); }
    int getOpoznienieTransportowe() { return m_ProstyUAR.get_ARX()->getOpoznienieTransportowe(); }
    double getWartoscZaklocenia() { return m_ProstyUAR.get_ARX()->getWartoscZaklocenia(); }

    bool czyTimerAktywny() const { return m_timer->isActive(); }

    void on_start_clicked() {
        if (!m_timer->isActive())
            m_timer->start();
    }

    void on_stop_clicked() {
        m_timer->stop();
    }


    void on_reset_clicked() {
        m_timer->stop();
        resetSymulacji();
        m_czas = 0.0;
    }

    void   set_Okno(double o);
    double get_Okno(){return m_okno;}
    double get_Czas(){return m_czas;}
    void   set_Czas(double c){m_czas = c;}

signals:
    void ogarnijWykresy();

private slots:
    void onTimeout();
};
