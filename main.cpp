#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Guimarães Tecnologia");
    QCoreApplication::setApplicationName("Invoice Manager");

    MainWindow w;
    w.show();
    return a.exec();
}
