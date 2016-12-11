#include "product.h"

Product::Product(QString g_name, double g_price, QString g_categorie) : name(g_name), price(g_price), categorie(g_categorie)
{
}

double Product::getPrice()
{
    return price;
}

QString Product::getName()
{
    return name;
}

QString Product::getCategorie()
{
    return categorie;
}
