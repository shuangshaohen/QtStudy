#include "DirItem.h"
#include <QDebug>

DirItem::DirItem(const QString &text, int type) : QTableWidgetItem(text,type)
{

}

QVariant DirItem::data(int role) const
{
    if (role == Qt::TextAlignmentRole)
        return (int)Qt::AlignCenter;
    if(role == Qt::DisplayRole){
        int dir = QTableWidgetItem::data(Qt::EditRole).toInt();
        if(dir > 0)
            return QString("盈%1").arg(dir);
        else if(dir < 0)
            return QString("亏%1").arg(0-dir);
        else
            return QObject::tr("平");
    }
    return QTableWidgetItem::data(role);
}
