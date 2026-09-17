#ifndef OBUSTRONNY_H
#define OBUSTRONNY_H

#include <QObject>
#include <QTimer>
#include <QList>
#include "communication_protocol.h"


class MainWindow;

class ObustronnyManager : public QObject
{
    Q_OBJECT

public:
    struct BufferedFrame {
        uint32_t packet_id;
        communication_protocol::dane_symulacji dane;
    };

    struct MasterFrame {
        uint32_t packet_id;
        double w;
        double u;
        double uP;
        double uI;
        double uD;
        double e;
    };

private:
    MainWindow* m_mainWindow;
    QTimer* m_slaveTimer;
    
  
    double m_lastU;
    double m_lastW;
    bool m_isEmergencyActive;
    
    QList<BufferedFrame> m_slaveBuffer;


    QMap<uint32_t, MasterFrame> m_masterBuffer;

    //Desync trakowanie
    int m_consecutiveLosses;
    bool m_receivedResponseForCurrentTick;
    
    //Time index sync
    double m_t0;
    int64_t m_lastReceivedPacketId;

    //true po start false po stop/reset.
    bool m_isSimulationStarted;

    //true przez jeden takt po zmianie interwału po stronie reg.
    //ustawiona filtry odrzucania starych pakietów są pomijane (pozwala zaakceptować pierwsze pakiety od obiektu)
    bool m_intervalJustChanged;

    
    bool m_reconnectedJustNow;

public:
    explicit ObustronnyManager(MainWindow* mainWindow, QObject* parent = nullptr);
    ~ObustronnyManager();

    void startSimulation();
    void stopSimulation();
    void resetSimulation();

    //true if symulacja została uruchomiona
    bool isRunning() const { return m_isSimulationStarted; }

    //handlery komend sieciowych
    void handleStartCommand(double t0 = 0.0, uint32_t numer = 0);
    void handleStopCommand();
    void handleResetCommand();
    void handleIntervalCommand(int newInterval, int newWindow, double t0 = 0.0);


    void handleMasterTimerTimeout();

    //naprzychodzące ramki
    void handleOdebranoSymulacje(uint32_t numer, const communication_protocol::dane_symulacji& dane);
    
    //connection
    void handlePolaczono();
    void handleRozlaczono();


private slots:

    void handleSlaveTimerTick();
};

#endif // OBUSTRONNY_H
