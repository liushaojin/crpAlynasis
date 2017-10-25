#include "ApplicationTaskListWidget.h"
#include <QFile>
#include <qdebug.h>
#include "UI/MainWidget/ApplicationWidget/SampleApplication.h"

ApplicationTaskListWidget::ApplicationTaskListWidget(QDialog *parent)
    : QDialog(parent)
{
    Count = 0;

    resultCurveWidget = NULL;
    initForm();              //初始化
    initTableFormat();       //格式化表格
    initStyle();             //界面风格
    initConnect();           //连接信号与槽
    initData();              //初始化数据
}

//初始化
void ApplicationTaskListWidget::initForm()
{
    resultCurveWidget = new ResultCurveWidget(0);
    resultCurveWidget->setModal(true);

    tbvList = new QTableView;            //列表
    modelList = new QStandardItemModel;  //列表模型
    tbvList->setModel(modelList);

    btnAllNone = new QPushButton("全选/反选");
    btnDelete = new QPushButton("删 除");         //删除
    btnFormer = new QPushButton("上一页");        //上一页
    btnNext = new QPushButton("下一页");          //下一页
    btnCurve = new QPushButton("反应曲线");       //查看反应曲线
    btnReTest = new QPushButton("重 测");         //重测
    btnCancel = new QPushButton("返 回");         //返回

    //总布局
    layout = new QGridLayout;
    layout->setMargin(5);
    layout->setSpacing(2);
    //layout->addWidget(btnAllNone, 0,  0,  5, 10, Qt::AlignRight);
    layout->addWidget(btnDelete,  0,  0,  5, 10, Qt::AlignCenter);

    layout->addWidget(btnFormer,  0, 20,  5, 10, Qt::AlignRight);
    layout->addWidget(btnNext,    0, 30,  5, 10, Qt::AlignLeft);
    layout->addWidget(btnCurve,   0, 40,  5, 10);
    layout->addWidget(btnCancel,  0, 60,  5, 10);

    layout->addWidget(tbvList,    5,  0, 95, 70);
    this->setLayout(layout);
}
//格式化表格
void ApplicationTaskListWidget::initTableFormat()
{
    int tbvRowCount = ROWCOUNT; //列表最大能显示的行数
    //列表
    QStringList headerList_h;
    headerList_h << "" << "id" << "h_id" << "样本编号" << "状态" << "测试编号" << "申请时间"
                 << "项目简称" << "急诊否" << "样本位置" << "测试类型" << "样本类型" << "重测否" << "sampleCode" << "stateID";
    modelList->setHorizontalHeaderLabels( headerList_h );
    int row = 0;
    tbvList->hideColumn(row);                //""-隐藏
    tbvList->setColumnWidth( row ++, 30);    //选择列
    tbvList->hideColumn(row ++);             //id-隐藏
    tbvList->hideColumn(row ++);             //h_id-隐藏
    tbvList->setColumnWidth( row ++, 180);   //样本编号
    tbvList->setColumnWidth( row ++, 100);   //状态
    tbvList->setColumnWidth( row ++, 180);   //测试编号
    tbvList->setColumnWidth( row ++, 180);   //申请时间
    tbvList->setColumnWidth( row ++, 100);   //项目简称
    tbvList->setColumnWidth( row ++, 60);    //急诊否
    tbvList->setColumnWidth( row ++, 80);    //样本位置
    tbvList->setColumnWidth( row ++, 80);    //测试类型
    tbvList->setColumnWidth( row ++, 80);    //样本类型
    tbvList->setColumnWidth( row ++, 80);    //重测否
    tbvList->setColumnWidth( row ++, 80);    //幅度
    tbvList->setColumnWidth( row ++, 80);    //结果
    tbvList->hideColumn(10);            //测试类型-隐藏
    tbvList->hideColumn(13);            //-隐藏
    tbvList->hideColumn(14);            //-隐藏

    tbvList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvList->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvList->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvList->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
    tbvList->setSelectionMode(QAbstractItemView::MultiSelection);     //MultiSelection, ExtendedSelection, ContiguousSelection
    tbvList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    //tbvList->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
    tbvList->horizontalHeader()->setStyleSheet("font-size:18px;");         //表头字体样式

    QStandardItem *item = 0;
    //初始化每一个单元格
    for( int i = 0; i < tbvRowCount; i ++ )
    {
        item = new QStandardItem;
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);

        modelList->setItem( i, 0, item);
        modelList->setItem( i, 1, new QStandardItem( "" ));  //id(隐藏)
        modelList->setItem( i, 2, new QStandardItem( "" ));  //h_id-隐藏
        modelList->setItem( i, 3, new QStandardItem( "" ));  //
        modelList->setItem( i, 4, new QStandardItem( "" ));  //
        modelList->setItem( i, 5, new QStandardItem( "" ));  //
        modelList->setItem( i, 6, new QStandardItem( "" ));  //
        modelList->setItem( i, 7, new QStandardItem( "" ));  //
        modelList->setItem( i, 8, new QStandardItem( "" ));  //
        modelList->setItem( i, 9, new QStandardItem( "" ));  //
        modelList->setItem( i, 10, new QStandardItem( "" ));  //
        modelList->setItem( i, 11, new QStandardItem( "" ));  //
        modelList->setItem( i, 12, new QStandardItem( "" ));  //
        modelList->setItem( i, 13, new QStandardItem( "" ));  //
        modelList->setItem( i, 14, new QStandardItem( "" ));  //

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
    }

}
//界面风格
void ApplicationTaskListWidget::initStyle()
{
    //图标路径
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    this->setWindowTitle("选择样本位");
    this->setMinimumWidth(800);
    this->setWindowFlags(this->windowFlags()& ~Qt::WindowMinMaxButtonsHint | Qt::WindowStaysOnTopHint);       //置顶&隐藏最大/最小化

    //设置背景
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap( ":/resource/resource/bg/mainBg.png" )));  //
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    //QSS
    QFile qss( ":/QSS/QSS/mvStyle.qss" );
    qss.open(QFile::ReadOnly);
    setStyleSheet( qss.readAll());
    qss.close();

    btnAllNone->setObjectName("button6");
    btnFormer->setObjectName("button6");        //上一页
    btnNext->setObjectName("button6");          //下一页
    btnCurve->setObjectName("button6");         //查看反应曲线
    btnReTest->setObjectName("button6");        //重测
    btnDelete->setObjectName("button6");        //删除
    btnCancel->setObjectName("button6");        //返回

    QString styleTable =                        //列表样式
            "QTableView::item:selected {background: rgb(130, 180, 100);}"
            "QTableView::indicator {width:25px; height:25px; font-size:20px;}"
            "QTableView::indicator:checked{image:url(://resource/resource/tableView/checked.png);}"
            "QTableView::indicator:unchecked{image:url(://resource/resource/tableView/unChecked.png);}";
    tbvList->setStyleSheet(styleTable);
}
//连接信号与槽
void ApplicationTaskListWidget::initConnect()
{
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(slt_btnCancel_Clicked()));
    connect(btnCurve, SIGNAL(clicked()), this, SLOT(slt_btnCurve_Clicked()));

    connect(btnAllNone, SIGNAL(clicked()), this, SLOT(slt_btnAllNone_Clicked()));
    connect(btnFormer, SIGNAL(clicked()), this, SLOT(slt_btnFormer_Clicked()));
    connect(btnNext, SIGNAL(clicked()), this, SLOT(slt_btnNext_Clicked()));

    connect(btnReTest, SIGNAL(clicked()), this, SLOT(slt_btnReTest_Clicked()));
    connect(btnDelete, SIGNAL(clicked()), this, SLOT(slt_btnDelete_Clicked()));

    connect(gScheduler, SIGNAL(sig_Update_ApplicationTaskList()), this, SLOT(slt_UpdateApplicationTaskList()));
    connect(tbvList, SIGNAL(pressed(QModelIndex)), this, SLOT(slt_tbvList_test(QModelIndex)));
}
//初始化数据
int ApplicationTaskListWidget::initData()
{
    int flag = 0;
    //从数据库中取出用户申请的测试信息，并存入循环队列，等待执行
    QString str  =
            //任务id，h_id，测试编号，申请时间，项目id，测试类型，样本类型，急诊否，样本位置，样本条码，重测否
            "select TaskT.id, TaskT.HistoryT_id, TaskT.sampleCode, StateT.name, TaskT.no, TaskT.dateTime, ProjectT.name, TaskT.isEmergency, TaskT.samplePos,"
            "TaskT.typeID, SampleTypeT.name, TaskT.isRetest, TaskT.sampleCode, TaskT.stateID,"
            " isCalculateProj, nameEn from TaskT "
            //连接方式(任务队列表、项目表)
            " Inner join ProjectT ON TaskT.projectID = ProjectT.id "
            //连接方式(表、计算项目表)
            " Inner join CalculateProjectT ON TaskT.calculateProjID = CalculateProjectT.id "
            //连接方式(任务队列表、样本类型表)
            " Inner join SampleTypeT ON TaskT.sampleTypeID = SampleTypeT.id "
            //连接方式(任务队列表、状态表表)
            " Inner join StateT ON TaskT.stateID = StateT.id "
            //查询条件(不查询完成状态)
            " where TaskT.stateID != " + QString::number( State_FINISHED ) + " and  TaskT.stateID != " + QString::number( State_unFINISHED ) + " "
            " and typeID =" + QString::number( MV_State_Normal ) + " "

            //排序查询：紧急否降序
            " order by TaskT.SamplePosNo asc, ProjectT.orderN asc, TaskT.no asc,  TaskT.stateID desc "
            " limit " + QString::number(Count) +", " + QString::number(ROWCOUNT)
            +";";

    //qDebug() << str;
    //清空数据
    for( int i = 0; i < ROWCOUNT; i ++ )
    {
        modelList->item(i, 0)->setCheckState(Qt::Unchecked);
        modelList->item(i, 1)->setText("");  //id(隐藏)
        modelList->item(i, 2)->setText("");  //h_id-隐藏
        modelList->item(i, 3)->setText("");  //
        modelList->item(i, 4)->setText("");  //
        modelList->item(i, 5)->setText("");  //
        modelList->item(i, 6)->setText("");  //
        modelList->item(i, 7)->setText("");  //
        modelList->item(i, 8)->setText("");  //
        modelList->item(i, 9)->setText("");  //
        modelList->item(i, 10)->setText("");  //
        modelList->item(i, 11)->setText("");  //
        modelList->item(i, 12)->setText("");  //
        modelList->item(i, 13)->setText("");  //
        modelList->item(i, 14)->setText("");  //
    }

    int i = 0;
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    while(query.next())
    {
        modelList->item(i, 1)->setText(query.value(0).toString());  //id(隐藏)
        modelList->item(i, 2)->setText(query.value(1).toString());  //h_id-隐藏
        modelList->item(i, 3)->setText(query.value(2).toString());  //
        modelList->item(i, 4)->setText(query.value(3).toString());
        modelList->item(i, 5)->setText(query.value(4).toString());  //
        modelList->item(i, 6)->setText(query.value(5).toString());  //
        modelList->item(i, 7)->setText(query.value(6).toString());  //

        if(query.value(7).toInt())
            modelList->item(i, 8)->setText("是");  //急诊否
        else
            modelList->item(i, 8)->setText("否");  //急诊否
        modelList->item(i, 9)->setText(query.value(8).toString());  //
        modelList->item(i, 10)->setText(query.value(9).toString());  //
        modelList->item(i, 11)->setText(query.value(10).toString()); //
        if(query.value(11).toInt())
            modelList->item(i, 12)->setText("是");  //
        else
            modelList->item(i, 12)->setText("否");  //

        modelList->item(i, 13)->setText(query.value(12).toString()); //
        modelList->item(i, 14)->setText(query.value(13).toString()); //

        if(query.value(14).toInt())//是否为计算项目
            modelList->item(i, 7)->setText(query.value(15).toString());  //
        i++;
        flag = 1;
        isAllNull_Flag = false;
    }
    return flag;
}

