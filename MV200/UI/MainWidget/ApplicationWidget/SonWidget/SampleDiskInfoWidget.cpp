#include "SampleDiskInfoWidget.h"
#include <QFile>
#include <qdebug.h>

SampleDiskInfoWidget *gSampleApplicationSampleDiskInfo; //样本申请-样本盘信息
SampleDiskInfoWidget *gQCApplicationSampleDiskInfo;     //质控申请-样本盘信息
SampleDiskInfoWidget::SampleDiskInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();              //初始化
    initData();              //初始化数据
    initStyle();             //界面风格
    initConnect();           //连接信号与槽
}

//重置样本编号
void SampleDiskInfoWidget::setSampleNoStart(int no)
{
    SampleNoStart = no;
}

//初始化
void SampleDiskInfoWidget::initForm()
{
    initSampleDiskForm();
    layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(10);
    layout->addWidget(wdgSample,       0,  0, 100, 50);
    this->setLayout(layout);

    sigMapper = new QSignalMapper(this);
}

//初始化样本盘
void SampleDiskInfoWidget::initSampleDiskForm()
{
    //第一排样本架
    wdgSample1 = new QWidget;
    lytSample1 = new QGridLayout;
    lytSample1->setMargin(2);
    lytSample1->setSpacing(1);
    labSample1 = new QLabel("样本架一"); //样本架一
    editBatch1 = new QLineEdit;
    lytSample1->addWidget(labSample1,    0,  0, 1, 5, Qt::AlignCenter);
    lytSample1->addWidget(editBatch1,    0,  5, 1, 5, Qt::AlignLeft);
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        btnSample1[i] = new QRadioButton(" "+QString::number(i+1));
        btnSample1[i]->setFixedSize(50, 50);
        btnSample1[i]->setAutoExclusive(false);  //支持多选
        editSample1[i] = new QLineEdit;
        lytSample1->addWidget(btnSample1[i],   i+2,  0, 1, 5, Qt::AlignRight);
        lytSample1->addWidget(editSample1[i],  i+2,  5, 1, 5, Qt::AlignLeft);
    }
    wdgSample1->setLayout(lytSample1);

    //第二排样本架
    wdgSample2 = new QWidget;
    lytSample2 = new QGridLayout;
    lytSample2->setMargin(2);
    lytSample2->setSpacing(1);
    labSample2 = new QLabel("样本架二"); //样本架二
    editBatch2 = new QLineEdit;
    lytSample2->addWidget(labSample2,    0,  0, 1, 5, Qt::AlignCenter);
    lytSample2->addWidget(editBatch2,    0,  5, 1, 5, Qt::AlignLeft);
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        btnSample2[i] = new QRadioButton(" "+QString::number(i+1));
        btnSample2[i]->setFixedSize(50, 50);
        btnSample2[i]->setAutoExclusive(false);  //支持多选
        editSample2[i] = new QLineEdit;
        lytSample2->addWidget(btnSample2[i],   i+2,  0, 1, 5, Qt::AlignRight);
        lytSample2->addWidget(editSample2[i],  i+2,  5, 1, 5, Qt::AlignLeft);
    }
    wdgSample2->setLayout(lytSample2);

    //第三排样本架
    wdgSample3 = new QWidget;
    lytSample3 = new QGridLayout;
    lytSample3->setMargin(2);
    lytSample3->setSpacing(1);
    labSample3 = new QLabel("样本架三"); //样本架三
    editBatch3 = new QLineEdit;
    lytSample3->addWidget(labSample3,    0,  0, 1, 5, Qt::AlignCenter);
    lytSample3->addWidget(editBatch3,    0,  5, 1, 5, Qt::AlignLeft);
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        btnSample3[i] = new QRadioButton(" "+QString::number(i+1));
        btnSample3[i]->setFixedSize(50, 50);
        btnSample3[i]->setAutoExclusive(false);  //支持多选
        editSample3[i] = new QLineEdit;
        lytSample3->addWidget(btnSample3[i],   i+2,  0, 1, 5, Qt::AlignRight);
        lytSample3->addWidget(editSample3[i],  i+2,  5, 1, 5, Qt::AlignLeft);
    }
    wdgSample3->setLayout(lytSample3);

    //急诊样本架
    wdgSample0 = new QWidget;
    labSample0 = new QLabel("急诊样本"); //急诊样本架
    btnSample0[0] = new QRadioButton(" 1");
    btnSample0[1] = new QRadioButton(" 2");
    btnSample0[0]->setFixedSize(50, 50);
    btnSample0[1]->setFixedSize(50, 50);
    btnSample0[0]->setAutoExclusive(false);  //支持多选
    btnSample0[1]->setAutoExclusive(false);  //支持多选
    editSample0[0] = new QLineEdit;
    editSample0[1] = new QLineEdit;
