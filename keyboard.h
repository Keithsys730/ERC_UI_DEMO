#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QDialog>

namespace Ui {
class KeyBoard;
}

class KeyBoard : public QDialog
{
    Q_OBJECT

public:
    explicit KeyBoard(QWidget *parent = 0);
    ~KeyBoard();

private slots:
    void receiveShow(QString setValue);
    void on_number1Button_clicked();
    void on_numberEnterButton_clicked();
    void on_number2Button_clicked();
    void on_number3Button_clicked();
    void on_number4Button_clicked();
    void on_number5Button_clicked();
    void on_number6Button_clicked();
    void on_number7Button_clicked();
    void on_number8Button_clicked();
    void on_number9Button_clicked();
    void on_number0Button_clicked();
    void on_numberDotButton_clicked();
    void on_numberDelButton_clicked();

private:
    Ui::KeyBoard *ui;
    void setInputValue();

signals:
    void sendInputValue(QString);
};

#endif // KEYBOARD_H
