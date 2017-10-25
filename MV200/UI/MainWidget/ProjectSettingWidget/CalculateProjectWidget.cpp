#include "CalculateProjectWidget.h"

//实例对象
CalculateProjectWidget *CalculateProjectWidget::_instance = 0;
CalculateProjectWidget * CalculateProjectWidget::getInstance()
{
    if (_instance == 0)
        _instance = new CalculateProjectWidget();
    return _instance;
}

//构造函数
CalculateProjectWidget::CalculateProjectWidget(QWidget *parent) : QWidget(parent)
{
    initForm();                 //初始化
    initTableForm();            //格式化表格
    initConnect();              //连接信号与槽
    initStyle();                //界面风格
    initData();                 //初始化数据
}

//初始化
void CalculateProjectWidget::initForm()
{
    //项目清单
    grbCalculateProjectList = new QGroupBox("计算项目清单");
    tbvCalculateProjectList = new QTableView;
    modelCalculateProjectList = new QStandardItemModel;
    lytCalculateProjectList = new QGridLayout;
    lytCalculateProjectList->setContentsMargins(0, 20, 0, 0);
    lytCalculateProjectList->setSpacing(0);
    lytCalculateProjectList->addWidget(tbvCalculateProjectList, 2, 0, 100, 50);
    grbCalculateProjectList->setLayout(lytCalculateProjectList);
    tbvCalculateProjectList->setModel(modelCalculateProjectList);

    //项目
    grbCalculateInfo = new QGroupBox("计算项目信息");      //项目信息
    labProjectNameEN = new QLabel("   简称:");    //项目名称（英文简称）
    editProjectNameEN = new QLineEdit;
    labProjectNameCN = new QLabel("全称:");    //项目名称（中文全称）
    editProjectNameCN = new QLineEdit;
    labUnit = new QLabel("项目单位:");             //项目单位
    editUnit = new QLineEdit;
    labAccuracy = new QLabel("结果精度:");         //结果精度
    //editAccuracy = new QLineEdit;
    cboAccuracy = new QComboBox;
    labRange = new QLabel("参考范围:");            //参考范围
    editRange1 = new QLineEdit;
    editRange2 = new QLineEdit;
    lytCalculateInfo = new QGridLayout;
    lytCalculateInfo->setMargin(10);
    lytCalculateInfo->setSpacing(10);
    lytCalculateInfo->addWidget(labProjectNameEN,  0,  0, 2,  5, Qt::AlignRight);
    lytCalculateInfo->addWidget(editProjectNameEN, 0,  5, 2, 10, Qt::AlignLeft);
    lytCalculateInfo->addWidget(labProjectNameCN,  0, 15, 2, 5, Qt::AlignRight);
    lytCalculateInfo->addWidget(editProjectNameCN, 0, 25, 2, 10, Qt::AlignLeft);
    lytCalculateInfo->addWidget(labUnit,           0, 35, 2, 5, Qt::AlignRight);
    lytCalculateInfo->addWidget(editUnit,          0, 40, 2, 10, Qt::AlignLeft);
    lytCalculateInfo->addWidget(labAccuracy,       2,  0, 2,  5, Qt::AlignRight);
    lytCalculateInfo->addWidget(cboAccuracy,       2,  5, 2, 10, Qt::AlignLeft);
    lytCalculateInfo->addWidget(labRange,          2, 15, 2, 5, Qt::AlignRight);
    lytCalculateInfo->addWidget(editRange1,        2, 25, 2, 10);
    lytCalculateInfo->addWidget(new QLabel("-"),   2, 35, 2, 1);
    lytCalculateInfo->addWidget(editRange2,        2, 36, 2, 14, Qt::AlignLeft);
    grbCalculateInfo->setLayout(lytCalculateInfo);

    //表达式显示区
    grbExpression = new QGroupBox("表达式显示区");   //表达式显示区
    labExpression = new QLabel("表达式:");
    editExpression = new QLineEdit;
    hlytExpression = new QHBoxLayout;
    hlytExpression->setContentsMargins(0, 18, 0, 5);
    hlytExpression->addWidget(labExpression);
    hlytExpression->addWidget(editExpression);
    grbExpression->setLayout(hlytExpression);

    //计算功能键显示区
    grbCalculate = new QGroupBox("计算功能键显示区"); //计算功能键显示区
    lytCalculate = new QGridLayout;
    lytCalculate->setContentsMargins(0, 18, 0, 5);
    lytCalculate->setSpacing(0);
    QString str = "0123456789+-*/.()?";
    for(int i = 0; i < CALCULATE_BTN_COUNT; i ++)
    {
        btnCalculate[i] = new QPushButton(str.mid(i,1));
        if(i >= str.count()) break; //要new但不加入到layout中去，
        if(i<10)
            lytCalculate->addWidget( btnCalculate[i], 0, i*3, 1, 3);
        else
            lytCalculate->addWidget( btnCalculate[i], 1, (i-10)*3, 1, 3);
    }
    grbCalculate->setLayout(lytCalculate);
    btnCalculate[17]->setText("删除");
    sigMapper = new QSignalMapper(this);

    //项目选择区
    grbProjectList = new QGroupBox("项目选择区");   //项目选择区
    lytProjectList = new QGridLayout;
    lytProjectList->setContentsMargins(0, 18, 0, 5);
    lytProjectList->setSpacing(5);
    for(int i = 0; i < PROJECT_COUNT; i ++)
    {
        rdoRoutineItem[i] = new QRadioButton(" none");     //常规项目
        rdoRoutineItem[i]->setObjectName("rdo2");
        //rdoRoutineItem[i]->setAutoExclusive(false);
        rdoRoutineItem[i]->setVisible(false);
        lytProjectList->addWidget(rdoRoutineItem[i], i/8*3, i%8*5, 3, 5);
    }
    grbProjectList->setLayout(lytProjectList);
    sigMapperProject = new QSignalMapper(this);

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

    wdgCalculateProject = new QWidget;
    lytCalculateProject = new QGridLayout;
    lytCalculateProject->setMargin(10);
    lytCalculateProject->setSpacing(10);
    lytCalculateProject->addWidget(grbCalculateInfo,  0, 0, 15, 50);
    lytCalculateProject->addWidget(grbExpression,    15, 0,  5, 50);
    lytCalculateProject->addWidget(grbCalculate,     20, 0, 15, 50);
    lytCalculateProject->addWidget(grbProjectList,   35, 0, 55, 50);
    lytCalculateProject->addWidget(wdgButtonList,    90, 0, 10, 50);
    wdgCalculateProject->setLayout(lytCalculateProject);

    //
    layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(10);
    layout->addWidget(grbCalculateProjectList);
    layout->addWidget(wdgCalculateProject);
    this->setLayout(layout);
}

