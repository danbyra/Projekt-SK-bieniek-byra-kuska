#ifndef JEDNOSTRONNY_H
#define JEDNOSTRONNY_H

#include <QObject>
#include <QTimer>
#include <QQueue>
#include "communication_protocol.h"


class MainWindow;

class JednostronnyManager : public QObject
{
    Q_OBJECT
private:
    MainWindow* m_mainWindow;
    QTimer* m_emergencyTimer;
    
    double m_lastU;
    double m_lastW;
    bool m_isEmergencyActive;
    bool m_responseReceived;
    int m_missedPacketsCount;

    //true po startSimulation(), false po stop/reset.
    bool m_isSimulationStarted;
    bool m_reconnectedJustNow;

    // Kolejka FIFO snapshotów dla wysłanej ramki.
    struct SentFrameInfo { double time; double w; };
    QQueue<SentFrameInfo> m_sentFramesQueue;

public:
    explicit JednostronnyManager(MainWindow* mainWindow, QObject* parent = nullptr);
    ~JednostronnyManager();

    void startSimulation();
    void stopSimulation();
    void resetSimulation();

    //true jeśli symulacja została uruchomiona
    bool isRunning() const { return m_isSimulationStarted; }

    //handlery komend sieciowych
    void handleStartCommand(double t0 = 0.0, uint32_t numer = 0);
    void handleStopCommand();
    void handleResetCommand();

    //eventy dla mainwindow
    void handleTimerTimeout();
    void handleOdebranoSymulacje(uint32_t numer, const communication_protocol::dane_symulacji& dane);
    void handlePolaczono();
    void handleRozlaczono();
    void handleIntervalChanged();

private slots:
    void handleEmergencyTick();
};

#endif // JEDNOSTRONNY_H
