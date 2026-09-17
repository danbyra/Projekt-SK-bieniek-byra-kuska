#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ust_arx.h"
#include "jednostronny.h"
#include "obustronny.h"
#include <QNetworkInterface>
#include <QWidget>
#include <QDataStream>

constexpr QMargins zeroweMarginesy{0, 0, 0, 0};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    m_pierwszaProbkaPoStart = false;
    m_jednostronny = new JednostronnyManager(this, this);
    m_obustronny = new ObustronnyManager(this, this);

   
    connect(&m_centrala, &Centrala::ogarnijWykresy, this,
            &MainWindow::ogarnianieWykresow);

    // wykres wartosci zadanej i regulowanej
    m_seriaWartosciZadanej->setName("Zadana");
    m_seriaWartosciRegulowanej->setName("Regulowana");
    QChart *wykresWartosci = new QChart();
    wykresWartosci->addSeries(m_seriaWartosciZadanej);
    wykresWartosci->addSeries(m_seriaWartosciRegulowanej);
    wykresWartosci->setMargins(zeroweMarginesy);
    wykresWartosci->setAnimationOptions(QChart::NoAnimation);

    m_axisY1->setTitleText("Wartość");

    wykresWartosci->addAxis(m_axisX1, Qt::AlignBottom);
    wykresWartosci->addAxis(m_axisY1, Qt::AlignLeft);

    m_seriaWartosciZadanej->attachAxis(m_axisX1);
    m_seriaWartosciZadanej->attachAxis(m_axisY1);
    m_seriaWartosciRegulowanej->attachAxis(m_axisX1);
    m_seriaWartosciRegulowanej->attachAxis(m_axisY1);

    QChartView *widokWykresuWartosci = new QChartView(wykresWartosci);
    // tryb odświeżania z FullViewportUpdate na MinimalViewportUpdate-> zapobiega blokowaniu wątku głównego
    widokWykresuWartosci->setViewportUpdateMode(
        QGraphicsView::MinimalViewportUpdate);
    widokWykresuWartosci->setRenderHint(QPainter::Antialiasing);
    widokWykresuWartosci->setStyleSheet("background: white;");
    ui->wykresy->addWidget(widokWykresuWartosci, 4);

    // wykres uchybu
    QChart *wykresUchybu = new QChart();
    wykresUchybu->addSeries(m_seriaUchybu);
    wykresUchybu->legend()->hide();
    wykresUchybu->setMargins(zeroweMarginesy);
    wykresUchybu->setAnimationOptions(QChart::NoAnimation);

    m_axisY2->setTitleText("Uchyb");

    wykresUchybu->addAxis(m_axisX2, Qt::AlignBottom);
    wykresUchybu->addAxis(m_axisY2, Qt::AlignLeft);

    m_seriaUchybu->attachAxis(m_axisX2);
    m_seriaUchybu->attachAxis(m_axisY2);

    QChartView *widokWykresuUchybu = new QChartView(wykresUchybu);
    // odświeżanie z FullViewportUpdate na MinimalViewportUpdate.
    widokWykresuUchybu->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    widokWykresuUchybu->setRenderHint(QPainter::Antialiasing);
    widokWykresuUchybu->setStyleSheet("background: white;");
    ui->wykresy->addWidget(widokWykresuUchybu, 2);

    // wykres sterowania
    QChart *wykresSterowania = new QChart();
    wykresSterowania->addSeries(m_seriaSterowania);
    wykresSterowania->legend()->hide();
    wykresSterowania->setMargins(zeroweMarginesy);
    wykresSterowania->setAnimationOptions(QChart::NoAnimation);

    m_axisY3->setTitleText("Sterowanie");

    wykresSterowania->addAxis(m_axisX3, Qt::AlignBottom);
    wykresSterowania->addAxis(m_axisY3, Qt::AlignLeft);

    m_seriaSterowania->attachAxis(m_axisX3);
    m_seriaSterowania->attachAxis(m_axisY3);

    QChartView *widokWykresuSterowania = new QChartView(wykresSterowania);
    // odświeżanie z FullViewportUpdate na MinimalViewportUpdate.
    widokWykresuSterowania->setViewportUpdateMode(
        QGraphicsView::MinimalViewportUpdate);
    widokWykresuSterowania->setRenderHint(QPainter::Antialiasing);
    widokWykresuSterowania->setStyleSheet("background: white;");
    ui->wykresy->addWidget(widokWykresuSterowania, 2);

    // wykres składowych
    m_seriaSkladowejP->setName("P");
    m_seriaSkladowejI->setName("I");
    m_seriaSkladowejD->setName("D");

    QChart *wykresSkladowych = new QChart();

    m_axisY4->setTitleText("Składowe");
    wykresSkladowych->addAxis(m_axisX4, Qt::AlignBottom);
    wykresSkladowych->addAxis(m_axisY4, Qt::AlignLeft);

    wykresSkladowych->addSeries(m_seriaSkladowejP);
    wykresSkladowych->addSeries(m_seriaSkladowejI);
    wykresSkladowych->addSeries(m_seriaSkladowejD);
    wykresSkladowych->setMargins(zeroweMarginesy);
    wykresSkladowych->setAnimationOptions(QChart::NoAnimation);

    m_seriaSkladowejP->attachAxis(m_axisX4);
    m_seriaSkladowejP->attachAxis(m_axisY4);
    m_seriaSkladowejI->attachAxis(m_axisX4);
    m_seriaSkladowejI->attachAxis(m_axisY4);
    m_seriaSkladowejD->attachAxis(m_axisX4);
    m_seriaSkladowejD->attachAxis(m_axisY4);

    QChartView *widokWykresuSkladowych = new QChartView(wykresSkladowych);
   // odświeżanie z FullViewportUpdate na MinimalViewportUpdate
    widokWykresuSkladowych->setViewportUpdateMode(
        QGraphicsView::MinimalViewportUpdate);
    widokWykresuSkladowych->setRenderHint(QPainter::Antialiasing);
    widokWykresuSkladowych->setStyleSheet("background: white;");
    ui->wykresy->addWidget(widokWykresuSkladowych, 2);

    ui->rodzajCalkowaniaCombo->addItem("Stała przed sumą"); // index 0
    ui->rodzajCalkowaniaCombo->addItem("Stała pod sumą");   // index 1
    ui->rodzajCalkowaniaCombo->setCurrentIndex(0);

    ui->generatorCombo->addItem("Sinusoida");
    ui->generatorCombo->addItem("Prostokąt");
    ui->generatorCombo->setCurrentIndex(0);

    // wywolywanie wszystkich seterow z domyslnymi wartosciami
    on_okno_editingFinished();
    on_interwal_editingFinished();

    on_generatorCombo_currentIndexChanged(0);
    on_okres_editingFinished();
    on_amplituda_editingFinished();
    on_skladowaStala_editingFinished();
    on_wypelnienie_editingFinished();
    on_wzmocnienie_editingFinished();
    on_stalaI_editingFinished();
    on_stalaD_editingFinished();
    on_rodzajCalkowaniaCombo_currentIndexChanged(0);
#ifndef STARY_CHECKBOX
    on_ograniczenia_checkStateChanged(Qt::Checked);
#else  // dwa nastepne sloty to fallback do qt < 6.7
    on_ograniczenia_stateChanged(2);
#endif // STARY_CHECKBOX
    on_ograniczeniaMin_editingFinished();
    on_ograniczeniaMax_editingFinished();

    m_punktySeriiWartosciZadanej.reserve(5000);
    m_punktySeriiWartosciRegulowanej.reserve(5000);
    m_punktySeriiUchybu.reserve(5000);
    m_punktySeriiSterowania.reserve(5000);
    m_punktySeriiSkladowejP.reserve(5000);
    m_punktySeriiSkladowejI.reserve(5000);
    m_punktySeriiSkladowejD.reserve(5000);

    m_ostatnieY1.reserve(10000);
    m_ostatnieY2.reserve(5000);
    m_ostatnieY3.reserve(5000);
    m_ostatnieY4.reserve(15000);


    //blokowanie trybu niesieciowego-ikony
    ui->klient_radioButton->setEnabled(false);
    ui->server_radioButton->setEnabled(false);
    ui->wybor_serializacji_comboBox->setEnabled(false);
    ui->lcdNumber->setEnabled(false);
    ui->taktowanie_comboBox->setEnabled(false);
    ui->ip_adres_wyswietl_TextEdit->setEnabled(false);


connect(&m_hub, &network_hub::rozlaczono, this, &MainWindow::czy_rozlaczylo);
    connect(&m_hub, &network_hub::polaczono, this, &MainWindow::Czy_polaczylo);

    //axis
    m_axisX1->setTitleText("czas [s]");
    m_axisX2->setTitleText("czas [s]");
    m_axisX3->setTitleText("czas [s]");
    m_axisX4->setTitleText("czas [s]");
    m_axisX1->setLabelFormat("%.2f");
    m_axisX2->setLabelFormat("%.2f");
    m_axisX3->setLabelFormat("%.2f");
    m_axisX4->setLabelFormat("%.2f");
    m_axisY1->setLabelFormat("%.2f");
    m_axisY2->setLabelFormat("%.2f");
    m_axisY3->setLabelFormat("%.2f");
    m_axisY4->setLabelFormat("%.2f");
    zegar = new QTimer(this);
    zegar->setTimerType(Qt::PreciseTimer);

    connect(zegar, &QTimer::timeout, this, &MainWindow::on_zegar_timeout);

    //Wyświetlanie pakietów na sekundę (PPS)
    connect(&m_hub.get_protokol(),
            &communication_protocol::wydajnosc_zaktualizowana,
            [&](double pps) { ui->lcdNumber->display(pps); });

    //sygnały sieciowe
    connect(&m_hub, &network_hub::odebrano_konfiguracje, this, &MainWindow::na_odebrano_konfiguracje, Qt::QueuedConnection);
    connect(&m_hub, &network_hub::odebrano_symulacje, this, &MainWindow::na_odebrano_symulacje, Qt::QueuedConnection);
    connect(&m_hub, &network_hub::odebrano_sterowanie, this, &MainWindow::na_odebrano_sterowanie, Qt::QueuedConnection);

    // Połączenie z wykresem serwera
    connect(&m_centralaSieciowa, &Centrala::ogarnijWykresy, this, &MainWindow::ogarnianieWykresow);

    //sprawdzanie synchronizacji/opóźnienia
    QTimer *syncTimer = new QTimer(this);
    connect(syncTimer, &QTimer::timeout, this, [this]() {
        if (ui->comboBox->currentText() == "Sieciowy" && (ui->server_radioButton->isChecked() || ui->klient_radioButton->isChecked())) {
            int interwal_ms = ui->interwal->value();
            bool nadaza = m_hub.get_protokol().czy_nadaza(interwal_ms);
            bool zsynchronizowane = (nr_probki == 0 || ostatni_numer_sieciowy == 0) ||
                m_hub.get_protokol().czy_zsynchronizowane(nr_probki, ostatni_numer_sieciowy, 3);

            if (!zsynchronizowane) {
                ui->label_sync_status->setText(QString("Desynchronizacja! Lok:%1 Sieć:%2").arg(nr_probki).arg(ostatni_numer_sieciowy));
                ui->label_sync_status->setStyleSheet("color: orange; font-weight: bold;");
            } else if (nadaza) {
                ui->label_sync_status->setText("Synchronizacja OK");
                ui->label_sync_status->setStyleSheet("color: green; font-weight: bold;");
            } else {
                ui->label_sync_status->setText("Opóźnienie / Utrata");
                ui->label_sync_status->setStyleSheet("color: red; font-weight: bold;");
            }
        } else {
            ui->label_sync_status->setText("Brak danych");
            ui->label_sync_status->setStyleSheet("");
        }
    });
    syncTimer->start(500);
    m_lastChartRepaintTimer.start();
    wylacz_wlacz_kontrolki_sieciowe(false);
}
MainWindow::~MainWindow() { delete ui; }

