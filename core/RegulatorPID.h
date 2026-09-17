#pragma once

#include "include.h"

#include "IRegulator.h"

class RegulatorPID : public IRegulator {
public:
    enum class SposobLiczeniaCalki {
        StalaPrzedSuma,
        StalaPodSuma
    };

private:
    double m_uP = 0.0; // skladowa proporcjonalna
    double m_uI = 0.0; // skladowa calkujaca
    double m_uD = 0.0; // skladowa rozniczkujaca

    double m_k = 0.5;  // wzmocnienie (P)
    double m_Ti = 0.0; // stala calkowania (I)
    double m_Td = 0.0; // stala rozniczkowania (D)

    SposobLiczeniaCalki m_sposobLiczeniaCalki = SposobLiczeniaCalki::StalaPrzedSuma;
    double m_sumaE = 0.0;  // suma uchybow
    double m_poprzE = 0.0; // poprzedni uchyb

    bool m_saOgraniczenia = false;
    double m_uMin = -10.0;
    double m_uMax = 10.0;

public:
    RegulatorPID(double k = 0.5, double Ti = 0.0, double Td = 0.0);

    double symuluj(double uchyb) override;

    void resetPamieci() override;
    void resetCalki();
    void resetRozniczki();

    //!-Bezuderzeniowe przejście
    void dostosujCalkeDoU(double u_cel, double e);

    //settery
    void set_k(double k);
    void set_Ti(double Ti);
    void set_Td(double Td);

    void set_sposobLiczeniaCalki(SposobLiczeniaCalki sposob);

    void set_saOgraniczenia(bool wlacz);
    void set_uMinMax(double min, double max);

    //gettery
    double get_uP() const { return m_uP; };
    double get_uI() const { return m_uI; };
    double get_uD() const { return m_uD; };

    double get_k() const { return m_k; };
    double get_Ti() const { return m_Ti; };
    double get_Td() const { return m_Td; };

    SposobLiczeniaCalki get_sposobLiczeniaCalki() const { return m_sposobLiczeniaCalki; };

    bool   get_saOgraniczenia() const { return m_saOgraniczenia; };
    double get_uMin() const { return m_uMin; };
    double get_uMax() const { return m_uMax; };
};
