#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QDateTime>
#include <QVector>

using namespace QtCharts;

class Graph : public QWidget
{
    Q_OBJECT
public:
    explicit Graph(QWidget *parent = nullptr);
    QChart *chart = new QChart();
    QDateTimeAxis *axisX = new QDateTimeAxis;
    QValueAxis *axisY = new QValueAxis;
    QChartView *chartView = new QChartView(chart);
    QLineSeries *series = new QLineSeries();
private:
    QGridLayout *layout = new QGridLayout;
signals:
    void logMessage(QString);
public slots:
    void repaint(double temp);
};

#endif // GRAPH_H
