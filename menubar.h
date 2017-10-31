#ifndef MENUBAR_H
#define MENUBAR_H
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMenuBar>


class MenuBar : public QMenuBar
{
public:
    MenuBar();
    QMenu *toolsMenu;
    QAction *settingsOption = new QAction("Настройки");
};

#endif // MENUBAR_H
