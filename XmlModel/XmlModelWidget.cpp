#include "XmlModelWidget.h"
#include "ui_XmlModelWidget.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

XmlModelWidget::XmlModelWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    model = new XmlItemModel(nullptr);
}

XmlModelWidget::~XmlModelWidget()
{
    delete ui;
}


void XmlModelWidget::on_pushButton_clicked()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open xml File"),
                                         QDir::currentPath(),
                                         tr("XML Files (*.xml *.*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("XML Information"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument domDocument;

    if (!domDocument.setContent(&file, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        return;
    }

    QDomElement root = domDocument.documentElement();
    model->SetDomDocument(root);
    ui->treeView->setModel(model);
}
