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
    }

    if (db.record("products").isEmpty())
    {
        QSqlQuery query(db);
        query.exec("CREATE TABLE products ( \
                   id INTEGER PRIMARY KEY, \
                   name VARCHAR, \
                   price INTEGER UNSIGNED, \
                   colour VARCHAR, \
                   sold INTEGER \
               )");

    }
    if (db.record("till").isEmpty())
    {
        QSqlQuery query(db);
        query.exec("CREATE TABLE till ( id INTEGER PRIMARY KEY, type REAL, number INTEGER )");
        query.prepare("INSERT INTO till (type, number) VALUES (:value, 0)");

        double tableau[12] = {0.01, 0.02, 0.05, 0.10, 0.20, 0.50, 1.00, 2.00, 5.00, 10.00, 20.00, 50.00};
        for (int i = 0 ; i < 12 ; i++)
        {
            query.bindValue(":value", QVariant(tableau[i]));
            query.exec();
            query.prepare("INSERT INTO till (type, number) VALUES (:value, 0)");
        }
    }
    if (db.record("sellers").isEmpty())
    {
        QSqlQuery query(db);
        query.exec("CREATE TABLE sellers (id INTEGER , name VARCHAR, class VARCHAR, amount REAL, PRIMARY KEY (id) )");
    }

    if (db.record("miscellaneous").isEmpty())
    {
        QSqlQuery query(db);
        query.exec("CREATE TABLE miscellaneous (id INTEGER PRIMARY KEY, name VARCHAR, value VARCHAR )");
    }
}

QSqlDatabase* DbManager::getDB()
{
    return &db;
}
