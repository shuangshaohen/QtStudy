#ifndef DATATABLE_H
#define DATATABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QSpinBox>
#include <QComboBox>

struct tdsData
{
    int     id;
    QString name;
    float   val;
    bool    check;
    int     dir;
    QString sex;

    tdsData(int num)
    {
        id = num;
        name = "Data" + QString::number(num);
        val = (float)num/10;
        check = false;
        dir = 0;
        sex = "男";
    }

    QString info()
    {
        QString info = QString("id = %1, name = %2, val = %3, check = %4 , dir = %5, sex = %6")
                .arg(id).arg(name).arg(val).arg(check).arg(dir).arg(sex.toUtf8().data());
        return info;
    }
};

class DataTable : public QTableWidget
{
    Q_OBJECT

public:
    explicit DataTable(QWidget *parent = nullptr);

signals:

public slots:
    void printInfo();

protected slots:
    void valChanged(int val);
    void strChanged(const QString &text);
private:
    QVector<tdsData *>  m_datas;

    void    update();

    void    createSpinBox(int row);
    void    createComboBox(int row);
};

#endif // DATATABLE_H
