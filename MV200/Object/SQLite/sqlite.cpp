#include "sqlite.h"

SQLite *SQLite::_instance = 0;
SQLite::SQLite(QObject *parent) :
    QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("MV200DB.db"); //设置Data库名
    openDB();
}

//实例对象
SQLite * SQLite::getInstance()
{
    if (_instance == 0)
        _instance = new SQLite;
    return _instance;
}

//打开Data库
bool SQLite::openDB()
{
    bool flag = db.open();
    if(flag)
        qDebug() << "Open DB-Succeed!!";
    else
        qDebug() << "Open DB-Fail";
    return flag;
}

//关闭Data库
void SQLite::closeDB()
{
    if(db.open())
        db.close();
}

//用户登陆
int SQLite::Login(QString user , QString pwd)
{
    int userID;
    QString PWD = "";
    QString str = "select pwd, id from UserT where name = '" + user +"'";
    QSqlQuery query(str);
    while(query.next())
    {
        PWD = query.value(0).toString();
        userID = query.value(1).toInt();
    }
    if (PWD == "")
        return 0;        //用户不存在
    else if (PWD != pwd)
        return 2;        //密码错误
    else
    {
        gParameter->setUserID(userID);
        return 1;        //允许用户登录
    }
}

//统计表中的数据总行数
int SQLite::getTableCount(QString str)
{
    int row = 0;
    QSqlQuery query(str);

    while(query.next())
        row = query.value(0).toInt();

    return row;
}

//事物处理
#include <QTime>
bool SQLite::Transaction(QStringList strList)
{
    int count = strList.count();
    if(count == 0) return 0;

    QSqlQuery query("", db);
    bool bsuccess;
    QTime tmpTime;

    db.transaction();// 开始启动事务
    tmpTime.start();
    int i = 0;
    for( i = 0; i<count; i++)
    {
        bsuccess = query.exec(strList.at(i));
        if (!bsuccess)
            break;
    }
    db.commit();// 提交事务，这个时候才是真正打开文件执行SQL语句的时候

    qDebug() << "#####事物处理个数：" << i << count <<" 数据耗时："<<tmpTime.elapsed()<<"ms";
}

//查询Data
QSqlQuery SQLite::Select(QString str)
{
    //qDebug() << str;
    QSqlQuery query(str);
    return query;
}

//插入Data
bool SQLite::Insert(QString str)
{
    //qDebug() << str;
    QSqlQuery query;
    bool flag = query.exec(str);
    if(flag != true)
        qDebug() << "Data Insert-Fail" << str;
    return flag;
}

//删除Data
bool SQLite::Delete(QString str)
{
    //qDebug() << str;
    QSqlQuery query;
    bool flag = query.exec(str);
    if(flag != true)
        qDebug() << "Delete Data-Fail" << str;
    return flag;
}

//修改Data
bool SQLite::Update(QString str)
{
    //qDebug() << str;
    QSqlQuery query;
    bool flag = query.exec(str);
    if(flag != true)
        qDebug() << "Alter Data-Fail" << str;
    return flag;
}

//检查数据库中的表是否存在
bool SQLite::CheckDBTable(QString tableName)
{
    QString str = "select COUNT(*) from sqlite_master where  type='table' and name = '" + tableName +"'";
    QSqlQuery query(str);
    while(query.next())
    {
        return  query.value(0).toBool();
    }
}

//建立表
bool SQLite::CreateTable(QString str)
{
    qDebug() << str;
    QSqlQuery query;
    bool flag = query.exec(str);
    if(flag == true)
        qDebug() << "CreateTable-Succeed!!";
    else
        qDebug() << "CreateTable-Fail";
    return flag;
}

//清空表,并且自增列归零
bool SQLite::CleanTableData(QString table)
{
    bool flag;
    QString str;
    QSqlQuery query;

    //清空数据
    str =  "delete from " + table +" ;";
    flag =  query.exec(str);
    //自增长ID为0
    str = "update sqlite_sequence set seq=0 where name = '" + table + "' ;";
    qDebug() << str;
    flag = query.exec(str);

    if(flag == true)
        qDebug() << "CleanTableData-Succeed!!";
    else
        qDebug() << "CleanTableData-Fail";
    return flag;
}

//删除表
bool SQLite::DropTable(QString table)
{
    qDebug() << "Delete Table:" << table;
    QSqlQuery query;
    bool flag = query.exec("DROP TABLE IF EXISTS '" + table + "'");

    if(flag == true)
        qDebug() << "DropTable-Succeed!!";
    else
        qDebug() << "DropTable-Fail";
    return flag;
}




