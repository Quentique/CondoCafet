#ifndef SELLER_H
#define SELLER_H

#include <QString>
#include <QDateTime>

class Seller
{
public:
    Seller(QString g_name, QString g_class, QString birth);
    void addSold(double g_price);
    QString getName();
    int getSoldCount();
    double getAmount();
    QString getClass();
    QDateTime getEnterTime();
    QString getBirthday();

private:
    QString name, classe;
    double sold;
    int nb_ventes;
    QDateTime enter_date;
    QString birthday;
};

#endif // SELLER_H