Centrala& MainWindow::getAktywnaCentrala() {
    if (ui->comboBox->currentText() == "Sieciowy") {
        return m_centralaSieciowa;
    }
    return m_centrala;
}

void MainWindow::ogarnianieWykresow() {
    Centrala& centrala = getAktywnaCentrala();
    double krok = centrala.getTaktowanie() / 1000.0;
    double okno = centrala.get_Okno();
    double czas = centrala.get_Czas();

    // przesunOsX jest na poczatku bo ustawia smoothscroll okna potrzebny do
    // usuwania punktow
    przesunOsX(krok, okno, czas);

    dodajPunkty(czas);

    usunPunkty(czas);

    // Throtling przerysowywania GUI dla bardzo małych interwałów (< 50ms)
    if (!m_lastChartRepaintTimer.isValid() || m_lastChartRepaintTimer.elapsed() >= 30) {
        podmienPunkty();

        // przesunOsY jest po punktach bo do tego potrzebne sa wektory z usunietymi
        // punktami
        przesunOsY();
        m_lastChartRepaintTimer.restart();
    }


}

void MainWindow::przesunOsX(double krok, double okno, double czas) {
    // smoothscroll osi X przy zmianie okna
    // dziala tylko z tym min i max
    if (okno < m_poprzOkno)
        okno = std::max(m_poprzOkno - krok, okno);
    else if (okno > m_poprzOkno)
        okno = std::min(m_poprzOkno + krok, okno);

    double startX = (czas > okno) ? (czas - okno) : 0;

  
    m_axisX1->setRange(startX, czas);
    m_axisX2->setRange(startX, czas);
    m_axisX3->setRange(startX, czas);
    m_axisX4->setRange(startX, czas);

    m_poprzOkno = okno;
}

Limity znajdzMinMax(const QVector<double> &w) {
    auto [min, max] = std::minmax_element(w.begin(), w.end());
    return {*min, *max};
}

double obliczPadding(const Limity l) {
    constexpr double PADDING = 0.1;
    constexpr double MIN_ROZNICA = 1.0;

    double roznica = l.max - l.min;
    if (roznica < MIN_ROZNICA)
        return PADDING;
    return roznica * PADDING;
}

void MainWindow::przesunOsY() {
    // obliczanie min, max i padding osi Y
    Limity l1 = znajdzMinMax(m_ostatnieY1);
    Limity l2 = znajdzMinMax(m_ostatnieY2);
    Limity l3 = znajdzMinMax(m_ostatnieY3);
    Limity l4 = znajdzMinMax(m_ostatnieY4);

    double p1 = obliczPadding(l1);
    double p2 = obliczPadding(l2);
    double p3 = obliczPadding(l3);
    double p4 = obliczPadding(l4);

    // ustawianie paddingu na osi Y (gora i dol)
    m_axisY1->setRange(l1.min - p1, l1.max + p1);
    m_axisY2->setRange(l2.min - p2, l2.max + p2);
    m_axisY3->setRange(l3.min - p3, l3.max + p3);
    m_axisY4->setRange(l4.min - p4, l4.max + p4);
}

void MainWindow::wyczyscWykresy() {
    m_seriaWartosciZadanej->clear();
    m_seriaWartosciRegulowanej->clear();
    m_seriaUchybu->clear();
    m_seriaSterowania->clear();
    m_seriaSkladowejP->clear();
    m_seriaSkladowejI->clear();
    m_seriaSkladowejD->clear();

    m_punktySeriiWartosciZadanej.clear();
    m_punktySeriiWartosciRegulowanej.clear();
    m_punktySeriiUchybu.clear();
    m_punktySeriiSterowania.clear();
    m_punktySeriiSkladowejP.clear();
    m_punktySeriiSkladowejI.clear();
    m_punktySeriiSkladowejD.clear();

    m_ostatnieY1.clear();
    m_ostatnieY2.clear();
    m_ostatnieY3.clear();
    m_ostatnieY4.clear();

    m_poprzOkno = 0.0;
    nr_probki = 0;
    ostatni_numer_sieciowy = 0;
    m_pierwszaProbkaPoStart = false;
    m_czasSieciowy = 0.0;
    m_ostatniaProbkaSieciowa = 0;
}

//asynchroniczne oknie dialogowym
void MainWindow::messageBoxInnyWatek(QMessageBox::Icon ikona, const QString& tytul, const QString& tekst)
{
    QTimer::singleShot(0, this, [this, ikona, tytul, tekst]() {
        QMessageBox* msgBox = new QMessageBox(ikona, tytul, tekst, QMessageBox::Ok, this);
        msgBox->setAttribute(Qt::WA_DeleteOnClose);
        msgBox->setModal(false);
        msgBox->show();
    });
}

void MainWindow::dodajPunkty(double czas) {
    // Zabezpieczenie monotoniczności osi X
    if (!m_punktySeriiUchybu.isEmpty() && czas <= m_punktySeriiUchybu.last().x() + 1e-5) {
        if (czas < m_punktySeriiUchybu.last().x() - 1e-5) {
            while (!m_punktySeriiUchybu.isEmpty() && m_punktySeriiUchybu.last().x() >= czas - 1e-5) {
                m_punktySeriiWartosciZadanej.removeLast();
                m_punktySeriiWartosciRegulowanej.removeLast();
                m_punktySeriiUchybu.removeLast();
                m_punktySeriiSterowania.removeLast();
                m_punktySeriiSkladowejP.removeLast();
                m_punktySeriiSkladowejI.removeLast();
                m_punktySeriiSkladowejD.removeLast();
            }
        } else {
            return;
        }
    }

    // pobranie danych z aktywnej centrali (lokalna || sieciowa)
    Centrala& centrala = getAktywnaCentrala();
    double y1 = centrala.getAktualnaWartoscZadana(),
        y2 = centrala.getAktualnaWartoscRegulowana();
    double e = centrala.getAktualnyUchyb();
    double u = centrala.getAktualneSterowanie();
    double uP = centrala.getSkladowaP(), uI = centrala.getSkladowaI(),
        uD = centrala.getSkladowaD();

    // te wektory beda podmienane do serii
    m_punktySeriiWartosciZadanej.emplaceBack(czas, y1);
    m_punktySeriiWartosciRegulowanej.emplaceBack(czas, y2);
    m_punktySeriiUchybu.emplaceBack(czas, e);
    m_punktySeriiSterowania.emplaceBack(czas, u);
    m_punktySeriiSkladowejP.emplaceBack(czas, uP);
    m_punktySeriiSkladowejI.emplaceBack(czas, uI);
    m_punktySeriiSkladowejD.emplaceBack(czas, uD);

    // kompresowanie danych do 4 wektorow zeby latwo obliczyc min max
    m_ostatnieY1.emplaceBack(y1);
    m_ostatnieY1.emplaceBack(y2);
    m_ostatnieY2.emplaceBack(e);
    m_ostatnieY3.emplaceBack(u);
    m_ostatnieY4.emplaceBack(uP);
    m_ostatnieY4.emplaceBack(uI);
    m_ostatnieY4.emplaceBack(uD);
}
void MainWindow::usunPunkty(double czas) {
    constexpr double PADDING_USUWANIA = 0.2;

    // Zliczanie ilu punktów należy się pozbyć
    int punktyDoUsuniecia = 0;
    int rozmiarUchybu = m_punktySeriiUchybu.size();

    while (punktyDoUsuniecia < rozmiarUchybu &&
           m_punktySeriiUchybu[punktyDoUsuniecia].x() < (czas - m_poprzOkno - PADDING_USUWANIA)) {
        punktyDoUsuniecia++;
    }

    /* Zmieniłe wcześniejsze removeFirst na bardziej optymalną wersje.*/
    if (punktyDoUsuniecia > 0) {
        m_punktySeriiWartosciZadanej.remove(0, punktyDoUsuniecia);
        m_punktySeriiWartosciRegulowanej.remove(0, punktyDoUsuniecia);
        m_punktySeriiUchybu.remove(0, punktyDoUsuniecia);
        m_punktySeriiSterowania.remove(0, punktyDoUsuniecia);
        m_punktySeriiSkladowejP.remove(0, punktyDoUsuniecia);
        m_punktySeriiSkladowejI.remove(0, punktyDoUsuniecia);
        m_punktySeriiSkladowejD.remove(0, punktyDoUsuniecia);

        m_ostatnieY1.remove(0, punktyDoUsuniecia * 2);
        m_ostatnieY2.remove(0, punktyDoUsuniecia);
        m_ostatnieY3.remove(0, punktyDoUsuniecia);
        m_ostatnieY4.remove(0, punktyDoUsuniecia * 3);
    }
}

void MainWindow::podmienPunkty() {
    // podmiana punktow w kazdej serii
    m_seriaWartosciZadanej->replace(m_punktySeriiWartosciZadanej);
    m_seriaWartosciRegulowanej->replace(m_punktySeriiWartosciRegulowanej);
    m_seriaUchybu->replace(m_punktySeriiUchybu);
    m_seriaSterowania->replace(m_punktySeriiSterowania);
    m_seriaSkladowejP->replace(m_punktySeriiSkladowejP);
    m_seriaSkladowejI->replace(m_punktySeriiSkladowejI);
    m_seriaSkladowejD->replace(m_punktySeriiSkladowejD);
}

void MainWindow::on_start_clicked() {
    if (ui->comboBox->currentText() != "Sieciowy") {
        m_centrala.on_start_clicked();
        if (zegar) {
            zegar->stop();
        }
        m_centralaSieciowa.setTaktowanie(ui->interwal->value());

    } else { //dla sieciowego

        // Blok dla serializacji i trybu taktowania w trakcie symulacji
        //ui->wybor_serializacji_comboBox->setEnabled(false);
       // ui->taktowanie_comboBox->setEnabled(false);

        m_centralaSieciowa.setTaktowanie(ui->interwal->value());
        if (ui->taktowanie_comboBox->currentText() == "Jednostronne") {
            m_jednostronny->startSimulation();
        } else if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
            m_obustronny->startSimulation();
        }
        //klient (regulator)
        if (ui->klient_radioButton->isChecked()) {
            if (zegar) {
                zegar->stop();
                zegar->start(ui->interwal->value());

            }
            // W trybie Jednostronnym wyślij START wprost.
            // W trybie Obustronnym startSimulation() wysłałany start przez Komenda_wysyl,
            if (ui->taktowanie_comboBox->currentText() != "Obustronne") {
                PID_wysyl();
                Gen_wysyl();
                Okno_wysyl();
                Komenda_wysyl("START");
            }
        }
        //server(obiekt)
        else if (ui->server_radioButton->isChecked()) {
            if (zegar) {
                zegar->stop();


            }
        }
    }
}

void MainWindow::on_stop_clicked() {
    m_centrala.on_stop_clicked();
    m_centralaSieciowa.on_stop_clicked();
    if (zegar) zegar->stop();
    if (ui->comboBox->currentText() == "Sieciowy" && ui->klient_radioButton->isChecked()) {
        Komenda_wysyl("STOP");
    }
    m_jednostronny->stopSimulation();
    m_obustronny->stopSimulation();


    if (ui->comboBox->currentText() == "Sieciowy") {
        // Odblokowanie kontrolek
        ui->wybor_serializacji_comboBox->setEnabled(true);
        ui->taktowanie_comboBox->setEnabled(true);
    }
}

void MainWindow::on_reset_clicked() {
    m_centrala.on_reset_clicked();
    m_centralaSieciowa.on_reset_clicked();
    if (zegar) zegar->stop();
    
    wyczyscWykresy();

    if (ui->comboBox->currentText() == "Sieciowy" && ui->klient_radioButton->isChecked()) {
        Komenda_wysyl("RESET");
    }
    m_jednostronny->resetSimulation();
    m_obustronny->resetSimulation();

    // Odblokowanie kontrolek
    if (ui->comboBox->currentText() == "Sieciowy") {
        ui->wybor_serializacji_comboBox->setEnabled(true);
        ui->taktowanie_comboBox->setEnabled(true);
    }
}

