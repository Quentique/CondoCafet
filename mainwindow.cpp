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
#include <QCoreApplication>
#include <QDebug>
#include <QKeySequence>
#include <QSqlQuery>
#include "sellerselector.h"
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
    current = 0;
    multiplyby = 0;
   /* current->addArticle(new Product("Chocolat", 10.20, "Beu"), 5);
    current->addArticle(new Product("Bonbons", 152, "rouge"), 1);*/

    psettings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    if(psettings->value("sold", -1).toInt() == -1)
        psettings->setValue("sold", 0);

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

    dated = new QLineEdit;
    dated->setText(QDate::currentDate().toString("dddd d MMMM yyyy"));
    dated->setReadOnly(true);

    sellerd = new QLineEdit;
    sellerd->setReadOnly(true);
    sellerd->setAlignment(Qt::AlignCenter);
    if (seller == 0)
    {
        sellerd->setText(tr("VERROUILLÉ"));
    } else { sellerd->setText(seller->getName()); }

    soldd = new QLabel;

    countd = new QDoubleSpinBox;
    countd->setReadOnly(true);
    countd->setValue(0.00f);
    countd->setButtonSymbols(QDoubleSpinBox::NoButtons);
    countd->setSuffix(" €");

    QHBoxLayout *displayer = new QHBoxLayout;
    displayer->addWidget(dated, 0, Qt::AlignLeft);
    displayer->addWidget(sellerd, 1, Qt::AlignCenter);
    displayer->addWidget(soldd, 0, Qt::AlignRight);
    displayer->addWidget(countd, 0, Qt::AlignRight);

    sign = new QPushButton(tr("Sign In"));
    rush = new QPushButton(tr("RUSH Mode"));
    totald = new QLineEdit;
    totald->setReadOnly(true);
    totald->setText("0.00 €");
    totald->setAlignment(Qt::AlignRight);
    totald->setFont(QFont("Calibri", 36));


    QHBoxLayout *utilities = new QHBoxLayout;
    utilities->addWidget(sign, 0, Qt::AlignLeft);
    utilities->addWidget(rush, 0, Qt::AlignLeft);
    utilities->addWidget(totald, 2, Qt::AlignRight);

    manager = new DbManager("C:\\Users\\Quentin DE MUYNCK\\Desktop\\database.db");

    sold_details = new QTableWidget(1, 4);
    sold_details->setSelectionMode(QAbstractItemView::NoSelection);
    sold_details->setEditTriggers(QAbstractItemView::NoEditTriggers);
    sold_details->setSelectionBehavior(QAbstractItemView::SelectRows);
    sold_details->setShowGrid(false);
    sold_details->verticalHeader()->setVisible(false);
    sold_details->setFocusPolicy(Qt::NoFocus);
    sold_details->horizontalHeader()->setStretchLastSection(true);
        sold_details->setSortingEnabled(false);
    totald->setMinimumWidth(sold_details->sizeHint().width());
    actualiseTable();

    QGridLayout *calclay = new QGridLayout;

    calc_mapper = new QSignalMapper;
    for (int i = 0 ; i < 10 ; i++)
    {
        calc[i] = new QPushButton(QString::number(i));
        calc_mapper->setMapping(calc[i], i);
        connect(calc[i], SIGNAL(clicked(bool)), calc_mapper, SLOT(map()));
    }
    calclay->addWidget(calc[0], 3, 0);
    calclay->addWidget(calc[1], 2, 0);
    calclay->addWidget(calc[2], 2, 1);
    calclay->addWidget(calc[3], 2, 2);
    calclay->addWidget(calc[4], 1, 0);
    calclay->addWidget(calc[5], 1, 1);
    calclay->addWidget(calc[6], 1, 2);
    calclay->addWidget(calc[7], 0, 0);
    calclay->addWidget(calc[8], 0, 1);
    calclay->addWidget(calc[9], 0, 2);

    calc[10] = new QPushButton("×");
    calc[11] = new QPushButton("C");
    calc[12] = new QPushButton("00");
    calc[13] = new QPushButton(".");
    calc[14] = new QPushButton("⇧");
    calc[15] = new QPushButton("⇩");

    connect(calc[11], SIGNAL(clicked(bool)), this, SLOT(touchC()));
    connect(calc[14], SIGNAL(clicked(bool)), this, SLOT(up()));
    connect(calc[15], SIGNAL(clicked(bool)), this, SLOT(down()));

    calclay->addWidget(calc[14],2, 3);
    calclay->addWidget(calc[15], 3, 3);
    calclay->addWidget(calc[10], 0, 3);
    calclay->addWidget(calc[11], 1, 3);
    calclay->addWidget(calc[12], 3, 1);
    calclay->addWidget(calc[13], 3, 2);

    calclay->setSizeConstraint(QLayout::SetFixedSize);


    pay = new QPushButton(tr("PAYER"));
    cancel = new QPushButton(tr("ANNULER"));
    retour = new QPushButton(tr("Retour"));
    totalmd = new QPushButton(tr("TOTAL"));

    QVBoxLayout *cash = new QVBoxLayout;
    cash->addWidget(pay, 0);
    cash->addWidget(cancel, 0);
    cash->addWidget(retour, 0);
    cash->addWidget(totalmd, 2, Qt::AlignTop);

    QStringList money;
    money << "0.01" << "0.02" << "0.05" << "0.10" << "0.20" << "0.50" << "1" << "2" << "5" << "10" << "20" << "50";

    for (int i = 0 ; i < 12 ; i++)
    {
        coins[i] = new QPushButton;
        coins[i]->setIcon(QIcon(QPixmap(":/coins/" + money.at(i) + ".png")));
        coins[i]->setToolTip(money.at(i));
        coins[i]->setIconSize(QSize(70, 70));

    }

    QGridLayout *moneylay = new QGridLayout;
    moneylay->addWidget(coins[0], 5, 0);
    moneylay->addWidget(coins[1], 4, 0);
    moneylay->addWidget(coins[2], 3, 0);
    moneylay->addWidget(coins[3], 2, 0);
    moneylay->addWidget(coins[4], 1, 0);
    moneylay->addWidget(coins[5], 0, 0);
    moneylay->addWidget(coins[6], 5, 1);
    moneylay->addWidget(coins[7], 4, 1);
    moneylay->addWidget(coins[8], 3, 1);
    moneylay->addWidget(coins[9], 2, 1);
    moneylay->addWidget(coins[10], 1, 1);
    moneylay->addWidget(coins[11], 0, 1);

    QMap<QString, QVariant> cat = psettings->value("colours").toMap();
    QSqlQuery query("SELECT name, price, colour FROM products ORDER BY colour", *manager->getDB());
    product_list = new QHash<QString, Product*>;
    products_mapper = new QSignalMapper;

    while (query.next())
    {
        Product *produit = new Product(query.value("name").toString(), query.value("price").toDouble(), query.value("colour").toString());
        product_list->insert(query.value("name").toString(), produit);
    }
    QGridLayout *products_lay = new QGridLayout;
    int number_columns = 0;
    int cell = 0;
    for (QMap<QString, QVariant>::iterator it = cat.begin() ; it != cat.end() ; it++)
    {
        for (QHash<QString, Product*>::iterator its = product_list->begin() ; its != product_list->end() ; its++)
        {
            if (its.value()->getCategorie() == it.value().toString())
            {
                QPushButton *button = new QPushButton(its.key() + "\n" + QString::number(its.value()->getPrice(), 'g', 2) + " €");
                button->setStyleSheet("background-color: " + its.value()->getCategorie());
                products_mapper->setMapping(button, its.key());
                connect(button, SIGNAL(clicked(bool)), products_mapper, SLOT(map()));
                products_lay->addWidget(button, cell, number_columns);
                cell++;
                if(cell>5) {
                    number_columns++;
                    cell = 0;
                }
            }
        }
        number_columns++;
        cell = 0;
    }


    QHBoxLayout *options = new QHBoxLayout;
    options->addLayout(moneylay);
    options->addLayout(cash);

    QVBoxLayout *general = new QVBoxLayout;

    general->addLayout(displayer, 0);
    general->addLayout(utilities, 2);

    QGridLayout *grill = new QGridLayout;
    grill->addWidget(sold_details, 0, 1);
    grill->addLayout(products_lay, 0, 0);
    grill->addLayout(calclay, 1, 1);
    grill->addLayout(options, 1, 0);

    grill->setAlignment(calclay, Qt::AlignTop);
    grill->setSizeConstraint(QLayout::SetFixedSize);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(general);
    layout->addLayout(grill);



    setCentralWidget(centralWidget);
    //setWindowState(Qt::WindowFullScreen);
    setWindowTitle("CondoCafèt'");
    centralWidget->setLayout(layout);
    show();

    connect(productsManagement, SIGNAL(triggered(bool)), this, SLOT(showProductsEdit()));
    connect(sellersManagement, SIGNAL(triggered(bool)), this, SLOT(showSellersEdit()));
    connect(settingsModif, SIGNAL(triggered(bool)), this, SLOT(showSettings()));
    connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(sign, SIGNAL(clicked(bool)), this, SLOT(sign_slot()));
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(cancelSell()));
    connect(totalmd, SIGNAL(clicked(bool)), this, SLOT(showTotal()));
    connect(calc_mapper, SIGNAL(mapped(int)), this, SLOT(multiply(int)));
    connect(products_mapper, SIGNAL(mapped(QString)), this, SLOT(addProduct(QString)));
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
        sold_details->horizontalHeader()->setVisible(false);
        QTableWidgetItem *item = new QTableWidgetItem(tr("VERROUILLE"));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont("Arial", 20));
        sold_details->setItem(0, 0, item);
        /*sold_details->setColumnWidth(0, sold_details->width());
        sold_details->setRowHeight(0, sold_details->height());*/

    }
    else
    {
        sold_details->clear();
        sold_details->setRowCount(0);
        sold_details->setColumnCount(2);
        QStringList name;
        name << "Qt" << "Dénomination";
        sold_details->horizontalHeader()->setVisible(true);
        sold_details->setHorizontalHeaderLabels(name);
        if(current!=0)
        {
            for (int i = 0 ; i != current->count() ; i++)
            {
                QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(current->getArticle(i).getQuantity()) + " ×");
                QTableWidgetItem *item2 = new QTableWidgetItem(current->at(i));
                sold_details->insertRow(sold_details->rowCount());
                sold_details->setItem(sold_details->rowCount()-1, 0, item1);
                sold_details->setItem(sold_details->rowCount()-1, 1, item2);
            }
           // sold_details->setCurrentCell(sold_details->rowCount()-1, 0);
            sold_details->setRangeSelected(QTableWidgetSelectionRange(sold_details->rowCount()-1, 0, sold_details->rowCount()-1, sold_details->columnCount()-1), true);
            sold_details->resizeRowsToContents();
            sold_details->resizeColumnToContents(0);
            sold_details->horizontalHeader()->setStretchLastSection(true);
            if (totald->text().contains("TOTAL"))
            {
                showTotal();
            }
        }
       /* sold_details->setColumnWidth(0, sold_details->width()*0.2);
        sold_details->setColumnWidth(1, sold_details->width()*0.7);
        sold_details->setColumnWidth(2, sold_details->width()*0.15);
        sold_details->setColumnWidth(3, sold_details->width()*0.15);*/
    }
}

