#include "Scheduler.h"
#include <QDebug>
#include "LISCommunicateThread.h"

GScheduler * gScheduler;
GScheduler::GScheduler(QThread *parent)
    : QThread(parent)
{
    gMVFrame = new MVFrame;  //指令参数类

    WorkModeFlag = MV_State_unChecked; //工作模式状态标志=开机未自检

    //开机/复位初始化参数
    initParam();

    //周期定时器
    CycleTimer = new QTimer;
    connect(CycleTimer, SIGNAL(timeout()), this, SLOT(slt_CycleTimer_timeout()));
    connect(this, SIGNAL(sig_CycleTimer_start(int)), CycleTimer,SLOT(start(int)));
    connect(this, SIGNAL(sig_CycleTimer_stop()), CycleTimer,SLOT(stop()));

    //
    connect(this, SIGNAL(sig_TcpServer_Write(QString)), gTcpServer, SLOT(slt_Send(QString)));
    connect(this, SIGNAL(sig_TcpServer_Write(char*, int)), gTcpServer, SLOT(slt_Send(char*, int)));
    connect(this, SIGNAL(sig_ConnectTCP(QString, int)), gTcpServer, SLOT(slt_TCP_Connect(QString, int)));
    connect(this, SIGNAL(sig_disConnectTCP()), gTcpServer, SLOT(slt_TCP_disConnect()));
}

//开机/复位初始化参数
void GScheduler::initParam()
{
    EmptySampleCount = 0;
    EmptyCycleTimes = 0;
    CurrentSampleSlot = "1";    //当前测试的样本架的编号（用于判断是前后测试是否在同一样本架上）
    isCurrentSampleNeedMix= 0;
    CurrentMixSampleSlot = 0;     //当前搅拌位的样本架
    CurrentMixSampleNo = 0;       //当前搅拌位的样本位
    CurrentMixSamplePos = "";  //当前搅拌的样本位置
    ChangedSampleSlot = CHANGESAMPLESLOT;
    isNextSampleExist = 1;
    NewTestInTeam = 0;
    CurrentSamplePos = "";


    isBootCleanedFlag = false; //开机是否已经清洗了反应杯
    m_BootCleanedTimes = 0;    //开机清洗的反应杯个数

    CurrentCleaningCupNo = 0;  //当前清洗的反应杯编号（从0开始计数）
    m_UsedCup = 1;             //记录反应杯的使用情况
    m_TaskPt = -1;              //记录队列中新增测试在任务队列中的位置
    for(int i = 0; i < TASKCOUNT; i ++)
    {
        TaskList[i].Status = 0;
        TaskList[i].AD_Count = 0;
        TaskList[i].isAdded = false;
        TaskList[i].SamplePos = "";
    }

    //最后一个测试加完R2后还要跑的空周期次数
    m_BlankCycleCount = BLANKCYCLE_R2;

}

//机器是否正在运行中（即1：普通测试，质控测试，定标测试，暂停。   0：其他工作状态）
int GScheduler::get_isRunning()
{
    if( WorkModeFlag == MV_State_Normal || WorkModeFlag == MV_State_QC
            || WorkModeFlag == MV_State_CalibrateT || WorkModeFlag == MV_State_Pause)
        return 1;
    else
        return 0;
}

//工作模式状态标志
int GScheduler::getWorkModeFlag() { return WorkModeFlag; }
void GScheduler::setWorkModeFlag(int flag) { WorkModeFlag = flag; }

//查询“任务队列”中pt位置的任务信息
Task GScheduler::getTaskList(int pt) { return TaskList[pt]; }

//开机是否已经清洗了反应杯
int GScheduler::get_isBootCleanedFlag()  {return isBootCleanedFlag;}

//是否跑周期指令
bool GScheduler::get_isCycleFlag() {return isCycleFlag;}

//测试完成（即采光采完成）
void GScheduler::FinishedTest(int pt)
{
    if( TaskList[pt].Status ==1 )
    {
        TaskList[pt].AD_Count = 0; //采光次数
        TaskList[pt].Status = 0;   //状态：（0:空闲， 1:任务测试）
        TaskList[pt].SamplePos = "";
        TaskList[pt].isAdded = false;

        //更新反应杯状态
        emit sig_setCupStatus(TaskList[pt].Cup[0], "End2");
        emit sig_setCupStatus(TaskList[pt].Cup[1], "End2");
    }
}


//测试周期
int GScheduler::ActionStart() {return setMotionType(0);}
//开机初始化（复位）
int GScheduler::InitialMachine()  {return setMotionType(1);}
//开始测试
int GScheduler::TaskStart()
{
    qDebug() << "MV200 WorkModeFlag:" << WorkModeFlag;
    //MV100工作状态
    if( WorkModeFlag == MV_State_Normal || WorkModeFlag == MV_State_QC || WorkModeFlag == MV_State_CalibrateT ) //测试正在进行中，返回
    {
        return 0;
    }
    else if( WorkModeFlag == MV_State_Stop )
    {
        //isCycleFlag = 1;       //是否跑周期指令
        WorkModeFlag = MV_State_Normal;  //继续加样
        return 1;
    }
    else if( WorkModeFlag == MV_State_Pause )
    {
        //isCycleFlag = 1;   //是否跑周期指令
        WorkModeFlag = MV_State_Normal;  //继续加样
        return 1;
    }
    else
    {
        isCycleFlag = 1;   //是否跑周期指令
        return setMotionType(2);
    }
}
//读Mifare卡片（空闲才能读卡）
int GScheduler::StartReadCard()  {return setMotionType(3);}
//重置中位机服务器IP
int GScheduler::ResetServerIP(QString ip) {ServerIP = ip;return setMotionType(4);}
//设置调用分支
int GScheduler::setMotionType(int type)
{
    if(this->isRunning())
    {
        qDebug() << "****GScheduler is running...";
        return 0; //线程工作中(未执行)
    }

    m_MotionFinished=0;
    m_MotionType = type;
    this->start();

    return 1;
}

void GScheduler::run()
{
    m_MotionFinished = 0;//工作中

    switch(m_MotionType)
    {
    case 0:  //测试周期
        Action();
        break;
    case 1:          //开机初始化
        initialMachine();
        break;
    case 2:          //开始测试
        taskStart();
        break;
    case 3:          //读Mifare卡片（空闲才能读卡）
        readCard();
        break;
    case 4:          //重置中位机服务器IP
        resetServerIP();
        break;
    }

    m_MotionFinished = 1;//工作完成
}

//槽：周期定时器计时
void GScheduler::slt_CycleTimer_timeout()
{
    QDateTime time = QDateTime::currentDateTime();               //获取系统现在的时间
    QString date = time.toString("[yyyy-MM-dd hh:mm:ss.zzz] ");  //设置显示格式
    qDebug() << "";
    qDebug() << "#########################################################################################:" << date << "WorkMode:" << WorkModeFlag;
    //qDebug() << "****************************************************************************:" << date << "WorkMode:" << WorkModeFlag;
    if( gTcpServer->get_isCanSendNextFrame_Flag() == 1 )
    {
        if( isBootCleanedFlag ) //开机未清洗反应杯
            emit sig_CycleTimer_start(gParameter->getCycleTime());
        ActionStart();
        //Action();
    }
    else
    {
        qDebug() << "slt_CycleTimer_Stop..." << "3";
        WorkModeFlag = MV_State_Error;
        emit sig_CycleTimer_stop();
    }
}

//槽：停止周期定时器计时（意外停机）
void GScheduler::slt_CycleTimer_Stop()
{
    qDebug() << "停止周期定时器计时...";
    emit sig_CycleTimer_stop();
}

//槽：暂停
void GScheduler::slt_Suspend()
{
    //MV100工作状态
    if( WorkModeFlag == MV_State_Normal || WorkModeFlag == MV_State_QC || WorkModeFlag == MV_State_CalibrateT )
        WorkModeFlag = MV_State_Pause;
}

//槽：停止
void GScheduler::slt_Stop()
{
    //MV100工作状态
    if( WorkModeFlag == MV_State_Normal || WorkModeFlag == MV_State_QC
            || WorkModeFlag == MV_State_CalibrateT || WorkModeFlag == MV_State_Pause)
    {
        qDebug() << "slt_CycleTimer_Stop..." << "1 槽：停止";
        isCycleFlag = 0;       //是否跑周期指令
        WorkModeFlag = MV_State_Stop;
        emit sig_CycleTimer_stop(); //停止周期定时器计时
    }
}

