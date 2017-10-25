#include "SelectSampleWidget.h"
#include <QFile>
#include <qdebug.h>

SelectSampleWidget *gSelectSampleWidget; //样本申请-样本盘信息
SelectSampleWidget::SelectSampleWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();              //初始化
    initData();              //初始化数据
    initStyle();             //界面风格
    initConnect();           //连接信号与槽
}

//重置样本架编号
void SelectSampleWidget::setSampleNoStart(int no)
{
    SampleNoStart = no;
}

//更新样本架编号（确定申请后调用）
void SelectSampleWidget::updateSampleSlotNo()
{
    ClearAll();
    gParameter->setSampleNo(SampleNoStart);

    editSlotNo3->setVisible(false);
    fraSample3->setVisible(false);
    editBatch3->setVisible(false);
    editBatch3->clear();
    editSlotNo4->setVisible(false);
    fraSample4->setVisible(false);
    editBatch4->setVisible(false);
    editBatch4->clear();
    editSlotNo5->setVisible(false);
    fraSample5->setVisible(false);
    editBatch5->setVisible(false);
    editBatch5->clear();
    SampleSlotNo = ++ slot; //先自增
    editSlotNo1->setText(QString::number(SampleSlotNo ++));
    editSlotNo2->setText(QString::number(SampleSlotNo ++));
}

//初始化
void SelectSampleWidget::initForm()
{
    initSampleDiskForm();
    layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(10);
    layout->addWidget(wdgSample,       0,  0, 100, 100);
    this->setLayout(layout);

    sigMapper = new QSignalMapper(this);
}

