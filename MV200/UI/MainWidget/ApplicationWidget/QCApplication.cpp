#include "QCApplication.h"
#include <qdebug.h>
//#include <QApplication>
#include <QDesktopWidget>

QCApplication::QCApplication(QWidget *parent)
    : QWidget(parent)
{
    initForm();          //初始化
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}

//初始化
void QCApplication::initForm()
{
    gQCApplicationSampleDiskInfo = new SampleDiskInfoWidget;
    initQCInfoForm();         //申请测试的信息
    initProjectListForm();    //项目列表
    initQCTableForm();        //初始化质控申请列表

    //总布局
    layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 10, 0, 0);
    layout->addWidget(wdgSampleInfo,        0,  0, 10, 100);
    layout->addWidget(grbProjectList,      10,  0, 90,  60);
    layout->addWidget(wdgQCList,           10, 60, 90,  40);
    this->setLayout(layout);
}

//申请测试的的信息
void QCApplication::initQCInfoForm()
{
    //申请测试的样本的信息
    wdgSampleInfo = new QWidget;
    labSamplePos = new QLabel("样本位:");        //样本位
    editSamplePos = new QLineEdit;
    btnSamplePos = new QPushButton("…");
    labNo = new QLabel("编号:");                 //编号
    editNo = new QLineEdit;
    labQCLiquid = new QLabel("质控液:");         //质控液
    cboQCLiquid = new QComboBox;
    btnApplicate = new QPushButton("申请");        //申请
    btnQCSetting = new QPushButton("质控设置");     //质控设置
    lytSampleInfo = new QGridLayout;
    lytSampleInfo->setMargin(0);
    lytSampleInfo->setSpacing(0);
    lytSampleInfo->addWidget(labSamplePos,  0,  0, 2,  5, Qt::AlignRight);
    lytSampleInfo->addWidget(editSamplePos, 0,  5, 2, 10);
    lytSampleInfo->addWidget(btnSamplePos,  0, 15, 2,  3, Qt::AlignRight);
    lytSampleInfo->addWidget(labNo,         0, 20, 2,  5, Qt::AlignRight);
    lytSampleInfo->addWidget(editNo,        0, 25, 2, 10);
    lytSampleInfo->addWidget(labQCLiquid,   0, 40, 2,  5, Qt::AlignRight);
    lytSampleInfo->addWidget(cboQCLiquid,   0, 45, 2,  8, Qt::AlignLeft);
    lytSampleInfo->addWidget(btnApplicate,     0, 55, 2, 10, Qt::AlignLeft);
    lytSampleInfo->addWidget(btnQCSetting,     0, 85, 2, 10, Qt::AlignCenter);
    wdgSampleInfo->setLayout(lytSampleInfo);
}
//项目列表
void QCApplication::initProjectListForm()
{
    //项目列表
    lytProjectList = new QFlowLayout;
    lytProjectList->setMargin(2);
    lytProjectList->setSpacing(2);
    lytProjectList->setContentsMargins(2, 20, 0, 0);
    for(int i = 0; i < PROJECT_COUNT; i ++)
    {
        rdoRoutineItem[i] = new QRadioButton;     //常规项目
        rdoRoutineItem[i]->setAutoExclusive(false);
        lytProjectList->addWidget(rdoRoutineItem[i]);
    }

    grbProjectList = new QGroupBox("项目选择");
    grbProjectList->setLayout(lytProjectList);
}

