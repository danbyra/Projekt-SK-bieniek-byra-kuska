#pragma once
#include <QTimer>
#include <QElapsedTimer>
#include "include.h"
#include "Centrala.h"
#include "communication_protocol.h"
#include "ust_arx.h"
#include "Hub/network_hub.h"
#include <QLineEdit>
#include <QInputDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class JednostronnyManager;
class ObustronnyManager;

struct Limity {
    double min;
    double max;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

    friend class JednostronnyManager;
    friend class ObustronnyManager;

private:
    Ui::MainWindow *ui;
    Centrala m_centrala{};

    bool m_blokujMonitTrybuPracy=false;

    //
    Centrala m_centralaSieciowa;
    network_hub m_hub;
    uint32_t nr_probki=0;
    uint32_t ostatni_numer_sieciowy=0;
    QTimer *zegar;
    bool m_pierwszaProbkaPoStart = false;//dla sygnału start
    bool m_resetCalkiWyslij = false;
    bool m_resetRozniczkiWyslij = false;
    double m_czasSieciowy = 0.0; //!Przechowuje skumulowany czas symulacji sieciowej-unikniecie skoku czasu
    int64_t m_ostatniaProbkaSieciowa = 0; //!Przechowuje numer ostatniej próbki sieciowej
    JednostronnyManager* m_jednostronny;
    ObustronnyManager* m_obustronny;


    QLineSeries *m_seriaWartosciZadanej = new QLineSeries(), *m_seriaWartosciRegulowanej = new QLineSeries();
    QLineSeries *m_seriaUchybu = new QLineSeries();
    QLineSeries *m_seriaSterowania = new QLineSeries();
    QLineSeries *m_seriaSkladowejP = new QLineSeries(), *m_seriaSkladowejI = new QLineSeries(), *m_seriaSkladowejD = new QLineSeries();

    QValueAxis *m_axisX1 = new QValueAxis(), *m_axisY1 = new QValueAxis();
    QValueAxis *m_axisX2 = new QValueAxis(), *m_axisY2 = new QValueAxis();
    QValueAxis *m_axisX3 = new QValueAxis(), *m_axisY3 = new QValueAxis();
    QValueAxis *m_axisX4 = new QValueAxis(), *m_axisY4 = new QValueAxis();

    QVector<QPointF> m_punktySeriiWartosciZadanej{}, m_punktySeriiWartosciRegulowanej{};
    QVector<QPointF> m_punktySeriiUchybu{};
    QVector<QPointF> m_punktySeriiSterowania{};
    QVector<QPointF> m_punktySeriiSkladowejP{}, m_punktySeriiSkladowejI{}, m_punktySeriiSkladowejD{};

    QVector<double> m_ostatnieY1{};
    QVector<double> m_ostatnieY2{};
    QVector<double> m_ostatnieY3{};
    QVector<double> m_ostatnieY4{};

    double m_poprzOkno = 0.0;
    QElapsedTimer m_lastChartRepaintTimer;

    Centrala& getAktywnaCentrala();

    void przesunOsX(double krok, double okno, double czas);
    void przesunOsY();
    void dodajPunkty(double czas);
    void usunPunkty(double czas);
    void podmienPunkty();
    void wyczyscWykresy();
    void messageBoxInnyWatek(QMessageBox::Icon ikona, const QString& tytul, const QString& tekst);

    double ostatnie_odebrane_u = 0.0;
    double ostatnie_odebrane_y = 0.0;

    // Deklaracje funkcji wysyłających
    void wyslij_sterowanie_przez_siec(double u);
    void wyslij_wyjscie_przez_siec(double y);

    // metody serializacji i komunikacji przeniesione z klasy Komunikacja
    communication_protocol::dane_konfiguracyjne PID_do_wyslania_tekstowe();
    void PID_odbior_tekstowe(const communication_protocol::dane_konfiguracyjne& dane);
    communication_protocol::dane_konfiguracyjne ARX_do_wyslania_tekstowe();
    void ARX_odbior_tekstowe(const communication_protocol::dane_konfiguracyjne& dane);
    communication_protocol::dane_konfiguracyjne Generator_do_wyslania_tekstowe();
    void generator_odbior_tekstowe(const communication_protocol::dane_konfiguracyjne& dane);
    communication_protocol::dane_sterujace Interwal_do_wyslania_tekstowe(double t0 = 0.0);
    void Interwal_odbior_tekstowe(const communication_protocol::dane_sterujace& dane);
    communication_protocol::dane_sterujace Okno_do_wyslania_tekstowe();
    void Okno_odbior_tekstowe(const communication_protocol::dane_sterujace& dane);

