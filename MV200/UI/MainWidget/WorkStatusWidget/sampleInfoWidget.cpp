#include "sampleInfoWidget.h"
#include <qdebug.h>

SampleInfoWidget::SampleInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    CurrentSamplePos = "";

    initForm();          //初始化
    initTableFormat();
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}

//初始化
void SampleInfoWidget::initForm()
{
    initSampleForm();
    initSampleInfoForm();    //
    //initButtonForm();
    layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(3);
    layout->addWidget(wdgSample,       0,  0, 100, 30);
    layout->addWidget(wdgSampleInfo,   0, 30, 100, 70);
    //layout->addWidget(wdgBtn,         90, 30,  10, 70);
    this->setLayout(layout);
}

void SampleInfoWidget::initSampleForm()
{
    //第一排样本架
    wdgSample1 = new QWidget;
    labSample1 = new QLabel("样本架1 "); //样本架一
    labSample2 = new QLabel("样本架2 "); //样本架2
    labSample3 = new QLabel("样本架3 "); //样本架3
    labSample4 = new QLabel("急诊样本 "); //急诊样本架
    lytSample1 = new QGridLayout;
    lytSample1->setMargin(5);
    lytSample1->setSpacing(5);
    lytSample1->addWidget(labSample1,  0,  0, 1,  5, Qt::AlignCenter);
    lytSample1->addWidget(labSample2,  0, 10, 1,  5, Qt::AlignCenter);
    lytSample1->addWidget(labSample3,  0, 20, 1,  5, Qt::AlignCenter);
    //lytSample1->addWidget(labSample4,  0, 30, 1, 10, Qt::AlignCenter);
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        btnSample1[i] = new QPushButton(QString::number(i+1));
        btnSample2[i] = new QPushButton(QString::number(i+1));
        btnSample3[i] = new QPushButton(QString::number(i+1));

        lytSample1->addWidget(btnSample1[i],  i*3+1,  0, 1, 5, Qt::AlignCenter);
        lytSample1->addWidget(btnSample2[i],  i*3+1, 10, 1, 5, Qt::AlignCenter);
        lytSample1->addWidget(btnSample3[i],  i*3+1, 20, 1, 5, Qt::AlignCenter);
    }
    btnSample4[0] = new QPushButton("1");
    btnSample4[1] = new QPushButton("2");
    //lytSample1->addWidget(btnSample4[0],  0*3+1, 30, 1, 5, Qt::AlignCenter);
    //lytSample1->addWidget(btnSample4[1],  0*3+1, 35, 1, 5, Qt::AlignCenter);
    wdgSample1->setLayout(lytSample1);


    //状态说明
    //空闲、待测试、测试中、已完成、未完成、缺少样本
    QString str = "空闲:+已申请:+测试中:+已完成:+未完成:";
    wdgStatusInfo = new QWidget;
    lytStatusInfo = new QGridLayout;
    lytStatusInfo->setMargin(2);
    lytStatusInfo->setSpacing(5);
    for(int i = 0; i < STATUSCOUNT; i ++ )
    {
        if( i < str.split("+").count())
        {
            labStatusName[i] = new QLabel(str.split("+").at(i)); //状态名称
            labStatusPic[i] = new QLabel;                        //状态图标
            labStatusPic[i]->setFixedSize(30, 30);
            lytStatusInfo->addWidget(labStatusName[i], 5*i,  0, 5, 10, Qt::AlignRight);
            lytStatusInfo->addWidget(labStatusPic[i],  5*i, 10, 5, 10, Qt::AlignLeft);
        }
    }
    wdgStatusInfo->setLayout(lytStatusInfo);


    labTimes = new QLabel("剩余时间: 0 min");   //大概剩余时间
    wdgSample = new QWidget;
    lytSample = new QGridLayout;
    lytSample->setMargin(0);
    lytSample->setSpacing(5);
    lytSample->addWidget(wdgSample1,      0,  0, 50, 30, Qt::AlignCenter);
    lytSample->addWidget(wdgStatusInfo,  20, 30, 30, 10, Qt::AlignCenter | Qt::AlignTop);
    lytSample->addWidget(labTimes,       50,  0,  2, 20, Qt::AlignCenter);
    wdgSample->setLayout(lytSample);
}

