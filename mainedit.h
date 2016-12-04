#ifndef MAINEDIT_H
#define MAINEDIT_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QPushButton>


class MainEdit : public QWidget
{
    Q_OBJECT
public:
    explicit MainEdit(QSqlDatabase *db, QString gtable);

public slots:
    void addRow();
    void selectRow();
    void deleteRow();

protected:
    QPushButton *end, *add, *remove;
    QTableView *view;
    QSqlTableModel *model;
    QString *table;
};
#endif // MAINEDIT_H
