#include "mv100_MainWidget.h"
#include <qdebug.h>

MV100_MainWidget *gMV100_MainWidget = 0;
MV100_MainWidget::MV100_MainWidget(QWidget *parent) : QMainWindow(parent)
{
    initForm();          //初始化
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}

//初始化
void MV100_MainWidget::initForm()
{
    initFunctionMenuBarForm();  //功能菜单工具栏（顶部工具栏）
    initShortcutBtnBarForm();   //快捷按钮工具栏（左侧工具栏）
    initStatusBarForm();        //状态栏(浮子传感器、温度、试剂状态信息)
    initStackWidget();          //初始化堆叠部件
    initStatusMsgBarForm();     //状态栏（警告、错误信息显示区）

    wdgTop = new QWidget;
    lytTop = new QGridLayout;
    lytTop->setSpacing(0);
    lytTop->setMargin(0);
    lytTop->addWidget(wdgFunctionMenuBar, 0,   0, 10, 180, Qt::AlignLeft);
    lytTop->addWidget(wdgStatusBar,       0, 180, 10, 50, Qt::AlignRight);
    wdgTop->setLayout(lytTop);

    QWidget *widget1 = new QWidget;
    QGridLayout *lyt = new QGridLayout;
    lyt->setMargin(0);
    lyt->setSpacing(0);
    lyt->addWidget(stackWidget,         0, 0, 100, 200);
    lyt->addWidget(wdgStatusMsgBar,   100, 0,   5, 200);
    widget1->setLayout(lyt);

    //总布局
    widget = new QWidget(this);
    this->setCentralWidget(widget);
    layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(wdgTop,                    0,  0,  10, 200);
    layout->addWidget(wdgShortcutBtnBar,        10,  0, 100,  12);
    layout->addWidget(widget1,                  10, 12, 100, 188);
    widget->setLayout(layout);

    versionWidget = new VersionWidget;
    sysSettingWidget = new SysSettingWidget;
    save2HistoryProgressBar = new Save2HistoryProgressBar; //转移结果到历史库
}

//功能菜单工具栏（顶部工具栏）
void MV100_MainWidget::initFunctionMenuBarForm()
{
    //功能菜单工具栏（顶部工具栏）
    wdgFunctionMenuBar = new QWidget;   //功能菜单工具栏（顶部工具栏）
    fraLogo = new QFrame;               //MV100 LOGO
    btnTestApplication = new QPushButton("测试申请");     //测试申请
    btnWorkStatus = new QPushButton("工作状态");          //工作状态
    btnResultQuery = new QPushButton("结果查看");         //结果查询
    btnStatisticalReport = new QPushButton("统计报表");   //统计报表       ？？
    btnProjectSetting = new QPushButton("项目设置");      //项目设置
    btnSysSetting = new QPushButton("系统设置");          //系统设置
    btnMaintain = new QPushButton("维 护");            //维护
    btnAbout = new QPushButton("关 于");               //关于
    hlytFunctionMenuBar = new QHBoxLayout;
    hlytFunctionMenuBar->setMargin(2);
    hlytFunctionMenuBar->setSpacing(5);
    hlytFunctionMenuBar->addWidget(fraLogo, Qt::AlignLeft);
    hlytFunctionMenuBar->addWidget(btnTestApplication, Qt::AlignCenter);
    hlytFunctionMenuBar->addWidget(btnWorkStatus, Qt::AlignCenter);
    hlytFunctionMenuBar->addWidget(btnResultQuery, Qt::AlignCenter);
    hlytFunctionMenuBar->addWidget(btnStatisticalReport, Qt::AlignCenter);
    hlytFunctionMenuBar->addWidget(btnProjectSetting, Qt::AlignCenter);
    hlytFunctionMenuBar->addWidget(btnSysSetting, Qt::AlignCenter);
    hlytFunctionMenuBar->addWidget(btnMaintain, Qt::AlignCenter);
    hlytFunctionMenuBar->addWidget(btnAbout, Qt::AlignCenter);
    wdgFunctionMenuBar->setLayout(hlytFunctionMenuBar);
    wdgFunctionMenuBar->setMaximumHeight(80);

    initProjectSettingMenuButton();  //"项目设置"下拉按钮菜单
    initSysSettingMenuButton();      //"系统设置"下拉按钮菜单
    initMaintainMenuButton();        //"维护"下拉按钮菜单
}

//快捷按钮工具栏（左侧工具栏）
void MV100_MainWidget::initShortcutBtnBarForm()
{
    //快捷按钮工具栏（左侧工具栏）
    wdgShortcutBtnBar = new QWidget;    //快捷按钮工具栏（左侧工具栏）
    labWaiting = new QLabel;
    btnStart = new QPushButton;               //开始
    btnPause = new QPushButton;               //暂停
    btnStop = new QPushButton;                //停止
    QFrame *frame1 = new QFrame;
    btnLockScreen = new QPushButton;          //锁屏
    QFrame *frame2 = new QFrame;
    btnEmergencyShutdown = new QPushButton;   //紧急关机
    btnRestart = new QPushButton;
    btnShutdown = new QPushButton;            //关机
    vlytShortCutBtnBar = new QVBoxLayout;
    vlytShortCutBtnBar->setMargin(5);
    vlytShortCutBtnBar->setSpacing(5);
    vlytShortCutBtnBar->addWidget(btnStart, Qt::AlignCenter);
    vlytShortCutBtnBar->addWidget(btnPause, Qt::AlignCenter);
    vlytShortCutBtnBar->addWidget(btnStop, Qt::AlignCenter);
    vlytShortCutBtnBar->addWidget(frame1, Qt::AlignCenter);
    vlytShortCutBtnBar->addWidget(frame2, Qt::AlignCenter);
    vlytShortCutBtnBar->addWidget(btnRestart, Qt::AlignCenter);
    vlytShortCutBtnBar->addWidget(btnShutdown, Qt::AlignCenter | Qt::AlignBottom);
    frame1->setFixedHeight(80);
    frame2->setFixedHeight(80);
    wdgShortcutBtnBar->setLayout(vlytShortCutBtnBar);
    timerDelay = new QTimer;
}

