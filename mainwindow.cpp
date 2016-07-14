#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initialButtonBackgroundColor();
    burnTime=0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(startBurnTime()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialButtonBackgroundColor()
{
    ui->burnOffButton->setStyleSheet("background-color: #FFA6B5");
    ui->burnOnButton->setStyleSheet("background-color: #1DFF33");
}

void MainWindow::on_burnOffButton_pressed()
{
    //ui->burnTimeLabel->setText(QString::number(0));
    if(ui->burnOffButton->isChecked())
    {
        ui->burnOffButton->setStyleSheet("background-color: #FFA6B5");
        ui->burnOnButton->setStyleSheet("background-color: #1DFF33");
    }
    else
    {
        timer->stop();
        burnTime=0;
        ui->burnOffButton->setStyleSheet("background-color: #F95C77");
        ui->burnOnButton->setStyleSheet("background-color: #1DFF33");
    }
}

void MainWindow::on_burnOnButton_pressed()
{
    if(!ui->burnOnButton->isChecked())
    {
        ui->burnTimeLabel->setText("000");
        timer->start(1000);
        ui->burnOnButton->setStyleSheet("background-color: #1DFF33");
        ui->burnOffButton->setStyleSheet("background-color: #F95C77");
    }
    else
    {
        ui->burnOnButton->setStyleSheet("background-color: #B5FFBC");
        ui->burnOffButton->setStyleSheet("background-color: #F95C77");
    }
}

void MainWindow::startBurnTime()
{
    burnTime += 1;
    QString value;
    ui->burnTimeLabel->setText(value.sprintf("%03d", burnTime));
}

void MainWindow::on_settingButton_clicked()
{
    emit showSettingDialog();
    this->hide();
}

void MainWindow::on_statusButton_clicked()
{
    emit showStatusDialog();
    this->hide();
}

void MainWindow::on_logButton_clicked()
{
    emit showLogDialog();
    this->hide();
}

void MainWindow::on_informationButton_clicked()
{
    emit showInformationDialog();
    this->hide();
}

void MainWindow::receiveShow()
{
    this->show();
}

void MainWindow::receiveInputValue(QString setValue)
{
    ui->burnSetButton->setText(validInputValue(setValue));
}

/*
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    ui->burnLabel->setText(QString::number(this->x()));
    if((this->pos().x()-startPressX) > 10)
    {
        emit showInformationDialog();
        this->hide();
    }
    emit showLogDialog();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    startPressX = this->pos().x();
    startPressY = this->pos().y();
    ui->burnTimeLabel->setText(QString::number(startPressX));
}
*/

void MainWindow::on_websiteButton_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.meanwell.com"));
}

void MainWindow::on_burnSetButton_clicked()
{
    emit showKeyBoard();
}

QString MainWindow::validInputValue(QString value)
{
    if(value == "" || value.toDouble()<0.5 || value.toDouble()>30.0)
    {
        //showErrorMessage...
        return ui->burnSetButton->text();
    }
    else if(value.toDouble()<10.0)
    {
        return value.sprintf("0%02.1f", value.toDouble());
    }
    else
        return value.sprintf("%02.1f", value.toDouble());
}