//格式化表格
void CalculateProjectWidget::initTableForm()
{
    int tbvRowCount = CALCULATE_PROJECT_COUNT+1; //列表最大能显示的行数
    //列表
    QStringList headerList_h;
    headerList_h << "id" << "项目名称" << "表达式";
    modelCalculateProjectList->setHorizontalHeaderLabels( headerList_h );
    int row = 0;
    tbvCalculateProjectList->hideColumn(row ++);             //id-隐藏
    tbvCalculateProjectList->setColumnWidth( row ++, 100);   //项目名称
    tbvCalculateProjectList->setColumnWidth( row ++, 200);   //表达式

    tbvCalculateProjectList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvCalculateProjectList->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvCalculateProjectList->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvCalculateProjectList->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
    tbvCalculateProjectList->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    //tbvCalculateProjectList->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
    tbvCalculateProjectList->horizontalHeader()->setStyleSheet("font-size:18px;");         //表头字体样式

    //初始化每一个单元格
    for( int i = 0; i < tbvRowCount; i ++ )
    {
        modelCalculateProjectList->setItem( i, 0, new QStandardItem( "" ));  //id(隐藏)
        modelCalculateProjectList->setItem( i, 1, new QStandardItem( "" ));  //
        modelCalculateProjectList->setItem( i, 2, new QStandardItem( "" ));  //

        //列表内容显示方式
        modelCalculateProjectList->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        modelCalculateProjectList->item(i, 2)->setTextAlignment(Qt::AlignCenter);
    }
}

