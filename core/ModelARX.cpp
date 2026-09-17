#include "ModelARX.h"

ModelARX::ModelARX() {}

ModelARX::ModelARX(vector<double> A, vector<double> B, int oT, double wartoscZ)
    : wspolczynnikiA(A), wspolczynnikiB(B),
      odchylenieStandardoweSzumu(wartoscZ),
      rozkladNormalny(
          0.0, 1.0) // usunalem referencje do vectorow bo w testach nie dzialalo
{
  set_opoznienieTransportowe(oT);

  uzupelnijWspolczynniki(wspolczynnikiA);
  uzupelnijWspolczynniki(wspolczynnikiB);

  random_device rng;
  generator.seed(rng());

  size_t u_rozmiar = wspolczynnikiB.size();
  u_buffor = deque<double>(u_rozmiar, 0.0);
  size_t y_rozmiar = wspolczynnikiA.size();
  y_buffor = deque<double>(y_rozmiar,
                           0.0); // inicjalizacja buforow zeby nie byly puste
  delay_buffor = deque<double>(opoznienieTransportowe, 0.0);
}

double ModelARX::symuluj(double probkaSygnaluSterujacego) {
  double u_zastepcze = 0.0;

  if (wlacznikSterowania) {
    if (probkaSygnaluSterujacego < minSterowania)
      u_zastepcze = minSterowania;
    else if (probkaSygnaluSterujacego > maxSterowania)
      u_zastepcze = maxSterowania;
    else
      u_zastepcze = probkaSygnaluSterujacego;
  } else
    u_zastepcze = probkaSygnaluSterujacego;

  // bufor opoznienia transportowego
  delay_buffor.push_back(u_zastepcze);
  double u_k = delay_buffor.front();
  delay_buffor.pop_front();

  //  bufor wejsciowy
  u_buffor.push_front(u_k);
  if (u_buffor.size() > wspolczynnikiB.size())
    u_buffor.pop_back();

  // obliczanie wyjscia modelu
  double y_now = 0.0;

  // skladnik od wejscia (czesc B)
  for (size_t i = 0; i < wspolczynnikiB.size(); i++)
    y_now += wspolczynnikiB[i] * u_buffor[i];
  // double y1 = inner_product(wspolczynnikiB.begin(), wspolczynnikiB.end(),
  // u_buffor.begin(), 0.0);

  // skladnik od wyjscia (czesc A)
  for (size_t i = 0; i < wspolczynnikiA.size(); i++)
    y_now -= wspolczynnikiA[i] * y_buffor[i];
  // double y2 = inner_product(wspolczynnikiA.begin(), wspolczynnikiA.end(),
  // y_buffor.begin(), 0.0); y_now = y1 - y2;

  if (odchylenieStandardoweSzumu > 0) {
    double szum = rozkladNormalny(generator) * odchylenieStandardoweSzumu;
    y_now += szum;
  }

  double y_zastepcze = 0.0;
  if (wlacznikRegulacji) {
    if (y_now < minRegulowania)
      y_zastepcze = minRegulowania;
    else if (y_now > maxRegulowania)
      y_zastepcze = maxRegulowania;
    else
      y_zastepcze = y_now;
  } else
    y_zastepcze = y_now;

  y_buffor.push_front(y_zastepcze);
  if (y_buffor.size() > wspolczynnikiA.size())
    y_buffor.pop_back();

  return y_zastepcze;
}

// historia buforów do prawidłowej symulacji po przerwaniu połaczenia sieciowego
void ModelARX:: aktualizujHistoriaBuforow(double u_nowe, double y_nowe)
{
    double u_zastepcze=wlacznikSterowania ? std::clamp(u_nowe, minSterowania,maxSterowania): u_nowe;

    //opoznienie kolejki -> opoznienie transportowe
    delay_buffor.push_back(u_zastepcze);
    double u_k=delay_buffor.front();
    delay_buffor.pop_front();

    //Aktualizacja bufor wejścia
    u_buffor.push_front(u_k);
    if(u_buffor.size()>wspolczynnikiB.size())
    {
        u_buffor.pop_back();
    }
    double y_zastepcze=wlacznikRegulacji ? std::clamp(y_nowe,minRegulowania,maxRegulowania): y_nowe;

    //Aktualizacja buforu wyjścia
    y_buffor.push_front(y_zastepcze);
    if(y_buffor.size()>wspolczynnikiA.size())
    {
        y_buffor.pop_back();
    }
}

