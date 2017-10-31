#include "calibratingwindow.h"
CalibratingWindow::CalibratingWindow(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new  QGridLayout;
    QDoubleSpinBox *TMJ = new QDoubleSpinBox;
    QDoubleSpinBox *voltage = new QDoubleSpinBox;
    done->setText("Готово");
    done->setEnabled(false);
    cancel->setText("Отмена");
    add->setText("Добавить точку");
    layout->addWidget(labelVoltage,0,0);
    layout->addWidget(labelTemp,0,1);
    layout->addWidget(voltage,1,0);
    layout->addWidget(TMJ,1,1);
    layout->addWidget(pointsVoltage,2,0);
    layout->addWidget(pointsTRJ,2,1);
    layout->addWidget(add,3,0,1,2);
    layout->addWidget(done,4,0);
    layout->addWidget(cancel,4,1);
    setLayout(layout);
    setWindowTitle("Калибровка");
    QObject::connect(add,SIGNAL(pressed()),this,SLOT(AppendPoint()));
    QObject::connect(TMJ,SIGNAL(valueChanged(double)),this,SLOT(SetTMJ(double)));
    QObject::connect(voltage,SIGNAL(valueChanged(double)),this,SLOT(SetVoltage(double)));
    QObject::connect(cancel,SIGNAL(pressed()),this,SLOT(close()));
    QObject::connect(done,SIGNAL(pressed()),this,SLOT(approximation()));
    QObject::connect(done,SIGNAL(pressed()),this,SLOT(close()));
}
void CalibratingWindow::approximation()
{
    int SIZE=10;
    double G[2][SIZE], GT[SIZE][2], GNN[2][2]={{0}}, det, INV[2][2],GTY[2]={0}, C1=0, C3=0;
    int i,j,r;
    for(i=0;i<SIZE;i++)
    {
        G[0][i]=x[i];
        G[1][i]=pow(x[i],3);
    }
    for(i=0;i<SIZE;i++)
    {
        GT[i][0]=G[0][i];
        GT[i][1]=G[1][i];
    }
    for(i=0;i<2;i++)
        for(j=0;j<2;j++)
            for(r=0;r<SIZE;r++)
            {
                GNN[i][j]+=G[i][r]*GT[r][j];
            }

    for(r=0;r<SIZE;r++)//GT*y
    {
        GTY[0]+=G[0][r]*y[r];
        GTY[1]+=G[1][r]*y[r];
    }
    det=GNN[0][0]*GNN[1][1]-GNN[0][1]*GNN[1][0];
    INV[0][0]=GNN[1][1]/det;
    INV[0][1]=-GNN[1][0]/det;
    INV[1][0]=-GNN[0][1]/det;
    INV[1][1]=GNN[0][0]/det;

    /*  for(i=0;i<2;i++)
    for(r=0;r<2;r++)
    {
        C[i]+=INV[i][r]*GTY[r];
    }*/

    for(r=0;r<2;r++)
    {
        C3+=INV[1][r]*GTY[r];
        C1+=INV[0][r]*GTY[r];
    }
    emit sendCoefficients(C1, C3);
}
void CalibratingWindow::AppendPoint()
{
    QString *tmp = new QString;
    pointsVoltage->append(tmp->number(voltage));
    pointsTRJ->append(tmp->number(TMJ));
    x[points]=voltage;
    y[points]=TMJ-TRJ;
    points++;
    if(points>=10)
    {
        add->setEnabled(false);
        done->setEnabled(true);
    }
}
void CalibratingWindow::SetTMJ(double setTMJ)
{
    TMJ=setTMJ;
}
void CalibratingWindow::SetTRJ(double setTRJ)
{
    TRJ=setTRJ;
}
void CalibratingWindow::SetVoltage(double setVoltage)
{
    voltage=setVoltage;
}
void CalibratingWindow::setData(double data[16][2])
{
    TRJ=data[TCNumber][0];
    voltage=data[TCNumber][1];
}
