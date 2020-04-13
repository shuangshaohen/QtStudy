#include "widget.h"
#include "ui_widget.h"
#include <QKeyEvent>
#include <QClipboard>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    rightMenu = new QMenu(ui->tableView);
    copyAction = new QAction("复制",this);
    pasteAction = new QAction("粘贴",this);
    cutAction = new QAction("剪切行",this);
    insertAction = new QAction("插入行",this);
    deleteAction = new QAction("删除行",this);

    rightMenu->addAction(cutAction);
    rightMenu->addAction(copyAction);
    rightMenu->addAction(pasteAction);
    rightMenu->addAction(insertAction);
    rightMenu->addAction(deleteAction);

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

    connect(cutAction,SIGNAL(triggered()),this,SLOT(TableCut()));
    connect(copyAction,SIGNAL(triggered()),this,SLOT(TableCopy()));
    connect(pasteAction,SIGNAL(triggered()),this,SLOT(TablePaste()));
    connect(insertAction,SIGNAL(triggered()),this,SLOT(TableInsert()));
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(TableDelete()));

    cutList.clear();
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

bool Widget::event(QEvent *event)
{
    QWidget * fWidget = focusWidget();
    if(ui->tableView != qobject_cast<QTableView *>(fWidget))
        return QWidget::event(event);

    if (event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->matches(QKeySequence::Paste)){
            TablePaste();
            event->accept();
            return true;
        }
        else if(keyEvent->matches(QKeySequence::Copy)){
            TableCopy();
            event->accept();
            return true;
        }
        else if(keyEvent->matches(QKeySequence::Save)){
            TableSubmit();
            event->accept();
            return true;
        }
        else if(keyEvent->matches(QKeySequence::Undo)){
            model->revertAll(); //取消所有动作
            model->submitAll(); //提交动作
            event->accept();
            return true;
        }
    }

    return QWidget::event(event);
}

void Widget::on_buttonAdd_clicked()
{
    TableNew();
}

void Widget::on_buttonConfirm_clicked()
{
    TableSubmit();
}

void Widget::on_buttonCancel_clicked()
{
    TableRevert();
}

void Widget::on_buttonDel_clicked()
{
    TableDelete();
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

void Widget::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    //获取选中的模型
    QItemSelectionModel * sModel = ui->tableView->selectionModel();
    //取出模型中的索引
    QModelIndexList list = sModel->selectedRows();

    rightMenu->exec(cursor().pos());
}

void Widget::TableNew()
{
    //添加空记录
    QSqlRecord record = model->record();    //获取空记录

    //获取选中的模型
    QItemSelectionModel * sModel = ui->tableView->selectionModel();
    //取出模型中的索引
    QModelIndexList list = sModel->selectedRows();

    //获取行号
    int row = model->rowCount();
    if(list.isEmpty() == false)
    {
        int maxRow = 0;
        for(int i = 0; i < list.size(); i++)
        {
            if(maxRow < list.at(i).row())
                maxRow = list.at(i).row();
        }
        row = maxRow;
    }
    model->insertRecord(row,record);
}

void Widget::TableCut()
{
    cutList.clear();

    //获取选中的模型
    QItemSelectionModel * sModel = ui->tableView->selectionModel();
    //取出模型中的索引
    cutList = sModel->selectedRows();
}

void Widget::TableCopy()
{
    QString copied_text;
    QModelIndexList current_selected_indexs = ui->tableView->selectionModel()->selectedIndexes();
    if(current_selected_indexs.isEmpty())
        return;

    int current_row = current_selected_indexs.at(0).row();
    for(int i = 0; i < current_selected_indexs.count(); i++){
        if(current_row != current_selected_indexs.at(i).row()){
            current_row = current_selected_indexs.at(i).row();
            copied_text.append("\n");
            copied_text.append(current_selected_indexs.at(i).data().toString());
            continue;
        }
        if(0 != i){
            copied_text.append("\t");
        }
        copied_text.append(current_selected_indexs.at(i).data().toString());
    }
    copied_text.append("\n");
    QApplication::clipboard()->setText(copied_text);
}

void Widget::TablePaste()
{
    QString text_to_past = QApplication::clipboard()->text();
    QStringList table_row_data_list = text_to_past.split("\n", QString::SkipEmptyParts);

    QModelIndex current_index = ui->tableView->currentIndex();
    for (int i = 0; i < model->rowCount() - current_index.row() && i < table_row_data_list.length(); ++i){
        QStringList row_data_list = table_row_data_list.at(i).split("\t");
        for(int k = 0; k < model->columnCount() - current_index.column() && k<row_data_list.length(); k++){
            QModelIndex temp_index = current_index.sibling(i+current_index.row(), k+current_index.column());
            model->setData(temp_index,row_data_list.at(k));
        }
    }
}

void Widget::TableInsert()
{
    //获取选中的模型
    QItemSelectionModel * sModel = ui->tableView->selectionModel();
    //取出模型中的索引
    QModelIndexList list = sModel->selectedRows();

    //获取行号
    int row = ui->tableView->currentIndex().row();

    if(cutList.isEmpty() == false)
    {
        for(int i = 0; i < cutList.size(); i++)
        {
            if(row == cutList.at(i).row())
                continue;
            else if(row > cutList.at(i).row())
            {
                QSqlRecord record = model->record(cutList.at(i).row());
                model->insertRecord(row,record);
                model->removeRow(cutList.at(i).row());
            }
            if(row < cutList.at(i).row())
            {
                QSqlRecord record = model->record(cutList.at(i).row());
                model->removeRow(cutList.at(i).row());
                model->insertRecord(row,record);
            }

        }

        cutList.clear();
    }
}

void Widget::TableDelete()
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

void Widget::TableSubmit()
{
    //    for (int i = 0; i < model->rowCount(); i++) {
    //        model->setData(model->index(i,0),i);
    //    }


    if(!model->submitAll())
    {
        QMessageBox::warning(this,"保存失败",model->lastError().text());
    }
}

void Widget::TableRevert()
{
    model->revertAll();
    TableSubmit();
}