//开机初始化
void GScheduler::initialMachine()
{
    if( CycleTimer->isActive() )
        CycleTimer->stop();

    QTime time;
    time.start();
    msleep(200);
    emit sig_initMachine_Step( gParameter->getMachineName() + "系统初始化。。。");   //信号：开机初始化的步骤-XX200系统初始化。。。
    msleep(1000);

    //打开TCP-Server
    if(1)
    {
        QString ip = gParameter->getTCPServerIP().trimmed();
        int port = gParameter->getTCPServerPort();
        emit sig_ConnectTCP(ip, port);  //连接服务器

        int waitTimes = 0;
        while(1)  //等待中位机复位完成
        {
            msleep(1000);
            if( !gTcpServer->isTcpConnected() )
                waitTimes ++;
            else
            {
                qDebug() << "连接中位机服务器成功！";
                emit sig_initMachine_Step("连接中位机服务器成功！");   //信号：开机初始化的步骤-连接中位机成功！
                msleep(1000);
                gTcpServer->StartMonitor(); //开始监控网口返回数据
                break;
            }
            //是否超时
            if( waitTimes >= 5 )  //5秒连接成功即连接失败
            {
                qDebug() << "连接中位机服务器失败！！";
                emit sig_initMachine_Step("连接中位机服务器失败！！");   //信号：开机初始化的步骤-连接中位机失败
                msleep(2000);
                emit sig_Finished_init();
                return;
            }
        }
    }

    //是否要开机自检（复位）
    bool isReset;
    if( gParameter->get_isSelfCheck() )
    {
        WorkModeFlag = MV_State_BootReset; //工作模式状态标志=开机复位

        //
        msleep(300);
        emit sig_initMachine_Step("初始化错误表...");   //信号：开机初始化的步骤-初始化错误表
        if( gMV_ErrTable->get_initErrTableState() )
        {
            msleep(300);
            emit sig_initMachine_Step("初始化错误表完成！");
        }
        else //
        {
            WorkModeFlag = MV_State_unChecked ; //MV100工作状态
            msleep(1000);
            emit sig_initMachine_Step("初始化错误表不成功！");
            sleep(2);
            emit sig_initMachine_Step("跳过系统复位，即将进入主界面...");   //信号：开机初始化的步骤-样本搅拌杆复位
            msleep(3000);
            emit sig_Finished_init();
            return;
        }

        //样本搅拌杆复位 (样本针、样本搅拌复位到无动作干涉)
        isReset = reset(gMVFrame->getResetSampleMix(), LENGHT_RESETSAMPLEMIX, "样本搅拌杆复位", 8);
        //系统分步自检
        isReset = reset(gMVFrame->getResetCmdFrame(), LENGHT_RESET, "系统分步自检", 15);

        /////各种液路灌注////////////
        if(1)
        {
            //1.灌注样本针液路
            isReset = reset(gMVFrame->getPerfuse_sNeedle(5), LENGHT_PERFUSE_SN, "灌注样本针液路", 10);
            //2.灌注试剂针液路
            isReset = reset(gMVFrame->getPerfuse_rNeedle(5), LENGHT_PERFUSE_RN, "灌注试剂针液路", 10);
            //3.灌注清洗针液路 （用清洗反应杯指令来替代）
            //isReset = reset(gMVFrame->getPerfuse_cNeedle(3), LENGHT_PERFUSE_CN, "灌注清洗针液路", 10); //目前不能用
            int isTurn = 0; //1:反应盘转动  0:反应盘不转动
            for(int i = 0; i < 3; i ++)
            {
                isReset = reset(gMVFrame->getCleamCupCmd(isTurn), LENGHT_CLEANCUP, "灌注清洗针液路", 15);
                msleep(1000);
            }
            //4.灌注搅拌液路
            isReset = reset(gMVFrame->getPerfuse_Mix(5), LENGHT_PERFUSE_M, "灌注搅拌液路", 10);
            //5.灌注样本搅拌液路
            isReset = reset(gMVFrame->getPerfuse_sMix(5), LENGHT_PERFUSE_SM, "灌注样本搅拌液路", 10);
        }

        //以上机构是否完成
        if( !isReset ) //未复位成功
        {
            WorkModeFlag = MV_State_unChecked ; //MV100工作状态
            msleep(1000);
            emit sig_initMachine_Step("系统初始化不成功，即将进入主界面...");   //信号：开机初始化的步骤-样本搅拌杆复位
            msleep(3000);
            emit sig_Finished_init();
            return;
        }
        else
        {
            WorkModeFlag = MV_State_IDLE ; //MV100工作状态
        }

    }
    else
        WorkModeFlag = MV_State_unChecked ; //MV100工作状态

    msleep(2000);
    emit sig_Finished_init();
    qDebug() << "开机初始化 initialMachine time:" << time.elapsed() << "状态：" << WorkModeFlag << isReset;;
}

//复位
bool GScheduler::reset(char *cmd, int lenght, QString str, int waitTime)
{
    bool isReset = false;
    int times;
    if( gTcpServer->get_isCanSendNextFrame_Flag() == 1)
    {
        qDebug()<< "";
        msleep(500);
        emit sig_initMachine_Step(str+"...");   //信号：开机初始化的步骤-样本搅拌杆复位
        emit sig_TcpServer_Write(cmd, lenght);    //信号：写数据
        times = 0;
        isReset = false;
        while(1)  //等待中位机复位完成
        {
            sleep(1);
            if( gTcpServer->get_isCanSendNextFrame_Flag() == 0 )
                times ++;
            else if( gTcpServer->get_isCanSendNextFrame_Flag() == 1)
            {
                isReset = true;
                emit sig_initMachine_Step(str + "完成!");   //信号：开机初始化的步骤-系统复位失败
                break;
            }
            else //当前指令执行有错误返回 & 当前指令超时
            {
                isReset = false;
                emit sig_initMachine_Step( str +"超时!!");   //信号：开机初始化的步骤-系统复位失败
                break;
            }

            if( times == waitTime )  //waitTime秒没有复位成功
            {
                qDebug() << str << "waitTimes:" << times;
                isReset = false;
                emit sig_initMachine_Step( str +"失败!!");   //信号：开机初始化的步骤-系统复位失败
                break;
            }
        }
    }
    return isReset;
}

//读卡（读S50卡上16个扇区全部的数据）
void GScheduler::readCard()
{
    WorkModeFlag = MV_STATE_ReadCard;
    char *cmd;
    int times;

    //读第一块数据等待用户将卡片放置到读卡区
    qDebug() << "检测卡片。。。。。。";
    int testTimes = 0;
    while(1)
    {
        times = 0;
        cmd = gMVFrame->getReadCardCmd(0);
        emit sig_TcpServer_Write(cmd, LENGHT_READCARD);    //信号：写数据
        msleep(100);
        while(1)  //等待中位机复位完成
        {
            msleep(50);
            if( gTcpServer->get_isCanSendNextFrame_Flag() == 0 ) times ++;
            else if( gTcpServer->get_isCanSendNextFrame_Flag() == 1)  break;
            else return;//当前指令执行有错误返回 & 当前指令超时

            if( times == 40 )  //800毫秒没有结果返回，即可能是通讯出错
            {
                qDebug() << "times:" << times;
                return;
            }
        }

        //收到扇区内的有效数据（跳出while循环，开始读卡）
        if( gTcpServer->get_isDataReceived() )
        {
            qDebug() << "检测到卡片在读卡区中!";
            break;
        }

        msleep(100);
        testTimes ++;
        if(testTimes == 10)
        {
            qDebug() << "没有有效卡在读卡区内！！！！";
            emit sig_ReadCard_State(NO_TAG_ERR);
            return;
        }
    }

    //开始读卡
    qDebug() << endl << "开始读卡。。。。。。";
    int SectorCount = 10; //目前只读10个扇区
    gTcpServer->clearReadCardData();
    msleep(100);
    for(int i = 0; i < SectorCount; i ++)
    {
        qDebug() << "第" << i << "扇区...";
        times = 0;
        cmd = gMVFrame->getReadCardCmd(i);
        emit sig_TcpServer_Write(cmd, LENGHT_READCARD);    //信号：写数据
        msleep(20);
        while(1)  //等待中位机复位完成
        {
            msleep(50);
            if( gTcpServer->get_isCanSendNextFrame_Flag() == 0 ) times ++;
            else if( gTcpServer->get_isCanSendNextFrame_Flag() == 1)
            {
                emit sig_ReadCardProgressBar_ValueChanged(i+1);
                break;
            }
            else //当前指令执行有错误返回 & 当前指令超时
            {
                qDebug() << "读卡出错啦！！！！！";
                return;
            }

            if( times == 40 )  //800毫秒没有结果返回，即可能是通讯出错
            {
                qDebug() << "waitTimes:" << times;
                return;
            }
        }

        //未收到扇区内的有效数据
        if( !gTcpServer->get_isDataReceived() )
            break;
        msleep(50);
    }

    //qDebug() << "卡内数据:"<< gTcpServer->getReadCardData();
    emit sig_ReadCard_Data(gTcpServer->getReadCardData().right( gTcpServer->getReadCardData().count()-32) );
    gTcpServer->clearReadCardData();
}

