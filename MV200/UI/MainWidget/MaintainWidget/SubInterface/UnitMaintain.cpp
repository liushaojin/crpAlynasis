#include "UnitMaintain.h"

UnitMaintain::UnitMaintain(QWidget *parent)
    : QWidget(parent)
{
    initForm();          //初始化界面控件
    initStyle();         //初始化样式
    initData();          //初始化数据
    initConnect();       //连接信号与槽
}


//初始化界面控件
void UnitMaintain::initForm()
{
    initReactionDiskForm(); //反应盘
    initCleanNeedleForm();  //清洗机构
    initX_MotorForm();      //X电机
    initY_MotorForm();      //Y电机
    initProbeForm();        //加样针
    initMixForm();          //搅拌杆
    initSyringeForm();      //注射器
    initPumbValveForm();    //
    initT_CtlForm();        //
    initDC_MixMotorForm();
    initCleanMotorForm();   //清洗液进液泵
    initSelfDefineForm();//自定义

    //总布局
    layout = new QGridLayout;
    layout->setMargin(10);
    layout->setSpacing(10);
    layout->addWidget(grbReactionDisk,            0,  0,  4, 25);
    layout->addWidget(grbX_Motor,                 4,  0,  4, 25);
    layout->addWidget(grbProbe,                   8,  0,  4, 25);
    layout->addWidget(grbSyringe,                12,  0,  4, 25);
    layout->addWidget(grbDC_MixMotor,            16,  0,  4, 25);
    layout->addWidget(grbT_Ctl,                  20,  0,  4, 25);

    layout->addWidget(grbCleanNeedle,             0, 25,  4, 25);
    layout->addWidget(grbY_Motor,                 4, 25,  4, 25);
    layout->addWidget(grbMix,                     8, 25,  4, 25);
    layout->addWidget(grbPumbValve,              12, 25,  4, 25);
    layout->addWidget(grbCleanMotor,             16, 25,  4, 25);
    layout->addWidget(grbSelfDefine,             20, 25,  4, 25);

    this->setLayout(layout);
}


//反应盘
void UnitMaintain::initReactionDiskForm()
{
    grbReactionDisk = new QGroupBox("反应盘单元");
    btnReactionDisk_Reset = new QPushButton("复 位");    //复位
    rdoReactionDisk_Forward = new QRadioButton("正 转"); //正转
    rdoReactionDisk_Reverse = new QRadioButton("反 转"); //反转
    labReactionDisk_Move = new QLabel("旋转指定个杯位:");          //旋转指定个杯位
    editReactionDisk_Cup = new QLineEdit;
    btnReactionDisk_Move = new QPushButton("运 行");
    lytReactionDisk = new QGridLayout;
    lytReactionDisk->setMargin(5);
    lytReactionDisk->setSpacing(5);
    lytReactionDisk->addWidget(labReactionDisk_Move,    0,  0, 2,  5, Qt::AlignRight);
    lytReactionDisk->addWidget(editReactionDisk_Cup,    0,  5, 2,  5, Qt::AlignLeft);
    lytReactionDisk->addWidget(rdoReactionDisk_Forward, 0, 10, 2,  5, Qt::AlignCenter);
    lytReactionDisk->addWidget(rdoReactionDisk_Reverse, 0, 15, 2,  5, Qt::AlignCenter);
    lytReactionDisk->addWidget(btnReactionDisk_Reset,   2,  0, 2, 10, Qt::AlignCenter);
    lytReactionDisk->addWidget(btnReactionDisk_Move,    2, 10, 2, 10, Qt::AlignCenter);
    grbReactionDisk->setLayout(lytReactionDisk);
}

//清洗机构
void UnitMaintain::initCleanNeedleForm()
{
    //清洗机构
    grbCleanNeedle = new QGroupBox("清洗机构");
    btnCleanNeedle_Reset = new QPushButton("复位");             //复位
    btnCleanNeedle_MoveDownCup = new QPushButton("下到反应杯内"); //向下运动到反应杯内
    btnCleanNeedle_MoveUpCup = new QPushButton("上到反应杯口");   //向上运行到反应杯口
    labCleanNeedle_UpSteps = new QLabel("步数：");          //向上运动一定步数
    editCleanNeedle_UpSteps = new  QLineEdit;              //
    btnCleanNeedle_Up = new QPushButton("上");            //
    labCleanNeedle_DownSteps = new QLabel("步数:");          //向下运动一定步数
    editCleanNeedle_DownSteps = new QLineEdit;             //
    btnCleanNeedle_Down = new QPushButton("下");          //
    lytCleanNeedle = new QGridLayout;
    lytCleanNeedle->setMargin(5);
    lytCleanNeedle->setSpacing(5);
    lytCleanNeedle->addWidget(btnCleanNeedle_Reset,        0,  0, 5,  10, Qt::AlignCenter);
    lytCleanNeedle->addWidget(btnCleanNeedle_MoveDownCup,  0, 10, 5,  10, Qt::AlignCenter);
    lytCleanNeedle->addWidget(btnCleanNeedle_MoveUpCup,    0, 20, 5,  10, Qt::AlignCenter);
    lytCleanNeedle->addWidget(labCleanNeedle_UpSteps,      5,  0, 5,  10, Qt::AlignRight);
    lytCleanNeedle->addWidget(editCleanNeedle_UpSteps,     5, 10, 5,  10, Qt::AlignLeft);
    lytCleanNeedle->addWidget(btnCleanNeedle_Up,           5, 20, 5,  10, Qt::AlignCenter);
    lytCleanNeedle->addWidget(labCleanNeedle_DownSteps,    10,  0, 5,  10, Qt::AlignRight);
    lytCleanNeedle->addWidget(editCleanNeedle_DownSteps,   10, 10, 5,  10, Qt::AlignLeft);
    lytCleanNeedle->addWidget(btnCleanNeedle_Down,         10, 20, 5,  10, Qt::AlignCenter);
    grbCleanNeedle->setLayout(lytCleanNeedle);
}

