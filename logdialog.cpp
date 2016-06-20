#include "logdialog.h"
#include "ui_logdialog.h"

LogDialog::LogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogDialog)
{
    ui->setupUi(this);
}

LogDialog::~LogDialog()
{
    delete ui;
}

void LogDialog::receiveShow()
{
    this->show();
}

void LogDialog::on_returnMainButton_clicked()
{
    emit showMainPage();
    this->hide();
}
