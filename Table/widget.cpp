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

    QPushButton * button = new QPushButton(this);
    button->setText("调试");
    button->move(900,500);

    connect(m_buttonPrint,SIGNAL(clicked()),m_table,SLOT(printInfo()));
    connect(button,SIGNAL(clicked()),m_table,SLOT(debugInfo()));
}

Widget::~Widget()
{
    delete ui;
}