    communication_protocol::dane_konfiguracyjne PID_do_wyslania_binarnka();
    void PID_odbior_binarnka(const communication_protocol::dane_konfiguracyjne& dane);
    communication_protocol::dane_konfiguracyjne ARX_do_wyslania_binarnka();
    void ARX_odbior_binarnka(const communication_protocol::dane_konfiguracyjne& dane);
    communication_protocol::dane_konfiguracyjne Generator_do_wyslania_binarnka();
    void generator_odbior_binarnka(const communication_protocol::dane_konfiguracyjne& dane);
    communication_protocol::dane_sterujace Interwal_do_wyslania_binarnka(double t0 = 0.0);
    void Interwal_odbior_binarnka(const communication_protocol::dane_sterujace& dane);
    communication_protocol::dane_sterujace Okno_do_wyslania_binarnka();
    void Okno_odbior_binarnka(const communication_protocol::dane_sterujace& dane);

    communication_protocol::dane_sterujace Komenda_do_wyslania_tekstowe(QString komenda, double t0 = 0.0);
    communication_protocol::dane_sterujace Komenda_do_wyslania_binarnka(QString komenda, double t0 = 0.0);


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void ogarnianieWykresow();

    void on_start_clicked();
    void on_stop_clicked();
    void on_reset_clicked();
    void on_okno_editingFinished();
    void on_interwal_editingFinished();

    void on_parametryARX_clicked();

    void on_generatorCombo_currentIndexChanged(const int index);
    void on_okres_editingFinished();
    void on_amplituda_editingFinished();
    void on_skladowaStala_editingFinished();
    void on_wypelnienie_editingFinished();

    // void on_regulatorCombo_currentIndexChanged(const int index);
    void on_stalaI_editingFinished();
    void on_stalaD_editingFinished();
    void on_wzmocnienie_editingFinished();
    void on_rodzajCalkowaniaCombo_currentIndexChanged(const int index);
    void on_resetCalki_clicked();
    void on_resetRozniczki_clicked();




#ifndef STARY_CHECKBOX
    void on_ograniczenia_checkStateChanged(const Qt::CheckState &arg1);
    // void on_antiWindup_checkStateChanged(const Qt::CheckState &arg1);
#else    // dwa nastepne sloty to fallback do wersji < 6.7
    void on_ograniczenia_stateChanged(const int &arg1);
    // void on_antiWindup_stateChanged(const int &arg1);
#endif  // STARY_CHECKBOX
    void on_ograniczeniaMin_editingFinished();
    void on_ograniczeniaMax_editingFinished();
    // void on_ON_editingFinished();
    // void on_histereza_editingFinished();

    void on_jsonZapisz_clicked();
    void on_jsonWczytaj_clicked();

    //
    //void Odbierz_paczke(uint32_t nr_probki, const communication_protocol::dane_konfiguracyjne& dane);

    //wybor trybu sieciowego
    void on_wybor_serializacji_comboBox_currentIndexChanged(int index);
    void on_taktowanie_comboBox_currentIndexChanged(int index);

    //Ustawienia potwierdzania trybu pracy
    void on_comboBox_currentTextChanged(const QString &arg1);

    void Czy_polaczylo();
    //odpalenie
    void on_server_radioButton_clicked();
    void on_klient_radioButton_clicked();
    //
    void czy_rozlaczylo();
    //kontrolki
    void wylacz_wlacz_kontrolki(bool reg, bool arx);
    void wylacz_wlacz_kontrolki_sieciowe(bool b);
    void wylacz_wlacz_kontrolki_start_stop(bool s);
    //pid
    void PID_wysyl();
    //void PID_odbior();
    //arx
    void ARX_wysyl();
    //void ARX_odbior();
    //gen
    void Gen_wysyl();
    //void Gen_odbior();
    //interwal
    void Interwal_wysyl(double t0 = 0.0);
    //void Interwal_odbior();
    void Komenda_wysyl(QString komenda, double t0 = 0.0);
    void Okno_wysyl();

    // Symulacja
    //sloty sieciowe
    void na_odebrano_konfiguracje(uint32_t numer, communication_protocol::dane_konfiguracyjne dane);
    void na_odebrano_symulacje(uint32_t numer, communication_protocol::dane_symulacji dane);
    void na_odebrano_sterowanie(uint32_t numer, communication_protocol::dane_sterujace dane);

    void on_zegar_timeout();

    void wczytajSekcjeRegulatora(const QJsonObject &json);
    void wczytajSekcjeObiektu(const QJsonObject &json);

};
