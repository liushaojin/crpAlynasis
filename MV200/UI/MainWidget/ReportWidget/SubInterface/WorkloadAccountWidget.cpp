#include "WorkloadAccountWidget.h"

WorkloadAccountWidget::WorkloadAccountWidget(QWidget *parent) : QWidget(parent)
{
    initForm();              //初始化
    initStyle();             //界面风格
    initConnect();           //连接信号与槽
    initData();              //初始化数据
}

//初始化
void WorkloadAccountWidget::initForm()
{
    //查询条件
    grbQueryTerms = new QGroupBox("查询条件");
    rdoProject = new QRadioButton("按项目查询");    //按项目查询
    cboProject = new QComboBox;
    rdoSampleType = new QRadioButton("按样本查询"); //按样本查询
    cboSampleType = new QComboBox;
    rdoDate = new QRadioButton("按日期查询");       //按日期查询
    dateQuery = new QDateEdit;
    btnQuery = new QPushButton("查 询");           //查询
    lytQueryTerms = new QGridLayout;
    lytQueryTerms->setMargin(5);
    lytQueryTerms->setSpacing(5);
    lytQueryTerms->addWidget(rdoProject, 0,  0, 2, 10, Qt::AlignCenter);
    lytQueryTerms->addWidget(cboProject, 0, 10, 2, 10);
    lytQueryTerms->addWidget(rdoSampleType, 2,  0, 2, 10, Qt::AlignCenter);
    lytQueryTerms->addWidget(cboSampleType, 2, 10, 2, 10);
    lytQueryTerms->addWidget(rdoDate,   6,  0, 2, 10, Qt::AlignCenter);
    lytQueryTerms->addWidget(dateQuery, 6, 10, 2, 10);
    lytQueryTerms->addWidget(btnQuery,  8,  0, 2, 20, Qt::AlignCenter);
    grbQueryTerms->setLayout(lytQueryTerms);

    //查询结果
    labResult0 = new QLabel("测试个数:");   //结果
    labResult = new QLabel;
    lytQueryTerms->addWidget(labResult0,  10,  0, 2, 10, Qt::AlignCenter);
    lytQueryTerms->addWidget(labResult,   10, 10, 2, 10);

    //总布局
    QWidget *wdg = new QWidget;
    QGridLayout *lyt = new QGridLayout;
    lyt->setMargin(10);
    lyt->setSpacing(10);
    lyt->addWidget(grbQueryTerms, 0, 0, 10, 50, Qt::AlignTop | Qt::AlignLeft);
    wdg->setLayout(lyt);

    layout = new QGridLayout;
    layout->setMargin(5);
    layout->setSpacing(5);
    layout->addWidget(wdg);
    this->setLayout(layout);
}

//界面风格
void WorkloadAccountWidget::initStyle()
{
    btnQuery->setObjectName("button6");
    rdoProject->setChecked(true);
}

//连接信号与槽
void WorkloadAccountWidget::initConnect()
{
    connect(btnQuery, SIGNAL(clicked(bool)), this, SLOT(slt_btnQuery_Clicked()));
}

//初始化数据
void WorkloadAccountWidget::initData()
{
    QStringList list;
    list.clear();
    list << "所有" << "全血" << "血清"  << "血浆";
    cboSampleType->addItems(list);

    list.clear();
    list.append("所有");
    QString str  = "select name from ProjectT;";
    QSqlQuery query = SQLite::getInstance()->Select(str);
    while(query.next())
        list.append( query.value(0).toString().trimmed() );
    cboProject->addItems(list);

    dateQuery->setAlignment(Qt::AlignCenter);
    dateQuery->setDateTime(QDateTime::currentDateTime());
    dateQuery->setDisplayFormat("yyyy-MM-dd");
    dateQuery->setStyleSheet("QDateEdit{width:120; height:30; font-size:15px;}");
}

//槽：查询
void WorkloadAccountWidget::slt_btnQuery_Clicked()
{
    int count = 0;
    QString str;

    if( rdoProject->isChecked() )//按项目查询
    {
        if(cboProject->currentIndex() == 0) //所有项目
        {
            str = "select COUNT(*) from TempHistoryT;";
            QSqlQuery  query = SQLite::getInstance()->Select(str);
            while(query.next())
                count = query.value(0).toInt();
        }
        else
        {
            int pid = gParameter->getProject_id(cboProject->currentText());
            str = "select COUNT(*) from TempHistoryT where projectID="+ QString::number(pid) +";";
            QSqlQuery  query = SQLite::getInstance()->Select(str);
            while(query.next())
                count = query.value(0).toInt();
        }
    }
    else if( rdoSampleType->isChecked() ) //按样本类型查询
    {
        int sampleType = cboSampleType->currentIndex();
        if(sampleType == 0)
        {
            str = "select COUNT(*) from TempHistoryT;";
            QSqlQuery  query = SQLite::getInstance()->Select(str);
            while(query.next())
                count = query.value(0).toInt();
        }
        else
        {
            str = "select COUNT(*) from TempHistoryT where sampleTypeID="+ QString::number(sampleType) +";";
            QSqlQuery  query = SQLite::getInstance()->Select(str);
            while(query.next())
                count = query.value(0).toInt();
        }
    }
    else if( rdoDate->isChecked() ) //按日期查询
    {
        QString date = dateQuery->text();

        qDebug() << date;
    }

    labResult->setText(QString::number(count));
}