void ApplicationTaskListWidget::slt_tbvList_test(QModelIndex)
{
}

//返回
void ApplicationTaskListWidget::slt_btnCancel_Clicked()
{
    this->hide();
    //qDebug() << this->width() << this->height();
}

//查看反应曲线
void ApplicationTaskListWidget::slt_btnCurve_Clicked()
{
    int row = tbvList->currentIndex().row();  //当前行
    if(row == -1) return; //未选中
    int id = modelList->item(row, 2)->text().toInt();  //h_id列

    if(id > 0)
    {
        resultCurveWidget->setSelect_id(id);
        int x = ( QApplication::desktop()->width() - 1360 ) / 2;
        int y = ( QApplication::desktop()->height() - 800 ) / 2;
        resultCurveWidget->setGeometry(x, y, 1360, 800);
        resultCurveWidget->show();
    }
}

//全选/反选
void ApplicationTaskListWidget::slt_btnAllNone_Clicked()
{
    for(int i = 0; i < ROWCOUNT; i ++ )
    {
        if(modelList->item( i, 1)->text().toInt() > 0)
        {
            if (isAllNull_Flag)              //全选/反选标志
                modelList->item( i, 0)->setCheckState(Qt::Unchecked);
            else
                modelList->item( i, 0)->setCheckState(Qt::Checked);
        }
    }
    isAllNull_Flag = !isAllNull_Flag;
}

