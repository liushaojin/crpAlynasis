#include "SampleDiskWidget.h"
#include <QFile>
#include <qdebug.h>

SampleDiskWidget::SampleDiskWidget(QWidget *parent) : QWidget(parent)
{
    initForm();              //初始化
    initData();              //初始化数据
    initStyle();             //界面风格
    initConnect();           //连接信号与槽
}

//初始化
void SampleDiskWidget::initForm()
{
    initSampleDiskForm();
    layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(10);
    layout->addWidget(wdgSample,       0,  0, 100, 50);
    this->setLayout(layout);
}

//初始化样本盘
void SampleDiskWidget::initSampleDiskForm()
{
    //第一排样本架
    wdgSample1 = new QWidget;
    lytvSample1 = new QVBoxLayout;
    lytvSample1->setMargin(2);
    lytvSample1->setSpacing(1);
    labSample1 = new QLabel("样本架一"); //样本架一
    editBatch1 = new QLineEdit;
    lytvSample1->addWidget(labSample1, Qt::AlignCenter);
    lytvSample1->addWidget(editBatch1, Qt::AlignCenter);
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        btnSample1[i] = new QRadioButton(" "+QString::number(i+1));
        btnSample1[i]->setFixedSize(50, 50);
        btnSample1[i]->setAutoExclusive(false);  //支持多选
        lytvSample1->addWidget(btnSample1[i], Qt::AlignCenter);
    }
    wdgSample1->setLayout(lytvSample1);

    //第二排样本架
    wdgSample2 = new QWidget;
    lytvSample2 = new QVBoxLayout;
    lytvSample2->setMargin(2);
    lytvSample2->setSpacing(1);
    labSample2 = new QLabel("样本架二"); //样本架二
    editBatch2 = new QLineEdit;
    lytvSample2->addWidget(labSample2, Qt::AlignCenter);
    lytvSample2->addWidget(editBatch2, Qt::AlignCenter);
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        btnSample2[i] = new QRadioButton(" "+QString::number(i+1));
        btnSample2[i]->setFixedSize(50, 50);
        btnSample2[i]->setAutoExclusive(false);  //支持多选
        lytvSample2->addWidget(btnSample2[i], Qt::AlignCenter);
    }
    wdgSample2->setLayout(lytvSample2);

    //第三排样本架
    wdgSample3 = new QWidget;
    lytvSample3 = new QVBoxLayout;
    lytvSample3->setMargin(2);
    lytvSample3->setSpacing(1);
    labSample3 = new QLabel("样本架三"); //样本架三
    editBatch3 = new QLineEdit;
    lytvSample3->addWidget(labSample3, Qt::AlignCenter);
    lytvSample3->addWidget(editBatch3, Qt::AlignCenter);
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        btnSample3[i] = new QRadioButton(" "+QString::number(i+1));
        btnSample3[i]->setFixedSize(50, 50);
        btnSample3[i]->setAutoExclusive(false);  //支持多选
        lytvSample3->addWidget(btnSample3[i], Qt::AlignCenter);
    }
    wdgSample3->setLayout(lytvSample3);

    //急诊样本架
    wdgSample0 = new QWidget;
    labSample0 = new QLabel("急诊样本"); //急诊样本架
    btnSample0[0] = new QRadioButton(" 1");
    btnSample0[1] = new QRadioButton(" 2");
    btnSample0[0]->setFixedSize(50, 50);
    btnSample0[1]->setFixedSize(50, 50);
    btnSample0[0]->setAutoExclusive(false);  //支持多选
    btnSample0[1]->setAutoExclusive(false);  //支持多选
    lytSample0 = new QGridLayout;
    lytSample0->setMargin(2);
    lytSample0->setSpacing(1);
    lytSample0->addWidget(labSample0,    0,  0, 5, 20, Qt::AlignCenter);
    lytSample0->addWidget(btnSample0[0], 5,  0, 5, 10, Qt::AlignCenter);
    lytSample0->addWidget(btnSample0[1], 5, 10, 5, 10, Qt::AlignCenter);
    wdgSample0->setLayout(lytSample0);

    wdgButton = new QWidget;
    btnAppend = new QPushButton("追 加");     //追加
    btnSure = new QPushButton("确 定");       //确定
    btnReset = new QPushButton("重 置");      //重置
    btnCancel = new QPushButton("取 消");     //取消
    lytvButton = new QVBoxLayout;
    lytvButton->setMargin(3);
    lytvButton->setSpacing(10);
    //lytvButton->addWidget(btnAppend, Qt::AlignCenter);
    lytvButton->addWidget(btnSure, Qt::AlignCenter);
    lytvButton->addWidget(btnReset, Qt::AlignCenter);
    lytvButton->addWidget(btnCancel, Qt::AlignCenter);
    wdgButton->setLayout(lytvButton);

    wdgSample = new QWidget;
    lytSample = new QGridLayout;
    lytSample->setMargin(5);
    lytSample->setSpacing(2);
    lytSample->addWidget(wdgSample1,     0,  0, 50, 10);
    lytSample->addWidget(wdgSample2,     0, 10, 50, 10);
    lytSample->addWidget(wdgSample3,     0, 20, 50, 10);
    lytSample->addWidget(wdgSample0,     0, 30, 20, 10, Qt::AlignTop);
    lytSample->addWidget(wdgButton,     20, 30, 30, 10, Qt::AlignHCenter | Qt::AlignBottom);
    wdgSample->setLayout(lytSample);
}

