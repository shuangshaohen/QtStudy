#ifndef DIRITEM_H
#define DIRITEM_H

#include <QTableWidgetItem>

class DirItem : public QTableWidgetItem
{
public:
    explicit DirItem(int * dir, int type = Type);

    QVariant data(int role) const;
    void setData(int role, const QVariant &value);
signals:

private:
    int * dir;

};

#endif // DIRITEM_H
