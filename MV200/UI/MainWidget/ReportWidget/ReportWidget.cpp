#include "ReportWidget.h"

//实例对象
ReportWidget *ReportWidget::_instance = 0;
ReportWidget * ReportWidget::getInstance()
{
    if (_instance == 0)
        _instance = new ReportWidget();
    return _instance;
}

ReportWidget::ReportWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();          //初始化
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}

//初始化
void ReportWidget::initForm()
{
    layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(new HistoryResultWidget,  0, 0, 100, 100);
    this->setLayout(layout);
}

//界面风格
void ReportWidget::initStyle()
{
}

//连接信号与槽
void ReportWidget::initConnect()
{
    //connect(btnCancel, SIGNAL(clicked()), this, SLOT(slt_btnCancel_Clicked()));
    //connect(btnLogin, SIGNAL(clicked()), this, SLOT(slt_btnLogin_Clicked()));
}

//初始化数据
void ReportWidget::initData()
{
}
