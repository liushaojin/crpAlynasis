#ifndef UNITMAINTAIN_H
#define UNITMAINTAIN_H

#include <QWidget>
#include <qlabel.h>
#include <qcheckbox.h>
#include <QRadioButton>
#include <QComboBox>
#include <QGroupBox>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qgridlayout.h>
#include <QTimer>

#include "qdebug.h"
#include "AppHelper.h"
#include "Thread/SerialThread.h"

class UnitMaintain : public QWidget
{
    Q_OBJECT
public:
    explicit UnitMaintain(QWidget *parent = 0);

private:
    void initForm();          //初始化界面控件
    void initReactionDiskForm(); //反应盘
    void initCleanNeedleForm();  //清洗机构
    void initX_MotorForm();      //X电机
    void initY_MotorForm();      //Y电机
    void initProbeForm();        //加样针
    void initMixForm();          //搅拌杆
    void initSyringeForm();      //注射器
    void initPumbValveForm();    //泵阀
    void initT_CtlForm();        //温控
    void initDC_MixMotorForm();  //搅拌直流电机
    void initCleanMotorForm();   //清洗液进液泵

    void initSelfDefineForm();//自定义
    void initStyle();         //初始化样式
    void initData();          //初始化数据
    void initComboList();     //初始化下拉列表
    void initConnect();       //连接信号与槽

    QTimer *SensorTimer;                      //检测传感器定时器

signals:

public slots:
    //反应盘
    void slt_btnReactionDisk_Reset_Clicked();    //复位
    void slt_btnReactionDisk_Move_Clicked();     //反应盘转到指定杯位
    //清洗机构
    void slt_btnCleanNeedle_Reset_Clicked();       //清洗机构-复位
    void slt_btnCleanNeedle_MoveDownCup_Clicked(); //清洗机构-向下运动到反应杯内
    void slt_btnCleanNeedle_MoveUpCup_Clicked();   //清洗机构-向上运行到反应杯口
    void slt_btnCleanNeedle_Up_Clicked();          //清洗机构-上
    void slt_btnCleanNeedle_Down_Clicked();        //清洗机构-下
    //X电机
    void slt_btnX_Motor_Reset_Clicked();  //X电机复位
    void slt_btnX_Motor_Home_Clicked();   //X方向运行到清洗池位置
    void slt_btnX_Motor_Disk_Clicked();   //X方向运行到反应盘搅拌试剂样本位置
    void slt_btnX_Motor_Move2R_Clicked(); //X方向运行到指定试剂位号取试剂位置
    void slt_btnX_Motor_Move2S_Clicked();     //X方向运行到指定样本位取样本位置
    void slt_btnX_Motor_Move2S_Mix_Clicked(); //X方向运行到指定样本位搅拌样本位置
    //Y电机
    void slt_btnY_Motor_Reset_Clicked();  //Y电机复位
    void slt_btnY_Motor_Home_Clicked();   //Y方向运行到清洗池位置
    void slt_btnY_Motor_Disk_Clicked();   //Y方向运行到反应盘搅拌试剂样本位置
    void slt_btnY_Motor_Move2R_Clicked(); //Y方向运行到指定试剂位号取试剂位置
    void slt_btnY_Motor_Move2S_Clicked();     //Y方向运行到指定样本位取样本位置
    void slt_btnY_Motor_Move2S_Mix_Clicked(); //Y方向运行到指定样本位搅拌样本位置
    //加样针
    void slt_btnProbeReset_Clicked(); //加样针-垂直复位
    void slt_btnProbeClean_Clicked(); //加样针-向下运动到清洗处
    void slt_btnProbeDownDisk_Clicked();//加样针-向下运动到反应盘
    void slt_btnProbeDetect_Clicked();  //加样针-向下运动到探测面
    void slt_btnProbeDown_Clicked();//加样针-向下运动一定步数
    void slt_btnProbeUp_Clicked();  //加样针-向上运动一定步数
    //搅拌杆
    void slt_btnMixReset_Clicked(); //搅拌杆-垂直复位
    void slt_btnMixClean_Clicked();//搅拌杆-向下运动到清洗处
    void slt_btnMixDownDisk_Clicked();//搅拌杆-向下运动到反应盘
    void slt_btnMixDownMixS_Clicked();//搅拌杆-向下运动到样本搅拌处
    void slt_btnMixDown_Clicked();//搅拌杆-向下运动一定步数
    void slt_btnMixUp_Clicked();  //搅拌杆-向上运动一定步数
    //注射器
    void slt_btnSyringeReset_Clicked(); //注射器-垂直复位
    void slt_btnSyringeAbsorb_Clicked();//注射器-吸（0.1ul）
    void slt_btnSyringeInject_Clicked();  //注射器-吐（0.1ul）
    //泵阀
    void slt_btnOpenPumbValve_Clicked();  //常开
    void slt_btnClosePumbValve_Clicked(); //常闭
    void slt_btnPumbValveTime_Clicked();  //打开
    //温控
    void slt_btnT_Ctl_Disk_Clicked();     //反应盘温度
    void slt_btnT_Ctl_Reagent_Clicked();  //试剂仓温度
    //搅拌直流电机
    void slt_btnOpenDC_MixMotorForward_Clicked();  //(搅拌直流电机)正向旋转固定的周期数
    void slt_btnOpenDC_MixMotorReverse_Clicked();  //(搅拌直流电机)反向向旋转固定的周期数
    void slt_btnOpenDC_MixMotor_Clicked();         //(搅拌直流电机)停止直流电机的旋转
    void slt_btnCloseDC_MixMotor_Clicked();        //(搅拌直流电机)直流电机常开
    //清洗液进液泵
    void slt_btnOpenCleanMotorForward_Clicked();  //（清洗液进液泵）正向旋转固定的周期数
    void slt_btnOpenCleanMotorReverse_Clicked();  //（清洗液进液泵）反向向旋转固定的周期数
    void slt_btnOpenCleanMotor_Clicked();         //（清洗液进液泵）停止直流电机的旋转
    void slt_btnCloseCleanMotor_Clicked();        //（清洗液进液泵）直流电机常开

