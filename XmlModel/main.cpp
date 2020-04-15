#include "XmlModelWidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XmlModelWidget w;
    w.show();
    return a.exec();
}