void SampleInfoWidget::initSampleInfoForm()
{
    btnDelete = new QPushButton("删除");     //删除

    grbSampleInfo = new QGroupBox("样本信息");
    labSampleSlot = new QLabel("样本架:");       //样本加
    editSampleSlot = new QLineEdit;
    labSamplePos = new QLabel("样本位:");       //样本位
    editSamplePos = new QLineEdit;
    labSampleType = new QLabel("样本类型:");      //样本类型
    editSampleType = new QLineEdit;
    lytSampleInfo = new QGridLayout;
    lytSampleInfo->setMargin(2);
    lytSampleInfo->setSpacing(0);
    lytSampleInfo->addWidget(labSampleSlot,   0,  0, 3, 10, Qt::AlignRight);
    lytSampleInfo->addWidget(editSampleSlot,  0, 10, 3, 10, Qt::AlignLeft);
    lytSampleInfo->addWidget(labSamplePos,    0, 20, 3, 10, Qt::AlignRight);
    lytSampleInfo->addWidget(editSamplePos,   0, 30, 3, 10, Qt::AlignLeft);
    lytSampleInfo->addWidget(new QFrame,      0, 40, 3, 5);
    lytSampleInfo->addWidget(btnDelete,       0, 45, 3, 10, Qt::AlignCenter);
    grbSampleInfo->setLayout(lytSampleInfo);

    tbvList = new QTableView;
    modelList = new QStandardItemModel;
    tbvList->setModel(modelList);

    wdgSampleInfo = new QWidget;
    lytvSampleInfo = new QVBoxLayout;
    lytvSampleInfo->setMargin(0);
    lytvSampleInfo->setSpacing(5);
    lytvSampleInfo->addWidget(grbSampleInfo);
    lytvSampleInfo->addWidget(tbvList);
    wdgSampleInfo->setLayout(lytvSampleInfo);
}

void SampleInfoWidget::initButtonForm()
{
}

void SampleInfoWidget::initTableFormat()
{
    int tbvRowCount = ROWCOUNT; //列表最大能显示的行数
    //定标测试列表
    QStringList headerList_h;
    headerList_h << "id" << "h_id" << " " << "状态" << "项目" << "测试编号" << "试管编号" << "申请时间"  << "样本类型" << "急诊否"<< "重测否" << "state";
    modelList->setHorizontalHeaderLabels( headerList_h );
    tbvList->hideColumn(0);             //id-隐藏
    tbvList->hideColumn(1);             //h_id-隐藏
    //tbvList->setColumnWidth( 0, 40);    //id-隐藏
    //tbvList->setColumnWidth( 1, 40);    //h_id-隐藏
    tbvList->setColumnWidth( 2, 40);    //选择
    tbvList->setColumnWidth( 3, 60);    //状态
    tbvList->setColumnWidth( 4, 80);    //项目
    //tbvList->setColumnWidth( 5, 160); //测试编号
    //tbvList->hideColumn(5);             //测试编号
    tbvList->setColumnWidth( 6, 150);   //试管编号
    tbvList->setColumnWidth( 7, 150);   //申请时间
    tbvList->setColumnWidth( 8, 100);   //样本类型
    tbvList->setColumnWidth( 9, 80);    //急诊否
    tbvList->setColumnWidth( 10, 80);   //重测否
    tbvList->hideColumn(11);             //state-隐藏
    tbvList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvList->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvList->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvList->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
    tbvList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
//    tbvList->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
    tbvList->horizontalHeader()->setStyleSheet("font-size:18px;");         //表头字体样式

    //初始化每一个单元格
    for( int i = 0; i < tbvRowCount; i ++ )
    {
        modelList->setItem( i, 0, new QStandardItem( "" ));    //id-隐藏
        modelList->setItem( i, 1, new QStandardItem( "" ));    //h_id-隐藏
        modelList->setItem( i, 2, new QStandardItem( "" ));    //选择
        modelList->setItem( i, 3, new QStandardItem( "" ));    //状态
        modelList->setItem( i, 4, new QStandardItem( "" ));    //项目
        modelList->setItem( i, 5, new QStandardItem( "" ));    //测试编号
        modelList->setItem( i, 6, new QStandardItem( "" ));    //试管编号
        modelList->setItem( i, 7, new QStandardItem( "" ));    //申请时间
        modelList->setItem( i, 8, new QStandardItem( "" ));    //样本类型
        modelList->setItem( i, 9, new QStandardItem( "" ));    //急诊否
        modelList->setItem( i, 10, new QStandardItem( "" ));   //重测否
        modelList->setItem( i, 11, new QStandardItem( "" ));   //重测否

        //列表内容显示方式  居中
        modelList->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 4)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 5)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 6)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 7)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 8)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 9)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 10)->setTextAlignment(Qt::AlignCenter);
    }

}

