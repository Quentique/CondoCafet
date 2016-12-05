#include "settings.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QMap>
#include <QStandardItemModel>
#include <QStandardItem>

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

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(colours);
    layout->addLayout(boutons);

    setLayout(layout);
    setWindowTitle(tr("Paramètres"));

    connect(ok, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));

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
