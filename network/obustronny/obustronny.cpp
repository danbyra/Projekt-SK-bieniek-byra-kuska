#include "obustronny.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <cstdint>
#include <algorithm>

ObustronnyManager::ObustronnyManager(MainWindow* mainWindow, QObject* parent)
    : QObject(parent)
    , m_mainWindow(mainWindow)
    , m_slaveTimer(new QTimer(this))
    , m_lastU(0.0)
    , m_lastW(0.0)
    , m_isEmergencyActive(false)
    , m_consecutiveLosses(0)
    , m_receivedResponseForCurrentTick(false)
    , m_t0(0.0)
    , m_lastReceivedPacketId(-1)
    , m_intervalJustChanged(false)
    , m_reconnectedJustNow(false)
    , m_isSimulationStarted(false)
{
    m_slaveTimer->setSingleShot(true);
    connect(m_slaveTimer, &QTimer::timeout, this, &ObustronnyManager::handleSlaveTimerTick);
}

ObustronnyManager::~ObustronnyManager()
{
    delete m_slaveTimer;
}

void ObustronnyManager::startSimulation()
{
    m_isSimulationStarted = true;
    m_consecutiveLosses = 0;
    m_receivedResponseForCurrentTick = false;
    m_isEmergencyActive = false;

    //blok ikon
    m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(false);
    m_mainWindow->ui->taktowanie_comboBox->setEnabled(false);

    if (m_mainWindow->ui->klient_radioButton->isChecked()) {
        if (m_mainWindow->nr_probki == 0 && m_t0 == 0.0) {
            // reset
            m_t0 = 0.0;
            m_mainWindow->nr_probki = 0;
            m_mainWindow->ostatni_numer_sieciowy = 0;
            m_mainWindow->m_czasSieciowy = 0.0;
            m_mainWindow->m_ostatniaProbkaSieciowa = 0;
            m_lastReceivedPacketId = -1;
            m_masterBuffer.clear();

            m_mainWindow->m_centralaSieciowa.getGenerator().ResetPamieci();
            m_mainWindow->m_centralaSieciowa.getProstyUAR().ResetStanow();


        } else {
            // Odpauzowanie
            m_masterBuffer.clear();
            m_lastReceivedPacketId = static_cast<int64_t>(m_mainWindow->nr_probki) - 1;

        }

        m_mainWindow->PID_wysyl();
        m_mainWindow->Gen_wysyl();
        m_mainWindow->Okno_wysyl();
        m_mainWindow->Komenda_wysyl("START");
    } else if (m_mainWindow->ui->server_radioButton->isChecked()) {
        m_slaveTimer->stop(); // stop timer
        qDebug() << "[Obiekt] Uruchomiono symulację. Oczekiwanie na pierwszą ramkę do synchronizacji timera...";
        //blok ikon
        m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(false);
        m_mainWindow->ui->taktowanie_comboBox->setEnabled(false);
    }
}

void ObustronnyManager::stopSimulation()
{
    m_isSimulationStarted = false;
    if (m_mainWindow->ui->klient_radioButton->isChecked()) {
        m_mainWindow->Komenda_wysyl("STOP");
    }
    m_slaveTimer->stop(); // stop bezwarunkowo
    m_isEmergencyActive = false;

    //ublok ikon
    m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(true);
    m_mainWindow->ui->taktowanie_comboBox->setEnabled(true);
}

void ObustronnyManager::resetSimulation()
{
    m_isSimulationStarted = false;
    m_isEmergencyActive = false;
    m_lastU = 0.0;
    m_lastW = 0.0;
    m_consecutiveLosses = 0;
    m_receivedResponseForCurrentTick = false;
    m_slaveBuffer.clear();
    m_masterBuffer.clear();
    m_t0 = 0.0;
    m_lastReceivedPacketId = -1;
    //ublok ikon
    m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(true);
    m_mainWindow->ui->taktowanie_comboBox->setEnabled(true);

    if (m_mainWindow->ui->klient_radioButton->isChecked()) {
        m_mainWindow->m_centralaSieciowa.getGenerator().ResetPamieci();
        m_mainWindow->m_centralaSieciowa.getProstyUAR().ResetStanow();
        m_mainWindow->Komenda_wysyl("RESET");
    }
    m_slaveTimer->stop(); // stop bezwarunkowo
}

