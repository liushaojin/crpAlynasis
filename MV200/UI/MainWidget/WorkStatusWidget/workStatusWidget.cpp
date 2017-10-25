#include "workStatusWidget.h"

//实例对象
WorkStatusWidget *WorkStatusWidget::_instance = 0;
WorkStatusWidget * WorkStatusWidget::getInstance()
{
    if (_instance == 0)
        _instance = new WorkStatusWidget();
    return _instance;
}

WorkStatusWidget::WorkStatusWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();          //初始化
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}


//初始化
void WorkStatusWidget::initForm()
{
    tabWorkStatus = new QTabWidget;
    layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(tabWorkStatus,  0, 0, 100, 100);
    this->setLayout(layout);

    //
    tabWorkStatus->addTab(new SampleInfoWidget, "样本信息");
    tabWorkStatus->addTab(new ReagentInfoWidget, "试剂盘");
    tabWorkStatus->addTab(new ReactionDiskInfoWidget, "反应盘");
}

//界面风格
void WorkStatusWidget::initStyle()
{
    tabWorkStatus->setObjectName("tabW");
}

//连接信号与槽
void WorkStatusWidget::initConnect()
{
    //connect(btnCancel, SIGNAL(clicked()), this, SLOT(slt_btnCancel_Clicked()));
    //connect(btnLogin, SIGNAL(clicked()), this, SLOT(slt_btnLogin_Clicked()));
}

//初始化数据
void WorkStatusWidget::initData()
{
}
