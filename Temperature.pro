#-------------------------------------------------
#
# Project created by QtCreator 2016-05-07T10:42:05
#
#-------------------------------------------------

QT += serialbus serialport widgets core gui charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Temperature
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    calibratingwindow.cpp \
    settingsdialog.cpp \
    log.cpp \
    ModbusThread.cpp \
    MyModbus.cpp \
    menubar.cpp \
    graph.cpp \
    slaveTable.cpp


HEADERS  += widget.h \
    calibratingwindow.h \
    settingsdialog.h \
    log.h \
    ModbusThread.h \
    MyModbus.h \
    menubar.h \
    graph.h \
    slaveTable.h


FORMS    += widget.ui\
    settingsdialog.ui
greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}
