#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>

#include <QLabel>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <slaveTable.h>
#include <QThread>
#include <ModbusThread.h>
#include <QDebug>
#include "settingsdialog.h"
#include <QVariant>
#include <QMenuBar>
#include "MyModbus.h"
#include "log.h"

namespace Ui {
class Widget;
}
class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QMenuBar *menuInit(void);
        SlaveTable *slave_Table=new SlaveTable();
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
