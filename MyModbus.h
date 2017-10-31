#ifndef MYMODBUS_H
#define MYMODBUS_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QModbusDataUnit>
#include <QtSerialBus/QModbusClient>
#include <QtSerialBus/QModbusRtuSerialMaster>
#include <QString>

class MyModbus : public QObject
{
    Q_OBJECT
public:
    explicit MyModbus(QString *comNum, QObject *parent = 0);
    struct settings {
        // int portName=QSerialPort::portName();
        int parity = QSerialPort::EvenParity;
        int baud = QSerialPort::Baud19200;
        int dataBits = QSerialPort::Data8;
        int stopBits = QSerialPort::OneStop;
        int responseTime = 1000;
        int numberOfRetries = 3;

    } new_settings;

    QModbusClient *modbusDevice;
    uint_fast16_t *data;
    int valueCount, dataUpdatedFlag;
    void read(int slaveAdress, int registerGroup, int startAddress, int numberOfEntries);
    void write(int slaveAdress, int registerGroup, int startAddress, int numberOfEntries, uint16_t *data);
    void connect(QString port);
    QModbusDataUnit readRequest(int registerGroup, int startAddress, int numberOfEntries);
    QModbusDataUnit writeRequest(int registerGroup, int startAddress, int numberOfEntries);
    QString *logMsg = new QString;
signals:
    QString logMessage(QString msg);

public slots:
    void readReady();
    void writeReady();
};

#endif // MYMODBUS_H
