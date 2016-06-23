#ifndef INFORMATIONDIALOG_H
#define INFORMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class InformationDialog;
}

class InformationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InformationDialog(QWidget *parent = 0);
    ~InformationDialog();

private slots:
    void receiveShow();
    void on_returnMainButton_clicked();
    void on_aboutPushButton_clicked();
    void on_infoPushButton_clicked();

private:
    Ui::InformationDialog *ui;

signals:
    void showMainPage();
};

#endif // INFORMATIONDIALOG_H
