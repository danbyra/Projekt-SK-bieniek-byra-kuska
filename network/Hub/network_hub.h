#ifndef NETWORK_HUB_H
#define NETWORK_HUB_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QTcpSocket>
#include <QTcpServer>
#include <vector>

#include "../../core/RegulatorPID.h"
#include "../../core/ModelARX.h"
#include "../../core/GenSyg.h"
#include "../communication_protocol.h"


class network_hub : public QObject {
    Q_OBJECT
public:
    explicit network_hub(QObject* parent = nullptr);
    ~network_hub(){rozlacz();}

    void uruchom_jako_serwer(quint16 port);
    void uruchom_jako_klient(const QString& ip, quint16 port);
    void rozlacz();

    QString get_serwer_ip();
    QString get_serwer_gniazdo();

    // wysyłanie 3 bloków symulacji - gen nie pewny
    void wyslij_zrzut_konfiguracji(uint32_t numer_probki, RegulatorPID* pid, ModelARX* arx, GenSyg* gen);

    // wysyłąnie samych wyjść/wejść
    void wyslij_ramke(uint32_t numer_probki, double wejscie_obiektu_u, double wyjscie_obiektu_y, double wartosc_zadana_w, double czas = 0.0);

    // dla startu/stopu/resetu i zmiany interwału - do sprawdzenia
    void wyslij_komende(uint32_t numer_probki, const QString& komenda, int interwal);

    
    void transmituj_konfiguracje(uint32_t numer_probki, const communication_protocol::dane_konfiguracyjne& pakiet);
    void transmituj_ramke(uint32_t numer_probki, const communication_protocol::dane_symulacji& pakiet);
    void transmituj_komende(uint32_t nr_probki, const communication_protocol::dane_sterujace& pakiet);

    // PPS, synchronizacja
    communication_protocol& get_protokol() { return m_protokol; }

signals:
    void odebrano_symulacje(uint32_t numer_probki, communication_protocol::dane_symulacji dane);
    void odebrano_sterowanie(uint32_t numer_probki, communication_protocol::dane_sterujace dane);
    void odebrano_konfiguracje(uint32_t numer_probki, communication_protocol::dane_konfiguracyjne dane);
    void polaczono();
    void rozlaczono();

private slots:
    void na_nowe_polaczenie();
    void na_nowe_dane();
    void na_rozlaczenie();


private:
    QString formatuj_pid(RegulatorPID* pid);
    QString formatuj_arx(ModelARX* arx);
    QString formatuj_wektor(const std::vector<double>& wektor);
    QString formatuj_gen_syg(GenSyg* gen);
    
    
   // void transmituj_konfiguracje(uint32_t numer_probki, const communication_protocol::dane_konfiguracyjne& pakiet);
   // void transmituj_ramke(uint32_t numer_probki, const communication_protocol::dane_symulacji& pakiet);
    //void transmituj_komende(uint32_t nr_probki, const communication_protocol::dane_sterujace& pakiet);

    QTcpServer* m_serwer;
    QTcpSocket* m_gniazdo;
    communication_protocol m_protokol;
    QByteArray m_bufor;
};

#endif // NETWORK_HUB_H
