#include "communication_protocol.h"

communication_protocol::communication_protocol(QObject *parent)
    : QObject(parent), aktualny_typ_serializacji(binarna), 
      liczba_pakietow(0), aktualne_pps(0.0), czas_ostatniego_pakietu_ms(0) {

  stoper_wydajnosci.start(); // QElapsedTimer
}

void communication_protocol::ustaw_typ_serializacji(typ_serializacji typ) {
  aktualny_typ_serializacji = typ;
}

communication_protocol::typ_serializacji
communication_protocol::podaj_typ_serializacji() const {
  return aktualny_typ_serializacji;
}

QByteArray communication_protocol::stworz_naglowek(typ_wiadomosci typ,
                                                  uint32_t numer_probki,
                                                  uint32_t rozmiar_danych) {
  QByteArray naglowek;
  QDataStream strumien(&naglowek, QIODevice::WriteOnly);
  strumien << (quint32)typ << rozmiar_danych << numer_probki;
  return naglowek;
}

QByteArray
communication_protocol::stworz_wiadomosc_symulacji(uint32_t numer_probki,
                                                  const dane_symulacji &dane) {
  QByteArray czyste_dane;
  if (aktualny_typ_serializacji == binarna) {
    QDataStream strumien(&czyste_dane, QIODevice::WriteOnly);
    strumien << dane.sygnal_sterujacy << dane.wartosc_regulowana
             << dane.wartosc_zadana << dane.czas;
  } else {
    QTextStream strumien(&czyste_dane, QIODevice::WriteOnly);
    strumien << dane.sygnal_sterujacy << ";" << dane.wartosc_regulowana << ";"
             << dane.wartosc_zadana << ";" << dane.czas;
    strumien.flush(); // trzeba zamknąć
  }

  // karzda wiadomość musi mieć nagłówek
  QByteArray naglowek =
      stworz_naglowek(ramka_symulacji, numer_probki, czyste_dane.size());
  return naglowek + czyste_dane;
}

communication_protocol::dane_symulacji
communication_protocol::parsuj_dane_symulacji(const QByteArray &dane) {
  dane_symulacji wynik_rozkodowania = {0.0, 0.0, 0.0, 0.0};
  if (aktualny_typ_serializacji == binarna) {
    QDataStream strumien(dane);
    strumien >> wynik_rozkodowania.sygnal_sterujacy >>
        wynik_rozkodowania.wartosc_regulowana >>
        wynik_rozkodowania.wartosc_zadana >>
        wynik_rozkodowania.czas;
  } else {
    QString zwykly_tekst = QString::fromUtf8(dane);
    QStringList wartosci = zwykly_tekst.split(
        ';'); 
    if (wartosci.size() >= 4) {
      wynik_rozkodowania.sygnal_sterujacy = wartosci[0].toDouble();
      wynik_rozkodowania.wartosc_regulowana = wartosci[1].toDouble();
      wynik_rozkodowania.wartosc_zadana = wartosci[2].toDouble();
      wynik_rozkodowania.czas = wartosci[3].toDouble();
    }
  }
  return wynik_rozkodowania;
}

// to samo dla danych sterujących - inna struktura
QByteArray communication_protocol::stworz_wiadomosc_sterujaca(uint32_t numer_probki,
                                                  const dane_sterujace &dane) {
  QByteArray czyste_dane;
  if (aktualny_typ_serializacji == binarna) {
    QDataStream strumien(&czyste_dane, QIODevice::WriteOnly);
    strumien << dane.polecenie << dane.interwal << dane.okno;
  } else {
    QTextStream strumien(&czyste_dane, QIODevice::WriteOnly);
    strumien << dane.polecenie << ";" << dane.interwal << ";" << dane.okno;
    strumien.flush();
  }

  QByteArray naglowek =
      stworz_naglowek(komenda_sterujaca, numer_probki, czyste_dane.size());
  return naglowek + czyste_dane;
}

// to samo dla danych sterujących - inna struktura
communication_protocol::dane_sterujace
communication_protocol::parsuj_dane_sterujace(const QByteArray &dane) {
  dane_sterujace wynik_rozkodowania;
  if (aktualny_typ_serializacji == binarna) {
    QDataStream strumien(dane);
    strumien >> wynik_rozkodowania.polecenie >> wynik_rozkodowania.interwal >> wynik_rozkodowania.okno;
  } else {
    QString zwykly_tekst = QString::fromUtf8(dane);
    QStringList wartosci = zwykly_tekst.split(';');
    if (wartosci.size() >= 2) {
      wynik_rozkodowania.polecenie = wartosci[0];
      wynik_rozkodowania.interwal = wartosci[1].toInt();
      if (wartosci.size() >= 3) {
        wynik_rozkodowania.okno = wartosci[2].toInt();
      } else {
        wynik_rozkodowania.okno = 0;
      }
    }
  }
  return wynik_rozkodowania;
}



