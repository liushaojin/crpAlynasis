#include "HistoryResultWidget.h"

HistoryResultWidget::HistoryResultWidget(QWidget *parent) : QWidget(parent)
{
    CurrentResultT_Count = 0;      //查询行数标志
    QueryTermsResultT = "";        //查询条件

    initForm();          //初始化
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();
}

//初始化
void HistoryResultWidget::initForm()
{
    initQueryTermsForm(); //
    initTableFormat();    //格式化表格
    initResultList();

    //总布局
    layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(2);
    layout->addWidget(grbQueryTerms,   0,  0, 5,  80);
    layout->addWidget(wdgResultList,   5, 0, 85, 100);
    this->setLayout(layout);
}

//查询条件
void HistoryResultWidget::initQueryTermsForm()
{
    //查询条件
    labProject = new QLabel("    项目:");            //项目
    cboProject = new QComboBox;
    chk_isDateTime = new QCheckBox("申请日期");
    dateDateTime = new QDateEdit;
    chk_isTestDateTime = new QCheckBox("测试日期:");//是否按测试日期查询
    dateTestDateTime = new QDateEdit;
    labSampleType = new QLabel("样本类型:");      //样本类型
    cboSampleType = new QComboBox;
    chkSampleCode = new QCheckBox("样本编号:");     //样本编号（条码）
    editSampleCode = new  QLineEdit;
    btnQuery = new QPushButton("查 询");         //查询
    labTestCount = new QLabel("测试个数:");         //测试个数
    editTestCount = new QLineEdit;
    grbQueryTerms = new QGroupBox("查询条件");
    lytQueryTerms = new QGridLayout;
    lytQueryTerms->setMargin(3);
    lytQueryTerms->setSpacing(10);
    lytQueryTerms->addWidget(labProject,           0,  0, 2, 5, Qt::AlignRight);
    lytQueryTerms->addWidget(cboProject,           0,  5, 2, 5, Qt::AlignLeft);
    lytQueryTerms->addWidget(new QLabel,           0, 10, 2, 5, Qt::AlignCenter);
    lytQueryTerms->addWidget(chk_isDateTime,       0, 15, 2, 5, Qt::AlignRight);
    lytQueryTerms->addWidget(dateDateTime,         0, 20, 2, 5, Qt::AlignLeft);
    lytQueryTerms->addWidget(new QLabel,           0, 25, 2, 5, Qt::AlignCenter);
    lytQueryTerms->addWidget(chk_isTestDateTime,   0, 30, 2, 5, Qt::AlignRight);
    lytQueryTerms->addWidget(dateTestDateTime,     0, 35, 2, 5, Qt::AlignLeft);
    lytQueryTerms->addWidget(new QLabel,           0, 40, 2, 5, Qt::AlignCenter);
    lytQueryTerms->addWidget(labSampleType,        0, 45, 2, 5, Qt::AlignRight);
    lytQueryTerms->addWidget(cboSampleType,        0, 50, 2, 5, Qt::AlignLeft);
    lytQueryTerms->addWidget(new QLabel,           0, 55, 2, 5, Qt::AlignCenter);
    //lytQueryTerms->addWidget(chkSampleCode,        0, 60, 2, 5, Qt::AlignRight);
    //lytQueryTerms->addWidget(editSampleCode,       0, 65, 2, 5, Qt::AlignLeft);
    lytQueryTerms->addWidget(new QLabel,           0, 70, 2, 5, Qt::AlignCenter);
    lytQueryTerms->addWidget(btnQuery,             0, 75, 2, 10, Qt::AlignCenter);
    lytQueryTerms->addWidget(new QLabel,           0, 80, 2, 5, Qt::AlignCenter);
    lytQueryTerms->addWidget(labTestCount,         0, 85, 2, 5, Qt::AlignRight);
    lytQueryTerms->addWidget(editTestCount,        0, 90, 2, 5, Qt::AlignLeft);
    grbQueryTerms->setLayout(lytQueryTerms);
}

