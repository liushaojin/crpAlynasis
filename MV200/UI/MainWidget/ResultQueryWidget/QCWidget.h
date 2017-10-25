#ifndef QCWIDGET_H
#define QCWIDGET_H

#include <QWidget>
#include <qlabel.h>
#include <qgroupbox.h>
#include <QRadioButton>
#include <qpushbutton.h>
#include <qgridlayout.h>
#include <QHBoxLayout>
#include <QDateEdit>
#include <QTabWidget>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView
#include <QMenu>
#include <QAction>
#include <QVector>

#include "SonWidget/QcWestgardWidget.h"

class QCWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCWidget(QWidget *parent = 0);
    void updateTableData();      //更新列表数据（质控液、项目列表）

private:
    void initForm();              //初始化
    void createRightMenu();       //创建一个右键菜单
    void initTableForm();         //初始化表格
    void initQueryDataForm();
    void initStyle();             //界面风格
    void initConnect();           //连接信号与槽
    void initData();              //初始化数据
    int QueryQcData(int id);      //查询质控液相应的测试数据
    void QueryWestgardData(int pid); //查询Westgard图对应的数据

    int Count;
    int CurrentQcLiquid_id;      //当前选中的质控液的id
    int CurrentProjece_id;       //当前选中的项目的id
    bool isAllNull_Flag;         //是否全选标志

    QVector<double> vecResult;   //查询到的质控浓度结果
    double qcSD, qcAverageV;     //质控标准差，质控平均值
    QVector<double> vecData;     //绘质控曲线数据

signals:
    void sig_updateGraph_Data(QVector<double>); //信号：更新曲线数据
    void sig_updateGraph_Data(QVector<double>, double, double); //信号：更新曲线数据
    void sig_updateGraph_Data(QVector<double>data, int lower, int upper, int sd); //信号：更新曲线数据&Y轴的上下限&sd

public slots:
    void slt_rdoRealtimeQC_clicked(bool);
    void slt_rdoInternalQC_clicked(bool);
    void slt_rdoDayQC_clicked(bool);

    void slt_tabQueryData_Changed(int index);      //槽：切换界面
    void slt_tbvQCLiquid_Clicked(QModelIndex);     //槽：选中质控液
    void slt_tbvProjectName_Clicked(QModelIndex);  //槽：选中常规项目

    void clicked_rightMenu(const QPoint &pos);     //槽：质控数据右键菜单（右键信号槽函数）
    void slt_formerAction_Triggered();             //槽：右键菜单-下一页
    void slt_nextAction_Triggered();               //槽：右键菜单-上一页
    void slt_curveAction_Triggered();              //槽：右键菜单-曲线
    void slt_allAction_Triggered();                //槽：右键菜单-全选
    void slt_deleteAction_Triggered();             //槽：右键菜单-删除
private:
    //查询条件
    QGroupBox *grbQueryTerms;
    QRadioButton *rdoRealtimeQC;   //实时质控
    QRadioButton *rdoDayQC;        //天间质控
    QRadioButton *rdoInternalQC;   //天内质控
    QLabel *labDate;               //日期
    QDateEdit *dateStart;          //查询开始日期
    QDateEdit *dateEnd;            //查询结束日期
    QGridLayout *lytQueryTerms;

    //项目名称
    QTableView *tbvProjectName;
    QStandardItemModel *modelProjectName;

    //质控液
    QTableView *tbvQCLiquid;
    QStandardItemModel *modelQCLiquid;

    //查询数据（包括：多规则质控图、累计和质控图、twin plo图、质控数据）
    QTabWidget *tabQueryData;
    QcWestgardWidget *qcWestgardWidget;  //多规则质控图
    QTableView *tbvQueryData;            //质控数据
    QStandardItemModel *modelQueryData;

    //质控数据-右键菜单
    QMenu *rightMenu;
    QAction *formerAction;  //上一页
    QAction *nextAction;    //下一页
    QAction *curveAction;   //曲线
    QAction *allAction;     //全选
    QAction *deleteAction;  //删除

    //总布局
    QGridLayout *layout;

};

#endif // QCWIDGET_H