//每次开机时都对数据库进行检查（常规检查：检查表是否存在）
bool SQLite::initCheckDB()
{
    /**********异常检查**************************************************************************
    ********************************************************************************************
    ***    在程序目录下没有找到数据库文件时，执行这一段程序。
    ***    本函数为异常检查，即在数据库被误删除时，下次开机时执行，并新建各个数据表。
    ***
    ***   各数据表说明参照函数initCheckDB() 中的说明
    ********************************************************************************************/

    //20160803 by hituck
    CreateUserT();                   //用户表
    //CreateUserLoginRecordT();        //用户登录记录表
    CreateSysParamT();               //系统参数表
    CreateStateT();                  //状态表
    CreateTaskT();                   //任务表
    CreateTestTypeT();               //测试类型表
    CreateReactionDiskT();           //反应盘表
    CreateSampleSlotT();             //样本盘表
    CreateReagentT();                //试剂表
    CreateSampleTypeT();             //样本类型表
    CreateProjectT();                //项目表
    CreateCombineProjectT();         //组合项目表
    CreateCalculateProjectT();       //计算项目表

    CreateItemUnitT();              //项目单位表
    CreatePointNumT();              //小数位数表
    CreateReactionDirT();           //反应方向表
    CreateTestMethodT();            //分析方法表
    CreateCalibrateMethodT();       //定标方式表
    CreateTempHistoryT();           //历史结果数据临时表
    CreateHistoryT();               //历史结果数据表

    CreateQCLiquidT();              //质控液表
    CreateQCParamT();               //质控参数表
    //CreateQCDataT();                 //质控数据表


    //    CreateCalibrateBaseSetT();       //定标基本表
    //    CreateCalibrateSolutionT();      //定标液表
    //    CreateCalibrateDataT();          //定标数据表
    //    CreateCalibrateADDataT();        //定标项目AD数据表

    //    CreateStrokeParamT();            //行程参数表
    //    CreateMotorMoveParamT();         //电机运动参数表
    //    CreateOffsetT();                 //偏移步数表

    CreateErrorT();                  //MV100错误表
    CreateLISParamT();               //LIS参数表

    return 1;
}

/*************************************************************************************************************************/

//用户表
int SQLite::CreateUserT()
{
    QString str;
    //检查*用户表*是否存在
    if(!  CheckDBTable("UserT"))
    {
        str = "create table UserT(id INTEGER primary key AUTOINCREMENT not null, name varchar(50), pwd varchar(20));";
        if( CreateTable(str) )  //判断‘用户表’是否建立成功
        {
            str = "insert into UserT(name, pwd) values('root', 'root');";
            Insert(str);
        }
        else  return 0; //返回建表不成功
    }
    else
        qDebug() << "UserT is exist" ;
    return 1;        //返回建表成功
}

//用户登录记录表
int SQLite::CreateUserLoginRecordT()
{
    QString str;
    //检查*用户登录记录表*是否存在
    if(!  CheckDBTable("UserLoginRecordT"))
    {
        str = "create table UserLoginRecordT(id INTEGER primary key AUTOINCREMENT not null, dateTime datetime, remarks varchar(200));";
        if (! CreateTable(str) )//判断‘用户登录记录表’是否建立成功
            return 0;//返回建表不成功
    }
    else
        qDebug() << "UserLoginRecordT is exist" ;
    return 1;//返回建表成功
}

//系统参数表
int SQLite::CreateSysParamT()
{
    QString str;
    if(!  CheckDBTable("SysParamT")) //检查*系统参数表*是否存在
    {
        str = "create table SysParamT(id INTEGER primary key AUTOINCREMENT not null, "
              " CupDirtyAD int, isLiquidAlarm int, lessWaterTimes int, "
              " lessCleanLiquidTimes int, Full_L_WasteTimes int, Full_H_WasteTimes int, "
              "hospitalName varchar(50), isAutoTransfer int, SampleNO int)";
        if( CreateTable(str) )//判断‘系统参数表’是否建立成功
        {
            str = " insert into SysParamT(CupDirtyAD, isLiquidAlarm, lessWaterTimes, lessCleanLiquidTimes, Full_L_WasteTimes, Full_H_WasteTimes, hospitalName, isAutoTransfer, SampleNO) "
                  " values(8000, 1, 10, 10, 10, 10, '', 0, 1); ";
            Insert(str);
        }
        else
            return 0; //返回建表不成功
    }
    else
        qDebug() << "SysParamT is exist" ;
    return 1;//返回建表成功
}

//状态表
int SQLite::CreateStateT()
{
    QString str;
    if(! CheckDBTable("StateT")) //检查*状态表*是否存在
    {
        str = "create table StateT(id INTEGER primary key AUTOINCREMENT not null, name varchar(20), state int, remarks varchar(100));";
        if( CreateTable(str) ) //判断‘状态表’是否建立成功
        {
            str = "insert into StateT (state, name, remarks) values(0, '空闲', 'Idle');";
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(1, '申请', 'Applicated');";
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(2, '等待', 'Wait');";
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(3, '测试中', 'Testing');";
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(4, '加稀释液', 'Diluent');";
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(5, '加样本', 'Sample');";
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(6, '加R1', 'R1');";
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(7, '加稀释样本', 'R_S');";  //稀释后样本
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(8, '加R2', 'R2');";
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(9, '脏杯', 'Dirty');";
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(10, '完成', 'Finished');";
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(11, '清洗', 'Clean');";;
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(12, '错误', 'Error');"; //出错
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(13, '空跑', 'Blank');"; //空跑
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(14, '未完成', 'unFinished');"; //未完成
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(15, '缺少样本', 'noSample');"; //缺少样本
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(16, 'End1', 'End1');"; //End1
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(17, 'End2', 'End2');"; //End2
            Insert(str);
            str = "insert into StateT (state, name, remarks) values(18, '作废', 'BlankOut');"; //取消，作废
            Insert(str);
        }
        else
            return 0; //返回建表不成功
    }
    else
        qDebug() << "StateT is exist" ;
    return 1; //返回建表成功
}