/*Reset pamieci ARX */
void ModelARX::ResetPamieci()
{
    //wypelnienie 0.0 bufforów dla ARX
    fill(u_buffor.begin(),u_buffor.end(),0.0);
    fill(y_buffor.begin(),y_buffor.end(),0.0);
    fill(delay_buffor.begin(),delay_buffor.end(),0.0);
}


void ModelARX::set_opoznienieTransportowe(int oT) {
  opoznienieTransportowe = (oT < 1) ? 1 : oT;
}

void ModelARX::uzupelnijWspolczynniki(vector<double> &uzupelnienie) {
  while (uzupelnienie.size() < 3)
    uzupelnienie.push_back(0.0);
}

void ModelARX::setWspolczynnikA(vector<double> A) {
  wspolczynnikiA = A;
  uzupelnijWspolczynniki(wspolczynnikiA);

  size_t n_rozmiar = wspolczynnikiA.size();
  while (y_buffor.size() > n_rozmiar)
    y_buffor.pop_back();
  while (y_buffor.size() < n_rozmiar)
    y_buffor.push_back(0.0);
}

void ModelARX::setWspolczynnikB(vector<double> B) {
  wspolczynnikiB = B;
  uzupelnijWspolczynniki(wspolczynnikiB);

  size_t n_rozmiar = wspolczynnikiB.size();
  while (u_buffor.size() > n_rozmiar)
    u_buffor.pop_back();
  while (u_buffor.size() < n_rozmiar)
    u_buffor.push_back(0.0);

  size_t delay_rozmiar = opoznienieTransportowe;
  while (delay_buffor.size() > delay_rozmiar)
    delay_buffor.pop_back();
  while (delay_buffor.size() < delay_rozmiar)
    delay_buffor.push_back(0.0);
}

void ModelARX::setWartoscSterowania(double minS, double maxS) {
  double nowyMin = minS;
  double nowyMax = maxS;

  if (nowyMin < -100.0)
    nowyMin = -100.0;
  if (nowyMin > 100.0)
    nowyMin = 100.0;

  if (nowyMax < -100.0)
    nowyMax = -100.0;
  if (nowyMax > 100.0)
    nowyMax = 100.0;

  if (nowyMin > nowyMax) {
    this->minSterowania = nowyMax;
    this->maxSterowania = nowyMin;
  } else {
    this->minSterowania = nowyMin;
    this->maxSterowania = nowyMax;
  }
}

void ModelARX::setWartoscRegulowania(double minR, double maxR) {
  double nowyMin = minR;
  double nowyMax = maxR;

  if (nowyMin < -100.0)
    nowyMin = -100.0;
  if (nowyMin > 100.0)
    nowyMin = 100.0;

  if (nowyMax < -100.0)
    nowyMax = -100.0;
  if (nowyMax > 100.0)
    nowyMax = 100.0;

  if (nowyMin > nowyMax) {
    this->minRegulowania = nowyMax;
    this->maxRegulowania = nowyMin;
  } else {
    this->minRegulowania = nowyMin;
    this->maxRegulowania = nowyMax;
  }
}

void ModelARX::setWartoscZaklocenia(double wartoscZ) {
  if (wartoscZ < 0.0)
    this->odchylenieStandardoweSzumu = 0.0;
  else
    this->odchylenieStandardoweSzumu = wartoscZ;
}

void ModelARX::wlaczSterowanie(bool wlacznikS) {
  this->wlacznikSterowania = wlacznikS;
}

void ModelARX::wlaczRegulacje(bool wlacznikR) {
  this->wlacznikRegulacji = wlacznikR;
}
