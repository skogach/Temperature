#ifndef CALIBRATINGWINDOW_H
#define CALIBRATINGWINDOW_H
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QTextBrowser>

class CalibratingWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CalibratingWindow(QWidget *parent = 0);
    double C[2], TRJ, voltage, TMJ;
    QTextBrowser *pointsVoltage = new QTextBrowser;
    QTextBrowser *pointsTRJ = new QTextBrowser;
    double x[10], y[10];
    int points=0, TCNumber;
    QPushButton *done = new QPushButton;
    QPushButton *cancel = new QPushButton;
    QPushButton *add = new QPushButton;
    QLabel *labelTemp = new QLabel("Температура, °С");
    QLabel *labelVoltage = new QLabel("ЭДС термопары, мВ");
signals:
    void sendCoefficients(double C1, double C3);
public slots:
    void AppendPoint();
    void SetVoltage(double setVoltage);
    void SetTRJ(double setTRJ);
    void SetTMJ(double setTMJ);
    void setData(double data[16][2]);
    void approximation();
private:
};

#endif // CALIBRATINGWINDOW_H
