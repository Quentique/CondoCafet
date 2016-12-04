#include "productsedit.h"
#include <QSqlRecord>
#include "numberdelegate.h"
#include "combodelegate.h"
#include <QDebug>
#include <QtMath>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>
#include <QIcon>
#include <QSqlField>

ProductsEdit::ProductsEdit(QSqlDatabase *db) : MainEdit(db, "products")
{

    model->setHeaderData(1, Qt::Horizontal, tr("Dénomination"));
    model->setHeaderData(2, Qt::Horizontal, tr("Prix"));
    model->setHeaderData(3, Qt::Horizontal, tr("Couleur"));

    view->setItemDelegateForColumn(2, new NumberDelegate(this));
    view->setItemDelegateForColumn(3, new ComboDelegate(this));
    view->hideColumn(0);
    view->hideColumn(4);

    setWindowTitle(tr("Édition des produits"));
    connect(add, SIGNAL(clicked(bool)), this, SLOT(addRow()));
    connect(model, SIGNAL(beforeUpdate(int,QSqlRecord&)), this, SLOT(check(int,QSqlRecord&)));
}

void ProductsEdit::check(int, QSqlRecord &record)
{
    if (record.value("price").toDouble() <= 0)
    {
        record.setValue("price", qFabs(record.value("price").toDouble()));
    }
}

void ProductsEdit::addRow()
{
    qDebug() << "DONE";
    QSqlQuery query(model->database());
    query.prepare("INSERT INTO products (name, price, colour, sold) VALUES (:object, 1.00, '#FFFFFF', 0)");
    query.bindValue(":object", tr("Nouveau"));
    query.exec();
    qDebug() << query.lastError().text();
    model->sort(0, Qt::AscendingOrder);
    model->select();
}