//任务表
int SQLite::CreateTaskT()
{
    QString str;
    if(!  CheckDBTable("TaskT"))   //检查*任务表*是否存在
    {
        str = "create table TaskT(id INTEGER primary key AUTOINCREMENT not null, HistoryT_id int, no varchar(10), dateTime datetime, testTime datetime, "
              " userID int, sampleTypeID int, typeID int, isCalculateProj int, isEmergency int, stateID int, samplePos varchar(20), sampleCode varchar(20),"
              " projectID int, calculateProjID int, QcLiquidID int, isRetest int, solutionID int, samplePosNo int);";
        CreateTable(str);
    }
    else
    {
        qDebug() << "TaskT is exist";

        //重新开机后将“测试中”修改成“未完成”
        str = "update TaskT set stateID=" + QString::number(State_unFINISHED)
                +  " where stateID =" + QString::number(State_TESTING)  +  ";";
        Update(str);
    }
    return 1; //返回建表成功
}

//测试类型表
int SQLite::CreateTestTypeT()
{
    QString str;
    if(!  CheckDBTable("TestTypeT"))    //检查*测试类型表*是否存在
    {
        str = "create table TestTypeT(id INTEGER primary key AUTOINCREMENT not null, name varchar(10), type int, remarks varchar(20));";
        if( CreateTable(str) ) //判断‘测试类型表’是否建立成功
        {
            str = "insert into TestTypeT(name, type, remarks) values('样本测试', 0, 'SampleTest');";//样本测试
            Insert(str);
            str = "insert into TestTypeT(name, type, remarks) values('质控测试', 1, 'QC Test');"; //质控测试
            Insert(str);
            str = "insert into TestTypeT(name, type, remarks) values('定标测试', 2, 'Calibration Test');"; //定标测试
            Insert(str);
        }
        else
            return 0; //返回建表不成功
    }
    else
        qDebug() << "TestTypeT is exist" ;
    return 1; //返回建表成功
}

//反应盘表
int SQLite::CreateReactionDiskT()
{
    QString str;
    if(!  CheckDBTable("ReactionDiskT"))     //检查*反应盘表*是否存在
    {
        str = "create table ReactionDiskT(id INTEGER primary key AUTOINCREMENT not null, isdirty int);";
        if( CreateTable(str) )  //判断‘反应盘表’是否建立成功
        {
            QStringList list;
            for(int i = 0; i < 60; i ++)
            {
                str = "insert into ReactionDiskT(isdirty) values(0);";
                //Insert(str);
                list.append(str);
            }
            Transaction(list);
        }
        else
            return 0; //返回建表不成功
    }
    else
    {
        str = "update ReactionDiskT set isdirty=0;";
        Update(str);
        qDebug() << "ReactionDiskT is exist" ;
    }
    return 1; //返回建表成功
}

//样本盘表
int SQLite::CreateSampleSlotT()
{
    QString str;
    if(!  CheckDBTable("SampleSlotT"))     //检查*样本盘表*是否存在
    {
        str = "create table SampleSlotT(id INTEGER primary key AUTOINCREMENT not null, pos varchar(10), stateID int, isMix int, count int);";
        if( CreateTable(str) ) //判断‘样本盘表’是否建立成功
        {
            //急诊位
            str = "insert into SampleSlotT(pos, stateID, isMix, count) values( '0-1', 0, 0, 0);";
            Insert(str);
            str = "insert into SampleSlotT(pos, stateID, isMix, count) values( '0-2', 0, 0, 0)";
            Insert(str);

            QString pos;
            for(int i = 0; i < 30; i ++) //30普通样本位
            {
                if(i < 10)
                    pos = "1-"+QString::number(i+1);
                else if( i > 9 && i < 20)
                    pos = "2-"+QString::number(i+1-10);
                else if( i > 19 && i < 30)
                    pos = "3-"+QString::number(i+1-20);
                str = "insert into SampleSlotT(pos, stateID, isMix, count) values( '" + pos +"', 0, 0, 0);";
                Insert(str);
            }
        }
        else return 0; //返回建表不成功
    }
    else
    {
        qDebug() << "SampleSlotT is exist" ;
        str = "update SampleSlotT set isMix=0,count=0 where stateID != 3;";
        Update(str);
    }
    return 1; //返回建表成功
}

//试剂表
int SQLite::CreateReagentT()
{
    QString str;
    if(! CheckDBTable("ReagentT"))     //检查*试剂表*是否存在
    {
        str =
                "create table ReagentT(id INTEGER primary key AUTOINCREMENT not null,"
                "name varchar(20), no varchar(20), volume int, usedV int);";
        if( CreateTable(str) )  //判断‘试剂表’是否建立成功
        {
            for( int i = 0; i < REAGENT_COUNT; i ++)
            {
                str = "insert into ReagentT(name, no, volume, usedV) values('-', '-', 0, 0);"; //
                Insert(str);
            }
        }
        else
            return 0;             //返回建表不成功
    }
    else
        qDebug() << "ReagentT is exist!" ;
    return 1; //返回建表成功
}