//状态栏(浮子传感器、温度、试剂状态信息)
void MV100_MainWidget::initStatusBarForm()
{
    //状态栏(浮子传感器、温度、试剂状态信息)
    wdgStatusBar = new QWidget;         //状态栏(浮子传感器、温度、试剂状态信息)
    labTemperature0 = new QLabel("试剂槽温度:");       //温度状态（试剂槽）
    labTemperature_Disk0 = new QLabel("反应盘温度:");  //温度状态（反应盘）
    labDeionizedWater0 = new QLabel("去离子水:");      //去离子水（纯净水）
    labCleaningLiquid0 = new QLabel("清洗液:");       //清洗液
    labLowWaste0 = new QLabel("低浓度废液:");          //低浓度废液
    labHighWaste0 = new QLabel("高浓度废液:");         //高浓度废液
    labTemperature = new QLabel;        //温度状态（试剂槽）
    labTemperature_Disk = new QLabel;   //温度状态（反应盘）
    barReactionDiskT = new QProgressBar; //反应盘温度
    barReagentT = new QProgressBar;      //试剂仓温度

    ThermometerDisk = new Thermometer;    //反应盘温度
    ThermometerReagent = new Thermometer;//试剂仓温度
    labDeionizedWater = new QLabel;     //去离子水（纯净水）
    labCleaningLiquid = new QLabel;     //清洗液
    labLowWaste = new QLabel;           //低浓度废液
    labHighWaste = new QLabel;          //高浓度废液
    lytStatusBar = new QGridLayout;
    lytStatusBar->setMargin(0);
    lytStatusBar->setSpacing(15);
    lytStatusBar->addWidget(labDeionizedWater,   0,  5, 10, 5, Qt::AlignCenter);
    lytStatusBar->addWidget(labCleaningLiquid,   0, 15, 10, 5, Qt::AlignCenter);
    lytStatusBar->addWidget(labLowWaste,         0, 25, 10, 5, Qt::AlignCenter);
    lytStatusBar->addWidget(labHighWaste,        0, 35, 10, 5, Qt::AlignCenter);
    lytStatusBar->addWidget(ThermometerDisk,     0, 45, 10, 5, Qt::AlignCenter);
    lytStatusBar->addWidget(ThermometerReagent,  0, 55, 10, 5, Qt::AlignCenter);
    wdgStatusBar->setMaximumHeight(80);
}

//状态栏（警告、错误信息显示区）
void MV100_MainWidget::initStatusMsgBarForm()
{
    //状态栏（警告、错误信息显示区）
    wdgStatusMsgBar = new QWidget;       //状态栏（警告、错误信息显示区）
    labAlertMsg = new QLabel;            //警告信息
    labAlertMsgLiquid = new QLabel;
    btnCleanAlertMsg = new QPushButton("C"); //清除警告信息
    btnShowAlertMsg = new QPushButton("A");  //查看警告信息
    labDateTime = new QLabel;            //日期时间
    lytStatusMsgBar = new QGridLayout;
    lytStatusMsgBar->setMargin(3);
    lytStatusMsgBar->setSpacing(0);
    lytStatusMsgBar->addWidget(btnShowAlertMsg,  0,   0, 5,  1);
    lytStatusMsgBar->addWidget(btnCleanAlertMsg, 0,   1, 5,  1);
    lytStatusMsgBar->addWidget(labAlertMsg,      0,  20, 5, 40);
    lytStatusMsgBar->addWidget(labAlertMsgLiquid,0,  60, 5, 40);
    lytStatusMsgBar->addWidget(labDateTime,      0, 100, 5, 10, Qt::AlignRight);
    wdgStatusMsgBar->setLayout(lytStatusMsgBar);
    timer = new QTimer;
    timer->start(10000);
}

//"项目设置"下拉按钮菜单
void MV100_MainWidget::initProjectSettingMenuButton()
{
    //"项目设置"下拉按钮菜单
    menuProjectSetting = new QMenu;
    RoutineItem = new QAction("常规项目", this);         //常规项目
    ComputingItem = new QAction("计算项目", this);       //计算项目
    CombinateItem = new QAction("组合项目", this);       //组合项目
    CrossContamination = new QAction("交叉污染", this);  //交叉污染
    CalibrateSetting = new QAction("定标设置", this);    //定标设置
    QCSetting = new QAction("质控设置", this);           //质控设置
    TestOrderSetting = new QAction("顺序设置", this);           //测试顺序设置

    menuProjectSetting->addAction(RoutineItem);
    menuProjectSetting->addAction(ComputingItem);
    menuProjectSetting->addAction(CombinateItem);
    //menuProjectSetting->addAction(CrossContamination);
    //menuProjectSetting->addAction(CalibrateSetting);
    menuProjectSetting->addAction(QCSetting);
    menuProjectSetting->addAction(TestOrderSetting);

    btnProjectSetting->setMenu(menuProjectSetting);

    testOrder = new TestOrder;
}

//"系统设置"下拉按钮菜单
void MV100_MainWidget::initSysSettingMenuButton()
{
    //"系统设置"下拉按钮菜单
    menuSysSetting = new QMenu;
    SysSetting = new QAction("常规设置", this);      //系统设置
    HospitalSetting = new QAction("医院设置", this); //医院设置
    TimeSetting = new QAction("时间设置", this);     //时间设置
    LISSetting = new QAction("网络设置", this);      //LIS设置
    UserManager = new QAction("用户管理", this);     //用户管理
    PrintSetting = new QAction("打印设置", this);    //打印设置
    BarcodeSetting = new QAction("条码设置", this);  //条码设置
    EN = new QAction("切换英文", this);              //英文
    menuSysSetting->addAction(SysSetting);
    menuSysSetting->addAction(TimeSetting);
    menuSysSetting->addAction(LISSetting);
    //    menuSysSetting->addAction(UserManager);
    //    menuSysSetting->addAction(PrintSetting);
    //    menuSysSetting->addAction(BarcodeSetting);
    //    menuSysSetting->addAction(EN);
    btnSysSetting->setMenu(menuSysSetting);
}

//"维护"下拉按钮菜单
void MV100_MainWidget::initMaintainMenuButton()
{
    DailyMaintain = new QAction("日常维护", this);     //日常维护
    TemperatureCurve = new QAction("温度曲线", this);  //温度曲线
    Maintain = new QAction("进入维护", this);          //进入维护
    SaveResult2History = new QAction("转移历史库", this);//转移结果到历史库
    AddNewUser = new QAction("用户管理", this);      //用户管理
    ViewLog = new QAction("查看日志", this);         //查看日志
    Upload2LIS = new QAction("上传到LIS", this);     //上传到LIS

    //"维护"下拉按钮菜单
    menuMaintain = new QMenu;
    menuMaintain->addAction(AddNewUser);         //用户管理
    menuMaintain->addAction(DailyMaintain);      //日常维护
    menuMaintain->addAction(Maintain);           //进入维护
    menuMaintain->addAction(TemperatureCurve);   //温度曲线
    menuMaintain->addAction(ViewLog);            //查看日志
    menuMaintain->addAction(SaveResult2History); //转移结果到历史库
    menuMaintain->addAction(Upload2LIS);         //上传到LIS

    btnMaintain->setMenu(menuMaintain);
}

