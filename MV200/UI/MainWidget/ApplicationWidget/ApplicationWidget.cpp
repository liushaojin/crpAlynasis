#include "ApplicationWidget.h"

//实例对象
ApplicationWidget *ApplicationWidget::_instance = 0;
ApplicationWidget * ApplicationWidget::getInstance()
{
    if (_instance == 0)
        _instance = new ApplicationWidget();
    return _instance;
}

ApplicationWidget::ApplicationWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();          //初始化
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}


//初始化
void ApplicationWidget::initForm()
{
    tabApplication = new QTabWidget;
    layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(tabApplication);
    this->setLayout(layout);

    //
    tabApplication->addTab( gSampleApplication = new SampleApplication, "样本申请");
    tabApplication->addTab(new QCApplication, "质控申请");
    //tabApplication->addTab(new QWidget, "定标申请");
}

//界面风格
void ApplicationWidget::initStyle()
{
    tabApplication->setObjectName("tabW");
}

//连接信号与槽
void ApplicationWidget::initConnect()
{
    //connect(btnCancel, SIGNAL(clicked()), this, SLOT(slt_btnCancel_Clicked()));
    //connect(btnLogin, SIGNAL(clicked()), this, SLOT(slt_btnLogin_Clicked()));
}

//初始化数据
void ApplicationWidget::initData()
{
}
