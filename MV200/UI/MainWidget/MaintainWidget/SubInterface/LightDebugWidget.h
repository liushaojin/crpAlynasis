#ifndef LIGHTDEBUGWIDGET_H
#define LIGHTDEBUGWIDGET_H

#include <QWidget>
#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <qlineedit.h>
#include <QPushButton>
#include <QGridLayout>
#include <qtimer.h>
#include <QTime>
#include <QDateTime>
#include <QIntValidator>

#include "Thread/SerialThread.h"
#include "UI/Widget/QCustomPlot/qcustomplot.h"

class LightDebugWidget : public QWidget
{
    Q_OBJECT
public:
    static LightDebugWidget *getInstance();

private:
    static LightDebugWidget *_instance;
    explicit LightDebugWidget(QWidget *parent = 0);
    void initForm();                           //初始化界面控件
    void initStyle();                          //初始化样式
    void initConnect();                        //连接信号与槽
    void initLanguage();                       //初始化语言
    void initData();                           //初始化数据
    void updateData();                         //更新数据

signals:

public slots:
    void slt_btnReset_Clicked();               //槽：复位
    void slt_btnStart_Clicked();               //槽：开始采光
    void slt_Timer_timeout();

private:
    QGroupBox * grbSettingInfo;  //
    QPushButton * btnReset;      //反应盘复位
    QLabel * labStartCup;        //开始采光杯位
    QLineEdit * editStartCup;
    QLabel * labDelay;        //采集延迟时间
    QLineEdit * editDelay;
    QPushButton * btnStart;      //开始采光
    QGridLayout * lytSettingInfo;

    QCustomPlot *customPlot;
    QCPGraph *graphResult;      //原始数据
    QCPGraph *graphResultJump;  //跳变数据
    QCPGraph *graphResultSensor;//编码盘数据
    QCPGraph *graphOriginData;  //原始数据

    //总布局
    QGridLayout * layout;
};

#endif // LIGHTDEBUGWIDGET_H
