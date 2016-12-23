#include "article.h"

Article::Article()
{

}
Article::Article(Product *g_product, int g_quantity) : produit(g_product), quantite(g_quantity)
{
    prix_total = quantite * g_product->getPrice();
}

Product* Article::getProduct()
{
    return produit;
}
void Article::setQuantity(int g_quantity)
{
    quantite = g_quantity;
    prix_total = produit->getPrice() * quantite;
}

double Article::getTotal()
{
    return prix_total;
}

int Article::getQuantity()
{
    return quantite;
}

void Article::addQuantity(int quantity)
{
    quantite += quantity;
    prix_total = quantite * produit->getPrice();
}