//重置中位机服务器IP
void GScheduler::resetServerIP()
{
    bool isReset = false; //是否修改成功

    if( gTcpServer->isTcpConnected() )
    {

        int ip1 = ServerIP.split(".").at(0).toInt();
        int ip2 = ServerIP.split(".").at(1).toInt();
        int ip3 = ServerIP.split(".").at(2).toInt();
        int ip4 = ServerIP.split(".").at(3).toInt();

        if(1)
        {
            msleep(500);
            int times = 0;
            char *cmd = gMVFrame->getSetServerIP(0, ip1, ip2);
            emit sig_TcpServer_Write(cmd, LENGHT_RESET_IP);    //信号：写数据
            while(1)  //等待中位机复位完成
            {
                msleep(100);
                if( gTcpServer->get_isCanSendNextFrame_Flag() == 0 )
                    times ++;
                else if( gTcpServer->get_isCanSendNextFrame_Flag() == 1)
                {
                    isReset = true;
                    break;
                }
                else //当前指令执行有错误返回 & 当前指令超时
                {
                    isReset = false;
                    break;
                }

                if( times == 5 )  //500毫秒没有结果返回，即可能是通讯出错
                {
                    qDebug() << "waitTimes:" << times;
                    isReset = false;
                    break;
                }
            }
        }

        //前面的ip是否设置成功
        if(isReset && gTcpServer->get_isCanSendNextFrame_Flag()==1)
        {
            msleep(500);
            int times = 0;
            char *cmd = gMVFrame->getSetServerIP(1, ip3, ip4);
            emit sig_TcpServer_Write(cmd, LENGHT_RESET_IP);    //信号：写数据
            while(1)  //等待中位机复位完成
            {
                msleep(100);
                if( gTcpServer->get_isCanSendNextFrame_Flag() == 0 )
                    times ++;
                else if( gTcpServer->get_isCanSendNextFrame_Flag() == 1)
                {
                    isReset = true;
                    break;
                }
                else //当前指令执行有错误返回 & 当前指令超时
                {
                    isReset = false;
                    break;
                }

                if( times == 5 )  //500毫秒没有结果返回，即可能是通讯出错
                {
                    qDebug() << "waitTimes:" << times;
                    isReset = false;
                    break;
                }
            }
        }
        else
        {
            isReset = false;
        }

    }

    //最终IP是否修改成功
    if(isReset)
    {
        emit sig_ResetIP(1); //信号：修改中位机IP是否成功（0：失败， 1：成功）
        qDebug() << "修改IP成功：" << ServerIP;
    }
    else
        emit sig_ResetIP(0); //信号：修改中位机IP是否成功（0：失败， 1：成功）
}

//开始测试
void GScheduler::taskStart()
{
    AddedSampleCup = 0;
    msleep(50);
    if( !isBootCleanedFlag ) //开机未清洗反应杯
    {
        qDebug() << "开始测试.....";
        //判断第一个测试的样本是否需要搅拌
        QString str  =
                " select TaskT.id, SampleTypeT.isStir, samplePos from TaskT "
                //连接方式(项目表、样本类型表)
                " Inner join SampleTypeT ON TaskT.sampleTypeID = SampleTypeT.id "
                //只查找未完成状态的任务（待测试）
                " where TaskT.stateID = " + QString::number(State_WAIT) + " "
                //排序查询：紧急否降序＋样本号升序＋是否搅拌排序（同一样本所申请的测试按样本不需要搅拌的先进行，然后再测要搅拌的项目）
                " order by TaskT.SamplePosNo asc "
                //每次取出的已经申请的测试个数
                " limit 0, 1;";

        QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
        while(query.next())
        {
            isCurrentSampleNeedMix = query.value(1).toInt();
            CurrentMixSamplePos = query.value(2).toString();
            CurrentMixSampleSlot = CurrentMixSamplePos.split("-").at(0).toInt();     //当前搅拌位的样本架
            CurrentMixSampleNo = CurrentMixSamplePos.split("-").at(1).toInt();       //当前搅拌位的样本位
        }
        qDebug() << "开始测试 CurrentMixSamplePos：" << CurrentMixSamplePos << "isCurrentSampleNeedMix:" << isCurrentSampleNeedMix;

        //更换样本架(默认)
        CurrentMixSampleNo = CurrentMixSampleNo;//+SAMPLESLOT_COUNT;

        WorkModeFlag = MV_State_Normal;  //工作模式状态标志=普通样本测试
        emit sig_CycleTimer_start(CLEANCUP_TIME);
        Action();
    }
    else //
    {
        if( getTaskInfo() )
        {
            qDebug() << "开始测试2.....";
            WorkModeFlag = MV_State_Normal;  //工作模式状态标志=普通样本测试
            emit sig_CycleTimer_start(gParameter->getCycleTime());
            Action();
        }
        else
        {
            qDebug() << "slt_CycleTimer_Stop..." << "2  taskStart...";
            WorkModeFlag = MV_State_IDLE;  //工作模式状态标志=空闲状态（开机自检完成）
            emit sig_CycleTimer_stop();
            qDebug() << "*****CycleTimer:" << CycleTimer->isActive();
        }
    }
}

//分配反应杯前都要检查反应杯是否清洗干净(0:正常，1:脏杯)
int GScheduler::CheckCupStatus(int cNo)
{
    //每次都检查两个反应杯（杯1， 杯2）

    //测试阶段-返回正常
    return 0;
}

