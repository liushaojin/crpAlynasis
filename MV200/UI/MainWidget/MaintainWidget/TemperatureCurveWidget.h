#ifndef TEMPERATURECURVEWIDGET_H
#define TEMPERATURECURVEWIDGET_H

#include <QWidget>
#include <QVector>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <qtimer.h>

#include "Garameter.h"
#include "Thread/SerialThread.h"
#include "UI/Widget/QCustomPlot/qcustomplot.h"

#define TEMPERATURE_COUNT 60 //温度曲线X轴的个数
#define TEMPERATURE_H     50 //温度曲线的-上限值
#define TEMPERATURE_L      0 //温度曲线的-下限值

class TemperatureCurveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TemperatureCurveWidget(QWidget *parent = 0);

private:
    void initData();        //初始化数据
    void initForm();        //初始化界面控件
    void initStyle();       //初始化样式
    void initConnect();     //连接信号与槽
    void updateReactionDiskCurve(); //更新反应盘温度曲线
    void updateReagentSlotCurve();  //更新反应盘温度曲线

    QVector<double> vecReactionDisk; //反应盘温度动态数组（固定长度）
    QVector<double> vecReagentSlot;  //试剂槽温度动态数组（固定长度）

signals:

public slots:
    void slt_appendNewReactionDist_T(double t); //槽：追加反应盘温度到动态数据中
    void slt_appendNewReagentSlot_T(double t);  //槽：追加试剂槽温度到动态数据中
    void slt_timerReactionDisk_timeout();       //槽：反应盘温度定时器
    void slt_timerReagentSlot_timeout();        //槽：试剂槽温度定时器
    void slt_btnReactionDisk_Clicked();         //槽：反应盘温度
    void slt_btnReagentSlot_Clicked();          //槽：试剂槽温度

private:
    //定时器
    QTimer *timerReactionDisk;    //反应盘温度定时器
    QTimer *timerReagentSlot;     //试剂槽温度定时器

    //查询设置
    QWidget *wdtSetting;
    QCheckBox *chkReactionDisk;     //反应盘
    QLabel *labReactionDisk;
    QLineEdit *editReactionDisk;
    QPushButton *btnReactionDisk;
    QLabel *labReactionDiskAlarm;   //温度报警
    QCheckBox *chkReagentSlot;      //试剂槽
    QLabel *labReagentSlot;
    QLineEdit *editReagentSlot;
    QPushButton *btnReagentSlot;
    QLabel *labReagentSlotAlarm;    //温度报警
    QGridLayout *lytSetting;

    //温度曲线
    QCustomPlot *customPlot;
    QCPGraph *graphResult_Disk;
    QCPGraph *graphResult_Reagent;

    //总布局
    QGridLayout *layout;

};

#endif // TEMPERATURECURVEWIDGET_H
