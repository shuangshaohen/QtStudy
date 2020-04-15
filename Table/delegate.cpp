#include "delegate.h"
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
//#include <QCheckBox>
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
    if ((index.isValid() && index.column() == EN_DATATABLE_COLUMN_ID)
            ||(index.isValid() && index.column() == EN_DATATABLE_COLUMN_DIR))
    {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        editor->installEventFilter(const_cast<Delegate *>(this));
        return editor;
    }
    else if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_VAL)
    {
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        editor->installEventFilter(const_cast<Delegate *>(this));
        return editor;
    }
//    else if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_CHECK)
//    {
//        QCheckBox *editor = new QCheckBox(parent);
//        editor->setText("检查");
//        editor->installEventFilter(const_cast<Delegate *>(this));
//        return editor;
//    }
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
        box->setRange(0,65535);
        box->setSingleStep(1);
        box->setValue(value);
    }
    else if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_VAL)
    {
        double value = index.model()->data(index, Qt::EditRole).toDouble();
        QDoubleSpinBox *box = static_cast<QDoubleSpinBox *>(editor);
        box->setValue(value);
        box->setSingleStep(0.01);
    }
//    else if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_CHECK)
//    {
//        bool value = index.model()->data(index, Qt::EditRole).toBool();
//        QCheckBox *box = static_cast<QCheckBox *>(editor);
//        box->setChecked(value);
//    }
    else if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_DIR)
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *box = static_cast<QSpinBox *>(editor);
        box->setRange(-9999,9999);
        box->setSingleStep(1);
        box->setValue(value);
//        QVariant point = index.model()->data(index,Qt::UserRole+1).toUInt();
//        if(point.isValid())
//        {
//            int * p = (int *)point.toUInt();
//            if(nullptr != p)
//            {
//                box->setValue(*p);
//            }
//        }
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
    else if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_VAL)
    {
        QDoubleSpinBox *box = static_cast<QDoubleSpinBox *>(editor);
        model->setData(index, box->value());
        QVariant point = model->data(index,Qt::UserRole+1).toUInt();
        if(point.isValid())
        {
            double * p = (double *)point.toUInt();
            if(nullptr != p)
                *p = box->value();
        }
    }
//    else if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_CHECK)
//    {
//        QCheckBox *box = static_cast<QCheckBox *>(editor);
//        model->setData(index, box->isChecked());
//        QVariant point = model->data(index,Qt::UserRole+1).toUInt();
//        if(point.isValid())
//        {
//            bool * p = (bool *)point.toUInt();
//            if(nullptr != p)
//                *p = box->isChecked();
//        }
//    }
    else if (index.isValid() && index.column() == EN_DATATABLE_COLUMN_DIR)
    {
        QSpinBox *box = static_cast<QSpinBox *>(editor);
        model->setData(index, box->value(),Qt::EditRole);
//        qDebug() <<"model setData box = " <<box->value();

        QVariant point = model->data(index,Qt::UserRole+1).toUInt();
        if(point.isValid())
        {
            int * p = (int *)point.toUInt();
            if(nullptr != p)
            {
//                qDebug() <<"set dir val = " <<box->value();
                *p = box->value();
            }
        }

//        if(box->value() > 0)
//            model->setData(index,QString("盈%1").arg(box->value()),Qt::DisplayRole);
//        else if(box->value()< 0)
//            model->setData(index,QString("亏%1").arg(-1*box->value()),Qt::DisplayRole);
//        else
//            model->setData(index,QString("平"),Qt::DisplayRole);

//        qDebug() << "DisplayRole value = " << model->data(index,Qt::DisplayRole).toString();
//        qDebug() << "EditRole value = " << model->data(index,Qt::EditRole).toString();
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
