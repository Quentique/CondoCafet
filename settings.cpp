#include "settings.h"
#include "colordelegate.h"
#include "color_wheel.hpp"

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
#include <QHeaderView>
#include <QColor>
#include <QPixmap>
#include <QSqlQuery>
#include <QFormLayout>
#include <QInputDialog>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QStandardPaths>

Settings::Settings(QSqlDatabase *db) : QDialog()
{
    settings = new QSettings(QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).at(1) + "/settings.ini", QSettings::IniFormat);
    ok = new QPushButton(tr("Valider"));
    ok->setDefault(true);
    cancel = new QPushButton(tr("Annuler"));
    sql = db;

    QHBoxLayout *boutons = new QHBoxLayout;
    boutons->addWidget(ok, 0, Qt::AlignRight);
    boutons->addWidget(cancel, 0, Qt::AlignRight);

    general = new QGroupBox;
    general->setTitle(tr("Général"));

    passwordchange = new QPushButton("Changer");

    QFormLayout *form = new QFormLayout;
    form->addRow("Mot de passe : ", passwordchange);
    general->setLayout(form);

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
    coloursT->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    coloursT->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    coloursT->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    coloursT->setStyleSheet("QTableView{ background-color: #F5F5F5; }");
    coloursT->setItemDelegateForColumn(1, new ColorDelegate(this));
    coloursT->setMinimumHeight(220);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(general);
    layout->addWidget(colours);
    layout->addLayout(boutons);

    setLayout(layout);
    setWindowTitle(tr("Paramètres"));

    create = new QAction(tr("Nouveau"));
    create->setIcon(QIcon(":images/add.png"));
    delete_r = new QAction(tr("Supprimer"));
    delete_r->setIcon(QIcon(":images/minus.png"));

    fullInformation();

    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(coloursT, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenu(QPoint)));
    connect(create, SIGNAL(triggered(bool)), this, SLOT(createRow()));
    connect(delete_r, SIGNAL(triggered(bool)), this, SLOT(deleteRow()));
    connect(coloursT, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(expandRow(QModelIndex)));
    connect(coloursT->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(resizeRow(QModelIndex,QModelIndex)));
    connect(ok, SIGNAL(clicked(bool)), this, SLOT(writeInformation()));
    connect(passwordchange, SIGNAL(clicked(bool)), this, SLOT(passwordHelp()));
}

void Settings::fullInformation()
{
    QMap<QString, QVariant> colours_data = settings->value("colours").toMap();
    QStandardItemModel* table_model = new QStandardItemModel(colours_data.size(), 2);
    QMap<QString, QVariant>::iterator it = colours_data.begin();
    for (int i =0 ;i < colours_data.size() ;i++)
    {
        QStandardItem *item = new QStandardItem(it.key());
        QStandardItem *item2 = new QStandardItem(it.value().toString());
        item2->setData(it.value().toString());
        QPixmap map(100, 100);
        map.fill(QColor(it.value().toString()));
        item2->setIcon(QIcon(map));
        table_model->setItem(i, 0, item);
        table_model->setItem(i, 1, item2);
        it++;
    }
    table_model->setHeaderData(0, Qt::Horizontal, "Nom");
    table_model->setHeaderData(1, Qt::Horizontal, "Couleur");
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


    QPixmap map(100, 100);
    map.fill(QColor("#FFFFFF"));
    QIcon icon(map);

    itemPof->setIcon(icon);

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

void Settings::expandRow(QModelIndex index)
{
    if (index.column() == 1)
        coloursT->setRowHeight(index.row(), 200);
}

void Settings::resizeRow(QModelIndex, QModelIndex index)
{
    coloursT->setRowHeight(index.row(), 30);

    if (index.column() == 1) {
        QStandardItemModel *modell =  static_cast<QStandardItemModel*>(coloursT->model());
        QStandardItem *itemPif = modell->itemFromIndex(index);

        QPixmap map(100, 100);
        map.fill(QColor(itemPif->data(Qt::DisplayRole).toString()));
        QIcon icon(map);
        itemPif->setIcon(icon);

        modell->setItem(index.row(), index.column(), itemPif);
        coloursT->setModel(modell);
    }
}

void Settings::writeInformation()
{
    QStandardItemModel *model = static_cast<QStandardItemModel*>(coloursT->model());
    QMap<QString, QVariant> map;

    for (int i = 0 ; i != model->rowCount() ; i++)
    {
        map.insert(model->item(i, 0)->text(), model->item(i, 1)->text());
        qDebug() << model->item(i, 0)->text();
        QSqlQuery query(*sql);
        query.prepare("UPDATE products SET colour = ? WHERE colour = ?");
        query.bindValue(0, model->item(i, 1)->text());
        query.bindValue(1, model->item(i, 1)->data().toString());
        query.exec();
    }

    settings->setValue("colours", map);
    accept();
}

void Settings::passwordHelp()
{
    QString hash = settings->value("admin", "NULL").toString();

    if (hash != "NULL")
    {
         QString password = QInputDialog::getText(this, tr("Modification"), tr("Merci d'entrer l'ancien mot de passe"), QLineEdit::Password);
         if (hash != QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256))
         {
            return;
         }
    }

    QString passwordnew, passwordnew2;
    passwordnew = "NULL";
    passwordnew2 = "NULL";
    bool continued = true;

    while(continued)
    {
        passwordnew = QInputDialog::getText(this, tr("Modification"), tr("Merci d'entrer le nouveau mot de passe"), QLineEdit::Password);
        passwordnew2 = QInputDialog::getText(this, tr("Modification"), tr("Merci d'entrer de nouveau le nouveau mot de passe"), QLineEdit::Password);
        if (passwordnew == passwordnew2)
            continued = false;
        else
            QMessageBox::warning(this, tr("Erreur"), tr("Les mots de passe ne sont pas identiques"));
    }

    settings->setValue("admin", QCryptographicHash::hash(passwordnew.toUtf8(), QCryptographicHash::Sha256));
    QMessageBox::information(this, tr("Mot de passe modifié"), tr("Le mot de passe administrateur a bien été modifié."));

}
