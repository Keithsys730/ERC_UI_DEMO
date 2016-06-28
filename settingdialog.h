#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();

private slots:
    void receiveShow();
    void on_returnMainButton_2_clicked();
    void on_cpRadioButton_clicked();
    void on_ccRadioButton_clicked();

private:
    Ui::SettingDialog *ui;

signals:
    void showMainPage();
};

#endif // SETTINGDIALOG_H
