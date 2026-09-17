#pragma once

#include "include.h"

using namespace std;

class ModelARX
{
private:
	vector<double> wspolczynnikiA;
	vector<double> wspolczynnikiB;
	deque<double> u_buffor;
	deque<double> y_buffor;
	deque<double> delay_buffor;

	int opoznienieTransportowe;
	int numerSkoku = 0;
	double minSterowania = -10.0;
	double maxSterowania = 10.0;
	double minRegulowania = -10.0;
	double maxRegulowania = 10.0;
	double odchylenieStandardoweSzumu = 0.0;
	bool wlacznikSterowania = true;
	bool wlacznikRegulacji = true;

	default_random_engine generator;
	normal_distribution<double> rozkladNormalny;
		
public:
    ModelARX();
	ModelARX(vector<double> A, vector<double> B, int oT = 1, double wartoscZ = 0.0);

	double symuluj(double probkaSygnaluSterujacego);

    /*reset pamięci ARX DODANAE*/
    void ResetPamieci();

    //Pamiec stanów dodana na potrzeby bezuderzeniowej pracy
    void aktualizujHistoriaBuforow(double u_nowe, double y_nowe);

	void set_opoznienieTransportowe(int oT);
	void setWspolczynnikA(vector<double> A);
	void setWspolczynnikB(vector<double> B);
	void setWartoscZaklocenia(double wartoscZ);
	void setWartoscSterowania(double minS, double maxS);
	void setWartoscRegulowania(double minR, double maxR);

	void uzupelnijWspolczynniki(vector<double>& uzupelnienie);
	void wlaczSterowanie(bool wlacznikS);
	void wlaczRegulacje(bool wlacznikR);
    bool isSterowanieWlaczone() const { return wlacznikSterowania; }
    bool isRegulacjaWlaczona() const { return wlacznikRegulacji; }

    double getMinSterowania() const { return minSterowania; }
    double getMaxSterowania() const { return maxSterowania; }
    double getMinRegulowania() const { return minRegulowania; }
    double getMaxRegulowania() const { return maxRegulowania; }
    const vector<double>& getWspolczynnikiA() const { return wspolczynnikiA; }
    const vector<double>& getWspolczynnikiB() const { return wspolczynnikiB; }
    int getOpoznienieTransportowe() const { return opoznienieTransportowe; }
    double getWartoscZaklocenia() const { return odchylenieStandardoweSzumu; }


};

