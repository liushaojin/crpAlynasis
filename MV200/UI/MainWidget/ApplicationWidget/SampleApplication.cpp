#include "SampleApplication.h"
#include <qdebug.h>
#include "UI/MainWidget/ApplicationWidget/SonWidget/SelectSampleWidget.h"

SampleApplication *gSampleApplication;
SampleApplication::SampleApplication(QWidget *parent)
    : QWidget(parent)
{
    initForm();          //初始化
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}

//初始化
void SampleApplication::initForm()
{
    gSelectSampleWidget = new SelectSampleWidget;
    gSampleApplicationSampleDiskInfo = new SampleDiskInfoWidget;
    applicationTaskListWidget = new ApplicationTaskListWidget; //申请列表
    applicationTaskListWidget->setModal(true);

    initSampleInfoForm();     //申请测试的样本的信息
    initProjectListForm();    //项目列表
    initCombinationItemForm();//组合项目

    //总布局
    layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(wdgSampleInfo,        0,  0, 10, 100, Qt::AlignLeft);
    layout->addWidget(grbProjectList,      10,  0, 80, 80);
    layout->addWidget(grbCombinationItem,  10, 80, 80, 20);
    this->setLayout(layout);
}

//申请测试的样本的信息
void SampleApplication::initSampleInfoForm()
{
    //申请测试的样本的信息
    wdgSampleInfo = new QWidget;
    labSamplePos = new QLabel("样本位:");        //样本位
    editSamplePos = new QLineEdit;
    btnSamplePos = new QPushButton("…");
    labNo = new QLabel("测试编号:");              //测试编号
    editNo = new QLineEdit;
    btnSampleCode = new QPushButton("编号");
    labSampleNo = new QLabel("样本编号:");         //样本编号
    editSampleNo = new QLineEdit;
    labSampleType = new QLabel("类型:");         //类型
    cboSampleType = new QComboBox;
    labSampleRepeat = new QLabel("重复次数:");     //重复(样本重复测试的个数)
    editSampleRepeat = new QLineEdit;
    labBatchApplicate = new QLabel("批量:");     //批量
    cboBatchApplicate = new QComboBox;
    btnDetailInfo = new QPushButton("详细信息");  //详细信息
    btnApplicate = new QPushButton("申请");   //申请
    btnApplicateList = new QPushButton("申请列表"); //申请列表

    lythSampleInfo = new QHBoxLayout;
    lythSampleInfo->setMargin(0);
    lythSampleInfo->setSpacing(1);
    lythSampleInfo->addWidget(labSamplePos);
    lythSampleInfo->addWidget(editSamplePos);
    lythSampleInfo->addWidget(btnSamplePos);
    lythSampleInfo->addWidget(new QLabel("   "));
    //lythSampleInfo->addWidget(labNo);
    //lythSampleInfo->addWidget(editNo);
    //lythSampleInfo->addWidget(btnSampleCode);
    //lythSampleInfo->addWidget(labSampleNo, Qt::AlignRight);
    //lythSampleInfo->addWidget(editSampleNo, Qt::AlignLeft);
    lythSampleInfo->addWidget(new QLabel("   "));
    lythSampleInfo->addWidget(labSampleType);
    lythSampleInfo->addWidget(cboSampleType);
    lythSampleInfo->addWidget(new QLabel("   "));
    lythSampleInfo->addWidget(labSampleRepeat);
    lythSampleInfo->addWidget(editSampleRepeat);
    lythSampleInfo->addWidget(new QLabel("   "));
    //lythSampleInfo->addWidget(labBatchApplicate);
    //lythSampleInfo->addWidget(cboBatchApplicate);
    lythSampleInfo->addWidget(new QLabel("   "));
    //lythSampleInfo->addWidget(btnDetailInfo);
    lythSampleInfo->addWidget(btnApplicate);
    lythSampleInfo->addWidget(btnApplicateList, Qt::AlignRight);

    wdgSampleInfo->setLayout(lythSampleInfo);
}
//项目列表(常规项目&计算项目)
void SampleApplication::initProjectListForm()
{
    //项目列表
    //treeWidget = new QTreeWidget;
    QGroupBox *grbRoutineItem = new QGroupBox("常规项目");
    QFlowLayout *lytRoutineItem = new QFlowLayout(this);
    lytRoutineItem->setMargin(18);
    lytRoutineItem->setSpacing(5);
    for(int i = 0; i < PROJECT_COUNT; i ++)
    {
        rdoRoutineItem[i] = new QRadioButton;     //常规项目
        lytRoutineItem->addWidget(rdoRoutineItem[i]);
    }
    grbRoutineItem->setLayout(lytRoutineItem);

    QGroupBox *grbComputingItem = new QGroupBox("计算项目");
    QFlowLayout *lytComputingItem = new QFlowLayout;
    lytComputingItem->setMargin(18);
    lytComputingItem->setSpacing(5);
    for(int i = 0; i < CALCULATE_PROJECT_COUNT; i ++)
    {
        rdoComputingItem[i] = new QRadioButton;   //计算项目
        lytComputingItem->addWidget(rdoComputingItem[i]);
    }
    grbComputingItem->setLayout(lytComputingItem);

    grbProjectList = new QGroupBox;
    lytvProjectList = new QVBoxLayout;
    lytvProjectList->setMargin(0);
    lytvProjectList->setSpacing(0);
    lytvProjectList->addWidget(grbRoutineItem);
    lytvProjectList->addWidget(grbComputingItem);
    grbProjectList->setLayout(lytvProjectList);
}
//组合项目
void SampleApplication::initCombinationItemForm()
{
    //组合项目
    grbCombinationItem = new QGroupBox("组合项目");
    lytvCombinationItem = new QFlowLayout;
    lytvCombinationItem->setMargin(18);
    lytvCombinationItem->setSpacing(2);
    for(int i = 0; i < COMBINATIONITEMCOUNT; i++)
    {
        rdoCombinationItem[i] = new QRadioButton;   //组合项目
        rdoCombinationItem[i]->setAutoExclusive(false);  //支持多选
        rdoCombinationItem[i]->setObjectName("rdo2");     //组合项目
        if(i%2 == 0)
            lytvCombinationItem->addWidget(rdoCombinationItem[i]);
        else
            lytvCombinationItem->addWidget(rdoCombinationItem[i]);
    }
    grbCombinationItem->setLayout(lytvCombinationItem);
}

