#include "IPAddressEdit.h"
#include <qdebug.h>

IPAddressEdit::IPAddressEdit(QWidget *parent) : QWidget(parent)
{
    label1=new QLabel(".");
    label2=new QLabel(".");
    label3=new QLabel(".");
    number1=new QLineEdit;
    number2=new QLineEdit;
    number3=new QLineEdit;
    number4=new QLineEdit;
    layout = new QGridLayout;
    layout->setMargin(5);
    layout->setSpacing(0);
    layout->addWidget(number1, 0,  0, 2, 5);
    layout->addWidget(label1,  0,  4, 2, 1, Qt::AlignRight);
    layout->addWidget(number2, 0,  5, 2, 5);
    layout->addWidget(label2,  0,  9, 2, 1, Qt::AlignRight);
    layout->addWidget(number3, 0, 10, 2, 5);
    layout->addWidget(label3,  0, 14, 2, 1, Qt::AlignRight);
    layout->addWidget(number4, 0, 15, 2, 5);
    this->setLayout(layout);

    number1->setMaxLength(3);
    number2->setMaxLength(3);
    number3->setMaxLength(3);
    number4->setMaxLength(3);
    number1->setAlignment(Qt::AlignCenter);
    number2->setAlignment(Qt::AlignCenter);
    number3->setAlignment(Qt::AlignCenter);
    number4->setAlignment(Qt::AlignCenter);
}

bool IPAddressEdit::validCheck()
{//判断IP地址是否合理
    return subCheck(number1) && subCheck(number2) && subCheck(number3) && subCheck4(number4);
}

bool IPAddressEdit::subCheck(QLineEdit *number)
{
    QString str=number->text();
    bool ok=true;
    int tmp=str.toInt(&ok,10);//合理性判断
    if(!ok)
        return false;

    if(tmp<0 || tmp>255)
        return false;

    return true;
}

bool IPAddressEdit::subCheck4(QLineEdit *number)
{
    QString str=number->text();
    bool ok=true;
    int tmp=str.toInt(&ok,10);//合理性判断
    if(!ok)
        return false;

    if(tmp<2 || tmp>254)
        return false;

    return true;
}

//get the IP address
QString IPAddressEdit::getIPAddress()
{
    QString str;

    str+=number1->text().trimmed();
    str+=".";
    str+=number2->text().trimmed();
    str+=".";
    str+=number3->text().trimmed();
    str+=".";
    str+=number4->text().trimmed();

    return str;
}

void IPAddressEdit::setIPAddress(QString ip)
{
    QStringList list = ip.split(".");
    if(list.count() != 4) return;

    number1->setText(list.at(0));
    number2->setText(list.at(1));
    number3->setText(list.at(2));
    number4->setText(list.at(3));
}

void IPAddressEdit::textClear()
{
    number1->clear();
    number2->clear();
    number3->clear();
    number4->clear();
}

//键盘事件
void IPAddressEdit::keyPressEvent(QKeyEvent  *event)
{
    //    //回车键 & 确定（数字键）
    //    if(event->key()== Qt::Key_Return || event->key()== Qt::Key_Enter)
    //    {
    //        qDebug() << "*********444*";
    //    }
}









