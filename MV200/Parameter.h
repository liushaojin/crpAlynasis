#ifndef GPARAMETER_H
#define GPARAMETER_H

#include <QObject>
#include <QVector>
#include <QMapIterator>
#include "ParamDefine.h"
#include "Object/SQLite/sqlite.h"

//常规项目 结构体保存各个项目的参数（包括Logic5P、项目其他参数）
struct ProjectParamS
{
    bool isDiluentPorj;       //是否为稀释项目（0：非稀释项目，1：稀释项目）
    QString Param_unit;       //项目单位  （是否有用，待定）
    int Param_accuracy;       //项目精度
    double Param_lowLinear;   //线性下限
    double Param_factorK;     //K因数值
    double Param_bloodFactor; //全血因子（全血模式有效，非全血则为1）
    int Param_lightPos1;      //测光点1
    int Param_lightPos2;      //测光点2

    int Param_Reagent_R0;     //R0试剂ID（试剂表，0：未选择试剂位）
    int Param_Reagent_R1;     //R1试剂ID（试剂表，0：未选择试剂位）
    int Param_Reagent_R2;     //R2试剂ID（试剂表，0：未选择试剂位）

    int Param_SVol;           //样本量（ul）
    int Param_R0Vol;          //探针吸R0量（ul）
    int Param_R1Vol;          //探针吸R1量（ul）
    int Param_RSVol;          //探针吸稀释后的样本量（ul）样本转移量
    int Param_R2Vol;          //探针吸R2量（ul）

    double Param_R0;   //参数R0
    double Param_K;    //参数K
    double Param_a;    //参数a
    double Param_b;    //参数b
    double Param_c;    //参数c
    double Param_R;    //参数-最大幅度
    double Param_C;    //参数-最大浓度

    bool isLogic5P2;   //是否有第二段曲线
    double Param2_R0;   //参数R0
    double Param2_K;    //参数K
    double Param2_a;    //参数a
    double Param2_b;    //参数b
    double Param2_c;    //参数c
    double Param2_R;    //参数-最大幅度
    double Param2_C;    //参数-最大浓度
};
//组合项目
struct CombineProject
{
    //int CombID;                 //组合项目ID
    QString CombProjectNameEn;    //组合项目简称
    QString CombProjectNameCn;    //组合项目全称
    QString CombProjectMember;    //项目成员，以“+”为间隔，如（CCP+hs-CRP）
    QString CombProjectMemberID;  //项目id，以“-”为间隔，如（1-2-3）
};
//计算项目
struct CalculateProject
{
    //int CalculateID;                     //计算项目ID
    QString CalculateProjectNameEn;        //计算项目简称
    QString CalculateProjectNameCn;        //计算项目全称
    QString ProjectList;                   //项目列表，以“+”为分隔 如：TP+hs-CRP
    QString CalculateProjectExpression;    //表达式，如：{TP}-{ALB}
    QString Unit;          //项目单位
    int Accracy;           //小数位数
    double Range1;         //参考范围1
    double Range2;         //参考范围2

    QString CalculateProjectExpressionNO;  //编号表达式，如：1-2
};

//质控项目参数队列
struct QCParam
{
    int Proj_id;         //项目id
    QString ProjName;    //项目名称
    double TargetValue;  //靶值
    double SD;           //标准差
    double TargetUpperL; //靶值上限
    double TargetLowerL; //靶值下限
};
//质控液
struct QCLiquid
{
    int id;                //质控液ID
    QString Name;          //质控液名称
    int SampleType;        //质控液样本类型
    QString BatchNo;       //批号
    QString ValidDate;     //有效期
    QString BarCode;       //条码
    QCParam Param[PROJECT_COUNT]; //项目参数
};

//状态枚举表（对应数据库状态表的id）
enum StateTable
{
    State_IDLE = 1,           //空闲
    State_APPLICATE = 2,      //已申请
    State_WAIT = 3,           //待测试（等待）
    State_TESTING = 4,        //测试中
    State_ADD_DILUENT = 5,    //加稀释液（R0）
    State_ADD_SAMPLE = 6,     //加样本
    State_ADD_R1 = 7,         //加R1
    State_ADD_RS = 8,         //加稀释样本（RS）
    State_ADD_R2 = 9,         //加R2
    State_DIRTY = 10,         //脏杯（反应杯）
    State_FINISHED = 11,      //完成（测试完成）
    State_CLEAN = 12,         //清洗（反应杯）
    State_ERROR = 13,         //出错
    State_BLANK = 14,         //空跑
    State_unFINISHED = 15,    //未完成
    State_noSAMPLE = 16,      //缺少样本
    State_END1 = 17,          //测试加样完成（END1）
    State_END2 = 18,          //测试采光完成（END2）
    State_BLANKOUT = 19,      //取消/作废
    State_noREAGENT = 20      //缺少试剂（20170706新增）

    //有新状态，则往后添加
};

class GParameter : public QObject
{
    Q_OBJECT
public:
    explicit GParameter(QObject *parent = 0);

