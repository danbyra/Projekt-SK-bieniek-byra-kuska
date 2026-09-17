#include "test_symulacja.h"
#include "GenSyg.h"
#include "RegulatorPID.h"
#include "ModelARX.h"
#include "ProstyUAR.h"
#include "Centrala.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

namespace TESTY_LogikaSymulacji
{
    // porownaj czy liczby sa prawie takie same
    static bool czyBlisko(double a, double b, double tol = 1e-3)
    {
        return std::abs(a - b) <= tol;
    }

    bool test_1_generator_prostokat_wartosci_wykresu()
    {
        try
        {
            // 5 razy gora 3.0, potem 5 razy dol 1.0
            GenSyg gen(GenSyg::TypSyg::Prostokat, 10.0, 1000, 0.5, 2.0, 1.0);
            gen.setLicznikProbki(0);

            std::vector<double> spodziewane = { 3.0, 3.0, 3.0, 3.0, 3.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
            std::vector<double> faktyczne(10);

            for (size_t i = 0; i < 10; ++i)
            {
                faktyczne[i] = gen.generuj();
            }

            for (size_t i = 0; i < 10; ++i)
            {
                if (!czyBlisko(spodziewane[i], faktyczne[i]))
                {
                    std::cerr << "FAIL! [TEST 1] Generator prostokatny -> wartosci wykresu w(k): Mismatch w kroku "
                        << i << ": oczekiwano " << spodziewane[i] << ", otrzymano " << faktyczne[i] << "\n";
                    return false;
                }
            }
            return true;
        }
        catch (...)
        {
            std::cerr << "FAIL! [TEST 1] Generator prostokatny -> niespodziewany wyjatek!\n";
            return false;
        }
    }

    bool test_2_generator_sinusoida_faza_i_wartosci()
    {
        try
        {
            // sprawdza czy sinus leci po dobrych punktach
            GenSyg gen(GenSyg::TypSyg::Sinusoida, 8.0, 1000, 0.5, 5.0, 2.0);
            gen.setLicznikProbki(0);

            const double pierw2_pol = std::sqrt(2.0) / 2.0;
            std::vector<double> spodziewane = {
                2.0,
                5.0 * pierw2_pol + 2.0,
                7.0,
                5.0 * pierw2_pol + 2.0,
                2.0,
                5.0 * (-pierw2_pol) + 2.0,
                -3.0,
                5.0 * (-pierw2_pol) + 2.0
            };

            std::vector<double> faktyczne(8);
            for (size_t i = 0; i < 8; ++i)
            {
                faktyczne[i] = gen.generuj();
            }

            for (size_t i = 0; i < 8; ++i)
            {
                if (!czyBlisko(spodziewane[i], faktyczne[i]))
                {
                    std::cerr << "FAIL! [TEST 2] Generator sinusoidalny -> faza i punkty wezlowe: Mismatch w probce "
                        << i << ": oczekiwano " << spodziewane[i] << ", otrzymano " << faktyczne[i] << "\n";
                    return false;
                }
            }
            return true;
        }
        catch (...)
        {
            std::cerr << "FAIL! [TEST 2] Generator sinusoidalny -> niespodziewany wyjatek!\n";
            return false;
        }
    }

    bool test_3_pid_skladniki_sterowania_p_i_d()
    {
        try
        {
            // zwykly pid
            RegulatorPID pid(2.0, 4.0, 0.5);

            // krok 1: wchodzi 2, ma wyjsc 5.5
            double u1 = pid.symuluj(2.0);
            if (!czyBlisko(pid.get_uP(), 4.0) ||
                !czyBlisko(pid.get_uI(), 0.5) ||
                !czyBlisko(pid.get_uD(), 1.0) ||
                !czyBlisko(u1, 5.5))
            {
                std::cerr << "FAIL! [TEST 3] Regulator PID -> skladowe sterowania: Krok 1: uP=" << pid.get_uP()
                    << " (oczekiwano 4.0), uI=" << pid.get_uI() << " (oczekiwano 0.5), uD=" << pid.get_uD()
                    << " (oczekiwano 1.0), u=" << u1 << " (oczekiwano 5.5)\n";
                return false;
            }

            // krok 2: wchodzi 1, ma wyjsc 2.25
            double u2 = pid.symuluj(1.0);
            if (!czyBlisko(pid.get_uP(), 2.0) ||
                !czyBlisko(pid.get_uI(), 0.75) ||
                !czyBlisko(pid.get_uD(), -0.5) ||
                !czyBlisko(u2, 2.25))
            {
                std::cerr << "FAIL! [TEST 3] Regulator PID -> skladowe sterowania: Krok 2: uP=" << pid.get_uP()
                    << " (oczekiwano 2.0), uI=" << pid.get_uI() << " (oczekiwano 0.75), uD=" << pid.get_uD()
                    << " (oczekiwano -0.5), u=" << u2 << " (oczekiwano 2.25)\n";
                return false;
            }

            return true;
        }
        catch (...)
        {
            std::cerr << "FAIL! [TEST 3] Regulator PID -> skladowe sterowania: niespodziewany wyjatek!\n";
            return false;
        }
    }

    bool test_4_pid_nasycenie_ograniczenia_u()
    {
        try
        {
            RegulatorPID pid(10.0, 0.0, 0.0);
            pid.set_saOgraniczenia(true);
            pid.set_uMinMax(-5.0, 5.0);

            // za duzo na plus, ma uciac do 5
            double u_dodatnie = pid.symuluj(2.0);
            if (!czyBlisko(u_dodatnie, 5.0))
            {
                std::cerr << "FAIL! [TEST 4] Regulator PID -> nasycenie: dla e=2.0 otrzymano u="
                    << u_dodatnie << " (oczekiwano 5.0)\n";
                return false;
            }

            // za duzo na minus, ma uciac do -5
            double u_ujemne = pid.symuluj(-2.0);
            if (!czyBlisko(u_ujemne, -5.0))
            {
                std::cerr << "FAIL! [TEST 4] Regulator PID -> nasycenie: dla e=-2.0 otrzymano u="
                    << u_ujemne << " (oczekiwano -5.0)\n";
                return false;
            }

            // miesci sie w limicie, ma byc 3
            double u_liniowe = pid.symuluj(0.3);
            if (!czyBlisko(u_liniowe, 3.0))
            {
                std::cerr << "FAIL! [TEST 4] Regulator PID -> nasycenie: dla e=0.3 otrzymano u="
                    << u_liniowe << " (oczekiwano 3.0)\n";
                return false;
            }

            return true;
        }
        catch (...)
        {
            std::cerr << "FAIL! [TEST 4] Regulator PID -> nasycenie: niespodziewany wyjatek!\n";
            return false;
        }
    }

    bool test_5_pid_bumpless_transfer_dostosuj_calke()
    {
        try
        {
            RegulatorPID pid(2.0, 5.0, 0.5);

            // ma ustawic calke tak zeby wyszlo 4
            pid.dostosujCalkeDoU(4.0, 1.5);

            if (!czyBlisko(pid.get_uP(), 3.0) || !czyBlisko(pid.get_uI(), 1.0))
            {
                std::cerr << "FAIL! [TEST 5] Regulator PID -> bumpless transfer: Blad inicjalizacji: uP="
                    << pid.get_uP() << " (oczekiwano 3.0), uI=" << pid.get_uI() << " (oczekiwano 1.0)\n";
                return false;
            }

            // ma nie skakac do zera tylko plynnie dac 4.3
            double u_kolejne = pid.symuluj(1.5);
            if (!czyBlisko(u_kolejne, 4.3))
            {
                std::cerr << "FAIL! [TEST 5] Regulator PID -> bumpless transfer: Krok nastepny: u="
                    << u_kolejne << " (oczekiwano 4.3)\n";
                return false;
            }

            return true;
        }
        catch (...)
        {
            std::cerr << "FAIL! [TEST 5] Regulator PID -> bumpless transfer: niespodziewany wyjatek!\n";
            return false;
        }
    }

    bool test_6_arx_opoznienie_transportowe_i_wielomiany()
    {
        try
        {
            // arx z opoznieniem 2
            ModelARX arx({ -0.5 }, { 0.8 }, 2, 0.0);

            // na poczatku zera bo jeszcze czeka
            double y0 = arx.symuluj(0.0);
            double y1 = arx.symuluj(1.0);
            double y2 = arx.symuluj(1.0);
            // tu w koncu cos wychodzi
            double y3 = arx.symuluj(1.0);
            double y4 = arx.symuluj(1.0);
            double y5 = arx.symuluj(1.0);

            if (!czyBlisko(y0, 0.0) || !czyBlisko(y1, 0.0) || !czyBlisko(y2, 0.0) ||
                !czyBlisko(y3, 0.8) || !czyBlisko(y4, 1.2) || !czyBlisko(y5, 1.4))
            {
                std::cerr << "FAIL! [TEST 6] Model ARX -> opoznienie transportowe k=2: Otrzymano sekwencje ["
                    << y0 << ", " << y1 << ", " << y2 << ", " << y3 << ", " << y4 << ", " << y5
                    << "] (oczekiwano [0.0, 0.0, 0.0, 0.8, 1.2, 1.4])\n";
                return false;
            }

            return true;
        }
        catch (...)
        {
            std::cerr << "FAIL! [TEST 6] Model ARX -> opoznienie transportowe: niespodziewany wyjatek!\n";
            return false;
        }
    }

    bool test_7_arx_ograniczenia_sterowania_i_regulacji()
    {
        try
        {
            ModelARX arx({ -0.5 }, { 1.0 }, 1, 0.0);
            arx.setWartoscSterowania(-2.0, 2.0);
            arx.setWartoscRegulowania(-2.5, 2.5);
            arx.wlaczSterowanie(true);
            arx.wlaczRegulacje(true);

            // jeszcze czeka, ma byc 0
            double y1 = arx.symuluj(50.0);
            if (!czyBlisko(y1, 0.0))
            {
                std::cerr << "FAIL! [TEST 7] Model ARX -> nasycenie: Krok 1 (oT=1): y1=" << y1
                    << " (oczekiwano 0.0)\n";
                return false;
            }

            // wejscie uciete do 2
            double y2 = arx.symuluj(50.0);
            if (!czyBlisko(y2, 2.0))
            {
                std::cerr << "FAIL! [TEST 7] Model ARX -> nasycenie: Krok 2 (wejscie): y2=" << y2
                    << " (oczekiwano 2.0)\n";
                return false;
            }

            // wyjscie uciete do max 2.5
            double y3 = arx.symuluj(50.0);
            if (!czyBlisko(y3, 2.5))
            {
                std::cerr << "FAIL! [TEST 7] Model ARX -> nasycenie: Krok 3 (nasycenie wyjscia +): y3="
                    << y3 << " (oczekiwano 2.5)\n";
                return false;
            }

            // spadek w dol na maksa
            arx.symuluj(-50.0);
            arx.symuluj(-50.0);
            arx.symuluj(-50.0);
            // wyjscie uciete do min -2.5
            double y7 = arx.symuluj(-50.0);
            if (!czyBlisko(y7, -2.5))
            {
                std::cerr << "FAIL! [TEST 7] Model ARX -> nasycenie: Krok 7 (nasycenie wyjscia -): y7="
                    << y7 << " (oczekiwano -2.5)\n";
                return false;
            }

            return true;
        }
        catch (...)
        {
            std::cerr << "FAIL! [TEST 7] Model ARX -> nasycenie: niespodziewany wyjatek!\n";
            return false;
        }
    }

    bool test_8_prosty_uar_pelna_petla_uchyb_i_wykresy()
    {
        try
        {
            ModelARX arx({ -0.4 }, { 0.6 }, 1, 0.0);
            RegulatorPID pid(0.5, 5.0, 0.2);
            ProstyUAR uar(arx, pid);

            // krok 1: start ze skokiem 1
            double y1 = uar.symuluj(1.0);

            if (!czyBlisko(uar.getUchyb(), 1.0) ||
                !czyBlisko(uar.getSkladowaP(), 0.5) ||
                !czyBlisko(uar.getSkladowaI(), 0.2) ||
                !czyBlisko(uar.getSkladowaD(), 0.2) ||
                !czyBlisko(uar.getSterowania(), 0.9) ||
                !czyBlisko(y1, 0.0))
            {
                std::cerr << "FAIL! [TEST 8] ProstyUAR -> petla zamknieta: Krok 1: e=" << uar.getUchyb()
                    << " (oczekiwano 1.0), u=" << uar.getSterowania() << " (oczekiwano 0.9), y="
                    << y1 << " (oczekiwano 0.0)\n";
                return false;
            }

            // krok 2: drugi krok
            double y2 = uar.symuluj(1.0);

            if (!czyBlisko(uar.getUchyb(), 1.0) ||
                !czyBlisko(uar.getSkladowaP(), 0.5) ||
                !czyBlisko(uar.getSkladowaI(), 0.4) ||
                !czyBlisko(uar.getSkladowaD(), 0.0) ||
                !czyBlisko(uar.getSterowania(), 0.9) ||
                !czyBlisko(y2, 0.54))
            {
                std::cerr << "FAIL! [TEST 8] ProstyUAR -> petla zamknieta: Krok 2: e=" << uar.getUchyb()
                    << " (oczekiwano 1.0), u=" << uar.getSterowania() << " (oczekiwano 0.9), y="
                    << y2 << " (oczekiwano 0.54)\n";
                return false;
            }

            // krok 3: trzeci krok
            double y3 = uar.symuluj(1.0);

            if (!czyBlisko(uar.getUchyb(), 0.46) ||
                !czyBlisko(uar.getSkladowaP(), 0.23) ||
                !czyBlisko(uar.getSkladowaI(), 0.492) ||
                !czyBlisko(uar.getSkladowaD(), -0.108) ||
                !czyBlisko(uar.getSterowania(), 0.614) ||
                !czyBlisko(y3, 0.756))
            {
                std::cerr << "FAIL! [TEST 8] ProstyUAR -> petla zamknieta: Krok 3: e=" << uar.getUchyb()
                    << " (oczekiwano 0.46), u=" << uar.getSterowania() << " (oczekiwano 0.614), y="
                    << y3 << " (oczekiwano 0.756)\n";
                return false;
            }

            // krok 4: czwarty krok
            double y4 = uar.symuluj(1.0);

            if (!czyBlisko(uar.getUchyb(), 0.244) ||
                !czyBlisko(uar.getSterowania(), 0.6196) ||
                !czyBlisko(y4, 0.6708))
            {
                std::cerr << "FAIL! [TEST 8] ProstyUAR -> petla zamknieta: Krok 4: e=" << uar.getUchyb()
                    << " (oczekiwano 0.244), u=" << uar.getSterowania() << " (oczekiwano 0.6196), y="
                    << y4 << " (oczekiwano 0.6708)\n";
                return false;
            }

            // czy u to po prostu P dodac I dodac D
            double sumaSkladowych = uar.getSkladowaP() + uar.getSkladowaI() + uar.getSkladowaD();
            if (!czyBlisko(uar.getSterowania(), sumaSkladowych))
            {
                std::cerr << "FAIL! [TEST 8] ProstyUAR -> niespojnosc sumy skladowych: u=" << uar.getSterowania()
                    << " != P+I+D=" << sumaSkladowych << "\n";
                return false;
            }

            return true;
        }
        catch (...)
        {
            std::cerr << "FAIL! [TEST 8] ProstyUAR -> niespodziewany wyjatek!\n";
            return false;
        }
    }

    bool test_9_centrala_reset_symulacji()
    {
        try
        {
            Centrala c;
            c.setTypGenerator(GenSyg::TypSyg::Prostokat);
            c.setAmplitudaGenerator(2.0);
            c.setOkresGenerator(10.0);

            // pokrec troche zeby cos wpadlo
            for (int i = 0; i < 5; ++i)
            {
                c.wykonajKrokSymulacji();
            }

            // czyszczenie do zera
            c.resetSymulacji();

            if (!czyBlisko(c.getAktualnaWartoscZadana(), 0.0) ||
                !czyBlisko(c.getAktualnaWartoscRegulowana(), 0.0) ||
                !czyBlisko(c.getAktualnyUchyb(), 0.0) ||
                !czyBlisko(c.getAktualneSterowanie(), 0.0) ||
                !czyBlisko(c.getSkladowaP(), 0.0) ||
                !czyBlisko(c.getSkladowaI(), 0.0) ||
                !czyBlisko(c.getSkladowaD(), 0.0))
            {
                std::cerr << "FAIL! [TEST 9] Centrala -> reset symulacji: Niewyczyszczone stany po resecie: w="
                    << c.getAktualnaWartoscZadana() << ", y=" << c.getAktualnaWartoscRegulowana()
                    << ", u=" << c.getAktualneSterowanie() << "\n";
                return false;
            }

            // czy rusza od nowa normalnie
            c.wykonajKrokSymulacji();
            if (c.getAktualnaWartoscZadana() == 0.0 && c.getAktualneSterowanie() == 0.0)
            {
                std::cerr << "FAIL! [TEST 9] Centrala -> reset symulacji: Generator nie wystartowal poprawnie po resecie\n";
                return false;
            }

            return true;
        }
        catch (...)
        {
            std::cerr << "FAIL! [TEST 9] Centrala -> reset symulacji: niespodziewany wyjatek!\n";
            return false;
        }
    }

    bool test_10_generator_zmiana_taktowania_ciaglosc_fazy()
    {
        try
        {
            GenSyg gen(GenSyg::TypSyg::Sinusoida, 10.0, 200, 0.5, 5.0, 0.0);
            gen.setLicznikProbki(0);

            // zrob kawalek wykresu
            double w_przed = 0.0;
            for (int i = 0; i < 12; ++i)
            {
                w_przed = gen.generuj();
            }

            // zmien zegar w locie
            gen.set_Tt_zachowujFaze(100);

            // wez kolejny punkt
            double w_po = gen.generuj();

            // ma nie byc wielkiego skoku
            if (std::abs(w_przed - w_po) > 0.5)
            {
                std::cerr << "FAIL! [TEST 10] Generator -> zmiana taktowania: Skok fazy przy zmianie Tt: w_przed="
                    << w_przed << ", w_po=" << w_po << "\n";
                return false;
            }

            return true;
        }
        catch (...)
        {
            std::cerr << "FAIL! [TEST 10] Generator -> zmiana taktowania: niespodziewany wyjatek!\n";
            return false;
        }
    }

    
    bool wykonaj_testy()
    {
        bool ok = true;

        if (!test_1_generator_prostokat_wartosci_wykresu()) ok = false;
        if (!test_2_generator_sinusoida_faza_i_wartosci()) ok = false;
        if (!test_3_pid_skladniki_sterowania_p_i_d()) ok = false;
        if (!test_4_pid_nasycenie_ograniczenia_u()) ok = false;
        if (!test_5_pid_bumpless_transfer_dostosuj_calke()) ok = false;
        if (!test_6_arx_opoznienie_transportowe_i_wielomiany()) ok = false;
        if (!test_7_arx_ograniczenia_sterowania_i_regulacji()) ok = false;
        if (!test_8_prosty_uar_pelna_petla_uchyb_i_wykresy()) ok = false;
        if (!test_9_centrala_reset_symulacji()) ok = false;
        if (!test_10_generator_zmiana_taktowania_ciaglosc_fazy()) ok = false;

        return ok;
    }
}