//X电机
void UnitMaintain::initX_MotorForm()
{
    grbX_Motor = new QGroupBox("X电机");
    btnX_Motor_Reset = new QPushButton("X电机复位");  //X电机复位
    btnX_Motor_Home = new QPushButton("移动到清洗池");   //X方向运行到清洗池位置
    btnX_Motor_Disk = new QPushButton("反应盘搅拌位");   //X方向运行到反应盘搅拌试剂样本位置
    labX_Motor_R_Pos = new QLabel("试剂位:");       //试剂位
    editX_Motor_R_Pos = new QLineEdit;
    btnX_Motor_Move2R = new QPushButton("移动到试剂位"); //X方向运行到指定试剂位号取试剂位置
    labX_Motor_S_Pos = new QLabel("样本位:");       //样本位
    editX_Motor_Slot = new QLineEdit;    //样本槽号
    editX_Motor_SlotNo = new QLineEdit;  //样本位
    btnX_Motor_Move2S = new QPushButton("移动到样本位");     //X方向运行到指定样本位取样本位置
    btnX_Motor_Move2S_Mix = new QPushButton("搅拌样本"); //X方向运行到指定样本位搅拌样本位置
    lytX_Motor = new QGridLayout;
    lytX_Motor->setMargin(5);
    lytX_Motor->setSpacing(5);
    lytX_Motor->addWidget(btnX_Motor_Reset,  0,  0, 5, 10, Qt::AlignCenter);
    lytX_Motor->addWidget(btnX_Motor_Home,   0, 10, 5, 10, Qt::AlignCenter);
    lytX_Motor->addWidget(btnX_Motor_Disk,   0, 20, 5, 10, Qt::AlignCenter);
    lytX_Motor->addWidget(labX_Motor_R_Pos,   5,  0, 5, 10, Qt::AlignRight);
    lytX_Motor->addWidget(editX_Motor_R_Pos,  5, 10, 5, 10, Qt::AlignLeft);
    lytX_Motor->addWidget(btnX_Motor_Move2R,  5, 20, 5, 10, Qt::AlignCenter);
    lytX_Motor->addWidget(labX_Motor_S_Pos,   10,  0, 5, 10, Qt::AlignRight);
    lytX_Motor->addWidget(editX_Motor_Slot,   10, 10, 5, 10, Qt::AlignLeft);
    lytX_Motor->addWidget(editX_Motor_SlotNo, 10, 20, 5, 10, Qt::AlignCenter);
    lytX_Motor->addWidget(btnX_Motor_Move2S,      15,  0, 5, 10, Qt::AlignCenter);
    lytX_Motor->addWidget(btnX_Motor_Move2S_Mix,  15, 20, 5, 10, Qt::AlignCenter);
    grbX_Motor->setLayout(lytX_Motor);
}

//Y电机
void UnitMaintain::initY_MotorForm()
{
    grbY_Motor = new QGroupBox("Y电机");
    btnY_Motor_Reset = new QPushButton("Y电机复位");  //Y电机复位
    btnY_Motor_Home = new QPushButton("移动到清洗池");   //Y方向运行到清洗池位置
    btnY_Motor_Disk = new QPushButton("反应盘搅拌位");   //Y
    labY_Motor_R_Pos = new QLabel("试剂位:");       //试剂位
    editY_Motor_R_Pos = new QLineEdit;
    btnY_Motor_Move2R = new QPushButton("移动到试剂位"); //Y方向运行到指定试剂位号取试剂位置
    labY_Motor_S_Pos = new QLabel("样本位:");       //样本位
    editY_Motor_Slot = new QLineEdit;    //样本槽号
    editY_Motor_SlotNo = new QLineEdit;  //样本位
    btnY_Motor_Move2S = new QPushButton("移动到样本位");     //Y方向运行到指定样本位取样本位置
    btnY_Motor_Move2S_Mix = new QPushButton("搅拌样本"); //Y方向运行到指定样本位搅拌样本位置
    lytY_Motor = new QGridLayout;
    lytY_Motor->setMargin(5);
    lytY_Motor->setSpacing(5);
    lytY_Motor->addWidget(btnY_Motor_Reset,  0,  0, 5, 10, Qt::AlignCenter);
    lytY_Motor->addWidget(btnY_Motor_Home,   0, 10, 5, 10, Qt::AlignCenter);
    lytY_Motor->addWidget(btnY_Motor_Disk,   0, 20, 5, 10, Qt::AlignCenter);
    lytY_Motor->addWidget(labY_Motor_R_Pos,   5,  0, 5, 10, Qt::AlignRight);
    lytY_Motor->addWidget(editY_Motor_R_Pos,  5, 10, 5, 10, Qt::AlignLeft);
    lytY_Motor->addWidget(btnY_Motor_Move2R,  5, 20, 5, 10, Qt::AlignCenter);
    lytY_Motor->addWidget(labY_Motor_S_Pos,   10,  0, 5, 10, Qt::AlignRight);
    lytY_Motor->addWidget(editY_Motor_Slot,   10, 10, 5, 10, Qt::AlignLeft);
    lytY_Motor->addWidget(editY_Motor_SlotNo, 10, 20, 5, 10, Qt::AlignCenter);
    lytY_Motor->addWidget(btnY_Motor_Move2S,      15,  0, 5, 10, Qt::AlignCenter);
    lytY_Motor->addWidget(btnY_Motor_Move2S_Mix,  15, 20, 5, 10, Qt::AlignCenter);
    grbY_Motor->setLayout(lytY_Motor);
}

