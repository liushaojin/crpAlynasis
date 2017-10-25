#include "SampleWidget.h"

SampleWidget::SampleWidget(QWidget *parent)
    : QWidget(parent)
{
    CurrentResultT_Count = 0;      //（结果列表）查询行数标志
    QueryTermsResultT = "";        //（结果列表）查询条件
    CurrentSampleCode_Count = 0;   //（样本列表）结果列表）查询行数标志
    QueryTermsSampleCode = "";      //（样本列表）查询条件

    initForm();          //初始化
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();
}

//初始化
void SampleWidget::initForm()
{
    modifyWidget = new ModifyResultWidget;
    connect(modifyWidget,SIGNAL(sig_ModifyResult()), this, SLOT(slt_ModifyResult()));

    initQueryTermsForm(); //
    initTableFormat();    //格式化表格
    initSampleCode();
    initResultList();

    //总布局
    layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(2);
    layout->addWidget(grbQueryTerms,   0,  0, 5,   50);
    layout->addWidget(wdgSampleCode,   5,  0, 85,  10);
    layout->addWidget(wdgResultList,   5, 10, 85, 100);
    this->setLayout(layout);
}

//查询条件
void SampleWidget::initQueryTermsForm()
{
    //查询条件
    labStatus = new QLabel("测试状态:");          //状态
    cboStauts = new QComboBox;
    chk_isDateTime = new QCheckBox("申请日期");
    dateDateTime = new QDateEdit;
    chk_isTestTime = new QCheckBox("测试日期");    //是否按测试日期查询
    dateTestTime = new QDateEdit;
    labProject = new QLabel("项目:");            //项目
    cboProject = new QComboBox;
    labSampleType = new QLabel("样本类型:");      //样本类型
    cboSampleType = new QComboBox;
    chkSampleCode = new QCheckBox("样本编号:");     //样本编号（条码）
    editSampleCode = new  QLineEdit;
    btnQuery = new QPushButton("查 询");         //查询
    grbQueryTerms = new QGroupBox("查询条件");
    lytQueryTerms = new QGridLayout;
    lytQueryTerms->setMargin(3);
    lytQueryTerms->setSpacing(10);
    lytQueryTerms->addWidget(labStatus,      0,  0, 2, 5, Qt::AlignRight);
    lytQueryTerms->addWidget(cboStauts,      0,  5, 2, 5, Qt::AlignLeft);
    lytQueryTerms->addWidget(chk_isDateTime, 0, 10, 2, 5, Qt::AlignRight);
    lytQueryTerms->addWidget(dateDateTime,   0, 15, 2, 5, Qt::AlignLeft);
    lytQueryTerms->addWidget(chk_isTestTime, 0, 20, 2, 5, Qt::AlignRight);
    lytQueryTerms->addWidget(dateTestTime,   0, 25, 2, 5, Qt::AlignLeft);
    //lytQueryTerms->addWidget(labSampleType,  0, 30, 2, 5, Qt::AlignRight);
    //lytQueryTerms->addWidget(cboSampleType,  0, 35, 2, 5, Qt::AlignLeft);
    lytQueryTerms->addWidget(chkSampleCode,  0, 40, 2, 5, Qt::AlignRight);
    lytQueryTerms->addWidget(editSampleCode,  0, 45, 2, 5, Qt::AlignLeft);
    lytQueryTerms->addWidget(btnQuery,       0, 50, 2, 10, Qt::AlignCenter);
    grbQueryTerms->setLayout(lytQueryTerms);
}

//样本编号
void SampleWidget::initSampleCode()
{
    //样本列表
    wdgSampleCode = new QWidget;
    btnSampleCodeUpdate = new QPushButton("修改");       //修改样本条码
    btnSampleCodeFormer = new QPushButton("上一页");     //
    btnSampleCodeNext = new QPushButton("下一页");       //
    lytSampleCode = new QGridLayout;
    lytSampleCode->setMargin(0);
    lytSampleCode->setSpacing(1);
    lytSampleCode->addWidget(tbvSampleList,         0,  0, 100, 30);
    lytSampleCode->addWidget(btnSampleCodeUpdate, 100,  0,   5, 10, Qt::AlignCenter);
    lytSampleCode->addWidget(btnSampleCodeFormer, 100, 10,   5, 10, Qt::AlignRight);
    lytSampleCode->addWidget(btnSampleCodeNext,   100, 20,   5, 10, Qt::AlignLeft);
    wdgSampleCode->setLayout(lytSampleCode);
}