//初始化堆叠部件
void MV100_MainWidget::initStackWidget()
{
    stackWidget = new QStackedWidget; //堆叠部件
    stackWidget->addWidget(ApplicationWidget::getInstance());   //测试申请
    stackWidget->addWidget(WorkStatusWidget::getInstance());    //工作状态
    stackWidget->addWidget(ResultQueryWidget::getInstance());   //结果查询
    stackWidget->addWidget(ReportWidget::getInstance());        //统计报表
    ////stackWidget->addWidget(WorkStatusWidget::getInstance());   //项目设置
    ////stackWidget->addWidget(WorkStatusWidget::getInstance());   //系统设置
    ////stackWidget->addWidget(WorkStatusWidget::getInstance());   //维护

    //"项目设置"下拉按钮菜单
    stackWidget->addWidget(RoutineProjectSettingWidget::getInstance());  //常规项目
    stackWidget->addWidget(ProjectGroupWidget::getInstance());           //组合项目
    stackWidget->addWidget(CalculateProjectWidget::getInstance());       //计算项目
    //    QAction *CrossContamination;  //交叉污染
    //    QAction *CalibrateSetting;    //定标设置
    //    QAction *QCSetting;           //质控设置
    //"系统设置"下拉按钮菜单
    //    QAction *SysSetting;          //系统设置
    //    QAction *HospitalSetting;     //医院设置
    //    QAction *UserManager;         //用户管理
    //    QAction *PrintSetting;        //打印设置
    //    QAction *BarcodeSetting;      //条码设置
    //    QAction *EN;                  //英文

    //"维护"下拉按钮菜单
    stackWidget->addWidget(MaintainWidget::getInstance());     //日常维护
    temperatureCurveWidget = new TemperatureCurveWidget;
    stackWidget->addWidget(temperatureCurveWidget);            //温度曲线
    debugMaintainWidget = new DebugMaintainWidget;
    stackWidget->addWidget(debugMaintainWidget);               //装机人员维护界面


    //    QAction *Maintain;          //进入维护

}

//界面风格
void MV100_MainWidget::initStyle()
{
    int width, height;
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();
    width = deskRect.width();
    height = deskRect.height();
#ifdef Q_OS_WIN//如果是windows系统
    width = 1366;
    height = 768;
#else          //如果是unix或者其他系统
    this->setWindowFlags(Qt::FramelessWindowHint);       //去掉标题栏
#endif

    this->setGeometry(0, 0, width, height);
    widget->setMinimumSize(width, height);

    //设置背景
    QPalette palette = widget->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap( ":/resource/resource/bg/mainBg.png" )));  //
    widget->setPalette(palette);
    widget->setAutoFillBackground(true);

    //图标路径
    QIcon icon("://resource/resource/MV100.png");
    this->setWindowIcon(icon);

    //QSS
    QFile qss( ":/QSS/QSS/mv100.qss" );
    qss.open(QFile::ReadOnly);
    QString ss = qss.readAll();
    setStyleSheet( ss );
    //wdgStatusBar->setStyleSheet( ss );
    qss.close();

    stackWidget->setStyleSheet("QStackedWidget{background-color:#F6F6F6; background: rgba(246,246,246, 100);}");

    wdgTop->setObjectName("MainWidgetTop");
    //    wdgTop->setFixedWidth(width);
    //功能菜单工具栏（顶部工具栏）
    //wdgFunctionMenuBar->setFixedHeight(100);
    btnTestApplication->setToolTip("测试申请");     //测试申请
    btnWorkStatus->setToolTip("工作状态");          //工作状态
    btnResultQuery->setToolTip("结果查询");         //结果查询
    btnStatisticalReport->setToolTip("统计报表");   //统计报表
    btnProjectSetting->setToolTip("项目设置");      //项目设置
    btnSysSetting->setToolTip("系统设置");          //系统设置
    btnMaintain->setToolTip("维护");               //维护
    fraLogo->setObjectName("frameLogo");
    fraLogo->setFixedSize(130, 40);
    wdgFunctionMenuBar->setObjectName("FunctionMenuBar");
    btnTestApplication->setObjectName("btnTestApplication");
    btnWorkStatus->setObjectName("btnWorkStatus");
    btnResultQuery->setObjectName("btnResultQuery");
    btnStatisticalReport->setObjectName("btnStatisticalReport");
    btnProjectSetting->setObjectName("btnProjectSetting");
    btnSysSetting->setObjectName("btnSysSetting");
    btnMaintain->setObjectName("btnMaintain");
    btnAbout->setObjectName("btnWorkStatus");
    btnTestApplication->setFixedSize(100, 60);     //测试申请
    btnWorkStatus->setFixedSize(100, 60);          //工作状态
    btnResultQuery->setFixedSize(100, 60);         //结果查询
    btnStatisticalReport->setFixedSize(100, 60);   //统计报表
    btnProjectSetting->setFixedSize(100, 60);      //项目设置
    btnSysSetting->setFixedSize(100, 60);          //系统设置
    btnMaintain->setFixedSize(100, 60);            //维护
    btnAbout->setFixedSize(100, 60);                //关于


    QString menu =
            "QMenu{ border-radius:5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #113845, stop: 0.2 #1878B1, stop: 1.0 #15A8FF);border: 1px solid gray;}"
            "QMenu::item { width:120; background-color: transparent; font:20px Times New Roman;"
            "padding:10px 20px;"//设置菜单项文字上下和左右的内边距，效果就是菜单中的条目左右上下有了间隔
            "margin:0px 5px;"//设置菜单项的外边距
            "border-bottom:1px solid #DBDBDB;}"//为菜单项之间添加横线间隔
            "QMenu::item:selected{background-color: #2dabf9;}";
    menuProjectSetting->setStyleSheet(menu);
    menuMaintain->setStyleSheet(menu);
    menuSysSetting->setStyleSheet(menu);


    //快捷按钮工具栏（左侧工具栏）
    //    wdgShortcutBtnBar->setFixedWidth(90);
    btnStart->setToolTip("开始");               //开始
    btnPause->setToolTip("暂停");               //暂停
    btnStop->setToolTip("停止");                //停止
    btnLockScreen->setToolTip("锁屏");          //锁屏
    btnEmergencyShutdown->setToolTip("紧急关机");//紧急关机
    btnRestart->setToolTip("重启");
    btnShutdown->setToolTip("关机");            //关机
    wdgShortcutBtnBar->setObjectName("ShortcutBtnBar");
    btnStart->setObjectName("btnStart");               //开始
    btnPause->setObjectName("btnPause");               //暂停
    btnStop->setObjectName("btnStop");                 //停止
    btnLockScreen->setObjectName("btnLockScreen");     //锁屏
    btnEmergencyShutdown->setObjectName("btnEmergencyShutdown");   //紧急关机
    btnRestart->setObjectName("btnRestart");           //重启
    btnShutdown->setObjectName("btnShutdown");         //关机


    //状态栏(浮子传感器、温度状态信息)
    labDeionizedWater->setFixedSize(25, 60);
    labCleaningLiquid->setFixedSize(25, 60);
    labHighWaste->setFixedSize(25, 60);
    labLowWaste->setFixedSize(25, 60);
    labDeionizedWater->setObjectName("Water_normal");         //去离子水（纯净水）
    labCleaningLiquid->setObjectName("CleaningLiquid_normal");//清洗液
    labLowWaste->setObjectName("LowWaste_normal");            //低浓度废液
    labHighWaste->setObjectName("HighWaste_normal");          //高浓度废液
    labLowWaste->setToolTip(tr("低浓度废液"));
    labHighWaste->setToolTip(tr("高浓度废液"));
    labCleaningLiquid->setToolTip(tr("清洗液"));
    labDeionizedWater->setToolTip(tr("去离子水"));
    ThermometerReagent->setToolTip(tr("试剂仓:0"));
    ThermometerReagent->setValue(0);
    ThermometerReagent->setMinValue(-5);
    ThermometerReagent->setMaxValue(25);
    ThermometerDisk->setToolTip(tr("反应盘:0"));
    ThermometerDisk->setValue(0);
    ThermometerDisk->setMinValue(0);
    ThermometerDisk->setMaxValue(55);
    wdgStatusBar->setLayout(lytStatusBar);

    //堆叠部件

    //状态栏（警告、错误信息显示区）
    wdgStatusMsgBar->setObjectName("statusMsgBar");
    btnShowAlertMsg->setObjectName("button");
    btnCleanAlertMsg->setObjectName("button");
    labAlertMsg->setObjectName("AlertMsg");
    labAlertMsgLiquid->setObjectName("AlertMsg");
    labDateTime->setObjectName("lab12");
    btnShowAlertMsg->setFixedSize(20, 20);
    btnCleanAlertMsg->setFixedSize(20, 20);

    btnStart->setEnabled(true);
    btnPause->setEnabled(false);
    btnStop->setEnabled(false);
}