void MainWindow::sign_slot()
{
    if (sign->text() == tr("Sign In"))
    {
        SellerSelector *selector = new SellerSelector(0, manager->getDB());
        selector->exec();
        int result = selector->getResult();
        QSqlQuery query("SELECT name, class FROM sellers WHERE id = " + QString::number(result), *manager->getDB());
        query.next();
        Seller *sellerr = new Seller(query.value("name").toString(), query.value("class").toString());
        seller = sellerr;
        actualiseVendeur();
        sign->setText(tr("Sign Out"));
        current = new Vente(psettings->value("sold").toInt() + 1);
    }
    else
    {
        QSqlQuery query("SELECT amount FROM sellers WHERE name = '" + seller->getName() + "'", *manager->getDB());
        query.next();
        double amount = query.value("amount").toDouble();
        qDebug() << amount;
        amount += seller->getAmount();
        query.prepare("UPDATE sellers SET amount = ? WHERE name = ?");
        query.bindValue(0, amount);
        query.bindValue(1, seller->getName());
        query.exec();
        delete seller;
        seller = 0;
        actualiseVendeur();
        sign->setText(tr("Sign In"));
        delete current;
        current = 0;
    }
}

void MainWindow::actualiseVendeur()
{
    if (seller != 0) {
    actualiseTable();
    sellerd->setText(seller->getName());
    soldd->setText("Vente N°" + QString::number(psettings->value("sold").toInt() + 1));
    } else {
        actualiseTable();
        sellerd->setText("VERROUILLÉ");
        soldd->setText("");
    }
}
void MainWindow::cancelSell()
{
    if (current != 0) {
    int number = current->getNumber();
    delete current;
    current = new Vente(number);
    actualiseTable();
    multiplyby = 0;
    }
}

