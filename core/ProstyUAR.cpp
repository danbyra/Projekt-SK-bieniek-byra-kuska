#include "ProstyUAR.h"

ProstyUAR::ProstyUAR(ModelARX arx, RegulatorPID regulatorPID)
    : m_arx(std::move(arx))
    , m_regulatorPID(std::move(regulatorPID))
    , m_typRegulatora(TypRegulatora::PID)
{

}

double ProstyUAR::symuluj(double sygnalWejsciowy)
{
    w = sygnalWejsciowy;
    e = w - y;

    if (m_typRegulatora == TypRegulatora::PID)
        u = m_regulatorPID.symuluj(e); //wartosc sterowania z regulatora

    y = m_arx.symuluj(u);

    return y;
}

double ProstyUAR::symulujTylkoRegulator(double sygnalWejsciowy)
{
    w = sygnalWejsciowy;
    e = w - y; //  stary y

    if (m_typRegulatora == TypRegulatora::PID)
        u = m_regulatorPID.symuluj(e);

    return u;
}

double ProstyUAR::symulujTylkoARX()
{
    e = w - y; // uchyb dla servera
    // u zaktualizowane z sieci
    y = m_arx.symuluj(u);
    return y;
}

void ProstyUAR::wymusY(double noweY) { y = noweY; }
void ProstyUAR::wymusU(double noweU) { u = noweU; }
void ProstyUAR::wymusW(double noweW) { w = noweW; }

 //Aktualizacja wartości ARX && PID w tle -> przypadku zerwania połaczenia sieciowego
void ProstyUAR::aktualizujTloARX(double noweU,double noweY)
{
    m_arx.aktualizujHistoriaBuforow(noweU,noweY);
    u=noweU;
    y=noweY;
}

void ProstyUAR::aktualizujTloPID(double noweW,double noweY)
{
    double stareY = y;
    w=noweW;
    y=noweY;
    e=w-y;
    if(m_typRegulatora==TypRegulatora::PID)
        m_regulatorPID.symuluj(e);
    y=stareY;
}

//!Bezuderzeniowe dostosowanie członu całkującego PID do zadanego u_cel
void ProstyUAR::dostosujCalkeDoU(double u_cel, double e)
{
    if (m_typRegulatora == TypRegulatora::PID) {
        m_regulatorPID.dostosujCalkeDoU(u_cel, e);
    }
    this->u = u_cel;
    this->e = e;
}

//!Reset stanów PID bez zerowania buforów modelu ARX
void ProstyUAR::resetPID()
{
    m_regulatorPID.resetPamieci();
    e = 0.0;
    u = 0.0;
}

/*RESET pamieci ARX && PID */
void ProstyUAR::ResetStanow()
{
    m_arx.ResetPamieci();
    m_regulatorPID.resetPamieci();
    w = 0.0; //wartosc zadana
    y = 0.0; //wartosc zmierzona z arx
    e = 0.0; //uchyb, e_i = w_i - y_(i-1)
    u = 0.0; //wartosc sterowania, u_i = regulator(e_i)
}

double ProstyUAR::getUchyb() const
{
    return e;
}
double ProstyUAR::getSterowania() const
{
    return u;
}

void ProstyUAR::setRegulator(TypRegulatora typ)
{
    if (m_typRegulatora == typ)
        return;

    m_typRegulatora = typ;
}
ProstyUAR::TypRegulatora ProstyUAR::GetRegulator() const
{
    return m_typRegulatora;
}
void ProstyUAR::setPID_K(double k)
{
    m_regulatorPID.set_k(k);
}
void ProstyUAR::setPID_Ti(double Ti)
{
    m_regulatorPID.set_Ti(Ti);
}
void ProstyUAR::setPID_Td(double Td)
{
    m_regulatorPID.set_Td(Td);
}
void ProstyUAR::setPID_TrybCalki(RegulatorPID::SposobLiczeniaCalki sposob)
{
    m_regulatorPID.set_sposobLiczeniaCalki(sposob);
}
void ProstyUAR::resetPID_Calka()
{
    m_regulatorPID.resetCalki();
}
void ProstyUAR::resetPID_Rozniczka()
{
    m_regulatorPID.resetRozniczki();
}
void ProstyUAR::setPID_Nasycenie(bool wlacznik, double min, double max)
{
    m_regulatorPID.set_saOgraniczenia(wlacznik);
    m_regulatorPID.set_uMinMax(min, max);
}

void ProstyUAR::setPID_Ograniczenia(bool wlacz)
{
    m_regulatorPID.set_saOgraniczenia(wlacz);
}
void ProstyUAR::setPID_ZakresOgraniczen(double min, double max)
{
    m_regulatorPID.set_uMinMax(min, max);
}

void ProstyUAR::zastosujParametryARX(vector<double> A, vector<double> B, double opoznienie, double szum, bool wlaczSter, double minS, double maxS, bool wlaczReg, double minR, double maxR)
{
    m_arx.set_opoznienieTransportowe(opoznienie);
    m_arx.setWspolczynnikA(A);
    m_arx.setWspolczynnikB(B);
    m_arx.setWartoscZaklocenia(szum);
    m_arx.wlaczSterowanie(wlaczSter);
    m_arx.setWartoscSterowania(minS, maxS);
    m_arx.wlaczRegulacje(wlaczReg);
    m_arx.setWartoscRegulowania(minR, maxR);
}
double ProstyUAR::getPID_K() const
{
    return m_regulatorPID.get_k();
}
double ProstyUAR::getPID_Ti() const
{
    return m_regulatorPID.get_Ti();
}
double ProstyUAR::getPID_Td() const
{
    return m_regulatorPID.get_Td();
}
RegulatorPID::SposobLiczeniaCalki ProstyUAR::getPID_TrybCalki() const
{
    return m_regulatorPID.get_sposobLiczeniaCalki();
}
bool ProstyUAR::getPID_Ograniczenia() const
{
    return m_regulatorPID.get_saOgraniczenia();
}
double ProstyUAR::getPID_U_Min() const
{
    return m_regulatorPID.get_uMin();
}
double ProstyUAR::getPID_U_Max() const
{
    return m_regulatorPID.get_uMax();
}

double ProstyUAR::getSkladowaP()
{
    // if (getTypRegulatora() == TypRegulatora::PID)
    // {
    return m_regulatorPID.get_uP();
    // }
    // return 0.0;
}
double ProstyUAR::getSkladowaI()
{
    // if (getTypRegulatora() == TypRegulatora::PID)
    // {
    return m_regulatorPID.get_uI();
    // }
    // return 0.0;
}
double ProstyUAR::getSkladowaD()
{
    // if (getTypRegulatora() == TypRegulatora::PID)
    // {
    return m_regulatorPID.get_uD();
    // }
    // return 0.0;
}