//从数据库中取出测试数据
int GScheduler::getTaskInfo()
{
    //暂停状态（即将正在进行的测试正常进行，等待状态的测试不进行加样）
    if( WorkModeFlag >= MV_State_Pause )
        return 0;

    //数据库中是否有已经申请且未测试的任务
    int value = 0;
    QString sampleCode;   //样本条码
    //任务id，项目d，测试类型，样本类型，急诊否，样本搅拌否，稀释否
    int id, projectID, testType, sampleMode, isEmergency, isStirSample, isMixCurrentSample, isDiluent;
    //样本编号，稀释液编号，R1编号，R2编号，样本量，稀释液量，R1量，稀释后样本量，R2量
    int R0, R1, R2, SampleV, R0V, R1V, RSV, R2V;
    //测光点1，测光点2
    int lightPos1, lightPos2, isRetest;
    QString taskNo, dateTime, SamplePos;  //测试编号、申请时间、样本位置
    int isCalculateProj; //计算项目否;
    int QcLiquidID; //质控液ID

    //从数据库中取出用户申请的测试信息，并存入循环队列，等待执行
    QString str  =
            //任务id，状态，测试编号，申请时间，项目id，测试类型，样本类型，样本条码，
            " select TaskT.id, stateID, TaskT.no, dateTime, projectID, typeID, sampleTypeID, sampleCode, "
            //急诊否，样本搅拌否，稀释否，样本编号，稀释液编号，R1编号，R2编号，样本量，稀释液量，R1量，稀释后样本量，R2量
            " isEmergency, SampleTypeT.isStir, ProjectT.isDiluent, samplePos, r0ID, r1ID, r2ID, sample_V, r0_V, r1_V, rs_V, r2_V, "
            //测光点1，测光点2, 重测否, 计算项目否
            " ProjectT.lightPos1, ProjectT.lightPos2, isRetest, isCalculateProj, calculateProjID, QcLiquidID "
            " from TaskT "
            //连接方式(任务队列表、项目表)
            " Inner join ProjectT ON TaskT.projectID = ProjectT.id "
            //连接方式(表、计算项目表)
            " Inner join CalculateProjectT ON TaskT.calculateProjID = CalculateProjectT.id "
            //连接方式(项目表、样本类型表)
            " Inner join SampleTypeT ON TaskT.sampleTypeID = SampleTypeT.id "
            //只查找未完成状态的任务（待测试）
            " where TaskT.stateID = 3 "
            //是否缺样本、试剂
            //+ lackReagentSample +
            //排序查询：
            " order by TaskT.SamplePosNo asc, ProjectT.orderN asc, TaskT.no asc, isCalculateProj desc "
            //每次取出的已经申请的测试个数
            " limit 0, 1;";
    //" limit 0, "+ QString::number( RUNCOUNT ) +";";

    //qDebug() << str;
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    while(query.next())
    {
        id            = query.value(0).toInt();
        taskNo      = query.value(2).toString();//测试编号
        dateTime      = query.value(3).toString();  //申请时间
        projectID     = query.value(4).toInt();
        testType      = query.value(5).toInt();
        sampleMode    = query.value(6).toInt();
        sampleCode    = query.value(7).toString(); //条码是字符串
        isEmergency   = query.value(8).toInt();
        isMixCurrentSample  = query.value(9).toInt();
        isDiluent     = query.value(10).toInt();
        SamplePos     = query.value(11).toString();
        R0            = query.value(12).toInt();
        R1            = query.value(13).toInt();
        R2            = query.value(14).toInt();
        SampleV       = query.value(15).toInt();
        R0V           = query.value(16).toInt();
        R1V           = query.value(17).toInt();
        RSV           = query.value(18).toInt();
        R2V           = query.value(19).toInt();
        lightPos1     = query.value(20).toInt();
        lightPos2     = query.value(21).toInt();
        isRetest      = query.value(22).toInt();
        isCalculateProj= query.value(23).toInt();
        if(isCalculateProj == 1) //计算项目
            projectID = query.value(24).toInt();
        QcLiquidID= query.value(25).toInt();


        //判断搅拌位的样本是否需要搅拌
        isStirSample = 0;
        int slot = SamplePos.split("-").at(0).toInt();
        int sampleNo = SamplePos.split("-").at(1).toInt();       //当前搅拌位的样本位
        QString pos = QString::number(slot) + "-" + QString::number(sampleNo+2);
        QString str  =
                " select TaskT.id, SampleTypeT.isStir from TaskT "
                //连接方式(项目表、样本类型表)
                " Inner join SampleTypeT ON TaskT.sampleTypeID = SampleTypeT.id "
                //只查找未完成状态的任务（待测试）
                " where TaskT.stateID = " + QString::number(State_WAIT) + " and samplePos='" + pos + "' "
                //每次取出的已经申请的测试个数
                " limit 0, 1;";
        QSqlQuery query1 = SQLite::getInstance()->Select(str);  //查询数据库
        while(query1.next())
        {
            isStirSample  = query1.value(1).toInt();
        }

        //是否成功进入任务队列
        if( enTaskList(id, projectID, testType, sampleMode, isMixCurrentSample, isStirSample, isDiluent, sampleCode,
                       SamplePos, R0, R1, R2, SampleV, R0V, R1V, RSV, R2V, dateTime, lightPos1, lightPos2, isRetest, taskNo, isCalculateProj, QcLiquidID) )
        {
            m_BlankCycleCount = BLANKCYCLE_R2;  //最后一个测试加完R2后还要跑的空周期次数
            value = 1;

            //测试打印信息
            //qDebug() << "";
            //qDebug() << "***********";
            qDebug() << "ueryTaskT()...id=" << id;
            qDebug() << "TaskPt:" << m_TaskPt << " ,id:" << id << "ProjectID:" << projectID << " CalculateProj:" << isCalculateProj
                     << " ,cup1:" << TaskList[m_TaskPt].Cup[0] <<" ,cup2:"<< TaskList[m_TaskPt].Cup[1];
            qDebug() << "StirSample:" << isStirSample << " ,Diluent:" << isDiluent << "SamplePos:" << SamplePos;

            msleep(5);
            QString str =
                    "update TaskT "
                    " set stateID =" + QString::number(State_TESTING) +" , "  //测试状态
                    " HistoryT_id = "
                    + QString::number( TaskList[m_TaskPt].History_id )
                    + " where id ="+ QString::number( TaskList[m_TaskPt].id ) +";";
            //list.append(str);
            SQLite::getInstance()->Update(str);

            msleep(10);
            emit sig_Update_ApplicationTaskList();     //信号：更新已申请的测试的状态
            emit sig_AutoUpdateSampleList();           //信号：有新的样本进行测试，"结果查询"界面"样本编号"列表
            emit sig_UpdateApplicatedCount();          //信号：实时更新数据库中状态为“等待”的测试的个数
        }
    }

    //计算项目，属于虚拟项目无实际加样动作.
    if(isCalculateProj == 1 && value ==1)
    {
        qDebug() << "计算项目。。。。。重新查询.........";
        msleep(5);
        value = getTaskInfo();
    }

    return value;
}

//将从数库中取出来的测试放到任务队列中
int GScheduler::enTaskList(int id, int projectID, int testType, int sampleMode, int isMixCurrentSample, int isStirSample, int isDiluent,
                           QString SampleCode, QString SamplePos, int R0, int R1, int R2, int SampleV, int R0V, int R1V, int RSV, int R2V,
                           QString applicateTime, int lightPos1, int lightPos2, int isRetest, QString taskNo, int isCalculateP, int QcLiquidID)
{
    m_TaskPt ++;

    if( m_TaskPt >= TASKCOUNT )//超出任务队列长度
        m_TaskPt = 0;
    if( TaskList[m_TaskPt].Status == 0 )          //检查队列的状态是完为空闲，否则不能进队列（0:空闲， 1:任务测试）
    {
        //加样开始执行的步骤及反应杯的分配
        if( isCalculateP == 0) //常规项目才需要分配反应杯
        {
            //每次分配反应杯都要检查反应杯是否能使用（即是否干净）(0:正常，1:脏杯)
            if( CheckCupStatus(2) ) return 0;

            TaskList[m_TaskPt].Cup[0] = 99;
            TaskList[m_TaskPt].Cup[1] = 99;
            //            if( m_UsedCup > REACTION_CUP_COUNT )
            //                m_UsedCup = m_UsedCup-REACTION_CUP_COUNT;
            //            TaskList[m_TaskPt].Cup[0] = m_UsedCup ++; //测试用的反应杯号

            //            if( m_UsedCup > REACTION_CUP_COUNT )
            //                m_UsedCup = m_UsedCup-REACTION_CUP_COUNT;
            //            TaskList[m_TaskPt].Cup[1] = m_UsedCup ++; //测试用的反应杯号
        }
        else
        {
            TaskList[m_TaskPt].Cup[0] = 99;
            TaskList[m_TaskPt].Cup[1] = 99;
        }

        TaskList[m_TaskPt].Status = 2;            //状态：（0:空闲， 1:任务测试）
        TaskList[m_TaskPt].id = id;               //对应数据库中任务队列表中的ID。
        TaskList[m_TaskPt].AD_Count = 1;          //采光次数
        TaskList[m_TaskPt].LightPos1 = lightPos1; //采光点1
        TaskList[m_TaskPt].LightPos2 = lightPos2; //采光点2

        TaskList[m_TaskPt].isCalculateProj = isCalculateP;//计算项目否
        TaskList[m_TaskPt].ProjectId = projectID;       //项目id（常规项目id/计算项目id）
        TaskList[m_TaskPt].TestType = testType;         //测试类型（1：样本测试，2：质控测试，3：定标测试）
        TaskList[m_TaskPt].SampleType = sampleMode;     //标本类型（1：全血，2：血清，3：血浆）

        TaskList[m_TaskPt].isMixNextSample = isStirSample; //样本是否需要搅拌
        TaskList[m_TaskPt].isMixCurrentSample = isMixCurrentSample; //当前样本是否需要搅拌（切换样本架时需要用到）
        TaskList[m_TaskPt].isDiluent = isDiluent;       //样本是否需要稀释
        TaskList[m_TaskPt].SampleCode = SampleCode;  //样本试管编号（条码）
        TaskList[m_TaskPt].SamplePos = SamplePos;    //样本

        TaskList[m_TaskPt].Reagent0 = R0;            //稀释液编号
        TaskList[m_TaskPt].Reagent1 = R1;            //R1编号
        TaskList[m_TaskPt].Reagent2 = R2;            //R2编号

        TaskList[m_TaskPt].VolumeR0 = R0V;        //稀释液R0量
        TaskList[m_TaskPt].VolumeSample = SampleV;//样本量
        TaskList[m_TaskPt].VolumeR1 = R1V;        //R1量
        TaskList[m_TaskPt].VolumeRS = RSV;        //稀释后样本的量
        TaskList[m_TaskPt].VolumeR2 = R2V;        //R2量

        int historyId =  insertIntoTempHistoryT(m_TaskPt, taskNo, applicateTime, QcLiquidID, isRetest);
        if( historyId ) //id >0
            TaskList[m_TaskPt].History_id = historyId;
        else
        {
            return 0; //失败  咋整？？
        }


        //若为计算项目，则无加样动作
        if(isCalculateP == 1)
            TaskList[m_TaskPt].LightPos2 = 10; //采光点2

        return 1; //成功
    }
    else   //分配不成功 咋整？？？
    {
        qDebug() << "Status:" << TaskList[m_TaskPt].Status << " function:" << __func__  << " ,Line:" << __LINE__;
        return 0;
    }
}

