/**************************************************************************************
Copyright (C),    2016~ ,    中山迈胜有限公司，Co
    File Name:    sqlite.h
       Author:    郑文洋
      version:    1
         Date:    2015-12
  Description:    对SQLite的操作, .db是程序运行极为重要的文件，
                  没有此文件对程序运行有极大的影响。machineSQL.db文件保存程序运行的设置及申请的测试等。

                  每次开机时先检查数据库文件是否存在，若不存在则新建数据库，并创建数据表及初始化特殊表的数据。
                  若存在，则逐个检查数据表，若数据表不存在，则新建数据表。
       Others:
Function list:    1.initCheckDB();
                  2.initCheckDBError();
                  3.CheckDBTable();
                  4.CreateTable();
                  5.CleanTableData();
                  6.Login();
                  7.Select();
                  8.Insert();
                  9.Delete();
                  10.Update();
                  11.openDB();
                  12.closeDB();
                  13.SQLite();
      History:
****************************************************************************************/
#ifndef SQLITE_H
#define SQLITE_H

#include <QObject>
#include <QSqlDatabase>
#include "qsqlerror.h"
#include <QSqlQuery> //类用来执行SQL语句
#include <QStringList>
#include <QDebug>

#include "ParamDefine.h"
#include "Parameter.h"

class SQLite : public QObject
{
    Q_OBJECT
public:
    static SQLite * getInstance();
    bool openDB();                       //打开数据库
    void closeDB();                      //关闭数据库

    bool initCheckDB();             //每次开机时都对数据库进行检查（常规检查：检查表是否存在）
    bool CheckDBTable(QString table);    //检查数据库中的表是否存在
    bool CreateTable(QString str);       //建立表
    bool CleanTableData(QString table);  //清空表,并且自增列归零
    bool DropTable(QString table);       //删除表

    int Login(QString user, QString psd);//用户登陆
    int getTableCount(QString str);      //统计表中的数据总行数
    QSqlQuery Select(QString str);       //查询数据
    bool Insert(QString str);            //插入数据
    bool Delete(QString str);            //删除数据
    bool Update(QString str);            //修改数据
    bool Transaction(QStringList strList);//事物处理

private:
    explicit SQLite(QObject *parent = 0);
    static SQLite * _instance;
    QSqlDatabase db;

    int CreateUserT();                   //用户表

    int CreateUserLoginRecordT();        //用户登录记录表
    int CreateSysParamT();               //系统参数表
    int CreateStateT();                  //状态表
    int CreateTaskT();                   //任务表
    int CreateTestTypeT();               //测试类型表
    int CreateReactionDiskT();           //反应盘表
    int CreateSampleSlotT();             //样本盘表
    int CreateReagentT();                //试剂表
    int CreateSampleTypeT();             //样本类型表
    int CreateProjectT();                //项目表
    int CreateCombineProjectT();         //组合项目表
    int CreateCalculateProjectT();       //计算项目表

    int CreateItemUnitT();               //项目单位表
    int CreatePointNumT();               //小数位数表
    int CreateReactionDirT();            //反应方向表
    int CreateTestMethodT();             //分析方法表
    int CreateCalibrateMethodT();        //定标方式表
    int CreateTempHistoryT();            //历史结果数据临时表
    int CreateHistoryT();                //历史结果数据表
    int CreateStrokeParamT();            //行程参数表
    int CreateCalibrateBaseSetT();       //定标基本表
    int CreateCalibrateSolutionT();      //定标液表
    int CreateCalibrateDataT();          //定标数据表
    int CreateCalibrateADDataT();        //定标项目AD数据表

    int CreateQCParamT();                //质控参数表
    int CreateQCLiquidT();               //质控液表

    int CreateQCDataT();                 //质控数据表
    int CreateMotorMoveParamT();         //电机运动参数表
    int CreateOffsetT();                 //偏移步数表


    int CreateErrorT();                  //MV100错误表
    int CreateLISParamT();               //LIS参数表



signals:
    
public slots:
    
};

#endif // SQLITE_H
