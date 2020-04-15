#include "DirItem.h"
#include <QDebug>
#define DIR_J -1
#define DIR_D 1

DirItem::DirItem(int *dir, int type) : QTableWidgetItem(type)
{
    this->dir = dir;
}

QVariant DirItem::data(int role) const
{
    if (role == Qt::TextAlignmentRole)
        return (int)Qt::AlignCenter;
    if(role == Qt::DisplayRole){
        if(*dir == DIR_J)
            return QObject::tr("借");
        else if(*dir == DIR_D)
            return QObject::tr("贷");
        else
            return QObject::tr("平");
    }
    if(role == Qt::EditRole)
        return *dir;
    return QTableWidgetItem::data(role);
}

void DirItem::setData(int role, const QVariant &value)
{
    if(role == Qt::EditRole)
    {
        qDebug() << "DirItem::setData called";
        *dir = value.toInt();
    }
    QTableWidgetItem::setData(role, value);
}
