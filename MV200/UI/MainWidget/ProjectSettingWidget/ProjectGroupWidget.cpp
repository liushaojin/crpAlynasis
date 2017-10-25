#include "ProjectGroupWidget.h"

//实例对象
ProjectGroupWidget *ProjectGroupWidget::_instance = 0;
ProjectGroupWidget * ProjectGroupWidget::getInstance()
{
    if (_instance == 0)
        _instance = new ProjectGroupWidget();
    return _instance;
}

//构造函数
ProjectGroupWidget::ProjectGroupWidget(QWidget *parent) : QWidget(parent)
{
    initForm();                 //初始化
    initTableFormat();          //格式化表格
    initStyle();                //界面风格
    initConnect();              //连接信号与槽
    initData();                 //初始化数据
}

//初始化
void ProjectGroupWidget::initForm()
{
    //组合项目清单
    grbCombProjectList = new QGroupBox("组合项目清单");
    tbvCombProjectList = new QTableView;
    modelCombProjectList = new QStandardItemModel;
    lytCombProjectList = new QGridLayout;
    lytCombProjectList->setContentsMargins(0, 20, 0, 0);
    lytCombProjectList->setSpacing(0);
    lytCombProjectList->addWidget(tbvCombProjectList, 2, 0, 100, 50);
    grbCombProjectList->setLayout(lytCombProjectList);
    tbvCombProjectList->setModel(modelCombProjectList);

    //组合项目
    grbCombInfo = new QGroupBox("组合项目信息");      //组合项目信息
    labProjectNameEN = new QLabel("   简称:");    //组合项目名称（英文简称）
    editProjectNameEN = new QLineEdit;
    labProjectNameCN = new QLabel("全称:");    //组合项目名称（中文全称）
    editProjectNameCN = new QLineEdit;
    lytCombInfo = new QGridLayout;
    lytCombInfo->setMargin(10);
    lytCombInfo->setSpacing(10);
    lytCombInfo->addWidget(labProjectNameEN,  0,  0, 2,  5, Qt::AlignRight);
    lytCombInfo->addWidget(editProjectNameEN, 0,  5, 2, 10, Qt::AlignLeft);
    lytCombInfo->addWidget(labProjectNameCN,  0, 15, 2, 5, Qt::AlignRight);
    lytCombInfo->addWidget(editProjectNameCN, 0, 25, 2, 10, Qt::AlignLeft);
    grbCombInfo->setLayout(lytCombInfo);

    grbProjectList = new QGroupBox("项目选择区");   //项目选择区
    lytProjectList = new QGridLayout;
    lytProjectList->setMargin(20);
    lytProjectList->setSpacing(5);
    for(int i = 0; i < PROJECT_COUNT; i ++)
    {
        rdoRoutineItem[i] = new QRadioButton;     //常规项目
        rdoRoutineItem[i]->setObjectName("rdo");
        rdoRoutineItem[i]->setAutoExclusive(false);
        rdoRoutineItem[i]->setVisible(false);
        lytProjectList->addWidget(rdoRoutineItem[i], i/4*3, i%4*5, 3, 5);
    }
    grbProjectList->setLayout(lytProjectList);

    //按键组
    wdgButtonList = new QWidget;
    btnAddNew = new QPushButton("新增");  //新增
    btnModify = new QPushButton("修改");  //修改
    btnDelete = new QPushButton("删除");  //删除
    btnCancle = new QPushButton("取消");  //取消
    btnSave = new QPushButton("保存");    //保存
    hlytButtonList = new QHBoxLayout;
    hlytButtonList->setMargin(2);
    hlytButtonList->setSpacing(10);
    hlytButtonList->addWidget(btnAddNew, Qt::AlignCenter);
    hlytButtonList->addWidget(btnModify, Qt::AlignCenter);
    hlytButtonList->addWidget(btnDelete, Qt::AlignCenter);
    hlytButtonList->addWidget(btnCancle, Qt::AlignCenter);
    hlytButtonList->addWidget(btnSave, Qt::AlignCenter);
    wdgButtonList->setLayout(hlytButtonList);

    wdgCombProject = new QWidget;
    lytCombProject = new QGridLayout;
    lytCombProject->setMargin(10);
    lytCombProject->setSpacing(10);
    lytCombProject->addWidget(grbCombInfo,  0, 0, 10, 50);
    lytCombProject->addWidget(grbProjectList, 10, 0, 70, 50);
    lytCombProject->addWidget(wdgButtonList, 80, 0, 10, 50);
    wdgCombProject->setLayout(lytCombProject);

    //
    layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(10);
    layout->addWidget(grbCombProjectList);
    layout->addWidget(wdgCombProject);
    this->setLayout(layout);
}

