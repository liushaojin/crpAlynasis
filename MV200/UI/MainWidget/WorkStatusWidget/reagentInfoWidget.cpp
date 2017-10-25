#include "reagentInfoWidget.h"
#include <qdebug.h>

ReagentInfoWidget::ReagentInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();          //初始化
    initTableFormat();   //
    initConnect();       //连接信号与槽
    initData();          //初始化数据
    initStyle();         //界面风格
    setUsedReagentPosState();
}

//初始化
void ReagentInfoWidget::initForm()
{
    initReagentForm();
    initReagentInfoForm();

    layout = new QGridLayout;
    layout->setMargin(3);
    layout->setSpacing(5);
    layout->addWidget(wdg,      0,  0, 100,  40, Qt::AlignCenter);
    layout->addWidget(wdg2,     0, 40, 100,  70);
    this->setLayout(layout);
}

void ReagentInfoWidget::initReagentForm()
{
    //试剂模拟图
    wdgReagent = new QWidget;
    ltyReagent = new QGridLayout;
    ltyReagent->setMargin(5);
    ltyReagent->setSpacing(20);
    for( int i = 0; i < REAGENT_COUNT; i ++ )
    {
        btnReagent[i] = new QPushButton(QString::number(i+1));
        ltyReagent->addWidget(btnReagent[i],  30-i/4*4, i%4*4, 4, 4, Qt::AlignCenter);
    }
    wdgReagent->setLayout(ltyReagent);

    //试剂模拟图状态说明
    QString str = "未使用:+已使用:+不足:+已过期:+稀释液:+R1:+R2:";
    wdgStatusInfo = new QWidget;
    lytStatusInfo = new QGridLayout;
    lytStatusInfo->setMargin(0);
    lytStatusInfo->setSpacing(5);
    for( int i = 0; i < 10; i ++ )
    {
        if(i < str.split("+").count() )
        {
            labStatusName[i] = new QLabel( str.split("+").at(i) ); //状态名称
            labStatusPic[i] = new QLabel;                          //状态图标
            labStatusPic[i]->setFixedSize(30, 30);
            lytStatusInfo->addWidget(labStatusName[i], 5*i,  0, 5, 10, Qt::AlignRight);
            lytStatusInfo->addWidget(labStatusPic[i],  5*i, 10, 5, 10, Qt::AlignLeft);
        }
    }
    wdgStatusInfo->setLayout(lytStatusInfo);

    //
    wdg = new QWidget;
    lyt = new QGridLayout;
    lyt->setMargin(0);
    lyt->setSpacing(5);
    lyt->addWidget(wdgReagent,     0,  0, 100, 50, Qt::AlignCenter);
    lyt->addWidget(wdgStatusInfo,  0, 50, 100, 50, Qt::AlignCenter);
    wdg->setLayout(lyt);
}

