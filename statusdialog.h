#ifndef STATUSDIALOG_H
#define STATUSDIALOG_H

#include <QDialog>

namespace Ui {
class StatusDialog;
}

class StatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatusDialog(QWidget *parent = 0);
    ~StatusDialog();

private slots:
    void receiveShow();
    void on_returnMainButton_clicked();

private:
    Ui::StatusDialog *ui;

signals:
    void showMainPage();
};

#endif // STATUSDIALOG_H
