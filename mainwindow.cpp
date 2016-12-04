#include "mainwindow.h"
#include "dbmanager.h"
#include "productsedit.h"
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

    QAction *productsManagement = new QAction(tr("&Gestion des produits"), this);
    admin->addAction(productsManagement);

    manager = new DbManager("C:\\Users\\Quentin DE MUYNCK\\Desktop\\database.db");

    setCentralWidget(centralWidget);
    setMinimumHeight(450);
    setMinimumWidth(900);
    setWindowTitle("CondoCafèt'");
    show();

    connect(productsManagement, SIGNAL(triggered(bool)), this, SLOT(showProductsEdit()));
}

void MainWindow::showProductsEdit()
{
    ProductsEdit *edit = new ProductsEdit(manager->getDB());
    edit->show();
}