//界面风格
void SampleApplication::initStyle()
{
    editSampleRepeat->setAlignment(Qt::AlignCenter);
    //申请测试的样本的信息
    btnSamplePos->setObjectName("button8");
    btnSampleCode->setObjectName("button8");
    btnDetailInfo->setObjectName("button2");

    //项目列表&计算项目
    for(int i = 0; i < PROJECT_COUNT; i ++)
    {
        rdoRoutineItem[i]->setVisible(false);        //默认隐藏
        rdoRoutineItem[i]->setAutoExclusive(false);  //支持多选
        rdoRoutineItem[i]->setObjectName("rdo");     //常规项目
        rdoRoutineItem[i]->setFixedSize(150, 64);
    }
    for(int i = 0; i < CALCULATE_PROJECT_COUNT; i ++)
    {
        rdoComputingItem[i]->setVisible(false);        //默认隐藏
        rdoComputingItem[i]->setAutoExclusive(false);  //支持多选
        rdoComputingItem[i]->setObjectName("rdo");   //计算项目
    }

    //组合项目
    for(int i = 0; i < COMBINATIONITEMCOUNT; i++)
    {
        rdoCombinationItem[i]->setVisible(false);         //默认隐藏
        rdoCombinationItem[i]->setAutoExclusive(false);   //支持多选
        rdoCombinationItem[i]->setObjectName("rdo2");     //组合项目
    }

    btnApplicate->setObjectName("button2");
    btnApplicateList->setObjectName("button2");

    editNo->setReadOnly(true);
    editSamplePos->setReadOnly(true);
    editSampleRepeat->setFixedWidth(60);
}