//上一页
void ApplicationTaskListWidget::slt_btnFormer_Clicked()
{
    if(Count > 0)
    {
        Count -= ROWCOUNT;
        if( ! initData() )
        {
            //            btnFormer->setEnabled(false);
            Count += ROWCOUNT;
            initData();
            //            if( initData() )
            //                btnNext->setEnabled(true);
        }
        //        else
        //        {
        //            btnFormer->setEnabled(true);
        //            btnNext->setEnabled(true);
        //        }
    }
    //    else
    //    {
    //        btnFormer->setEnabled(false);
    //        btnNext->setEnabled(true);
    //    }
}

//下一页
void ApplicationTaskListWidget::slt_btnNext_Clicked()
{
    Count += ROWCOUNT;
    if( ! initData() )
    {
        //        btnNext->setEnabled(false);
        Count -= ROWCOUNT;
        initData();
        //        if(initData())
        //            btnFormer->setEnabled(true);
    }
    //    else
    //    {
    //        btnFormer->setEnabled(true);
    //        btnNext->setEnabled(true);
    //    }
}

//重测
void ApplicationTaskListWidget::slt_btnReTest_Clicked()
{
    int row = tbvList->currentIndex().row();  //当前行
    if(row == -1) return; //未选中
    int id = modelList->item(row, 1)->text().toInt();  //id列
    int h_id = modelList->item(row, 2)->text().toInt();  //h_id列

    QStringList list;
    if( h_id > 0 )
    {
        QString str = "delete from TempHistoryT where id=" + QString::number(h_id) + ";";
        list.append(str);

        QString s =
                " update TaskT set HistoryT_id= '', "  //空
                " stateID = 3, "                       //”待测试“状态
                " isRetest = 1 "                       //重测标志
                " where id=" +QString::number(id) +";";
        list.append(s);
        SQLite::getInstance()->Transaction(list);
    }
}