void MainWindow::on_wzmocnienie_editingFinished() {
    m_centrala.setPID_K(ui->wzmocnienie->value());
    m_centralaSieciowa.setPID_K(ui->wzmocnienie->value());
    PID_wysyl();
    //PID_odbior();
}

void MainWindow::on_stalaI_editingFinished() {
    m_centrala.setPID_Ti(ui->stalaI->value());
    m_centralaSieciowa.setPID_Ti(ui->stalaI->value());
    PID_wysyl();
    //PID_odbior();
}

void MainWindow::on_stalaD_editingFinished() {
    m_centrala.setPID_Td(ui->stalaD->value());
    m_centralaSieciowa.setPID_Td(ui->stalaD->value());
    PID_wysyl();
    //PID_odbior();
}

void MainWindow::on_okres_editingFinished() {
    m_centrala.setOkresGenerator(ui->okres->value());
    m_centralaSieciowa.setOkresGenerator(ui->okres->value());
    Gen_wysyl();
    //Gen_odbior();
}

void MainWindow::on_amplituda_editingFinished() {
    m_centrala.setAmplitudaGenerator(ui->amplituda->value());
    m_centralaSieciowa.setAmplitudaGenerator(ui->amplituda->value());
    Gen_wysyl();
    //Gen_odbior();
}

void MainWindow::on_skladowaStala_editingFinished() {
    m_centrala.setSkladowaStalaGenerator(ui->skladowaStala->value());
    m_centralaSieciowa.setSkladowaStalaGenerator(ui->skladowaStala->value());
    Gen_wysyl();
    //Gen_odbior();
}

void MainWindow::on_wypelnienie_editingFinished() {
    m_centrala.setWypelnienieGenerator(ui->wypelnienie->value());
    m_centralaSieciowa.setWypelnienieGenerator(ui->wypelnienie->value());
    Gen_wysyl();
    //Gen_odbior();
}

void MainWindow::on_interwal_editingFinished() {
    // setTaktowanie wewnętrznie używa set_Tt_zachowujFaze - zachowuje fazę
    // generatora sygnału bez skoku sygnału W przy zmianie interwału.
    m_centrala.setTaktowanie(ui->interwal->value());
    m_centralaSieciowa.setTaktowanie(ui->interwal->value());

    //Obustronny dla klienta
    if (ui->comboBox->currentText() == "Sieciowy" && ui->klient_radioButton->isChecked()) {
        if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
            if (zegar && zegar->isActive()) {
                m_obustronny->handleIntervalCommand(ui->interwal->value(), ui->okno->value(), m_czasSieciowy);
                Interwal_wysyl(m_czasSieciowy);
                // Restart zegara z nowym interwałem bez żadnych resetów stanu
                zegar->stop();
                zegar->start(ui->interwal->value());
                return;
            }
        }
    }

    Interwal_wysyl(m_czasSieciowy);

    if (ui->comboBox->currentText() == "Sieciowy" && ui->klient_radioButton->isChecked()) {
        if (zegar && zegar->isActive()) {
            m_jednostronny->handleIntervalChanged();
            // Restart zegara z nowym interwałem - brak clear stanów i buforów
            zegar->stop();
            zegar->start(ui->interwal->value());
        }
    }
}


void MainWindow::on_rodzajCalkowaniaCombo_currentIndexChanged(const int index) {
    if (index == -1)
        return;

    if (index == 0){
        m_centrala.setPID_TrybCalki(RegulatorPID::SposobLiczeniaCalki::StalaPrzedSuma);
        m_centralaSieciowa.setPID_TrybCalki(RegulatorPID::SposobLiczeniaCalki::StalaPrzedSuma);
    }
    if (index == 1){
        m_centrala.setPID_TrybCalki(RegulatorPID::SposobLiczeniaCalki::StalaPodSuma);
        m_centralaSieciowa.setPID_TrybCalki(RegulatorPID::SposobLiczeniaCalki::StalaPodSuma);
    }
    PID_wysyl();
}

void MainWindow::on_generatorCombo_currentIndexChanged(const int index) {
    if (index == -1)
        return;

    if (index == 0){
        m_centrala.setTypGenerator(TypSyg::Sinusoida);
        m_centralaSieciowa.setTypGenerator(TypSyg::Sinusoida);
    }
    if (index == 1){
        m_centrala.setTypGenerator(TypSyg::Prostokat);
        m_centralaSieciowa.setTypGenerator(TypSyg::Prostokat);
    }
    Gen_wysyl();
}

void MainWindow::on_parametryARX_clicked() {
    UST_ARX okno(this);

    okno.setWspolczynnikiA(m_centrala.getWspolczynnikiA());
    okno.setWspolczynnikiB(m_centrala.getWspolczynnikiB());
    okno.setOpoznienie(m_centrala.getOpoznienieTransportowe());
    okno.setSzum(m_centrala.getWartoscZaklocenia());

    okno.setOgraniczeniaSterowania(m_centrala.isSterowanieWlaczone(),
                                   m_centrala.getMinSterowania(),
                                   m_centrala.getMaxSterowania());
    okno.setOgraniczeniaRegulacji(m_centrala.isRegulacjaWlaczona(),
                                  m_centrala.getMinRegulowania(),
                                  m_centrala.getMaxRegulowania());

    if (ui->klient_radioButton->isChecked()) {
        okno.zablokujEdycje();
    }

    QMetaObject::Connection conn = QObject::connect(&m_hub, &network_hub::odebrano_konfiguracje,
                                                    [&](uint32_t numer, communication_protocol::dane_konfiguracyjne dane) {
                                                        if (!dane.ustawienia_arx.isEmpty()) {
                                                            okno.setWspolczynnikiA(m_centralaSieciowa.getWspolczynnikiA());
                                                            okno.setWspolczynnikiB(m_centralaSieciowa.getWspolczynnikiB());
                                                            okno.setOpoznienie(m_centralaSieciowa.getOpoznienieTransportowe());
                                                            okno.setSzum(m_centralaSieciowa.getWartoscZaklocenia());
                                                            okno.setOgraniczeniaSterowania(m_centralaSieciowa.isSterowanieWlaczone(),
                                                                                           m_centralaSieciowa.getMinSterowania(),
                                                                                           m_centralaSieciowa.getMaxSterowania());
                                                            okno.setOgraniczeniaRegulacji(m_centralaSieciowa.isRegulacjaWlaczona(),
                                                                                          m_centralaSieciowa.getMinRegulowania(),
                                                                                          m_centralaSieciowa.getMaxRegulowania());
                                                        }
                                                    });

    if (okno.exec() == QDialog::Accepted) {
        std::vector<double> noweA = okno.getWspolczynnikiA();
        std::vector<double> noweB = okno.getWspolczynnikiB();
        int noweOpoznienie = okno.getOpoznienie();
        double nowySzum = okno.getSzum();

        m_centrala.zastosujParametryARX(
            noweA, noweB, noweOpoznienie, nowySzum,
            // Sterowanie
            okno.getSterowanieWlaczone(), okno.getMinSterowania(),
            okno.getMaxSterowania(),
            // Regulacja
            okno.getRegulacjaWlaczona(), okno.getMinRegulowania(),
            okno.getMaxRegulowania());

        // Kopiowanie zmian prosto do komunikacji (dwie osobne obiekty m_centrala)
        m_centralaSieciowa.zastosujParametryARX(
            noweA, noweB, noweOpoznienie, nowySzum,
            okno.getSterowanieWlaczone(), okno.getMinSterowania(), okno.getMaxSterowania(),
            okno.getRegulacjaWlaczona(), okno.getMinRegulowania(), okno.getMaxRegulowania());

        // Automatyczne rozełanie ustawień do sieci
        ARX_wysyl();
    }
    QObject::disconnect(conn);
}
#ifndef STARY_CHECKBOX
void MainWindow::on_ograniczenia_checkStateChanged(const Qt::CheckState &arg1) {
    bool wlacz = (arg1 == Qt::Checked);
    m_centrala.setPID_Ograniczenia(wlacz);
    m_centralaSieciowa.setPID_Ograniczenia(wlacz);
    PID_wysyl();
}
#else  // dwa nastepne sloty to fallback do wersji < 6.7
void MainWindow::on_ograniczenia_stateChanged(const int &arg1) {
    bool wlacz = (arg1 == 2);
    m_centrala.setPID_Ograniczenia(wlacz);
    m_centralaSieciowa.setPID_Ograniczenia(wlacz);
    PID_wysyl();
}

#endif // STARY_CHECKBOX

void MainWindow::on_ograniczeniaMin_editingFinished() {
    m_centrala.setPID_ZakresOgraniczen(ui->ograniczeniaMin->value(),
                                       ui->ograniczeniaMax->value());
    m_centralaSieciowa.setPID_ZakresOgraniczen(ui->ograniczeniaMin->value(),
                                              ui->ograniczeniaMax->value());
    PID_wysyl();
}

void MainWindow::on_ograniczeniaMax_editingFinished() {
    on_ograniczeniaMin_editingFinished();
}

void MainWindow::on_okno_editingFinished() {
    m_centrala.set_Okno(ui->okno->value());
    m_centralaSieciowa.set_Okno(ui->okno->value());
    Okno_wysyl();
}

void MainWindow::on_resetCalki_clicked() {
    m_centrala.resetPID_Calka();
    m_centralaSieciowa.resetPID_Calka();
    if (ui->comboBox->currentText() == "Sieciowy") {
        m_resetCalkiWyslij = true;
        PID_wysyl();
    }
}
void MainWindow::on_resetRozniczki_clicked() {
    m_centrala.resetPID_Rozniczka();
    m_centralaSieciowa.resetPID_Rozniczka();
    if (ui->comboBox->currentText() == "Sieciowy") {
        m_resetRozniczkiWyslij = true;
        PID_wysyl();
    }
}
void MainWindow::on_jsonZapisz_clicked() {
    // Wybor pliku
    QString fileName =
        QFileDialog::getSaveFileName(this, "Zapisz parametry do JSON", "",
                                     "JSON Files (*.json);;All Files (*)");

    if (fileName.isEmpty())
        return;

    // Utworzeniie obiektu JSON
    QJsonObject json;

    // Parametry ModelARX
    QJsonObject modelARX;
    // ModelARX* model = m_centrala.getModelARX();

    QJsonArray wspA;
    for (double val : m_centrala.getWspolczynnikiA()) {
        wspA.append(val);
    }
    modelARX["wspolczynnikiA"] = wspA;

    QJsonArray wspB;
    for (double val : m_centrala.getWspolczynnikiB()) {
        wspB.append(val);
    }
    modelARX["wspolczynnikiB"] = wspB;

    modelARX["opoznienieTransportowe"] = m_centrala.getOpoznienieTransportowe();
    modelARX["szum"] = m_centrala.getWartoscZaklocenia();
    modelARX["wlacznikSterowania"] = m_centrala.isSterowanieWlaczone();
    modelARX["minSterowania"] = m_centrala.getMinSterowania();
    modelARX["maxSterowania"] = m_centrala.getMaxSterowania();
    modelARX["wlacznikRegulacji"] = m_centrala.isRegulacjaWlaczona();
    modelARX["minRegulacji"] = m_centrala.getMinRegulowania();
    modelARX["maxRegulacji"] = m_centrala.getMaxRegulowania();

    json["modelARX"] = modelARX;

    // Parametry Regulatora PID
    QJsonObject regulatorPID;
    regulatorPID["K"] = m_centrala.getPID_K();
    regulatorPID["Ti"] = m_centrala.getPID_Ti();
    regulatorPID["Td"] = m_centrala.getPID_Td();
    regulatorPID["trybCalki"] =
        (m_centrala.getPID_TrybCalki() ==
         RegulatorPID::SposobLiczeniaCalki::StalaPrzedSuma)
            ? "Stała przed sumą"
            : "Stała pod sumą";
    regulatorPID["ograniczenia"] = m_centrala.getPID_Ograniczenia();
    regulatorPID["u_min"] = m_centrala.getPID_U_Min();
    regulatorPID["u_max"] = m_centrala.getPID_U_Max();

    json["regulatorPID"] = regulatorPID;

    // Parametry Generatora
    QJsonObject generator;
    generator["typ"] = (m_centrala.getGenerator_Typ() == TypSyg::Sinusoida)
                           ? "Sinusoida"
                           : "Prostokat";
    generator["okres"] = m_centrala.getGenerator_Okres();
    generator["amplituda"] = m_centrala.getGenerator_Amplituda();
    generator["skladowaStala"] = m_centrala.getGenerator_SkladowaStala();
    generator["wypelnienie"] = m_centrala.getGenerator_Wypelnienie();

    json["generator"] = generator;

    // inne parametry
    json["typRegulatora"] =
        (m_centrala.GetRegulator() == ProstyUAR::TypRegulatora::PID);
    json["taktowanie"] = m_centrala.getTaktowanie();
    json["czas"] = ui->okno->text().toDouble();

    // dokument JSON i zapis do pliku
    QJsonDocument doc(json);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
      messageBoxInnyWatek(QMessageBox::Warning, "Błąd",
                                 "Nie można zapisać pliku: " + file.errorString());
        return;
    }

    file.write(doc.toJson());
    file.close();

  messageBoxInnyWatek(QMessageBox::Information, "Sukces",
                             "Parametry zostały zapisane do pliku JSON.");
}

