#include "MyModbus.h"
#include <QModbusReply>
#include <QModbusDataUnit>
#include <QDebug>
#include "settingsdialog.h"

MyModbus::MyModbus(QString *comNum,QObject *parent) : QObject(parent)
{
}

void MyModbus::readReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
    {
        emit logMessage("[Ошибка] Нет ответа");
        return;
    }
    const QModbusDataUnit unit = reply->result();
    if (reply->error() == QModbusDevice::NoError)
    {   // delete data;
        data = new uint_fast16_t [unit.valueCount()];
        valueCount = unit.valueCount();

        dataUpdatedFlag = valueCount ? 1 : 0;

        for (uint i = 0; i < unit.valueCount(); i++) {
            *(data+i) = unit.value(i);
        }}
    else if (reply->error() == QModbusDevice::ProtocolError)
    {
        emit logMessage(QString("[Ошибка] Ошибка ответа при чтении: %1 (Код исключения Modbus: 0x%2)").arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
    } else {
        emit logMessage(QString("[Ошибка] Ошибка ответа при чтении: %1 (код: 0x%2)").arg(reply->errorString()).arg(reply->error(), -1, 16));

    }
    reply->deleteLater();

}

void MyModbus::writeReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;
    const QModbusDataUnit unit = reply->result();
    // qDebug() << "Value count read ready"<<  unit.valueCount();

    //    if (reply->error() == QModbusDevice::NoError)
    //       delete data;
    //     data = new uint_fast16_t [unit.valueCount()];
    //   valueCount = unit.valueCount();
    // for (uint i = 0; i < unit.valueCount(); i++) {
    /*  unsigned char ch1=0, ch2=0;
                    ch1=0,ch1|=unit.value(i);
                    ch2=0,ch2|=unit.value(i)>>8;
                    qDebug()<<QString::number(ch2,16)<<QString::number(ch1,16);*/
    /*             *(data+i) = unit.value(i);
          }
*///        } else if (reply->error() == QModbusDevice::ProtocolError) {
    //            statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
    //                                        arg(reply->errorString()).
    //                                        arg(reply->rawResult().exceptionCode(), -1, 16), 5000);
    //        } else {
    //            statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
    //                                        arg(reply->errorString()).
    //                                        arg(reply->error(), -1, 16), 5000);
    //        }

    reply->deleteLater();
}

void MyModbus::read(int slaveAdress,int registerGroup, int startAddress, int numberOfEntries)
{
    if (auto *reply = modbusDevice->sendReadRequest(readRequest(registerGroup, startAddress, numberOfEntries), slaveAdress)) {
        const QModbusDataUnit unit = reply->result();
        // qDebug() << "Value count read"<<  unit.valueCount();
        if (!reply->isFinished())
            // QObject::connect(reply, &QModbusReply::finished, new MyModbus(), &MyModbus::readReady);
            QObject::connect(reply, &QModbusReply::finished, this, readReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        //statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(), 5000);
    }
}

void MyModbus::write(int slaveAdress,int registerGroup, int startAddress, int numberOfEntries, uint16_t *data)
{
    QModbusDataUnit writeUnit = writeRequest(registerGroup, startAddress, numberOfEntries);
    // QModbusDataUnit::RegisterType table = 4;
    for (uint i = 0; i < writeUnit.valueCount(); i++) {
        writeUnit.setValue(i, *(data+i) );
    }
    auto *write_reply = modbusDevice->sendWriteRequest(writeUnit, slaveAdress);
    if (!write_reply->isFinished())
        // QObject::connect(reply, &QModbusReply::finished, new MyModbus(), &MyModbus::readReady);
        QObject::connect(write_reply, &QModbusReply::finished, this, writeReady);
    else
        delete write_reply; // broadcast replies return immediately
}

QModbusDataUnit MyModbus::readRequest(int registerGroup, int startAddress, int numberOfEntries)
{
    const auto table =
            static_cast<QModbusDataUnit::RegisterType> (registerGroup); //regiser type
    // Q_ASSERT(startAddress >= 0 && startAddress < 10);
    // do not go beyond 10 entries
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}

QModbusDataUnit MyModbus::writeRequest(int registerGroup, int startAddress, int numberOfEntries)
{
    const auto table =
            static_cast<QModbusDataUnit::RegisterType> (registerGroup);
    // Q_ASSERT(startAddress >= 0 && startAddress < 10);
    // do not go beyond 10 entries
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}
void MyModbus::connect(QString port)
{

    modbusDevice = new QModbusRtuSerialMaster();  //Create
    if (!modbusDevice)
        emit logMessage("[Ошибка] Невозможно создать экземпляр класса Modbus");
    else
    {
        modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                             "COM8");
        modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                             QSerialPort::NoParity);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                             QSerialPort::Baud2400);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                             QSerialPort::Data8);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                             QSerialPort::OneStop);
        modbusDevice->setTimeout(1000);
        modbusDevice->setNumberOfRetries(3);
        //-------connecting----------
        modbusDevice->connectDevice();
        if (!modbusDevice->connectDevice())
           // emit logMessage("[Информация] Связь с измерителем установлена");
            qt_noop();
    }
}