//测试结果查询
void SampleWidget::initResultList()
{  
    wdgMenuButton = new QWidget;
    btnAllNull = new QPushButton("全选/反选"); //全选/反选
    btnFormer = new QPushButton("上一页");     //上一页
    btnNext = new QPushButton("下一页");       //下一页
    btnReTest = new QPushButton("重测");       //重测
    btnReCalculate = new QPushButton("重算");  //重算
    btnPrint = new QPushButton("打印");        //打印
    btnCurve = new QPushButton("反应曲线");    //反应曲线
    btnDelete = new QPushButton("删除");
    btnLIS = new QPushButton("LIS");
    btnAlter = new QPushButton("");
    lytMenuButton = new QGridLayout;
    lytMenuButton->setMargin(0);
    lytMenuButton->setSpacing(10);
    lytMenuButton->addWidget(btnAllNull,       0,  10, 5, 10, Qt::AlignCenter);
    lytMenuButton->addWidget(btnFormer,        0,  20, 5, 10, Qt::AlignCenter);
    lytMenuButton->addWidget(btnNext,          0,  30, 5, 10, Qt::AlignCenter);
    lytMenuButton->addWidget(btnReTest,        0,  40, 5, 10, Qt::AlignCenter);
    lytMenuButton->addWidget(btnReCalculate,   0,  50, 5, 10, Qt::AlignCenter);
    lytMenuButton->addWidget(btnPrint,         0,  60, 5, 10, Qt::AlignCenter);
    lytMenuButton->addWidget(btnCurve,         0,  70, 5, 10, Qt::AlignCenter);
    lytMenuButton->addWidget(btnDelete,        0,  80, 5, 10, Qt::AlignCenter);
    lytMenuButton->addWidget(btnLIS,           0,  90, 5, 10, Qt::AlignCenter);

    lytMenuButton->addWidget(btnAlter,         0,  120, 5, 10, Qt::AlignRight);
    wdgMenuButton->setLayout(lytMenuButton);

    wdgResultList = new QWidget;
    lytResultList = new QGridLayout;
    lytResultList->setMargin(0);
    lytResultList->setSpacing(5);
    lytResultList->addWidget(tbvList,         0, 0, 100, 100);
    lytResultList->addWidget(wdgMenuButton, 100, 0,   5, 100, Qt::AlignCenter);
    wdgResultList->setLayout(lytResultList);
}

//界面风格
void SampleWidget::initStyle()
{
    //
    btnQuery->setObjectName("button6");
    btnSampleCodeUpdate->setObjectName("button6");
    btnSampleCodeFormer->setObjectName("button6");
    btnSampleCodeNext->setObjectName("button6");
    btnAllNull->setObjectName("button6");
    btnFormer->setObjectName("button6");         //
    btnNext->setObjectName("button6");           //
    btnReTest->setObjectName("button6");         //重测
    btnPrint->setObjectName("button6");          //打印
    btnCurve->setObjectName("button6");          //反应曲线
    btnReCalculate->setObjectName("button6");    //重算
    btnLIS->setObjectName("button6");
    btnDelete->setObjectName("button6");

    QString styleTable =                //列表样式
            "QTableView::indicator{width:25px; height:25px;}"
            "QTableView::item:selected{background: rgb(130, 180, 100);}"
            "QTableView::indicator:checked{image:url(://resource/resource/tableView/checked.png);}"
            "QTableView::indicator:unchecked{image:url(://resource/resource/tableView/unChecked.png);}";
    tbvList->setStyleSheet(styleTable);
    tbvSampleList->setStyleSheet(styleTable);

    chk_isDateTime->setChecked(false);
    chk_isTestTime->setChecked(true);
    btnAlter->setStyleSheet("QPushButton{width:60; height:30; border-radius:8px; background-color:rgba(50,50,50,5);}");
}

//连接信号与槽
void SampleWidget::initConnect()
{
    connect(btnSampleCodeUpdate, SIGNAL(clicked()), this, SLOT(slt_btnSampleCodeUpdate_Clicked()));
    connect(btnSampleCodeFormer, SIGNAL(clicked()), this, SLOT(slt_btnSampleCodeFormer_Clicked()));
    connect(btnSampleCodeNext, SIGNAL(clicked()), this, SLOT(slt_btnSampleCodeNext_Clicked()));
    connect(btnQuery, SIGNAL(clicked()), this, SLOT(slt_btnQuery_Clicked()));

    connect(btnAllNull, SIGNAL(clicked()), this, SLOT(slt_btnAllNull_Clicked()));
    connect(btnFormer, SIGNAL(clicked()), this, SLOT(slt_btnFormer_Clicked()));
    connect(btnNext, SIGNAL(clicked()), this, SLOT(slt_btnNext_Clicked()));
    connect(btnReTest, SIGNAL(clicked()), this, SLOT(slt_btnReTest_Clicked()));
    connect(btnCurve, SIGNAL(clicked()), this, SLOT(slt_btnCurve_Clicked()));
    connect(btnPrint, SIGNAL(clicked()), this, SLOT(slt_btnPrint_Clicked()));
    connect(btnDelete, SIGNAL(clicked()), this, SLOT(slt_btnDelete_Clicked()));
    connect(btnAlter, SIGNAL(clicked()), this, SLOT(slt_btnAlter_Clicked()));
    connect(btnLIS, SIGNAL(clicked()), this, SLOT(slt_btnLIS_Clicked()));
    connect(btnReCalculate, SIGNAL(clicked()), this, SLOT(slt_btnReCalculate_Clicked()));
    connect(tbvList, SIGNAL(clicked(QModelIndex)), this, SLOT(slt_tbvListSelectRow(QModelIndex)));
    connect(tbvSampleList, SIGNAL(clicked(QModelIndex)), this, SLOT(slt_tbvSampleListSelectRow(QModelIndex)));

    connect(gScheduler, SIGNAL(sig_AutoUpdateSampleList()), this, SLOT(slt_AutoUpdateSampleList()));

    connect(this, SIGNAL(sig_UpdateApplicatedCount()), gSampleApplication, SLOT(slt_UpdateApplicatedCount()));
}

