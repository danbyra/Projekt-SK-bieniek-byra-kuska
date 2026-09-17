#include "jednostronny.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

JednostronnyManager::JednostronnyManager(MainWindow* mainWindow, QObject* parent)
    : QObject(parent)
    , m_mainWindow(mainWindow)
    , m_emergencyTimer(new QTimer(this))
    , m_lastU(0.0)
    , m_lastW(0.0)
    , m_isEmergencyActive(false)
    , m_responseReceived(false)
    , m_missedPacketsCount(0)
    , m_isSimulationStarted(false)
    , m_reconnectedJustNow(false)
{
    connect(m_emergencyTimer, &QTimer::timeout, this, &JednostronnyManager::handleEmergencyTick);
}

JednostronnyManager::~JednostronnyManager()
{
    delete m_emergencyTimer;
}

void JednostronnyManager::startSimulation()
{
    m_isSimulationStarted = true;
    m_responseReceived = false;
    m_missedPacketsCount = 0;
    m_isEmergencyActive = false;
    m_emergencyTimer->stop();
    m_sentFramesQueue.clear();
    //block ikoin
    m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(false);
    m_mainWindow->ui->taktowanie_comboBox->setEnabled(false);
}

void JednostronnyManager::stopSimulation()
{
    m_isSimulationStarted = false;
    m_isEmergencyActive = false;
    m_emergencyTimer->stop();
    //unblock ikoin
    m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(true);
    m_mainWindow->ui->taktowanie_comboBox->setEnabled(true);
}

void JednostronnyManager::resetSimulation()
{
    m_isSimulationStarted = false;
    m_isEmergencyActive = false;
    m_emergencyTimer->stop();
    m_lastU = 0.0;
    m_lastW = 0.0;
    m_responseReceived = false;
    m_missedPacketsCount = 0;
    m_sentFramesQueue.clear();
    //unblock ikoin
    m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(true);
    m_mainWindow->ui->taktowanie_comboBox->setEnabled(true);
}

void JednostronnyManager::handleStartCommand(double t0, uint32_t numer)
{
    m_isSimulationStarted = true;
    m_isEmergencyActive = false;
    m_emergencyTimer->stop();
    m_responseReceived = false;
    m_missedPacketsCount = 0;
    m_sentFramesQueue.clear();

    if (m_mainWindow->zegar) {
        m_mainWindow->zegar->stop();
    }

    if (m_mainWindow->nr_probki > 0 || m_mainWindow->m_czasSieciowy > 0.0) {
        // Wznowienie / Odpauzowanie
        qDebug() << "[Obiekt/Jednostronny] Odebrano polecenie: START (Wznowienie). Synchronizacja nr_probki =" << numer;
        m_mainWindow->nr_probki = numer;
        m_mainWindow->ostatni_numer_sieciowy = numer;
    } else {
        // Nowy start
        m_mainWindow->nr_probki = 0;
        m_mainWindow->ostatni_numer_sieciowy = 0;
        m_mainWindow->m_czasSieciowy = t0;
        m_mainWindow->m_ostatniaProbkaSieciowa = 0;
        m_lastU = 0.0;
        m_lastW = 0.0;

        //Reset generatora i PID (bez kasowania pamięci modelu ARX dla bezuderzeniowości)
        m_mainWindow->m_centralaSieciowa.getGenerator().ResetPamieci();
        m_mainWindow->m_centralaSieciowa.resetPID();

        qDebug() << "[Obiekt/Jednostronny] Odebrano polecenie: START (Nowy start). Oczekiwanie na pierwszą ramkę...";
    }

    // Blokada kontrolek
    m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(false);
    m_mainWindow->ui->taktowanie_comboBox->setEnabled(false);
}

void JednostronnyManager::handleStopCommand()
{
    m_isSimulationStarted = false;
    m_isEmergencyActive = false;
    m_emergencyTimer->stop();

    if (m_mainWindow->zegar) {
        m_mainWindow->zegar->stop();
    }

    qDebug() << "[Obiekt/Jednostronny] Odebrano polecenie: STOP. Zatrzymano symulację.";

    // Odblokowanie kontrolek
    m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(true);
    m_mainWindow->ui->taktowanie_comboBox->setEnabled(true);
}

