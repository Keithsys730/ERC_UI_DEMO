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

void SettingDialog::on_returnMainButton_clicked()
{
    emit showMainPage();
    this->hide();
}