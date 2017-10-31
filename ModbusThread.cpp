#include "ModbusThread.h"
#include <QDebug>
ModbusThread::ModbusThread(QObject *parent) : QObject(parent)
{      
}

void ModbusThread::Interfacing(void)
{
    if (portFlag)
    {
        for (int i = 1; i<=247;i++)
        {
            if(interviewingStatus[i])
            {
                modbus_class->read(i, 4, readAddress, 3);
                delay(delayValue);
                if((modbus_class->dataUpdatedFlag))
                    emit sendData(i,*((modbus_class->data)),*((modbus_class->data)+1),*((modbus_class->data)+2));
            }
        }
    }

    emit slaveInterrogated();
}
void ModbusThread::getportint(int prt)
{
    portnumber=prt+1;
    QString *tmp = new QString;
    tmp->append("COM");
    tmp->append(QString::number(portnumber));
    portName=tmp;
    //delete modbus_class;
    qDebug()<<portName;
    modbus_class = new MyModbus(portName);
    portFlag=1;
    QObject::connect(modbus_class,SIGNAL(logMessage(QString)),this,SLOT(logMessageSlot(QString)));
    modbus_class->connect(*portName);
}
void ModbusThread::delay(int msec)
{
    QTime dieTime= QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void ModbusThread::getWriteAddr(int writeAddr)
{
    writeAddress=writeAddr;
}
void ModbusThread::getReadAddr(int readAddr)
{
    readAddress=readAddr;
}
void ModbusThread::getWriteVal(int writeVal)
{
    writeValue=writeVal;
}
void ModbusThread::writeToSlave(void)
{
    modbus_class->write(1, 4, writeAddress, 1, &writeValue);
    delay(delayValue);
}
void ModbusThread::getDelay(int delay)
{
    delayValue=delay;
}
void ModbusThread::writeStart(void)
{
    uint16_t startCommand = 1;
    modbus_class->write(1, 4, commandRegister, 1, &startCommand);
    delay(delayValue);
}
void ModbusThread::writeStop(void)
{
    uint16_t stopCommand = 5;
    modbus_class->write(1, 4, commandRegister, 1, &stopCommand);
    delay(delayValue);
}
void ModbusThread::logMessageSlot(QString msg)
{
    emit logMessageSignal(msg);
}
void ModbusThread::interviewingStatusChange(int channel, bool status)
{
    interviewingStatus[channel]=status;
}
