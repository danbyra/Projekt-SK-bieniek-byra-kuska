#include "network_hub.h"
#include <QStringList>


QString network_hub::get_serwer_ip()
{// return m_serwer->serverAddress().toString();
    if (m_serwer) {
        return m_serwer->serverAddress().toString();
    }
    if (m_gniazdo) {
        return m_gniazdo->peerAddress().toString();
    }
    return "Brak";
}

QString network_hub::get_serwer_gniazdo()
{
    return QString::number(m_serwer->serverPort());
}

network_hub::network_hub(QObject* parent) 
    : QObject(parent), m_serwer(nullptr), m_gniazdo(nullptr) 
{
    connect(&m_protokol, &communication_protocol::odebrano_symulacje, this, &network_hub::odebrano_symulacje);
    connect(&m_protokol, &communication_protocol::odebrano_sterowanie, this, &network_hub::odebrano_sterowanie);
    connect(&m_protokol, &communication_protocol::odebrano_konfiguracje, this, &network_hub::odebrano_konfiguracje);
}

void network_hub::uruchom_jako_serwer(quint16 port) {
    rozlacz();
    m_bufor.clear();
    m_serwer = new QTcpServer(this);
    connect(m_serwer, &QTcpServer::newConnection, this, &network_hub::na_nowe_polaczenie);
    if (m_serwer->listen(QHostAddress::Any, port)) {//any by było widoczne
    }
}

void network_hub::uruchom_jako_klient(const QString& ip, quint16 port) {
    rozlacz();
    m_bufor.clear();
    m_gniazdo = new QTcpSocket(this);
    //wyeliminowanie opóźnień buforowania TCP
    m_gniazdo->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    connect(m_gniazdo, &QTcpSocket::connected, this, &network_hub::polaczono);
    connect(m_gniazdo, &QTcpSocket::disconnected, this, &network_hub::na_rozlaczenie);
    connect(m_gniazdo, &QTcpSocket::readyRead, this, &network_hub::na_nowe_dane);
    m_gniazdo->connectToHost(ip, port);
}



void network_hub::rozlacz() {
    m_bufor.clear();
    if (m_gniazdo) {
        m_gniazdo->disconnectFromHost();
        m_gniazdo->deleteLater();
        m_gniazdo = nullptr; //o tym zapomniałem
    }
    if (m_serwer) {
        m_serwer->close();
        m_serwer->deleteLater();
        m_serwer = nullptr;// o tym też
    }
}

void network_hub::na_nowe_polaczenie() {
    if (m_serwer && m_serwer->hasPendingConnections()) {
        if (m_gniazdo) {
            m_gniazdo->deleteLater();
        }
        m_gniazdo = m_serwer->nextPendingConnection();
        //wyeliminowanie opóźnień buforowania TCP na serverze
        m_gniazdo->setSocketOption(QAbstractSocket::LowDelayOption, 1);
        connect(m_gniazdo, &QTcpSocket::disconnected, this, &network_hub::na_rozlaczenie);
        connect(m_gniazdo, &QTcpSocket::readyRead, this, &network_hub::na_nowe_dane);
        emit polaczono();
    }
}

void network_hub::na_nowe_dane() {
    if (!m_gniazdo) return;
    m_bufor.append(m_gniazdo->readAll());
    m_protokol.przetworz_strumien(m_bufor);
    // Rejestracja zarejestrowanych pakietow pps
    m_protokol.zarejestruj_odebranie_pakietu();
}

void network_hub::na_rozlaczenie() {
    m_bufor.clear();
    emit rozlaczono();
}




void network_hub::wyslij_zrzut_konfiguracji(uint32_t numer_probki, RegulatorPID* pid, ModelARX* arx, GenSyg* gen) {
    communication_protocol::dane_konfiguracyjne kompilacja;

    kompilacja.ustawienia_pid = formatuj_pid(pid);
    kompilacja.ustawienia_arx = formatuj_arx(arx);
    kompilacja.ustawienia_generatora = formatuj_gen_syg(gen);

    transmituj_konfiguracje(numer_probki, kompilacja);
}

void network_hub::wyslij_ramke(uint32_t numer_probki, double wejscie_obiektu_u, double wyjscie_obiektu_y, double wartosc_zadana_w, double czas) {
    communication_protocol::dane_symulacji ramka;
    ramka.sygnal_sterujacy = wejscie_obiektu_u;
    ramka.wartosc_regulowana = wyjscie_obiektu_y;
    ramka.wartosc_zadana = wartosc_zadana_w;
    ramka.czas = czas;

    transmituj_ramke(numer_probki, ramka);
}

void network_hub::wyslij_komende(uint32_t numer_probki, const QString& komenda, int interwal) {
    communication_protocol::dane_sterujace kom;
    kom.polecenie = komenda;
    kom.interwal = interwal;

    transmituj_komende(numer_probki, kom);
}


QString network_hub::formatuj_wektor(const std::vector<double>& wektor) {
    QStringList tekstowe;
    for (double w : wektor) {
        tekstowe.append(QString::number(w, 'f', 2));
    }
    QString wynik = "[";
    wynik.append(tekstowe.join(", "));
    wynik.append("]");

    return wynik;
}

QString network_hub::formatuj_pid(RegulatorPID* pid) {
    if (!pid) return "Brak";
    return QString("k=%1;Ti=%2;Td=%3").arg(pid->get_k()).arg(pid->get_Ti()).arg(pid->get_Td());
}

QString network_hub::formatuj_arx(ModelARX* arx) {
    if (!arx) return "Brak";
    return QString("A=%1;B=%2;opz=%3")
        .arg(formatuj_wektor(arx->getWspolczynnikiA()))
        .arg(formatuj_wektor(arx->getWspolczynnikiB()))
        .arg(arx->getOpoznienieTransportowe());
}

QString network_hub::formatuj_gen_syg(GenSyg* gen) {
    if (!gen) return "Brak";
    QString typNazwa = (gen->get_typ() == GenSyg::TypSyg::Sinusoida) ? "Sinusoida" : "Prostokat";
    return QString("Typ=%1;Trz=%2;Tt=%3;A=%4;S=%5")
        .arg(typNazwa)
        .arg(gen->get_Trz())
        .arg(gen->get_Tt())
        .arg(gen->get_A())
        .arg(gen->get_S());
}


void network_hub::transmituj_konfiguracje(uint32_t nr, const communication_protocol::dane_konfiguracyjne& pakiet) {
    if (m_gniazdo && m_gniazdo->isOpen()) {
        QByteArray dane = m_protokol.stworz_wiadomosc_konfiguracyjna(nr, pakiet);
        m_gniazdo->write(dane);
        m_protokol.zarejestruj_wyslanie_pakietu();
    }
}

void network_hub::transmituj_ramke(uint32_t nr, const communication_protocol::dane_symulacji& pakiet) {
    if (m_gniazdo && m_gniazdo->isOpen()) {
        QByteArray dane = m_protokol.stworz_wiadomosc_symulacji(nr, pakiet);
        m_gniazdo->write(dane);
        m_protokol.zarejestruj_wyslanie_pakietu();
    }
}

void network_hub::transmituj_komende(uint32_t nr, const communication_protocol::dane_sterujace& pakiet) {
    if (m_gniazdo && m_gniazdo->isOpen()) {
        QByteArray dane = m_protokol.stworz_wiadomosc_sterujaca(nr, pakiet);
        m_gniazdo->write(dane);
        m_protokol.zarejestruj_wyslanie_pakietu();
    }
}
