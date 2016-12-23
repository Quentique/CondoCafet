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
void Vente::deleteArticle(Product *g_product, int quantity)
{
    if (contains(g_product->getName()))
    {
        int index = indexOf(g_product->getName());
    if (quantity == 0)
    {
        remove(index);
        Article article = articles->at(index);
        total -= article.getTotal();
        articles->remove(index);
    }
    else
    {
        Article article = articles->at(index);
        total -= article.getTotal();
        article.setQuantity(article.getQuantity()-quantity);
        total += article.getTotal();
        articles->replace(index, article);
    }
    }
}

void Vente::setQuantity(Product *g_product, int g_quantity)
{
    if (contains(g_product->getName()))
    {
        Article article = articles->at(indexOf(g_product->getName()));
        total -= article.getTotal();
        article.setQuantity(g_quantity);
        total += article.getTotal();
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