void ObustronnyManager::handleStartCommand(double t0, uint32_t numer)
{
    // Zabezpieczenie przed opóźnionym duplikatem START- dla dzialałacej symulacji
    if (m_slaveTimer->isActive() && m_mainWindow->nr_probki > 10) {
        qDebug() << "handleStartCommand: ignorowano duplikat START (Obiekt aktywny przy nr_probki="
                 << m_mainWindow->nr_probki << ")";
        return;
    }

    m_isSimulationStarted = true;
    m_isEmergencyActive = false;

    m_slaveTimer->stop(); // Stop timer zostanie zsynchronizowany i uruchomiony przy pierwszej ramce
    qDebug() << "[Obiekt] Odebrano START. Oczekiwanie na pierwszą ramkę do synchronizacji timera...";

    if (m_mainWindow->nr_probki > 0 || m_t0 > 0.0) {
        // Odpauzowanie
        qDebug() << "Odebrano polecenie: START (Odpauzowanie). Synchronizacja nr_probki =" << numer << "t0 =" << t0;
        m_mainWindow->nr_probki = numer;
        m_mainWindow->ostatni_numer_sieciowy = numer;
        m_mainWindow->m_czasSieciowy = t0;
        double T = m_mainWindow->m_centralaSieciowa.getTaktowanie() / 1000.0;
        if (T <= 0.0) T = 0.1;
        m_t0 = t0 - (numer * T);
        m_mainWindow->m_centralaSieciowa.set_Czas(t0);
        m_lastReceivedPacketId = static_cast<int64_t>(numer) - 1;
        m_slaveBuffer.clear();
        m_masterBuffer.clear();
        //block ikoin
        m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(false);
        m_mainWindow->ui->taktowanie_comboBox->setEnabled(false);
        return;
    }

    m_slaveBuffer.clear();
    m_masterBuffer.clear();
    m_t0 = t0;
    m_mainWindow->nr_probki = 0;
    m_mainWindow->ostatni_numer_sieciowy = 0;
    m_mainWindow->m_czasSieciowy = t0;
    m_mainWindow->m_ostatniaProbkaSieciowa = 0;
    m_lastReceivedPacketId = -1;
    m_lastU = 0.0;
    m_lastW = 0.0;

    //!Reset generatora i PID (bez kasowania pamięci modelu ARX obiektu dla bezuderzeniowości)
    m_mainWindow->m_centralaSieciowa.getGenerator().ResetPamieci();
    m_mainWindow->m_centralaSieciowa.resetPID();

    qDebug() << "Odebrano polecenie: START (Nowy start). Zresetowano parametry, oczekiwanie na pierwszą ramkę...";

    //blok ikon
    m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(false);
    m_mainWindow->ui->taktowanie_comboBox->setEnabled(false);

}

void ObustronnyManager::handleStopCommand()
{
    m_isSimulationStarted = false;
    m_slaveTimer->stop();
    m_isEmergencyActive = false;
    qDebug() << "Odebrano polecenie: STOP. Zatrzymano taktowanie.";

    //unblok ikon
    m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(true);
    m_mainWindow->ui->taktowanie_comboBox->setEnabled(true);
}

void ObustronnyManager::handleResetCommand()
{
    m_isSimulationStarted = false;
    m_slaveTimer->stop();
    m_isEmergencyActive = false;
    m_slaveBuffer.clear();
    m_masterBuffer.clear();
    m_lastU = 0.0;
    m_lastW = 0.0;
    m_t0 = 0.0;
    m_lastReceivedPacketId = -1;
    m_mainWindow->nr_probki = 0;
    m_mainWindow->ostatni_numer_sieciowy = 0;
    m_mainWindow->m_czasSieciowy = 0.0;
    m_mainWindow->m_ostatniaProbkaSieciowa = 0;
    m_mainWindow->wyczyscWykresy();
    qDebug() << "Odebrano polecenie: RESET. Przywrócono stan początkowy.";
    //unblok ikon
    m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(true);
    m_mainWindow->ui->taktowanie_comboBox->setEnabled(true);
}

