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

    DbManager manager("C:\\Users\\Quentin DE MUYNCK\\Desktop\\database.db");

    ProductsEdit *edit = new ProductsEdit(manager.getDB());
    edit->show();

    setCentralWidget(centralWidget);
    setMinimumHeight(450);
    setMinimumWidth(900);
    setWindowTitle("CondoCafèt'");
    show();
}