//测试结果查询
void HistoryResultWidget::initResultList()
{
    wdgMenuButton = new QWidget;
    btnAllNull = new QPushButton("全选/反选");        //全选/反选
    btnFormer = new QPushButton("上一页");   //上一页
    btnNext = new QPushButton("下一页");     //下一页
    btnPrint = new QPushButton("打印");          //打印
    btnCurve = new QPushButton("反应曲线");          //反应曲线
    lytMenuButton = new QGridLayout;
    lytMenuButton->setMargin(0);
    lytMenuButton->setSpacing(10);
    lytMenuButton->addWidget(btnAllNull,       0,  10, 5, 10, Qt::AlignCenter);
    lytMenuButton->addWidget(btnFormer,        0,  20, 5, 10, Qt::AlignCenter);
    lytMenuButton->addWidget(btnNext,          0,  30, 5, 10, Qt::AlignCenter);
    lytMenuButton->addWidget(btnPrint,         0,  60, 5, 10, Qt::AlignCenter);
    lytMenuButton->addWidget(btnCurve,         0,  70, 5, 10, Qt::AlignCenter);
    wdgMenuButton->setLayout(lytMenuButton);

    wdgResultList = new QWidget;
    lytResultList = new QGridLayout;
    lytResultList->setMargin(0);
    lytResultList->setSpacing(5);
    lytResultList->addWidget(tbvList,         0, 0, 100, 100);
    lytResultList->addWidget(wdgMenuButton, 100, 0,   5, 100, Qt::AlignLeft);
    wdgResultList->setLayout(lytResultList);
}

//界面风格
void HistoryResultWidget::initStyle()
{
    btnQuery->setObjectName("button6");
    btnAllNull->setObjectName("button6");
    btnFormer->setObjectName("button6");         //
    btnNext->setObjectName("button6");           //
    btnPrint->setObjectName("button6");          //打印
    btnCurve->setObjectName("button6");          //反应曲线

    QString styleTable =                //列表样式
            "QTableView::indicator {width:25px; height:25px;}"
            "QTableView::indicator:checked{image:url(://resource/resource/tableView/checked.png);}"
            "QTableView::indicator:unchecked{image:url(://resource/resource/tableView/unChecked.png);}";
    tbvList->setStyleSheet(styleTable);

    dateDateTime->setDisplayFormat("yyyy-MM-dd");
    dateDateTime->setAlignment(Qt::AlignCenter);
    dateDateTime->setStyleSheet("QDateEdit{width:120; height:30; font-size:15px;}");
    dateTestDateTime->setAlignment(Qt::AlignCenter);
    dateTestDateTime->setDisplayFormat("yyyy-MM-dd");
    dateTestDateTime->setStyleSheet("QDateEdit{width:120; height:30; font-size:15px;}");

    editTestCount->setReadOnly(true);
    editTestCount->setFixedWidth(40);
    editTestCount->setEnabled(false);
}

//连接信号与槽
void HistoryResultWidget::initConnect()
{
    connect(btnQuery, SIGNAL(clicked()), this, SLOT(slt_btnQuery_Clicked()));

    connect(btnAllNull, SIGNAL(clicked()), this, SLOT(slt_btnAllNull_Clicked()));
    connect(btnFormer, SIGNAL(clicked()), this, SLOT(slt_btnFormer_Clicked()));
    connect(btnNext, SIGNAL(clicked()), this, SLOT(slt_btnNext_Clicked()));
    connect(btnCurve, SIGNAL(clicked()), this, SLOT(slt_btnCurve_Clicked()));
    connect(btnPrint, SIGNAL(clicked()), this, SLOT(slt_btnPrint_Clicked()));
    connect(tbvList, SIGNAL(clicked(QModelIndex)), this, SLOT(slt_tbvListSelectRow(QModelIndex)));
}

