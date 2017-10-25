#include "ApplicateThread.h"

ApplicateThread * gApplicateThread;
ApplicateThread::ApplicateThread(QThread *parent)
    : QThread(parent)
{
    //样本申请信息
    RequestedNO = 1;            //申请的测试的个数
    SamplePos.clear();          //样本位置信息
    StartNo.clear();            //申请开始的编号（批量时要自增）
    SampleType = 1;             //样本类型（血清）
    ProjectIdList.clear();      //项目
    CalculateIdList.clear();
    RepeatTime = 1;             //重复次数(1次)
}

//申请测试
int ApplicateThread::ApplicateTest(int testType, QString samplePos, QString startNo, int sampleType, QStringList projectList,
                                   QStringList calculateIdList, QMap<int, QString> sampleNoMap, int repeatTime, int qcLiquid)
{
    if(this->isRunning())
    {
        qDebug() << "ApplicateThread running...";
        return 0; //线程工作中(未执行)
    }

    //样本申请信息
    TestType = testType;
    ProjectIdList.clear();
    CalculateIdList.clear();
    SamplePos = samplePos;          //样本位置信息
    StartNo = startNo;              //申请开始的编号（批量时要自增）
    SampleType = sampleType;        //样本类型
    ProjectIdList = projectList;    //项目
    CalculateIdList = calculateIdList;
    SampleNoMap = sampleNoMap;//样本条码
    RepeatTime = repeatTime;        //重复次数
    QcLiquidID = qcLiquid;

    this->start(LowestPriority);
    return 1;
}

void ApplicateThread::run()
{
    applicateTest(TestType, SamplePos, StartNo, SampleType, ProjectIdList, CalculateIdList, SampleNoMap, RepeatTime, QcLiquidID);     //申请测试
}

//申请测试
void ApplicateThread::applicateTest(int testType, QString samplePos, QString startNo, int sampleType,
                                    QStringList projectList,  QStringList calculateIdList,
                                    QMap<int, QString> sampleNoMap,  int repeatTime, int qcLiquidID)
{
    QString strSplit = ",";  //以空格符作为数据的分割符
    int SampleCount = samplePos.split(strSplit).count()-1; //要申请的样本个数
    QString sPos;       //样本位 "样本架-样本位"
    QString sampleCode; //样本条码
    bool isUrgent = 0;  //是否为急诊（默认为普通测试）
    int pt; //

    sqlListStr.clear();
    for( int i = 0; i < SampleCount; i ++ ) //样本个数
    {
        sampleCode = "";
        sPos = samplePos.split(strSplit).at(i).mid(1, samplePos.split(strSplit).at(i).count()-2);
        pt = (sPos.split("-").at(0).toInt()-1)*10 + sPos.split("-").at(1).toInt()-1;


        //常规项目个数
        for(int j = 0; j < projectList.count(); j ++)
        {
            sampleCode = sampleNoMap.value(pt);
            for( int k = 0; k < repeatTime; k ++) //重复次数
            {
                RequestTask(testType, isUrgent, sPos, 0, projectList.at(j).toInt(), startNo, sampleType, sampleCode, qcLiquidID);
                startNo = UpdateRequestedNO();
            }
        }

        //计算项目
        for(int k = 0; k < calculateIdList.count(); k ++)
        {
            RequestTask(testType, isUrgent, sPos, 1, calculateIdList.at(k).toInt(), startNo, sampleType, sampleCode, 1);
            startNo = UpdateRequestedNO();
        }

    }

    //普通样本才需要记住编号（质控则每次开机都是从1开始）
    if(TestType == MV_State_Normal)
    {
        QString  str = "update SysParamT set SampleNO=" + QString::number(gParameter->getSampleNo()) + ";";
        sqlListStr.append(str);
    }

    SQLite::getInstance()->Transaction(sqlListStr);
    sqlListStr.clear();

    //信号：实时更新数据库中状态为“等待”的测试的个数
    msleep(100);
    emit sig_UpdateApplicatedCount();
    emit sig_updateAllSampleStatus(); //信号：更新“工作状态”-“样本信息”中的样本状态
}

//更新测试编号
QString ApplicateThread::UpdateRequestedNO()
{
    RequestedNO ++;
    QDateTime time = QDateTime::currentDateTime();     //获取系统现在的时间
    QString date = time.toString("yyyyMMddhhmm");      //设置显示格式

    QString temp = "00000";
    QString count = QString::number(RequestedNO);
    temp = temp.replace(5-count.count(), count.count(), count );
    return date+temp;
}

//申请任务（即将申请的任务保存到数据库中）
void ApplicateThread::RequestTask(int TestType, int isUrgent, QString SamplePos, int isCalculateP, int ProjectID,
                                  QString TaskNo, int SampleType, QString sampleCode, int qcLiquid)
{
    QDateTime time = QDateTime::currentDateTime();            //获取系统现在的时间
    QString date = time.toString("yyyy-MM-dd hh:mm:ss");      //设置显示格式

    int calculateProjID, projectID;

    if(TestType != MV_State_QC)
        qcLiquid = 1;
    if(isCalculateP) //计算项目
    {
        calculateProjID = ProjectID;
        projectID = 1;
    }
    else
    {
        calculateProjID = 1;
        projectID = ProjectID;
    }

    //排序按  样本架号*100 + 样本编号  （如：1-1 = 101， 1-10 = 110）
    int samplePosNo = SamplePos.split("-").at(0).toInt() * 100 + SamplePos.split("-").at(1).toInt();
    QString str;
    str =
            "insert into TaskT( no, HistoryT_id, samplePosNo, sampleCode, dateTime, userID, typeID, isCalculateProj, isEmergency, stateID, samplePos, projectID, calculateProjID, QcLiquidID, sampleTypeID, isRetest ) "
            " values('"
            + TaskNo + "',"            //测试编号
            + " 0, "                   //历史结果数据表中的id（即无）
            + " " + QString::number(samplePosNo) + ", " +   //样本架编号(MV200特有属性，方便任务排序)
            + "'" + sampleCode + "',"
            + "'" + date + "', "       //申请日期
            +QString::number( gParameter->getUserID() ) + "," //申请者id
            + QString::number(TestType) +", "  //测试类型（1：样本测试，2：质控测试，3：定标测试）（注：在这里申请的都是样本测试）
            + QString::number(isCalculateP) +" ,"  //是否为计算项目(0：常规项目，1：计算项目)
            + QString::number(isUrgent) + ","//急诊否
            + QString::number(State_WAIT) +","   //”待测试“状态
            + "'"+SamplePos + "',"               //样本位置
            + QString::number(projectID) + ", "  //项目id
            + QString::number(calculateProjID) + ", "  //计算项目id
            + QString::number(qcLiquid) + ", "         //质控液ID
            + QString::number(SampleType) + ", " //样本类型ID
            + "0 "                               //是否重测(默认否)
            +");";
    sqlListStr.append(str);
    //usleep(20);
}

