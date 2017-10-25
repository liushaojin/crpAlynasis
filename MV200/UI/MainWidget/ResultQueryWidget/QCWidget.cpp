#include "QCWidget.h"
#include <math.h>
#include "ParamDefine.h"
#include "Parameter.h"
#include "Thread/Scheduler.h"
#include "UI/Widget/MessageBox/messageBox.h"
#include "UI/MainWidget/CurveWidget/resultCurveWidget.h"

QCWidget::QCWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();          //初始化
    initTableForm();     //初始化表格
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}

//更新列表数据（质控液、项目列表）
void QCWidget::updateTableData()
{
    QVector<QString> vec = gParameter->get_ProjectName();
    for(int i = 0; i < vec.count(); i ++)
    {
        if(i < vec.count())
            modelProjectName->item(i, 0)->setText(vec.at(i));
        else
            modelProjectName->item(i, 0)->setText("");
    }

    modelQCLiquid->clear();
    QStringList headerList_h;
    headerList_h  <<"质控液";
    modelQCLiquid->setHorizontalHeaderLabels( headerList_h );
    QStringList list = gParameter->getQcLiquidName();
    int i = 0;
    for( i; i < list.count(); i ++)
    {
        modelQCLiquid->setItem( i, 0, new QStandardItem( list.at(i) ));
        modelQCLiquid->item(i, 0)->setTextAlignment(Qt::AlignCenter); //居中
    }
}

//初始化
void QCWidget::initForm()
{
    //查询条件
    grbQueryTerms = new QGroupBox("查询条件");
    rdoRealtimeQC = new QRadioButton("实时质控");   //实时质控
    rdoDayQC = new QRadioButton("天间质控");        //天间质控
    rdoInternalQC = new QRadioButton("天内质控");   //天内质控
    labDate = new QLabel("日期:");                 //日期
    dateStart = new QDateEdit;          //查询开始日期
    dateEnd = new QDateEdit;            //查询结束日期
    lytQueryTerms = new QGridLayout;
    lytQueryTerms->setContentsMargins(10, 20, 0, 10);
    lytQueryTerms->addWidget(rdoRealtimeQC,  0,  0, 3, 5, Qt::AlignCenter);
    lytQueryTerms->addWidget(rdoInternalQC,  0,  5, 3, 5, Qt::AlignCenter);
    lytQueryTerms->addWidget(rdoDayQC,       0, 10, 3, 5, Qt::AlignCenter);
    lytQueryTerms->addWidget(labDate,        5,  0, 3, 3, Qt::AlignCenter);
    lytQueryTerms->addWidget(dateStart,      5,  3, 3, 5, Qt::AlignCenter);
    lytQueryTerms->addWidget(dateEnd,        5,  8, 3, 7, Qt::AlignCenter);
    grbQueryTerms->setLayout(lytQueryTerms);
    //项目名称
    tbvProjectName = new QTableView;
    modelProjectName = new QStandardItemModel;
    tbvProjectName->setModel(modelProjectName);
    //质控液
    tbvQCLiquid = new QTableView;
    modelQCLiquid = new QStandardItemModel;
    tbvQCLiquid->setModel(modelQCLiquid);

    initQueryDataForm();
    createRightMenu();   //创建一个右键菜单

    //总布局
    layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(grbQueryTerms,    0,  0,  10,  10);
    layout->addWidget(tbvQCLiquid,     10,  0,  90,   5);
    layout->addWidget(tbvProjectName,  10,  5,  90,   5);
    layout->addWidget(tabQueryData,     0, 10, 100, 100);

    this->setLayout(layout);
}

//创建一个右键菜单
void QCWidget::createRightMenu()
{
    tbvQueryData->setContextMenuPolicy(Qt::CustomContextMenu);  //少这句，右键没有任何反应的。
    rightMenu = new QMenu;
    formerAction = new QAction("上一页",this);
    nextAction = new QAction("下一页",this);
    curveAction = new QAction("曲 线",this);
    allAction = new QAction("全选", this);
    deleteAction = new QAction("删 除",this);

    rightMenu->addAction(formerAction);
    rightMenu->addAction(nextAction);
    rightMenu->addAction(curveAction);
    rightMenu->addAction(allAction);
    rightMenu->addAction(deleteAction);
}