    /*************************************************************************************************************************/
    /**Build类型（发布版、研发版）**/
    bool get_isRelease();    //0:研发版，即供公司内部使用。1：发布版，即供客户端使用。
    void set_isRelease(bool flag);

    /*************************************************************************************************************************/
    /**用户信息**/
    QString getCompanyName_En(); //公司英文名称
    QString getCompanyName_Cn(); //公司中文名称
    void setUserName(QString);   //设置用户名
    QString getUserName();       //获取用户名
    void setUserID(int id);      //设置用户ID
    int getUserID();             //获取用户ID
    QString getHospitalName();   //获取医院名称（打印表头）
    void setHospitalName(QString name);
    QString getMachineName();    //机器名称，MV200

    QString getTCPServerIP();             //tcp服务器ip
    void setTCPServerIP(QString ip);
    int getTCPServerPort();               //端口
    int setTCPServerPort(int port);

    int get_isLISAutoStart();    //是否开机自动连接LIS服务器
    void set_isLISAutoStart(int flag);
    QString get_LIS_IP();      //LIS ip
    void set_LIS_IP(QString ip);
    int get_LIS_Port();        //LIS 端口
    void set_LIS_Port(int port);
    int get_ACKTime();         //ACK超时
    void set_ACKTime(int time);

    /*************************************************************************************************************************/
    /**开机流程**/
    void  set_isSelfCheck(int isCheck); //设置开机是否自检
    int get_isSelfCheck();              //获取开机是否自检

    /*************************************************************************************************************************/
    /**项目信息**/
    ProjectParamS getProjectParam(int id);            //查询id项目的项目参数
    void setProjectParam(int id, ProjectParamS param);//新增/修改项目参数
    void removeProjectParam(int id);                  //移除项目参数（删除项目时用到）
    QVector<QString> get_ProjectName();           //MV100测试项目名称
    QMap<int, QString> getProjectMap();           //项目名称<id, 项目>
    void setProjectMap(QMap<int, QString> map);
    int appendProjectMap(int id, QString name);  //追加项目
    int removeProjectMap(int id);                //移除Map中的项目
    int getProject_id(QString project);
    QString getPorject_Name(int id);
    QMap<int, int> getMapReagentSlot();   //试剂仓信息映射<试剂瓶号，项目id>
    void setMapReagentSlot(QMap<int, int> map);
    void setMapReagentSlot(int key, int value);//设置key位置的项目id为value
    int getMapReagentSlotValue(int rNo);  //获取试剂位对应的项目ID
    void ReleaseReagentNo(int rNo);       //释放rNo试剂位
    int ReleaseAllReagent();              //释放全部试剂位
    bool getReagentNoState(int rNo);      //查看rNo试剂位的状态（是否已经占用,0:未占用，1:已占用）
    int getReagentPosCount(int pID);      //查看项目对应的试剂位置个数

    //组合项目
    QString getCombProjectMember(int id);   //获取组合项目的成员名称组合
    QString getCombProjectMemberID(int id); //获取组合项目的成员项目id组合
    QVector<QString> getCombineProjectName();                //组合项目的名称
    void setCombineProjectMap(QMap<int, CombineProject> map);//组合项目
    void setCombineProject(int id, CombineProject param);    //新增/修改组合项目
    int removeCombineProject(int id);                        //移除组合项目
    int isComgineProjectExist(QString name); //查看name组合项目是否已经存在（若存在，则返回组合项目的id）

    //计算项目
    int getCalculateProjectAccracy(int id);          //计算项目的精度（小数位数）
    int getCalculateProject_id(QString name);        //获取计算项目的id
    QString getCalculateProjectExpression(int id);   //获取计算项目表达式
    QStringList getCalculateProjectList(int id);     //获取计算项目的子项目列表
    QString getCalculateProjectExpressionNO(int id); //获取计算项目编号表达式(未使用)
    QVector<QString> getCalculateProjectName(); //计算项目的名称
    CalculateProject getCalculateProjectParam(int id); //获取指定id的计算项目的参数
    void setCalculateProjectMap(QMap<int, CalculateProject> map);//计算项目
    void setCalculateProject(int id, CalculateProject param);    //新增/修改计算项目
    int removeCalculateProject(int id);                        //移除计算项目
    int isCalculateProjectExist(QString name); //查看name计算项目是否已经存在（若存在，则返回计算项目的id）

    //质控液
    void ResetQcLiquidMap();   //重置质控液映射
    void setQCLiquidMap(QMap<int, QCLiquid> map);  //质控液（质控液id， 质控液参数）
    QMap<int, QCLiquid> getQCLiquidMap();
    QStringList getQcLiquidName();                 //质控液名称
    int insertQcLiquidMap(int id, QCLiquid qc);    //添加质控液参数
    int removeQcLiquidMap(int id);                 //移除质控液
    int updateQcLiquidMap(int id, QCLiquid param); //更新质控液参数
    QCLiquid getQCParam(int id);                   //相应质控液的参数
    QCParam *getQcProjParam(int id);               //质控液对应的项目参数
    QStringList getQcProject(QString name);        //质控液对应的项目名称
    int getQcLiquidID(QString name);               //质控液id


    //加样针到试剂瓶底的步数
    void setProbe2Bottom(int step);
    int getProbe2Bottom();

