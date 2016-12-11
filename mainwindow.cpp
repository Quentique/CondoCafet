#include "mainwindow.h"
#include "dbmanager.h"
#include "productsedit.h"
#include "sellersedit.h"
#include "settings.h"
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
static QSize myGetQTableWidgetSize(QTableWidget *t) {
   int w = t->verticalHeader()->width() + 4; // +4 seems to be needed
   for (int i = 0; i < t->columnCount(); i++)
      w += t->columnWidth(i); // seems to include gridline (on my machine)
   int h = t->horizontalHeader()->height() + 4;
   for (int i = 0; i < t->rowCount(); i++)
      h += t->rowHeight(i);
   return QSize(w, h);
}
MainWindow::MainWindow()
{
    centralWidget = new QWidget();
    seller = 0;

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

    sold_details = new QTableWidget(1, 4);
    sold_details->setMaximumWidth(300);
    sold_details->setSelectionMode(QAbstractItemView::NoSelection);
    sold_details->setEditTriggers(QAbstractItemView::NoEditTriggers);
    sold_details->setShowGrid(false);
    sold_details->horizontalHeader()->setVisible(false);
    sold_details->verticalHeader()->setVisible(false);
    sold_details->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    sold_details->setMinimumSize(myGetQTableWidgetSize(sold_details));
    sold_details->setMinimumHeight((height()*0.5));
    sold_details->setFrameStyle(QFrame::NoFrame);
    sold_details->setFocusPolicy(Qt::NoFocus);
    actualiseTable();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(sold_details, 4, Qt::AlignRight);

    centralWidget->setLayout(layout);
    //centralWidget->layout()->setSizeConstraint(QLayout::SetMinimumSize);

    setCentralWidget(centralWidget);
   // setWindowState(Qt::WindowFullScreen);
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

void MainWindow::actualiseTable()
{
    if (seller == 0)
    {
        sold_details->clear();
        sold_details->setColumnCount(1);
        QTableWidgetItem *item = new QTableWidgetItem(tr("VERROUILLE"));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont("Arial", 20));
        sold_details->setItem(0, 0, item);
        sold_details->setColumnWidth(0, sold_details->width());
        sold_details->setRowHeight(0, sold_details->height());
    }
}