void MainWindow::wczytajSekcjeRegulatora(const QJsonObject &json) {
    // Taktowanie (interwał próbkowania)
    if (json.contains("taktowanie")) {
        int taktowanie = json["taktowanie"].toInt();
        if (taktowanie > 0) {
            m_centrala.setTaktowanie(taktowanie);
            m_centralaSieciowa.setTaktowanie(taktowanie);
            ui->interwal->blockSignals(true);
            ui->interwal->setValue(taktowanie);
            ui->interwal->blockSignals(false);
        }
    }

    // Okno czasowe wykresów
    if (json.contains("czas")) {
        double czas = json["czas"].toDouble();
        if (czas > 0) {
            m_centrala.set_Okno(czas);
            m_centralaSieciowa.set_Okno(czas);
            ui->okno->blockSignals(true);
            ui->okno->setValue(czas);
            ui->okno->blockSignals(false);
        }
    }

    // Generator sygnału (wartości zadanej)
    if (json.contains("generator")) {
        QJsonObject g = json["generator"].toObject();

        if (g.contains("typ")) {
            QString typStr = g["typ"].toString();
            TypSyg typ = (typStr == "Prostokat") ? TypSyg::Prostokat : TypSyg::Sinusoida;
            m_centrala.setTypGenerator(typ);
            m_centralaSieciowa.setTypGenerator(typ);
            ui->generatorCombo->blockSignals(true);
            ui->generatorCombo->setCurrentIndex(typ == TypSyg::Prostokat ? 1 : 0);
            ui->generatorCombo->blockSignals(false);
        }

        if (g.contains("okres")) {
            double v = g["okres"].toDouble();
            m_centrala.setOkresGenerator(v);
            m_centralaSieciowa.setOkresGenerator(v);
            ui->okres->blockSignals(true);
            ui->okres->setValue(v);
            ui->okres->blockSignals(false);
        }

        if (g.contains("amplituda")) {
            double v = g["amplituda"].toDouble();
            m_centrala.setAmplitudaGenerator(v);
            m_centralaSieciowa.setAmplitudaGenerator(v);
            ui->amplituda->blockSignals(true);
            ui->amplituda->setValue(v);
            ui->amplituda->blockSignals(false);
        }

        if (g.contains("skladowaStala")) {
            double v = g["skladowaStala"].toDouble();
            m_centrala.setSkladowaStalaGenerator(v);
            m_centralaSieciowa.setSkladowaStalaGenerator(v);
            ui->skladowaStala->blockSignals(true);
            ui->skladowaStala->setValue(v);
            ui->skladowaStala->blockSignals(false);
        }

        if (g.contains("wypelnienie")) {
            double v = g["wypelnienie"].toDouble();
            m_centrala.setWypelnienieGenerator(v);
            m_centralaSieciowa.setWypelnienieGenerator(v);
            ui->wypelnienie->blockSignals(true);
            ui->wypelnienie->setValue(v);
            ui->wypelnienie->blockSignals(false);
        }
    }

    // Nastawy regulatora PID
    if (json.contains("regulatorPID")) {
        QJsonObject p = json["regulatorPID"].toObject();

        if (p.contains("K")) {
            double v = p["K"].toDouble();
            m_centrala.setPID_K(v);
            m_centralaSieciowa.setPID_K(v);
            ui->wzmocnienie->blockSignals(true);
            ui->wzmocnienie->setValue(v);
            ui->wzmocnienie->blockSignals(false);
        }

        if (p.contains("Ti")) {
            double v = p["Ti"].toDouble();
            m_centrala.setPID_Ti(v);
            m_centralaSieciowa.setPID_Ti(v);
            ui->stalaI->blockSignals(true);
            ui->stalaI->setValue(v);
            ui->stalaI->blockSignals(false);
        }

        if (p.contains("Td")) {
            double v = p["Td"].toDouble();
            m_centrala.setPID_Td(v);
            m_centralaSieciowa.setPID_Td(v);
            ui->stalaD->blockSignals(true);
            ui->stalaD->setValue(v);
            ui->stalaD->blockSignals(false);
        }

        if (p.contains("trybCalki")) {
            QString typCalkiStr = p["trybCalki"].toString();
            auto tryb = (typCalkiStr == "Stała pod sumą")
                            ? RegulatorPID::SposobLiczeniaCalki::StalaPodSuma
                            : RegulatorPID::SposobLiczeniaCalki::StalaPrzedSuma;
            m_centrala.setPID_TrybCalki(tryb);
            m_centralaSieciowa.setPID_TrybCalki(tryb);
            ui->rodzajCalkowaniaCombo->blockSignals(true);
            ui->rodzajCalkowaniaCombo->setCurrentIndex(
                tryb == RegulatorPID::SposobLiczeniaCalki::StalaPodSuma ? 1 : 0);
            ui->rodzajCalkowaniaCombo->blockSignals(false);
        }

        if (p.contains("ograniczenia")) {
            bool ogr = p["ograniczenia"].toBool();
            double uMin = p["u_min"].toDouble();
            double uMax = p["u_max"].toDouble();
            m_centrala.setPID_Ograniczenia(ogr);
            m_centralaSieciowa.setPID_Ograniczenia(ogr);
            m_centrala.setPID_ZakresOgraniczen(uMin, uMax);
            m_centralaSieciowa.setPID_ZakresOgraniczen(uMin, uMax);
            ui->ograniczenia->blockSignals(true);
            ui->ograniczenia->setChecked(ogr);
            ui->ograniczenia->blockSignals(false);
            ui->ograniczeniaMin->blockSignals(true);
            ui->ograniczeniaMin->setValue(uMin);
            ui->ograniczeniaMin->blockSignals(false);
            ui->ograniczeniaMax->blockSignals(true);
            ui->ograniczeniaMax->setValue(uMax);
            ui->ograniczeniaMax->blockSignals(false);
        }
    }

    if (json.contains("typRegulatora")) {
        QString typStr = json["typRegulatora"].toString();
        if (typStr == "PID") {
            m_centrala.SetRegulator(ProstyUAR::TypRegulatora::PID);
            m_centralaSieciowa.SetRegulator(ProstyUAR::TypRegulatora::PID);
        }
    }
}

void MainWindow::wczytajSekcjeObiektu(const QJsonObject &json) {
    if (!json.contains("modelARX")) {
        return;
    }

    QJsonObject aObj = json["modelARX"].toObject();

    auto jsonToVec = [](QJsonValue val) {
        std::vector<double> v;
        for (auto item : val.toArray())
            v.push_back(item.toDouble());
        return v;
    };

    std::vector<double> vA = jsonToVec(aObj["wspolczynnikiA"]);
    std::vector<double> vB = jsonToVec(aObj["wspolczynnikiB"]);
    int opoznienie = aObj["opoznienieTransportowe"].toInt();
    double szum = aObj["szum"].toDouble();

    bool sterWl = aObj["wlacznikSterowania"].toBool();
    double minS = aObj["minSterowania"].toDouble();
    double maxS = aObj["maxSterowania"].toDouble();

    bool regWl = aObj["wlacznikRegulacji"].toBool();
    double minR = aObj["minRegulacji"].toDouble();
    double maxR = aObj["maxRegulacji"].toDouble();

    // Aplikacja parametrów do centrali lokalnej oraz sieciowej
    m_centrala.zastosujParametryARX(vA, vB, opoznienie, szum, sterWl, minS,
                                    maxS, regWl, minR, maxR);
    m_centralaSieciowa.zastosujParametryARX(vA, vB, opoznienie, szum, sterWl, minS,
                                           maxS, regWl, minR, maxR);
}

void MainWindow::on_jsonWczytaj_clicked() {
    QString fName =
        QFileDialog::getOpenFileName(this, "Wczytaj JSON", "", "*.json");
    if (fName.isEmpty())
        return;

    QFile file(fName);
    if (!file.open(QIODevice::ReadOnly)) {
        messageBoxInnyWatek(QMessageBox::Warning, "Błąd", "Nie można otworzyć pliku JSON do odczytu.");
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonObject json = QJsonDocument::fromJson(fileData, &parseError).object();
    if (parseError.error != QJsonParseError::NoError) {
        messageBoxInnyWatek(QMessageBox::Warning, "Błąd JSON", "Błąd parsowania pliku JSON: " + parseError.errorString());
        return;
    }

    bool isSieciowy = (ui->comboBox->currentText() == "Sieciowy");
    bool isRegulator = isSieciowy && ui->klient_radioButton->isChecked();
    bool isObiekt = isSieciowy && ui->server_radioButton->isChecked();

    // powiadom kiedy nie wybrano tryby
    if (isSieciowy && !isRegulator && !isObiekt) {
        messageBoxInnyWatek(QMessageBox::Warning, "Wybór roli wymagany",
                            "Wybierz role (obiekt albo regulator)");
        return;
    }

    if (isRegulator) {
        // Regulator: wczytuje wyłącznie nastawy regulatora, generatora, taktowania i okna
        wczytajSekcjeRegulatora(json);

        // Rozesłanie zaktualizowanej konfiguracji regulatora do obiektu
        PID_wysyl();
        Gen_wysyl();
        Interwal_wysyl(m_czasSieciowy);
        Okno_wysyl();

        if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
            m_obustronny->handleIntervalCommand(m_centralaSieciowa.getTaktowanie(), m_centralaSieciowa.get_Okno(), m_czasSieciowy);
        } else {
            if (zegar && zegar->isActive()) {
                zegar->setInterval(m_centralaSieciowa.getTaktowanie());
            }
        }

        messageBoxInnyWatek(QMessageBox::Information, "Wczytano JSON (Regulator)",
                            "Wczytano nastawy regulatora i generatora. Przesłano konfigurację do obiektu.");
    } else if (isObiekt) {
        // Obiekt: wczytuje wyłącznie nastawy modelu ARX
        wczytajSekcjeObiektu(json);

        // Rozesłanie zaktualizowanej konfiguracji ARX do regulatora
        ARX_wysyl();

        messageBoxInnyWatek(QMessageBox::Information, "Wczytano JSON (Obiekt)",
                            "Wczytano parametry obiektu ARX. Przesłano konfigurację do regulatora.");
    } else {
        // Tryb stacjonarny: wczytanie pełnej konfiguracji
        wczytajSekcjeRegulatora(json);
        wczytajSekcjeObiektu(json);

        messageBoxInnyWatek(QMessageBox::Information, "Wczytano JSON",
                            "Wczytano pełną konfigurację symulacji.");
    }
}

void MainWindow::on_wybor_serializacji_comboBox_currentIndexChanged(int index)
{
    communication_protocol::typ_serializacji typ=(index==0) ? communication_protocol::binarna: communication_protocol::tekstowa;
    m_hub.get_protokol().ustaw_typ_serializacji(typ);
}

