#include "vente.h"

Vente::Vente(int g_number) : number(g_number)
{
    total = 0;
    articles = new QVector<Article>;
}

void Vente::addArticle(Product *g_product, int g_quantity)
{
    if (!contains(g_product->getName()))
    {
        Article article(g_product, g_quantity);
        total += article.getTotal();
        append(g_product->getName());
        articles->append(article);

    } else {
        Article article = articles->at(indexOf(g_product->getName()));
        total += g_product->getPrice() * g_quantity;
        article.addQuantity(g_quantity);
        articles->replace(indexOf(g_product->getName()), article);
    }
}

double Vente::getTotal()
{
    return total;
}

int Vente::getNumber()
{
    return number;
}

Article Vente::getArticle(int pos)
{
    return articles->at(pos);
}
