#include "colordelegate.h"
#include "color_wheel.hpp"

ColorDelegate::ColorDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}
QWidget *ColorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    color_widgets::ColorWheel *edit = new color_widgets::ColorWheel(parent);
    return edit;
}

void ColorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.data().toString();

    color_widgets::ColorWheel *editt = static_cast<color_widgets::ColorWheel*>(editor);

    editt->setColor(QColor(value));
}

void ColorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    color_widgets::ColorWheel *spinBox = static_cast<color_widgets::ColorWheel*>(editor);
    QString value = spinBox->color().toRgb().name();

    model->setData(index, value, Qt::EditRole);
}
void ColorDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}
