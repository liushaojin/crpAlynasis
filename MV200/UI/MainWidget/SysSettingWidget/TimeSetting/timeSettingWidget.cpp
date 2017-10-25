#include "timeSettingWidget.h"
#include "QDebug"

TimeSettingWidget * TimeSettingWidget::_instance = 0;
TimeSettingWidget::TimeSettingWidget()
    : QDialog()
{
    initForm();                           //初始化界面控件
    initStyle();                          //初始化样式
    initData();                           //初始化数据
    initConnect();                        //连接信号与槽
    initLanguage();
}

//实例对象
TimeSettingWidget * TimeSettingWidget::getInstance()
{
    if (_instance == 0)
        _instance = new TimeSettingWidget;
    return _instance;
}

//语言设置
void TimeSettingWidget::initLanguage()
{
    labTitle->setText("时间设定");
    lab_year->setText("年：");
    lab_month->setText("月：");
    lab_date->setText("日：");
    lab_hour->setText("时：");
    lab_min->setText("分：");
    lab_sec->setText("秒：");
    btnBack->setText("返 回");
    btn_reset->setText("重 置");
    btn_sure->setText("确 定");
}

//初始化界面控件
void TimeSettingWidget::initForm()
{
    labTitle = new QLabel;
    lab_year = new QLabel;     //
    lab_month = new QLabel;
    lab_date = new QLabel;
    lab_hour = new QLabel;
    lab_min = new QLabel;
    lab_sec = new QLabel;
    txt_year = new QLineEdit;
    txt_month = new QLineEdit;
    txt_date = new QLineEdit;
    txt_hour = new QLineEdit;
    txt_min = new QLineEdit;
    txt_sec = new QLineEdit;
    btnBack = new QPushButton;
    btn_reset = new QPushButton;  //重置
    btn_sure = new QPushButton;
    layout = new QGridLayout;
    layout->setMargin(10);
    layout->setSpacing(10);
    layout->addWidget(labTitle,   0,  0,  1, 45,  Qt::AlignCenter);
    layout->addWidget(lab_year,   1,  0,  1,  5,  Qt::AlignRight);
    layout->addWidget(txt_year,   1,  5,  1, 10, Qt::AlignLeft);
    layout->addWidget(lab_month,  1, 15,  1,  5,  Qt::AlignRight);
    layout->addWidget(txt_month,  1, 20,  1, 10, Qt::AlignLeft);
    layout->addWidget(lab_date,   1, 30,  1,  5,  Qt::AlignRight);
    layout->addWidget(txt_date,   1, 35,  1, 10, Qt::AlignLeft);
    layout->addWidget(lab_hour,   2,  0,  1,  5,  Qt::AlignRight);
    layout->addWidget(txt_hour,   2,  5,  1, 10, Qt::AlignLeft);
    layout->addWidget(lab_min,    2, 15,  1,  5,  Qt::AlignRight);
    layout->addWidget(txt_min,    2, 20,  1, 10, Qt::AlignLeft);
    layout->addWidget(lab_sec,    2, 30,  1,  5,  Qt::AlignRight);
    layout->addWidget(txt_sec,    2, 35,  1, 10, Qt::AlignLeft);
    layout->addWidget(btnBack,    6,  0,  5, 10,  Qt::AlignCenter);
    layout->addWidget(btn_reset,  6, 25,  5, 10,  Qt::AlignCenter);
    layout->addWidget(btn_sure,   6, 35,  5, 10, Qt::AlignCenter);

    this->setLayout(layout);

    timer = new QTimer;
    timer->start(1000);

    //调用外部程序
    process2 = new QProcess(this);
    process = new QProcess(this);
    QObject::connect(process,SIGNAL(readyRead()),this,SLOT(slt_readOutput()));  //当准备从进程里读取数据的时候触发输出数据的槽
}

//初始化样式
void TimeSettingWidget::initStyle()
{
    this->setMaximumSize(300, 300);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    //设置背景
    this->setAutoFillBackground( true );
    QString bg_str = ":/resource/resource/bg/bg1.png";
    QPalette palette = this->palette();

    palette.setBrush(QPalette::Window, QBrush(QPixmap( bg_str )));
    this->setPalette(palette);

    labTitle->setStyleSheet("QLabel{font-size:25px;}");

    QString styleEDIT =
            "QLineEdit{width:100; height:40; background:rgba(0,0,0,0%); border:1px; font-size:20px;}"
            "QLineEdit:!hover{background:rgba(0,0,0,0%);}"
            "QLineEdit:focus {width:100; height:40; border-image:none; border:1px groove yellow; font-size:20px;}";
    txt_sec->setEnabled(0);
    txt_year->setAlignment(Qt::AlignCenter);
    txt_month->setAlignment(Qt::AlignCenter);
    txt_date->setAlignment(Qt::AlignCenter);
    txt_hour->setAlignment(Qt::AlignCenter);
    txt_min->setAlignment(Qt::AlignCenter);
    txt_sec->setAlignment(Qt::AlignCenter);
    txt_year->setStyleSheet(styleEDIT);
    txt_month->setStyleSheet(styleEDIT);
    txt_date->setStyleSheet(styleEDIT);
    txt_hour->setStyleSheet(styleEDIT);
    txt_min->setStyleSheet(styleEDIT);
    txt_sec->setStyleSheet(styleEDIT);

    QString styleBTN =
            "QPushButton{width:120; height:60;  border-radius:8px; font-size:20px; background-color:rgba(50,50,50,30); }"
            "QPushButton:hover{width:120;height:60; border-radius:8px; font-size:23px; color:darkCyan; background-color:rgba(75,75,75,90);}"
            "QPushButton:pressed{width:120;height:60; border-radius:8px; font-size:21px; color:darkMagenta; background-color:rgba(80,80,80,99);}";
    btnBack->setStyleSheet(styleBTN);
    btn_reset->setStyleSheet(styleBTN);
    btn_sure->setStyleSheet(styleBTN);
}

