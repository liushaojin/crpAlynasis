#ifndef SYSSETTINGWIDGET_H
#define SYSSETTINGWIDGET_H

#include <QFile>
#include <QLabel>
#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QTabWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QRadioButton>
#include <QMouseEvent>

#include "Parameter.h"
#include "Object/SQLite/sqlite.h"
#include "UI/Widget/MessageBox/messageBox.h"

class SysSettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SysSettingWidget(QWidget *parent = 0);

private:
    void initForm();
    void initLiquidSettingForm();    //液体报警及是否停机设置
    void initReactionDiskForm();     //反应盘设置
    void initOtherSettingForm();     //其他参数设置
    void initStyle();               //初始化样式表
    void initData();                //初始化数据
    void initConnection();          //初始化信号与槽
    QStringList updateStrList;      //
    QPoint move_point;                             //记录鼠标位置
    void mousePressEvent(QMouseEvent * event );   //按下鼠标事件
    void mouseMoveEvent(QMouseEvent *event);       //移动鼠标事件

signals:

public slots:
    void slt_btnSaveLiquidParam_Clicked();         //槽：保存液体参数
    void slt_btnSaveReactionDiskParam_Clicked();   //槽：保存反应盘参数
    void slt_btnSaveOtherParam_Clicked();          //槽：保存其他参数
    void slt_btnClose_Clicked();                   //槽：退出

private:
    //液体报警及是否停机设置
    QWidget *wdgLiquidSetting;
    QRadioButton *rdo_isLiquidOnlyAlarm; //仅报警（不停机）
    QRadioButton *rdo_isLiquidAlarmStop; //报警停机
    QLabel *labLessWaterTimes;           //提示“缺少去离子水”后最多能运行的次数
    QLabel *labLessCleanLiquidTimes;     //提示“缺少清洗液”后最多能运行的次数
    QLabel *labFull_L_WasteTimes;        //提示“低浓度废液满”后最多能运行的次数
    QLabel *labFull_H_WasteTimes;        //提示“高浓度废液满”后最多能运行的次数
    QLineEdit *editLessWaterTimes;           //
    QLineEdit *editLessCleanLiquidTimes;     //
    QLineEdit *editFull_L_WasteTimes;        //
    QLineEdit *editFull_H_WasteTimes;        //
    QPushButton *btnSaveLiquidParam;         //保存
    QGridLayout *lytLiquidSetting;

    //反应盘设置
    QWidget *wdgReactionDisk;
    QLabel *labDirtyCupAD;                  //脏杯AD值
    QLineEdit *editDirtyCupAD;
    QPushButton *btnSaveReactionDiskParam;  //保存
    QGridLayout *lytReactionDisk;

    //其他设置
    QWidget *wdgOtherSetting;
    QLabel *labHospitalName;              //医院信息设置(医院名称)
    QLineEdit *editHospitalName;
    QLabel *labResultTransfer;            //临时结果是否自动转到结果表
    QRadioButton *rdoAutoTransfer_Y;
    QRadioButton *rdoAutoTransfer_N;
    QPushButton *btnSaveOtherParam;
    QGridLayout *lytOtherSetting;

    //总布局
    QPushButton *btnClose;  //退出
    QTabWidget *tabWidget;
    QGridLayout *layout;

};

#endif // SYSSETTINGWIDGET_H