//格式化表格（样本编号表、结果表）
void SampleWidget::initTableFormat()
{
    tbvList = new QTableView;            //列表
    modelList = new QStandardItemModel;  //列表模型
    tbvList->setModel(modelList);

    int tbvRowCount = ROWCOUNT; //列表最大能显示的行数
    //列表
    QStringList headerList_h;
    headerList_h << "T_id" << "id" << "" << "项目简称" << "样本位置" << "样本类型" << "幅度" << "结果"
                 << "重测否" << "状态" << "申请时间"  << "测试类型" << "急诊否" << "测试编号" << "反应杯1" << "反应杯2"
                 << "" << "state" << "sampleType" << "unit";
    modelList->setHorizontalHeaderLabels( headerList_h );
    tbvList->hideColumn(0);             //T_id-隐藏
    tbvList->hideColumn(1);             //id-隐藏
    tbvList->setColumnWidth( 2, 30);    //""
    tbvList->setColumnWidth( 3, 100);    //
    tbvList->setColumnWidth( 4, 100);    //
    tbvList->setColumnWidth( 5, 100);    //
    tbvList->setColumnWidth( 6, 100);    //
    tbvList->setColumnWidth( 7, 100);    //
    tbvList->setColumnWidth( 8, 100);    //
    tbvList->setColumnWidth( 9, 100);    //
    tbvList->setColumnWidth( 10, 180);    //
    tbvList->setColumnWidth( 11, 80);    //
    tbvList->setColumnWidth( 12, 80);    //
    tbvList->setColumnWidth( 13, 180);    //测试编号
    tbvList->setColumnWidth( 14, 80);    //反应杯2
    tbvList->setColumnWidth( 15, 80);    //反应杯2
#ifdef Q_OS_UNIX //最终Linux版会隐藏（目前方便调试） 20170411
    tbvList->hideColumn(11);             //测试类型-隐藏
    tbvList->hideColumn(13);             //测试编号-隐藏
    tbvList->hideColumn(14);             //反应杯1-隐藏
    tbvList->hideColumn(15);             //反应杯2-隐藏
#endif
    tbvList->hideColumn(17);             //state-隐藏
    tbvList->hideColumn(18);             //sampleType-隐藏
    tbvList->hideColumn(19);             //unit-隐藏
    tbvList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvList->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvList->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvList->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
//    tbvList->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
    tbvList->horizontalHeader()->setStyleSheet("font-size:18px;");         //表头字体样式

    QStandardItem *item = 0;
    //初始化每一个单元格
    for( int i = 0; i < tbvRowCount; i ++ )
    {
        item = new QStandardItem;
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);

        modelList->setItem( i, 0, new QStandardItem( "" ));
        modelList->setItem( i, 1, new QStandardItem( "" ));
        modelList->setItem( i, 2, item);
        modelList->setItem( i, 3, new QStandardItem( "" ));
        modelList->setItem( i, 4, new QStandardItem( "" ));
        modelList->setItem( i, 5, new QStandardItem( "" ));
        modelList->setItem( i, 6, new QStandardItem( "" ));
        modelList->setItem( i, 7, new QStandardItem( "" ));
        modelList->setItem( i, 8, new QStandardItem( "" ));
        modelList->setItem( i, 9, new QStandardItem( "" ));
        modelList->setItem( i, 10, new QStandardItem( "" ));
        modelList->setItem( i, 11, new QStandardItem( "" ));
        modelList->setItem( i, 12, new QStandardItem( "" ));
        modelList->setItem( i, 13, new QStandardItem( "" ));
        modelList->setItem( i, 14, new QStandardItem( "" ));
        modelList->setItem( i, 15, new QStandardItem( "" ));
        modelList->setItem( i, 16, new QStandardItem( "" ));
        modelList->setItem( i, 17, new QStandardItem( "" ));
        modelList->setItem( i, 18, new QStandardItem( "" ));
        modelList->setItem( i, 19, new QStandardItem( "" ));

        //列表内容显示方式
        modelList->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 4)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 5)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 6)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 7)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 8)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 9)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i,10)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i,11)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i,12)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i,13)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i,14)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i,15)->setTextAlignment(Qt::AlignCenter);
    }

    //
    tbvSampleList = new QTableView;            //样本列表
    modelSampleList = new QStandardItemModel;  //样本列表模型
    tbvSampleList->setModel(modelSampleList);
    QStringList list;
    list << "" << "样本编号";
    modelSampleList->setHorizontalHeaderLabels(list);
    //tbvSampleList->setColumnWidth( 0, 30);
    tbvSampleList->hideColumn(0);           //""-隐藏
    tbvSampleList->setColumnWidth( 1, 120);
    tbvSampleList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvSampleList->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvSampleList->setEditTriggers(QAbstractItemView::DoubleClicked);            //设置表格的单元属性
    tbvSampleList->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
//    tbvSampleList->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
    tbvSampleList->horizontalHeader()->setStyleSheet("font-size:18px;");         //表头字体样式

    QStandardItem *item1 = 0;
    //初始化每一个单元格
    for( int i = 0; i < tbvRowCount; i ++ )
    {
        item1 = new QStandardItem;
        item1->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item1->setCheckState(Qt::Unchecked);
        modelSampleList->setItem( i, 0, item1);
        modelSampleList->setItem( i, 1, new QStandardItem( "" ));
        modelSampleList->item(i, 1)->setTextAlignment(Qt::AlignCenter);
    }
}

