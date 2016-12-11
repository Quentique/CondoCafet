#include "seller.h"

Seller::Seller(QString g_name, QString g_class) : name(g_name), classe(g_class), enter_date(QDateTime::currentDateTime()), nb_ventes(0), sold(0.00f)
{
}

void Seller::addSold(double g_price)
{
    sold += g_price;
    nb_ventes++;
}