//初始化样本盘
void SelectSampleWidget::initSampleDiskForm()
{
    //第一排样本架
    editSlotNo1 = new QLineEdit;  //样本架号
    fraSample1 = new QFrame;
    QHBoxLayout *hlytSample1 = new QHBoxLayout;
    hlytSample1->setMargin(0);
    hlytSample1->setSpacing(5);
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        editSample1[i] = new QLineEdit;//??
        btnSample1[i] = new QRadioButton(" "+QString::number(i+1));
        btnSample1[i]->setFixedSize(50, 50);
        btnSample1[i]->setAutoExclusive(false);  //支持多选
        hlytSample1->addWidget(btnSample1[i]);
    }
    fraSample1->setFrameShape(QFrame::Box);
    fraSample1->setLayout(hlytSample1);
    editBatch1 = new QLineEdit;

    //第二排样本架
    editSlotNo2 = new QLineEdit;  //样本架号
    fraSample2 = new QFrame;
    QHBoxLayout *hlytSample2 = new QHBoxLayout;
    hlytSample2->setMargin(0);
    hlytSample2->setSpacing(5);
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        editSample2[i] = new QLineEdit;//??
        btnSample2[i] = new QRadioButton(" "+QString::number(i+1));
        btnSample2[i]->setFixedSize(50, 50);
        btnSample2[i]->setAutoExclusive(false);  //支持多选
        hlytSample2->addWidget(btnSample2[i]);
    }
    fraSample2->setFrameShape(QFrame::Box);
    fraSample2->setLayout(hlytSample2);
    editBatch2 = new QLineEdit;


    //第3排样本架
    editSlotNo3 = new QLineEdit;  //样本架号
    fraSample3 = new QFrame;
    QHBoxLayout *hlytSample3 = new QHBoxLayout;
    hlytSample3->setMargin(0);
    hlytSample3->setSpacing(5);
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        editSample3[i] = new QLineEdit;//??
        btnSample3[i] = new QRadioButton(" "+QString::number(i+1));
        btnSample3[i]->setFixedSize(50, 50);
        btnSample3[i]->setAutoExclusive(false);  //支持多选
        hlytSample3->addWidget(btnSample3[i]);
    }
    fraSample3->setFrameShape(QFrame::Box);
    fraSample3->setLayout(hlytSample3);
    editBatch3 = new QLineEdit;

    //第4排样本架
    editSlotNo4 = new QLineEdit;  //样本架号
    fraSample4 = new QFrame;
    QHBoxLayout *hlytSample4 = new QHBoxLayout;
    hlytSample4->setMargin(0);
    hlytSample4->setSpacing(5);
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        editSample4[i] = new QLineEdit;//??
        btnSample4[i] = new QRadioButton(" "+QString::number(i+1));
        btnSample4[i]->setFixedSize(50, 50);
        btnSample4[i]->setAutoExclusive(false);  //支持多选
        hlytSample4->addWidget(btnSample4[i]);
    }
    fraSample4->setFrameShape(QFrame::Box);
    fraSample4->setLayout(hlytSample4);
    editBatch4 = new QLineEdit;

    //第5排样本架
    editSlotNo5 = new QLineEdit;  //样本架号
    fraSample5 = new QFrame;
    QHBoxLayout *hlytSample5 = new QHBoxLayout;
    hlytSample5->setMargin(0);
    hlytSample5->setSpacing(5);
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        editSample5[i] = new QLineEdit;//??
        btnSample5[i] = new QRadioButton(" "+QString::number(i+1));
        btnSample5[i]->setFixedSize(50, 50);
        btnSample5[i]->setAutoExclusive(false);  //支持多选
        hlytSample5->addWidget(btnSample5[i]);
    }
    fraSample5->setFrameShape(QFrame::Box);
    fraSample5->setLayout(hlytSample5);
    editBatch5 = new QLineEdit;

    //按钮菜单
    wdgButton = new QWidget;
    btnAddSlot = new QPushButton("+");    //样本架+
    btnRdSlot = new QPushButton("-");     //样本架-
    btnAppend = new QPushButton("追 加");     //追加
    btnSure = new QPushButton("确 定");       //确定
    btnReset = new QPushButton("重 置");      //重置
    btnCancel = new QPushButton("取 消");     //取消
    btnBack = new QPushButton("返 回");       //返回
    lythButton = new QHBoxLayout;
    lythButton->setMargin(3);
    lythButton->setSpacing(10);
    lythButton->addWidget(btnAddSlot, Qt::AlignCenter);
    lythButton->addWidget(btnRdSlot, Qt::AlignCenter);
    lythButton->addWidget(btnSure, Qt::AlignCenter);
    lythButton->addWidget(btnReset, Qt::AlignCenter);
    lythButton->addWidget(btnCancel, Qt::AlignCenter);
    lythButton->addWidget(btnBack, Qt::AlignCenter);
    wdgButton->setLayout(lythButton);

    //样本架
    QLabel *labSlot = new QLabel("样本架");
    QLabel *labSample = new QLabel("选择样本位");
    wdgSample = new QWidget;
    lytSample = new QGridLayout;
    lytSample->setMargin(5);
    lytSample->setSpacing(5);
    lytSample->addWidget(labSlot,        0,  0, 2, 5, Qt::AlignCenter);
    lytSample->addWidget(labSample,      0,  5, 2, 20, Qt::AlignCenter);

    lytSample->addWidget(editSlotNo1,    2,  0, 4, 5, Qt::AlignCenter);
    lytSample->addWidget(fraSample1,     2,  5, 4, 20, Qt::AlignVCenter);
    lytSample->addWidget(editBatch1,     2, 25, 4, 5, Qt::AlignCenter);
    lytSample->addWidget(editSlotNo2,    6,  0, 4, 5, Qt::AlignCenter);
    lytSample->addWidget(fraSample2,     6,  5, 4, 20, Qt::AlignVCenter);
    lytSample->addWidget(editBatch2,     6, 25, 4, 5, Qt::AlignCenter);
    lytSample->addWidget(editSlotNo3,   10,  0, 4, 5, Qt::AlignCenter);
    lytSample->addWidget(fraSample3,    10,  5, 4, 20, Qt::AlignVCenter);
    lytSample->addWidget(editBatch3,    10, 25, 4, 5, Qt::AlignCenter);
    lytSample->addWidget(editSlotNo4,   14,  0, 4, 5, Qt::AlignCenter);
    lytSample->addWidget(fraSample4,    14,  5, 4, 20, Qt::AlignVCenter);
    lytSample->addWidget(editBatch4,    14, 25, 4, 5, Qt::AlignCenter);
    lytSample->addWidget(editSlotNo5,   18,  0, 4, 5, Qt::AlignCenter);
    lytSample->addWidget(fraSample5,    18,  5, 4, 20, Qt::AlignVCenter);
    lytSample->addWidget(editBatch5,    18, 25, 4, 5, Qt::AlignCenter);

    lytSample->addWidget(wdgButton,     22,  0, 4, 30, Qt::AlignCenter | Qt::AlignBottom);
    wdgSample->setLayout(lytSample);
}