//初始化数据
void SampleWidget::initData()
{
    QStringList list;

    list.clear();
    list << "所有" << "完成" << "测试中" << "未完成";
    cboStauts->addItems(list);

    list.clear();
    list << "所有" << "血清" << "全血" << "血浆";
    cboSampleType->addItems(list);

    list.clear();
    list.append("所有");
    QString str  = "select name from ProjectT;";
    QSqlQuery query = SQLite::getInstance()->Select(str);
    while(query.next())
        list.append( query.value(0).toString().trimmed() );
    cboProject->addItems(list);

    dateDateTime->setAlignment(Qt::AlignCenter);
    dateDateTime->setDateTime(QDateTime::currentDateTime());
    dateDateTime->setDisplayFormat("yyyy-MM-dd");
    dateDateTime->setStyleSheet("QDateEdit{width:120; height:30; font-size:15px;}");

    dateTestTime->setAlignment(Qt::AlignCenter);
    dateTestTime->setDateTime(QDateTime::currentDateTime());
    dateTestTime->setDisplayFormat("yyyy-MM-dd");
    dateTestTime->setStyleSheet("QDateEdit{width:120; height:30; font-size:15px;}");

    QueryTermsApplicateTime = "";
    isAlter_flag = -1;   //是否修改标志
}

//查询样本编号
int SampleWidget::QuerySampleLisData()
{
    int flag = 0;
    QString str  =
            "select DISTINCT sampleCode from TempHistoryT "
            + QueryTermsSampleCode
            //排序查询：
            //+ " order by applyTime asc , sampleCode asc "
            //排序查询： (解决了样本排序，1，10，2，3，4 的问题)
            +" order by TempHistoryT.applyTime asc, TempHistoryT.no asc, TempHistoryT.samplePos asc "
            +" limit " + QString::number(CurrentSampleCode_Count) +", " +QString::number(ROWCOUNT)
            +";";
    int i = 0;
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    while(query.next())
    {
        modelSampleList->item(i, 0)->setCheckState(Qt::Unchecked);
        modelSampleList->item(i, 0)->setText(query.value(0).toString());
        modelSampleList->item(i, 1)->setText(query.value(0).toString());
        flag = 1;
        i++;
    }

    if( i > 0)
    {
        //滚动条置顶
        tbvSampleList->scrollToTop();
        isAlter_flag = -1;
        //设置表格的单元属性
        tbvSampleList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        btnSampleCodeUpdate->setText("修改");
    }

    //清空数据
    for( i; i < ROWCOUNT; i ++ )
    {
        modelSampleList->item(i, 0)->setCheckState(Qt::Unchecked);
        modelSampleList->item(i, 0)->setText("");
        modelSampleList->item(i, 1)->setText("");  //
    }

    return flag;
}

//查询数据
int SampleWidget::QueryData()
{
    int flag = 0;
    QString str  =
            //
            " select TempHistoryT.T_id, TempHistoryT.id, StateT.name, TempHistoryT.no, TempHistoryT.applyTime, ProjectT.name, TempHistoryT.isEmergency,"
            " TempHistoryT.samplePos, TempHistoryT.Cup0, TempHistoryT.Cup1, "
            " TempHistoryT.typeID, SampleTypeT.name, TempHistoryT.isRetest, TempHistoryT.sampleCode, amplitude, result,"
            " TempHistoryT.statusID, TempHistoryT.sampleTypeID, ProjectT.unit, "
            //计算项目相关
            " isCalculateProj, nameEn, CalculateProjectT.unit "
            " from TempHistoryT "
            //连接方式(表、项目表)
            " Inner join ProjectT ON TempHistoryT.projectID = ProjectT.id "
            //连接方式(表、计算项目表)
            " Inner join CalculateProjectT ON TempHistoryT.calculateProjID = CalculateProjectT.id "
            //连接方式(表、样本类型表)
            " Inner join SampleTypeT ON TempHistoryT.sampleTypeID = SampleTypeT.id "
            //连接方式(表、状态表表)
            " Inner join StateT ON TempHistoryT.statusID = StateT.id "
            //查询条件
            + QueryTermsResultT
            //排序查询：紧急否降序
            + " order by TempHistoryT.isEmergency desc "
            + " limit " + QString::number(CurrentResultT_Count) +", " +QString::number(ROWCOUNT)
            +";";
    //qDebug() << str;
    double result;
    int i = 0;
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    while(query.next())
    {
        //qDebug() << "**" << i;
        modelList->item(i, 0)->setText(query.value(0).toString());  //id(隐藏)
        modelList->item(i, 1)->setText(query.value(1).toString());  //h_id-隐藏
        modelList->item(i, 2)->setCheckState(Qt::Checked);

        modelList->item(i, 9)->setText(query.value(2).toString());  //状态
        modelList->item(i, 13)->setText(query.value(3).toString());  //测试编号
        modelList->item(i, 10)->setText(query.value(4).toString());  //申请时间
        modelList->item(i, 3)->setText(query.value(5).toString());  //项目简称
        if(query.value(6).toInt())
            modelList->item(i, 12)->setText("是");  //急诊否
        else
            modelList->item(i, 12)->setText("否");  //急诊否
        modelList->item(i, 4)->setText(query.value(7).toString());  //样本位置

        modelList->item(i, 14)->setText(query.value(8).toString());  //反应杯1
        modelList->item(i, 15)->setText(query.value(9).toString());  //反应杯2

        modelList->item(i, 11)->setText(query.value(10).toString());  //测试类型
        modelList->item(i, 5)->setText(query.value(11).toString()); //样本类型
        if(query.value(12).toInt())
            modelList->item(i, 8)->setText("是");  //重测否
        else
            modelList->item(i, 8)->setText("否");  //重测否

        //幅度
        modelList->item(i, 6)->setText(query.value(14).toString());
        //结果
        result = query.value(15).toDouble();
        if(result == 0)
            modelList->item(i, 7)->setText("");
        else
            modelList->item(i, 7)->setText(QString::number(result, 'g', 6));

        modelList->item(i, 17)->setText(query.value(16).toString()); //state
        modelList->item(i, 18)->setText(query.value(17).toString()); //sampleType
        modelList->item(i, 19)->setText(query.value(18).toString()); //unit

        if(query.value(19).toInt())//是否为计算项目
        {
            modelList->item(i, 3)->setText(query.value(20).toString());  //项目简称
            modelList->item(i, 19)->setText(query.value(21).toString()); //unit
        }
        //qDebug() << query.value(8) << query.value(9);
        i++;
        flag = 1;
    }

    //滚动条置顶
    tbvList->scrollToTop();

    //清空数据
    for( i; i < ROWCOUNT; i ++ )
    {
        modelList->item(i, 0)->setText("");  //
        modelList->item(i, 1)->setText("");  //
        modelList->item(i, 2)->setCheckState(Qt::Unchecked);
        modelList->item(i, 3)->setText("");  //
        modelList->item(i, 4)->setText("");  //
        modelList->item(i, 5)->setText("");
        modelList->item(i, 6)->setText("");  //
        modelList->item(i, 7)->setText("");  //
        modelList->item(i, 8)->setText("");  //
        modelList->item(i, 9)->setText("");  //
        modelList->item(i, 10)->setText("");  //
        modelList->item(i, 11)->setText("");  //
        modelList->item(i, 12)->setText("");  //
        modelList->item(i, 13)->setText("");  //
        modelList->item(i, 14)->setText("");  //
        modelList->item(i, 15)->setText("");  //
        modelList->item(i, 16)->setText("");  //
        modelList->item(i, 17)->setText("");  //
        modelList->item(i, 18)->setText("");  //
    }

    isAllNull_Flag = 1;
    return flag;
}

