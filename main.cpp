#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    a.setOverrideCursor(QCursor(Qt::BlankCursor));
    w.setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    //w.showFullScreen();
    w.show();

    return a.exec();
}
