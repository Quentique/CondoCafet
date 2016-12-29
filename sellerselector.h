#ifndef SELLERSELECTOR_H
#define SELLERSELECTOR_H

#include <QWidget>
#include <QListView>
#include <QLabel>
#include <QPushButton>
#include <QSqlDatabase>
#include <QDialog>
#include <QLineEdit>
#include <QSortFilterProxyModel>

class SellerSelector : public QDialog
{
    Q_OBJECT
public:
    explicit SellerSelector(QWidget *parent = 0, QSqlDatabase *sql = 0);
    int getResult();
signals:

public slots:
    void select();
    void change();

private:
    int result;
    QSqlDatabase *db;
    QListView *selection;
    QLabel *title, *explanation;
    QPushButton *ok, *cancel;
    QLineEdit *search;
    QSortFilterProxyModel *model;

};

#endif // SELLERSELECTOR_H
