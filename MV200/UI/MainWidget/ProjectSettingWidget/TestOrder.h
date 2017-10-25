#ifndef TESTORDER_H
#define TESTORDER_H

#include <QWidget>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView
#include <QPushButton>
#include <QGridLayout>
#include "UI/Widget/MessageBox/messageBox.h"

class TestOrder : public QWidget
{
    Q_OBJECT
public:
    explicit TestOrder(QWidget *parent = 0);
    void QueryProjectInfo();      //查询项目数据

private:
    void initForm();              //初始化
    void initProjectList();       //常规项目列表
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据

signals:

public slots:
    void slt_btnTop_Clicked();       //槽：置顶
    void slt_btnUp_Clicked();        //槽：上移
    void slt_btnDown_Clicked();      //槽：下移
    void slt_btnBottom_Clicked();    //槽：底部
    void slt_btnCancel_Clicked();    //槽：取消
    void slt_btnSave_Clicked();      //槽：保存

private:
    QTableView *tbvProject;
    QStandardItemModel *modelPorject;

    QPushButton *btnTop;       //置顶
    QPushButton *btnUp;        //上移
    QPushButton *btnDown;      //下移
    QPushButton *btnBottom;    //底部
    QPushButton *btnCancel;    //取消
    QPushButton *btnSave;      //保存

    QGridLayout *layout;
};

#endif // TESTORDER_H
