#include "messageBox.h"

MVMessageBox *gMessageBox;
MVMessageBox::MVMessageBox(QWidget *parent) :
    QDialog(parent)
{
    initForm();                               //初始化窗口
    initConnection();                         //初始化信号与槽
    initStyleSheet();                         //初始化样式表
    translateLanguage();                      //设置语言
}

//初始化窗口
void MVMessageBox::initForm()
{
    this->resize(500, 250);
    //获取主界面的宽度
    int width = this->width();
    int height = this->height();
    //初始化为未按下鼠标左键
    isMousePressed = false;
    //设置标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowStaysOnTopHint);

    //设置信息弹窗各控件的大小
    int titleFont = 18;  //标题字体大小
    int AskFont = 16;    //提示信息字体大小
    int RemeberFont = 12;//记住选择字体大小
    int closeW = 40;     //关闭按钮的宽
    int closeH = 40;     //关闭按钮的高
    int iconW = 100;     //提示图片的宽
    int iconH = 100;     //提示图片的高
    int btnW = 100;      //确定＆取消的宽
    int btnH = 50;       //确定＆取消的高


    btnClose = new QPushButton(this);
    btnClose->setGeometry(width-closeW-1, 1, closeW, closeH);

    //设置标题
    labTitle = new QLabel(this);
    QFont font("", titleFont);                  //设置标题字体大小
    labTitle->setFont(font);
    labTitle->setGeometry(10, 0, width-closeW-11, closeH);

    //设置提示图片
    labIcon = new QLabel(this);
    labIcon->setGeometry(30, 60, iconW, iconH);
    labIcon->setScaledContents(true);

    //设置提示信息
    labAsk = new QLabel(this);
    labAsk->setGeometry(iconW+labIcon->pos().x()+5, labIcon->pos().y()+10, width-iconW-labIcon->pos().x(), 100);
    QFont fontAsk("", AskFont);           //设置标题字体大小
    labAsk->setFont(fontAsk);
    labAsk->setWordWrap(true);             //让QLabel能够自动判断并换行显示
    labAsk->setAlignment(Qt::AlignHCenter);    //显示置顶

    //记住选择
    chkRemeber = new QCheckBox(this);
    QFont fontRemeber("", RemeberFont);           //设置标题字体大小
    chkRemeber->setFont(fontRemeber);
    chkRemeber->setGeometry(10, height - 40, width-btnW*2-50, 35);
    chkRemeber->setHidden(true);

    //确定＆取消
    btnCancel = new QPushButton(this);
    btnCancel->resize(btnW, btnH);
    btnCancel->move(width - btnCancel->width() - 10, height - btnH - 10);
    btnSure = new QPushButton(this);
    btnSure->resize(btnW, btnH);
    btnSure->move(width - btnSure->width() - btnCancel->width() - 20, height - btnH - 10);
}

//初始化信号与槽
void MVMessageBox::initConnection()
{
    //信号和槽
    QObject::connect(btnSure, SIGNAL(clicked()), this, SLOT(slt_btnSure_Clicked()));
    QObject::connect(btnClose, SIGNAL(clicked()), this, SLOT(slt_btnCancel_Clicked()));
    QObject::connect(btnCancel, SIGNAL(clicked()), this, SLOT(slt_btnCancel_Clicked()));
}

//初始化样式表
void MVMessageBox::initStyleSheet()
{
    QString styleBG = "QDialog{ background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #113845, stop: 0.2 #1878B1, stop: 1.0 #15A8FF); }";
    this->setStyleSheet(styleBG);

    //样式表可根据实际应用做适当更改
    QString styleBtnClose =
            "QPushButton{width:40;height:40; border-radius:8px;  background-image:url(:/resource/resource/MessageBox/close.png);background-repeat:no-repeat; background-position:center;}"
            "QPushButton:hover{width:40;height:40; border-radius:8px;  background-image:url(:/resource/resource/MessageBox/close_hover.png);background-repeat:no-repeat; background-position:center;}"
            "QPushButton:pressed{width:40;height:40; border-radius:8px; background-image:url(:/resource/resource/MessageBox/close_pressed.png);background-repeat:no-repeat; background-position:center;}";
    btnClose->setStyleSheet(styleBtnClose);

    QString styleBtn =
            "QPushButton{width:100;height:50; font-size:18px; border-radius:8px; background-image:url(:/resource/resource/MessageBox/btn.png);background-repeat:no-repeat; background-position:center;}"
            "QPushButton:hover{width:100;height:50; font-size:18px; border-radius:8px; background-image:url(:/resource/resource/MessageBox/btn_pressed.png);background-repeat:no-repeat; background-position:center;}";
    btnCancel->setStyleSheet(styleBtn);
    btnSure->setStyleSheet(styleBtn);

    QString strChkStyle =
            "QCheckBox {spacing:5px; background:transparent;/*控件背景透明*/}"
            "QCheckBox::indicator{width: 13px;height: 13px;}"

            "QCheckBox::indicator:unchecked{image: url(:/resource/resource/MessageBox/checkbox_unchecked.png);}"
            "QCheckBox::indicator:unchecked:hover{image: url(:/resource/resource/MessageBox/checkbox_unchecked_hover.png);}"
            "QCheckBox::indicator:unchecked:pressed{image: url(:/resource/resource/MessageBox/checkbox_unchecked_pressed.png);}"

            "QCheckBox::indicator:checked{image: url(:/resource/resource/MessageBox/checkbox_checked.png);}"
            "QCheckBox::indicator:checked:pressed{image: url(:/resource/resource/MessageBox/checkbox_checked_pressed.png);}"
            "QCheckBox::indicator:checked:hover{image: url(:/resource/resource/MessageBox/checkbox_checked_hover.png);}";
    chkRemeber->setStyleSheet(strChkStyle);

    labTitle->setStyleSheet("QLabel{background: transparent; font-size:22px; color:black; }");
    labAsk->setStyleSheet("QLabel{background: transparent; font-size:20px; color:black; }");
}