void ReagentInfoWidget::initReagentInfoForm()
{
    //项目试剂位信息
    wdgProjectReagentPos = new QWidget;
    lytProjectReagentPos = new QGridLayout;
    labProjectName = new QLabel("");
    labReagent[0] = new QLabel("稀释液:");
    labReagent[1] = new QLabel("R1:");
    labReagent[2] = new QLabel("R2:");
    cboReagent0 = new QComboBox;   //试剂-稀释液
    cboReagent1 = new QComboBox;   //试剂-R1
    cboReagent2 = new QComboBox;   //试剂-R2
    btnSaveReagentPos = new QPushButton("保 存"); //保存
    btnRelease = new QPushButton("释放");         //释放当前选中的项目试剂位置
    btnReleaseAll = new QPushButton("全盘释放");  //全盘释放
    lytProjectReagentPos->setMargin(0);
    lytProjectReagentPos->setSpacing(5);
    lytProjectReagentPos->addWidget(labReagent[0],     0,  5, 5, 5, Qt::AlignRight);
    lytProjectReagentPos->addWidget(cboReagent0,       0, 10, 5, 5, Qt::AlignLeft);
    lytProjectReagentPos->addWidget(labReagent[1],     0, 15, 5, 5, Qt::AlignRight);
    lytProjectReagentPos->addWidget(cboReagent1,       0, 20, 5, 5, Qt::AlignLeft);
    lytProjectReagentPos->addWidget(labReagent[2],     0, 25, 5, 5, Qt::AlignRight);
    lytProjectReagentPos->addWidget(cboReagent2,       0, 30, 5, 5, Qt::AlignLeft);
    lytProjectReagentPos->addWidget(btnSaveReagentPos, 0, 35, 5, 5);
    lytProjectReagentPos->addWidget(btnRelease,        0, 45, 5, 5);
    lytProjectReagentPos->addWidget(btnReleaseAll,     0, 65, 5, 5, Qt::AlignRight);
    wdgProjectReagentPos->setLayout(lytProjectReagentPos);

    //项目列表
    grbProject = new QGroupBox;
    lytProject = new QGridLayout;
    lytProject->setMargin(5);
    lytProject->setSpacing(3);
    QFlowLayout *flowLayout = new QFlowLayout;
    int i;
    for(i = 0; i < PROJECT_COUNT; i ++ )
    {
        rdoProject[i] = new QRadioButton;
        rdoProject[i]->setVisible(false);        //默认隐藏
        flowLayout->addWidget(rdoProject[i]);
    }
    lytProject->addLayout(flowLayout,   0, 0, 20, 90);
    lytProject->addWidget(wdgProjectReagentPos, 20, 0, 3, 90, Qt::AlignBottom);
    grbProject->setLayout(lytProject);

    //项目试剂信息列表
    tbvReagentList = new QTableView;
    modelList = new QStandardItemModel;
    tbvReagentList->setModel(modelList);

    wdg2 = new QWidget;
    lyt2 = new QGridLayout;
    lyt2->setMargin(0);
    lyt2->setSpacing(0);
    lyt2->addWidget(grbProject,          0,  0,  90, 50);
    lyt2->addWidget(tbvReagentList,     90,  0,  10, 50);
    wdg2->setLayout(lyt2);
}

void ReagentInfoWidget::initTableFormat()
{
    //定标测试列表
    QStringList headerList_h;
    headerList_h  << "项目名称" << "试剂类型" << "批号" << "位置" << "余量(次 )" << "总量(ml)" << "有效期(年)" << "开瓶保质期(天)" << "";
    modelList->setHorizontalHeaderLabels( headerList_h );
    tbvReagentList->setColumnWidth( 0, 100);    //项目名称

    tbvReagentList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvReagentList->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvReagentList->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvReagentList->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
    tbvReagentList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    //tbvReagentList->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
    tbvReagentList->horizontalHeader()->setStyleSheet("font-size:18px;");         //表头字体样式

    //初始化每一个单元格
    for( int i = 0; i < REAGETN_ROWCOUNT; i ++ )
    {
        modelList->setItem( i, 0, new QStandardItem( "" ));
        modelList->setItem( i, 1, new QStandardItem( "" ));
        modelList->setItem( i, 2, new QStandardItem( "" ));
        modelList->setItem( i, 3, new QStandardItem( "" ));
        modelList->setItem( i, 4, new QStandardItem( "" ));
        modelList->setItem( i, 5, new QStandardItem( "" ));
        modelList->setItem( i, 6, new QStandardItem( "" ));
        modelList->setItem( i, 7, new QStandardItem( "" ));

        //列表内容显示方式 //居中
        modelList->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 4)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 5)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 6)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 7)->setTextAlignment(Qt::AlignCenter);
    }
}

//清空试剂信息表
void ReagentInfoWidget::ClearTable()
{
    CurrentRow = 0;
    //清空数据
    for(int i = 0; i < REAGETN_ROWCOUNT; i ++ )
    {
        modelList->item(i, 0)->setText("");
        modelList->item(i, 1)->setText("");
        modelList->item(i, 2)->setText("");
        modelList->item(i, 3)->setText("");
        modelList->item(i, 4)->setText("");
        modelList->item(i, 5)->setText("");
        modelList->item(i, 6)->setText("");
        modelList->item(i, 7)->setText("");
    }
}