//连接信号与槽
void MV100_MainWidget::initConnect()
{
    //功能菜单工具栏（顶部工具栏）
    connect(btnTestApplication, SIGNAL(clicked()), this, SLOT(slt_btnTestApplication_Clicked()));
    connect(btnWorkStatus, SIGNAL(clicked()), this, SLOT(slt_btnWorkStatus_Clicked()));
    connect(btnResultQuery, SIGNAL(clicked()), this, SLOT(slt_btnResultQuery_Clicked()));
    connect(btnStatisticalReport, SIGNAL(clicked()), this, SLOT(slt_btnStatisticalReport_Clicked()));
    connect(btnAbout, SIGNAL(clicked()), this, SLOT(slt_btnAbout_Clicked()));

    //"项目设置"下拉按钮菜单
    connect(RoutineItem, SIGNAL(triggered(bool)), this, SLOT(slt_RoutineItem_Triggered()));
    connect(ComputingItem, SIGNAL(triggered(bool)), this, SLOT(slt_ComputingItem_Triggered()));
    connect(CombinateItem, SIGNAL(triggered(bool)), this, SLOT(slt_CombinateItem_Triggered()));
    connect(CrossContamination, SIGNAL(triggered(bool)), this, SLOT(slt_CrossContamination_Triggered()));
    connect(CalibrateSetting, SIGNAL(triggered(bool)), this, SLOT(slt_CalibrateSetting_Triggered()));
    connect(QCSetting, SIGNAL(triggered(bool)), this, SLOT(slt_QCSetting_Triggered()));
    connect(TestOrderSetting, SIGNAL(triggered(bool)), this, SLOT(slt_TestOrderSetting_Triggered()));

    //"系统设置"下拉按钮菜单
    connect(SysSetting, SIGNAL(triggered(bool)), this, SLOT(slt_SysSetting_Triggered()));
    connect(HospitalSetting, SIGNAL(triggered(bool)), this, SLOT(slt_HospitalSetting_Triggered()));
    connect(TimeSetting, SIGNAL(triggered(bool)), this, SLOT(slt_TimeSetting_Triggered()));
    connect(LISSetting, SIGNAL(triggered(bool)), this, SLOT(slt_LISSetting_triggered()));
    connect(UserManager, SIGNAL(triggered(bool)), this, SLOT(slt_UserManager_Triggered()));
    connect(PrintSetting, SIGNAL(triggered(bool)), this, SLOT(slt_PrintSetting_Triggered()));
    connect(BarcodeSetting, SIGNAL(triggered(bool)), this, SLOT(slt_BarcodeSetting_Triggered()));
    connect(EN, SIGNAL(triggered(bool)), this, SLOT(slt_EN_Triggered()));

    //"维护"下拉按钮菜单
    connect(DailyMaintain, SIGNAL(triggered(bool)), this, SLOT(slt_DailyMaintain_Triggered()));
    connect(TemperatureCurve, SIGNAL(triggered(bool)), this, SLOT(slt_TemperatureCurve_Triggered()));
    connect(SaveResult2History, SIGNAL(triggered(bool)), this, SLOT(slt_SaveResult2History_Triggered()));
    connect(Maintain, SIGNAL(triggered(bool)), this, SLOT(slt_Maintain_Triggered()));
    connect(AddNewUser, SIGNAL(triggered(bool)), this, SLOT(slt_AddNewUser_Triggered()));
    connect(ViewLog, SIGNAL(triggered(bool)), this, SLOT(slt_ViewLog_Triggered()));
    connect(Upload2LIS, SIGNAL(triggered(bool)), this, SLOT(slt_Upload2LIS_Triggered()));


    //快捷按钮工具栏（左侧工具栏）
    connect(btnStart, SIGNAL(clicked()), this, SLOT(slt_btnStart_Clicked()));
    connect(btnPause, SIGNAL(clicked()), this, SLOT(slt_btnPause_Clicked()));
    connect(btnStop, SIGNAL(clicked()), this, SLOT(slt_btnStop_Clicked()));
    connect(btnLockScreen, SIGNAL(clicked()), this, SLOT(slt_btnLockScreen_Clicked()));
    connect(btnEmergencyShutdown, SIGNAL(clicked()), this, SLOT(slt_btnEmergencyShutdown_Clicked()));
    connect(btnShutdown, SIGNAL(clicked()), this, SLOT(slt_btnShutdown_Clicked()));
    connect(btnRestart, SIGNAL(clicked()), this, SLOT(slt_btnRestart_Clicked()));
    connect(timerDelay, SIGNAL(timeout()), this, SLOT(slt_timerDelay_timeout()));
    connect(gScheduler, SIGNAL(sig_UpdateMainWidget3Button_State(int)), this, SLOT(slt_setButtonState(int)));


    //状态栏(浮子传感器、温度、试剂状态信息)
    connect(gScheduler, SIGNAL(sigReagentSlotT(double)), this, SLOT(slt_setTemperature(double)));
    connect(gScheduler, SIGNAL(sigReactionDiskT(double)), this, SLOT(slt_setTemperature_Disk(double)));
    connect(gScheduler, SIGNAL(sigLiquidStatus(int)), this, SLOT(slt_sortOutLiquidStatus(int)));

    //堆叠部件

    //状态栏（警告、错误信息显示区）
    connect(timer, SIGNAL(timeout()), this, SLOT(slt_timer_timeout()));
    connect(btnShowAlertMsg, SIGNAL(clicked()), this, SLOT(slt_btnShowAlertMsg_Clicked()));
    connect(btnCleanAlertMsg, SIGNAL(clicked()), this, SLOT(slt_btnCleanAlertMsg_Clicked()));

    connect(gSerialThread, SIGNAL(sig_AlertMsg_show(int,QString)), this, SLOT(slt_AlertMsg_show(int,QString)));
    connect(gSerialThread2, SIGNAL(sig_AlertMsg_show(int,QString)), this, SLOT(slt_AlertMsg_show(int,QString)));

}