QByteArray communication_protocol::stworz_wiadomosc_konfiguracyjna(
    uint32_t numer_probki, const dane_konfiguracyjne &dane) {
  QByteArray czyste_dane;
  if (aktualny_typ_serializacji == binarna) {
    QDataStream strumien(&czyste_dane, QIODevice::WriteOnly);
    strumien << dane.ustawienia_pid << dane.ustawienia_arx
             << dane.ustawienia_generatora;
  } else {
    QTextStream strumien(&czyste_dane, QIODevice::WriteOnly);
    // inny separator by sie nie miesszało - sajgon
    strumien << dane.ustawienia_pid << "|" << dane.ustawienia_arx << "|"
             << dane.ustawienia_generatora;
    strumien.flush();
  }

  QByteArray naglowek =
      stworz_naglowek(konfiguracja_uar, numer_probki, czyste_dane.size());
  return naglowek + czyste_dane;
}

communication_protocol::dane_konfiguracyjne
communication_protocol::parsuj_dane_konfiguracyjne(const QByteArray &dane) {
  dane_konfiguracyjne wynik_rozkodowania;
  if (aktualny_typ_serializacji == binarna) {
    QDataStream strumien(dane);
    strumien >> wynik_rozkodowania.ustawienia_pid >>
        wynik_rozkodowania.ustawienia_arx >>
        wynik_rozkodowania.ustawienia_generatora;
  } else {
    QString zwykly_tekst = QString::fromUtf8(dane);
    QStringList wartosci = zwykly_tekst.split('|');
    if (wartosci.size() >= 3) {
      wynik_rozkodowania.ustawienia_pid = wartosci[0];
      wynik_rozkodowania.ustawienia_arx = wartosci[1];
      wynik_rozkodowania.ustawienia_generatora = wartosci[2];
    }
  }
  return wynik_rozkodowania;
}

void communication_protocol::przetworz_strumien(QByteArray &surowe_dane) {
  while (!surowe_dane.isEmpty()) {
    int waga_naglowka_w_pamieci = 3 * sizeof(quint32);

    // Czekamy aż będziemy mieli pełny nagłówek binarny
    if (surowe_dane.size() < waga_naglowka_w_pamieci)
      return;

    QDataStream strumien(surowe_dane);
    quint32 odczytany_typ, odczytany_rozmiar, wyjsciowy_numer_probki;

    strumien >> odczytany_typ >> odczytany_rozmiar >> wyjsciowy_numer_probki;

    if (strumien.status() != QDataStream::Ok)
      return;

    int calkowityKosz = waga_naglowka_w_pamieci + (int)odczytany_rozmiar;

    // czekamy aż przyjdzie cała zawartość pakietu (nagłówek + ramka) - nie usuwać
    if (surowe_dane.size() < calkowityKosz)
      return;

    QByteArray wyjsciowe_dane =
        surowe_dane.mid(waga_naglowka_w_pamieci, odczytany_rozmiar);
    surowe_dane.remove(0, calkowityKosz);

    typ_wiadomosci wyjsciowy_typ = static_cast<typ_wiadomosci>(odczytany_typ);
    if (wyjsciowy_typ == ramka_symulacji) {
      emit odebrano_symulacje(wyjsciowy_numer_probki,
                              parsuj_dane_symulacji(wyjsciowe_dane));
    } else if (wyjsciowy_typ == komenda_sterujaca) {
      emit odebrano_sterowanie(wyjsciowy_numer_probki,
                               parsuj_dane_sterujace(wyjsciowe_dane));
    } else if (wyjsciowy_typ == konfiguracja_uar) {
      emit odebrano_konfiguracje(wyjsciowy_numer_probki,
                                 parsuj_dane_konfiguracyjne(wyjsciowe_dane));
    }
  }
}

void communication_protocol::zarejestruj_wyslanie_pakietu() {
  liczba_pakietow++;
  qint64 jak_dlugo_leci = stoper_wydajnosci.elapsed();

  // statystyki
  if (jak_dlugo_leci >= 1000) {
    aktualne_pps = (double)liczba_pakietow / (jak_dlugo_leci / 1000.0);
    liczba_pakietow = 0;
    stoper_wydajnosci.restart();

    emit wydajnosc_zaktualizowana(aktualne_pps);
  }
}

void communication_protocol::zarejestruj_odebranie_pakietu() {
  // dla nadzoru
  czas_ostatniego_pakietu_ms = QDateTime::currentMSecsSinceEpoch();
}

double communication_protocol::podaj_pakiety_na_sekunde() const {
  return aktualne_pps;
}

// tolerancja dla asynch
bool communication_protocol::czy_zsynchronizowane(
    uint32_t lokalny_numer_probki, uint32_t zdalny_numer_probki,
    uint32_t maksymalne_odchylenie) const {
  uint32_t rozstrzelenie = (lokalny_numer_probki > zdalny_numer_probki)
                               ? (lokalny_numer_probki - zdalny_numer_probki)
                               : (zdalny_numer_probki - lokalny_numer_probki);

  return rozstrzelenie <= maksymalne_odchylenie;
}


bool communication_protocol::czy_nadaza(int interwal_symulacji_ms) const {
  if (czas_ostatniego_pakietu_ms == 0)
    return true; // dla 1 pakietu -- i am dump

  qint64 obecnie_na_zegarze_ms = QDateTime::currentMSecsSinceEpoch();
  qint64 pustka_od_ostatniego_pakietu =
      obecnie_na_zegarze_ms - czas_ostatniego_pakietu_ms;

  // tolerujemy laga w przeciągu trzech kroków interwału - profilaktycznie pod wykresy GUI
  return pustka_od_ostatniego_pakietu <= (interwal_symulacji_ms * 3.0);
}