void JednostronnyManager::handleResetCommand()
{
    m_isSimulationStarted = false;
    m_isEmergencyActive = false;
    m_emergencyTimer->stop();

    if (m_mainWindow->zegar) {
        m_mainWindow->zegar->stop();
    }

    m_sentFramesQueue.clear();
    m_lastU = 0.0;
    m_lastW = 0.0;
    m_responseReceived = false;
    m_missedPacketsCount = 0;

    m_mainWindow->nr_probki = 0;
    m_mainWindow->ostatni_numer_sieciowy = 0;
    m_mainWindow->m_czasSieciowy = 0.0;
    m_mainWindow->m_ostatniaProbkaSieciowa = 0;

    m_mainWindow->m_centralaSieciowa.getGenerator().ResetPamieci();
    m_mainWindow->m_centralaSieciowa.getProstyUAR().ResetStanow();
    m_mainWindow->m_centralaSieciowa.on_reset_clicked();

    m_mainWindow->wyczyscWykresy();

    qDebug() << "[Obiekt/Jednostronny] Odebrano polecenie: RESET. Przywrócono stan początkowy.";

    // Odblokowanie kontrolek
    m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(true);
    m_mainWindow->ui->taktowanie_comboBox->setEnabled(true);
}

void JednostronnyManager::handleIntervalChanged()
{
    m_sentFramesQueue.clear();
}

void JednostronnyManager::handleTimerTimeout()
{
    if (m_mainWindow->ui->comboBox->currentText() != "Sieciowy") return;
    if (m_mainWindow->ui->taktowanie_comboBox->currentText() != "Jednostronne") return;
    // Blok takt gdy połączenie jest utracone
    if (m_isEmergencyActive) return;

    if (m_mainWindow->ui->klient_radioButton->isChecked()) {
        // Reg (Generator/Regulator)
        if (m_mainWindow->nr_probki > 0 && !m_responseReceived) {
            //tolerancja 3 taktów sieci i buforowanie TCP
            m_missedPacketsCount++;
            if (m_missedPacketsCount >= 3) {
                m_mainWindow->ui->label_sync_status->setText(QString("Opóźnienie / Utrata (%1)").arg(m_missedPacketsCount));
                m_mainWindow->ui->label_sync_status->setStyleSheet("color: red; font-weight: bold;");
            }
        } else {
            m_mainWindow->ui->label_sync_status->setText("Synchronizacja OK");
            m_mainWindow->ui->label_sync_status->setStyleSheet("color: green; font-weight: bold;");
            m_missedPacketsCount = 0;
        }

        m_responseReceived = false;

        // Kumuluj czas przyrostowo
        double dt = m_mainWindow->m_centralaSieciowa.getTaktowanie() / 1000.0;
        m_mainWindow->m_czasSieciowy += dt;
        m_mainWindow->m_centralaSieciowa.set_Czas(m_mainWindow->m_czasSieciowy);

        // Regulator
        m_mainWindow->m_centralaSieciowa.wykonajKrokKlienta();
        double u = m_mainWindow->m_centralaSieciowa.getAktualneSterowanie();

        // Dodaj czas, W do kolejki (FIFO)
        if (m_sentFramesQueue.size() < 100) {
            m_sentFramesQueue.enqueue({
                m_mainWindow->m_czasSieciowy,
                m_mainWindow->m_centralaSieciowa.getAktualnaWartoscZadana()
            });
        }
        //wysył u
        m_mainWindow->wyslij_sterowanie_przez_siec(u);
        m_mainWindow->nr_probki++;
    }
}

