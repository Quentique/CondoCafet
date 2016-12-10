#include "mainwindow.h"
#include "dbmanager.h"
#include "productsedit.h"
#include "sellersedit.h"
#include "settings.h"
#include <QMenu>
#include <QAction>
#include <QMenuBar>

MainWindow::MainWindow()
{
    centralWidget = new QWidget();

    QMenu *fichier = menuBar()->addMenu(tr("&Fichier"));

    QAction *quitAction = new QAction(tr("&Quitter"), this);
    fichier->addAction(quitAction);

    QMenu *admin = menuBar()->addMenu(tr("&Administration"));

    QAction *productsManagement = new QAction(tr("Gestion des &produits"), this);
    QAction *sellersManagement = new QAction(tr("Gestion des &vendeurs"), this);
    QAction *settingsModif = new QAction(tr("Paramètres"), this);
    admin->addAction(productsManagement);
    admin->addAction(sellersManagement);
    admin->addSeparator();
    admin->addAction(settingsModif);

    manager = new DbManager("C:\\Users\\Quentin DE MUYNCK\\Desktop\\database.db");

    setCentralWidget(centralWidget);
    setMinimumHeight(450);
    setMinimumWidth(900);
    setWindowTitle("CondoCafèt'");
    show();

    connect(productsManagement, SIGNAL(triggered(bool)), this, SLOT(showProductsEdit()));
    connect(sellersManagement, SIGNAL(triggered(bool)), this, SLOT(showSellersEdit()));
    connect(settingsModif, SIGNAL(triggered(bool)), this, SLOT(showSettings()));
    connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
}

void MainWindow::showProductsEdit()
{
    ProductsEdit *pedit = new ProductsEdit(manager->getDB());
    pedit->show();
}

void MainWindow::showSellersEdit()
{
    SellersEdit *sedit = new SellersEdit(manager->getDB());
    sedit->show();
}

void MainWindow::showSettings()
{
    Settings *settings = new Settings;
    settings->exec();
}
