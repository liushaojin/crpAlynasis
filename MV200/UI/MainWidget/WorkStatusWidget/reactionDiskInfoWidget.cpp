#include "reactionDiskInfoWidget.h"

ReactionDiskInfoWidget::ReactionDiskInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();          //初始化
    initStyle();         //界面风格
    initTableFormat();   //格式化表格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}

//初始化
void ReactionDiskInfoWidget::initForm()
{
    reactionDisk = new ReactionDiskWidget;
    tbvList = new QTableView;            //列表
    modelList = new QStandardItemModel;  //列表模型
    tbvList->setModel(modelList);
    btnCurve = new QPushButton("反应曲线");          //查看反应曲线

    //总布局
    layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(reactionDisk,  0,  0, 100, 60);
    //layout->addWidget(btnCurve,     95, 50,   5, 10);
    //layout->addWidget(tbvList,       0, 60, 100, 40);
    //layout->addWidget(btnCurve,    100, 90,   2, 10);
    this->setLayout(layout);
}

//格式化表格
void ReactionDiskInfoWidget::initTableFormat()
{
    int tbvRowCount = ROWCOUNT; //列表最大能显示的行数
    //列表
    QStringList headerList_h;
    headerList_h << "" << "h_id" << "测试编号"  << "项目" << "状态" << "试管编号"
                 << "申请时间" << "测试时间" << "样本类型" << "急诊否"<< "样本位" << "重测否";
    modelList->setHorizontalHeaderLabels( headerList_h );
    tbvList->setColumnWidth( 0, 30);     //
    tbvList->hideColumn(1);              //h_id-隐藏
    //tbvList->setColumnWidth( 2, 130);  //测试编号
    tbvList->hideColumn(2);              //测试编号
    tbvList->setColumnWidth( 3, 60);     //
    tbvList->setColumnWidth( 4, 60);     //
    tbvList->setColumnWidth( 5, 140);    //
    tbvList->setColumnWidth( 6, 130);    //
    tbvList->setColumnWidth( 7, 130);    //
    tbvList->hideColumn(5);
    tbvList->hideColumn(6);
    tbvList->hideColumn(7);
    tbvList->setColumnWidth( 8, 70);     //
    tbvList->setColumnWidth( 9, 80);     //
    tbvList->setColumnWidth( 10, 80);    //
    tbvList->setColumnWidth( 11, 50);    //

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
        modelList->setItem( i, 0, new QStandardItem( "" ));  //
        modelList->setItem( i, 1, new QStandardItem( "" ));  //
        modelList->setItem( i, 2, new QStandardItem( "" ));  //
        modelList->setItem( i, 3, new QStandardItem( "" ));  //
        modelList->setItem( i, 4, new QStandardItem( "" ));  //
        modelList->setItem( i, 5, new QStandardItem( "" ));  //
        modelList->setItem( i, 6, new QStandardItem( "" ));  //
        modelList->setItem( i, 7, new QStandardItem( "" ));  //
        modelList->setItem( i, 8, new QStandardItem( "" ));  //
        modelList->setItem( i, 9, new QStandardItem( "" ));  //
        modelList->setItem( i, 10, new QStandardItem( "" ));  //
        modelList->setItem( i, 11, new QStandardItem( "" ));  //

        //列表内容显示方式
        modelList->item(i, 1)->setTextAlignment(Qt::AlignCenter); //居中
        modelList->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 4)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 5)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 6)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 7)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 8)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 9)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 10)->setTextAlignment(Qt::AlignCenter);
        modelList->item(i, 11)->setTextAlignment(Qt::AlignCenter);
    }
}

//界面风格
void ReactionDiskInfoWidget::initStyle()
{
    btnCurve->setObjectName("button6");
    tbvList->setObjectName("tableV");
}

//连接信号与槽
void ReactionDiskInfoWidget::initConnect()
{
    connect(gScheduler, SIGNAL(sig_setCupStatus(int,QString)), this, SLOT(slt_setCupStatus(int,QString)));
    connect(reactionDisk, SIGNAL(sig_CurrentText(QString)), this, SLOT(slt_CurrentItemText(QString)));
    connect(btnCurve, SIGNAL(clicked(bool)), this, SLOT(slt_btnCurve_Clicked()));
    connect(tbvList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slt_SelectTbv_doubleClicked(QModelIndex)));
}