//样本类型表
int SQLite::CreateSampleTypeT()
{
    QString str;
    if(!  CheckDBTable("SampleTypeT")) //检查*样本类型表*是否存在
    {
        str = "create table SampleTypeT(id INTEGER primary key AUTOINCREMENT not null, type int, name varchar(10), isStir int, remarks varchar(50));";
        if( CreateTable(str) ) //判断‘样本类型表’是否建立成功
        {
            str = "insert into SampleTypeT(type, isStir, name) values(0, 1,'全血');"; //全血
            Insert(str);
            str = "insert into SampleTypeT(type, isStir, name) values(1, 0,'血清');"; //血清
            Insert(str);
            str = "insert into SampleTypeT(type, isStir, name) values(2, 0,'血浆');"; //血浆
            Insert(str);
            str = "insert into SampleTypeT(type, isStir, name) values(3, 0,'尿液');"; //尿液
            Insert(str);
        }
        else  return 0; //返回建表不成功
    }
    else
        qDebug() << "SampleTypeT is exist";
    return 1; //返回建表成功
}

//项目表
int SQLite::CreateProjectT()
{
    QString str;
    if(!CheckDBTable("ProjectT"))     //检查*项目表*是否存在
    {
        str =
                "create table ProjectT(id INTEGER primary key AUTOINCREMENT not null,"
                "no varchar(20), name varchar(20), orderN int, abbreviation varchar(50),  batchNum varchar(50),"
                " isDiluent int, unit varchar(20),"
                " lowLinear double, accuracy int, testMethodID int, reactionDirID int,"
                "r0ID int, r1ID int, r2ID int, sample_V int, r0_V int, r1_V int, rs_V int, r2_V int,"
                "lightPos1 int, lightPos2 int, K double, alarmTimes int, bloodFactor double,"
                "calibrateID int,"
                //定标参数 曲线一
                "calibrate1_R0 double, calibrate1_K double, calibrate1_a double, calibrate1_b double, calibrate1_c double,"
                "calibrate1_RR double, calibrate1_CC double,"
                //定标参数 曲线二
                "isCalibrate2 int, calibrate2_R0 double, calibrate2_K double, calibrate2_a double, calibrate2_b double, calibrate2_c double, "
                "calibrate2_RR double, calibrate2_CC double"
                ");";

        if( !CreateTable(str) )  //判断‘项目表’是否建立成功
            return 0; //返回建表不成功
    }
    else
        qDebug() << "ProjectT is exist";
    return 1; //返回建表成功
}

//组合项目表
int SQLite::CreateCombineProjectT()
{
    QString str;
    if(!CheckDBTable("CombineProjectT"))     //检查*组合项目表*是否存在
    {
        str =
                "create table CombineProjectT(id INTEGER primary key AUTOINCREMENT not null,"
                " nameEn varchar(20), nameCn varchar(20), pMember varchar(200), pID varchar(50)"
                ");";

        if( !CreateTable(str) )  //判断‘组合项目表’是否建立成功
            return 0; //返回建表不成功
    }
    else
        qDebug() << "CombineProjectT is exist";
    return 1; //返回建表成功
}

//计算项目表
int SQLite::CreateCalculateProjectT()
{
    QString str;
    if(!CheckDBTable("CalculateProjectT"))     //检查*计算项目表*是否存在
    {
        str =
                "create table CalculateProjectT(id INTEGER primary key AUTOINCREMENT not null,"
                " nameEn varchar(20), nameCn varchar(20), unit varchar(10), accuracy int,"
                " projectList varchar(200), expression varchar(200), expression_n varchar(100),"
                " rangeA1 double, rangeA2 double, rangeB1 double, rangeB2 double, rangeC1 double, rangeC2 double"
                ");";

        if( !CreateTable(str) )  //判断‘计算项目表’是否建立成功
            return 0; //返回建表不成功
        else
        {
            str = "insert into CalculateProjectT(id, nameEn, nameCn, unit, accuracy, projectList, expression, rangeA1, rangeA2)"
                  " values(1, '01', '01', '01' ,01,'hs-CRP+', '1+1{hs-CRP}', '01', '10');";
            Insert(str);
        }
    }
    else
        qDebug() << "CalculateProjectT is exist";
    return 1; //返回建表成功
}

//项目单位表
int SQLite::CreateItemUnitT()
{
    QString str;
    //检查*项目单位表*是否存在
    if(!  CheckDBTable("ItemUnitT"))
    {
        str = "create table ItemUnitT(id INTEGER primary key AUTOINCREMENT not null, name varchar(10));";
        if( ! CreateTable(str) )//判断‘项目单位表’是否建立成功
            return 0;//返回建表不成功
        else  //??
        {
            str = "insert into ItemUnitT(Name) values('dl/L');";
            Insert(str);
            str = "insert into ItemUnitT(Name) values('umol/L');";
            Insert(str);
            str = "insert into ItemUnitT(Name) values('U/L');";
            Insert(str);
            str = "insert into ItemUnitT(Name) values('mmol/L');";
            Insert(str);
            str = "insert into ItemUnitT(Name) values('g/L');";
            Insert(str);
            str = "insert into ItemUnitT(Name) values('mg/L');";
            Insert(str);
            str = "insert into ItemUnitT(Name) values('IU/ML');";
            Insert(str);
            str = "insert into ItemUnitT(Name) values('mm/L');";
            Insert(str);
        }
    }
    else
        qDebug() << "ItemUnitT is exist" ;
    return 1; //返回建表成功
}

