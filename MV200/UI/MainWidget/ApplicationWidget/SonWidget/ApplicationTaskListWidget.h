#ifndef APPLICATIONTASKLISTWIDGET_H
#define APPLICATIONTASKLISTWIDGET_H

#include <QWidget>
#include <QDialog>
#include <qpushbutton.h>
#include <QGridLayout>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView

#include "ParamDefine.h"
#include "Thread/Scheduler.h"
#include "Object/SQLite/sqlite.h"
#include "UI/MainWidget/CurveWidget/resultCurveWidget.h"

class ApplicationTaskListWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ApplicationTaskListWidget(QDialog *parent = 0);
    int initData();              //初始化数据

private:
    void initForm();              //初始化
    void initTableFormat();       //格式化表格
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽

    int Count;
    bool isAllNull_Flag; //全选/反选标志

signals:
     void sig_UpdateApplicatedCount(); //信号：实时更新数据库中状态为“等待”的测试的个数

public slots:
    void slt_btnAllNone_Clicked();        //全选/反选
    void slt_btnFormer_Clicked();         //上一页
    void slt_btnNext_Clicked();           //下一页
    void slt_btnReTest_Clicked();         //重测
    void slt_btnDelete_Clicked();         //删除
    void slt_btnCurve_Clicked();          //查看反应曲线
    void slt_btnCancel_Clicked();         //返回
    void slt_UpdateApplicationTaskList(); //更新申请列表

    void slt_tbvList_test(QModelIndex);

private:
    ResultCurveWidget *resultCurveWidget;

    QTableView *tbvList;            //列表
    QStandardItemModel *modelList;  //列表模型

    QPushButton *btnAllNone;        //全选/反选
    QPushButton *btnFormer;         //上一页
    QPushButton *btnNext;           //下一页
    QPushButton *btnReTest;         //重测
    QPushButton *btnCancel;         //返回
    QPushButton *btnDelete;         //删除
    QPushButton *btnCurve;          //查看反应曲线

    //总布局
    QGridLayout *layout;


};

#endif // APPLICATIONTASKLISTWIDGET_H