//重新计算幅度和结果
QString SampleWidget::ReCalculate(int projectID, int h_id)
{
    //项目参数
    ProjectParamS pParam = gParameter->getProjectParam(projectID);

    //反应幅度
    double AD;
    int sampleType;
    QString str =
            "select AD_A" + QString::number(pParam.Param_lightPos1)+", "
            +" AD_A" + QString::number(pParam.Param_lightPos2)+" "
            +", sampleTypeID from TempHistoryT where id = " + QString::number(h_id) + ";";
    QSqlQuery query = SQLite::getInstance()->Select(str);
    while(query.next())          //返回总行数
    {
        AD = query.value(1).toDouble() - query.value(0).toDouble(); //反应幅度
        sampleType = query.value(2).toInt();                        //样本类型
    }

    if(AD < 0) AD = 0;

    //logic5P参数
    long double R0, K, a, b, c;
    double C;
    //用第1段参数曲线计算(默认)
    R0 = pParam.Param_R0;
    K = pParam.Param_K;
    a = pParam.Param_a;
    b  = pParam.Param_b;
    c = pParam.Param_c;
    C = pParam.Param_C;

    //用第2段参数曲线计算
    //qDebug() << pParam.Param_R << AD;
    if( AD > pParam.Param_R)
    {
        if(pParam.isLogic5P2)
        {
            R0 = pParam.Param2_R0;
            K = pParam.Param2_K;
            a = pParam.Param2_a;
            b = pParam.Param2_b;
            c = pParam.Param2_c;
            C = pParam.Param2_C;
        }
    }

    //
    double bloodFactor = pParam.Param_bloodFactor; //全血因子
    if( bloodFactor <= 0 ) bloodFactor = 1;
    if( sampleType != 1 ) bloodFactor = 1; //sampleType=1，时表示样本类型为全血

    //计算浓度
    //查看用哪种定标方式计算(目前都用LOGIC5P)
    MV_Logic5P logic5P;
    double result = logic5P.Consistence_Logic5P(0, C, R0, K, a, b, c, AD) * pParam.Param_factorK / bloodFactor;
    //qDebug() << C << bloodFactor << AD << result;

    //项目线性下限
    if( result < pParam.Param_lowLinear)
        result = pParam.Param_lowLinear;

    QString temp =
            "update TempHistoryT set result = " + QString::number(result, 'f', pParam.Param_accuracy)
            +" , amplitude = " + QString::number(AD)
            +" " + " where ID = " + QString::number(h_id) + ";";
    return temp;
}

//槽：查询样本编号（条码）（样本列表）
void SampleWidget::slt_btnQuery_Clicked()
{
    QString terms;

    //状态
    int statusIndex = cboStauts->currentIndex();
    if( statusIndex == 0 ) //所有
        terms += "";
    else if( statusIndex == 1 ) //完成
        terms += " TempHistoryT.statusID = " + QString::number(State_FINISHED) + " ";
    else if( statusIndex == 2 ) //测试中
        terms += " TempHistoryT.statusID = " + QString::number(State_TESTING) + " ";
    else if( statusIndex == 3 ) //未完成
        terms += " TempHistoryT.statusID = " + QString::number(State_unFINISHED) + " ";

    //申请时间
    if(chk_isDateTime->isChecked())
    {
        QueryTermsApplicateTime = "";
        if(statusIndex > 0)
            terms += " and ";
        QString date = dateDateTime->text();
        QueryTermsApplicateTime = " TempHistoryT.applyTime like '%" + date + "%' ";
        terms += QueryTermsApplicateTime;
    }
    else
    {
        QueryTermsApplicateTime = "";
    }

    //测试时间
    if(chk_isTestTime->isChecked())
    {
        QueryTermsTestTime = "";
        if(terms.count() > 0)
            terms += " and ";
        QString date = dateTestTime->text();
        terms += QueryTermsTestTime = " TempHistoryT.testDate like '%" + date + "%' ";
    }
    else
    {
        QueryTermsTestTime = "";
    }


    //样本编号
    if(chkSampleCode->isChecked())
    {
        if(terms.count() > 0)
            terms += " and ";
        terms = " where " + terms + " TempHistoryT.sampleCode='" + editSampleCode->text().trimmed() +"' ";
    }

    if(terms.count() > 0)
        terms = " and " + terms;

    QueryTermsSampleCode = " where TempHistoryT.typeID=" + QString::number(MV_State_Normal) + terms;
    CurrentSampleCode_Count = 0;
    //qDebug() << "QueryTermsSampleCode:" << QueryTermsSampleCode;
    QuerySampleLisData();
}