//加样针
void UnitMaintain::initProbeForm()
{
    //加样针
    grbProbe = new QGroupBox("加样针");
    btnProbeReset = new QPushButton("垂直复位"); //垂直复位
    btnProbeClean = new QPushButton("下-清洗池"); //向下运动到清洗处
    btnProbeDownDisk = new QPushButton("下-反应盘");//向下运动到反应盘
    btnProbeDetect = new QPushButton("下-液面");  //向下运动到探测面
    labProbeStep = new QLabel("步数:");         //运动步数
    editProbeStep = new QLineEdit;
    btnProbeDown = new QPushButton("下");//向下运动一定步数
    btnProbeUp = new QPushButton("上");  //向上运动一定步数
    lytProbe = new QGridLayout;
    lytProbe->setMargin(5);
    lytProbe->setSpacing(5);
    lytProbe->addWidget(btnProbeReset,     0,  0, 5, 10, Qt::AlignCenter);
    lytProbe->addWidget(btnProbeClean,     0, 10, 5, 10, Qt::AlignCenter);
    lytProbe->addWidget(btnProbeDownDisk,  0, 20, 5, 10, Qt::AlignCenter);
    lytProbe->addWidget(btnProbeDetect,    0, 30, 5, 10, Qt::AlignCenter);
    lytProbe->addWidget(labProbeStep,     5,  0, 5, 10, Qt::AlignCenter);
    lytProbe->addWidget(editProbeStep,    5, 10, 5, 10, Qt::AlignCenter);
    lytProbe->addWidget(btnProbeDown,     5, 20, 5, 10, Qt::AlignCenter);
    lytProbe->addWidget(btnProbeUp,       5, 30, 5, 10, Qt::AlignCenter);
    grbProbe->setLayout(lytProbe);
}

//搅拌杆
void UnitMaintain::initMixForm()
{
    grbMix = new QGroupBox("搅拌杆");
    btnMixReset = new QPushButton("垂直复位"); //垂直复位
    btnMixClean = new QPushButton("下-清洗池"); //向下运动到清洗处
    btnMixDownDisk = new QPushButton("下-反应盘");//向下运动到反应盘
    btnMixDownMixS = new QPushButton("下-搅样");  //
    labMixStep = new QLabel("步数:");         //运动步数
    editMixStep = new QLineEdit;
    btnMixDown = new QPushButton("下");//向下运动一定步数
    btnMixUp = new QPushButton("上");  //向上运动一定步数
    lytMix = new QGridLayout;
    lytMix->setMargin(5);
    lytMix->setSpacing(5);
    lytMix->addWidget(btnMixReset,     0,  0, 5, 10, Qt::AlignCenter);
    lytMix->addWidget(btnMixClean,     0, 10, 5, 10, Qt::AlignCenter);
    lytMix->addWidget(btnMixDownDisk,  0, 20, 5, 10, Qt::AlignCenter);
    lytMix->addWidget(btnMixDownMixS,  0, 30, 5, 10, Qt::AlignCenter);
    lytMix->addWidget(labMixStep,     5,  0, 5, 10, Qt::AlignCenter);
    lytMix->addWidget(editMixStep,    5, 10, 5, 10, Qt::AlignCenter);
    lytMix->addWidget(btnMixDown,     5, 20, 5, 10, Qt::AlignCenter);
    lytMix->addWidget(btnMixUp,       5, 30, 5, 10, Qt::AlignCenter);
    grbMix->setLayout(lytMix);
}

//注射器
void UnitMaintain::initSyringeForm()
{
    //注射器
    grbSyringe = new QGroupBox("注射器");
    btnSyringeReset = new QPushButton("垂直复位"); //垂直复位
    labSyringeStep = new QLabel("样本量（ul）");       //运动步数
    editSyringeStep = new QLineEdit;
    btnSyringeAbsorb = new QPushButton("吸");//吸（0.1ul）
    btnSyringeInject = new QPushButton("排");  //吐（0.1ul）
    lytSyringe = new QGridLayout;
    lytSyringe->setMargin(5);
    lytSyringe->setSpacing(5);
    lytSyringe->addWidget(btnSyringeReset,   0, 10, 5, 10, Qt::AlignCenter);
    lytSyringe->addWidget(labSyringeStep,    5,  0, 5, 10, Qt::AlignRight);
    lytSyringe->addWidget(editSyringeStep,   5, 10, 5, 10, Qt::AlignLeft);
    lytSyringe->addWidget(btnSyringeAbsorb, 10,  0, 5, 10, Qt::AlignCenter);
    lytSyringe->addWidget(btnSyringeInject, 10, 10, 5, 10, Qt::AlignCenter);
    grbSyringe->setLayout(lytSyringe);
}

//泵阀
void UnitMaintain::initPumbValveForm()
{
    grbPumbValve = new QGroupBox("泵阀");
    cboPumbValve = new QComboBox;
    btnOpenPumbValve = new QPushButton("打开");
    btnClosePumbValve = new QPushButton("关闭");
    labPumbValveTime = new QLabel("时间(ms):");
    editPumbValveTime = new QLineEdit;
    btnPumbValveTime = new QPushButton("打开");
    lytPumbValve = new QGridLayout;
    lytPumbValve->setMargin(5);
    lytPumbValve->setSpacing(5);
    lytPumbValve->addWidget(cboPumbValve,        0,  0, 5, 10, Qt::AlignCenter);
    lytPumbValve->addWidget(btnOpenPumbValve,    0, 10, 5, 10, Qt::AlignCenter);
    lytPumbValve->addWidget(btnClosePumbValve,   0, 20, 5, 10, Qt::AlignCenter);
    lytPumbValve->addWidget(labPumbValveTime,    5,  0, 5, 10, Qt::AlignRight);
    lytPumbValve->addWidget(editPumbValveTime,   5, 10, 5, 10, Qt::AlignLeft);
    lytPumbValve->addWidget(btnPumbValveTime,    5, 20, 5, 10, Qt::AlignCenter);
    grbPumbValve->setLayout(lytPumbValve);
}

