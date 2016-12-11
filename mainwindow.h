#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTableWidget>
#include "dbmanager.h"
#include "vente.h"
#include "seller.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

public slots:
    void showProductsEdit();
    void showSellersEdit();
    void showSettings();

private:
    QWidget *centralWidget;
    DbManager *manager;

};

#endif // MAINWINDOW_H
