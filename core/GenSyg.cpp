#include "GenSyg.h"

GenSyg::GenSyg(TypSyg typ, double Trz, int Tt, double p, double A, double S)
    : m_typ(typ)
    , m_Trz(Trz)
    , m_Tt(Tt)
    , m_p(p)
    , m_A(A)
    , m_S(S)
{
        set_T();
}

double GenSyg::generuj() {
        double wartosc = 0.0;

        switch (m_typ) {
        case TypSyg::Sinusoida:
                wartosc = m_A * sin(fmod(m_licz, m_T) / (double)(m_T) * 2.0 * M_PI) + m_S;
                break;
        case TypSyg::Prostokat:
                if (fmod(m_licz, m_T) < m_p * m_T)
                        wartosc = m_A + m_S;
                else
                        wartosc = m_S;
                break;
        }

        m_licz++;

        return wartosc;
}

// settery
void GenSyg::set_typ(TypSyg typ) {
        m_typ = typ;
}

void GenSyg::set_Trz(double Trz) {
        m_Trz = Trz;
        set_T();
}

void GenSyg::set_Tt(int Tt) {
        m_Tt = Tt;
        set_T();
}

void GenSyg::set_T() {
        m_T = (int)(m_Trz / (double)m_Tt * 1000.0);
        if (m_T < 1)
                m_T = 1;
}

void GenSyg::set_p(double p) {
        m_p = p;
}

void GenSyg::set_A(double A) {
        m_A = A;
}

void GenSyg::set_S(double S) {
        m_S = S;
}