//温控
void UnitMaintain::initT_CtlForm()
{
    //温控
    grbT_Ctl = new QGroupBox("温控");
    btnT_Ctl_Disk = new QPushButton("反应盘温度");
    editT_Ctl_Disk = new QLineEdit;
    btnT_Ctl_Reagent = new QPushButton("试剂仓温度");
    editT_Ctl_Reagent = new QLineEdit;
    lytT_Ctl = new QGridLayout;
    lytT_Ctl->setMargin(5);
    lytT_Ctl->setSpacing(5);
    lytT_Ctl->addWidget(btnT_Ctl_Disk,  0,  0, 5, 10, Qt::AlignCenter);
    lytT_Ctl->addWidget(editT_Ctl_Disk, 0, 10, 5, 10, Qt::AlignCenter);
    lytT_Ctl->addWidget(btnT_Ctl_Reagent,  5,  0, 5, 10, Qt::AlignCenter);
    lytT_Ctl->addWidget(editT_Ctl_Reagent, 5, 10, 5, 10, Qt::AlignCenter);
    grbT_Ctl->setLayout(lytT_Ctl);
}

//搅拌直流电机
void UnitMaintain::initDC_MixMotorForm()
{
    //搅拌直流电机
    grbDC_MixMotor = new QGroupBox("搅拌直流电机");
    labDC_MixMotorCycleCount = new QLabel("周期数：");
    editDC_MixMotor = new QLineEdit;
    btnOpenDC_MixMotorForward = new QPushButton("正向转X周期");
    btnOpenDC_MixMotorReverse = new QPushButton("反向转X周期");
    btnOpenDC_MixMotor = new QPushButton("开");
    btnCloseDC_MixMotor = new QPushButton("关");
    lytDC_MixMotor = new QGridLayout;
    lytDC_MixMotor->setMargin(5);
    lytDC_MixMotor->setSpacing(5);
    lytDC_MixMotor->addWidget(labDC_MixMotorCycleCount,    0,  0, 5, 10, Qt::AlignRight);
    lytDC_MixMotor->addWidget(editDC_MixMotor,             0, 10, 5, 10, Qt::AlignLeft);
    lytDC_MixMotor->addWidget(btnOpenDC_MixMotorForward,   5,  0, 5, 10, Qt::AlignCenter);
    lytDC_MixMotor->addWidget(btnOpenDC_MixMotorReverse,   5, 10, 5, 10, Qt::AlignCenter);
    lytDC_MixMotor->addWidget(btnOpenDC_MixMotor,         10,  0, 5, 10, Qt::AlignCenter);
    lytDC_MixMotor->addWidget(btnCloseDC_MixMotor,        10, 10, 5, 10, Qt::AlignCenter);
    grbDC_MixMotor->setLayout(lytDC_MixMotor);
}

//清洗液进液泵
void UnitMaintain::initCleanMotorForm()
{
    grbCleanMotor = new QGroupBox("清洗液进液泵");
    labCleanMotorCycleCount = new QLabel("周期数：");
    editCleanMotor = new QLineEdit;
    btnOpenCleanMotorForward = new QPushButton("正向转X周期");
    btnOpenCleanMotorReverse = new QPushButton("反向转X周期");
    btnOpenCleanMotor = new QPushButton("开");
    btnCloseCleanMotor = new QPushButton("关");
    lytCleanMotor = new QGridLayout;
    lytCleanMotor->setMargin(5);
    lytCleanMotor->setSpacing(5);
    lytCleanMotor->addWidget(labCleanMotorCycleCount,    0,  0, 5, 10, Qt::AlignRight);
    lytCleanMotor->addWidget(editCleanMotor,             0, 10, 5, 10, Qt::AlignLeft);
    lytCleanMotor->addWidget(btnOpenCleanMotorForward,   5,  0, 5, 10, Qt::AlignCenter);
    lytCleanMotor->addWidget(btnOpenCleanMotorReverse,   5, 10, 5, 10, Qt::AlignCenter);
    lytCleanMotor->addWidget(btnOpenCleanMotor,         10,  0, 5, 10, Qt::AlignCenter);
    lytCleanMotor->addWidget(btnCloseCleanMotor,        10, 10, 5, 10, Qt::AlignCenter);
    grbCleanMotor->setLayout(lytCleanMotor);
}

//自定义
void UnitMaintain::initSelfDefineForm()
{
    //自定义指令
    grbSelfDefine = new QGroupBox;
    labSelfDefine = new QLabel;
    editSelfDefine = new QLineEdit;
    btnSelfDefine = new QPushButton;
    layoutSelfDefine = new QGridLayout;
    layoutSelfDefine->setMargin(5);
    layoutSelfDefine->setSpacing(10);
    layoutSelfDefine->addWidget(labSelfDefine,  0,  0, 2, 10, Qt::AlignCenter);
    layoutSelfDefine->addWidget(editSelfDefine, 0, 10, 2, 60);
    layoutSelfDefine->addWidget(btnSelfDefine,  0, 70, 2, 10, Qt::AlignCenter);
    grbSelfDefine->setLayout(layoutSelfDefine);

    //自定义指令
    grbSelfDefine->setTitle("自定义测试");
    labSelfDefine->setText("自定义指令:");
    btnSelfDefine->setText("发送");
    editSelfDefine->setText("FA 0f 01 3F 40 71 03 01 00 FB");

}

