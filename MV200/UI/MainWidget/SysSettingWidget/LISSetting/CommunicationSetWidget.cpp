#include "CommunicationSetWidget.h"
#include "Object/SQLite/sqlite.h"
#include "Thread/Scheduler.h"
#include "UI/Widget/MessageBox/messageBox.h"

//实例对象
CommunicationSetWidget *CommunicationSetWidget::_instance = 0;
CommunicationSetWidget *CommunicationSetWidget::getInstance()
{
    if (_instance == 0)
        _instance = new CommunicationSetWidget;
    return _instance;
}
CommunicationSetWidget::CommunicationSetWidget(QWidget *parent)
    : QWidget(parent)
{
    initForm();                           //初始化界面控件
    initStyle();                          //初始化样式
    initData();                           //初始化数据
    initConnect();                        //连接信号与槽
}

//初始化界面控件
void CommunicationSetWidget::initForm()
{
    //MV200服务器地址
    grbServer = new QGroupBox("服务器设置");
    labServer_IP = new QLabel("服务器 IP地址:");            //Lis ip地址
    Server_IPAdress = new IPAddressEdit;
    labServerPort = new QLabel("服务器 端口:");             //Lis 端口
    editServerPort = new QLineEdit;
    btnResetIP = new QPushButton("更新IP");           //更新中位机IP
    btnSaveIP = new QPushButton("保存");            //保存中位机IP
    lytServer = new QGridLayout;
    lytServer->setSpacing(10);
    lytServer->setMargin(10);
    lytServer->addWidget(labServer_IP,       0,  0,  2,  10, Qt::AlignLeft);
    lytServer->addWidget(Server_IPAdress,    0, 10,  2,  20);
    lytServer->addWidget(labServerPort,      2,  0,  2,  10, Qt::AlignLeft);
    lytServer->addWidget(editServerPort,     2, 10,  2,  20);
    lytServer->addWidget(btnResetIP,         4,  0,  2,  15, Qt::AlignCenter);
    lytServer->addWidget(btnSaveIP,          4, 15,  2,  15, Qt::AlignCenter);
    grbServer->setLayout(lytServer);

    grbLis = new QGroupBox("LIS设置");
    labAgreement = new QLabel("通讯协议:");           //通讯协议
    cboAgreement = new QComboBox;
    labLIS_IP = new QLabel("LIS IP地址:");            //Lis ip地址
    LIS_IPAdress = new IPAddressEdit;
    labLISPort = new QLabel("LIS 端口:");             //Lis 端口
    editLISPort = new QLineEdit;
    btnSave = new QPushButton("保存");           //保存
    btnCancel = new QPushButton("取消");         //取消
    lytLis = new QGridLayout;
    lytLis->setSpacing(10);
    lytLis->setMargin(5);
    lytLis->addWidget(labAgreement,    0,  0,  2,  10, Qt::AlignLeft);
    lytLis->addWidget(cboAgreement,    0, 10,  2,  20);
    lytLis->addWidget(labLIS_IP,       2,  0,  2,  10, Qt::AlignLeft);
    lytLis->addWidget(LIS_IPAdress,    2, 10,  2,  20);
    lytLis->addWidget(labLISPort,      4,  0,  2,  10, Qt::AlignLeft);
    lytLis->addWidget(editLISPort,     4, 10,  2,  20);
    lytLis->addWidget(btnSave,         6,  0,  2,  15, Qt::AlignCenter);
    lytLis->addWidget(btnCancel,       6, 15,  2,  15, Qt::AlignCenter);
    grbLis->setLayout(lytLis);

    QGridLayout *layout = new QGridLayout;
    layout->setMargin( 10 );
    layout->setSpacing( 20 );
    layout->addWidget(grbServer,   0, 0, 20, 50);
    layout->addWidget(grbLis,     20, 0, 25, 50);
    this->setLayout(layout);
    this->setFixedSize(480, 420);
}
//初始化样式
void CommunicationSetWidget::initStyle()
{
    //置顶&隐藏最大/最小化
    this->setWindowFlags(this->windowFlags()& ~Qt::WindowMinMaxButtonsHint | Qt::WindowStaysOnTopHint);

    //设置背景
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap( ":/resource/resource/bg/mainBg.png" )));  //
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    //图标路径
    QIcon icon("://resource/resource/MV200.png");
    this->setWindowIcon(icon);

    //QSS
    QFile qss( ":/QSS/QSS/mvStyle.qss" );
    qss.open(QFile::ReadOnly);
    QString ss = qss.readAll();
    setStyleSheet( ss );
    qss.close();

    btnSave->setObjectName("button6");
    btnCancel->setObjectName("button6");
    btnResetIP->setObjectName("button6");
    btnSaveIP->setObjectName("button6");
}
//初始化数据
void CommunicationSetWidget::initData()
{
    cboAgreement->addItem("HL7");
    QueryLISParam();         //查询LIS参数表
}
//连接信号与槽
void CommunicationSetWidget::initConnect()
{
    connect(btnCancel, SIGNAL(clicked(bool)), this, SLOT(slt_btnCancel_Clicked()));
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(slt_btnSave_Clicked()));

    connect(btnResetIP, SIGNAL(clicked(bool)), this, SLOT(slt_btnResetIP_Clicked()));
    connect(btnSaveIP, SIGNAL(clicked(bool)), this, SLOT(slt_btnSaveIP_Clicked()));
    connect(gScheduler, SIGNAL(sig_ResetIP(int)), this, SLOT(slt_ResetServerIP(int)));
}
//查询LIS参数表
void CommunicationSetWidget::QueryLISParam()
{
    QString str = "SELECT * from LISParamT;";
    QSqlQuery query = SQLite::getInstance()->Select(str);
    while (query.next())
    {
        Server_IPAdress->setIPAddress(query.value(2).toString());
        editServerPort->setText(query.value(3).toString());

        cboAgreement->setCurrentIndex(query.value(5).toInt()-1);
        LIS_IPAdress->setIPAddress(query.value(6).toString());
        editLISPort->setText(query.value(7).toString());
    }
}