//将正在进行的测试（刚从数据库任务表中取出来的测试）添加到数据库历史结果数据表中
int GScheduler::insertIntoTempHistoryT(int TaskT_id, QString taskNo, QString applicateT, int QcLiquidID, int isRetest)
{
    //返回新增历史结果数据表（TempHistoryT）的ID
    int id = -1; //添加不成功
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString formatT = time.toString("yyyy-MM-dd hh:mm:ss");      //设置显示格式

    //保存反应杯号
    QString cup1, cup2;
    cup1 = QString::number(TaskList[TaskT_id].Cup[0]);
    cup2 = QString::number(TaskList[TaskT_id].Cup[1]);

    //qDebug() << "*******cup1.2***********" << TaskList[TaskT_id].Cup[0] << TaskList[TaskT_id].Cup[1] << cup1 << cup2;
    int cID, pID;
    if(TaskList[TaskT_id].isCalculateProj == 1) //计算项目
    {
        cID = TaskList[TaskT_id].ProjectId;
        pID = 1;
    }
    else
    {
        cID = 1;
        pID = TaskList[TaskT_id].ProjectId;
    }

    //添加一条记录到数据库中
    QString str =
            "insert into TempHistoryT("
            //测试编号、状态、申请时间、测试时间、用户id、测试类型、是否为计算项目、常规项目id、计算项目ID、质控液ID
            "T_id, no, statusID, applyTime, testDate, userID, typeID, isCalculateProj, projectID, calculateProjID, qcLiquidID, "
            //样本条码、样本编号、样本类型、打印否、急诊否、重测否、修改否、测光点1、测光点2
            " sampleCode, SamplePos, Cup0, Cup1, sampleTypeID, "
            " isPrint, isRetest, isModify, lightPos1, lightPos2) "
            " values( "
            + QString::number(TaskList[TaskT_id].id)+ ", " //任务表中对应的id
            " '" + taskNo + "', "
            +" " + QString::number(State_TESTING)  +", "
            +"'" + applicateT + "', " //申请时间
            +"'" + formatT + "'" + ", "       //测试时间
            + QString::number(gParameter->getUserID())+ ", " //用户id
            + QString::number(TaskList[TaskT_id].TestType)+ ", "   //测试类型

            + QString::number(TaskList[TaskT_id].isCalculateProj)+ ", " //计算项目否
            + QString::number(pID)+ ", "       //项目id
            + QString::number(cID)+ ", "       //项目id
            + QString::number(QcLiquidID)+ ", "       //质控液id

            + "'"+ TaskList[TaskT_id].SampleCode  + "', "               //样本条码
            + "'"+ TaskList[TaskT_id].SamplePos + "', "                  //样本位置

            + "'"+ QString::number(TaskList[TaskT_id].Cup[0]) + "', "                       //反应杯1
            + "'"+ QString::number(TaskList[TaskT_id].Cup[1]) + "', "                      //反应杯2

            + QString::number(TaskList[TaskT_id].SampleType)+ ", " //样本类型
            + "0, " //打印否
            + QString::number(isRetest)+ ", " //重测否
            + "0, " //修改否
            + QString::number(TaskList[TaskT_id].LightPos1) + ", " //测光点1
            + QString::number(TaskList[TaskT_id].LightPos2)        //测光点2
            + " );";

    //qDebug() << str;

    if( SQLite::getInstance()->Insert(str) )
    {

        //获取历史结果数据表新增行的id（即最大id）
        str  = "SELECT MAX(id) FROM TempHistoryT;";  //表的总行数
        QSqlQuery  query = SQLite::getInstance()->Select(str);
        while(query.next())
            id = query.value(0).toInt();
    }
    else
    {
        return 0;
        qDebug() << "**" << " function:" << __func__  << " ,Line:" << __LINE__;
    }
    return id;
}