//界面风格
void SampleInfoWidget::initStyle()
{
    initStyle2();

    labStatusPic[0]->setObjectName("Idle");
    labStatusPic[1]->setObjectName("Wait");
    labStatusPic[2]->setObjectName("Testing");
    labStatusPic[3]->setObjectName("Finished");
    labStatusPic[4]->setObjectName("unFinished");
    //labStatusPic[5]->setObjectName("noSample");

    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        btnSample1[i]->setObjectName("Idle");
        btnSample2[i]->setObjectName("Idle");
        btnSample3[i]->setObjectName("Idle");
    }
    btnSample4[0]->setObjectName("Idle");
    btnSample4[1]->setObjectName("Idle");

    //
    editSamplePos->setAlignment(Qt::AlignCenter);
    editSampleSlot->setAlignment(Qt::AlignCenter);
    editSampleType->setAlignment(Qt::AlignCenter);

    //
    editSamplePos->setReadOnly(true);
    editSampleSlot->setReadOnly(true);
    editSampleType->setReadOnly(true);
    btnDelete->setObjectName("button6");


    tbvList->setObjectName("tableV");
    labTimes->setObjectName("lab20");
}
void SampleInfoWidget::initStyle2()
{
    QString strLab =
            "QLabel#Idle       { image: url(:/resource/resource/SampleStatus/Idle.png); }"
            "QLabel#Wait   { image: url(:/resource/resource/SampleStatus/Waiting.png); }"
            "QLabel#Testing    { image: url(:/resource/resource/SampleStatus/Testing.png); }"
            "QLabel#Finished   { image: url(:/resource/resource/SampleStatus/Finished.png); }"
            "QLabel#unFinished { image: url(:/resource/resource/SampleStatus/unFinished.png); }"
            "QLabel#noSample   { image: url(:/resource/resource/SampleStatus/noSample.png); }"
            ;
    QString strBtn =
            //空闲
            "QPushButton#Idle{ width:50; height:50;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/SampleStatus/Idle.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Idle:hover{ width:50; height:50; border-radius:8px; font-size:28px; color:darkCyan; background-image:url(:/resource/resource/SampleStatus/Idle.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Idle:checked{ width:50; height:50; border-radius:8px; font-size:30px; color:darkMagenta; background-image:url(:/resource/resource/SampleStatus/Idle.png); background-repeat:no-repeat; background-position:center; }"
            //待测试
            "QPushButton#Wait{width:50; height:50;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/SampleStatus/Waiting.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Wait:hover{width:50; height:50; border-radius:8px; font-size:28px; color:darkCyan; background-image:url(:/resource/resource/SampleStatus/Waiting.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Wait:checked{width:50; height:50; border-radius:8px; font-size:30px; color:darkMagenta; background-image:url(:/resource/resource/SampleStatus/Waiting.png); background-repeat:no-repeat; background-position:center; }"
            //测试中
            "QPushButton#Testing{width:50; height:50;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/SampleStatus/Testing.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Testing:hover{width:50; height:50; border-radius:8px; font-size:28px; color:darkCyan; background-image:url(:/resource/resource/SampleStatus/Testing.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Testing:checked{width:50; height:50; border-radius:8px; font-size:30px; color:darkMagenta; background-image:url(:/resource/resource/SampleStatus/Testing.png); background-repeat:no-repeat; background-position:center; }"
            //已完成
            "QPushButton#Finished{width:50; height:50;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/SampleStatus/Finished.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Finished:hover{width:50; height:50; border-radius:8px; font-size:28px; color:darkCyan; background-image:url(:/resource/resource/SampleStatus/Finished.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#Finished:checked{width:50; height:50; border-radius:8px; font-size:30px; color:darkMagenta; background-image:url(:/resource/resource/SampleStatus/Finished.png); background-repeat:no-repeat; background-position:center; }"
            //未完成
            "QPushButton#unFinished{width:50; height:50;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/SampleStatus/unFinished.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#unFinished:hover{width:50; height:35; border-radius:8px; font-size:28px; color:darkCyan; background-image:url(:/resource/resource/SampleStatus/unFinished.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#unFinished:checked{width:50; height:50; border-radius:8px; font-size:30px; color:darkMagenta; background-image:url(:/resource/resource/SampleStatus/unFinished.png); background-repeat:no-repeat; background-position:center; }"
            //缺少样本
            "QPushButton#noSample{width:50; height:50;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/SampleStatus/noSample.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#noSample:hover{width:50; height:50; border-radius:8px; font-size:28px; color:darkCyan; background-image:url(:/resource/resource/SampleStatus/noSample.png); background-repeat:no-repeat; background-position:center; }"
            "QPushButton#noSample:checked{width:50; height:50; border-radius:8px; font-size:30px; color:darkMagenta; background-image:url(:/resource/resource/SampleStatus/noSample.png); background-repeat:no-repeat; background-position:center; }"
            //共用
            "QPushButton::indicator {width:1px; height:1px; background-image:url(:/resource/resource/SampleStatus);}"
            ;
    this->setStyleSheet(strLab + strBtn);
}

