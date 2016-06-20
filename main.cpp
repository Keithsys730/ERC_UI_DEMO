#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QThread>
#include "settingdialog.h"
#include "statusdialog.h"
#include "logdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainPage;
    SettingDialog settingDialog;
    StatusDialog statusDialog;
    LogDialog logDialog;

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
    statusDialog.setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    logDialog.setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    //mainPage.showFullScreen();

    mainPage.show();

    QObject::connect(&mainPage, SIGNAL(showSettingDialog()),&settingDialog,SLOT(receiveShow()));
    QObject::connect(&settingDialog, SIGNAL(showMainPage()),&mainPage,SLOT(receiveShow()));
    QObject::connect(&mainPage, SIGNAL(showStatusDialog()),&statusDialog,SLOT(receiveShow()));
    QObject::connect(&statusDialog, SIGNAL(showMainPage()),&mainPage,SLOT(receiveShow()));
    QObject::connect(&mainPage, SIGNAL(showLogDialog()),&logDialog,SLOT(receiveShow()));
    QObject::connect(&logDialog, SIGNAL(showMainPage()),&mainPage,SLOT(receiveShow()));

    return app.exec();
}
