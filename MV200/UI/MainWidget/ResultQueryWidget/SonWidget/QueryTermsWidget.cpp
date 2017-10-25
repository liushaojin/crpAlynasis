#include "QueryTermsWidget.h"

QueryTermsWidget::QueryTermsWidget(QDialog *parent)
    : QDialog(parent)
{
    initForm();              //初始化
    initStyle();             //界面风格
    initConnect();           //连接信号与槽
    initData();              //初始化数据
}

//初始化
void QueryTermsWidget::initForm()
{
    labStatus = new QLabel("测试状态:");            //状态
    cboStauts = new QComboBox;

    chk_isDateTime = new QCheckBox("申请日期");
    //labDateTime = new QLabel("申请日期:");          //日期时间
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
    layout->addWidget(labStatus,      0,  0, 2, 5, Qt::AlignRight);
    layout->addWidget(cboStauts,      0,  5, 2, 5, Qt::AlignLeft);
    layout->addWidget(chk_isDateTime, 0, 10, 2, 5, Qt::AlignRight);
    layout->addWidget(dateDateTime,   0, 15, 2, 5);

    layout->addWidget(labProject,     2,  0, 2, 5, Qt::AlignRight);
    layout->addWidget(cboProject,     2,  5, 2, 5, Qt::AlignLeft);
    layout->addWidget(labSampleType,  2, 10, 2, 5, Qt::AlignRight);
    layout->addWidget(cboSampleType,  2, 15, 2, 5, Qt::AlignLeft);

    layout->addWidget(btnSure,       10,  0, 2, 10, Qt::AlignCenter);
    layout->addWidget(btnCancel,     10, 10, 2, 10, Qt::AlignCenter);

    this->setLayout(layout);
}

//界面风格
void QueryTermsWidget::initStyle()
{
    //QSS
    QFile qss( ":/QSS/QSS/mv100.qss" );
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

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);       //去掉标题栏


    cboSampleType->setEnabled(false);
}

//连接信号与槽
void QueryTermsWidget::initConnect()
{
    connect(btnSure, SIGNAL(clicked(bool)), this, SLOT(slt_btnSure_Clicked()));
    connect(btnCancel, SIGNAL(clicked(bool)), this, SLOT(slt_btnCancel_Clicked()));
}

//初始化数据
void QueryTermsWidget::initData()
{
    QStringList list;

    list.clear();
    list << "所有" << "完成" << "测试中" << "未完成";
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
void QueryTermsWidget::slt_btnSure_Clicked()
{
    QString terms;

    //状态
    int statusIndex = cboStauts->currentIndex();
    if( statusIndex == 0 ) //所有
        terms += " ";
    else if( statusIndex == 1 ) //完成
        terms += " TaskT.stateID = 11 ";
    else if( statusIndex == 2 ) //测试中
        terms += " TaskT.stateID = 4 ";
    else if( statusIndex == 3 ) //未完成
        terms += " TaskT.stateID = 15 ";

    //时间
    if(chk_isDateTime->isChecked())
    {
        if(statusIndex > 0)
            terms += " and ";
        QString date = dateDateTime->text();
        terms += " TaskT.dateTime like '%" + date + "%' and ";
    }

    //项目名称
    int ProjectIndex = cboProject->currentIndex();
    if( ProjectIndex == 0 )
        terms += " ";
    else
        terms += " ProjectT.name = '" + cboProject->currentText() +"' ";

    if(terms.count() > 10)
        terms = " where " + terms;
    qDebug() <<"Terms:" << terms;
    emit sig_QueryTerms_Change(terms);
    this->hide();
}

//槽:取消
void QueryTermsWidget::slt_btnCancel_Clicked()
{
    this->hide();
}