void ObustronnyManager::handleIntervalCommand(int newInterval, int newWindow, double t0)
{
    Q_UNUSED(newWindow);

    if (m_mainWindow->ui->klient_radioButton->isChecked()) {
        m_mainWindow->m_centralaSieciowa.setTaktowanie(newInterval);
        m_masterBuffer.clear();
        double T = newInterval / 1000.0;
        if (T <= 0.0) T = 0.1;
        m_t0 = m_mainWindow->m_czasSieciowy - (m_mainWindow->nr_probki * T);
        m_lastReceivedPacketId = -1;
        m_intervalJustChanged = true;

        qDebug() << "[Regulator] Zmiana interwału:" << newInterval
                 << "ms. Zachowano nr_probki =" << m_mainWindow->nr_probki << ", m_t0 =" << m_t0;

    } else if (m_mainWindow->ui->server_radioButton->isChecked()) {
        m_mainWindow->m_centralaSieciowa.setTaktowanie(newInterval);
        double T = newInterval / 1000.0;
        if (T <= 0.0) T = 0.1;
        m_t0 = m_mainWindow->m_czasSieciowy - (m_mainWindow->nr_probki * T);
        m_lastReceivedPacketId = -1;
        m_isEmergencyActive = false;

        // Zachowane ostatnio odebrane u i w
        m_lastU = m_mainWindow->m_centralaSieciowa.getAktualneSterowanie();
        m_lastW = m_mainWindow->m_centralaSieciowa.getAktualnaWartoscZadana();

        m_slaveBuffer.clear();
        m_slaveTimer->stop();
        qDebug() << "[Obiekt] Zmiana interwału do" << newInterval << "ms. Zachowano nr_probki =" << m_mainWindow->nr_probki << ", t0 =" << m_t0;
    }
}

void ObustronnyManager::handleMasterTimerTimeout()
{
    if (m_mainWindow->ui->comboBox->currentText() != "Sieciowy") return;
    if (m_mainWindow->ui->taktowanie_comboBox->currentText() != "Obustronne") return;
    if (!m_mainWindow->ui->klient_radioButton->isChecked()) return;
    if (!m_isSimulationStarted) return;
    // Block takt gdy połączenie jest utracone
    if (m_isEmergencyActive) return;

    // Regulator aktywny krok symulacji
    if (m_mainWindow->nr_probki > 0 && !m_receivedResponseForCurrentTick) {
        m_consecutiveLosses++;
        m_mainWindow->ui->label_sync_status->setText(QString("Opóźnienie / Utrata (%1)").arg(m_consecutiveLosses));
        m_mainWindow->ui->label_sync_status->setStyleSheet("color: red; font-weight: bold;");
    } else {
        m_mainWindow->ui->label_sync_status->setText("Synchronizacja OK");
        m_mainWindow->ui->label_sync_status->setStyleSheet("color: green; font-weight: bold;");
        m_consecutiveLosses = 0;
    }

    m_receivedResponseForCurrentTick = false;

    // Czas bezwzględny na podstawie licznika próbek  eliminujebłąd obliczeń i timera
    double T = m_mainWindow->m_centralaSieciowa.getTaktowanie() / 1000.0;
    double t = m_t0 + (m_mainWindow->nr_probki * T);
    m_mainWindow->m_czasSieciowy = t;
    m_mainWindow->m_centralaSieciowa.set_Czas(t);

    m_mainWindow->m_centralaSieciowa.wykonajKrokKlienta();
    double u = m_mainWindow->m_centralaSieciowa.getAktualneSterowanie();

    // Zapis stanu ramki sterującej wyliczonej przez Regulatora dla próbki
    MasterFrame mf;
    mf.packet_id = m_mainWindow->nr_probki;
    mf.w = m_mainWindow->m_centralaSieciowa.getAktualnaWartoscZadana();
    mf.u = u;
    mf.uP = m_mainWindow->m_centralaSieciowa.getSkladowaP();
    mf.uI = m_mainWindow->m_centralaSieciowa.getSkladowaI();
    mf.uD = m_mainWindow->m_centralaSieciowa.getSkladowaD();
    mf.e = m_mainWindow->m_centralaSieciowa.getAktualnyUchyb();
    m_masterBuffer.insert(mf.packet_id, mf);

    m_mainWindow->wyslij_sterowanie_przez_siec(u);

    m_mainWindow->nr_probki++;
}