//界面风格
void CalculateProjectWidget::initStyle()
{
    editExpression->setReadOnly(true);
    editExpression->setFixedWidth(850);

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
void CalculateProjectWidget::initConnect()
{
//    sigMapper->setMapping(btnSample4[1], "0-2" ); //急诊样本2
//    connect(btnSample4[1],SIGNAL(clicked()),sigMapper,SLOT(map()));
//    connect(sigMapper,SIGNAL(mapped(const QString&)), this,SLOT(slt_sigMapReceive(const QString&)));


    //计算功能键
    for(int i = 0; i < CALCULATE_BTN_COUNT; i ++)
    {
        //不使用的功能键不连接
        if(btnCalculate[i]->text() == "") break;

        sigMapper->setMapping(btnCalculate[i], i);
        //connect(btnCalculate[i], SIGNAL(released()), sigMapper, SLOT(map()));
        connect(btnCalculate[i], SIGNAL(clicked()), sigMapper, SLOT(map()));
    }
    connect(sigMapper, SIGNAL(mapped(int)), this, SLOT(slt_sigMapper_Handle(int)));

    //项目选择
    for(int i = 0; i < PROJECT_COUNT; i ++)
    {
        sigMapperProject->setMapping(rdoRoutineItem[i], i);
        connect(rdoRoutineItem[i], SIGNAL(clicked(bool)), sigMapperProject, SLOT(map()));
    }
    connect(sigMapperProject, SIGNAL(mapped(int)), this, SLOT(slt_sigMapperProject_Handle(int)));

    connect(gParameter, SIGNAL(sig_newProject()), this, SLOT(slt_UpdateRoutineItem()));

    connect(btnAddNew, SIGNAL(clicked(bool)), this, SLOT(slt_btnAddNew_Clicked()));
    connect(btnModify, SIGNAL(clicked(bool)), this, SLOT(slt_btnModify_Clicked()));
    connect(btnDelete, SIGNAL(clicked(bool)), this, SLOT(slt_btnDelete_Clicked()));
    connect(btnCancle, SIGNAL(clicked(bool)), this, SLOT(slt_btnCancle_Clicked()));
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(slt_btnSave_Clicked()));

    connect(tbvCalculateProjectList, SIGNAL(clicked(QModelIndex)), this, SLOT(slt_tbvSelectRow_info(QModelIndex)));
}

//初始化数据
void CalculateProjectWidget::initData()
{
    ProjectList.clear();
    ExpressionList.clear();

    //绑定组合项目到列表中
    BindingCombProject();

    //槽：更新项目选择区中的项目（常规项目）
    slt_UpdateRoutineItem();

    //
    QStringList list;
    list << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8";
    cboAccuracy->addItems(list);
}

//绑定组合项目到列表中
void CalculateProjectWidget::BindingCombProject()
{
    QString str = "SELECT id, nameEn, nameCn, unit, accuracy, expression, expression_n, "
                  "rangeA1, rangeA2, rangeB1, rangeB2, rangeC1, rangeC2 from CalculateProjectT where id > 1;";
    QSqlQuery query = SQLite::getInstance()->Select(str);
    int row = 0;
    while (query.next())
    {
        modelCalculateProjectList->item(row, 0)->setText( QString::number(query.value(0).toInt()));
        modelCalculateProjectList->item(row, 1)->setText( query.value(1).toString());
        modelCalculateProjectList->item(row, 2)->setText( query.value(5).toString());
        row ++;
    }

    for(row; row < modelCalculateProjectList->rowCount(); row ++)
    {
        modelCalculateProjectList->item(row, 0)->setText("");
        modelCalculateProjectList->item(row, 1)->setText("");
        modelCalculateProjectList->item(row, 2)->setText("");
    }
}


//槽：更新项目选择区中的项目（常规项目）
void CalculateProjectWidget::slt_UpdateRoutineItem()
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
            rdoRoutineItem[i]->setText(routineP.at(i));
        }
        else
        {
            rdoRoutineItem[i]->setText("");
            rdoRoutineItem[i]->setChecked(false);
            rdoRoutineItem[i]->setVisible(false);        //默认隐藏
        }
    }
}

//槽：计算功能键信号转发器
void CalculateProjectWidget::slt_sigMapper_Handle(int index)
{
    //越界判断
    if(index < 0) return;
    if(index >= CALCULATE_BTN_COUNT) return;

    if(index == CALCULATE_BTN_COUNT-3) //删除键
    {
        if(ExpressionList.isEmpty()) //表达式为空，不能删除
        {
            ProjectList.clear();
            return;
        }

        ExpressionList.removeLast();
        QString text;
        for(int i = 0; i < ExpressionList.count(); i ++)
            text += ExpressionList.at(i);
        editExpression->setText(text);

        if(ExpressionList.isEmpty())
            ProjectList.clear();
        else
        {
            int lastProjectCount = ProjectList.left(ProjectList.count() - 1).split("+").last().count(); //项目列表中最后一个项目名称的长度
            ProjectList = ProjectList.left(ProjectList.count() - lastProjectCount -1); //"-1"去掉后面的"+"连接符
        }
    }
    else
    {
        QString name = btnCalculate[index]->text().trimmed();
        ExpressionList.append(name);
        editExpression->setText(editExpression->text() + name );
    }
}