//连接信号与槽
void SampleInfoWidget::initConnect()
{
    sigMapper = new QSignalMapper(this);

    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        sigMapper->setMapping(btnSample1[i], "1-"+ QString::number(i+1) ); //1号样本架
        sigMapper->setMapping(btnSample2[i], "2-"+ QString::number(i+1) ); //2号样本架
        sigMapper->setMapping(btnSample3[i], "3-"+ QString::number(i+1) ); //3号样本架

        connect(btnSample1[i],SIGNAL(clicked()),sigMapper,SLOT(map()));
        connect(btnSample2[i],SIGNAL(clicked()),sigMapper,SLOT(map()));
        connect(btnSample3[i],SIGNAL(clicked()),sigMapper,SLOT(map()));
    }

    sigMapper->setMapping(btnSample4[0], "0-1" ); //急诊样本1
    connect(btnSample4[0],SIGNAL(clicked()),sigMapper,SLOT(map()));
    sigMapper->setMapping(btnSample4[1], "0-2" ); //急诊样本2
    connect(btnSample4[1],SIGNAL(clicked()),sigMapper,SLOT(map()));
    connect(sigMapper,SIGNAL(mapped(const QString&)), this,SLOT(slt_sigMapReceive(const QString&)));

    //按钮
    connect(btnDelete, SIGNAL(clicked(bool)), this, SLOT(slt_btnDelete_Clicked()));
    connect(gApplicateThread, SIGNAL(sig_updateAllSampleStatus()), this, SLOT(slt_updateAllSampleStatus()));
    connect(gScheduler, SIGNAL(sig_updateSampleStatus(QString,QString)), this, SLOT(slt_updateSampleStatus(QString,QString)));
    connect(gScheduler, SIGNAL(sig_UpdateRemainingTime(int)), this, SLOT(slt_UpdateRemainingTime(int)));

    //
    connect(tbvList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slt_tbvList_DoubleClicked(QModelIndex)));
}

//初始化数据
void SampleInfoWidget::initData()
{
    slt_updateAllSampleStatus();   //槽：更新所有样本状态
}