//小数位数表
int SQLite::CreatePointNumT()
{
    QString str;
    if(!  CheckDBTable("PointNumT"))    //检查*小数位数表*是否存在
    {
        str = "create table PointNumT(id INTEGER primary key AUTOINCREMENT not null, num int, name varchar(10));";
        if( CreateTable(str) )//判断‘小数位数表’是否建立成功
        {
            str = "insert into PointNumT(num, name) values(0, '0');";
            Insert(str);
            str = "insert into PointNumT(num, name) values(1, '0.0');";
            Insert(str);
            str = "insert into PointNumT(num, name) values(2, '0.00');";
            Insert(str);
            str = "insert into PointNumT(num, name) values(3, '0.000');";
            Insert(str);
            str = "insert into PointNumT(num, name) values(4, '0.0000');";
            Insert(str);
        }
        else
            return 0;//返回建表不成功
    }
    else
        qDebug() << "PointNumT is exist" ;
    return 1; //返回建表成功
}

//反应方向表
int SQLite::CreateReactionDirT()
{
    QString str;
    if(!  CheckDBTable("ReactionDirT"))     //检查*反应方向表*是否存在
    {
        str = "create table ReactionDirT(id INTEGER primary key AUTOINCREMENT not null, dir int, remarks varchar(100));";
        if( CreateTable(str) )//判断‘反应方向表’是否建立成功
        {
            str = "insert into ReactionDirT(dir, remarks) values(0, 'Up');";//向上
            Insert(str);
            str = "insert into ReactionDirT(dir, remarks) values(1, 'Down');";//向下
            Insert(str);
        }
        else
            return 0;//返回建表不成功
    }
    else
        qDebug() << "ReactionDirT is exist";
    return 1; //返回建表成功
}

//测试方法表
int SQLite::CreateTestMethodT()
{
    QString str;
    //检查*分析方法表*是否存在
    if(! CheckDBTable("TestMethodT"))
    {
        str = "create table TestMethodT(id INTEGER primary key AUTOINCREMENT not null, name varchar(20), remarks varchar(100));";
        if( CreateTable(str) )//判断‘分析方法表’是否建立成功
        {
            str = "insert into TestMethodT(name, remarks) values('Two-Point', '');"; //两点法
            Insert(str);
            str = "insert into TestMethodT(name, remarks) values('End-Point', '');"; //终点法
            Insert(str);
        }
        else
            return 0;//返回建表不成功
    }
    else
        qDebug() << "TestMethodT is exist" ;
    return 1; //返回建表成功
}

//定标方式表
int SQLite::CreateCalibrateMethodT()
{
    QString str;
    if(!  CheckDBTable("CalibrateMethodT"))  //检查*定标方式表*是否存在
    {
        str = "create table CalibrateMethodT(id INTEGER primary key AUTOINCREMENT not null, name varchar(20), remarks varchar(100));";
        if( CreateTable(str) )//判断‘定标方式表’是否建立成功
        {
            str = "insert into CalibrateMethodT(name, remarks) values('Logit-5p', 'Logit-5p');";
            Insert(str);
            str = "insert into CalibrateMethodT(name, remarks) values('Line', 'Linear');";  //线性
            Insert(str);
            str = "insert into CalibrateMethodT(name, remarks) values('Spline', 'Spline');";
            Insert(str);
            str = "insert into CalibrateMethodT(name, remarks) values('Factor', 'Factor');"; //因数
            Insert(str);
        }
        else
            return 0;//返回建表不成功
    }
    else
        qDebug() << "CalibrateMethodT is exist" ;
    return 1; //返回建表成功
}

//历史结果数据临时表
int SQLite::CreateTempHistoryT()
{
    QString str;
    if(! CheckDBTable("TempHistoryT"))   //检查*历史结果数据表*是否存在
    {
        QString strAD_A = " ", strAD_B = " ", strT = " ";
        QString sssss;
        for(int i = 0; i < 50; i ++)
        {
            strAD_A += "AD_A" + QString::number(i +1) + " real, ";
            strAD_B += "AD_B" + QString::number(i +1) + " real, ";
            strT += "T" + QString::number(i +1) + " datetime, ";
            sssss  += "AD_A" + QString::number(i +1) + ", ";
        }

        str = "create table TempHistoryT(id INTEGER primary key AUTOINCREMENT not null, no varchar(20),"
              "T_id int, statusID int, applyTime datetime, inspectionDate datetime,"
              "testDate datetime, reprotDate datetime, userID int, typeID int,"
              " isCalculateProj int, projectID int, calculateProjID int, qcLiquidID int, sampleCode varchar(20),"
              "SamplePos varchar(10), Cup0 varchar(10), Cup1 varchar(10), sampleTypeID int,"
              "isPrint int, isEmergency int, result double, amplitude double, isRetest int,"
              "isModify int, modifyDate datetime, modifierID int, result1 double, lightPos1 int, lightPos2 int, path1 int, path2 int,"
                + strAD_A
                + strAD_B
                + strT +
                "remarks varchar(200));";
        CreateTable(str); //返回建表不成功
    }
    else
    {
        qDebug() << "TempHistoryT is exist" ;
        //重新开机后将“测试中”修改成“未完成”
        str = "update TempHistoryT set statusID=15 where statusID == 4;";
        Update(str);
    }
    return 1; //返回建表成功
}