void MainWindow::showTotal()
{
    qDebug() << "cool";
    if (current != 0) {
    totald->setText("TOTAL : " + QString::number(current->getTotal(), 'f', 2) + " €");
    qDebug() << "work;";
    multiplyby = 0;
    }
}

void MainWindow::multiply(int gnumber)
{
    if (current != 0) {
    multiplyby = (multiplyby==0) ? gnumber : QString(QString::number(multiplyby) + QString::number(gnumber)).toInt();
    totald->setText(QString::number(multiplyby) + " ×");
    }
}

void MainWindow::touchC()
{
    if (multiplyby != 0)
    {
        multiplyby = 0;
        totald->setText("0 ×");
    }
}

void MainWindow::addProduct(QString gname)
{
    if (current != 0)
    {
        current->addArticle(product_list->find(gname).value(), (multiplyby!=0) ? multiplyby : 1);
        actualiseTable();
        showTotal();
    }
}

void MainWindow::up()
{
    if (!sold_details->selectedRanges().isEmpty())
    {
        if (sold_details->selectedRanges().first().bottomRow() != 0) {
        int r = sold_details->selectedRanges().first().bottomRow();
        sold_details->clearSelection();
        sold_details->setRangeSelected(QTableWidgetSelectionRange(r-1, 0, r-1, sold_details->columnCount()-1), true);
        }
    }
}

void MainWindow::down()
{
    if (!sold_details->selectedRanges().isEmpty())
    {
        if (sold_details->selectedRanges().first().bottomRow() != sold_details->rowCount()-1) {
        int r = sold_details->selectedRanges().first().bottomRow();
        sold_details->clearSelection();
        sold_details->setRangeSelected(QTableWidgetSelectionRange(r+1, 0, r+1, sold_details->columnCount()-1), true);
        }
    }
}
