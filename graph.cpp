#include "graph.h"
#include <QDebug>
#include <QPen>

Graph::Graph(QWidget *parent) : QWidget(parent)
{
    this->setLayout(layout);
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Температура");

    axisX->setFormat("hh:mm:ss");
    axisX->setTitleText("Время");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY->setLabelFormat("%i");
    axisY->setTitleText("Температура");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    //chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView,0,0);
    QPen *pen = new QPen;
    pen->setWidth(1);
    series->setPen(*pen);
    axisX->setMin(QDateTime::currentDateTime());
    axisX->setMax(QDateTime::currentDateTime());
    axisY->setMax(25);
    axisY->setMin(25);
    this->setMinimumHeight(500);
    this->setMinimumWidth(1000);
}

void Graph::repaint(double temp)
{
    series->append(QDateTime::currentDateTime().toMSecsSinceEpoch(),QString::number(temp, 'f', 1).toDouble());
    axisX->setMax(QDateTime::currentDateTime());
    if(axisY->max()<temp)
        axisY->setMax(temp);
    if(axisY->min()>temp)
        axisY->setMin(temp);
    axisX->setTickCount(10);
}