//历史结果数据表
int SQLite::CreateHistoryT()
{
    QString str;
    if(!  CheckDBTable("HistoryT"))   //检查*历史结果数据表*是否存在
    {
        QString strAD_A = " ", strAD_B = " ", strT = " ";
        for(int i = 0; i < 50; i ++)
        {
            strAD_A += "AD_A" + QString::number(i +1) + " real, ";
            strAD_B += "AD_B" + QString::number(i +1) + " real, ";
            strT += "T" + QString::number(i +1) + " datetime, ";
        }

        str = "create table HistoryT(id INTEGER primary key AUTOINCREMENT not null, no varchar(20), statusID int, applyTime datetime, inspectionDate datetime,"
              "testDate datetime, reprotDate datetime, userID int, typeID int, "
              " isCalculateProj int, projectID int, calculateProjID int, qcLiquidID int, sampleCode varchar(20),"
              " SamplePos varchar(10), Cup0 varchar(10), Cup1 varchar(10), sampleTypeID int,"
              "isPrint int, isEmergency int, result double, amplitude double, isRetest int,"
              "isModify int, modifyDate datetime, modifierID int, result1 double, lightPos1 int, lightPos2 int, path1 int, path2 int,"
                + strAD_A
                + strAD_B
                + strT +
                "remarks varchar(200));";
        if ( ! CreateTable(str) ) return 0; //返回建表不成功
    }
    else
        qDebug() << "HistoryT is exist" ;
    return 1; //返回建表成功
}

//行程参数表
int SQLite::CreateStrokeParamT()
{
    QString str;
    if(!  CheckDBTable("StrokeParamT"))     //检查*行程参数表表*是否存在
    {
        str =
                "create table StrokeParamT(id INTEGER primary key AUTOINCREMENT not null, cleanMachineDownStep int, cleanProbeDownStep int, "
                "cleanMixDownStep int,addSampleDownStep int, absorbSolutionDownStep int, absorbSampleDownStep int, mixSampleDownStep int, "
                "absorbReagentDownStep int, mixSolutionDownStep int, X_Origin_Sample1 int, Y_Origin_Sample1 int, X_Sample1_Sample11 int, Y_Sample1_Sample11 int, "
                "X_Sample1_Sample2 int, Y_Sample1_Sample2 int, X_Origin_Urgent1 int, Y_Origin_Urgent1 int, X_Urgent1_Urgent2 int, "
                "Y_Urgent1_Urgent2 int, X_Origin_Reagent1 int, Y_Origin_Reagent1 int, X_Reagent1_Reagent2 int, Y_Reagent1_Reagent2 int, "
                "X_Reagent1_Reagent7 int, Y_Reagent1_Reagent7 int, X_Origin_AddSamplePos int, Y_Origin_AddSamplePos int, Probe_Mix int);";
        if( CreateTable(str) ) //判断‘行程参数表’是否建立成功
        {
            str =
                    "insert into StrokeParamT("
                    ////"350, 500, 500,"
                    "cleanMachineDownStep, cleanProbeDownStep, cleanMixDownStep, "
                    ////"1800, 1800, 1800, 1800, 400, 1800,"
                    "addSampleDownStep, absorbSolutionDownStep, absorbSampleDownStep, mixSampleDownStep, absorbReagentDownStep, mixSolutionDownStep,"
                    ////"-6700, 1950, 2500, 0, 0, 1950, "
                    "X_Origin_Sample1, Y_Origin_Sample1, X_Sample1_Sample11, Y_Sample1_Sample11, X_Sample1_Sample2, Y_Sample1_Sample2, "
                    ////"-3500, -15600, 2100, 0, "
                    "X_Origin_Urgent1, Y_Origin_Urgent1, X_Urgent1_Urgent2, Y_Urgent1_Urgent2,"
                    ////"2850, -15800, 2200, 0, 0, 3500, "
                    "X_Origin_Reagent1, Y_Origin_Reagent1, X_Reagent1_Reagent2, Y_Reagent1_Reagent2, X_Reagent1_Reagent7, Y_Reagent1_Reagent7,"
                    ////"0, 6120, 4000);";
                    "X_Origin_AddSamplePos, Y_Origin_AddSamplePos, Probe_Mix ) values("
                    "350, 500, 500, "
                    "1800, 1800, 1800, 1800, 400, 1800, "
                    "-6700, 1950, 2500, 0, 0, 1950, "
                    "-3500, -15600, 2100, 0, "
                    "2850, -15800, 2200, 0, 0, 3500, "
                    "0, 6120, 4000);";
            Insert(str);
        }
        else
            return 0; //返回建表不成功
    }
    else
        qDebug() << "StrokeParamT is exist" ;
    return 1; //返回建表成功
}

