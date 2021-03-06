#ifndef ARTICLE_H
#define ARTICLE_H

#include "product.h"

class Article
{
public:
    Article();
    Article(Product *g_product, int g_quantity);
    Product* getProduct();
    void addQuantity(int quantity);
    int getQuantity();
    double getTotal();
    void setQuantity(int g_quantity);

private:
    Product *produit;
    int quantite;
    double prix_total;
};

#endif // ARTICLE_H
