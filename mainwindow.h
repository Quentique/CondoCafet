#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTableWidget>
#include <QSettings>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
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
    Vente *current;
    QSettings *psettings;
    QLineEdit *dated, *sellerd, *soldd, *totald;
    QPushButton *sign, *rush, *pay, *cancel, *retour;
    QPushButton *calc[14], *coins[12];
    QDoubleSpinBox *countd;
};

#endif // MAINWINDOW_H
