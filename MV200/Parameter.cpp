#include "Parameter.h"
#include <QSettings>

GParameter *gParameter;
GParameter::GParameter(QObject *parent) : QObject(parent)
{
    //仪器名称（如MV200, LF200）
    //读取200.ini
    QSettings *gSetting = new QSettings("icon/200.ini", QSettings::IniFormat);
    gSetting->setIniCodec("GB2312"); //设置为GB2312,否则中文乱码
    gSetting->beginGroup("MachineName");
    MachineName = gSetting->value("name").toString();         //MV100
    CompanyName = gSetting->value("CompanyName").toString();  //MyVision
    CompanyName2 = gSetting->value("CompanyName2").toString();//中山迈盛医疗有限公司
    gSetting->endGroup();
    qDebug() << endl << "MachineName:" << MachineName << CompanyName << CompanyName2;

    QString str;
    QSqlQuery  query;

    //项目名称&项目参数
    query.clear();
    QMap<int, QString> map;
    QMap<int, int> mapReagentSlot;
    MapProjectParamS = new QMap<int, ProjectParamS>;
    str  = " select id, name, unit, accuracy, lowLinear, K, bloodFactor, lightPos1, lightPos2, "
           " r0ID, r1ID, r2ID, "
           " sample_V, r0_V, r1_V, rs_V, r2_V, "
           " calibrate1_R0,calibrate1_K,calibrate1_a,calibrate1_b,calibrate1_c,calibrate1_RR,calibrate1_CC,"
           " isCalibrate2,calibrate2_R0,calibrate2_K,calibrate2_a,calibrate2_b,calibrate2_c,calibrate2_RR,calibrate2_CC, "
           " isDiluent from ProjectT;";
    query = SQLite::getInstance()->Select(str);
    int Pid;
    while(query.next())
    {
        Pid = query.value(0).toInt(); //项目ID
        //项目<id,  项目名称>
        map.insert( Pid, query.value(1).toString() );

        //项目参数
        ProjectParamS param;
        param.Param_unit = query.value(2).toString();        //项目单位  （是否有用，待定）
        param.Param_accuracy = query.value(3).toInt();       //项目精度
        param.Param_lowLinear = query.value(4).toDouble();   //线性下限
        param.Param_factorK = query.value(5).toDouble();     //K因数值
        param.Param_bloodFactor = query.value(6).toDouble(); //全血因子（全血模式有效，非全血则为1）
        param.Param_lightPos1 = query.value(7).toInt();      //测光点1
        param.Param_lightPos2 = query.value(8).toInt();      //测光点2

        param.Param_Reagent_R0 = query.value(9).toInt();      //R0试剂ID（试剂表，0：未选择试剂位）
        param.Param_Reagent_R1 = query.value(10).toInt();     //R1试剂ID（试剂表，0：未选择试剂位）
        param.Param_Reagent_R2 = query.value(11).toInt();     //R2试剂ID（试剂表，0：未选择试剂位）
        param.Param_SVol = query.value(12).toInt();      //样本量（ul）
        param.Param_R0Vol = query.value(13).toInt();     //探针吸R0量（ul）
        param.Param_R1Vol = query.value(14).toInt();     //探针吸R1量（ul）
        param.Param_RSVol = query.value(15).toInt();     //探针吸稀释后的样本量（ul）样本转移量
        param.Param_R2Vol = query.value(16).toInt();     //探针吸R2量（ul）

        //定标曲线1
        param.Param_R0 = query.value(17).toDouble();   //参数R0
        param.Param_K = query.value(18).toDouble();    //参数K
        param.Param_a = query.value(19).toDouble();    //参数a
        param.Param_b = query.value(20).toDouble();    //参数b
        param.Param_c = query.value(21).toDouble();    //参数c
        param.Param_R = query.value(22).toDouble();    //参数-最大幅度
        param.Param_C = query.value(23).toDouble();    //参数-最大浓度

        //定标曲线2
        param.isLogic5P2 = query.value(24).toInt();
        param.Param2_R0 = query.value(25).toDouble();   //参数R0
        param.Param2_K = query.value(26).toDouble();    //参数K
        param.Param2_a = query.value(27).toDouble();    //参数a
        param.Param2_b = query.value(28).toDouble();    //参数b
        param.Param2_c = query.value(29).toDouble();    //参数c
        param.Param2_R = query.value(30).toDouble();    //参数-最大幅度
        param.Param2_C = query.value(31).toDouble();    //参数-最大浓度

        //是否为稀释项目
        param.isDiluentPorj = query.value(32).toInt();  //是否为稀释项目
        MapProjectParamS->insert(Pid, param);

        mapReagentSlot.insert(param.Param_Reagent_R0-1, Pid);
        mapReagentSlot.insert(param.Param_Reagent_R1-1, Pid);
        mapReagentSlot.insert(param.Param_Reagent_R2-1, Pid);
    }
    setProjectMap(map);
    setMapReagentSlot(mapReagentSlot);
    qDebug() << map;

    //组合项目
    query.clear();
    str = "SELECT id, nameEn, nameCn, pMember, pID from CombineProjectT;";
    query = SQLite::getInstance()->Select(str);
    CombineProject combP;
    int combPid;
    while (query.next())
    {
        combPid = query.value(0).toInt();
        combP.CombProjectNameEn =  query.value(1).toString();
        combP.CombProjectNameCn =  query.value(2).toString();
        combP.CombProjectMember =  query.value(3).toString();
        combP.CombProjectMemberID =  query.value(4).toString();
        setCombineProject(combPid, combP);
    }

    //计算项目
    query.clear();
    str = "SELECT id, nameEn, nameCn, unit, accuracy, expression, expression_n, "
          "rangeA1, rangeA2, rangeB1, rangeB2, rangeC1, rangeC2, projectList from CalculateProjectT "
          " where id > 1;";
    query = SQLite::getInstance()->Select(str);
    CalculateProject calculateParam;
    int calculateID;
    while (query.next())
    {
        calculateID = query.value(0).toInt();
        calculateParam.CalculateProjectNameEn = query.value(1).toString();        //计算项目简称
        calculateParam.CalculateProjectNameCn = query.value(2).toString();        //计算项目全称
        calculateParam.Unit = query.value(3).toString();          //项目单位
        calculateParam.Accracy = query.value(4).toInt();          //小数位数
        calculateParam.CalculateProjectExpression = query.value(5).toString();    //表达式，如：{TP}-{ALB}
        calculateParam.CalculateProjectExpressionNO = query.value(6).toString();  //编号表达式，如：1-2
        calculateParam.Range1 = query.value(7).toDouble();         //参考范围1
        calculateParam.Range2 = query.value(8).toDouble();         //参考范围2
        calculateParam.ProjectList = query.value(13).toString();    //项目列表
        setCalculateProject(calculateID, calculateParam);
        qDebug() << "id:" << calculateID << "name:" << calculateParam.CalculateProjectNameEn << ", ProjectList:" << calculateParam.ProjectList ;
    }

    //质控液
    QcParam = new QCParam[PROJECT_COUNT]; //质控液对应的项目参数
    ResetQcLiquidMap();

    //样本类型
    query.clear();
    QStringList sampleTypeList;
    str  = "select name from SampleTypeT;";
    query = SQLite::getInstance()->Select(str);
    while(query.next())
        sampleTypeList.append( query.value(0).toString() );
    setSampleTypeList(sampleTypeList);

    //系统参数表（SysParamT）
    query.clear();
    str  = "select CupDirtyAD,isLiquidAlarm,lessWaterTimes,lessCleanLiquidTimes,full_L_WasteTimes,full_H_WasteTimes,hospitalName,isAutoTransfer,SampleNO from SysParamT;";
    query = SQLite::getInstance()->Select(str);
    while(query.next())
    {
        CupDirtyAD = query.value(0).toInt();
        isLiquidAlarmStop = query.value(1).toBool();
        LessWaterTimes = query.value(2).toInt();
        LessCleanLiquidTimes = query.value(3).toInt();
        Full_L_WasteTimes = query.value(4).toInt();
        Full_H_WasteTimes = query.value(5).toInt();
        HospitalName = query.value(6).toString();
        isAutoSaveResult = query.value(7).toBool();
        SampleNo = query.value(8).toInt();
    }
    query.clear();

    //LIS参数表
    str = "SELECT isAutoStart, IP, LIS_IP, LISPort, ACKTime, SubnetMask from LISParamT;";
    query = SQLite::getInstance()->Select(str);
    while (query.next())
    {
        //是否开机自动连接LIS服务器
        isLISAutoStart = query.value(0).toInt();
        //tcp服务器ip（中位机）
        Host_IP = query.value(1).toString();
        //LIS ip
        LIS_IP = query.value(2).toString();
        //LIS 端口
        LIS_Port = query.value(3).toInt();
        //ACK超时
        ACKTime = query.value(4).toInt();
        //端口（中位机）
        Port = query.value(5).toInt();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    //测试
    setCycleTime( 40000 ); //每个周期的时间长度
    setProbe2Bottom(12000);//加样针到试剂瓶底的步数

    //清洗机构-清水吸液量（但实际上是控泵阀的开关时间）
    setWaterVol(800);
    //清洗机构-清液吸液量（但实际上是控泵阀的开关时间）
    setCleanLiquidVol(600);

}

/*************************************************************************************************************************/
/**Build类型（发布版、研发版）**/
//0:研发版，即供公司内部使用。1：发布版，即供客户端使用。
bool GParameter::get_isRelease()  {return isRelease;}
void GParameter::set_isRelease(bool flag){isRelease = flag;}

/*************************************************************************************************************************/
/**用户信息**/

//公司英文名称
QString GParameter::getCompanyName_En() {return CompanyName;}
//公司中文名称
QString GParameter::getCompanyName_Cn() {return CompanyName2;}

//设置用户名
void GParameter::setUserName(QString name)  {UserName = name;}
QString GParameter::getUserName()           {return UserName;}
//设置用户ID
void GParameter::setUserID(int id)   {UserID = id;}
int GParameter::getUserID()          {return UserID;}

//获取医院名称（打印表头）
QString GParameter::getHospitalName() {return HospitalName;}
void GParameter::setHospitalName(QString name) {HospitalName = name;}
//机器名称，MV200
QString GParameter::getMachineName() { return MachineName; }

//tcp服务器ip
QString GParameter::getTCPServerIP() { return Host_IP; }
void GParameter::setTCPServerIP(QString ip) {Host_IP=ip;}
//端口
int GParameter::getTCPServerPort() { return Port; }
int GParameter::setTCPServerPort(int port) {Port = port;}

//是否开机自动连接LIS服务器
int GParameter::get_isLISAutoStart() {return isLISAutoStart;}
void GParameter::set_isLISAutoStart(int flag) {isLISAutoStart=flag;}
//LIS ip
QString GParameter::get_LIS_IP() {return LIS_IP.trimmed();}
void GParameter::set_LIS_IP(QString ip) {LIS_IP = ip;}
//LIS 端口
int GParameter::get_LIS_Port() {return LIS_Port;}
void GParameter::set_LIS_Port(int port) {LIS_Port = port;}
//ACK超时
int GParameter::get_ACKTime()          {return ACKTime;}
void GParameter::set_ACKTime(int time) {ACKTime = time;}

/*************************************************************************************************************************/
/**开机流程**/
//设置开机是否自检
void GParameter::set_isSelfCheck(int isCheck) {isSelfCheck = isCheck;}
int GParameter::get_isSelfCheck()             {return isSelfCheck;}

/*************************************************************************************************************************/
/**项目信息**/
//测试项目名称

//查询id项目的项目参数
ProjectParamS GParameter::getProjectParam(int id)
{
    if( MapProjectParamS->contains(id) )
        return MapProjectParamS->value(id);
}
//新增/修改项目参数
void GParameter::setProjectParam(int id, ProjectParamS param) {MapProjectParamS->insert(id, param);}
//移除项目参数（删除项目时用到）
void GParameter::removeProjectParam(int id)
{
    if( MapProjectParamS->contains(id) )
        MapProjectParamS->remove(id);
}
//测试项目名称
QVector<QString> GParameter::get_ProjectName()
{
    QVector<QString> list;
    QMap<int, QString>::iterator it = ProjectMap.begin();//迭代器
    while(it != ProjectMap.end())
    {
        list.append(it.value());
        it ++;
    }
    return list;
}
//项目名称<id, 项目>
QMap<int, QString> GParameter::getProjectMap()  {return ProjectMap;}
void GParameter::setProjectMap(QMap<int, QString> map)
{
    ProjectMap.clear();
    ProjectMap = map;
}
//追加项目
int GParameter::appendProjectMap(int id, QString name)
{
    if(! ProjectMap.contains(id))   //map中不存在key值时允许添加
    {
        ProjectMap.insert(id, name);
        emit sig_newProject();   //信号：添加了新的测试项目
    }
}
//移除Map中的项目
int GParameter::removeProjectMap(int id)
{
    if(ProjectMap.contains(id))
        ProjectMap.remove(id);
    emit sig_newProject();   //信号：添加了新的测试项目
    return 1;
}
int GParameter::getProject_id(QString project)
{
    int id = 0;
    id = ProjectMap.key(project);
    return id;
}
QString GParameter::getPorject_Name(int id)
{
    QString name = "";
    if( ProjectMap.contains(id) )
        name = ProjectMap.value(id);
    return name;
}

//试剂仓信息映射<试剂瓶号，项目id>
QMap<int, int> GParameter::getMapReagentSlot()         { return MapReagentSlot; }
void GParameter::setMapReagentSlot(QMap<int, int> map) { MapReagentSlot = map; }
//设置key位置的项目id为value
void GParameter::setMapReagentSlot(int key, int value)
{
    key = key-1;
    if(key < REAGENT_COUNT)
        MapReagentSlot.insert(key, value);
}
//获取试剂位对应的项目ID
int GParameter::getMapReagentSlotValue (int rNo)
{
    if(MapReagentSlot.contains(rNo-1))
        return MapReagentSlot.value(rNo-1);
    else
        return 0; //未选择
}
//释放rNo试剂位
void GParameter::ReleaseReagentNo(int rNo)
{
    if(MapReagentSlot.contains(rNo-1))
        MapReagentSlot.insert(rNo-1, 0);
}
//释放全部试剂位
int GParameter::ReleaseAllReagent()
{
    for(int i = 0; i < REAGENT_COUNT; i ++)
        MapReagentSlot.insert(i, 0);  //默认“0”为该试剂位尚未被使用
    return 1;
}
//查看rNo试剂位的状态（是否已经占用, 0:未占用，1:已占用）
bool GParameter::getReagentNoState(int rNo)
{
    if(MapReagentSlot.contains(rNo-1))
    {
        if(MapReagentSlot.value(rNo-1) != 0)
            return 1;
        else
            return 0;
    }
    else
        return 0;
}
//查看项目对应的试剂位置个数
int GParameter::getReagentPosCount(int pID)
{
    int count = 0;
    for(int i = 0; i < REAGENT_COUNT; i ++)
    {
        if(MapReagentSlot.value(i) == pID)
            count ++;
    }
    return count;
}


//获取组合项目的成员名称组合
QString GParameter::getCombProjectMember(int id)
{
    //组合项目id存在
    if( CombineProjectMap.contains(id) )
        return CombineProjectMap.value(id).CombProjectMember;//项目成员，以“+”为间隔，如（CCP+hs-CRP）
    else
        return "";//组合项目不存在，返回空
}
//获取组合项目的成员项目id组合
QString GParameter::getCombProjectMemberID(int id)
{
    //组合项目id存在
    if( CombineProjectMap.contains(id) )
        return CombineProjectMap.value(id).CombProjectMemberID; //项目id，以“-”为间隔，如（1-2-3）
    else
        return "";//组合项目不存在，返回空
}
//组合项目的名称
QVector<QString> GParameter::getCombineProjectName()
{
    QVector<QString> list;
    QMap<int, CombineProject>::iterator it = CombineProjectMap.begin();//迭代器
    while(it != CombineProjectMap.end())
    {
        list.append(CombineProjectMap.value(it.key()).CombProjectNameEn);
        it ++;
    }
    return list;
}
//设置组合项目
void GParameter::setCombineProjectMap(QMap<int, CombineProject> map) { CombineProjectMap = map; }
//新增/修改组合项目
void GParameter::setCombineProject(int id, CombineProject param)
{
    bool flag = false;
    //判断是否是新增
    if(!CombineProjectMap.contains(id)) flag = true;

    CombineProjectMap.insert(id, param);

    //新增要更新界面
    if(flag)
        emit sig_newCombineProject(); //信号：添加了新的组合项目
}
//移除组合项目
int GParameter::removeCombineProject(int id)
{
    if(CombineProjectMap.contains(id))
        CombineProjectMap.remove(id);
    emit sig_newCombineProject(); //信号：添加了新的组合项目
    return 1;
}
//查看name组合项目是否已经存在（若存在，则返回组合项目的id）（只查简称)
int GParameter::isComgineProjectExist(QString name)
{
    QMap<int, CombineProject>::iterator it = CombineProjectMap.begin();//迭代器
    while(it != CombineProjectMap.end())
    {
        if( name == CombineProjectMap.value(it.key()).CombProjectNameEn )
            return it.key();
        it ++;
    }

    return 0;
}


//计算项目
//计算项目的精度（小数位数）
int GParameter::getCalculateProjectAccracy(int id)
{
    //项目id存在
    if( CalculateProjectMap.contains(id) )
        return CalculateProjectMap.value(id).Accracy;
    else
        return 0;//项目不存在，返回空
}
//获取计算项目的id
int GParameter::getCalculateProject_id(QString name)
{
    QMap<int, CalculateProject>::iterator it = CalculateProjectMap.begin();//迭代器
    while(it != CalculateProjectMap.end())
    {
        if( name == CalculateProjectMap.value(it.key()).CalculateProjectNameEn )
            return it.key();
        it ++;
    }
    return 0;
}
//获取计算项目表达式
QString GParameter::getCalculateProjectExpression(int id)
{
    //项目id存在
    if( CalculateProjectMap.contains(id) )
        return CalculateProjectMap.value(id).CalculateProjectExpression;
    else
        return "";//项目不存在，返回空
}
//获取计算项目的子项目列表
QStringList GParameter::getCalculateProjectList(int id)
{
    QStringList list;
    //项目id存在
    if( CalculateProjectMap.contains(id) )
        list = CalculateProjectMap.value(id).ProjectList.split("+");
    return list;
}
//获取计算项目编号表达式(未使用)
QString GParameter::getCalculateProjectExpressionNO(int id)
{
    //项目id存在
    if( CalculateProjectMap.contains(id) )
        return CalculateProjectMap.value(id).CalculateProjectExpressionNO;
    else
        return "";//组合项目不存在，返回空

}
//计算项目的名称
QVector<QString> GParameter::getCalculateProjectName()
{
    QVector<QString> list;
    QMap<int, CalculateProject>::iterator it = CalculateProjectMap.begin();//迭代器
    while(it != CalculateProjectMap.end())
    {
        list.append(CalculateProjectMap.value(it.key()).CalculateProjectNameEn);
        it ++;
    }
    return list;
}
//获取指定id的计算项目的参数
CalculateProject GParameter::getCalculateProjectParam(int id)
{
    if(CalculateProjectMap.contains(id))
        return CalculateProjectMap.value(id);
    else
    {
        qDebug() << "CalculateProject == null, " << " function:" << __func__  << " ,Line:" << __LINE__;
        CalculateProject param;
        return param;
    }
}
//计算项目
void GParameter::setCalculateProjectMap(QMap<int, CalculateProject> map) {CalculateProjectMap = map;}
//新增/修改计算项目
void GParameter::setCalculateProject(int id, CalculateProject param)
{
    bool flag = false;
    //判断是否是新增
    if(!CalculateProjectMap.contains(id)) flag = true;

    CalculateProjectMap.insert(id, param);

    //新增要更新界面
    if(flag)
        emit sig_newCalculateProject(); //信号：添加了新的计算项目
}
//移除计算项目
int GParameter::removeCalculateProject(int id)
{
    if(CalculateProjectMap.contains(id))
        CalculateProjectMap.remove(id);
    emit sig_newCalculateProject(); //信号：添加了新的计算项目
    return 1;
}
//查看name计算项目是否已经存在（若存在，则返回计算项目的id）
int GParameter::isCalculateProjectExist(QString name)
{
    QMap<int, CalculateProject>::iterator it = CalculateProjectMap.begin();//迭代器
    while(it != CalculateProjectMap.end())
    {
        if( name == CalculateProjectMap.value(it.key()).CalculateProjectNameEn )
            return it.key();
        it ++;
    }

    return 0;
}


//重置质控液映射
void GParameter::ResetQcLiquidMap()
{
    QcLiquidNameList.clear();
    QCLiquidMap.clear();
    for(int i = 0; i< PROJECT_COUNT; i ++)
        QcParam[i].Proj_id = 0;

    QSqlQuery query;
    query.clear();
    QString str = "SELECT id, name, sampleType, batchNo, validDate, barCode from QCLiquidT  WHERE id > 1;";
    query = SQLite::getInstance()->Select(str);
    while (query.next())
    {
        QCLiquid qc;
        qc.id = query.value(0).toInt();
        qc.Name = query.value(1).toString();
        qc.SampleType = query.value(2).toInt();
        qc.BatchNo = query.value(3).toString();
        qc.ValidDate = query.value(4).toString();
        qc.BarCode = query.value(5).toString();

        str = "SELECT projectID, projectName, targetValue, SD, TargetValue0, TargetValue1 from QCParamT "
              " WHERE  QCLiquidID="+ QString::number(qc.id)  +";";
        QSqlQuery query1 = SQLite::getInstance()->Select(str);
        int i = 0;
        while (query1.next())
        {
            qc.Param[i].Proj_id = query1.value(0).toInt();         //项目id
            qc.Param[i].ProjName = query1.value(1).toString();     //项目名称
            qc.Param[i].TargetValue = query1.value(2).toDouble();  //
            qc.Param[i].SD = query1.value(3).toDouble();           //
            qc.Param[i].TargetUpperL = query1.value(4).toDouble(); //
            qc.Param[i].TargetLowerL = query1.value(5).toDouble(); //
            qDebug() << "质控液：" << qc.Name << qc.Param[i].ProjName;
            i ++;
        }
        for( i; i< PROJECT_COUNT; i ++)
            qc.Param[i].Proj_id = 0;

        insertQcLiquidMap(qc.id, qc);
        QcLiquidNameList.append(qc.Name);

    }

    //信号：更新质控液列表
    emit sig_UpdateQcLiquidName();
}
//质控液（质控液id， 质控液参数）
void GParameter::setQCLiquidMap(QMap<int, QCLiquid> map)  {QCLiquidMap = map;}
QMap<int, QCLiquid> GParameter::getQCLiquidMap()  {return QCLiquidMap;}
//质控液名称
QStringList GParameter::getQcLiquidName() { return QcLiquidNameList; }
//添加质控液参数
int GParameter::insertQcLiquidMap(int id, QCLiquid qc)
{
    QCLiquidMap.insert(id, qc);
    return 1;
}
//移除质控液
int GParameter::removeQcLiquidMap(int id)
{
    if(QCLiquidMap.contains(id))
    {
        QCLiquidMap.remove(id);
        return 1;
    }
    return 0;
}
//更新质控液参数
int GParameter::updateQcLiquidMap(int id, QCLiquid param)
{
    if(QCLiquidMap.contains(id))
    {
        QCLiquidMap.insert(id, param);
        return 1;
    }
    return 0;
}
//相应质控液的参数
QCLiquid GParameter::getQCParam(int id)
{
    if(QCLiquidMap.contains(id))
        return QCLiquidMap.value(id);
}
//质控液对应的项目参数
QCParam *GParameter::getQcProjParam(int id)
{
    for(int i = 0; i< PROJECT_COUNT; i ++)
        QcParam[i].Proj_id = 0;

    if(QCLiquidMap.contains(id))
    {
        for(int i = 0; i< PROJECT_COUNT; i ++)
            QcParam[i] = QCLiquidMap.value(id).Param[i];
    }
    return QcParam;
}
//质控液对应的项目名称
QStringList GParameter::getQcProject(QString name)
{
    QStringList list;
    list.clear();

    int id;
    QMap<int, QCLiquid>::iterator it = QCLiquidMap.begin();//迭代器
    while(it != QCLiquidMap.end())
    {
        if(it.value().Name == name)
        {
            id = it.value().id;
            if(QCLiquidMap.contains(id))
            {
                for(int i = 0; i< PROJECT_COUNT; i ++)
                {
                    if(it.value().Param[i].Proj_id > 0)
                        list.append( QCLiquidMap.value(id).Param[i].ProjName );
                }
            }
        }
        it ++;
    }
    return list;
}
//质控液id
int GParameter::getQcLiquidID(QString name)
{
    int id;
    QMap<int, QCLiquid>::iterator it = QCLiquidMap.begin();//迭代器
    while(it != QCLiquidMap.end())
    {
        if(it.value().Name == name)
        {
            id = it.value().id;
            if(QCLiquidMap.contains(id))
                return id;
        }
        it ++;
    }
    return 0;
}









//加样针到试剂瓶底的步数
void GParameter::setProbe2Bottom(int step) {Probe2Bottom = step;}
int GParameter::getProbe2Bottom()  {return Probe2Bottom;}


//样本编号（记录作用）
int GParameter::getSampleNo() { return SampleNo; }
void GParameter::setSampleNo(int no) { SampleNo = no; }
//样本类型
QStringList GParameter::getSampleTypeList() { return SampleTypeList; }
void GParameter::setSampleTypeList(QStringList list)  { SampleTypeList = list;}

/*************************************************************************************************************************/
/**周期参数**/
//每个周期的时间长度
int GParameter::getCycleTime()           {return CycleTime;}
void GParameter::setCycleTime(int time)  {CycleTime = time;}

/*************************************************************************************************************************/
/**系统参数**/
//液体状态报警是否要停机
bool GParameter::get_isLiquidAlarmStop(){return isLiquidAlarmStop;}
void GParameter::set_isLiquidAlarmStop(bool flag) { isLiquidAlarmStop = flag;}
//提示“缺少去离子水”后最多能运行的次数
int GParameter::get_LessWaterTimes() {return LessWaterTimes;}
void GParameter::set_LessWaterTimes(int times) {LessWaterTimes = times;}
//提示“缺少清洗液”后最多能运行的次数
int GParameter::get_LessCleanLiquidTimes() {return LessCleanLiquidTimes;}
void GParameter::set_LessCleanLiquidTimes(int times) {LessCleanLiquidTimes = times;}
//提示“低浓度废液满”后最多能运行的次数
int GParameter::get_Full_L_WasteTimes() {return Full_L_WasteTimes;}
void GParameter::set_Full_L_WasteTimes(int times) {Full_L_WasteTimes = times;}
//提示“高浓度废液满”后最多能运行的次数
int GParameter::get_Full_H_WasteTimes() {return Full_H_WasteTimes;}
void GParameter::set_Full_H_WasteTimes(int times) {Full_H_WasteTimes = times;}

//结果是否自动保存到数据库
bool GParameter::get_isAutoSaveResult() {return isAutoSaveResult;}
void GParameter::set_isAutoSaveResult(bool flag) {isAutoSaveResult=flag;}

//设置脏杯的AD值
void GParameter::setCupDirtyAD(int value){ CupDirtyAD = value; }
int GParameter::getCupDirtyAD() { return CupDirtyAD; }


//清洗机构-清水吸液量（但实际上是控泵阀的开关时间）
int GParameter::getWaterVol() {return WaterVol;}
void GParameter::setWaterVol(int vol) {WaterVol = vol;}

//清洗机构-清液吸液量（但实际上是控泵阀的开关时间）
int GParameter::getCleanLiquidVol() {return CleanLiquidVol;}
void GParameter::setCleanLiquidVol(int vol) {CleanLiquidVol = vol;}

