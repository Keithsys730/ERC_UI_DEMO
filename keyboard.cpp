#include "keyboard.h"
#include "ui_keyboard.h"

KeyBoard::KeyBoard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyBoard)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    setGeometry(400,100,285,320);
    //ui->number1Button->installEventFilter(this);
}

KeyBoard::~KeyBoard()
{
    delete ui;
}
/*
bool KeyBoard::eventFilter(QObject *object,QEvent *event)
{
    if(object == ui->number1Button)
    {
        if(event->type()==QEvent::MouseButtonRelease)
        {
            emit sendInputValue(ui->inputLineEdit->text());
            this->hide();
            return true;
        }
    }
    //return KeyBoard::eventFilter(object, event);
}
*/
void KeyBoard::receiveShow()
{
    this->show();
    ui->inputLineEdit->setText("");
}

void KeyBoard::on_numberEnterButton_clicked()
{
    emit sendInputValue(ui->inputLineEdit->text());
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

void KeyBoard::on_numberClearButton_clicked()
{
    ui->inputLineEdit->setText("");
    /*
    QString text = ui->inputLineEdit->text();
    text.chop(1);
    if(text.isEmpty())
        text="";
    ui->inputLineEdit->setText(text);
    */
}

void KeyBoard::on_numberEscButton_clicked()
{
    emit sendInputValue("");
    this->hide();
}
