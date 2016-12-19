#include <QApplication>
#include <QPushButton>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setStyleSheet("QPushButton { width: 100px; height: 70px; background-color: white; }");

    return app.exec();
}
