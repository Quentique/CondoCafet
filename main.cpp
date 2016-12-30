#include <QApplication>
#include <QPushButton>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setStyleSheet("QPushButton { width: 140px; height: 90px; background-color: white; font-size: 15px; } QMessageBox QPushButton, QInputDialog QPushButton {width: inherit; height: inherit; background-color: none; font-size: inherit;} *[stylegen=\"true\"] { font-size: 18px;} QLineEdit[stylegen=\"true\"] { width: 250%;}");
    window.setFont(QFont("Arial", 15));

    return app.exec();
}
