#include "productsedit.h"
#include <QSqlRecord>
#include "numberdelegate.h"
#include "combodelegate.h"
#include <QDebug>
#include <QtMath>

ProductsEdit::ProductsEdit(QSqlDatabase *db) : QWidget()
{
    setGeometry(150, 150, 500, 500);

    model = new QSqlTableModel(0, *db);
    model->setTable("products");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setHeaderData(1, Qt::Horizontal, tr("Dénomination"));
    model->setHeaderData(2, Qt::Horizontal, tr("Prix"));
    model->setHeaderData(3, Qt::Horizontal, tr("Couleur"));

    model->select();

    view = new QTableView(this);

    view->setModel(model);
    view->setItemDelegateForColumn(2, new NumberDelegate(this));
    view->setItemDelegateForColumn(3, new ComboDelegate(this));
        view->hideColumn(0);
        view->hideColumn(4);
        view->setSortingEnabled(true);
        view->setMinimumWidth(500);
    view->show();
    view->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(model, SIGNAL(beforeUpdate(int,QSqlRecord&)), this, SLOT(check(int,QSqlRecord&)));
}

void ProductsEdit::check(int, QSqlRecord &record)
{
    if (record.value("price").toDouble() < 0)
    {
        record.setValue("price", qFabs(record.value("price").toDouble()));
    }
}
