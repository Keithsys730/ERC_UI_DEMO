#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QElapsedTimer>

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
    void on_statusButton_clicked();
    void on_logButton_clicked();
    void on_informationButton_clicked();
    void receiveShow();
    void on_websiteButton_clicked();
    void on_burnSetButton_clicked();
    void receiveInputValue(QString setValue);

private:
    Ui::MainWindow *ui;
    void initialButtonBackgroundColor();
    QString validInputValue(QString value);
//    QElapsedTimer burnTimer;
//    int burnTime;
//    bool burnTimeStart;
//    int startPressX, startPressY;

//    virtual void mouseMoveEvent(QMouseEvent *event);
//    virtual void mousePressEvent(QMouseEvent *event);

signals:
    void showSettingDialog();
    void showStatusDialog();
    void showLogDialog();
    void showInformationDialog();
    void showKeyBoard(QString);
};

#endif // MAINWINDOW_H
