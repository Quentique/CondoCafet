#ifndef NUMBERDELEGATE_H
#define NUMBERDELEGATE_H


#include <QStyledItemDelegate>

class NumberDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
    explicit NumberDelegate(QObject *parent = 0);
    virtual QString displayText(const QVariant &value, const QLocale &locale) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const Q_DECL_OVERRIDE;

       void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
       void setModelData(QWidget *editor, QAbstractItemModel *model,
                         const QModelIndex &index) const Q_DECL_OVERRIDE;

       void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

};
#endif // NUMBERDELEGATE_H