//连接信号与槽
void SampleApplication::initConnect()
{
    connect(btnApplicate, SIGNAL(clicked()), this, SLOT(slt_btnApplicate_Clicked()));
    connect(btnSamplePos, SIGNAL(clicked()), this, SLOT(slt_btnSamplePos_Clicked()));
    connect(btnApplicateList, SIGNAL(clicked()), this, SLOT(slt_btnApplicateList_Clicked()));

    connect(gParameter, SIGNAL(sig_newProject()), this, SLOT(slt_UpdateRoutineItem()));
    connect(gParameter, SIGNAL(sig_newCombineProject()), this, SLOT(slt_UpdateCombinationItem()));
    connect(gParameter, SIGNAL(sig_newCalculateProject()), this, SLOT(slt_UpdateComputingItem()));

    connect(this, SIGNAL(sig_UpdateApplicationTaskList()),
            applicationTaskListWidget, SLOT(slt_UpdateApplicationTaskList()));

    connect(gSampleApplicationSampleDiskInfo, SIGNAL(sig_SelectedSampleList(QString,QMap<int, QString>)),
            this, SLOT(slt_SelectedSampleList(QString,QMap<int,QString>)));
    connect(gSelectSampleWidget, SIGNAL(sig_SelectedSampleList(QString,QMap<int, QString>)),
            this, SLOT(slt_SelectedSampleList(QString,QMap<int,QString>)));

    connect(gApplicateThread, SIGNAL(sig_UpdateApplicatedCount()), this,  SLOT(slt_UpdateApplicatedCount()) );
    connect(gScheduler, SIGNAL(sig_UpdateApplicatedCount()), this,  SLOT(slt_UpdateApplicatedCount()) );
    connect( applicationTaskListWidget, SIGNAL(sig_UpdateApplicatedCount()), this, SLOT(slt_UpdateApplicatedCount()) );

    //
    for(int i = 0; i < COMBINATIONITEMCOUNT; i ++)
        connect(rdoCombinationItem[i], SIGNAL(clicked(bool)), this, SLOT(slt_CombineProject_Clicked()));

    //
    for(int i = 0; i < CALCULATE_PROJECT_COUNT; i ++)
        connect(rdoComputingItem[i], SIGNAL(clicked(bool)), this, SLOT(slt_CalculateProject_Clicked()));
}

//初始化数据
void SampleApplication::initData()
{
    RequestedNO = 1;  //已经申请的测试的个数，每次开机都清零
    editSampleRepeat->setText("1");

    //    cboSampleType->addItem("全血");
    //    cboSampleType->addItem("血清");
    //    cboSampleType->addItem("血浆");
    //    cboSampleType->addItem("尿液");
    cboSampleType->addItems(gParameter->getSampleTypeList());
    cboSampleType->setCurrentIndex(1);

    editNo->setText( UpdateRequestedNO() );    //更新测试编号

    slt_UpdateRoutineItem();          //槽：更新常规项目
    slt_UpdateComputingItem();        //槽：更新计算项目
    slt_UpdateCombinationItem();      //槽：更新组合项目

    SampleNoMap.clear();
    slt_UpdateApplicatedCount();
}

//槽：选择样本位
void SampleApplication::slt_btnSamplePos_Clicked()
{
    int x = ( qApp->desktop()->width() - 800 ) / 2;
    int y = ( qApp->desktop()->height() - 600 ) / 2;
    //gSampleApplicationSampleDiskInfo->setGeometry(x, y, 800, 600);
    //gSampleApplicationSampleDiskInfo->show();

    gSelectSampleWidget->setGeometry(x, y, 800, 600);
    gSelectSampleWidget->show();
}

//槽：已经选择的样本
void SampleApplication::slt_SelectedSampleList(QString s)
{
    editSamplePos->setText(s);
}

//槽：已经选择的样本位
void SampleApplication::slt_SelectedSampleList(QString s, QMap<int, QString> NoMap)
{
    editSamplePos->setText(s);
    SampleNoMap = NoMap;
}

//更新测试编号
QString SampleApplication::UpdateRequestedNO()
{
    QDateTime time = QDateTime::currentDateTime();            //获取系统现在的时间
    QString date = time.toString("yyyyMMddhhmm");      //设置显示格式

    QString temp = "00000";
    QString count = QString::number(RequestedNO);
    temp = temp.replace(5-count.count(), count.count(), count );
    return date+temp;
}