//界面风格
void ReagentInfoWidget::initStyle()
{
    btnSaveReagentPos->setObjectName("button6");
    btnRelease->setObjectName("button6");
    btnReleaseAll->setObjectName("button6");

    initStyle2();

    for(int i = 0; i < REAGENT_COUNT; i ++)
        btnReagent[i]->setObjectName("Normal");

    labStatusPic[0]->setObjectName("Normal");
    labStatusPic[1]->setObjectName("Insufficient");
    labStatusPic[2]->setObjectName("Exhausted");
    labStatusPic[3]->setObjectName("Overdue");
    labStatusPic[4]->setObjectName("Diluent");
    labStatusPic[5]->setObjectName("R1");
    labStatusPic[6]->setObjectName("R2");

    //
    for( int i = 0; i < PROJECT_COUNT; i ++ )
        rdoProject[i]->setObjectName("rdo2");     //常规项目

    tbvReagentList->setObjectName("tableV");
}
//界面风格
void ReagentInfoWidget::initStyle2()
{
    QString strLab =
            "QLabel#Normal       { image: url(:/resource/resource/ReagentStatus/Normal.png); }"
            "QLabel#Insufficient { image: url(:/resource/resource/ReagentStatus/Insufficient.png); }"
            "QLabel#Exhausted    { image: url(:/resource/resource/ReagentStatus/Exhausted.png); }"
            "QLabel#Overdue      { image: url(:/resource/resource/ReagentStatus/Overdue.png); }"
            "QLabel#Diluent      { image: url(:/resource/resource/ReagentStatus/Diluent.png); }"
            "QLabel#R1           { image: url(:/resource/resource/ReagentStatus/R1.png); }"
            "QLabel#R2           { image: url(:/resource/resource/ReagentStatus/R2.png); }"
            ;
    QString strBtn =
            //正常
            "QPushButton#Normal{ width:72; height:72;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/ReagentStatus/Normal.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Normal:hover{ width:72; height:72; border-radius:8px; font-size:28px; color:darkCyan; background-image:url(:/resource/resource/ReagentStatus/Normal.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Normal:pressed{ width:72; height:72; border-radius:8px; font-size:30px; color:darkMagenta; background-image:url(:/resource/resource/ReagentStatus/Normal.png); background-repeat:no-repeat; background-position:center; }"
            //不足
            "QPushButton#Insufficient{width:72; height:72;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/ReagentStatus/Insufficient.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Insufficient:hover{width:72; height:72; border-radius:8px; font-size:28px; color:darkCyan; background-image:url(:/resource/resource/ReagentStatus/Insufficient.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Insufficient:pressed{width:72; height:72; border-radius:8px; font-size:30px; color:darkMagenta; background-image:url(:/resource/resource/ReagentStatus/Insufficient.png); background-repeat:no-repeat; background-position:center; }"
            //耗尽
            "QPushButton#Exhausted{width:72; height:72;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/ReagentStatus/Exhausted.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Exhausted:hover{width:72; height:72; border-radius:8px; font-size:28px; color:darkCyan; background-image:url(:/resource/resource/ReagentStatus/Exhausted.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Exhausted:pressed{width:72; height:72; border-radius:8px; font-size:30px; color:darkMagenta; background-image:url(:/resource/resource/ReagentStatus/Exhausted.png); background-repeat:no-repeat; background-position:center; }"
            //已过期
            "QPushButton#Overdue{width:72; height:72;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/ReagentStatus/Overdue.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Overdue:hover{width:72; height:72; border-radius:8px; font-size:28px; color:darkCyan; background-image:url(:/resource/resource/ReagentStatus/Overdue.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Overdue:pressed{width:72; height:72; border-radius:8px; font-size:30px; color:darkMagenta; background-image:url(:/resource/resource/ReagentStatus/Overdue.png); background-repeat:no-repeat; background-position:center; }"
            //稀释液
            "QPushButton#Diluent{width:72; height:72;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/ReagentStatus/Diluent.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Diluent:hover{width:72; height:35; border-radius:8px; font-size:28px; color:darkCyan; background-image:url(:/resource/resource/ReagentStatus/Diluent.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Diluent:pressed{width:72; height:72; border-radius:8px; font-size:30px; color:darkMagenta; background-image:url(:/resource/resource/ReagentStatus/Diluent.png); background-repeat:no-repeat; background-position:center; }"
            //R1
            "QPushButton#R1{width:72; height:72;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/ReagentStatus/R1.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#R1:hover{width:72; height:72; border-radius:8px; font-size:28px; color:darkCyan; background-image:url(:/resource/resource/ReagentStatus/R1.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#R1:pressed{width:72; height:72; border-radius:8px; font-size:30px; color:darkMagenta; background-image:url(:/resource/resource/ReagentStatus/R1.png); background-repeat:no-repeat; background-position:center; }"
            //R2
            "QPushButton#R2{width:72; height:72;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/ReagentStatus/R2.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#R2:hover{width:72; height:72; border-radius:8px; font-size:28px; color:darkCyan; background-image:url(:/resource/resource/ReagentStatus/R2.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#R2:pressed{width:72; height:72; border-radius:8px; font-size:30px; color:darkMagenta; background-image:url(:/resource/resource/ReagentStatus/R2.png); background-repeat:no-repeat; background-position:center; }"
            ;
    this->setStyleSheet(strLab + strBtn);
}