//设置语言
void MVMessageBox::translateLanguage()
{
    btnClose->setToolTip(tr("Close this Windows!"));  //未点击时提示(即鼠标移到按钮的区域时显示提示信息)
    chkRemeber->setText(tr("Remember"));
    str_btnSure_Text = tr("Sure");
    str_btnCancel_Text = tr("Return");
}

void MVMessageBox::setInfo(QString titleInfo, QString msgInfo, QPixmap pixmap, bool isChkHidden, bool isBtnSureHiddent)
{
    //设置标题
    labTitle->setText( titleInfo );
    //设置提示信息
    labAsk->setText(msgInfo);
    labIcon->setPixmap(pixmap);
    //是否隐藏复选框
    chkRemeber->setChecked(false);
    chkRemeber->setHidden(isChkHidden);
    //是否隐藏确定按钮
    btnSure->setHidden(isBtnSureHiddent);
    //设置按钮文本
    btnSure->setText(str_btnSure_Text);
    btnCancel->setText(str_btnCancel_Text);
    //设置默认按钮为取消按钮
    btnCancel->setFocus();
}

//设置按钮文本
void MVMessageBox::setButtonText(ButtonType type, QString text)
{
    if(type == Sure)
    {
        btnSure->setText(text);
    }
    else if(type == Cancel)
    {
        btnCancel->setText(text);
    }
}

//重绘事件
void MVMessageBox::paintEvent(QPaintEvent *)
{
    //    //背景可根据实际应用做适当更改
    //    QPainter painter(this);
    //    painter.drawPixmap(rect(), QPixmap(":/icon/bg.png"));  //设置背景
    //    QBitmap bitmap(this->size());
    //    QPainter painter2(&bitmap);
    //    painter2.fillRect(bitmap.rect(), Qt::white);
    //    painter2.setBrush(QColor(0, 0, 0));
    //    painter2.drawRoundedRect(rect(), 4, 4);
    //    setMask(bitmap);
}

//按下鼠标事件
void MVMessageBox::mousePressEvent( QMouseEvent * event )
{
    //只能是鼠标左键移动和改变大小
    if(event->button() == Qt::LeftButton)
        isMousePressed = true;
    //窗口移动距离
    move_point = event->globalPos() - pos();
}

//释放鼠标事件
void MVMessageBox::mouseReleaseEvent( QMouseEvent *)
{
    isMousePressed = false;
}

//移动鼠标事件
void MVMessageBox::mouseMoveEvent(QMouseEvent *event)
{
    //移动窗口
    if(isMousePressed)
    {
        QPoint move_pos = event->globalPos();
        move(move_pos - move_point);
    }
}

//槽：确定
void MVMessageBox::slt_btnSure_Clicked()
{
    bool isHidden = chkRemeber->isHidden();
    if(!isHidden)
    {
        bool isChkRember_Checked = chkRemeber->isChecked();
        emit sig_SureClicked(isChkRember_Checked, true);
    }

    this->accept();
}

//槽：取消
void MVMessageBox::slt_btnCancel_Clicked()
{
    //    bool isChk_Hidden = chkRemeber->isHidden();
    //    bool is_btnSure_Hidden = btnSure->isHidden();
    //    if(!isChk_Hidden)
    //    {
    //        bool isChecked = chkRemeber->isChecked();
    //        if(!is_btnSure_Hidden)
    //            emit msgChecked(isChecked, false);
    //        else
    //            emit okMessageHidden(isChecked);
    //    }

    //直接返回拒绝
    this->reject();
}
