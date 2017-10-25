#include "InitWidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>


InitWidget::InitWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();         //初始化
    initStyle();        //界面风格
    initConnect();      //连接信号与槽
    initData();         //初始化数据
}

//初始化
void InitWidget::initForm()
{
    fraLogo = new QFrame;
    labInitMsg = new QLabel;
    layout = new QGridLayout;
    layout->setMargin(10);
    layout->setSpacing(10);
    layout->addWidget(fraLogo,      0,  0, 35, 100, Qt::AlignCenter);
    layout->addWidget(labInitMsg,  30,  0,  5, 100, Qt::AlignCenter);
    this->setLayout(layout);
}

//界面风格
void InitWidget::initStyle()
{
    int width, height;
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();
    width = deskRect.width();
    height = deskRect.height();
    this->setGeometry(0, 0, width, height);
    this->setMaximumSize(width, height);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowShadeButtonHint);       //去掉标题栏

    //图标设置
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

    //等待界面（设置背景）
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap( QString::fromUtf8("icon/Wait.png") )));
    fraLogo->setPalette(palette);
    fraLogo->setAutoFillBackground(true);
    fraLogo->setFixedSize(999, 500);

    QFont font;
    font.setPointSize(30);
    labInitMsg->setFont(font);
}

//连接信号与槽
void InitWidget::initConnect()
{
    connect(gScheduler, SIGNAL(sig_initMachine_Step(QString)), this, SLOT(slt_initStep(QString)));
    connect(gScheduler, SIGNAL(sig_Finished_init()), this, SLOT(slt_ShowMainWidget()));
}

//初始化数据
void InitWidget::initData()
{
    slt_initStep("开机初始化...");
}

//开机初始化
void InitWidget::StartInit()
{
    this->show();
    gScheduler->InitialMachine();
}

//进入MV100主界面
void InitWidget::showMainWidget()
{
    this->close();
    gMV200_MainWidget->show();
}

//退出程序
void InitWidget::closeMV200()
{
    this->close();
}

//槽：初始化进行的步骤
void InitWidget::slt_initStep(QString msg)
{
    labInitMsg->setText(msg);
}


//槽：进入MV100主界面
void InitWidget::slt_ShowMainWidget()
{
    //界面动画，改变透明度的方式消失1 - 0渐变
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, SIGNAL(finished()), this, SLOT(slt_CloseMV100()));

    gMV200_MainWidget->show();
    QPropertyAnimation  *animation2 = new QPropertyAnimation(gMV200_MainWidget, "windowOpacity");
    animation2->setDuration(1500);
    animation2->setStartValue(0);
    animation2->setEndValue(1);
    animation2->start();
}

//槽：退出程序
void InitWidget::slt_CloseMV100()
{
    this->close();
}




























