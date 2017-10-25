#ifndef MVMESSABEBOX_H
#define MVMESSABEBOX_H

#include <QDialog>
#include <QIcon>
#include <qlabel.h>
#include <QCheckBox>
#include <qpushbutton.h>
#include <QVBoxLayout>
#include <QPainter>
#include <QBitmap>
#include <QMouseEvent>

enum ButtonType
{
    Sure  = 0,
    Cancel = 1
};

class MVMessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit MVMessageBox(QWidget *parent = 0);
    void setInfo(QString titleInfo, QString msgInfo, QPixmap pixmap, bool isChkHidden, bool isBtnSureHiddent);
    void setButtonText(ButtonType type, QString text);//设置按钮文本

private:
    void initForm();                               //初始化窗口
    void initConnection();                         //初始化信号与槽
    void initStyleSheet();                         //初始化样式表
    void translateLanguage();                      //设置语言

    void paintEvent(QPaintEvent *);                //重绘事件
    void mousePressEvent( QMouseEvent * event );   //按下鼠标事件
    void mouseReleaseEvent( QMouseEvent *);        //释放鼠标事件
    void mouseMoveEvent(QMouseEvent *event);       //移动鼠标事件

signals:
    //void msgChecked(bool check, bool);   //信号：
    //void okMessageHidden(bool check);    //信号：
    void sig_SureClicked(bool check, bool sureClicked); //信号：

public slots:
    void slt_btnSure_Clicked();          //槽：确定
    void slt_btnCancel_Clicked();        //槽：取消

private:
    //变量
    QPoint move_point;                   //记录鼠标位置
    bool isMousePressed;                 //鼠标是否按下
    QString str_btnSure_Text;            //确定按钮文本
    QString str_btnCancel_Text;          //取消按钮文本
    //控件
    QPushButton *btnClose;               //关闭按钮
    QLabel *labTitle;                    //标题标签
    QLabel *labIcon;                     //提示图片标签
    QLabel *labAsk;                      //提示信息标签
    QCheckBox *chkRemeber;               //记住选择复选框
    QPushButton *btnCancel;              //取消按钮
    QPushButton *btnSure;                //确定按钮
};
extern MVMessageBox * gMessageBox;
#endif // MVMESSABEBOX_H