//初始化数据
void SampleDiskWidget::initData()
{
    editBatch1->setPlaceholderText("批量");
    editBatch2->setPlaceholderText("批量");
    editBatch3->setPlaceholderText("批量");
}

//界面风格
void SampleDiskWidget::initStyle()
{
    this->setWindowFlags(Qt::WindowStaysOnTopHint);       //去掉标题栏
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);       //去掉标题栏

    QString strBtn =
            //空闲
            "QRadioButton{ width:50; height:50;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/SampleStatus/Idle.png); background-repeat:no-repeat; background-position:center; }"
            "QRadioButton:hover{ width:50; height:50; border-radius:8px; font-size:22px; color:darkCyan; background-image:url(:/resource/resource/SampleStatus/Idle.png); background-repeat:no-repeat; background-position:center; }"
            "QRadioButton:checked{ width:50; height:50; border-radius:8px; font-size:35px; color:red; background-image:url(:/resource/resource/SampleStatus/Idle.png); background-repeat:no-repeat; background-position:center; }"
            "QRadioButton::indicator {width:1px; height:1px; background-image:url(:/resource/resource/SampleStatus);}";

    //QSS
    QFile qss( ":/QSS/QSS/mv100.qss" );
    qss.open(QFile::ReadOnly);
    setStyleSheet( qss.readAll() +strBtn);
    qss.close();

    btnSure->setObjectName("button6");
    btnCancel->setObjectName("button6");
    btnAppend->setObjectName("button6");
    btnReset->setObjectName("button6");

    editBatch1->setFixedWidth(50);
    editBatch2->setFixedWidth(50);
    editBatch3->setFixedWidth(50);

    editBatch1->setValidator(new QIntValidator(1, 10, this));
    editBatch2->setValidator(new QIntValidator(1, 10, this));
    editBatch3->setValidator(new QIntValidator(1, 10, this));

    editBatch1->setAlignment(Qt::AlignCenter);
    editBatch2->setAlignment(Qt::AlignCenter);
    editBatch3->setAlignment(Qt::AlignCenter);
}

