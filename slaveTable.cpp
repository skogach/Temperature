#include "SlaveTable.h"
#include <QDebug>
SlaveTable::SlaveTable(QWidget *parent)
    : QTableWidget(parent)
{

    setColumnCount(6);
    setRowCount(247);
    setHorizontalHeaderItem(0,new QTableWidgetItem("Имя канала"));
    setHorizontalHeaderItem(1,new QTableWidgetItem("Температура, °С:"));
    setHorizontalHeaderItem(2,new QTableWidgetItem("Опрос"));
    setHorizontalHeaderItem(3,new QTableWidgetItem("Нижняя уставка"));
    setHorizontalHeaderItem(4,new QTableWidgetItem("Верхняя уставка"));
    setHorizontalHeaderItem(5,new QTableWidgetItem("Тип датчика"));

    for(int i=0;i<247;i++)
    {
        typeChange[i].addItem("Тип J (ТЖК)");
        typeChange[i].addItem("Тип K (TXA)");
        typeChange[i].addItem("Тип L (ТХK)");
        typeChange[i].addItem("Тип N (ТНН)");
        typeChange[i].addItem("Тип T (ТМК)");
        QWidget *wdg = new QWidget;
        QGridLayout *layout = new QGridLayout();
        wdg->setLayout(layout);
        layout->addWidget(&typeChange[i],0,0);
        layout->setContentsMargins(0,0,0,0);
        setCellWidget(i,5,wdg);
        typeChange[i].setMinimumHeight(25);

        setCellWidget(i,2,&interviewCheckBox[i]);
        interviewCheckBox[i].setStyleSheet("margin-left:50%; margin-right:50%;");
        interviewCheckBox[i].setObjectName(QString::number(i));
        typeChange[i].setObjectName(QString::number(i));
        settings[i].name="Канал  "+QString::number(i+1);

        QObject::connect(&interviewCheckBox[i],SIGNAL(stateChanged(int)),this,SLOT(interviewingFlagChanged(int)));
        QObject::connect(&typeChange[i],SIGNAL(currentIndexChanged(int)),this,SLOT(sensorTypeChanged(int)));
    }
    QObject::connect(this,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(tableDataChanged(QTableWidgetItem*)));

    QFile settingsFile("settings.txt");
    if (!settingsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

        if (!settingsFile.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream stream(&settingsFile);
        stream<<QString("#Имя канала|Тип датчика|Опрос|Нижняя уставка|Верхняя уставка\n");
        for(int i=1;i<248;i++)
        {
            stream<<QString("Канал ")<<i<<"|0|0|-200|1200\n";
        }
        settingsFile.close();
        emit logMessage(QString("[Предупреждение] Невозможно открыть файл настроек, создан файл с настройками по умолчанию"));
    }
    if (!settingsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&settingsFile);
        int i=0;
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (line.startsWith("#"))
                continue;
            QStringList values = line.split("|", QString::SkipEmptyParts);
            setItem(i,0,new QTableWidgetItem(values[0]));

            if(values[2].toInt())
            {
                interviewCheckBox[i].setCheckState(Qt::CheckState::Checked);
                emit interviewingFlagChangedSignal(i,1);
                settings[i].interviewing=1;
            }
            typeChange[i].setCurrentIndex(values[1].toInt());
            settings[i].sensorType=values[1].toInt();
            setItem(i,3,new QTableWidgetItem(values[3]));
            settings[i].lowSetpoint=values[3].toDouble();
            setItem(i,4,new QTableWidgetItem(values[4]));
            settings[i].highSetpoint=values[4].toDouble();
            i++;
        }
    }
    settingsFile.close();
    graphTemp->show();
}


void SlaveTable::setTemperature(double voltage, double TRJ, int TCtype)
{
    QString compensatedTemp=QString::number(ColdJunctionCompensation(TRJ,voltage,TCtype),'f',1);
    temp->display(compensatedTemp);
}