//格式化表格（样本编号表、结果表）
void HistoryResultWidget::initTableFormat()
{
    tbvList = new QTableView;            //列表
    modelList = new QStandardItemModel;  //列表模型
    tbvList->setModel(modelList);

    int tbvRowCount = ROWCOUNT; //列表最大能显示的行数
    //列表
    QStringList headerList_h;
    headerList_h << "id" << "" << "样本编号"  << "项目简称" << "样本位置" << "样本类型" << "幅度" << "结果"
                 << "重测否" << "状态" << "申请时间"  << "测试时间" << "急诊否" << "测试编号" << "反应杯1" << "反应杯2"
                 << "" << "state" << "sampleType" << "unit";
    modelList->setHorizontalHeaderLabels( headerList_h );
    tbvList->hideColumn(0);             //id-隐藏
    tbvList->setColumnWidth( 1, 30);    //""
    tbvList->setColumnWidth( 2, 180);    //
    tbvList->setColumnWidth( 3, 100);    //
    tbvList->setColumnWidth( 4, 100);    //
    tbvList->setColumnWidth( 5, 100);    //
    tbvList->setColumnWidth( 6, 100);    //
    tbvList->setColumnWidth( 7, 100);    //
    tbvList->setColumnWidth( 8, 100);    //
    tbvList->setColumnWidth( 9, 100);    //
    tbvList->setColumnWidth( 10, 180);    //
    tbvList->setColumnWidth( 11, 180);    //
    tbvList->setColumnWidth( 12, 80);    //
    tbvList->setColumnWidth( 13, 180);    //测试编号
    tbvList->setColumnWidth( 14, 80);    //反应杯2
    tbvList->setColumnWidth( 15, 80);    //反应杯2
#ifdef Q_OS_UNIX //最终Linux版会隐藏（目前方便调试） 20170411
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
    //tbvList->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
    tbvList->horizontalHeader()->setStyleSheet("font-size:18px;");         //表头字体样式

    QStandardItem *item = 0;
    //初始化每一个单元格
    for( int i = 0; i < tbvRowCount; i ++ )
    {
        item = new QStandardItem;
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);

        modelList->setItem( i, 0, new QStandardItem( "" ));
        modelList->setItem( i, 1, item);
        modelList->setItem( i, 2, new QStandardItem( "" ));
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
}

//初始化数据
void HistoryResultWidget::initData()
{
    QStringList list;
    list.clear();
    list << "所有" << "全血" << "血清" << "血浆" << "尿液";
    cboSampleType->addItems(list);

    list.clear();
    list.append("所有");
    QString str  = "select name from ProjectT;";
    QSqlQuery query = SQLite::getInstance()->Select(str);
    while(query.next())
        list.append( query.value(0).toString().trimmed() );
    cboProject->addItems(list);

    dateDateTime->setDateTime(QDateTime::currentDateTime().addDays(-1)); //前一天
    dateTestDateTime->setDateTime(QDateTime::currentDateTime());
}