//初始化数据
void TimeSettingWidget::initData()
{
    //显示时间
    QDateTime dt=QDateTime::currentDateTime();
    QDate date=dt.date();
    QTime time=dt.time();
    txt_year->setText( QString("%1").arg(date.year() ) );
    txt_month->setText( QString("%1").arg(date.month() ) );
    txt_date->setText( QString("%1").arg(date.day() ) );
    txt_hour->setText( QString("%1").arg(time.hour() ) );
    txt_min->setText( QString("%1").arg(time.minute() ) );
    txt_sec->setText( QString("%1").arg(time.second() ));
}

//连接信号与槽
void TimeSettingWidget::initConnect()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(slt_timeOut()));
    connect(btn_reset, SIGNAL(clicked()), this, SLOT(slt_btnReset_Clicked()));
    connect(btn_sure, SIGNAL(clicked()), this, SLOT(slt_btnSure_Clicked()));
    connect(btnBack, SIGNAL(clicked()), this, SLOT(slt_btnBack_Clicked()));
}

//槽：
void TimeSettingWidget::slt_timeOut()
{
    //显示时间
    QDateTime dt=QDateTime::currentDateTime();
    QTime time=dt.time();
    if (time.second() == 59)           //分不要更新太频繁，用户有可能会修改
        txt_min->setText( QString("%1").arg(time.minute() ) );
    txt_sec->setText( QString("%1").arg(time.second()));
}

//槽：返回
void TimeSettingWidget::slt_btnBack_Clicked()
{
    timer->stop();
    this->close();
}

//槽：重置
void TimeSettingWidget::slt_btnReset_Clicked()
{
    initData();
}

//槽：确定
void TimeSettingWidget::slt_btnSure_Clicked()
{
    QString  year, month, day, hour, min, second;

    //检查年份
    if ( txt_year->text().count() ==  4 )
        year = txt_year->text() ;
    else
    {
        qDebug( ) << "year input error";
        return;
    }
    //检查月份
    if ( txt_month->text().toInt()  < 10 &&  txt_month->text().count() ==1 )
        month = "0" + txt_month->text() ;
    else if ( txt_month->text().toInt()  < 10 &&  txt_month->text().count() ==2 )
        month = txt_month->text() ;
    else if ( txt_month->text().toInt()  > 9 && txt_month->text().toInt() < 13 )
        month = txt_month->text();
    else
    {
        qDebug( ) << "month input error";
        //sig_changeMSG( TIMESETTING_WIDGET, MSG_MONTH_ERROR );
        return;
    }

    //检查日期(没有判断是否润月)
    if ( txt_date->text().toInt()  < 10 &&  txt_date->text().count() ==1 )
        day = "0" + txt_date->text() ;
    else if ( txt_date->text().toInt()  < 10 &&  txt_date->text().count() ==2 )
        day = txt_date->text() ;
    else if ( txt_date->text().toInt()  > 9 && txt_date->text().toInt() < 32 )
        day = txt_date->text();
    else
    {
        qDebug( ) << "day input error";
        return;
    }

    //检查时间－小时
    if ( txt_hour->text().toInt()  < 10 &&  txt_hour->text().count() ==1 )
        hour = "0" + txt_hour->text() ;
    else if ( txt_hour->text().toInt()  < 10 &&  txt_hour->text().count() ==2 )
        hour = txt_hour->text() ;
    else if ( txt_hour->text().toInt()  > 9 && txt_hour->text().toInt() < 24 )
        hour = txt_hour->text();
    else
    {
        qDebug( ) << "hout input error";
        return;
    }
    //检查时间－分
    if ( txt_min->text().toInt()  < 10 &&  txt_min->text().count() ==1 )
        min = "0" + txt_min->text() ;
    else if ( txt_min->text().toInt()  < 10 &&  txt_min->text().count() ==2 )
        min = txt_min->text() ;
    else if ( txt_min->text().toInt()  > 9 && txt_min->text().toInt() < 60 )
        min = txt_min->text();
    else
    {
        qDebug( ) << "min input error";
        return;
    }

    second = txt_sec->text();

    //Tue Apr  4 20:19:09 CST 2017
    //#date 2017.4.20-09:21:00
    //#hwclock -w
    //第一句的意思是设置系统时间为2012.8.27-16:05:59
    //第二句的意思是将系统时间写入到硬件实时时钟中

    QString str;
    str = "-s " + year + "." + month + "." + day + "-" + hour + ":" + min + ":" + second;
    QString program = "date";
    QStringList arguments;
    arguments << str;
    process->start(program, arguments);
}

//用来测试时间是否设定成功
void TimeSettingWidget::slt_readOutput()
{
    QString out;
    out = process->readAll();
    qDebug() << out;
    process->close();


    QString program = "hwclock";
    QString str = "-w";
    QStringList arguments;
    arguments << str;
    process2->start(program, arguments);

    this->close();
}

//按下鼠标事件
void TimeSettingWidget::mousePressEvent(QMouseEvent * event )
{
    //窗口移动距离
    move_point = event->globalPos() - pos();
}
//移动鼠标事件
void TimeSettingWidget::mouseMoveEvent(QMouseEvent *event)
{
    //移动窗口
    QPoint move_pos = event->globalPos();
    move(move_pos - move_point);
}
