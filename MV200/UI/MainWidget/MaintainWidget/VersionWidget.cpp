#include "VersionWidget.h"
#include "Parameter.h"

VersionWidget::VersionWidget(QDialog *parent)
    : QDialog(parent)
{
    initForm();          //初始化
    initStyle();         //界面风格
}

//初始化
void VersionWidget::initForm()
{
    labName0 = new QLabel("a）软件名称：");            //软件名称
    labName = new QLabel("特定蛋白分析仪");
    labSpecifications0 = new QLabel("b）软件型号规格：");  //型号规格
    labSpecifications = new QLabel(gParameter->getMachineName());
    labVersion0 = new QLabel("c）发布版本：");            //版本
    labVersion = new QLabel("Specific Protein Analyze " + gParameter->getMachineName() + " V 1.00.00.00");
    labCVersion0 = new QLabel("d）完整版本：");            //版本
    labCVersion = new QLabel("Specific Protein Analyze " + gParameter->getMachineName() + " V 1.00.00.00");

    layout = new QGridLayout;
    layout->setMargin(10);
    layout->setSpacing(5);
    layout->addWidget(labName0,           0,  0, 5, 10, Qt::AlignLeft);
    layout->addWidget(labName,            0, 10, 5, 20, Qt::AlignLeft);
    layout->addWidget(labSpecifications0, 5,  0, 5, 10, Qt::AlignLeft);
    layout->addWidget(labSpecifications,  5, 10, 5, 20, Qt::AlignLeft);
    layout->addWidget(labVersion0,       10,  0, 5, 10, Qt::AlignLeft);
    layout->addWidget(labVersion,        10, 10, 5, 20, Qt::AlignLeft);
    layout->addWidget(labCVersion0,      15,  0, 5, 10, Qt::AlignLeft);
    layout->addWidget(labCVersion,       15, 10, 5, 20, Qt::AlignLeft);
    this->setLayout(layout);
}

//界面风格
void VersionWidget::initStyle()
{
    //图标设置
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    this->setWindowTitle("关于");

    //QSS
    QFile qss( ":/QSS/QSS/style2.qss" );
    qss.open(QFile::ReadOnly);
    setStyleSheet( qss.readAll() );
    qss.close();

    this->setWindowFlags(Qt::WindowStaysOnTopHint);       //置顶
}
