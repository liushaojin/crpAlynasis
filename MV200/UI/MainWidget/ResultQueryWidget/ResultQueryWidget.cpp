#include "ResultQueryWidget.h"
#include <qdebug.h>

//实例对象
ResultQueryWidget *ResultQueryWidget::_instance = 0;
ResultQueryWidget * ResultQueryWidget::getInstance()
{
    if (_instance == 0)
        _instance = new ResultQueryWidget();
    return _instance;
}

ResultQueryWidget::ResultQueryWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();          //初始化
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}


//初始化
void ResultQueryWidget::initForm()
{
    gSampleWidget = new SampleWidget;
    qcWidget = new QCWidget;

    tabResultQuery = new QTabWidget;
    layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
//    layout->addWidget(gSampleWidget,  0, 0, 100, 100);
    layout->addWidget(tabResultQuery,  0, 0, 100, 100);
    this->setLayout(layout);


    //
    tabResultQuery->addTab(gSampleWidget, "样本");
    tabResultQuery->addTab(qcWidget, "质控");

//    tabResultQuery->addTab(new QWidget, "定标");
//    tabResultQuery->addTab(new ReagentBlankWidget, "试剂空白");
}

//界面风格
void ResultQueryWidget::initStyle()
{
    tabResultQuery->setObjectName("tabW");
}

//连接信号与槽
void ResultQueryWidget::initConnect()
{
    //connect(tabResultQuery, SIGNAL(currentChanged(int)), SLOT(slt_tabCurrentChanged(int)));
    //connect(btnCancel, SIGNAL(clicked()), this, SLOT(slt_btnCancel_Clicked()));
    //connect(btnLogin, SIGNAL(clicked()), this, SLOT(slt_btnLogin_Clicked()));
    connect(tabResultQuery, SIGNAL(currentChanged(int)), this, SLOT(slt_tabCurrentChanged(int)));
}

//初始化数据
void ResultQueryWidget::initData()
{
}

//槽：切换界面
void ResultQueryWidget::slt_tabCurrentChanged(int index)
{
    if(index == 1 )
        qcWidget->updateTableData();
}

