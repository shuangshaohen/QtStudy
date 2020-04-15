#include "widget.h"
#include "ui_widget.h"
#include <QVariant>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    resize(1000,800);

    m_table = new DataTable(this);

    m_buttonPrint = new QPushButton(this);
    m_buttonPrint->setText("打印");
    m_buttonPrint->move(900,300);

    connect(m_buttonPrint,SIGNAL(clicked()),m_table,SLOT(printInfo()));




}

Widget::~Widget()
{
    delete ui;
}