void MainWindow::on_taktowanie_comboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if (ui->comboBox->currentText() != "Sieciowy") return;

    qDebug() << "Zmiana trybu taktowania sieciowego na:" << ui->taktowanie_comboBox->currentText();

    if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
        m_obustronny->handleIntervalCommand(m_centralaSieciowa.getTaktowanie(), m_centralaSieciowa.get_Okno(), m_czasSieciowy);
    } else {
        if (zegar && zegar->isActive()) {
            zegar->setInterval(m_centralaSieciowa.getTaktowanie());
        }
    }

    if (ui->klient_radioButton->isChecked()) {
        Interwal_wysyl(m_czasSieciowy);
    }
}
void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    bool wasRunning = false;
    if (arg1 == "Stacjonarny") {
        wylacz_wlacz_kontrolki_sieciowe(false);
        ui->taktowanie_comboBox->setEnabled(false);
        ui->wybor_serializacji_comboBox->setEnabled(false);

        bool zegarAktywny = (zegar && zegar->isActive());
        bool managerAktywny = m_obustronny->isRunning() || m_jednostronny->isRunning();
        wasRunning = zegarAktywny || managerAktywny;
    } else {
        wasRunning = m_centrala.czyTimerAktywny();
    }

    bool b=true, arx=true, pid=true;

    if(arg1=="Sieciowy"){
        if(m_blokujMonitTrybuPracy)
        {
            wylacz_wlacz_kontrolki_sieciowe(b);
            return;
        }

            m_centrala.on_stop_clicked();
            if (zegar) zegar->stop();

            wylacz_wlacz_kontrolki_sieciowe(b);

            //SSynchronizacja stanów po ponownym połączeniu
            m_centralaSieciowa.skopiujStan(m_centrala);
            int interwal=m_centralaSieciowa.getTaktowanie();
            if(interwal >0)
            {
                nr_probki=static_cast<uint32_t>(m_centralaSieciowa.get_Czas()) / (interwal/ 1000.0);
            }
            m_czasSieciowy = m_centralaSieciowa.get_Czas(); //Inicjalizacja skumulowanego czasu sieciowego wartością czasu stacjonarnego
            m_ostatniaProbkaSieciowa = nr_probki;
    }
    else{

            wylacz_wlacz_kontrolki_sieciowe(false);
        ui->taktowanie_comboBox->setEnabled(false);
            ui->wybor_serializacji_comboBox->setEnabled(false);
            ui->klient_radioButton->setChecked(b);
            ui->server_radioButton->setChecked(b);
            wylacz_wlacz_kontrolki(arx, pid);
            wylacz_wlacz_kontrolki_start_stop(true);
            if (zegar) zegar->stop();


            m_jednostronny->stopSimulation();
            m_obustronny->stopSimulation();

            m_hub.rozlacz();
            
            // Pobranie ostatniego sygnału sterującego z sieci przed skopiowaniem
            double u_ostatnie = m_centralaSieciowa.getAktualneSterowanie();
            
            m_centrala.skopiujStan(m_centralaSieciowa);
            //Bezuderzeniowe dostosowanie całki w stacjonarnym
            double e_uchyb = m_centralaSieciowa.getAktualnaWartoscZadana() - m_centralaSieciowa.getAktualnaWartoscRegulowana();
            m_centrala.dostosujCalkeDoU(u_ostatnie, e_uchyb);
            // Przejście bezuderzeniowe: przesunięcie czasu o 1 krok w przód
            m_centrala.set_Czas(m_centrala.get_Czas() + (m_centrala.getTaktowanie() / 1000.0));

            if (wasRunning) {
                m_centrala.on_start_clicked();
            }

    }
}

void MainWindow::Czy_polaczylo()
{
    //Obsługa połączenia bez niszczenia historii wykresów
    if (ui->taktowanie_comboBox->currentText() == "Jednostronne") {
        m_jednostronny->handlePolaczono();
    } else if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
        m_obustronny->handlePolaczono();
    }
    ui->ip_adres_wyswietl_TextEdit->appendPlainText(m_hub.get_serwer_ip());
  messageBoxInnyWatek(QMessageBox::Information, "Info", "Połączono z serwerem");
}


void MainWindow::on_server_radioButton_clicked()
{
    // flaga przed przełączeniem huba (czy_rozlaczylo() pominęło QMessageBox i tryb awaryjny)
    m_blokujMonitTrybuPracy = true;

    if (zegar) zegar->stop();

    m_hub.uruchom_jako_serwer(1235);

    m_blokujMonitTrybuPracy = false;

    QString ip;
    ui->ip_adres_wyswietl_TextEdit->setReadOnly(false);
    ui->ip_adres_wyswietl_TextEdit->setPlainText(ip);
    //ikony
    bool reg=false, arx=true, s=false;
    wylacz_wlacz_kontrolki(reg, arx);
    wylacz_wlacz_kontrolki_start_stop(s);
    ARX_wysyl();
}


void MainWindow::on_klient_radioButton_clicked()
{
    bool ok, reg=true, arx=false, s=true;
    QString text = QInputDialog::getText(this, "Tytuł okna", "Podaj adres IP serwera:", QLineEdit::Normal, "127.0.0.1", &ok);
    if (!ok || text.isEmpty()) {
        return;
    }
    m_blokujMonitTrybuPracy = true;

    if (zegar) zegar->stop();

    m_hub.uruchom_jako_klient(text, 1235);

    m_blokujMonitTrybuPracy = false;
//ikony
    wylacz_wlacz_kontrolki(reg, arx);
    wylacz_wlacz_kontrolki_start_stop(s);

    PID_wysyl();
    Gen_wysyl();
    Interwal_wysyl(m_czasSieciowy);
    Okno_wysyl();
}

void MainWindow::czy_rozlaczylo()
{
    if (ui->comboBox->currentText() != "Sieciowy") {
        return;
    }

    if (m_blokujMonitTrybuPracy) {
        return;
    }

  messageBoxInnyWatek(QMessageBox::Warning, "Błąd połączenia", "Połączenie sieciowe zostało utracone! Przełączono w tryb stacjonarny.");

    if (ui->taktowanie_comboBox->currentText() == "Jednostronne") {
        m_jednostronny->handleRozlaczono();
    } else if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
        m_obustronny->handleRozlaczono();
    }

    //Przełączenie na tryb stacjonarny na obu instancjach w przypadku rozłączenia
    m_blokujMonitTrybuPracy = true;
    ui->comboBox->setCurrentText("Stacjonarny");
    m_blokujMonitTrybuPracy = false;
}

void MainWindow::wylacz_wlacz_kontrolki_sieciowe(bool b)
{
    ui->klient_radioButton->setEnabled(b);
    ui->server_radioButton->setEnabled(b);
    ui->wybor_serializacji_comboBox->setEnabled(b);
    ui->lcdNumber->setEnabled(b);
    ui->taktowanie_comboBox->setEnabled(b);
    ui->ip_adres_wyswietl_TextEdit->setEnabled(b);
    ui->ip_adres_wyswietl_TextEdit->clear();

}
void MainWindow::wylacz_wlacz_kontrolki_start_stop(bool s)
{
    ui->start->setEnabled(s);
    ui->stop->setEnabled(s);
    ui->reset->setEnabled(s);
}

void MainWindow::PID_wysyl()
{
    if(ui->wybor_serializacji_comboBox->currentText()=="Binarna")
    {
        auto dane = PID_do_wyslania_binarnka();
        m_hub.transmituj_konfiguracje(nr_probki, dane);

    }
    else if(ui->wybor_serializacji_comboBox->currentText()=="Tekstowa")
    {
        auto dane = PID_do_wyslania_tekstowe();
        m_hub.transmituj_konfiguracje(nr_probki, dane);
    }
}

void MainWindow::na_odebrano_konfiguracje(uint32_t numer, communication_protocol::dane_konfiguracyjne dane)
{
    if (!dane.ustawienia_pid.isEmpty()) {
        if(ui->wybor_serializacji_comboBox->currentText()=="Binarna") PID_odbior_binarnka(dane);
        else if(ui->wybor_serializacji_comboBox->currentText()=="Tekstowa") PID_odbior_tekstowe(dane);

        ui->wzmocnienie->blockSignals(true); ui->stalaD->blockSignals(true); ui->stalaI->blockSignals(true);
        ui->rodzajCalkowaniaCombo->blockSignals(true);
        ui->ograniczenia->blockSignals(true); ui->ograniczeniaMin->blockSignals(true); ui->ograniczeniaMax->blockSignals(true);

        ui->wzmocnienie->setValue(m_centralaSieciowa.getPID_K());
        ui->stalaD->setValue(m_centralaSieciowa.getPID_Td());
        ui->stalaI->setValue(m_centralaSieciowa.getPID_Ti());
        ui->rodzajCalkowaniaCombo->setCurrentIndex((int)m_centralaSieciowa.getPID_TrybCalki());
        ui->ograniczenia->setChecked(m_centralaSieciowa.getPID_Ograniczenia());
        ui->ograniczeniaMin->setValue(m_centralaSieciowa.getPID_U_Min());
        ui->ograniczeniaMax->setValue(m_centralaSieciowa.getPID_U_Max());

        ui->wzmocnienie->blockSignals(false); ui->stalaD->blockSignals(false); ui->stalaI->blockSignals(false);
        ui->rodzajCalkowaniaCombo->blockSignals(false);
        ui->ograniczenia->blockSignals(false); ui->ograniczeniaMin->blockSignals(false); ui->ograniczeniaMax->blockSignals(false);

        // Synchronizacja param lokalnej centrali
        m_centrala.setPID_K(m_centralaSieciowa.getPID_K());
        m_centrala.setPID_Ti(m_centralaSieciowa.getPID_Ti());
        m_centrala.setPID_Td(m_centralaSieciowa.getPID_Td());
        m_centrala.setPID_TrybCalki(m_centralaSieciowa.getPID_TrybCalki());
        m_centrala.setPID_Ograniczenia(m_centralaSieciowa.getPID_Ograniczenia());
        m_centrala.setPID_ZakresOgraniczen(m_centralaSieciowa.getPID_U_Min(), m_centralaSieciowa.getPID_U_Max());
    }


    if (!dane.ustawienia_arx.isEmpty()) {
        if(ui->wybor_serializacji_comboBox->currentText()=="Binarna") ARX_odbior_binarnka(dane);
        else if(ui->wybor_serializacji_comboBox->currentText()=="Tekstowa") ARX_odbior_tekstowe(dane);
        // Synchronizacja param lokalnej centrali

        m_centrala.zastosujParametryARX(
            m_centralaSieciowa.getWspolczynnikiA(),
            m_centralaSieciowa.getWspolczynnikiB(),
            m_centralaSieciowa.getOpoznienieTransportowe(),
            m_centralaSieciowa.getWartoscZaklocenia(),
            m_centralaSieciowa.isSterowanieWlaczone(),
            m_centralaSieciowa.getMinSterowania(),
            m_centralaSieciowa.getMaxSterowania(),
            m_centralaSieciowa.isRegulacjaWlaczona(),
            m_centralaSieciowa.getMinRegulowania(),
            m_centralaSieciowa.getMaxRegulowania()
            );

    }

    if (!dane.ustawienia_generatora.isEmpty()) {
        if(ui->wybor_serializacji_comboBox->currentText()=="Binarna") generator_odbior_binarnka(dane);
        else if(ui->wybor_serializacji_comboBox->currentText()=="Tekstowa") generator_odbior_tekstowe(dane);

        ui->amplituda->blockSignals(true); ui->okres->blockSignals(true); ui->skladowaStala->blockSignals(true);
        ui->wypelnienie->blockSignals(true); ui->generatorCombo->blockSignals(true);
        ui->amplituda->setValue(m_centralaSieciowa.getGenerator_Amplituda());
        ui->okres->setValue(m_centralaSieciowa.getGenerator_Okres());
        ui->skladowaStala->setValue(m_centralaSieciowa.getGenerator_SkladowaStala());
        ui->wypelnienie->setValue(m_centralaSieciowa.getGenerator_Wypelnienie());
        if (m_centralaSieciowa.getGenerator_Typ() == TypSyg::Sinusoida) ui->generatorCombo->setCurrentIndex(0);
        else if (m_centralaSieciowa.getGenerator_Typ() == TypSyg::Prostokat) ui->generatorCombo->setCurrentIndex(1);
        ui->amplituda->blockSignals(false); ui->okres->blockSignals(false); ui->skladowaStala->blockSignals(false);
        ui->wypelnienie->blockSignals(false); ui->generatorCombo->blockSignals(false);

        // Synchronizacja param lokalnej centrali

        m_centrala.setTypGenerator(m_centralaSieciowa.getGenerator_Typ());
        m_centrala.setOkresGenerator(m_centralaSieciowa.getGenerator_Okres());
        m_centrala.setAmplitudaGenerator(m_centralaSieciowa.getGenerator_Amplituda());
        m_centrala.setSkladowaStalaGenerator(m_centralaSieciowa.getGenerator_SkladowaStala());
        m_centrala.setWypelnienieGenerator(m_centralaSieciowa.getGenerator_Wypelnienie());
    }

}

