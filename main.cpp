#include "include.h"
#include "iostream"
#include "mainwindow.h"
#include "test_pk.cpp"
#include "test_symulacja.cpp"


#ifdef MAIN

int main(int argc, char *argv[])
{



    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    TESTY_ModelARX::wykonaj_testy();
    TESTY_RegulatorPID::wykonaj_testy();
   // TESTY_RegulatorOnOff::wykonaj_testy();
    TESTY_ProstyUAR::wykonaj_testy();
    TESTY_LogikaSymulacji::wykonaj_testy();

    return a.exec();
}

#endif //MAIN
