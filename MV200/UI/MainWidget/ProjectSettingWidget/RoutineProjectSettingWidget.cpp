#include "RoutineProjectSettingWidget.h"

//实例对象
RoutineProjectSettingWidget *RoutineProjectSettingWidget::_instance = 0;
RoutineProjectSettingWidget * RoutineProjectSettingWidget::getInstance()
{
    if (_instance == 0)
        _instance = new RoutineProjectSettingWidget();
    return _instance;
}

RoutineProjectSettingWidget::RoutineProjectSettingWidget(QWidget *parent)
    : QWidget(parent)
{
    isAdd_flag = 0;              //是否增加标志
    isUpdate_flag = 0;           //是否修改标志

    initForm();          //初始化
    initTbvRoutineProject(); //常规项目列表
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}

//初始化
void RoutineProjectSettingWidget::initForm()
{
    readCareWidget = new ReadCareWidget;
    readCareWidget->setModal(true);  //使用模态窗口

    initBasicParamForm();    //基本参数
    QWidget *wdg = new QWidget;
    QVBoxLayout *lyt = new QVBoxLayout;
    lyt->setMargin(0);
    lyt->setSpacing(0);
    lyt->addWidget(wdgBasicParam);
    wdg->setLayout(lyt);

    initQCParamForm();       //质控参数
    initButtonForm();

    //常规项目
    tbvRoutineProject = new QTableView;
    modelRoutineProject = new QStandardItemModel;
    tbvRoutineProject->setModel(modelRoutineProject);

    //参数设置
    tabParam = new QTabWidget;
    tabParam->addTab(wdg, "基本参数");
    //tabParam->addTab(new QWidget, "质控参数");

    //总布局
    layout = new QGridLayout;
    layout->setMargin(3);
    layout->setSpacing(0);
    layout->addWidget(tbvRoutineProject, 0,  0, 100, 20);
    layout->addWidget(tabParam,          0, 20,  90, 80, Qt::AlignLeft);
    layout->addWidget(wdgButton,        90, 20,  10, 80, Qt::AlignLeft);
    this->setLayout(layout);
}