//定标基本表
int SQLite::CreateCalibrateBaseSetT()
{
    QString str;
    if(!  CheckDBTable("CalibrateBaseSetT"))  //检查*定标基本表*是否存在
    {
        str = "create table CalibrateBaseSetT(id INTEGER primary key AUTOINCREMENT not null, solutionID int, projectID int, concentration real);";
        if( CreateTable(str) )//判断*定标基本表*是否建立成功
            return 1;//返回建表成功
        else
            return 0;//返回建表不成功
    }
    else
        qDebug() << "CalibrateBaseSetT is exist";
    return 1; //返回建表成功
}
//定标液表
int SQLite::CreateCalibrateSolutionT()
{
    QString str;
    if(!  CheckDBTable("CalibrateSolutionT"))  //检查*定标液表*是否存在
    {
        str = "create table CalibrateSolutionT(id INTEGER primary key AUTOINCREMENT not null, name varchar(50), remarks varchar(200));";
        if( CreateTable(str) )//判断*定标液表*是否建立成功
            return 1;//返回建表成功
        else
            return 0;//返回建表不成功
    }
    else
        qDebug() << "CalibrateSolutionT is exist" ;
    return 1; //返回建表成功
}
//定标数据表
int SQLite::CreateCalibrateDataT()
{
    QString str;
    if(!  CheckDBTable("CalibrateDataT"))  //检查*定标数据表*是否存在
    {
        str = "create table CalibrateDataT(id INTEGER primary key AUTOINCREMENT not null, calibrateNo int, "
              " calibrateDate datetime, projectID int,  solutionID int, concentration real, amplitude real, "
              " factors real, r0 real, k real, a real, b real, c real, d real, SD real, coefficient real, sensitivity real, "
              " repeatability1 real, repeatability2 real, repeatability3 real, isCalibration int, isEdited int, remarks varchar(200));";
        if( CreateTable(str) )//判断*定标数据表*是否建立成功
            return 1;//返回建表成功
        else
            return 0;//返回建表不成功
    }
    else
        qDebug() << "CalibrateDataT is exist" ;
    return 1; //返回建表成功
}
//定标项目AD数据表
int SQLite::CreateCalibrateADDataT()
{
    QString str;
    if(!  CheckDBTable("CalibrateADDataT"))  //检查*定标项目AD数据表*是否存在
    {
        QString strAD_A = " ", strAD_B = " ", strT = " ";
        for(int i = 0; i < 50; i ++)
        {
            strAD_A += "AD_A" + QString::number(i +1) + " real, ";
            strAD_B += "AD_B" + QString::number(i +1) + " real, ";
            strT += "T" + QString::number(i +1) + " datetime, ";
        }

        str = "create table CalibrateADDataT (id INTEGER primary key AUTOINCREMENT not null, "
              " calibrateNo int, no int, calibrateTime datetime, projectID int, solutionID int, concentration real, amplitude real, "
              " lightPos1 int, lightPos2 int, pathA int, pathB int, "
                + strAD_A+strAD_B + strT +" remarks varchar(200))";
        if( CreateTable(str) )//判断*定标项目AD数据表*是否建立成功
            return 1;//返回建表成功
        else
            return 0;//返回建表不成功
    }
    else
        qDebug() << "CalibrateADDataT is exist" ;
    return 1; //返回建表成功
}

//质控参数表
int SQLite::CreateQCParamT()
{
    QString str;
    if(!  CheckDBTable("QCParamT"))  //检查*质控参数表*是否存在
    {
        str = "create table QCParamT(id INTEGER primary key AUTOINCREMENT not null, "
              "QCLiquidID int, projectID int, projectName varchar(20), targetValue real, "
              "SD real, TargetValue0 real, TargetValue1 real, remarks varchar(100));";
        if( CreateTable(str) )//判断*质控参数表*是否建立成功
            return 1;//返回建表成功
        else
            return 0;//返回建表不成功
    }
    else
        qDebug() << "QCParamT is exist" ;
    return 1; //返回建表成功
}
//质控液表
int SQLite::CreateQCLiquidT()
{
    QString str;
    if(!  CheckDBTable("QCLiquidT"))  //检查*质控液表*是否存在
    {
        str = "create table QCLiquidT(id INTEGER primary key AUTOINCREMENT not null, name varchar(50), "
              "sampleType int, batchNo varchar(20), validDate datetime, barCode varchar(30), remarks varchar(200));";
        if( CreateTable(str) )//判断*质控液表*是否建立成功
        {
            str = "insert into QCLiquidT(id, sampleType, batchNo, validDate, barCode)"
                  " values(1, 1, '000', '2017/01/01' ,'000');";
            Insert(str);
            return 1;//返回建表成功
        }
        else
            return 0;//返回建表不成功
    }
    else
        qDebug() << "QCLiquidT is exist" ;
    return 1; //返回建表成功
}



