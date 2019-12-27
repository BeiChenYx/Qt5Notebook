#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QItemDelegate>

class DateDelegate: public QItemDelegate
{
    Q_OBJECT

public:
    DateDelegate(QObject *parent = nullptr);
    ~DateDelegate();
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem & option, const QModelIndex &index) const;
};

#endif // WIDGET_H
