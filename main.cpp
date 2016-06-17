#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QThread>
#include "settingdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainPage;
    SettingDialog settingDialog;

    /*
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/buttonphotos/Meanwell_2.png"));
    splash->show();

    Qt::Alignment bottomCenter = Qt::AlignBottom | Qt::AlignCenter;
    splash->showMessage(QObject::tr("Starting the ERC program..."),bottomCenter, Qt::white);
    //QThread::msleep(2000);
    //splash->hide();
    */

    app.setOverrideCursor(QCursor(Qt::BlankCursor));
    mainPage.setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    settingDialog.setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    //mainPage.showFullScreen();

    mainPage.show();

    QObject::connect(&mainPage, SIGNAL(showSettingDialog()),&settingDialog,SLOT(receiveShow()));

    return app.exec();
}
