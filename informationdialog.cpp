#include "informationdialog.h"
#include "ui_informationdialog.h"

InformationDialog::InformationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InformationDialog)
{
    ui->setupUi(this);
}

InformationDialog::~InformationDialog()
{
    delete ui;
}

void InformationDialog::receiveShow()
{
    this->show();
}

void InformationDialog::on_returnMainButton_clicked()
{
    emit showMainPage();
    this->hide();
}

void InformationDialog::on_aboutPushButton_clicked()
{
    ui->InformationStackedWidget->setCurrentIndex(1);
}

void InformationDialog::on_infoPushButton_clicked()
{
    ui->InformationStackedWidget->setCurrentIndex(0);
}