void MainWindow::ARX_wysyl()
{
    if(ui->wybor_serializacji_comboBox->currentText()=="Binarna")
    {
        auto dane = ARX_do_wyslania_binarnka();
        m_hub.transmituj_konfiguracje(nr_probki, dane);
    }
    else if(ui->wybor_serializacji_comboBox->currentText()=="Tekstowa")
    {
        auto dane = ARX_do_wyslania_tekstowe();
        m_hub.transmituj_konfiguracje(nr_probki, dane);
    }
}
void MainWindow::Gen_wysyl()
{
    if(ui->wybor_serializacji_comboBox->currentText()=="Binarna")
    {
        auto dane = Generator_do_wyslania_binarnka();
        m_hub.transmituj_konfiguracje(nr_probki, dane);
    }
    else if(ui->wybor_serializacji_comboBox->currentText()=="Tekstowa")
    {
        auto dane = Generator_do_wyslania_tekstowe();
        m_hub.transmituj_konfiguracje(nr_probki, dane);
    }
}
void MainWindow::wylacz_wlacz_kontrolki(bool reg, bool arx)
{
    //kontrola sym
    ui->okno->setEnabled(reg);
    ui->interwal->setEnabled(reg);
    //gen
    ui->generatorCombo->setEnabled(reg);
    ui->amplituda->setEnabled(reg);
    ui->okres->setEnabled(reg);
    ui->wypelnienie->setEnabled(reg);
    ui->skladowaStala->setEnabled(reg);
    //PID
    ui->wzmocnienie->setEnabled(reg);
    ui->stalaI->setEnabled(reg);
    ui->stalaD->setEnabled(reg);
    //reset I && D
    ui->resetRozniczki->setEnabled(reg);
    ui->resetCalki->setEnabled(reg);
    //calka
    ui->rodzajCalkowaniaCombo->setEnabled(reg);
    //ograniczenia
    ui->ograniczenia->setEnabled(reg);
    ui->ograniczeniaMax->setEnabled(reg);
    ui->ograniczeniaMin->setEnabled(reg);
    //arx
    ui->parametryARX->setEnabled(arx);
}
void MainWindow::na_odebrano_symulacje(uint32_t numer, communication_protocol::dane_symulacji dane)
{
    if (ui->taktowanie_comboBox->currentText() == "Jednostronne") {
        m_jednostronny->handleOdebranoSymulacje(numer, dane);
        return;
    }
    if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
        m_obustronny->handleOdebranoSymulacje(numer, dane);
        return;
    }
    ostatni_numer_sieciowy = numer;
    // Kontrola synchronizacji numerów próbek
    bool zsynchronizowane = m_hub.get_protokol().czy_zsynchronizowane(nr_probki, numer, 3);
    if (!zsynchronizowane && nr_probki > 0 && numer > 0) {
        ui->label_sync_status->setText(QString("Desync! Lok:%1 Sieć:%2").arg(nr_probki).arg(numer));
        ui->label_sync_status->setStyleSheet("color: orange; font-weight: bold;");
    }

    if (ui->klient_radioButton->isChecked()) {
        m_centralaSieciowa.odbierzY(dane.wartosc_regulowana);

        // przerwanie polaczenia sieciowego
        m_centralaSieciowa.getProstyUAR().aktualizujTloARX(dane.sygnal_sterujacy, dane.wartosc_regulowana);

        // w t=k*T->rysowanie wykresów dla obu trybów na tej podstaw3ie
        if (ui->taktowanie_comboBox->currentText() == "Jednostronne") {
            if (numer == 0) {
                m_czasSieciowy = 0.0;
                m_ostatniaProbkaSieciowa = 0;
                double T = m_centralaSieciowa.getTaktowanie() / 1000.0;
                double dt = (numer - m_ostatniaProbkaSieciowa) * T;
                if (dt > 0) {
                    m_czasSieciowy += dt;
                }
                m_ostatniaProbkaSieciowa = numer;
            }
            m_centralaSieciowa.set_Czas(m_czasSieciowy);
            //emitujWykres() zamiast zaktualizujWykres()
            m_centralaSieciowa.emitujWykres();

        } else if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
            if (numer == 0) {
                m_czasSieciowy = 0.0;
                m_ostatniaProbkaSieciowa = 0;
            } else {
                double T = m_centralaSieciowa.getTaktowanie() / 1000.0;
                double dt = (numer - m_ostatniaProbkaSieciowa) * T;
                if (dt > 0) {
                    m_czasSieciowy += dt;
                }
                m_ostatniaProbkaSieciowa = numer;
            }
            m_centralaSieciowa.set_Czas(m_czasSieciowy);
            m_centralaSieciowa.emitujWykres();
        }
    } else if (ui->server_radioButton->isChecked()) {
        m_centralaSieciowa.odbierzU(dane.sygnal_sterujacy);
        m_centralaSieciowa.odbierzW(dane.wartosc_zadana);

        if (ui->taktowanie_comboBox->currentText() == "Jednostronne" || ui->taktowanie_comboBox->currentText() == "Obustronne") {
            if (numer == 0) {
                m_czasSieciowy = 0.0;
                m_ostatniaProbkaSieciowa = 0;
            } else {
                double T = m_centralaSieciowa.getTaktowanie() / 1000.0;
                double dt = (numer - m_ostatniaProbkaSieciowa) * T;
                if (dt > 0) {
                    m_czasSieciowy += dt;
                }
                m_ostatniaProbkaSieciowa = numer;
            }
            m_centralaSieciowa.set_Czas(m_czasSieciowy);
            
            m_centralaSieciowa.wykonajKrokSerwera();
            //dodane na potrzeby przerwania polaczenia sieciowego -> obliczenia w tle
            m_centralaSieciowa.getProstyUAR().aktualizujTloPID(dane.wartosc_zadana, dane.wartosc_regulowana);

            // Synchronizacja licznika próbek generatora na serwerze
            m_centralaSieciowa.getGenerator().setLicznikProbki(numer + 1);

            double y = m_centralaSieciowa.getAktualnaWartoscRegulowana();
            // nr_probki zaktualizowany przed wysłaniem y
            nr_probki = numer + 1;
            wyslij_wyjscie_przez_siec(y);
            m_centralaSieciowa.emitujWykres();

            if (ui->taktowanie_comboBox->currentText() == "Obustronne") {

                m_pierwszaProbkaPoStart = false;
                if (zegar) {
                    int interwal = m_centralaSieciowa.getTaktowanie();
                    zegar->start(interwal);

                }
            }
        }
    }
}

void MainWindow::on_zegar_timeout()
{
    if (ui->comboBox->currentText() != "Sieciowy") {
        m_centralaSieciowa.wykonajKrokSymulacji();
        return;
    }

    if (ui->taktowanie_comboBox->currentText() == "Jednostronne") {
        m_jednostronny->handleTimerTimeout();
        return;
    }

    if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
        m_obustronny->handleMasterTimerTimeout();
        return;
    }

    if (ui->klient_radioButton->isChecked()) {
        if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
            if (nr_probki == 0) {
                m_czasSieciowy = 0.0;
                m_ostatniaProbkaSieciowa = 0;
                m_punktySeriiWartosciZadanej.clear();
                m_punktySeriiWartosciRegulowanej.clear();
                m_punktySeriiUchybu.clear();
                m_punktySeriiSterowania.clear();
                m_punktySeriiSkladowejP.clear();
                m_punktySeriiSkladowejI.clear();
                m_punktySeriiSkladowejD.clear();
                m_ostatnieY1.clear();
                m_ostatnieY2.clear();
                m_ostatnieY3.clear();
                m_ostatnieY4.clear();
                m_poprzOkno = 0.0;
            } else {
            }

            m_centralaSieciowa.wykonajKrokKlienta();
            double u = m_centralaSieciowa.getAktualneSterowanie();
            wyslij_sterowanie_przez_siec(u);
            // Wykres klienta w trybie Obustr jest aktualizowany w na_odebrano_symulacje
            nr_probki++;

        } else {
            // Jednostronne
            m_centralaSieciowa.wykonajKrokKlienta();
            double u = m_centralaSieciowa.getAktualneSterowanie();
            wyslij_sterowanie_przez_siec(u);
            nr_probki++;
        }
    }
    else if (ui->server_radioButton->isChecked()) {
        if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
            return;
        }
    }
}
void MainWindow::wyslij_sterowanie_przez_siec(double u) {

    double w = m_centralaSieciowa.getAktualnaWartoscZadana();
    double y = m_centralaSieciowa.getAktualnaWartoscRegulowana();

    m_hub.wyslij_ramke(nr_probki, u, y, w, m_czasSieciowy);
}

void MainWindow::wyslij_wyjscie_przez_siec(double y) {
    double u = m_centralaSieciowa.getAktualneSterowanie();

    m_hub.wyslij_ramke(nr_probki, u, y, 0.0, m_czasSieciowy);
}
void MainWindow::Interwal_wysyl(double t0)
{
    communication_protocol::dane_sterujace dane;
    if(ui->wybor_serializacji_comboBox->currentText()=="Binarna") {
        dane = Interwal_do_wyslania_binarnka(t0);
    } else {
        dane = Interwal_do_wyslania_tekstowe(t0);
    }
    m_hub.transmituj_komende(nr_probki, dane);
}
void MainWindow::Komenda_wysyl(QString komenda, double t0)
{
    communication_protocol::dane_sterujace dane;
    if(ui->wybor_serializacji_comboBox->currentText()=="Binarna") {
        dane = Komenda_do_wyslania_binarnka(komenda, t0);

    } else {
        dane = Komenda_do_wyslania_tekstowe(komenda, t0);

    }
    m_hub.transmituj_komende(nr_probki, dane);
}
void MainWindow::Okno_wysyl()
{
    communication_protocol::dane_sterujace dane;
    if(ui->wybor_serializacji_comboBox->currentText()=="Binarna") {
        dane = Okno_do_wyslania_binarnka();
    } else {
        dane = Okno_do_wyslania_tekstowe();
    }
    m_hub.transmituj_komende(nr_probki, dane);
}

