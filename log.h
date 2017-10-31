#ifndef LOG_H
#define LOG_H

#include <QWidget>
#include <QPlainTextEdit>

class Log : public QWidget
{
    Q_OBJECT
    QPlainTextEdit *logtext = new QPlainTextEdit;
public:
    explicit Log(QWidget *parent = 0);

signals:

public slots:
    addMessage(QString message);


};

#endif // LOG_H