//    lytSample0 = new QGridLayout;
//    lytSample0->setMargin(2);
//    lytSample0->setSpacing(1);
//    lytSample0->addWidget(labSample0,     0,  0, 5, 20, Qt::AlignCenter);
//    lytSample0->addWidget(btnSample0[0],  5,  0, 5, 10, Qt::AlignCenter);
//    lytSample0->addWidget(editSample0[0], 5, 10, 5, 10, Qt::AlignLeft);
//    lytSample0->addWidget(btnSample0[1], 10,  0, 5, 10, Qt::AlignCenter);
//    lytSample0->addWidget(editSample0[1],10, 10, 5, 10, Qt::AlignLeft);
    //wdgSample0->setLayout(lytSample0);

    wdgButton = new QWidget;
    btnAppend = new QPushButton("追 加");     //追加
    btnSure = new QPushButton("确 定");       //确定
    btnReset = new QPushButton("重 置");      //重置
    btnCancel = new QPushButton("取 消");     //取消
    btnBack = new QPushButton("返 回");       //返回
    lytvButton = new QVBoxLayout;
    lytvButton->setMargin(3);
    lytvButton->setSpacing(10);
    //lytvButton->addWidget(btnAppend, Qt::AlignCenter);
    lytvButton->addWidget(btnSure, Qt::AlignCenter);
    lytvButton->addWidget(btnReset, Qt::AlignCenter);
    lytvButton->addWidget(btnCancel, Qt::AlignCenter);
    lytvButton->addWidget(btnBack, Qt::AlignCenter);
    wdgButton->setLayout(lytvButton);

    wdgSample = new QWidget;
    lytSample = new QGridLayout;
    lytSample->setMargin(5);
    lytSample->setSpacing(5);
    lytSample->addWidget(wdgSample1,     0,  0, 50, 10);
    lytSample->addWidget(wdgSample2,     0, 10, 50, 10);
    lytSample->addWidget(wdgSample3,     0, 20, 50, 10);
    lytSample->addWidget(wdgSample0,     0, 30, 13, 10);
    lytSample->addWidget(wdgButton,     20, 30, 30, 10, Qt::AlignHCenter | Qt::AlignBottom);
    wdgSample->setLayout(lytSample);
}

//初始化数据
void SampleDiskInfoWidget::initData()
{
    SampleNoStart2 = SampleNoStart = 1;

    editBatch1->setPlaceholderText("批量");
    editBatch2->setPlaceholderText("批量");
    editBatch3->setPlaceholderText("批量");

    //样本个数（30个普通样本）, 急诊样本个数（2个急诊样本）
    for(int i = 0; i < SAMPLE_COUNT+URGENT_COUNT; i ++)
        SampleNoMap.insert(i, "");
}

//界面风格
void SampleDiskInfoWidget::initStyle()
{
    //图标路径
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    this->setWindowTitle("选择样本位");
    this->setMinimumWidth(800);
    this->setWindowFlags(this->windowFlags()& ~Qt::WindowMinMaxButtonsHint | Qt::WindowStaysOnTopHint);       //置顶&隐藏最大/最小化

    //设置背景
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap( ":/resource/resource/bg/mainBg.png" )));  //
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    QString strBtn =//空闲
            "QRadioButton{background: transparent; width:50; height:50;  border-radius:5px; font-size:25px; background-image:url(:/resource/resource/SampleStatus/Idle.png); background-repeat:no-repeat; background-position:center; }"
            "QRadioButton:hover{background: transparent; width:50; height:50; border-radius:8px; font-size:22px; color:darkCyan; background-image:url(:/resource/resource/SampleStatus/Idle.png); background-repeat:no-repeat; background-position:center; }"
            "QRadioButton:checked{background: transparent; width:50; height:50; border-radius:8px; font-size:35px; color:red; background-image:url(:/resource/resource/SampleStatus/Idle.png); background-repeat:no-repeat; background-position:center; }"
            "QRadioButton::indicator {background: transparent; width:1px; height:1px; background-image:url(:/resource/resource/SampleStatus);}";

    //QSS
    QFile qss( ":/QSS/QSS/mvStyle.qss" );
    qss.open(QFile::ReadOnly);
    setStyleSheet( qss.readAll() + strBtn);
    qss.close();

    btnSure->setObjectName("button6");
    btnCancel->setObjectName("button6");
    btnAppend->setObjectName("button6");
    btnReset->setObjectName("button6");
    btnBack->setObjectName("button6");
    editBatch1->setFixedWidth(100);
    editBatch2->setFixedWidth(100);
    editBatch3->setFixedWidth(100);
    editBatch1->setValidator(new QIntValidator(1, 10, this));
    editBatch2->setValidator(new QIntValidator(1, 10, this));
    editBatch3->setValidator(new QIntValidator(1, 10, this));
    editBatch1->setAlignment(Qt::AlignCenter);
    editBatch2->setAlignment(Qt::AlignCenter);
    editBatch3->setAlignment(Qt::AlignCenter);

}

