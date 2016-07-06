#include "keyboard.h"
#include "ui_keyboard.h"

KeyBoard::KeyBoard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyBoard)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
}

KeyBoard::~KeyBoard()
{
    delete ui;
}

void KeyBoard::receiveShow()
{
    this->show();
    ui->inputLineEdit->setText("");
}

void KeyBoard::on_numberEnterButton_clicked()
{
    emit showMainPage();
    this->hide();
}

void KeyBoard::on_number1Button_clicked()
{
    setInputValue();
}

void KeyBoard::on_number2Button_clicked()
{
    setInputValue();
}

void KeyBoard::on_number3Button_clicked()
{
    setInputValue();
}

void KeyBoard::on_number4Button_clicked()
{
    setInputValue();
}

void KeyBoard::on_number5Button_clicked()
{
    setInputValue();
}

void KeyBoard::on_number6Button_clicked()
{
    setInputValue();
}

void KeyBoard::on_number7Button_clicked()
{
    setInputValue();
}

void KeyBoard::on_number8Button_clicked()
{
    setInputValue();
}

void KeyBoard::on_number9Button_clicked()
{
    setInputValue();
}

void KeyBoard::on_number0Button_clicked()
{
    setInputValue();
}

void KeyBoard::on_numberDotButton_clicked()
{
    setInputValue();
}

void KeyBoard::setInputValue()
{
    ui->inputLineEdit->setText(ui->inputLineEdit->text() + qobject_cast<QPushButton *>(sender())->text());
}

void KeyBoard::on_numberDelButton_clicked()
{
    QString text = ui->inputLineEdit->text();
    text.chop(1);
    if(text.isEmpty())
        text="";
    ui->inputLineEdit->setText(text);
}
