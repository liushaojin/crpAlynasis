#include "testListInfoWidget.h"

TestListInfoWidget::TestListInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();          //初始化
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}

void TestListInfoWidget::initForm()
{
    wdgInvalid = new QWidget;
    tbwInvalid = new QTableWidget;     //已作废
    btnRecovery = new QPushButton("恢复");      //恢复
    lytInvalid = new QGridLayout;
    lytInvalid->setMargin(0);
    lytInvalid->setSpacing(2);
    lytInvalid->addWidget(tbwInvalid,   0, 0, 90, 100);
    lytInvalid->addWidget(btnRecovery, 90, 0, 10, 100, Qt::AlignLeft | Qt::AlignBottom);
    wdgInvalid->setLayout(lytInvalid);

    tabWidget = new QTabWidget;
    tbwWaitTest = new QTableWidget;     //待测试
    tbwTesting = new QTableWidget;      //测试中
    tbwQueueing = new QTableWidget;     //排队中

    tabWidget->addTab(tbwWaitTest,  "待测试");
    tabWidget->addTab(tbwTesting,  "测试中");
    tabWidget->addTab(tbwQueueing,  "排队中");
    tabWidget->addTab(wdgInvalid,  "已作废");

    //
    layout = new QGridLayout;
    //layout->setMargin(0);
    layout->setContentsMargins(0, 1, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(tabWidget,  0, 0, 100, 100);
    this->setLayout(layout);

    //列表格式化
    initWaitTestFormat();
    initTestingFormat();
    initQueueingFormat();
    initInvalidFormat();
}

void TestListInfoWidget::initWaitTestFormat()
{
    tbwWaitTest->setColumnCount(7); //设置列数
    tbwWaitTest->horizontalHeader()->setDefaultSectionSize(120);
    tbwWaitTest->horizontalHeader()->setClickable(true); //设置表头不可点击（默认点击后进行排序）

    //设置表头内容
    QStringList header;
    header<<tr("测试类型")<<tr("测试项目")<<tr("编号")<<tr("重复次数")<<tr("样本位")<<tr("急诊")<<tr("申请日期")<<tr("其他信息");
    tbwWaitTest->setHorizontalHeaderLabels(header);

    //设置表头字体加粗
    //QFont font;
    //font.setBold(true);
    //tbwWaitTest->horizontalHeader()->setFont(font);

    tbwWaitTest->horizontalHeader()->setStretchLastSection(false); //设置充满表宽度
    tbwWaitTest->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    tbwWaitTest->verticalHeader()->setDefaultSectionSize(20); //设置行高
    tbwWaitTest->setFrameShape(QFrame::NoFrame); //设置无边框
    tbwWaitTest->setShowGrid(true); //设置显示格子线
    tbwWaitTest->verticalHeader()->setVisible(false); //设置垂直头不可见
    //tbwWaitTest->setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
    tbwWaitTest->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    tbwWaitTest->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    tbwWaitTest->horizontalHeader()->resizeSection(0,100); //设置表头第一列的宽度为150
    tbwWaitTest->horizontalHeader()->resizeSection(1,100); //设置表头第一列的宽度为150
    tbwWaitTest->horizontalHeader()->resizeSection(6,150); //设置表头第一列的宽度为150
    tbwWaitTest->horizontalHeader()->resizeSection(7,200); //设置表头第一列的宽度为150
    tbwWaitTest->horizontalHeader()->setFixedHeight(30); //设置表头的高度
    tbwWaitTest->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    tbwWaitTest->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色

}
void TestListInfoWidget::initTestingFormat()
{
    tbwTesting->setColumnCount(7); //设置列数
    tbwTesting->horizontalHeader()->setDefaultSectionSize(120);
    tbwTesting->horizontalHeader()->setClickable(true); //设置表头不可点击（默认点击后进行排序）

    //设置表头内容
    QStringList header;
    header<<tr("测试类型")<<tr("测试项目")<<tr("编号")<<tr("样本位")<<tr("状态")<<tr("反应杯号")<<tr("急诊")<<tr("剩余时间");
    tbwTesting->setHorizontalHeaderLabels(header);

    //设置表头字体加粗
    //QFont font;
    //font.setBold(true);
    //tbwTesting->horizontalHeader()->setFont(font);

    tbwTesting->horizontalHeader()->setStretchLastSection(false); //设置充满表宽度
    tbwTesting->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    tbwTesting->verticalHeader()->setDefaultSectionSize(20); //设置行高
    tbwTesting->setFrameShape(QFrame::NoFrame); //设置无边框
    tbwTesting->setShowGrid(true); //设置显示格子线
    tbwTesting->verticalHeader()->setVisible(false); //设置垂直头不可见
    //tbwTesting->setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
    tbwTesting->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    tbwTesting->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    tbwTesting->horizontalHeader()->resizeSection(0,100); //设置表头第一列的宽度为150
    tbwTesting->horizontalHeader()->resizeSection(1,100); //设置表头第一列的宽度为150
    tbwTesting->horizontalHeader()->resizeSection(6,150); //设置表头第一列的宽度为150
    tbwTesting->horizontalHeader()->resizeSection(7,200); //设置表头第一列的宽度为150
    tbwTesting->horizontalHeader()->setFixedHeight(30); //设置表头的高度
    tbwTesting->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    tbwTesting->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
}
void TestListInfoWidget::initQueueingFormat()
{
    tbwQueueing->setColumnCount(6); //设置列数
    tbwQueueing->horizontalHeader()->setDefaultSectionSize(120);
    tbwQueueing->horizontalHeader()->setClickable(true); //设置表头不可点击（默认点击后进行排序）

    //设置表头内容
    QStringList header;
    header<<tr("测试类型")<<tr("测试项目")<<tr("编号")<<tr("样本位")<<tr("急诊")<<tr("所需时间");
    tbwQueueing->setHorizontalHeaderLabels(header);

    //设置表头字体加粗
    //QFont font;
    //font.setBold(true);
    //tbwQueueing->horizontalHeader()->setFont(font);

    tbwQueueing->horizontalHeader()->setStretchLastSection(false); //设置充满表宽度
    tbwQueueing->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    tbwQueueing->verticalHeader()->setDefaultSectionSize(20); //设置行高
    tbwQueueing->setFrameShape(QFrame::NoFrame); //设置无边框
    tbwQueueing->setShowGrid(true); //设置显示格子线
    tbwQueueing->verticalHeader()->setVisible(false); //设置垂直头不可见
    //tbwQueueing->setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
    tbwQueueing->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    tbwQueueing->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    tbwQueueing->horizontalHeader()->resizeSection(0,100); //设置表头第一列的宽度为150
    tbwQueueing->horizontalHeader()->setFixedHeight(30); //设置表头的高度
    tbwQueueing->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    tbwQueueing->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
}
void TestListInfoWidget::initInvalidFormat()
{
    tbwInvalid->setColumnCount(8); //设置列数
    tbwInvalid->horizontalHeader()->setDefaultSectionSize(120);
    tbwInvalid->horizontalHeader()->setClickable(true); //设置表头不可点击（默认点击后进行排序）

    //设置表头内容
    QStringList header;
    header<<tr("选择")<<tr("测试类型")<<tr("测试项目")<<tr("编号")<<tr("重复次数")<<tr("样本位")<<tr("急诊")<<tr("备注");
    tbwInvalid->setHorizontalHeaderLabels(header);

    //设置表头字体加粗
    //QFont font;
    //font.setBold(true);
    //tbwInvalid->horizontalHeader()->setFont(font);

    tbwInvalid->horizontalHeader()->setStretchLastSection(false); //设置充满表宽度
    tbwInvalid->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    tbwInvalid->verticalHeader()->setDefaultSectionSize(20); //设置行高
    tbwInvalid->setFrameShape(QFrame::NoFrame); //设置无边框
    tbwInvalid->setShowGrid(true); //设置显示格子线
    tbwInvalid->verticalHeader()->setVisible(false); //设置垂直头不可见
    //tbwInvalid->setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
    tbwInvalid->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    tbwInvalid->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    tbwInvalid->horizontalHeader()->resizeSection(0,50); //设置表头第一列的宽度为150
    tbwInvalid->horizontalHeader()->setFixedHeight(30); //设置表头的高度
    tbwInvalid->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    tbwInvalid->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
}


//界面风格
void TestListInfoWidget::initStyle()
{
    btnRecovery->setObjectName("button6");
}

//连接信号与槽
void TestListInfoWidget::initConnect()
{
    //connect(btnCancel, SIGNAL(clicked()), this, SLOT(slt_btnCancel_Clicked()));
    //connect(btnLogin, SIGNAL(clicked()), this, SLOT(slt_btnLogin_Clicked()));
}

//初始化数据
void TestListInfoWidget::initData()
{
}