//初始化数据
void MV100_MainWidget::initData()
{
    Water_run_times = 0;          //提示“缺少去离子水”的次数
    CleanLiquid_run_times = 0;    //提示“缺少清洗液”的次数
    H_waste_run_times = 0;        //提示“低浓度废液满”的次数
    L_waste_run_times = 0;        //提示“高浓度废液满”的次数

    //更新状态栏上的时间
    slt_timer_timeout();
    //默认状态
    slt_sortOutLiquidStatus(0);
    slt_setTemperature(0);     //槽:温度状态（试剂槽）
    slt_setTemperature_Disk(0);//槽:温度状态(反应盘)
}

//关机动画
void MV100_MainWidget::Shutdown()
{
    //    //界面动画，改变透明度的方式消失1 - 0渐变
    //    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    //    animation->setDuration(2000);
    //    animation->setStartValue(1);
    //    animation->setEndValue(0);
    //    animation->start();
    //    connect(animation, SIGNAL(finished()), this, SLOT(slt_closeAllWindows()));

    slt_closeAllWindows();
}

//关机
void MV100_MainWidget::slt_closeAllWindows()
{
    //退出系统
    stackWidget->setVisible(0);
    stackWidget->hide();
    this->close();
    //qApp->closeAllWindows();              //关闭所有窗体
}

//查看数据库是否有已经申请完的任务，若有才能够进行测试
int MV100_MainWidget::isDataBaseHasTask()
{
    int flag = 0;

    //从数据库中取出用户申请的测试信息，并存入循环队列，等待执行
    QString str  =
            //任务id，状态，样本位置
            " select TaskT.id, stateID, samplePos "
            " from TaskT "
            //连接方式(任务队列表、项目表)
            " Inner join ProjectT ON TaskT.projectID = ProjectT.id "
            //连接方式(项目表、样本类型表)
            " Inner join SampleTypeT ON TaskT.sampleTypeID = SampleTypeT.id "
            //只查找未完成状态的任务（待测试）
            " where TaskT.stateID = 3 "
            //排序查询：紧急否降序＋样本号升序＋是否搅拌排序（同一样本所申请的测试按样本不需要搅拌的先进行，然后再测要搅拌的项目）
            " order by  isEmergency desc, dateTime asc, SampleTypeT.isStir asc "
            //每次取出的已经申请的测试个数
            " limit 0, "+ QString::number( RUNCOUNT ) +";";
    //qDebug() << str;
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    SamplePos = "";       //样本位置（用于检查样本架是否就绪）
    while(query.next())
    {
        SamplePos = query.value(2).toString();  //样本位置（用于检查样本架是否就绪）
        flag = 1;
    }

    return flag;
}

