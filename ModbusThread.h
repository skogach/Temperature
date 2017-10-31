#ifndef ModbusThread_H
#define ModbusThread_H

#include <QObject>
#include <QString>
#include <Qtime>
#include "MyModbus.h"
#include <QCoreApplication>
#define REF 2.5
class ModbusThread : public QObject
{
    Q_OBJECT
public:
    explicit ModbusThread(QObject *parent = 0);

    int i=1, portnumber=1;
    int bytecount, tmp;
    uint16_t msg[128];
    QString *portName = new QString("COM1");
    MyModbus *modbus_class;
    uint16_t readAddress=3329, writeAddress=8192, writeValue=5;
    int delayValue=100;
    const uint16_t commandRegister = 8192;
    int portFlag=0;
    bool interviewingStatus[247]={false};
signals:
    int sendData(int channel, int16_t adcReading, int temp_msb_lsb, int temp_xlsb);
    void portChanged();
    void slaveInterrogated();
    void logMessageSignal(QString msg);

public slots:
    void Interfacing(void);
    void getportint(int prt);
    void getReadAddr(int readAddr);
    void getWriteAddr(int writeAddr);
    void getWriteVal(int writeVal);
    void writeToSlave(void);
    void writeStart(void);
    void writeStop(void);
    void getDelay(int delay);
    void logMessageSlot(QString msg);
    void interviewingStatusChange(int channel, bool status);

private:
    void delay(int msec);
};

#endif // ModbusThread_H
