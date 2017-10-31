#ifndef slaveTable_H
#define slaveTable_H
#include "calibratingwindow.h"
#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLCDNumber>
#include <QComboBox>
#include <QString>
#include <QPushButton>
#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QTableWidget>
#include <QCheckBox>
#include "graph.h"
class SlaveTable : public QTableWidget
{
    Q_OBJECT
public:
    SlaveTable(QWidget *parent=0);
    ~SlaveTable();
    double voltage=0, TRJ=0, C1=0, C3=0;
    int TCtype=0, TCNumber;
    QLCDNumber *temp = new QLCDNumber;
    QComboBox typeChange[247];
    QCheckBox interviewCheckBox[248];

    QPushButton *Calibrate = new QPushButton;

    QFile *file = new QFile;
    QString *title = new QString("settings.txt");
    bool highSetpointFlag[247], lowSetpointFlag[247];
    Graph *graphTemp = new Graph;

    struct channelSettings {
    public:
        QString name="Канал ";
        int sensorType = 1;
        float lowSetpoint = -200;
        float highSetpoint = 1200;
        bool interviewing = false;

    };
    typedef int32_t BMP280_S32_t;
    channelSettings settings[247];
    BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T);
protected:
private slots:
signals:
    void logMessage(QString);
    void interviewingFlagChangedSignal(int channel, bool state);

public slots:
    void setVoltage(double inputVoltage);
    void setTRJ(double inputTRJ);
    void setTCType(int inputType);
    void setData(int channel, int16_t adcReading, int temp_msb_lsb, int temp_xlsb);
    void setApproximationCoefficients(double setC1, double setC3);
    void showCalibrateWindow();
    void interviewingFlagChanged(int state);
    void sensorTypeChanged(int type);
    void tableDataChanged(QTableWidgetItem*);
    void sendInterviewingStatus(void);
private:
    double TUJ(double u);
    double TUK(double u);
    double TUL(double u);
    double TUN(double u);
    double TUT(double u);
    void setTemperature(double voltage, double TRJ, int TCtype);
    double ColdJunctionCompensation(double TRJ, double uTC, int TCType);


};
#endif // slaveTable_H
