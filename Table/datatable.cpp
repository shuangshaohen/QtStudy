#include "datatable.h"
#include <QDebug>

DataTable::DataTable(QWidget *parent) : QTableWidget(parent)
{
    resize(800,600);

    m_datas.clear();
    for (int i = 0; i < 10; i++) {
        tdsData * p = new tdsData(i);
        m_datas.push_back(p);
    }

    update();
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

void DataTable::update()
{
    clearContents();
    setRowCount(m_datas.size());
    setColumnCount(4);

    for (int i = 0; i < 10; i++) {
        //ID
        createSpinBox(i);
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
