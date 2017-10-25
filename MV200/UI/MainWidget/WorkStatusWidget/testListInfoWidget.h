#ifndef TESTLISTINFOWIDGET_H
#define TESTLISTINFOWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView

#include <QPushButton>
#include <QTableWidget>
#include <qgridlayout.h>


class TestListInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TestListInfoWidget(QWidget *parent = 0);

private:
    void initForm();              //初始化
    void initWaitTestFormat();
    void initTestingFormat();
    void initQueueingFormat();
    void initInvalidFormat();
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据


signals:

public slots:

private:
    QTabWidget *tabWidget;
    QTableWidget *tbwWaitTest;     //待测试
    QTableWidget *tbwTesting;      //测试中
    QTableWidget *tbwQueueing;     //排队中

     //已作废
    QWidget *wdgInvalid;
    QTableWidget *tbwInvalid;
    QPushButton *btnRecovery;      //恢复
    QGridLayout *lytInvalid;


    //
    QGridLayout *layout;

};

#endif // TESTLISTINFOWIDGET_H