//连接信号与槽
void SampleDiskInfoWidget::initConnect()
{
    connect(btnSure, SIGNAL(clicked()), this, SLOT(slt_btnSure_Clicked()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(slt_btnCancel_Clicked()));
    connect(btnReset, SIGNAL(clicked()), this, SLOT(slt_btnReset_Clicked()));
    connect(btnAppend, SIGNAL(clicked()), this, SLOT(slt_btnAppend_Clicked()));
    connect(btnBack, SIGNAL(clicked()), this, SLOT(slt_btnBack_Clicked()));

    connect(editBatch1, SIGNAL(textChanged(QString)), this, SLOT(slt_editBatch1_Change(QString)));
    connect(editBatch2, SIGNAL(textChanged(QString)), this, SLOT(slt_editBatch2_Change(QString)));
    connect(editBatch3, SIGNAL(textChanged(QString)), this, SLOT(slt_editBatch3_Change(QString)));

    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        sigMapper->setMapping(btnSample1[i], i);
        connect(btnSample1[i],SIGNAL(clicked()),sigMapper,SLOT(map()));

        sigMapper->setMapping(btnSample2[i], 10+i);
        connect(btnSample2[i],SIGNAL(clicked()),sigMapper,SLOT(map()));

        sigMapper->setMapping(btnSample3[i], 20+i);
        connect(btnSample3[i],SIGNAL(clicked()),sigMapper,SLOT(map()));
    }
    sigMapper->setMapping(btnSample0[0], 30);
    connect(btnSample0[0],SIGNAL(clicked()),sigMapper,SLOT(map()));
    sigMapper->setMapping(btnSample0[1], 31);
    connect(btnSample0[1],SIGNAL(clicked()),sigMapper,SLOT(map()));
    connect(sigMapper,SIGNAL(mapped(const int)), this,SLOT(slt_sigMapReceive(const int)));
}

//生成系统条码(样本条码)
QString SampleDiskInfoWidget::DefaultSampleCode(int no)
{
    SampleNoStart ++;
    QString temp;
    temp = QString::number(no);
    return temp;
}

//清空所有
void SampleDiskInfoWidget::ClearAll()
{
    btnSample0[0]->setChecked(false);
    btnSample0[1]->setChecked(false);
    editSample0[0]->setPlaceholderText("");
    editSample0[1]->setPlaceholderText("");
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        btnSample1[i]->setChecked(false);
        btnSample2[i]->setChecked(false);
        btnSample3[i]->setChecked(false);
        editSample1[i]->setPlaceholderText("");
        editSample2[i]->setPlaceholderText("");
        editSample3[i]->setPlaceholderText("");
        editSample1[i]->setText("");
        editSample2[i]->setText("");
        editSample3[i]->setText("");
    }
    editBatch1->clear();
    editBatch2->clear();
    editBatch3->clear();

    //样本个数（30个普通样本）, 急诊样本个数（2个急诊样本）
    for(int i = 0; i < SAMPLE_COUNT+URGENT_COUNT; i ++)
        SampleNoMap.insert(i, "");
}

