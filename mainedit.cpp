#include "mainedit.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>
#include <QSqlError>

MainEdit::MainEdit(QSqlDatabase *db, QString gtable) : QWidget()
{
    table = new QString(gtable);

    Smodel = new QSqlTableModel(0, *db);
    Smodel->setTable(*table);
    Smodel->setEditStrategy(QSqlTableModel::OnFieldChange);

    Smodel->select();

    model = new QSortFilterProxyModel;
    model->setDynamicSortFilter(true);
    model->setFilterKeyColumn(1);
    model->setSourceModel(Smodel);

    view = new QTableView(this);
    view->setModel(model);
    view->setSortingEnabled(true);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->verticalHeader()->setVisible(false);
    view->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
    view->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    end = new QPushButton("Ok");
    end->setMinimumWidth(75);

    add = new QPushButton(tr("Ajouter"));
    add->setIcon(QIcon(":images/add.png"));
    remove = new QPushButton(tr("Supprimer"));
    remove->setIcon(QIcon(":images/delete.png"));
    remove->setEnabled(false);

    sort = new QLineEdit;
    sort->setClearButtonEnabled(true);
    sort->setPlaceholderText(tr("Nom d'un produit"));

    QVBoxLayout *buttons = new QVBoxLayout;
    buttons->addWidget(add, 1, Qt::AlignTop);
    buttons->addWidget(remove, 1, Qt::AlignTop);
    buttons->addSpacing(200);

    QHBoxLayout *Lview = new QHBoxLayout;
    Lview->addWidget(view);
    Lview->addLayout(buttons);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(sort, 0, Qt::AlignRight);
    layout->addLayout(Lview);
    layout->addWidget(end, 0, Qt::AlignRight);

    connect(end, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(add, SIGNAL(clicked(bool)), this, SLOT(addRow()));
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(selectRow()));
    connect(remove, SIGNAL(clicked(bool)), this, SLOT(deleteRow()));
    connect(sort, SIGNAL(textChanged(QString)), this, SLOT(sortBy()));

    setLayout(layout);
    setWindowFlags(Qt::Dialog);
    setWindowModality(Qt::ApplicationModal);
    layout->setSizeConstraint(QLayout::SetFixedSize);
}

void MainEdit::addRow()
{
}

void MainEdit::selectRow()
{
    remove->setEnabled(true);
}

void MainEdit::deleteRow()
{
    int id = Smodel->record(view->selectionModel()->selection().indexes().at(0).row()).field("id").value().toInt();
    QSqlQuery query(Smodel->database());
    query.prepare("DELETE FROM " + *table + " WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
    Smodel->select();
    remove->setEnabled(false);
}

void MainEdit::sortBy()
{
    QRegExp reg("(" + sort->text() +")", Qt::CaseInsensitive);
    model->setFilterRegExp(reg);
}
