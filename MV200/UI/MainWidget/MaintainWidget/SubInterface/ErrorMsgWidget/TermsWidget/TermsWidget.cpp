#include "TermsWidget.h"

TermsWidget::TermsWidget(QDialog *parent)
    : QDialog(parent)
{
    initForm();              //初始化
    initStyle();             //界面风格
    initConnect();           //连接信号与槽
    initData();              //初始化数据
}

//初始化
void TermsWidget::initForm()
{
    labStatus = new QLabel("测试状态:");            //状态
    cboStauts = new QComboBox;
    labDateTime = new QLabel("日期时间:");          //日期时间
    dateDateTime = new QDateEdit;
    labProject = new QLabel("项目:");           //项目
    cboProject = new QComboBox;
    labSampleType = new QLabel("样本类型:");        //样本类型
    cboSampleType = new QComboBox;
    labEmergency = new QLabel("是否急诊:");         //是否急诊
    rdoEmergency_n = new QRadioButton; //
    rdoEmergency_y = new QRadioButton; //
    btnSure = new QPushButton("确定");
    btnCancel = new QPushButton("取消");
    layout = new QGridLayout;
    layout->setMargin(10);
    layout->setSpacing(5);
//    layout->addWidget(labStatus,      0,  0, 2, 5, Qt::AlignRight);
//    layout->addWidget(cboStauts,      0,  5, 2, 5, Qt::AlignLeft);
//    layout->addWidget(labDateTime,    0, 10, 2, 5, Qt::AlignRight);
//    layout->addWidget(dateDateTime,   0, 15, 2, 5);

//    layout->addWidget(labProject,     2,  0, 2, 5, Qt::AlignRight);
//    layout->addWidget(cboProject,     2,  5, 2, 5, Qt::AlignLeft);
//    layout->addWidget(labSampleType,  2, 10, 2, 5, Qt::AlignRight);
//    layout->addWidget(cboSampleType,  2, 15, 2, 5, Qt::AlignLeft);

//    layout->addWidget(btnSure,       10,  0, 2, 10, Qt::AlignCenter);
//    layout->addWidget(btnCancel,     10, 10, 2, 10, Qt::AlignCenter);

    layout->addWidget(labDateTime,    0, 0, 2, 5, Qt::AlignRight);
    layout->addWidget(dateDateTime,   0, 5, 2, 5);
    layout->addWidget(btnSure,       10, 0, 2, 5, Qt::AlignCenter);
    layout->addWidget(btnCancel,     10, 5, 2, 5, Qt::AlignCenter);
    this->setLayout(layout);
}

//界面风格
void TermsWidget::initStyle()
{
    //图标路径
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

    //QSS
    QFile qss( ":/QSS/QSS/mvStyle.qss" );
    qss.open(QFile::ReadOnly);
    setStyleSheet( qss.readAll() );
    qss.close();

    this->setFixedSize(400, 180);

    btnCancel->setObjectName("button6");
    btnSure->setObjectName("button6");

    dateDateTime->setAlignment(Qt::AlignCenter);
    dateDateTime->setDateTime(QDateTime::currentDateTime());
    dateDateTime->setDisplayFormat("yyyy-MM-dd");
    dateDateTime->setStyleSheet("QDateEdit{width:120; height:30; font-size:15px;}");

    this->setWindowFlags(Qt::WindowStaysOnTopHint);       //去掉标题栏


    cboSampleType->setEnabled(false);
}

//连接信号与槽
void TermsWidget::initConnect()
{
    connect(btnSure, SIGNAL(clicked(bool)), this, SLOT(slt_btnSure_Clicked()));
    connect(btnCancel, SIGNAL(clicked(bool)), this, SLOT(slt_btnCancel_Clicked()));
}

//初始化数据
void TermsWidget::initData()
{
    QStringList list;

    list.clear();
    list << "所有" << "完成" << "测试中";
    cboStauts->addItems(list);

    list.clear();
    list << "所有" << "血清" << "全血" << "血浆";
    cboSampleType->addItems(list);

    list.clear();
    list.append("所有");
    QString str  = "select name from ProjectT;";
    QSqlQuery query = SQLite::getInstance()->Select(str);
    while(query.next())
        list.append( query.value(0).toString().trimmed() );
    cboProject->addItems(list);
}

//槽:确定
void TermsWidget::slt_btnSure_Clicked()
{
    QString terms = " where ";

    //状态
    int statusIndex = cboStauts->currentIndex();
    if( statusIndex == 0 )
        terms += " ";
    else if( statusIndex == 1 )
        terms += " TaskT.stateID = 11 and ";
    else if( statusIndex == 2 )
       terms += " TaskT.stateID = 4 and ";

    //时间
    QString date = dateDateTime->text();
    terms += " ErrorT.dateTime like '%" + date + "%' ";

    //项目名称
    int ProjectIndex = cboProject->currentIndex();
    if( ProjectIndex == 0 )
        terms += " ";
    else
        terms += " and ProjectT.name = '" + cboProject->currentText() +"' ";

    //qDebug() <<"Terms:" << terms;
    emit sig_QueryTerms_Change(terms);
    this->hide();
}

//槽:取消
void TermsWidget::slt_btnCancel_Clicked()
{
    this->hide();
}