void GScheduler::Action()
{
    //    //测试专用
    //    QString str = "FA 01 01 01 10 40 04 00 01 06 20 04 58 01 02 03 03 10 00 0A 00 28 00 64 01 00 64 00 01 00 00 00 00 00 00 00 00 00 00 00 72 FB";
    //    emit sig_CycleTimer_start(gParameter->getCycleTime());
    //    emit sig_TcpServer_Write(str);  //信号：写数据
    //    return;

    //测试前是否需要先清洗反应杯（刚开完机后需要清洗一段反应杯）
    if( !isBootCleanedFlag )
    {
        if( WorkModeFlag == MV_State_Pause ) return;

        //清洗反应杯3次
        if( m_BootCleanedTimes < BLANKCYCLE_INIT )
        {
            emit sig_CycleTimer_start(CLEANCUP_TIME);
            int isTurn = 1; //1:反应盘转动  0:反应盘不转动
            emit sig_TcpServer_Write(gMVFrame->getCleamCupCmd(isTurn), LENGHT_CLEANCUP);  //信号：写数据
            m_BootCleanedTimes ++;
            emit sig_setCupStatus(m_BootCleanedTimes, "Clean");
            CurrentCleaningCupNo = m_BootCleanedTimes;
            qDebug() << "清洗机构第1阶：" << CurrentCleaningCupNo;
            qDebug() << "..开机已经清洗了反应杯:" << m_BootCleanedTimes;
            return;
        }
        //周期指令清洗反应杯2次（共4个反应杯）
        else if( m_BootCleanedTimes < BLANKCYCLE_INIT + BLANKCYCLE_INIT2*2  )
        {
            m_BootCleanedTimes = m_BootCleanedTimes + 2;
            CurrentCleaningCupNo = m_BootCleanedTimes;
            qDebug() << "清洗机构第1阶：" << CurrentCleaningCupNo;
            qDebug() << "..开机已经清洗了反应杯:" << m_BootCleanedTimes;
            emit sig_setCupStatus(m_BootCleanedTimes, "Clean");

            emit sig_CycleTimer_start(gParameter->getCycleTime());
            //
            CombineEmptyCycleCmd(1, CurrentMixSampleNo, isCurrentSampleNeedMix, 1, 0, 0);

            if(m_BootCleanedTimes == BLANKCYCLE_INIT + BLANKCYCLE_INIT2*2)
            {
                ////从数据库中取出任务
                getTaskInfo();

                isCurrentSampleNeedMix= 0;
                CurrentMixSampleSlot = 0;     //当前搅拌位的样本架
                CurrentMixSampleNo = 0;       //当前搅拌位的样本位
                CurrentMixSamplePos = "";  //当前搅拌的样本位置
            }
            else
            {
                //查看当前搅拌位的下一个样本是否需要搅拌
                CurrentMixSampleNo = CurrentMixSamplePos.split("-").at(1).toInt();       //当前搅拌位的样本位
                QString pos = QString::number(CurrentMixSampleSlot) + "-" + QString::number(CurrentMixSampleNo+1);
                QString str  =
                        " select TaskT.id, SampleTypeT.isStir, samplePos from TaskT "
                        //连接方式(项目表、样本类型表)
                        " Inner join SampleTypeT ON TaskT.sampleTypeID = SampleTypeT.id "
                        //只查找未完成状态的任务（待测试）
                        " where TaskT.stateID = " + QString::number(State_WAIT) + " and samplePos='" + pos +"' "
                        //排序查询：
                        " order by TaskT.SamplePosNo asc "
                        //每次取出的已经申请的测试个数
                        " limit 0, 1;";
                //qDebug() << str;
                QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
                isCurrentSampleNeedMix = 0;
                while(query.next())
                {
                    isCurrentSampleNeedMix = query.value(1).toInt();
                    CurrentMixSamplePos = query.value(2).toString();
                    CurrentMixSampleSlot = CurrentMixSamplePos.split("-").at(0).toInt();     //当前搅拌位的样本架
                    CurrentMixSampleNo = CurrentMixSamplePos.split("-").at(1).toInt();       //当前搅拌位的样本位
                }
                //当前测试的样本架的编号（用于判断是前后测试是否在同一样本架上）
                CurrentSampleSlot = QString::number( CurrentMixSampleSlot );
                //qDebug() << "CurrentMixSamplePos：" << CurrentMixSamplePos << "isCurrentSampleNeedMix:" << isCurrentSampleNeedMix;
            }

            return;
        }
        else
        {
            emit sig_CycleTimer_start(gParameter->getCycleTime());
            isBootCleanedFlag = true;//开机已经清洗了反应杯,可以进行测试了
        }
    }

    //所有加样完成后的*空周期*指令
    if(TaskList[m_TaskPt].Status == 1 || TaskList[m_TaskPt].Status == 0)
    {
        if( m_BlankCycleCount > 0)
        {
            qDebug() << "";
            qDebug() << "********空周期指令 剩余次数:" << m_BlankCycleCount;

            int samplePos = 0;
            int isAD =1;
            if( m_BlankCycleCount == BLANKCYCLE_R2 ) //推出当前样本架
                samplePos = 22;
            //当前测试的周期指令整合
            CombineEmptyCycleCmd(1, samplePos, 0, 0, isAD, 0);

            if(AddedSampleCup != 0) //即上一周期是否有加样
            {
                emit sig_setCupStatus(AddedSampleCup, "End1");
                emit sig_setCupStatus(AddedSampleCup+1, "End1");
                AddedSampleCup = 0;
            }

            //m_UsedCup = m_UsedCup+2; //反应杯号递进2个
            m_UsedCup ++; //反应杯号递进2个
            int CleanCup = m_UsedCup + BLANKCYCLE_INIT + BLANKCYCLE_INIT2*2;
            if( CleanCup > REACTION_CUP_COUNT)
                CleanCup = CleanCup - REACTION_CUP_COUNT;

            m_UsedCup ++; //反应杯号递进2个
            if( m_UsedCup > REACTION_CUP_COUNT )
                m_UsedCup = m_UsedCup-REACTION_CUP_COUNT;
            CurrentCleaningCupNo = CleanCup;
            emit sig_setCupStatus(CleanCup, "Clean");
            qDebug() << "**清洗机构第1阶：" << CleanCup;

            //是否要更新样本架信息
            if(CurrentSamplePos != "")
            {
                emit sig_updateSampleStatus(CurrentSamplePos, "");
                CurrentSamplePos = "";
            }

            NewTestInTeam = 0;
            m_BlankCycleCount --;

            msleep(500);
            NewTestInTeam = getTaskInfo(); //新测试进入队列
        }
        else
        {
            qDebug() << "********所有测试已经完成了........";
            emit sig_CycleTimer_stop(); //定时器停止工作
            WorkModeFlag = MV_State_IDLE;   //空闲状态（开机自检完成 | 测试完停机状态）
            //信号：更新“开始”“暂停”“停止”三个按钮的状态 (type=0：所有测试都完成， type=1:运行出错)
            emit sig_UpdateMainWidget3Button_State(0);
        }

        return;
    }

    //更换样本架（要发x个空周期）
    if(CurrentSampleSlot != TaskList[m_TaskPt].SamplePos.split("-").at(0))
    {
        qDebug() << "更换样本架:" <<  CurrentSampleSlot << TaskList[m_TaskPt].SamplePos.split("-").at(0)
                 << " 第"<< ChangedSampleSlot << "次";

        //m_UsedCup = m_UsedCup+2; //反应杯号递进2个
        m_UsedCup ++; //反应杯号递进2个
        int CleanCup = m_UsedCup + BLANKCYCLE_INIT + BLANKCYCLE_INIT2*2;
        if( CleanCup > REACTION_CUP_COUNT)
            CleanCup = CleanCup - REACTION_CUP_COUNT;
        CurrentCleaningCupNo = CleanCup;
        emit sig_setCupStatus(CleanCup, "Clean");
        qDebug() << "**清洗机构第1阶：" << CleanCup;
        m_UsedCup ++; //反应杯号递进2个
        if( m_UsedCup > REACTION_CUP_COUNT )
            m_UsedCup = m_UsedCup-REACTION_CUP_COUNT;
        if(AddedSampleCup != 0) //即上一周期是否有加样
        {
            emit sig_setCupStatus(AddedSampleCup, "End1");
            emit sig_setCupStatus(AddedSampleCup+1, "End1");
            AddedSampleCup = 0;
        }

        NewTestInTeam = 0;

        //第一个空周期（切换样本架）
        if(ChangedSampleSlot == CHANGESAMPLESLOT)
        {
            CombineEmptyCycleCmd(1, 1 + SAMPLESLOT_COUNT, 0, 1, 1, 0);
            ChangedSampleSlot --;
            return;
        }
        //第2个空周期（移动当前样本到搅拌位）
        if(ChangedSampleSlot == CHANGESAMPLESLOT-1)
        {
            int sPos = TaskList[m_TaskPt].SamplePos.split("-").at(1).toInt();
            int isMix = TaskList[m_TaskPt].isMixCurrentSample;
            qDebug() << "是否搅拌：" << isMix << "SamplePos:" << sPos;
            CombineEmptyCycleCmd(1, sPos, isMix, 1, 1, 0);
            ChangedSampleSlot --;
            return;
        }
        //第3个空周期（下一样本是否也需要搅拌）
        else if( ChangedSampleSlot == CHANGESAMPLESLOT-2)
        {
            //查看当前搅拌位的下一个样本是否需要搅拌
            QString slot =  TaskList[m_TaskPt].SamplePos.split("-").at(0);
            int CurrentSampleNo = TaskList[m_TaskPt].SamplePos.split("-").at(1).toInt() +1;       //当前搅拌位的样本位
            QString pos = slot + "-" + QString::number(CurrentSampleNo);
            QString str  =
                    " select TaskT.id, SampleTypeT.isStir, samplePos from TaskT "
                    //连接方式(项目表、样本类型表)
                    " Inner join SampleTypeT ON TaskT.sampleTypeID = SampleTypeT.id "
                    //只查找未完成状态的任务（待测试）
                    " where TaskT.stateID = " + QString::number(State_WAIT) + " and samplePos='" + pos +"' "
                    //排序查询：紧急否降序＋样本号升序＋是否搅拌排序（同一样本所申请的测试按样本不需要搅拌的先进行，然后再测要搅拌的项目）
                    " order by TaskT.SamplePosNo asc "
                    //每次取出的已经申请的测试个数
                    " limit 0, 1;";
            //qDebug() << str;
            QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
            int isMix = 0;
            while(query.next())
            {
                isMix = query.value(1).toInt();
            }

            qDebug() << "是否搅拌：" << isMix << "SamplePos:" << pos;
            CombineEmptyCycleCmd(1, CurrentSampleNo, isMix, 1, 1, 0);
            ChangedSampleSlot --;

            CurrentSampleSlot = TaskList[m_TaskPt].SamplePos.split("-").at(0);
            //切换样本架（需要加两个空周期）
            ChangedSampleSlot = CHANGESAMPLESLOT;

            return;
        }
    }

    //在发“空周期”指令时，有新的测试进入队列
    if( NewTestInTeam == 1 )
    {
        //
    }

    //样本架不连续（间隔一个样本或多个样本）
    if(EmptyCycleTimes > 0)
    {
        m_UsedCup ++; //反应杯号递进2个
        //m_UsedCup = m_UsedCup+2; //反应杯号递进2个
        int CleanCup = m_UsedCup + BLANKCYCLE_INIT + BLANKCYCLE_INIT2*2;
        if( CleanCup > REACTION_CUP_COUNT)
            CleanCup = CleanCup - REACTION_CUP_COUNT;
        CurrentCleaningCupNo = CleanCup;
        emit sig_setCupStatus(CleanCup, "Clean");
        qDebug() << " 间隔样本个数" << EmptySampleCount << "EmptyCycleTimes:" << EmptyCycleTimes;
        qDebug() << "**清洗机构第1阶：" << CleanCup;
        m_UsedCup ++; //反应杯号递进2个
        if( m_UsedCup > REACTION_CUP_COUNT )
            m_UsedCup = m_UsedCup-REACTION_CUP_COUNT;

        if(AddedSampleCup != 0) //即上一周期是否有加样
        {
            emit sig_setCupStatus(AddedSampleCup, "End1");
            emit sig_setCupStatus(AddedSampleCup+1, "End1");
            AddedSampleCup = 0;
        }

        //间隔样本个数:1
        if(EmptySampleCount == 1)
        {
            qDebug() << "间隔样本个数:" << EmptySampleCount;
            //查看当前搅拌位的下一个样本是否需要搅拌
            QString slot =  TaskList[m_TaskPt].SamplePos.split("-").at(0);
            int CurrentSampleNo = TaskList[m_TaskPt].SamplePos.split("-").at(1).toInt()-1;       //当前搅拌位的样本位
            QString pos = slot + "-" + QString::number(CurrentSampleNo+2);
            QString str  =
                    " select TaskT.id, SampleTypeT.isStir from TaskT "
                    //连接方式(项目表、样本类型表)
                    " Inner join SampleTypeT ON TaskT.sampleTypeID = SampleTypeT.id "
                    //只查找未完成状态的任务（待测试）
                    " where TaskT.stateID = " + QString::number(State_WAIT) + " and samplePos='" + pos +"' "
                    //每次取出的已经申请的测试个数
                    " limit 0, 1;";
            qDebug() << str;
            QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
            int isMix = 0;
            while(query.next())
                isMix = query.value(1).toInt();


            CombineEmptyCycleCmd(1, CurrentSampleNo, isMix, 0, 1, 0);
            //int turnRD, int samplePos, int isMix, int isMixPox, int isAD, int isScanSampleCode
        }
        else if(EmptySampleCount >= 2)
        {
            qDebug() << "间隔样本个数:" << EmptySampleCount << " ,空周期个数" << EmptyCycleTimes;
            if( EmptyCycleTimes != 1)
            {
                //查看当前搅拌位的下一个样本是否需要搅拌
                QString slot =  TaskList[m_TaskPt].SamplePos.split("-").at(0);
                int CurrentSampleNo = TaskList[m_TaskPt].SamplePos.split("-").at(1).toInt()-2;       //即将推到吸样本位的样本位置
                QString pos = slot + "-" + QString::number(CurrentSampleNo+2);
                QString str  =
                        " select TaskT.id, SampleTypeT.isStir from TaskT "
                        //连接方式(项目表、样本类型表)
                        " Inner join SampleTypeT ON TaskT.sampleTypeID = SampleTypeT.id "
                        //只查找未完成状态的任务（待测试）
                        " where TaskT.stateID = " + QString::number(State_WAIT) + " and samplePos='" + pos +"' "
                        //每次取出的已经申请的测试个数
                        " limit 0, 1;";
                //qDebug() << str;
                QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
                int isMix = 0;
                while(query.next())
                    isMix = query.value(1).toInt();
                CombineEmptyCycleCmd(1, CurrentSampleNo, isMix, 0, 1, 0);
            }
            else
            {
                //查看当前搅拌位的下一个样本是否需要搅拌
                QString slot =  TaskList[m_TaskPt].SamplePos.split("-").at(0);
                int CurrentSampleNo = TaskList[m_TaskPt].SamplePos.split("-").at(1).toInt()-1;       //即将推到吸样本位的样本位置
                QString pos = slot + "-" + QString::number(CurrentSampleNo+1);
                QString str  =
                        " select TaskT.id, SampleTypeT.isStir from TaskT "
                        //连接方式(项目表、样本类型表)
                        " Inner join SampleTypeT ON TaskT.sampleTypeID = SampleTypeT.id "
                        //只查找未完成状态的任务（待测试）
                        " where TaskT.stateID = " + QString::number(State_WAIT) + " and samplePos='" + pos +"' "
                        //每次取出的已经申请的测试个数
                        " limit 0, 1;";
                //qDebug() << str;
                QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
                int isMix = 0;
                while(query.next())
                    isMix = query.value(1).toInt();
                CombineEmptyCycleCmd(1, CurrentSampleNo, isMix, 0, 1, 0);
            }
        }

        EmptyCycleTimes --;
        return;
    }

    //整合周期指令（发送到中位机）
    if( TaskList[m_TaskPt].Status == 2)
    {
        QString a = TaskList[m_TaskPt].SamplePos.split("-").at(1);
        int no = a.toInt();
        CombineCycleCmd(no);
        TaskList[m_TaskPt].Status = 1;

        //记录反应杯使用情况（保存对应反应杯的ad值）
        if(TaskList[m_TaskPt].isCalculateProj == 0)//常规项目才需要分配反应杯
        {
            if( m_UsedCup > REACTION_CUP_COUNT )
                m_UsedCup = m_UsedCup-REACTION_CUP_COUNT;
            TaskList[m_TaskPt].Cup[0] = m_UsedCup ++; //测试用的反应杯号
            if( m_UsedCup > REACTION_CUP_COUNT )
                m_UsedCup = m_UsedCup-REACTION_CUP_COUNT;
            TaskList[m_TaskPt].Cup[1] = m_UsedCup; //测试用的反应杯号
        }
        else
        {
            TaskList[m_TaskPt].Cup[0] = 99;
            TaskList[m_TaskPt].Cup[1] = 99;
        }

        int CleanCup = m_UsedCup + BLANKCYCLE_INIT + BLANKCYCLE_INIT2*2;
        if( CleanCup > REACTION_CUP_COUNT)
            CleanCup = CleanCup - REACTION_CUP_COUNT;
        CurrentCleaningCupNo = CleanCup;
        emit sig_setCupStatus(CleanCup, "Clean");
        m_UsedCup ++;
        if( m_UsedCup > REACTION_CUP_COUNT )
            m_UsedCup = m_UsedCup-REACTION_CUP_COUNT;
        qDebug() << "**清洗机构第1阶：" << CleanCup;

        //设置反应杯状态
        if(AddedSampleCup != 0) //即上一周期是否有加样
        {
            emit sig_setCupStatus(AddedSampleCup, "End1");
            emit sig_setCupStatus(AddedSampleCup+1, "End1");
        }
        AddedSampleCup = TaskList[m_TaskPt].Cup[0];
        emit sig_setCupStatus(TaskList[m_TaskPt].Cup[0], "R2");
        emit sig_setCupStatus(TaskList[m_TaskPt].Cup[1], "R2");

        msleep(5);
        emit sig_Update_ApplicationTaskList();     //信号：更新已申请的测试的状态
        emit sig_AutoUpdateSampleList();           //信号：有新的样本进行测试，"结果查询"界面"样本编号"列表
        emit sig_UpdateApplicatedCount();          //信号：实时更新数据库中状态为“等待”的测试的个数

        CurrentSamplePos = TaskList[m_TaskPt].SamplePos;

        //改变任务表中当前测试任务的状态
        msleep(10);
        //QStringList list;
        //        QString str =
        //                "update TaskT "
        //                " set stateID =" + QString::number(State_TESTING) +" , "  //测试状态
        //                " HistoryT_id = "
        //                + QString::number( TaskList[m_TaskPt].History_id )
        //                + " where id ="+ QString::number( TaskList[m_TaskPt].id ) +";";
        //        //list.append(str);
        //        SQLite::getInstance()->Update(str);
        QString str =
                "update TempHistoryT set Cup0='"+ QString::number(TaskList[m_TaskPt].Cup[0]) + "', " //反应杯1
                + " Cup1='"+ QString::number(TaskList[m_TaskPt].Cup[1]) + "' "      //反应杯2
                + " where id ="+ QString::number( TaskList[m_TaskPt].History_id ) +";";
        //list.append(str);
        //SQLite::getInstance()->Transaction(list);
        SQLite::getInstance()->Update(str);
    }

    //当前样本位置
    QString pos = TaskList[m_TaskPt].SamplePos;

    //取出下一个测试的信息-从数据库中取出测试数据
    msleep(5); //要有延时，不能太快查询数据库
    getTaskInfo();

    //判断前后两个测试的样本
    if(pos.split("-").at(0) == TaskList[m_TaskPt].SamplePos.split("-").at(0) ) //样本架是否是同一个
    {
        int no = pos.split("-").at(1).toInt();//当前测试的样本号
        int noNext = TaskList[m_TaskPt].SamplePos.split("-").at(1).toInt(); //下一个测试的样本号
        if( no+1 != noNext ) //前后两个测试的样本号是否相连（不相连）
        {
            //样本号小于8
            if( no <= SAMPLESLOT_COUNT-2 )
            {
                int count = noNext - no;
                EmptySampleCount = count-1;

                if( count == 1 )
                {
                    EmptyCycleTimes = 1;
                    return;
                }
                else if (count > 1)
                {
                    EmptyCycleTimes = 2;
                    return;
                }
            }
        }
    }

    //是否要更新样本架信息
    if(CurrentSamplePos != TaskList[m_TaskPt].SamplePos)
    {
        emit sig_updateSampleStatus(CurrentSamplePos, TaskList[m_TaskPt].SamplePos);
        CurrentSamplePos = "";
    }

    EmptyCycleTimes = 0;
}

