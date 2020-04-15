#include "datatable.h"
#include <QDebug>
#include "DirItem.h"
#include "delegate.h"
#include <QCheckBox>

DataTable::DataTable(QWidget *parent) : QTableWidget(parent)
{
    resize(800,600);

    m_datas.clear();
    for (int i = 0; i < 10; i++) {
        tdsData * p = new tdsData(i);
        m_datas.push_back(p);
    }

    update();

    setItemDelegate(new Delegate(this));
//    this->setItemDelegateForColumn(EN_DATATABLE_COLUMN_ID,new Delegate(this));
//    this->setItemDelegateForColumn(EN_DATATABLE_COLUMN_NAME,new Delegate(this));
//    this->setItemDelegateForColumn(EN_DATATABLE_COLUMN_SEX,new Delegate(this));
}

void DataTable::printInfo()
{
    for (int i = 0; i < 10; i++) {
        qDebug() << m_datas.at(i)->info();
    }
}

void DataTable::debugInfo()
{
    //修改某个item
    item(2,4)->setData(Qt::EditRole,555);
    QVariant point = item(2,4)->data(Qt::UserRole+1).toUInt();
    if(point.isValid())
    {
        int * p = (int *)point.toUInt();
        if(nullptr != p)
        {
            *p = 555;
        }
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

void DataTable::checkBoxChanged(int state)
{
    QVariant point = QObject::sender()->property("boolPoint");
    if(point.isValid())
    {
        bool * p = (bool *)point.toUInt();
        if(Qt::Unchecked == state)
            *p = false;
        else
            *p = true;
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
        pID->setData(Qt::UserRole+1,(unsigned int)&m_datas[i]->id);
        setItem(i,EN_DATATABLE_COLUMN_ID,pID);

        //name
        QTableWidgetItem * pName = new QTableWidgetItem(m_datas[i]->name);
        pName->setData(Qt::UserRole+1,(unsigned int)&m_datas[i]->name);
        setItem(i,EN_DATATABLE_COLUMN_NAME,pName);

        //name
        QTableWidgetItem * pCoe = new QTableWidgetItem(QString("%1").arg(m_datas[i]->val));
        pCoe->setData(Qt::UserRole+1,(unsigned int)&m_datas[i]->val);
        setItem(i,EN_DATATABLE_COLUMN_VAL,pCoe);

        //check
//        QTableWidgetItem * pCheck = new QTableWidgetItem();
//        pCheck->setData(Qt::UserRole+1,(unsigned int)&m_datas[i]->check);
//        setItem(i,EN_DATATABLE_COLUMN_CHECK,pCheck);
        QCheckBox *pCheck = new QCheckBox("检查",this);
        pCheck->setChecked(m_datas[i]->check);
        pCheck->setProperty("boolPoint",(uint)&m_datas[i]->check);
        setCellWidget(i,EN_DATATABLE_COLUMN_CHECK,pCheck);
        connect(pCheck,SIGNAL(stateChanged(int)),this,SLOT(checkBoxChanged(int)));

        //盈亏
//        DirItem * pDir = new DirItem(&m_datas[i]->dir);
//        setItem(i,EN_DATATABLE_COLUMN_DIR,pDir);
        DirItem * pDir = new DirItem(QString::number(m_datas[i]->dir));
//        if(m_datas[i]->dir > 0)
//            pDir->setData(Qt::DisplayRole,QString("盈%1").arg(m_datas[i]->dir));
//        else if(m_datas[i]->dir < 0)
//            pDir->setData(Qt::DisplayRole,QString("亏%1").arg(-1*m_datas[i]->dir));
//        else
//            pDir->setData(Qt::DisplayRole,QString("平"));
        pDir->setData(Qt::UserRole+1,(unsigned int)&m_datas[i]->dir);
        setItem(i,EN_DATATABLE_COLUMN_DIR,pDir);

        //SEX
        //createComboBox(i);
        QTableWidgetItem * pSex = new QTableWidgetItem(m_datas[i]->sex);
        pSex->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        pSex->setData(Qt::UserRole+1,(unsigned int)&m_datas[i]->sex);
        setItem(i,EN_DATATABLE_COLUMN_SEX,pSex);
    }

    for (int row= 0; row < rowCount(); row++) {
        for (int column = 0; column < columnCount(); column++) {
            if(nullptr != item(row,column))
                item(row,column)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
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
