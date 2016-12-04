#include "combodelegate.h"
#include <QDebug>

ComboDelegate::ComboDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    combo = new QComboBox();
    combo->setFrame(false);
    combo->setStyleSheet("QComboBox { border: none; opacity: 0; }");
    QMap<QString, QString> map = getColours();
    for (QMap<QString, QString>::iterator it = map.begin() ; it!=map.end() ; it++)
    {
        combo->addItem(it.value(), it.key());
    }
}

QWidget* ComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    combo->setParent(parent);
    return combo;
}

void ComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.data(Qt::EditRole).toString();
    QComboBox *combo = static_cast<QComboBox*>(editor);

    combo->setCurrentIndex(combo->findData(value));
}

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *combo = static_cast<QComboBox*>(editor);
    model->setData(index, combo->currentData(), Qt::EditRole);
}
void ComboDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

QString ComboDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    return getColours().value(value.toString());
}

QMap<QString, QString> ComboDelegate::getColours()
{
    QMap<QString, QString> map;
    map.insert("#RRRRRR", "Red");
    map.insert("#FFFFFF", "Aucune");
    map.insert("#FFF000", "Orange");

    return map;
}
