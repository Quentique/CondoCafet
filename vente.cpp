#include "vente.h"

Vente::Vente(int g_number) : number(g_number)
{
    produits = new QMap<QString, Article>;
    total = 0;
}

void Vente::addArticle(Product *g_product, int g_quantity)
{
    if (!produits->contains(g_product->getName()))
    {
        Article article(g_product, g_quantity);
        produits->insert(g_product->getName(), article);
    } else {
        Article article = produits->find(g_product->getName()).value();
        article.addQuantity(g_quantity);
        produits->insert(g_product->getName(), article);
    }
}
