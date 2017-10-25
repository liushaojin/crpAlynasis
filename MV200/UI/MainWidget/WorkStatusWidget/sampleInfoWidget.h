#ifndef SAMPLEINFOWIDGET_H
#define SAMPLEINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QGridLayout>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView
#include <QSignalMapper>

#include "ParamDefine.h"
#include "Thread/Scheduler.h"
#include "Object/SQLite/sqlite.h"
#include "../ApplicationWidget/SampleApplication.h"

#define STATUSCOUNT 10  //状态种类

//反应杯状态枚举
enum Sampletatus
{
    //空闲、待测试、测试中、已完成、未完成、缺少样本
    Idle       = 0,  //空闲
    SampleWaiting    = 1,  //待测试
    Testing    = 2,  //测试中
    SampleFinished   = 3,  //已完成
    unFinished = 4,  //未完成
    noSample   = 5   //缺少样本
};

class SampleInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SampleInfoWidget(QWidget *parent = 0);

private:
    void initSampleForm();        //
    void initSampleInfoForm();    //
    void initButtonForm();
    void initForm();              //初始化
    void initTableFormat();       //
    void initStyle();             //界面风格
    void initStyle2();            //
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据
    void setSampleStatus(int slot, int no, QString objName); //设置slot号样本架上no号样本的状态
    void ChangeSamplePosData(QString pos);  //更改样本位置（查询数据）
    QString CurrentSamplePos;

signals:

public slots:
    void slt_sigMapReceive(const QString&); //槽：信号与槽的管理群
    void slt_SetSampleStatus(int slot, int no, QString objName); //设置slot号样本架上no号样本的状态

    void slt_updateAllSampleStatus();                                   //槽：更新所有样本状态
    void slt_updateSampleStatus(QString currentPos);                    //槽：更新当前指定一个样本状态
    void slt_updateSampleStatus(QString formerPos, QString currentPos); //槽：更新指定两个样本的状态

    void slt_btnDelete_Clicked();    //槽:删除
    void slt_btnCurve_Clicked();     //槽:/反应曲线
    void slt_tbvList_DoubleClicked(QModelIndex);  //槽：列表双击事件
    void slt_UpdateRemainingTime(int cycle);   //槽：更新剩余反应时间(UI上显示测试剩余时间)

private:
    QWidget *wdgSample;
    QGridLayout *lytSample;

    QWidget *wdgSample1;
    QGridLayout *lytSample1;
    QLabel *labSample1;
    QLabel *labSample2;
    QLabel *labSample3;
    QLabel *labSample4;
    QPushButton *btnSample1[SAMPLESLOT_COUNT];    //第一排样本架
    QPushButton *btnSample2[SAMPLESLOT_COUNT];    //第二排样本
    QPushButton *btnSample3[SAMPLESLOT_COUNT];    //第三排样本架
    QPushButton *btnSample4[2];                   //急诊样本架

    QSignalMapper *sigMapper;  //信号管理器

    //状态说明
    //空闲、待测试、测试中、已完成、未完成、缺少样本
    QWidget *wdgStatusInfo;
    QGridLayout *lytStatusInfo;
    QLabel *labStatusName[STATUSCOUNT];      //状态名称
    QLabel *labStatusPic[STATUSCOUNT];       //状态图标

    QLabel *labTimes;   //大概剩余时间

    //
    QWidget *wdgSampleInfo;
    QVBoxLayout *lytvSampleInfo;
    QGroupBox *grbSampleInfo;
    QGridLayout *lytSampleInfo;
    QLabel *labSampleSlot;       //样本加
    QLineEdit *editSampleSlot;
    QLabel *labSamplePos;       //样本位
    QLineEdit *editSamplePos;
    QLabel *labSampleType;      //样本类型
    QLineEdit *editSampleType;

    QTableView *tbvList;
    QStandardItemModel *modelList;

    //按钮
    QWidget *wdgBtn;
    QPushButton *btnDelete;   //删除
    QHBoxLayout *lythBtn;

    //总布局
    QGridLayout *layout;
};

#endif // SAMPLEINFOWIDGET_H





