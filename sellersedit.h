#ifndef SELLERSEDIT_H
#define SELLERSEDIT_H

#include <QString>
#include <QSqlDatabase>
#include "mainedit.h"

class SellersEdit : public MainEdit
{
    Q_OBJECT
public:
    SellersEdit(QSqlDatabase *db);
public slots:
    void check(int, QSqlRecord &record);
    void addRow();
};

#endif // SELLERSEDIT_H