//清空已经选择的样本位
void SampleApplication::CleanSelectSamplePos()
{
    editSamplePos->clear();
}

//清空已经选择的项目
void SampleApplication::CleanAllProject()
{
//    QVector<QString> routineP =  gParameter->get_ProjectName();
//    QVector<QString> ComputingP =  gParameter->getCalculateProjectName();
//    for(int i = 0; i < PROJECT_COUNT; i ++)
//    {
//        if( i < routineP.count() )
//            rdoRoutineItem[i]->setChecked(false);
//        if( i < ComputingP.count() )
//            rdoComputingItem[i]->setChecked(false);
//    }
    for(int i = 0; i < PROJECT_COUNT; i ++)
    {
        rdoRoutineItem[i]->setChecked(false);
    }
    for(int i = 0; i < CALCULATE_PROJECT_COUNT; i ++)
    {
        rdoComputingItem[i]->setChecked(false);
    }
    for(int i = 0; i < COMBINATIONITEMCOUNT; i ++)
    {
        rdoCombinationItem[i]->setChecked(false);
    }
}

//按组合项目id的成员，设置常规项目是否选中flag（返回不能申请的项目的名称，即未设置试剂位的项目）
QStringList SampleApplication::setRoutineCheckState(int id, bool flag)
{
    QStringList list;
    QStringList pName = gParameter->getCombProjectMember(id).split("+");
    for(int k = 0; k < pName.count(); k ++ )
    {
        for( int i = 0; i < gParameter->get_ProjectName().count(); i ++ )
        {
            if(rdoRoutineItem[i]->text().trimmed() == pName.at(k).trimmed())
            {
                if(rdoRoutineItem[i]->isEnabled()) //是否能申请（即是否已经设置了试剂位）
                    rdoRoutineItem[i]->setChecked(flag);
                else
                {
                    list.append(pName.at(k).trimmed());
                    rdoRoutineItem[i]->setChecked(false);
                }
            }
        }
    }
    return list;
}

