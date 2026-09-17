#pragma once

#include <vector>
#include <string>

namespace TESTY_LogikaSymulacji
{
    // uruchomienie testów
    bool wykonaj_testy();

    // 1. Weryfikacja wartości sygnału prostokątnego z generatora (amplituda, składowa stała, wypełnienie)
    bool test_1_generator_prostokat_wartosci_wykresu();

    // 2. Weryfikacja fazy i wartości sygnału sinusoidalnego w punktach węzłowych
    bool test_2_generator_sinusoida_faza_i_wartosci();

    // 3. Weryfikacja podziału sterowania na składowe P, I, D oraz ich sumy
    bool test_3_pid_skladniki_sterowania_p_i_d();

    // 4. Weryfikacja nasycenia i ograniczeń uMin / uMax w regulatorze PID
    bool test_4_pid_nasycenie_ograniczenia_u();

    // 5. Weryfikacja bezuderzeniowego przełączania (bumpless transfer / dostosujCalkeDoU)
    bool test_5_pid_bumpless_transfer_dostosuj_calke();

    // 6. Weryfikacja modelu ARX z opóźnieniem transportowym k > 1 i wielomianami A, B
    bool test_6_arx_opoznienie_transportowe_i_wielomiany();

    // 7. Weryfikacja nasycenia wejścia (minS, maxS) i wyjścia (minR, maxR) w modelu ARX
    bool test_7_arx_ograniczenia_sterowania_i_regulacji();

    // 8. Weryfikacja pętli sprzężenia zwrotnego ProstyUAR i spójności danych wykresowych (w, y, e, u, P, I, D)
    bool test_8_prosty_uar_pelna_petla_uchyb_i_wykresy();

    // 9. Weryfikacja resetu symulacji w Centrali (zerowanie stanów, buforów i generatora)
    bool test_9_centrala_reset_symulacji();

    // 10. Weryfikacja zachowania ciągłości fazy generatora przy zmianie interwału taktowania (brak skoku na wykresie)
    bool test_10_generator_zmiana_taktowania_ciaglosc_fazy();
}
