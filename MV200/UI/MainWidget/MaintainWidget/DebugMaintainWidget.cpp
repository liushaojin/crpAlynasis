#include "DebugMaintainWidget.h"

DebugMaintainWidget::DebugMaintainWidget(QWidget *parent)
    : QWidget(parent)
{

    initForm();          //初始化
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}


//初始化
void DebugMaintainWidget::initForm()
{
    tabMaintain = new QTabWidget;
    layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(tabMaintain,  0, 0, 100, 100);
    this->setLayout(layout);

    //
    tabMaintain->addTab(new UnitMaintain, "单元测试");
    tabMaintain->addTab(LightDebugWidget::getInstance(), "采光调试界面");
}

//界面风格
void DebugMaintainWidget::initStyle()
{
    tabMaintain->setObjectName("tabW");
}

//连接信号与槽
void DebugMaintainWidget::initConnect()
{
}

//初始化数据
void DebugMaintainWidget::initData()
{
}