//初始化样式
void UnitMaintain::initStyle()
{
    //    //QSS
    //    QFile qss( ":/QSS/QSS/style2.qss" );
    //    qss.open(QFile::ReadOnly);
    //    setStyleSheet( qss.readAll() );
    //    qss.close();
    //    btnReactionDisk_Reset->setFixedSize(150, 55);
    //    btnReactionDisk_Move->setFixedSize(150, 55);
}

//初始化数据
void UnitMaintain::initData()
{
    initComboList();     //初始化下拉列表
}

//初始化下拉列表
void UnitMaintain::initComboList()
{
}

//连接信号与槽
void UnitMaintain::initConnect()
{
    //反应盘
    connect(btnReactionDisk_Reset, SIGNAL(clicked(bool)), this, SLOT(slt_btnReactionDisk_Reset_Clicked()));
    connect(btnReactionDisk_Move, SIGNAL(clicked(bool)), this, SLOT(slt_btnReactionDisk_Move_Clicked()));
    //清洗机构
    connect(btnCleanNeedle_Reset, SIGNAL(clicked(bool)), this, SLOT(slt_btnCleanNeedle_Reset_Clicked()));
    connect(btnCleanNeedle_MoveDownCup, SIGNAL(clicked(bool)), this, SLOT(slt_btnCleanNeedle_MoveDownCup_Clicked()));
    connect(btnCleanNeedle_MoveUpCup, SIGNAL(clicked(bool)), this, SLOT(slt_btnCleanNeedle_MoveUpCup_Clicked()));
    connect(btnCleanNeedle_Up, SIGNAL(clicked(bool)), this, SLOT(slt_btnCleanNeedle_Up_Clicked()));
    connect(btnCleanNeedle_Down, SIGNAL(clicked(bool)), this, SLOT(slt_btnCleanNeedle_Down_Clicked()));
    //X电机
    connect(btnX_Motor_Reset, SIGNAL(clicked(bool)), this, SLOT(slt_btnX_Motor_Reset_Clicked()));
    connect(btnX_Motor_Home, SIGNAL(clicked(bool)), this, SLOT(slt_btnX_Motor_Home_Clicked()));
    connect(btnX_Motor_Disk, SIGNAL(clicked(bool)), this, SLOT(slt_btnX_Motor_Disk_Clicked()));
    connect(btnX_Motor_Move2R, SIGNAL(clicked(bool)), this, SLOT(slt_btnX_Motor_Move2R_Clicked()));
    connect(btnX_Motor_Move2S, SIGNAL(clicked(bool)), this, SLOT(slt_btnX_Motor_Move2S_Clicked()));
    connect(btnX_Motor_Move2S_Mix, SIGNAL(clicked(bool)), this, SLOT(slt_btnX_Motor_Move2S_Mix_Clicked()));
    //Y电机
    connect(btnY_Motor_Reset, SIGNAL(clicked(bool)), this, SLOT(slt_btnY_Motor_Reset_Clicked()));
    connect(btnY_Motor_Home, SIGNAL(clicked(bool)), this, SLOT(slt_btnY_Motor_Home_Clicked()));
    connect(btnY_Motor_Disk, SIGNAL(clicked(bool)), this, SLOT(slt_btnY_Motor_Disk_Clicked()));
    connect(btnY_Motor_Move2R, SIGNAL(clicked(bool)), this, SLOT(slt_btnY_Motor_Move2R_Clicked()));
    connect(btnY_Motor_Move2S, SIGNAL(clicked(bool)), this, SLOT(slt_btnY_Motor_Move2S_Clicked()));
    connect(btnY_Motor_Move2S_Mix, SIGNAL(clicked(bool)), this, SLOT(slt_btnY_Motor_Move2S_Mix_Clicked()));
    //加样针
    connect(btnProbeReset, SIGNAL(clicked(bool)), this, SLOT(slt_btnProbeReset_Clicked()));
    connect(btnProbeClean, SIGNAL(clicked(bool)), this, SLOT(slt_btnProbeClean_Clicked()));
    connect(btnProbeDownDisk, SIGNAL(clicked(bool)), this, SLOT(slt_btnProbeDownDisk_Clicked()));
    connect(btnProbeDetect, SIGNAL(clicked(bool)), this, SLOT(slt_btnProbeDetect_Clicked()));
    connect(btnProbeDown, SIGNAL(clicked(bool)), this, SLOT(slt_btnProbeDown_Clicked()));
    connect(btnProbeUp, SIGNAL(clicked(bool)), this, SLOT(slt_btnProbeUp_Clicked()));
    //搅拌杆
    connect(btnMixReset, SIGNAL(clicked(bool)), this, SLOT(slt_btnMixReset_Clicked()));
    connect(btnMixClean, SIGNAL(clicked(bool)), this, SLOT(slt_btnMixClean_Clicked()));
    connect(btnMixDownDisk, SIGNAL(clicked(bool)), this, SLOT(slt_btnMixDownDisk_Clicked()));
    connect(btnMixDownMixS, SIGNAL(clicked(bool)), this, SLOT(slt_btnMixDownMixS_Clicked()));
    connect(btnMixDown, SIGNAL(clicked(bool)), this, SLOT(slt_btnMixDown_Clicked()));
    connect(btnMixUp, SIGNAL(clicked(bool)), this, SLOT(slt_btnMixUp_Clicked()));
    //注射器
    connect(btnSyringeReset, SIGNAL(clicked(bool)), this, SLOT(slt_btnSyringeReset_Clicked()));
    connect(btnSyringeAbsorb, SIGNAL(clicked(bool)), this, SLOT(slt_btnSyringeAbsorb_Clicked()));
    connect(btnSyringeInject, SIGNAL(clicked(bool)), this, SLOT(slt_btnSyringeInject_Clicked()));
    //泵阀
    connect(btnOpenPumbValve, SIGNAL(clicked(bool)), this, SLOT(slt_btnOpenPumbValve_Clicked()));
    connect(btnClosePumbValve, SIGNAL(clicked(bool)), this, SLOT(slt_btnClosePumbValve_Clicked()));
    connect(btnPumbValveTime, SIGNAL(clicked(bool)), this, SLOT(slt_btnPumbValveTime_Clicked()));
    //温控
    connect(btnT_Ctl_Disk, SIGNAL(clicked(bool)), this, SLOT(slt_btnT_Ctl_Disk_Clicked()));
    connect(btnT_Ctl_Reagent, SIGNAL(clicked(bool)), this, SLOT(slt_btnT_Ctl_Reagent_Clicked()));

    //搅拌直流电机
    connect(btnOpenDC_MixMotorForward, SIGNAL(clicked(bool)), this, SLOT(slt_btnOpenDC_MixMotorForward_Clicked()));
    connect(btnOpenDC_MixMotorReverse, SIGNAL(clicked(bool)), this, SLOT(slt_btnOpenDC_MixMotorReverse_Clicked()));
    connect(btnOpenDC_MixMotor, SIGNAL(clicked(bool)), this, SLOT(slt_btnOpenDC_MixMotor_Clicked()));
    connect(btnCloseDC_MixMotor, SIGNAL(clicked(bool)), this, SLOT(slt_btnCloseDC_MixMotor_Clicked()));

    //清洗液进液泵
    connect(btnOpenCleanMotorForward, SIGNAL(clicked(bool)), this, SLOT(slt_btnOpenCleanMotorForward_Clicked()));
    connect(btnOpenCleanMotorReverse, SIGNAL(clicked(bool)), this, SLOT(slt_btnOpenCleanMotorReverse_Clicked()));
    connect(btnOpenCleanMotor, SIGNAL(clicked(bool)), this, SLOT(slt_btnOpenCleanMotor_Clicked()));
    connect(btnCloseCleanMotor, SIGNAL(clicked(bool)), this, SLOT(slt_btnCloseCleanMotor_Clicked()));

    //
    connect(btnSelfDefine, SIGNAL(clicked(bool)), this, SLOT(slt_btnSelfDefine_Clicked()));
}


