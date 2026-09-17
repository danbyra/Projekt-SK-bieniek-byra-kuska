QT += core gui network
QT += charts
QT += core gui widgets charts network
CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

equals(QT_MAJOR_VERSION, 6):lessThan(QT_MINOR_VERSION, 7) {
    DEFINES += STARY_CHECKBOX
}
equals(QT_MAJOR_VERSION, 5) {
    DEFINES += STARY_CHECKBOX
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Add new folder subsets to include paths so older #include directives work properly
INCLUDEPATH += $$PWD/ui $$PWD/core $$PWD/network $$PWD/network/jednostronny $$PWD/network/obustronny $$PWD/tests

SOURCES += \
    main.cpp \
    test.cpp \
    core/Centrala.cpp \
    core/GenSyg.cpp \
    core/ModelARX.cpp \
    core/ProstyUAR.cpp \
    core/RegulatorPID.cpp \
    network/communication_protocol.cpp \
    network/Hub/network_hub.cpp \
    network/jednostronny/jednostronny.cpp \
    network/obustronny/obustronny.cpp \
    test_pk.cpp \
    ui/mainwindow.cpp \
    ui/ust_arx.cpp \

HEADERS += \
    core/Centrala.h \
    core/GenSyg.h \
    core/IRegulator.h \
    core/ModelARX.h \
    core/ProstyUAR.h \
    core/RegulatorPID.h \
    core/include.h \
    network/communication_protocol.h \
    network/Hub/network_hub.h \
    network/jednostronny/jednostronny.h \
    network/obustronny/obustronny.h \
    ui/mainwindow.h \
    ui/ust_arx.h \

FORMS += \
    ui/mainwindow.ui \
    ui/ust_arx.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    README.md
