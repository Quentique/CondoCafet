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
#include <QDate>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>

ProductsEdit::ProductsEdit(QSqlDatabase *db) : MainEdit(db, "products")
{

    model->setHeaderData(1, Qt::Horizontal, tr("Dénomination"));
    model->setHeaderData(2, Qt::Horizontal, tr("Prix"));
    model->setHeaderData(3, Qt::Horizontal, tr("Couleur"));

    view->setItemDelegateForColumn(2, new NumberDelegate(this));
    view->setItemDelegateForColumn(3, new ComboDelegate(this));
    //view->hideColumn(0);
    view->hideColumn(4);
    view->hideColumn(5);
    view->setColumnWidth(1, 200);
    view->setColumnWidth(2, view->width()/1.8);
    view->setColumnWidth(3, view->width()/1.5);

    setWindowTitle(tr("Édition des produits"));
    connect(add, SIGNAL(clicked(bool)), this, SLOT(addRow()));
    connect(remove, SIGNAL(clicked(bool)), this, SLOT(deleteRow()));
    connect(static_cast<QSqlTableModel*>(model->sourceModel()), SIGNAL(beforeUpdate(int,QSqlRecord&)), this, SLOT(check(int,QSqlRecord&)));
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
    QSqlQuery query(static_cast<QSqlTableModel*>(model->sourceModel())->database());
    query.prepare("INSERT INTO products (name, price, colour, sold, addDate) VALUES (:object, 1.00, '#FFFFFF', 0, :date)");
    query.bindValue(":object", tr("Nouveau"));
    query.bindValue(":date", QDate::currentDate().toString("dd-MM-yyyy"));
    query.exec();

    static_cast<QSqlTableModel*>(model->sourceModel())->sort(3, Qt::AscendingOrder);
    static_cast<QSqlTableModel*>(model->sourceModel())->select();
}

void ProductsEdit::deleteRow()
{
    qDebug() << "HEY";
    int id = view->selectionModel()->selectedRows(0).at(0).data().toInt();
    QSqlQuery query(Smodel->database());
    query.prepare("SELECT * FROM products WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
    query.next();

    QString location = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).at(1);
    location += tr("/log/Suppression_du_produit_") + query.value("name").toString() + ".log";
    qDebug() << location;
    QFile file(location);
    file.open(QFile::WriteOnly | QFile::Append | QFile::Text);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << "******************************" << endl << "         CondoCafet         " << endl << "******************************" << endl << tr("Suppresion de l'article ") << query.value("name").toString() << tr(" vendu au prix de ") << QString::number(query.value("price").toDouble(), 'f', 2) << QString::fromUtf8(" € l'unité.");
    stream << endl << tr("Produit ajouté le ").toUtf8() << query.value("addDate").toString() << tr(" et enlevé le ").toUtf8() << QDate::currentDate().toString("dd-MM-yyyy") << endl;
    stream << tr("Produit vendu ") << query.value("sold").toString() << " fois" << endl << "******************************" << endl;

    query.prepare("SELECT name, price, sold, addDate FROM products");
    query.exec();
    stream.setFieldWidth(20);
    stream << left << " " << endl;
    stream <<  "Nom du produit" << " | "  << "Date d'ajout" << " | " << "Prix" << " | " << "Exemplaires vendus" << endl;
    while (query.next())
    {
        stream << left << query.value("name").toString() << " | " << query.value("addDate").toString() << " | " << QString::number(query.value("price").toDouble(), 'f', 2) + QString::fromUtf8(" €") << " | " << query.value("sold").toString() << endl;
    }
    file.close();

    MainEdit::deleteRow();
}
