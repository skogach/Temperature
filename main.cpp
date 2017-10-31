#include "widget.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.move(QApplication::desktop()->screen()->rect().topLeft() - w.rect().topLeft());//App will be shown in top left corner of the screen
    w.setMinimumHeight(700);
    w.setMinimumWidth(1400);
    w.setWindowTitle("Многоканальный измеритель температуры");
    w.show();
    return a.exec();
}