//连接信号与槽
void ReagentInfoWidget::initConnect()
{
    for(int i = 0; i < PROJECT_COUNT; i ++ )
        connect(rdoProject[i], SIGNAL(clicked(bool)), this, SLOT(slt_btnReagent_Clicked()));

    connect(gParameter, SIGNAL(sig_newProject()), this, SLOT(slt_UpdateRoutineItem()));
    connect(btnSaveReagentPos, SIGNAL(clicked()), this, SLOT(slt_btnSaveReagentPos_Clicked()));

    connect(btnRelease, SIGNAL(clicked()), this, SLOT(slt_btnRelease_Clicked()));
    connect(btnReleaseAll, SIGNAL(clicked()), this, SLOT(slt_btnReleaseAll_Clicked()));

    connect(this, SIGNAL(sig_updatePReagentPos()), gSampleApplication, SLOT(slt_UpdateRoutineItem()));
    connect(this, SIGNAL(sig_updatePReagentPos()), ProjectGroupWidget::getInstance(), SLOT(slt_UpdateRoutineItem()));
    connect(this, SIGNAL(sig_updatePReagentPos()), CalculateProjectWidget::getInstance(), SLOT(slt_UpdateRoutineItem()));
}

//初始化数据
void ReagentInfoWidget::initData()
{
    R0_v = 0; R1_v = 0; R2_v = 0; //项目试剂加样时吸取的量（ul）

    QStringList list;
    list << "--"
         << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12"
         << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23" << "24";
    cboReagent0->addItems(list);
    cboReagent1->addItems(list);
    cboReagent2->addItems(list);

    slt_UpdateRoutineItem();          //槽：更新常规项目
}

//槽：查看项目
void ReagentInfoWidget::slt_btnReagent_Clicked()
{
    ClearTable();  //清空试剂信息表
    labProjectName->setText("");
    cboReagent0->setCurrentIndex(0);
    cboReagent1->setCurrentIndex(0);
    cboReagent2->setCurrentIndex(0);
    QString batchNum;

    QString name= qobject_cast<QRadioButton*>(sender())->text().trimmed();
    labProjectName->setText(name);

    //设置项目的试剂个数
    bool isDiluent = gParameter->getProjectParam(gParameter->getProject_id(name.trimmed())).isDiluentPorj; //是否为稀释项目
    if(isDiluent) setProjectReagentCount(3);
    else setProjectReagentCount(2);

    QString str =
            " select r0ID, r1ID, r2ID, batchNum, r0_V, r1_V, r2_V from ProjectT "
            " where name='" + name +"' or abbreviation='" + name +"';";
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    vDiluent=0, vR1=0, vR2=0;
    while(query.next())
    {
        vDiluent = query.value(0).toInt();
        vR1 = query.value(1).toInt();
        vR2 = query.value(2).toInt();

        batchNum = query.value(3).toString();
        R0_v = query.value(4).toInt();
        R1_v = query.value(5).toInt();
        R2_v = query.value(6).toInt();
    }

    initStyle();
    setUsedReagentPosState();         //设置试剂仓上已经被项目占用的试剂位置（设置成“已使用”状态）

    if(vR2)
    {
        cboReagent0->setCurrentIndex(vDiluent);
        cboReagent1->setCurrentIndex(vR1);
        cboReagent2->setCurrentIndex(vR2);

        setReagentStatus(vDiluent, "Diluent");//设置试剂的状态
        setReagentStatus(vR1, "R1");//设置试剂的状态
        setReagentStatus(vR2, "R2");//设置试剂的状态

        //查询项目的各种试剂的信息
        QueryReagentInfo(name,batchNum, vDiluent, R0_v);
        QueryReagentInfo(name,batchNum, vR1, R1_v);
        QueryReagentInfo(name,batchNum, vR2, R2_v);
    }
}