void ObustronnyManager::handleSlaveTimerTick()
{
    if (m_mainWindow->ui->comboBox->currentText() != "Sieciowy") {
        m_slaveTimer->stop();
        return;
    }
    if (!m_mainWindow->ui->server_radioButton->isChecked()) return;
    if (!m_isSimulationStarted) {
        m_slaveTimer->stop();
        return;
    }

    if (m_slaveBuffer.isEmpty()) {
        // Bufor jest pusty -> do 1 krok awaryjny
        m_mainWindow->m_centralaSieciowa.odbierzU(m_lastU);
        m_mainWindow->m_centralaSieciowa.odbierzW(m_lastW);

        if (!m_isEmergencyActive) {
            m_isEmergencyActive = true;
            qDebug() << "AWARIA: Brak danych na czas taktu (pusty bufor, oczekiwany ID:" << m_mainWindow->nr_probki << "). Rysowanie płaskiej linii.";
        }

        double T = m_mainWindow->m_centralaSieciowa.getTaktowanie() / 1000.0;
        double t = m_t0 + (m_mainWindow->nr_probki * T);
        m_mainWindow->m_czasSieciowy = t;
        m_mainWindow->m_centralaSieciowa.set_Czas(m_mainWindow->m_czasSieciowy);

        double y_prev = m_mainWindow->m_centralaSieciowa.getAktualnaWartoscRegulowana();
        m_mainWindow->m_centralaSieciowa.getProstyUAR().aktualizujTloPID(m_lastW, y_prev);

        m_mainWindow->m_centralaSieciowa.wykonajKrokSerwera();
        int currentLicz = m_mainWindow->m_centralaSieciowa.getGenerator().getLicznikProbki();
        m_mainWindow->m_centralaSieciowa.getGenerator().setLicznikProbki(currentLicz + 1);

        double y = m_mainWindow->m_centralaSieciowa.getAktualnaWartoscRegulowana();
        double u_slave = m_mainWindow->m_centralaSieciowa.getAktualneSterowanie();
        m_mainWindow->m_hub.wyslij_ramke(m_mainWindow->nr_probki, u_slave, y, m_lastW, m_mainWindow->m_czasSieciowy);
        m_mainWindow->m_centralaSieciowa.emitujWykres();
        m_mainWindow->nr_probki++;
    } else {
        // Bufor nie jest pusty -> procesuj zawartość bufora
        while (m_mainWindow->ui->server_radioButton->isChecked() && !m_slaveBuffer.isEmpty()) {

            uint32_t expectedId = m_mainWindow->nr_probki;
            int foundIndex = -1;

            //Przeszukanie buffera po pakiet
            for (int i = 0; i < m_slaveBuffer.size(); ++i) {
                if (m_slaveBuffer[i].packet_id == expectedId) {
                    foundIndex = i;
                    break;
                }
            }

            if (foundIndex != -1) {

                BufferedFrame bf = m_slaveBuffer[foundIndex];
                m_lastU = bf.dane.sygnal_sterujacy;
                m_lastW = bf.dane.wartosc_zadana;

                m_mainWindow->m_centralaSieciowa.odbierzU(m_lastU);
                m_mainWindow->m_centralaSieciowa.odbierzW(m_lastW);

                for (int i = 0; i <= foundIndex; ++i) {
                    m_slaveBuffer.removeFirst();
                }

                if (m_isEmergencyActive) {
                    m_isEmergencyActive = false;
                    qDebug() << "Połączenie przywrócone - powrót do normalnego taktowania.";
                }
            } else {
                // dla brakujacych ramek-wez ostatnia
                m_mainWindow->m_centralaSieciowa.odbierzU(m_lastU);
                m_mainWindow->m_centralaSieciowa.odbierzW(m_lastW);

                if (!m_isEmergencyActive) {
                    m_isEmergencyActive = true;
                    qDebug() << "AWARIA: Brak danych na czas taktu (oczekiwany ID:" << expectedId << "). Rysowanie płaskiej linii.";
                }
            }


            double T = m_mainWindow->m_centralaSieciowa.getTaktowanie() / 1000.0;
            double t = m_t0 + (m_mainWindow->nr_probki * T);
            m_mainWindow->m_czasSieciowy = t;
            m_mainWindow->m_centralaSieciowa.set_Czas(m_mainWindow->m_czasSieciowy);

            // Aktualizacja PID na serwerze przed wyliczeniem nowego y_k
            double y_prev = m_mainWindow->m_centralaSieciowa.getAktualnaWartoscRegulowana();
            m_mainWindow->m_centralaSieciowa.getProstyUAR().aktualizujTloPID(m_lastW, y_prev);

            m_mainWindow->m_centralaSieciowa.wykonajKrokSerwera();
            int currentLicz = m_mainWindow->m_centralaSieciowa.getGenerator().getLicznikProbki();
            m_mainWindow->m_centralaSieciowa.getGenerator().setLicznikProbki(currentLicz + 1);

            double y = m_mainWindow->m_centralaSieciowa.getAktualnaWartoscRegulowana();

            // Wysył y && m_lastW
            double u_slave = m_mainWindow->m_centralaSieciowa.getAktualneSterowanie();
            m_mainWindow->m_hub.wyslij_ramke(m_mainWindow->nr_probki, u_slave, y, m_lastW, m_mainWindow->m_czasSieciowy);
            m_mainWindow->m_centralaSieciowa.emitujWykres();
            m_mainWindow->nr_probki++;

            //brak oczekiwanej ramki, przerwij pętlę i zaczekaj na kolejny takt
            if (foundIndex == -1) break;
        }
    }

    // Uruchomienie timera jako backup dla kolejnego taktu
    if (m_isSimulationStarted && m_mainWindow->ui->server_radioButton->isChecked()) {
        int interwal = m_mainWindow->m_centralaSieciowa.getTaktowanie();
        if (interwal <= 0) interwal = 100;

        if (m_isEmergencyActive) {
            // W trybie awaryjnym (brak sieci)
            m_slaveTimer->start(interwal);
        } else {
            // większy margines na przybycie kolejnego pakietu przed wył awarii
            m_slaveTimer->start(std::max(1000, interwal * 4));
        }
    }
}

