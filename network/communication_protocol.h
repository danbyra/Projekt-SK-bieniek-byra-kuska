#ifndef COMMUNICATION_PROTOCOL_H
#define COMMUNICATION_PROTOCOL_H

#include <QByteArray>
#include <QString>
#include <QDataStream>
#include <QTextStream>
#include <QElapsedTimer>
#include <QDateTime>
#include <QObject>
#include <QStringList>
#include <QIODevice>
#include <cstdint>

class communication_protocol : public QObject {
    Q_OBJECT

public:

    enum typ_wiadomosci {
        ramka_symulacji = 1,   //(sygnał sterujący, wartość regulowana, wartość zadana)
        komenda_sterujaca = 2, //(start, stop, reset, zmiana interwału)
        konfiguracja_uar = 3   //(parametry)
    };
   
    enum typ_serializacji {
        binarna, 
        tekstowa 
    };
    
    struct dane_symulacji {
        double sygnal_sterujacy;   // u
        double wartosc_regulowana; // y
        double wartosc_zadana;     // w
        double czas;               // t
    };
    
    struct dane_sterujace {
        QString polecenie; // START,STOP, RESET
        int interwal;      
        qint32 okno;
    };
   
    struct dane_konfiguracyjne {
        QString ustawienia_pid;       
        QString ustawienia_arx;
        QString ustawienia_generatora;

    };

    explicit communication_protocol(QObject* parent = nullptr);
    
    void ustaw_typ_serializacji(typ_serializacji typ);
    
    typ_serializacji podaj_typ_serializacji() const;

    //pakiet danych dla ramki symulacji
    QByteArray stworz_wiadomosc_symulacji(uint32_t numer_probki, const dane_symulacji& dane);

    //pakiet z poleceniem sterującym
    QByteArray stworz_wiadomosc_sterujaca(uint32_t numer_probki, const dane_sterujace& dane);

    //pakiet z ustawieniami całego przebiegu
    QByteArray stworz_wiadomosc_konfiguracyjna(uint32_t numer_probki, const dane_konfiguracyjne& dane);

    // główny parser strumienia TCP 
    void przetworz_strumien(QByteArray& surowe_dane);

    //! po wysłaniu dla synchronizacji
    void zarejestruj_wyslanie_pakietu();

    //! po odebraniu dla synchronizacji
    void zarejestruj_odebranie_pakietu();
    
    double podaj_pakiety_na_sekunde() const;
    
    
    bool czy_zsynchronizowane(uint32_t lokalny_numer_probki, uint32_t zdalny_numer_probki, uint32_t maksymalne_odchylenie = 3) const;

    // dla sprawdzenia opuznienia
    bool czy_nadaza(int interwal_symulacji_ms) const;

signals:
    // aktualizacja interwału
    void wydajnosc_zaktualizowana(double pakiety_na_sekunde);

    // sygnały z odebranymi strukturami danych
    void odebrano_symulacje(uint32_t numer_probki, communication_protocol::dane_symulacji dane);
    void odebrano_sterowanie(uint32_t numer_probki, communication_protocol::dane_sterujace dane);
    void odebrano_konfiguracje(uint32_t numer_probki, communication_protocol::dane_konfiguracyjne dane);

private:
    typ_serializacji aktualny_typ_serializacji;
    
    // pod pomiar wydajności
    QElapsedTimer stoper_wydajnosci; //precyzyjniejsze niż Timer czytelniejsze jednostki - nie zmieniać
    int liczba_pakietow;
    double aktualne_pps; // pps - pakiety na sekunde btw

    // taki long long ale QElapsedTimer go zwraca
    qint64 czas_ostatniego_pakietu_ms;

    // dodawanie nagłówka 
    QByteArray stworz_naglowek(typ_wiadomosci typ, uint32_t numer_probki, uint32_t rozmiar_danych);

    dane_symulacji parsuj_dane_symulacji(const QByteArray& dane);
    dane_sterujace parsuj_dane_sterujace(const QByteArray& dane);
    dane_konfiguracyjne parsuj_dane_konfiguracyjne(const QByteArray& dane);
};

#endif // COMMUNICATION_PROTOCOL_H
