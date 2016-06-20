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