//初始化数据
void SelectSampleWidget::initData()
{
    SampleNoStart2 = SampleNoStart = gParameter->getSampleNo();

    editBatch1->setPlaceholderText("批量");
    editBatch2->setPlaceholderText("批量");
    editBatch3->setPlaceholderText("批量");
    editBatch4->setPlaceholderText("批量");
    editBatch5->setPlaceholderText("批量");

    SampleSlotNo = 1;
    editSlotNo1->setText(QString::number(SampleSlotNo ++));
    editSlotNo2->setText(QString::number(SampleSlotNo ++));
}

//界面风格
void SelectSampleWidget::initStyle()
{
    //图标路径
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    this->setWindowTitle("选择样本位");
    this->setFixedSize(680, 400);
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

    btnAddSlot->setObjectName("button8");
    btnRdSlot->setObjectName("button8");
    btnSure->setObjectName("button6");
    btnCancel->setObjectName("button6");
    btnAppend->setObjectName("button6");
    btnReset->setObjectName("button6");
    btnBack->setObjectName("button6");

    editBatch1->setValidator(new QIntValidator(1, 10, this));
    editBatch2->setValidator(new QIntValidator(1, 10, this));
    editBatch3->setValidator(new QIntValidator(1, 10, this));
    editBatch4->setValidator(new QIntValidator(1, 10, this));
    editBatch5->setValidator(new QIntValidator(1, 10, this));
    editBatch1->setAlignment(Qt::AlignCenter);
    editBatch2->setAlignment(Qt::AlignCenter);
    editBatch3->setAlignment(Qt::AlignCenter);
    editBatch4->setAlignment(Qt::AlignCenter);
    editBatch5->setAlignment(Qt::AlignCenter);

    editSlotNo1->setAlignment(Qt::AlignCenter);
    editSlotNo2->setAlignment(Qt::AlignCenter);
    editSlotNo3->setAlignment(Qt::AlignCenter);
    editSlotNo4->setAlignment(Qt::AlignCenter);
    editSlotNo5->setAlignment(Qt::AlignCenter);

    //隐藏
    editSlotNo3->setVisible(false);
    fraSample3->setVisible(false);
    editBatch3->setVisible(false);
    editSlotNo4->setVisible(false);
    fraSample4->setVisible(false);
    editBatch4->setVisible(false);
    editSlotNo5->setVisible(false);
    fraSample5->setVisible(false);
    editBatch5->setVisible(false);
}

//连接信号与槽
void SelectSampleWidget::initConnect()
{
    connect(btnAddSlot, SIGNAL(clicked()), this, SLOT(slt_btnAddSlot_Clicked()));
    connect(btnRdSlot, SIGNAL(clicked()), this, SLOT(slt_btnRdSlot_Clicked()));
    connect(btnSure, SIGNAL(clicked()), this, SLOT(slt_btnSure_Clicked()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(slt_btnCancel_Clicked()));
    connect(btnReset, SIGNAL(clicked()), this, SLOT(slt_btnReset_Clicked()));
    connect(btnAppend, SIGNAL(clicked()), this, SLOT(slt_btnAppend_Clicked()));
    connect(btnBack, SIGNAL(clicked()), this, SLOT(slt_btnBack_Clicked()));

    connect(editBatch1, SIGNAL(textChanged(QString)), this, SLOT(slt_editBatch1_Change(QString)));
    connect(editBatch2, SIGNAL(textChanged(QString)), this, SLOT(slt_editBatch2_Change(QString)));
    connect(editBatch3, SIGNAL(textChanged(QString)), this, SLOT(slt_editBatch3_Change(QString)));
    connect(editBatch4, SIGNAL(textChanged(QString)), this, SLOT(slt_editBatch4_Change(QString)));
    connect(editBatch5, SIGNAL(textChanged(QString)), this, SLOT(slt_editBatch5_Change(QString)));

    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        sigMapper->setMapping(btnSample1[i], i);
        connect(btnSample1[i],SIGNAL(clicked()),sigMapper,SLOT(map()));

        sigMapper->setMapping(btnSample2[i], 10+i);
        connect(btnSample2[i],SIGNAL(clicked()),sigMapper,SLOT(map()));

        sigMapper->setMapping(btnSample3[i], 20+i);
        connect(btnSample3[i],SIGNAL(clicked()),sigMapper,SLOT(map()));

        sigMapper->setMapping(btnSample4[i], 30+i);
        connect(btnSample4[i],SIGNAL(clicked()),sigMapper,SLOT(map()));

        sigMapper->setMapping(btnSample5[i], 40+i);
        connect(btnSample5[i],SIGNAL(clicked()),sigMapper,SLOT(map()));
    }
    connect(sigMapper,SIGNAL(mapped(const int)), this,SLOT(slt_sigMapReceive(const int)));
}