//槽：修改样本编号
void SampleWidget::slt_btnSampleCodeUpdate_Clicked()
{
    if(isAlter_flag != 1)
    {
        isAlter_flag = 1;
        btnSampleCodeUpdate->setText("保存");
        tbvSampleList->setEditTriggers(QAbstractItemView::DoubleClicked); //设置表格的单元属性
    }
    else if (isAlter_flag ==1)
    {
        isAlter_flag = 0;
        btnSampleCodeUpdate->setText("修改");
        tbvSampleList->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置表格的单元属性

        QString str;
        QStringList list;
        list.clear();
        QString code1, code2;
        for(int i = 0; i < modelSampleList->rowCount(); i ++)
        {
            code1 = modelSampleList->item(i, 0)->text().trimmed();
            code2 = modelSampleList->item(i, 1)->text().trimmed();
            if( code1 !=  code2 )
            {
                str = "update TempHistoryT set sampleCode ='"+ code2 +"' "
                        +" where sampleCode='"+ code1 +"';";
                list.append(str);
            }
        }
        if(list.count() > 0)
            SQLite::getInstance()->Transaction(list);
    }
}

//槽：上一页（样本列表）
void SampleWidget::slt_btnSampleCodeFormer_Clicked()
{
    if(CurrentSampleCode_Count > 0)
    {
        CurrentSampleCode_Count -= ROWCOUNT;
        if( ! QuerySampleLisData() )//查询不成功，还原‘查询行数标志’
        {
            CurrentSampleCode_Count += ROWCOUNT;
            QuerySampleLisData();
        }
    }
}

//槽：下一页（样本列表）
void SampleWidget::slt_btnSampleCodeNext_Clicked()
{
    CurrentSampleCode_Count += ROWCOUNT;
    if( ! QuerySampleLisData() )
    {
        CurrentSampleCode_Count -= ROWCOUNT;
        QuerySampleLisData();
    }
}

//槽：全选/反选
void SampleWidget::slt_btnAllNull_Clicked()
{
    for(int i = 0; i < ROWCOUNT; i ++ )
    {
        if(modelList->item( i, 0)->text().toInt() > 0)
        {
            if (isAllNull_Flag)              //全选/反选标志
                modelList->item( i, 2)->setCheckState(Qt::Unchecked);
            else
                modelList->item( i, 2)->setCheckState(Qt::Checked);
        }
    }
    isAllNull_Flag = !isAllNull_Flag;
}

//槽：反应曲线（结果列表）
void SampleWidget::slt_btnCurve_Clicked()
{
    int row = tbvList->currentIndex().row();  //当前行
    if(row == -1) return; //未选中
    int id = modelList->item(row, 1)->text().toInt();  //id列

    ResultCurveWidget::getInstance()->setSelect_id(id);
    int x = ( QApplication::desktop()->width() - 1360 ) / 2;
    int y = ( QApplication::desktop()->height() - 800 ) / 2;
    ResultCurveWidget::getInstance()->setGeometry(x, y, 1360, 800);
    ResultCurveWidget::getInstance()->show();
}

//槽：上一页（结果列表）
void SampleWidget::slt_btnFormer_Clicked()
{
    if(CurrentResultT_Count > 0)
    {
        CurrentResultT_Count -= ROWCOUNT;
        if( ! QueryData() )//查询不成功，还原‘查询行数标志’
        {
            CurrentResultT_Count += ROWCOUNT;
            QueryData();
        }
    }
}

//槽：下一页（结果列表）
void SampleWidget::slt_btnNext_Clicked()
{
    CurrentResultT_Count += ROWCOUNT;
    if( ! QueryData() )
    {
        CurrentResultT_Count -= ROWCOUNT;
        QueryData();
    }
}

