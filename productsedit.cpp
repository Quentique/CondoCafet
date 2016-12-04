#include "productsedit.h"
#include <QSqlRecord>
#include "numberdelegate.h"
#include "combodelegate.h"
#include <QDebug>
#include <QtMath>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>

ProductsEdit::ProductsEdit(QSqlDatabase *db) : QWidget()
{

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
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    view->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    end = new QPushButton("Ok");
    end->setMinimumWidth(75);

    add = new QPushButton(tr("Ajouter"));
    remove = new QPushButton(tr("Supprimer"));
    remove->setEnabled(false);

    QVBoxLayout *buttons = new QVBoxLayout;
    buttons->addWidget(add, 1, Qt::AlignTop);
    buttons->addWidget(remove, 1, Qt::AlignTop);
    buttons->addSpacing(100);

    QHBoxLayout *Lview = new QHBoxLayout;
    Lview->addWidget(view);
    Lview->addLayout(buttons);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(Lview);
    layout->addWidget(end, 0, Qt::AlignRight);

    connect(model, SIGNAL(beforeUpdate(int,QSqlRecord&)), this, SLOT(check(int,QSqlRecord&)));
    connect(end, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(add, SIGNAL(clicked(bool)), this, SLOT(addRow()));

    setLayout(layout);
    setWindowTitle(tr("Édition des produits"));
    setWindowFlags(Qt::Dialog);
    setWindowModality(Qt::ApplicationModal);
    layout->setSizeConstraint(QLayout::SetFixedSize);

}

void ProductsEdit::check(int, QSqlRecord &record)
{
    if (record.value("price").toDouble() < 0)
    {
        record.setValue("price", qFabs(record.value("price").toDouble()));
    }
}

void ProductsEdit::addRow()
{
    qDebug() << "DONE";
    QSqlQuery query(model->database());
    query.prepare("INSERT INTO products (name, price, colour, sold) VALUES (:object, 1.00, '#FFFFFF', 0)");
    query.bindValue(":object", tr("Nouveau"));
    query.exec();
    qDebug() << query.lastError().text();
    model->sort(0, Qt::AscendingOrder);
    model->select();
}