void QCWidget::initQueryDataForm()
{
    //查询数据（包括：多规则质控图、累计和质控图、twin plo图、质控数据）
    tabQueryData = new QTabWidget;

    //多规则质控图
    qcWestgardWidget = new QcWestgardWidget;

    //质控数据
    tbvQueryData = new QTableView;            //质控数据
    modelQueryData = new QStandardItemModel;
    tbvQueryData->setModel(modelQueryData);

    tabQueryData->addTab(tbvQueryData, "质控数据");
    tabQueryData->addTab(qcWestgardWidget, "多规则质控图");
    //tabQueryData->addTab(new QWidget, "累计和质控图");
    //tabQueryData->addTab(new QWidget, "twin plo图");
}

//初始化表格
void QCWidget::initTableForm()
{
    //质控液列表
    tbvQCLiquid->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvQCLiquid->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvQCLiquid->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvQCLiquid->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvQCLiquid->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
    //tbvQCLiquid->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动

    //项目名称列表
    QStringList headerList_h;
    headerList_h << "项目";
    modelProjectName->setHorizontalHeaderLabels( headerList_h );
    tbvProjectName->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvProjectName->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvProjectName->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvProjectName->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
    //tbvProjectName->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
    //初始化每一个单元格
    for( int i = 0; i < PROJECT_COUNT; i ++ )
    {
        modelProjectName->setItem( i, 0, new QStandardItem( "" ));
        modelProjectName->item(i, 0)->setTextAlignment(Qt::AlignCenter); //居中
        //        modelProjectName->item(i, 0)->setFont( QFont( "", 12 ) );        //字体大小
    }

    //质控数据列表
    headerList_h.clear();
    headerList_h << "" << "id" << "质控液" <<"项目" << "状态"  << "结果"  << "靶值" << "标准差"<< "是否默认"
                 << "样本类型" << "质控批号" << "申请时间" << "测试时间" << "SamplePos" << "state" << "sampleType";
    modelQueryData->setHorizontalHeaderLabels( headerList_h );
    tbvQueryData->setColumnWidth( 0, 30);    //选择
    //tbvQueryData->hideColumn(0);             //选择-隐藏
    tbvQueryData->hideColumn(1);             //id-隐藏
    tbvQueryData->setColumnWidth( 2, 90);    //质控液
    tbvQueryData->setColumnWidth( 3, 80);    //项目
    tbvQueryData->setColumnWidth( 4, 80);    //状态
    tbvQueryData->setColumnWidth( 5, 70);    //结果
    tbvQueryData->setColumnWidth( 6, 70);    //靶值
    tbvQueryData->setColumnWidth( 7, 70);    //标准差
    tbvQueryData->hideColumn(8);             //是否默认-隐藏
    tbvQueryData->setColumnWidth( 9, 70);    //样本类型
    tbvQueryData->setColumnWidth( 10,110);    //质控批号
    tbvQueryData->setColumnWidth( 11,180);   //申请时间
    tbvQueryData->setColumnWidth( 12,180);   //测试时间
    tbvQueryData->hideColumn(13);            //测试编号-隐藏
    tbvQueryData->hideColumn(14);            //state-隐藏
    tbvQueryData->hideColumn(15);            //sampleType-隐藏
    tbvQueryData->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvQueryData->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvQueryData->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvQueryData->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
    tbvQueryData->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    //tbvQueryData->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
    tbvQueryData->horizontalHeader()->setStyleSheet("font-size:18px;");         //表头字体样式

    QStandardItem *item = 0;
    for( int i = 0; i < ROWCOUNT; i ++ )
    {
        item = new QStandardItem;
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);

        modelQueryData->setItem( i, 0, item);
        modelQueryData->setItem( i, 1, new QStandardItem( "" ));
        modelQueryData->setItem( i, 2, new QStandardItem( "" ));
        modelQueryData->setItem( i, 3, new QStandardItem( "" ));
        modelQueryData->setItem( i, 4, new QStandardItem( "" ));
        modelQueryData->setItem( i, 5, new QStandardItem( "" ));
        modelQueryData->setItem( i, 6, new QStandardItem( "" ));
        modelQueryData->setItem( i, 7, new QStandardItem( "" ));
        modelQueryData->setItem( i, 8, new QStandardItem( "" ));
        modelQueryData->setItem( i, 9, new QStandardItem( "" ));
        modelQueryData->setItem( i, 10, new QStandardItem( "" ));
        modelQueryData->setItem( i, 11, new QStandardItem( "" ));
        modelQueryData->setItem( i, 12, new QStandardItem( "" ));
        modelQueryData->setItem( i, 13, new QStandardItem( "" ));
        modelQueryData->setItem( i, 14, new QStandardItem( "" ));
        modelQueryData->setItem( i, 15, new QStandardItem( "" ));

        //列表内容显示方式
        for(int j = 2; j < 14; j ++)
            modelQueryData->item(i, j)->setTextAlignment(Qt::AlignCenter); //居中
    }
}

