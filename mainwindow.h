#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_burnOffButton_pressed();

    void on_burnOnButton_pressed();

    void startBurnTime();

    void on_settingButton_clicked();

private:
    Ui::MainWindow *ui;

signals:
    void showSettingDialog();
};

#endif // MAINWINDOW_H