//检查样本架是否就绪
int MV100_MainWidget::isSampleSlotReady()
{
    if(SamplePos == "") return 0;  //提示呢？？

    int sampleSlot = SamplePos.split("-").at(0).toInt(); //样本架（急诊位为0）

    //样本加越界
    if(sampleSlot < 0 || sampleSlot > 3) return 0;

    if( gScheduler->getSampleSlotStatus()[sampleSlot] != 0 ) //样本架未放置好
    {
        gMessageBox->setInfo(QString("警告"), QString("样本架未放置到指定位置,\n是否要继续？"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, false);
        gMessageBox->setButtonText(Cancel, "否");
        gMessageBox->setButtonText(Sure, "是");
        if(gMessageBox->exec() == MVMessageBox::Accepted)
            return 1;
        else
            return 0;
    }

    return 1;
}


/*******************功能菜单工具栏（顶部工具栏）********************************************/
//槽:测试申请
void MV100_MainWidget::slt_btnTestApplication_Clicked()     {stackWidget->setCurrentWidget(ApplicationWidget::getInstance());}
//槽:工作状态
void MV100_MainWidget::slt_btnWorkStatus_Clicked()          {stackWidget->setCurrentWidget(WorkStatusWidget::getInstance());}
//槽:结果查询
void MV100_MainWidget::slt_btnResultQuery_Clicked()         {stackWidget->setCurrentWidget(ResultQueryWidget::getInstance());}
//槽:统计报表
void MV100_MainWidget::slt_btnStatisticalReport_Clicked()   {stackWidget->setCurrentWidget(ReportWidget::getInstance());}
//槽:关于
void MV100_MainWidget::slt_btnAbout_Clicked()
{
    versionWidget->setGeometry(this->width()/2-320, this->height()/2-200, 320, 200);
    versionWidget->show();
}

//"项目设置"下拉按钮菜单
//槽:常规项目
void MV100_MainWidget::slt_RoutineItem_Triggered()   {stackWidget->setCurrentWidget(RoutineProjectSettingWidget::getInstance());}
//槽:计算项目
void MV100_MainWidget::slt_ComputingItem_Triggered() {stackWidget->setCurrentWidget(CalculateProjectWidget::getInstance());}
//槽:组合项目
void MV100_MainWidget::slt_CombinateItem_Triggered() {stackWidget->setCurrentWidget(ProjectGroupWidget::getInstance());}
//槽:交叉污染
void MV100_MainWidget::slt_CrossContamination_Triggered() { }
//槽:定标设置
void MV100_MainWidget::slt_CalibrateSetting_Triggered()   { }
//槽:质控设置
void MV100_MainWidget::slt_QCSetting_Triggered()
{
    QCSettingWidget::getInstance()->show();
}
//槽:测试顺序设置
void MV100_MainWidget::slt_TestOrderSetting_Triggered()
{
    testOrder->QueryProjectInfo();
    testOrder->show();
}

//"系统设置"下拉按钮菜单
//槽:系统设置
void MV100_MainWidget::slt_SysSetting_Triggered(){sysSettingWidget->show();}
//槽:医院设置
void MV100_MainWidget::slt_HospitalSetting_Triggered() { }
//槽:LIS设置
void MV100_MainWidget::slt_LISSetting_triggered() {CommunicationSetWidget::getInstance()->show();}
//槽:时间设置
void MV100_MainWidget::slt_TimeSetting_Triggered(){TimeSettingWidget::getInstance()->show();}
//槽:用户管理
void MV100_MainWidget::slt_UserManager_Triggered() { }
//槽:打印设置
void MV100_MainWidget::slt_PrintSetting_Triggered() { }
//槽:条码设置
void MV100_MainWidget::slt_BarcodeSetting_Triggered() { }
//槽:英文
void MV100_MainWidget::slt_EN_Triggered() { }

//"维护"下拉按钮菜单
//槽:日常维护
void MV100_MainWidget::slt_DailyMaintain_Triggered()
{
    if( gScheduler->get_isRunning() ) //正在测试中，不允许进入
    {
        gMessageBox->setInfo(QString("提示"), QString("有测试正在进行中，不能进入“日常维护”界面！"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return;
    }
    stackWidget->setCurrentWidget(MaintainWidget::getInstance());
}
//槽:温度曲线
void MV100_MainWidget::slt_TemperatureCurve_Triggered()
{
    if( gScheduler->get_isRunning() ) //正在测试中，不允许进入
    {
        gMessageBox->setInfo(QString("提示"), QString("有测试正在进行中，不能进入“温度曲线”界面！"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return;
    }
    stackWidget->setCurrentWidget(temperatureCurveWidget);
}
//槽:转移结果到历史库
void MV100_MainWidget::slt_SaveResult2History_Triggered()
{
    if( gScheduler->get_isRunning() )//正在测试中，不允许修改
    {
        gMessageBox->setInfo(QString("提示"), QString("有测试正进行中，不能转移结果到历史库！\n请等待所有测试完成后再进行!!"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
        return;
    }
    gMessageBox->setInfo(QString("提示"), QString("是否要将临时结果表中所有的数据转移到结果表中!"),
                         QPixmap("://resource/resource/MessageBox/information.png"),  true, false);
    gMessageBox->setButtonText(Sure, "是(Y)");
    gMessageBox->setButtonText(Cancel, "否(N)");
    if(gMessageBox->exec() == MVMessageBox::Rejected)
        return;
    save2HistoryProgressBar->StartSave2HistoryT();
}
//槽:进入维护（装机调试人员用）
void MV100_MainWidget::slt_Maintain_Triggered()
{
    if( gScheduler->get_isRunning() ) //正在测试中，不允许进入
    {
        gMessageBox->setInfo(QString("提示"), QString("有测试正在进行中，不能进入“进入维护”界面！"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return;
    }

    stackWidget->setCurrentWidget(debugMaintainWidget);
    //MV100工作状态
    gScheduler->setMV100_WorkStatus(MV_State_unChecked);
    gScheduler->StopIdleTimer(); //进入维护后停止-状态查询定时器
}
//槽:用户管理
void MV100_MainWidget::slt_AddNewUser_Triggered()
{
    //只有root用户才能进入“用户管理”
    if(gParameter->getUserID() != 1)
    {
        gMessageBox->setInfo(QString("权限提示"), QString("当前用户没有“用户管理”权限,\n\n请用“root”用户登录！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return;
    }
    UserManagementWidget::getInstance()->show();
}
//槽:查看日志
void MV100_MainWidget::slt_ViewLog_Triggered()
{
    ErrorMsgWidget::getInstance()->show();
}
//槽:上传到LIS库
void MV100_MainWidget::slt_Upload2LIS_Triggered()
{

}

/*******************快捷按钮工具栏（左侧工具栏）*****************************************/

//槽:开始
void MV100_MainWidget::slt_btnStart_Clicked()
{
    int Status = gScheduler->getMV100_WorkStatus(); //MV100工作状态
    if( Status == 1 || Status == 2 || Status == 3 ) //测试正在进行中，返回（开机清洗反应杯暂停后也不能）
        return;
    else if( Status == 4 ) //复位失败
    {
        gMessageBox->setInfo(QString("警告"), QString("开机复位失败，请重新复位!"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return;
    }
    else if( Status == 5 ) //开机没有自检
    {
        gMessageBox->setInfo(QString("警告"), QString("开机或进入维护后没有自检，\n不能进行测试，请重新启动!"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return;
    }
    else if( Status == 6 ) //暂停
    {
        gMessageBox->setInfo(QString("提示"), QString("是否继续测试已经申请的任务!"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, false);
        gMessageBox->setButtonText(Cancel, "否");
        gMessageBox->setButtonText(Sure, "是");
        if(gMessageBox->exec() == MVMessageBox::Accepted)
        {
            gScheduler->TaskStart();
            btnStart->setEnabled(false);
            btnPause->setEnabled(true);
            btnStop->setEnabled(true);
        }
    }
    else//
    {
        char *CMD = gMVFrame->getFrameIdle(); //空闲状态查询（温度，液体状态）
        gSerialThread->WriteDebugFrame(CMD, LENGHT_FRAME_IDLE);
        timerDelay->start(400); //延时
    }
}
//槽:暂停
void MV100_MainWidget::slt_btnPause_Clicked()
{
    //MV100工作状态（0：空闲，1：普通测试，2：质控测试，3：定标测试，4：开机复位, 5：开机未自检, 6：暂停， 7：停止）
    if(gScheduler->get_isRunning() && gScheduler->getMV100_WorkStatus() != 6)
    {
        gMessageBox->setInfo(QString("暂停提示"), QString("有未完成的测试，是否要继续暂停！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, false);
        gMessageBox->setButtonText(Cancel, "否");
        gMessageBox->setButtonText(Sure, "是");
        if(gMessageBox->exec() == MVMessageBox::Accepted)
        {
            gScheduler->slt_Suspend();

            btnStart->setEnabled(true);
            btnPause->setEnabled(false);
            btnStop->setEnabled(true);
        }
    }
}
//槽:停止
void MV100_MainWidget::slt_btnStop_Clicked()
{
    if(gScheduler->get_isRunning() == 1)
    {
        gMessageBox->setInfo(QString("停止提示"), QString("有未完成的测试，是否要停止机器！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, false);
        gMessageBox->setButtonText(Cancel, "否");
        gMessageBox->setButtonText(Sure, "是");
        if(gMessageBox->exec() == MVMessageBox::Accepted)
        {
            gScheduler->slt_Stop();
            btnStart->setEnabled(false);
            btnPause->setEnabled(false);
            btnStop->setEnabled(false);
        }
    }

    //    qint64 s = 20170516;
    //    qint64 Status[4];
    //    Status[3] = s & 0x7f;
    //    Status[2] = (s & 0x3f80) >> 7;
    //    Status[1] = (s & 0x1fc000) >> 14;
    //    Status[0] = (s & 0x1e00000) >> 21;
    //    qDebug() << Status[0] << Status[1] << Status[2] << Status[3];
}
//槽:锁屏
void MV100_MainWidget::slt_btnLockScreen_Clicked()   {  }
//槽:紧急关机
void MV100_MainWidget::slt_btnEmergencyShutdown_Clicked()   {  }
//槽:重启
void MV100_MainWidget::slt_btnRestart_Clicked()
{
    if(gScheduler->get_isRunning() == 1)
    {
        gMessageBox->setInfo(QString("重启提示"), QString("\n有未完成的测试，是否要重启软件！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, false);
        gMessageBox->setButtonText(Cancel, "否");
        gMessageBox->setButtonText(Sure, "是");
        if(gMessageBox->exec() == MVMessageBox::Accepted)
        {
            qApp->closeAllWindows();
#ifdef Q_OS_WIN//windows系统
            QProcess::startDetached(qApp->applicationFilePath(), QStringList());
#else          //unix或者其他系统
            QProcess::startDetached("/bin/mv100", QStringList());
#endif
        }
    }
    else
    {
        gMessageBox->setInfo(QString("重启提示"), QString("\n是否要重启软件！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, false);
        gMessageBox->setButtonText(Cancel, "否");
        gMessageBox->setButtonText(Sure, "是");
        if(gMessageBox->exec() == MVMessageBox::Accepted)
        {
            qApp->closeAllWindows();
#ifdef Q_OS_WIN//windows系统
            QProcess::startDetached(qApp->applicationFilePath(), QStringList());
#else          //unix或者其他系统
            QProcess::startDetached("/bin/mv100", QStringList());
#endif
        }
    }
}
//槽:关机
void MV100_MainWidget::slt_btnShutdown_Clicked()
{
    gMessageBox->setInfo(QString("关机"), QString("\n是否要停止机器，退出系统!"),
                         QPixmap("://resource/resource/MessageBox/information.png"), true, false);
    gMessageBox->setButtonText(Cancel, "否");
    gMessageBox->setButtonText(Sure, "是");
    if(gMessageBox->exec() == MVMessageBox::Accepted)
        Shutdown();
}
//槽:开始测试延时，（空闲状态查询（温度，液体状态））
void MV100_MainWidget::slt_timerDelay_timeout()
{
    //停止计时器
    timerDelay->stop();

    if( !gScheduler->get_isBootCleanedFlag() ) //开机未清洗反应杯
    {
        //数据库中是否有测试存在
        if( isDataBaseHasTask() )
        {
            //样本加是否到位
            if( isSampleSlotReady() )
                gScheduler->TaskStart();
            else
                return;
        }
        else return; //未有申请好的测试，则返回
    }
    else
    {
        //数据库中是否有测试存在
        if( isDataBaseHasTask() ) gScheduler->TaskStart();
        else return;//未有申请好的测试，则返回
    }

    btnStart->setEnabled(false);
    btnPause->setEnabled(true);
    btnStop->setEnabled(true);
}
//槽:设置“开始”“暂停”“停止”三个按钮的状态（所有测试都完成，或 运行出错时 调用） (type=0：所有测试都完成， type=1:运行出错)
void MV100_MainWidget::slt_setButtonState(int type)
{
     //所有测试都完成
    if(type == 0)
    {
        btnStart->setEnabled(true);
        btnPause->setEnabled(false);
        btnStop->setEnabled(false);
    }
    else if(type == 1) //运行出错，停机
    {
        btnStart->setEnabled(false);
        btnPause->setEnabled(false);
        btnStop->setEnabled(false);
    }
}

/*******************状态栏(浮子传感器、温度、试剂状态信息)*****************************************/
//槽:温度状态（试剂槽）
void MV100_MainWidget::slt_setTemperature(double t)
{
    if(t > 40)
        slt_AlertMsg_show(0, "试剂盘温度过高");
    ThermometerReagent->setValue(t);
    ThermometerReagent->setToolTip(tr("试剂仓:")+QString::number(t, 'f', 1));
}
//槽:温度状态(反应盘)
void MV100_MainWidget::slt_setTemperature_Disk(double t)
{
    if(t<34)
    {
    }
    else if( (t>=34) && (t<=42) )
    {
        t = 36.0;
    }
    else //报警
    {
        if(t > 50)
            slt_AlertMsg_show(0, "反应盘温度过高");
    }
    ThermometerDisk->setValue(t);
    ThermometerDisk->setToolTip(tr("反应盘:")+QString::number(t, 'f', 1));
}

//槽：分类4种液体的状态
void MV100_MainWidget::slt_sortOutLiquidStatus(int s)
{
    labAlertMsgLiquid->clear();

    int Status[4];
    Status[0] = s & 0x01;  //去离子水
    Status[1] = s & 0x02;  //清洗液
    Status[2] = s & 0x04;  //稀废液
    Status[3] = s & 0x08;  //浓废液
    slt_DeionizedWater(Status[0]);
    slt_CleaningLiquid(Status[1]);
    slt_LowWaste(Status[2]);
    slt_HighWaste(Status[3]);

    //QSS
    QFile qss( ":/QSS/QSS/mv100.qss" );
    qss.open(QFile::ReadOnly);
    setStyleSheet( qss.readAll() );
    qss.close();
}
//槽:去离子水（纯净水）
void MV100_MainWidget::slt_DeionizedWater(int s)
{
    if(s == 0) //清零表示正常
    {
        Water_run_times = 0;
        labDeionizedWater->setObjectName("Water_normal");  //去离子水（纯净水）
    }
    else       //去离子水少
    {
        Water_run_times ++;
        labDeionizedWater->setObjectName("Water_lack"); //去离子水（纯净水）
        slt_AlertMsg_show(0, "缺去离子水");

        //缺水停机
        if(Water_run_times == gParameter->get_LessWaterTimes())
        {
            if(!gScheduler->get_isCycleFlag())//非周期指令，不作停机处理
            {
                Water_run_times = 0;
                return;
            }

            if(gParameter->get_isLiquidAlarmStop() == 1)
            {
                gScheduler->setMV100_WorkStatus(MV_State_Stop);
                gScheduler->slt_CycleTimer_Stop();
                slt_AlertMsg_show(0, "缺水停机");
            }
        }
    }
}
//槽:清洗液
void MV100_MainWidget::slt_CleaningLiquid(int s)
{
    if(s == 0) //清零表示正常
    {
        CleanLiquid_run_times = 0;
        labCleaningLiquid->setObjectName("CleaningLiquid_normal");  //去离子水（纯净水）
    }
    else       //浓清洗液少
    {
        CleanLiquid_run_times ++;
        labCleaningLiquid->setObjectName("CleaningLiquid_lack"); //去离子水（纯净水）
        slt_AlertMsg_show(0, "缺清洗液");

        //缺清洗液停机
        if(CleanLiquid_run_times == gParameter->get_LessCleanLiquidTimes())
        {
            if(!gScheduler->get_isCycleFlag())//非周期指令，不作停机处理
            {
                CleanLiquid_run_times = 0;
                return;
            }

            if(gParameter->get_isLiquidAlarmStop() == 1)
            {
                gScheduler->setMV100_WorkStatus(MV_State_Stop);
                gScheduler->slt_CycleTimer_Stop();
                slt_AlertMsg_show(0, "缺清洗液停机");
            }
        }
    }
}
//槽:低浓度废液
void MV100_MainWidget::slt_LowWaste(int s)
{
    if(s == 0) //清零表示正常
    {
        L_waste_run_times = 0;
        labLowWaste->setObjectName("LowWaste_normal");
    }
    else       //稀废液满
    {
        L_waste_run_times ++;
        labLowWaste->setObjectName("LowWaste_full");
        slt_AlertMsg_show(0, "稀废液满");

        //缺清洗液停机
        if(L_waste_run_times == gParameter->get_Full_L_WasteTimes())
        {
            if(!gScheduler->get_isCycleFlag())//非周期指令，不作停机处理
            {
                L_waste_run_times = 0;
                return;
            }

            if(gParameter->get_isLiquidAlarmStop() == 1)
            {
                gScheduler->setMV100_WorkStatus(MV_State_Stop);
                gScheduler->slt_CycleTimer_Stop();
                slt_AlertMsg_show(0, "稀废液满即将");
            }
        }
    }
}
//槽:高浓度废液
void MV100_MainWidget::slt_HighWaste(int s)
{
    if(s == 0) //清零表示正常
    {
        H_waste_run_times = 0;
        labHighWaste->setObjectName("HighWaste_normal");
    }
    else       //浓废液满
    {
        H_waste_run_times ++;
        labHighWaste->setObjectName("HighWaste_full");
        slt_AlertMsg_show(0, "浓废液满");

        //缺清洗液停机
        if(H_waste_run_times == gParameter->get_Full_H_WasteTimes())
        {
            if(!gScheduler->get_isCycleFlag())//非周期指令，不作停机处理
            {
                H_waste_run_times = 0;
                return;
            }

            if(gParameter->get_isLiquidAlarmStop() == 1)
            {
                gScheduler->setMV100_WorkStatus(MV_State_Stop);
                gScheduler->slt_CycleTimer_Stop();
                slt_AlertMsg_show(0, "浓废液满");
            }
        }
    }
}


/*******************堆叠部件*****************************************/
//新增界面到stack堆上并显示widget界面
void MV100_MainWidget::addWidgetToStack(QWidget *widget) {}
//设置当前要显示的界面
void MV100_MainWidget::showChildWidget(QWidget *widget) {}
//从stack堆上移除widget界面
void MV100_MainWidget::removeFromStack(QWidget *widget) {}


/*******************状态栏（警告、错误信息显示区）*****************************************/
//显示错误警告信息
void MV100_MainWidget::slt_AlertMsg_show(int type, QString msg)
{
    if(type == 1) //警告信息（周期指令信息）
    {
        if(msg == "")
            labAlertMsg->clear();
        else
        {
            QString text = labAlertMsg->text();
            if(text != "") text += ", ";
            labAlertMsg->setText(text + msg);
        }
    }
    else if(type == 0)  //警告信息（液体状态信息）
    {
        if(msg == "")
            labAlertMsgLiquid->clear();
        else
        {
            QString text = labAlertMsgLiquid->text();
            if(text != "") text += ", ";
            labAlertMsgLiquid->setText(text + msg);
        }
    }
}

//槽:更新状态栏上的时间
void MV100_MainWidget::slt_timer_timeout()
{
    QDateTime dateTime = QDateTime::currentDateTime();         //获取系统现在的时间
    QString str = dateTime.toString("yyyy-MM-dd\n   hh:mm");   //设置显示格式
    labDateTime->setText(str);
}

//槽:清除警告信息
void MV100_MainWidget::slt_btnCleanAlertMsg_Clicked() {labAlertMsg->clear(); labAlertMsgLiquid->clear();}

//槽:查看警告信息
void MV100_MainWidget::slt_btnShowAlertMsg_Clicked() {ErrorMsgWidget::getInstance()->show();}
