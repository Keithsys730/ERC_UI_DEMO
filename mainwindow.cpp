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
    if(ui->burnOnButton->isChecked())
    {
        ui->burnOnButton->setChecked(false);
        //ui->burnTimeLabel->setText(QString::number(0));
    }
}

void MainWindow::on_burnOnButton_pressed()
{
    if(ui->burnOffButton->isChecked())
    {
        ui->burnOffButton->setChecked(false);
        ui->burnTimeLabel->setText(QString::number(0));
        startBurnTime();
    }
}

void MainWindow::startBurnTime()
{
    int burnTime=0;
    for(int temp;temp<100;temp++)
    {
        burnTime+=1;
        ui->burnTimeLabel->setText(QString::number(burnTime));
    }
}
