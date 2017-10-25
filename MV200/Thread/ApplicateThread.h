/*******************************************************************
Copyright (C),    2016~ ,    中山迈胜有限公司，Co
    File Name:    Scheduler.h
       Author:    郑文洋
      version:    1
         Date:    2016-01
  Description:    线程类：调度程序，
       Others:
Function list:
      History:
*********************************************************************/

#ifndef APPLICATETHREAD_H
#define APPLICATETHREAD_H


#include <QThread>
#include <QTime>
#include "Parameter.h"
#include "Thread/Scheduler.h"

enum TestType
{
    TestType_NomalSample = 0,   //普通样本测试
    TestType_QC = 1,            //质控样本测试
    TestType_Calibrate = 2      //定标样本测试
};

class ApplicateThread : public QThread
{
    Q_OBJECT
public:
    explicit ApplicateThread(QThread *parent = 0);

    //申请测试
    int ApplicateTest(int testType, QString samplePos, QString startNo, int sampleType, QStringList projectList, QStringList calculateIdList,
                      QMap<int, QString> sampleNoMap, int repeatTime, int qcLiquid);

private:
    //样本申请信息
    int TestType;               //测试类型
    int RequestedNO;            //申请的测试的个数
    QString SamplePos;          //样本位置信息
    QString StartNo;            //申请开始的编号（批量时要自增）
    int SampleType;             //样本类型
    QStringList ProjectIdList;  //常规项目id列表
    QStringList CalculateIdList;//计算项目id列表
    QMap<int, QString> SampleNoMap; //样本条码
    int RepeatTime;             //重复次数
    int QcLiquidID;

    //申请测试
    void applicateTest(int testType, QString samplePos, QString startNo, int sampleType, QStringList projectList, QStringList calculateIdList,
                       QMap<int, QString> sampleNoMap, int repeatTime, int qcLiquid);
    //更新测试编号
    QString UpdateRequestedNO();
    //申请任务（即将申请的任务保存到数据库中）
    void RequestTask(int TestType, int isUrgent, QString SamplePos, int isCalculateP, int ProjectID,
                     QString TaskNo, int SampleType, QString sampleCode, int qcLiquid);

    QStringList sqlListStr;   //提交给数据库处理的事物语句

signals:
    void sig_updateAllSampleStatus(); //信号：更新“工作状态”-“样本信息”中的样本状态
    void sig_UpdateApplicatedCount(); //信号：实时更新数据库中状态为“等待”的测试的个数

protected:
    void run();

};
extern ApplicateThread * gApplicateThread;
#endif // APPLICATETHREAD_H
