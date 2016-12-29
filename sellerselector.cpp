#include "sellerselector.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>

SellerSelector::SellerSelector(QWidget *parent, QSqlDatabase *sql) : QDialog(parent)
{
    db = sql;
    result = -1;
    title = new QLabel("Sélection du vendeur");
    explanation = new QLabel("Veuillez choisir le vendeur dans la liste déroulante afin de commencer une session de vente");
    selection = new QListView;
    QStandardItemModel *Qmodel = new QStandardItemModel;
    model = new QSortFilterProxyModel;

    QSqlQuery query("SELECT id, name, class FROM sellers ORDER BY name", *db);
    while(query.next())
    {
        QStandardItem *item = new QStandardItem;
        item->setText(query.value("name").toString() + ", " + query.value("class").toString());
        item->setData(query.value("id"));
        Qmodel->setItem(Qmodel->rowCount(), 0, item);
    }

    model->setSourceModel(Qmodel);
    selection->setModel(model);
    selection->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ok = new QPushButton("Valider");
    cancel = new QPushButton("Annuler");
    search = new QLineEdit;
    search->setClearButtonEnabled(true);
    search->setPlaceholderText(tr("Nom..."));

    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addWidget(ok, 0, Qt::AlignRight);
    buttons->addWidget(cancel, 0, Qt::AlignRight);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(title);
    layout->addWidget(explanation);
    layout->addSpacing(20);
    layout->addWidget(search);
    layout->addWidget(selection);
    layout->addSpacing(20);
    layout->addLayout(buttons);

    setLayout(layout);
    setWindowFlags(Qt::Dialog);
    setWindowModality(Qt::ApplicationModal);

    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ok, SIGNAL(clicked(bool)), this, SLOT(select()));
    connect(search, SIGNAL(textChanged(QString)), this, SLOT(change()));
    connect(selection, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(select()));
}

int SellerSelector::getResult()
{
    return result;
}

void SellerSelector::select()
{
    //QStandardItemModel *modell = static_cast<QStandardItemModel*>(model->sourceModel());
    result = model->data(selection->currentIndex(), Qt::UserRole+1).toInt();

    close();
}

void SellerSelector::change()
{
    QRegExp reg("(" + search->text() +")", Qt::CaseInsensitive);
    model->setFilterRegExp(reg);
}