void SlaveTable::setVoltage(double inputVoltage)
{
    voltage=inputVoltage;
    setTemperature(voltage, TRJ, TCtype);
}
void SlaveTable::setTRJ(double inputTRJ)
{
    TRJ=inputTRJ;
    setTemperature(voltage, TRJ, TCtype);
}
void SlaveTable::setTCType(int inputType)
{
    TCtype=inputType;
    setTemperature(voltage, TRJ, TCtype);
    if(inputType==3)
        Calibrate->setEnabled(true);
    else
        Calibrate->setEnabled(false);

    QFile file(*title);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << TCtype << "\n";
}
//------------------------------------------------------
double SlaveTable::TUJ(double u){//T(u) for Type J thermocouple(-210(-8.095);+1200(69.553))
    double T=0;
    if (u<0)
        T=-8.3823321*pow(10,-5)*pow(u,8)
                -2.3963370*pow(10,-3)*pow(u,7)
                -2.8131513*pow(10,-2)*pow(u,6)
                -1.7256713*pow(10,-1)*pow(u,5)
                -5.9086933*pow(10,-1)*pow(u,4)
                -1.0752178*pow(10,0)*pow(u,3)
                -1.2286185*pow(10,0)*pow(u,2)
                +1.9528268*pow(10,1)*pow(u,1)
                +0*pow(u,0);
    else if(u>=0&&u<42.919)
        T=5.099890*pow(10,-10)*pow(u,7)
                -5.344285*pow(10,-8)*pow(u,6)
                +3.585153*pow(10,-6)*pow(u,5)
                -2.549687*pow(10,-4)*pow(u,4)
                +1.036969*pow(10,-2)*pow(u,3)
                -2.001204*pow(10,-1)*pow(u,2)
                +1.978425*pow(10,1)*pow(u,1)
                +0*pow(10,-6)*pow(u,0);
    else
        T=4.73886084*pow(10,-6)*pow(u,5)
                -1.43033468*pow(10,-3)*pow(u,4)
                +1.70276630*pow(10,-1)*pow(u,3)
                -9.94773230*pow(10,0)*pow(u,2)
                +3.00543684*pow(10,2)*pow(u,1)
                -3.11358187*pow(10,3)*pow(u,0);
    return T;
}