//整合周期指令（发送到中位机）
void GScheduler::CombineCycleCmd(int sampleNo)
{
    //当前测试的周期指令整合
    char *TestCycle = gMVFrame->getTestCycleCmd();

    //当前反应盘的杯位（0表示不转反应盘）
    TestCycle[8]  = 0x01;

    //
    int water = gParameter->getWaterVol();
    int liquid = gParameter->getCleanLiquidVol();
    TestCycle[9]  = water/128;  //清水吸液量高7位
    TestCycle[10] = water%128;  //清水吸液量低7位（0不动作）
    TestCycle[11] = liquid/128;  //清液吸液量低7位
    TestCycle[12] = liquid%128;  //清液吸液量低7位（0不动作）

    //稀释液、R1、R2在试剂盘上位置
    TestCycle[13] = TaskList[m_TaskPt].Reagent0;
    TestCycle[14] = TaskList[m_TaskPt].Reagent1;
    TestCycle[15] = TaskList[m_TaskPt].Reagent2;

    //（稀释液）第一试剂吸液量高7位（单位ul）
    TestCycle[16] = TaskList[m_TaskPt].VolumeR0/128;
    //（稀释液）第一试剂吸液量低7位（吸液量0表示不吸第一试剂）
    TestCycle[17] = TaskList[m_TaskPt].VolumeR0%128;

    //（R1）第二试剂吸液量高7位（单位ul）
    TestCycle[18] = TaskList[m_TaskPt].VolumeR1/128;
    //（R1）第二试剂吸液量低7位（吸液量0表示不吸第一试剂）
    TestCycle[19] = TaskList[m_TaskPt].VolumeR1%128;

    //（转移样本）转移液体量高7位（单位ul）
    TestCycle[20] = TaskList[m_TaskPt].VolumeRS/128;
    //（转移样本）转移液体量低7位（转移量0表示不转移液体）
    TestCycle[21] = TaskList[m_TaskPt].VolumeRS%128;

    //（R2）第三试剂吸液量高7位（单位ul）
    TestCycle[22] = TaskList[m_TaskPt].VolumeR2/128;
    //（R2）第三试剂吸液量低7位（吸液量0表示不吸第一试剂）
    TestCycle[23] = TaskList[m_TaskPt].VolumeR2%128;

    //是否推动样本架  1—10推到当前试管架对应杯位；11-20推到下一条试管架对应杯位；21自动推试管模式；22推出当前试管架 0不推试管。
    TestCycle[24] = sampleNo;

    //（样本）样本吸液量高7位（单位ul）
    TestCycle[25] = TaskList[m_TaskPt].VolumeSample/128;
    //（样本）样本吸液量低7位 （吸液量0表示不吸样本）
    TestCycle[26] = TaskList[m_TaskPt].VolumeSample%128;

    //是否搅拌样本（1：全血搅拌，0：不搅拌）
    TestCycle[27] = TaskList[m_TaskPt].isMixNextSample;

    //是否采集AD数据（1：打开AD采集，0：关闭AD采集）
    TestCycle[28] = 0x01;
    //是否打开条码扫描（1：打开条码扫描，0：关闭条码扫描）
    TestCycle[29] = 0x00;
    //推到样本位or推到搅拌位（0：推到样本位，1：推到搅拌位）
    TestCycle[30] = 0x00;

    emit sig_CycleTimer_start(gParameter->getCycleTime());
    emit sig_TcpServer_Write(TestCycle, LENGHT_TESTCYCLE);
}

