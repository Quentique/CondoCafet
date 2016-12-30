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
                   addDate TEXT \
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
