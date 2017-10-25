/*******************************************************************
Copyright (C),    2017~ ,    中山迈盛有限公司，Co
    File Name:    Scheduler.h
       Author:    郑文洋
      version:    1
         Date:    2017-05
  Description:    线程类：调度程序，
       Others:
Function list:
      History:
*********************************************************************/

#ifndef GSCHEDULER_H
#define GSCHEDULER_H

#include <QTime>
#include <QObject>
#include <QThread>
#include <qtimer.h>
#include "Parameter.h"
#include "TcpServer.h"
#include "FrameDefine.h"
#include "ParamDefine.h"
#include "Object/ErrorTable/MV_ErrTable.h"

#define TASKCOUNT             30          //任务队列长度

#define BLANKCYCLE_INIT        3          //空周期指令个数（开机清洗反应杯）
#define BLANKCYCLE_INIT2       2          //初始化搅拌（并且清洗反应杯）

#define CHANGESAMPLESLOT       3          //切换样本架（需要加X个空周期）

#define BLANKCYCLE_R2         29          //空周期指令个数（最后一个测试加完R2后还要跑的空周期次数）

#define CLEANCUP_TIME        6000         //测试前清洗反应杯的间隔时间


//工作模式状态
enum MV_State     //仪器运行状态
{
    MV_State_unChecked = -1,  //未自检（不能进行测试或维护）
    MV_State_IDLE = 0,       //空闲
    MV_State_Normal = 1,     //普通测试(样本测试)
    MV_State_QC = 2,         //质控测试
    MV_State_CalibrateT = 3, //定标测试
    MV_State_BootReset = 4,  //开机复位
    MV_State_Pause =5,       //暂停
    MV_State_Stop = 6,       //停止
    MV_State_Error = 7,      //出错，通讯故障[前条指令无应答/结果返回]
    MV_STATE_ReadCard = 8    //读卡
};

//测试结构体
struct Task
{
    int id;                      //对应数据库中任务队列表中的ID
    int History_id;              //对应数据库中历史结果数据列表中的ID
    int Status;                  //状态：（0:空闲， 1:任务测试, 2:任务已经进入队列）
    bool isAdded;                //是否加样完成，（0：未完成，1：加样完成,同时收集AD数据）

    int AD_Count;                //采光次数
    int LightPos1;               //测光点1
    int LightPos2;               //测光点2

    int TestType;                //测试类型
    int ProjectId;               //项目id
    int isCalculateProj;         //是否为计算项目（0：常规项目，1：计算项目）

    int isMixNextSample;         //样本是否需要搅拌（注意需与结构结合，这里批的是当前样本后两位的样本是否需要搅拌， 与MV100不同）
    int isMixCurrentSample;      //当前样本是否需要搅拌（切换样本架时需要用到）
    int isDiluent;               //样本是否需要稀释
    int SampleType;              //标本类型
    QString SampleCode;          //样本试管编号（条码）
    QString SamplePos;           //样本位置（如：1-1，1-2）

    int Cup[2];                  //测试用的反应杯号（注：cup[0]用稀释样本，cup[2]用于孵育）

    int Reagent0;                //稀释液编号(0＝无此试剂)
    int Reagent1;                //R1编号(0＝无此试剂)
    int Reagent2;                //R2编号(0＝无此试剂)

    int VolumeR0;                //稀释液R0量
    int VolumeSample;            //样本量
    int VolumeR1;                //R1量
    int VolumeRS;                //稀释后样本的量
    int VolumeR2;                //R2量
};

enum SampleType   //样本类型
{
    //1:全血，2：血清，3：血浆，4：尿液，5：其他
    SampleType_Blood = 1,
    SampleType_Serum = 2,
    SampleType_Plasma = 3,
    SampleType_Urine = 4,
    SampleType_Other = 5
};

class GScheduler : public QThread
{
    Q_OBJECT
public:
    explicit GScheduler(QThread *parent = 0);

    int get_isRunning();    //机器是否正在运行中
    int getWorkModeFlag();  //工作模式状态标志
    void setWorkModeFlag(int flag);

    Task getTaskList(int pt); //查询“任务队列”中pt位置的任务信息
    void FinishedTest(int pt); //测试完成（即采光采完成）

    int ActionStart();      //测试周期
    int InitialMachine();   //开机初始化（复位）
    int TaskStart();        //开始测试
    int StartReadCard();    //读Mifare卡片（空闲才能读卡）
    int ResetServerIP(QString ip);    //重置中位机服务器IP
    int setMotionType(int type); //设置调用分支

    int get_isBootCleanedFlag();  //开机是否已经清洗了反应杯
    bool get_isCycleFlag();       //是否跑周期指令
    struct Task TaskList[TASKCOUNT];   //任务队列,保存测试信息

private:  
    int m_MotionFinished;   //是否工作中(0：工作中， 1：空闲)
    int m_MotionType;       //模式分支
    int WorkModeFlag;        //工作模式状态标志
    int m_TaskPt;            //记录队列中新增测试在任务队列中的位置
    int m_UsedCup;           //记录反应杯的使用情况
    int m_BlankCycleCount;   //最后一个测试加完R2后还要跑的空周期次数
    int isBootCleanedFlag;   //开机是否已经清洗了反应杯
    int m_BootCleanedTimes;  //开机清洗的反应杯个数
    int CurrentCleaningCupNo;//当前清洗的反应杯编号（从0开始计数,界面显示从1开始）
    bool isCycleFlag;       //是否跑周期指令
    QTimer *CycleTimer;     //周期定时器