//界面风格
void QCWidget::initStyle()
{
    tbvProjectName->setMaximumWidth(200);
    tbvQCLiquid->setMaximumWidth(200);
    rdoRealtimeQC->setChecked(true);
    dateStart->setEnabled(false);//查询开始日期
    dateEnd->setEnabled(false);  //查询结束日期
}

//连接信号与槽
void QCWidget::initConnect()
{
    connect(rdoRealtimeQC, SIGNAL(clicked(bool)), this, SLOT(slt_rdoRealtimeQC_clicked(bool)));
    connect(rdoInternalQC, SIGNAL(clicked(bool)), this, SLOT(slt_rdoInternalQC_clicked(bool)));
    connect(rdoDayQC, SIGNAL(clicked(bool)), this, SLOT(slt_rdoDayQC_clicked(bool)));
    connect( this, SIGNAL(sig_updateGraph_Data(QVector<double>, double, double)),
             qcWestgardWidget, SLOT(slt_UpdateGraph_Data(QVector<double>, double, double)) );

    connect(tabQueryData, SIGNAL(currentChanged(int)), this, SLOT(slt_tabQueryData_Changed(int)));
    connect(tbvQCLiquid, SIGNAL(clicked(QModelIndex)), this, SLOT(slt_tbvQCLiquid_Clicked(QModelIndex)));
    connect(tbvProjectName, SIGNAL(clicked(QModelIndex)), this, SLOT(slt_tbvProjectName_Clicked(QModelIndex)));

    connect(tbvQueryData,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(clicked_rightMenu(QPoint)));
    connect(formerAction, SIGNAL(triggered(bool)), this, SLOT(slt_formerAction_Triggered()));
    connect(nextAction, SIGNAL(triggered(bool)), this, SLOT(slt_nextAction_Triggered()));
    connect(curveAction, SIGNAL(triggered(bool)), this, SLOT(slt_curveAction_Triggered()));
    connect(allAction, SIGNAL(triggered(bool)), this, SLOT(slt_allAction_Triggered()));
    connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(slt_deleteAction_Triggered()));
}

//初始化数据
void QCWidget::initData()
{
    isAllNull_Flag = 0;         //是否全选标志
    Count = 0;
    //当前选中的质控洗衣机的行数
    CurrentProjece_id = 0;
    CurrentQcLiquid_id = 0;

    dateStart->setDisplayFormat("yyyy-MM-dd");
    dateEnd->setDisplayFormat("yyyy-MM-dd");
    dateStart->setDate(QDate::currentDate());          //查询开始日期
    dateEnd->setDate(QDate::currentDate());            //查询结束日期

    QString menu =
            "QMenu{ border-radius:5px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #113845, stop: 0.2 #1878B1, stop: 1.0 #15A8FF);border: 1px solid gray;}"
            "QMenu::item { width:120; background-color: transparent; font:20px Times New Roman;"
            "padding:10px 20px;"//设置菜单项文字上下和左右的内边距，效果就是菜单中的条目左右上下有了间隔
            "margin:0px 5px;"//设置菜单项的外边距
            "border-bottom:1px solid #DBDBDB;}"//为菜单项之间添加横线间隔
            "QMenu::item:selected{background-color: #2dabf9;}";
    rightMenu->setStyleSheet(menu);
}

