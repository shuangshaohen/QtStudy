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

private slots:
    void on_buttonAdd_clicked();

    void on_buttonConfirm_clicked();

    void on_buttonCancel_clicked();

    void on_buttonDel_clicked();

    void on_buttonSearch_clicked();

private:
    Ui::Widget *ui;

    QSqlTableModel * model;
};
#endif // WIDGET_H