//生成系统条码(样本条码)
QString SelectSampleWidget::DefaultSampleCode(int no)
{
    SampleNoStart ++;
    QString temp;
    temp = QString::number(no);
    return temp;
}

//清空所有
void SelectSampleWidget::ClearAll()
{
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        btnSample1[i]->setChecked(false);
        btnSample2[i]->setChecked(false);
        btnSample3[i]->setChecked(false);
        btnSample4[i]->setChecked(false);
        btnSample5[i]->setChecked(false);
        editSample1[i]->setPlaceholderText("");
        editSample2[i]->setPlaceholderText("");
        editSample3[i]->setPlaceholderText("");
        editSample4[i]->setPlaceholderText("");
        editSample5[i]->setPlaceholderText("");
    }
    editBatch1->clear();
    editBatch2->clear();
    editBatch3->clear();
    editBatch4->clear();
    editBatch5->clear();
}

//样本架+
void SelectSampleWidget::slt_btnAddSlot_Clicked()
{
    if( !fraSample2->isVisible())
    {
        editSlotNo2->setVisible(true);
        fraSample2->setVisible(true);
        editBatch2->setVisible(true);
        editSlotNo2->setText(QString::number(SampleSlotNo ++));
    }
    else if( !fraSample3->isVisible())
    {
        editSlotNo3->setVisible(true);
        fraSample3->setVisible(true);
        editBatch3->setVisible(true);
        editSlotNo3->setText(QString::number(SampleSlotNo ++));
    }
    else if( !fraSample4->isVisible())
    {
        editSlotNo4->setVisible(true);
        fraSample4->setVisible(true);
        editBatch4->setVisible(true);
        editSlotNo4->setText(QString::number(SampleSlotNo ++));
    }
    else if( !fraSample5->isVisible())
    {
        editSlotNo5->setVisible(true);
        fraSample5->setVisible(true);
        editBatch5->setVisible(true);
        editSlotNo5->setText(QString::number(SampleSlotNo ++));
    }
}

//样本架-
void SelectSampleWidget::slt_btnRdSlot_Clicked()
{
    if( fraSample5->isVisible())
    {
        editSlotNo5->setVisible(false);
        fraSample5->setVisible(false);
        editBatch5->setVisible(false);
        SampleSlotNo --;
    }
    else if( fraSample4->isVisible())
    {
        editSlotNo4->setVisible(false);
        fraSample4->setVisible(false);
        editBatch4->setVisible(false);
        SampleSlotNo --;
    }
    else if( fraSample3->isVisible())
    {
        editSlotNo3->setVisible(false);
        fraSample3->setVisible(false);
        editBatch3->setVisible(false);
        SampleSlotNo --;
    }
    else if( fraSample2->isVisible())
    {
        editSlotNo2->setVisible(false);
        fraSample2->setVisible(false);
        editBatch2->setVisible(false);
        SampleSlotNo --;
    }
}

//确定
void SelectSampleWidget::slt_btnSure_Clicked()
{
    SampleNoMap.clear();
    QString sample = "";
    QString code = "";

    //样本架一
    slot = editSlotNo1->text().trimmed().toInt();
    for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
    {
        if(btnSample1[i]->isChecked())
        {
            code = editSample1[i]->placeholderText().trimmed();
            SampleNoMap.insert(i + (slot-1)*10, code);
            code = "";
            sample += "(" + QString::number(slot) + "-" + QString::number(i+1) + "),";
        }
    }

    //样本架二
    if(fraSample2->isVisible())
    {
        slot = editSlotNo2->text().trimmed().toInt();
        for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
            if(btnSample2[i]->isChecked())
            {
                code = editSample2[i]->placeholderText().trimmed();
                SampleNoMap.insert(i + (slot-1)*10, code);
                code = "";
                sample += "(" + QString::number(slot) + "-" + QString::number(i+1) + "),";
            }
    }

    //样本架三
    if(fraSample3->isVisible())
    {
        slot = editSlotNo3->text().trimmed().toInt();
        for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
            if(btnSample3[i]->isChecked())
            {
                code = editSample3[i]->placeholderText().trimmed();
                SampleNoMap.insert( i+(slot-1)*10, code);
                code = "";
                sample += "(" + QString::number(slot) + "-" + QString::number(i+1) + "),";
            }
    }

    //样本架4
    if(fraSample4->isVisible())
    {
        slot = editSlotNo4->text().trimmed().toInt();
        for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
            if(btnSample4[i]->isChecked())
            {
                code = editSample4[i]->placeholderText().trimmed();
                SampleNoMap.insert(i+(slot-1)*10, code);
                code = "";
                sample += "(" + QString::number(slot) + "-" + QString::number(i+1) + "),";
            }
    }

    //样本架5
    if(fraSample5->isVisible())
    {
        slot = editSlotNo5->text().trimmed().toInt();
        for(int i = 0; i < SAMPLESLOT_COUNT; i ++)
            if(btnSample5[i]->isChecked())
            {
                code = editSample5[i]->placeholderText().trimmed();
                SampleNoMap.insert( i+(slot-1)*10, code);
                code = "";
                sample += "(" + QString::number(slot) + "-" + QString::number(i+1) + "),";
            }
    }

    //qDebug() << sample.split(",").count()-1 << sample  << endl << SampleNoMap.count() << SampleNoMap;

    emit sig_SelectedSampleList(sample, SampleNoMap);
    SampleNoStart2 = SampleNoStart;
    this->hide();
}

