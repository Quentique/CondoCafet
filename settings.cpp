#include "settings.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QMap>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QCursor>

Settings::Settings() : QDialog()
{
    settings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    QMap<QString, QVariant> map;
    map.insert("red", "#ffffff");
    settings->setValue("colours", map);
    ok = new QPushButton(tr("Valider"));
    ok->setDefault(true);
    cancel = new QPushButton(tr("Annuler"));

    QHBoxLayout *boutons = new QHBoxLayout;
    boutons->insertSpacing(0, 170);
    boutons->addWidget(ok, 0, Qt::AlignRight);
    boutons->addWidget(cancel, 0, Qt::AlignRight);

    colours = new QGroupBox;
    colours->setTitle(tr("Gestion des catégories"));
    coloursT = new QTableView;
    colours_explanation = new QLabel(tr("Permet de gérer les différentes catégories de produits, en les distinguant par couleurs."));
    QVBoxLayout *colours_lay = new QVBoxLayout;
    colours_lay->addWidget(colours_explanation);
    colours_lay->addWidget(coloursT);
    colours->setLayout(colours_lay);
    coloursT->setContextMenuPolicy(Qt::CustomContextMenu);
    coloursT->setSelectionMode(QAbstractItemView::SingleSelection);
    coloursT->setSelectionBehavior(QAbstractItemView::SelectRows);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(colours);
    layout->addLayout(boutons);

    setLayout(layout);
    setWindowTitle(tr("Paramètres"));

    create = new QAction(tr("Nouveau"));
    create->setIcon(QIcon(":images/add.png"));
    delete_r = new QAction(tr("Supprimer"));
    delete_r->setIcon(QIcon(":images/minus.png"));

    connect(ok, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(coloursT, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenu(QPoint)));
       connect(create, SIGNAL(triggered(bool)), this, SLOT(createRow()));
       connect(delete_r, SIGNAL(triggered(bool)), this, SLOT(deleteRow()));

    fullInformations();
}

void Settings::fullInformations()
{
    QMap<QString, QVariant> colours_data = settings->value("colours").toMap();
    QStandardItemModel* table_model = new QStandardItemModel(colours_data.size(), 2);
    QMap<QString, QVariant>::iterator it = colours_data.begin();
    for (int i =0 ;i < colours_data.size() ;i++)
    {
        QStandardItem *item = new QStandardItem(it.key());
        QStandardItem *item2 = new QStandardItem(it.value().toString());
        table_model->setItem(i, 0, item);
        table_model->setItem(i, 1, item2);
        it++;
    }
    coloursT->setModel(table_model);
}

void Settings::customMenu(const QPoint &pos)
{
   QMenu con_menu(this);

   if(!coloursT->indexAt(pos).isValid())
   {
       delete_r->setEnabled(false);
   }
   else
   {
       delete_r->setEnabled(true);
   }
   con_menu.addAction(create);
   con_menu.addAction(delete_r);
   con_menu.exec(QCursor::pos());
}
void Settings::createRow()
{
       QStandardItemModel *model =  static_cast<QStandardItemModel*>(coloursT->model());
    QStandardItem *itemPif = new QStandardItem("None");
    QStandardItem *itemPof = new QStandardItem("#FFFFFF");

   model->setItem(model->rowCount(), 0, itemPif);
   model->setItem(model->rowCount()-1, 1, itemPof);
   coloursT->setModel(model);
}

void Settings::deleteRow()
{
    QStandardItemModel *model = static_cast<QStandardItemModel*>(coloursT->model());
    model->removeRow(coloursT->selectionModel()->selection().indexes().at(0).row());
    coloursT->setModel(model);
}
