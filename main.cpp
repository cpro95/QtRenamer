#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationDomain("kmk.com");
    QCoreApplication::setOrganizationName("kmk");
    QCoreApplication::setApplicationName("QtRenamer");
    QCoreApplication::setApplicationVersion("1.1.0");

    MainWindow w;
    w.show();

    return a.exec();
}
