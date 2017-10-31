#include "widget.h"
#include "ui_widget.h"
#include "settingsdialog.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include "menubar.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QGridLayout *layout = new QGridLayout;
    MenuBar *menuBar = new MenuBar;
    //connect(menuBar->NewAct,(SIGNAL()))
    ModbusThread *interfacing = new ModbusThread;

    QObject::connect(slave_Table,SIGNAL(interviewingFlagChangedSignal(int,bool)),interfacing,SLOT(interviewingStatusChange(int,bool)));
    //slave_Table =new SlaveTable();
    slave_Table->sendInterviewingStatus();

    //--------------------------------------------
    QLabel *portlabel = new QLabel("Порт:");
    //  portlabel->setAlignment(Qt::AlignRight);
    /*  QFont font;
    font.setPointSize(10);
    portlabel->setFont(font);*/
    QComboBox *portbox = new QComboBox;
    // portbox->setFont(font);
    portbox->addItem("COM1");
    portbox->addItem("COM2");
    portbox->addItem("COM3");
    portbox->addItem("COM4");
    portbox->addItem("COM5");
    portbox->addItem("COM6");
    portbox->addItem("COM7");
    portbox->addItem("COM8");
    portbox->addItem("COM9");
    portbox->addItem("COM10");
    portbox->addItem("COM11");
    QSpinBox *delay = new QSpinBox;
    QPushButton *connectButton = new QPushButton("Подключиться");
    SettingsDialog *settingsDialog = new SettingsDialog;
    Log *log = new Log;

    layout->addWidget(menuBar,0,0,1,4);
    layout->addWidget(slave_Table,1,0,1,4);
    layout->addWidget(log,2,0,1,4);


    QThread *thread = new QThread;
    interfacing->moveToThread(thread);

    QObject::connect(thread,SIGNAL(started()),interfacing,SLOT(Interfacing()));
    thread->start();

    QObject::connect(portbox,SIGNAL(currentIndexChanged(int)),interfacing,SLOT(getportint(int)));
    QObject::connect(interfacing,SIGNAL(slaveInterrogated()),interfacing,SLOT(Interfacing()),Qt::QueuedConnection);

    qRegisterMetaType < int16_t >("int16_t");
    QObject::connect(interfacing,SIGNAL(sendData(int, int16_t, int, int)),slave_Table,SLOT(setData(int, int16_t, int, int)));
    QObject::connect(slave_Table,SIGNAL(logMessage(QString)),log,SLOT(addMessage(QString)));

    QObject::connect(interfacing,SIGNAL(logMessageSignal(QString)),log,SLOT(addMessage(QString)),Qt::QueuedConnection);
    QObject::connect(menuBar->settingsOption,&QAction::triggered,settingsDialog,&QWidget::show);
    //connect(settingsButton,SIGNAL(clicked(bool)),
    delay->setValue(1000);
    portbox->setCurrentIndex(9);

    this->setLayout(layout);
}
Widget::~Widget()
{
    delete ui;
}