    QStringList getSampleTypeList();      //样本类型
    void setSampleTypeList(QStringList list);
    int getSampleNo();                    //样本编号（记录作用）
    void setSampleNo(int no);



    /*************************************************************************************************************************/
    /**周期参数**/
    int getCycleTime();          //每个周期的时间长度
    void setCycleTime(int time);

    /*************************************************************************************************************************/
    /**系统参数**/
    bool get_isLiquidAlarmStop();       //液体状态报警是否要停机
    void set_isLiquidAlarmStop(bool flag);
    int get_LessWaterTimes();           //提示“缺少去离子水”后最多能运行的次数
    void set_LessWaterTimes(int times);
    int get_LessCleanLiquidTimes();     //提示“缺少清洗液”后最多能运行的次数
    void set_LessCleanLiquidTimes(int times);
    int get_Full_L_WasteTimes();        //提示“低浓度废液满”后最多能运行的次数
    void set_Full_L_WasteTimes(int times);
    int get_Full_H_WasteTimes();        //提示“高浓度废液满”后最多能运行的次数
    void set_Full_H_WasteTimes(int times);

    bool get_isAutoSaveResult();        //结果是否自动保存到数据库
    void set_isAutoSaveResult(bool flag);
    void setCupDirtyAD(int value);      //设置脏杯的AD值
    int getCupDirtyAD();

    //清洗机构-清水吸液量（但实际上是控泵阀的开关时间）
    int getWaterVol();
    void setWaterVol(int vol);
    //清洗机构-清液吸液量（但实际上是控泵阀的开关时间）
    int getCleanLiquidVol();
    void setCleanLiquidVol(int vol);


private:
    /*************************************************************************************************************************/
    /**Build类型（发布版、研发版）**/
    bool isRelease;    //0:研发版，即供公司内部使用。1：发布版，即供客户端使用。

    /*************************************************************************************************************************/
    /**用户信息**/
    QString UserName;    //用户名
    int UserID;          //用户ID
    QString HospitalName;//获取医院名称
    QString MachineName; //机器名称，MV200
    QString CompanyName; //公司名称(英文)
    QString CompanyName2;//公司名称(中文)

    int Port;            //端口（中位机）
    QString Host_IP;     //tcp服务器ip（中位机）
    QString LIS_IP;      //LIS ip
    int LIS_Port;        //LIS 端口
    int ACKTime;         //ACK超时
    int isLISAutoStart;  //是否开机自动连接LIS服务器

    /*************************************************************************************************************************/
    /**开机流程**/
    int isSelfCheck;     //开机是否自检

    /*************************************************************************************************************************/
    /**项目信息**/
    //常规项目
    QMap<int, QString> ProjectMap;                   //项目名称 <id, 项目>
    QMap<int, ProjectParamS> *MapProjectParamS;  //项目参数映射<项目id， 参数表>
    //有点重复 ？？ （在项目参数中包含了， 待修改）
    QMap<int, int> MapReagentSlot;   //试剂仓信息映射<试剂瓶号，项目id> 目前用处：在设置项目对应的试剂位置时，不允许不同项目使用同一个试剂位

    //组合项目
    QMap<int, CombineProject> CombineProjectMap;     //组合项目（项目id， 项目参数）

    //计算项目
    QMap<int, CalculateProject> CalculateProjectMap; //计算项目（项目id， 项目参数）

    //质控液
    QCParam *QcParam;                                //质控液对应的项目参数
    QMap<int, QCLiquid> QCLiquidMap;                 //质控液（质控液id， 质控液参数）
    QStringList QcLiquidNameList;                    //质控液名称列表


    //加样针复位位置到试剂瓶底的步数（计算试剂余量）
    int Probe2Bottom;

    QStringList SampleTypeList;  //样本类型
    int SampleNo;                //样本编号（记录作用,申请样本测试用）

    /*************************************************************************************************************************/
    /**周期参数**/
    int CycleTime;       //每个周期的时间长度

    /*************************************************************************************************************************/
    /**系统参数**/
    bool isLiquidAlarmStop;       //液体状态报警是否要停机
    int LessWaterTimes;           //提示“缺少去离子水”后最多能运行的次数
    int LessCleanLiquidTimes;     //提示“缺少清洗液”后最多能运行的次数
    int Full_L_WasteTimes;        //提示“低浓度废液满”后最多能运行的次数
    int Full_H_WasteTimes;        //提示“高浓度废液满”后最多能运行的次数

    bool isAutoSaveResult;   //结果是否自动保存到数据库
    int CupDirtyAD;          //脏杯的AD值

    //清洗机构-清水吸液量
    int WaterVol;
    //清洗机构-清液吸液量
    int CleanLiquidVol;

signals:
    void sig_newProject();          //信号：添加了新的测试项目
    void sig_newCombineProject();   //信号：添加了新的组合项目
    void sig_newCalculateProject(); //信号：添加了新的计算项目
    void sig_UpdateQcLiquidName();  //信号：更新质控液列表

public slots:
};

extern GParameter * gParameter;
#endif // GPARAMETER_H