    void initParam();       //开机/复位初始化参数
    void initialMachine();  //开机初始化
    bool reset(char *cmd, int lenght, QString str, int waitTime); //复位
    void taskStart();       //开始测试
    void readCard();        //读卡（读S50卡上16个扇区全部的数据）
    void resetServerIP();   //重置中位机服务器IP

    int CheckCupStatus(int cNo);  //分配反应杯前都要检查反应杯是否清洗干净(0:正常，1:脏杯)
    int getTaskInfo();            //从数据库中取出测试数据
    //将从数库中取出来的测试放到任务队列中
    int enTaskList(int id, int projectID, int testType, int sampleMode, int isMixCurrentSample, int isStirSample, int isDiluent,QString SampleCode, QString SamplePos, int R0, int R1, int R2, int SampleV, int R0V, int R1V, int RSV, int R2V, QString applicateTime, int lightPos1, int lightPos2, int isRetest, QString taskNo, int isCalculateP, int QcLiquidID);
    //将正在进行的测试（刚从数据库任务表中取出来的测试）添加到数据库历史结果数据表中
    int insertIntoTempHistoryT(int TaskT_id, QString taskNo, QString applicateT, int QcLiquidID, int isRetest);
    void Action();                //周期动作
    void CombineCycleCmd(int sampleNo); //整合周期指令（发送到中位机）
    void CombineEmptyCycleCmd(int turnRD, int samplePos, int isMix, int isMixPox, int isAD, int isScanSampleCode);//整合空周期指令（发送到中位机）
    bool isSampleExist(QString samplePos); //样本是否存在

    //201708
    QString CurrentSampleSlot;    //当前测试的样本架的编号（用于判断是前后测试是否在同一样本架上）
    int isCurrentSampleNeedMix;   //当前搅拌位置的样本是否需要搅拌
    int CurrentMixSampleSlot;     //当前搅拌位的样本架
    int CurrentMixSampleNo;       //当前搅拌位的样本位
    QString CurrentMixSamplePos;  //当前搅拌位的样本位置
    int ChangedSampleSlot;        //切换样本架（需要加X个空周期）
    bool isNextSampleExist;       //下一样本是否存在
    int EmptyCycleTimes;          //要发的空周期个数
    int EmptySampleCount;         //间隔样本个数
    bool NewTestInTeam;           //新测试进入队列
    int AddedSampleCup;           //上周期加样的反应杯
    QString CurrentSamplePos;     //当前测试的样本位，若前后两个测试的样本位一不样，则样本需要搅拌（以及更新样本状态）

    //重置服务器ip
    QString ServerIP;



protected:
    void run();

signals:
    void sig_ConnectTCP(QString ip, int port);//信号：
    void sig_disConnectTCP();                 //信号：

    void sig_CycleTimer_start(int);            //信号：开始周期定时器
    void sig_CycleTimer_stop();                //信号：停止周期定时器
    void sig_Finished_init();                  //信号：初始化完成
    void sig_initMachine_Step(QString name);   //信号：开机初始化的步骤
    void sig_TcpServer_Write(QString data);    //信号：写数据
    void sig_TcpServer_Write(char *data, int lenght);      //信号：写数据
    void sig_Update_ApplicationTaskList();     //信号：更新申请列表

    void sig_setCupStatus(int CupNo, QString objName);           //信号：设置反应杯的状态
    void sig_AutoUpdateSampleList();  //信号：有新的样本进行测试，"结果查询"界面"样本编号"列表
    void sig_UpdateApplicatedCount(); //信号：实时更新数据库中状态为“等待”的测试的个数

    //反应盘温度、试剂仓温度、复合指令加样针下降高度、4种液体状态（去离子水、清洗液、低浓度废液、高浓度废液）
    void sigReactionDiskT(double t);
    void sigReagentSlotT(double t);
    void sigLiquidStatus(int s);

    //信号：更新“开始”“暂停”“停止”三个按钮的状态（所有测试都完成，或 运行出错时 调用） (type=0：所有测试都完成， type=1:运行出错)
    void sig_UpdateMainWidget3Button_State(int type);

    void sig_UpdateRemainingTime(int cycle);   //信号：更新剩余反应时间(UI上显示测试剩余时间)
    void sig_updateSampleStatus(QString formerPos, QString currentPos); //信号：更新样本状态信息

    void sig_FinishedMaintain(int type);              //信号：完成维护或者维护过程中出错(type=0：完成维护，type=1：维护出错)

    void sig_ResetIP(int state); //信号：修改中位机IP是否成功（0：失败， 1：成功）

    void sig_ReadCard_State(int state); //信号：读卡信号（返回状态）
    void sig_ReadCard_Data(QString data);//信号：读卡数据
    void sig_ReadCardProgressBar_ValueChanged(int s);//信号：当前读卡的扇区（读卡进度条）


public slots:
    void slt_CycleTimer_Stop();                //槽：停止周期定时器计时（意外停机）
    void slt_CycleTimer_timeout();             //槽：周期定时器

    void slt_Suspend();                        //槽：暂停
    void slt_Stop();                           //槽：停止

};
extern GScheduler * gScheduler;
#endif // GSCHEDULER_H












