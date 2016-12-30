#include "seller.h"

Seller::Seller(QString g_name, QString g_class, QString birth) : name(g_name), classe(g_class), enter_date(QDateTime::currentDateTime()), nb_ventes(0), sold(0.00f), birthday(birth)
{
}

void Seller::addSold(double g_price)
{
    sold += g_price;
    nb_ventes++;
}

QString Seller::getName()
{
    return name;
}

QDateTime Seller::getEnterTime()
{
    return enter_date;
}

int Seller::getSoldCount() { return nb_ventes; }
double Seller::getAmount() { return sold; }
QString Seller::getClass() { return classe; }
QString Seller::getBirthday() { return birthday; }