//查询质控液相应的测试数据
int QCWidget::QueryQcData(int id)
{
    int flag = 0;
    QString str;

    QString temp;
    if(rdoRealtimeQC->isChecked())
        temp = "";
    else if(rdoInternalQC->isChecked())
    {
        QString date2 = dateEnd->date().toString("yyyy-MM-dd");
        temp = " and TempHistoryT.applyTime like '%" + date2 + "%' ";
    }
    else if(rdoDayQC->isChecked())
    {
        QString date1 = dateStart->date().toString("yyyy-MM-dd 00:00:00");
        QString date2 = dateEnd->date().toString("yyyy-MM-dd 23:59:59");
        temp = " and TempHistoryT.applyTime between '" + date1 + "' and '" + date2 + "' ";
    }

    if(CurrentProjece_id == 0)
    {
        str=
                " select TempHistoryT.T_id, TempHistoryT.id,"
                " QCLiquidT.name, ProjectT.name, StateT.name,"
                " TempHistoryT.result, QCParamT.targetValue, QCParamT.SD,  "
                " TempHistoryT.sampleCode, SampleTypeT.name, "
                " QCLiquidT.batchNo, TempHistoryT.applyTime, TempHistoryT.testDate, TempHistoryT.samplePos, "
                " TempHistoryT.statusID, TempHistoryT.sampleTypeID "
                " from TempHistoryT "
                //连接方式(表、质控液表)
                " Inner join QCLiquidT ON TempHistoryT.qcLiquidID = QCLiquidT.id "
                //连接方式(表、质控参数表)
                " Inner join QCParamT ON TempHistoryT.qcLiquidID = QCParamT.QCLiquidID "
                //连接方式(表、项目表)
                " Inner join ProjectT ON TempHistoryT.projectID = ProjectT.id "
                //连接方式(表、样本类型表)
                " Inner join SampleTypeT ON TempHistoryT.sampleTypeID = SampleTypeT.id "
                //连接方式(表、状态表)
                " Inner join StateT ON TempHistoryT.statusID = StateT.id "
                //查询条件
                " where  QCParamT.QCLiquidID = QCLiquidT.id and TempHistoryT.projectID = QCParamT.projectID "
                " and TempHistoryT.qcLiquidID="+ QString::number(id)
                +" and TempHistoryT.typeID =" + QString::number( MV_State_QC ) + " "
                //排序查询：紧急否降序
                " order by TempHistoryT.id  asc "
                " limit " + QString::number(Count) +", " +QString::number(ROWCOUNT)
                +";";
    }
    else
    {
        str=
                " select TempHistoryT.T_id, TempHistoryT.id,"
                " QCLiquidT.name, ProjectT.name, StateT.name,"
                " TempHistoryT.result, QCParamT.targetValue, QCParamT.SD,  "
                " TempHistoryT.sampleCode, SampleTypeT.name, "
                " QCLiquidT.batchNo, TempHistoryT.applyTime, TempHistoryT.testDate, TempHistoryT.samplePos, "
                " TempHistoryT.statusID, TempHistoryT.sampleTypeID "
                " from TempHistoryT "
                //连接方式(表、质控液表)
                " Inner join QCLiquidT ON TempHistoryT.qcLiquidID = QCLiquidT.id "
                //连接方式(表、质控参数表)
                " Inner join QCParamT ON TempHistoryT.qcLiquidID = QCParamT.QCLiquidID "
                //连接方式(表、项目表)
                " Inner join ProjectT ON TempHistoryT.projectID = ProjectT.id "
                //连接方式(表、样本类型表)
                " Inner join SampleTypeT ON TempHistoryT.sampleTypeID = SampleTypeT.id "
                //连接方式(表、状态表)
                " Inner join StateT ON TempHistoryT.statusID = StateT.id "
                //查询条件
                " where  QCParamT.QCLiquidID = QCLiquidT.id and TempHistoryT.projectID = QCParamT.projectID "
                " and TempHistoryT.qcLiquidID="+ QString::number(id)
                +" and TempHistoryT.projectID="+ QString::number(CurrentProjece_id)
                +" and TempHistoryT.typeID =" + QString::number( MV_State_QC )
                + temp +
                //排序查询：紧急否降序
                " order by TempHistoryT.id asc "
                " limit " + QString::number(Count) +", " +QString::number(ROWCOUNT)
                +";";
    }

    int i = 0;
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    while(query.next())
    {
        modelQueryData->item(i, 0)->setCheckState(Qt::Unchecked);
        //modelQueryData->item(i, 0)->setText(query.value(0).toString());  //
        modelQueryData->item(i, 1)->setText(query.value(1).toString());  //
        modelQueryData->item(i, 2)->setText(query.value(2).toString());  //
        modelQueryData->item(i, 3)->setText(query.value(3).toString());  //
        modelQueryData->item(i, 4)->setText(query.value(4).toString());  //
        modelQueryData->item(i, 5)->setText(query.value(5).toString());  //
        modelQueryData->item(i, 6)->setText(query.value(6).toString());  //
        modelQueryData->item(i, 7)->setText(query.value(7).toString());  //
        modelQueryData->item(i, 8)->setText(query.value(8).toString());  //
        modelQueryData->item(i, 9)->setText(query.value(9).toString());  //
        modelQueryData->item(i, 10)->setText(query.value(10).toString());  //
        modelQueryData->item(i, 11)->setText(query.value(11).toString());  //
        modelQueryData->item(i, 12)->setText(query.value(12).toString());  //
        modelQueryData->item(i, 13)->setText(query.value(13).toString());  //
        modelQueryData->item(i, 14)->setText(query.value(14).toString());  //
        modelQueryData->item(i, 15)->setText(query.value(15).toString());  //
        i++;
        flag = 1;
    }

    //滚动条置顶
    tbvQueryData->scrollToTop();
    //是否全选标志
    isAllNull_Flag = 0;
    allAction->setText("全选");
    //清空数据
    for( i; i < modelQueryData->rowCount(); i ++ )
    {
        modelQueryData->item(i, 0)->setCheckState(Qt::Unchecked);
        modelQueryData->item(i, 1)->setText("");  //
        modelQueryData->item(i, 2)->setText("");  //
        modelQueryData->item(i, 3)->setText("");  //
        modelQueryData->item(i, 4)->setText("");  //
        modelQueryData->item(i, 5)->setText("");
        modelQueryData->item(i, 6)->setText("");  //
        modelQueryData->item(i, 7)->setText("");  //
        modelQueryData->item(i, 8)->setText("");  //
        modelQueryData->item(i, 9)->setText("");  //
        modelQueryData->item(i, 10)->setText("");  //
        modelQueryData->item(i, 11)->setText("");  //
        modelQueryData->item(i, 12)->setText("");  //
        modelQueryData->item(i, 13)->setText("");  //
        modelQueryData->item(i, 14)->setText("");  //
        modelQueryData->item(i, 15)->setText("");  //
    }
    return flag;
}

