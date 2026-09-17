#include "RegulatorONOFF.h"

RegulatorONOFF::RegulatorONOFF(double ON, double histereza)
    : m_ON(ON)
    , m_histereza(histereza)
{
}

double RegulatorONOFF::symuluj(double uchyb) {
        if (uchyb > m_histereza)
                m_wyjscie = m_ON;
        if (uchyb < -m_histereza)
                m_wyjscie = 0.0;

        return m_wyjscie;
}

void RegulatorONOFF::resetPamieci() {
        m_wyjscie = 0.0;
}

//settery
void RegulatorONOFF::set_ON(double ON) {
        m_ON = ON;


        if(m_wyjscie != 0.0)
                m_wyjscie = m_ON;
}

void RegulatorONOFF::set_histereza(double histereza) {
        m_histereza = histereza;
}
