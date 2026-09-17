#pragma once

#include "include.h"

#include "GenSyg.h"
#include "ModelARX.h"
#include "RegulatorPID.h"

class ProstyUAR
{
public:
    enum class TypRegulatora{
        PID,
        ONOFF
    };
private:
    ModelARX m_arx;
    RegulatorPID m_regulatorPID;
    TypRegulatora m_typRegulatora;

    double w = 0.0; //wartosc zadana
    double y = 0.0; //wartosc zmierzona z arx
    double e = 0.0; //uchyb, e_i = w_i - y_(i-1)
    double u = 0.0; //wartosc sterowania, u_i = regulator(e_i)

public:
    // ProstyUAR();
    ProstyUAR(ModelARX arx, RegulatorPID regulatorPID);// UWAGA trzeba ustawic typ bo domyslny to PID

    double symuluj(double sygnalwejsciowy); //sygnal z generatora
    double symulujTylkoRegulator(double sygnalWejsciowy); //tylko PID
    double symulujTylkoARX(); //tylko obiekt ARX

    void wymusY(double noweY);
    void wymusU(double noweU);
    void wymusW(double noweW);

    //Aktualizacja wartości ARX && PID w tle potrzebne w przypadku zerwania połaczenia sieciowego
    void aktualizujTloARX(double noweU,double noweY);
    void aktualizujTloPID(double noweW,double noweY);

    //!Bezuderzeniowe przejście
    void dostosujCalkeDoU(double u_cel, double e);
    void resetPID();

    /*RESET pamieci ARX oraz PID */
    void ResetStanow();
    void resetPID_Calka();
    void resetPID_Rozniczka();
    void zastosujParametryARX(vector<double> A, vector<double> B, double opoznienie, double szum, bool wlaczSter, double minS, double maxS, bool wlaczReg, double minR, double maxR);

    void setPID_K(double K);
    void setPID_Ti(double Ti);
    void setPID_Td(double Td);
    /*Roboczo wykorzystałem poprzednia logikę zwiazaną z typem regulatora*/
    void setRegulator(TypRegulatora typ);
    void setPID_TrybCalki(RegulatorPID::SposobLiczeniaCalki tryb);
    void setPID_Nasycenie(bool wlacznik, double min, double max);
    void setPID_Ograniczenia(bool wlacz);
    void setPID_ZakresOgraniczen(double min, double max);

    double getUchyb() const;
    double getSterowania() const;
    /*Roboczo wykorzystałem poprzednia logikę zwiazaną z typem regulatora*/
    TypRegulatora GetRegulator()const;
    double getPID_K() const;
    double getPID_Ti() const;
    double getPID_Td() const;
    bool getPID_Ograniczenia() const;
    double getPID_U_Min() const;
    double getPID_U_Max() const;
    double getSkladowaP();
    double getSkladowaI();
    double getSkladowaD();
    RegulatorPID::SposobLiczeniaCalki getPID_TrybCalki() const;

    ModelARX* get_ARX() { return &m_arx; };
    RegulatorPID* get_PID() { return &m_regulatorPID; };


};
