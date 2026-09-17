#pragma once

#include "include.h"

class GenSyg {
    public:
        enum class TypSyg {
                Sinusoida,
                Prostokat,
        };

    private:
        TypSyg m_typ = TypSyg::Sinusoida;

        double m_Trz = 10.0; //okres sygnalu rzeczywisty w sekundach
        int    m_Tt  = 200;  //taktowanie w milisekundach, czas pomiedy krokami symulacji
        int    m_T   = 0;    //okres wyrazony w probkach na okres sygnalu, (Trz / Tt)

        double m_p = 0.5; //wypelnienie, (0 -- 1)
        double m_A = 1.0; //amplituda sygnalu
        double m_S = 0.0; //stala skladowa sygnalu

        int m_licz = 1; //licznik probek

        void set_T();

    public:
        GenSyg(TypSyg typ = TypSyg::Sinusoida, double Trz = 10.0, int Tt = 200, double p = 0.5, double A = 1.0, double S = 0.0);

        double generuj();

        void set_typ(TypSyg typ);
        void set_Tt(int Tt);
        void set_Trz(double Trz);
        void set_p(double p);
        void set_A(double A);
        void set_S(double S);

        TypSyg get_typ() const { return m_typ; }
        double get_Trz() const { return m_Trz; }
        int    get_Tt() const { return m_Tt; }
        double get_p() const { return m_p; }
        double get_A() const { return m_A; }
        double get_S() const { return m_S; }

        /*reset pamięci generatora DODANE*/
        void ResetPamieci(){m_licz=1;}

        // Zmiana Tt z zachowaniem fazy sygnału - przelicza m_licz proporcjonalnie
        // aby faza (m_licz % m_T) pozostała taka sama po zmianie okresu w próbkach.
        void set_Tt_zachowujFaze(int nowyTt) {
            if (m_T > 0) {
                double faza = static_cast<double>(m_licz % m_T) / m_T; // faza [0,1)
                m_Tt = nowyTt;
                set_T(); // przelicza m_T dla nowego Tt
                m_licz = static_cast<int>(faza * m_T); // przywraca fazę w nowym m_T
                if (m_licz < 1) m_licz = 1;
            } else {
                m_Tt = nowyTt;
                set_T();
            }
        }

        void setLicznikProbki(int n) { m_licz = n; }
        int getLicznikProbki() const { return m_licz; }
};

