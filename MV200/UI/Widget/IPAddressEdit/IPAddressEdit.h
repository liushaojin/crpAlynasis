#ifndef IPADDRESSEDIT_H
#define IPADDRESSEDIT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QKeyEvent>

class IPAddressEdit : public QWidget
{
    Q_OBJECT
public:
    explicit IPAddressEdit(QWidget *parent = 0);

    bool validCheck();
    bool subCheck(QLineEdit*);
    bool subCheck4(QLineEdit*);
    void setIPAddress(QString ip);
    QString getIPAddress();
    void textClear();

private:
    void keyPressEvent(QKeyEvent  *event);         //键盘事件

signals:

public slots:

public:
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;

    QHBoxLayout *mainLayout;
    QLineEdit *number1;
    QLineEdit *number2;
    QLineEdit *number3;
    QLineEdit *number4;

    QGridLayout *layout;
};

#endif // IPADDRESSEDIT_H