//初始化质控申请列表
void QCApplication::initQCTableForm()
{
    wdgQCList = new QWidget;
    tbvQCList = new QTableView;
    modelQCList = new QStandardItemModel(this);
    tbvQCList->setModel(modelQCList);
    btnAllNone = new QPushButton("全选/反选");
    btnFormer = new QPushButton("上一页");
    btnNext = new QPushButton("下一页");
    btnDelete = new QPushButton("删 除");
    lytQCList = new QGridLayout;
    lytQCList->setSpacing(0);
    lytQCList->setSpacing(0);
    lytQCList->addWidget(tbvQCList,    0,  0, 90, 100);
    lytQCList->addWidget(btnAllNone,  90,  0, 10,  10, Qt::AlignCenter | Qt::AlignBottom);
    lytQCList->addWidget(btnFormer,   90, 10, 10,  10, Qt::AlignCenter | Qt::AlignBottom);
    lytQCList->addWidget(btnNext,     90, 20, 10,  10, Qt::AlignCenter | Qt::AlignBottom);
    lytQCList->addWidget(btnDelete,   90, 90, 10,  10, Qt::AlignCenter | Qt::AlignBottom);
    wdgQCList->setLayout(lytQCList);

    int tbvRowCount = ROWCOUNT; //列表最大能显示的行数
    //列表
    QStringList headerList_h;
    headerList_h << "" << "id" << "h_id"  << "状态" << "编号" << "申请时间" << "质控液"<< "项目" << "样本位"  << "sampleCode" << "stateID";
    modelQCList->setHorizontalHeaderLabels( headerList_h );
    int row = 0;
    tbvQCList->setColumnWidth( row ++, 20);    //选择列
    tbvQCList->hideColumn(row ++);             //id-隐藏
    tbvQCList->hideColumn(row ++);             //h_id-隐藏
    tbvQCList->setColumnWidth( row ++, 50);    //状态
    tbvQCList->setColumnWidth( row ++, 100);   //测试编号
    tbvQCList->setColumnWidth( row ++, 100);   //申请时间
    tbvQCList->setColumnWidth( row ++, 80);    //质控液
    tbvQCList->setColumnWidth( row ++, 80);    //项目简称
    tbvQCList->setColumnWidth( row ++, 60);    //样本位置
    tbvQCList->hideColumn(9);            //sampleCode-隐藏
    tbvQCList->hideColumn(10);           //stateID-隐藏

    tbvQCList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvQCList->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvQCList->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvQCList->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
    tbvQCList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    //tbvQCList->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
    tbvQCList->horizontalHeader()->setStyleSheet("font-size:18px;");         //表头字体样式

    QStandardItem *item = 0;
    //初始化每一个单元格
    for( int i = 0; i < tbvRowCount; i ++ )
    {
        item = new QStandardItem;
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);

        modelQCList->setItem( i, 0, item);
        modelQCList->setItem( i, 1, new QStandardItem( "" ));  //id(隐藏)
        modelQCList->setItem( i, 2, new QStandardItem( "" ));  //h_id-隐藏
        modelQCList->setItem( i, 3, new QStandardItem( "" ));  //
        modelQCList->setItem( i, 4, new QStandardItem( "" ));  //
        modelQCList->setItem( i, 5, new QStandardItem( "" ));  //
        modelQCList->setItem( i, 6, new QStandardItem( "" ));  //
        modelQCList->setItem( i, 7, new QStandardItem( "" ));  //
        modelQCList->setItem( i, 8, new QStandardItem( "" ));  //
        modelQCList->setItem( i, 9, new QStandardItem( "" ));  //
        modelQCList->setItem( i, 10, new QStandardItem( "" ));  //


        //列表内容显示方式
        modelQCList->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        modelQCList->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        modelQCList->item(i, 4)->setTextAlignment(Qt::AlignCenter);
        modelQCList->item(i, 5)->setTextAlignment(Qt::AlignCenter);
        modelQCList->item(i, 6)->setTextAlignment(Qt::AlignCenter);
        modelQCList->item(i, 7)->setTextAlignment(Qt::AlignCenter);
        modelQCList->item(i, 8)->setTextAlignment(Qt::AlignCenter);
        modelQCList->item(i, 9)->setTextAlignment(Qt::AlignCenter);
        modelQCList->item(i, 10)->setTextAlignment(Qt::AlignCenter);
    }

}

//界面风格
void QCApplication::initStyle()
{
    cboQCLiquid->setMinimumWidth(100);
    //申请测试的样本的信息
    btnSamplePos->setObjectName("button8");
    btnApplicate->setObjectName("button2");
    btnQCSetting->setObjectName("button6");

    //项目列表
    for(int i = 0; i < PROJECT_COUNT; i ++)
        rdoRoutineItem[i]->setObjectName("rdo");     //常规项目

    btnAllNone->setObjectName("button6");
    btnFormer->setObjectName("button6");
    btnNext->setObjectName("button6");
    btnDelete->setObjectName("button6");
}