void MainWindow::na_odebrano_sterowanie(uint32_t numer, communication_protocol::dane_sterujace dane)
{
    int stary_interwal = m_centralaSieciowa.getTaktowanie();
    double stare_okno = m_centralaSieciowa.get_Okno();
    double t0 = 0.0;
    QString polecenieStr;

    if (ui->wybor_serializacji_comboBox->currentText()=="Binarna") {
        QByteArray ramka = QByteArray::fromBase64(dane.polecenie.toLatin1());
        QDataStream strumien(ramka);
        int intervalVal = 0;
        qint32 oknoVal = 0;
        strumien >> polecenieStr >> intervalVal >> oknoVal;
        if (!strumien.atEnd()) {
            strumien >> t0;
        }
        if (polecenieStr != "okno") {
            m_centralaSieciowa.setTaktowanie(intervalVal);
        }
        if (oknoVal > 0) {
            m_centralaSieciowa.set_Okno(oknoVal);
            m_centrala.set_Okno(oknoVal);
        }
    } else {
        QJsonDocument doc = QJsonDocument::fromJson(dane.polecenie.toUtf8());
        if (!doc.isNull() && doc.object().contains("komenda")) {
            polecenieStr = doc.object()["komenda"].toString();
            if (doc.object().contains("wartosc") && polecenieStr != "okno") {
                m_centralaSieciowa.setTaktowanie(doc.object()["wartosc"].toInt());
            }
            if (doc.object().contains("okno")) {
                double oknoVal = doc.object()["okno"].toDouble();
                if (oknoVal > 0) {
                    m_centralaSieciowa.set_Okno(oknoVal);
                    m_centrala.set_Okno(oknoVal);
                }
            }
            if (doc.object().contains("t0")) {
                t0 = doc.object()["t0"].toDouble();
            }
        }
    }

    // blockSignals zapobiega pętlom zwrotnym
    if (polecenieStr == "interwal" || (polecenieStr != "okno" && stary_interwal != m_centralaSieciowa.getTaktowanie())) {
        ui->interwal->blockSignals(true);
        ui->interwal->setValue(m_centralaSieciowa.getTaktowanie());
        ui->interwal->blockSignals(false);
        if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
            m_obustronny->handleIntervalCommand(m_centralaSieciowa.getTaktowanie(), m_centralaSieciowa.get_Okno(), t0);
        } else {
            if(zegar && zegar->isActive()) {
                zegar->setInterval(m_centralaSieciowa.getTaktowanie());
            }
        }
    }

    if(stare_okno != m_centralaSieciowa.get_Okno() || polecenieStr == "okno") {
        ui->okno->blockSignals(true);
        ui->okno->setValue(m_centralaSieciowa.get_Okno());
        ui->okno->blockSignals(false);
        m_centrala.set_Okno(m_centralaSieciowa.get_Okno());
    }

    if (ui->server_radioButton->isChecked()) {
        if (polecenieStr == "START") {
            m_pierwszaProbkaPoStart = true;
            if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
                m_obustronny->handleStartCommand(t0, numer);
            } else if (ui->taktowanie_comboBox->currentText() == "Jednostronne") {
                m_jednostronny->handleStartCommand(t0, numer);
            }
        } else if (polecenieStr == "STOP") {
            if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
                m_obustronny->handleStopCommand();
            } else if (ui->taktowanie_comboBox->currentText() == "Jednostronne") {
                m_jednostronny->handleStopCommand();
            }
        } else if (polecenieStr == "RESET") {
            if (ui->taktowanie_comboBox->currentText() == "Obustronne") {
                m_obustronny->handleResetCommand();
            } else if (ui->taktowanie_comboBox->currentText() == "Jednostronne") {
                m_jednostronny->handleResetCommand();
            } else {
                on_reset_clicked();
            }
        }
    }
}

// Tekstowe
communication_protocol::dane_konfiguracyjne MainWindow::PID_do_wyslania_tekstowe()
{
    communication_protocol::dane_konfiguracyjne dane;

    QJsonObject pid;
    pid["p"] = m_centralaSieciowa.getPID_K();
    pid["i"] = m_centralaSieciowa.getPID_Ti();
    pid["d"] = m_centralaSieciowa.getPID_Td();
    pid["trybCalki"] = (int)m_centralaSieciowa.getPID_TrybCalki();
    pid["ograniczenia"] = m_centralaSieciowa.getPID_Ograniczenia();
    pid["uMin"] = m_centralaSieciowa.getPID_U_Min();
    pid["uMax"] = m_centralaSieciowa.getPID_U_Max();
    pid["resetCalki"] = m_resetCalkiWyslij;
    pid["resetRozniczki"] = m_resetRozniczkiWyslij;

    m_resetCalkiWyslij = false;
    m_resetRozniczkiWyslij = false;

    dane.ustawienia_pid = QString(QJsonDocument(pid).toJson(QJsonDocument::Compact));

    return dane;
}

void MainWindow::PID_odbior_tekstowe(const communication_protocol::dane_konfiguracyjne &dane)
{
    QJsonDocument docPid = QJsonDocument::fromJson(dane.ustawienia_pid.toUtf8());
    QJsonObject pid = docPid.object();
    m_centralaSieciowa.setPID_K(pid["p"].toDouble());
    m_centralaSieciowa.setPID_Ti(pid["i"].toDouble());
    m_centralaSieciowa.setPID_Td(pid["d"].toDouble());
    if (pid.contains("trybCalki")) {
        m_centralaSieciowa.setPID_TrybCalki((RegulatorPID::SposobLiczeniaCalki)pid["trybCalki"].toInt());
    }
    if (pid.contains("ograniczenia")) {
        m_centralaSieciowa.setPID_Ograniczenia(pid["ograniczenia"].toBool());
        m_centralaSieciowa.setPID_ZakresOgraniczen(pid["uMin"].toDouble(), pid["uMax"].toDouble());
    }
    if (pid.contains("resetCalki") && pid["resetCalki"].toBool()) {
        m_centralaSieciowa.resetPID_Calka();
        m_centrala.resetPID_Calka();
    }
    if (pid.contains("resetRozniczki") && pid["resetRozniczki"].toBool()) {
        m_centralaSieciowa.resetPID_Rozniczka();
        m_centrala.resetPID_Rozniczka();
    }
}

communication_protocol::dane_konfiguracyjne MainWindow::ARX_do_wyslania_tekstowe()
{
    communication_protocol::dane_konfiguracyjne dane;

    QJsonObject arx;
    QJsonArray wsp_a;
    QJsonArray wsp_b;

    for (double val : m_centralaSieciowa.getWspolczynnikiA()) {
        wsp_a.append(val);
    }
    for (double val : m_centralaSieciowa.getWspolczynnikiB()) {
        wsp_b.append(val);
    }

    arx["wspolczynnikiA"] = wsp_a;
    arx["wspolczynnikiB"] = wsp_b;
    arx["opoznienie"] = m_centralaSieciowa.getOpoznienieTransportowe();
    arx["szumy"] = m_centralaSieciowa.getWartoscZaklocenia();
    arx["minSter"] = m_centralaSieciowa.getMinSterowania();
    arx["maxSter"] = m_centralaSieciowa.getMaxSterowania();
    arx["minReg"] = m_centralaSieciowa.getMinRegulowania();
    arx["maxReg"] = m_centralaSieciowa.getMaxRegulowania();
    arx["isSterowanieWlaczone"] = m_centralaSieciowa.isSterowanieWlaczone();
    arx["isRegulacjaWlaczona"] = m_centralaSieciowa.isRegulacjaWlaczona();

    dane.ustawienia_arx = QString(QJsonDocument(arx).toJson(QJsonDocument::Compact));
    return dane;
}

void MainWindow::ARX_odbior_tekstowe(const communication_protocol::dane_konfiguracyjne &dane)
{
    QJsonDocument docArx = QJsonDocument::fromJson(dane.ustawienia_arx.toUtf8());
    QJsonObject arx = docArx.object();

    QJsonArray wsp_a_odbior = arx["wspolczynnikiA"].toArray();
    std::vector<double> A;
    for (QJsonValue val : wsp_a_odbior) {
        A.push_back(val.toDouble());
    }

    QJsonArray wsp_b_odbior = arx["wspolczynnikiB"].toArray();
    std::vector<double> B;
    for (QJsonValue val : wsp_b_odbior) {
        B.push_back(val.toDouble());
    }

    int opoznienie = arx["opoznienie"].toInt();
    double szumy = arx["szumy"].toDouble();
    double min_sterowanie = arx["minSter"].toDouble();
    double max_sterowanie = arx["maxSter"].toDouble();
    double min_regulowana = arx["minReg"].toDouble();
    double max_regulowana = arx["maxReg"].toDouble();
    bool is_ster_wlacz = arx["isSterowanieWlaczone"].toBool();
    bool is_reg_wlacz = arx["isRegulacjaWlaczona"].toBool();

    m_centralaSieciowa.zastosujParametryARX(A, B, opoznienie, szumy, is_ster_wlacz, min_sterowanie, max_sterowanie, is_reg_wlacz, min_regulowana, max_regulowana);
}

communication_protocol::dane_konfiguracyjne MainWindow::Generator_do_wyslania_tekstowe()
{
    communication_protocol::dane_konfiguracyjne dane;

    QJsonObject gen;

    gen["getGenerator_Amplituda"]=m_centralaSieciowa.getGenerator_Amplituda();
    gen["getGenerator_Okres"]=m_centralaSieciowa.getGenerator_Okres();
    gen["getGenerator_SkladowaStala"]=m_centralaSieciowa.getGenerator_SkladowaStala();
    gen["getGenerator_Wypelnienie"]=m_centralaSieciowa.getGenerator_Wypelnienie();
    gen["getGenerator_Typ"] = (int)m_centralaSieciowa.getGenerator_Typ();
    dane.ustawienia_generatora=QString(QJsonDocument(gen).toJson(QJsonDocument::Compact));
    return dane;
}

void MainWindow::generator_odbior_tekstowe(const communication_protocol::dane_konfiguracyjne &dane)
{
    QJsonDocument docGen = QJsonDocument::fromJson(dane.ustawienia_generatora.toUtf8());
    QJsonObject gen = docGen.object();

    m_centralaSieciowa.setTypGenerator((TypSyg)gen["getGenerator_Typ"].toInt());
    m_centralaSieciowa.setWypelnienieGenerator( gen["getGenerator_Wypelnienie"].toDouble());
    m_centralaSieciowa.setAmplitudaGenerator(gen["getGenerator_Amplituda"].toDouble());
    m_centralaSieciowa.setSkladowaStalaGenerator( gen["getGenerator_SkladowaStala"].toDouble());
    m_centralaSieciowa.setOkresGenerator(gen["getGenerator_Okres"].toDouble());
}

communication_protocol::dane_sterujace MainWindow::Interwal_do_wyslania_tekstowe(double t0)
{
    communication_protocol::dane_sterujace dane;
    QJsonObject obj;
    obj["komenda"] = "interwal";
    obj["wartosc"] = m_centralaSieciowa.getTaktowanie();
    //obj["okno"] = static_cast<qint32>(m_centralaSieciowa.get_Okno());
    obj["okno"]=m_centralaSieciowa.get_Okno();
    obj["t0"] = t0;
    dane.polecenie = QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));

    dane.interwal = m_centralaSieciowa.getTaktowanie();
    dane.okno = static_cast<qint32>(m_centralaSieciowa.get_Okno());
    return dane;
}

void MainWindow::Interwal_odbior_tekstowe(const communication_protocol::dane_sterujace &dane)
{
    QJsonDocument doc = QJsonDocument::fromJson(dane.polecenie.toUtf8());
    if(!doc.isNull() && doc.object().contains("komenda") && doc.object()["komenda"].toString() == "interwal") {
        m_centralaSieciowa.setTaktowanie(doc.object()["wartosc"].toInt());
        if(doc.object().contains("okno")) {
            m_centralaSieciowa.set_Okno(doc.object()["okno"].toInt());
        }
    } else {
        if (dane.polecenie == "interwal") {
            m_centralaSieciowa.setTaktowanie(dane.interwal);
            m_centralaSieciowa.set_Okno(dane.okno);
        }
    }
}

communication_protocol::dane_sterujace MainWindow::Okno_do_wyslania_tekstowe()
{
    communication_protocol::dane_sterujace dane;
    QJsonObject obj;
    obj["komenda"] = "okno";
    obj["wartosc"] = m_centralaSieciowa.getTaktowanie();
    obj["okno"] = m_centralaSieciowa.get_Okno();
    obj["t0"] = m_czasSieciowy;
    dane.polecenie = QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));

    dane.interwal = m_centralaSieciowa.getTaktowanie();
    dane.okno = static_cast<qint32>(m_centralaSieciowa.get_Okno());
    return dane;
}

