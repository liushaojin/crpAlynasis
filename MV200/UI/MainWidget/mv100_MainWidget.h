#ifndef MV100_MAINWIDGET_H
#define MV100_MAINWIDGET_H

#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <qfile.h>
#include <QLabel>
#include <QMovie>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QDesktopWidget>
#include <QApplication>
#include <QTimer>
#include <QDateTime>
#include <QPropertyAnimation>

#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QProgressBar>

#include "ParamDefine.h"
#include "ApplicationWidget/ApplicationWidget.h"
#include "WorkStatusWidget/workStatusWidget.h"
#include "ResultQueryWidget/ResultQueryWidget.h"
#include "ReportWidget/ReportWidget.h"
#include "MaintainWidget/VersionWidget.h"
#include "ProjectSettingWidget/RoutineProjectSettingWidget.h"
#include "MaintainWidget/MaintainWidget.h"
#include "MaintainWidget/TemperatureCurveWidget.h"
#include "UI/MainWidget/SysSettingWidget/TimeSetting/timeSettingWidget.h"
#include "UI/MainWidget/MaintainWidget/DebugMaintainWidget.h"

#include "UI/MainWidget/SysSettingWidget/SysSetting/SysSettingWidget.h"
#include "UI/MainWidget/MaintainWidget/SubInterface/Save2HistoryProgressBar.h"
#include "UI/MainWidget/MaintainWidget/UserManagementWidget.h"
#include "UI/MainWidget/ProjectSettingWidget/ProjectGroupWidget.h"
#include "UI/MainWidget/ProjectSettingWidget/CalculateProjectWidget.h"
#include "UI/MainWidget/ProjectSettingWidget/QCSettingWidget.h"
#include "UI/Widget/Thermometer/Thermometer.h"
#include "UI/MainWidget/ProjectSettingWidget/TestOrder.h"
#include "UI/MainWidget/SysSettingWidget/LISSetting/CommunicationSetWidget.h"

#include "Thread/Scheduler.h"


class MV100_MainWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit MV100_MainWidget(QWidget *parent = 0);

private:
    QTimer *timer;           //更新状态栏上的时间
    QTimer *timerDelay;      //开始测试延时，（空闲状态查询（温度，液体状态））

private:
    void initForm();         //初始化

    void initFunctionMenuBarForm();  //功能菜单工具栏（顶部工具栏）
    void initShortcutBtnBarForm();   //快捷按钮工具栏（左侧工具栏）
    void initStatusBarForm();        //状态栏(浮子传感器、温度、试剂状态信息)
    void initStackWidget();          //初始化堆叠部件
    void initStatusMsgBarForm();     //状态栏（警告、错误信息显示区）
    void initProjectSettingMenuButton();  //"项目设置"下拉按钮菜单
    void initSysSettingMenuButton();      //"系统设置"下拉按钮菜单
    void initMaintainMenuButton();        //"维护"下拉按钮菜单
    void initStyle();        //界面风格
    void initConnect();      //连接信号与槽
    void initData();         //初始化数据
    void Shutdown();         //关机动画
    void addWidgetToStack(QWidget *widget);   //新增界面到stack堆上并显示widget界面
    void showChildWidget(QWidget *widget);    //设置当前要显示的界面
    void removeFromStack(QWidget *widget);    //从stack堆上移除widget界面
    int isDataBaseHasTask();     //查看数据库是否有已经申请完的任务，若有才能够进行测试
	
    int isSampleSlotReady();     //检查样本架是否就绪

    //
    QString SamplePos;       //样本位置（用于检查样本架是否就绪）

    VersionWidget *versionWidget;       //软件版本
    SysSettingWidget *sysSettingWidget; //系统设置
    Save2HistoryProgressBar *save2HistoryProgressBar; //转移结果到历史库

    //4种液体状态
    int Water_run_times;          //提示“缺少去离子水”的次数
    int CleanLiquid_run_times;    //提示“缺少清洗液”的次数
    int H_waste_run_times;        //提示“低浓度废液满”的次数
    int L_waste_run_times;        //提示“高浓度废液满”的次数

signals:


public slots:
    void slt_closeAllWindows();                //关机

    //功能菜单工具栏（顶部工具栏）
    void slt_btnTestApplication_Clicked();     //槽:测试申请
    void slt_btnWorkStatus_Clicked();          //槽:工作状态
    void slt_btnResultQuery_Clicked();         //槽:结果查询
    void slt_btnStatisticalReport_Clicked();   //槽:统计报表       ？？
    void slt_btnAbout_Clicked();               //槽:关于

    //"项目设置"下拉按钮菜单
    void slt_RoutineItem_Triggered();         //槽:常规项目
    void slt_ComputingItem_Triggered();       //槽:计算项目
    void slt_CombinateItem_Triggered();       //槽:组合项目
    void slt_CrossContamination_Triggered();  //槽:交叉污染
    void slt_CalibrateSetting_Triggered();    //槽:定标设置
    void slt_QCSetting_Triggered();           //槽:质控设置
    void slt_TestOrderSetting_Triggered();    //槽:测试顺序设置
    //"系统设置"下拉按钮菜单
    void slt_SysSetting_Triggered();          //槽:系统设置
    void slt_HospitalSetting_Triggered();     //槽:医院设置
    void slt_LISSetting_triggered();          //槽:LIS设置
    void slt_TimeSetting_Triggered();         //槽:时间设置
    void slt_UserManager_Triggered();         //槽:用户管理
    void slt_PrintSetting_Triggered();        //槽:打印设置
    void slt_BarcodeSetting_Triggered();      //槽:条码设置
    void slt_EN_Triggered();                  //槽:英文

    //"维护"下拉按钮菜单
    void slt_DailyMaintain_Triggered();     //槽:日常维护
    void slt_TemperatureCurve_Triggered();  //槽:温度曲线
    void slt_Maintain_Triggered();          //槽:进入维护
    void slt_SaveResult2History_Triggered();//槽:转移结果到历史库
    void slt_AddNewUser_Triggered();        //槽:新增新用户
    void slt_ViewLog_Triggered();           //槽:查看日志
    void slt_Upload2LIS_Triggered();        //槽:上传到LIS库

    //快捷按钮工具栏（左侧工具栏）
    void slt_btnStart_Clicked();               //槽:开始
    void slt_btnPause_Clicked();               //槽:暂停
    void slt_btnStop_Clicked();                //槽:停止
    void slt_btnLockScreen_Clicked();          //槽:锁屏
    void slt_btnEmergencyShutdown_Clicked();   //槽:紧急关机
    void slt_btnRestart_Clicked();             //槽:重启
    void slt_btnShutdown_Clicked();            //槽:关机
    void slt_timerDelay_timeout();             //槽:开始测试延时，（空闲状态查询（温度，液体状态））
    void slt_setButtonState(int type);        //槽:设置“开始”“暂停”“停止”三个按钮的状态 （所有测试都完成，或 运行出错时 调用）

    //状态栏(浮子传感器、温度、试剂状态信息)
    void slt_setTemperature(double t);     //槽:温度状态（试剂槽）
    void slt_setTemperature_Disk(double t);//槽:温度状态(反应盘)
    void slt_sortOutLiquidStatus(int s);   //槽：分类4种液体的状态
    void slt_DeionizedWater(int w);     //槽:去离子水（纯净水）
    void slt_CleaningLiquid(int c);     //槽:清洗液
    void slt_LowWaste(int l);           //槽:低浓度废液
    void slt_HighWaste(int h);          //槽:高浓度废液

    //堆叠部件

    //状态栏（警告、错误信息显示区）
    void slt_AlertMsg_show(int type, QString msg);        //显示错误警告信息
    void slt_timer_timeout();                   //槽:更新状态栏上的时间
    void slt_btnCleanAlertMsg_Clicked();        //槽:清除警告信息
    void slt_btnShowAlertMsg_Clicked();         //槽:查看警告信息

