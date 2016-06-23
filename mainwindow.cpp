#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_burnOffButton_pressed()
{
    //ui->burnTimeLabel->setText(QString::number(0));
}

void MainWindow::on_burnOnButton_pressed()
{
    if(!ui->burnOnButton->isChecked())
    {
        //burnTime = 0;
        /*burnTimeStart = true;
        burnTimer.start();
        while(burnTimeStart)
        {
            //ui->burnTimeLcdNumber->value(burnTimeMinutes.elapsed());
            if(burnTimer.elapsed()>1000)
            {
                burnTimer.restart();
                burnTime += 1;
                ui->burnTimeLabel->setText(QString::number(burnTime));
            }
        }
        */
    }

}

void MainWindow::startBurnTime()
{
    int burnTime=0;
    for(int temp=0;temp<100;temp++)
    {
        burnTime+=1;
        ui->burnTimeLabel->setText(QString::number(burnTime));
    }
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
