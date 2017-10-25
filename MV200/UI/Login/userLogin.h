#ifndef USERLOGIN_H
#define USERLOGIN_H

#include <QFile>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QSettings>

#include "AppHelper.h"
#include "Parameter.h"
#include "UI/Widget/MessageBox/messageBox.h"

#define KEY  100 //密码异或加密基数

class UserLogin : public QDialog
{
    Q_OBJECT
public:
    explicit UserLogin(QDialog *parent = 0);

private:
    QString message;                               //登陆失败提示信息
    void initForm();                               //初始化
    void initStyle();                              //界面风格
    void initConnect();                            //连接信号与槽
    void initData();                               //初始化数据

    QPoint move_point;                             //记录鼠标位置
    void mousePressEvent( QMouseEvent * event );   //按下鼠标事件
    void mouseMoveEvent(QMouseEvent *event);       //移动鼠标事件
    void keyPressEvent(QKeyEvent  *event);         //键盘事件

    QSettings *gSetting;      //用户参数配置

signals:


public slots:
    void slt_btnLogin_Clicked();                   //槽：登陆
    void slt_btnCancel_Clicked();                  //槽：取消

private:
    QFrame * fraImage;
    QLabel * labTitle;
    QLabel * labUser;        //用户名
    QLineEdit * editUser;
    QLabel * labPwd;         //密码
    QLineEdit * editPwd;
    QCheckBox * chkSelfCheck; //是否要开机自检（即开机复位）
    QPushButton * btnLogin;
    QPushButton * btnCancel;
    QGridLayout * layout;

};

#endif // USERLOGIN_H