//查询Westgard图对应的数据
void QCWidget::QueryWestgardData(int pid)
{
    QString str;
    if(rdoRealtimeQC->isChecked()) //实时质控
    {
        str  =
                " select TempHistoryT.id, TempHistoryT.result, QCParamT.SD, QCParamT.targetValue, TempHistoryT.statusID, QCParamT.TargetValue0, QCParamT.TargetValue1  from TempHistoryT "
                //连接方式(表、质控参数表)
                " Inner join QCParamT ON TempHistoryT.qcLiquidID = QCParamT.QCLiquidID "
                //查询条件
                " where TempHistoryT.projectID = QCParamT.projectID and TempHistoryT.projectID="+ QString::number( pid )
                +" and TempHistoryT.statusID="+ QString::number( State_FINISHED )
                +" and TempHistoryT.qcLiquidID="+ QString::number( CurrentQcLiquid_id )
                +" and TempHistoryT.typeID =" + QString::number( MV_State_QC ) + " "
                //排序查询：紧急否降序
                " order by TempHistoryT.id desc limit 0, 50;";
    }
    else if(rdoInternalQC->isChecked()) //天内质控
    {
        QString date1 = dateEnd->date().toString("yyyy-MM-dd");
        str  =
                " select TempHistoryT.id, TempHistoryT.result, QCParamT.SD, QCParamT.targetValue, TempHistoryT.statusID, QCParamT.TargetValue0, QCParamT.TargetValue1 from TempHistoryT "
                //连接方式(表、质控参数表)
                " Inner join QCParamT ON TempHistoryT.qcLiquidID = QCParamT.QCLiquidID "
                //查询条件
                " where TempHistoryT.projectID = QCParamT.projectID and TempHistoryT.projectID="+ QString::number( pid )
                +" and TempHistoryT.statusID="+ QString::number( State_FINISHED )
                +" and TempHistoryT.qcLiquidID="+ QString::number( CurrentQcLiquid_id )
                +" and TempHistoryT.typeID =" + QString::number( MV_State_QC )
                +" and TempHistoryT.applyTime like '%" + date1 + "%' "
                //排序查询：紧急否降序
                " order by TempHistoryT.id asc;";
    }
    else if(rdoDayQC->isChecked()) //天间质控
    {
        QString date1 = dateStart->date().toString("yyyy-MM-dd 00:00:00");
        QString date2 = dateEnd->date().toString("yyyy-MM-dd 23:59:59");

        str  =
                " select TempHistoryT.id, TempHistoryT.result, QCParamT.SD, QCParamT.targetValue, TempHistoryT.statusID, QCParamT.TargetValue0, QCParamT.TargetValue1 from TempHistoryT "
                //连接方式(表、质控参数表)
                " Inner join QCParamT ON TempHistoryT.qcLiquidID = QCParamT.QCLiquidID "
                //查询条件
                " where TempHistoryT.projectID = QCParamT.projectID and TempHistoryT.projectID="+ QString::number( pid )
                +" and TempHistoryT.statusID="+ QString::number( State_FINISHED )
                +" and TempHistoryT.qcLiquidID="+ QString::number( CurrentQcLiquid_id )
                +" and TempHistoryT.typeID =" + QString::number( MV_State_QC )
                +" and TempHistoryT.applyTime between '" + date1 + "' and '" + date2 + "' "
                //排序查询：紧急否降序
                " order by TempHistoryT.id";
    }

    vecResult.clear();   //查询到的质控浓度结果
    double TargetValue0, TargetValue1;//靶值上、下限
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    double resule;
    double target;
    double sd;
    while(query.next())
    {
        resule = query.value(1).toDouble(); //结果
        qcSD = query.value(2).toDouble();   //质控标准差
        target = query.value(3).toDouble(); //靶值
        TargetValue0 = query.value(5).toDouble(); //上限
        TargetValue1 = query.value(6).toDouble();//下限
        sd = (resule - target)/qcSD;
        vecResult.append( sd );
    }

    if(vecResult.count()> 0)
    {
        //实时质控
        if(rdoRealtimeQC->isChecked())
        {
            //保留最后一个数
            vecResult.clear();
            vecResult.append(sd);
        }
    }

    //qDebug() << "SD0" << qcSD << TargetValue1 << TargetValue0;
    //qDebug() << "vecResult" << vecResult;

    //信号：更新曲线数据
    emit sig_updateGraph_Data(vecResult, target, qcSD);
}