//查询数据
int HistoryResultWidget::QueryData()
{
    int flag = 0;
    QString str  =
            //
            "select HistoryT.id, StateT.name, HistoryT.no, HistoryT.applyTime, ProjectT.name, HistoryT.isEmergency,"
            " HistoryT.samplePos, HistoryT.Cup0, HistoryT.Cup1, "
            " HistoryT.testDate, SampleTypeT.name, HistoryT.isRetest, HistoryT.sampleCode, amplitude, result,"
            " HistoryT.statusID, HistoryT.sampleTypeID, sampleCode, ProjectT.unit, "
            //计算项目相关
            " isCalculateProj, nameEn, CalculateProjectT.unit "
            " from HistoryT "
            //连接方式(表、项目表)
            " Inner join ProjectT ON HistoryT.projectID = ProjectT.id "
            //连接方式(表、计算项目表)
            " Inner join CalculateProjectT ON HistoryT.calculateProjID = CalculateProjectT.id "
            //连接方式(表、样本类型表)
            " Inner join SampleTypeT ON HistoryT.sampleTypeID = SampleTypeT.id "
            //连接方式(表、状态表表)
            " Inner join StateT ON HistoryT.statusID = StateT.id "
            //查询条件
            + QueryTermsResultT
            //排序查询：紧急否降序
            + " order by HistoryT.no asc "
            + " limit " + QString::number(CurrentResultT_Count) +", " +QString::number(ROWCOUNT)
            + ";";

    //qDebug() << str;
    int i = 0;
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库

    while(query.next())
    {
        modelList->item(i, 0)->setText(query.value(0).toString());  //id(隐藏)
        modelList->item(i, 1)->setCheckState(Qt::Checked);
        modelList->item(i, 2)->setText(query.value(17).toString());  //样本编号

        modelList->item(i, 9)->setText(query.value(1).toString());  //状态
        modelList->item(i, 13)->setText(query.value(2).toString());  //测试编号
        modelList->item(i, 10)->setText(query.value(3).toString());  //申请时间
        if(query.value(5).toInt())
            modelList->item(i, 12)->setText("是");  //急诊否
        else
            modelList->item(i, 12)->setText("否");  //急诊否
        modelList->item(i, 4)->setText(query.value(6).toString());  //样本位置

        modelList->item(i, 14)->setText(query.value(7).toString());  //反应杯1
        modelList->item(i, 15)->setText(query.value(8).toString());  //反应杯2

        modelList->item(i, 11)->setText(query.value(9).toString());  //
        modelList->item(i, 5)->setText(query.value(10).toString()); //样本类型
        if(query.value(11).toInt())
            modelList->item(i, 8)->setText("是");  //重测否
        else
            modelList->item(i, 8)->setText("否");  //重测否

        modelList->item(i, 7)->setText(QString::number(query.value(14).toDouble(), 'g', 6)); //结果
        modelList->item(i, 17)->setText(query.value(15).toString()); //state
        modelList->item(i, 18)->setText(query.value(16).toString()); //sampleType

        if(query.value(19).toInt() == 1) //计算项目相关
        {
            modelList->item(i, 3)->setText(query.value(20).toString());  //项目简称
            modelList->item(i, 6)->setText("-"); //幅度
            modelList->item(i, 19)->setText(query.value(21).toString()); //unit
        }
        else //常规项目
        {
            modelList->item(i, 3)->setText(query.value(4).toString());  //项目简称
            modelList->item(i, 6)->setText(query.value(13).toString()); //幅度
            modelList->item(i, 19)->setText(query.value(18).toString()); //unit
        }

        i++;
        flag = 1;
    }

    //滚动条置顶
    tbvList->scrollToTop();

    //清空数据
    for( i; i < ROWCOUNT; i ++ )
    {
        modelList->item(i, 0)->setText("");  //
        modelList->item(i, 1)->setCheckState(Qt::Unchecked);
        modelList->item(i, 2)->setText("");  //
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
        modelList->item(i, 19)->setText("");  //
    }

    isAllNull_Flag = 1;
    return flag;
}

//槽：查询
void HistoryResultWidget::slt_btnQuery_Clicked()
{
    QString terms;

    //项目
    if( cboProject->currentIndex() == 0)
        terms = "";
    else
        terms += " HistoryT.projectID=" + QString::number(gParameter->getProject_id(cboProject->currentText().trimmed())) + " ";

    //申请时间
    QString termTime = "";
    if(chk_isDateTime->isChecked())
    {
        if(terms.count() > 0)
            terms += " and ";
        QString date = dateDateTime->text();
        termTime += " HistoryT.applyTime like '%" + date + "%' ";
    }
    //测试时间
    if(chk_isTestDateTime->isChecked())
    {
        if(terms.count() > 0 || termTime.count() > 0)
            termTime += " and ";
        QString date = dateTestDateTime->text();
        termTime += " HistoryT.testDate like '%" + date + "%' ";
    }
    terms += termTime;  //加上时间限定

    //样本类型
    int sampleType = cboSampleType->currentIndex();
    if(sampleType > 0)
    {
        if(terms.count() > 0)
            terms += " and ";
        terms += " HistoryT.sampleTypeID=" + QString::number(sampleType) +" ";
    }

    //样本编号
    if(chkSampleCode->isChecked())
    {
        if(terms.count() > 0)
            terms += " and ";
        terms = " where " + terms + " HistoryT.sampleCode='" + editSampleCode->text().trimmed() +"' ";
    }
    else
    {
        if(terms.count() > 0)
            terms = " where " + terms;
    }

    //qDebug() << "terms:" << terms;

    //清空数据
    for(int i = 0; i < ROWCOUNT; i ++ )
    {
        modelList->item(i, 0)->setText("");  //
        modelList->item(i, 1)->setCheckState(Qt::Unchecked);
        modelList->item(i, 2)->setText("");  //
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
        modelList->item(i, 19)->setText("");  //
    }

    QString str = "select COUNT(*) from HistoryT "+ terms;
    QSqlQuery  query = SQLite::getInstance()->Select(str);
    while(query.next())
        editTestCount->setText( query.value(0).toString() );
    QueryTermsResultT = terms;

    CurrentResultT_Count = 0;
    QueryData();
}