void JednostronnyManager::handleOdebranoSymulacje(uint32_t numer, const communication_protocol::dane_symulacji& dane)
{
    if (m_mainWindow->ui->taktowanie_comboBox->currentText() != "Jednostronne") return;

    if (m_mainWindow->ui->klient_radioButton->isChecked()) {

        m_responseReceived = true;
        m_missedPacketsCount = 0;
        m_mainWindow->ostatni_numer_sieciowy = numer;

        bool zsynchronizowane = m_mainWindow->m_hub.get_protokol().czy_zsynchronizowane(m_mainWindow->nr_probki, numer, 3);
        if (!zsynchronizowane && m_mainWindow->nr_probki > 0 && numer > 0) {
            m_mainWindow->ui->label_sync_status->setText(QString("Desync! Lok:%1 Sieć:%2").arg(m_mainWindow->nr_probki).arg(numer));
            m_mainWindow->ui->label_sync_status->setStyleSheet("color: orange; font-weight: bold;");
        }

        m_mainWindow->m_centralaSieciowa.odbierzY(dane.wartosc_regulowana);
        m_mainWindow->m_centralaSieciowa.getProstyUAR().aktualizujTloARX(dane.sygnal_sterujacy, dane.wartosc_regulowana);

        //Bezuderzeniowe przejście po ponownym połączeniu
        if (m_reconnectedJustNow) {
            m_reconnectedJustNow = false;
            double err = m_mainWindow->m_centralaSieciowa.getAktualnaWartoscZadana() - dane.wartosc_regulowana;
            m_mainWindow->m_centralaSieciowa.dostosujCalkeDoU(dane.sygnal_sterujacy, err);
            qDebug() << "[Regulator/Jednostronny] Zastosowano bezuderzeniowe przejście PID ) dla y =" << dane.wartosc_regulowana;
        }

        if (!m_sentFramesQueue.isEmpty()) {
            auto fi = m_sentFramesQueue.dequeue();

            double savedW    = m_mainWindow->m_centralaSieciowa.getAktualnaWartoscZadana();
            double savedTime = m_mainWindow->m_czasSieciowy;

            m_mainWindow->m_centralaSieciowa.odbierzW(fi.w);
            m_mainWindow->m_centralaSieciowa.set_Czas(fi.time);
            m_mainWindow->m_centralaSieciowa.emitujWykres();

            m_mainWindow->m_centralaSieciowa.odbierzW(savedW);
            m_mainWindow->m_centralaSieciowa.set_Czas(savedTime);
        } else {
            //kolejka pusta (reset) - używa bieżącego czasu
            m_mainWindow->m_centralaSieciowa.set_Czas(m_mainWindow->m_czasSieciowy);
            m_mainWindow->m_centralaSieciowa.emitujWykres();
        }

    } else if (m_mainWindow->ui->server_radioButton->isChecked()) {

        if (m_isEmergencyActive) {
            m_isEmergencyActive = false;
            m_emergencyTimer->stop();
            qDebug() << "Połączenie przywrócone - powrót do pracy rozproszonej.";
        }

        m_lastU = dane.sygnal_sterujacy;
        m_lastW = dane.wartosc_zadana;

        m_mainWindow->m_centralaSieciowa.odbierzU(dane.sygnal_sterujacy);
        m_mainWindow->m_centralaSieciowa.odbierzW(dane.wartosc_zadana);

        //Wyrównanie czasu do nadrzędnego Regulatora (Regulatora)
        m_mainWindow->m_czasSieciowy = dane.czas;
        m_mainWindow->m_centralaSieciowa.set_Czas(m_mainWindow->m_czasSieciowy);
        
        m_mainWindow->m_centralaSieciowa.wykonajKrokSerwera();
        m_mainWindow->m_centralaSieciowa.getProstyUAR().aktualizujTloPID(dane.wartosc_zadana, dane.wartosc_regulowana);
        m_mainWindow->m_centralaSieciowa.getGenerator().setLicznikProbki(numer + 2);

        double y = m_mainWindow->m_centralaSieciowa.getAktualnaWartoscRegulowana();
        m_mainWindow->nr_probki = numer + 1;
        m_mainWindow->ostatni_numer_sieciowy = numer;
        m_mainWindow->wyslij_wyjscie_przez_siec(y);
        m_mainWindow->m_centralaSieciowa.emitujWykres();
    }
}