void ObustronnyManager::handleOdebranoSymulacje(uint32_t numer, const communication_protocol::dane_symulacji& dane)
{
    if (m_mainWindow->ui->taktowanie_comboBox->currentText() != "Obustronne") return;

    if (m_mainWindow->ui->klient_radioButton->isChecked()) {

        int interwal = m_mainWindow->m_centralaSieciowa.getTaktowanie();
        int oknoTolerancji = (interwal > 0) ? std::max(30, 2000 / interwal) : 30;

        // Resetowanie m_lastReceivedPacketId przy starcie na nowo
        if (numer < 5 && m_lastReceivedPacketId > 20) {
            qDebug() << "Regulator: Wykryto nową epokę numeracji próbek (numer =" << numer << ", stary id =" << m_lastReceivedPacketId << ")";
            m_lastReceivedPacketId = -1;
            m_masterBuffer.clear();
        }

        // Odrzucane pakirty
        if ((int64_t)numer >= (int64_t)m_mainWindow->nr_probki) {
            qDebug() << "Regulator: Odrzucono ramkę ze starej epoki / przyszłości (numer=" << numer << " >= nr_probki=" << m_mainWindow->nr_probki << ")";
            return;
        }

        // Odrzucanie tylko pakietów starszych
        if (m_lastReceivedPacketId >= 0 && (int64_t)numer < m_lastReceivedPacketId - oknoTolerancji) {
            qDebug() << "Regulator: Odrzucono bardzo stary pakiet zwrotny:" << numer << "<" << m_lastReceivedPacketId - oknoTolerancji;
            return;
        }

        // if pakiet był już przetworzony
        if ((int64_t)numer <= m_lastReceivedPacketId && !m_masterBuffer.contains(numer)) {
            qDebug() << "Regulator: Odrzucono już przetworzony/zduplikowany pakiet zwrotny:" << numer;
            return;
        }

        if ((int64_t)numer > m_lastReceivedPacketId) {
            m_lastReceivedPacketId = numer;
        }
        m_receivedResponseForCurrentTick = true;
        m_mainWindow->ostatni_numer_sieciowy = numer;

        bool zsynchronizowane = m_mainWindow->m_hub.get_protokol().czy_zsynchronizowane(m_mainWindow->nr_probki, numer, 3);
        if (!zsynchronizowane && m_mainWindow->nr_probki > 0 && numer > 0) {
            int opoznienie = static_cast<int>(m_mainWindow->nr_probki) - static_cast<int>(numer);
            m_mainWindow->ui->label_sync_status->setText(QString("Desync! Opóźnienie: %1").arg(opoznienie));
            m_mainWindow->ui->label_sync_status->setStyleSheet("color: orange; font-weight: bold;");
        } else {
            m_mainWindow->ui->label_sync_status->setText("Synchronizacja OK");
            m_mainWindow->ui->label_sync_status->setStyleSheet("color: green; font-weight: bold;");
        }

        // Zaktualizuj y[k] od obiektu i historię ARX
        m_mainWindow->m_centralaSieciowa.odbierzY(dane.wartosc_regulowana);
        m_mainWindow->m_centralaSieciowa.getProstyUAR().aktualizujTloARX(dane.sygnal_sterujacy, dane.wartosc_regulowana);

        //!Bezuderzeniowe przejście po ponownym połączeniu
        if (m_reconnectedJustNow) {
            m_reconnectedJustNow = false;
            double err = dane.wartosc_zadana - dane.wartosc_regulowana;
            m_mainWindow->m_centralaSieciowa.dostosujCalkeDoU(dane.sygnal_sterujacy, err);
            qDebug() << "[Regulator] Zastosowano bezuderzeniowe przejście PID (bumpless transfer) dla y =" << dane.wartosc_regulowana << "u =" << dane.sygnal_sterujacy;
        }

        double w_val = dane.wartosc_zadana;
        double u_val = dane.sygnal_sterujacy;
        double uP_val = m_mainWindow->m_centralaSieciowa.getSkladowaP();
        double uI_val = m_mainWindow->m_centralaSieciowa.getSkladowaI();
        double uD_val = m_mainWindow->m_centralaSieciowa.getSkladowaD();
        double e_val = w_val - dane.wartosc_regulowana;

        if (m_masterBuffer.contains(numer)) {
            MasterFrame mf = m_masterBuffer.take(numer);
            w_val = mf.w;
            u_val = mf.u;
            uP_val = mf.uP;
            uI_val = mf.uI;
            uD_val = mf.uD;
            e_val = mf.e;
        }

        // clear ze starych ramek
        auto it = m_masterBuffer.begin();
        while (it != m_masterBuffer.end()) {
            if ((int64_t)it.key() + oknoTolerancji < (int64_t)numer) {
                it = m_masterBuffer.erase(it);
            } else {
                ++it;
            }
        }

        // draw wykres z czasem t = t0 + numer*T
        double T = m_mainWindow->m_centralaSieciowa.getTaktowanie() / 1000.0;
        double t = m_t0 + (numer * T);
        m_mainWindow->m_czasSieciowy = t;
        m_mainWindow->m_centralaSieciowa.set_Czas(t);

        m_mainWindow->m_centralaSieciowa.ustawCustomDaneWykresu(w_val, dane.wartosc_regulowana, u_val, e_val, uP_val, uI_val, uD_val);
        m_mainWindow->m_centralaSieciowa.emitujWykres();
        m_mainWindow->m_centralaSieciowa.czyscCustomDaneWykresu();

    } else if (m_mainWindow->ui->server_radioButton->isChecked()) {

        int interwal = m_mainWindow->m_centralaSieciowa.getTaktowanie();
        int oknoTolerancji = (interwal > 0) ? std::max(30, 2000 / interwal) : 30;

        // Resetowanie m_lastReceivedPacketId przy starcie
        if (m_reconnectedJustNow || (numer < 5 && m_lastReceivedPacketId > 20)) {
            qDebug() << "Obiekt: Wykryto synchronizację z Regulatorem (numer =" << numer << ", stary id =" << m_lastReceivedPacketId << ")";
            m_lastReceivedPacketId = -1;
            m_slaveBuffer.clear();
            m_reconnectedJustNow = false;
        }

        if (m_lastReceivedPacketId >= 0 && (int64_t)numer < m_lastReceivedPacketId - oknoTolerancji) {
            qDebug() << "Obiekt: Odrzucono opóźniony pakiet sterujący:" << numer << "<" << m_lastReceivedPacketId - oknoTolerancji;
            return;
        }
        if ((int64_t)numer > m_lastReceivedPacketId) {
            m_lastReceivedPacketId = numer;
        }

        // Natychmiastowe procesowanie ramki nadesłanej przez Regulatora (symulacja i rysowanie wykresu)
        m_isSimulationStarted = true;
        m_slaveTimer->stop();

        m_lastU = dane.sygnal_sterujacy;
        m_lastW = dane.wartosc_zadana;

        m_mainWindow->m_centralaSieciowa.odbierzU(m_lastU);
        m_mainWindow->m_centralaSieciowa.odbierzW(m_lastW);

        //!Wyrównanie czasu i licznika próbek do nadrzędnego Regulatora
        m_mainWindow->nr_probki = numer;
        m_mainWindow->ostatni_numer_sieciowy = numer;

        double T = m_mainWindow->m_centralaSieciowa.getTaktowanie() / 1000.0;
        if (T <= 0.0) T = 0.1;
        double t = (dane.czas > 0.0 || numer == 0) ? dane.czas : (m_t0 + (m_mainWindow->nr_probki * T));
        m_t0 = t - (m_mainWindow->nr_probki * T);
        m_mainWindow->m_czasSieciowy = t;
        m_mainWindow->m_centralaSieciowa.set_Czas(m_mainWindow->m_czasSieciowy);

        double y_prev = m_mainWindow->m_centralaSieciowa.getAktualnaWartoscRegulowana();
        m_mainWindow->m_centralaSieciowa.getProstyUAR().aktualizujTloPID(m_lastW, y_prev);
        m_mainWindow->m_centralaSieciowa.getGenerator().setLicznikProbki(numer + 2); // Synchronizuj licznik próbek generatora

        m_mainWindow->m_centralaSieciowa.wykonajKrokSerwera();

        double y = m_mainWindow->m_centralaSieciowa.getAktualnaWartoscRegulowana();
        double u_slave = m_mainWindow->m_centralaSieciowa.getAktualneSterowanie();
        m_mainWindow->m_hub.wyslij_ramke(m_mainWindow->nr_probki, u_slave, y, m_lastW, m_mainWindow->m_czasSieciowy);
        m_mainWindow->m_centralaSieciowa.emitujWykres();

        m_mainWindow->nr_probki++;

        if (m_isEmergencyActive) {
            m_isEmergencyActive = false;
            qDebug() << "Połączenie przywrócone - powrót do normalnego taktowania.";
        }

        int interwal_taktu = m_mainWindow->m_centralaSieciowa.getTaktowanie();
        if (interwal_taktu <= 0) interwal_taktu = 100;
        m_slaveTimer->start(std::max(1000, interwal_taktu * 4));
    }
}

