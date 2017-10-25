#ifndef HISTORYRESULTWIDGET_H
#define HISTORYRESULTWIDGET_H

#include <QWidget>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qpushbutton.h>
#include <qgridlayout.h>
#include <QHBoxLayout>
#include <QComboBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QTableView>          //1.
#include <QStandardItemModel>  //2.
#include <QHeaderView>         //3.除了QTableWidget还需要引用QHeaderView
#include "ParamDefine.h"
#include "Object/SQLite/sqlite.h"
//#include "Thread/PrinterThread.h"
#include "Object/Logic5P/MV_Logic5P.h"
#include "UI/Widget/MessageBox/messageBox.h"
#include "UI/MainWidget/CurveWidget/resultCurveWidget.h"

class HistoryResultWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryResultWidget(QWidget *parent = 0);

private:
    void initForm();                    //初始化
    void initQueryTermsForm();          //查询条件
    void initResultList();              //测试结果查询
    void initData();                    //初始化数据
    void initTableFormat();             //格式化表格（结果表）
    void initStyle();                   //界面风格
    void initConnect();                 //连接信号与槽
    int QueryData();                    //查询数据

    int CurrentResultT_Count;        //查询行数标志
    QString QueryTermsResultT;       //查询条件
    int isAllNull_Flag;              //全选/反选标志

signals:

public slots:
    void slt_btnQuery_Clicked();      //槽：查询
    void slt_btnAllNull_Clicked();    //槽：全选/反选
    void slt_btnCurve_Clicked();      //槽：反应曲线
    void slt_btnFormer_Clicked();     //槽：上一页
    void slt_btnNext_Clicked();       //槽：下一页
    void slt_btnPrint_Clicked();      //槽：打印
    void slt_tbvListSelectRow(const QModelIndex &index);       //槽：击列表选择中某行，列表对应行左边复选框对应变化

private:
    //查询条件
    QLabel *labStatus;            //状态
    QComboBox *cboStauts;

    QLabel *labProject;           //项目
    QComboBox *cboProject;
    QCheckBox *chk_isDateTime;    //是否按申请日期查询
    QDateEdit *dateDateTime;
    QCheckBox *chk_isTestDateTime;//是否按测试日期查询
    QDateEdit *dateTestDateTime;
    QLabel *labSampleType;        //样本类型
    QComboBox *cboSampleType;
    QCheckBox *chkSampleCode;     //样本编号（条码）
    QLineEdit *editSampleCode;
    QPushButton *btnQuery;        //查询
    QLabel *labTestCount;         //测试个数
    QLineEdit *editTestCount;
    QGroupBox *grbQueryTerms;
    QGridLayout *lytQueryTerms;

    //结果列表
    QWidget *wdgResultList;
    QGridLayout *lytResultList;
    QTableView *tbvList;            //结果列表
    QStandardItemModel *modelList;  //结果列表模型
    QWidget *wdgMenuButton;
    QPushButton *btnAllNull;        //全选/反选
    QPushButton *btnFormer;         //上一页
    QPushButton *btnNext;           //下一页
    QPushButton *btnPrint;          //打印
    QPushButton *btnCurve;          //反应曲线
    QGridLayout *lytMenuButton;

    //总布局
    QGridLayout *layout;
};

#endif // HISTORYRESULTWIDGET_H