//槽：重测（结果列表）
void SampleWidget::slt_btnReTest_Clicked()
{
    bool flag = false;
    QStringList list;
    list.clear();
    int T_id, H_id;
    QString str;
    for(int i = 0; i < modelList->rowCount(); i ++)
    {
        if (modelList->item(i, 2)->checkState())
        {
            flag = true;
            T_id = modelList->item(i, 0)->text().toInt();  //T_id列
            H_id = modelList->item(i, 1)->text().toInt();  //id列

            str ="update TaskT set "
                 " HistoryT_id=0, "
                    //" testTime='', "
                    " isRetest=1, "
                    " stateID=3 "
                    " where id=" + QString::number(T_id) + ";";
            list.append(str);

            str = "delete from TempHistoryT where id = "+ QString::number(H_id) + ";";
            list.append(str);

            //////////////before 2070726
            //            flag = true;
            //            T_id = modelList->item(i, 0)->text().toInt();  //T_id列
            //            H_id = modelList->item(i, 1)->text().toInt();  //id列

            //            if( modelList->item(i, 4)->text().split("-").at(0) == "0" ) //是否急诊(查看样本位置)
            //                isUrgent = "1";
            //            else
            //                isUrgent = "0";

            //            str =
            //                    "insert into TaskT( no, HistoryT_id, sampleCode, dateTime, userID, typeID, isEmergency, stateID, samplePos, projectID, sampleTypeID, isRetest ) "
            //                    " values('"
            //                    + modelList->item(i, 13)->text() + "'," //测试编号
            //                    + " 0, "          //历史结果数据表中的id
            //                    + "'" + CurrentSelectSampleCode + "',"
            //                    + "'" + modelList->item(i, 10)->text() + "', "     //申请日期
            //                    +QString::number( gParameter->getUserID() ) + "," //申请者id
            //                    + " 1,"             //测试类型（1：样本测试，2：质控测试，3：定标测试）（注：在这里申请的都是样本测试）
            //                    + isUrgent + ","    //急诊否
            //                    + " 3,"                                      //”待测试“状态
            //                    + "'"+ modelList->item(i, 4)->text() + "',"  //样本位置
            //                    + QString::number( gParameter->getProject_id( modelList->item(i, 3)->text()) ) + ", " //项目id
            //                    + modelList->item(i, 18)->text() + ", " //样本类型ID
            //                    + "1 "                                  //是否重测(默认否)
            //                    +");";
            //            list.append(str);

            //            str = "delete from TempHistoryT where id = "+ QString::number(H_id) + ";";
            //            list.append(str);

            //            str ="update TaskT set stateID=19 where id=" + QString::number(T_id) + ";";
            //            str = "delete from TaskT where id = "+ QString::number(T_id) + ";";
            //            list.append(str);
        }
    }

    if(list.count() > 0)
    {
        gMessageBox->setInfo(QString("提示"), QString("是否要重新测试选中的测试！"), QPixmap("://resource/resource/MessageBox/information.png"),  true, false);
        gMessageBox->setButtonText(Sure, "确定");
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Accepted)
        {
            SQLite::getInstance()->Transaction(list);
            //清空数据
            for( int i = 0; i < ROWCOUNT; i ++ )
            {
                if(modelList->item(i, 0)->text() == "") continue;

                modelList->item(i, 0)->setText("");  //
                modelList->item(i, 1)->setText("");  //
                modelList->item(i, 2)->setCheckState(Qt::Unchecked);
                modelList->item(i, 3)->setText("");  //
                modelList->item(i, 4)->setText("");  //
                modelList->item(i, 5)->setText("");
                modelList->item(i, 6)->setText("");  //
                modelList->item(i, 7)->setText("");  //
                modelList->item(i, 8)->setText("");  //
                modelList->item(i, 9)->setText("");  //
                modelList->item(i, 10)->setText("");  //
                modelList->item(i, 11)->setText("");  //
                modelList->item(i, 12)->setText("");  //
                modelList->item(i, 13)->setText("");  //
                modelList->item(i, 14)->setText("");  //
                modelList->item(i, 15)->setText("");  //
                modelList->item(i, 16)->setText("");  //
                modelList->item(i, 17)->setText("");  //
                modelList->item(i, 18)->setText("");  //
            }

            emit sig_UpdateApplicatedCount();
        }
    }
}