//槽：保存
void CommunicationSetWidget::slt_btnSave_Clicked()
{
    if( gScheduler->get_isRunning() )//正在测试中，不允许修改
    {
        gMessageBox->setInfo(QString("提示"), QString("有测试正在进行中，不能更新数据！\n请等待所有测试完成后再进行!!"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return;
    }

    if( !LIS_IPAdress->validCheck() )
    {
        gMessageBox->setInfo(QString("IP"), QString("LIS IP输入不全法！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
        return;
    }



    QString str =
            "update LISParamT set isAutoStart = 0, "
            //" IP='"+ Server_IPAdress->getIPAddress() +"', "
            //+" SubnetMask='"+ editServerPort->text().trimmed() +"', " //作端口保存
            " LIS_IP='"+ LIS_IPAdress->getIPAddress() +"', "
            +" LISPort='"+ editLISPort->text().trimmed() +"' "
            +" where id =1;";
    SQLite::getInstance()->Insert(str);

    //gParameter->setTCPServerIP( Server_IPAdress->getIPAddress() );
    ///gParameter->setTCPServerPort( editServerPort->text().trimmed().toInt() );
    gParameter->set_LIS_IP( LIS_IPAdress->getIPAddress() );
    gParameter->set_LIS_Port( editLISPort->text().trimmed().toInt() );

    gMessageBox->setInfo(QString("网络设置"), QString("保存成功！"),
                         QPixmap("://resource/resource/MessageBox/information.png"), true, true);
    gMessageBox->setButtonText(Cancel, "确定");
    gMessageBox->show();
}
//槽：取消
void CommunicationSetWidget::slt_btnCancel_Clicked()
{
    QueryLISParam();
    this->close();
}

//槽：保存中位机IP
void CommunicationSetWidget::slt_btnSaveIP_Clicked()
{
    if( !Server_IPAdress->validCheck() )
    {
        gMessageBox->setInfo(QString("IP"), QString("服务器IP输入不合法！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
        return;
    }

    QString ip = Server_IPAdress->getIPAddress();
    QString str =
            "update LISParamT set isAutoStart = 0, "
            " IP='"+ ip +"', "
            +" SubnetMask='"+ editServerPort->text().trimmed() +"' " //作端口保存
            +" where id =1;";
    SQLite::getInstance()->Insert(str);
    gParameter->setTCPServerIP( Server_IPAdress->getIPAddress() );
    gParameter->setTCPServerPort( editServerPort->text().trimmed().toInt() );

    gMessageBox->setInfo(QString("提示"), QString("服务器IP保存成功！"),
                         QPixmap("://resource/resource/MessageBox/information.png"), true, true);
    gMessageBox->setButtonText(Cancel, "返回");
    gMessageBox->show();
}
//槽：更新中位机IP
void CommunicationSetWidget::slt_btnResetIP_Clicked()
{
    //正在测试中，不允许修改
    if( gScheduler->get_isRunning() )
    {
        gMessageBox->setInfo(QString("提示"), QString("有测试正在进行中，不能更新中位机IP卡！\n请等待所有测试完成后再进行!!"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
            return;
    }

    //服务器地址输入不合法
    if( !Server_IPAdress->validCheck() )
    {
        gMessageBox->setInfo(QString("IP"), QString("服务器IP输入不合法！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
        return;
    }

    QString ip = Server_IPAdress->getIPAddress();
    gScheduler->ResetServerIP(ip);

}
//槽：修改中位机IP是否成功（0：失败， 1：成功）
void CommunicationSetWidget::slt_ResetServerIP(int state)
{
    if(state == 1)
    {
        QString ip = Server_IPAdress->getIPAddress();
        QString str =
                "update LISParamT set isAutoStart = 0, "
                " IP='"+ ip +"', "
                +" SubnetMask='"+ editServerPort->text().trimmed() +"' " //作端口保存
                +" where id =1;";
        SQLite::getInstance()->Insert(str);
        gParameter->setTCPServerIP( Server_IPAdress->getIPAddress() );
        gParameter->setTCPServerPort( editServerPort->text().trimmed().toInt() );

        gMessageBox->setInfo(QString("提示"), QString("服务器IP修改成功！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
    }
    else
    {
        gMessageBox->setInfo(QString("提示"), QString("服务器IP修改不成功！"),
                             QPixmap("://resource/resource/MessageBox/information.png"), true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        gMessageBox->show();
    }
}