//槽：全选/反选
void HistoryResultWidget::slt_btnAllNull_Clicked()
{
    for(int i = 0; i < ROWCOUNT; i ++ )
    {
        if(modelList->item( i, 0)->text().toInt() > 0)
        {
            if (isAllNull_Flag)              //全选/反选标志
                modelList->item( i, 1)->setCheckState(Qt::Unchecked);
            else
                modelList->item( i, 1)->setCheckState(Qt::Checked);
        }
    }
    isAllNull_Flag = !isAllNull_Flag;
}

//槽：反应曲线（计算项目没有曲线是否不要显示 ？？？？？？？？？？？？？）
void HistoryResultWidget::slt_btnCurve_Clicked()
{
    int row = tbvList->currentIndex().row();  //当前行
    if(row == -1) return; //未选中
    int id = modelList->item(row, 0)->text().toInt();  //id列

    ResultCurveWidget::getInstance()->setSelect_History_id(id);
    int x = ( QApplication::desktop()->width() - 1360 ) / 2;
    int y = ( QApplication::desktop()->height() - 800 ) / 2;
    ResultCurveWidget::getInstance()->setGeometry(x, y, 1360, 800);
    ResultCurveWidget::getInstance()->show();
}

//槽：上一页
void HistoryResultWidget::slt_btnFormer_Clicked()
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

//槽：下一页
void HistoryResultWidget::slt_btnNext_Clicked()
{
    CurrentResultT_Count += ROWCOUNT;
    if( ! QueryData() )
    {
        CurrentResultT_Count -= ROWCOUNT;
        QueryData();
    }
}

//槽：打印
void HistoryResultWidget::slt_btnPrint_Clicked()
{
    gMessageBox->setInfo(QString("提示"), QString("'打印'功能尚未开放!!"),
                         QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
    gMessageBox->setButtonText(Cancel, "返回");
    gMessageBox->show();
    return;


    QVector<QString> vecSampleNo;
    QStringList SampleNoList;
    QVector< QVector<QStringList> > vecInfoList;
    SampleNoList.clear();

    QString sampleNo, sampleNo2, project, result;
    QVector<QStringList> vecList;
    vecList.clear();
    QStringList pList;
    for(int i = 0; i < modelList->rowCount(); i ++)
    {
        if (modelList->item(i, 1)->checkState())
        {
            sampleNo = modelList->item(i, 2)->text(); //样本编号

            pList.clear();
            project = modelList->item(i, 3)->text();  //项目名称
            //result = modelList->item(i, 7)->text();   //结果
            result = modelList->item(i, 7)->text()+modelList->item(i, 19)->text();   //结果+单位

            pList.append(project);
            pList.append(result);
            vecList.append(pList);
        }

        //按样本编号打印
        if(i+1 < modelList->rowCount())
        {
            sampleNo2 = modelList->item(i+1, 2)->text();
            if(sampleNo != sampleNo2 && vecList.count() > 0)
            {
                vecSampleNo.append(sampleNo);
                SampleNoList.append(sampleNo);
                vecInfoList.append(vecList);
                vecList.clear();
            }
        }
        else if(i+1 == modelList->rowCount())
        {
            if(vecList.count() > 0)
            {
                vecSampleNo.append(sampleNo);
                SampleNoList.append(sampleNo);
                vecInfoList.append(vecList);
                vecList.clear();
            }
        }
    }

    //if(SampleNoList.count() > 0)
        //gPrinterThread->Write(SampleNoList, vecInfoList);
}

//槽：击列表选择中某行，列表对应行左边复选框对应变化
void HistoryResultWidget::slt_tbvListSelectRow(const QModelIndex &index)
{
//    if (index.isValid())
//    {
//        if (modelList->item( index.row(), 1)->checkState())
//            modelList->item( index.row(), 1)->setCheckState(Qt::Unchecked);
//        else
//        {
//            if(modelList->item( index.row(), 0)->text().toInt() > 0)
//            {
//                modelList->item( index.row(), 1)->setCheckState(Qt::Checked);
//            }
//        }
//    }
}
