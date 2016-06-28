#include "statusdialog.h"
#include "ui_statusdialog.h"

StatusDialog::StatusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatusDialog)
{
    ui->setupUi(this);
}

StatusDialog::~StatusDialog()
{
    delete ui;
}

void StatusDialog::receiveShow()
{
    this->show();
}

void StatusDialog::on_returnMainButton_clicked()
{
    emit showMainPage();
    this->hide();
}

void StatusDialog::on_nextPushButton_clicked()
{
    ui->statusStackedWidget->setCurrentIndex(1);
}

void StatusDialog::on_previousPushButton_2_clicked()
{
    ui->statusStackedWidget->setCurrentIndex(0);
}

void StatusDialog::on_nextPushButton_2_clicked()
{
    ui->statusStackedWidget->setCurrentIndex(2);
}

void StatusDialog::on_previousPushButton_clicked()
{
    ui->statusStackedWidget->setCurrentIndex(1);
}
