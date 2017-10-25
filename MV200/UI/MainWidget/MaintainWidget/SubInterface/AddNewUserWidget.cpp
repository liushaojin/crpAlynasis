#include "AddNewUserWidget.h"
#include <qdebug.h>

//实例对象
AddNewUserWidget *AddNewUserWidget::_instance = 0;
AddNewUserWidget * AddNewUserWidget::getInstance()
{
    if (_instance == 0)
        _instance = new AddNewUserWidget();
    return _instance;
}

AddNewUserWidget::AddNewUserWidget(QWidget *parent) : QWidget(parent)
{
    initForm();              //初始化
    initStyle();             //界面风格
    initConnect();           //连接信号与槽
}

//初始化
void AddNewUserWidget::initForm()
{
    labName = new QLabel("用户名：");
    labPassword = new QLabel("密码：");
    editName = new QLineEdit;
    editPassword = new QLineEdit;
    btnCancel = new QPushButton("取 消");
    btnSure = new QPushButton("确 定");
    layout = new QGridLayout;
    layout->setMargin(10);
    layout->setSpacing(10);
    layout->addWidget(labName,      0,  0, 2, 10, Qt::AlignRight);
    layout->addWidget(editName,     0, 10, 2, 10, Qt::AlignLeft);
    layout->addWidget(labPassword,  2,  0, 2, 10, Qt::AlignRight);
    layout->addWidget(editPassword, 2, 10, 2, 10, Qt::AlignLeft);
    layout->addWidget(btnCancel,    6,  0, 2, 10, Qt::AlignCenter);
    layout->addWidget(btnSure,      6, 10, 2, 10, Qt::AlignCenter);
    this->setLayout(layout);
}

//界面风格
void AddNewUserWidget::initStyle()
{
    //去掉标题栏
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    //图标路径
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("icon/WinIcon.png")), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

    QFile  qss(":/QSS/QSS/LoginWidget.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();
}

//连接信号与槽
void AddNewUserWidget::initConnect()
{
    connect(btnCancel, SIGNAL(clicked(bool)), this, SLOT(slt_btnCancel_Clicked()));
    connect(btnSure, SIGNAL(clicked(bool)), this, SLOT(slt_btnSure_Clicked()));
}

//槽:
void AddNewUserWidget::slt_btnCancel_Clicked()
{
    editName->clear();
    editPassword->clear();
    this->close();
}

//槽:
void AddNewUserWidget::slt_btnSure_Clicked()
{
    QString name = editName->text().trimmed();
    QString pwd = editPassword->text().trimmed();

    if( name == "") //未输入用户名
    {
        gMessageBox->setInfo(QString("提示"), QString("用户名不能为空！"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
        {
            editName->setFocus();
            return;
        }
    }
    else if( pwd == "" ) //密码为空
    {
        gMessageBox->setInfo(QString("提示"), QString("密码不能为空！"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
        {
            editPassword->setFocus();
            return;
        }
    }

    QString str = "insert into UserT(name, pwd) values('"+ name +"', '"+ pwd +"');";
    if(SQLite::getInstance()->Insert(str))
    {
        emit sig_AddNewUser();
        //        gMessageBox->setInfo(QString("提示"), QString("新增用户成功！"),
        //                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        //        gMessageBox->setButtonText(Cancel, "确定");
        //        if(gMessageBox->exec() == MVMessageBox::Rejected)
        //        {
        editName->clear();
        editPassword->clear();
        this->close();
        return;
        //        }
    }
    else
    {
        gMessageBox->setInfo(QString("提示"), QString("新增用户不成功！\n可能保存到数据库出错！"),
                             QPixmap("://resource/resource/MessageBox/information.png"),  true, true);
        gMessageBox->setButtonText(Cancel, "返回");
        if(gMessageBox->exec() == MVMessageBox::Rejected)
        {
            this->close();
            return;
        }
    }
}

//按下鼠标事件
void AddNewUserWidget::mousePressEvent( QMouseEvent * event )
{
    //窗口移动距离
    move_point = event->globalPos() - pos();
}
//移动鼠标事件
void AddNewUserWidget::mouseMoveEvent(QMouseEvent *event)
{
    //移动窗口
    QPoint move_pos = event->globalPos();
    move(move_pos - move_point);
}
//键盘事件
void AddNewUserWidget::keyPressEvent(QKeyEvent  *event)
{
    //回车键 & 确定（数字键）
    if(event->key()== Qt::Key_Return || event->key()== Qt::Key_Enter)
    {
        slt_btnSure_Clicked();
    }
}


