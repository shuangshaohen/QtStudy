#include "delegate.h"
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QDebug>

Delegate::Delegate(QObject *parent)
    : QItemDelegate(parent)
{
    m_sexList.clear();
    m_sexList << "男" << "女" << "未知";
}

Delegate::~Delegate()
{

}
void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QItemDelegate::sizeHint(option, index);
}

QWidget *Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                                   const QModelIndex &index) const
{
    if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_ID)
    {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        editor->installEventFilter(const_cast<Delegate *>(this));
        return editor;
    }
    else if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_NAME)
    {
        QLineEdit *editor = new QLineEdit(parent);
        editor->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        editor->installEventFilter(const_cast<Delegate *>(this));
        return editor;
    }
    else if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_SEX)
    {
        QComboBox *editor = new QComboBox(parent);
        editor->setEditable(true);
        editor->installEventFilter(const_cast<Delegate *>(this));
        return editor;
    }
    else
    {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_ID)
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *box = static_cast<QSpinBox *>(editor);
        box->setValue(value);
        box->setRange(0,65535);
        box->setSingleStep(1);
    }
    else if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_NAME)
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QLineEdit *line = static_cast<QLineEdit *>(editor);
        line->setText(value);
    }
    else if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_SEX)
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *combox = static_cast<QComboBox *>(editor);
        combox->addItems(m_sexList);
        if(m_sexList.contains(value))
            combox->setCurrentText(value);
        else
            combox->setCurrentText(m_sexList.last());
    }
    else
    {
        QItemDelegate::setEditorData(editor, index);
    }
}

void Delegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                             const QModelIndex &index) const
{
    if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_ID)
    {
        QSpinBox *box = static_cast<QSpinBox *>(editor);
        model->setData(index, box->value());
        QVariant point = model->data(index,Qt::UserRole+1).toUInt();
        if(point.isValid())
        {
            int * p = (int *)point.toUInt();
            if(nullptr != p)
                *p = box->value();
        }
    }
    else if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_NAME)
    {
        QLineEdit *line = static_cast<QLineEdit *>(editor);
        //唯一性检查
        bool bSame = false;
        for(int i = 0; i < model->rowCount(); i++)
        {
            if(line->text() == model->data(index.siblingAtRow(i)).toString())
            {
                bSame = true;
                break;
            }
        }
        if(!bSame)
        {
            model->setData(index, line->text());
            QVariant point = model->data(index,Qt::UserRole+1).toUInt();
            if(point.isValid())
            {
                QString * p = (QString *)point.toUInt();
                if(nullptr != p)
                    *p = line->text();
            }
        }
    }
    else if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_SEX)
    {
        QComboBox *combox = static_cast<QComboBox *>(editor);
        //自检
        if(m_sexList.contains(combox->currentText()))
        {
            model->setData(index, combox->currentText());
            QVariant strPoint = model->data(index,Qt::UserRole+1).toUInt();
            if(strPoint.isValid())
            {
                QString * p = (QString *)strPoint.toUInt();
                if(nullptr != p)
                    *p = combox->currentText();
            }
        }
    }
    else
    {
        QItemDelegate::setModelData(editor, model, index);
    }
}