    //
    void slt_btnSelfDefine_Clicked();

private:
    //反应盘
    QGroupBox *grbReactionDisk;
    QPushButton *btnReactionDisk_Reset;    //复位
    QRadioButton *rdoReactionDisk_Forward; //正转
    QRadioButton *rdoReactionDisk_Reverse; //反转
    QLabel *labReactionDisk_Move;          //旋转指定个杯位
    QLineEdit *editReactionDisk_Cup;
    QPushButton *btnReactionDisk_Move;
    QGridLayout *lytReactionDisk;

    //清洗机构
    QGroupBox *grbCleanNeedle;
    QPushButton *btnCleanNeedle_Reset;       //复位
    QPushButton *btnCleanNeedle_MoveDownCup; //向下运动到反应杯内
    QPushButton *btnCleanNeedle_MoveUpCup;   //向上运行到反应杯口
    QLabel *labCleanNeedle_UpSteps;          //向上运动一定步数
    QLineEdit *editCleanNeedle_UpSteps;      //
    QPushButton *btnCleanNeedle_Up;          //
    QLabel *labCleanNeedle_DownSteps;        //向下运动一定步数
    QLineEdit *editCleanNeedle_DownSteps;    //
    QPushButton *btnCleanNeedle_Down;        //
    QGridLayout *lytCleanNeedle;

    //X电机
    QGroupBox *grbX_Motor;
    QPushButton *btnX_Motor_Reset;  //X电机复位
    QPushButton *btnX_Motor_Home;   //X方向运行到清洗池位置
    QPushButton *btnX_Motor_Disk;   //X方向运行到反应盘搅拌试剂样本位置
    QLabel *labX_Motor_R_Pos;       //试剂位
    QLineEdit *editX_Motor_R_Pos;
    QPushButton *btnX_Motor_Move2R; //X方向运行到指定试剂位号取试剂位置
    QLabel *labX_Motor_S_Pos;       //样本位
    QLineEdit *editX_Motor_Slot;    //样本槽号
    QLineEdit *editX_Motor_SlotNo;  //样本位
    QPushButton *btnX_Motor_Move2S;     //X方向运行到指定样本位取样本位置
    QPushButton *btnX_Motor_Move2S_Mix; //X方向运行到指定样本位搅拌样本位置
    QGridLayout *lytX_Motor;

