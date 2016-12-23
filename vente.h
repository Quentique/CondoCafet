#ifndef VENTE_H
#define VENTE_H

#include <QMap>
#include "article.h"
#include <QStandardItemModel>

class Vente : public QVector<QString>
{
public:
    Vente(int g_number);
    void addArticle(Product* g_product, int g_quantity);
    void deleteArticle(Product* g_product, int quantity = 0);
    double getTotal();
    int getNumber();
    Article getArticle(int pos);
    void setQuantity(Product *g_product, int g_quantity);

private:
    double total;
    const int number;
    QVector<Article> *articles;
};

#endif // VENTE_H
