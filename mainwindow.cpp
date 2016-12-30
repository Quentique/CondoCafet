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
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QTextStream>
#include <QCloseEvent>
#include <QInputDialog>
#include <QCryptographicHash>
#include "sellerselector.h"

MainWindow::MainWindow()
{
    centralWidget = new QWidget();
    seller = 0;
    current = 0;
    multiplyby = 0;
    payment = 0.00f;
    payMode = false;
    rushMode = false;

    psettings = new QSettings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    if(psettings->value("sold", -1).toInt() == -1)
        psettings->setValue("sold", 0);

    QMenu *fichier = menuBar()->addMenu(tr("&Fichier"));

    QAction *quitAction = new QAction(tr("&Quitter"), this);
    fichier->addAction(quitAction);

    QMenu *admin = menuBar()->addMenu(tr("&Administration"));

    productsManagement = new QAction(tr("Gestion des &produits"), this);
    sellersManagement = new QAction(tr("Gestion des &vendeurs"), this);
    settingsModif = new QAction(tr("Paramètres"), this);
    QAction *unlock = new QAction(tr("Dévérouiller"), this);
    productsManagement->setEnabled(false);
    sellersManagement->setEnabled(false);
    settingsModif->setEnabled(false);
    admin->addAction(productsManagement);
    admin->addAction(sellersManagement);
    admin->addAction(settingsModif);
    admin->addSeparator();
    admin->addAction(unlock);

    dated = new QLineEdit;
    dated->setText(QDate::currentDate().toString("dddd d MMMM yyyy"));
    dated->setReadOnly(true);
    dated->setMinimumWidth(dated->sizeHint().width()+50);
    dated->setProperty("stylegen", true);

    sellerd = new QLineEdit;
    sellerd->setReadOnly(true);
    sellerd->setAlignment(Qt::AlignCenter);
    sellerd->setProperty("stylegen", true);
    if (seller == 0) { sellerd->setText(tr("VERROUILLÉ"));
    } else { sellerd->setText(seller->getName()); }

    soldd = new QLabel;
    soldd->setProperty("stylegen", true);

    countd = new QDoubleSpinBox;
    countd->setReadOnly(true);
    countd->setValue(0.00f);
    countd->setButtonSymbols(QDoubleSpinBox::NoButtons);
    countd->setSuffix(" €");
    countd->setProperty("stylegen", true);

    QHBoxLayout *displayer = new QHBoxLayout;
    displayer->addWidget(dated, 0, Qt::AlignLeft);
    displayer->addWidget(sellerd, 1, Qt::AlignCenter);
    displayer->addWidget(soldd, 0, Qt::AlignRight);
    displayer->addWidget(countd, 0, Qt::AlignRight);

    sign = new QPushButton(tr("Sign In"));
    rush = new QPushButton(tr("RUSH Mode"));
    rush->setStyleSheet("background-color: red; color: white; font-weight: bold;");
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
    sold_details->setMaximumWidth(300+sold_details->sizeHint().width());
    totald->setMinimumWidth(sold_details->sizeHint().width());
    actualiseTable();

    QGridLayout *calclay = new QGridLayout;

    calc_mapper = new QSignalMapper;
    for (int i = 0 ; i < 10 ; i++)
    {
        calc[i] = new QPushButton(QString::number(i));
        calc[i]->setShortcut(QKeySequence(QString::number(i)));
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
    calc[10]->setShortcut(QKeySequence("*"));
    calc[11] = new QPushButton("C");
    calc[11]->setShortcut(QKeySequence(Qt::Key_Backspace));
    calc[12] = new QPushButton("⇧");
    calc[12]->setShortcut(QKeySequence(Qt::Key_Up));
    calc[13] = new QPushButton("⇩");
    calc[13]->setShortcut(QKeySequence(Qt::Key_Down));

    for (int i = 0 ; i < 14 ; i++)
    {
        calc[i]->setStyleSheet("font-size: 20px; font-family: Calibri");
        calc[i]->setMaximumWidth(100);
    }

    connect(calc[10], SIGNAL(clicked(bool)), this, SLOT(touchX()));
    connect(calc[11], SIGNAL(clicked(bool)), this, SLOT(touchC()));
    connect(calc[12], SIGNAL(clicked(bool)), this, SLOT(up()));
    connect(calc[13], SIGNAL(clicked(bool)), this, SLOT(down()));

    calclay->addWidget(calc[12], 2, 3);
    calclay->addWidget(calc[13], 3, 3);
    calclay->addWidget(calc[10], 0, 3);
    calclay->addWidget(calc[11], 1, 3);

    pay = new QPushButton(tr("PAYER"));
    pay->setStyleSheet("width: inherit;");
    cancel = new QPushButton(tr("ANNULER"));
    totalmd = new QPushButton(tr("TOTAL"));

    QVBoxLayout *cash = new QVBoxLayout;
    cash->addWidget(pay, 0);
    cash->addWidget(cancel, 0);
    cash->addWidget(totalmd, 2, Qt::AlignTop);

    QStringList money;
    money << "0.01" << "0.02" << "0.05" << "0.10" << "0.20" << "0.50" << "1" << "2" << "5" << "10" << "20" << "50";

    money_mapper = new QSignalMapper;

    for (int i = 0 ; i < 12 ; i++)
    {
        coins[i] = new QPushButton;
        coins[i]->setIcon(QIcon(QPixmap(":/coins/" + money.at(i) + ".png")));
        coins[i]->setToolTip(money.at(i));
        coins[i]->setIconSize(QSize(60, 60));
        money_mapper->setMapping(coins[i], money.at(i));
        connect(coins[i], SIGNAL(clicked(bool)), money_mapper, SLOT(map()));
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
                QPushButton *button = new QPushButton(its.key() + "\n" + QString::number(its.value()->getPrice(), 'f', 2) + " €");
                button->setStyleSheet("background-color: " + its.value()->getCategorie());
                products_mapper->setMapping(button, its.key());
                connect(button, SIGNAL(clicked(bool)), products_mapper, SLOT(map()));
                products_lay->addWidget(button, cell, number_columns);
                cell++;
                if(cell>4) {
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
    grill->addWidget(sold_details, 0, 2);
    grill->addLayout(products_lay, 0, 0);
    grill->addLayout(calclay, 1, 2, 1, 1, Qt::AlignRight);
    grill->addLayout(options, 1, 0);

    grill->setAlignment(calclay, Qt::AlignTop);
    grill->setColumnStretch(1, 20);
    grill->addItem(new QSpacerItem(20, 20), 0, 1);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(general);
    layout->addLayout(grill);

    setCentralWidget(centralWidget);
   // setWindowState(Qt::WindowFullScreen);
    setWindowTitle("CondoCafet'");
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
    connect(money_mapper, SIGNAL(mapped(QString)), this, SLOT(moneyTouch(QString)));
    connect(pay, SIGNAL(clicked(bool)), this, SLOT(paySlot()));
    connect(rush, SIGNAL(clicked(bool)), this, SLOT(rushTouch()));
    connect(unlock, SIGNAL(triggered(bool)), this, SLOT(unlock()));
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
    Settings *settings = new Settings(manager->getDB());
    settings->exec();
}

void MainWindow::unlock()
{
    QAction *action = static_cast<QAction*>(sender());
    if (action->text() == tr("Dévérouiller"))
    {
        if (psettings->value("admin", "NULL") == "NULL")
        {
            sellersManagement->setEnabled(true);
            productsManagement->setEnabled(true);
            settingsModif->setEnabled(true);
            action->setText(tr("Verouiller"));
        }
        else
        {
            QString hash = psettings->value("admin").toString();
            QString password = QInputDialog::getText(this, tr("Accès protégé"), tr("Merci d'entrer le mot de passe administrateur"), QLineEdit::Password);
            if (hash == QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256))
            {
                sellersManagement->setEnabled(true);
                productsManagement->setEnabled(true);
                settingsModif->setEnabled(true);
                action->setText(tr("Verouiller"));
            }
            else
            {
                QMessageBox::critical(this, tr("Accès refusé"), tr("Mot de passe erroné\nAccès refusé"));
            }
        }
    } else
    {
        sellersManagement->setEnabled(false);
        productsManagement->setEnabled(false);
        settingsModif->setEnabled(false);
        action->setText(tr("Dévérouiller"));
    }
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
        sold_details->setItem(sold_details->rowCount()-1, 0, item);
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
            sold_details->setRangeSelected(QTableWidgetSelectionRange(sold_details->rowCount()-1, 0, sold_details->rowCount()-1, sold_details->columnCount()-1), true);
            sold_details->resizeRowsToContents();
            sold_details->resizeColumnToContents(0);
            sold_details->horizontalHeader()->setStretchLastSection(true);

            if (totald->text().contains("TOTAL"))
            {
                showTotal();
            }
        }
    }
}