double SlaveTable::TUK(double u){//T(u) for Type K thermocouple(-200(-5.891);+1372(54.886))
    double T=0;
    if (u<0)
        T=-5.1920577*pow(10,-4)*pow(u,8)
                -1.0450598*pow(10,-2)*pow(u,7)
                -8.6632643*pow(10,-2)*pow(u,6)
                -3.7342377*pow(10,-1)*pow(u,5)
                -8.9773540*pow(10,-1)*pow(u,4)
                -1.0833638*pow(10,0)*pow(u,3)
                -1.1662878*pow(10,0)*pow(u,2)
                +2.5173462*pow(10,1)*pow(u,1)
                +0*pow(u,0);
    else if(u>=0&&u<20.644)
        T=-1.052755*pow(10,-8)*pow(u,9)
                +1.057734*pow(10,-6)*pow(u,8)
                -4.413030 *pow(10,-5)*pow(u,7)
                +9.804036*pow(10,-4)*pow(u,6)
                -1.228034*pow(10,-2)*pow(u,5)
                +8.315270 *pow(10,-2)*pow(u,4)
                -2.503131*pow(10,-1)*pow(u,3)
                +7.860106*pow(10,-2)*pow(u,2)
                +2.508355*pow(10,1)*pow(u,1)
                +0*pow(u,0);
    else
        T=-3.110810*pow(10,-8)*pow(u,6)
                +8.8021934*pow(10,-6)*pow(u,5)
                -9.650715*pow(10,-4)*pow(u,4)
                +5.464731*pow(10,-2)*pow(u,3)
                -1.646031*pow(10,0)*pow(u,2)
                +4.830222*pow(10,1)*pow(u,1)
                -1.318058*pow(10,2)*pow(u,0);
    return T;
}
double SlaveTable::TUL(double u){//T(u) for Type L thermocouple(-200(-9.488);+800(66.466))
    double T=0;
    if(u<0)
        T=  1.5131878*pow(10,-6)*pow(u,9)
                +6.0654431*pow(10,-5)*pow(u,8)
                +1.0301283*pow(10,-3)*pow(u,7)
                +9.5288883*pow(10,-3)*pow(u,6)
                +5.1946958*pow(10,-2)*pow(u,5)
                +0.1666128*pow(10,0)*pow(u,4)
                +0.3170064*pow(10,0)*pow(u,3)
                +4.0458554*pow(10,-2)*pow(u,2)
                +1.5884573*pow(10,1)*pow(u,1)
                +1.1573067*pow(10,-4)*pow(u,0);
    else
        T=5.4857331*pow(10,-14)*pow(u,9)
                -1.7130654*pow(10,-11)*pow(u,8)
                +2.3816891*pow(10,-9)*pow(u,7)
                -1.9323678*pow(10,-7)*pow(u,6)
                +1.0050886*pow(10,-5)*pow(u,5)
                -3.5394655*pow(10,-4)*pow(u,4)
                +9.4286756*pow(10,-3)*pow(u,3)
                -0.2261183*pow(10,0)*pow(u,2)
                +1.5775525*pow(10,1)*pow(u,1)
                +7.2069422*pow(10,-3)*pow(u,0);
    return T;
}
double SlaveTable::TUN(double u){//T(u) for TypeN thermocouple
    double T=0;
    if(u<0)
        T=   7,3138868 *pow(10,-1)*pow(u,9)
                +1.1582665 *pow(10,-1)*pow(u,8)
                +7.7075166 *pow(10,-1)*pow(u,7)
                +2.7754916 *pow(10,0)*pow(u,6)
                +5.8488586 *pow(10,0)*pow(u,5)
                +7.2060525 *pow(10,0)*pow(u,4)
                +5.2229312*pow(10,0)*pow(u,3)
                +1.1010485 *pow(10,0)*pow(u,2)
                +3.8436847*pow(10,1)*pow(u,1)
                +0*pow(10,0)*pow(u,0);
    else if(u<20.613)
        T=   -7.98156 *pow(10,-8)*pow(u,7)
                +5.39280 *pow(10,-6)*pow(u,6)
                -1.17272 *pow(10,-4)*pow(u,5)
                -2.12169  *pow(10,-6)*pow(u,4)
                +4.70205 *pow(10,-2)*pow(u,3)
                -1.08267 *pow(10,0)*pow(u,2)
                +3.86896*pow(10,1)*pow(u,1)
                +0*pow(10,0)*pow(u,0);

    else
        T=         7.767022 *pow(10,-7)*pow(u,5)
                -1,274371*pow(10,-4)*pow(u,4)
                + 9,855391 *pow(10,-3)*pow(u,3)
                -3,915159*pow(10,-1)*pow(u,2)
                +3.300943 *pow(10,1)*pow(u,1)
                +1.972485*pow(10,1)*pow(u,0);
    return T;
}
double SlaveTable::TUT(double u){//T(u) for Type T thermocouple
    double T=0;
    if(u<0)
        T=+ 1.2668171 *pow(10,-3)*pow(u,7)
                + 2.0241446 *pow(10,-2)*pow(u,6)
                + 1.3304473  *pow(10,-1)*pow(u,5)
                + 4.2527777 *pow(10,-1)*pow(u,4)
                +7.9018692*pow(10,-1)*pow(u,3)
                -2.1316967 *pow(10,-1)*pow(u,2)
                + 2.5949192*pow(10,1)*pow(u,1)
                +0*pow(10,0)*pow(u,0);
    else
        T=       -7.293422  *pow(10,-6)*pow(u,6)
                +6.048144  *pow(10,-5)*pow(u,5)
                -2.165394*pow(10,-3)*pow(u,4)
                +  4.637791  *pow(10,-2)*pow(u,3)
                -7.602961*pow(10,-1)*pow(u,2)
                +2.592800 *pow(10,1)*pow(u,1)
                +0*pow(10,0)*pow(u,0);
    return T;
}
double SlaveTable::ColdJunctionCompensation(double TRJ, double uTC, int TCType){
    double TC;
    enum TCNames{J, K, L, N, T, User};
    TCNames Names = (TCNames)TCType;
    switch (Names)
    {
    case J:
        TC=TUJ(uTC)+TRJ;
        break;
    case K:
        TC=TUK(uTC)+TRJ;
        break;
    case L:
        TC=TUL(uTC)+TRJ;
        break;
    case N:
        TC=TUN(uTC)+TRJ;
        break;
    case T:
        TC=TUT(uTC)+TRJ;
        break;
    case User:
        TC=C3*pow(uTC,3)+C1*uTC+TRJ;
        break;
    }
    return TC;
}
void SlaveTable::setApproximationCoefficients(double setC1, double setC3)
{
    C1=setC1;
    C3=setC3;
}
void SlaveTable::setData(int channel, int16_t adcReading, int temp_msb_lsb, int temp_xlsb)
{
    int32_t adc_T=(((uint32_t)(temp_msb_lsb))<<4|temp_xlsb>>4);
    double roomTemp=(double)bmp280_compensate_T_int32(adc_T)/100;
    setItem(channel-1,1,new QTableWidgetItem(QString::number(ColdJunctionCompensation(roomTemp,(float)adcReading*256.0/32767.0,settings[channel-1].sensorType))));
    if(channel==3)
    {
        graphTemp->repaint(ColdJunctionCompensation(roomTemp,(float)adcReading*256.0/32767.0,settings[channel-1].sensorType));
    }
    if((this->item(channel-1,1)->text().toDouble())<(this->item(channel-1,3)->text().toDouble()))//less than low setpoint
    {
        if(!lowSetpointFlag[channel-1])
        {
            lowSetpointFlag[channel-1]=1;

            emit logMessage("[Предупреждение] Значение температуры канала "+QString::number(channel)+" опустилось ниже нижней уставки");
        }
        this->item(channel-1,1)->setBackgroundColor(Qt::blue);
    }
    else if((this->item(channel-1,1)->text().toDouble())>(this->item(channel-1,4)->text().toDouble()))//more than high setpoint
    {
        if(!highSetpointFlag[channel-1])
        {
            highSetpointFlag[channel-1]=1;
            emit logMessage("[Предупреждение] Значение температуры канала "+QString::number(channel)+" превысило верхнюю уставку");
        }
        this->item(channel-1,1)->setBackgroundColor(Qt::red);
    }
    else

    {
        if(highSetpointFlag[channel-1]||lowSetpointFlag[channel-1])
        {
            highSetpointFlag[channel-1]=0;
            lowSetpointFlag[channel-1]=0;
            emit logMessage("[Предупреждение] Значение температуры канала "+QString::number(channel)+" вернулось в норму");
        }
        this->item(channel-1,1)->setBackgroundColor(Qt::green);
    }
}