//槽：单选框
void QCWidget::slt_rdoRealtimeQC_clicked(bool flag)
{
    Count = 0;
    dateStart->setEnabled(false);//查询开始日期
    dateEnd->setEnabled(false);  //查询结束日期
}
void QCWidget::slt_rdoInternalQC_clicked(bool flag)
{
    Count = 0;
    dateStart->setEnabled(false);//查询开始日期
    dateEnd->setEnabled(true);  //查询结束日期
}
void QCWidget::slt_rdoDayQC_clicked(bool flag)
{
    Count = 0;
    dateStart->setEnabled(true);//查询开始日期
    dateEnd->setEnabled(true);  //查询结束日期
}

//槽：切换界面
void QCWidget::slt_tabQueryData_Changed(int index)
{
    if(index == 0) //多规则质控图
    {

    }
    else if(index == 1) //质控数据
    {

    }
}

//槽：选中质控液
void QCWidget::slt_tbvQCLiquid_Clicked(QModelIndex index)
{
    //选中无效
    if( !index.isValid() )
    {
        CurrentQcLiquid_id = 0;
        return;
    }
    if(index.row() < 0)
    {
        CurrentQcLiquid_id = 0;
        return;
    }

    //查询数据并绑定到质控数据表中
    QString qcName = modelQCLiquid->item(index.row(), 0)->text().trimmed();
    CurrentQcLiquid_id = gParameter->getQcLiquidID(qcName);

    //20170829 修改同时查曲线图&数据 方便用户查看
    QueryQcData(CurrentQcLiquid_id);
    QueryWestgardData(CurrentProjece_id);
}
//槽：选中常规项目
void QCWidget::slt_tbvProjectName_Clicked(QModelIndex index)
{
    if(CurrentQcLiquid_id == 0) return;

    if(modelProjectName->item(index.row(), 0)->text() != "")
        CurrentProjece_id = gParameter->getProject_id( modelProjectName->item(index.row(), 0)->text() );
    else CurrentProjece_id = 0;

    //20170829 修改同时查曲线图&数据 方便用户查看
    QueryQcData(CurrentQcLiquid_id);
    QueryWestgardData(CurrentProjece_id);
}