//反应盘复位
void UnitMaintain::slt_btnReactionDisk_Reset_Clicked()
{
    QString msg = "FA 0f 01 3F 41 01 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//反应盘转到指定杯位
void UnitMaintain::slt_btnReactionDisk_Move_Clicked()
{
    int cups = editReactionDisk_Cup->text().toInt();
    QString msg;
    if(rdoReactionDisk_Forward->isChecked()) //是否正转
        msg = "FA 0f 01 3F 41 05 01 " + AppHelper::IntToHexStr(cups, 2) +" 00 00 FB";
    else
        msg = "FA 0f 01 3F 41 05 00 " + AppHelper::IntToHexStr(cups, 2) +" 00 00 FB";

    gSerialThread->WriteDebugFrame(msg);
}


//清洗机构-复位
void UnitMaintain::slt_btnCleanNeedle_Reset_Clicked()
{
    QString msg = "FA 0f 01 3F 42 01 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//清洗机构-向下运动到反应杯内
void UnitMaintain::slt_btnCleanNeedle_MoveDownCup_Clicked()
{
    QString msg = "FA 0f 01 3F 42 04 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//清洗机构-向上运行到反应杯口
void UnitMaintain::slt_btnCleanNeedle_MoveUpCup_Clicked()
{
    QString msg = "FA 0f 01 3F 42 05 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//清洗机构-上
void UnitMaintain::slt_btnCleanNeedle_Up_Clicked()
{
    int steps = editCleanNeedle_UpSteps->text().toInt();
    QString p1 = AppHelper::IntToHexStr(steps/128, 2);    //试剂量
    QString p2 = AppHelper::IntToHexStr(steps%128, 2);    //试剂量
    QString msg = "FA 0f 01 3F 42 03 " + p1 + " " + p2 + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//清洗机构-下
void UnitMaintain::slt_btnCleanNeedle_Down_Clicked()
{
    int steps = editCleanNeedle_UpSteps->text().toInt();
    QString p1 = AppHelper::IntToHexStr(steps/128, 2);    //试剂量
    QString p2 = AppHelper::IntToHexStr(steps%128, 2);    //试剂量
    QString msg = "FA 0f 01 3F 42 02 " + p1 + " " + p2 + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}


//X电机复位
void UnitMaintain::slt_btnX_Motor_Reset_Clicked()
{
    QString msg = "FA 0f 01 3F 43 01 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//X方向运行到清洗池位置
void UnitMaintain::slt_btnX_Motor_Home_Clicked()
{
    QString msg = "FA 0f 01 3F 43 02 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//X方向运行到反应盘搅拌试剂样本位置
void UnitMaintain::slt_btnX_Motor_Disk_Clicked()
{
    QString msg = "FA 0f 01 3F 43 03 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//X方向运行到指定试剂位号取试剂位置
void UnitMaintain::slt_btnX_Motor_Move2R_Clicked()
{
    QString pos;
    int Rno = editX_Motor_R_Pos->text().toInt();
    if( Rno % 2 )
        pos = "01";
    else
        pos = "02";
    QString msg = "FA 0f 01 3F 43 05 " + pos + " " + AppHelper::IntToHexStr(Rno,2) + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//X方向运行到指定样本位取样本位置
void UnitMaintain::slt_btnX_Motor_Move2S_Clicked()
{
    int slot = editX_Motor_Slot->text().toInt();
    int sNo = editX_Motor_SlotNo->text().toInt();
    QString msg = "FA 0f 01 3F 43 06 " + AppHelper::IntToHexStr(slot, 2) + " " + AppHelper::IntToHexStr(sNo, 2) + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//X方向运行到指定样本位搅拌样本位置
void UnitMaintain::slt_btnX_Motor_Move2S_Mix_Clicked()
{
    int slot = editX_Motor_Slot->text().toInt();
    int sNo = editX_Motor_SlotNo->text().toInt();
    QString msg = "FA 0f 01 3F 43 07 " + AppHelper::IntToHexStr(slot, 2) + " " + AppHelper::IntToHexStr(sNo, 2) + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}


//Y电机复位
void UnitMaintain::slt_btnY_Motor_Reset_Clicked()
{
    QString msg = "FA 0f 01 3F 44 01 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//Y方向运行到清洗池位置
void UnitMaintain::slt_btnY_Motor_Home_Clicked()
{
    QString msg = "FA 0f 01 3F 44 02 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//Y方向运行到反应盘搅拌试剂样本位置
void UnitMaintain::slt_btnY_Motor_Disk_Clicked()
{
    QString msg = "FA 0f 01 3F 44 03 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//Y方向运行到指定试剂位号取试剂位置
void UnitMaintain::slt_btnY_Motor_Move2R_Clicked()
{
    QString pos;
    int Rno = editY_Motor_R_Pos->text().toInt();
    if( Rno % 2 )
        pos = "01";
    else
        pos = "02";
    QString msg = "FA 0f 01 3F 44 04 " + pos + " " + AppHelper::IntToHexStr(Rno,2) + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//Y方向运行到指定样本位取样本位置
void UnitMaintain::slt_btnY_Motor_Move2S_Clicked()
{
    int slot = editY_Motor_Slot->text().toInt();
    int sNo = editY_Motor_SlotNo->text().toInt();
    QString msg = "FA 0f 01 3F 44 05 " + AppHelper::IntToHexStr(slot, 2) + " " + AppHelper::IntToHexStr(sNo, 2) + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//Y方向运行到指定样本位搅拌样本位置
void UnitMaintain::slt_btnY_Motor_Move2S_Mix_Clicked()
{

}


//加样针-垂直复位
void UnitMaintain::slt_btnProbeReset_Clicked()
{
    QString msg = "FA 0f 01 3F 45 01 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//加样针-向下运动到清洗处
void UnitMaintain::slt_btnProbeClean_Clicked()
{
    QString msg = "FA 0f 01 3F 45 04 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//加样针-向下运动到反应盘
void UnitMaintain::slt_btnProbeDownDisk_Clicked()
{
    QString msg = "FA 0f 01 3F 45 05 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//加样针-向下运动到探测面
void UnitMaintain::slt_btnProbeDetect_Clicked()
{
    QString msg = "FA 0f 01 3F 45 06 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//加样针-向下运动一定步数
void UnitMaintain::slt_btnProbeDown_Clicked()
{
    int step = editProbeStep->text().toInt();
    QString hight = AppHelper::IntToHexStr(step/128, 2);
    QString low = AppHelper::IntToHexStr(step%128, 2);
    QString msg = "FA 0f 01 3F 45 02 " + hight + " " + low + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//加样针-向上运动一定步数
void UnitMaintain::slt_btnProbeUp_Clicked()
{
    int step = editProbeStep->text().toInt();
    QString hight = AppHelper::IntToHexStr(step/128, 2);
    QString low = AppHelper::IntToHexStr(step%128, 2);
    QString msg = "FA 0f 01 3F 45 03 " + hight + " " + low + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}


//搅拌杆-垂直复位
void UnitMaintain::slt_btnMixReset_Clicked()
{
    QString msg = "FA 0f 01 3F 46 01 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//搅拌杆-向下运动到清洗处
void UnitMaintain::slt_btnMixClean_Clicked()
{
    QString msg = "FA 0f 01 3F 46 04 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//搅拌杆-向下运动到反应盘
void UnitMaintain::slt_btnMixDownDisk_Clicked()
{
    QString msg = "FA 0f 01 3F 46 05 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//搅拌杆-向下运动到样本搅拌处
void UnitMaintain::slt_btnMixDownMixS_Clicked()
{
    QString msg = "FA 0f 01 3F 46 06 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//搅拌杆-向下运动一定步数
void UnitMaintain::slt_btnMixDown_Clicked()
{
    int step = editMixStep->text().toInt();
    QString hight = AppHelper::IntToHexStr(step/128, 2);
    QString low = AppHelper::IntToHexStr(step%128, 2);
    QString msg = "FA 0f 01 3F 46 02 " + hight + " " + low + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//搅拌杆-向上运动一定步数
void UnitMaintain::slt_btnMixUp_Clicked()
{
    int step = editMixStep->text().toInt();
    QString hight = AppHelper::IntToHexStr(step/128, 2);
    QString low = AppHelper::IntToHexStr(step%128, 2);
    QString msg = "FA 0f 01 3F 46 03 " + hight + " " + low + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}


//注射器-垂直复位
void UnitMaintain::slt_btnSyringeReset_Clicked()
{
    QString msg = "FA 0f 01 3F 47 01 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//注射器-吸（0.1ul）
void UnitMaintain::slt_btnSyringeAbsorb_Clicked()
{
    int vol = editSyringeStep->text().toInt();
    QString hight = AppHelper::IntToHexStr(vol*10/128, 2);
    QString low = AppHelper::IntToHexStr(vol*10%128, 2);
    QString msg = "FA 0f 01 3F 47 02 " + hight + " " + low + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//注射器-吐（0.1ul）
void UnitMaintain::slt_btnSyringeInject_Clicked()
{
    int vol = editSyringeStep->text().toInt();
    QString hight = AppHelper::IntToHexStr(vol*10/128, 2);
    QString low = AppHelper::IntToHexStr(vol*10%128, 2);
    QString msg = "FA 0f 01 3F 47 03 " + hight + " " + low + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}


//泵阀-常开
void UnitMaintain::slt_btnOpenPumbValve_Clicked()
{
    int index = cboPumbValve->currentIndex();
    QString obj;
    switch (index) {
    case 0:
        obj = "01";
        break;
    case 2:
        obj = "02";
        break;
    case 3:
        obj = "04";
        break;
    case 4:
        obj = "08";
        break;
    case 5:
        obj = "16";
        break;
    case 6:
        obj = "32";
        break;
    case 7:
        obj = "64";
        break;
    }
    QString msg = "FA 0f 01 3F 48 01 01 " + obj + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//泵阀-常闭
void UnitMaintain::slt_btnClosePumbValve_Clicked()
{
    int index = cboPumbValve->currentIndex();
    QString obj;
    switch (index) {
    case 0:
        obj = "01";
        break;
    case 2:
        obj = "02";
        break;
    case 3:
        obj = "04";
        break;
    case 4:
        obj = "08";
        break;
    case 5:
        obj = "16";
        break;
    case 6:
        obj = "32";
        break;
    case 7:
        obj = "64";
        break;
    }
    QString msg = "FA 0f 01 3F 48 01 00 " + obj + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//泵阀-打开
void UnitMaintain::slt_btnPumbValveTime_Clicked()
{
    int index = cboPumbValve->currentIndex();
    int time = editPumbValveTime->text().toInt();
    QString T1 = AppHelper::IntToHexStr(time/128, 2);
    QString T2 = AppHelper::IntToHexStr(time%128, 2);

    QString obj;
    switch (index) {
    case 0:
        obj = "01";
        break;
    case 2:
        obj = "02";
        break;
    case 3:
        obj = "04";
        break;
    case 4:
        obj = "08";
        break;
    case 5:
        obj = "16";
        break;
    case 6:
        obj = "32";
        break;
    case 7:
        obj = "64";
        break;
    }
    QString msg = "FA 0f 01 3F 48 02 00 " + obj + " " + T1 + " " + T2 + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}


//温控-反应盘温度
void UnitMaintain::slt_btnT_Ctl_Disk_Clicked()
{
    QString msg = "FA 0f 01 3F 49 01 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//试剂仓温度
void UnitMaintain::slt_btnT_Ctl_Reagent_Clicked()
{
    QString msg = "FA 0f 01 3F 49 02 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}


//(搅拌直流电机)正向旋转固定的周期数
void UnitMaintain::slt_btnOpenDC_MixMotorForward_Clicked()
{
    int time = editDC_MixMotor->text().toInt();
    QString T1 = AppHelper::IntToHexStr(time/128, 2);
    QString T2 = AppHelper::IntToHexStr(time%128, 2);

    QString msg = "FA 0f 01 3F 4a 01 "+ T1 + " " + T2 + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//(搅拌直流电机)反向向旋转固定的周期数
void UnitMaintain::slt_btnOpenDC_MixMotorReverse_Clicked()
{
    int time = editDC_MixMotor->text().toInt();
    QString T1 = AppHelper::IntToHexStr(time/128, 2);
    QString T2 = AppHelper::IntToHexStr(time%128, 2);

    QString msg = "FA 0f 01 3F 4a 02 "+ T1 + " " + T2 + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//(搅拌直流电机)停止直流电机的旋转
void UnitMaintain::slt_btnOpenDC_MixMotor_Clicked()
{
    QString msg = "FA 0f 01 3F 4a 03 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//(搅拌直流电机)直流电机常开
void UnitMaintain::slt_btnCloseDC_MixMotor_Clicked()
{
    QString msg = "FA 0f 01 3F 4a 04 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}


//（清洗液进液泵）正向旋转固定的周期数
void UnitMaintain::slt_btnOpenCleanMotorForward_Clicked()
{
    int time = editCleanMotor->text().toInt();
    QString T1 = AppHelper::IntToHexStr(time/128, 2);
    QString T2 = AppHelper::IntToHexStr(time%128, 2);

    QString msg = "FA 0f 01 3F 4b 01 "+ T1 + " " + T2 + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//（清洗液进液泵）反向向旋转固定的周期数
void UnitMaintain::slt_btnOpenCleanMotorReverse_Clicked()
{
    int time = editCleanMotor->text().toInt();
    QString T1 = AppHelper::IntToHexStr(time/128, 2);
    QString T2 = AppHelper::IntToHexStr(time%128, 2);

    QString msg = "FA 0f 01 3F 4b 02 "+ T1 + " " + T2 + " 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//（清洗液进液泵）停止直流电机的旋转
void UnitMaintain::slt_btnOpenCleanMotor_Clicked()
{
    QString msg = "FA 0f 01 3F 4b 03 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}
//（清洗液进液泵）直流电机常开
void UnitMaintain::slt_btnCloseCleanMotor_Clicked()
{
    QString msg = "FA 0f 01 3F 4b 04 00 FB";
    gSerialThread->WriteDebugFrame(msg);
}


void UnitMaintain::slt_btnSelfDefine_Clicked()
{
    QString msg = editSelfDefine->text();
    gSerialThread->WriteDebugFrame(msg);
}