//质控数据表
int SQLite::CreateQCDataT()
{
    QString str;
    QString strAD_A = " ", strAD_B = " ", strT = " ";
    for(int i = 0; i < 50; i ++)
    {
        strAD_A += "AD_A" + QString::number(i +1) + " real, ";
        strAD_B += "AD_B" + QString::number(i +1) + " real, ";
        strT += "T" + QString::number(i +1) + " datetime, ";
    }

    if(!  CheckDBTable("QCDataT"))  //检查*质控数据表*是否存在
    {
        str = "create table QCDataT(id INTEGER primary key AUTOINCREMENT not null,applicateTime datetime, "
              " QCTime datetime,testTime datetime, reportTime datetime,no varchar(20), QCNo varchar(20), "
              " projectID int,concentration real, amplitude real, lightPos1 int, lightPos2 int, pathA int, pathB int,"
                + strAD_A+strAD_B + strT +" remarks varchar(200));";
        if( CreateTable(str) )//判断*质控数据表*是否建立成功
            return 1;//返回建表成功
        else
            return 0;//返回建表不成功
    }
    else
        qDebug() << "QCDataT is exist" ;
    return 1; //返回建表成功
}



//电机运动参数表
int SQLite::CreateMotorMoveParamT()
{
    QString str;
    //检查*电机运动参数表*是否存在
    if(! CheckDBTable("MotorMoveParamT"))
    {
        str = "create table MotorMoveParamT( id INTEGER primary key AUTOINCREMENT not null, Type int, MotorAddr int,"
              "AccStageSpeed int, AccStageSteps int,"
              "DecStageSpeed int, DecStageSteps int,"
              "WorkSpeedAddr int, TtlMoveSteps int,"
              "MaxMoveSteps int, ResetDelayTime int,"
              "ForwardOffSet int, ReverseOffSet int,"
              "WorkMode int, remarks varchar(100) );";

        if( CreateTable(str) )//判断‘电机运动参数表’是否建立成功
        {
            //插入初始化数据
            return 1; //返回建表成功
        }
        else  return 0;//返回建表不成功
    }
    else
        qDebug() << "MotorMoveParamT is exist" ;
    return 2; //返回'电机运动参数表'已经存在
}

//偏移步数表
int SQLite::CreateOffsetT()
{
    QString str;
    //检查*电机运动参数表*是否存在
    if(! CheckDBTable("OffsetT"))
    {
        str = "create table OffsetT( id INTEGER primary key AUTOINCREMENT not null, "
              "X_Forward int, X_Reverse int, Y_Forward int, Y_Reverse int, ProbeOrigin int, StirRodOrigin int,"
              "CleanMachineOrigin int, LightingDelayTime int, CupOffset int,"
              "ReagentOffset int, SampleOffset int, SolutionOffset_absorb int, SolutionOffset_inject int,"
              "remarks varchar(100));";

        if( CreateTable(str) )//判断‘电机运动参数表’是否建立成功
        {
            //插入初始化数据
            str =
                    "insert into OffsetT("
                    "X_Forward,X_Reverse,Y_Forward,Y_Reverse,ProbeOrigin,StirRodOrigin,CleanMachineOrigin, LightingDelayTime, CupOffset,"
                    "ReagentOffset, SampleOffset, SolutionOffset_absorb, SolutionOffset_inject"
                    ") values("
                    "850, 1050, 450, 650, 400, 400, 200, 1, 15,"
                    "150, 50, 200, 10"
                    ");";
            Insert(str);

            return 1; //返回建表成功
        }
        else  return 0;//返回建表不成功
    }
    else
        qDebug() << "OffsetT is exist" ;
    return 2; //返回'电机运动参数表'已经存在
}

//MV100错误表
int SQLite::CreateErrorT()
{
    QString str;
    //检查*MV100错误表*是否存在
    if(! CheckDBTable("ErrorT"))
    {
        str = "create table ErrorT( id INTEGER primary key AUTOINCREMENT not null, "
              "dateTime datetime, errID int, errNo int, details varchar(200) );";

        if( CreateTable(str) )//判断‘MV100错误表’是否建立成功
            return 1; //返回建表成功
        else  return 0;//返回建表不成功
    }
    else
        qDebug() << "ErrorT is exist" ;
    return 2; //返回'MV100错误表'已经存在
}

//LIS参数表
int SQLite::CreateLISParamT()
{
    QString str;
    //检查*LIS参数表*是否存在
    if(! CheckDBTable("LISParamT"))
    {
        str = "create table LISParamT( id INTEGER primary key AUTOINCREMENT not null, "
              " isAutoStart int, IP varchar(20), SubnetMask varchar(20), GateWay varchar(20), Agreement int, LIS_IP  varchar(20), LISPort int, ACKTime int, remarks  varchar(200));";
        if( CreateTable(str) )//判断‘LIS参数表’是否建立成功
        {
            str = "insert into LISParamT(isAutoStart, IP, SubnetMask, GateWay, Agreement, LIS_IP, LISPort, ACKTime)"
                  " values(0, '192.168.1.10', '200', '0.0.0.0', 1, '192.168.1.88', 100, 100 );";
            Insert(str);
            return 1; //返回建表成功
        }
        else  return 0;//返回建表不成功
    }
    else
        qDebug() << "LISParamT is exist" ;
    return 2; //返回'LIS参数表'已经存在
}