//格式化表格
void ProjectGroupWidget::initTableFormat()
{
    int tbvRowCount = COMBINATIONITEMCOUNT; //列表最大能显示的行数
    //列表
    QStringList headerList_h;
    headerList_h << "id" << "项目名称" << "项目成员" << "nameCn" << "member_idList";
    modelCombProjectList->setHorizontalHeaderLabels( headerList_h );
    int row = 0;
    tbvCombProjectList->hideColumn(row ++);             //id-隐藏
    tbvCombProjectList->setColumnWidth( row ++, 100);   //项目名称
    tbvCombProjectList->setColumnWidth( row ++, 200);   //项目成员
    tbvCombProjectList->hideColumn(row ++);             //nameCn-隐藏
    tbvCombProjectList->hideColumn(row ++);             //member_idList-隐藏

    tbvCombProjectList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvCombProjectList->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvCombProjectList->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvCombProjectList->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
    tbvCombProjectList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    //tbvCombProjectList->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
    tbvCombProjectList->horizontalHeader()->setStyleSheet("font-size:18px;");         //表头字体样式

    //初始化每一个单元格
    for( int i = 0; i < tbvRowCount; i ++ )
    {
        modelCombProjectList->setItem( i, 0, new QStandardItem( "" ));  //id(隐藏)
        modelCombProjectList->setItem( i, 1, new QStandardItem( "" ));  //
        modelCombProjectList->setItem( i, 2, new QStandardItem( "" ));  //
        modelCombProjectList->setItem( i, 3, new QStandardItem( "" ));  //
        modelCombProjectList->setItem( i, 4, new QStandardItem( "" ));  //

        //列表内容显示方式
        modelCombProjectList->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        modelCombProjectList->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        modelCombProjectList->item(i, 2)->setTextAlignment(Qt::AlignCenter);
    }
}

//界面风格
void ProjectGroupWidget::initStyle()
{
    btnAddNew->setObjectName("button5");
    btnModify->setObjectName("button5");
    btnDelete->setObjectName("button5");
    btnCancle->setObjectName("button5");
    btnSave->setObjectName("button5");

    btnAddNew->setEnabled(true);
    btnModify->setEnabled(true);
    btnDelete->setEnabled(true);
    btnCancle->setEnabled(false);
    btnSave->setEnabled(false);
}

//连接信号与槽
void ProjectGroupWidget::initConnect()
{
    connect(btnAddNew, SIGNAL(clicked(bool)), this, SLOT(slt_btnAddNew_Clicked()));
    connect(btnModify, SIGNAL(clicked(bool)), this, SLOT(slt_btnModify_Clicked()));
    connect(btnDelete, SIGNAL(clicked(bool)), this, SLOT(slt_btnDelete_Clicked()));
    connect(btnCancle, SIGNAL(clicked(bool)), this, SLOT(slt_btnCancle_Clicked()));
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(slt_btnSave_Clicked()));

    connect(gParameter, SIGNAL(sig_newProject()), this, SLOT(slt_UpdateRoutineItem()));
    connect(tbvCombProjectList, SIGNAL(clicked(QModelIndex)), this, SLOT(slt_tbvSelectRow_info(QModelIndex)));
}

//初始化数据
void ProjectGroupWidget::initData()
{
    //槽：更新项目选择区中的项目（常规项目）
    slt_UpdateRoutineItem();
    BindingCombProject();
}

//绑定组合项目到列表中
void ProjectGroupWidget::BindingCombProject()
{
    QString str = "SELECT id, nameEn, nameCn, pMember, pID from CombineProjectT where id;";
    QSqlQuery query = SQLite::getInstance()->Select(str);
    int row = 0;
    while (query.next())
    {
        modelCombProjectList->item(row, 0)->setText( QString::number(query.value(0).toInt()));
        modelCombProjectList->item(row, 1)->setText( query.value(1).toString());
        modelCombProjectList->item(row, 2)->setText( query.value(3).toString());
        modelCombProjectList->item(row, 3)->setText( query.value(2).toString());
        modelCombProjectList->item(row, 4)->setText( query.value(4).toString());
        row ++;
    }

    for(row; row < modelCombProjectList->rowCount(); row ++)
    {
        modelCombProjectList->item(row, 0)->setText("");
        modelCombProjectList->item(row, 1)->setText("");
        modelCombProjectList->item(row, 2)->setText("");
        modelCombProjectList->item(row, 3)->setText("");
        modelCombProjectList->item(row, 4)->setText("");
    }
}

