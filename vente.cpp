#include "vente.h"

Vente::Vente(int g_number) : number(g_number)
{
    total = 0;
}

void Vente::addArticle(Product *g_product, int g_quantity)
{
    if (!contains(g_product->getName()))
    {
        Article article(g_product, g_quantity);
        insert(g_product->getName(), article);
    } else {
        Article article = find(g_product->getName()).value();
        article.addQuantity(g_quantity);
        insert(g_product->getName(), article);
    }
}

double Vente::getTotal()
{
    return total;
}
