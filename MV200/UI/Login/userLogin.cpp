#include "userLogin.h"

UserLogin::UserLogin(QDialog *parent) : QDialog(parent)
{
    initForm();          //初始化
    initStyle();         //界面风格
    initConnect();       //连接信号与槽
    initData();          //初始化数据
}

//初始化
void UserLogin::initForm()
{
    fraImage = new QFrame;
    labTitle = new QLabel("用户登录");
    labUser = new QLabel("用 户:");
    editUser = new QLineEdit;
    labPwd = new QLabel("密 码:");
    editPwd = new QLineEdit;
    chkSelfCheck = new QCheckBox("是否开机自检"); //是否要开机自检
    btnLogin = new QPushButton("登 录");
    btnCancel = new QPushButton("取 消");

    layout = new QGridLayout;
    layout->setMargin(10);
    layout->setSpacing(10);
    layout->addWidget(fraImage,     1,  0, 3,  5, Qt::AlignRight);
    layout->addWidget(labTitle,     1,  5, 3, 15, Qt::AlignLeft);
    layout->addWidget(labUser,      4,  0, 1,  5, Qt::AlignRight);
    layout->addWidget(editUser,     4,  5, 1, 15, Qt::AlignLeft);
    layout->addWidget(labPwd,       6,  0, 1,  5, Qt::AlignRight);
    layout->addWidget(editPwd,      6,  5, 1, 15, Qt::AlignLeft);
    layout->addWidget(chkSelfCheck,10,  5, 1, 15, Qt::AlignLeft);
    layout->addWidget(btnCancel,   12,  0, 1, 10, Qt::AlignCenter);
    layout->addWidget(btnLogin,    12, 10, 1, 10, Qt::AlignCenter);
    this->setLayout(layout);
}

//界面风格
void UserLogin::initStyle()
{
    this->setFixedSize(360, 300);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);       //去掉标题栏

    //图标设置
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

    QFile  qss(":/QSS/QSS/LoginWidget.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    this->setObjectName("loginDlg");
    fraImage->setFixedSize(64, 64);
    fraImage->setObjectName("userLoginFrame");
    labTitle->setObjectName("lab30");

    chkSelfCheck->setChecked(true);
    editPwd->setEchoMode(QLineEdit::Password);
}

//连接信号与槽
void UserLogin::initConnect()
{
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(slt_btnCancel_Clicked()));
    connect(btnLogin, SIGNAL(clicked()), this, SLOT(slt_btnLogin_Clicked()));
}

//初始化数据
void UserLogin::initData()
{
    editUser->setPlaceholderText("UserName");
    editPwd->setPlaceholderText("PassWord");

    //读取MVLogin.ini文件获取最后一次登录的的用户名及密码
    gSetting = new QSettings("MV200Login.ini", QSettings::IniFormat);
    gSetting->beginGroup("Login");
    editUser->setText(gSetting->value("name").toString());
    editPwd->setText(AppHelper::getXorEncryptDecrypt(gSetting->value("pwd").toString(), KEY));
    gSetting->endGroup();
}

//槽：登陆
void UserLogin::slt_btnLogin_Clicked()
{
    //（0：用户不存在，1：允许登录，2：密码错误）
    QString user = editUser->text();
    QString psd = editPwd->text();
    int login = SQLite::getInstance()->Login(user, psd);
    if( login == 1 )
    {
        int selfCheck = chkSelfCheck->isChecked(); //是否开机自检
        gParameter->set_isSelfCheck(selfCheck);
        //保存最后登录的用户信息
        gSetting->beginGroup("Login");
        gSetting->setValue("name", editUser->text().trimmed());

        //Build类型（发布版、研发版）
        if( !gParameter->get_isRelease()) //0:研发版
            gSetting->setValue("pwd", AppHelper::getXorEncryptDecrypt(editPwd->text().trimmed(), KEY));
        else
            gSetting->setValue("pwd", AppHelper::getXorEncryptDecrypt("", KEY));

        gSetting->endGroup();
        this->accept();
        delete gSetting;
    }
    else if( login == 2)
    {
        gMessageBox->setInfo(QString("提示"), QString("密码错误！"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
        {
            editPwd->clear();
            return;
        }
    }
    else
    {
        gMessageBox->setInfo(QString("提示"), QString("用户不存在！"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
        {
            editUser->clear();
            editPwd->clear();
            return;
        }
    }
}

//槽：取消
void UserLogin::slt_btnCancel_Clicked()
{
    this->reject();
}

//按下鼠标事件
void UserLogin::mousePressEvent( QMouseEvent * event )
{
    //窗口移动距离
    move_point = event->globalPos() - pos();
}
//移动鼠标事件
void UserLogin::mouseMoveEvent(QMouseEvent *event)
{
    //移动窗口
    QPoint move_pos = event->globalPos();
    move(move_pos - move_point);
}
//键盘事件
void UserLogin::keyPressEvent(QKeyEvent  *event)
{
    //回车键 & 确定（数字键）
    if(event->key()== Qt::Key_Return || event->key()== Qt::Key_Enter)
    {
        slt_btnLogin_Clicked();
    }
}
