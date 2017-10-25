#include "SysSettingWidget.h"

SysSettingWidget::SysSettingWidget(QWidget *parent) : QWidget(parent)
{
    initForm();
    initStyle();               //初始化样式表
    initData();                //初始化数据
    initConnection();          //初始化信号与槽
}

void SysSettingWidget::initForm()
{
    initLiquidSettingForm();    //液体报警及是否停机设置
    initReactionDiskForm();     //反应盘设置
    initOtherSettingForm();     //其他参数设置

    //
    tabWidget = new QTabWidget;
    tabWidget->addTab(wdgLiquidSetting, "液体报警设置");
    tabWidget->addTab(wdgReactionDisk,  "反应盘设置");
    tabWidget->addTab(wdgOtherSetting,  "医院名称设置");

    btnClose = new QPushButton;

    layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(btnClose,   0, 0,  1, 52, Qt::AlignRight);
    layout->addWidget(tabWidget,  0, 0, 50, 50);
    this->setLayout(layout);
}

//液体报警及是否停机设置
void SysSettingWidget::initLiquidSettingForm()
{
    //液体报警及是否停机设置
    wdgLiquidSetting = new QWidget;
    rdo_isLiquidOnlyAlarm = new QRadioButton("仅报警（不停机）"); //仅报警（不停机）
    rdo_isLiquidAlarmStop = new QRadioButton("报警停机");        //报警停机
    labLessWaterTimes = new QLabel("缺少去离子水的次数：");        //提示“缺少去离子水”后最多能运行的次数
    labLessCleanLiquidTimes = new QLabel("缺少清洗液的次数：");    //提示“缺少清洗液”后最多能运行的次数
    labFull_L_WasteTimes = new QLabel("低浓度废液满的次数：");     //提示“低浓度废液满”后最多能运行的次数
    labFull_H_WasteTimes = new QLabel("高浓度废液满的次数：");     //提示“高浓度废液满”后最多能运行的次数
    editLessWaterTimes = new QLineEdit;           //
    editLessCleanLiquidTimes = new QLineEdit;     //
    editFull_L_WasteTimes = new QLineEdit;        //
    editFull_H_WasteTimes = new QLineEdit;        //
    btnSaveLiquidParam = new QPushButton("保 存");         //保存
    lytLiquidSetting = new QGridLayout;
    lytLiquidSetting->setMargin(5);
    lytLiquidSetting->setSpacing(5);
    lytLiquidSetting->addWidget(rdo_isLiquidOnlyAlarm,       0,  0,  2,  5, Qt::AlignRight);
    lytLiquidSetting->addWidget(rdo_isLiquidAlarmStop,       0,  5,  2, 10, Qt::AlignLeft);
    lytLiquidSetting->addWidget(labLessWaterTimes,           2,  0,  2,  5, Qt::AlignRight);
    lytLiquidSetting->addWidget(editLessWaterTimes,          2,  5,  2, 10, Qt::AlignLeft);
    lytLiquidSetting->addWidget(labLessCleanLiquidTimes,     4,  0,  2,  5, Qt::AlignRight);
    lytLiquidSetting->addWidget(editLessCleanLiquidTimes,    4,  5,  2, 10, Qt::AlignLeft);
    lytLiquidSetting->addWidget(labFull_L_WasteTimes,        6,  0,  2,  5, Qt::AlignRight);
    lytLiquidSetting->addWidget(editFull_L_WasteTimes,       6,  5,  2, 10, Qt::AlignLeft);
    lytLiquidSetting->addWidget(labFull_H_WasteTimes,        8,  0,  2,  5, Qt::AlignRight);
    lytLiquidSetting->addWidget(editFull_H_WasteTimes,       8,  5,  2, 10, Qt::AlignLeft);
    lytLiquidSetting->addWidget(btnSaveLiquidParam,         10,  0,  2, 15, Qt::AlignCenter);
    wdgLiquidSetting->setLayout(lytLiquidSetting);
}
//反应盘设置
void SysSettingWidget::initReactionDiskForm()
{
    //反应盘设置
    wdgReactionDisk = new QWidget;
    labDirtyCupAD = new QLabel("脏杯AD值:");            //脏杯AD值
    editDirtyCupAD = new QLineEdit;
    btnSaveReactionDiskParam = new QPushButton("保存");  //保存
    lytReactionDisk = new QGridLayout;
    lytReactionDisk->setMargin(5);
    lytReactionDisk->setSpacing(5);
    lytReactionDisk->addWidget(labDirtyCupAD,              0, 0, 2,  5, Qt::AlignRight);
    lytReactionDisk->addWidget(editDirtyCupAD,             0, 5, 2, 10, Qt::AlignLeft);
    lytReactionDisk->addWidget(btnSaveReactionDiskParam,   2, 5, 2, 10, Qt::AlignCenter);
    wdgReactionDisk->setLayout(lytReactionDisk);
}
//其他参数设置
void SysSettingWidget::initOtherSettingForm()
{
    //其他设置
    wdgOtherSetting = new QWidget;
    labHospitalName = new QLabel("医院名称:");              //医院信息设置(医院名称)
    editHospitalName = new QLineEdit;
    labResultTransfer = new QLabel("结果是否自动保存:");            //临时结果是否自动转到结果表
    rdoAutoTransfer_Y = new QRadioButton("是");
    rdoAutoTransfer_N = new QRadioButton("否");
    btnSaveOtherParam = new QPushButton("保存");
    lytOtherSetting = new QGridLayout;
    lytOtherSetting->setMargin(5);
    lytOtherSetting->setSpacing(5);
    lytOtherSetting->addWidget(labHospitalName,    0,  0, 2, 5, Qt::AlignRight);
    lytOtherSetting->addWidget(editHospitalName,   0,  5, 2, 15);
    lytOtherSetting->addWidget(btnSaveOtherParam,  0, 20, 2, 5, Qt::AlignCenter);
    wdgOtherSetting->setLayout(lytOtherSetting);
}