//组合项目成员，以“+”为分隔
QString ProjectGroupWidget::CombineProjectNameList()
{
    QString temp;
    for(int i = 0; i < PROJECT_COUNT; i ++)
    {
        if(rdoRoutineItem[i]->isChecked())
            temp += rdoRoutineItem[i]->text().trimmed() + "+";
    }
    return temp.left(temp.count() -1);
}

//组合项目成员ID，以"-"分隔
QString ProjectGroupWidget::CombineProject_idList()
{
    QString temp;
    for(int i = 0; i < PROJECT_COUNT; i ++)
    {
        if(rdoRoutineItem[i]->isChecked())
            temp +=  QString::number( gParameter->getProject_id(rdoRoutineItem[i]->text().trimmed()) ) + "-";
    }
    return temp.left(temp.count() -1);
}

//槽：新增
void ProjectGroupWidget::slt_btnAddNew_Clicked()
{
    is_AddNew_flag = 1;    //标志，新增/修改标志，（0：修改，1：新增）
    btnAddNew->setEnabled(false);
    btnModify->setEnabled(false);
    btnDelete->setEnabled(false);
    btnCancle->setEnabled(true);
    btnSave->setEnabled(true);

    editProjectNameCN->clear();
    editProjectNameEN->clear();

    for(int i = 0; i < PROJECT_COUNT; i ++)
        rdoRoutineItem[i]->setChecked(false);
}
//槽：修改
void ProjectGroupWidget::slt_btnModify_Clicked()
{
    is_AddNew_flag = 0;    //标志，新增/修改标志，（0：修改，1：新增）
    btnAddNew->setEnabled(false);
    btnModify->setEnabled(false);
    btnDelete->setEnabled(false);
    btnCancle->setEnabled(true);
    btnSave->setEnabled(true);
}
//槽：删除
void ProjectGroupWidget::slt_btnDelete_Clicked()
{
    int id = modelCombProjectList->item(tbvCombProjectList->currentIndex().row(), 0)->text().toInt();

    if(id > 0)
    {
        QString name = modelCombProjectList->item(tbvCombProjectList->currentIndex().row(), 1)->text(); //组合项目名称
        gMessageBox->setInfo(QString("删除提示"), QString("\n是否要删除组合项目“" + name +"”？"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, false);
        gMessageBox->setButtonText(Cancel, "否(N)");
        gMessageBox->setButtonText(Sure, "是(Y)");
        if(gMessageBox->exec() == MVMessageBox::Accepted)
        {
            QString str  = "delete from CombineProjectT where id="+ QString::number(id) +";";
            SQLite::getInstance()->Delete(str);

            //移除组合项目
            gParameter->removeCombineProject(id);
            BindingCombProject();
        }
    }
}
//槽：取消
void ProjectGroupWidget::slt_btnCancle_Clicked()
{
    btnAddNew->setEnabled(true);
    btnModify->setEnabled(true);
    btnDelete->setEnabled(true);
    btnCancle->setEnabled(false);
    btnSave->setEnabled(false);
}
//槽：保存
void ProjectGroupWidget::slt_btnSave_Clicked()
{
    QString nameEn = editProjectNameEN->text().trimmed();
    QString nameCn = editProjectNameCN->text().trimmed();

    //空值提示
    if(nameEn == "")
    {
        gMessageBox->setInfo(QString("警告"), QString("项目简称不能为空！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
        return;
    }
    else if ( nameCn == "" )
    {
        gMessageBox->setInfo(QString("警告"), QString("项目全称不能为空！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
        return;
    }

    //判断是否有项目选择
    bool isProjectSelected = false;
    for(int i = 0; i < gParameter->get_ProjectName().count(); i ++)
    {
        if(rdoRoutineItem[i]->isChecked())
            isProjectSelected = true;
    }
    if(!isProjectSelected) //
    {
        gMessageBox->setInfo(QString("警告"), QString("\n请选择要组合的项目！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "确定");
        gMessageBox->show();
        return;
    }

    QString pList = CombineProjectNameList();
    QString idList = CombineProject_idList();
    //保存到全局参数
    CombineProject param;
    param.CombProjectNameEn = nameEn;
    param.CombProjectNameCn = nameCn;
    param.CombProjectMember = pList;
    param.CombProjectMemberID = idList;
    if(is_AddNew_flag)  //新增
    {
        //组合项目是否已经达到了上限
        if(gParameter->getCombineProjectName().count() < COMBINATIONITEMCOUNT)
        {
            //判断是否已经存在
            if(gParameter->isComgineProjectExist(nameEn))
            {
                gMessageBox->setInfo(QString("警告"), QString("新增组合项目“"+ nameEn +"”已经存在!"),
                                     QPixmap("://resource/resource/MessageBox/information.png"), true, true);
                gMessageBox->setButtonText(Cancel, "返回");
                gMessageBox->show();
                return;
            }
            else
            {
                int id = 1;
                QString cMaxId = "SELECT MAX(id) from CombineProjectT;";
                QSqlQuery query = SQLite::getInstance()->Select(cMaxId);
                while (query.next()) {
                    id = query.value(0).toInt() + 1;
                }

                QString str = "insert into CombineProjectT(id, nameEn, nameCn, pMember, pID) values("
                        + QString::number(id) + ", '"+ nameEn +"', '"+ nameCn +"', '" + pList + "', '"+ idList + "');";
                SQLite::getInstance()->Insert(str);

                //新增
                gParameter->setCombineProject(id, param);

                //绑定组合项目到列表中
                BindingCombProject();
            }
        }
        else //新增不成功，组合项目已经达到上限了
        {
            gMessageBox->setInfo(QString("警告"), QString("组合项目已经达到上限!\n\n请删除无效组合项目后再保存"),
                                 QPixmap("://resource/resource/MessageBox/information.png"), true, true);
            gMessageBox->setButtonText(Cancel, "返回");
            gMessageBox->show();
        }
    }
    else //修改
    {
        int id = modelCombProjectList->item( tbvCombProjectList->currentIndex().row(), 0)->text().toInt();

        QString str = "update CombineProjectT set nameEn='" +nameEn+ "', nameCn='" +nameCn+ "',"
                +" pMember='" +pList+ "', pID='" +idList+ "' where id="+ QString::number(id) +";" ;
        SQLite::getInstance()->Update(str);

        //修改
        gParameter->setCombineProject(id, param);
        BindingCombProject();
    }

    btnAddNew->setEnabled(true);
    btnModify->setEnabled(true);
    btnDelete->setEnabled(true);
    btnCancle->setEnabled(false);
    btnSave->setEnabled(false);
}

//槽：更新项目选择区中的项目（常规项目）
void ProjectGroupWidget::slt_UpdateRoutineItem()
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
                rdoRoutineItem[i]->setToolTip("未设置试剂位  ");
                rdoRoutineItem[i]->setEnabled(false);
            }
            else
            {
                rdoRoutineItem[i]->setToolTip("");
                rdoRoutineItem[i]->setEnabled(true);
            }

            rdoRoutineItem[i]->setChecked(false);
            rdoRoutineItem[i]->setVisible(true);
            rdoRoutineItem[i]->setText( "  " + routineP.at(i));
        }
        else
        {
            rdoRoutineItem[i]->setText("");
            rdoRoutineItem[i]->setChecked(false);
            rdoRoutineItem[i]->setVisible(false);        //默认隐藏
        }
    }
}

//槽：更新选中组合项目的信息到右侧界面
void ProjectGroupWidget::slt_tbvSelectRow_info(QModelIndex index)
{
    //<< "id" << "项目名称" << "项目成员" << "nameCn" << "member_idList";
    int row = index.row();
    if(modelCombProjectList->item(row, 0)->text().toInt() > 0)
    {
        editProjectNameEN->setText(modelCombProjectList->item(row, 1)->text());
        editProjectNameCN->setText(modelCombProjectList->item(row, 3)->text());

        QStringList pList = modelCombProjectList->item(row, 2)->text().split("+"); //常规项目名称
        int pCount = gParameter->get_ProjectName().count(); //常规项目的个数


        //清空项目选择
        for(int j = 0; j < pCount; j ++)
            rdoRoutineItem[j]->setChecked(false);


        for(int i = 0; i < pList.count(); i++)
        {
            for(int j = 0; j < pCount; j ++)
            {
                if( rdoRoutineItem[j]->text().trimmed() == pList.at(i) )
                    rdoRoutineItem[j]->setChecked(true);
            }
        }

    }
}