void MainWindow::sign_slot()
{
    if (sign->text() == tr("Sign In"))
    {
        SellerSelector *selector = new SellerSelector(0, manager->getDB());
        selector->exec();
        int result = selector->getResult();

        QSqlQuery query("SELECT name, class, birthday FROM sellers WHERE id = " + QString::number(result), *manager->getDB());
        query.next();

        QString password = query.value("birthday").toString();
        password.remove("-");

        QString entree = QInputDialog::getText(this, "Connexion", "Merci de saisir votre date de naissance sous le format JJMMAAA\n(sans tirets) afin de vous connecter",  QLineEdit::Password, "", nullptr, Qt::Dialog , Qt::ImhDigitsOnly);
        if (entree == password)
        {
            Seller *sellerr = new Seller(query.value("name").toString(), query.value("class").toString(), query.value("birthday").toString());
            seller = sellerr;
            actualiseVendeur();
            sign->setText(tr("Sign Out"));
            current = new Vente(psettings->value("sold").toInt() + 1);
            totald->setText("0.00 €");

            QString location = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).at(1);
            location += "/log";
            QFileInfo info(location);
            if (!info.exists())
            {
                QDir cur(QDir::current());
                cur.mkpath(location);
            }

            location += "/Ventes_" + QDate::currentDate().toString("dd-MM-yyyy") + ".log";

            QFile file(location);
            QFileInfo info2(location);
            if (!info2.exists()) {
                file.open(QFile::WriteOnly | QFile::Append | QFile::Text);
                QTextStream stream(&file);
                stream.setCodec("UTF-8");
                stream.setGenerateByteOrderMark(true);

                stream << "************** CondoCafet **************" << endl;
                stream << QDate::currentDate().toString("dddd dd MMMM yyyy") << endl;
                stream << "****************************************" << endl;
                file.close();
            }

            if (!file.open(QFile::WriteOnly | QFile::Append | QFile::Text))
            {
                QMessageBox::critical(this, tr("Erreur d'E/S"), tr("CondoCafet n'a pas pu écrire le log de cette session de vente."));
            }
            else
            {
                QTextStream stream(&file);
                stream.setCodec("UTF-8");
                stream << endl << QString::fromUtf8("Ouverture à ") << seller->getEnterTime().toString("HH:mm") << " par " << seller->getName() << ", " << seller->getClass();
                file.close();
            }
        } else
            QMessageBox::critical(this, tr("Mot de passe erroné"), tr("La date de naissance entrée est erronée.\nConnexion refusée"));
    }
    else
    {
        int reponse = 0x00004000;
        if (!current->isEmpty())
        {
             reponse = QMessageBox::warning(this, tr("ATTENTION"), tr("Vous essayez de vous déconnecter alors que la vente en cours n'est pas terminée\nÊtes-vous sûr de vouloir vous déconnecter ?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        }

        if (reponse == QMessageBox::Yes) {
            QSqlQuery query("SELECT amount FROM sellers WHERE name = '" + seller->getName() + "'", *manager->getDB());
            query.next();
            double amount = query.value("amount").toDouble();
            amount += seller->getAmount();

            query.prepare("UPDATE sellers SET amount = ? WHERE name = ?");
            query.bindValue(0, amount);
            query.bindValue(1, seller->getName());
            query.exec();

            QString location = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).at(1);
            location += "/log/Ventes_" + QDate::currentDate().toString("dd-MM-yyyy") + ".log";
            QFile file(location);
            if (!file.open(QFile::WriteOnly | QFile::Append | QFile::Text))
            {
                QMessageBox::critical(this, tr("Erreur d'E/S"), tr("CondoCafet n'a pas pu écrire le log de cette session de vente."));
            }
            else
            {
                QTextStream stream(&file);
                stream.setCodec("UTF-8");
                stream << endl << QString::fromUtf8("Fermeture à ") << QDateTime::currentDateTime().toString("HH:mm") << ", " << QString::number(seller->getSoldCount()) << " ventes pour un total de " << QString::number(seller->getAmount(), 'f', 2) << QString::fromUtf8(" €") << endl;
                stream << "****************************************" << endl;
                file.close();
            }
            delete seller;
            seller = 0;
            actualiseVendeur();
            sign->setText(tr("Sign In"));
            totald->setText(tr("VERROUILLÉ"));
            delete current;
            current = 0;
            countd->setValue(0.00f);
        }
    }
}

void MainWindow::actualiseVendeur()
{
    if (seller != 0) {
        actualiseTable();
        sellerd->setText(seller->getName());
        soldd->setText(tr("Vente N°") + QString::number(psettings->value("sold").toInt() + 1));
    } else {
        actualiseTable();
        sellerd->setText(tr("VERROUILLÉ"));
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
        payment = 0;
        payMode = false;
    }
}

void MainWindow::showTotal()
{
    if (current != 0) {
        QString start = (payMode) ? tr("RESTANT : ") : tr("TOTAL : ");
        if (payMode && payment > current->getTotal())
            start = tr("À RENDRE : ");

        totald->setText(start + QString::number((payMode) ? current->getTotal() - payment : current->getTotal(), 'f', 2) + " €");

        if (start == "À RENDRE : ")
            totald->setText(start + QString::number(qAbs(current->getTotal()-payment), 'f', 2) + " €");
        if (rushMode)
            totald->setText("RUSH IN PROGRESS");

        multiplyby = 0;
    }
}

void MainWindow::multiply(int gnumber)
{
    if (current != 0) {
        multiplyby = (multiplyby==0) ? gnumber : QString(QString::number(multiplyby) + QString::number(gnumber)).toInt();
        QString what = (!payMode) ? " ×" : "";
        totald->setText(QString::number(multiplyby) + what);
    }
}

void MainWindow::touchC()
{
    if (calc[11]->text() == "C") {
        if (multiplyby != 0) {
            multiplyby = 0;
            showTotal();
        }
        else if (!sold_details->selectedRanges().isEmpty()) {
            current->deleteArticle(product_list->find(sold_details->item(sold_details->selectedRanges().first().bottomRow(), 1)->text()).value());
            actualiseTable();
        }
    } else {
        current->setQuantity(product_list->find(sold_details->item(sold_details->selectedRanges().first().bottomRow(), 1)->text()).value(), sold_details->item(sold_details->selectedRanges().first().topRow(), 0)->text().left(sold_details->item(sold_details->selectedRanges().first().topRow(), 0)->text().length()-2).toInt() - 1);
        actualiseTable();
    }
}

void MainWindow::touchX()
{
    if (!payMode) {
        if (multiplyby != 0 && !sold_details->selectedRanges().isEmpty()) {
            current->setQuantity(product_list->find(sold_details->item(sold_details->selectedRanges().first().bottomRow(), 1)->text()).value(), multiplyby);
            showTotal();
            actualiseTable();
            multiplyby = 0;
        }
    } else {
        if (multiplyby != 0) {
            totald->setText(totald->text() + " ×");
        }
    }
}



void MainWindow::addProduct(QString gname)
{
    if (current != 0) {
        current->addArticle(product_list->find(gname).value(), (multiplyby!=0) ? multiplyby : 1);
        actualiseTable();
        showTotal();
    }
}

void MainWindow::up()
{
    if (!sold_details->selectedRanges().isEmpty()) {
        if (sold_details->selectedRanges().first().bottomRow() != 0) {
            int r = sold_details->selectedRanges().first().bottomRow();
            sold_details->clearSelection();
            sold_details->setRangeSelected(QTableWidgetSelectionRange(r-1, 0, r-1, sold_details->columnCount()-1), true);
        }
    }
}

void MainWindow::down()
{
    if (!sold_details->selectedRanges().isEmpty()) {
        if (sold_details->selectedRanges().first().bottomRow() != sold_details->rowCount()-1) {
            int r = sold_details->selectedRanges().first().bottomRow();
            sold_details->clearSelection();
            sold_details->setRangeSelected(QTableWidgetSelectionRange(r+1, 0, r+1, sold_details->columnCount()-1), true);
        }
    }
}
void MainWindow::paySlot()
{
    if (current != 0 && !current->isEmpty()) {
        if(payment > current->getTotal()) {
            endSell();
        } else if (multiplyby != 0) {
            payment += multiplyby;
            multiplyby = 0;
            if (payment == current->getTotal()) {
                endSell();
            } else if (payment > current->getTotal()) {
                showTotal();
                qDebug() << "à rendre";
            }
        } else if (payMode) {
            endSell();

        } else {
            payMode = true;
            showTotal();
        }
    }
}

void MainWindow::moneyTouch(QString data)
{
    if (payMode) {
        int mals = (multiplyby!=0) ? multiplyby : 1;
        payment += mals * data.toDouble();
        showTotal();
        if (payment == current->getTotal()) {
            endSell();
        }
    }
}

void MainWindow::rushTouch()
{
    if (rush->text() == tr("RUSH Mode") && seller != 0) {
        int reponse = QMessageBox::warning(this, tr("Zone protégée"), tr("Vous êtes sur le point d'entrer en <strong>mode Rush</strong>\nCe mode est destiné uniquement à la vente à la volée, lors des récréations par exemple, quand l'affluence est trop fort.\n\n<h3>Voulez-vous continuer ?</h3>"), QMessageBox::Yes | QMessageBox::Abort, QMessageBox::Abort);
        if (reponse == QMessageBox::Yes)
        {
            if (QInputDialog::getText(this, tr("Authentification"), tr("Merci d'entrer une nouvelle fois votre mot de passe\n(date de naissance, format JJMMAAAA)"), QLineEdit::Password, "", nullptr, Qt::Dialog, Qt::ImhDigitsOnly) == seller->getBirthday().remove("-"))
            {
                rushMode = true;
                pay->setEnabled(false);
                cancel->setEnabled(false);
                calc[11]->setText("-");
                totalmd->setEnabled(false);
                totald->setText("RUSH IN PROGRESS");
                totald->setStyleSheet("background-color: red; color: white;");
                rush->setText(tr("TERMINER"));

                QString location = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).at(1);
                location += "/log/Ventes_" + QDate::currentDate().toString("dd-MM-yyyy") + ".log";
                QFile file(location);

                file.open(QFile::WriteOnly | QFile::Append | QFile::Text);
                QTextStream stream(&file);
                stream.setCodec("UTF-8");
                stream << endl << "          ********************          "  << endl << "CODE RED ALERT : RUSH MODE ENGAGED AT " << QDateTime::currentDateTime().toString("HH:mm") << endl;
                file.close();
            } else {
                QMessageBox::critical(this, tr("Connexion refusée"), tr("<strong>Accès refusé.</strong>\n Le mot de passe entré est erroné"));
            }
        }
    } else if (rush->text() == tr("TERMINER")) {
        int reponse = QMessageBox::information(this, tr("Sortie de vente"), tr("Vous êtes sur le point de sortir du mode Rush.\nLes biens listés seront ajoutés à la comptabilité.\n\nVoulez-vous continuer ?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (reponse == QMessageBox::Yes)
        {
            double amount = current->getTotal();
            endSell();
            rush->setText(tr("RUSH Mode"));
            pay->setEnabled(true);
            cancel->setEnabled(true);
            totalmd->setEnabled(true);
            calc[11]->setText("C");
            rushMode = false;
            QString location = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).at(1);
            location += "/log/Ventes_" + QDate::currentDate().toString("dd-MM-yyyy") + ".log";

            QFile file(location);

            file.open(QFile::WriteOnly | QFile::Append | QFile::Text);
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream << QString::number(amount, 'f', 2) + QString::fromUtf8(" €") << tr(" réalisés pendant le Rush").toUtf8() << endl << "EXIT RUSH MODE AT " << QDateTime::currentDateTime().toString("HH:mm") << endl <<"          ********************          ";
            file.close();
            totald->setText(tr("RUSH TERMINÉ"));
            totald->setStyleSheet("background-color: white; color: black;");
        }
    }
}

void MainWindow::endSell()
{
    current->end(manager->getDB(), rushMode);
    seller->addSold(current->getTotal());
    psettings->setValue("sold", current->getNumber());

    delete current;
    current = new Vente(psettings->value("sold").toInt()+1);

    soldd->setText("Vente N° " + QString::number(current->getNumber()));
    countd->setValue(seller->getAmount());
    totald->setText(tr("VENTE TERMINÉE"));

    payment = 0;
    multiplyby = 0;
    payMode = false;

    actualiseTable();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (seller != 0) {
        if (current->isEmpty() && !rushMode) {
            sign_slot();

            event->accept();
        } else if (current->isEmpty() && rushMode) {
            endSell();
            rushTouch();
            sign_slot();

            event->accept();
        } else {
            int reponse = QMessageBox::warning(this, tr("Instance non terminée"), tr("La vente en cours n'est pas terminée.\nVoulez-vous poursuivre la fermeture ?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (reponse == QMessageBox::No) {
                event->ignore();
            } else {
                if (rushMode)
                    rushTouch();
                sign_slot();

                event->accept();
            }
        }
    }
}