void SlaveTable::showCalibrateWindow()
{
    CalibratingWindow *calibrationWindow = new CalibratingWindow;
    calibrationWindow->TCNumber=TCNumber;
    calibrationWindow->show();
    QObject::connect(calibrationWindow,SIGNAL(sendCoefficients(double,double)),this,SLOT(setApproximationCoefficients(double,double)));
}

SlaveTable::~SlaveTable()
{
    QFile settingsFile("settings.txt");
    if (!settingsFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream stream(&settingsFile);
    stream<<QString("#Имя канала|Тип датчика|Опрос|Нижняя уставка|Верхняя уставка\n");
    for(int i=0;i<247;i++)
        stream<<(QString)settings[i].name<<'|'<<settings[i].sensorType <<'|'<<settings[i].interviewing<<'|'<<settings[i].lowSetpoint<<'|'<<settings[i].highSetpoint<<"\n";
    settingsFile.close();
}
void SlaveTable::interviewingFlagChanged(int state)
{
    settings[sender()->objectName().toInt()].interviewing=state;
    if(!state)
    {
        this->item(sender()->objectName().toInt(),1)->setBackgroundColor(Qt::white);
        this->item(sender()->objectName().toInt(),1)->setText("\0");
    }
    emit interviewingFlagChangedSignal(sender()->objectName().toInt()+1,state);
}
void SlaveTable::sensorTypeChanged(int type)
{
    settings[sender()->objectName().toInt()].sensorType=type;
}
void SlaveTable::tableDataChanged(QTableWidgetItem *item)
{
    if(item->column()==0)
        settings[item->row()].name=item->text();
    if(item->column()==3)
    {
        settings[item->row()].lowSetpoint=item->text().toDouble();
    }
    if(item->column()==4)
        settings[item->row()].highSetpoint=item->text().toDouble();
}
void SlaveTable::sendInterviewingStatus(void)
{
    for(int i=1;i<=247;i++)
        emit interviewingFlagChangedSignal(i,settings[i-1].interviewing);
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value
typedef int32_t BMP280_S32_t;
BMP280_S32_t SlaveTable::bmp280_compensate_T_int32(BMP280_S32_t adc_T)
{
    uint32_t dig_T1=27687;
    int32_t dig_T2=26285, dig_T3=50;
    BMP280_S32_t t_fine;
    BMP280_S32_t var1, var2, T;
    var1 = ((((adc_T>>3) - ((BMP280_S32_t)dig_T1<<1))) * ((BMP280_S32_t)dig_T2)) >> 11;
    var2 = (((((adc_T>>4) - ((BMP280_S32_t)dig_T1)) * ((adc_T>>4) - ((BMP280_S32_t)dig_T1))) >> 12) *
            ((BMP280_S32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}