//整合空周期指令（发送到中位机）
void GScheduler::CombineEmptyCycleCmd(int turnRD, int samplePos, int isMix, int isMixPox, int isAD, int isScanSampleCode)
{
    //    qDebug() << "samplePos:" << samplePos;
    int mixPos = isMixPox;

    //样本位大于2，则不能直接推到搅拌位，而是要推到吸样本位
    if(isMixPox)
    {
        if(samplePos > 2 && samplePos <= SAMPLESLOT_COUNT)
        {
            samplePos = samplePos - 2;
            mixPos = 0;
        }
    }
    qDebug() << "samplePos:" << samplePos;

    //当前测试的周期指令整合
    char *TestCycle = gMVFrame->getTestCycleCmd();

    //当前反应盘的杯位（0表示不转反应盘）
    TestCycle[8]  = turnRD;

    //
    int water = gParameter->getWaterVol();
    int liquid = gParameter->getCleanLiquidVol();
    TestCycle[9]  = water/128;  //清水吸液量高7位
    TestCycle[10] = water%128;  //清水吸液量低7位（0不动作）
    TestCycle[11] = liquid/128;  //清液吸液量低7位
    TestCycle[12] = liquid%128;  //清液吸液量低7位（0不动作）

    //是否推动样本架  1—10推到当前试管架对应杯位；11-20推到下一条试管架对应杯位；21自动推试管模式；22推出当前试管架 0不推试管。
    TestCycle[24] = samplePos;

    //是否搅拌样本（1：全血搅拌，0：不搅拌）
    TestCycle[27] = isMix;

    //是否采集AD数据（1：打开AD采集，0：关闭AD采集）
    TestCycle[28] = isAD;
    //是否打开条码扫描（1：打开条码扫描，0：关闭条码扫描）
    TestCycle[29] = isScanSampleCode;
    //推到样本位or推到搅拌位（0：推到样本位，1：推到搅拌位）
    TestCycle[30] = mixPos;

    emit sig_TcpServer_Write(TestCycle, LENGHT_TESTCYCLE);
}

//样本是否存在
bool GScheduler::isSampleExist(QString samplePos)
{
    //不同样本架时，默认样本存在
    if(CurrentSampleSlot != samplePos.split("-").at(0)) return 1;

    //每个样本架只判断前8个样本
    if(samplePos.split("-").at(1).toInt() > SAMPLESLOT_COUNT-2) return 1;

    //查看样本是否存在
    QString str  =
            " select TaskT.id from TaskT "
            //只查找未完成状态的任务（待测试）
            " where TaskT.stateID = " + QString::number(State_WAIT) + " and samplePos='" + samplePos +"' "
            //每次取出的个数
            " limit 0, 1;";
    //qDebug() << str;
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    while(query.next())
        return 1;
    return 0;
}



