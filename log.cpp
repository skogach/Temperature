#include "log.h"
#include <QGridLayout>
#include <QTextEdit>
#include <QDateTime>

Log::Log(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(logtext, 0, 0);
    this->setLayout(layout);
}
Log::addMessage(QString message)
{
    logtext->appendPlainText((QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz "))+message);
}
