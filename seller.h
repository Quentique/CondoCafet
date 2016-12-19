#ifndef SELLER_H
#define SELLER_H

#include <QString>
#include <QDateTime>

class Seller
{
public:
    Seller(QString g_name, QString g_class);
    void addSold(double g_price);
    QString getName();
    int getSoldCount();
    double getAmount();

private:
    QString name, classe;
    double sold;
    int nb_ventes;
    QDateTime enter_date;
};

#endif // SELLER_H
