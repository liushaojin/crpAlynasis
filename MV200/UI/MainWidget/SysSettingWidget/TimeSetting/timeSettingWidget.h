#ifndef TIMESETTINGWIDGET_H
#define TIMESETTINGWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <qprocess.h>
#include <QTimer>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QMouseEvent>

class TimeSettingWidget : public QDialog
{
    Q_OBJECT
public:
    static TimeSettingWidget *getInstance();

private:
    TimeSettingWidget();
    static TimeSettingWidget *_instance;
    void initLanguage();                       //语言设置
    void initForm();                           //初始化界面控件
    void initStyle();                          //初始化样式
    void initData();                           //初始化数据
    void initConnect();                        //连接信号与槽
    QProcess *process;                        //调用外部程序
    QProcess *process2 ;
    QTimer * timer;
    QPoint move_point;                            //记录鼠标位置
    void mousePressEvent(QMouseEvent * event );   //按下鼠标事件
    void mouseMoveEvent(QMouseEvent *event);      //移动鼠标事件

signals:

public slots:
    void slt_btnBack_Clicked();                //槽：返回
    void slt_btnReset_Clicked();               //槽：
    void slt_btnSure_Clicked();                //槽：
    void slt_timeOut();                        //槽：
    void slt_readOutput();

private:
    QLabel * labTitle;
    QLabel * lab_year;
    QLabel * lab_month;
    QLabel * lab_date;
    QLabel * lab_hour;
    QLabel * lab_min;
    QLabel * lab_sec;
    QLineEdit * txt_year;
    QLineEdit * txt_month;
    QLineEdit * txt_date;
    QLineEdit * txt_hour;
    QLineEdit * txt_min;
    QLineEdit * txt_sec;
    QPushButton * btn_reset;  //重置
    QPushButton * btn_sure;
    QPushButton * btnBack;
    QGridLayout * layout;
};

#endif // TIMESETTINGWIDGET_H