    //Y电机
    QGroupBox *grbY_Motor;
    QPushButton *btnY_Motor_Reset;  //Y电机复位
    QPushButton *btnY_Motor_Home;   //Y方向运行到清洗池位置
    QPushButton *btnY_Motor_Disk;   //Y方向运行到反应盘搅拌试剂样本位置
    QLabel *labY_Motor_R_Pos;       //试剂位
    QLineEdit *editY_Motor_R_Pos;
    QPushButton *btnY_Motor_Move2R; //X方向运行到指定试剂位号取试剂位置
    QLabel *labY_Motor_S_Pos;       //样本位
    QLineEdit *editY_Motor_Slot;    //样本槽号
    QLineEdit *editY_Motor_SlotNo;  //样本位
    QPushButton *btnY_Motor_Move2S;     //X方向运行到指定样本位取样本位置
    QPushButton *btnY_Motor_Move2S_Mix; //X方向运行到指定样本位搅拌样本位置
    QGridLayout *lytY_Motor;

    //加样针
    QGroupBox *grbProbe;
    QPushButton *btnProbeReset; //垂直复位
    QPushButton *btnProbeClean; //向下运动到清洗处
    QPushButton *btnProbeDownDisk;//向下运动到反应盘
    QPushButton *btnProbeDetect;  //向下运动到探测面
    QLabel *labProbeStep;         //运动步数
    QLineEdit *editProbeStep;
    QPushButton *btnProbeDown;//向下运动一定步数
    QPushButton *btnProbeUp;  //向上运动一定步数
    QGridLayout *lytProbe;

    //搅拌杆
    QGroupBox *grbMix;
    QPushButton *btnMixReset; //垂直复位
    QPushButton *btnMixClean;//向下运动到清洗处
    QPushButton *btnMixDownDisk;//向下运动到反应盘
    QPushButton *btnMixDownMixS;//向下运动到样本搅拌处
    QLabel *labMixStep;         //运动步数
    QLineEdit *editMixStep;
    QPushButton *btnMixDown;//向下运动一定步数
    QPushButton *btnMixUp;  //向上运动一定步数
    QGridLayout *lytMix;

    //注射器
    QGroupBox *grbSyringe;
    QPushButton *btnSyringeReset; //垂直复位
    QLabel *labSyringeStep;       //运动步数
    QLineEdit *editSyringeStep;
    QPushButton *btnSyringeAbsorb;//吸（0.1ul）
    QPushButton *btnSyringeInject;  //吐（0.1ul）
    QGridLayout *lytSyringe;

    //泵阀
    QGroupBox *grbPumbValve;
    QComboBox *cboPumbValve;
    QPushButton *btnOpenPumbValve;
    QPushButton *btnClosePumbValve;
    QLabel *labPumbValveTime;
    QLineEdit *editPumbValveTime;
    QPushButton *btnPumbValveTime;
    QGridLayout *lytPumbValve;

    //温控
    QGroupBox *grbT_Ctl;
    QPushButton *btnT_Ctl_Disk;
    QLabel *labT_Ctl_Disk;
    QLineEdit *editT_Ctl_Disk;
    QPushButton *btnT_Ctl_Reagent;
    QLabel *labT_Ctl_Reagent;
    QLineEdit *editT_Ctl_Reagent;
    QGridLayout *lytT_Ctl;

    //搅拌直流电机
    QGroupBox *grbDC_MixMotor;
    QLabel *labDC_MixMotorCycleCount;
    QLineEdit *editDC_MixMotor;
    QPushButton *btnOpenDC_MixMotorForward;
    QPushButton *btnOpenDC_MixMotorReverse;
    QPushButton *btnOpenDC_MixMotor;
    QPushButton *btnCloseDC_MixMotor;
    QGridLayout *lytDC_MixMotor;

    //清洗液进液泵
    QGroupBox *grbCleanMotor;
    QLabel *labCleanMotorCycleCount;
    QLineEdit *editCleanMotor;
    QPushButton *btnOpenCleanMotorForward;
    QPushButton *btnOpenCleanMotorReverse;
    QPushButton *btnOpenCleanMotor;
    QPushButton *btnCloseCleanMotor;
    QGridLayout *lytCleanMotor;

    //自定义指令
    QGroupBox * grbSelfDefine;
    QLabel * labSelfDefine;
    QLineEdit * editSelfDefine;
    QPushButton * btnSelfDefine;
    QGridLayout * layoutSelfDefine;

    //总布局
    QGridLayout *layout;

};

#endif // UNITMAINTAIN_H
