#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "XmlItemModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class XmlModelWidget : public QWidget
{
    Q_OBJECT

public:
    XmlModelWidget(QWidget *parent = nullptr);
    ~XmlModelWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    XmlItemModel * model;
};
#endif // WIDGET_H
