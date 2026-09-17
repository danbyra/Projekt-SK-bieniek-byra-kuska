#include "ModelARX.h"
#include "ProstyUAR.h"
#include "RegulatorPID.h"
#include "include.h"
#include "network/Hub/network_hub.h"
#include <QCoreApplication>
#include <QTimer>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

class TESTY_ProstyUAR {
public:
  static void test_UAR_1_skokJednostkowyPID();
};

void TESTY_ProstyUAR::test_UAR_1_skokJednostkowyPID() {

  std::cerr << "UAR_1 PID -> test skoku jednostkowego: ";
  try {
    // Przygotowanie danych:
    RegulatorPID testPID(0.5, 5.0, 0.2);
    ModelARX testARX({-0.4}, {0.6});
    ProstyUAR instancjaTestowa(testARX, testPID);
    constexpr size_t LICZ_ITER = 30;
    std::vector<double> sygWe(LICZ_ITER); // pobudzenie modelu (tu same 0)
    std::vector<double> spodzSygWy(
        LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
    std::vector<double> faktSygWy(LICZ_ITER); // faktyczna sekwencja wy

    // Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
    for (int i = 0; i < LICZ_ITER; i++)
      sygWe[i] = !!i;
    spodzSygWy = {0.0,      0.0,      0.54,     0.756,    0.6708,   0.64008,
                  0.729,    0.810437, 0.834499, 0.843338, 0.8664,   0.8936,
                  0.911886, 0.923312, 0.93404,  0.944929, 0.954065, 0.961042,
                  0.966815, 0.971965, 0.97642,  0.980096, 0.983143, 0.985741,
                  0.987964, 0.989839, 0.991411, 0.992739, 0.993865, 0.994818};

    // Symulacja UAR:

    for (int i = 0; i < LICZ_ITER; i++)
      faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

    // Walidacja poprawności i raport:
    bool ok = true;
    for (size_t i = 0; i < spodzSygWy.size(); ++i) {
      if (std::abs(spodzSygWy[i] - faktSygWy[i]) > 1e-4) {
        ok = false;
        std::cerr << "FAILED! Mismatch at index " << i << ": expected "
                  << spodzSygWy[i] << ", actual " << faktSygWy[i] << "\n";
        exit(1);
      }
    }
    if (ok) {
      std::cerr << "SUCCESS!\n";
    }
  } catch (...) {
    std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
  }
}

class TestSieciowy : public QObject {
public:
  network_hub serwer;
  network_hub klient;

  ModelARX arx;
  RegulatorPID pid;

  std::vector<double> sygWe;
  std::vector<double> spodz;
  std::vector<double> fakt;

  int krok = 0;
  int iteracja = 0;
  int ok_count = 0;

  TestSieciowy() : arx({-0.4}, {0.6}), pid(0.5, 5.0, 0.2) {
    sygWe.resize(30);
    fakt.resize(30);
    for (int i = 0; i < 30; i++)
      sygWe[i] = (i > 0);
    spodz = {0.0,      0.0,      0.54,     0.756,    0.6708,   0.64008,
             0.729,    0.810437, 0.834499, 0.843338, 0.8664,   0.8936,
             0.911886, 0.923312, 0.93404,  0.944929, 0.954065, 0.961042,
             0.966815, 0.971965, 0.97642,  0.980096, 0.983143, 0.985741,
             0.987964, 0.989839, 0.991411, 0.992739, 0.993865, 0.994818};

    serwer.get_protokol().ustaw_typ_serializacji(
        communication_protocol::binarna);
    klient.get_protokol().ustaw_typ_serializacji(
        communication_protocol::binarna);

    QObject::connect(&klient, &network_hub::polaczono,
                     [this]() { odpal_iteracje(); });

    QObject::connect(
        &serwer, &network_hub::odebrano_symulacje,
        [this](uint32_t nr, communication_protocol::dane_symulacji d) {
          if (nr == 0)
            arx.ResetPamieci();
          double y = arx.symuluj(d.sygnal_sterujacy);
          communication_protocol::dane_symulacji odp;
          odp.sygnal_sterujacy = 0.0;
          odp.wartosc_regulowana = y;
          odp.wartosc_zadana = 0.0;
          serwer.transmituj_ramke(nr, odp);
        });

    QObject::connect(
        &klient, &network_hub::odebrano_symulacje,
        [this](uint32_t nr, communication_protocol::dane_symulacji d) {
          Q_UNUSED(nr);
          fakt[krok] = d.wartosc_regulowana;
          krok++;

          if (krok < 30) {
            double e = sygWe[krok] - d.wartosc_regulowana;
            double u = pid.symuluj(e);
            communication_protocol::dane_symulacji ramka;
            ramka.sygnal_sterujacy = u;
            ramka.wartosc_regulowana = 0.0;
            ramka.wartosc_zadana = sygWe[krok];
            klient.transmituj_ramke(krok, ramka);
          } else {
            bool jest_ok = true;
            for (int i = 0; i < 30; i++) {
              if (std::abs(spodz[i] - fakt[i]) > 1e-4) {
                jest_ok = false;
                break;
              }
            }

            iteracja++;
            if (jest_ok) {
              ok_count++;
              std::cout << "Test " << iteracja << ": SUCCESS\n" << std::flush;
            } else {
              std::cout << "Test " << iteracja << ": FAILED\n" << std::flush;
            }

            if (iteracja < 1000) {
              odpal_iteracje();
            } else {
              std::cout << "-------------------------------------------\n";
              std::cout << "TESTOW: " << iteracja << "\n";
              std::cout << "UDANE TESTY: " << ok_count << " / 1000\n";
              if (ok_count == 1000) {
                std::cout << "Wszystko poszlo dobrze\n" << std::flush;
                QCoreApplication::exit(0);
              } else {
                std::cerr << "Cos poszlo nie tak\n" << std::flush;
                QCoreApplication::exit(1);
              }
            }
          }
        });
  }

  void start() {
    quint16 port = 12499;
    serwer.uruchom_jako_serwer(port);
    klient.uruchom_jako_klient("127.0.0.1", port);
  }

  void odpal_iteracje() {
    pid.resetPamieci();
    krok = 0;
    double u = pid.symuluj(sygWe[0] - 0.0);
    communication_protocol::dane_symulacji ramka;
    ramka.sygnal_sterujacy = u;
    ramka.wartosc_regulowana = 0.0;
    ramka.wartosc_zadana = sygWe[0];
    klient.transmituj_ramke(0, ramka);
  }
};

#ifdef DEBUG
int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);



 
  TestSieciowy t;
  t.start();

 
  QTimer::singleShot(15000, []() {
    std::cerr << "TIMEOUT: Cos sie zawiesilo!\n" << std::flush;
    QCoreApplication::exit(1);
  });

  return app.exec();
}
#endif