//连接信号与槽
void QCApplication::initConnect()
{
    connect(btnSamplePos, SIGNAL(clicked()), this, SLOT(slt_btnSamplePos_Clicked()));
    connect(btnApplicate, SIGNAL(clicked()), this, SLOT(slt_btnApplicate_Clicked()));
    connect(btnQCSetting, SIGNAL(clicked()), this, SLOT(slt_btnQCSetting_Clicked()));

    connect(cboQCLiquid, SIGNAL(currentIndexChanged(int)), this, SLOT(slt_cboQCLiquid_IndexChanged(int)));

    connect(btnAllNone, SIGNAL(clicked()), this, SLOT(slt_btnAllNone_Clicked()));
    connect(btnFormer, SIGNAL(clicked()), this, SLOT(slt_btnFormer_Clicked()));
    connect(btnNext, SIGNAL(clicked()), this, SLOT(slt_btnNext_Clicked()));
    connect(btnDelete, SIGNAL(clicked()), this, SLOT(slt_btnDelete_Clicked()));

    connect(gQCApplicationSampleDiskInfo, SIGNAL(sig_SelectedSampleList(QString,QMap<int, QString>)),
            this, SLOT(slt_SelectedSampleList(QString,QMap<int,QString>)));

    connect(gParameter, SIGNAL(sig_UpdateQcLiquidName()), this, SLOT(slt_cboQCLiquid_UpdateItems()));
    connect(gApplicateThread, SIGNAL(sig_UpdateApplicatedCount()), this, SLOT(slt_tbvQCList_Update()));
}

//初始化数据
void QCApplication::initData()
{
    //槽：更新质控液下拉列表
    slt_cboQCLiquid_UpdateItems();

    SampleNoMap.clear();
    Count = 0;
    isAllNull_Flag = false; //全选/反选标志

    slt_tbvQCList_Update();
}

//槽：更新质控液下拉列表
void QCApplication::slt_cboQCLiquid_UpdateItems()
{
    cboQCLiquid->clear();
    cboQCLiquid->addItem("");
    cboQCLiquid->addItems(gParameter->getQcLiquidName());
}

//槽：更新质控液下拉列表
void QCApplication::slt_cboQCLiquid_Update()
{

}

//槽：样本位选择
void QCApplication::slt_btnSamplePos_Clicked()
{
    int x = ( QApplication::desktop()->width() - 800 ) / 2;
    int y = ( QApplication::desktop()->height() - 600 ) / 2;
    gQCApplicationSampleDiskInfo->setGeometry(x, y, 800, 600);
    gQCApplicationSampleDiskInfo->show();
}

