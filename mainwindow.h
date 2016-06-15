#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
