#ifndef ADDNEWUSERWIDGET_H
#define ADDNEWUSERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QMouseEvent>
#include <QKeyEvent>
#include "Object/SQLite/sqlite.h"
#include "UI/Widget/MessageBox/messageBox.h"

class AddNewUserWidget : public QWidget
{
    Q_OBJECT
public:
    static AddNewUserWidget *getInstance(); //实例对象

private:
    explicit AddNewUserWidget(QWidget *parent = 0);
    static AddNewUserWidget *_instance;  //本类单例对象
    void initForm();              //初始化
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽

    QPoint move_point;                             //记录鼠标位置
    void mousePressEvent( QMouseEvent * event );   //按下鼠标事件
    void mouseMoveEvent(QMouseEvent *event);       //移动鼠标事件
    void keyPressEvent(QKeyEvent  *event);         //键盘事件


signals:
    void sig_AddNewUser();  //信号：添加新用户成功

public slots:
    void slt_btnCancel_Clicked();  //槽:
    void slt_btnSure_Clicked();    //槽:

private:
    QLabel *labName;
    QLabel *labPassword;
    QLineEdit *editName;
    QLineEdit *editPassword;
    QPushButton *btnCancel;
    QPushButton *btnSure;
    QGridLayout *layout;
};
#endif // ADDNEWUSERWIDGET_H
