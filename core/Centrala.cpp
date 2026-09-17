    #include "Centrala.h"

const vector<double> DOMYSLNE_A = {-0.4};
const vector<double> DOMYSLNE_B = {0.6};
const double DOMYSLNY_PID_K = 0.5;
const double DOMYSLNY_PID_TI = 5.0;
const double DOMYSLNY_PID_TD = 0.2;
const double DOMYSLNY_ON = 1.0;
const double DOMYSLNA_HIST = 0.1;
// const int DOMYSLNE_TAKTOWANIE = 200;

Centrala::Centrala()
    : m_generatorSygnalu(),
    m_ProstyUAR(
        ModelARX{DOMYSLNE_A, DOMYSLNE_B, 1, 0.0},
        RegulatorPID{DOMYSLNY_PID_K, DOMYSLNY_PID_TI, DOMYSLNY_PID_TD}),
    m_timer(new QTimer(this)) {
    m_timer->setTimerType(Qt::PreciseTimer);

    connect(m_timer, &QTimer::timeout, this, &Centrala::onTimeout);
}

Centrala::~Centrala() {}
void Centrala::zaktualizujWykres() {
    emit ogarnijWykresy();
    m_czas += (getTaktowanie() / 1000.0);
}



void Centrala::emitujWykres() {
    emit ogarnijWykresy();
}



void Centrala::onTimeout() {
    wykonajKrokSymulacji();
    zaktualizujWykres();
}

void Centrala::set_Okno(double o) { m_okno = o; }

void Centrala::resetSymulacji()
{

    /*Reset pamieci ARX oraz PID przez ProstyUAR oraz reset pamieci generatora */
    m_ProstyUAR.ResetStanow();
    m_generatorSygnalu.ResetPamieci();

    //m_ProstyUAR.get_PID()->resetPamieci();
    m_ostatnie_w = 0.0;
    m_ostatnie_y = 0.0;
}
void Centrala::wykonajKrokSymulacji() {
    m_ostatnie_w = m_generatorSygnalu.generuj();
    m_ostatnie_y = m_ProstyUAR.symuluj(m_ostatnie_w);
}

void Centrala::wykonajKrokKlienta() {
    m_ostatnie_w = m_generatorSygnalu.generuj();
    m_ProstyUAR.symulujTylkoRegulator(m_ostatnie_w);
}

void Centrala::wykonajKrokSerwera() {
    m_ostatnie_y = m_ProstyUAR.symulujTylkoARX();
}

void Centrala::odbierzY(double noweY) {
    m_ostatnie_y = noweY;
    m_ProstyUAR.wymusY(noweY);
}

void Centrala::odbierzU(double noweU) {
    m_ProstyUAR.wymusU(noweU);
}

void Centrala::odbierzW(double noweW) {
    m_ostatnie_w = noweW;
    m_ProstyUAR.wymusW(noweW);
}

double Centrala::getAktualnaWartoscZadana() const { return m_ostatnie_w; }
double Centrala::getAktualnaWartoscRegulowana() const { return m_ostatnie_y; }
double Centrala::getAktualnyUchyb() const {
    if (m_uzyjCustomWykres) return m_custom_e;
    return m_ProstyUAR.getUchyb();
}
double Centrala::getAktualneSterowanie() const {
    if (m_uzyjCustomWykres) return m_custom_u;
    return m_ProstyUAR.getSterowania();
}
double Centrala::getSkladowaP() {
    if (m_uzyjCustomWykres) return m_custom_uP;
    return m_ProstyUAR.getSkladowaP();
}
double Centrala::getSkladowaI() {
    if (m_uzyjCustomWykres) return m_custom_uI;
    return m_ProstyUAR.getSkladowaI();
}
double Centrala::getSkladowaD() {
    if (m_uzyjCustomWykres) return m_custom_uD;
    return m_ProstyUAR.getSkladowaD();
}

