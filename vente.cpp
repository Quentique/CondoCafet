#include "vente.h"
#include <QSqlQuery>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>
#include <QDateTime>
#include <QObject>

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
        if (article.getQuantity() <= 0)
        {
            deleteArticle(article.getProduct());
        } else {
        total += article.getTotal();
        articles->replace(indexOf(g_product->getName()), article);
        }
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
void Vente::end(QSqlDatabase *sql, bool rush)
{
    QString location = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).at(1);
    location += "/log/Ventes_" + QDate::currentDate().toString("dd-MM-yyyy") + ".log";

            QFile file(location);

            file.open(QFile::WriteOnly | QFile::Append | QFile::Text);
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            if (!rush)
             stream << endl << QString::fromUtf8("N°") << QString::number(getNumber()) << QString::fromUtf8(" à ") << QDateTime::currentDateTime().toString("HH:mm") << ", " << QString::number(getTotal(), 'f', 2) + QString::fromUtf8(" € : ");

    for (int i = 0 ; i < count() ; i++)
    {
        Article article = articles->at(i);
        QSqlQuery query(*sql);
        query.prepare("SELECT sold FROM products WHERE name = ?");
        query.bindValue(0, at(i));
        query.exec();
        query.next();
        int amount = query.value("sold").toInt();
        amount += article.getQuantity();
        query.prepare("UPDATE products SET sold = ? WHERE name = ?");
        query.bindValue(0, amount);
        query.bindValue(1, at(i));
        query.exec();
        QString fin = (i+1 == count()) ? "" : ", ";
        fin = (rush) ? "\n" : fin;
        stream << QString::number(article.getQuantity()) << QString::fromUtf8("× ") << QString::fromUtf8(article.getProduct()->getName().toUtf8()) << fin;

    }
    file.close();
}
