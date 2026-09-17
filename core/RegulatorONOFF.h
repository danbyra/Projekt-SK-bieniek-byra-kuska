#pragma once

#include "include.h"

#include "IRegulator.h"

class RegulatorONOFF : public IRegulator {
    private:
        double m_ON = 1.0;        // wartosc wyjscia gdy przekroczona histereza
        double m_histereza = 0.1; // wartosc histerezy
        double m_wyjscie = 0.0;   // aktualne wyjscie

    public:
        RegulatorONOFF(double ON = 1.0, double histereza = 0.1);

        double symuluj(double uchyb) override;
        void resetPamieci() override;

        //settery
        void set_ON(double ON);
        void set_histereza(double histereza);

        //gettery
        double get_ON() const { return m_ON; }
        double get_histereza() const { return m_histereza; }
};
