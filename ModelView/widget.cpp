#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    createConnection();

    model = new QSqlTableModel(this);
    model->setTable("student");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Age"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Score"));

    ui->tableView->setModel(model);
    setWindowTitle("QSqlTableModel");

    queryAll();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::createConnection()
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../info.db");

    if (!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"
                        "This example needs SQLite support. Please read "
                        "the Qt SQL driver documentation for information how "
                        "to build it.\n\n"
                        "Click Cancel to exit."), QMessageBox::Cancel);
        return;
    }

//    QSqlQuery query;
//    query.exec("create table person (id int primary key, "
//               "firstname varchar(20), lastname varchar(20))");
//    query.exec("insert into person values(101, 'Danny', 'Young')");
//    query.exec("insert into person values(102, 'Christine', 'Holand')");
//    query.exec("insert into person values(103, 'Lars', 'Gordon')");
//    query.exec("insert into person values(104, 'Roberto', 'Robitaille')");
//    query.exec("insert into person values(105, 'Maria', 'Papadopoulos')");

//    query.exec("create table student (id int primary key, "
//               "name varchar(20), age int, score int)");
//    query.exec("insert into student values(0, 'Danny', 12, 45)");
//    query.exec("insert into student values(1, 'Christine', 13, 56)");
//    query.exec("insert into student values(2, 'Lars', 14, 53)");
//    query.exec("insert into student values(3, 'Roberto', 15, 67)");
//    query.exec("insert into student values(4, 'Maria', 16, 66)");

}

void Widget::queryAll()
{
    QSqlDatabase db = QSqlDatabase::database(); //建立数据库连接
    QSqlQuery query(db);

    query.exec("select * from student");
    QSqlRecord rec = query.record();
    qDebug() << QObject::tr("student表字段数：" ) << rec.count();

    while(query.next())
    {
        qDebug() << query.value("id").toInt()
                  << query.value("name").toString()
                     << query.value("age").toInt()
                        << query.value("score").toInt();
    }

}

void Widget::on_buttonAdd_clicked()
{
    //添加空记录
    QSqlRecord record = model->record();    //获取空记录
    //获取行号
    int row = model->rowCount();
    model->insertRecord(row,record);
}

void Widget::on_buttonConfirm_clicked()
{
    model->submitAll(); //提交动作
}

void Widget::on_buttonCancel_clicked()
{
    model->revertAll(); //取消所有动作
    model->submitAll(); //提交动作
}

void Widget::on_buttonDel_clicked()
{
    //获取选中的模型
    QItemSelectionModel * sModel = ui->tableView->selectionModel();
    //取出模型中的索引
    QModelIndexList list = sModel->selectedRows();
    //删除所有选中行
    for(int i = 0; i < list.size(); i++)
    {
        model->removeRow(list.at(i).row());
    }

}

void Widget::on_buttonSearch_clicked()
{
    QString name = ui->lineEdit->text();

    if("" == name)
        model->setFilter("");
    else
        model->setFilter(QString("name = '%1'").arg(name));

    model->select();
}