//取消
void SelectSampleWidget::slt_btnCancel_Clicked()
{
    SampleNoStart = SampleNoStart2;
    emit sig_SelectedSampleList("", SampleNoMap);
    ClearAll();
    this->hide();
}

//追加
void SelectSampleWidget::slt_btnAppend_Clicked()
{

}

//重置
void SelectSampleWidget::slt_btnReset_Clicked()
{
    SampleNoStart = SampleNoStart2;
    ClearAll();
}

//返回
void SelectSampleWidget::slt_btnBack_Clicked()
{
    this->hide();
}

//槽：批量申请
void SelectSampleWidget::slt_editBatch1_Change(QString text)
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
void SelectSampleWidget::slt_editBatch2_Change(QString text)
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
            editSample2[i+pt]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
            btnSample2[i+pt]->setChecked(true);
        }
        else
            btnSample2[i+pt]->setChecked(false);
    }
}

//槽：批量申请
void SelectSampleWidget::slt_editBatch3_Change(QString text)
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
            editSample3[i+pt]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
            btnSample3[i+pt]->setChecked(true);
        }
        else
            btnSample3[i+pt]->setChecked(false);
    }
}

//槽：批量申请
void SelectSampleWidget::slt_editBatch4_Change(QString text)
{
    if( text.count() == 0) return;
    int index;
    int pt = 0; //批量申请开始的位置
    for(index = 0; index < SAMPLESLOT_COUNT; index ++)
    {
        if( btnSample4[index]->isChecked() )
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
            editSample4[i+pt]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
            btnSample4[i+pt]->setChecked(true);
        }
        else
            btnSample4[i+pt]->setChecked(false);
    }
}

//槽：批量申请
void SelectSampleWidget::slt_editBatch5_Change(QString text)
{
    if( text.count() == 0) return;
    int index;
    int pt = 0; //批量申请开始的位置
    for(index = 0; index < SAMPLESLOT_COUNT; index ++)
    {
        if( btnSample5[index]->isChecked() )
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
            editSample5[i+pt]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
            btnSample5[i+pt]->setChecked(true);
        }
        else
            btnSample5[i+pt]->setChecked(false);
    }
}

//槽：选中样本自动生成样本编号
void SelectSampleWidget::slt_sigMapReceive(const int id)
{
    if(id < 10)
    {
        if(btnSample1[id]->isChecked())
            editSample1[id]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
        else
            editSample1[id]->setPlaceholderText("");
    }
    else if( id >= 10 && id < 20)
    {
        if(btnSample2[id-10]->isChecked())
            editSample2[id-10]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
        else
            editSample2[id-10]->setPlaceholderText("");
    }
    else if (id >= 20 &&id < 30)
    {
        if(btnSample3[id-20]->isChecked())
            editSample3[id-20]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
        else
            editSample3[id-20]->setPlaceholderText("");
    }
    else if (id >= 30 &&id < 40)
    {
        if(btnSample4[id-30]->isChecked())
            editSample4[id-30]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
        else
            editSample4[id-30]->setPlaceholderText("");
    }
    else if (id >= 40 &&id < 50)
    {
        if(btnSample5[id-40]->isChecked())
            editSample5[id-40]->setPlaceholderText(DefaultSampleCode(SampleNoStart));
        else
            editSample5[id-40]->setPlaceholderText("");
    }
}

