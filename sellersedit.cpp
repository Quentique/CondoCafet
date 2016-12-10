#include "sellersedit.h"
#include "mainedit.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QRegExp>
#include <QMessageBox>

SellersEdit::SellersEdit(QSqlDatabase *db) : MainEdit(db, "sellers")
{
    model->setHeaderData(1, Qt::Horizontal, tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, tr("Classe"));

    view->hideColumn(0);
    view->hideColumn(3);
    view->setColumnWidth(1, 200);

    setWindowTitle(tr("Modification des vendeurs"));
    connect(add, SIGNAL(clicked(bool)), this, SLOT(addRow()));
    connect(static_cast<QSqlTableModel*>(model->sourceModel()), SIGNAL(beforeUpdate(int,QSqlRecord&)), this, SLOT(check(int,QSqlRecord&)));
}

void SellersEdit::addRow()
{
    QSqlQuery query(static_cast<QSqlTableModel*>(model->sourceModel())->database());
    query.prepare("INSERT INTO sellers (name, class, amount) VALUES (:name, :class, 0)");
    query.bindValue(":name", tr("'Prénom NOM'"));
    query.bindValue(":class", tr("Classe"));
    query.exec();
    qDebug() << query.lastError().text();
    static_cast<QSqlTableModel*>(model->sourceModel())->select();
}

void SellersEdit::check(int, QSqlRecord &record)
{
    QRegExp exp("^[1T]+[A-Z]{1,4}[1-9]+$|^2[A-Z]$|Classe");
    if (!record.value("class").toString().contains(exp))
    {
        record.setValue("class", tr("Classe"));
        QMessageBox::warning(this, tr("Erreur de parsage"), tr("Le texte entré est invalide.\nLa classe doit être rentrée sous une forme telle que : 2A, 1S2, TSMTG3, etc..."));
    }
}
