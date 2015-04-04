#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    setlocale(LC_NUMERIC,"C"); // Qt truncates atof(s)

    MainWindow w;
    w.show();

    return a.exec();
}
