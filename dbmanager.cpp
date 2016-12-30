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
    if (db.record("sellers").isEmpty())
    {
        QSqlQuery query(db);
        query.exec("CREATE TABLE sellers (id INTEGER , name VARCHAR, class VARCHAR, amount REAL, birthday TEXT, PRIMARY KEY (id) )");
    }
}

QSqlDatabase* DbManager::getDB()
{
    return &db;
}
