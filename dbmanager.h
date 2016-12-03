#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QString>

class DbManager
{
public:
    DbManager(const QString &path);
private:
    QSqlDatabase db;

};

#endif // DBMANAGER_H