//槽：申请
void SampleApplication::slt_btnApplicate_Clicked()
{
    QString sampleNo = editSamplePos->text().trimmed();  //样本位
    QString no = editNo->text().trimmed();               //编号
    int sampleType = cboSampleType->currentIndex()+1;    //样本类型
    int repeatTime = editSampleRepeat->text().toInt();   //重复测试的个数
    //常规项目、计算项目
    QStringList ProjectList, CalculateProjectList;

    //检查样本位是否为空
    QString strSplit = ",";  //以空格符作为数据的分割符
    int sampleCount = sampleNo.split(strSplit).count()-1;
    if( sampleCount == 0) //未选择样本位
    {
        gMessageBox->setInfo(QString("输入错误"), QString(" 未选择样本位，\n\n请重新输入！"),
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
            }
        }

        //计算项目
        for(int i = 0; i < CALCULATE_PROJECT_COUNT; i ++)
        {
            if( !rdoComputingItem[i]->isVisible() ) break; //跳出for循环

            //是否选中计算项目
            if(rdoComputingItem[i]->isChecked())
            {
                int id = gParameter->getCalculateProject_id( rdoComputingItem[i]->text().trimmed() );
                CalculateProjectList.append(QString::number(id));
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

        gApplicateThread->ApplicateTest( MV_State_Normal, sampleNo, no, sampleType, ProjectList, CalculateProjectList, SampleNoMap, repeatTime, 1);
        RequestedNO += sampleCount*repeatTime*ProjectList.count();
    }

    emit sig_UpdateApplicationTaskList();      //信号：更新申请列表
    editNo->setText( UpdateRequestedNO() );    //更新测试编号
    CleanSelectSamplePos(); //清空已经选择的样本位
    CleanAllProject();      //清空已经选择的项目
    gSelectSampleWidget->updateSampleSlotNo();
}

//槽：申请列表
void SampleApplication::slt_btnApplicateList_Clicked()
{
    applicationTaskListWidget->initData();
    int x = ( qApp->desktop()->width() - 1050 ) / 2;
    int y = ( qApp->desktop()->height() - 810 ) / 2;
    applicationTaskListWidget->setGeometry(x, y, 1050, 810);
    applicationTaskListWidget->show();
}

//槽：更新常规项目
void SampleApplication::slt_UpdateRoutineItem()
{
    //项目列表
    int pID;
    QVector<QString> routineP =  gParameter->get_ProjectName();
    for(int i = 0; i < PROJECT_COUNT; i ++)
    {
        if(i < routineP.count())
        {
            //判断是否已经设置了项目相应的试剂位
            pID = gParameter->getProject_id(routineP.at(i));
            if(gParameter->getReagentPosCount(pID) == 0) //查看项目的试剂个数，若为“0”则表示未设置试剂位
            {
                rdoRoutineItem[i]->toolTip().setNum(10);
                rdoRoutineItem[i]->setToolTip("未设置试剂位     ");
                rdoRoutineItem[i]->setEnabled(false);
            }
            else
            {
                rdoRoutineItem[i]->setToolTip("");
                rdoRoutineItem[i]->setEnabled(true);
            }

            rdoRoutineItem[i]->setChecked(false);
            rdoRoutineItem[i]->setVisible(true);
            rdoRoutineItem[i]->setText( FillSpace(routineP.at(i).trimmed().count()) + routineP.at(i).trimmed());
        }
        else
        {
            rdoRoutineItem[i]->setChecked(false);
            rdoRoutineItem[i]->setText("");
            rdoRoutineItem[i]->setVisible(false);        //默认隐藏
        }
    }
}

//槽：更新计算项目
void SampleApplication::slt_UpdateComputingItem()
{
    //项目列表
    QVector<QString> name =  gParameter->getCalculateProjectName();
    for(int i = 0; i < COMBINATIONITEMCOUNT; i ++)
    {
        if(i < name.count())
        {
            rdoComputingItem[i]->setChecked(false);
            rdoComputingItem[i]->setVisible(true);
            rdoComputingItem[i]->setText( FillSpace(name.at(i).trimmed().count()) + name.at(i).trimmed());
        }
        else
        {
            rdoComputingItem[i]->setText("");
            rdoComputingItem[i]->setChecked(false);
            rdoComputingItem[i]->setVisible(false);
        }
    }
}

//槽：更新组合项目
void SampleApplication::slt_UpdateCombinationItem()
{
    //项目列表
    QVector<QString> name =  gParameter->getCombineProjectName();
    for(int i = 0; i < COMBINATIONITEMCOUNT; i ++)
    {
        if(i < name.count())
        {
            rdoCombinationItem[i]->setChecked(false);
            rdoCombinationItem[i]->setVisible(true);
            rdoCombinationItem[i]->setText( " " + name.at(i));
        }
        else
        {
            rdoCombinationItem[i]->setText("");
            rdoCombinationItem[i]->setChecked(false);
            rdoCombinationItem[i]->setVisible(false);
        }
    }
}

//槽：实时更新数据库中状态为“等待”的测试的个数
void SampleApplication::slt_UpdateApplicatedCount()
{
    QString str = "select count(*) from TaskT where TaskT.stateID = " + QString::number(State_WAIT)
            + " and isCalculateProj=0  and typeID =" + QString::number( MV_State_Normal ) +";";
    int count;
    QSqlQuery  query = SQLite::getInstance()->Select(str);  //查询数据库
    while(query.next())
        count = query.value(0).toInt();

    btnApplicateList->setText("申请列表("+ QString::number(count) +")");
}

//槽：选择组合项目时在常规项目中选中相应的项目
void SampleApplication::slt_CombineProject_Clicked()
{
    //当前触发信号的对象
    QRadioButton *rdo = qobject_cast<QRadioButton*>(sender());
    int combP_id = gParameter->isComgineProjectExist(rdo->text().trimmed());
    if(combP_id == 0) return;  //组合项目不存在

    if(rdo->isChecked())  //选择组合项目
    {
        QStringList list = setRoutineCheckState(combP_id, true);

        //提示不能申请的项目的名称及原因
        if(list.count() > 0)
        {
            QString member;
            for( int i = 0; i < list.count(); i ++)
                member += "“" + list.at(i) + "”,";

            gMessageBox->setInfo(QString("警告"), QString("组合项目“"+ rdo->text().trimmed() +"”的成员项目\n"
                                                        + member.left(member.count()-1) + "未设置试剂位!"),
                                 QPixmap("://resource/resource/MessageBox/information.png"), true, true);
            gMessageBox->setButtonText(Cancel, "确定");
            gMessageBox->show();
            return;
        }
    }
    else  //取消组合
    {
        setRoutineCheckState(combP_id, false);

        //遍历下组合项目以防已经选择的组合项目中与当前取消
        for(int i = 0; i < gParameter->getCombineProjectName().count(); i ++)
        {
            if(rdoCombinationItem[i]->isChecked())
            {
                combP_id = gParameter->isComgineProjectExist(rdoCombinationItem[i]->text().trimmed());
                if(combP_id == 0) return;  //组合项目不存在
                setRoutineCheckState(combP_id, true);
            }
        }
    }
}

//槽：选择计算项目时判断该计算项目的子项目是否全部可以申请测试
void SampleApplication::slt_CalculateProject_Clicked()
{
    //当前触发信号的对象
    QRadioButton *rdo = qobject_cast<QRadioButton*> (sender());
    if(!rdo->isChecked()) return; //若取消状态，则返回（即当前操作为取消选中）

    int Pid = gParameter->isCalculateProjectExist(rdo->text().trimmed());
    if( Pid == 0 ) return;  //项目不存在

    QStringList list = gParameter->getCalculateProjectList(Pid);
    if(list.count() == 0) return;

    int id; //项目id
    for(int i = 0; i < list.count()-1; i ++) //去掉最后一个项目 “”空项目
    {
        if(list.at(i) == "") break;

        id = gParameter->getProject_id(list.at(i).trimmed());
        ///项目不存在
        if(id == 0)
        {
            //提示
            gMessageBox->setInfo(QString("警告"), QString("计算项目“"+ rdo->text().trimmed() +"”的成员项目\n"
                                                        + list.at(i) + "不存在!"),
                                 QPixmap("://resource/resource/MessageBox/information.png"), true, true);
            gMessageBox->setButtonText(Cancel, "确定");
            gMessageBox->show();

            rdo->setChecked(false); //不能申请该计算项目
            return;
        }

        //查看项目的试剂个数，若为“0”则表示未设置试剂位
        if(gParameter->getReagentPosCount(id) == 0)
        {
            //提示
            gMessageBox->setInfo(QString("警告"), QString("计算项目“"+ rdo->text().trimmed() +"”的成员项目\n\n"
                                                        + "“"  + list.at(i) + "”" + "不能申请!"),
                                 QPixmap("://resource/resource/MessageBox/information.png"), true, true);
            gMessageBox->setButtonText(Cancel, "确定");
            gMessageBox->show();

            rdo->setChecked(false); //不能申请该计算项目
            return;
        }
    }

    //遍历当前计算项目的常规子项目是否已经选择
    //虽然与上一个for循环一样，但不能合并，因为要先查看所有子项目是否能申请，后再查看所有子项目是否已经选中了（有先后顺序）
    for(int i = 0; i < list.count()-1; i ++)
    {
        for(int p = 0; p < PROJECT_COUNT; p ++)
        {
            //未定义的则跳过
            if( !rdoRoutineItem[p]->isVisible() ) break;

            //是否为子项目
            if(rdoRoutineItem[p]->text().trimmed() == list.at(i).trimmed())
            {
                if( rdoRoutineItem[p]->isChecked() ) break; //若子项目已经选中，则跳过
                else //提示
                {
                    gMessageBox->setInfo(QString("警告"), QString("计算项目“"+ rdo->text().trimmed() +"”的成员项目\n"
                                                                + "“"  + list.at(i) + "”" + "未选中!"),
                                         QPixmap("://resource/resource/MessageBox/information.png"), true, true);
                    gMessageBox->setButtonText(Cancel, "返回");
                    gMessageBox->show();
                    rdo->setChecked(false); //不能申请该计算项目
                    return;
                }
            }
        }
    }

}

//根据项目名称长度来确定空格长度
QString SampleApplication::FillSpace(int count)
{
    QString str = "          "; //空格长度10
    return str.left( (str.count()-count)/2 );
}

