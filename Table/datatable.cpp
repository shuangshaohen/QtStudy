#include "datatable.h"
#include <QDebug>
#include "DirItem.h"
#include "delegate.h"

DataTable::DataTable(QWidget *parent) : QTableWidget(parent)
{
    resize(800,600);

    m_datas.clear();
    for (int i = 0; i < 10; i++) {
        tdsData * p = new tdsData(i);
        m_datas.push_back(p);
    }

    update();

    this->setItemDelegateForColumn(EN_DATATABLE_COLUMN_ID,new Delegate(this));
    this->setItemDelegateForColumn(EN_DATATABLE_COLUMN_NAME,new Delegate(this));
    this->setItemDelegateForColumn(EN_DATATABLE_COLUMN_SEX,new Delegate(this));
}

void DataTable::printInfo()
{
    for (int i = 0; i < 10; i++) {
        qDebug() << m_datas.at(i)->info();
    }
}

void DataTable::valChanged(int val)
{
    QVariant intPoint = QObject::sender()->property("intPoint");
    if(intPoint.isValid())
    {
        int * p = (int *)intPoint.toUInt();
        *p = val;
    }
}

void DataTable::strChanged(const QString &text)
{
    QVariant strPoint = QObject::sender()->property("strPoint");
    if(strPoint.isValid())
    {
        QString * p = (QString *)strPoint.toUInt();
        *p = text;
    }
}

void DataTable::update()
{
    clearContents();
    setRowCount(m_datas.size());
    setColumnCount(6);
    QStringList headerList;
    headerList << "编号" << "姓名" << "系数" << "状态" << "盈亏" << "性别";
    setHorizontalHeaderLabels(headerList);

    for (int i = 0; i < 10; i++) {
        //ID
        //createSpinBox(i);
        QTableWidgetItem * pID = new QTableWidgetItem(QString::number(m_datas[i]->id));
        pID->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        pID->setData(Qt::UserRole+1,(unsigned int)&m_datas[i]->id);
        setItem(i,EN_DATATABLE_COLUMN_ID,pID);

        //name
        QTableWidgetItem * pName = new QTableWidgetItem(m_datas[i]->name);
        pName->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        pName->setData(Qt::UserRole+1,(unsigned int)&m_datas[i]->name);
        setItem(i,EN_DATATABLE_COLUMN_NAME,pName);

        //盈亏
        DirItem * pDir = new DirItem(&m_datas[i]->dir);
        setItem(i,EN_DATATABLE_COLUMN_DIR,pDir);

        //SEX
        //createComboBox(i);
        QTableWidgetItem * pSex = new QTableWidgetItem(m_datas[i]->sex);
        pSex->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        pSex->setData(Qt::UserRole+1,(unsigned int)&m_datas[i]->sex);
        setItem(i,EN_DATATABLE_COLUMN_SEX,pSex);
    }
}

void DataTable::createSpinBox(int row)
{
    QSpinBox * pSpinBox = new QSpinBox();
    pSpinBox->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    pSpinBox->setRange(0,65535);
    pSpinBox->setProperty("intPoint",(uint)&m_datas[row]->id);
    pSpinBox->setValue(m_datas[row]->id);
    pSpinBox->setSingleStep(2);
    connect(pSpinBox, SIGNAL(valueChanged(int)),this,SLOT(valChanged(int)));

    setCellWidget(row,EN_DATATABLE_COLUMN_ID,pSpinBox);
}

void DataTable::createComboBox(int row)
{
    QComboBox * box = new QComboBox();
    QStringList strList;
    strList << "男" << "女" << "未知";
    box->addItems(strList);
    box->setCurrentText(m_datas[row]->sex);
    box->setProperty("strPoint",(uint)&m_datas[row]->sex);
    connect(box, SIGNAL(currentIndexChanged(QString)),this,SLOT(strChanged(QString)));

    setCellWidget(row,EN_DATATABLE_COLUMN_SEX,box);
}