//确定
void SampleDiskInfoWidget::slt_btnSure_Clicked()
{
    QString sample = "";
    QString code = "";

    //急诊样本
    if(btnSample0[0]->isChecked())
    {
        code = editSample0[0]->text().trimmed();
        if(code.count() == 0)
            code = editSample0[0]->placeholderText().trimmed();
        SampleNoMap.insert(30, code);
        code = "";
        sample += "(0-1),";
    }
    if(btnSample0[1]->isChecked())
    {
        code = editSample0[1]->text().trimmed();
        if(code.count() == 0)
            code = editSample0[1]->placeholderText().trimmed();
        SampleNoMap.insert(31, code);
        code = "";
        sample += "(0-2),";
    }

    //样本架一
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
        if(btnSample1[i]->isChecked())
        {
            code = editSample1[i]->text().trimmed();
            if(code.count() == 0)
                code = editSample1[i]->placeholderText().trimmed();
            SampleNoMap.insert(i, code);
            code = "";
            sample += "(1-" + QString::number(i+1) + "),";
        }

    //样本架二
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
        if(btnSample2[i]->isChecked())
        {
            code = editSample2[i]->text().trimmed();
            if(code.count() == 0)
                code = editSample2[i]->placeholderText().trimmed();
            SampleNoMap.insert(10+i, code);
            code = "";
            sample += "(2-" + QString::number(i+1) + "),";
        }

    //样本架三
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
        if(btnSample3[i]->isChecked())
        {
            code = editSample3[i]->text().trimmed();
            if(code.count() == 0)
                code = editSample3[i]->placeholderText().trimmed();
            SampleNoMap.insert(20+i, code);
            code = "";
            sample += "(3-" + QString::number(i+1) + "),";
        }

    emit sig_SelectedSampleList(sample, SampleNoMap);
    ClearAll();
    SampleNoStart2 = SampleNoStart;
    ///gParameter->setSampleNo(SampleNoStart);
    this->hide();
}

//取消
void SampleDiskInfoWidget::slt_btnCancel_Clicked()
{
    SampleNoStart = SampleNoStart2;
    emit sig_SelectedSampleList("", SampleNoMap);
    ClearAll();
    this->hide();
}

//追加
void SampleDiskInfoWidget::slt_btnAppend_Clicked()
{

}

//重置
void SampleDiskInfoWidget::slt_btnReset_Clicked()
{
    SampleNoStart = SampleNoStart2;
    ClearAll();
}

//返回
void SampleDiskInfoWidget::slt_btnBack_Clicked()
{
    this->hide();
}

//槽：批量申请
void SampleDiskInfoWidget::slt_editBatch1_Change(QString text)
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
        {
            if(editSample1[i+pt]->placeholderText().count()==0)
                editSample1[i+pt]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
            btnSample1[i+pt]->setChecked(true);
        }
        else
        {
            btnSample1[i+pt]->setChecked(false);
        }
    }
}

//槽：批量申请
void SampleDiskInfoWidget::slt_editBatch2_Change(QString text)
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
        {
            if(editSample2[i+pt]->placeholderText().count()==0)
                editSample2[i+pt]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
            btnSample2[i+pt]->setChecked(true);
        }
        else
            btnSample2[i+pt]->setChecked(false);
    }
}

//槽：批量申请
void SampleDiskInfoWidget::slt_editBatch3_Change(QString text)
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
        {
            if(editSample3[i+pt]->placeholderText().count()==0)
                editSample3[i+pt]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
            btnSample3[i+pt]->setChecked(true);
        }
        else
            btnSample3[i+pt]->setChecked(false);
    }
}

//槽：选中样本自动生成样本编号
void SampleDiskInfoWidget::slt_sigMapReceive(const int id)
{

    if(id < 10)
    {
        if(btnSample1[id]->isChecked())
        {
            if(editSample1[id]->placeholderText().count()==0)
                editSample1[id]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
        }
        else
        {
            editSample1[id]->setPlaceholderText("");
            editSample1[id]->clear();
        }
    }
    else if( id >= 10 && id < 20)
    {
        if(btnSample2[id-10]->isChecked())
        {
            if(editSample2[id-10]->placeholderText().count()==0)
                editSample2[id-10]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
        }
        else
        {
            editSample2[id-10]->setPlaceholderText("");
            editSample2[id-10]->clear();
        }
    }
    else if (id >= 20 &&id < 30)
    {
        if(btnSample3[id-20]->isChecked())
        {
            if(editSample3[id-20]->placeholderText().count()==0)
                editSample3[id-20]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
        }
        else
        {
            editSample3[id-20]->setPlaceholderText("");
            editSample3[id-20]->clear();
        }
    }
    else
    {
        if(btnSample0[id-30]->isChecked())
        {
            if(editSample0[id-30]->placeholderText().count()==0)
                editSample0[id-30]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
        }
        else
        {
            editSample0[id-30]->setPlaceholderText("");
            editSample0[id-30]->clear();
        }
    }
}

