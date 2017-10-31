#include "menubar.h"

MenuBar::MenuBar()
{
    toolsMenu = this->addMenu(("&Инструменты"));
    toolsMenu->addAction(settingsOption);
}
