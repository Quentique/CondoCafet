#ifndef MAINEDIT_H
#define MAINEDIT_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QPushButton>
#include <QLineEdit>
#include <QSortFilterProxyModel>

class MainEdit : public QWidget
{
    Q_OBJECT
public:
    explicit MainEdit(QSqlDatabase *db, QString gtable);

public slots:
    void addRow();
    void selectRow();
    void deleteRow();
    void sortBy();

protected:
    QPushButton *end, *add, *remove;
    QLineEdit *sort;
    QTableView *view;
    QSqlTableModel *Smodel;
    QSortFilterProxyModel *model;
    QString *table;
};
#endif // MAINEDIT_H