//连接信号与槽
void SampleDiskWidget::initConnect()
{
    connect(btnSure, SIGNAL(clicked()), this, SLOT(slt_btnSure_Clicked()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(slt_btnCancel_Clicked()));
    connect(btnReset, SIGNAL(clicked()), this, SLOT(slt_btnReset_Clicked()));
    connect(btnAppend, SIGNAL(clicked()), this, SLOT(slt_btnAppend_Clicked()));

    connect(editBatch1, SIGNAL(textChanged(QString)), this, SLOT(slt_editBatch1_Change(QString)));
    connect(editBatch2, SIGNAL(textChanged(QString)), this, SLOT(slt_editBatch2_Change(QString)));
    connect(editBatch3, SIGNAL(textChanged(QString)), this, SLOT(slt_editBatch3_Change(QString)));
}

//确定
void SampleDiskWidget::slt_btnSure_Clicked()
{
    QString sample = "";

    //急诊样本
    if(btnSample0[0]->isChecked())
        sample += "(0-1),";
    if(btnSample0[1]->isChecked())
        sample += "(0-2),";

    //样本架一
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
        if(btnSample1[i]->isChecked())
            sample += "(1-" + QString::number(i+1) + "),";

    //样本架二
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
        if(btnSample2[i]->isChecked())
            sample += "(2-" + QString::number(i+1) + "),";

    //样本架三
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
        if(btnSample3[i]->isChecked())
            sample += "(3-" + QString::number(i+1) + "),";

    //qDebug() << sample;

    emit sig_SelectedSampleList(sample);
    setSampleStatus(false);
    this->hide();
}

//取消
void SampleDiskWidget::slt_btnCancel_Clicked()
{
    emit sig_SelectedSampleList("");
    this->hide();
}

//追加
void SampleDiskWidget::slt_btnAppend_Clicked()
{

}

//重置
void SampleDiskWidget::slt_btnReset_Clicked()
{
    btnSample0[0]->setChecked(false);
    btnSample0[1]->setChecked(false);
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        btnSample1[i]->setChecked(false);
        btnSample2[i]->setChecked(false);
        btnSample3[i]->setChecked(false);
    }
    editBatch1->clear();
    editBatch2->clear();
    editBatch3->clear();
}

//设置样本状态
void SampleDiskWidget::setSampleStatus(int flag)
{
    for( int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        btnSample1[i]->setChecked(flag);
        btnSample2[i]->setChecked(flag);
        btnSample3[i]->setChecked(flag);
    }

    btnSample0[0]->setChecked(flag);
    btnSample0[1]->setChecked(flag);

    editBatch1->clear();
    editBatch2->clear();
    editBatch3->clear();
}

//槽：批量申请
void SampleDiskWidget::slt_editBatch1_Change(QString text)
{
    if( text.count() == 0) return;
    int index;
    int pt = 0; //批量申请开始的位置
    for(index = 0; index < SAMPLESLOT_COUNT; index ++)
    {
        if( btnSample1[index]->isChecked() )
        {
            pt = index;
            break;
        }
    }

    int count = text.toInt();
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        if (i +pt == SAMPLESLOT_COUNT) break;
        if( i < count)
            btnSample1[i+pt]->setChecked(true);
        else
            btnSample1[i+pt]->setChecked(false);
    }
}

//槽：批量申请
void SampleDiskWidget::slt_editBatch2_Change(QString text)
{
    if( text.count() == 0) return;
    int index;
    int pt = 0; //批量申请开始的位置
    for(index = 0; index < SAMPLESLOT_COUNT; index ++)
    {
        if( btnSample2[index]->isChecked() )
        {
            pt = index;
            break;
        }
    }

    int count = text.toInt();
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        if (i +pt == SAMPLESLOT_COUNT) break;
        if( i < count)
            btnSample2[i+pt]->setChecked(true);
        else
            btnSample2[i+pt]->setChecked(false);
    }
}

//槽：批量申请
void SampleDiskWidget::slt_editBatch3_Change(QString text)
{
    if( text.count() == 0) return;
    int index;
    int pt = 0; //批量申请开始的位置
    for(index = 0; index < SAMPLESLOT_COUNT; index ++)
    {
        if( btnSample3[index]->isChecked() )
        {
            pt = index;
            break;
        }
    }

    int count = text.toInt();
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        if (i +pt == SAMPLESLOT_COUNT) break;
        if( i < count)
            btnSample3[i+pt]->setChecked(true);
        else
            btnSample3[i+pt]->setChecked(false);
    }
}