void ObustronnyManager::handlePolaczono()
{
    if (m_mainWindow->ui->taktowanie_comboBox->currentText() != "Obustronne") return;

    // reset stanów awaryjnych bez kasowania wykresów
    m_isEmergencyActive = false;
    m_consecutiveLosses = 0;
    m_receivedResponseForCurrentTick = false;
    m_slaveBuffer.clear();
    m_masterBuffer.clear();
    m_lastReceivedPacketId = -1;
    m_intervalJustChanged = false;
    m_reconnectedJustNow = true;

    if (m_mainWindow->ui->server_radioButton->isChecked()) {
        // Obiekt zatrzymuje timer awaryjny i oczekuje na nadrzędne taktowanie Regulatora
        m_slaveTimer->stop();
        m_mainWindow->wylacz_wlacz_kontrolki(false, true);
        m_mainWindow->wylacz_wlacz_kontrolki_start_stop(false);
        m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(false);
        m_mainWindow->ui->taktowanie_comboBox->setEnabled(false);
        m_mainWindow->ui->server_radioButton->setEnabled(true);
        m_mainWindow->ui->klient_radioButton->setEnabled(true);
        qDebug() << "[Obiekt] Połączenie nawiązane ponownie. Zatrzymano watchdog timer, oczekiwanie na pierwszą ramkę Regulatora...";

    } else if (m_mainWindow->ui->klient_radioButton->isChecked()) {

        m_mainWindow->wylacz_wlacz_kontrolki(true, false);
        m_mainWindow->wylacz_wlacz_kontrolki_start_stop(true);
        m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(false);
        m_mainWindow->ui->taktowanie_comboBox->setEnabled(false);

        // Rozesłanie pełnej konfiguracji do Obiekttu po połączeniu
        m_mainWindow->PID_wysyl();
        m_mainWindow->Gen_wysyl();
        m_mainWindow->Interwal_wysyl(m_mainWindow->m_czasSieciowy);
        m_mainWindow->Okno_wysyl();

        if (m_isSimulationStarted) {
            m_mainWindow->Komenda_wysyl("START", m_mainWindow->m_czasSieciowy);
            if (m_mainWindow->zegar && !m_mainWindow->zegar->isActive()) {
                int interwal = m_mainWindow->m_centralaSieciowa.getTaktowanie();
                if (interwal <= 0) interwal = 100;
                m_mainWindow->zegar->start(interwal);
                qDebug() << "[Regulator] Połączenie nawiązane ponownie. Wznowiono zegar, czas =" << m_mainWindow->m_czasSieciowy;
            }
        } else {
            qDebug() << "[Regulator] Połączenie nawiązane ponownie. czas =" << m_mainWindow->m_czasSieciowy;
        }

        m_mainWindow->ui->label_sync_status->setText("Połączono — zsynchronizowano");
        m_mainWindow->ui->label_sync_status->setStyleSheet("color: green; font-weight: bold;");
    }
}

