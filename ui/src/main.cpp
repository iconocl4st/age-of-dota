#include "lobbybrowser.h"
#include "gamewindow.h"
#include <QApplication>

int main2()
{
    int argc = 0;
    char *argv[0];
    QApplication a(argc, argv);

    LobbyBrowser w;
    w.show();

    GameWindow gw;
    gw.show();

    return a.exec();
}