//槽：项目选择键信号转发器
void CalculateProjectWidget::slt_sigMapperProject_Handle(int index)
{
    //越界判断
    if(index < 0) return;
    if(index >= PROJECT_COUNT) return;

    QString name = "{" + rdoRoutineItem[index]->text().trimmed() + "}";
    ExpressionList.append(name);
    editExpression->setText(editExpression->text() + name );

    ProjectList += rdoRoutineItem[index]->text().trimmed() + "+";
}

//槽：新增
void CalculateProjectWidget::slt_btnAddNew_Clicked()
{
    is_AddNew_flag = 1;    //标志，新增/修改标志，（0：修改，1：新增）
    btnAddNew->setEnabled(false);
    btnModify->setEnabled(false);
    btnDelete->setEnabled(false);
    btnCancle->setEnabled(true);
    btnSave->setEnabled(true);

    editProjectNameCN->clear();
    editProjectNameEN->clear();
    editUnit->clear();
    //editAccuracy->clear();
    cboAccuracy->setCurrentIndex(0);
    editRange1->clear();
    editRange2->clear();
    editExpression->clear();
    ProjectList.clear();

    for(int i = 0; i < PROJECT_COUNT; i ++)
        rdoRoutineItem[i]->setChecked(false);
}
//槽：修改
void CalculateProjectWidget::slt_btnModify_Clicked()
{
    is_AddNew_flag = 0;    //标志，新增/修改标志，（0：修改，1：新增）
    btnAddNew->setEnabled(false);
    btnModify->setEnabled(false);
    btnDelete->setEnabled(false);
    btnCancle->setEnabled(true);
    btnSave->setEnabled(true);
}
//槽：删除
void CalculateProjectWidget::slt_btnDelete_Clicked()
{
    int id = modelCalculateProjectList->item(tbvCalculateProjectList->currentIndex().row(), 0)->text().toInt();

    if(id > 0)
    {
        QString name = modelCalculateProjectList->item(tbvCalculateProjectList->currentIndex().row(), 1)->text(); //项目名称
        gMessageBox->setInfo(QString("删除提示"), QString("\n是否要删除计算项目“" + name +"”？"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, false);
        gMessageBox->setButtonText(Cancel, "否(N)");
        gMessageBox->setButtonText(Sure, "是(Y)");
        if(gMessageBox->exec() == MVMessageBox::Accepted)
        {
            QString str  = "delete from CalculateProjectT where id="+ QString::number(id) +";";
            SQLite::getInstance()->Delete(str);

            //移除项目
            gParameter->removeCalculateProject(id);
            BindingCombProject();
        }
    }
}
//槽：取消
void CalculateProjectWidget::slt_btnCancle_Clicked()
{
    btnAddNew->setEnabled(true);
    btnModify->setEnabled(true);
    btnDelete->setEnabled(true);
    btnCancle->setEnabled(false);
    btnSave->setEnabled(false);

    if(is_AddNew_flag == 1)
    {
        editProjectNameCN->clear();
        editProjectNameEN->clear();
        editUnit->clear();
        //editAccuracy->clear();
        cboAccuracy->setCurrentIndex(0);
        editRange1->clear();
        editRange2->clear();
        editExpression->clear();

        for(int i = 0; i < PROJECT_COUNT; i ++)
            rdoRoutineItem[i]->setChecked(false);
    }
}
//槽：保存
void CalculateProjectWidget::slt_btnSave_Clicked()
{
    QString nameEn = editProjectNameEN->text().trimmed();
    QString nameCn = editProjectNameCN->text().trimmed();
    QString unit = editUnit->text().trimmed();
    QString accuracy = cboAccuracy->currentText().trimmed();
    QString range1 = editRange1->text().trimmed();
    QString range2 = editRange2->text().trimmed();
    QString expression = editExpression->text().trimmed();

    //空值提示
    if(nameEn == "")
    {
        gMessageBox->setInfo(QString("警告"), QString("\n项目简称不能为空！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
        return;
    }
    else if ( nameCn == "" )
    {
        gMessageBox->setInfo(QString("警告"), QString("\n项目全称不能为空！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
        return;
    }
    if ( unit == "" )
    {
        gMessageBox->setInfo(QString("警告"), QString("\n项目单位不能为空！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
        return;
    }
    if ( range1 == "" )
    {
        gMessageBox->setInfo(QString("警告"), QString("\n参考范围不能为空！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
        return;
    }
    if ( range2 == "" )
    {
        gMessageBox->setInfo(QString("警告"), QString("\n参考范围不能为空！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
        return;
    }
    if ( expression == "" )
    {
        gMessageBox->setInfo(QString("警告"), QString("\n表达式不能为空！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
        return;
    }


    //计算项目参数
    CalculateProject param;
    param.CalculateProjectNameEn = nameEn;
    param.CalculateProjectNameCn = nameCn;
    param.CalculateProjectExpression = expression;
    param.Unit = unit;               //项目单位
    param.Accracy = accuracy.toInt();//小数位数
    param.Range1 = range1.toDouble();//参考范围1
    param.Range2 = range2.toDouble();//参考范围2
    param.ProjectList = ProjectList;

    if(is_AddNew_flag)  //新增
    {
        //计算项目是否已经达到了上限
        if(gParameter->getCalculateProjectName().count() < CALCULATE_PROJECT_COUNT)
        {
            //判断是否已经存在
            if(gParameter->isCalculateProjectExist(nameEn))
            {
                gMessageBox->setInfo(QString("警告"), QString("新增计算项目“"+ nameEn +"”已经存在!"),
                                     QPixmap("://resource/resource/MessageBox/information.png"), true, true);
                gMessageBox->setButtonText(Cancel, "返回");
                gMessageBox->show();
                return;
            }
            else
            {
                int id = 1;
                QString cMaxId = "SELECT MAX(id) from CalculateProjectT;";
                QSqlQuery query = SQLite::getInstance()->Select(cMaxId);
                while (query.next()) {
                    id = query.value(0).toInt() + 1;
                }

                QString str = "insert into CalculateProjectT(id, nameEn, nameCn, unit, accuracy, projectList, expression, rangeA1, rangeA2) values("
                        + QString::number(id) + ", '"+ nameEn +"', '"+ nameCn +"', '" + unit + "' ,"+ accuracy +","
                        + "'" + ProjectList +"', '"+ expression + "', '"+ range1  + "', '" + range2 + "');";
                SQLite::getInstance()->Insert(str);
                //qDebug() << str;
                //新增
                gParameter->setCalculateProject(id, param);

                //绑定项目到列表中
                BindingCombProject();
            }
        }
        else //新增不成功，组合项目已经达到上限了
        {
            gMessageBox->setInfo(QString("警告"), QString("计算项目已经达到上限!\n\n请删除无效项目后再保存"),
                                 QPixmap("://resource/resource/MessageBox/information.png"), true, true);
            gMessageBox->setButtonText(Cancel, "返回");
            gMessageBox->show();
        }
    }
    else //修改
    {
        int id = modelCalculateProjectList->item( tbvCalculateProjectList->currentIndex().row(), 0)->text().toInt();
        QString str = "update CalculateProjectT set nameEn='" +nameEn+ "', nameCn='" +nameCn+ "', expression='"  + expression +"', "
                +" unit='" +unit+ "', accuracy=" +accuracy + ", projectList='"+ ProjectList + "', rangeA1='" + range1 +"', rangeA2='" + range2 + "' where id="+ QString::number(id) +";" ;
        SQLite::getInstance()->Update(str);

       // qDebug() << str;
        //修改
        gParameter->setCalculateProject(id, param);
        BindingCombProject();
    }

    btnAddNew->setEnabled(true);
    btnModify->setEnabled(true);
    btnDelete->setEnabled(true);
    btnCancle->setEnabled(false);
    btnSave->setEnabled(false);
    ProjectList.clear();
}

//槽：更新选中组合项目的信息到右侧界面
void CalculateProjectWidget::slt_tbvSelectRow_info(QModelIndex index)
{
    //<< "id" << "项目名称" << "项目成员" << "nameCn" << "member_idList";
    int row = index.row();
    int id = modelCalculateProjectList->item(row, 0)->text().toInt();
    if(id > 0)
    {
        //计算项目参数
        CalculateProject param = gParameter->getCalculateProjectParam(id);
        if(param.CalculateProjectNameEn == "") return; //判断参数是否为空

        editProjectNameEN->setText(param.CalculateProjectNameEn);
        editProjectNameCN->setText(param.CalculateProjectNameCn);
        editUnit->setText(param.Unit);
        //editAccuracy->setText(QString::number(param.Accracy));
        cboAccuracy->setCurrentIndex(param.Accracy);
        editRange1->setText(QString::number(param.Range1));
        editRange2->setText(QString::number(param.Range2));
        editExpression->setText(param.CalculateProjectExpression);
        ProjectList = param.ProjectList;

        ExpressionList.clear();
        ExpressionList.append(param.CalculateProjectExpression);

        //清空项目选择
        int pCount = gParameter->get_ProjectName().count(); //常规项目的个数
        for(int j = 0; j < pCount; j ++)
            rdoRoutineItem[j]->setChecked(false);
    }
}