//更改样本位置（查询数据）
void SampleInfoWidget::ChangeSamplePosData(QString pos)
{
    QString str  =
            //任务id，h_id，
            " select TaskT.id, HistoryT_id, StateT.id, StateT.name, ProjectT.name, TaskT.no, TaskT.sampleCode, "
            " TaskT.dateTime, SampleTypeT.name, TaskT.isEmergency from TaskT "
            //连接方式(任务队列表、项目表)
            " Inner join ProjectT ON TaskT.projectID = ProjectT.id "
            //连接方式(任务队列表、)
            " Inner join StateT ON TaskT.stateID = StateT.id "
            //连接方式(任务队列表、)
            " Inner join SampleTypeT ON TaskT.sampleTypeID = SampleTypeT.id "
            " where "
            " TaskT.SamplePos='" + pos +"' "
            //查询条件：等待或测试中
            " and (StateT.id = 4 or StateT.id = 3)"
            //排序查询:
            " order by TaskT.id asc, TaskT.no asc "
            ";";

    int i = 0;
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    while(query.next())
    {
        if( i == ROWCOUNT ) break;
        modelList->item(i, 0)->setText(query.value(0).toString());  //id(隐藏)
        modelList->item(i, 1)->setText(query.value(1).toString());  //h_id-隐藏
        modelList->item(i, 3)->setText(query.value(3).toString());  //状态
        modelList->item(i, 4)->setText(query.value(4).toString());  //项目
        modelList->item(i, 5)->setText(query.value(5).toString());  //编号
        modelList->item(i, 6)->setText(query.value(6).toString());  //试管编号
        modelList->item(i, 7)->setText(query.value(7).toString());  //申请时间
        modelList->item(i, 8)->setText(query.value(8).toString());  //样本类型
        if( query.value(9).toInt() )  //急诊否
            modelList->item(i, 9)->setText("Y");
        else
            modelList->item(i, 9)->setText("N");
        if( query.value(10).toInt() ) //重测否
            modelList->item(i, 10)->setText("Y");
        else
            modelList->item(i, 10)->setText("N");
        modelList->item(i, 11)->setText(query.value(2).toString());  //状态

        i ++;
    }

    //清空数据
    for( i; i < ROWCOUNT; i ++ )
    {
        modelList->item(i, 0)->setText("");
        modelList->item(i, 1)->setText("");
        modelList->item(i, 2)->setText("");
        modelList->item(i, 3)->setText("");
        modelList->item(i, 4)->setText("");
        modelList->item(i, 5)->setText("");
        modelList->item(i, 6)->setText("");
        modelList->item(i, 7)->setText("");
        modelList->item(i, 8)->setText("");
        modelList->item(i, 9)->setText("");
        modelList->item(i, 10)->setText("");
        modelList->item(i, 11)->setText("");
    }
}

//设置slot号样本架上no号样本的状态
void SampleInfoWidget::setSampleStatus(int slot, int no, QString objName)
{
    //qDebug() << "**************Sample*slot:" << slot << "no:" << no << objName;

    if( no > SAMPLESLOT_COUNT || no < 0) return;

    if(slot == 1) //第一排样本架
        btnSample1[no]->setObjectName(objName);
    else if(slot == 2) //第二排样本架
        btnSample2[no]->setObjectName(objName);
    else if(slot == 3) //第三排样本架
        btnSample3[no]->setObjectName(objName);
    else if(slot == 0) //急诊样本位
    {
        if( no >= 2 ) return;
        btnSample4[no]->setObjectName(objName);
    }
    initStyle2();
}

//槽：更新所有样本状态
void SampleInfoWidget::slt_updateAllSampleStatus()
{
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        btnSample1[i]->setObjectName("Idle");
        btnSample2[i]->setObjectName("Idle");
        btnSample3[i]->setObjectName("Idle");
    }
    btnSample4[0]->setObjectName("Idle");
    btnSample4[1]->setObjectName("Idle");

    int slot, no;
    QString str  =
            "select TaskT.samplePos, StateT.remarks from TaskT "
            " Inner join StateT ON TaskT.stateID = StateT.id "
            " where TaskT.stateID =3 and  typeID =" + QString::number( MV_State_Normal )
            + " group by TaskT.samplePos "
              " order by TaskT.samplePos "
            +";";
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    while(query.next())
    {
        slot = query.value(0).toString().split("-").at(0).toInt();
        no = query.value(0).toString().split("-").at(1).toInt();
        setSampleStatus(slot, no-1, "Wait");
    }
}
//槽：更新当前指定一个样本状态
void SampleInfoWidget::slt_updateSampleStatus(QString currentPos)
{
    if(currentPos == "") return;
    int slot, no;
    slot = currentPos.split("-").at(0).toInt();
    no = currentPos.split("-").at(1).toInt();
    setSampleStatus(slot, no-1, "Testing");
}
//槽：更新指定两个样本的状态
void SampleInfoWidget::slt_updateSampleStatus(QString formerPos, QString currentPos)
{
    qDebug() << "formerPos:" << formerPos << "currentPos:" << currentPos;
    int slot, no;
    if( formerPos != "")
    {
        QString str  =
                "select TaskT.samplePos, StateT.remarks from TaskT "
                " Inner join StateT ON TaskT.stateID = StateT.id "
                " where TaskT.stateID =3 and  typeID =" + QString::number( MV_State_Normal )
                + " and  TaskT.samplePos='" + formerPos + "' "
                +";";
        int flag = 0;
        QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
        while(query.next())
        {
            slot = query.value(0).toString().split("-").at(0).toInt();
            no = query.value(0).toString().split("-").at(1).toInt();
            setSampleStatus(slot, no-1, "Wait");
            flag = 1;
        }
        if(flag ==0)
        {
            slot = formerPos.split("-").at(0).toInt();
            no = formerPos.split("-").at(1).toInt();
            setSampleStatus(slot, no-1, "Finished");
        }
    }

    if(currentPos != "")
    {
        slot = currentPos.split("-").at(0).toInt();
        no = currentPos.split("-").at(1).toInt();
        setSampleStatus(slot, no-1, "Testing");
    }
}
//设置slot号样本架上no号样本的状态
void SampleInfoWidget::slt_SetSampleStatus(int slot, int no, QString objName)
{
    setSampleStatus(slot,  no, objName);
}

