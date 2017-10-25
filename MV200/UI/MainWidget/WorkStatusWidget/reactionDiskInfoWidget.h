#ifndef REACTIONDISKINFOWIDGET_H
#define REACTIONDISKINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QGridLayout>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView

#include "ParamDefine.h"
#include "Thread/Scheduler.h"
#include "UI/Widget/ReactionDisk/ReactionDiskWidget.h"
#include "UI/MainWidget/CurveWidget/resultCurveWidget.h"

class ReactionDiskInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReactionDiskInfoWidget(QWidget *parent = 0);

private:
    void initForm();              //初始化
    void initTableFormat();       //格式化表格
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据

    //设置反应杯的状态
    void setCupStatus(int CupNo, QString objName);

signals:

public slots:
    void slt_setCupStatus(int CupNo, QString objName);  //槽：设置反应杯的状态
    void slt_CurrentItemText(QString text);             //槽：当前选中item的文本
    void slt_btnCurve_Clicked();                        //槽：查看反应曲线
    void slt_SelectTbv_doubleClicked(QModelIndex);//槽：表格双击事件

private:
    ReactionDiskWidget *reactionDisk;//反应盘状态
    QTableView *tbvList;             //列表
    QStandardItemModel *modelList;   //定标测试列表模型
    QPushButton *btnCurve;           //查看反应曲线

    //总布局
    QGridLayout *layout;
};

#endif // REACTIONDISKINFOWIDGET_H
