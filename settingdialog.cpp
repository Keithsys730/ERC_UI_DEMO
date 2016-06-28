#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::receiveShow()
{
    this->show();
}

void SettingDialog::on_returnMainButton_2_clicked()
{
    emit showMainPage();
    this->hide();
}

void SettingDialog::on_cpRadioButton_clicked()
{
    if(ui->cpRadioButton->isChecked())
    {
        ui->setCurrentLabel->setEnabled(false);
        ui->setCurrentLineEdit->setEnabled(false);

        ui->setPowerLabel->setEnabled(true);
        ui->setPowerLineEdit->setEnabled(true);
    }
}

void SettingDialog::on_ccRadioButton_clicked()
{
    if(ui->ccRadioButton->isChecked())
    {
        ui->setCurrentLabel->setEnabled(true);
        ui->setCurrentLineEdit->setEnabled(true);

        ui->setPowerLabel->setEnabled(false);
        ui->setPowerLineEdit->setEnabled(false);
    }
}
