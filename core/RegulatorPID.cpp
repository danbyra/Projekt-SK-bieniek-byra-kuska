#include "RegulatorPID.h"

RegulatorPID::RegulatorPID(double k, double Ti, double Td)
    : m_k(k), m_Ti(Ti), m_Td(Td) {}

double RegulatorPID::symuluj(double e /*uchyb*/) {
    // skladowa proporcjonalna
    m_uP = m_k * e;

    // skladowa rozniczkujaca

    m_uD = m_Td * (e - m_poprzE);
    m_poprzE = e;

    // skladowa calkujaca
    if (m_Ti == 0.0)
        m_uI = 0.0;
    else {
        // zapis starych wartosci na wypadek przesycenia (nieużywane)
        // double staraSumaE = m_sumaE;
        // double stareUI = m_uI;

        switch (m_sposobLiczeniaCalki) {
        case SposobLiczeniaCalki::StalaPrzedSuma:
            m_sumaE += e;
            m_uI = m_sumaE / m_Ti;
            break;

        case SposobLiczeniaCalki::StalaPodSuma:
            m_sumaE += e / m_Ti;
            m_uI = m_sumaE;
            break;
        }


    }

    double uWyjsciowe = m_uP + m_uI + m_uD;

    if (m_saOgraniczenia)
        uWyjsciowe = std::clamp(uWyjsciowe, m_uMin, m_uMax);

    return uWyjsciowe;
}

void RegulatorPID::resetPamieci() {
    m_uP = 0.0;
    resetCalki();
    resetRozniczki();
}

void RegulatorPID::resetCalki() {
    m_sumaE = 0.0;
    m_uI = 0.0;
}

void RegulatorPID::resetRozniczki() {
    m_poprzE = 0.0;
    m_uD = 0.0;
}

//!Bezuderzeniowe przejście: integrator tracking dopasowujący uI do u_cel i e
void RegulatorPID::dostosujCalkeDoU(double u_cel, double e) {
    m_poprzE = e;
    m_uP = m_k * e;
    m_uD = 0.0;
    m_uI = u_cel - m_uP;
    if (m_sposobLiczeniaCalki == SposobLiczeniaCalki::StalaPrzedSuma) {
        m_sumaE = (m_Ti != 0.0) ? (m_uI * m_Ti) : 0.0;
    } else {
        m_sumaE = m_uI;
    }
}

// settery
void RegulatorPID::set_k(double k) {
    m_k = k;
}

void RegulatorPID::set_Ti(double Ti) {
    m_Ti = Ti;
}

void RegulatorPID::set_Td(double Td) {
    m_Td = Td;
}

void RegulatorPID::set_sposobLiczeniaCalki(SposobLiczeniaCalki sposob) {
    if (m_sposobLiczeniaCalki == SposobLiczeniaCalki::StalaPrzedSuma &&
        sposob == SposobLiczeniaCalki::StalaPodSuma) {
        if (m_Ti != 0.0)
            m_sumaE /= m_Ti;
    }
    if (m_sposobLiczeniaCalki == SposobLiczeniaCalki::StalaPodSuma &&
        sposob == SposobLiczeniaCalki::StalaPrzedSuma) {
        m_sumaE *= m_Ti;
    }

    m_sposobLiczeniaCalki = sposob;
}

void RegulatorPID::set_saOgraniczenia(bool b) { m_saOgraniczenia = b; }
void RegulatorPID::set_uMinMax(double min, double max) {
    if (min < max) {
        m_uMin = min;
        m_uMax = max;
    }
}
