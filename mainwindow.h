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
    void actualiseTable();

private:
    QWidget *centralWidget;
    DbManager *manager;
    QTableWidget *sold_details;
    Seller *seller;
};

#endif // MAINWINDOW_H
