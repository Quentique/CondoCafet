#include "dbmanager.h"
#include <QMessageBox>
#include <QtSql>
#include <QObject>


DbManager::DbManager(const QString& path)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if(!db.open())
    {
        //QMessageBox::warning(this, QObject::tr("Aucune base de données"), QObject::tr("La base de données indiquée n'existe pas. Une nouvelle base a été crée."));
        QSqlQuery query(db);
        query.exec("CREATE DATABASE database");
        qDebug() << query.lastError().text();
    }

    if (db.record("products").isEmpty())
    {
        QSqlQuery query(db);
        query.exec("CREATE TABLE products ( \
                   id INTEGER PRIMARY KEY, \
                   name VARCHAR(40), \
                   price INTEGER, \
                   colour VARCHAR (10), \
                   sold INTEGER \
               )");
                qDebug() << query.lastError().text();

    }
    if (db.record("till").isEmpty())
    {
        QSqlQuery query(db);
        query.exec("CREATE TABLE till ( id INTEGER PRIMARY KEY, type REAL UNSIGNED NOT NULL, number INTEGER UNSIGNED )");
        qDebug() << query.lastError().text();
        query.prepare("INSERT INTO till (type, number) VALUES (:value, 0)");

        double tableau[12] = {0.01, 0.02, 0.05, 0.10, 0.20, 0.50, 1.00, 2.00, 5.00, 10.00, 20.00, 50.00};
        for (int i = 0 ; i < 12 ; i++)
        {
            query.bindValue(":value", QVariant(tableau[i]));
            query.exec();
            query.prepare("INSERT INTO till (type, number) VALUES (:value, 0)");
            qDebug() << query.lastError().text() << " + " << tableau[i];
        }
    }
    if (db.record("sellers").isEmpty())
    {
        QSqlQuery query(db);
        query.exec("CREATE TABLE sellers (id INTEGER , name VARCHAR(40), class VARCHAR(10), amount REAL, PRIMARY KEY (id) )");
    }


}