void ObustronnyManager::handleRozlaczono()
{
    if (m_mainWindow->ui->comboBox->currentText() != "Sieciowy") return;
    if (m_mainWindow->ui->taktowanie_comboBox->currentText() != "Obustronne") return;

    if (m_mainWindow->ui->server_radioButton->isChecked()) {
        // Obiekt ma pracować dalej z ostatnimi zamrożonymi wartościami u/w
        m_isEmergencyActive = true;
        int interwal = m_mainWindow->m_centralaSieciowa.getTaktowanie();
        if (interwal <= 0) interwal = 100;
        m_slaveTimer->start(interwal);
        qDebug() << "[Obiekt] AWARIA: Połączenie utracone! Obiekt pracuje w trybie awaryjnym (zapętlenie wartości).";

    } else if (m_mainWindow->ui->klient_radioButton->isChecked()) {
        //stop zegar
        if (m_mainWindow->zegar) {
            m_mainWindow->zegar->stop();
        }
        m_isEmergencyActive = true;  // blokuje handleMasterTimerTimeout() przed podwójnym startem
        m_mainWindow->ui->label_sync_status->setText("POŁĄCZENIE UTRACONE — oczekiwanie...");
        m_mainWindow->ui->label_sync_status->setStyleSheet("color: red; font-weight: bold;");
        qDebug() << "[Regulator] Blad: Połączenie utracone! Zatrzymano generację zegara. Symulacja zostanie wznowiona po ponownym połączeniu.";
    }
}
