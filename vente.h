#ifndef VENTE_H
#define VENTE_H

#include <QMap>
#include "article.h"
#include <QStandardItemModel>

class Vente
{
public:
    Vente(int g_number);
    void addArticle(Product* g_product, int g_quantity);
    void deleteArticle(Product* g_product, int quantity = 0);

private:
    QMap<QString, Article> *produits;
    double total;
    const int number;
};

#endif // VENTE_H