//初始化数据
void ReactionDiskInfoWidget::initData()
{
}

//设置反应杯的状态
void ReactionDiskInfoWidget::setCupStatus(int CupNo, QString objName)
{
    //qDebug() << CupNo << objName << "*****";
    reactionDisk->setCupStatus(CupNo, objName);
}

//槽：设置反应杯的状态
void ReactionDiskInfoWidget::slt_setCupStatus(int CupNo, QString objName)
{
    setCupStatus(CupNo, objName);
}

//槽：当前选中item的文本
void ReactionDiskInfoWidget::slt_CurrentItemText(QString text)
{
    QString str  =
            //任务id，h_id，
            " select TempHistoryT.id, TempHistoryT.no, ProjectT.name, StateT.name, TempHistoryT.sampleCode, "
            " TempHistoryT.applyTime, TempHistoryT.testDate, SampleTypeT.name, TempHistoryT.isEmergency, "
            " TempHistoryT.SamplePos, TempHistoryT.isRetest, StateT.id from TempHistoryT "

            //连接方式(结果临时表、项目表)
            " Inner join ProjectT ON TempHistoryT.projectID = ProjectT.id "
            //连接方式(结果临时表、)
            " Inner join StateT ON TempHistoryT.statusID = StateT.id "
            //连接方式(结果临时表、)
            " Inner join SampleTypeT ON TempHistoryT.sampleTypeID = SampleTypeT.id "
            " where "
            //当前选中反应杯
            " (Cup0=" +text + " or Cup1=" + text+") "
            //查询条件：测试中&完成
             " and StateT.id = 4 "
            //排序查询:
            " order by TempHistoryT.id asc, TempHistoryT.no asc "
            ";";
    //qDebug() << str;

    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    int i = 0;
    while(query.next())
    {
        if( i == ROWCOUNT ) break;

        modelList->item(i, 1)->setText(query.value(0).toString());  //h_id-隐藏
        modelList->item(i, 2)->setText(query.value(1).toString());  //测试编号
        modelList->item(i, 3)->setText(query.value(2).toString());  //项目名称
        modelList->item(i, 4)->setText(query.value(3).toString());  //状态
        modelList->item(i, 5)->setText(query.value(4).toString());  //试管编号
        modelList->item(i, 6)->setText(query.value(5).toString());  //申请时间
        modelList->item(i, 7)->setText(query.value(6).toString());  //测试时间
        modelList->item(i, 8)->setText(query.value(7).toString());  //样本类型
        if( query.value(8).toInt() )  //急诊否
            modelList->item(i, 9)->setText("Y");
        else
            modelList->item(i, 9)->setText("N");
        modelList->item(i, 10)->setText(query.value(9).toString());  //样本位
        if( query.value(10).toInt() ) //重测否
            modelList->item(i, 11)->setText("Y");
        else
            modelList->item(i, 11)->setText("N");
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

//槽：查看反应曲线
void ReactionDiskInfoWidget::slt_btnCurve_Clicked()
{
    int row = tbvList->currentIndex().row();  //当前行
    if(row == -1) return; //未选中
    int id = modelList->item(row, 1)->text().toInt();  //h_id列
    if(id == 0) return;   //未开始测试

    ResultCurveWidget::getInstance()->setSelect_id(id);
    ResultCurveWidget::getInstance()->show();
}

//槽：表格双击事件 (shit 这双击事件不起作用  待研究   ??? why)
void ReactionDiskInfoWidget::slt_SelectTbv_doubleClicked(QModelIndex index)
{
    int row = index.row();  //当前行
    if(row == -1) return; //未选中

    int id = modelList->item(row, 1)->text().toInt();  //h_id列
    if(id == 0) return;   //未开始测试

    if (index.isValid())
    {
        ResultCurveWidget::getInstance()->setSelect_id(id);
        ResultCurveWidget::getInstance()->show();
    }
}