//基本参数
void RoutineProjectSettingWidget::initBasicParamForm()
{
    //基本参数
    labProjectName0 = new QLabel("项目简称:");     //项目简称
    editProjectName0 = new QLineEdit;
    labProjectName = new QLabel("项目名称:");      //项目名称
    editProjectName = new QLineEdit;
    labBatchNum = new QLabel("批号:");             //批号
    editBatchNum = new QLineEdit;
    labAccuracy = new QLabel("结果精度:");         //结果精度
    cboAccuracy = new QComboBox;
    labAccuracy2 = new QLabel("位  ");            //结果单位小数几位
    labIsDiluent = new QLabel("是否要稀释:");      //是否要稀释
    cboIsDiluent = new QComboBox;
    labUnit = new QLabel("结果单位:");             //结果单位
    editUnit = new QLineEdit;
    labParam_K = new QLabel("K因数值:");          //K因数值
    editParam_K = new QLineEdit;
    labAllowanceWarm = new QLabel("余量报警(次):");    //余量报警(次)
    editAllowanceWarm = new QLineEdit;
    labBloodFactor = new QLabel("全血因子:");      //全血因子
    editBloodFactor = new QLineEdit;
    labLowLinear = new QLabel("线性下限:");
    editLowLinear = new QLineEdit;

    //加样量设置
    grbAddingAmount = new QGroupBox("加样量设置(ul)");
    lytAddingAmount = new QGridLayout;
    labDiluent = new QLabel("稀释液:");          //稀释液
    editDiluent = new QLineEdit;
    labSample = new QLabel("样本:");             //样本
    labS_Transfer = new QLabel("样本转移量:");    //样本转移量
    editS_Transfer = new QLineEdit;
    editSample = new QLineEdit;
    labR1 = new QLabel("R1:");               //R1
    editR1 = new QLineEdit;
    labR2 = new QLabel("R2:");               //R2
    editR2 = new QLineEdit;
    lytAddingAmount->setMargin(5);
    lytAddingAmount->setSpacing(2);
    lytAddingAmount->addWidget(labR1,         2,  0, 2, 5, Qt::AlignRight);
    lytAddingAmount->addWidget(editR1,        2,  5, 2, 5, Qt::AlignLeft);
    lytAddingAmount->addWidget(labSample,          2, 10, 2, 5, Qt::AlignRight);
    lytAddingAmount->addWidget(editSample,         2, 15, 2, 5, Qt::AlignLeft);
    lytAddingAmount->addWidget(labR2,              2, 20, 2, 5, Qt::AlignRight);
    lytAddingAmount->addWidget(editR2,             2, 25, 2, 5, Qt::AlignLeft);
    lytAddingAmount->addWidget(labDiluent,      4,  0, 2, 5, Qt::AlignRight);
    lytAddingAmount->addWidget(editDiluent,     4,  5, 2, 5, Qt::AlignLeft);
    lytAddingAmount->addWidget(labS_Transfer,              4, 10, 2, 5, Qt::AlignRight);
    lytAddingAmount->addWidget(editS_Transfer,             4, 15, 2, 5, Qt::AlignLeft);
    grbAddingAmount->setLayout(lytAddingAmount);

    //项目定标参数
    grbCalibrateParam_1 = new QGroupBox("参数1");
    lytCalibrateParam_1 = new QGridLayout;
    chkCalibrateParam_1 = new QCheckBox("参数");  //参数是否启用
    lab_R0_1 = new QLabel("R0:");
    edit_R0_1 = new QLineEdit;
    lab_K_1 = new QLabel("K:");
    edit_K_1 = new QLineEdit;
    lab_a_1 = new QLabel("a:");
    edit_a_1 = new QLineEdit;
    lab_b_1 = new QLabel("b:");
    edit_b_1 = new QLineEdit;
    lab_c_1 = new QLabel("c:");
    edit_c_1 = new QLineEdit;
    labCalibrateParam_R_1 = new QLabel("最大幅度:");     //第一段曲线反应幅度分段点
    editCalibrateParam_R_1 = new QLineEdit;
    labCalibrateParam_C_1 = new QLabel("最高浓度:");     //第一段曲线浓度分段点
    editCalibrateParam_C_1 = new QLineEdit;
    lytCalibrateParam_1->setMargin(5);
    lytCalibrateParam_1->setSpacing(2);
    lytCalibrateParam_1->addWidget(chkCalibrateParam_1,   16, 0, 2, 5, Qt::AlignCenter);
    lytCalibrateParam_1->addWidget(lab_R0_1,   14,  5, 2, 5, Qt::AlignRight);
    lytCalibrateParam_1->addWidget(edit_R0_1,  14, 10, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_1->addWidget(lab_K_1,    14, 15, 2, 5, Qt::AlignRight);
    lytCalibrateParam_1->addWidget(edit_K_1,   14, 20, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_1->addWidget(lab_a_1,    16,  5, 2, 5, Qt::AlignRight);
    lytCalibrateParam_1->addWidget(edit_a_1,   16, 10, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_1->addWidget(lab_b_1,    16, 15, 2, 5, Qt::AlignRight);
    lytCalibrateParam_1->addWidget(edit_b_1,   16, 20, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_1->addWidget(lab_c_1,    16, 25, 2, 5, Qt::AlignRight);
    lytCalibrateParam_1->addWidget(edit_c_1,   16, 30, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_1->addWidget(labCalibrateParam_R_1,    18,  5, 2, 5, Qt::AlignRight);
    lytCalibrateParam_1->addWidget(editCalibrateParam_R_1,   18, 10, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_1->addWidget(labCalibrateParam_C_1,    18, 15, 2, 5, Qt::AlignRight);
    lytCalibrateParam_1->addWidget(editCalibrateParam_C_1,   18, 20, 2, 5, Qt::AlignLeft);
    grbCalibrateParam_1->setLayout(lytCalibrateParam_1);

    grbCalibrateParam_2 = new QGroupBox("参数2");
    lytCalibrateParam_2 = new QGridLayout;
    chkCalibrateParam_2 = new QCheckBox("参数");  //参数是否启用
    lab_R0_2 = new QLabel("R0:");
    edit_R0_2 = new QLineEdit;
    lab_K_2 = new QLabel("K:");
    edit_K_2 = new QLineEdit;
    lab_a_2 = new QLabel("a:");
    edit_a_2 = new QLineEdit;
    lab_b_2 = new QLabel("b:");
    edit_b_2 = new QLineEdit;
    lab_c_2 = new QLabel("c:");
    edit_c_2 = new QLineEdit;
    labCalibrateParam_R_2 = new QLabel("最大幅度:");     //第二段曲线反应幅度分段点
    editCalibrateParam_R_2 = new QLineEdit;
    labCalibrateParam_C_2 = new QLabel("最高浓度:");     //第二段曲线浓度分段点
    editCalibrateParam_C_2 = new QLineEdit;
    lytCalibrateParam_2->setMargin(5);
    lytCalibrateParam_2->setSpacing(2);
    lytCalibrateParam_2->addWidget(chkCalibrateParam_2,   16, 0, 2, 5, Qt::AlignCenter);
    lytCalibrateParam_2->addWidget(lab_R0_2,   14,  5, 2, 5, Qt::AlignRight);
    lytCalibrateParam_2->addWidget(edit_R0_2,  14, 10, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_2->addWidget(lab_K_2,    14, 15, 2, 5, Qt::AlignRight);
    lytCalibrateParam_2->addWidget(edit_K_2,   14, 20, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_2->addWidget(lab_a_2,    16,  5, 2, 5, Qt::AlignRight);
    lytCalibrateParam_2->addWidget(edit_a_2,   16, 10, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_2->addWidget(lab_b_2,    16, 15, 2, 5, Qt::AlignRight);
    lytCalibrateParam_2->addWidget(edit_b_2,   16, 20, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_2->addWidget(lab_c_2,    16, 25, 2, 5, Qt::AlignRight);
    lytCalibrateParam_2->addWidget(edit_c_2,   16, 30, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_2->addWidget(labCalibrateParam_R_2,    18,  5, 2, 5, Qt::AlignRight);
    lytCalibrateParam_2->addWidget(editCalibrateParam_R_2,   18, 10, 2, 5, Qt::AlignLeft);
    lytCalibrateParam_2->addWidget(labCalibrateParam_C_2,    18, 15, 2, 5, Qt::AlignRight);
    lytCalibrateParam_2->addWidget(editCalibrateParam_C_2,   18, 20, 2, 5, Qt::AlignLeft);
    grbCalibrateParam_2->setLayout(lytCalibrateParam_2);

    labReactionCycle = new QLabel("反应周期(个数):");    //反应周期
    editCycle1 = new QLineEdit;
    editCycle2 = new QLineEdit;
    editTime1 = new QLineEdit;
    editTime2 = new QLineEdit;

    lytBasicParam = new QGridLayout;
    lytBasicParam->setMargin(10);
    lytBasicParam->setSpacing(2);
    //lytBasicParam->addWidget(labProjectName0,    0,  0, 2, 5, Qt::AlignRight);
    //lytBasicParam->addWidget(editProjectName0,   0,  5, 2, 5, Qt::AlignLeft);
    lytBasicParam->addWidget(labProjectName,     0,  0, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(editProjectName,    0,  5, 2, 5, Qt::AlignLeft);
    lytBasicParam->addWidget(labBatchNum,        0, 10, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(editBatchNum,       0, 15, 2, 5, Qt::AlignLeft);
    lytBasicParam->addWidget(labIsDiluent,       2,  0, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(cboIsDiluent,       2,  5, 2, 5, Qt::AlignLeft);
    lytBasicParam->addWidget(labAccuracy,        2, 10, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(cboAccuracy,        2, 15, 2, 5, Qt::AlignLeft);
    lytBasicParam->addWidget(labAccuracy2,       2, 15, 2, 5, Qt::AlignRight);

    lytBasicParam->addWidget(labUnit,            2, 20, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(editUnit,           2, 25, 2, 5, Qt::AlignLeft);
    lytBasicParam->addWidget(grbAddingAmount,    4,  0, 6, 30);
    lytBasicParam->addWidget(labReactionCycle,  10,  0, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(editCycle1,        10,  5, 2, 5, Qt::AlignCenter);
    lytBasicParam->addWidget(editTime1,         10, 10, 2, 5, Qt::AlignCenter);
    lytBasicParam->addWidget(new QLabel("~"),   10, 15, 2, 5, Qt::AlignCenter);
    lytBasicParam->addWidget(editCycle2,        10, 20, 2, 5, Qt::AlignCenter);
    lytBasicParam->addWidget(editTime2,         10, 25, 2, 5, Qt::AlignCenter);
    lytBasicParam->addWidget(labParam_K,        12,  0, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(editParam_K,       12,  5, 2, 5, Qt::AlignLeft);
    lytBasicParam->addWidget(labBloodFactor,    12, 10, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(editBloodFactor,   12, 15, 2, 5, Qt::AlignLeft);
    lytBasicParam->addWidget(labLowLinear,      12, 20, 2, 5, Qt::AlignRight);
    lytBasicParam->addWidget(editLowLinear,     12, 25, 2, 5, Qt::AlignLeft);
    lytBasicParam->addWidget(grbCalibrateParam_1,     16, 0, 6, 30);
    lytBasicParam->addWidget(grbCalibrateParam_2,     22, 0, 6, 30);

    wdgBasicParam = new QWidget;
    wdgBasicParam->setLayout(lytBasicParam);
}

//质控参数
void RoutineProjectSettingWidget::initQCParamForm()
{

}

//功能按钮
void RoutineProjectSettingWidget::initButtonForm()
{
    //功能按钮
    wdgButton = new QWidget;
    lytButton = new QGridLayout;
    lytButton->setMargin(0);
    lytButton->setSpacing(10);
    btnAdd = new QPushButton("新增项目");
    btnUpdate = new QPushButton("修改项目参数");
    btnDelete = new QPushButton("删除");
    btnCancel = new QPushButton("取消");
    btnSave = new QPushButton("保存");
    btnReadCard = new QPushButton("读卡");
    btnSaveParam_K = new QPushButton("保存参数");  //保存参数（K因数值）
    lytButton->addWidget(btnDelete,    0, 20, 5, 10, Qt::AlignCenter);

    //Build类型（发布版、研发版）
    if( !gParameter->get_isRelease() ) //0:研发版
    {
        lytButton->addWidget(btnAdd,       0,  0, 5, 10, Qt::AlignCenter);
        lytButton->addWidget(btnUpdate,    0, 10, 5, 10, Qt::AlignCenter);
        lytButton->addWidget(btnCancel,    0, 30, 5, 10, Qt::AlignCenter);
        lytButton->addWidget(btnSave,      0, 40, 5, 10, Qt::AlignCenter);
    }
    lytButton->addWidget(btnSaveParam_K,  0, 50, 5, 10, Qt::AlignCenter);
    lytButton->addWidget(btnReadCard,     0, 60, 5, 10, Qt::AlignCenter);
    wdgButton->setLayout(lytButton);
}

//常规项目列表
void RoutineProjectSettingWidget::initTbvRoutineProject()
{
    //int tbvRowCount = gParameter->getProjectCount(); //列表最大能显示的行数
    //列表
    QStringList headerList_h;
    headerList_h << "id" << "项目";
    modelRoutineProject->clear();
    modelRoutineProject->setHorizontalHeaderLabels( headerList_h );
    tbvRoutineProject->hideColumn(0);             //id-隐藏
    tbvRoutineProject->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvRoutineProject->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvRoutineProject->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvRoutineProject->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
    tbvRoutineProject->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    //tbvRoutineProject->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动
}

//界面风格
void RoutineProjectSettingWidget::initStyle()
{
    //功能按钮
    btnAdd->setObjectName("button5");
    btnUpdate->setObjectName("button5");
    btnDelete->setObjectName("button5");
    btnCancel->setObjectName("button5");
    btnSave->setObjectName("button5");
    btnReadCard->setObjectName("button5");
    btnSaveParam_K->setObjectName("button5");
    tbvRoutineProject->setObjectName("tableV");
    tabParam->setObjectName("tabW");
    cboAccuracy->setFixedWidth(80);

    editTime1->setReadOnly(true);
    editTime2->setReadOnly(true);
    chkCalibrateParam_1->setEnabled(false);
    chkCalibrateParam_1->setChecked(true);
    chkCalibrateParam_2->setChecked(false);

    btnAdd->setEnabled(true);
    btnUpdate->setEnabled(true);
    btnDelete->setEnabled(true);
    btnCancel->setEnabled(false);
    btnSave->setEnabled(false);
    btnSaveParam_K->setEnabled(true);
    btnReadCard->setEnabled(true);

    //Build类型（发布版、研发版）
    if( gParameter->get_isRelease() ) //发布版
    {
        editProjectName->setReadOnly(true);
        editProjectName0->setReadOnly(true);
        editBatchNum->setReadOnly(true);
        cboIsDiluent->setEnabled(false);
        cboAccuracy->setEnabled(false);
        editUnit->setReadOnly(true);
        editDiluent->setReadOnly(true);
        editR1->setReadOnly(true);
        editR2->setReadOnly(true);
        editSample->setReadOnly(true);
        editS_Transfer->setReadOnly(true);
        editCycle1->setReadOnly(true);
        editCycle2->setReadOnly(true);
        editTime1->setEnabled(false);
        editTime2->setEnabled(false);
        editBloodFactor->setReadOnly(true);
        chkCalibrateParam_1->setEnabled(false);
        chkCalibrateParam_1->setChecked(true);
        edit_R0_1->setReadOnly(true);
        edit_K_1->setReadOnly(true);
        edit_a_1->setReadOnly(true);
        edit_b_1->setReadOnly(true);
        edit_c_1->setReadOnly(true);
        editCalibrateParam_C_1->setReadOnly(true);
        editCalibrateParam_R_1->setReadOnly(true);

        chkCalibrateParam_2->setEnabled(false);
        edit_R0_2->setReadOnly(true);
        edit_K_2->setReadOnly(true);
        edit_a_2->setReadOnly(true);
        edit_b_2->setReadOnly(true);
        edit_c_2->setReadOnly(true);
        editCalibrateParam_C_2->setReadOnly(true);
        editCalibrateParam_R_2->setReadOnly(true);

        //显示命名混淆
        lab_R0_1->setText("a:");
        lab_K_1->setText("b:");
        lab_a_1->setText("c:");
        lab_b_1->setText("d:");
        lab_c_1->setText("e:");
        labCalibrateParam_C_1->setText("f:");
        labCalibrateParam_R_1->setText("g:");
        lab_R0_2->setText("i:");
        lab_K_2->setText("j:");
        lab_a_2->setText("k:");
        lab_b_2->setText("l:");
        lab_c_2->setText("m:");
        labCalibrateParam_C_2->setText("n:");
        labCalibrateParam_R_2->setText("o:");
    }

    btnSaveParam_K->setEnabled(false);
    btnDelete->setEnabled(false);
}

//连接信号与槽
void RoutineProjectSettingWidget::initConnect()
{
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(slt_btnAdd_Clicked()));
    connect(btnSave, SIGNAL(clicked()), this, SLOT(slt_btnSave_Clicked()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(slt_btnCancel_Clicked()));
    connect(btnUpdate, SIGNAL(clicked()), this, SLOT(slt_btnUpdate_Clicked()));
    connect(btnDelete, SIGNAL(clicked()), this, SLOT(slt_btnDelete_Clicked()));
    connect(btnReadCard, SIGNAL(clicked()), this, SLOT(slt_btnReadCard_Clicked()));
    connect(btnSaveParam_K, SIGNAL(clicked()), this, SLOT(slt_btnSaveParam_K_Clicked()));

    connect(tbvRoutineProject, SIGNAL(clicked(QModelIndex)), this, SLOT(slt_tbvSelectRow(QModelIndex)));
    connect(chkCalibrateParam_2, SIGNAL(stateChanged(int)), this, SLOT(slt_chkCalibrateParam_2_Check(int)));
    connect(readCareWidget, SIGNAL(sig_updateProject(QString)), this, SLOT(slt_updateProject(QString)));
    connect(cboIsDiluent, SIGNAL(currentIndexChanged(int)), this, SLOT(slt_cboIsDiluent_IndexChanged(int)));
}

//初始化数据
void RoutineProjectSettingWidget::initData()
{
    initDataComboBox();      //初始化下拉列表
    initDataTbvRoutineProject();//初始化数据-常规项目列表
}

//初始化数据-常规项目列表
void RoutineProjectSettingWidget::initDataTbvRoutineProject()
{
    QStringList headerList_h;
    headerList_h << "id" << "项目";
    modelRoutineProject->clear();
    modelRoutineProject->setHorizontalHeaderLabels( headerList_h );
    tbvRoutineProject->hideColumn(0);             //id-隐藏
    tbvRoutineProject->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    tbvRoutineProject->verticalHeader()->setVisible(false);                          //隐藏左边列（显示编号的列）                                                        //隐藏列表头  (注：除了QTableWidget还需要引用QHeaderView)
    tbvRoutineProject->setEditTriggers(QAbstractItemView::NoEditTriggers);           //设置表格的单元为只读属性，即不能编辑
    tbvRoutineProject->setSelectionBehavior(QAbstractItemView::SelectRows);          //整行选择
    tbvRoutineProject->horizontalHeader()->setStretchLastSection(true);              //设置充满表宽度
    //tbvRoutineProject->horizontalHeader()->setResizeMode(QHeaderView::Fixed);        //列表不能移动

    //初始化每一个单元格
    QString str;
    QSqlQuery  query;
    QVector<QString> p_id;
    QVector<QString> name;
    query.clear();
    name.clear();
    str  = "select id, name from ProjectT;";
    query = SQLite::getInstance()->Select(str);
    while(query.next())
    {
        p_id.append( query.value(0).toString().trimmed() );
        name.append( query.value(1).toString().trimmed() );
    }

    for( int i = 0; i < name.count(); i ++ )
    {
        modelRoutineProject->setItem( i, 0, new QStandardItem( p_id.at(i) ));  //id(隐藏)
        modelRoutineProject->setItem( i, 1, new QStandardItem( name.at(i) ));
        modelRoutineProject->item(i, 1)->setTextAlignment(Qt::AlignCenter); //居中
    }
}

//初始化下拉列表
void RoutineProjectSettingWidget::initDataComboBox()
{
    QStringList list;
    //结果精度（小数点位数）
    list.clear();
    list<< "0" << "1" << "2" << "3" << "4" << "5";
    cboAccuracy->addItems(list);

    //是否要稀释
    list.clear();
    list << "0-否(N)" << "1-是(Y)";
    cboIsDiluent->addItems(list);
}

//清空所有输入
void RoutineProjectSettingWidget::CleanAll()
{
    //基本参数
    editProjectName0->clear();
    editProjectName->clear();
    editBatchNum->clear();
    editParam_K->clear();
    editAllowanceWarm->clear();
    editDiluent->clear();
    editSample->clear();
    editS_Transfer->clear();
    editR1->clear();
    editR2->clear();
    editCycle1->clear();
    editTime1->clear();
    editCycle2->clear();
    editTime2->clear();

    editBloodFactor->clear();
    editUnit->clear();
    cboAccuracy->setCurrentIndex(0);
    edit_R0_1->clear();
    edit_K_1->clear();
    edit_a_1->clear();
    edit_b_1->clear();
    edit_c_1->clear();
    editCalibrateParam_C_1->clear();
    editCalibrateParam_R_1->clear();

    chkCalibrateParam_2->setChecked(false);
    edit_R0_2->clear();
    edit_K_2->clear();
    edit_a_2->clear();
    edit_b_2->clear();
    edit_c_2->clear();
    editCalibrateParam_C_2->clear();
    editCalibrateParam_R_2->clear();
}

//新增项目时更新GParameter参数类中的映射表
ProjectParamS RoutineProjectSettingWidget::InsertProjectParamS()
{
    //项目参数
    ProjectParamS param;
    param.isDiluentPorj = cboIsDiluent->currentIndex();
    param.Param_unit = editUnit->text().trimmed();               //项目单位  （是否有用，待定）
    param.Param_accuracy = cboAccuracy->currentText().toInt(); //项目精度
    param.Param_lowLinear = editLowLinear->text().toDouble();     //线性下限
    param.Param_factorK = editParam_K->text().toDouble();         //K因数值
    param.Param_bloodFactor = editBloodFactor->text().toDouble(); //全血因子（全血模式有效，非全血则为1）

    param.Param_lightPos1 = editCycle1->text().toInt();      //测光点1
    param.Param_lightPos2 = editCycle2->text().toInt();      //测光点2

    param.Param_R0 = edit_R0_1->text().trimmed().toDouble();  //参数R0
    param.Param_K = edit_K_1->text().trimmed().toDouble();    //参数K
    param.Param_a = edit_a_1->text().trimmed().toDouble();    //参数a
    param.Param_b = edit_b_1->text().trimmed().toDouble();    //参数b
    param.Param_c = edit_c_1->text().trimmed().toDouble();    //参数c
    param.Param_R = editCalibrateParam_R_1->text().trimmed().toDouble();    //参数-最大幅度
    param.Param_C = editCalibrateParam_C_1->text().trimmed().toDouble();    //参数-最大浓度

    //是否有第二段曲线
    if(chkCalibrateParam_2->isChecked())
    {
        param.isLogic5P2 = true;
        param.Param2_R0 = edit_R0_2->text().trimmed().toDouble();  //参数R0
        param.Param2_K = edit_K_2->text().trimmed().toDouble();    //参数K
        param.Param2_a = edit_a_2->text().trimmed().toDouble();    //参数a
        param.Param2_b = edit_b_2->text().trimmed().toDouble();    //参数b
        param.Param2_c = edit_c_2->text().trimmed().toDouble();    //参数c
        param.Param2_R = editCalibrateParam_R_2->text().trimmed().toDouble();    //参数-最大幅度
        param.Param2_C = editCalibrateParam_C_2->text().trimmed().toDouble();    //参数-最大浓度
    }
    else
    {
        param.isLogic5P2 = false;
        //以下参数正常情况下无实际用处，以防出错，（若没有第二段曲线，则第二段曲线的参数与第一段的参数一致）
        param.Param2_R0 = param.Param_R0;   //参数R0
        param.Param2_K = param.Param_K;     //参数K
        param.Param2_a = param.Param_a;     //参数a
        param.Param2_b = param.Param_b;     //参数b
        param.Param_c = param.Param_c;      //参数c
        param.Param2_R = param.Param_R;     //参数-最大幅度
        param.Param2_C = param.Param_C;     //参数-最大浓度
    }
    return param;
}

//修改项目时更新GParameter参数类中的映射表
ProjectParamS RoutineProjectSettingWidget::UpdateProjectParamS(int id)
{
    //项目参数
    ProjectParamS param = gParameter->getProjectParam(id);
    param.isDiluentPorj = cboIsDiluent->currentIndex();
    param.Param_unit = editUnit->text().trimmed();               //项目单位  （是否有用，待定）
    param.Param_accuracy = cboAccuracy->currentText().toInt(); //项目精度
    param.Param_lowLinear = editLowLinear->text().toDouble();     //线性下限
    param.Param_factorK = editParam_K->text().toDouble();         //K因数值
    param.Param_bloodFactor = editBloodFactor->text().toDouble(); //全血因子（全血模式有效，非全血则为1）

    param.Param_lightPos1 = editCycle1->text().toInt();      //测光点1
    param.Param_lightPos2 = editCycle2->text().toInt();      //测光点2

    param.Param_R0 = edit_R0_1->text().trimmed().toDouble();  //参数R0
    param.Param_K = edit_K_1->text().trimmed().toDouble();    //参数K
    param.Param_a = edit_a_1->text().trimmed().toDouble();    //参数a
    param.Param_b = edit_b_1->text().trimmed().toDouble();    //参数b
    param.Param_c = edit_c_1->text().trimmed().toDouble();    //参数c
    param.Param_R = editCalibrateParam_R_1->text().trimmed().toDouble();    //参数-最大幅度
    param.Param_C = editCalibrateParam_C_1->text().trimmed().toDouble();    //参数-最大浓度

    //是否有第二段曲线
    if(chkCalibrateParam_2->isChecked())
    {
        param.isLogic5P2 = true;
        param.Param2_R0 = edit_R0_2->text().trimmed().toDouble();   //参数R0
        param.Param2_K = edit_K_2->text().trimmed().toDouble();    //参数K
        param.Param2_a = edit_a_2->text().trimmed().toDouble();    //参数a
        param.Param2_b = edit_b_2->text().trimmed().toDouble();    //参数b
        param.Param2_c = edit_c_2->text().trimmed().toDouble();    //参数c
        param.Param2_R = editCalibrateParam_R_2->text().trimmed().toDouble();    //参数-最大幅度
        param.Param2_C = editCalibrateParam_C_2->text().trimmed().toDouble();    //参数-最大浓度
    }
    else
    {
        param.isLogic5P2 = false;
    }

    return param;
}

//槽:是否启用定标参数2
void RoutineProjectSettingWidget::slt_chkCalibrateParam_2_Check(int status)
{
    //项目定标参数2
    edit_R0_2->setEnabled(status);
    edit_K_2->setEnabled(status);
    edit_a_2->setEnabled(status);
    edit_b_2->setEnabled(status);
    edit_c_2->setEnabled(status);
    editCalibrateParam_C_2->setEnabled(status);
    editCalibrateParam_R_2->setEnabled(status);
}

//槽：增加
void RoutineProjectSettingWidget::slt_btnAdd_Clicked()
{
    CleanAll();         //清空所有输入
    isAdd_flag = 1;     //是否增加标志
    isUpdate_flag = 0;  //是否修改标志

    btnAdd->setEnabled(true);
    btnUpdate->setEnabled(false);
    btnDelete->setEnabled(false);
    btnCancel->setEnabled(true);
    btnSave->setEnabled(true);
    btnSaveParam_K->setEnabled(false);
    btnReadCard->setEnabled(false);
}
//槽：修改项目参数
void RoutineProjectSettingWidget::slt_btnUpdate_Clicked()
{
    isAdd_flag = 0;              //是否增加标志
    if( ! gScheduler->get_isRunning() ) //未进行测试，可以修改
    {
        isUpdate_flag = 1;           //是否修改标志
        btnAdd->setEnabled(false);
        btnUpdate->setEnabled(true);
        btnDelete->setEnabled(false);
        btnCancel->setEnabled(true);
        btnSave->setEnabled(true);
        btnSaveParam_K->setEnabled(false);
        btnReadCard->setEnabled(false);
    }
    else //正在测试中，不允许修改
    {
        gMessageBox->setInfo(QString("提示"), QString("有测试正在进行中，不能修改参数！\n请等待所有测试完成后再修改!!"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return;
    }
}
//槽：删除
void RoutineProjectSettingWidget::slt_btnDelete_Clicked()
{
    isAdd_flag = 0;              //是否增加标志
    isUpdate_flag = 0;           //是否修改标志

    if( gScheduler->get_isRunning() ) //正在测试中，不允许修改
    {
        gMessageBox->setInfo(QString("提示"), QString("有测试正在进行中，不能删除项目！\n请等待所有测试完成后再进行!!"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return;
    }

    int currentRow = tbvRoutineProject->currentIndex().row();
    if(currentRow >= 0)
    {
        int p_id = modelRoutineProject->item( currentRow, 0)->text().toInt();

        //删除
        QString str;
        str  = "delete from ProjectT where id = "+ QString::number( p_id ) + ";";
        SQLite::getInstance()->Delete(str);

        //提示 & 更新
        gMessageBox->setInfo(QString("提示"), QString("成功删除选择的项目！"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "确定");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
        {
            gParameter->removeProjectMap(p_id);
            initDataTbvRoutineProject();//初始化数据-常规项目列表

            for(int i = 0; i < REAGENT_COUNT; i ++)    //释放试剂仓上的位置
            {
                //找到已删除项目的位置
                if( gParameter->getMapReagentSlotValue(i) == p_id )
                    gParameter->ReleaseReagentNo(i);    //释放试剂仓上的位置
            }

            return;
        }
    }
}
//槽：取消
void RoutineProjectSettingWidget::slt_btnCancel_Clicked()
{
    isAdd_flag = 0;              //是否增加标志
    isUpdate_flag = 0;           //是否修改标志

    btnAdd->setEnabled(true);
    btnUpdate->setEnabled(true);
    btnDelete->setEnabled(true);
    btnCancel->setEnabled(false);
    btnSave->setEnabled(false);
    btnSaveParam_K->setEnabled(true);
    btnReadCard->setEnabled(true);
}
//槽：保存
void RoutineProjectSettingWidget::slt_btnSave_Clicked()
{
    btnAdd->setEnabled(true);
    btnUpdate->setEnabled(true);
    btnDelete->setEnabled(true);
    btnCancel->setEnabled(false);
    btnSave->setEnabled(false);
    btnSaveParam_K->setEnabled(true);
    btnReadCard->setEnabled(true);

    if( isAdd_flag )
    {
        isAdd_flag = 0;

        QString name = editProjectName->text().trimmed();          //项目名称
        QString abbreviation = editProjectName0->text().trimmed(); //项目简称
        QString batchNum = editBatchNum->text().trimmed();         //批号
        int isDiluent = cboIsDiluent->currentIndex();              //是否要稀释
        int accuracy = cboAccuracy->currentText().toInt();         //结果精度
        QString unit = editUnit->text().trimmed();                 //结果单位
        double param_K = editParam_K->text().toDouble();           //K因数值
        double bloodFactor = editBloodFactor->text().toDouble();   //全血因子
        double lowLinear = editLowLinear->text().toDouble();       //线性下限
        int diluent = editDiluent->text().toInt();                 //稀释液
        int sample = editSample->text().toInt();                   //样本
        int transfer = editS_Transfer->text().toInt();             //样本转移量
        int r1 = editR1->text().toInt();                           //R1
        int r2 = editR2->text().toInt();                           //R2
        int reactionCycle1 = editCycle1->text().toInt();           //反应周期/时间(s)
        int reactionCycle2 = editCycle2->text().toInt();           //反应周期/时间(s)

        //定标分段曲线1
        long double calibrate1_R0 = edit_R0_1->text().toDouble();
        long double calibrate1_K = edit_K_1->text().toDouble();
        long double calibrate1_a = edit_a_1->text().toDouble();
        long double calibrate1_b = edit_b_1->text().toDouble();
        long double calibrate1_c = edit_c_1->text().toDouble();
        double calibrate1_RR = editCalibrateParam_R_1->text().toDouble();
        double calibrate1_CC = editCalibrateParam_C_1->text().toDouble();

        //定标分段曲线2
        int isCalibrate2 = chkCalibrateParam_2->isChecked();
        long double calibrate2_R0 = edit_R0_2->text().toDouble();
        long double calibrate2_K = edit_K_2->text().toDouble();
        long double calibrate2_a = edit_a_2->text().toDouble();
        long double calibrate2_b = edit_b_2->text().toDouble();
        long double calibrate2_c = edit_c_2->text().toDouble();
        double calibrate2_RR = editCalibrateParam_R_2->text().toDouble();
        double calibrate2_CC = editCalibrateParam_C_2->text().toDouble();

        QString str =
                "insert into ProjectT(name, abbreviation, batchNum,"
                "isDiluent, accuracy, unit, "
                "r0ID, r1ID, r2ID, sample_V, r0_V, r1_V, rs_V, r2_V,"
                "lightPos1, lightPos2, K, bloodFactor, lowLinear, "
                //定标参数
                "calibrateID, "
                "calibrate1_R0, calibrate1_K, calibrate1_a, calibrate1_b, calibrate1_c, calibrate1_RR, calibrate1_CC, "
                "isCalibrate2, calibrate2_R0, calibrate2_K, calibrate2_a, calibrate2_b, calibrate2_c, calibrate2_RR, calibrate2_CC) values( "

                "'" + name + "', "             //项目名称
                + "'" + abbreviation +"', "    //项目简称
                + "'" + batchNum +"', "        //批号

                + QString::number( isDiluent ) +", "   //稀释否（是否需要稀释样本）
                + QString::number( accuracy   ) +", "  //结果精度
                + "'" + unit+"', "                     //项目单位

                + " 0, "             //R0试剂ID（试剂表）(试剂位置)
                + " 0, "             //R1试剂ID（试剂表）(试剂位置)
                + " 0, "             //R2试剂ID（试剂表）(试剂位置)

                + QString::number( sample ) +", "       //样本量（ul）
                + QString::number( diluent ) +", "      //探针吸R0量（ul）
                + QString::number( r1 ) +", "           //探针吸R1量（ul）
                + QString::number( transfer ) +", "     //探针吸稀释后的样本量（ul）样本转移量
                + QString::number( r2 ) +", "           //探针吸R2量（ul）

                + QString::number( reactionCycle1 ) +", "       //测光点1
                + QString::number( reactionCycle2 ) +", "       //测光点2
                + QString::number( param_K ) +", "      //K因数值
                + QString::number( bloodFactor ) +", "  //全血因子
                + QString::number( lowLinear ) +", "    //线性下限

                + QString::number( 0 ) +", "                 //定标方式ID（定标方式表）
                + QString::number( calibrate1_R0, 'f', 6 ) +", "     //定标分段曲线1-R0
                + QString::number( calibrate1_K, 'f', 6 ) +", "      //定标分段曲线1-K
                + QString::number( calibrate1_a, 'f', 6 ) +", "      //定标分段曲线1-a
                + QString::number( calibrate1_b, 'f', 6 ) +", "      //定标分段曲线1-b
                + QString::number( calibrate1_c, 'f', 6 ) +", "      //定标分段曲线1-c
                + QString::number( calibrate1_RR ) +", "     //定标分段曲线1-最大幅度
                + QString::number( calibrate1_CC ) +", "     //定标分段曲线1-最高浓度

                + QString::number( isCalibrate2 ) +", "          //定标分段曲线2-是否有定标参数二
                + QString::number( calibrate2_R0, 'f', 6 ) +", "         //定标分段曲线2-R0
                + QString::number( calibrate2_K, 'f', 6 ) +", "          //定标分段曲线2-K
                + QString::number( calibrate2_a, 'f', 6 ) +", "          //定标分段曲线2-a
                + QString::number( calibrate2_b, 'f', 6 ) +", "          //定标分段曲线2-b
                + QString::number( calibrate2_c, 'f', 6 ) +", "          //定标分段曲线2-c
                + QString::number( calibrate2_RR ) +", "         //定标分段曲线2-最大幅度
                + QString::number( calibrate2_CC )               //定标分段曲线2-最高浓度
                + " );";
        SQLite::getInstance()->Insert(str);

        //新增项目时更新GParameter参数类中的映射表
        ProjectParamS param = InsertProjectParamS();

        int id;
        str  = "select id, name from ProjectT where name ='" + name +"';";
        QSqlQuery query = SQLite::getInstance()->Select(str);
        while(query.next())
        {
            id = query.value(0).toInt();
            gParameter->appendProjectMap( id, query.value(1).toString() );
        }

        gParameter->setProjectParam(id, param);   //
        initDataTbvRoutineProject();              //初始化数据-常规项目列表
    }
    if( isUpdate_flag )
    {
        isUpdate_flag = 0;

        int currentRow = tbvRoutineProject->currentIndex().row();
        int p_id = modelRoutineProject->item( currentRow, 0)->text().toInt();

        QString name = editProjectName->text().trimmed();          //项目名称
        QString abbreviation = editProjectName0->text().trimmed(); //项目简称
        QString batchNum = editBatchNum->text().trimmed();         //批号
        int isDiluent = cboIsDiluent->currentIndex();              //是否要稀释
        int accuracy = cboAccuracy->currentText().toInt();         //结果精度
        QString unit = editUnit->text().trimmed();                 //结果单位
        double  param_K = editParam_K->text().toDouble();          //K因数值
        double lowLinear = editLowLinear->text().toDouble();       //线性下限
        double bloodFactor = editBloodFactor->text().toDouble();   //全血因子
        int diluent = editDiluent->text().toInt();                 //稀释液
        int sample = editSample->text().toInt();                   //样本
        int transfer = editS_Transfer->text().toInt();             //样本转移量
        int r1 = editR1->text().toInt();                           //R1
        int r2 = editR2->text().toInt();                           //R2
        int reactionCycle1 = editCycle1->text().toInt();           //反应周期/时间(s)
        int reactionCycle2 = editCycle2->text().toInt();           //反应周期/时间(s)
        //定标分段曲线1
        long double  calibrate1_R0 = edit_R0_1->text().toDouble();
        long double  calibrate1_K = edit_K_1->text().toDouble();
        long double  calibrate1_a = edit_a_1->text().toDouble();
        long double  calibrate1_b = edit_b_1->text().toDouble();
        long double  calibrate1_c = edit_c_1->text().toDouble();
        double  calibrate1_RR = editCalibrateParam_R_1->text().toDouble();
        double  calibrate1_CC = editCalibrateParam_C_1->text().toDouble();

        //定标分段曲线2
        int isCalibrate2 = chkCalibrateParam_2->isChecked();
        long double  calibrate2_R0 = edit_R0_2->text().toDouble();
        long double  calibrate2_K = edit_K_2->text().toDouble();
        long double  calibrate2_a = edit_a_2->text().toDouble();
        long double  calibrate2_b = edit_b_2->text().toDouble();
        long double  calibrate2_c = edit_c_2->text().toDouble();
        double  calibrate2_RR = editCalibrateParam_R_2->text().toDouble();
        double  calibrate2_CC = editCalibrateParam_C_2->text().toDouble();

        QString str =
                "update ProjectT set "
                " name='" + name + "', "                   //项目名称
                " abbreviation='" + abbreviation +"', "    //项目简称
                " batchNum='" + batchNum +"', "    //项目简称

                " isDiluent=" + QString::number( isDiluent ) +", "      //稀释否（是否需要稀释样本）
                " unit='"+ unit  +"', "                                 //项目单位ID
                " accuracy="+ QString::number( accuracy ) +", "         //结果精度

                " sample_V="+ QString::number( sample ) +", "      //样本量（ul）
                " r0_V="+ QString::number( diluent ) +", "         //探针吸R0量（ul）
                " r1_V="+ QString::number( r1 ) +", "              //探针吸稀释后的样本量（ul）样本转移量
                " rs_V="+ QString::number( transfer ) +", "        //探针吸R1量（ul）
                " r2_V="+ QString::number( r2 ) +", "              //探针吸R2量（ul）

                " lightPos1="+ QString::number( reactionCycle1 ) +", "       //测光点1
                " lightPos2="+ QString::number( reactionCycle2 ) +", "       //测光点2
                " K="+ QString::number( param_K ) +", "      //K因数值
                " lowLinear="+ QString::number( lowLinear ) +", "//线性下限
                " bloodFactor="+ QString::number( bloodFactor ) +", " //全血因子

                " calibrateID="+ QString::number( 0 ) +", "      //定标方式ID（定标方式表）
                " calibrate1_R0="+ QString::number( calibrate1_R0, 'f', 6 ) +", "    //定标参数
                " calibrate1_K="+ QString::number( calibrate1_K, 'f', 6 ) +", "      //定标参数
                " calibrate1_a="+ QString::number( calibrate1_a, 'f', 6 ) +", "      //定标参数
                " calibrate1_b="+ QString::number( calibrate1_b, 'f', 6 ) +", "      //定标参数
                " calibrate1_c="+ QString::number( calibrate1_c, 'f', 6 ) +", "       //定标参数
                " calibrate1_RR="+ QString::number( calibrate1_RR ) +", "    //定标参数
                " calibrate1_CC="+ QString::number( calibrate1_CC ) +", "    //定标参数

                " isCalibrate2=" + QString::number( isCalibrate2 ) +", "     //定标参数
                " calibrate2_R0="+ QString::number( calibrate2_R0, 'f', 6 ) +", "    //定标参数
                " calibrate2_K="+ QString::number( calibrate2_K, 'f', 6 ) +", "      //定标参数
                " calibrate2_a="+ QString::number( calibrate2_a, 'f', 6 ) +", "      //定标参数
                " calibrate2_b="+ QString::number( calibrate2_b, 'f', 6 ) +", "      //定标参数
                " calibrate2_c="+ QString::number( calibrate2_c, 'f', 6 ) +", "       //定标参数
                " calibrate2_RR="+ QString::number( calibrate2_RR ) +", "    //定标参数
                " calibrate2_CC="+ QString::number( calibrate2_CC ) +" "     //定标参数
                " where id =" + QString::number( p_id )+ ";";
        //qDebug() << str;
        SQLite::getInstance()->Insert(str);

        //修改项目时更新GParameter参数类中的映射表
        gParameter->setProjectParam(p_id, UpdateProjectParamS(p_id));
    }
}
//槽：读卡
void RoutineProjectSettingWidget::slt_btnReadCard_Clicked()
{
    isAdd_flag = 0;              //是否增加标志
    isUpdate_flag = 0;           //是否修改标志

    if( gScheduler->get_isRunning() )//正在测试中，不允许修改
    {
        gMessageBox->setInfo(QString("提示"), QString("有测试正在进行中，不能进行读卡！\n请等待所有测试完成后再进行!!"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return;
    }


//    gMessageBox->setInfo(QString("提示"), QString("读卡功能尚未开放!!"),
//                         QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
//    gMessageBox->setButtonText(Cancel, "返回");
//    gMessageBox->show();
//    return;

    //开始读卡 MV200不适用 （未统一读写卡协议）
    readCareWidget->StartReadCard();
}
//槽：保存参数（K因数值）
void RoutineProjectSettingWidget::slt_btnSaveParam_K_Clicked()
{
    int currentRow = tbvRoutineProject->currentIndex().row();
    if( currentRow == -1 ) return;

    int p_id = modelRoutineProject->item( currentRow, 0)->text().toInt();
    double  param_K = editParam_K->text().trimmed().toDouble();          //K因数值
    QString str =
            "update ProjectT set "
            " K="+ QString::number( param_K ) +" "      //K因数值
            " where id =" + QString::number( p_id )+ ";";
    //qDebug() << str;
    SQLite::getInstance()->Insert(str);
}

//单击列表选择中某行
void RoutineProjectSettingWidget::slt_tbvSelectRow(const QModelIndex &index)
{
    if (index.isValid())
    {
        int p_id = modelRoutineProject->item( index.row(), 0)->text().toInt();
        selectCurrentProject( p_id );
        btnSaveParam_K->setEnabled(true);
        btnDelete->setEnabled(true);
    }
}

//查看当前选中的项目
void RoutineProjectSettingWidget::selectCurrentProject(int p_id)
{
    QSqlQuery query;
    query.clear();
    QString str  = " select name, abbreviation, batchNum,isDiluent, accuracy, unit,"
                   " sample_V, r0_V, r1_V, rs_V, r2_V,"
                   " lightPos1, lightPos2, K, alarmTimes,"
                   " calibrateID, calibrate1_R0, calibrate1_K, calibrate1_a, calibrate1_b, calibrate1_c, calibrate1_RR, calibrate1_CC, "
                   " isCalibrate2, calibrate2_R0, calibrate2_K, calibrate2_a, calibrate2_b, calibrate2_c, calibrate2_RR, calibrate2_CC, "
                   " bloodFactor, lowLinear "
                   " from ProjectT where id = "+ QString::number( p_id ) + ";";
    query = SQLite::getInstance()->Select(str);

    while(query.next())
    {
        editProjectName->setText(query.value(0).toString().trimmed() ); //项目简称
        editProjectName0->setText(query.value(0).toString().trimmed() );//项目名称
        editBatchNum->setText(query.value(2).toString().trimmed() );

        cboIsDiluent->setCurrentIndex(query.value(3).toInt());
        cboAccuracy->setCurrentIndex(query.value(4).toInt());
        editUnit->setText(query.value(5).toString());

        editSample->setText(query.value(6).toString().trimmed() );
        editDiluent->setText(query.value(7).toString().trimmed() );
        editR1->setText(query.value(8).toString().trimmed() );
        editS_Transfer->setText(query.value(9).toString().trimmed() );
        editR2->setText(query.value(10).toString().trimmed() );

        editCycle1->setText(query.value(11).toString().trimmed() );
        editTime1->setText( QString::number( query.value(11).toInt() * gParameter->getCycleTime() /1000/2 ) );
        editCycle2->setText(query.value(12).toString().trimmed() );
        editTime2->setText( QString::number( query.value(12).toInt() * gParameter->getCycleTime() /1000/2 ) );
        editParam_K->setText( QString::number( query.value(13).toDouble(), 'g', 6) );
        editAllowanceWarm->setText(query.value(14).toString().trimmed() );

        //int CalibrateID = query.value(15).toInt();
        edit_R0_1->setText( QString::number( query.value(16).toDouble(), 'g', 10) );
        edit_K_1->setText( QString::number( query.value(17).toDouble(), 'g', 10 ) );
        edit_a_1->setText( QString::number( query.value(18).toDouble(), 'g', 10 ) );
        edit_b_1->setText( QString::number( query.value(19).toDouble(), 'g', 10 ) );
        edit_c_1->setText( QString::number( query.value(20).toDouble(), 'g', 10 ) );
        editCalibrateParam_R_1->setText( query.value(21).toString() );
        editCalibrateParam_C_1->setText( query.value(22).toString() );

        chkCalibrateParam_2->setChecked(query.value(23).toInt());
        edit_R0_2->setText( QString::number( query.value(24).toDouble(), 'g', 10) );
        edit_K_2->setText( QString::number( query.value(25).toDouble(), 'g', 10) );
        edit_a_2->setText( QString::number( query.value(26).toDouble(), 'g', 10) );
        edit_b_2->setText( QString::number( query.value(27).toDouble(), 'g', 10) );
        edit_c_2->setText( QString::number( query.value(28).toDouble(), 'g', 10) );
        editCalibrateParam_R_2->setText(query.value(29).toString());
        editCalibrateParam_C_2->setText(query.value(30).toString());
        editBloodFactor->setText( QString::number( query.value(31).toDouble(), 'g', 10) );
        editLowLinear->setText( QString::number( query.value(32).toDouble(), 'g', 10) );
    }

    //清空
    if(!chkCalibrateParam_2->isChecked())
    {
        edit_R0_2->setText("");
        edit_K_2->setText("");
        edit_a_2->setText("");
        edit_b_2->setText("");
        edit_c_2->setText("");
        editCalibrateParam_C_2->setText("");
        editCalibrateParam_R_2->setText("");
    }

}

//读写卡成功后更新数据
void RoutineProjectSettingWidget::slt_updateProject(QString name)
{
    initDataTbvRoutineProject();//初始化数据-常规项目列表

    QString str  = "select id, name from ProjectT where name ='" + name +"';";
    QSqlQuery query = SQLite::getInstance()->Select(str);
    int pID = 0;
    while(query.next())
    {
        pID = query.value(0).toInt();
        gParameter->appendProjectMap( pID, query.value(1).toString() );
    }

    if(pID == 0) return;
    //新增项目时更新GParameter参数类中的映射表
    selectCurrentProject(pID);
    ProjectParamS param = InsertProjectParamS();
    gParameter->setProjectParam(pID, param);
}

//槽:选择是否要稀释时，稀释液&转移量会有变化
void RoutineProjectSettingWidget::slt_cboIsDiluent_IndexChanged(int index)
{
    if(index == 0) //非稀释项目 （不可用&隐藏）
    {
        editDiluent->setVisible(false);
        editS_Transfer->setVisible(false);
        labDiluent->setVisible(false);
        labS_Transfer->setVisible(false);
        editDiluent->clear();
        editS_Transfer->clear();
    }
    else //稀释项目  （可用&显示）
    {
        editDiluent->setVisible(true);
        editS_Transfer->setVisible(true);
        labDiluent->setVisible(true);
        labS_Transfer->setVisible(true);
    }
}