//查询项目的各种试剂的信息
void ReagentInfoWidget::QueryReagentInfo(QString project, QString batchNo, int RNo, int vol)
{
    QString str, reagentType;
    double volume, usedV;
    QSqlQuery  query;
    int times;

    if(RNo != 0)
    {
        str = "select name, volume, usedV from ReagentT where id = " + QString::number(RNo) +";";
        //qDebug() << str;
        query = SQLite::getInstance()->Select(str);  //查询数据库
        while(query.next())
        {
            reagentType = query.value(0).toString();
            //volume = query.value(1).toDouble();
            volume = 50000;
            usedV = query.value(2).toDouble();
        }

        times = (int)(usedV/vol);
        if(times < 0) times = 0;

        modelList->item(CurrentRow, 0)->setText(project);
        modelList->item(CurrentRow, 1)->setText(reagentType);
        modelList->item(CurrentRow, 2)->setText(batchNo);
        modelList->item(CurrentRow, 3)->setText(QString::number(RNo));
        modelList->item(CurrentRow, 4)->setText(QString::number(times));
        modelList->item(CurrentRow, 5)->setText(QString::number(volume/1000));
        modelList->item(CurrentRow, 6)->setText("1");
        modelList->item(CurrentRow, 7)->setText("30");
        CurrentRow ++;
    }
}

