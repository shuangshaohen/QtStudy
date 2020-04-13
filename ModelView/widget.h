#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QMenu>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void createConnection();

    void queryAll();

protected:
    bool event(QEvent *event);

private slots:
    void on_buttonAdd_clicked();

    void on_buttonConfirm_clicked();

    void on_buttonCancel_clicked();

    void on_buttonDel_clicked();

    void on_buttonSearch_clicked();

    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void TableNew();
    void TableCut();
    void TableCopy();
    void TablePaste();
    void TableInsert();
    void TableDelete();

    void TableSubmit();
    void TableRevert();
private:
    Ui::Widget *ui;
    QMenu   *rightMenu;     //右键菜单
    QAction *cutAction;     //剪切
    QAction *copyAction;    //复制
    QAction *pasteAction;   //粘贴
    QAction *insertAction;  //插入
    QAction *deleteAction;  //删除

    QModelIndexList cutList;

    QSqlTableModel * model;
};
#endif // WIDGET_H
