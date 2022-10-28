#include "mainwindow.h"
#include <QApplication>
#include <QResource>
#include "game.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game game;
    MainWindow w(nullptr, game);
    w.show();

    return a.exec();
}