//槽：打印（结果列表）
void SampleWidget::slt_btnPrint_Clicked()
{
    gMessageBox->setInfo(QString("提示"), QString("'打印'功能尚未开放!!"),
                         QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
    gMessageBox->setButtonText(Cancel, "返回");
    gMessageBox->show();
    return;
//    bool flag = false;
//    QString project, result;
//    QVector<QStringList> vecList;
//    QStringList pList;
//    vecList.clear();
//    for(int i = 0; i < modelList->rowCount(); i ++)
//    {
//        if (modelList->item(i, 2)->checkState())
//        {
//            flag = true;
//            pList.clear();
//            project = modelList->item(i, 3)->text();  //项目名称
//            result = modelList->item(i, 7)->text()+modelList->item(i, 19)->text();   //结果+单位

//            pList.append(project);
//            pList.append(result);
//            vecList.append(pList);
//        }
//    }
//    gPrinterThread->Write(CurrentSelectSampleCode, vecList);
}

//槽：重算（结果列表）
void SampleWidget::slt_btnReCalculate_Clicked()
{
    QStringList list;
    list.clear();
    bool flag = false;
    for(int i = 0; i < modelList->rowCount(); i ++)
    {
        if (modelList->item(i, 2)->checkState())
        {
            if(modelList->item(i, 17)->text().toInt() == 11)
            {
                int p_id = gParameter->getProject_id(modelList->item(i, 3)->text());
                list.append( ReCalculate(p_id, modelList->item(i, 1)->text().toInt() ));
                flag = true;
            }
        }
    }
    SQLite::getInstance()->Transaction(list);

    if(flag)
        QueryData();
}

//槽：删除
void SampleWidget::slt_btnDelete_Clicked()
{
    QString str;
    QStringList list;
    list.clear();
    for(int i = 0; i < modelList->rowCount(); i ++)
    {
        if (modelList->item(i, 2)->checkState())
        {
            str = "delete from TempHistoryT where id=" + modelList->item(i, 1)->text() + ";";
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
            QuerySampleLisData();
            QueryData();
        }
    }
}

//槽：LIS
void SampleWidget::slt_btnLIS_Clicked()
{
//    gMessageBox->setInfo(QString("提示"), QString("'上传LIS'功能尚未开放!!"),
//                         QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
//    gMessageBox->setButtonText(Cancel, "返回");
//    gMessageBox->show();
//    return;

    if( gLISCommunicate->ConnectState() ) return;
    QString ip = gParameter->get_LIS_IP().trimmed();
    int port = gParameter->get_LIS_Port();
    if( !gLISCommunicate->ConnectoHost( ip, port )) return;

    QStringList list;
    list.clear();
    for(int i = 0; i < modelList->rowCount(); i ++)
    {
        if (modelList->item(i, 2)->checkState())
            if( modelList->item(i, 17)->text().toInt() == State_FINISHED) //完成状态
                list.append(modelList->item(i, 1)->text().trimmed());
    }

    if(list.count() > 0)
    {
        qDebug() << "LIS send sampleID:" << list;
        gLISCommunicate->Write(list);
    }
}

//槽：（样本列表）单击列表选择中某行，列表对应行左边复选框对应变化
void SampleWidget::slt_tbvSampleListSelectRow(const QModelIndex &index)
{
    if (index.isValid())
    {
        QString sampleCode = modelSampleList->item( index.row(), 1)->text().trimmed();
        if(sampleCode.count() > 0)
        {
            modelSampleList->item( index.row(), 0)->setCheckState(Qt::Checked);
            CurrentSelectSampleCode = sampleCode;

            QueryTermsResultT = "";

            if(QueryTermsSampleCode.count() < 5)
                QueryTermsResultT = " where TempHistoryT.typeID=" + QString::number(MV_State_Normal) +" ";
            else
                QueryTermsResultT += QueryTermsSampleCode + " and  TempHistoryT.typeID=" + QString::number(MV_State_Normal) +" ";

            QueryTermsResultT += " and sampleCode='" + sampleCode + "'";

            if(QueryTermsApplicateTime.count() > 0)
                QueryTermsResultT += " and " + QueryTermsApplicateTime;


            //测试时间
            QueryTermsTestTime = "";
            if(chk_isTestTime->isChecked())
            {
                QString date = dateTestTime->text();  
                if(QueryTermsResultT.count() > 10)
                    QueryTermsResultT += " and TempHistoryT.testDate like '%" + date + "%' ";
            }
            //qDebug() << "QueryTermsResultT:" << QueryTermsResultT;
            CurrentResultT_Count = 0;
            QueryData();
        }
        else if(sampleCode.count() == 0) //清空
        {
            //清空数据
            for(int i = 0; i < modelList->rowCount(); i ++ )
            {
                modelList->item(i, 0)->setText("");  //
                modelList->item(i, 1)->setText("");  //
                modelList->item(i, 2)->setCheckState(Qt::Unchecked);
                modelList->item(i, 3)->setText("");  //
                modelList->item(i, 4)->setText("");  //
                modelList->item(i, 5)->setText("");
                modelList->item(i, 6)->setText("");  //
                modelList->item(i, 7)->setText("");  //
                modelList->item(i, 8)->setText("");  //
                modelList->item(i, 9)->setText("");  //
                modelList->item(i, 10)->setText("");  //
                modelList->item(i, 11)->setText("");  //
                modelList->item(i, 12)->setText("");  //
                modelList->item(i, 13)->setText("");  //
                modelList->item(i, 14)->setText("");  //
                modelList->item(i, 15)->setText("");  //
                modelList->item(i, 16)->setText("");  //
                modelList->item(i, 17)->setText("");  //
                modelList->item(i, 18)->setText("");  //
            }

        }
    }
}

//槽：（结果列表）击列表选择中某行，列表对应行左边复选框对应变化
void SampleWidget::slt_tbvListSelectRow(const QModelIndex &index)
{
    //    if (index.isValid())
    //    {
    //        if (modelList->item( index.row(), 2)->checkState())
    //            modelList->item( index.row(), 2)->setCheckState(Qt::Unchecked);
    //        else
    //        {
    //            if(modelList->item( index.row(), 0)->text().toInt() > 0)
    //            {
    //                modelList->item( index.row(), 2)->setCheckState(Qt::Checked);
    //            }
    //        }
    //    }
}

//槽：修改反应幅度（隐藏）
void SampleWidget::slt_btnAlter_Clicked()
{
    QList<QStringList> ModifyInfo;
    QStringList info;
    ModifyInfo.clear();

    QString sNo = ""; //样本编号
    QModelIndex index = tbvSampleList->currentIndex();
    if(index.isValid())
        sNo = modelSampleList->item(index.row(), 1)->text().trimmed();
    else
        return;


    //遍历列表记住选择要修改的行ID
    for(int i = 0; i < modelList->rowCount(); i ++)
    {
        if (modelList->item(i, 2)->checkState())
        {
            info.clear();
            info.append(modelList->item(i, 1)->text().trimmed());       //id
            info.append(sNo);       //样本编号
            info.append(modelList->item(i, 3)->text().trimmed());       //项目
            info.append(modelList->item(i, 7)->text().trimmed());      //结果
            info.append(modelList->item(i, 6)->text().trimmed());      //幅度
            info.append(modelList->item(i, 5)->text().trimmed());      //样本类型
            ModifyInfo.append(info);
        }
    }

    //qDebug() << ModifyInfo.count();
    if (ModifyInfo.count() > 0)
    {
        modifyWidget->setList(ModifyInfo);
        modifyWidget->show();
    }
}

//修改返回的结果
void SampleWidget::slt_ModifyResult()
{
    QueryData();
}

//槽：自动更新样本列表
void SampleWidget::slt_AutoUpdateSampleList()
{
    QuerySampleLisData(); //查询样本编号
}