//删除
void ApplicationTaskListWidget::slt_btnDelete_Clicked()
{
    //////v1.0 before:20170602 function:只删除当前选择的项
    //    int row = tbvList->currentIndex().row();  //当前行
    //    if(row == -1) return; //未选中
    //    int id = modelList->item(row, 1)->text().toInt();  //id列
    //    if(modelList->item(row, 14)->text().toInt() == State_WAIT)
    //    {
    //        QString str = "delete from TaskT where id=" + QString::number(id) + ";";
    //        SQLite::getInstance()->Delete(str);  //查询数据库
    //        initData();
    //    }

    //////v2.0 date:20170602 function:删除多项
    //    QString str;
    //    QStringList list;
    //    list.clear();
    //    for(int i = 0; i < modelList->rowCount(); i ++)
    //    {
    //        if (modelList->item(i, 0)->checkState()) //选中状态
    //        {
    //            if(modelList->item(i, 14)->text().toInt() == State_WAIT) //等待状态
    //            {
    //                str = "delete from TaskT where id=" + modelList->item(i, 1)->text() + ";";
    //                list.append(str);
    //            }
    //        }
    //    }
    //    SQLite::getInstance()->Transaction(list);
    //    initData();

    //////v3.0 date:20170722 function:删除多项
    QItemSelectionModel *selections =  tbvList->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    QMap <int, int> rowMap;
    int value = 0;
    foreach (QModelIndex index, selected)
        rowMap.insert(index.row(), value++);
    //qDebug() << "print : " <<rowMap;
    //qDebug() << "acount : " <<rowMap.count() ;
    //qDebug() <<"i = 0 : "<<rowMap.keys();

    QString str;
    QStringList list;
    list.clear();
    tbvList->clearSelection();

    QMap<int, int>::iterator it = rowMap.begin();//迭代器
    while(it != rowMap.end())
    {
        if(modelList->item(it.key(), 14)->text().toInt() == State_WAIT) //等待状态
        {
            str = "delete from TaskT where id=" + modelList->item(it.key(), 1)->text() + ";";
            list.append(str);
            //qDebug() << str;
        }
        it ++;
    }
    SQLite::getInstance()->Transaction(list);

    initData();
    emit sig_UpdateApplicatedCount();
}

//更新申请列表
void ApplicationTaskListWidget::slt_UpdateApplicationTaskList()
{
    initData();
}








