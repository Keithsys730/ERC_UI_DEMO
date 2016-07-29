#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QThread>
#include "settingdialog.h"
#include "statusdialog.h"
#include "logdialog.h"
#include "informationdialog.h"
#include "keyboard.h"
#include <QTranslator>
#include "spi.h"

int main(int argc, char *argv[])
{
    //QApplication::setSetuidAllowed(true);
    QApplication app(argc, argv);
    QTranslator translator;

    translator.load(":/erc_demo_cn_zh.qm");
    app.installTranslator(&translator);

    MainWindow mainPage;
    SettingDialog settingDialog;
    StatusDialog statusDialog;
    LogDialog logDialog;
    InformationDialog informationDialog;
    KeyBoard keyBoard;
    SPI spi;

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
    informationDialog.setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    mainPage.showFullScreen();

    QObject::connect(&mainPage, SIGNAL(showSettingDialog()),&settingDialog,SLOT(receiveShow()));
    QObject::connect(&settingDialog, SIGNAL(showMainPage()),&mainPage,SLOT(receiveShow()));
    QObject::connect(&mainPage, SIGNAL(showStatusDialog()),&statusDialog,SLOT(receiveShow()));
    QObject::connect(&statusDialog, SIGNAL(showMainPage()),&mainPage,SLOT(receiveShow()));
    QObject::connect(&mainPage, SIGNAL(showLogDialog()),&logDialog,SLOT(receiveShow()));
    QObject::connect(&logDialog, SIGNAL(showMainPage()),&mainPage,SLOT(receiveShow()));
    QObject::connect(&mainPage, SIGNAL(showInformationDialog()),&informationDialog,SLOT(receiveShow()));
    QObject::connect(&informationDialog, SIGNAL(showMainPage()),&mainPage,SLOT(receiveShow()));
    QObject::connect(&mainPage, SIGNAL(showKeyBoard()),&keyBoard,SLOT(receiveShow()));
    QObject::connect(&keyBoard, SIGNAL(sendInputValue(QString)),&mainPage,SLOT(receiveInputValue(QString)));

    for (qint16 temp=0;temp<7;++temp)
    {
        spi.writeBufferTest(temp);
        //spi.readByte(MCP_RXB0CTRL+6);
        spi.delay(50000);
    }

    spi.writeTxBtoCAN();

    spi.delay(50000);

    //spi.readSPIConfig();

    mainPage.show();

    return app.exec();
}
