#ifndef PRODUCTSEDIT_H
#define PRODUCTSEDIT_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QPushButton>
#include "mainedit.h"


class ProductsEdit : public MainEdit
{
    Q_OBJECT
public:
    explicit ProductsEdit(QSqlDatabase *db);

signals:

public slots:
    void check(int, QSqlRecord &record);
    void addRow();
};

#endif // PRODUCTSEDIT_H