//槽：质控数据右键菜单（右键信号槽函数）
void QCWidget::clicked_rightMenu(const QPoint &pos)
{
    rightMenu->exec(QCursor::pos());
}

//槽：右键菜单-下一页
void QCWidget::slt_formerAction_Triggered()
{
    Count += ROWCOUNT;
    if( !QueryQcData(CurrentQcLiquid_id) )
    {
        Count -= ROWCOUNT;
        QueryQcData(CurrentQcLiquid_id);
    }
}
//槽：右键菜单-上一页
void QCWidget::slt_nextAction_Triggered()
{
    if(Count > 0)
    {
        Count -= ROWCOUNT;
        if( !QueryQcData(CurrentQcLiquid_id) )
        {
            Count += ROWCOUNT;
            QueryQcData(CurrentQcLiquid_id);
        }
    }
}
//槽：右键菜单-曲线
void QCWidget::slt_curveAction_Triggered()
{
    int row = tbvQueryData->currentIndex().row();  //当前行
    if(row == -1) return;  //未选中
    int id = modelQueryData->item(row, 1)->text().toInt();  //h_id列

    if(id > 0)
    {
        ResultCurveWidget::getInstance()->setSelect_id(id);
        ResultCurveWidget::getInstance()->show();
    }
}
//槽：右键菜单-全选
void QCWidget::slt_allAction_Triggered()
{
    for(int i = 0; i < ROWCOUNT; i ++ )
    {
        if(modelQueryData->item( i, 1)->text().toInt() > 0)
        {
            if (isAllNull_Flag)              //全选/反选标志
                modelQueryData->item( i, 0)->setCheckState(Qt::Unchecked);
            else
                modelQueryData->item( i, 0)->setCheckState(Qt::Checked);
        }
    }

    isAllNull_Flag = !isAllNull_Flag;
    if(isAllNull_Flag) allAction->setText("反选");
    else allAction->setText("全选");
}
//槽：右键菜单-删除
void QCWidget::slt_deleteAction_Triggered()
{
    QString str;
    QStringList list;
    list.clear();
    for(int i = 0; i < modelQueryData->rowCount(); i ++)
    {
        if (modelQueryData->item(i, 0)->checkState())
        {
            str = "delete from TempHistoryT where id=" + modelQueryData->item(i, 1)->text() + ";";
            list.append(str);
        }
    }

    if(list.count() > 0)
    {
        gMessageBox->setInfo(QString("提示"), QString("是否要删除所选中的数据?"), QPixmap("://resource/resource/MessageBox/information.png"),  true, false);
        gMessageBox->setButtonText(Sure, "确定");
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Accepted)
        {
            SQLite::getInstance()->Transaction(list);
            QueryQcData(CurrentQcLiquid_id);
        }
    }
}