//初始化样式表
void SysSettingWidget::initStyle()
{
    this->setMinimumSize(550, 350);
    //置顶
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    //设置背景
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap( ":/resource/resource/bg/mainBg.png" )));  //
    this->setPalette(palette);
    wdgLiquidSetting->setPalette(palette);
    wdgOtherSetting->setPalette(palette);
    wdgReactionDisk->setPalette(palette);
    wdgLiquidSetting->setAutoFillBackground(true);
    wdgOtherSetting->setAutoFillBackground(true);
    wdgReactionDisk->setAutoFillBackground(true);
    this->setAutoFillBackground(true);

    //图标路径
    QIcon icon("://resource/resource/MV200.png");
    this->setWindowIcon(icon);

    //QSS
    QFile qss( ":/QSS/QSS/mvStyle.qss" );
    qss.open(QFile::ReadOnly);
    QString ss = qss.readAll();
    setStyleSheet( ss );
    qss.close();

    QString styleBtnClose =
            "QPushButton{background:transparent; width:40;height:40; border-radius:8px; background: transparent; background-image:url(:/resource/resource/MessageBox/close.png);background-repeat:no-repeat; background-position:center;}"
            "QPushButton:hover{background:transparent; width:40;height:40; border-radius:8px; background: transparent; background-image:url(:/resource/resource/MessageBox/close_hover.png);background-repeat:no-repeat; background-position:center;}"
            "QPushButton:pressed{background:transparent; width:40;height:40; border-radius:8px; background: transparent; background-image:url(:/resource/resource/MessageBox/close_pressed.png);background-repeat:no-repeat; background-position:center;}";
    btnClose->setStyleSheet(styleBtnClose);

    btnSaveLiquidParam->setObjectName("button6");
    btnSaveOtherParam->setObjectName("button6");
    btnSaveReactionDiskParam->setObjectName("button6");

    editDirtyCupAD->setAlignment(Qt::AlignCenter);
    editFull_H_WasteTimes->setAlignment(Qt::AlignCenter);
    editFull_L_WasteTimes->setAlignment(Qt::AlignCenter);
    editHospitalName->setAlignment(Qt::AlignCenter);
    editLessCleanLiquidTimes->setAlignment(Qt::AlignCenter);
    editLessWaterTimes->setAlignment(Qt::AlignCenter);
}

