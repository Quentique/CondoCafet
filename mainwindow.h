#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTableWidget>
#include <QSettings>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QSignalMapper>
#include <QMap>
#include <QString>

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
    void sign_slot();
    void actualiseVendeur();
    void cancelSell();
    void showTotal();
    void multiply(int gnumber);
    void touchC();
    void addProduct(QString gname);

private:
    int multiplyby;
    QWidget *centralWidget;
    DbManager *manager;
    QTableWidget *sold_details;
    Seller *seller;
    Vente *current;
    QSettings *psettings;
    QLineEdit *dated, *sellerd, *totald;
    QLabel *soldd;
    QPushButton *sign, *rush, *pay, *cancel, *retour, *totalmd;
    QPushButton *calc[16], *coins[12];
    QDoubleSpinBox *countd;
    QSignalMapper *products_mapper, *calc_mapper;
    QHash<QString, Product*> *product_list;
};

#endif // MAINWINDOW_H