//槽：申请
void QCApplication::slt_btnApplicate_Clicked()
{
//    gMessageBox->setInfo(QString("提示"), QString("'质控申请'功能尚未开放!!"),
//                         QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
//    gMessageBox->setButtonText(Cancel, "返回");
//    gMessageBox->show();
//    return;

    QString sampleNo = editSamplePos->text().trimmed();  //样本位
    QString no = editNo->text().trimmed();               //编号
    int sampleType = SampleType_Serum;    //样本类型
    int repeatTime = 1;   //重复测试的个数
    //常规项目、计算项目
    QStringList ProjectList, CalculateProjectList;
    CalculateProjectList.clear();

    //检查样本位是否为空
    QString strSplit = ",";  //以空格符作为数据的分割符
    int sampleCount = sampleNo.split(strSplit).count()-1;
    if( sampleCount == 0) //未选择样本位
    {
        gMessageBox->setInfo(QString("输入错误"), QString("未选择样本位，\n请重新输入！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "是");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            editSamplePos->clear();
        return;
    }
    else
    {
        int count;
        bool isCheck;
        isCheck = false;//是否有项目选择

        //检查常规项目是否为空
        count = gParameter->getProjectMap().count(); //设置测试项目的个数（常规项目）;
        for(int j = 0; j < count; j ++)
        {
            if (rdoRoutineItem[j]->isChecked())  //如项目ｉ选中则表示申请该测试
            {
                isCheck = true;
                int projectID = gParameter->getProject_id( rdoRoutineItem[j]->text().trimmed() );
                ProjectList.append(QString::number(projectID));

                rdoRoutineItem[j]->setChecked(false);
            }
        }

        //未有任务项目选择
        if( ! isCheck )
        {
            gMessageBox->setInfo(QString("提示"), QString(" 请选择要申请的测试项目!"),
                                 QPixmap("://resource/resource/MessageBox/information.png"), true, true);
            gMessageBox->setButtonText(Cancel, "确定");
            if(gMessageBox->exec() == MVMessageBox::Rejected)
                return;
        }

        int qcLiquid_id = gParameter->getQcLiquidID(cboQCLiquid->currentText().trimmed());
        gApplicateThread->ApplicateTest(MV_State_QC, sampleNo, no, sampleType,
                                        ProjectList, CalculateProjectList, SampleNoMap, repeatTime, qcLiquid_id);
    }

    //更新申请列表
    editSamplePos->clear();
    editNo->clear();
    //gQCApplicationSampleDiskInfo->setSampleNoStart();
}

//槽：质控设置
void QCApplication::slt_btnQCSetting_Clicked()
{
    int x = ( QApplication::desktop()->width() - 1000 ) / 2;
    int y = ( QApplication::desktop()->height() - 680 ) / 2;
    QCSettingWidget::getInstance()->setGeometry(x, y, 1000, 680);
    QCSettingWidget::getInstance()->show();
}

//槽：下拉列表索引改变
void QCApplication::slt_cboQCLiquid_IndexChanged(int index)
{
    if(index == 0)
    {
        for(int i = 0; i < PROJECT_COUNT; i ++)
            rdoRoutineItem[i]->setVisible(false);
    }
    else
    {
        int pID;
        QStringList list = gParameter->getQcProject(cboQCLiquid->currentText());
        for(int i = 0; i < PROJECT_COUNT; i ++)
        {
            if(i >= list.count())
            {
                rdoRoutineItem[i]->setChecked(false);
                rdoRoutineItem[i]->setVisible(false);
            }
            else
            {
                //判断是否已经设置了项目相应的试剂位
                pID = gParameter->getProject_id(list.at(i));
                if(gParameter->getReagentPosCount(pID) == 0) //查看项目的试剂个数，若为“0”则表示未设置试剂位
                    rdoRoutineItem[i]->setEnabled(false);
                else
                    rdoRoutineItem[i]->setEnabled(true);

                rdoRoutineItem[i]->setText( FillSpace(list.at(i).trimmed().count()) + list.at(i).trimmed() );
                rdoRoutineItem[i]->setVisible(true);
            }
        }
    }
}

//槽：全选/反选
void QCApplication::slt_btnAllNone_Clicked()
{
    for(int i = 0; i < ROWCOUNT; i ++ )
    {
        if(modelQCList->item( i, 1)->text().toInt() > 0)
        {
            if (isAllNull_Flag)              //全选/反选标志
                modelQCList->item( i, 0)->setCheckState(Qt::Unchecked);
            else
                modelQCList->item( i, 0)->setCheckState(Qt::Checked);
        }
    }
    isAllNull_Flag = !isAllNull_Flag;
}

//槽：上一页
void QCApplication::slt_btnFormer_Clicked()
{
    if(Count > 0)
    {
        Count -= ROWCOUNT;
        if( !slt_tbvQCList_Update() )
        {
            Count += ROWCOUNT;
            slt_tbvQCList_Update();
        }
    }
}

//槽：下一页
void QCApplication::slt_btnNext_Clicked()
{
    Count += ROWCOUNT;
    if( !slt_tbvQCList_Update() )
    {
        Count -= ROWCOUNT;
        slt_tbvQCList_Update();
    }
}

//槽：删除
void QCApplication::slt_btnDelete_Clicked()
{
    QString str;
    QStringList list;
    list.clear();
    for(int i = 0; i < modelQCList->rowCount(); i ++)
    {
        if(modelQCList->item(i, 0)->checkState()) //选中状态
        {
            if(modelQCList->item(i, 10)->text().toInt() == State_WAIT ) //等待状态
            {
                str = "delete from TaskT where id=" + modelQCList->item(i, 1)->text() + ";";
                list.append(str);
            }
        }
    }
    if(list.count() == 0) return;

    SQLite::getInstance()->Transaction(list);
    slt_tbvQCList_Update();
}

//槽：已经选择的样本位
void QCApplication::slt_SelectedSampleList(QString s,QMap<int,QString> NoMap)
{
    editSamplePos->setText(s);
    SampleNoMap = NoMap;
}

//槽：更新已申请的质控列表
int QCApplication::slt_tbvQCList_Update()
{
    int flag = 0;
    //从数据库中取出用户申请的测试信息，并存入循环队列，等待执行
    QString str  =
            "select TaskT.typeID, TaskT.id, TaskT.HistoryT_id,  StateT.name, TaskT.sampleCode,"
            "TaskT.dateTime, QCLiquidT.name, ProjectT.name, TaskT.samplePos, TaskT.sampleCode, TaskT.stateID, TaskT.no "
            " from TaskT "
            //连接方式(任务队列表、项目表)
            " Inner join ProjectT ON TaskT.projectID = ProjectT.id "
            //连接方式(表、表)
            " Inner join QCLiquidT ON TaskT.QcLiquidID = QCLiquidT.id "
            //连接方式(任务队列表、样本类型表)
            " Inner join SampleTypeT ON TaskT.sampleTypeID = SampleTypeT.id "
            //连接方式(任务队列表、状态表表)
            " Inner join StateT ON TaskT.stateID = StateT.id "
            //查询条件(不查询完成状态)
            " where TaskT.stateID != 11 and  TaskT.stateID != 15  and typeID =" + QString::number( MV_State_QC ) + " "
            //排序查询：紧急否降序
            " order by TaskT.stateID desc, TaskT.isEmergency desc, TaskT.dateTime asc "
            " limit " + QString::number(Count) +", " + QString::number(ROWCOUNT)
           +";";

    //"" << "id" << "h_id"  << "状态" << "编号" << "申请时间" << "质控液"<< "项目" << "样本位"  << "sampleCode" << "stateID";
    //qDebug() << str;
    int i = 0;
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    while(query.next())
    {
        modelQCList->item(i, 1)->setText(query.value(1).toString());  //id(隐藏)
        modelQCList->item(i, 2)->setText(query.value(2).toString());  //h_id-隐藏
        modelQCList->item(i, 3)->setText(query.value(3).toString());  //
        modelQCList->item(i, 4)->setText(query.value(4).toString());  //
        modelQCList->item(i, 5)->setText(query.value(5).toString());  //
        modelQCList->item(i, 6)->setText(query.value(6).toString());  //
        modelQCList->item(i, 7)->setText(query.value(7).toString());  //
        modelQCList->item(i, 8)->setText(query.value(8).toString());  //
        modelQCList->item(i, 9)->setText(query.value(9).toString());  //
        modelQCList->item(i, 10)->setText(query.value(10).toString());//

        i++;
        isAllNull_Flag = false;
        flag = 1;
    }

    //清空数据
    for(i; i < ROWCOUNT; i ++ )
    {
        modelQCList->item(i, 0)->setCheckState(Qt::Unchecked);
        modelQCList->item(i, 1)->setText("");  //id(隐藏)
        modelQCList->item(i, 2)->setText("");  //h_id-隐藏
        modelQCList->item(i, 3)->setText("");  //
        modelQCList->item(i, 4)->setText("");  //
        modelQCList->item(i, 5)->setText("");  //
        modelQCList->item(i, 6)->setText("");  //
        modelQCList->item(i, 7)->setText("");  //
        modelQCList->item(i, 8)->setText("");  //
        modelQCList->item(i, 9)->setText("");  //
        modelQCList->item(i, 10)->setText("");  //
    }

    return flag;
}

//根据项目名称长度来确定空格长度
QString QCApplication::FillSpace(int count)
{
    QString str = "          "; //空格长度10
    return str.left( (str.count()-count)/2 );
}