void JednostronnyManager::handlePolaczono()
{
    if (m_mainWindow->ui->taktowanie_comboBox->currentText() != "Jednostronne") return;

    // reset stanów awaryjnych bez kasowania wykresów
    m_isEmergencyActive = false;
    m_emergencyTimer->stop();
    m_responseReceived = false;
    m_missedPacketsCount = 0;
    m_sentFramesQueue.clear();
    m_reconnectedJustNow = true;

    if (m_mainWindow->ui->server_radioButton->isChecked()) {
        m_mainWindow->wylacz_wlacz_kontrolki(false, true);
        m_mainWindow->wylacz_wlacz_kontrolki_start_stop(false);
        m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(false);
        m_mainWindow->ui->taktowanie_comboBox->setEnabled(false);
        m_mainWindow->ui->server_radioButton->setEnabled(true);
        m_mainWindow->ui->klient_radioButton->setEnabled(true);
        qDebug() << "[Obiekt/Jednostronny] Połączenie nawiązane ponownie. Zablokowano kontrolki, oczekiwanie na taktowanie Regulatora.";

    } else if (m_mainWindow->ui->klient_radioButton->isChecked()) {

        m_mainWindow->wylacz_wlacz_kontrolki(true, false);
        m_mainWindow->wylacz_wlacz_kontrolki_start_stop(true);
        m_mainWindow->ui->wybor_serializacji_comboBox->setEnabled(false);
        m_mainWindow->ui->taktowanie_comboBox->setEnabled(false);

        // Rozesłanie konfiguracji
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
                qDebug() << "[Regulator/Jednostronny] Połączenie nawiązane ponownie. Wznowiono zegar.";
            }
        } else {
            qDebug() << "[Regulator/Jednostronny] Połączenie nawiązane ponownie.";
        }

        m_mainWindow->ui->label_sync_status->setText("Połączono — zsynchronizowano");
        m_mainWindow->ui->label_sync_status->setStyleSheet("color: green; font-weight: bold;");
    }
}

void JednostronnyManager::handleRozlaczono()
{
    if (m_mainWindow->ui->comboBox->currentText() != "Sieciowy") return;
    if (m_mainWindow->ui->taktowanie_comboBox->currentText() != "Jednostronne") return;

    if (m_mainWindow->ui->server_radioButton->isChecked()) {
        //uruchom timer awaryjny - kontynuuje symulację z zamrożonymi wartościami
        m_isEmergencyActive = true;
        int interwal = m_mainWindow->m_centralaSieciowa.getTaktowanie();
        if (interwal <= 0) interwal = 100;
        m_emergencyTimer->start(interwal);
        qDebug() << "[Obiekt/Jednostronny] Blad: Połączenie utracone! Obiekt pracuje w trybie - zapętlenia wartości";

    } else if (m_mainWindow->ui->klient_radioButton->isChecked()) {
        // stop zegar

        if (m_mainWindow->zegar) {
            m_mainWindow->zegar->stop();
        }
        m_isEmergencyActive = true;  // blokuje handleTimerTimeout() przed podwójnym startem
        m_mainWindow->ui->label_sync_status->setText("POŁĄCZENIE UTRACONE — oczekiwanie...");
        m_mainWindow->ui->label_sync_status->setStyleSheet("color: red; font-weight: bold;");
        qDebug() << "[Regulator/Jednostronny] blad: Połączenie utracone! Wstrzymano generację zegara. Symulacja zostanie wznowiona po ponownym połączeniu.";
    }
}

void JednostronnyManager::handleEmergencyTick()
{
    if (!m_isEmergencyActive) return;
    if (m_mainWindow->ui->comboBox->currentText() != "Sieciowy") {
        m_emergencyTimer->stop();
        m_isEmergencyActive = false;
        return;
    }

    // symulacja uzywajac ostatniego stepu
    m_mainWindow->m_centralaSieciowa.odbierzU(m_lastU);
    m_mainWindow->m_centralaSieciowa.odbierzW(m_lastW);

    double T = m_mainWindow->m_centralaSieciowa.getTaktowanie() / 1000.0;
    m_mainWindow->m_czasSieciowy += T;
    m_mainWindow->m_centralaSieciowa.set_Czas(m_mainWindow->m_czasSieciowy);

    m_mainWindow->m_centralaSieciowa.wykonajKrokSerwera();
    int currentLicz = m_mainWindow->m_centralaSieciowa.getGenerator().getLicznikProbki();
    m_mainWindow->m_centralaSieciowa.getGenerator().setLicznikProbki(currentLicz + 1);
    m_mainWindow->m_centralaSieciowa.emitujWykres();
}
