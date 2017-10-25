#ifndef INITWIDGET_H
#define INITWIDGET_H

#include <QWidget>
#include <QMovie>
#include <QFrame>
#include <qlabel.h>
#include <QGridLayout>

#include "MV200_MainWidget.h"
#include "Thread/Scheduler.h"
#include "UI/Widget/MessageBox/messageBox.h"

class InitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InitWidget(QWidget *parent = 0);
    void StartInit();           //开机初始化

private:
    void initForm();         //初始化
    void initStyle();        //界面风格
    void initConnect();      //连接信号与槽
    void initData();         //初始化数据

    void showMainWidget();   //进入MV100主界面
    void closeMV200();       //退出程序

signals:

public slots:
    void slt_initStep(QString msg);    //槽：初始化进行的步骤
    void slt_ShowMainWidget();         //槽：进入MV100主界面
    void slt_CloseMV100();                    //槽：退出程序

private:
    QFrame *fraLogo;
    QLabel *labInitMsg;  //初始化信息
    QGridLayout *layout;

};

#endif // INITWIDGET_H