private:
    QWidget *wdgTop;
    QGridLayout *lytTop;

    //功能菜单工具栏（顶部工具栏）
    QWidget     *wdgFunctionMenuBar;     //功能菜单工具栏（顶部工具栏）
    QFrame      *fraLogo;                //MV100 LOGO
    QPushButton *btnTestApplication;     //测试申请
    QPushButton *btnWorkStatus;          //工作状态
    QPushButton *btnResultQuery;         //结果查询
    QPushButton *btnStatisticalReport;   //统计报表
    QPushButton *btnProjectSetting;      //项目设置
    QPushButton *btnSysSetting;          //系统设置
    QPushButton *btnMaintain;            //维护
     QPushButton *btnAbout;              //关于
    QHBoxLayout * hlytFunctionMenuBar;

    //"项目设置"下拉按钮菜单
    QMenu *menuProjectSetting;
    QAction *RoutineItem;         //常规项目
    QAction *ComputingItem;       //计算项目
    QAction *CombinateItem;       //组合项目
    QAction *CrossContamination;  //交叉污染
    QAction *CalibrateSetting;    //定标设置
    QAction *QCSetting;           //质控设置
    QAction *TestOrderSetting;    //测试顺序设置
    TestOrder *testOrder;         //窗体-测试顺序设置

    //"系统设置"下拉按钮菜单
    QMenu *menuSysSetting;
    QAction *SysSetting;          //系统设置
    QAction *TimeSetting;         //时间设置
    QAction *LISSetting;          //LIS设置
    QAction *HospitalSetting;     //医院设置
    QAction *UserManager;         //用户管理
    QAction *PrintSetting;        //打印设置
    QAction *BarcodeSetting;      //条码设置
    QAction *EN;                  //英文

    //"维护"下拉按钮菜单
    QMenu *menuMaintain;
    QAction *DailyMaintain;     //日常维护
    QAction *TemperatureCurve;  //温度曲线
    QAction *Maintain;          //进入维护
    QAction *SaveResult2History;//转移结果到历史库
    QAction *AddNewUser;        //用户管理
    QAction *ViewLog;           //查看日志
    QAction *Upload2LIS;        //上传到LIS库
    TemperatureCurveWidget *temperatureCurveWidget; //温度曲线界面
    DebugMaintainWidget *debugMaintainWidget;       //装机人员维护界面


    //快捷按钮工具栏（左侧工具栏）
    QWidget *wdgShortcutBtnBar;         //快捷按钮工具栏（左侧工具栏）
    QLabel *labWaiting;                  //等待
    QMovie *mvWaiting;                   //等侍动画
    QPushButton *btnStart;               //开始
    QPushButton *btnPause;               //暂停
    QPushButton *btnStop;                //停止
    QPushButton *btnLockScreen;          //锁屏
    QPushButton *btnEmergencyShutdown;   //紧急关机
    QPushButton *btnRestart;             //重启
    QPushButton *btnShutdown;            //关机
    QVBoxLayout *vlytShortCutBtnBar;

    //状态栏(浮子传感器、温度、试剂状态信息)
    QWidget *wdgStatusBar;         //状态栏(浮子传感器、温度、试剂状态信息)

    QLabel *labTemperature0;        //温度状态（试剂槽）
    QLabel *labTemperature_Disk0;   //温度状态（反应盘）
    QLabel *labDeionizedWater0;     //去离子水（纯净水）
    QLabel *labCleaningLiquid0;     //清洗液
    QLabel *labLowWaste0;           //低浓度废液
    QLabel *labHighWaste0;          //高浓度废液
    QProgressBar *barReactionDiskT; //反应盘温度
    QProgressBar *barReagentT;      //试剂仓温度

    Thermometer *ThermometerDisk;    //反应盘温度
    Thermometer *ThermometerReagent;//试剂仓温度
    QLabel *labTemperature;        //温度状态（试剂槽）
    QLabel *labTemperature_Disk;   //温度状态（反应盘）
    QLabel *labDeionizedWater;     //去离子水（纯净水）
    QLabel *labCleaningLiquid;     //清洗液
    QLabel *labLowWaste;           //低浓度废液
    QLabel *labHighWaste;          //高浓度废液

    QGridLayout *lytStatusBar;

    //堆叠部件
    QStackedWidget *stackWidget; //堆叠部件

    //状态栏（警告、错误信息显示区）
    QWidget *wdgStatusMsgBar;       //状态栏（警告、错误信息显示区）
    QLabel *labAlertMsg;            //警告信息（周期指令信息）
    QLabel *labAlertMsgLiquid;      //警告信息（液体状态信息）
    QPushButton *btnCleanAlertMsg;  //清除警告信息
    QPushButton *btnShowAlertMsg;   //查看警告信息
    QLabel *labDateTime;            //日期时间
    QHBoxLayout *hlytStatusMsgBar;
    QGridLayout *lytStatusMsgBar;


    //总布局
    QWidget *widget;
    QGridLayout * layout;


};
extern MV100_MainWidget *gMV100_MainWidget;
#endif // MV100_MAINWIDGET_H



//功能菜单工具栏（顶部工具栏）
//快捷按钮工具栏（左侧工具栏）
//状态栏(浮子传感器、温度、试剂状态信息)
//堆叠部件
//状态栏（警告、错误信息显示区）






