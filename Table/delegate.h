#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>
#include <QItemDelegate>
#include <QModelIndex>
#include <QPainter>
#include <QWidget>

enum _EN_DATATABLE_COLUMN_
{
    EN_DATATABLE_COLUMN_ID = 0,
    EN_DATATABLE_COLUMN_NAME,
    EN_DATATABLE_COLUMN_VAL,
    EN_DATATABLE_COLUMN_CHECK,
    EN_DATATABLE_COLUMN_DIR,
    EN_DATATABLE_COLUMN_SEX,
};

class Delegate : public QItemDelegate
{
    Q_OBJECT

public:
    Delegate(QObject *parent = nullptr);
    ~Delegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                    const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const;

private:
    QStringList m_sexList;
};


#endif // DELEGATE_H