//保存前检查试剂位置对应的状态
bool ReagentInfoWidget::CheckReagentState(int pID, int R0, int R1, int R2)
{
    //相同项目允许更改 & 该试剂位尚未使用
    //检查R0
    if( R0!=0 && !( gParameter->getMapReagentSlotValue(R0) == 0 || gParameter->getMapReagentSlotValue(R0) == pID || !gParameter->getReagentNoState(R0) ))
    {
        gMessageBox->setInfo(QString("提示"), QString("R0试剂位 %1 已经被使用，\n请重新选择或请先释放该试剂位！").arg(R0),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返 回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return false;
    }

    //检查R1
    if( R1!=0 && !(gParameter->getMapReagentSlotValue(R1) == 0 || gParameter->getMapReagentSlotValue(R1) == pID || !gParameter->getReagentNoState(R1) ))
    {
        gMessageBox->setInfo(QString("提示"), QString("R1试剂位 %1 已经被使用，\n请重新选择或请先释放该试剂位！").arg(R1),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返 回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return false;
    }

    //检查R2
    if( R2!=0 && !(gParameter->getMapReagentSlotValue(R2) == 0 || gParameter->getMapReagentSlotValue(R2) == pID || !gParameter->getReagentNoState(R2) ))
    {
        gMessageBox->setInfo(QString("提示"), QString("R2试剂位 %1 已经被使用，\n请重新选择或请先释放该试剂位！").arg(R2),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返 回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return false;
    }

    return true;
}

//设置试剂仓上已经被项目占用的试剂位置（设置成“已使用”状态）
void ReagentInfoWidget::setUsedReagentPosState()
{
    for(int i = 0; i < REAGENT_COUNT; i ++)
    {
        //查看rNo试剂位的状态（是否已经占用, 0:未占用，1:已占用）
        if(gParameter->getReagentNoState(i+1))
            setReagentStatus(i+1, "Insufficient"); //已使用
    }
}

//设置项目的试剂位个数（即可以选择的试剂个数，目前只分双试剂和三试剂两种，即稀释和非稀释的区别）
void ReagentInfoWidget::setProjectReagentCount(int count)
{
    if(count == 1)    //单试剂（只显示R2）
    {
        labReagent[0]->setEnabled(false);
        labReagent[1]->setEnabled(false);
        labReagent[2]->setEnabled(true);
        cboReagent0->setCurrentIndex(0);
        cboReagent1->setCurrentIndex(0);
        cboReagent0->setEnabled(false);         //试剂-稀释液
        cboReagent1->setEnabled(false);         //试剂-R1
        cboReagent2->setEnabled(true);         //试剂-R2
    }
    else if(count == 2) //双试剂项目（即非稀释项目，显示R1、R2）
    {
        labReagent[0]->setEnabled(false);
        labReagent[1]->setEnabled(true);
        labReagent[2]->setEnabled(true);
        cboReagent0->setCurrentIndex(0);
        cboReagent0->setEnabled(false);         //试剂-稀释液
        cboReagent1->setEnabled(true);         //试剂-R1
        cboReagent2->setEnabled(true);         //试剂-R2
    }
    else //三试剂项目（即稀释项目）
    {
        labReagent[0]->setEnabled(true);
        labReagent[1]->setEnabled(true);
        labReagent[2]->setEnabled(true);
        cboReagent0->setEnabled(true);         //试剂-稀释液
        cboReagent1->setEnabled(true);         //试剂-R1
        cboReagent2->setEnabled(true);         //试剂-R2
    }
}

//保存项目位置到数据库中
void ReagentInfoWidget::SaveReagentPos()
{
    QString name = labProjectName->text(); //项目名称
    int Pid = gParameter->getProject_id(name); //项目id
    int R0 = cboReagent0->currentIndex();  //稀释液位置
    int R1 = cboReagent1->currentIndex();  //R1位置
    int R2 = cboReagent2->currentIndex();  //R2位置

    //保存前检查试剂位置对应的状态
    if( !CheckReagentState(Pid, R0, R1, R2) )
        return;

    QStringList list;
    list.clear();
    QString str =
            " update ProjectT set "
            " r0ID ="+ QString::number(R0) +
            ", r1ID ="+ QString::number(R1) +
            ", r2ID ="+ QString::number(R2) +
            " where id=" + QString::number(Pid) +";";
    list.append(str);

    gParameter->ReleaseReagentNo(vDiluent);      //释放原来的试剂位
    gParameter->setMapReagentSlot(R0, Pid);      //重新定义新的试剂位
    list.append( SaveReagentType("R0", cboReagent0->currentIndex()) );

    gParameter->ReleaseReagentNo(vR1);
    gParameter->setMapReagentSlot(R1, Pid);
    list.append( SaveReagentType("R1", cboReagent1->currentIndex()) );

    gParameter->ReleaseReagentNo(vR2);
    gParameter->setMapReagentSlot(R2, Pid);
    list.append( SaveReagentType("R2", cboReagent2->currentIndex()) );
    SQLite::getInstance()->Transaction(list);
    emit sig_updatePReagentPos();   //信号：更新项目的试剂位信息
}

//槽：保存项目试剂位置信息
void ReagentInfoWidget::slt_btnSaveReagentPos_Clicked()
{
    //试剂位未选提示
    if(cboReagent0->isEnabled())
    {
        if(cboReagent0->currentIndex() == 0) //未选择
        {
            gMessageBox->setInfo(QString("提示"), QString("不能保存，请选择稀释液!"),
                                 QPixmap("://resource/resource/MessageBox/information.png"), true, true);
            gMessageBox->setButtonText(Cancel, "确定");
            gMessageBox->show();
            return;
        }
    }
    if(cboReagent1->isEnabled())
    {
        if(cboReagent1->currentIndex() == 0) //未选择
        {
            gMessageBox->setInfo(QString("提示"), QString("不能保存，请选择R1试剂!"),
                                 QPixmap("://resource/resource/MessageBox/information.png"), true, true);
            gMessageBox->setButtonText(Cancel, "确定");
            gMessageBox->show();
            return;
        }
    }
    if(cboReagent2->isEnabled())
    {
        if(cboReagent2->currentIndex() == 0) //未选择
        {
            gMessageBox->setInfo(QString("提示"), QString("不能保存，请选择R2试剂!"),
                                 QPixmap("://resource/resource/MessageBox/information.png"), true, true);
            gMessageBox->setButtonText(Cancel, "确定");
            gMessageBox->show();
            return;
        }
    }

    SaveReagentPos();
}

//保存试剂盘上对应位置的试剂类型（R0、R1、R2）
QString ReagentInfoWidget::SaveReagentType(QString type, int pos)
{
    if(pos == 0) type = "-";
    QString str = "update ReagentT set name='" + type + "'  where id = " + QString::number(pos) +";";
    return str;
}

//设置slot号样本架上no号样本的状态
void ReagentInfoWidget::setReagentStatus(int rNo, QString objName)
{
    if( rNo > REAGENT_COUNT || rNo <= 0) return;

    rNo = rNo -1;
    btnReagent[rNo]->setObjectName(objName);
    initStyle2();
}

//槽：更新常规项目
void ReagentInfoWidget::slt_UpdateRoutineItem()
{
    //项目列表
    QVector<QString> routineP =  gParameter->get_ProjectName();
    for(int i = 0; i < PROJECT_COUNT; i ++)
    {
        if(i < routineP.count())
        {
            rdoProject[i]->setVisible(true);
            rdoProject[i]->setText(routineP.at(i));
        }
        else
        {
            rdoProject[i]->setText("");
            rdoProject[i]->setVisible(false);        //默认隐藏
        }
    }
}

//槽：释放当前选中的项目试剂位置
void ReagentInfoWidget::slt_btnRelease_Clicked()
{
    if( gScheduler->get_isRunning() ) //正在测试中，不允许进入
    {
        gMessageBox->setInfo(QString("提示"), QString("有测试正在进行中，不能释放当前选中的项目试剂位置！"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return;
    }

    QString name = labProjectName->text(); //项目名称
    int Pid = gParameter->getProject_id(name); //项目id
    if(Pid == 0) return;

    gMessageBox->setInfo(QString("提示"), QString("是否要释放当前选择项目\n\n“"+ name +"”的试剂位!"),
                         QPixmap("://resource/resource/MessageBox/information.png"), true, false);
    gMessageBox->setButtonText(Cancel, "否(N)");
    gMessageBox->setButtonText(Sure, "是(Y)");
    if(gMessageBox->exec() == MVMessageBox::Accepted)
    {
        //释放
        cboReagent0->setCurrentIndex(0);
        cboReagent1->setCurrentIndex(0);
        cboReagent2->setCurrentIndex(0);
        //保存
        SaveReagentPos();

        emit sig_updatePReagentPos();   //信号：更新项目的试剂位信息
    }
}

//槽：全盘释放
void ReagentInfoWidget::slt_btnReleaseAll_Clicked()
{
    if( gScheduler->get_isRunning() ) //正在测试中，不允许进入
    {
        gMessageBox->setInfo(QString("提示"), QString("有测试正在进行中，全盘释放！"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return;
    }

    gMessageBox->setInfo(QString("提示"), QString("是否要释放所有项目的试剂位!"),
                         QPixmap("://resource/resource/MessageBox/information.png"), true, false);
    gMessageBox->setButtonText(Cancel, "否(N)");
    gMessageBox->setButtonText(Sure, "是(Y)");
    if(gMessageBox->exec() == MVMessageBox::Accepted)
    {
        QMap<int, QString> pMap = gParameter->getProjectMap();
        QMap<int, QString>::iterator it = pMap.begin();//迭代器
        QStringList list;
        list.clear();
        QString str;
        while(it != pMap.end())
        {
            str =
                    " update ProjectT set "
                    " r0ID ="+ QString::number(0) +
                    ", r1ID ="+ QString::number(0) +
                    ", r2ID ="+ QString::number(0) +
                    " where id=" + QString::number(it.key()) +";";
            list.append(str);
            it ++;
        }
        gParameter->ReleaseAllReagent(); //释放全部试剂位
        SQLite::getInstance()->Transaction(list);

        emit sig_updatePReagentPos();   //信号：更新项目的试剂位信息
    }
}