void Centrala::setTypGenerator(TypSyg typ) { m_generatorSygnalu.set_typ(typ); }
void Centrala::setOkresGenerator(double Trz) {
    m_generatorSygnalu.set_Trz(Trz);
}
void Centrala::setAmplitudaGenerator(double A) { m_generatorSygnalu.set_A(A); }
void Centrala::setSkladowaStalaGenerator(double S) {
    m_generatorSygnalu.set_S(S);
}
void Centrala::setWypelnienieGenerator(double p) {
    m_generatorSygnalu.set_p(p);
}
void Centrala::setTaktowanie(int Tt) {
    // Zachowuje fazę generatora przy zmianie interwału — brak skoku sygnału W.
    m_generatorSygnalu.set_Tt_zachowujFaze(Tt);
    m_timer->setInterval(Tt);
}
int Centrala::getTaktowanie() const { return m_generatorSygnalu.get_Tt(); }
void Centrala::setPID_K(double K) { m_ProstyUAR.setPID_K(K); }
void Centrala::setPID_Ti(double Ti) { m_ProstyUAR.setPID_Ti(Ti); }
void Centrala::setPID_Td(double Td) { m_ProstyUAR.setPID_Td(Td); }
void Centrala::setPID_TrybCalki(RegulatorPID::SposobLiczeniaCalki tryb) {
    m_ProstyUAR.setPID_TrybCalki(tryb);
}
void Centrala::resetPID_Calka() { m_ProstyUAR.resetPID_Calka(); }
void Centrala::resetPID_Rozniczka() { m_ProstyUAR.resetPID_Rozniczka(); }
void Centrala::setPID_Nasycenie(bool wlacznik, double min, double max) {
    m_ProstyUAR.setPID_Nasycenie(wlacznik, min, max);
}

void Centrala::setPID_Ograniczenia(bool wlacz) {
    m_ProstyUAR.setPID_Ograniczenia(wlacz);
}
void Centrala::setPID_ZakresOgraniczen(double min, double max) {
    m_ProstyUAR.setPID_ZakresOgraniczen(min, max);
}

void Centrala::zastosujParametryARX(vector<double> A, vector<double> B,
                                    int opoznienie, double szum, bool wlaczSter,
                                    double minS, double maxS, bool wlaczReg,
                                    double minR, double maxR) {
    m_ProstyUAR.zastosujParametryARX(A, B, opoznienie, szum, wlaczSter, minS,
                                     maxS, wlaczReg, minR, maxR);
}

void Centrala::SetRegulator(ProstyUAR::TypRegulatora typ)
{
    m_ProstyUAR.setRegulator(typ);
}
// PID
double Centrala::getPID_K() { return m_ProstyUAR.getPID_K(); }
double Centrala::getPID_Ti() { return m_ProstyUAR.getPID_Ti(); }
double Centrala::getPID_Td() { return m_ProstyUAR.getPID_Td(); }
RegulatorPID::SposobLiczeniaCalki Centrala::getPID_TrybCalki() const {
    return m_ProstyUAR.getPID_TrybCalki();
}
bool Centrala::getPID_Ograniczenia() const {
    return m_ProstyUAR.getPID_Ograniczenia();
}
double Centrala::getPID_U_Min() const { return m_ProstyUAR.getPID_U_Min(); }
double Centrala::getPID_U_Max() const { return m_ProstyUAR.getPID_U_Max(); }
// generator
TypSyg Centrala::getGenerator_Typ() const {
    return m_generatorSygnalu.get_typ();
}
double Centrala::getGenerator_Okres() const {
    return m_generatorSygnalu.get_Trz();
}
double Centrala::getGenerator_Amplituda() const {
    return m_generatorSygnalu.get_A();
}
double Centrala::getGenerator_SkladowaStala() const {
    return m_generatorSygnalu.get_S();
}
double Centrala::getGenerator_Wypelnienie() const {
    return m_generatorSygnalu.get_p();
}
void Centrala::skopiujStan(const Centrala& inna) {
    m_generatorSygnalu = inna.m_generatorSygnalu;
    m_ProstyUAR       = inna.m_ProstyUAR;
    m_czas            = inna.m_czas;
    m_okno            = inna.m_okno;
    m_ostatnie_w      = inna.m_ostatnie_w;
    m_ostatnie_y      = inna.m_ostatnie_y;
}
