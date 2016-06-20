#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>

namespace Ui {
class LogDialog;
}

class LogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogDialog(QWidget *parent = 0);
    ~LogDialog();

private slots:
    void receiveShow();
    void on_returnMainButton_clicked();

private:
    Ui::LogDialog *ui;

signals:
    void showMainPage();
};

#endif // LOGDIALOG_H
