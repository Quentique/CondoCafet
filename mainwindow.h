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
    void endSell();

protected:
    void closeEvent(QCloseEvent *event);

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
    void up();
    void down();
    void touchX();
    void paySlot();
    void rushTouch();
    void moneyTouch(QString data);
    void unlock();

private:
    double multiplyby;
    bool payMode, rushMode;
    double payment;
    QWidget *centralWidget;
    DbManager *manager;
    QTableWidget *sold_details;
    Seller *seller;
    Vente *current;
    QSettings *psettings;
    QLineEdit *dated, *sellerd, *totald;
    QLabel *soldd;
    QPushButton *sign, *rush, *pay, *cancel, *totalmd;
    QPushButton *calc[14], *coins[12];
    QDoubleSpinBox *countd;
    QSignalMapper *products_mapper, *calc_mapper, *money_mapper;
    QHash<QString, Product*> *product_list;
    QAction *productsManagement, *sellersManagement, *settingsModif;
};

#endif // MAINWINDOW_H