//初始化数据
void SysSettingWidget::initData()
{
    if(gParameter->get_isLiquidAlarmStop())
        rdo_isLiquidAlarmStop->setChecked(true);
    else
        rdo_isLiquidOnlyAlarm->setChecked(true);

    editLessWaterTimes->setText(QString::number(gParameter->get_LessWaterTimes()));
    editLessCleanLiquidTimes->setText(QString::number(gParameter->get_LessCleanLiquidTimes()));
    editFull_H_WasteTimes->setText(QString::number(gParameter->get_Full_H_WasteTimes()));
    editFull_L_WasteTimes->setText(QString::number(gParameter->get_Full_L_WasteTimes()));
    editDirtyCupAD->setText(QString::number(gParameter->getCupDirtyAD()));
    editHospitalName->setText(gParameter->getHospitalName());

    if(gParameter->get_isAutoSaveResult())
        rdoAutoTransfer_Y->setChecked(true);
    else
        rdoAutoTransfer_N->setChecked(true);

    updateStrList.clear();
}

//初始化信号与槽
void SysSettingWidget::initConnection()
{
    connect(btnClose, SIGNAL(clicked(bool)), this, SLOT(slt_btnClose_Clicked()));
    connect(btnSaveOtherParam, SIGNAL(clicked(bool)), this, SLOT(slt_btnSaveOtherParam_Clicked()));
    connect(btnSaveLiquidParam, SIGNAL(clicked(bool)), this, SLOT(slt_btnSaveLiquidParam_Clicked()));
    connect(btnSaveReactionDiskParam, SIGNAL(clicked(bool)), this, SLOT(slt_btnSaveReactionDiskParam_Clicked()));
}

//槽：保存液体参数
void SysSettingWidget::slt_btnSaveLiquidParam_Clicked()
{
    QString isCheck = "0";
    if(rdo_isLiquidAlarmStop->isChecked()) isCheck = "1";

    QString str =
            "update SysParamT set "
            " isLiquidAlarm=" + isCheck
            +", lessWaterTimes=" + editLessWaterTimes->text()
            +", lessCleanLiquidTimes=" + editLessCleanLiquidTimes->text()
            +", full_L_WasteTimes=" + editFull_L_WasteTimes->text()
            +", full_H_WasteTimes=" + editFull_H_WasteTimes->text()
            +";";
    //qDebug() << str;
    updateStrList.append(str);
    gParameter->set_isLiquidAlarmStop(rdo_isLiquidAlarmStop->isChecked());
}

//槽：保存反应盘参数
void SysSettingWidget::slt_btnSaveReactionDiskParam_Clicked()
{
    QString str = "update SysParamT set CupDirtyAD=" + editDirtyCupAD->text()+";";
    //qDebug() << str;
    updateStrList.append(str);
    gParameter->setCupDirtyAD(editDirtyCupAD->text().trimmed().toInt());
}

//槽：保存其他参数
void SysSettingWidget::slt_btnSaveOtherParam_Clicked()
{
    QString isCheck = "0";
    if(rdoAutoTransfer_Y->isChecked()) isCheck = "1";
    QString str =
            "update SysParamT set "
            " isAutoTransfer=" + isCheck
            +", hospitalName='" + editHospitalName->text().trimmed()+"';";
    //qDebug() << str;
    gParameter->setHospitalName(editHospitalName->text().trimmed());
    updateStrList.append(str);
}

//槽：退出
void SysSettingWidget::slt_btnClose_Clicked()
{
    SQLite::getInstance()->Transaction(updateStrList);
    updateStrList.clear();
    this->close();
}

//按下鼠标事件
void SysSettingWidget::mousePressEvent( QMouseEvent * event )
{
    //窗口移动距离
    move_point = event->globalPos() - pos();
}
//移动鼠标事件
void SysSettingWidget::mouseMoveEvent(QMouseEvent *event)
{
    //移动窗口
    QPoint move_pos = event->globalPos();
    move(move_pos - move_point);
}


