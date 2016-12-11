#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

class Product
{
public:
    Product(QString g_name, double g_price, QString g_categorie);
    double getPrice();
    QString getName();
    QString getCategorie();

private:
    QString name, categorie;
    double price;
};

#endif // PRODUCT_H