//槽：信号与槽的管理群
void SampleInfoWidget::slt_sigMapReceive(const QString& text)
{
    if( text.split("-").count() != 2 ) return;

    QString SampleSlot = text.split("-").at(0);
    QString SampleNo = text.split("-").at(1);

    editSampleSlot->setText(SampleSlot);
    if( SampleSlot == "0" ) editSampleSlot->setText("急诊位");

    editSamplePos->setText(SampleNo);

    //查询数据
    CurrentSamplePos = text;
    ChangeSamplePosData(text);
}

//删除（若选中的样本位没有可以删除的测试时怎么办 ????????? 应该不弹窗 20170706）
void SampleInfoWidget::slt_btnDelete_Clicked()
{
    //先更新下，以免删除了正在进行且还没更新状态的测试
    ChangeSamplePosData(CurrentSamplePos);
    QString pos = editSampleSlot->text().trimmed() + "-" + editSamplePos->text().trimmed();

    if(pos.count() == 1) return;//未选中样本

    gMessageBox->setInfo(QString("提示"), QString("是否要删除样本位“" + pos +"”\n所有等待状态的测试?"),
                         QPixmap("://resource/resource/MessageBox/information.png"),  true, false);
    gMessageBox->setButtonText(Cancel, "否(N)");
    gMessageBox->setButtonText(Sure, "是(Y)");
    if(gMessageBox->exec() == MVMessageBox::Accepted)
    {
        QStringList strList;
        strList.append("delete from TaskT where  stateID ="+ QString::number(State_WAIT) +" and samplePos='"+ pos +"';");
        SQLite::getInstance()->Delete(strList.at(0));
        //SQLite::getInstance()->Transaction(strList);

        //删除，重新查询（更新表格）
        ChangeSamplePosData(CurrentSamplePos);
    }
}

//槽:反应曲线（20170706 取消反应曲线按钮，改成双击表格弹出曲线界面）
void SampleInfoWidget::slt_btnCurve_Clicked()
{
    int row = tbvList->currentIndex().row();  //当前行
    if(row == -1) return; //未选中

    int id = 2;
    //    int id = modelList->item(row, 1)->text().toInt();  //h_id列
    //    if(id == 0) return;   //未开始测试

    ResultCurveWidget::getInstance()->setSelect_id(id);
    ResultCurveWidget::getInstance()->show();
}

//槽：更新剩余反应时间(UI上显示测试剩余时间)
void SampleInfoWidget::slt_UpdateRemainingTime(int cycle)
{
    int time = cycle*gParameter->getCycleTime()/1000/60;
    if(time < 0) time = 0;
    if(time > 1)
        labTimes->setText("剩余时间: " + QString::number( time ) + " min");   //大概剩余时间
    else
        labTimes->setText("剩余时间: " + QString::number( cycle*gParameter->getCycleTime()/1000 ) + " sec");   //大概剩余时间

}

//槽：列表双击事件
void SampleInfoWidget::slt_tbvList_DoubleClicked(QModelIndex index)
{
    int row = index.row();  //当前行
    if(row == -1) return; //未选中


    if( modelList->item(row, 11)->text().toInt() == State_TESTING )
    {
        int id = modelList->item(row, 1)->text().toInt();  //h_id列
        if(id == 0) return;   //未开始测试
        ResultCurveWidget::getInstance()->setSelect_id(id);
        ResultCurveWidget::getInstance()->show();
    }
}


