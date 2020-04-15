#ifndef DATATABLE_H
#define DATATABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QSpinBox>

struct tdsData
{
    int     id;
    QString name;
    float   val;
    bool    check;

    tdsData(int num)
    {
        id = num;
        name = "Data" + QString::number(num);
        val = (float)num/10;
        check = false;
    }

    QString info()
    {
        QString info = QString("id = %1, name = %2, val = %3, check = %4 ").arg(id).arg(name).arg(val).arg(check);
        return info;
    }
};

class DataTable : public QTableWidget
{
    Q_OBJECT

    enum _EN_DATATABLE_COLUMN_
    {
        EN_DATATABLE_COLUMN_ID = 0,
        EN_DATATABLE_COLUMN_NAME,
        EN_DATATABLE_COLUMN_VAL,
        EN_DATATABLE_COLUMN_CHECK,
    };

public:
    explicit DataTable(QWidget *parent = nullptr);

signals:

public slots:
    void printInfo();

protected slots:
    void valChanged(int val);
private:
    QVector<tdsData *>  m_datas;

    void    update();

    void    createSpinBox(int row);
};

#endif // DATATABLE_H