void MainWindow::Okno_odbior_tekstowe(const communication_protocol::dane_sterujace &dane)
{
    QJsonDocument doc = QJsonDocument::fromJson(dane.polecenie.toUtf8());
    if(!doc.isNull() && doc.object().contains("komenda") && doc.object()["komenda"].toString() == "okno") {
        if(doc.object().contains("okno")) {
            double oknoVal = doc.object()["okno"].toDouble();
            if (oknoVal > 0) {
                m_centralaSieciowa.set_Okno(oknoVal);
                m_centrala.set_Okno(oknoVal);
            }
        }
    } else {
        if (dane.polecenie == "okno" && dane.okno > 0) {
            m_centralaSieciowa.set_Okno(dane.okno);
            m_centrala.set_Okno(dane.okno);
        }
    }
}

communication_protocol::dane_sterujace MainWindow::Komenda_do_wyslania_tekstowe(QString komenda, double t0)
{
    communication_protocol::dane_sterujace dane;
    QJsonObject obj;
    obj["komenda"] = komenda;
    obj["wartosc"] = m_centralaSieciowa.getTaktowanie();
    obj["okno"] = static_cast<qint32>(m_centralaSieciowa.get_Okno());
    obj["t0"] = t0;
    dane.polecenie = QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));

    dane.interwal = m_centralaSieciowa.getTaktowanie();
    dane.okno = static_cast<qint32>(m_centralaSieciowa.get_Okno());
    return dane;
}
//BINARKA
communication_protocol::dane_sterujace MainWindow::Komenda_do_wyslania_binarnka(QString komenda, double t0)
{
    communication_protocol::dane_sterujace dane;
    QByteArray ramka;
    QDataStream strumien(&ramka, QIODevice::WriteOnly);
    strumien << komenda << m_centralaSieciowa.getTaktowanie() << static_cast<qint32>(m_centralaSieciowa.get_Okno()) << t0;

    dane.polecenie = QString::fromLatin1(ramka.toBase64());
    dane.interwal = m_centralaSieciowa.getTaktowanie();
    dane.okno = static_cast<qint32>(m_centralaSieciowa.get_Okno());
    return dane;
}

communication_protocol::dane_konfiguracyjne MainWindow::PID_do_wyslania_binarnka()
{
    communication_protocol::dane_konfiguracyjne dane;

    QByteArray ramka_PID;
    QDataStream stream_PID(&ramka_PID, QIODevice::WriteOnly);

    stream_PID << m_centralaSieciowa.getPID_K();
    stream_PID << m_centralaSieciowa.getPID_Ti();
    stream_PID << m_centralaSieciowa.getPID_Td();
    stream_PID << (qint32)m_centralaSieciowa.getPID_TrybCalki();
    stream_PID << m_centralaSieciowa.getPID_Ograniczenia();
    stream_PID << m_centralaSieciowa.getPID_U_Min();
    stream_PID << m_centralaSieciowa.getPID_U_Max();
    stream_PID << m_resetCalkiWyslij;
    stream_PID << m_resetRozniczkiWyslij;
    m_resetCalkiWyslij = false;
    m_resetRozniczkiWyslij = false;

    dane.ustawienia_pid = QString::fromLatin1(ramka_PID.toBase64());
    return dane;
}

void MainWindow::PID_odbior_binarnka(const communication_protocol::dane_konfiguracyjne &dane)
{
    QByteArray ramka_Pid = QByteArray::fromBase64(dane.ustawienia_pid.toLatin1());
    QDataStream stream_Pid(ramka_Pid);

    double p, i, d;
    stream_Pid >> p;
    stream_Pid >> i;
    stream_Pid >> d;

    m_centralaSieciowa.setPID_K(p);
    m_centralaSieciowa.setPID_Ti(i);
    m_centralaSieciowa.setPID_Td(d);

    if (!stream_Pid.atEnd()) {
        qint32 trybCalkiInt;
        bool ograniczenia;
        double uMin, uMax;
        stream_Pid >> trybCalkiInt;
        stream_Pid >> ograniczenia;
        stream_Pid >> uMin;
        stream_Pid >> uMax;

        m_centralaSieciowa.setPID_TrybCalki((RegulatorPID::SposobLiczeniaCalki)trybCalkiInt);
        m_centralaSieciowa.setPID_Ograniczenia(ograniczenia);
        m_centralaSieciowa.setPID_ZakresOgraniczen(uMin, uMax);
    }

    if (!stream_Pid.atEnd()) {
        bool resetCalki = false;
        stream_Pid >> resetCalki;
        if (resetCalki) {
            m_centralaSieciowa.resetPID_Calka();
            m_centrala.resetPID_Calka();
        }
    }
    if (!stream_Pid.atEnd()) {
        bool resetRozniczki = false;
        stream_Pid >> resetRozniczki;
        if (resetRozniczki) {
            m_centralaSieciowa.resetPID_Rozniczka();
            m_centrala.resetPID_Rozniczka();
        }
    }
}

communication_protocol::dane_konfiguracyjne MainWindow::ARX_do_wyslania_binarnka()
{
    communication_protocol::dane_konfiguracyjne dane;

    QByteArray ramka_ARX;
    QDataStream stream_ARX(&ramka_ARX, QIODevice::WriteOnly);

    stream_ARX << m_centralaSieciowa.getWartoscZaklocenia();
    stream_ARX << m_centralaSieciowa.getOpoznienieTransportowe();
    stream_ARX << m_centralaSieciowa.isRegulacjaWlaczona();
    stream_ARX << m_centralaSieciowa.isSterowanieWlaczone();
    stream_ARX << m_centralaSieciowa.getMaxRegulowania();
    stream_ARX << m_centralaSieciowa.getMinRegulowania();
    stream_ARX << m_centralaSieciowa.getMaxSterowania();
    stream_ARX << m_centralaSieciowa.getMinSterowania();

    std::vector<double> A = m_centralaSieciowa.getWspolczynnikiA();
    stream_ARX << (quint32)A.size();
    for(double val: A) {
        stream_ARX << val;
    }

    std::vector<double> B = m_centralaSieciowa.getWspolczynnikiB();
    stream_ARX << (quint32)B.size();
    for(double val: B) {
        stream_ARX << val;
    }

    dane.ustawienia_arx=QString::fromLatin1((ramka_ARX.toBase64()));
    return dane;
}

void MainWindow::ARX_odbior_binarnka(const communication_protocol::dane_konfiguracyjne &dane)
{
    QByteArray ramka_Arx = QByteArray::fromBase64(dane.ustawienia_arx.toLatin1());
    QDataStream streamArx(ramka_Arx);

    bool is_reg_wlacz, is_ster_wlacz;
    double min_reg, max_reg, min_ster, max_ster, szumy;
    int opoznienie;
    std::vector<double> A, B;
    quint32 wsp_a, wsp_b;

    streamArx >> szumy;
    streamArx >> opoznienie;
    streamArx >> is_reg_wlacz;
    streamArx >> is_ster_wlacz;
    streamArx >> max_reg;
    streamArx >> min_reg;
    streamArx >> max_ster;
    streamArx >> min_ster;

    streamArx >> wsp_a;
    for(quint32 i = 0; i < wsp_a; i++) {
        double a;
        streamArx >> a;
        A.push_back(a);
    }

    streamArx >> wsp_b;
    for(quint32 i = 0; i < wsp_b; i++) {
        double b;
        streamArx >> b;
        B.push_back(b);
    }

    m_centralaSieciowa.zastosujParametryARX(A, B, opoznienie, szumy, is_ster_wlacz, min_ster, max_ster, is_reg_wlacz, min_reg, max_reg);
}

communication_protocol::dane_konfiguracyjne MainWindow::Generator_do_wyslania_binarnka()
{
    communication_protocol::dane_konfiguracyjne dane;
    QByteArray ramka_gen;
    QDataStream stream_gen(&ramka_gen, QIODevice::WriteOnly);

    stream_gen<<m_centralaSieciowa.getGenerator_Amplituda();
    stream_gen<<m_centralaSieciowa.getGenerator_Okres();
    stream_gen<<m_centralaSieciowa.getGenerator_SkladowaStala();
    stream_gen<<m_centralaSieciowa.getGenerator_Wypelnienie();
    stream_gen << static_cast<int>(m_centralaSieciowa.getGenerator_Typ());

    dane.ustawienia_generatora=QString::fromLatin1((ramka_gen.toBase64()));
    return dane;
}

void MainWindow::generator_odbior_binarnka(const communication_protocol::dane_konfiguracyjne &dane)
{
    QByteArray ramka_gen = QByteArray::fromBase64(dane.ustawienia_generatora.toLatin1());
    QDataStream stream_gen(ramka_gen);

    double amplituda, okres, skladowa_stala, wypelnienie;
    int typ;
    stream_gen>>amplituda;
    stream_gen>>okres;
    stream_gen>>skladowa_stala;
    stream_gen>>wypelnienie;
    stream_gen>>typ;

    m_centralaSieciowa.setTypGenerator(static_cast<TypSyg>(typ));
    m_centralaSieciowa.setWypelnienieGenerator( wypelnienie);
    m_centralaSieciowa.setAmplitudaGenerator(amplituda);
    m_centralaSieciowa.setSkladowaStalaGenerator(skladowa_stala);
    m_centralaSieciowa.setOkresGenerator(okres);
}

communication_protocol::dane_sterujace MainWindow::Interwal_do_wyslania_binarnka(double t0)
{
    communication_protocol::dane_sterujace dane;
    QByteArray ramka;
    QDataStream strumien(&ramka, QIODevice::WriteOnly);
    strumien << QString("interwal") << m_centralaSieciowa.getTaktowanie() << static_cast<qint32>(m_centralaSieciowa.get_Okno()) << t0;

    dane.polecenie = QString::fromLatin1(ramka.toBase64());
    dane.interwal = m_centralaSieciowa.getTaktowanie();
    //dane.okno = static_cast<qint32>(m_centralaSieciowa.get_Okno());
    dane.okno =m_centralaSieciowa.get_Okno();
    return dane;
}

void MainWindow::Interwal_odbior_binarnka(const communication_protocol::dane_sterujace &dane)
{
    QByteArray ramka = QByteArray::fromBase64(dane.polecenie.toLatin1());
    QDataStream strumien(ramka);
    QString komenda;
    int wartosc;
    qint32 okno = 0;
    strumien >> komenda >> wartosc;
    if (!strumien.atEnd()) {
        strumien >> okno;
    }

    if(komenda == "interwal") {
        m_centralaSieciowa.setTaktowanie(wartosc);
        if (okno > 0) m_centralaSieciowa.set_Okno(okno);
    } else {
        if (dane.polecenie == "interwal") {
            m_centralaSieciowa.setTaktowanie(dane.interwal);
            m_centralaSieciowa.set_Okno(dane.okno);
        }
    }
}

communication_protocol::dane_sterujace MainWindow::Okno_do_wyslania_binarnka()
{
    communication_protocol::dane_sterujace dane;
    QByteArray ramka;
    QDataStream strumien(&ramka, QIODevice::WriteOnly);
    strumien << QString("okno") << m_centralaSieciowa.getTaktowanie() << static_cast<qint32>(m_centralaSieciowa.get_Okno()) << m_czasSieciowy;

    dane.polecenie = QString::fromLatin1(ramka.toBase64());
    dane.interwal = m_centralaSieciowa.getTaktowanie();
    dane.okno = static_cast<qint32>(m_centralaSieciowa.get_Okno());
    return dane;
}

void MainWindow::Okno_odbior_binarnka(const communication_protocol::dane_sterujace &dane)
{
    QByteArray ramka = QByteArray::fromBase64(dane.polecenie.toLatin1());
    QDataStream strumien(ramka);
    QString komenda;
    int wartosc;
    qint32 okno = 0;
    strumien >> komenda >> wartosc;
    if (!strumien.atEnd()) {
        strumien >> okno;
    }

    if(komenda == "okno") {
        if (okno > 0) {
            m_centralaSieciowa.set_Okno(okno);
            m_centrala.set_Okno(okno);
        }
    } else {
        if (dane.polecenie == "okno" && dane.okno > 0) {
            m_centralaSieciowa.set_Okno(dane.okno);
            m_centrala.set_Okno(dane.okno);
        }
    }
}



