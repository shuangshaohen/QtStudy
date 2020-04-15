#ifndef DIRITEM_H
#define DIRITEM_H

#include <QTableWidgetItem>

class DirItem : public QTableWidgetItem
{
public:
    explicit DirItem(const QString &text,int type = Type);

    QVariant data(int role) const;
signals:

private:
    int * dir;

};

#endif // DIRITEM_H
