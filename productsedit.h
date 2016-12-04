#ifndef PRODUCTSEDIT_H
#define PRODUCTSEDIT_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QPushButton>


class ProductsEdit : public QWidget
{
    Q_OBJECT
public:
    explicit ProductsEdit(QSqlDatabase *db);

private:
    QTableView *view;
    QSqlTableModel *model;

signals:

public slots:
    void check(int, QSqlRecord &record);
    void addRow();

private:
    QPushButton *end, *add, *remove;
};

#endif // PRODUCTSEDIT_H
